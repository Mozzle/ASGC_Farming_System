/*-----------------------------------------------------------------------------
 *
 * mixing_motor.h
 *
 * 		Mixing Motor Control Driver.
 *
 * 		GPIO interaction with a DROK L298 Motor Driver
 *
 *  Created on: June 1, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "mixing_motor.h"

bool mixing_motor_Init() {

	bool ret_val = MIXING_MOTOR_INIT_FAIL;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = MIXING_MOTOR_INIT_SUCCEED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}

SYS_RESULT mixing_motor_drive(uint8_t speed) {

	SYS_RESULT ret_val = SYS_INVALID;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}

SYS_RESULT mixing_motor_stop() {

	SYS_RESULT ret_val = SYS_INVALID;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}
