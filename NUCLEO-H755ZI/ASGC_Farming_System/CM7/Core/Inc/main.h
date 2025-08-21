/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdbool.h>
#include "functionality_mngmnt.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define Start_Button_Pin GPIO_PIN_6
#define Start_Button_GPIO_Port GPIOC
#define Start_Button_EXTI_IRQn EXTI9_5_IRQn
#define Estop_ButtonNC_Pin GPIO_PIN_7
#define Estop_ButtonNC_GPIO_Port GPIOC
#define Estop_ButtonNC_EXTI_IRQn EXTI9_5_IRQn
#define Estop_ButtonNO_Pin GPIO_PIN_8
#define Estop_ButtonNO_GPIO_Port GPIOC
#define Estop_ButtonNO_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* ----------------------------------------------------------------------------
 * 		THIS IS THE FEATURE SWITCHBOARD. THE DEFINES BELOW DETERMINE WHETHER
 * 		OR NOT CERTAIN DEVICES ARE OR ARE NOT CONNECTED TO THE MICROCONTROLLER
 * 		IF THEY ARE NOT CONNECTED, CALLS TO SYSTEM OUTPUT FUNCTIONS (DISPLAYS,
 * 		MOTOR DRIVERS, ETC) WILL DO NOTHING. CALLS TO SYSTEM INPUTS (SENSORS)
 * 		WILL PRODUCE KNOWN INVALID VALUES.
 -----------------------------------------------------------------------------*/

/* ILI9341 LCD Display -------------------------------------------------------*/
#define ILI9341_ENABLED					        SYS_FEATURE_DISABLED


/* AHT20 Temperature and Humidity Sensor -------------------------------------*/
#define AHT20_ENABLED					          SYS_FEATURE_ENABLED
	 /* AHT20.c */

/* AS7341 Light Sensor -------------------------------------------------------*/
#define AS7341_ENABLED					        SYS_FEATURE_DISABLED


/* Computer Fan PWM Control Interface ----------------------------------------*/
#define FAN_PWM_INTERFACE_ENABLED		    SYS_FEATURE_ENABLED
	 /* FAN_pwm_intf.c */

/* Serial USB to Interface with CNC Control board ----------------------------*/
#define RASPBERRY_PI_INTERFACE_ENABLED	SYS_FEATURE_ENABLED
    /* CNC.c */


/* SEN0244 Electrical Conductivity Sensor ------------------------------------*/
#define SEN0244_ENABLED					        SYS_FEATURE_DISABLED
	 /* SEN0244.c */

/* SEN0169-V2 pH Sensor ------------------------------------------------------*/
#define SEN0169_ENABLED					        SYS_FEATURE_ENABLED
	 /* SEN0169.c */

/* VL53L1X Time-Of-Flight Sensor ---------------------------------------------*/
#define VL53L1X_ENABLED					        SYS_FEATURE_DISABLED


/* MOSFET/Solid State Relay switching interface for Pumps and Valves ---------*/
#define MOSFET_SSR_SWITCHING_ENABLED	  SYS_FEATURE_DISABLED
	 /* GPIO_switching_intf.c */

/* Mixing Motor Controller Interface -----------------------------------------*/
#define MIXING_MOTOR_ENABLED			      SYS_FEATURE_DISABLED
	 /* mixing_motor.c */

/* Start and E-Stop Button Interface -----------------------------------------*/
#define START_BUTTON_ENABLED			      SYS_FEATURE_DISABLED
#define ESTOP_BUTTON_ENABLED			      SYS_FEATURE_DISABLED
	 /* Buttons.h */

/*------------------------------------------------------------------------------
 * Normal Defines
------------------------------------------------------------------------------*/
#define SYSTEM_ON     true
#define SYSTEM_OFF    false

/*------------------------------------------------------------------------------
 * Function Declarations
------------------------------------------------------------------------------*/
void ASGC_System_Startup();
void ASGC_System_ESTOP();

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
