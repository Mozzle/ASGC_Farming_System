/*-----------------------------------------------------------------------------
 *
 * FAN_pwm_intf.h
 *
 * 		Pulse-Width Modulation (PWM) Interface for driving air-circulating
 * 		fans.
 *
 *  Created on: May 28, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef FAN_PWM_INTF_H
#define FAN_PWM_INTF_H

#include "main.h"
#include "functionality_mngmnt.h"
#include <stdbool.h>
#include <stdint.h>

// Fan PWM Intf Defines
#define FAN_PWM_INTF_INIT_FAIL		false
#define FAN_PWM_INTF_INIT_SUCCEED 	true

#define FAN_PWM_INTF_0_PCT_DUTY		(uint16_t) 0
#define FAN_PWM_INTF_10_PCT_DUTY	(uint16_t) 1200
#define FAN_PWM_INTF_20_PCT_DUTY	(uint16_t) 2400
#define FAN_PWM_INTF_25_PCT_DUTY	(uint16_t) 3000
#define FAN_PWM_INTF_30_PCT_DUTY	(uint16_t) 3600
#define FAN_PWM_INTF_40_PCT_DUTY	(uint16_t) 4800
#define FAN_PWM_INTF_50_PCT_DUTY	(uint16_t) 6000
#define FAN_PWM_INTF_60_PCT_DUTY	(uint16_t) 7200
#define FAN_PWM_INTF_70_PCT_DUTY	(uint16_t) 8400
#define FAN_PWM_INTF_75_PCT_DUTY	(uint16_t) 9000
#define FAN_PWM_INTF_80_PCT_DUTY	(uint16_t) 9600
#define FAN_PWM_INTF_90_PCT_DUTY	(uint16_t) 10800
#define FAN_PWM_INTF_100_PCT_DUTY	(uint16_t) 12000

/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/
bool FAN_pwm_intf_Init();
SYS_RESULT FAN_pwm_intf_set_duty(uint16_t duty);
uint16_t FAN_pwm_intf_get_duty_cycle();


#endif /* FAN_PWM_INTF_H */
