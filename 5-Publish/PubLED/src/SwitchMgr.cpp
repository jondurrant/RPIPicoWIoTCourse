/*
 * SwitchMgr.cpp
 *
 *  Created on: 1 Jun 2022
 *      Author: jondurrant
 */

#include "SwitchMgr.h"
#include "pico/stdlib.h"
#include <stdlib.h>
#include "hardware/gpio.h"
#include "pico/time.h"
#include <stdio.h>
#include "GPIOInputMgr.h"


/***
 * Constructor
 * @param gp - GPIO number for the switch
 */
SwitchMgr::SwitchMgr(uint8_t gp) {
	//Store the GP for later
	xGP = gp;

	//Setup GPIO
	GPIOInputMgr::getMgr()->addObserver(xGP, this);
	gpio_pull_up (xGP);
}

SwitchMgr::~SwitchMgr() {
	// NOP
}

/***
 * Set single observer of the switch
 * @param obs - observer object
 */
void SwitchMgr::setObserver(SwitchObserver * obs){
	pObs = obs;
}

/***
 * Handle a short press event by notifying the observer
 */
void SwitchMgr::handleShortPress(){
	if (pObs != NULL){
		pObs->handleShortPress(xGP);
	}
}

/***
 * Handle a long press event by notifying the observer
 */
void SwitchMgr::handleLongPress(){
	if (pObs != NULL){
		pObs->handleLongPress(xGP);
	}
}


/***
 * handle GPIO push switch events
 * @param gpio - GPIO number
 * @param events - Event
 */
void SwitchMgr::handleGPIO(uint gpio, uint32_t events){
	//Check callback is for the right GPIO Pin
	if (gpio == xGP){

		//If Falling Edge then collect time
		if ((events & GPIO_IRQ_EDGE_FALL) > 0){
			xFallingTime = to_ms_since_boot (get_absolute_time ());
		}

		//If Rising Edge
		if ((events & GPIO_IRQ_EDGE_RISE) > 0){
			uint32_t stableTime = to_ms_since_boot (get_absolute_time ()) - xFallingTime;
			xFallingTime = 0;

			//If valid length then handle press
			if ( (stableTime > SHORT_PRESS_TIME) &&
				 (stableTime < LONG_PRESS_TIME)){
				handleShortPress();
			}
			if ( (stableTime >= LONG_PRESS_TIME) &&
					 (stableTime < MAX_PRESS_TIME)){
				handleLongPress();
			}

		}
	}


}
