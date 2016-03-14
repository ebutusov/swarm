#pragma once
#include <vector>
#include <map>
#include <iterator>
#include <functional>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SubManager.h"
#include "PubManager.h"

class MQTTNode
{
protected:
  WiFiClient m_wclient;
  PubSubClient m_pubsub;
  
public:
  MQTTNode(const char *wifi_ssid, const char *wifi_pass, const char *mqtt_server, int mqtt_port): 
  m_wclient(), 
  m_pubsub(m_wclient, mqtt_server, mqtt_port),
  m_submgr(m_pubsub),
  m_pubmgr(m_pubsub),
  m_wifi_ssid(wifi_ssid),
  m_wifi_pass(wifi_pass)
  {};

  void authMQTT(const char *client_name, const char *user, const char *pass);
  void cyclicPub(const String &topic, int interval, PUBCB cb);
  void onMsg(const String &topic, ONRECEIVED cb);
  
private:
  SubManager m_submgr;
  PubManager m_pubmgr;
  String m_wifi_ssid, m_wifi_pass;
  String m_mqtt_client, m_mqtt_user, m_mqtt_pass;
  
public:
  void loop();
};

