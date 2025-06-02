/*-----------------------------------------------------------------------------
 *
 * SEN0244.c
 *
 * 		SEN0244 Water EC Sensor
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "SEN0244.h"

bool SEN0244_Init() {

	bool ret_val = SEN0244_INIT_FAIL;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (SEN0244_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SEN0244_INIT_SUCCEED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}

SYS_RESULT SEN0244_Measure(float *data) {

	SYS_RESULT ret_val = SYS_INVALID;

	// If fan interface is switchboard disabled, exit without doing anything.
	if (SEN0169_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/* IMPLEMENT ME! */

	return ret_val;
}
