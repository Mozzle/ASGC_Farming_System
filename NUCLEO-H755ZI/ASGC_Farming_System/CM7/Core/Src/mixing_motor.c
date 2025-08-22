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

#include "mixing_motor.h"
#include "timer.h"

/*-----------------------------------------------------------------------------
Static Variable Declaration
 ----------------------------------------------------------------------------*/
static uint16_t s_duty_cycle;
static Motor_State motor_state;
static uint64_t mix_end_timestamp;

/*-----------------------------------------------------------------------------
Static Function Declaration
 ----------------------------------------------------------------------------*/
static void clamp_speed(uint16_t *speed);


/*-----------------------------------------------------------------------------
 *
 * 		mixing_motor_Init()
 *
 * 		Initializes the mixing motor driver
 *
 ----------------------------------------------------------------------------*/

bool mixing_motor_Init(TIM_HandleTypeDef htim) {

	bool ret_val = MIXING_MOTOR_INIT_FAIL;
	HAL_StatusTypeDef pwm_ret_val;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = MIXING_MOTOR_INIT_SUCCEED;
		return ret_val;
	}

	//Call HAL function starting the PWM generation
	pwm_ret_val = HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);

	// Set L298 to Brake mode
	HAL_GPIO_WritePin(L298_IN1_PORT, L298_IN1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L298_IN2_PORT, L298_IN2_PIN, GPIO_PIN_RESET);

	motor_state = MOTOR_STATE_NONE;
	mix_end_timestamp = 0;

	if (pwm_ret_val == HAL_OK) {
		ret_val = MIXING_MOTOR_INIT_SUCCEED;
	}

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		mixing_motor_drive()
 *
 * 		Drives the mixing motor at the given speed
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT mixing_motor_drive(uint16_t speed) {

	SYS_RESULT ret_val = SYS_INVALID;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	// Set L298 to forward mode
	HAL_GPIO_WritePin(L298_IN1_PORT, L298_IN1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(L298_IN2_PORT, L298_IN2_PIN, GPIO_PIN_RESET);

	clamp_speed(&speed);

	// Set PWM duty cycle (Motor speed command)
	TIM4->CCR1 = speed;
	s_duty_cycle = speed;

	ret_val = SYS_SUCCESS;
	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		mixing_motor_stop()
 *
 * 		Sets the pwm duty cycle (speed command) being sent to the motor
 * 		controller down to 0. NOTE that this function DOES NOT apply the brakes
 * 		to the motor, and the motor will idle down to a stop unless 
 * 		mixing_motor_apply_brake() is called.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT mixing_motor_stop() {

	SYS_RESULT ret_val = SYS_SUCCESS;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	// Set PWM duty cycle (Motor speed command)
	TIM4->CCR1 = MOTOR_SPEED_0_PCT;
	s_duty_cycle = MOTOR_SPEED_0_PCT;

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		mixing_motor_apply_brake()
 *
 * 		Applies the brakes to the motor controller.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT mixing_motor_apply_brake() {

	SYS_RESULT ret_val = SYS_SUCCESS;

	// If mixing motor is switchboard disabled, exit without doing anything.
	if (MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	// Set L298 to forward mode
	HAL_GPIO_WritePin(L298_IN1_PORT, L298_IN1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L298_IN2_PORT, L298_IN2_PIN, GPIO_PIN_RESET);

}

/*-----------------------------------------------------------------------------
 *
 * 		clamp_speed()
 *
 * 		Helper function to clamp duty cycle within an acceptable range
 *
 ----------------------------------------------------------------------------*/

static void clamp_speed(uint16_t *speed) {

	if (*speed > MOTOR_SPEED_100_PCT) 
		{

		*speed = MOTOR_SPEED_100_PCT;

		}

}

/*-----------------------------------------------------------------------------
 *
 * 		mixing_motor_mix_for_time()
 *
 * 		Uses the state machine handled in mixing_motor_handle_state() to 
 * 		drive the mixing motor for a given amount of time. NOTE: 
 * 		mixing_motor_handle_state() must be in the main loop of the program.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT mixing_motor_mix_for_time(uint16_t timeout_ms) {

	if (motor_state != MOTOR_STATE_NONE || MIXING_MOTOR_ENABLED == SYS_FEATURE_DISABLED) {
		return SYS_DEVICE_DISABLED;
	}

	// Set motor to 75% speed
	SYS_RESULT ret_val = mixing_motor_drive(MOTOR_SPEED_75_PCT);

	// Set motor state machine variables
	mix_end_timestamp = ((uint64_t)(getTimestamp() + timeout_ms));
	motor_state = MOTOR_STATE_RUNNING;

	return ret_val;

}

/*-----------------------------------------------------------------------------
 *
 * 		mixing_motor_handle_state()
 *
 * 		Handles state of mixing motor. Note that the purpose of the 'spindown'
 * 		state is to allow the motor to slow down to a stop before applying the
 * 		motor brake, to prevent a current spike.
 *
 ----------------------------------------------------------------------------*/

void mixing_motor_handle_state() {

	switch (motor_state) {

	case MOTOR_STATE_NONE:
		return;

	case MOTOR_STATE_RUNNING:
		if ( getTimestamp() > mix_end_timestamp ) {
			mixing_motor_stop();
			motor_state = MOTOR_STATE_SPINDOWN;
		}
		break;

	case MOTOR_STATE_SPINDOWN:
		if ( getTimestamp() > ( mix_end_timestamp + MOTOR_SPINDOWN_TIME_MS ) ) {
			mixing_motor_apply_brake();
			motor_state = MOTOR_STATE_NONE;
		}
		break;

	}
}
