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

#define BUTTONS_ON				true
#define BUTTONS_OFF				false

bool Buttons_Init();
SYS_RESULT Buttons_register_Start_button_intrpt();
SYS_RESULT Buttons_register_EStop_button_intrpt();

#endif /* INC_BUTTONS_H_ */
