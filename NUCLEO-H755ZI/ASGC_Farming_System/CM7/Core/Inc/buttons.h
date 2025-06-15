/*-----------------------------------------------------------------------------
 *
 * Buttons.c
 *
 * 		Code covering 'Start' and 'E-Stop' button interrupts and functionality
 *
 *  Created on: June 1, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "main.h"

#define BUTTONS_INIT_SUCCEED	true
#define BUTTONS_INIT_FAIL		false

bool Buttons_Init();
SYS_RESULT Buttons_start_button_intrpt(bool * start_state);
SYS_RESULT Buttons_estop_button_intrpt(bool * estop_state, bool start_state);

#endif /* INC_BUTTONS_H_ */
