#include "./ChickenLiberator.h"
#include <Arduino.h>

StaticJsonDocument<256> getPinJson(int iPin, int iState)
{
  StaticJsonDocument<256> doc;
  doc["pin"] = iPin;
  doc["state"] = iState;
  return doc;
}

ChickenLiberator::ChickenLiberator()
{
}

ChickenLiberator::~ChickenLiberator()
{
}

bool ChickenLiberator::changeStateOutputPin(int iPin, int iState)
{
  for (byte i = 0; i < OUTPUT_PINS_BUFFER_SIZE; i = i + 1)
  {
    if (m_outputPins[i] == iPin)
    {
      digitalWrite(iPin, iState);
      m_outputPinValues[i] = iState;
      return true;
    }
  }
  return false;
}

void ChickenLiberator::init()
{
  for (byte i = 0; i < OUTPUT_PINS_BUFFER_SIZE; i = i + 1)
  {
    pinMode(m_outputPins[i], OUTPUT);
    changeStateOutputPin(m_outputPins[i], LOW);
  }
}

StaticJsonDocument<1024> ChickenLiberator::getPins()
{
  StaticJsonDocument<1024> doc;
  JsonArray outputPinDocs = doc.to<JsonArray>();

  for (byte i = 0; i < OUTPUT_PINS_BUFFER_SIZE; i = i + 1)
  {
    int iPin = m_outputPins[i];
    int iState = m_outputPinValues[i];

    StaticJsonDocument pinDoc = getPinJson(iPin, iState);
    outputPinDocs.add(pinDoc.as<JsonObject>());
  }

  doc.garbageCollect();

  return doc;
}