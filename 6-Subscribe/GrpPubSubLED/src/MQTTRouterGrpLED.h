/*
 * MQTTRouterGrpLED.h
 *
 *  Created on: 28 Oct 2022
 *      Author: jondurrant
 */

#ifndef _MQTTROUTERGRPLED_H_
#define _SRC_MQTTROUTERGRPLED_H_

#include "pico/stdlib.h"
#include "MQTTRouterLED.h"


class MQTTRouterGrpLED : public MQTTRouterLED{
public:
	/***
	 * Constructor
	 */
	MQTTRouterGrpLED(LEDAgent *agent);
	virtual ~MQTTRouterGrpLED();

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
};


#endif /* _MQTTROUTERGRPLED_H_ */
