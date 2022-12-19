/*
 * MQTTRouterLED.cpp
 *
 *  Created on: 4 Oct 2022
 *      Author: jondurrant
 */

#include "MQTTRouterLED.h"
#include "MQTTTopicHelper.h"

#define LED_TOPIC  "LED"
#define PAYLOAD_ON "on"

MQTTRouterLED::MQTTRouterLED(LEDAgent *agent) {
	pAgent = agent;
}

MQTTRouterLED::~MQTTRouterLED() {
	// Nop
}



/***
 * Use the interface to setup all the subscriptions
 * @param interface
 */
void MQTTRouterLED::subscribe(MQTTInterface *interface){
	//Init topic if needed
	if (pLedTopic == NULL){
		const char *id = interface->getId();
		pLedTopic = (char *)pvPortMalloc(
			MQTTTopicHelper::lenThingTopic(id, MQTT_LED_REQ_TOPIC)
			);
		if (pLedTopic != NULL){
			MQTTTopicHelper::genThingTopic(pLedTopic, id, MQTT_LED_REQ_TOPIC);
		} else {
			LogError( ("Unable to allocate topic") );
		}
	}
	if (pLedTopic != NULL){
		interface->subToTopic(pLedTopic, 1);
	}
}

/***
 * Route the message the appropriate part of the application
 * @param topic
 * @param topicLen
 * @param payload
 * @param payloadLen
 * @param interface
 */
void MQTTRouterLED::route(const char *topic,
		size_t topicLen,
		const void * payload,
		size_t payloadLen,
		MQTTInterface *interface){

	if (pAgent != NULL){
		pAgent->addJSON(payload, payloadLen);
	}

}
