#include <OneWire.h>
#include <DallasTemperature.h>

struct OWD
{
  OWD(const DeviceAddress &address)
  {
    memcpy(addr, address, sizeof(DeviceAddress));
    strAddress = addrToString(addr); 
  }
  DeviceAddress addr;
  String strAddress;
};

class OWDevices
{
public:
  OWDevices(int pin, int precision):
    m_wire(pin),
    m_sensors(&m_wire),
    m_precision(precision)
    {
      m_sensors.begin();
    };
  void start()
  {
    int numberOfDevices = m_sensors.getDeviceCount();
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(numberOfDevices, DEC);
    Serial.println(" devices.");
  
    for (int i=0;i<numberOfDevices;++i)
    {
      DeviceAddress addr;
      if(m_sensors.getAddress(addr, i))
      {
        m_devs.emplace_back(addr);
        Serial.print("Found device with address: ");
        Serial.print(m_devs.back().strAddress);
        Serial.println();
        m_sensors.setResolution(addr, m_precision);
      }
    }
  }
  void publishAll(PubSubClient &client, const String &topic)
  {
    m_sensors.requestTemperatures();
    for(const OWD &dev : m_devs)
    {
      float t = m_sensors.getTempC(dev.addr);
      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.println("C");
      client.publish(topic+dev.strAddress,String(t));
    }
  }
private:
  OneWire m_wire;
  DallasTemperature m_sensors;
  std::vector<OWD> m_devs;
  int m_precision;
};
