/*-----------------------------------------------------------------------------
 *
 * gpio_switching_intf.c
 *
 *  Created on: May 29, 2025
 *
 *  	Interface to control the MOSFET and Solid State Relay switching of the
 *		system's pumps and valves
 *
-----------------------------------------------------------------------------*/

#include "gpio_switching_intf.h"

bool init_gpio_switching_intf() {

	bool ret_val = GPIO_SWITCHING_INTF_INIT_FAIL;

	/* IMPLEMENT ME! */

	return ret_val;
}

SYS_RESULT set_circulating_pump(bool state) {

	SYS_RESULT result = SYS_INVALID;

	/* IMPLEMENT ME! */

	return result;
}

SYS_RESULT set_drainage_pump(bool state) {

	SYS_RESULT result = SYS_INVALID;

	/* IMPLEMENT ME! */

	return result;
}

SYS_RESULT set_fill_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	/* IMPLEMENT ME! */

	return result;
}

SYS_RESULT set_nutrient_soln_a_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	/* IMPLEMENT ME! */

	return result;
}

SYS_RESULT set_nutrient_soln_b_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	/* IMPLEMENT ME! */

	return result;
}

SYS_RESULT set_ph_up_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	/* IMPLEMENT ME! */

	return result;
}

SYS_RESULT set_ph_down_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	/* IMPLEMENT ME! */

	return result;
}

