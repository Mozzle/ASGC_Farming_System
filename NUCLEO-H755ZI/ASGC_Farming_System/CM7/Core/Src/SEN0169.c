/*-----------------------------------------------------------------------------
 *
 * SEN0169.h
 *
 * 		SEN0169-V2 Water pH Sensor Driver
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "SEN0169.h"
#include "timer.h"

extern ADC_HandleTypeDef hadc1;

static struct pH_Moving_Avg pH_History[pH_MOVING_AVG_ENTRIES];
static uint8_t pH_HistoryIndex = 0;

static void clamp_pH(SEN0169_pH_Data *pH_Data);


bool SEN0169_Init() {

	if (SEN0169_ENABLED == SYS_FEATURE_DISABLED) {
			return SEN0169_INIT_SUCCEED;
	}

	HAL_StatusTypeDef status = HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);

	if ( status == HAL_OK ) {
		return SEN0169_INIT_SUCCEED;
	}
	else {
		return SEN0169_INIT_FAIL;
	}

}


/*-----------------------------------------------------------------------------
 *
 * 		SEN0169_Measure
 *
 * 		Takes an average of SEN0169_NUM_MEASUREMENTS from the SEN0169 pH meter
 * 		module to read the pH of the water-nutrient solution.
 *
 * 		Calling this function, unlike SEN0169_Measure_SMA(), causes very
 * 		little execution delay, but is more prone to noise errors.
 *
 ----------------------------------------------------------------------------*/
SYS_RESULT SEN0169_Measure(SEN0169_pH_Data *pH_Data) {
	/*-------------------------------------------------------------------------
	Local variables
	-------------------------------------------------------------------------*/
	int32_t sum = 0;
			/* sum is 32 bits - same as the measurement return of
			 * HAL_ADC_GetValue - because the ADC is configured to make 16-bit
			 * measurements, so there should be no chance of overflow here. */
	float avgMeasurement = 0;
	uint8_t i;

	SYS_RESULT ret_val = SYS_INVALID;

	/*-------------------------------------------------------------------------
	If driver is switchboard disabled, exit without doing anything.
	-------------------------------------------------------------------------*/
	if (SEN0169_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/*-------------------------------------------------------------------------
	Start the ADC. Exit if the ADC fails to start
	-------------------------------------------------------------------------*/
	if ( HAL_ADC_Start(&hadc1) != HAL_OK ) {
		ret_val = SYS_MEASUREMENT_GET_FAIL;
		return ret_val;
	}
	HAL_Delay(1);
	/*-------------------------------------------------------------------------
	Take SEN0169_NUM_MEASUREMENTS measurements from ADC
	-------------------------------------------------------------------------*/
	for( i = 0; i < SEN0169_NUM_MEASUREMENTS; i++ ) {
		HAL_ADC_PollForConversion(&hadc1, 1);
		sum += HAL_ADC_GetValue(&hadc1);
	}

	/*-------------------------------------------------------------------------
	Stop the ADC
	-------------------------------------------------------------------------*/
	if ( HAL_ADC_Stop(&hadc1) != HAL_OK ) {
			ret_val = SYS_MEASUREMENT_GET_FAIL;
			return ret_val;
	}

	/*-------------------------------------------------------------------------
	Calculate the pH:
	1) take average of measurements.
	2) voltage = (avg measurement * 3.3) / 4096
	3) pH = voltage * -5.6012 + 15.498
	-------------------------------------------------------------------------*/
	avgMeasurement = (float)sum / SEN0169_NUM_MEASUREMENTS;

	*pH_Data = ( avgMeasurement * SEN0169_CONVERSION_FACTOR ) + SEN0169_INTERCEPT_OFFSET;

	clamp_pH(pH_Data);

	ret_val = SYS_SUCCESS;

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		SEN0169_Measure_SMA
 *
 * 		Uses a Simple Moving Average to smooth out the pH meter measurements
 * 		and generate a less noisy output. This function can cause a blocking
 * 		delay of 6 ms.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT SEN0169_Measure_SMA(SEN0169_pH_Data * pH_Data) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	bool SMA_isCurrent = false;
	SEN0169_pH_Data * tmpData;
	SYS_RESULT ret_val;
	uint8_t i = 0;
	uint64_t curTime;
	uint64_t timeMinusTimeout;
	float sum = 0;

	/*-------------------------------------------------------------------------
	While not all of the data for the simple moving average is not considered
	'current':
	-------------------------------------------------------------------------*/
	while (SMA_isCurrent == false) {
		/*---------------------------------------------------------------------
		Take a measurement
		---------------------------------------------------------------------*/
		ret_val = SEN0169_Measure(tmpData);

		if (ret_val != SYS_SUCCESS) {
			return SYS_FAIL;
		}

		/*---------------------------------------------------------------------
		Add measurement to circular buffer
		---------------------------------------------------------------------*/
		pH_History[pH_HistoryIndex].pH_Data = *tmpData;
		curTime = getTimestamp();
		pH_History[pH_HistoryIndex].timestamp = curTime;

		pH_HistoryIndex = ( ( pH_HistoryIndex + 1 ) % pH_MOVING_AVG_ENTRIES);

		/*---------------------------------------------------------------------
		Determine the timestamp of pH_MOVING_AVG_TIMEOUT_MS ago.
		---------------------------------------------------------------------*/
		SMA_isCurrent = true;
		if (curTime < pH_MOVING_AVG_TIMEOUT_MS ) {
			timeMinusTimeout = 0;
		}
		else {
			timeMinusTimeout = curTime - pH_MOVING_AVG_TIMEOUT_MS;
		}

		/*---------------------------------------------------------------------
		Determine if every measurement in the circular buffer happened in the
		last pH_MOVING_AVG_TIMEOUT_MS milliseconds. If not, stay in while loop
		until all measurements are 'current'.
		---------------------------------------------------------------------*/
		for ( i = 0; i < pH_MOVING_AVG_ENTRIES; i++ ) {

			if ( ( timeMinusTimeout ) > pH_History[i].timestamp ) {
				SMA_isCurrent = false;
			}
		}

	}

	/*-------------------------------------------------------------------------
	Get average pH of all values in buffer and return that average.
	-------------------------------------------------------------------------*/
	for ( i = 0; i < pH_MOVING_AVG_ENTRIES; i++ ) {
		sum += pH_History[i].pH_Data;
	}

	*pH_Data = ( sum / (float) pH_MOVING_AVG_ENTRIES );

	clamp_pH(pH_Data);

	return SYS_SUCCESS;
}

/*-----------------------------------------------------------------------------
 *
 * 		clamp_pH
 *
 * 		Ensures measured pH values are within physically possible limits
 *
 ----------------------------------------------------------------------------*/

void clamp_pH(SEN0169_pH_Data *pH_Data) {
	if ( *pH_Data > 14.0 ) {
		*pH_Data = 14.0;
	}
	else if ( *pH_Data < 0 ) {
		*pH_Data = 0.0;
	}
}
