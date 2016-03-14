#include <ESP8266WiFi.h>
#include "PubSubClient.h"

typedef uint8_t DeviceAddress[8]; // xxx pragma once!

String addrToString(DeviceAddress deviceAddress)
{
  String ret;
  for (uint8_t i = 0; i < 8; i++)
    ret += String(deviceAddress[i], HEX);
  return ret;
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void gpioPublish(int pin, PubSubClient &client, const String& topic)
{
  static int currentState = 0;
  int p = digitalRead(pin) == HIGH ? 1 : 0;
  if (p != currentState)
  {
    client.publish(topic,String(p));
    currentState = p;
  }
}
