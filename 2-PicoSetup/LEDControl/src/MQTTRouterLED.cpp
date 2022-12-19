/*
 * MQTTRouterLED.cpp
 *
 *  Created on: 4 Oct 2022
 *      Author: jondurrant
 */

#include "MQTTRouterLED.h"
#include "MQTTTopicHelper.h"
#include "FreeRTOS.h"
#include <cstring>
#include "MQTTConfig.h"

#define LED_TOPIC  "LED/req"
#define PAYLOAD_ON "on"

MQTTRouterLED::MQTTRouterLED(char * id, uint8_t gp) {
	xLedPad = gp;
	init(id);
}

MQTTRouterLED::~MQTTRouterLED() {
	// Nop
}

/***
 * Initialise the GPIO pad for LED
 */
void MQTTRouterLED::init(char * id){
	gpio_init(xLedPad);
	gpio_set_dir(xLedPad, GPIO_OUT);
	gpio_put(xLedPad, 1);

	if (pLedTopic == NULL){
		pLedTopic = (char *)pvPortMalloc(
				MQTTTopicHelper::lenThingTopic(id, LED_TOPIC)
				);
		if (pLedTopic != NULL){
			MQTTTopicHelper::genThingTopic(pLedTopic, id, LED_TOPIC);
		} else {
			LogError( ("Unable to allocate topic") );
		}
	}
}


/***
 * Use the interface to setup all the subscriptions
 * @param interface
 */
void MQTTRouterLED::subscribe(MQTTInterface *interface){
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

	if (payloadLen >= LED_BUFFER){
		LogError(("Payload >= than %d", LED_BUFFER));
		return;
	}
	memcpy(pBuffer, payload, payloadLen);
	pBuffer[payloadLen] = 0;

	json_t const* json = json_create(pBuffer, pJsonPool, LED_JSON_POOL);
	if ( !json ) {
	 LogError(("Error json create."));
	 return ;
	}

	json_t const* on = json_getProperty( json, PAYLOAD_ON );
	if ( !on || JSON_BOOLEAN != json_getType( on ) ) {
	 LogError(("Error, the property is not found."));
	 return ;
	}
	bool  b = json_getBoolean(on);

	gpio_put(xLedPad, b);

}
