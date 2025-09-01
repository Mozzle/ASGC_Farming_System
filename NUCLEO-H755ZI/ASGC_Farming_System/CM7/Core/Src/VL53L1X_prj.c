/*-----------------------------------------------------------------------------
 * 	VL53L1X_prj.c
 *
 *  Created on: 8/31/2025
 *
 *  DESCRIPTION: Project specific implementation for the VL53L1X Time-Of-Flight
 *  sensor driver.
 *
-----------------------------------------------------------------------------*/

#include "vl53l1_api.h"
#include "VL53L1X_prj.h"

/*-----------------------------------------------------------------------------
 *
 * 		VL53L1X_prj_Init
 *
 * 		Calls numerous VL53L1X API functions to initialize the VL53L1X device.
 *
 ----------------------------------------------------------------------------*/

void VL53L1X_prj_Init(VL53L1_DEV Dev, I2C_HandleTypeDef *hi2c) {
	Dev->I2cHandle = hi2c;
	Dev->I2cDevAddr = VL53L1X_I2C_ADDRESS;
	

	VL53L1_WaitDeviceBooted( Dev );
	VL53L1_DataInit( Dev );
	VL53L1_StaticInit( Dev );
	VL53L1_SetDistanceMode( Dev, VL53L1_DISTANCEMODE_LONG );
	VL53L1_SetMeasurementTimingBudgetMicroSeconds( Dev, 50000 );
	VL53L1_SetInterMeasurementPeriodMilliSeconds( Dev, 500 );
	VL53L1_StartMeasurement( Dev );
}


/*-----------------------------------------------------------------------------
 *
 * 		VL53L1X_GetRangingMeasurementData
 *
 * 		Calls numerous VL53L1X API functions to get the latest ranging data
 *
 ----------------------------------------------------------------------------*/

void VL53L1X_GetRangingMeasurementData(VL53L1_DEV Dev, VL53L1_RangingMeasurementData_t *pRangingMeasurementData) {

	VL53L1_WaitMeasurementDataReady( Dev );
    VL53L1_GetRangingMeasurementData( Dev, pRangingMeasurementData );
    VL53L1_ClearInterruptAndStartMeasurement( Dev );

	}