/*-----------------------------------------------------------------------------
 *
 * GPIO_switching_intf.c
 *
 *  Created on: May 29, 2025
 *
 *  	Interface to control the MOSFET and Solid State Relay switching of the
 *		system's pumps and valves
 *
-----------------------------------------------------------------------------*/

#include "GPIO_switching_intf.h"


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_switching_intf_Init()
 *
 * 		Initializes the MOSFET/Solid State Relay Switching Module
 *
 ----------------------------------------------------------------------------*/

bool GPIO_switching_intf_Init() {

	bool ret_val = GPIO_SWITCHING_INTF_INIT_FAIL;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = GPIO_SWITCHING_INTF_INIT_SUCCEED;
		return ret_val;
	}


	//Intialize all GPIO pins to off (false), before it is changed with below functions 
	HAL_GPIO_WritePin(CIRCULATING_PUMP_PORT, CIRCULATING_PUMP_PIN, PUMP_OFF); 
	HAL_GPIO_WritePin(DRAINAGE_PUMP_PORT, DRAINAGE_PUMP_PIN, PUMP_OFF); 
	HAL_GPIO_WritePin(FILL_VALVE_PORT, FILL_VALVE_PIN, VALVE_CLOSED); 
	HAL_GPIO_WritePin(NUTRIENT_SOLN_A_PORT, NUTRIENT_SOLN_A_PIN, VALVE_CLOSED); 
	HAL_GPIO_WritePin(NUTRIENT_SOLN_B_PORT, NUTRIENT_SOLN_B_PIN, VALVE_CLOSED); 
	HAL_GPIO_WritePin(PH_UP_VALVE_PORT, PH_UP_VALVE_PIN, VALVE_CLOSED); 
	HAL_GPIO_WritePin(PH_DOWN_VALVE_PORT, PH_DOWN_VALVE_PIN, VALVE_CLOSED); 
	
	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_set_circulating_pump()
 *
 * 		Switches the circulating pump on or off. Values passed into this
 * 		function should either be PUMP_ON or PUMP_OFF for clarity's sake.
 *
 * 		The circulating pump is responsible for moving water-nutrient solution
 * 		from the reservoir tank to the NFT Channels.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT GPIO_set_circulating_pump(bool state) {

	SYS_RESULT result = SYS_SUCCESS;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	//Set GPIO pin E10 high, and return result 
	HAL_GPIO_WritePin(CIRCULATING_PUMP_PORT, CIRCULATING_PUMP_PIN, state); 
	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_set_drainage_pump()
 *
 * 		Switches the drainage pump on or off. Values passed into this
 * 		function should either be PUMP_ON or PUMP_OFF for clarity's sake.
 *
 * 		The drainage pump is responsible for moving water-nutrient solution
 * 		from the reservoir tank to waste-water tank at the end of a growth
 * 		cycle.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT GPIO_set_drainage_pump(bool state) {

	SYS_RESULT result = SYS_SUCCESS;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	//Set GPIO pin E11 high, and return result 
	HAL_GPIO_WritePin(DRAINAGE_PUMP_PORT, DRAINAGE_PUMP_PIN, state); 

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_set_fill_valve()
 *
 * 		Switches the fill valve open or closed. Values passed into this
 * 		function should either be VALVE_OPEN or VALVE_CLOSED for clarity's
 * 		sake.
 *
 * 		The fill valve is responsible for allowing water to pass from the main
 * 		water tank into the reservoir tank when filling.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT GPIO_set_fill_valve(bool state) {

	SYS_RESULT result = SYS_SUCCESS;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	//Set GPIO pin E12 high, and return result 
	HAL_GPIO_WritePin(FILL_VALVE_PORT, FILL_VALVE_PIN, state); 
	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_set_nutrient_soln_a_valve()
 *
 * 		Switches the nutrient solution A valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The nutrient solution valves are responsible for dispensing nutrients
 * 		into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT GPIO_set_nutrient_soln_a_valve(bool state) {

	SYS_RESULT result = SYS_SUCCESS;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	//Set GPIO pin E13 high, and return result 
	HAL_GPIO_WritePin(NUTRIENT_SOLN_A_PORT, NUTRIENT_SOLN_A_PIN, state); 

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_set_nutrient_soln_b_valve()
 *
 * 		Switches the nutrient solution B valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The nutrient solution valves are responsible for dispensing nutrients
 * 		into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT GPIO_set_nutrient_soln_b_valve(bool state) {

	SYS_RESULT result = SYS_SUCCESS;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	//Set GPIO pin E14 high, and return result 
	HAL_GPIO_WritePin(NUTRIENT_SOLN_B_PORT, NUTRIENT_SOLN_B_PIN, state); 

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_set_ph_up_valve()
 *
 * 		Switches the pH up buffer valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The pH buffer solution valves are responsible for dispensing the pH
 * 		altering solution into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT GPIO_set_ph_up_valve(bool state) {

	SYS_RESULT result = SYS_SUCCESS;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	//Set GPIO pin E15 high, and return result 
	HAL_GPIO_WritePin(PH_UP_VALVE_PORT, PH_UP_VALVE_PIN, state); 

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		GPIO_set_ph_down_valve()
 *
 * 		Switches the pH down buffer valve open or closed. Values passed
 * 		into this function should either be VALVE_OPEN or VALVE_CLOSED for
 * 		clarity's sake.
 *
 * 		The pH buffer solution valves are responsible for dispensing the pH
 * 		altering solution into the reservoir tank.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT GPIO_set_ph_down_valve(bool state) {

	SYS_RESULT result = SYS_SUCCESS;

	// If switching interface is switchboard disabled, exit without doing anything.
	if (MOSFET_SSR_SWITCHING_ENABLED == SYS_FEATURE_DISABLED) {
		result = SYS_DEVICE_DISABLED;
		return result;
	}

	//Set GPIO pin B10 high, and return result 
	HAL_GPIO_WritePin(PH_DOWN_VALVE_PORT, PH_DOWN_VALVE_PIN, GPIO_PIN_SET); 

	return result;
}

