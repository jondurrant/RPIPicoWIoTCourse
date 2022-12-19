/*
 * MQTTRouterGrpLED.cpp
 *
 *  Created on: 28 Oct 2022
 *      Author: jondurrant
 */

#include "MQTTRouterGrpLED.h"

MQTTRouterGrpLED::MQTTRouterGrpLED(LEDAgent *agent):
  MQTTRouterLED(agent) {
	// NOP
}

MQTTRouterGrpLED::~MQTTRouterGrpLED() {
	// NOP
}




/***
 * Use the interface to setup all the subscriptions
 * @param interface
 */
void MQTTRouterGrpLED::subscribe(MQTTInterface *interface){
	MQTTRouterLED::subscribe(interface);
	interface->subToTopic(
		pAgent->getGrpTopic(),
		1);
}

/***
 * Route the message the appropriate part of the application
 * @param topic
 * @param topicLen
 * @param payload
 * @param payloadLen
 * @param interface
 */
void MQTTRouterGrpLED::route(const char *topic,
		size_t topicLen,
		const void * payload,
		size_t payloadLen,
		MQTTInterface *interface){

	MQTTRouterLED::route(topic,
		topicLen,
		payload,
		payloadLen,
		interface);

	if (strlen(pAgent->getGrpTopic()) == topicLen){
		if (memcmp(topic, pAgent->getGrpTopic(), topicLen)==0){
			if (pAgent != NULL){
				pAgent->addGrpJSON(payload, payloadLen);
			}
		}
	}
}
