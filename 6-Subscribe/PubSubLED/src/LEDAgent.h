/*
 * LEDAgent.h
 *
 * Manage LED and latched switch software behaviour
 *
 *  Created on: 23 Oct 2022
 *      Author: jondurrant
 */

#ifndef _LEDAGENT_H_
#define _LEDAGENT_H_


#include "Agent.h"
#include "SwitchObserver.h"
#include "SwitchMgr.h"
#include "tiny-json.h"

#include "pico/stdlib.h"
#include "queue.h"
#include "message_buffer.h"
#include "MQTTConfig.h"
#include "MQTTInterface.h"

#define LED_QUEUE_LEN 	5
#define MQTT_TOPIC_LED_STATE "LED/state"
#define LED_BUFFER_LEN 	256
#define LED_JSON_LEN 	20
#define LED_JSON_POOL 	5


class LEDAgent : public Agent, public SwitchObserver {
public:
	/***
	 * Constructor
	 * @param ledGP - GPIO Pad of LED to control
	 * @param spstGP - GPIO Pad of SPST non latched switch
	 * @param interface - MQTT Interface that state will be notified to
	 */
	LEDAgent(uint8_t ledGP, uint8_t spstGP, MQTTInterface *interface);

	/***
	 * Destructor
	 */
	virtual ~LEDAgent();

	/***
	 * Set the states of the LED to - on
	 * @param on - boolean if the LED should be on or off
	 */
	void setOn(bool on);

	/***
	 * Toggle the state of the LED. so On becomes Off, etc.
	 */
	void toggle();


	/***
	 * Add a JSON string action
	 * @param jsonStr
	 */
	void addJSON(const void  *jsonStr, size_t len);

	/***
	 * Handle a short press from the switch
	 * @param gp - GPIO number of the switch
	 */
	virtual void handleShortPress(uint8_t gp);

	/***
	 * Handle a short press from the switch
	 * @param gp - GPIO number of the switch
	 */
	virtual void handleLongPress(uint8_t gp);

protected:
	/***
	 * Task main run loop
	 */
	virtual void run();

	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();

private:
	/***
	 * Toggle LED state from within an intrupt
	 */
	void intToggle();

	/***
	 * Execute the state on the LED and notify MQTT interface
	 * @param state
	 */
	void execLed(bool state);

	/***
	 * Parse a JSON string and add request to queue
	 * @param str - JSON Strging
	 */
	void parseJSON(char *str);

	//Interface to publish state to MQTT
	MQTTInterface *pInterface = NULL;

	// Topic to publish on
	char * pTopicLedState = NULL;

	//State of the LED
	bool xState = false;

	//LED and switch pads
	uint8_t xLedGP;
	uint8_t xSpstGP;

	// Switch manage to manage the SPST switch
	SwitchMgr *pSwitchMgr = NULL;


	//Queue of commands
	QueueHandle_t xCmdQ;

	// Message buffer handle
	MessageBufferHandle_t xBuffer = NULL;

	// Json decoding buffer
	json_t pJsonPool[ LED_JSON_POOL ];

};


#endif /* _LEDAGENT_H_ */
