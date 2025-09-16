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

static HAL_StatusTypeDef _send_i2c_packet( uint8_t *packetData, uint16_t packetSize, RPI_I2C_ACK_Packet_t ackPacket, uint32_t timeout );


/*-----------------------------------------------------------------------------
 *
 * RPI_I2C_Send_Gcode_Pkt
 *
 * 		Sends gcode command given by 'gcode' to the Raspberry Pi over I2C.
 * 		Timeout (milliseconds) is the maximum time to wait for the I2C
 * 		transaction to complete. Reccommended timeout is 10ms.
 *
 * 		Raspberry Pi I2C FIFO buffer only has a size of 16 bytes, so this
 * 		large GCode packet must be broken up into multiple chunks and sent
 * 		one-by-one.
 *
 * 		Returns SYS_SUCCESS if the command was sent successfully, otherwise
 * 		SYS_FAIL.
 *
 * 		Testing, this compound packet function takes ~15ms to complete. It
 * 		currently includes a minimum of 5ms of Delay() time
 *
-----------------------------------------------------------------------------*/

SYS_RESULT RPI_I2C_Send_Gcode_Pkt( const char *gcode, uint32_t timeout ) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_I2C_Packet_GCode_0_t gcode_0_packet;
	RPI_I2C_Packet_GCode_1_t gcode_1_packet;
	RPI_I2C_Packet_GCode_2_t gcode_2_packet;
	RPI_I2C_Packet_GCode_3_t gcode_3_packet;
	RPI_I2C_Packet_GCode_4_t gcode_4_packet;
	RPI_I2C_ACK_Packet_t ack_packet;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&gcode_0_packet, 0, sizeof(RPI_I2C_Packet_GCode_0_t));
	memset(&gcode_1_packet, 0, sizeof(RPI_I2C_Packet_GCode_1_t));
	memset(&gcode_2_packet, 0, sizeof(RPI_I2C_Packet_GCode_2_t));
	memset(&gcode_3_packet, 0, sizeof(RPI_I2C_Packet_GCode_3_t));
	memset(&gcode_4_packet, 0, sizeof(RPI_I2C_Packet_GCode_4_t));
	memset(&ack_packet, 0, sizeof(RPI_I2C_ACK_Packet_t));

	/*-------------------------------------------------------------------------
	Return invalid if no meanigful gcode is provided
	-------------------------------------------------------------------------*/
	if ( gcode == NULL || strlen(gcode) == 0 ) {
		return SYS_INVALID;
	}

	/*-------------------------------------------------------------------------
	Pack the packets
	-------------------------------------------------------------------------*/
	gcode_0_packet.packet_id = RPI_GCODE_0_PKT_ID;
	gcode_0_packet.valid = true;
	strncpy((char *)gcode_0_packet.gcode_str, gcode, sizeof(gcode_0_packet.gcode_str));
	
	gcode_1_packet.packet_id = RPI_GCODE_1_PKT_ID;
	strncpy((char *)gcode_1_packet.gcode_str, (gcode + sizeof(gcode_0_packet.gcode_str)), sizeof(gcode_1_packet.gcode_str));

	gcode_2_packet.packet_id = RPI_GCODE_2_PKT_ID;
	strncpy((char *)gcode_2_packet.gcode_str, (gcode + sizeof(gcode_0_packet.gcode_str) + sizeof(gcode_1_packet.gcode_str)), sizeof(gcode_2_packet.gcode_str));

	gcode_3_packet.packet_id = RPI_GCODE_3_PKT_ID;
	strncpy((char *)gcode_3_packet.gcode_str, (gcode + sizeof(gcode_0_packet.gcode_str) + (sizeof(gcode_1_packet.gcode_str) * 2)), sizeof(gcode_3_packet.gcode_str));

	gcode_4_packet.packet_id = RPI_GCODE_4_PKT_ID;
	strncpy((char *)gcode_4_packet.gcode_str, (gcode + sizeof(gcode_0_packet.gcode_str) + (sizeof(gcode_1_packet.gcode_str) * 3)), sizeof(gcode_4_packet.gcode_str));
	/*-------------------------------------------------------------------------
	Send packet chunk 0
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&gcode_0_packet, RPI_I2C_GCODE_0_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	/*-------------------------------------------------------------------------
	Reset the ACK packet
	-------------------------------------------------------------------------*/
	memset(&ack_packet, 0, sizeof(RPI_I2C_ACK_Packet_t));

	/*-------------------------------------------------------------------------
	Send packet chunk 1
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&gcode_1_packet, RPI_I2C_GCODE_1_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	/*-------------------------------------------------------------------------
	Reset the ACK packet
	-------------------------------------------------------------------------*/
	memset(&ack_packet, 0, sizeof(RPI_I2C_ACK_Packet_t));

	/*-------------------------------------------------------------------------
	Send packet chunk 2
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&gcode_2_packet, RPI_I2C_GCODE_2_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	/*-------------------------------------------------------------------------
	Reset the ACK packet
	-------------------------------------------------------------------------*/
	memset(&ack_packet, 0, sizeof(RPI_I2C_ACK_Packet_t));

	/*-------------------------------------------------------------------------
	Send packet chunk 3
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&gcode_3_packet, RPI_I2C_GCODE_3_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	/*-------------------------------------------------------------------------
	Reset the ACK packet
	-------------------------------------------------------------------------*/
	memset(&ack_packet, 0, sizeof(RPI_I2C_ACK_Packet_t));

	/*-------------------------------------------------------------------------
	Send packet chunk 4
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&gcode_4_packet, RPI_I2C_GCODE_4_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return SYS_SUCCESS;
}

SYS_RESULT RPI_I2C_Send_AHT20_Pkt(struct AHT20_Data AHT20_data, uint32_t timeout) {

	RPI_I2C_AHT20_Packet_t aht20_pkt;
	RPI_I2C_ACK_Packet_t ack_packet;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&aht20_pkt, 0, RPI_I2C_AHT20_PACKET_SIZE);
	memset(&ack_packet, 0, sizeof(ack_packet));

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	aht20_pkt.packet_id = RPI_AHT20_PKT_ID;
	aht20_pkt.valid = true;
	aht20_pkt.aht20_data = AHT20_data;

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&aht20_pkt, RPI_I2C_AHT20_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return status;
}

SYS_RESULT RPI_I2C_Send_SEN0169_Pkt(SEN0169_pH_Data SEN0169_data, uint32_t timeout) {

	RPI_I2C_SEN0169_Packet_t SEN0169_pkt;
	RPI_I2C_ACK_Packet_t ack_packet;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&SEN0169_pkt, 0, RPI_I2C_SEN0169_PACKET_SIZE);
	memset(&ack_packet, 0, sizeof(ack_packet));

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	SEN0169_pkt.packet_id = RPI_SEN0169_PKT_ID;
	SEN0169_pkt.SEN0169_data = SEN0169_data;

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&SEN0169_pkt, RPI_I2C_SEN0169_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return status;

}

SYS_RESULT RPI_I2C_Send_SEN0244_Pkt(SEN0244_TDS_Data SEN0244_data, uint32_t timeout) {

	RPI_I2C_SEN0244_Packet_t SEN0244_pkt;
	RPI_I2C_ACK_Packet_t ack_packet;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&SEN0244_pkt, 0, RPI_I2C_SEN0244_PACKET_SIZE);
	memset(&ack_packet, 0, sizeof(ack_packet));

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	SEN0244_pkt.packet_id = RPI_SEN0244_PKT_ID;
	SEN0244_pkt.SEN0244_data = SEN0244_data;

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&SEN0244_pkt, RPI_I2C_SEN0244_PACKET_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return status;
}

//uint16_t AS7341_Values[12];
SYS_RESULT RPI_I2C_Send_AS7341_Pkt(uint16_t AS7341_data[12], uint32_t timeout) {

	RPI_I2C_AS7341_Packet_0_t AS7341_0_pkt;
	RPI_I2C_AS7341_Packet_1_t AS7341_1_pkt;
	RPI_I2C_ACK_Packet_t ack_packet;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&AS7341_0_pkt, 0, RPI_I2C_AS7341_PACKET_0_SIZE);
	memset(&AS7341_1_pkt, 0, RPI_I2C_AS7341_PACKET_1_SIZE);
	memset(&ack_packet, 0, sizeof(ack_packet));

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	AS7341_0_pkt.packet_id = RPI_AS7341_0_PKT_ID;
	AS7341_1_pkt.packet_id = RPI_AS7341_1_PKT_ID;
	for (int i = 0; i < 7; i++) {
		AS7341_0_pkt.AS7341_data[i] = AS7341_data[i];
	}

	for (int i = 7; i < 12; i++) {
		AS7341_1_pkt.AS7341_data[i-7] = AS7341_data[i];
	}

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_i2c_packet(&AS7341_0_pkt, RPI_I2C_AS7341_PACKET_0_SIZE, ack_packet, timeout);


	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	memset(&ack_packet, 0, sizeof(ack_packet));

	status = _send_i2c_packet(&AS7341_1_pkt, RPI_I2C_AS7341_PACKET_1_SIZE, ack_packet, timeout);

	if (status != HAL_OK) {
			return SYS_FAIL;
	}

	return status;
}
/*-----------------------------------------------------------------------------
 *
 * _send_i2c_packet
 *
 * 		Helper function that directly interfaces with the HAL_I2C functions.
 * 		Sends the I2C Packet. If fails to send, tries up to
 * 		RPI_I2C_NUM_PKT_SEND_ATTEMPTS times. After sending packet, the function
 * 		waits to receive a RPI_I2C_ACK_Packet_t packet from the Raspberry Pi.
 * 		This function confirms that the Pi sends a positive ACK.
 *
 * 		Args:
 * 		*packetData: A packet struct defined in RPI_I2C.h
 * 		packetSize: sizeof(packetData)
 * 		ackPacket: the struct container for the ack packet.
 * 		timeout: Message send timeout in milliseconds
 *
 * 		NOTE: No individual packet should be longer than 16 bytes.
 *
-----------------------------------------------------------------------------*/
static HAL_StatusTypeDef _send_i2c_packet( uint8_t *packetData, uint16_t packetSize, RPI_I2C_ACK_Packet_t ackPacket, uint32_t timeout ) {
	HAL_StatusTypeDef status;
	uint8_t i;

	for ( i = 0; i < RPI_I2C_NUM_PKT_SEND_ATTEMPTS; i++ ) {
			/*-------------------------------------------------------------------------
			Send the packet using HAL
			-------------------------------------------------------------------------*/
			status = HAL_I2C_Master_Transmit(&hi2c1, RPI_I2C_ADDR_WRITE, packetData, packetSize, timeout);

			/*-------------------------------------------------------------------------
			If we sent the packet successfully, wait for ack packet
			-------------------------------------------------------------------------*/
			if (status == HAL_OK) {
				HAL_Delay(1);

				/*---------------------------------------------------------------------
				Receive the ACK packet
				---------------------------------------------------------------------*/
				status = HAL_I2C_Master_Receive(&hi2c1, RPI_I2C_ADDR_WRITE, &ackPacket, RPI_I2C_ACK_PACKET_SIZE, 3);

				if (status == HAL_OK && ackPacket.packet_id == RPI_ACK_PKT_ID && ackPacket.ack == true ) {
					break;
				}
			}
		}
	return status;

}
