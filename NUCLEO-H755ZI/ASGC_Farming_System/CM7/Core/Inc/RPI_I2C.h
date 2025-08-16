/*-----------------------------------------------------------------------------
 *
 * RPI_I2C.h
 *
 * 		Header containing I2C address and the I2C packet
 * 		structure for communication with the Raspberry Pi.
 *
 *  Created on: August 12, 2025
 *      Author: Dylan
 *
-----------------------------------------------------------------------------*/

#ifndef I2C_PACKETS_H
#define I2C_PACKETS_H

#include "AHT20.h"
#include <stdbool.h>
#include <stdio.h>

/*-----------------------------------------------------------------------------
Raspberry Pi Addresses
-----------------------------------------------------------------------------*/
#define RPI_I2C_ADDR_WRITE				(0x09 << 1) // 0x12
// To be determined
//#define RPI_I2C_ADDR_READ				0x00

/*-----------------------------------------------------------------------------
Raspberry Pi Packets
-----------------------------------------------------------------------------*/
// All RPI packets are 128 bytes long
#define RPI_I2C_MAX_PACKET_SIZE 		128
// Number of attempts to send a packet before giving up
#define RPI_I2C_NUM_PKT_SEND_ATTEMPTS	3

typedef uint8_t RPI_Packet_ID;
enum {
	RPI_ERR_PKT_ID				= 0,
	RPI_GCODE_0_PKT_ID,
	RPI_GCODE_1_PKT_ID,
	RPI_GCODE_2_PKT_ID,
	RPI_GCODE_3_PKT_ID,
	RPI_GCODE_4_PKT_ID,
	RPI_AHT20_PKT_ID,
	RPI_WATER_DATA_PKT_ID,		// Water EC and pH Data
	RPI_BUTTONS_PKT_ID,			// EStop and Start Button Data
	RPI_NET_POT_STATUS_PKT_ID,
	RPI_GET_AXES_POS_PKT_ID,
	RPI_ACK_PKT_ID,

	RPI_I2C_NUM_PKT_IDS			// Number of packet IDs
};

/*-----------------------------------------------------------------------------
GCode Packet Definitions
-----------------------------------------------------------------------------*/
#define RPI_I2C_GCODE_MAX_STR_LEN		74

typedef struct RPI_I2C_GCode_0_Packet {
	RPI_Packet_ID packet_id;
	bool valid;
	uint8_t gcode_str[14];
} RPI_I2C_Packet_GCode_0_t;

#define RPI_I2C_GCODE_0_PACKET_SIZE	sizeof(RPI_I2C_Packet_GCode_0_t)

typedef struct RPI_I2C_GCode_1_Packet {
	RPI_Packet_ID packet_id;
	uint8_t gcode_str[15];
} RPI_I2C_Packet_GCode_1_t;

#define RPI_I2C_GCODE_1_PACKET_SIZE	sizeof(RPI_I2C_Packet_GCode_1_t)

typedef struct RPI_I2C_GCode_2_Packet {
	RPI_Packet_ID packet_id;
	uint8_t gcode_str[15];
} RPI_I2C_Packet_GCode_2_t;

#define RPI_I2C_GCODE_2_PACKET_SIZE	sizeof(RPI_I2C_Packet_GCode_2_t)

typedef struct RPI_I2C_GCode_3_Packet {
	RPI_Packet_ID packet_id;
	uint8_t gcode_str[15];
} RPI_I2C_Packet_GCode_3_t;

#define RPI_I2C_GCODE_3_PACKET_SIZE	sizeof(RPI_I2C_Packet_GCode_3_t)

typedef struct RPI_I2C_GCode_4_Packet {
	RPI_Packet_ID packet_id;
	uint8_t gcode_str[15];
} RPI_I2C_Packet_GCode_4_t;

#define RPI_I2C_GCODE_4_PACKET_SIZE	sizeof(RPI_I2C_Packet_GCode_4_t)

/*-----------------------------------------------------------------------------
AHT20 Packet Definition
-----------------------------------------------------------------------------*/
typedef struct RPI_I2C_AHT20_Packet {
	RPI_Packet_ID packet_id;
	bool valid;
	uint8_t pad1[2];			// Structs (and most other multi-byte data types) must be aligned on memory addresses
	AHT20_Data_t aht20_data;	// that are a multiple of 4. So we add this 2-byte padding after the uint8 and bool to
								// be explicit about what the data structure looks like.
} RPI_I2C_AHT20_Packet_t;

#define RPI_I2C_AHT20_PACKET_SIZE	sizeof(RPI_I2C_AHT20_Packet_t)

/*-----------------------------------------------------------------------------
ACK Packet Definition
-----------------------------------------------------------------------------*/
typedef struct RPI_I2C_ACK_Packet {
	RPI_Packet_ID packet_id;
	bool ack;
} RPI_I2C_ACK_Packet_t;

#define RPI_I2C_ACK_PACKET_SIZE	sizeof(RPI_I2C_ACK_Packet_t)


/*-----------------------------------------------------------------------------
Function Prototypes
-----------------------------------------------------------------------------*/
SYS_RESULT RPI_I2C_Send_Gcode_Pkt(const char *gcode, uint32_t timeout);

#endif // I2C_PACKETS_H
