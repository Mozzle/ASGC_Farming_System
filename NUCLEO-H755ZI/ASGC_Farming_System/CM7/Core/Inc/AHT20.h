/*-----------------------------------------------------------------------------
 * 	AHT20.h
 *
 *  Created on:
 *
 *  DESCRIPTION: AHT20 Temperature and Humidity Sensor Driver
 *
-----------------------------------------------------------------------------*/

#ifndef AHT20_H
#define AHT20_H

#include "main.h"
#include "functionality_mngmnt.h"
#include <stdbool.h>

// I2C Addresses and Registers
#define AHT20_I2C_ADDR_WRITE			0x70
#define AHT20_I2C_ADDR_READ				0x71
#define AHT20_INITIALIZE_REG			0xBE
#define AHT20_MEASURE_TRIGGER_REG		0xAC
#define AHT20_SOFT_RESET_REG			0xBA

#define AHT20_TEMP_CONVERSION_FACTOR	0.00019073486
#define AHT20_HUMID_CONVERSION_FACTOR	1048576.0f

// Sensor Data Struct
struct AHT20_Data {
	float temperature;		// In Degrees Celsius
	float humidity;			// Relative Humidity as a percent
	SYS_RESULT validity;
};

// Function Declarations
bool AHT20_Init(I2C_HandleTypeDef *hi2c, uint32_t timeout);
struct AHT20_Data AHT20_Get_Data(I2C_HandleTypeDef *hi2c, uint32_t timeout);


#endif
