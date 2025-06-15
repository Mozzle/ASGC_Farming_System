/*-----------------------------------------------------------------------------
 *
 *	GPIO_switching_intf.h
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

/*-----------------------------------------------------------------------------
DEFINES
-----------------------------------------------------------------------------*/

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

/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/

bool GPIO_switching_intf_Init();
SYS_RESULT GPIO_set_circulating_pump(bool state);
SYS_RESULT GPIO_set_drainage_pump(bool state);
SYS_RESULT GPIO_set_fill_valve(bool state);
SYS_RESULT GPIO_set_nutrient_soln_a_valve(bool state);
SYS_RESULT GPIO_set_nutrient_soln_b_valve(bool state);
SYS_RESULT GPIO_set_ph_up_valve(bool state);
SYS_RESULT GPIO_set_ph_down_valve(bool state);

#endif /* INC_GPIO_SWITCHING_INTF_H_ */
