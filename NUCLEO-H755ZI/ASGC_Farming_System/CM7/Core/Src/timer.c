/*-----------------------------------------------------------------------------
 *
 * 	timer.c
 *
 *  Created on:
 *
 *  DESCRIPTION: HAL interface to allow timer overflow, so the system can keep
 *  accurate time for longer than 49.7 days. All timing-based implementations
 *  in this project should use getTimestamp() to fetch the milliseconds since
 *  system on, rather than the HAL_GetTick() function.
 *
-----------------------------------------------------------------------------*/

#include "timer.h"

static uint32_t unixTimeRefSec;
static uint64_t timestampAtUnixTimeRefMs;
static uint32_t nextMidnightTimeSec; 

static uint64_t s_overflowTimeMs;
static uint32_t prev_32_bit_timestampMs;

void ASGC_Timer_Init() {
	s_overflowTimeMs = 0;
	prev_32_bit_timestampMs = 0;
	nextMidnightTimeSec = 0;
}

// Define constants for Midnight Checker/Calculation functions
#define SEC_IN_DAY 86400
#define SEC_IN_HOUR 3600
#define SEC_IN_MIN 60

/*---------------------------------------------------------------------------------------------------
	setUnixTimeMidnightRef(const uint32_t currentTimeSec, const int8_t TimeZoneOffsetUTCHours)
	Takes in a unix timestamp in seconds
	Takes in a timezone offset in hours from UTC (i.e. UTC-5 should pass '-5', UTC+5 should pass '5')

	This function calculates the next midnight time in seconds, then updates the static variable
	nextMidnightTimeSec with the new value.
---------------------------------------------------------------------------------------------------*/
void setUnixTimeMidnightRef(const uint32_t currentTimeSec, const int8_t TimeZoneOffsetUTCHours) {
	// Set time reference
	unixTimeRefSec = currentTimeSec;
	timestampAtUnixTimeRefMs = getTimestamp();

    // Determine what hour, minute, and second we're at in the day
    const uint8_t currentHour = (((currentTimeSec % SEC_IN_DAY) / SEC_IN_HOUR) + TimeZoneOffsetUTCHours) % 24;
    const uint8_t currentMinute = ((currentTimeSec % SEC_IN_HOUR) / SEC_IN_MIN);
    const uint8_t currentSecond = (currentTimeSec % SEC_IN_MIN);

    // Determine the remaining time to midnight for H:M:S, uses ternary operators to avoid overlap (i.e. if seconds>0, then minutes must be decremented by 1)
    const uint8_t remainingSeconds = (SEC_IN_MIN - currentSecond) % SEC_IN_MIN;
    const uint8_t remainingMinutes = (remainingSeconds>0) ? (SEC_IN_MIN - currentMinute) % SEC_IN_MIN - 1 : (SEC_IN_MIN - currentMinute) % SEC_IN_MIN;
    const uint8_t remainingHours = (remainingMinutes>0) ? (24 - currentHour) % 24 - 1 : (24 - currentHour) % 24;
    
    // Calculate remaining seconds to midnight, and update the static variable
    const uint32_t secondsToMidnight = remainingSeconds + (remainingMinutes * SEC_IN_MIN) + (remainingHours * SEC_IN_HOUR);
    nextMidnightTimeSec = currentTimeSec + secondsToMidnight;
}

/*-------------------------------------------------------------------------------------
	isMidnight()

	Returns uint8_t integer code, 1 = IS_MIDNIGHT, 0 = NOT_MIDNIGHT

	This function checks if the current time is at or past midnight, based on the 
	unix time reference and the next midnight time calculated by setUnixTimeMidnightRef().
	If it is, it calculates the next midnight time, sets it and returns IS_MIDNIGHT
-------------------------------------------------------------------------------------*/
uint8_t isMidnight() {

	// Get the current time in seconds, calculate using the UnixTime and system offset
	const uint32_t currentTimeSec = unixTimeRefSec + ((getTimestamp() - timestampAtUnixTimeRefMs) / 1000);

	// Check if we're at or past Midnight -- if we are, recalculate and return IS_MIDNIGHT
	if (nextMidnightTimeSec != 0 && currentTimeSec >= nextMidnightTimeSec) {
        nextMidnightTimeSec += SEC_IN_DAY;
		return IS_MIDNIGHT;
	} 
	// Check if we've initialized nextMidnightTimeSec
	else if (nextMidnightTimeSec == 0) {
        return -1;
	}

	// If nextMidnightTimeSec is initialized and we haven't reached it, return NOT_MIDNIGHT
	return NOT_MIDNIGHT;
}

/*-----------------------------------------------------------------------------
 *
 * 		getTimestamp
 *
 * 		Returns the number of milliseconds elapsed since the system clocks
 * 		were initialized (power on).
 *
 * 		NOTE: This function needs to be tested. I think the STM32CubeIDE
 * 		debugger allows for variable manipulation to test this function.
 *
 ----------------------------------------------------------------------------*/

uint64_t getTimestamp() {

	uint32_t timestampMs_32_bit;
	uint64_t timestampMs_ret_val;

	__disable_irq();	// Lock: prevent interrupts while fetching time.

	timestampMs_32_bit = HAL_GetTick();	// Get 32 bit time elapsed
	// (Time elapsed since last overflow)

	// If an integer overflow has occurred (49.7 days have elapsed)
	if ( timestampMs_32_bit < prev_32_bit_timestampMs ) {
		s_overflowTimeMs += 0xFFFFFFFF;	// Add 32 bits worth of milliseconds to the time elapsed
	}

	prev_32_bit_timestampMs = timestampMs_32_bit; // update overflow comparison value
	timestampMs_ret_val = s_overflowTimeMs + timestampMs_32_bit;

	__enable_irq();

	return timestampMs_ret_val;

}
