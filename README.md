# RPIPicoWIoTCourse

Course Repository for Udemy Course: [IoT with Raspberry PI Pico-W (C++)](https://www.udemy.com/course/draft/4911302/?referralCode=331C13FDD6ABC00BFB81)

10 Examples C++ projects using the Pico-W. 8 of these demonstrate the capabilities of MQTT.

## Dependencies

+ [coreMQTT](https://github.com/FreeRTOS/coreMQTT) - v1.2.0 is used due to some instability I am still working through with v2 code
+ [coreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) - v1.1.0 is used due to some instabilitity with v1.2 code
+ [FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel)
+ [json-maker](https://github.com/rafagafe/json-maker)
+ [tine-json](https://github.com/rafagafe/tiny-json)
+ [twinThingPicoW](https://github.com/jondurrant/twinThingPicoW) - My own IoT library

## Config

All example take WIFI and MQTT credentials from environment variables. Template for scripts to set these environment variables up.

## 2 Pico Setup

+ LEDControl: Blink LED on GPIO0 and Control LED on GPIO15 over MQTT

## 4 Connectivity

+ Wifi: Wifi connection using CYW43 on the Pico-W
+ HTTPGet: Demonstrate TCPIP Socket Transport using an HTTP GET
+ MQTTConnect: Establish an MQTT Connection to a broker


## 5 Publish

+ Listen - Python script to listen to the device over MQTT
+ LifeCycle - Adding a birth message on connection along with the will on disconnect
+ PubLED - Manage an LED on GPIO 0 and Switch on GPIO 1 as a latched LED. Publish state to MQTT


## 6 Subscribe

+ PyPublish - Publish request to the device from Python over MQTT, to remotely turn on and off the LED
+ PubSubLED - Extended the PubLED example so that the latched LED can also be controlled over MQTT
+ GrpPubSubLED - Extend the PubSubLED so that the switch on each device controls all the devices, not just the one it is attached to
+ Ass PubSubLEDToggle - Extend PubSubLED to include a toggle MQTT message, to change the LED state

## 8 State

+ twinThingPicoWExample - Generate the state problem to allow the state of a device to be requested with "GET" and changed with "SET". Uses the demo library TwinThingPicoW



