#include "client_conf.h"
#include "helpers.h"
#include "MQTTNode.h"
#include "OWDevices.h"

#define ONE_WIRE_BUS 4
#define RELAY_GPIO 5
#define LED_GPIO 2
#define TEMPERATURE_PRECISION 9
#define NODE_ROOT "home/esp8266/"

MQTTNode node(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT);
OWDevices owd(ONE_WIRE_BUS, TEMPERATURE_PRECISION);
 
void setup() 
{ 
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  node.authMQTT(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS);
  owd.start();
  pinMode(RELAY_GPIO, OUTPUT);
  pinMode(LED_GPIO, OUTPUT);
  
  // subscriptions
  node.onMsg(String(NODE_ROOT)+"relay", 
    [](const String &t, const String &m) 
    { 
      int val = m.toInt() ? 1 : 0;
      digitalWrite(RELAY_GPIO, val);  
    });

  node.onMsg(String(NODE_ROOT)+"led",
    [](const String &t, const String &m)
    {
      int val = m.toInt() ? 1 : 0;
      digitalWrite(LED_GPIO, val);
    });

  // publications
  node.cyclicPub(String(NODE_ROOT)+"sensor/", 10000, 
  [](PubSubClient &client, const String &topic) 
  {
    owd.publishAll(client, topic);
  });
  
  node.cyclicPub(String(NODE_ROOT)+"relay/status", 1000, 
    [](PubSubClient &client, const String &topic) 
    {
      gpioPublish(RELAY_GPIO, client, topic);
    }); 
}

void loop() 
{ 
  node.loop();
  delay(100);
}

