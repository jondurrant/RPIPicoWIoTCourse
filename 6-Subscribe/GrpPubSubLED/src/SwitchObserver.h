/*
 * SwitchObserver.h
 *
 * Virtual observer class for switch.
 * Any observer should inherit from this class and override the handleShortpress
 * method.
 *
 *  Created on: 1 Jun 2022
 *      Author: jondurrant
 */

#ifndef SRC_SWITCHOBSERVER_H_
#define SRC_SWITCHOBSERVER_H_

#include "pico/stdlib.h"
#include <stdlib.h>

class SwitchObserver {
public:
	/***
	 * Constructor - does nothing
	 */
	SwitchObserver();

	/***
	 * Destructor - does nothing
	 */
	virtual ~SwitchObserver();

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
};

#endif /* SRC_SWITCHOBSERVER_H_ */
