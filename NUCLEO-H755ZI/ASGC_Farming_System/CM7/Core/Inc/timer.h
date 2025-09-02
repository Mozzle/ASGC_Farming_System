/*-----------------------------------------------------------------------------
 *
 * 	timer.h
 *
 *  Created on:
 *
 *  DESCRIPTION: HAL interface to allow timer overflow
 *
-----------------------------------------------------------------------------*/
#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

void 		ASGC_Timer_Init();
uint64_t 	getTimestamp();

#endif /* INC_TIMER_H_ */
