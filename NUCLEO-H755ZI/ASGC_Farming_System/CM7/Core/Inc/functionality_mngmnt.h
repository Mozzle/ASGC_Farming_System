/*
 * functionality_mngmnt.h
 *
 *  Created on: May 18, 2025
 *      Author: Dylan
 *
 *  DESCRIPTION: The purpose of this file is to allow for development in an incomplete system
 *  E.g.: To be able to run and debug aspects of the system without having every single sensor
 *  and interface connected to your microcontroller.
 *
 *  We will use preprocessor defines to create 'mocked' versions of sensor/device functions when
 *  those devices are not actually physically present in the system. These mocked functions will
 *  return known invalid values that are defined in this header file
 */

#ifndef INC_FUNCTIONALITY_MNGMNT_H_
#define INC_FUNCTIONALITY_MNGMNT_H_

typedef uint8_t SYS_FEATURE_ENABLE_u8;
enum {
	SYS_FEATURE_DISABLED = 0,
	SYS_FEATURE_ENABLED
};

// AHT 20 Temp & Humidity Sensor
#define AHT_20_TEMPERATURE_INVALID	-273.15f	// Absolute zero
#define AHT_20_HUMIDITY_INVALID		-1.0f

// AS7341 Light Sensor
#define AS7341_LIGHT_INVALID		-1.0f

//DFRobot SEN0244 EC Sensor
#define SEN0244_EC_INVALID			-1.0f

//DFRobot SEN0169-V2 pH Sensor
#define SEN0169_PH_INVALID			-1.0f

//VL52L1X ToF Sensor
#define VL53L1X_DIST_INVALID		-1.0f




#endif /* INC_FUNCTIONALITY_MNGMNT_H_ */
