#include "MQTTNode.h"

void MQTTNode::authMQTT(const char *client_name, const char *user, const char *pass)
{
  m_mqtt_client = client_name;
  m_mqtt_user = user;
  m_mqtt_pass = pass;
}

void MQTTNode::cyclicPub(const String &topic, int interval, PUBCB cb)
{
  Publication pub(topic, interval, cb);
  m_pubmgr.add(pub);
}

void MQTTNode::onMsg(const String &topic, ONRECEIVED cb)
{
  Subscription sub(topic, cb);
  m_submgr.add(sub);
}

void MQTTNode::loop()
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Connecting to ");
    Serial.print(m_wifi_ssid);
    Serial.println("...");
    WiFi.begin(m_wifi_ssid.c_str(), m_wifi_pass.c_str());

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) 
  {
    if (!m_pubsub.connected()) 
    {
      Serial.println("Connecting to MQTT server");
      if (m_pubsub.connect(MQTT::Connect(m_mqtt_client)
                         .set_auth(m_mqtt_user, m_mqtt_pass)))
      {
        Serial.println("Connected to MQTT server");
        m_submgr.doSubscriptions();
      } 
      else 
        Serial.println("Could not connect to MQTT server");   
    }

    if (m_pubsub.connected())
    {
      m_pubsub.loop();
      m_pubmgr.doPublications();
    }
  }
}
