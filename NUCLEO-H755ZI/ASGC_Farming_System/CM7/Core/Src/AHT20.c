/*-----------------------------------------------------------------------------
 * 	AHT20.h
 *
 *  Created on:
 *
 *  DESCRIPTION: AHT20 Temperature and Humidity Sensor Driver
 *
-----------------------------------------------------------------------------*/

#include "AHT20.h"


bool AHT20_Initialized = false;

/*-----------------------------------------------------------------------------
 *
 * 		AHT20_Init
 *
 * 		Initializes the AHT20 Module.
 * 		NOTE: At least 40ms must have transpired between power on and this
 * 		function being called
 *
 ----------------------------------------------------------------------------*/

bool AHT20_Init(I2C_HandleTypeDef *hi2c, uint32_t timeout) {
	HAL_StatusTypeDef ret;	//I2C Transmit Status
	uint8_t outMsg[3] = {AHT20_INITIALIZE_REG, 0x08, 0x00}; // Initialization Message

	// If AHT20 is not enabled, dont try sending data to it over I2C
	if (AHT20_ENABLED == SYS_FEATURE_DISABLED) {
		AHT20_Initialized = true;
		return AHT20_Initialized;
	}

	// Send the Initialization Message
	ret = HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDR_WRITE, outMsg, 3, timeout);

	if (ret != HAL_OK) {
		AHT20_Initialized = false;
	}
	else {
		AHT20_Initialized = true;
	}

	return AHT20_Initialized;
}


/*-----------------------------------------------------------------------------
 *
 * 		AHT20_Get_Data
 *
 * 		Returns a struct containing the temperature and relative humidity data
 * 		from the AHT20 module. NOTE: This is currently a blocking function that
 * 		will block execution for about 85ms
 *
 ----------------------------------------------------------------------------*/

struct AHT20_Data AHT20_Get_Data(I2C_HandleTypeDef *hi2c) {
	HAL_StatusTypeDef ret;	// I2C Receipt Status
	uint32_t timeout;		// I2C Timeout
	uint8_t inMsg[7];		// Message to be received
	uint8_t outMsg[3] = {AHT20_MEASURE_TRIGGER_REG, 0x33, 0x00}; // Take Measurement Command
	uint32_t rawData;
	struct AHT20_Data newData;

	timeout = 2000;
	newData.temperature = 	0x0;
	newData.humidity = 		0x0;
	newData.validity = 		SYS_INVALID;

	// If AHT20 is disabled, set invalid values and return
	if (AHT20_ENABLED == SYS_FEATURE_DISABLED) {
		newData.temperature = 	AHT_20_TEMPERATURE_INVALID;
		newData.humidity = 		AHT_20_HUMIDITY_INVALID;
		newData.validity = 		SYS_DEVICE_DISABLED;
		return newData;
	}

	// If AHT20 is not initialized, set valid byte and return
	if (!AHT20_Initialized) {
		newData.validity = SYS_NOT_INITIALIZED;
		return newData;
	}

	// Send Take Measurement Command
	ret = HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDR_WRITE, outMsg, 3, timeout);

	// If failed to send, set valid byte
	if (ret != HAL_OK) {
		newData.validity = SYS_MEASUREMENT_SEND_FAIL;
		return newData;
	}

	// HAL Delay for now, will maybe add scheduling at some point
	HAL_Delay(80);

	// Fetch Measurement
	ret = HAL_I2C_Master_Receive(hi2c, AHT20_I2C_ADDR_READ, inMsg, 7, timeout);

	// If failed to receive response, set valid byte and return
	if (ret != HAL_OK) {
		newData.validity = SYS_MEASUREMENT_GET_FAIL;
		return newData;
	}

	// If Data is NOT updated
	if ((inMsg[0] & 0b10000000) == 0b10000000) {
		newData.validity = SYS_INVALID;
		return newData;
	}

	// If Data is NOT calibrated
	if ((inMsg[0] & 0b00001000) != 0b00001000) {
		newData.validity = SYS_NOT_INITIALIZED;
		return newData;
	}

	// Convert and store temp
	rawData = (inMsg[3] & 0b00001111) << 16;
	rawData |= (inMsg[4] << 8);
	rawData |= (inMsg[5]);
	newData.temperature = ((float)rawData * AHT20_TEMP_CONVERSION_FACTOR) - 50.0;

	//Convert and store humidity
	rawData = (inMsg[1] << 12);
	rawData |= (inMsg[2] << 4);
	rawData |= (inMsg[3] >> 4);

	newData.humidity = (float)rawData / (float)AHT20_HUMID_CONVERSION_FACTOR;

	newData.validity = SYS_SUCCESS;

	return newData;
}
