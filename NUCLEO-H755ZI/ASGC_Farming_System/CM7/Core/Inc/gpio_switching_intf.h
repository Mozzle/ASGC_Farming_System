/*-----------------------------------------------------------------------------
 *
 *	gpio_switching_intf.h
 *
 *		Created on: May 29, 2025
 *
 *		Interface to control the MOSFET and Solid State Relay switching of the
 *		system's pumps and valves
 *
-----------------------------------------------------------------------------*/
#ifndef INC_GPIO_SWITCHING_INTF_H_
#define INC_GPIO_SWITCHING_INTF_H_

#include "main.h"
#include "functionality_mngmnt.h"

#define VALVE_OPEN				true
#define VALVE_CLOSED			false
#define PUMP_ON					true
#define PUMP_OFF				false

#define CIRCULATING_PUMP_PORT	GPIOA
#define CIRCULATING_PUMP_PIN	GPIO_PIN_0

#define DRAINAGE_PUMP_PORT		GPIOA
#define DRAINAGE_PUMP_PIN		GPIO_PIN_0

#define FILL_VALVE_PORT			GPIOA
#define FILL_VALVE_PIN			GPIO_PIN_0

#define NUTRIENT_SOLN_A_PORT	GPIOA
#define NUTRIENT_SOLN_A_PIN		GPIO_PIN_0

#define NUTRIENT_SOLN_B_PORT	GPIOA
#define NUTRIENT_SOLN_B_PIN		GPIO_PIN_0

#define PH_UP_VALVE_PORT		GPIOA
#define PH_UP_VALVE_PIN			GPIO_PIN_0

#define PH_DOWN_VALVE_PORT		GPIOA
#define PH_DOWN_VALVE_PIN		GPIO_PIN_0

#define GPIO_SWITCHING_INTF_INIT_SUCCEED	true
#define GPIO_SWITCHING_INTF_INIT_FAIL		false


#endif /* INC_GPIO_SWITCHING_INTF_H_ */
