#ifndef CHICKENLIBERATORSOURCE_H_
#define CHICKENLIBERATORSOURCE_H_

#include <LibConstants.h>
#include <ArduinoJson.h>

class ChickenLiberator{
    public:
    ChickenLiberator();
    ~ChickenLiberator();
    void init();
    bool changeStateOutputPin(int pin, int state);
    ArduinoJson::StaticJsonDocument<1024> getPins();
    

    private:

    const int m_outputPins[OUTPUT_PINS_BUFFER_SIZE] = OUTPUT_PINS;
    int m_outputPinValues[OUTPUT_PINS_BUFFER_SIZE] = {};
};

#endif /* CHICKENLIBERATORSOURCE_H_ */