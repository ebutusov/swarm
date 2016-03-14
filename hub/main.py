#!/usr/bin/python3

import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("home/#")

def doTemps(client, topic, payload):
    if topic.startswith("home/esp8266/sensor/"):
        print("temperature received: " + payload)
        if topic.endswith("28efd1a6300c4"):
            temp = float(payload)
            if (temp > 26):
                print("turn relay on")
                client.publish("home/esp8266/relay", 1)
                client.publish("home/esp8266/led", 1)
            if (temp < 25):
                print("turn relay off")
                client.publish("home/esp8266/relay", 0)
                client.publish("home/esp8266/led", 0)


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode("utf-8")
    print(topic+" " + payload)
    doTemps(client, topic, payload)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set("XXX", "XXX")
client.connect("localhost", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
