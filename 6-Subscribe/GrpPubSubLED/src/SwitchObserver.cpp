/*
 * SwitchObserver.cpp
 *
 *  Created on: 1 Jun 2022
 *      Author: jondurrant
 */

#include "SwitchObserver.h"
#include <stdio.h>

/***
 * Constructor
 */
SwitchObserver::SwitchObserver() {
	// NOP

}

/***
 * Destructor
 */
SwitchObserver::~SwitchObserver() {
	// NOP
}

/***
 * Handle short press from switch
 * @param gp - GPIO number of the switch
 */
void SwitchObserver::handleShortPress(uint8_t gp){
	printf("Short Press on %d\n", gp);
}


/***
 * Handle short press from switch
 * @param gp - GPIO number of the switch
 */
void SwitchObserver::handleLongPress(uint8_t gp){
	printf("Long Press on %d\n", gp);
}
