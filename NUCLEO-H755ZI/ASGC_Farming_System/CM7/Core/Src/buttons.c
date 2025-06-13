/*-----------------------------------------------------------------------------
 *
 * buttons.c
 *
 * 		Code covering 'Start' and 'E-Stop' button interrupts and functionality
 *
 *  Created on: June 1, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "buttons.h"

/*-----------------------------------------------------------------------------
	Static Values
 ----------------------------------------------------------------------------*/

static bool start_button_state;
static bool estop_button_state;

/*-----------------------------------------------------------------------------
 *
 * 		buttons_Init()
 *
 * 		Initializes the buttons Module.
 *
 ----------------------------------------------------------------------------*/

bool buttons_Init() {

	bool ret_val = BUTTONS_INIT_FAIL;

	// If start button is switchboard enabled, initialize it.
	if (START_BUTTON_ENABLED == SYS_FEATURE_ENABLED) {

		start_button_state = BUTTONS_OFF;

		/* IMPLEMENT ME! */

	}

	// If EStop button is switchboard enabled, initialize it.
	if (ESTOP_BUTTON_ENABLED == SYS_FEATURE_ENABLED) {

		estop_button_state = BUTTONS_OFF;

		/* IMPLEMENT ME! */

	}

	/* IMPLEMENT ME! */

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		buttons_register_Start_button_intrpt()
 *
 * 		Registers the Start Button's interrupt function:
 * 		(INSERT FUNC NAME HERE)
 *
 ----------------------------------------------------------------------------*/

// Look into how GPIO Interrupts are actually registered and used on STM32.
// May need to register a function in main.c that has global scope.
SYS_RESULT buttons_register_Start_button_intrpt() {

	SYS_RESULT ret_val = SYS_INVALID;

	// If Start Button is switchboard disabled, return without registering function
	if (START_BUTTON_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}

/*-----------------------------------------------------------------------------
 *
 * 		buttons_register_EStop_button_intrpt()
 *
 * 		Registers the EStop Button's interrupt function:
 * 		(INSERT FUNC NAME HERE)
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT buttons_register_EStop_button_intrpt() {

	SYS_RESULT ret_val = SYS_INVALID;

	// If EStop Button is switchboard disabled, return without registering function
	if (ESTOP_BUTTON_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}
