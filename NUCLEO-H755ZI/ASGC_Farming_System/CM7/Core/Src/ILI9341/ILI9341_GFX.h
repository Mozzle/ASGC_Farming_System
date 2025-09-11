//	MIT License
//
//	Copyright (c) 2017 Matej Artnak
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
//
//
//
//-----------------------------------
//	ILI9341 GFX library for STM32
//-----------------------------------
//
//	Very simple GFX library built upon ILI9342_STM32_Driver library.
//	Adds basic shapes, image and font drawing capabilities to ILI9341
//
//	Library is written for STM32 HAL library and supports STM32CUBEMX. To use the library with Cube software
//	you need to tick the box that generates peripheral initialization code in their own respective .c and .h file
//
//
//-----------------------------------
//	How to use this library
//-----------------------------------
//
//	-If using MCUs other than STM32F7 you will have to change the #include "stm32f7xx_hal.h" in the ILI9341_GFX.h to your respective .h file
//
//	If using "ILI9341_STM32_Driver" then all other prequisites to use the library have allready been met
//	Simply include the library and it is ready to be used
//
//-----------------------------------

#ifndef ILI9341_GFX_H
#define ILI9341_GFX_H

#include "stm32h7xx_hal.h"

#define HORIZONTAL_IMAGE	0
#define VERTICAL_IMAGE		1

// Sensor values
static uint32_t temperatureValue = 9310; //Default to 93.1 F
static _Bool pumpStatusValue = true; //Default to ONLINE
static uint16_t dliValue = 0; //Default to 0%
static char* uptimeValue = "0d 0h 0m"; //Default to 0d 0h 0m 
static uint32_t waterTDSValue = 75000; //Default to 750.00 ppm
static uint16_t waterpHValue = 660; //Default to 6.6
static uint16_t humidityValue = 10000; //Default to 100.00%

// Aeroponics Project Drawing Methods
void Display_StartupScreen();
void Display_EStopScreen();
void Display_Dashboard(uint8_t page);
void Write_Logo();

/*
    Aeroponics Project Update Sensor Value Methods.
    Any values expected to be a decimal will be uint16_t or uint32_t and require a scaling factor of 100 (i.e. 500.43 F = 50043)
*/
void ILI9341_Update_DLI(uint16_t dliValueNew);
void ILI9341_Update_Temperature(uint32_t temperatureValueNew);
void ILI9341_Update_Humidity(uint16_t humidityValueNew);
void ILI9341_Update_WaterpH(uint16_t pHValueNew);
void ILI9341_Update_WaterTDS(uint32_t tdsValueNew);
void ILI9341_Update_Uptime(char* uptimeStringNew);
void ILI9341_Update_PumpStatus(_Bool isPumpOnlineNew);

void ILI9341_Draw_Hollow_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour);
void ILI9341_Draw_Filled_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour);
void ILI9341_Draw_Hollow_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour);
void ILI9341_Draw_Filled_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour);
void ILI9341_Draw_Char(char Character, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour);
void ILI9341_Draw_Text(const char* Text, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour);
void ILI9341_Draw_Filled_Rectangle_Size_Text(uint16_t X0, uint16_t Y0, uint16_t Size_X, uint16_t Size_Y, uint16_t Colour);

//USING CONVERTER: http://www.digole.com/tools/PicturetoC_Hex_converter.php
//65K colour (2Bytes / Pixel)
void ILI9341_Draw_Image(const char* Image_Array, uint8_t Orientation);

#endif
