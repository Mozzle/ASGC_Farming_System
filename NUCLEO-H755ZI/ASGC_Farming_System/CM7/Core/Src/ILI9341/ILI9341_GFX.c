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


#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "5x5_font.h"
#include "stm32h7xx_hal_spi.h"
#include "main.h"

/*
	These variables are shared between Display_Dashboard() and Write_Logo()
	Write_Logo() should be called before Display_Dashboard to initalize these variables
*/
static uint8_t xBoundary;
static uint8_t yBoundary;

/*
	This method displays the startup screen
	This should ideally encourage the user to press the "Start" button
*/
void Display_StartupScreen()
{
	// Find the center of the screen
	uint16_t centerX = ILI9341_SCREEN_WIDTH / 2;
	uint16_t centerY = ILI9341_SCREEN_HEIGHT / 2;

	// Declare our text and font size
	const char *Text = "Press the START button";
	uint8_t fontSize = 3;

	// Find the pixel width and height of our text
	uint8_t TextPixelWidth = strlen(Text) * fontSize * CHAR_WIDTH;
	uint8_t TextPixelHeight = CHAR_HEIGHT * fontSize;

	// Calculate position to draw text at
	uint16_t drawX = centerX - (TextPixelWidth);
	uint16_t drawY = centerY - (TextPixelHeight / 2);

	// Draw the text
	ILI9341_Draw_Text(Text, drawX, drawY, WHITE, fontSize, BLACK);
}

/*
	This method displays the EStop screen
	This should ideally encourage the user to power cycle the system to restart
*/
void Display_EStopScreen()
{
		// Find the center of the screen
	uint16_t centerX = ILI9341_SCREEN_WIDTH / 2;
	uint16_t centerY = ILI9341_SCREEN_HEIGHT / 2;

	// Declare our text and font size
	const char *TextLine1 = "E-STOP PRESSED";
	const char *TextLine2 = "Power Cycle to Reset";
	uint8_t fontSize = 4;

	// Find the pixel width and height of our first line
	uint8_t Text1PixelWidth = strlen(TextLine1) * fontSize * CHAR_WIDTH;
	uint8_t Text1PixelHeight = CHAR_HEIGHT * fontSize;

	// Find the pixel width and height of our second line
	uint8_t Text2PixelWidth = strlen(TextLine2) * fontSize * CHAR_WIDTH;
	uint8_t Text2PixelHeight = Text1PixelHeight; // Same height as first line

	// Calculate where to draw our first line of text
	uint16_t drawX1 = centerX - (Text1PixelWidth);
	uint16_t drawY1 = centerY - (Text1PixelHeight);

	// Calculate where to draw our second line of text
	uint16_t drawX2 = centerX - (Text2PixelWidth);
	uint16_t drawY2 = centerY + (Text2PixelHeight / 2);

	// Draw the text
	ILI9341_Draw_Text(TextLine1, drawX1, drawY1, RED, fontSize, BLACK);
	ILI9341_Draw_Text(TextLine2, drawX2, drawY2, WHITE, fontSize, BLACK);
}

/*
	This method handles the actual display of sensor data via a dashboard/pages system
	There are a maximum of 3 pages (0, 1, 2)
	If an invalid integer/page number is passed, it will default to page 0

	Page 0: Water TDS, Water pH and Humidity
	Page 1: Temperature, Pump Status, Uptime and Daily Light Target
	Page 2+: (Reserved for future use)

	--- Example of sample data displayed ---
	Water TDS: 640ppm
	Water pH: 6.5
	Temperature: 88 F
	Humidity: 72%
	Daily Light Target: 65%
	(Maybe) [Status] Circulating Pump Running
	Uptime: 15d 16h
*/
void Display_Dashboard(uint8_t page)
{
	// If this is enabled, dummy info will be displayed instead of pulling from sensor data
	_Bool debugMode = false;

	// Calculate variables used for drawing
	uint16_t StartingXPos = 5;
	uint16_t StartingYPos = yBoundary + 10;
	uint8_t fontSize = 3;
	uint8_t displayValueFontSize = fontSize - 1;
	auto displayValueColor = WHITE;
	uint8_t TextPixelHeight = CHAR_HEIGHT * fontSize;
	uint8_t ScalingFactor = 100;

	/*
	Define static buffers that persist across several calls for this method
	This keeps our memory/makes sprintf calls not produce jumbled garbage text
	*/
    static char temperatureTextBuffer[20];
    static char lightLevelTextBuffer[20];
    static char waterTDSTextBuffer[20];
    static char waterpHTextBuffer[20];
    static char humidityTextBuffer[20];

	switch (page) {
		case 1:

			// Grab Temperature
			const char *temperatureText;
			sprintf(temperatureTextBuffer, "%u.%u F", temperatureValue / ScalingFactor, temperatureValue % ScalingFactor);
			temperatureText = temperatureTextBuffer;

			// Grab Light Level
			const char *lightLevelText;
			sprintf(lightLevelTextBuffer, "%u %%", dliValue);
			lightLevelText = lightLevelTextBuffer;

			// Grab Uptime
			const char *uptimeText;
			uptimeText = uptimeValue;

			// Grab pump status
			const char *pumpStatusText;
			auto pumpStatusColour = BLUE;
			if (pumpStatusValue || debugMode) {
				pumpStatusText = "ONLINE";
				pumpStatusColour = GREEN;
			} else {
				pumpStatusText = "OFFLINE";
				pumpStatusColour = RED;
			}

			// If debug mode is enabled, use dummy data
			if (debugMode) {
				temperatureText = "500.51 F";
				lightLevelText = "65%";
				uptimeText = "78d 21h 3m";
			}

			/// Draw the actual text ///
			// Temperature
			ILI9341_Draw_Text("Temp", StartingXPos, StartingYPos, BLUE, fontSize, BLACK);
			ILI9341_Draw_Text(temperatureText, StartingXPos, StartingYPos + (1*TextPixelHeight), displayValueColor, displayValueFontSize, BLACK);

			// Light Level
			ILI9341_Draw_Text("Light Level", StartingXPos, StartingYPos + (2*TextPixelHeight), BLUE, fontSize, BLACK);
			ILI9341_Draw_Text(lightLevelText, StartingXPos, StartingYPos + (3*TextPixelHeight), displayValueColor, displayValueFontSize, BLACK);

			// Pump Status
			ILI9341_Draw_Text("Pump Status", StartingXPos, StartingYPos + (4*TextPixelHeight), BLUE, fontSize, BLACK);
			ILI9341_Draw_Text(pumpStatusText, StartingXPos, StartingYPos + (5*TextPixelHeight), pumpStatusColour, displayValueFontSize, BLACK);

			// Uptime
			ILI9341_Draw_Text("Uptime", StartingXPos, StartingYPos + (6*TextPixelHeight), BLUE, fontSize, BLACK);
			ILI9341_Draw_Text(uptimeText, StartingXPos, StartingYPos + (7*TextPixelHeight), displayValueColor, displayValueFontSize, BLACK);
			break;
		default:

			// Grab Water TDS
			const char *waterTDSText;
			sprintf(waterTDSTextBuffer, "%u.%u ppm", waterTDSValue / ScalingFactor, waterTDSValue % ScalingFactor);
			waterTDSText = waterTDSTextBuffer;

			// Grab Water pH
			const char *waterpHText;
			sprintf(waterpHTextBuffer, "%u.%u", waterpHValue / ScalingFactor, waterpHValue % ScalingFactor);
			waterpHText = waterpHTextBuffer;

			// Grab Humidity Level
			const char *humidityText;
			sprintf(humidityTextBuffer, "%u.%u %%", humidityValue / ScalingFactor, humidityValue % ScalingFactor);
			humidityText = humidityTextBuffer;

			// If debug mode is enabled, use dummy data
			if (debugMode) {
				waterTDSText = "750 ppm";
				waterpHText = "6.0";
				humidityText = "66%";
			}

			/// Draw the actual text ///
			// Water TDS
			ILI9341_Draw_Text("Water TDS", StartingXPos, StartingYPos, BLUE, fontSize, BLACK);
			ILI9341_Draw_Text(waterTDSText, StartingXPos, StartingYPos + (1*TextPixelHeight), displayValueColor, displayValueFontSize, BLACK);

			// Water pH
			ILI9341_Draw_Text("Water pH", StartingXPos, StartingYPos + (2*TextPixelHeight), BLUE, fontSize, BLACK);
			ILI9341_Draw_Text(waterpHText, StartingXPos, StartingYPos + (3*TextPixelHeight), displayValueColor, displayValueFontSize, BLACK);

			// Humidity
			ILI9341_Draw_Text("Humidity", StartingXPos, StartingYPos + (4*TextPixelHeight), BLUE, fontSize, BLACK);
			ILI9341_Draw_Text(humidityText, StartingXPos, StartingYPos + (5*TextPixelHeight), displayValueColor, displayValueFontSize, BLACK);
			break;
	}
}

/*
	This method is simply prints the logo text at the top of the screen
	This mainly exists just for clarity/modularity
*/
void Write_Logo()
{
	// Define constants & calculate where to place our 'things'
	uint8_t fontSize = 4;
	uint8_t xOffset = 5;
	uint8_t yOffset = 5;
	char Text[] = "ASGC Farm";
	uint8_t TextPixelWidth = strlen(Text) * fontSize * CHAR_WIDTH;
	uint8_t TextPixelHeight = CHAR_HEIGHT * fontSize;

	xBoundary = xOffset + TextPixelWidth;
	yBoundary = yOffset + TextPixelHeight;

	// Draw 'things'
	ILI9341_Draw_Hollow_Rectangle_Coord(xOffset, yOffset, xBoundary, yBoundary, BLUE);
	ILI9341_Draw_Text(Text, xOffset, yOffset, RED, fontSize, BLACK);
}

void ILI9341_Update_DLI(uint16_t dliValueNew)
{
	dliValue = dliValueNew;
}

void ILI9341_Update_Temperature(uint32_t temperatureValueNew)
{
	temperatureValue = temperatureValueNew;
}

void ILI9341_Update_Humidity(uint16_t humidityValueNew)
{
	humidityValue = humidityValueNew;
}

void ILI9341_Update_WaterpH(uint16_t pHValueNew)
{
	waterpHValue = pHValueNew;
}

void ILI9341_Update_WaterTDS(uint32_t tdsValueNew)
{
	waterTDSValue = tdsValueNew;
}

void ILI9341_Update_Uptime(char *uptimeStringNew)
{
	uptimeValue = uptimeStringNew;
}

void ILI9341_Update_PumpStatus(_Bool isPumpOnlineNew)
{
	pumpStatusValue = isPumpOnlineNew;
}

/*Draw hollow circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ILI9341_Draw_Hollow_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{
	int x = Radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (Radius << 1);

    while (x >= y)
    {
        ILI9341_Draw_Pixel(X + x, Y + y, Colour);
        ILI9341_Draw_Pixel(X + y, Y + x, Colour);
        ILI9341_Draw_Pixel(X - y, Y + x, Colour);
        ILI9341_Draw_Pixel(X - x, Y + y, Colour);
        ILI9341_Draw_Pixel(X - x, Y - y, Colour);
        ILI9341_Draw_Pixel(X - y, Y - x, Colour);
        ILI9341_Draw_Pixel(X + y, Y - x, Colour);
        ILI9341_Draw_Pixel(X + x, Y - y, Colour);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += (-Radius << 1) + dx;
        }
    }
}

/*Draw filled circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ILI9341_Draw_Filled_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{
	
		int x = Radius;
    int y = 0;
    int xChange = 1 - (Radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = X - x; i <= X + x; i++)
        {
            ILI9341_Draw_Pixel(i, Y + y,Colour);
            ILI9341_Draw_Pixel(i, Y - y,Colour);
        }
        for (int i = X - y; i <= X + y; i++)
        {
            ILI9341_Draw_Pixel(i, Y + x,Colour);
            ILI9341_Draw_Pixel(i, Y - x,Colour);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
		//Really slow implementation, will require future overhaul
		//TODO:	https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles	
}

/*Draw a hollow rectangle between positions X0,Y0 and X1,Y1 with specified colour*/
void ILI9341_Draw_Hollow_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t 	X_length = 0;
	uint16_t 	Y_length = 0;
	uint8_t		Negative_X = 0;
	uint8_t 	Negative_Y = 0;
	float 		Calc_Negative = 0;
	
	Calc_Negative = X1 - X0;
	if(Calc_Negative < 0) Negative_X = 1;
	Calc_Negative = 0;
	
	Calc_Negative = Y1 - Y0;
	if(Calc_Negative < 0) Negative_Y = 1;
	
	
	//DRAW HORIZONTAL!
	if(!Negative_X)
	{
		X_length = X1 - X0;		
	}
	else
	{
		X_length = X0 - X1;		
	}
	ILI9341_Draw_Horizontal_Line(X0, Y0, X_length, Colour);
	ILI9341_Draw_Horizontal_Line(X0, Y1, X_length, Colour);
	
	
	
	//DRAW VERTICAL!
	if(!Negative_Y)
	{
		Y_length = Y1 - Y0;		
	}
	else
	{
		Y_length = Y0 - Y1;		
	}
	ILI9341_Draw_Vertical_Line(X0, Y0, Y_length, Colour);
	ILI9341_Draw_Vertical_Line(X1, Y0, Y_length, Colour);
	
	if((X_length > 0)||(Y_length > 0)) 
	{
		ILI9341_Draw_Pixel(X1, Y1, Colour);
	}
	
}

/*Draw a filled rectangle between positions X0,Y0 and X1,Y1 with specified colour*/
void ILI9341_Draw_Filled_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t 	X_length = 0;
	uint16_t 	Y_length = 0;
	uint8_t		Negative_X = 0;
	uint8_t 	Negative_Y = 0;
	int32_t 	Calc_Negative = 0;
	
	uint16_t X0_true = 0;
	uint16_t Y0_true = 0;
	
	Calc_Negative = X1 - X0;
	if(Calc_Negative < 0) Negative_X = 1;
	Calc_Negative = 0;
	
	Calc_Negative = Y1 - Y0;
	if(Calc_Negative < 0) Negative_Y = 1;
	
	
	//DRAW HORIZONTAL!
	if(!Negative_X)
	{
		X_length = X1 - X0;
		X0_true = X0;
	}
	else
	{
		X_length = X0 - X1;
		X0_true = X1;
	}
	
	//DRAW VERTICAL!
	if(!Negative_Y)
	{
		Y_length = Y1 - Y0;
		Y0_true = Y0;		
	}
	else
	{
		Y_length = Y0 - Y1;
		Y0_true = Y1;	
	}
	
	ILI9341_Draw_Rectangle(X0_true, Y0_true, X_length, Y_length, Colour);	
}

/*Draws a character (fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/
void ILI9341_Draw_Char(char Character, uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour) 
{
		uint8_t 	function_char;
    uint8_t 	i,j;
		
		function_char = Character;
		
    if (function_char < ' ') {
        Character = 0;
    } else {
        function_char -= 32;
		}
   	
		char temp[CHAR_WIDTH];
		for(uint8_t k = 0; k<CHAR_WIDTH; k++)
		{
		temp[k] = font[function_char][k];
		}
		
    // Draw pixels
		ILI9341_Draw_Rectangle(X, Y, CHAR_WIDTH*Size, CHAR_HEIGHT*Size, Background_Colour);
    for (j=0; j<CHAR_WIDTH; j++) {
        for (i=0; i<CHAR_HEIGHT; i++) {
            if (temp[j] & (1<<i)) {			
							if(Size == 1)
							{
              ILI9341_Draw_Pixel(X+j, Y+i, Colour);
							}
							else
							{
							ILI9341_Draw_Rectangle(X+(j*Size), Y+(i*Size), Size, Size, Colour);
							}
            }						
        }
    }
}

/*Draws an array of characters (MAX STRING SIZE OF 13 USUALLY -- fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/
void ILI9341_Draw_Text(const char* Text, uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour)
{
	// Calculate max characters we can fit on the screen
	uint8_t maxCharacters = (floor((ILI9341_SCREEN_WIDTH - X) / (Size * CHAR_WIDTH)));
	char* drawTextBuf = NULL;
	const char* drawText = Text;

	// Truncation check
	if (strlen(Text) > maxCharacters) {
		drawTextBuf = malloc(maxCharacters + 1);
		if (drawTextBuf == NULL) {
			return; // Exit if memory allocation failed
		}

		// Fill the drawTextBuf with the truncated version of Text
		strncpy(drawTextBuf, Text, maxCharacters);
		drawTextBuf[maxCharacters] = '\0'; // Null-terminator, my friend
		drawText = drawTextBuf;
	}

	const char* iter = drawText;
	while (*iter) {
		// Boundary check for x-axis
		if (X + CHAR_WIDTH * Size - 1 > ILI9341_SCREEN_WIDTH) {
			break;
		}

		// Draw the character, move to next position
		ILI9341_Draw_Char(*iter++, X, Y, Colour, Size, Background_Colour);
		X += CHAR_WIDTH * Size;
	}

	// After we've drawn everything, free the buffer we allocated previously (if we did use it)
	if (drawTextBuf != NULL) {
		free(drawTextBuf);
	}
}

/*Draws a full screen picture from flash. Image converted from RGB .jpeg/other to C array using online converter*/
//USING CONVERTER: http://www.digole.com/tools/PicturetoC_Hex_converter.php
//65K colour (2Bytes / Pixel)
void ILI9341_Draw_Image(const char* Image_Array, uint8_t Orientation)
{
	if(Orientation == SCREEN_HORIZONTAL_1)
	{
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_WIDTH,ILI9341_SCREEN_HEIGHT);
			
		HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi1, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);	
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	}
	else if(Orientation == SCREEN_HORIZONTAL_2)
	{
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_WIDTH,ILI9341_SCREEN_HEIGHT);
			
		HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi1, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);	
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	}
	else if(Orientation == SCREEN_VERTICAL_2)
	{
		ILI9341_Set_Rotation(SCREEN_VERTICAL_2);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_HEIGHT,ILI9341_SCREEN_WIDTH);
			
		HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi1, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);	
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	}
	else if(Orientation == SCREEN_VERTICAL_1)
	{
		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		ILI9341_Set_Address(0,0,ILI9341_SCREEN_HEIGHT,ILI9341_SCREEN_WIDTH);
			
		HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
		
		unsigned char Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
		{			
				for(uint32_t k = 0; k< BURST_MAX_SIZE; k++)
				{
					Temp_small_buffer[k]	= Image_Array[counter+k];		
				}						
				HAL_SPI_Transmit(&hspi1, (unsigned char*)Temp_small_buffer, BURST_MAX_SIZE, 10);	
				counter += BURST_MAX_SIZE;			
		}
		HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
	}
}


