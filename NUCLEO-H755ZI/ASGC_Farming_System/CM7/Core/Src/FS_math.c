/*-----------------------------------------------------------------------------
 *
 *  DESCRIPTION: Farming System math. Local library for math functions
 *
-----------------------------------------------------------------------------*/

#include "FS_math.h"

/*-----------------------------------------------------------------------------
 *
 * 		float_equals
 *
 * 		Returns true if the two given floats are within 1e-6 of each other.
 *
 ----------------------------------------------------------------------------*/
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


/*-----------------------------------------------------------------------------
 *
 * 		getMedian_u32
 *
 * 		Returns the median value of an array of uint32_t values.
 *
 ----------------------------------------------------------------------------*/
uint32_t getMedian_u32(uint32_t* arr, uint8_t len) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	uint32_t sortedArr[len];
	uint8_t i, j;
	uint32_t temp;

	/*-------------------------------------------------------------------------
	Copy input array to local array
	-------------------------------------------------------------------------*/
	for (i = 0; i < len; i++) {
		sortedArr[i] = arr[i];
	}

	/*-------------------------------------------------------------------------
	Bubble sort local array
	-------------------------------------------------------------------------*/
	for (i = 0; i < len - 1; i++) {
		for (j = 0; j < len - i - 1; j++) {
			if (sortedArr[j] > sortedArr[j + 1]) {
				temp = sortedArr[j];
				sortedArr[j] = sortedArr[j + 1];
				sortedArr[j + 1] = temp;
			}
		}
	}

	/*-------------------------------------------------------------------------
	Return median value
	-------------------------------------------------------------------------*/
	if (len % 2 == 0) {
		return (sortedArr[(len / 2) - 1] + sortedArr[len / 2]) / 2;
	}
	else {
		return sortedArr[len / 2];
	}
}



