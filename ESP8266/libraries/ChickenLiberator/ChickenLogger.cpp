#include "./ChickenLogger.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

void ChickenLogger::printf(const char *header, const char *format, ...)
{
    va_list arg;
    char output_sprintf[100];

    Serial.print(header);
    Serial.print(" ");

    va_start(arg, format);
    vsprintf(output_sprintf, format, arg);
    va_end(arg);

    Serial.println(output_sprintf);
}

void ChickenLogger::setup(const char *message)
{
    Serial.print("SETUP ");
    Serial.println(message);
}

void ChickenLogger::http(AsyncWebServerRequest *request)
{
    ChickenLogger::printf("HTTP", "%s %s", request->methodToString(), request->url().c_str());
}

void ChickenLogger::ws(AsyncWebSocketClient *client, WSEvent evnt)
{
    ChickenLogger::printf("WS", "#%u %s %s", client->id(), client->remoteIP().toString().c_str(), enum_str[evnt]);
}

void ChickenLogger::json(const char *header, const StaticJsonDocument<256> &doc)
{
    Serial.print(header);
    Serial.print(" ");
    serializeJson(doc, Serial);
    Serial.println();
}