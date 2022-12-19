# IoT Device ping
# Jon Durrant - 3-Oct-2022
#
# Send message {'id': int}
# To topic TNG/<<device>>/TPC/PING
# Takes <<device>> as first parameter on command line
# Experts following env variables to be set
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

#Check we have a target as a parameter on command line
if (len(sys.argv) != 2):
    print("Require target ID as parater")
    sys.exit()
targetId = sys.argv[1]

# Grab environment variables
clientId=os.environ.get("MQTT_CLIENT")
user=os.environ.get("MQTT_USER")
passwd=os.environ.get("MQTT_PASSWD")
host= os.environ.get("MQTT_HOST")
port=int(os.environ.get("MQTT_PORT"))
print("MQTT %s:%d - %s\n"%(host,port, user))

ping_topic = "TNG/" + targetId + "/TPC/PING"
pong_topic = "TNG/" + targetId + "/TPC/PONG"
lc_topic = "TNG/" + targetId + "/LC/#"

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

client.subscribe( lc_topic )
client.subscribe( pong_topic )
client.subscribe( ping_topic )
    

pingId = random.randint(0,999999);
j = {'id': pingId}
p = json.dumps(j)
print("Publishing ping %s to %s"%(p, ping_topic))
infot = client.publish(ping_topic, p,retain=False, qos=1)
infot.wait_for_publish()
pingId = pingId + 1

time.sleep(30)