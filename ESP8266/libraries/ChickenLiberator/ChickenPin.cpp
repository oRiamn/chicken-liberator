#include "./ChickenPin.h"
#include <Arduino.h>

ChickenPin::ChickenPin(int pin)
{
    m_pin = pin;
}


ChickenPin::ChickenPin(int pin, int state)
{
    m_pin = pin;
    m_state = state;
}

ChickenPin::~ChickenPin()
{
}

bool ChickenPin::setState(int state)
{
    if (state == 0 || state == 1)
    {
        m_state = state;
        digitalWrite(m_pin, m_state);
        return true;
    }
    return false;
}

int ChickenPin::getState()
{
    return m_state;
}


int ChickenPin::getPin()
{
    return m_pin;
}

StaticJsonDocument<128> ChickenPin::toJson()
{
    StaticJsonDocument<128> doc;
    doc["pin"] = m_pin;
    doc["state"] = m_state;
    doc.garbageCollect();
    return doc;
}
