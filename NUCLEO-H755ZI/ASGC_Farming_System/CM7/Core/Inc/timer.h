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

// Defines for midnight/time calculation functions
#define     IS_MIDNIGHT (uint8_t)1
#define     NOT_MIDNIGHT (uint8_t)0
#define     CST_OFFSET (int8_t)-5

void 		ASGC_Timer_Init();
uint64_t 	getTimestamp();
uint8_t     isMidnight();
void        setUnixTimeMidnightRef(const uint32_t currentTimeSec, const int8_t TimeZoneOffsetUTCHours);

#endif /* INC_TIMER_H_ */
