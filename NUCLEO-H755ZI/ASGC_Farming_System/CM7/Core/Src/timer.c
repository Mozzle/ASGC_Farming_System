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

static uint64_t s_overflowTimeMs;
static uint32_t prev_32_bit_timestampMs;

void ASGC_Timer_Init() {
	s_overflowTimeMs = 0;
	prev_32_bit_timestampMs = 0;
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
