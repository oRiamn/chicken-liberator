#ifndef CHICKENLOGGERSOURCE_H_
#define CHICKENLOGGERSOURCE_H_

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

static const char *enum_str[] = {"disconnected", "connected"};

enum WSEvent
{
    DISCONNECTED,
    CONNECTED
};

class ChickenLogger
{
private:
public:
    static void printf (const char *header, const char *format, ...);
    static void setup(const char* message);
    static void http(AsyncWebServerRequest *request);
    static void ws(AsyncWebSocketClient *client, WSEvent evnt);
    static void json(const char* header,  const StaticJsonDocument<256>& doc);
};

#endif /* CHICKENLOGGERSOURCE_H_ */