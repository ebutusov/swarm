#pragma once
//
// Class for MQTT publication management
// E.Butusov <ebutusov@gmail.com>

#include <PubSubClient.h>

using PUBCB = std::function<void(PubSubClient&, String topic)>;

class Publication
{
public:
  Publication(String topic, int schedule, PUBCB cb): 
  m_topic(topic), m_schedule(schedule), m_cb(cb), m_last_published(0)  
  {};
  
  void publish(PubSubClient& client)
  {
    if (millis() - m_last_published >= m_schedule)
    {
      m_cb(client, m_topic);
      m_last_published = millis();
    } 
  }
private:
  String m_topic;
  int m_schedule;
  PUBCB m_cb;
  unsigned long m_last_published;
};

class PubManager
{
public:
  PubManager(PubSubClient& c): m_client(c) {};
  template <typename T>
  void add(T&& pub)
  {
    m_pubs.push_back(pub);
  }

  void doPublications()
  {
    for (Publication& p : m_pubs)
      p.publish(m_client);
  }
private:
  std::vector<Publication> m_pubs;
  PubSubClient &m_client;
};
