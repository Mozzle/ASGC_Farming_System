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
#include "FS_math.h"

// ADC hanldler declared in main.c
extern ADC_HandleTypeDef hadc1;
bool SEN0169_ADC_On = false;

static struct pH_Moving_Avg pH_History[pH_MOVING_AVG_ENTRIES];
static uint8_t pH_HistoryIndex = 0;

static void clamp_pH(SEN0169_pH_Data *pH_Data);


bool SEN0169_Init() {

	if (SEN0169_ENABLED == SYS_FEATURE_DISABLED) {
			return SEN0169_INIT_SUCCEED;
	}

	/*-------------------------------------------------------------------------
	Run ADC Calibration
	-------------------------------------------------------------------------*/
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK ) {
		return SEN0169_INIT_FAIL;
	}

	/*-------------------------------------------------------------------------
	Start the ADC. Exit if the ADC fails to start
	-------------------------------------------------------------------------*/
	if ( HAL_ADC_Start(&hadc1) != HAL_OK ) {
		return SEN0169_INIT_FAIL;
	}
	SEN0169_ADC_On = true;

	return SEN0169_INIT_SUCCEED;

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
	uint32_t measurement[SEN0169_NUM_MEASUREMENTS] = {0};
	double medianMeasurement = 0;
	uint8_t i;

	SYS_RESULT ret_val = SYS_INVALID;

	/*-------------------------------------------------------------------------
	If driver is switchboard disabled, exit without doing anything.
	-------------------------------------------------------------------------*/
	if (SEN0169_ENABLED == SYS_FEATURE_DISABLED || SEN0169_ADC_On == false) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	/*-------------------------------------------------------------------------
	Take SEN0169_NUM_MEASUREMENTS measurements from ADC
	-------------------------------------------------------------------------*/
	for( i = 0; i < SEN0169_NUM_MEASUREMENTS; i++ ) {
		HAL_ADC_PollForConversion(&hadc1, 5);
		measurement[i] = HAL_ADC_GetValue(&hadc1);
	}

	/*-------------------------------------------------------------------------
	Calculate the pH:
	1) take median of measurements.
	2) voltage = (median measurement * 3.3) / 4096
	3) pH = voltage * -5.6012 + 15.498
	-------------------------------------------------------------------------*/
	medianMeasurement = getMedian_u32(measurement, SEN0169_NUM_MEASUREMENTS);

	*pH_Data = ( medianMeasurement * SEN0169_CONVERSION_FACTOR ) + SEN0169_INTERCEPT_OFFSET;

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

//SYS_RESULT SEN0169_Measure_SMA(SEN0169_pH_Data * pH_Data) {
//	/*-------------------------------------------------------------------------
//	Local Variables
//	-------------------------------------------------------------------------*/
//	bool SMA_isCurrent = false;
//	SEN0169_pH_Data * tmpData;
//	SYS_RESULT ret_val;
//	uint8_t i = 0;
//	uint64_t curTime;
//	uint64_t timeMinusTimeout;
//	float sum = 0;
//
//	/*-------------------------------------------------------------------------
//	While not all of the data for the simple moving average is not considered
//	'current':
//	-------------------------------------------------------------------------*/
//	while (SMA_isCurrent == false) {
//		/*---------------------------------------------------------------------
//		Take a measurement
//		---------------------------------------------------------------------*/
//		ret_val = SEN0169_Measure(tmpData);
//
//		if (ret_val != SYS_SUCCESS) {
//			return SYS_FAIL;
//		}
//
//		/*---------------------------------------------------------------------
//		Add measurement to circular buffer
//		---------------------------------------------------------------------*/
//		pH_History[pH_HistoryIndex].pH_Data = *tmpData;
//		curTime = getTimestamp();
//		pH_History[pH_HistoryIndex].timestamp = curTime;
//
//		pH_HistoryIndex = ( ( pH_HistoryIndex + 1 ) % pH_MOVING_AVG_ENTRIES);
//
//		/*---------------------------------------------------------------------
//		Determine the timestamp of pH_MOVING_AVG_TIMEOUT_MS ago.
//		---------------------------------------------------------------------*/
//		SMA_isCurrent = true;
//		if (curTime < pH_MOVING_AVG_TIMEOUT_MS ) {
//			timeMinusTimeout = 0;
//		}
//		else {
//			timeMinusTimeout = curTime - pH_MOVING_AVG_TIMEOUT_MS;
//		}
//
//		/*---------------------------------------------------------------------
//		Determine if every measurement in the circular buffer happened in the
//		last pH_MOVING_AVG_TIMEOUT_MS milliseconds. If not, stay in while loop
//		until all measurements are 'current'.
//		---------------------------------------------------------------------*/
//		for ( i = 0; i < pH_MOVING_AVG_ENTRIES; i++ ) {
//
//			if ( ( timeMinusTimeout ) > pH_History[i].timestamp ) {
//				SMA_isCurrent = false;
//			}
//		}
//
//	}
//
//	/*-------------------------------------------------------------------------
//	Get average pH of all values in buffer and return that average.
//	-------------------------------------------------------------------------*/
//	for ( i = 0; i < pH_MOVING_AVG_ENTRIES; i++ ) {
//		sum += pH_History[i].pH_Data;
//	}
//
//	*pH_Data = ( sum / (float) pH_MOVING_AVG_ENTRIES );
//
//	clamp_pH(pH_Data);
//
//	return SYS_SUCCESS;
//}

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

/*-----------------------------------------------------------------------------
 *
 * 		SEN0169_Stop_ADC
 *
 * 		Stops the ADC from taking any more measurements.
 *
 ----------------------------------------------------------------------------*/
void SEN0169_Stop_ADC() {
	HAL_ADC_Stop(&hadc1);
	SEN0169_ADC_On = false;
}
