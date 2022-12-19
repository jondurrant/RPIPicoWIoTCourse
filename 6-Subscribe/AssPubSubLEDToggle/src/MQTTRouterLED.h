/*
 * MQTTRouterLED.h
 *
 *  Created on: 4 Oct 2022
 *      Author: jondurrant
 */

#ifndef _MQTTROUTERLED_H_
#define _MQTTROUTERLED_H_

#include "pico/stdlib.h"
#include "MQTTRouter.h"

#include "tiny-json.h"
#include "LEDAgent.h"

#define MQTT_LED_REQ_TOPIC 	"LED/req"

class MQTTRouterLED : public MQTTRouter{
public:
	/***
	 * Constructor
	 */
	MQTTRouterLED(LEDAgent *agent);
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
	LEDAgent *pAgent = NULL;
	char *pLedTopic = NULL;

};


#endif /*_MQTTROUTERLED_H_ */
