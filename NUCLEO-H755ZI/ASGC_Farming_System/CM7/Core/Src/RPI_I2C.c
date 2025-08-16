/*-----------------------------------------------------------------------------
 *
 * RPI_I2C.c
 *
 * 		File containing the implementation of the I2C communication with the
 * 		Raspberry Pi.
 *
 *  Created on: August 13, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "RPI_I2C.h"

#include <string.h>

extern I2C_HandleTypeDef hi2c1;

/*-----------------------------------------------------------------------------
 *
 * RPI_I2C_Send_Gcode_Pkt
 *
 * 		Sends gcode command given by 'gcode' to the Raspberry Pi over I2C.
 * 		Timeout (milliseconds) is the maximum time to wait for the I2C
 * 		transaction to complete. Reccommended timeout is 100ms.
 * 		Returns SYS_SUCCESS if the command was sent successfully, otherwise
 * 		SYS_FAIL.
 *
-----------------------------------------------------------------------------*/

SYS_RESULT RPI_I2C_Send_Gcode_Pkt( const char *gcode, uint32_t timeout ) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_I2C_Packet_GCode_t gcode_packet;
	RPI_I2C_ACK_Packet_t ack_packet;
	HAL_StatusTypeDef status;
	uint8_t i;

	/*-------------------------------------------------------------------------
	Clear struct
	-------------------------------------------------------------------------*/
	memset(&gcode_packet, 0, sizeof(RPI_I2C_Packet_GCode_t));

	/*-------------------------------------------------------------------------
	Return invalid if no meanigful gcode is provided
	-------------------------------------------------------------------------*/
	if ( gcode == NULL || strlen(gcode) == 0 ) {
		return SYS_INVALID;
	}

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	gcode_packet.packet_id = RPI_GCODE_PKT_ID;
	gcode_packet.valid = true;
	strncpy((char *)gcode_packet.gcode_str, gcode, sizeof(gcode_packet.gcode_str) - 1);
	gcode_packet.gcode_str[sizeof(gcode_packet.gcode_str) - 1] = '\0'; // Ensure null termination

	for ( i = 0; i < RPI_I2C_NUM_PKT_SEND_ATTEMPTS; i++ ) {
		/*-------------------------------------------------------------------------
		Send the packet using HAL
		-------------------------------------------------------------------------*/
		status = HAL_I2C_Master_Transmit(&hi2c1, RPI_I2C_ADDR_WRITE, &gcode_packet, RPI_I2C_GCODE_PACKET_SIZE, timeout);

		/*-------------------------------------------------------------------------
		If we sent the packet successfully, wait for ack packet
		-------------------------------------------------------------------------*/
		if (status == HAL_OK) {
			HAL_Delay(1);
			status = HAL_I2C_Master_Receive(&hi2c1, RPI_I2C_ADDR_WRITE, &ack_packet, 2, 3);
			if (status == HAL_OK && ack_packet.packet_id == RPI_ACK_PKT_ID && ack_packet.ack == true ) {
				break;
			}
		}
	}
	return SYS_FAIL;
}
