/*-----------------------------------------------------------------------------
 *
 * 	AS7341.h
 *
 *  Created on: 8/27/25
 *
 *  DESCRIPTION: AS7341 11-channel multi-spectral light sensor
 *
-----------------------------------------------------------------------------*/

#ifndef AS7341_H
#define AS7341

#include "main.h"
#include "functionality_mngmnt.h"

#define AS7341_I2C_ADDR_WRITE           (0x39 << 1)
#define AS7341_I2C_ADDR_READ            ((0x39 << 1) + 0x01)

#define AS7341_ENABLE_REGISTER          0x80    // I think we want to send 0b00000011 for initialization

typedef struct AS7341_Data {
uint8_t blankForNow;
}AS7341_Data_t;


#endif
