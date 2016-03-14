#pragma once
#include <vector>
#include "DallasTemperature.h"

class PubSubClient;
class String;

String addrToString(DeviceAddress deviceAddress);
void printAddress(DeviceAddress deviceAddress);
void gpioPublish(int pin, PubSubClient &client, const String& topic);

