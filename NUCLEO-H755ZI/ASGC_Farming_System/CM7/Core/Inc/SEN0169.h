/*-----------------------------------------------------------------------------
 *
 * SEN0169.h
 *
 * 		SEN0169-V2 Water pH Sensor
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef INC_SEN0169_H_
#define INC_SEN0169_H_

#include "main.h"
#include "functionality_mngmnt.h"

#define SEN0169_INIT_FAIL 		false
#define SEN0169_INIT_SUCCEED 	true

typedef float SEN0169_EC_Data;

bool SEN0169_Init();

SYS_RESULT SEN0169_Measure(float *data);

#endif /* INC_SEN0169_H_ */
