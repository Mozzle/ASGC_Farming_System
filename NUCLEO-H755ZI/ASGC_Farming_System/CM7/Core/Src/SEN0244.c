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
#include "FS_math.h"

// ADC hanldler declared in main.c
extern ADC_HandleTypeDef hadc2;

bool SEN0244_Init() {

	/*-------------------------------------------------------------------------
	If EC sensor is switchboard disabled, exit without doing anything.
	-------------------------------------------------------------------------*/
	if (SEN0244_ENABLED == SYS_FEATURE_DISABLED) {
		return SEN0244_INIT_SUCCEED;
	}

	/*-------------------------------------------------------------------------
	Run ADC Calibration
	-------------------------------------------------------------------------*/
	if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK ) {
		return SEN0244_INIT_FAIL;
	}

	/*-------------------------------------------------------------------------
	Start the ADC. Exit if the ADC fails to start
	-------------------------------------------------------------------------*/
	if ( HAL_ADC_Start(&hadc2) != HAL_OK ) {
		return SEN0244_INIT_FAIL;
	}

}


/*-----------------------------------------------------------------------------
 *
 * 		SEN0244_Measure
 *
 * 		Description
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT SEN0244_Measure(float *data) {

	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	uint32_t measurement[SEN0244_NUM_MEASUREMENTS] = {0};
	uint32_t medianMeasurement = 0;
	SYS_RESULT ret_val = SYS_INVALID;
	uint8_t i = 0;

	/*-------------------------------------------------------------------------
	If driver is switchboard disabled, exit without doing anything.
	-------------------------------------------------------------------------*/
	if (SEN0244_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/*-------------------------------------------------------------------------
	Take SEN0244_NUM_MEASUREMENTS measurements from ADC
	-------------------------------------------------------------------------*/
	for( i = 0; i < SEN0244_NUM_MEASUREMENTS; i++ ) {
		HAL_ADC_PollForConversion(&hadc2, 1);
		measurement[i] += HAL_ADC_GetValue(&hadc2);
	}

	medianMeasurement = getMedian_u32(measurement, SEN0244_NUM_MEASUREMENTS);

	// Convert the median measurement to EC value here

	return ret_val;
}

/*-----------------------------------------------------------------------------
 *
 * 		SEN0244_Stop_ADC
 *
 * 		Stops the ADC from taking any more measurements.
 *
 ----------------------------------------------------------------------------*/

void SEN0244_Stop_ADC() {
	HAL_ADC_Stop(&hadc2);
}