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

#ifndef __CNC_H
#define __CNC_H

#include "main.h"

#include <math.h>
#include "functionality_mngmnt.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>

#define CNC_NUM_NFT_CHANNELS 				4
                    	/* NOTE: Channel index increases with increasing y 	 */
						/* position in the system							 */
#define CNC_NUM_NET_POTS_PER_NFT_CHANNEL 	10
						/* NOTE: Hole index increases with increasing x pos  */
						/* in the system									 */

#define CNC_MAX_X_POS_MM 					2000.0
						/* Maximum X position of the CNC system in mm. 		 */
						/* Value TBD but should be 1940 < x < 2000			 */
#define CNC_MAX_Y_POS_MM 					500.0
						/* Maximum Y position of the CNC system in mm. 		 */
						/* Value TBD but should be ~500			 			 */


#define SEED_DISPENSER_X_OFFSET_MM 			9999.9
						/* X offset of the seed dispenser dispensing tube	 */
						/* from the absolute CNC Position. Value TBD		 */
#define SEED_DISPENSER_Y_OFFSET_MM 			9999.9
						/* Y offset of the seed dispenser dispensing tube	 */
						/* from the absolute CNC Position. Value TBD		 */
#define SEED_DISPENSER_MAX_X_POS_MM 		1980.0
						/* Maximum X position of the seed dispenser in mm.   */
						/* Value TBD				 						 */
#define SEED_DISPENSER_DOCK_X_POS_MM 		9999.9
						/* X position of the seed dispenser tool change 	 */
						/* docking station in the farming system. Value TBD	 */
#define SEED_DISPENSER_DOCK_Y_POS_MM 		9999.9
						/* Y position of the seed dispenser tool change 	 */
						/* docking station in the farming system. Value TBD	 */

typedef struct {
	float x_pos; 		/* Hole X position in mm 						 	 */
	float y_pos; 		/* Hole Y position in mm 						     */
	bool is_empty; 		/* True if net pot is not present in channel hole 	 */
} CNC_NFT_Channel_Hole_Data;

typedef struct {
	CNC_NFT_Channel_Hole_Data channel_holes[CNC_NUM_NFT_CHANNELS][CNC_NUM_NET_POTS_PER_NFT_CHANNEL];
	                    /* Two dimensional array for every NFT channel hole  */
						/* in the farming system 							 */

} CNC_NFT_Data;

/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/
SYS_RESULT CNC_Init(void);
uint8_t* CNC_Find_Hole_Closest_To_Position(float x_pos, float y_pos);


SYS_RESULT CNC_Home_Command(void);
SYS_RESULT CNC_Move_To_Pos(float x_pos, float y_pos);
SYS_RESULT CNC_Move_To_Hole(uint8_t channel_index, uint8_t hole_index);
SYS_RESULT CNC_Dispense_Seeds();


#endif /* __CNC_H */
