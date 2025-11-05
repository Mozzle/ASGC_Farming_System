/*-----------------------------------------------------------------------------
 *
 * PWM.h
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
 *  TODO: Located in PWM.c. Delete this comment after all tasks are completed for Issue #27.
 *
-----------------------------------------------------------------------------*/

#ifndef PWM_H
#define PWM_H

#include "main.h"
#include "functionality_mngmnt.h"
#include <stdbool.h>
#include <stdint.h>

// PWM Success/Failure Defines
#define PWM_INIT_FAIL		        false
#define PWM_INIT_SUCCEED 	        true

// Servo Type Defines
#define VERTICAL_LIFTER_SERVO       (uint8_t) 0
#define SHUTTER_SERVO               (uint8_t) 1

// Servo Value Defines
#define SERVO_OPEN                  (uint8_t) 0
#define SERVO_CLOSED                (uint8_t) 1

// Vertical Lifter Servo PWM Percent Defines
#define PWM_VLIFTER_100_PCT_DUTY	(uint16_t) 12000 // Fix me!
#define PWM_VLIFTER_90_PCT_DUTY	    (uint16_t) (0.90) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_80_PCT_DUTY	    (uint16_t) (0.80) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_75_PCT_DUTY	    (uint16_t) (0.75) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_70_PCT_DUTY	    (uint16_t) (0.70) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_60_PCT_DUTY	    (uint16_t) (0.60) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_50_PCT_DUTY	    (uint16_t) (0.50) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_40_PCT_DUTY	    (uint16_t) (0.40) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_30_PCT_DUTY	    (uint16_t) (0.30) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_25_PCT_DUTY	    (uint16_t) (0.25) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_20_PCT_DUTY	    (uint16_t) (0.20) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_10_PCT_DUTY	    (uint16_t) (0.10) * PWM_VLIFTER_100_PCT_DUTY
#define PWM_VLIFTER_0_PCT_DUTY	    (uint16_t) 0

// Shutter Servo PWM Percent Defines
#define PWM_SHUTTER_100_PCT_DUTY	(uint16_t) 12000 // Fix me!
#define PWM_SHUTTER_90_PCT_DUTY	    (uint16_t) (0.90) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_80_PCT_DUTY	    (uint16_t) (0.80) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_75_PCT_DUTY	    (uint16_t) (0.75) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_70_PCT_DUTY	    (uint16_t) (0.70) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_60_PCT_DUTY	    (uint16_t) (0.60) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_50_PCT_DUTY	    (uint16_t) (0.50) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_40_PCT_DUTY	    (uint16_t) (0.40) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_30_PCT_DUTY	    (uint16_t) (0.30) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_25_PCT_DUTY	    (uint16_t) (0.25) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_20_PCT_DUTY	    (uint16_t) (0.20) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_10_PCT_DUTY	    (uint16_t) (0.10) * PWM_SHUTTER_100_PCT_DUTY
#define PWM_SHUTTER_0_PCT_DUTY	    (uint16_t) 0

/*-----------------------------------------------------------------------------
STRUCTS FOR SERVO CONFIGURATION
-----------------------------------------------------------------------------*/
typedef struct {
    uint8_t ServoType;  // e.g., VERTICAL_LIFTER_SERVO (0) or SHUTTER_SERVO (1). If neither of these, configuration is invalid and function will fail.
    uint16_t MaxDuty;   // Maximum Duty Cycle value for the servo (i.e. 12000 for 100%)
} PWM_Servo_Config;

/*-----------------------------------------------------------------------------
SERVO CONFIGS -- defined in PWM.c
-----------------------------------------------------------------------------*/
extern const PWM_Servo_Config VERTICAL_LIFTER_SERVO_CONFIG;
extern const PWM_Servo_Config SHUTTER_SERVO_CONFIG;

/*-----------------------------------------------------------------------------
FUNCTION PROTOTYPES
-----------------------------------------------------------------------------*/
bool PWM_VerticalServo_Init(TIM_HandleTypeDef htim);
bool PWM_ShutterServo_Init(TIM_HandleTypeDef htim);
SYS_RESULT PWMServo_SetDutyForConfig(PWM_Servo_Config config, uint16_t duty);
uint16_t PWMServo_GetDutyForConfig(PWM_Servo_Config config);

#endif /* PWM_H */
