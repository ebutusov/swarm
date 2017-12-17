#pragma once
//
// Class for MQTT subscription management
// E.Butusov <ebutusov@gmail.com>
#include <PubSubClient.h>

using ONRECEIVED = std::function<void(const String &topic, const String &payload)>;

class Subscription
{
public:
  Subscription(String topic, ONRECEIVED cb): 
    m_topic(topic), 
    m_onreceived(cb) {};
  
  void subscribe(PubSubClient &client) const
  {
    //client.set_callback(m_callback);
    client.subscribe(m_topic.c_str());
  }
  String getTopic() { return m_topic; }

  void do_callback(const String &topic, const String &msg)
  {
    m_onreceived(topic, msg);
  }
  
private:
  String m_topic;
  ONRECEIVED m_onreceived;
};

class SubManager
{
public:
  SubManager(PubSubClient& c): m_client(c) 
  {
    m_client.setCallback(std::bind(&SubManager::callback, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
  };
  template <typename T>
  void add(T&& sub)
  {
    m_subs.insert(std::pair<String, Subscription>(sub.getTopic(), sub));
  }
  void doSubscriptions() const
  {
    for (auto &kv : m_subs)
      kv.second.subscribe(m_client);
  }
private:
  void callback(char *topic, byte *payload, unsigned int length)
  {
    char buff[1024];
    int l = length>1023 ? 1023 : length;
    memcpy(buff, payload, l);
    buff[l] = '\0';
    auto it = m_subs.find(topic);
    if (it != m_subs.end())
      it->second.do_callback(topic, String((char*)buff));
  }
  PubSubClient& m_client;
  
  // XXX for some reason ESP8266 doesn't link against libstdc++ (and ld fails to find some inlined functions)
  // http://stackoverflow.com/questions/33450946/esp8266-for-arduino-ide-xtensa-lx106-elf-gcc-and-stdmap-linking-error
  // edit platforms.txt in $ARDUINO_IDE/hardware/esp8266com/esp8266, and add -lstdc++ to the following line:
  // compiler.c.elf.libs=-lm -lgcc -lhal -lphy -lnet80211 -llwip ...
  std::map<String, Subscription> m_subs;
};
