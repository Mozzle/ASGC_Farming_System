#include "AHT20.h"

bool AHT20_Initialized = false;

/*
 * Initializes the AHT20 Module.
 * NOTE: At least 40ms must have transpired between power on
 * and this function being called
 */
bool AHT20_Init(I2C_HandleTypeDef *hi2c, uint32_t timeout) {
	HAL_StatusTypeDef ret;	//I2C Transmit Status
	uint8_t outMsg[3] = {AHT20_INITIALIZE_REG, 0x08, 0x00}; // Initialization Message

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


/*
 * Returns a struct containing the temperature and relative humidity data from
 * the AHT20 module. NOTE: This is currently a blocking function that will block
 * execution for about 85ms
 */
struct AHT20_Data AHT20_Get_Data(I2C_HandleTypeDef *hi2c, uint32_t timeout) {
	HAL_StatusTypeDef ret;	// I2C Receipt Status
	uint8_t inMsg[7];		// Message to be received
	uint8_t outMsg[3] = {AHT20_MEASURE_TRIGGER_REG, 0x33, 0x00}; // Take Measurement Command
	uint32_t rawData;

	struct AHT20_Data newData;
	newData.temperature = 0x0;
	newData.humidity = 0x0;
	newData.valid = AHT20_INVALID;

	if (!AHT20_Initialized) {
		newData.valid = AHT20_NOT_INITIALIZED;
		return newData;
	}

	// Send Take Measurement Command
	ret = HAL_I2C_Master_Transmit(hi2c, AHT20_I2C_ADDR_WRITE, outMsg, 3, timeout);

	if (ret != HAL_OK) {
		newData.valid = AHT20_MEASUREMENT_SEND_FAIL;
		return newData;
	}

	HAL_Delay(80);

	// Fetch Measurement
	ret = HAL_I2C_Master_Receive(hi2c, AHT20_I2C_ADDR_READ, inMsg, 7, timeout);
	if (ret != HAL_OK) {
		newData.valid = AHT20_MEASUREMENT_GET_FAIL;
		return newData;
	}

	// If Data is NOT updated
	if ((inMsg[0] & 0b10000000) == 0b10000000) {
		newData.valid = AHT20_INVALID;
		return newData;
	}

	// If Data is NOT calibrated
	if ((inMsg[0] & 0b00001000) != 0b00001000) {
		newData.valid = AHT20_NOT_INITIALIZED;
		return newData;
	}

	rawData = (inMsg[3] & 0b00001111) << 16;
	rawData |= (inMsg[4] << 8);
	rawData |= (inMsg[5]);
	newData.temperature = ((float)rawData * AHT20_TEMP_CONVERSION_FACTOR) - 50.0;

	rawData = (inMsg[1] << 12);
	rawData |= (inMsg[2] << 4);
	rawData |= (inMsg[3] >> 4);

	newData.humidity = (float)rawData / (float)AHT20_HUMID_CONVERSION_FACTOR;

	newData.valid = AHT20_VALID;

	return newData;
}
