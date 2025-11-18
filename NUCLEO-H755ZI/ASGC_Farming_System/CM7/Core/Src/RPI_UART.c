/*-----------------------------------------------------------------------------
 *
 * RPI_UART.c
 *
 * 		File containing the implementation of the UART communication with the
 * 		Raspberry Pi.
 *
 *  Created on: August 13, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#include "RPI_UART.h"
#include "timer.h"
#include <string.h>

extern UART_HandleTypeDef huart7;

static HAL_StatusTypeDef _send_uart_packet( uint8_t *packetData, uint16_t packetSize, RPI_UART_Header_Packet_t *header_pkt, uint32_t timeout );
static HAL_StatusTypeDef _send_uart_unix_time_packet( uint8_t *packetData, uint16_t packetSize, RPI_UART_Unix_Time_t *unixTimePacket, uint32_t timeout );

/*-----------------------------------------------------------------------------
 *
 * RPI_UART_Send_Gcode_Pkt
 *
 * 		Sends gcode command given by 'gcode' to the Raspberry Pi over UART.
 * 		Timeout (milliseconds) is the maximum time to wait for the UART
 * 		transaction to complete. Reccommended timeout is 3ms.
 *
 * 		Returns SYS_SUCCESS if the command was sent successfully, otherwise
 * 		SYS_FAIL.
 *
-----------------------------------------------------------------------------*/

SYS_RESULT RPI_UART_Send_Gcode_Pkt( const char *gcode, uint32_t timeout ) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_UART_Packet_GCode_t gcode_packet;
	RPI_UART_Header_Packet_t header_pkt;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&gcode_packet, 0, RPI_UART_GCODE_PACKET_SIZE);
	memset(&header_pkt, 0, RPI_UART_HEADER_PACKET_SIZE);

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
	header_pkt.packet_id = RPI_GCODE_PKT_ID;
	strncpy((char *)gcode_packet.gcode_str, gcode, RPI_UART_GCODE_MAX_STR_LEN);

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_uart_packet((uint8_t*)&gcode_packet, RPI_UART_GCODE_PACKET_SIZE, &header_pkt, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return SYS_SUCCESS;
}

SYS_RESULT RPI_UART_Send_AHT20_Pkt(AHT20_Data_t aht20_data, uint32_t timeout) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_UART_AHT20_Packet_t aht20_packet;
	RPI_UART_Header_Packet_t header_pkt;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&aht20_packet, 0, RPI_UART_AHT20_PACKET_SIZE);
	memset(&header_pkt, 0, RPI_UART_HEADER_PACKET_SIZE);

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	aht20_packet.packet_id = RPI_AHT20_PKT_ID;
	header_pkt.packet_id = RPI_AHT20_PKT_ID;
	aht20_packet.valid = true;
	aht20_packet.aht20_data = aht20_data;

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_uart_packet((uint8_t*)&aht20_packet, RPI_UART_AHT20_PACKET_SIZE, &header_pkt, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return SYS_SUCCESS;
}

SYS_RESULT RPI_UART_Send_SEN0169_Pkt(SEN0169_pH_Data SEN0169_data, uint32_t timeout) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_UART_SEN0169_Packet_t SEN0169_packet;
	RPI_UART_Header_Packet_t header_pkt;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&SEN0169_packet, 0, RPI_UART_SEN0169_PACKET_SIZE);
	memset(&header_pkt, 0, RPI_UART_HEADER_PACKET_SIZE);

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	SEN0169_packet.packet_id = RPI_SEN0169_PKT_ID;
	header_pkt.packet_id = RPI_SEN0169_PKT_ID;
	SEN0169_packet.SEN0169_data = SEN0169_data;

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_uart_packet((uint8_t*)&SEN0169_packet, RPI_UART_SEN0169_PACKET_SIZE, &header_pkt, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return SYS_SUCCESS;
}

SYS_RESULT RPI_UART_Send_SEN0244_Pkt(SEN0244_TDS_Data SEN0244_data, uint32_t timeout) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_UART_SEN0244_Packet_t SEN0244_packet;
	RPI_UART_Header_Packet_t header_pkt;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&SEN0244_packet, 0, RPI_UART_SEN0244_PACKET_SIZE);
	memset(&header_pkt, 0, RPI_UART_HEADER_PACKET_SIZE);

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	SEN0244_packet.packet_id = RPI_SEN0244_PKT_ID;
	header_pkt.packet_id = RPI_SEN0244_PKT_ID;
	SEN0244_packet.SEN0244_data = SEN0244_data;

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_uart_packet((uint8_t*)&SEN0244_packet, RPI_UART_SEN0244_PACKET_SIZE, &header_pkt, timeout);

	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return SYS_SUCCESS;
}

SYS_RESULT RPI_UART_Send_AS7341_Pkt(uint16_t *AS7341_data, uint32_t timeout) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_UART_AS7341_Packet_t AS7341_pkt;
	RPI_UART_Header_Packet_t header_pkt;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&AS7341_pkt, 0, RPI_UART_AS7341_PACKET_SIZE);
	memset(&header_pkt, 0, RPI_UART_HEADER_PACKET_SIZE);

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	AS7341_pkt.packet_id = RPI_AS7341_PKT_ID;
	header_pkt.packet_id = RPI_AS7341_PKT_ID; 
	for (uint8_t i = 0; i < 12; i++) {
		AS7341_pkt.AS7341_data[i] = AS7341_data[i];
	}

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_uart_packet((uint8_t*)&AS7341_pkt, RPI_UART_AS7341_PACKET_SIZE, &header_pkt, timeout);
	if (status != HAL_OK) {
		return SYS_FAIL;
	}

	return status;
}

SYS_RESULT RPI_UART_Send_RPI_UNIX_TIME_REQUEST_Pkt(uint32_t timeout) {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	RPI_UART_Unix_Time_t UNIX_TIME_pkt;
	RPI_UART_Header_Packet_t header_pkt;
	HAL_StatusTypeDef status;

	/*-------------------------------------------------------------------------
	Clear structs
	-------------------------------------------------------------------------*/
	memset(&header_pkt, 0, RPI_UART_HEADER_PACKET_SIZE);

	/*-------------------------------------------------------------------------
	Pack the packet
	-------------------------------------------------------------------------*/
	header_pkt.packet_id = RPI_UNIX_TIME_REQUEST_PKT_ID;

	/*-------------------------------------------------------------------------
	Send packet
	-------------------------------------------------------------------------*/
	status = _send_uart_unix_time_packet((uint8_t*)&header_pkt, RPI_UART_HEADER_PACKET_SIZE, &UNIX_TIME_pkt, timeout);

	if (status != HAL_OK) {
			return SYS_FAIL;
	}

	/*-------------------------------------------------------------------------
	Store unix time reference values
	-------------------------------------------------------------------------*/
	setUnixTimeMidnightRef(UNIX_TIME_pkt.UNIX_time_value, UNIX_TIME_pkt.Offset);

	return status;
}

static HAL_StatusTypeDef _send_uart_packet( uint8_t *packetData, uint16_t packetSize, RPI_UART_Header_Packet_t *header_pkt, uint32_t timeout ) {
	RPI_UART_ACK_Packet_t ackPacket;
	HAL_StatusTypeDef status;
	uint8_t i;

	HAL_UART_Abort(&huart7);

	for ( i = 0; i < RPI_UART_NUM_PKT_SEND_ATTEMPTS; i++ ) {
			/*-------------------------------------------------------------------------
			Send the header packet using HAL
			-------------------------------------------------------------------------*/
			status = HAL_UART_Transmit(&huart7, (uint8_t*)header_pkt, RPI_UART_HEADER_PACKET_SIZE, timeout);
			//HAL_Delay(1);
			/*-------------------------------------------------------------------------
			Send the data packet using HAL
			-------------------------------------------------------------------------*/
			status = HAL_UART_Transmit(&huart7, packetData, packetSize, timeout);

			/*-------------------------------------------------------------------------
			If we sent the packet successfully, wait for ack packet
			-------------------------------------------------------------------------*/
			if (status == HAL_OK) {
				HAL_Delay(3);

				/*---------------------------------------------------------------------
				Receive the ACK packet
				---------------------------------------------------------------------*/
				status = HAL_UART_Receive(&huart7, (uint8_t*)&ackPacket, RPI_UART_ACK_PACKET_SIZE, timeout);

				if (status == HAL_OK && ackPacket.packet_id == RPI_ACK_PKT_ID && ackPacket.ack == true ) {
					break;
				}
			}
		}
	return status;
}

static HAL_StatusTypeDef _send_uart_unix_time_packet( uint8_t *packetData, uint16_t packetSize, RPI_UART_Unix_Time_t *unixTimePacket, uint32_t timeout ) {
	HAL_StatusTypeDef status;
	uint8_t i;

	HAL_UART_Abort(&huart7);

	for ( i = 0; i < RPI_UART_NUM_PKT_SEND_ATTEMPTS; i++ ) {
			/*-------------------------------------------------------------------------
			Send the packet using HAL
			-------------------------------------------------------------------------*/
			status = HAL_UART_Transmit(&huart7, packetData, packetSize, timeout);

			/*-------------------------------------------------------------------------
			If we sent the packet successfully, wait for ack packet
			-------------------------------------------------------------------------*/
			if (status == HAL_OK) {
				HAL_Delay(3);

				/*---------------------------------------------------------------------
				Receive the Unix time packet
				---------------------------------------------------------------------*/
				status = HAL_UART_Receive(&huart7, (uint8_t*)unixTimePacket, RPI_UART_Unix_Time_SIZE, timeout);

				if (status == HAL_OK && unixTimePacket->packet_id == RPI_UNIX_TIME_PKT_ID) {
					break;
				}
			}
		}
	return status;

}
