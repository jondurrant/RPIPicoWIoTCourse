/*
 * SwitchMgr.h
 *
 *  Created on: 1 Jun 2022
 *      Author: jondurrant
 *
 *      Manage a SPST switch on a specific GPIO pin.
 *      Notify observers of press of the switch
 *      Manage through Interrupt strategy
 */

#ifndef SRC_SWITCHMGR_H_
#define SRC_SWITCHMGR_H_

#include "SwitchObserver.h"
#include "GPIOObserver.h"

//Time switch must be stable as low to consider being a short press
#define SHORT_PRESS_TIME 5 //ms
#define LONG_PRESS_TIME 200 //ms
#define MAX_PRESS_TIME  2000 //ms



class SwitchMgr : public GPIOObserver{
public:
	/***
	 * Constructor
	 * @param gp - GPIO that the switch is connected too.
	 */
	SwitchMgr(uint8_t gp);

	/***
	 * Destructor
	 */
	virtual ~SwitchMgr();

	/***
	 * Set a single object that is observing the switches state
	 * @param obs - Observer object
	 */
	void setObserver(SwitchObserver * obs);

	/***
	 * handle GPIO push switch events
	 * @param gpio - GPIO number
	 * @param events - Event
	 */
	void handleGPIO(uint gpio, uint32_t events);
private:

	/***
	 * Handle a short press
	 */
	void handleShortPress();

	/***
	 * Handle a long press
	 */
	void handleLongPress();


	//GPIO number for the switch
	uint8_t xGP = 0;

	//Time falling edge was detected
	uint32_t xFallingTime;

	//Observer
	SwitchObserver *pObs = NULL;

};

#endif /* SRC_SWITCHMGR_H_ */
