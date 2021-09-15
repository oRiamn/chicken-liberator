#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LibConstants.h>
#include <HTTPConstants.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <ChickenLiberator.h>
#include <ChickenLogger.h>

const char *ssid = SSID;
const char *password = PASSWORD;
int prev_freeheap = 0;
int lps_request_time = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/api/gpio");
ChickenLiberator chicken;

const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width,initial-scale=1" />
    <meta http-equiv="x-ua-compatible" content="ie=edge" />
    <script src="https://cdn.jsdelivr.net/npm/vue@2/dist/vue.js"></script>
    <title>Chicken Liberator</title>
    <link rel="icon" href="https://oriamn.github.io/chicken-liberator/favicon.png">
    <script defer="defer" src="https://oriamn.github.io/chicken-liberator/js/runtime.bundle.js"></script>
    <script defer="defer" src="https://oriamn.github.io/chicken-liberator/js/main.bundle.js"></script>
    <link href="https://oriamn.github.io/chicken-liberator/styles/main.css" rel="stylesheet">
</head>
<body>
    <div id="root"></div>
</body>
</html>
)rawliteral";

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    ChickenLogger::ws(client, CONNECTED);
    break;
  case WS_EVT_DISCONNECT:
    ChickenLogger::ws(client, DISCONNECTED);
    break;
  case WS_EVT_DATA:
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void sendJsonWebserverRequest(AsyncWebServerRequest *request, const StaticJsonDocument<512> &doc)
{
  AsyncResponseStream *response = request->beginResponseStream(JSON_MIME_TYPE);
  serializeJson(doc, *response);
  request->send(response);
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String &var)
{
  Serial.println(var);
  return String();
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  chicken.init();

  Serial.printf("\r\r\r\nSETUP Connecting to %s\r\n", ssid);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  // Print ESP Local IP Address

  IPAddress ip = WiFi.localIP();
  Serial.printf("\r\nSETUP Wifi connected : http://%d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);

  initWebSocket();

  server.on("^\/api\/gpio\/out/([0-9]{1,2})\/([0-1])\/?$", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              ChickenLogger::http(request);

              int iPin = std::stoi(request->pathArg(0).c_str());
              int iState = std::stoi(request->pathArg(1).c_str());

              if (chicken.changeStateOutputPin(iPin, iState))
              {
                StaticJsonDocument pinDoc = chicken.getPin(iPin)->toJson();

                sendJsonWebserverRequest(request, pinDoc);
                ws.textAll(pinDoc.as<String>());

                ChickenLogger::json("PIN SET", pinDoc);
              }
              else
              {
                request->send(404, JSON_MIME_TYPE, "{}");
              }
            });

  server.on("/api/gpio/out", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              ChickenLogger::http(request);

              StaticJsonDocument pinListDoc = chicken.getPins();

              sendJsonWebserverRequest(request, pinListDoc);

              ChickenLogger::json("PIN GET", pinListDoc);
            });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              ChickenLogger::http(request);
              request->send_P(200, HTML_MIME_TYPE, index_html, processor);
            });

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
                      ChickenLogger::http(request);
                      request->send(404, TEXT_MIME_TYPE, "Not found");
                    });

  // Start server
  server.begin();
}

void loop()
{
  ws.cleanupClients();

  if (millis() - lps_request_time > FREEHEAPLOGTIME)
  {

    int current_freeheap = ESP.getFreeHeap();
    if (prev_freeheap != current_freeheap)
    {
      prev_freeheap = current_freeheap;
      Serial.printf("LOG FREEHEAP %d\r\n", current_freeheap);
    }

    lps_request_time = millis();
  }
}
