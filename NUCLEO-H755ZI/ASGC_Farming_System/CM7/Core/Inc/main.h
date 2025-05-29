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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* ----------------------------------------------------------------------------
 * 		THIS IS THE FEATURE SWITCHBOARD. THE DEFINES BELOW DETERMINE WHETHER
 * 		OR NOT CERTAIN DEVICES ARE OR ARE NOT CONNECTED TO THE MICROCONTROLLER
 * 		IF THEY ARE NOT CONNECTED, CALLS TO SYSTEM OUTPUT FUNCTIONS (DISPLAYS,
 * 		MOTOR DRIVERS, ETC) WILL DO NOTHING. CALLS TO SYSTEM INPUTS (SENSORS)
 * 		WILL PRODUCE KNOWN INVALID VALUES.
 -----------------------------------------------------------------------------*/

/* ILI9341 LCD Display -------------------------------------------------------*/
#define ILI9341_ENABLED					SYS_FEATURE_DISABLED

/* AHT20 Temperature and Humidity Sensor -------------------------------------*/
#define AHT20_ENABLED					SYS_FEATURE_ENABLED

/* AS7341 Light Sensor -------------------------------------------------------*/
#define AS7341_ENABLED					SYS_FEATURE_DISABLED

/* Computer Fan PWM Control Interface ----------------------------------------*/
#define FAN_PWM_INTERFACE_ENABLED		SYS_FEATURE_DISABLED

/* Serial USB to Interface with CNC Control board ----------------------------*/
#define SKR_MINI_USB_INTERFACE_ENABLED	SYS_FEATURE_DISABLED

/* Serial USB to Interface with CNC Control board ----------------------------*/
#define SKR_MINI_USB_INTERFACE_ENABLED	SYS_FEATURE_DISABLED

/* SEN0244 Electrical Conductivity Sensor ------------------------------------*/
#define SEN0244_ENABLED					SYS_FEATURE_DISABLED

/* SEN0169-V2 pH Sensor ------------------------------------------------------*/
#define SEN0169_ENABLED					SYS_FEATURE_DISABLED

/* VL53L1X Time-Of-Flight Sensor ---------------------------------------------*/
#define VL53L1X_ENABLED					SYS_FEATURE_DISABLED

/* MOSFET/Solid State Relay switching interface for Pumps and Valves ---------*/
#define MOSFET_SSR_SWITCHING_ENABLED	SYS_FEATURE_DISABLED

/* Mixing Motor Controller Interface -----------------------------------------*/
#define MIXING_MOTOR_ENABLED			SYS_FEATURE_DISABLED

/* Start and E-Stop Button Interface -----------------------------------------*/
#define START_BUTTON_ENABLED			SYS_FEATURE_DISABLED
#define ESTOP_BUTTON_ENABLED			SYS_FEATURE_DISABLED

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
