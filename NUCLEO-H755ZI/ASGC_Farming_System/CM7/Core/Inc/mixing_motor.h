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

#define L298_IN1_PIN                9
#define L298_IN1_PORT               GPIOF

#define L298_IN2_PIN                7
#define L298_IN2_PORT               GPIOF

#define MOTOR_SPEED_0_PCT           0
#define MOTOR_SPEED_10_PCT          3000
#define MOTOR_SPEED_25_PCT          7500
#define MOTOR_SPEED_50_PCT          15000
#define MOTOR_SPEED_75_PCT          22500
#define MOTOR_SPEED_100_PCT         30000

typedef enum {
    MOTOR_STATE_NONE,
    MOTOR_STATE_RUNNING,
    MOTOR_STATE_SPINDOWN,
} Motor_State;

#define MOTOR_SPINDOWN_TIME_MS      1100


/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/

bool mixing_motor_Init(TIM_HandleTypeDef htim);
SYS_RESULT mixing_motor_drive(uint16_t speed);
SYS_RESULT mixing_motor_stop();
SYS_RESULT mixing_motor_apply_brake();
SYS_RESULT mixing_motor_mix_for_time(uint16_t timeout_ms);
void mixing_motor_handle_state();


#endif /* INC_MIXING_MOTOR_H_ */
