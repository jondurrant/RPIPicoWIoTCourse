# IoT Device On
# Jon Durrant - 3-Oct-2022
#
# Send message {}
# To topic TNG/<<device>>/STATE/GET
# Takes <<device>> as first parameter on command line
# Experts following env variables to be set
# MQTT_CLIENT
# MQTT_USER
# MQTT_PASSWD
# MQTT_HOST
# MQTT_PORT
import paho.mqtt.client as mqtt
import json
import time
import sys
import os

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

all_topics = "TNG/" + targetId + "/#"
get_topic = "TNG/" + targetId + "/STATE/GET"
set_topic = "TNG/" + targetId + "/STATE/SET"

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

client.subscribe( all_topics )


j = {'GET': 1}
p = json.dumps(j)
print("Publishing ping %s"%p)
infot = client.publish(get_topic, p,retain=False, qos=1)
infot.wait_for_publish()

time.sleep(30)