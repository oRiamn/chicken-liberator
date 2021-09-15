#include "./ChickenLiberator.h"
#include "./ChickenPin.h"
#include <Arduino.h>

ChickenLiberator::ChickenLiberator()
{
  const int outputPins[OUTPUT_PINS_BUFFER_SIZE] = OUTPUT_PINS;
  for (byte i = 0; i < OUTPUT_PINS_BUFFER_SIZE; i = i + 1)
  {
    int iPin = outputPins[i];
    ChickenPin* oPin = new ChickenPin(iPin);
    m_outputPinList[iPin] = oPin;
  }
}

ChickenLiberator::~ChickenLiberator()
{
}

// dangerous method : auto pos = m_outputPinList.find(iPin) can throw exception
ChickenPin* ChickenLiberator::getPin(int iPin)
{
  auto pos = m_outputPinList.find(iPin);
  ChickenPin* oPin = pos->second;
  return oPin;
}

bool ChickenLiberator::changeStateOutputPin(int iPin, int iState)
{
  auto pos = m_outputPinList.find(iPin);
  if (pos == m_outputPinList.end())
  {
    return false;
  }
  return pos->second->setState(iState);
}

void ChickenLiberator::init()
{
  for (auto &itr : m_outputPinList)
  {
    ChickenPin* oPin = itr.second;
    pinMode(oPin->getPin(), OUTPUT);
    oPin->setState(LOW);
  }
}

StaticJsonDocument<256> ChickenLiberator::getPins()
{
  StaticJsonDocument<256> doc;
  JsonArray outputPinDocs = doc.to<JsonArray>();

  for (auto &itr : m_outputPinList)
  {
    StaticJsonDocument pinDoc = itr.second->toJson();
    outputPinDocs.add(pinDoc.as<JsonObject>());
  }

  doc.garbageCollect();

  return doc;
}