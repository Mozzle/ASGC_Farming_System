/*-----------------------------------------------------------------------------
 * FS_math.h
 *
 *  Created on: May 18, 2025
 *      Author: Dylan
 *
 *  DESCRIPTION: Farming System math. Local library for math functions
 *
-----------------------------------------------------------------------------*/

#ifndef INC_FS_MATH_H_
#define INC_FS_MATH_H_

#include "main.h"
#include <stdbool.h>
#include <math.h>


#define FLOAT_COMPARISON_EPSILON	1e-6f

bool float_equals(float a, float b);
uint32_t getMedian_u32(uint32_t* arr, uint8_t len);

#endif /* INC_FS_MATH_H_ */
