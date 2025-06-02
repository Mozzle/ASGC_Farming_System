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


/*-----------------------------------------------------------------------------
 *
 * 		gpio_switching_intf_Init()
 *
 * 		Initializes the MOSFET/Solid State Relay Switching Module
 *
 ----------------------------------------------------------------------------*/

bool gpio_switching_intf_Init() {

	bool ret_val = GPIO_SWITCHING_INTF_INIT_FAIL;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = GPIO_SWITCHING_INTF_INIT_SUCCEED;
		return ret_val;
	}


	/* IMPLEMENT ME! */

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		set_circulating_pump()
 *
 * 		Switches the circulating pump on or off. Values passed into this
 * 		function should either be PUMP_ON or PUMP_OFF for clarity's sake.
 *
 * 		The circulating pump is responsible for moving water-nutrient solution
 * 		from the reservoir tank to the NFT Channels.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT set_circulating_pump(bool state) {

	SYS_RESULT result = SYS_INVALID;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	/* IMPLEMENT ME! */

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		set_drainage_pump()
 *
 * 		Switches the drainage pump on or off. Values passed into this
 * 		function should either be PUMP_ON or PUMP_OFF for clarity's sake.
 *
 * 		The drainage pump is responsible for moving water-nutrient solution
 * 		from the reservoir tank to waste-water tank at the end of a growth
 * 		cycle.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT set_drainage_pump(bool state) {

	SYS_RESULT result = SYS_INVALID;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	/* IMPLEMENT ME! */

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		set_fill_valve()
 *
 * 		Switches the fill valve open or closed. Values passed into this
 * 		function should either be VALVE_OPEN or VALVE_CLOSED for clarity's
 * 		sake.
 *
 * 		The fill valve is responsible for allowing water to pass from the main
 * 		water tank into the reservoir tank when filling.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT set_fill_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	/* IMPLEMENT ME! */

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		set_nutrient_soln_a_valve()
 *
 * 		Switches the nutrient solution A valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The nutrient solution valves are responsible for dispensing nutrients
 * 		into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT set_nutrient_soln_a_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	/* IMPLEMENT ME! */

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		set_nutrient_soln_b_valve()
 *
 * 		Switches the nutrient solution B valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The nutrient solution valves are responsible for dispensing nutrients
 * 		into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT set_nutrient_soln_b_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	/* IMPLEMENT ME! */

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		set_ph_up_valve()
 *
 * 		Switches the pH up buffer valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The pH buffer solution valves are responsible for dispensing the pH
 * 		altering solution into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT set_ph_up_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	/* IMPLEMENT ME! */

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		set_ph_down_valve()
 *
 * 		Switches the pH down buffer valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The pH buffer solution valves are responsible for dispensing the pH
 * 		altering solution into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT set_ph_down_valve(bool state) {

	SYS_RESULT result = SYS_INVALID;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	/* IMPLEMENT ME! */

	return result;
}

