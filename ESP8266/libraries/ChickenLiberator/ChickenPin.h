#ifndef CHICKENPINSOURCE_H_
#define CHICKENPINSOURCE_H_

#include <ArduinoJson.h>

class ChickenPin
{
private:
    int m_pin;
    int m_state;

public:
    ChickenPin(int pin);
    ChickenPin(int pin, int state);
    ~ChickenPin();
    bool setState(int state);
    int getState();
    int getPin();
    ArduinoJson::StaticJsonDocument<256> toJson();
};

#endif /* CHICKENPINSOURCE_H_ */