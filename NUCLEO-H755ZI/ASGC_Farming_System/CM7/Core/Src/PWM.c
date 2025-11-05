/*-----------------------------------------------------------------------------
 *
 * PWM.c
 *
 * 		Pulse-Width Modulation (PWM) Interface for driving Seed Dispensor
 *      Shutter Servo, as well as Vertical Lifter Servo.
 *
 *  Created on: November 4th, 2025
 *      Authors: Ben, Nate
 * 
 *  NOTES:
 *   - The PWM Minimum Duty is always 0 (0%)
 *   - The PWM Maximum Duty is statically defined from a given PWM_Servo_Config struct, which is defined in PWM.c
 *   - Use DutyValuePCT(SERVO_CONFIG, percent) to get the correct duty value for a given percentage, instead of using defines as in fan_pwm_intf.h
 * 
 * 	 - The Shutter Servo moves from a 0 degree position (closed) to a 90 degree position (open)
 * 		- PWM Pulse correlates to POSITION of shutter
 * 
 *   - The Vertical Lifter Servo is CONTINUOUS
 * 	 	- PWM Pulse correlates to SPEED and DIRECTION of lifter
 * 
 *  TODO:
 * 	   - Update lines 47 and 62 in PWM.h to ensure the correct maximum duty values for each servo are set
 *     - Update lines 91 and 121 to ensure the correct TIM_CHANNEL_x is set
 *     - Review line 112 and ensure this is the correct "no action" duty cycle for vertical lifter servo
 * 	   - Update lines 158 and 181 with correct TIMx->CCRy registers for each servo
 * 
 * 	   - Implement the following function skeletons for PWM.h:
 * 			- PWM_Set_Seed_Dispenser_Servo(SERVO_VALUE)
 * 			- PWM_Set_Vert_Lifter_Servo(SERVO_VALUE)
 * 
 * 	   - Implement the methods described above in PWM.c
 * 
 * 	   - In Scheduler.h, implement the following tasks:
 * 			- PWM_SEED_DISPENSE_TASK
 * 			- PWM_VERTICAL_LIFT_DOWN_TASK
 * 			- PWM_VERTICAL_LIFT_UP_TASK
 * 
 * 	   - In Scheduler.c, initialize the tasks created above (with task interval < 10ms)
 * 	   - In main.c, implement task functions that will be associated with tasks above
 * 			- I believe SeedDispense may already be completed. Review main.c before starting on this.
 *
-----------------------------------------------------------------------------*/

#include "PWM.h"

/*-----------------------------------------------------------------------------
Static Variable Declaration
 ----------------------------------------------------------------------------*/
static uint16_t vlifter_duty_cycle;
static uint16_t shutter_duty_cycle;

/*-----------------------------------------------------------------------------
Servo Configs
-----------------------------------------------------------------------------*/
const PWM_Servo_Config VERTICAL_LIFTER_SERVO_CONFIG = {
    .ServoType = VERTICAL_LIFTER_SERVO,
    .MaxDuty = PWM_VLIFTER_100_PCT_DUTY
};

const PWM_Servo_Config SHUTTER_SERVO_CONFIG = {
    .ServoType = SHUTTER_SERVO,
    .MaxDuty = PWM_SHUTTER_100_PCT_DUTY
};

/*-----------------------------------------------------------------------------
Function Implementations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *
 * 		PWM_ShutterServo_Init()
 *
 * 		Initializes the Shutter Dispenser Servo PWM Interface Module
 *
 ----------------------------------------------------------------------------*/
 bool PWM_ShutterServo_Init(TIM_HandleTypeDef htim) {
	bool ret_val = PWM_INIT_FAIL;
	uint8_t pwm_ret_val;

	// Init duty cycle to 0
	shutter_duty_cycle = 0;

	// If shutter interface is switchboard disabled, exit without doing anything.
	if (SHUTTER_SERVO_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = PWM_INIT_SUCCEED;
		return ret_val;
	}

	// Call HAL function starting the PWM generation
	pwm_ret_val = HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1); // Fix Me!

	if (pwm_ret_val == HAL_OK) {
		ret_val = PWM_INIT_SUCCEED;
	}

	return ret_val;
}

/*-----------------------------------------------------------------------------
 *
 * 		PWM_VerticalServo_Init()
 *
 * 		Initializes the Vertical Servo PWM Interface Module
 *
 ----------------------------------------------------------------------------*/
 bool PWM_VerticalServo_Init(TIM_HandleTypeDef htim) {
	bool ret_val = PWM_INIT_FAIL;
	uint8_t pwm_ret_val;

	// Init duty cycle to midpoint of MaxDuty (midpoint = no action for vertical lifter)
	vlifter_duty_cycle = (VERTICAL_LIFTER_SERVO_CONFIG.MaxDuty / 2);		// Review Me!

	// If vertical lifter interface is switchboard disabled, exit without doing anything.
	if (VLIFTER_SERVO_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
		ret_val = PWM_INIT_SUCCEED;
		return ret_val;
	}

	// Call HAL function starting the PWM generation
	pwm_ret_val = HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);	// Fix Me!

	if (pwm_ret_val == HAL_OK) {
		ret_val = PWM_INIT_SUCCEED;
	}

	return ret_val;
}


/*-----------------------------------------------------------------------------
 *
 * 		PWM_SetDuty(PWM_Servo_Config config, uint16_t duty)
 *
 * 		Sets the PWM Duty Cycle of the specified servo config
 * 		This function will auto-clamp the passed in duty value
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT PWM_SetDuty(PWM_Servo_Config config, uint16_t duty) {

	/// Vertical Lifter Servo Logic ///
	if (config.ServoType == VERTICAL_LIFTER_SERVO) {
		// If vertical lifter interface is switchboard disabled, exit without doing anything.
		if (VLIFTER_SERVO_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
			return (SYS_RESULT) SYS_DEVICE_DISABLED;
		}

		// Clamp duty value
		if (duty > config.MaxDuty) {
			duty = config.MaxDuty;
		} else if (duty < 0) {
			duty = 0;
		}

		// Set corresponding duty cycle timer register
		TIM3->CCR1 = duty;								// FIX ME!
		// Set static duty cycle variable
		vlifter_duty_cycle = duty;

		// Return SYS_SUCCESS since by this point, we're successful....
		return (SYS_RESULT) SYS_SUCCESS;
	}

	/// Shutter Servo Logic ///
	else if (config.ServoType == SHUTTER_SERVO) {
		// If shutter interface is switchboard disabled, exit without doing anything.
		if (SHUTTER_SERVO_PWM_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
			return (SYS_RESULT) SYS_DEVICE_DISABLED;
		}

		// Clamp duty value
		if (duty > config.MaxDuty) {
			duty = config.MaxDuty;
		} else if (duty < 0) {
			duty = 0;
		}

		// Set corresponding duty cycle timer register
		TIM3->CCR1 = duty;								// FIX ME!
		// Set static duty cycle variable
		shutter_duty_cycle = duty;

		// Return SYS_SUCCESS since by this point, we're successful....
		return (SYS_RESULT) SYS_SUCCESS;
	}

	// Invalid servo type was passed
	return (SYS_RESULT) SYS_INVALID;
}

/*-----------------------------------------------------------------------------
 *
 * 		uint16_t PWM_GetDuty(PWM_Servo_Config config)
 *
 * 		Returns the current PWM duty cycle of the specified servo config
 * 		Returns 0 if invalid servo type is passed
 *
 ----------------------------------------------------------------------------*/
uint16_t PWM_GetDuty(PWM_Servo_Config config) {
	if (config.ServoType == VERTICAL_LIFTER_SERVO) {
		return vlifter_duty_cycle;
	}
	else if (config.ServoType == SHUTTER_SERVO) {
		return shutter_duty_cycle;
	}

	// Invalid servo type was passed
	return 0;
}