/*-----------------------------------------------------------------------------
 * 	VL53L1X_prj.h
 *
 *  Created on: 8/31/2025
 *
 *  DESCRIPTION: Project specific implementations for the VL53L1X Time-Of-Flight
 *  sensor driver.
 *
-----------------------------------------------------------------------------*/

#ifndef INC_VL53L1X_PRJ_H_
#define INC_VL53L1X_PRJ_H_

/*-----------------------------------------------------------------------------
Defines
-----------------------------------------------------------------------------*/
#define VL53L1X_I2C_ADDRESS 0x52

/*-----------------------------------------------------------------------------
Function Declarations
-----------------------------------------------------------------------------*/
void VL53L1X_prj_Init(VL53L1_DEV Dev, I2C_HandleTypeDef *hi2c);

void VL53L1X_GetRangingMeasurementData(VL53L1_DEV Dev, VL53L1_RangingMeasurementData_t *pRangingMeasurementData);

#endif /* INC_VL53L1X_PRJ_H_ */