/*
 * MQTTRouterLED.h
 *
 *  Created on: 4 Oct 2022
 *      Author: jondurrant
 */

#include "pico/stdlib.h"
#include "MQTTRouter.h"

#ifndef _MQTTROUTERLED_H_
#define _MQTTROUTERLED_H_

#include "tiny-json.h"


#define LED_JSON_POOL  4
#define LED_BUFFER    20

class MQTTRouterLED : public MQTTRouter{
public:
	/***
	 * Constructor
	 * @param id - ID of the device, used to build the topic
	 * @param gp - GPIO PAD number for the LED to control
	 */
	MQTTRouterLED(char *id, uint8_t gp);
	virtual ~MQTTRouterLED();

	/***
	 * Use the interface to setup all the subscriptions
	 * @param interface
	 */
	virtual void subscribe(MQTTInterface *interface);

	/***
	 * Route the message the appropriate part of the application
	 * @param topic
	 * @param topicLen
	 * @param payload
	 * @param payloadLen
	 * @param interface
	 */
	virtual void route(const char *topic, size_t topicLen, const void * payload,
			size_t payloadLen, MQTTInterface *interface);



private:
	/***
	 * Initialise the GPIO pad for LED
	 */
	void init(char * id);

	uint8_t xLedPad;
	char *pLedTopic = NULL;

	// Temporaty storage buffer so we can null terminate the string
	char pBuffer[LED_BUFFER];

	// Json decoding buffer
	json_t pJsonPool[ LED_JSON_POOL ];
};


#endif /*_MQTTROUTERLED_H_ */
