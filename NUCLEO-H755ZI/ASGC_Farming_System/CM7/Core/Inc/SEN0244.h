/*-----------------------------------------------------------------------------
 *
 * SEN0244.h
 *
 * 		SEN0244 Water EC Sensor
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef INC_SEN0244_H_
#define INC_SEN0244_H_

#include "main.h"
#include "functionality_mngmnt.h"

#define SEN0244_INIT_FAIL 		false
#define SEN0244_INIT_SUCCEED 	true

typedef float SEN0244_EC_Data;

bool SEN0244_Init();

SYS_RESULT SEN0244_Measure(float *data);



#endif /* INC_SEN0244_H_ */
