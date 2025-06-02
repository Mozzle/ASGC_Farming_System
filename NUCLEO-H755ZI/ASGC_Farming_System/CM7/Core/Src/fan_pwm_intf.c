/*-----------------------------------------------------------------------------
 *
 * fan_pwm_intf.c
 *
 * 		Pulse-Width Modulation (PWM) Interface for driving air-circulating
 * 		fans.
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "fan_pwm_intf.h"


/*-----------------------------------------------------------------------------
 *
 * 		fan_pwm_intf_Init()
 *
 * 		Initializes the Fan PWM Interface Module
 *
 ----------------------------------------------------------------------------*/

bool fan_pwm_intf_Init() {

	bool ret_val = FAN_PWM_INTF_INIT_FAIL;

	// If fan interface is switchboard disabled, exit without doing anything.
	if (FAN_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = FAN_PWM_INTF_INIT_SUCCEED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		fan_pwm_intf_set_duty()
 *
 * 		Sets the PWM Duty Cycle of the fans.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT fan_pwm_intf_set_duty(uint8_t duty) {

	SYS_RESULT ret_val = SYS_INVALID;

	// If fan interface is switchboard disabled, exit without doing anything.
	if (FAN_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		clamp_duty()
 *
 * 		Helper function to clamp duty cycle within an acceptable range
 * 		(This function may not be necessary to implement)
 *
 ----------------------------------------------------------------------------*/

void clamp_duty() {

	/* IMPLEMENT ME! */

}
