/*
 * MQTTAgentObserver.cpp
 *
 *  Created on: 23 Nov 2021
 *      Author: jondurrant
 */

#include "MQTTAgentObserver.h"
#include "MQTTConfig.h"

MQTTAgentObserver::MQTTAgentObserver() {
	// NOP

}

MQTTAgentObserver::~MQTTAgentObserver() {
	// NOP
}


 void MQTTAgentObserver::MQTTOffline(){
	 LogInfo(("MQTT Offline"));
 }

 void MQTTAgentObserver::MQTTOnline(){
	 LogInfo(("MQTT Online"));
 }

 void MQTTAgentObserver::MQTTSend(){
	 LogInfo(("MQTT Send"));
 }

 void MQTTAgentObserver::MQTTRecv(){
	 LogInfo(("MQTT Receive"));
 }
