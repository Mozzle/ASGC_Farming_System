/*-----------------------------------------------------------------------------
 *
 * RPI_UART.h
 *
 * 		Header the UART packet structure and communication functions for
 * 		communication with the Raspberry Pi.
 *
 *  Created on: October 2, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef RPI_UART_H
#define RPI_UART_H

#include "AHT20.h"
#include "SEN0169.h"
#include "SEN0244.h"
#include <stdbool.h>
#include <stdio.h>

SYS_RESULT RPI_UART_Send_Gcode_Pkt( const char *gcode, uint32_t timeout );
SYS_RESULT RPI_UART_Send_AHT20_Pkt(AHT20_Data_t aht20_data, uint32_t timeout);
SYS_RESULT RPI_UART_Send_SEN0169_Pkt(SEN0169_pH_Data SEN0169_data, uint32_t timeout);
SYS_RESULT RPI_UART_Send_SEN0244_Pkt(SEN0244_TDS_Data SEN0244_data, uint32_t timeout);
SYS_RESULT RPI_UART_Send_AS7341_Pkt(uint16_t *AS7341_data, uint32_t timeout);
SYS_RESULT RPI_UART_Send_RPI_UNIX_TIME_REQUEST_Pkt(uint32_t timeout);

/*-----------------------------------------------------------------------------
Raspberry Pi Packets
-----------------------------------------------------------------------------*/

#define RPI_UART_NUM_PKT_SEND_ATTEMPTS	3

typedef uint8_t RPI_Packet_ID;
enum {
	RPI_ERR_PKT_ID				= 0,
	RPI_GCODE_PKT_ID,
	RPI_AHT20_PKT_ID,
	RPI_SEN0169_PKT_ID,
	RPI_SEN0244_PKT_ID,
	RPI_AS7341_PKT_ID,
	RPI_BUTTONS_PKT_ID,			// EStop and Start Button Data
	RPI_NET_POT_STATUS_PKT_ID,
	RPI_GET_AXES_POS_PKT_ID,
	RPI_ACK_PKT_ID,
	RPI_UNIX_TIME_REQUEST_PKT_ID,
	RPI_UNIX_TIME_PKT_ID,

	RPI_UART_NUM_PKT_IDS			// Number of packet IDs
};

/*-----------------------------------------------------------------------------
This pragma directive will forcibly pack the following packets, to remove the
need for byte padding
-----------------------------------------------------------------------------*/
#pragma pack(push, 1)

/*-----------------------------------------------------------------------------
Header Packet Definition
Send as the header of all other packets. Communicates information on incoming
packet size and type.
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_Header_Packet {
	RPI_Packet_ID packet_id;
} RPI_UART_Header_Packet_t;
#define RPI_UART_HEADER_PACKET_SIZE	sizeof(RPI_UART_Header_Packet_t)

/*-----------------------------------------------------------------------------
GCode Packet Definition
-----------------------------------------------------------------------------*/

#define RPI_UART_GCODE_MAX_STR_LEN		100

typedef struct RPI_UART_GCode_Packet {
	RPI_Packet_ID packet_id;
	bool valid;
	uint8_t gcode_str[RPI_UART_GCODE_MAX_STR_LEN];
} RPI_UART_Packet_GCode_t;

#define RPI_UART_GCODE_PACKET_SIZE	sizeof(RPI_UART_Packet_GCode_t)

/*-----------------------------------------------------------------------------
AHT20 Packet Definition
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_AHT20_Packet {
	RPI_Packet_ID packet_id;
	bool valid;	
	AHT20_Data_t aht20_data;
								
} RPI_UART_AHT20_Packet_t;

#define RPI_UART_AHT20_PACKET_SIZE	sizeof(RPI_UART_AHT20_Packet_t)

/*-----------------------------------------------------------------------------
SEN0169 Packet Definition
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_SEN0169_Packet {
	RPI_Packet_ID packet_id;	
	SEN0169_pH_Data SEN0169_data;	
									
} RPI_UART_SEN0169_Packet_t;

#define RPI_UART_SEN0169_PACKET_SIZE	sizeof(RPI_UART_SEN0169_Packet_t)

/*-----------------------------------------------------------------------------
SEN0244 Packet Definition
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_SEN0244_Packet {
	RPI_Packet_ID packet_id;		
	SEN0244_TDS_Data SEN0244_data;
									
} RPI_UART_SEN0244_Packet_t;

#define RPI_UART_SEN0244_PACKET_SIZE	sizeof(RPI_UART_SEN0244_Packet_t)

/*-----------------------------------------------------------------------------
AS7341 Packet Definition
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_AS7341_Packet {
	RPI_Packet_ID packet_id;
	uint16_t AS7341_data[12];

} RPI_UART_AS7341_Packet_t;

#define RPI_UART_AS7341_PACKET_SIZE		sizeof(RPI_UART_AS7341_Packet_t)

/*-----------------------------------------------------------------------------
UNIX time request packet
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_Unix_Time_Request {
	RPI_Packet_ID packet_id;

} RPI_UART_Unix_Time_Request_t;

#define RPI_UART_Unix_Time_Request_SIZE	sizeof(RPI_UART_Unix_Time_Request_t)

/*-----------------------------------------------------------------------------
UNIX time data packet
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_Unix_Time {
	RPI_Packet_ID packet_id;
	uint32_t UNIX_time_value;
	uint8_t Offset;

} RPI_UART_Unix_Time_t;

#define RPI_UART_Unix_Time_SIZE	sizeof(RPI_UART_Unix_Time_t)

/*-----------------------------------------------------------------------------
ACK Packet Definition
-----------------------------------------------------------------------------*/
typedef struct RPI_UART_ACK_Packet {
	RPI_Packet_ID packet_id;
	bool ack;
} RPI_UART_ACK_Packet_t;

#define RPI_UART_ACK_PACKET_SIZE	sizeof(RPI_UART_ACK_Packet_t)

#pragma pack(pop)


#endif /* RPI_UART_H */