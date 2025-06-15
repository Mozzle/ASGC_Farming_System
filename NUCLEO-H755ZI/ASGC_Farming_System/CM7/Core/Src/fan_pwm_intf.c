/*-----------------------------------------------------------------------------
 *
 * FAN_pwm_intf.c
 *
 * 		Pulse-Width Modulation (PWM) Interface for driving air-circulating
 * 		fans.
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "FAN_pwm_intf.h"

/*-----------------------------------------------------------------------------
Static Variable Declaration
 ----------------------------------------------------------------------------*/
static uint16_t s_duty_cycle;

/*-----------------------------------------------------------------------------
Static Function Declaration
 ----------------------------------------------------------------------------*/
static void clamp_duty(uint16_t *duty);

/*-----------------------------------------------------------------------------
 *
 * 		FAN_pwm_intf_Init()
 *
 * 		Initializes the Fan PWM Interface Module
 *
 ----------------------------------------------------------------------------*/

 
 bool FAN_pwm_intf_Init(TIM_HandleTypeDef htim) {
	bool ret_val = FAN_PWM_INTF_INIT_FAIL;
	uint8_t pwm_ret_val;

	s_duty_cycle = 0;

	// If fan interface is switchboard disabled, exit without doing anything.
	if (FAN_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = FAN_PWM_INTF_INIT_SUCCEED;
		return ret_val;
	}

	//Call HAL function starting the PWM generation
	pwm_ret_val = HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);

	if (pwm_ret_val == HAL_OK) {
		ret_val = FAN_PWM_INTF_INIT_SUCCEED;
	}

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		FAN_pwm_intf_set_duty()
 *
 * 		Sets the PWM Duty Cycle of the fans.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT FAN_pwm_intf_set_duty(uint16_t duty) {

	SYS_RESULT ret_val = SYS_INVALID;

	// If fan interface is switchboard disabled, exit without doing anything.
	if (FAN_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = SYS_DEVICE_DISABLED;
		return ret_val;
	}

	clamp_duty(&duty);

	// Set duty cycle timer register
	TIM3->CCR1 = duty;
	// Set static duty cycle variable
	s_duty_cycle = duty;

	ret_val = SYS_SUCCESS;
	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		clamp_duty()
 *
 * 		Helper function to clamp duty cycle within an acceptable range
 * 		(This function may not be necessary to implement)
 *
 ----------------------------------------------------------------------------*/

static void clamp_duty(uint16_t *duty) {

	if (*duty > FAN_PWM_INTF_100_PCT_DUTY) 
		{

		*duty = FAN_PWM_INTF_100_PCT_DUTY;

		}

}

uint16_t FAN_pwm_intf_get_duty_cycle() {
	return s_duty_cycle;
}
