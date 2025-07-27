/*-----------------------------------------------------------------------------
 *
 * Buttons.c
 *
 * 		Code covering 'Start' and 'E-Stop' button interrupts and functionality
 *
 *  Created on: June 1, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "Buttons.h"

/*-----------------------------------------------------------------------------
	Static Values
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *
 * 		Buttons_Init()
 *
 * 		Initializes the buttons Module.
 *
 ----------------------------------------------------------------------------*/

bool Buttons_Init(bool *SYSTEM_START_STATE) {

	bool ret_val = BUTTONS_INIT_FAIL;

	// If start button is switchboard disabled, skip the need to wait for the start
	// button to be pressed to start the system.
	if (START_BUTTON_ENABLED == SYS_FEATURE_DISABLED) {

		*SYSTEM_START_STATE = SYSTEM_ON;

	}

	// If EStop button is switchboard disabled, initialize it.
	if (ESTOP_BUTTON_ENABLED == SYS_FEATURE_DISABLED) {

		/* IMPLEMENT ME! */

	}

	/* IMPLEMENT ME! */

	return ret_val;
}

/*-----------------------------------------------------------------------------
 *
 * 		Buttons_start_button_intrpt()
 *
 * 		Start Button's interrupt. Handles
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT Buttons_start_button_intrpt(bool * start_state) {

	SYS_RESULT ret_val = SYS_INVALID;

	// If Start Button is switchboard disabled, return without executing interrupt
	if (START_BUTTON_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* If transitioning from system state off (boot up) to system state on 	 */
	/* (start button pressed)												 */
	if (*start_state == SYSTEM_OFF) {

		*start_state = SYSTEM_ON;
		ASGC_System_Startup();
	}

	if (*start_state == SYSTEM_ON) {
		// Maybe have a future functionality for pressing the start button after
		// the machine has been started. There may be debounce behavior that will
		// need to be implemented as well, but likely isnt a problem when we only
		// need the one press of the start button.
	}

	return ret_val;
}

/*-----------------------------------------------------------------------------
 *
 * 		Buttons_estop_button_intrpt()
 *
 * 		E-Stop Button's interrupt function. Triggers entire system shutdown
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT Buttons_estop_button_intrpt(bool * estop_state, bool start_state) {

	SYS_RESULT ret_val = SYS_INVALID;

	// If EStop Button is switchboard disabled, return without executing interrupt
	if (ESTOP_BUTTON_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* If start button has not yet been pressed, do nothing 				 */
	if (start_state == SYSTEM_OFF) {
		return ret_val;
	}
	/* If transitioning from system state on to ESTOP on (estop pressed) 	 */
	else if (*estop_state == SYSTEM_OFF) {
		*estop_state = SYSTEM_ON;
		ASGC_System_ESTOP();
	}

	return ret_val;
}
