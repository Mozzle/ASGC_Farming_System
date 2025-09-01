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
bool SEN0244_ADC_On = false;


/*-----------------------------------------------------------------------------
 *
 * 		SEN0244_Init
 *
 * 		Initializes the ADC for the SEN0244 EC sensor.
 *
 ----------------------------------------------------------------------------*/

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
	SEN0244_ADC_On = true;

	return SEN0244_INIT_SUCCEED;

}


/*-----------------------------------------------------------------------------
 *
 * 		SEN0244_Measure
 *
 * 		Measures the water TDS in parts per million. Measurement formula from:
 * 		https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT SEN0244_Measure(double *tdsData, float tempData) {

	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	uint32_t measurement[SEN0244_NUM_MEASUREMENTS] = {0};
	double medianVoltage = 0; 
	double tempCompensation = 0;
	SYS_RESULT ret_val = SYS_SUCCESS;
	uint8_t i = 0;

	/*-------------------------------------------------------------------------
	If driver is switchboard disabled, exit without doing anything.
	-------------------------------------------------------------------------*/
	if (SEN0244_ENABLED == SYS_FEATURE_DISABLED || SEN0244_ADC_On == false) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/*-------------------------------------------------------------------------
	Take SEN0244_NUM_MEASUREMENTS measurements from ADC
	-------------------------------------------------------------------------*/
	for( i = 0; i < SEN0244_NUM_MEASUREMENTS; i++ ) {
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 2);
		measurement[i] = HAL_ADC_GetValue(&hadc2);
	}

	/*-------------------------------------------------------------------------
	Calculate TDS in ppm:
	1) get median ADC value
	2) convert median ADC value to voltage (3.3V reference, 12-bit ADC)
	3) apply temperature compensation (0.02/°C)
	4) TDS = (133.42*V^3 - 255.86*V^2 + 857.39*V)*0.5 + probe calibration offset
	--------------------------------------------------------------------------*/
	medianVoltage = ( ( getMedian_u32(measurement, SEN0244_NUM_MEASUREMENTS) * 3.3 ) / 4096.0);
	tempCompensation = medianVoltage / ( 1.0 + ( 0.02*(tempData-25.0 ) ) );
	*tdsData = ( (133.42*tempCompensation*tempCompensation*tempCompensation - 255.86*tempCompensation*tempCompensation + 857.39*tempCompensation)*0.5) + SEN0244_PROBE_CALIBRATION_OFFSET;

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
	SEN0244_ADC_On = false;
}
