/*
 * FS_math.c
 *
 *  Created on: May 18, 2025
 *      Author: Dylan
 *
 *  DESCRIPTION: Farming System math. Local library for math functions
 */

#include "FS_math.h"

bool float_equals(float a, float b) {
	bool ret_val = false;

	if (fabs(a - b) < FLOAT_COMPARISON_EPSILON) {
		ret_val = true;
	}
	else {
		ret_val = false;
	}

	return ret_val;
}



