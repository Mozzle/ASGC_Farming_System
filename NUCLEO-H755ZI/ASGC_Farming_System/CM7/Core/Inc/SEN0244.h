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

/*-----------------------------------------------------------------------------
Defines and includes
-----------------------------------------------------------------------------*/
#include "main.h"
#include "functionality_mngmnt.h"

#define SEN0244_INIT_FAIL 						false
#define SEN0244_INIT_SUCCEED 					true

#define SEN0244_ANALOG_BANK 					GPIOC
#define SEN0244_ANALOG_PIN 						GPIO_PIN_4

#define SEN0244_NUM_MEASUREMENTS				40

// NOTE: THIS VALUE MUST BE CHANGED ANY TIME THE PROBE IS CHANGED OR THE LENGTH
// OF THE WIRES BETWEEN THE PROBE AND THE ADC IS CHANGED
#define SEN0244_PROBE_CALIBRATION_OFFSET		-41.417

/*-----------------------------------------------------------------------------
typedefs
-----------------------------------------------------------------------------*/
typedef float SEN0244_EC_Data;

/*-----------------------------------------------------------------------------
Function Declarations
-----------------------------------------------------------------------------*/
bool SEN0244_Init();
SYS_RESULT SEN0244_Measure(double *tdsData, float tempData);
void SEN0244_Stop_ADC();


#endif /* INC_SEN0244_H_ */
