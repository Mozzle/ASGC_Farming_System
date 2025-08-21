/*-----------------------------------------------------------------------------
 *
 * SEN0169.h
 *
 * 		SEN0169-V2 Water-Nutrient solution pH Sensor Driver
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef INC_SEN0169_H_
#define INC_SEN0169_H_

#include "main.h"
#include "functionality_mngmnt.h"

#define SEN0169_INIT_FAIL 			false
#define SEN0169_INIT_SUCCEED 		true

#define SEN0169_ANALOG_BANK 		GPIOA
#define SEN0169_ANALOG_PIN 			GPIO_PIN_6

#define SEN0169_NUM_MEASUREMENTS	50

/*-------------------------------------------------------------------------
EXPERIMENTALLY DETERMINED VALUE
THIS CONVERSION FACTOR IS DETERMINED BY:
(pH Meter voltage relationship * reference voltage) / (max int value for ADC
 precision)
-------------------------------------------------------------------------*/
#define SEN0169_CONVERSION_FACTOR	(double)-0.0045126855
#define SEN0169_INTERCEPT_OFFSET	15.498


typedef float SEN0169_pH_Data;

struct pH_Moving_Avg {
	SEN0169_pH_Data pH_Data;
	uint64_t timestamp;
};

#define pH_MOVING_AVG_ENTRIES		4
#define pH_MOVING_AVG_TIMEOUT_MS	5000

/*-------------------------------------------------------------------------
FUNCTION DECLARATIONS
-------------------------------------------------------------------------*/
bool SEN0169_Init();

SYS_RESULT SEN0169_Measure(SEN0169_pH_Data *pH_Data);

SYS_RESULT SEN0169_Measure_SMA(SEN0169_pH_Data * pH_Data);


#endif /* INC_SEN0169_H_ */
