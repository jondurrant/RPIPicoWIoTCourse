# IoT Device Ping ALL
# Jon Durrant - 3-Oct-2022
#
# Send message {'on': True}
# To topic GRP/ALL/TPC/PING
# Experts following env variables to be set
# MQTT_CLIENT
# MQTT_USER
# MQTT_PASSWD
# MQTT_HOST
# MQTT_PORT

import paho.mqtt.client as mqtt
import json
import time
import os
import sys
import random

# Grab environment variables
clientId=os.environ.get("MQTT_CLIENT")
user=os.environ.get("MQTT_USER")
passwd=os.environ.get("MQTT_PASSWD")
host= os.environ.get("MQTT_HOST")
port=int(os.environ.get("MQTT_PORT"))
print("MQTT %s:%d - %s\n"%(host,port, user))

ping_topic = "GRP/ALL/TPC/PING"
pong_topic = "TNG/+/TPC/PONG"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Rcv topic=" +msg.topic+" msg="+str(msg.payload))

client = mqtt.Client(client_id=clientId)
client.username_pw_set(username=user, password=passwd)
client.on_connect = on_connect
client.on_message = on_message

client.connect(host, port, 60)

client.loop_start()

client.subscribe( ping_topic )
client.subscribe( pong_topic )  

pingId = random.randint(0,999999);
j = {'id': pingId}
p = json.dumps(j)
print("Publishing ping %s"%p)
infot = client.publish(ping_topic, p,retain=False, qos=1)
infot.wait_for_publish()
pingId = pingId + 1

time.sleep(30)