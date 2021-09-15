#include "./ChickenLogger.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

void ChickenLogger::setup(const char *message)
{
    Serial.print("SETUP ");
    Serial.println(message);
}

void ChickenLogger::http(AsyncWebServerRequest *request)
{
    Serial.printf("HTTP %s %s\r\n", request->methodToString(), request->url().c_str());
}

void ChickenLogger::ws(AsyncWebSocketClient *client, WSEvent evnt)
{
    Serial.printf("WS #%u %s %s\r\n", client->id(), client->remoteIP().toString().c_str(), enum_str[evnt]);
}


void ChickenLogger::json(const char* header, const StaticJsonDocument<256> &doc)
{
    Serial.print(header);
    Serial.print(" ");
    serializeJson(doc, Serial);
    Serial.println();
}