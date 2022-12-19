/*
 * LEDState.cpp
 *
 *  Created on: 26 Oct 2022
 *      Author: jondurrant
 */

#include "LEDState.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "json-maker/json-maker.h"

LEDState::LEDState() {
	elements=4;

	jsonHelpers[ONSLOT] = (StateFunc)&LEDState::jsonOn;
}

LEDState::~LEDState() {
	// NOP
}

/***
 * Copy constructor
 * @param other
 */
LEDState::LEDState(const LEDState &other): StateTemp(other) {
	on = other.getOn();
}

/***
 * Set On status of switch
 * @param bool
 */
void LEDState::setOn(bool b){
	on = b;
	setDirty(ONSLOT);
}

/***
 * Get on status of switch
 * @return
 */
bool LEDState::getOn() const{
	return on;
}

/***
 * Retrieve on status in JSON format
 * @param buf
 * @param len
 * @return
 */
char* LEDState::jsonOn(char *buf, unsigned int len){
	char *p = buf;
    p = json_bool( p, "on", getOn(), &len);
    return p;
}

/***
* Update state data from a json structure
* @param json
*/
void LEDState::updateFromJson(json_t const *json){
	StateTemp::updateFromJson(json);

	json_t const *jp;
	uint8_t newRGB[3]={0, 0, 0};
	bool rgbOK = true;

	jp = json_getProperty(json, "on");
	if (jp){
		if (JSON_BOOLEAN == json_getType(jp)){
			bool b = json_getBoolean(jp);
			setOn(b);
		}
	}
}

/***
 * Retrieve state of object in JSON format
 * @param buf - buffer to write to
 * @param len - length of buffer
 * @return length of json or zero if we ran out of space
 */
unsigned int LEDState::state(char *buf, unsigned int len){
	updateTemp();
	return StateTemp::state(buf, len);
}


