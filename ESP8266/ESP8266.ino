/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LibConstants.h>
#include <string> 
#include <Arduino.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char *ssid = LibConstants::ssid;
const char *password = LibConstants::password;

#define outputPinsBufferSize 5
int outputPins[] = {
  2,
  4,
  5,
  0,
  15
};
int outputPinValues[outputPinsBufferSize] = {};

bool ledState = 0;
const int ledPin = 2;

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

StaticJsonDocument<256> getPinJson(int iPin, int iState) {
  StaticJsonDocument<256> doc;
  doc["pin"] = iPin;
  doc["state"] = iState;
  return doc;
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void notifyClients(int iPin, int iState) {
  StaticJsonDocument pinDoc = getPinJson(iPin, iState);
  ws.textAll(pinDoc.as<String>());
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients(ledPin, ledState);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\r\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\r\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

bool changeStateOutputPin(int pin, int state) {
  for (byte i = 0; i < outputPinsBufferSize; i = i + 1)
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

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  for (byte i = 0; i < outputPinsBufferSize; i = i + 1)
  {
    pinMode(outputPins[i], OUTPUT);
    changeStateOutputPin(outputPins[i], LOW);
  }

  Serial.printf("\r\r\r\nConnecting to %s\r\n", ssid);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print ESP Local IP Address

  Serial.printf("\r\nWifi connected : http://%s\r\n", WiFi.localIP().toString().c_str());

  initWebSocket();

  server.on("^\/api\/gpio\/out/([0-9]{1,2})\/([0-1])\/?$", HTTP_GET, [](AsyncWebServerRequest *request) {

      int iPin = std::stoi(request->pathArg(0).c_str());
      int iState = std::stoi(request->pathArg(1).c_str());

      Serial.println(iPin, iState);

      if (changeStateOutputPin(iPin, iState)) {
        notifyClients(iPin, iState);
        StaticJsonDocument pinDoc = getPinJson(iPin, iState);
        request->send(200, "application/json", pinDoc.as<String>());
      } else {
        notFound(request);
      }
  });

 server.on("/api/gpio/out", HTTP_GET, [](AsyncWebServerRequest *request) {
      StaticJsonDocument<1024> doc;
      JsonArray array = doc.to<JsonArray>();
      for (byte i = 0; i < outputPinsBufferSize; i = i + 1){
        int iPin = outputPins[i];
        int iState = outputPinValues[i];
        StaticJsonDocument pinDoc = getPinJson(iPin, iState);
        array.add(pinDoc.as<JsonObject>());
      }
      request->send(200, "application/json", doc.as<String>());

  });


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.onNotFound(notFound);

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}
