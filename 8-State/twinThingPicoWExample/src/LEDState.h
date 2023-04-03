/*
 * LEDState.h
 *
 *  Created on: 26 Oct 2022
 *      Author: jondurrant
 */

#ifndef _LEDSTATE_H_
#define _LEDSTATE_H_

#include "StateTemp.h"
#include <stdbool.h>
#include "pico/stdlib.h"


#define ONSLOT  3


class LEDState : public StateTemp{
public:
	LEDState();
	LEDState(const LEDState &other);
	virtual ~LEDState();
	/***
	 * Gets the On status of the light
	 * @return
	 */
	bool getOn() const;

	/***
	 * Sets the on status of the light
	 * @param b
	 */
	void setOn(bool b);

	/***
	 * Update state data from a json structure
	 * @param json
	 */
	virtual void updateFromJson(json_t const *json);

	/***
	 * Retrieve state of object in JSON format
	 * @param buf - buffer to write to
	 * @param len - length of buffer
	 * @return length of json or zero if we ran out of space
	 */
	virtual unsigned int state(char *buf, unsigned int len) ;


protected:
	/***
	 * Retried On status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonOn(char *buf, unsigned int len);

private:

	//Is light on
	bool on = false;

};


#endif /* _LEDSTATE_H_ */
