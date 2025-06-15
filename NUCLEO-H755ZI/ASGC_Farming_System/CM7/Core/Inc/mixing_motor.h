/*-----------------------------------------------------------------------------
 *
 * mixing_motor.h
 *
 * 		Mixing Motor Control Driver.
 *
 * 		GPIO interaction with a DROK L298 Motor Driver
 *
 *  Created on: June 1, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef INC_MIXING_MOTOR_H_
#define INC_MIXING_MOTOR_H_

#include "main.h"

/*-----------------------------------------------------------------------------
DEFINES
-----------------------------------------------------------------------------*/

#define MIXING_MOTOR_STOP		 	0
#define MIXING_MOTOR_25_PCT			63
#define MIXING_MOTOR_50_PCT			127
#define MIXING_MOTOR_75_PCT			191
#define MIXING_MOTOR_100_PCT		255

#define MIXING_MOTOR_INIT_SUCCEED	true
#define MIXING_MOTOR_INIT_FAIL		false

/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/

bool mixing_motor_Init();
SYS_RESULT mixing_motor_drive(uint8_t speed);
SYS_RESULT mixing_motor_stop();


#endif /* INC_MIXING_MOTOR_H_ */
