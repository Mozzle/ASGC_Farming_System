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
 *     - Update lines 90 and 120 to ensure the correct TIM_CHANNEL_x is set
 *     - Review line 111 and ensure this is the correct "no action" duty cycle for vertical lifter servo
 * 	   - Update lines 156 and 179 with correct TIMx->CCRy registers for each servo
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

	// Init PWM counter to 0
	pwm_counter = 0;

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

	// Init PWM counter to 0
	pwm_counter = 0;
	VLifter_BobUp = 0;
	VLifter_State = VLIFTER_RESET;

	// Init duty cycle to midpoint of MaxDuty (midpoint = no action for vertical lifter)
	vlifter_duty_cycle = PWM_VLIFTER_50_PCT_DUTY;		// Review Me!

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
 * 		PWMServo_SetDutyForConfig(PWM_Servo_Config config, uint16_t duty)
 *
 * 		Sets the PWM Duty Cycle of the specified servo config
 * 		This function will auto-clamp the passed in duty value
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT PWMServo_SetDutyForConfig(PWM_Servo_Config config, uint16_t duty) {

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
 * 		uint16_t PWMServo_GetDutyForConfig(PWM_Servo_Config config)
 *
 * 		Returns the current PWM duty cycle of the specified servo config
 * 		Returns 0 if invalid servo type is passed
 *
 ----------------------------------------------------------------------------*/
uint16_t PWMServo_GetDutyForConfig(PWM_Servo_Config config) {
	if (config.ServoType == VERTICAL_LIFTER_SERVO) {
		return vlifter_duty_cycle;
	}
	else if (config.ServoType == SHUTTER_SERVO) {
		return shutter_duty_cycle;
	}

	// Invalid servo type was passed
	return 0;
}

/*-----------------------------------------------------------------------------
 *
 * 		SYS_RESULT PWM_VerticalServo_DownTask(void *arg)
 *
 * 		Task function to move the vertical lifter downwards
 * 		Returns SYS_SUCCESS once 'Down' motion is complete
 * 		Returns SYS_DEVICE_DISABLED if task is working
 * 		Returns SYS_FAILURE if Duty Config couldn't be set
 *
 ----------------------------------------------------------------------------*/
SYS_RESULT PWM_VerticalServo_DownTask(void *arg) {
	uint16_t msToGoDown = 3000;
	uint16_t msToGoUp = 500;

	// Bob up motion complete
	if ((VLifter_BobUp == 1) && ((pwm_counter + msToGoUp) >= getTimestamp())) {
		return (SYS_RESULT) SYS_SUCCESS;
	}

	// Begin down motion
	if (pwm_counter == 0) {
		pwm_counter = getTimestamp();
		if (PWMServo_SetDutyForConfig(VERTICAL_LIFTER_SERVO_CONFIG, PWM_VLIFTER_30_PCT_DUTY) != (SYS_SUCCESS)) {
			return (SYS_RESULT) SYS_FAIL;
		}
	}

	// Down motion complete
	if ((pwm_counter + msToGoDown) >= getTimestamp()) {
		pwm_counter = 0;
		VLifter_BobUp = 1;
	}

	// Start slight bob up motion
	if ((VLifter_BobUp == 1) && (pwm_counter == 0)) {
		pwm_counter = getTimestamp();
		if (PWMServo_SetDutyForConfig(VERTICAL_LIFTER_SERVO_CONFIG, PWM_VLIFTER_70_PCT_DUTY) != (SYS_SUCCESS)) {
			return (SYS_RESULT) SYS_FAIL;
		}
	}

	return (SYS_RESULT) SYS_DEVICE_DISABLED;
}

/*-----------------------------------------------------------------------------
 *
 * 		SYS_RESULT PWM_VerticalServo_UpTask(void *arg)
 *
 * 		Task function to move the vertical lifter upwards
 *
 ----------------------------------------------------------------------------*/
SYS_RESULT PWM_VerticalServo_UpTask(void *arg) {
	uint16_t msToGoUp = 2500;

	// Begin up motion
	if (pwm_counter == 0) {
		pwm_counter = getTimestamp();
		if (PWMServo_SetDutyForConfig(VERTICAL_LIFTER_SERVO_CONFIG, PWM_VLIFTER_70_PCT_DUTY) != (SYS_SUCCESS)) {
			return (SYS_RESULT) SYS_FAIL;
		}
	}

	// Up motion complete
	if ((pwm_counter + msToGoUp) >= getTimestamp()) {
		return (SYS_RESULT) SYS_SUCCESS;
	}

	return (SYS_RESULT) SYS_DEVICE_DISABLED;
}



/*-----------------------------------------------------------------------------
 *
 * 		SYS_RESULT PWM_VerticalServo_ResetDutyTask(void *arg)
 *
 * 		Task function to reset duty cycle of vertical servo
 *
 ----------------------------------------------------------------------------*/
SYS_RESULT PWM_VerticalServo_ResetDutyTask(void *arg) {
	pwm_counter = 0;
	VLifter_BobUp = 0;
	if (PWMServo_SetDutyForConfig(VERTICAL_LIFTER_SERVO_CONFIG, PWM_VLIFTER_50_PCT_DUTY) != (SYS_SUCCESS)) {
		return (SYS_RESULT) SYS_FAIL;
	}
	
	return (SYS_RESULT) SYS_SUCCESS;
}
