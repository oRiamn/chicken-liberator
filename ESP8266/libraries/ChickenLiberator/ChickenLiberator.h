#ifndef CHICKENLIBERATORSOURCE_H_
#define CHICKENLIBERATORSOURCE_H_

#include <LibConstants.h>
#include <ArduinoJson.h>
#include "./ChickenPin.h"
#include <map>

class ChickenLiberator
{
public:
    ChickenLiberator();
    ~ChickenLiberator();
    void init();
    bool changeStateOutputPin(int pin, int state);
    ArduinoJson::StaticJsonDocument<1024> getPins();
    ChickenPin* getPin(int iPin);

private:
    std::map<int, ChickenPin*> m_outputPinList;
};

#endif /* CHICKENLIBERATORSOURCE_H_ */