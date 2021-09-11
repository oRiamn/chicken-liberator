#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LibConstants.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

const char *ssid = SSID;
const char *password = PASSWORD;

const int outputPins[OUTPUT_PINS_BUFFER_SIZE] = OUTPUT_PINS;
int outputPinValues[OUTPUT_PINS_BUFFER_SIZE] = {};

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/api/gpio");

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

StaticJsonDocument<256> getPinJson(int iPin, int iState)
{
  StaticJsonDocument<256> doc;
  doc["pin"] = iPin;
  doc["state"] = iState;
  return doc;
}

void logRequest(AsyncWebServerRequest *request)
{
  Serial.printf("HTTP %s %s\r\n", request->methodToString(), request->url().c_str());
}

void notFound(AsyncWebServerRequest *request)
{
  logRequest(request);
  request->send(404, "text/plain", "Not found");
}

void notifyClients(int iPin, int iState)
{
  StaticJsonDocument pinDoc = getPinJson(iPin, iState);
  ws.textAll(pinDoc.as<String>());
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WS #%u %s connected\r\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WS #%u %s disconnected\r\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DATA:
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
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

bool changeStateOutputPin(int pin, int state)
{
  for (byte i = 0; i < OUTPUT_PINS_BUFFER_SIZE; i = i + 1)
  {
    if (outputPins[i] == pin)
    {
      digitalWrite(pin, state);
      outputPinValues[i] = state;
      return true;
    }
  }
  return false;
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  for (byte i = 0; i < OUTPUT_PINS_BUFFER_SIZE; i = i + 1)
  {
    pinMode(outputPins[i], OUTPUT);
    changeStateOutputPin(outputPins[i], LOW);
  }

  Serial.printf("\r\r\r\nSETUP Connecting to %s\r\n", ssid);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  // Print ESP Local IP Address

  Serial.printf("\r\nSETUP Wifi connected : http://%s\r\n", WiFi.localIP().toString().c_str());

  initWebSocket();

  server.on("^\/api\/gpio\/out/([0-9]{1,2})\/([0-1])\/?$", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              logRequest(request);

              int iPin = std::stoi(request->pathArg(0).c_str());
              int iState = std::stoi(request->pathArg(1).c_str());

              if (changeStateOutputPin(iPin, iState))
              {
                notifyClients(iPin, iState);
                AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
                StaticJsonDocument pinDoc = getPinJson(iPin, iState);
                serializeJson(pinDoc, *response);
                request->send(response);

                Serial.print("PIN SET ");
                serializeJson(pinDoc, Serial);
                Serial.println();
              }
              else
              {
                notFound(request);
              }
            });

  server.on("/api/gpio/out", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              logRequest(request);

              AsyncResponseStream *response = request->beginResponseStream("application/json; charset=utf-8");
              DynamicJsonDocument jsonBuffer(1024);
              JsonArray root = jsonBuffer.to<JsonArray>();
              for (byte i = 0; i < OUTPUT_PINS_BUFFER_SIZE; i = i + 1)
              {
                int iPin = outputPins[i];
                int iState = outputPinValues[i];
                StaticJsonDocument pinDoc = getPinJson(iPin, iState);
                root.add(pinDoc.as<JsonObject>());
              }
              serializeJson(root, *response);
              request->send(response);
            });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              logRequest(request);
              request->send_P(200, "text/html; charset=utf-8", index_html, processor);
            });

  server.onNotFound(notFound);

  // Start server
  server.begin();
}

void loop()
{
  ws.cleanupClients();
}
