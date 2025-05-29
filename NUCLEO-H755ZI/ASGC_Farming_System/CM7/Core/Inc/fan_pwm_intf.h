/*-----------------------------------------------------------------------------
 *
 * fan_pwm_intf.h
 *
 * 		Fan Pulse-Width Modulation (PWM) Interface.
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

#define FAN_PWM_INTF_INIT_FAIL		false
#define FAN_PWM_INTF_INIT_SUCCEED 	true

bool fan_pwm_intf_Init();

SYS_RESULT fan_pwm_intf_set_duty(uint8_t duty);

void clamp_duty();


#endif /* FAN_PWM_INTF_H */
