/*-----------------------------------------------------------------------------
 *
 * CNC.c
 *
 * 		Code covering the CNC module for the ASGC Farming System
 *      This module is responsible for handling the generation and sending of 
 * 		G-code commands to the SKR Mini E3 V3.0 CNC Control board via USB CDC.
 *
 *  Created on: July 27, 2025
 *      Author: Dylan Collier
 *
-----------------------------------------------------------------------------*/

#include "CNC.h"
#include "RPI_I2C.h"

static CNC_NFT_Data CNC_DATA;
bool CNC_Initialized = false;

/*-----------------------------------------------------------------------------
 *
 * 		usb_send_gcode
 *
 * 		Sends a G-code command to the CNC control board over USB CDC. Timeout 
 *      is num of ms to wait for the USB CDC interface to be ready to transmit.
 * 
 * 		NOTE: DO NOT make a timeout of longer than 500ms
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT usb_send_gcode( const char *gcode, uint32_t timeout ) {
	/*-------------------------------------------------------------------------
	Local Variables
 	-------------------------------------------------------------------------*/
	uint16_t gcode_len;

	/*-------------------------------------------------------------------------
	Validity Checks
 	-------------------------------------------------------------------------*/
	if ( gcode == NULL ) {
		return SYS_INVALID; // Return invalid if gcode is NULL
	}

	gcode_len = strlen( gcode );
	if ( gcode_len == 0 || gcode_len > RPI_I2C_GCODE_MAX_STR_LEN ) {
		return SYS_INVALID;
	}

	/*-------------------------------------------------------------------------
	Send Packets
 	-------------------------------------------------------------------------*/
	if (RPI_I2C_Send_Gcode_Pkt( gcode, timeout ) == SYS_SUCCESS) {
		return SYS_SUCCESS; // Return success if the G-code command was sent successfully
	}
	return SYS_FAIL; // Return fail if the G-code command was not sent successfully

}
/*-----------------------------------------------------------------------------
 *
 * 		CNC_Init
 *
 * 		Initializes the CNC module. This function will initialize the CNC_DATA
 * 		structure with the NFT hole position values for the farming system.
 *
 * 		NOTE: This function should be called after the USB CDC interface has been
 * 		initialized.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT CNC_Init() {

	// If CNC is not enabled, do not try to initialize it
	if (RASPBERRY_PI_INTERFACE_ENABLED == SYS_FEATURE_DISABLED) {
		CNC_Initialized = true;
		return SYS_SUCCESS;
	}

	CNC_DATA = (CNC_NFT_Data) {
		.channel_holes = {
			// The (x, y) of each hole will be experimentally determined and
			// hardcoded here.

			// !TODO: If ROM space becomes an issue, consider making an EEPROM 
			// config file to store these values.
			{ // Channel 0
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 0
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 1
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 2
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 3
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 4
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 5
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 6
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 7
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 8
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }  // Hole 9
			},
			{ // Channel 1
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 0
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 1
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 2
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 3
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 4
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 5
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 6
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 7
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 8
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }  // Hole 9
			},
			{ // Channel 2
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 0
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 1
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 2
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 3
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 4
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 5
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 6
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 7
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 8
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }  // Hole 9
			},
			{ // Channel 3
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 0
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 1
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 2
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 3
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 4
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 5
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 6
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 7
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }, // Hole 8
				{ .x_pos = 0000.00, .y_pos = 0000.00, .is_empty = false }  // Hole 9
			}
		}
	};

	// CNC homing is now handled by a FSM state.
	//if (CNC_Home_Command() != SYS_SUCCESS) {
	//	// If the homing command failed to send, we should not continue
	//	CNC_Initialized = false;
	//	return false;
	//}

	return CNC_Initialized;
}


/*-----------------------------------------------------------------------------
 *
 * 		CNC_Find_Hole_Closest_To_Position
 *
 * 		Finds the hole closest to the given (x, y) position in the farming 
 * 		system. This function returns the closest hole in the CNC_DATA 
 * 		structure in the form of a {uint8_t, uint8_t} array, where the first 
 * 		element is the channel index and the second is the hole index.
 * 
 * 		I can imagine this function being used with the ToF sensor to find 
 * 		ready to harvest plants.
 *
 ----------------------------------------------------------------------------*/

uint8_t* CNC_Find_Hole_Closest_To_Position(float x_pos, float y_pos) {
	static uint8_t closest_hole[2] = {0, 0}; // [channel_index, hole_index]
	float closest_distance = 999999.9; // Start with a large distance

	for (uint8_t channel = 0; channel < CNC_NUM_NFT_CHANNELS; channel++) {
		for (uint8_t hole = 0; hole < CNC_NUM_NET_POTS_PER_NFT_CHANNEL; hole++) {
			CNC_NFT_Channel_Hole_Data *current_hole = &CNC_DATA.channel_holes[channel][hole];
			if (current_hole->is_empty) continue; // Skip empty holes

			float distance = sqrtf(powf(current_hole->x_pos - x_pos, 2) + powf(current_hole->y_pos - y_pos, 2));
			if (distance < closest_distance) {
				closest_distance = distance;
				closest_hole[0] = channel;
				closest_hole[1] = hole;
			}
		}
	}

	return closest_hole;
}
 


/*-----------------------------------------------------------------------------
 *
 * 		CNC_Home_Command
 *
 * 		Sends a G-code command to the CNC control board to home the CNC system.
 * 		This will move the CNC system to the home position.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT CNC_Home_Command() {

	// Send the G-code command to home the CNC system
	const char *gcode = "G28"; // G28 is the G-code command for homing
	SYS_RESULT result = usb_send_gcode(gcode, 100); // 100ms timeout

	return result;
}


/*-----------------------------------------------------------------------------
 *
 * 		CNC_Move_To_Position
 *
 * 		Sends a G-code command to the CNC control board to move the CNC system 
 * 		to the given (x, y) position in the farming system, accounting for
 * 		toolhead offset. This function will return the result of the G-code
 * 		send command.
 *
 ----------------------------------------------------------------------------*/

SYS_RESULT CNC_Move_To_Position(float x_pos, float y_pos) {

	if (!CNC_Initialized) {
		return SYS_NOT_INITIALIZED; // If CNC is not initialized, return error
	}

//	if (CNC_DATA.equipped_toolhead == CNC_TOOLHEAD_SEED_DISPENSER) {
//		// If the seed dispenser is equipped, adjust the position for the dispenser offset
//		x_pos += SEED_DISPENSER_X_OFFSET_MM;
//		y_pos += SEED_DISPENSER_Y_OFFSET_MM;
//		
//		/* Example of clamping to max position if out of bounds
//		if (x_pos > SEED_DISPENSER_MAX_X_POS_MM) {
//			x_pos = SEED_DISPENSER_MAX_X_POS_MM;
//		} */
//
//	} else if (CNC_DATA.equipped_toolhead == CNC_TOOLHEAD_LIFTER_ARM) {
//		// If the lifter arm is equipped, adjust the position for the lifter arm offset
//		x_pos += LIFTER_X_OFFSET_MM;
//		y_pos += LIFTER_Y_OFFSET_MM;
//
//		/* Example of clamping to max position if out of bounds
//		if (x_pos > LIFTER_MAX_X_POS_MM) {
//			x_pos = LIFTER_MAX_X_POS_MM;
//		} */
//	}

	// Send the G-code command to move the CNC system to the given position
	char gcode[50];
	snprintf(gcode, sizeof(gcode), "G0 X%.2f Y%.2f\n", x_pos, y_pos); // G0 is the G-code command for rapid positioning
	SYS_RESULT result = usb_send_gcode(gcode, 75); // 75ms timeout

	return result; // Return the result of the G-code command
}

SYS_RESULT CNC_Move_To_Hole(uint8_t channel_index, uint8_t hole_index) {

	float x_destination, y_destination;

	// Can't move to hole if invalid channel/hole index or toolhead not 
	// determined
	if (channel_index >= CNC_NUM_NFT_CHANNELS || hole_index >= CNC_NUM_NET_POTS_PER_NFT_CHANNEL) 
	{
		return SYS_INVALID; 
	}

	x_destination = CNC_DATA.channel_holes[channel_index][hole_index].x_pos;
	y_destination = CNC_DATA.channel_holes[channel_index][hole_index].y_pos;

//	if (CNC_DATA.equipped_toolhead == CNC_TOOLHEAD_SEED_DISPENSER) {
//		// If the seed dispenser is equipped, adjust the position for the dispenser offset
//		x_destination -= (SEED_DISPENSER_X_OFFSET_MM + SEED_DISPENSER_X_OFFSET_MM);
//		y_destination -= (SEED_DISPENSER_Y_OFFSET_MM + SEED_DISPENSER_Y_OFFSET_MM);
//
//	} else if (CNC_DATA.equipped_toolhead == CNC_TOOLHEAD_LIFTER_ARM) {
//		// If the lifter arm is equipped, adjust the position for the lifter arm offset
//		x_destination -= (LIFTER_X_OFFSET_MM + LIFTER_X_OFFSET_MM);
//		y_destination -= (LIFTER_Y_OFFSET_MM + LIFTER_Y_OFFSET_MM);
//
//	}

	return CNC_Move_To_Position(x_destination, y_destination);
}

SYS_RESULT CNC_Dispense_Seeds() {
	// If CNC_Initialized is false, return error. 
	// It should be initialized (homed) before this function is called.
	if (!CNC_Initialized) {
		return SYS_NOT_INITIALIZED;
	}
	// Use static variables to keep track of state between calls

	// Send movement commands using CNC_Move_To_Hole()
	// Poll for confirmation that the gantry has made it to the hole. (This packet has not yet been defined)
	// Once the gantry is above the hole, Dispense seeds (PWM signal to a servo: THIS IS NOT YET IMPLEMENTED)
	// After a short time, close seed dispenser shutter (another PWM signal change)
	// Dwell for a second for seeds to finish falling
	// Send movement command to next hole.
	// Iterate through these steps for all 40 holes.
	return SYS_SUCCESS;
}
