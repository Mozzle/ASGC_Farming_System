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
#include "functionality_mngmnt.h"
#include "stm32h7xx_hal_spi.h"
#include <stdbool.h>

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
#define L298_IN2_Pin GPIO_PIN_7
#define L298_IN2_GPIO_Port GPIOF
#define L298_IN1_Pin GPIO_PIN_9
#define L298_IN1_GPIO_Port GPIOF
#define ADC2_SEN0244_Pin GPIO_PIN_0
#define ADC2_SEN0244_GPIO_Port GPIOC
#define ILI9341_CS_Pin GPIO_PIN_4
#define ILI9341_CS_GPIO_Port GPIOA
#define ADC1_SEN0169_Pin GPIO_PIN_6
#define ADC1_SEN0169_GPIO_Port GPIOA
#define CIRCULATING_PUMP_Pin GPIO_PIN_10
#define CIRCULATING_PUMP_GPIO_Port GPIOE
#define DRAINAGE_PUMP_Pin GPIO_PIN_11
#define DRAINAGE_PUMP_GPIO_Port GPIOE
#define FILL_VALVE_Pin GPIO_PIN_12
#define FILL_VALVE_GPIO_Port GPIOE
#define NUTRIENT_SOLN_A_Pin GPIO_PIN_13
#define NUTRIENT_SOLN_A_GPIO_Port GPIOE
#define NUTRIENT_SOLN_B_Pin GPIO_PIN_14
#define NUTRIENT_SOLN_B_GPIO_Port GPIOE
#define PH_UP_VALVE_Pin GPIO_PIN_15
#define PH_UP_VALVE_GPIO_Port GPIOE
#define PH_DOWN_VALVE_Pin GPIO_PIN_10
#define PH_DOWN_VALVE_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define TIM4_MOTOR_PWM_Pin GPIO_PIN_12
#define TIM4_MOTOR_PWM_GPIO_Port GPIOD
#define Start_Button_Pin GPIO_PIN_6
#define Start_Button_GPIO_Port GPIOC
#define Start_Button_EXTI_IRQn EXTI9_5_IRQn
#define Estop_ButtonNC_Pin GPIO_PIN_7
#define Estop_ButtonNC_GPIO_Port GPIOC
#define Estop_ButtonNC_EXTI_IRQn EXTI9_5_IRQn
#define Estop_ButtonNO_Pin GPIO_PIN_8
#define Estop_ButtonNO_GPIO_Port GPIOC
#define Estop_ButtonNO_EXTI_IRQn EXTI9_5_IRQn
#define ILI9341_DC_Pin GPIO_PIN_8
#define ILI9341_DC_GPIO_Port GPIOA
#define ILI9341_RST_Pin GPIO_PIN_9
#define ILI9341_RST_GPIO_Port GPIOA
#define TIM3_FANS_PWM_Pin GPIO_PIN_4
#define TIM3_FANS_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* ----------------------------------------------------------------------------
 * 		THIS IS THE FEATURE SWITCHBOARD. THE DEFINES BELOW DETERMINE WHETHER
 * 		OR NOT CERTAIN DEVICES ARE OR ARE NOT CONNECTED TO THE MICROCONTROLLER
 * 		IF THEY ARE NOT CONNECTED, CALLS TO SYSTEM OUTPUT FUNCTIONS (DISPLAYS,
 * 		MOTOR DRIVERS, ETC) WILL DO NOTHING. CALLS TO SYSTEM INPUTS (SENSORS)
 * 		WILL PRODUCE KNOWN INVALID VALUES.
 -----------------------------------------------------------------------------*/

/* ILI9341 LCD Display -------------------------------------------------------*/
#define ILI9341_ENABLED					              SYS_FEATURE_ENABLED


/* AHT20 Temperature and Humidity Sensor -------------------------------------*/
#define AHT20_ENABLED					                SYS_FEATURE_ENABLED
	 /* AHT20.c */

/* AS7341 Light Sensor -------------------------------------------------------*/
#define AS7341_ENABLED					              SYS_FEATURE_ENABLED


/* Computer Fan PWM Control Interface ----------------------------------------*/
#define FAN_PWM_INTERFACE_ENABLED		          SYS_FEATURE_DISABLED
	 /* FAN_pwm_intf.c */

/* Computer Fan PWM Control Interface ----------------------------------------*/
#define SHUTTER_SERVO_PWM_INTERFACE_ENABLED		SYS_FEATURE_DISABLED
	 /* PWM.c */

/* Computer Fan PWM Control Interface ----------------------------------------*/
#define VLIFTER_SERVO_PWM_INTERFACE_ENABLED		SYS_FEATURE_DISABLED
	 /* PWM.c */

/* Serial USB to Interface with CNC Control board ----------------------------*/
#define RASPBERRY_PI_INTERFACE_ENABLED	      SYS_FEATURE_DISABLED
    /* CNC.c */


/* SEN0244 Electrical Conductivity Sensor ------------------------------------*/
#define SEN0244_ENABLED					              SYS_FEATURE_ENABLED
	 /* SEN0244.c */

/* SEN0169-V2 pH Sensor ------------------------------------------------------*/
#define SEN0169_ENABLED					              SYS_FEATURE_ENABLED
	 /* SEN0169.c */

/* VL53L1X Time-Of-Flight Sensor ---------------------------------------------*/
#define VL53L1X_ENABLED					              SYS_FEATURE_DISABLED


/* MOSFET/Solid State Relay switching interface for Pumps and Valves ---------*/
#define MOSFET_SSR_SWITCHING_ENABLED	        SYS_FEATURE_DISABLED
	 /* GPIO_switching_intf.c */

/* Mixing Motor Controller Interface -----------------------------------------*/
#define MIXING_MOTOR_ENABLED			            SYS_FEATURE_DISABLED
	 /* mixing_motor.c */

/* Start and E-Stop Button Interface -----------------------------------------*/
#define START_BUTTON_ENABLED			            SYS_FEATURE_ENABLED
#define ESTOP_BUTTON_ENABLED			            SYS_FEATURE_ENABLED
	 /* Buttons.h */

/*------------------------------------------------------------------------------
 * Normal Defines
------------------------------------------------------------------------------*/
#define SYSTEM_ON     true
#define SYSTEM_OFF    false

/*------------------------------------------------------------------------------
 * Defines for DispenseSeeds Helper Function
------------------------------------------------------------------------------*/
#define DISPENSE_MOVING_MS        (uint16_t)2000  // Max time to wait for gantry to move to hole in ms
#define DISPENSE_SHUTTER_MS       (uint16_t)500   // Time to keep shutter open in ms
#define DISPENSE_FINISH_MS        (uint16_t)5000  // Time to wait after dispensing is finished
#define DISPENSE_SUCCESS          (uint8_t)0
#define DISPENSE_FAIL_GANTRY      (uint8_t)1
#define DISPENSE_FAIL_SHUTTER     (uint8_t)2
#define DISPENSE_FAIL_XY_POS      (uint8_t)3
#define DISPENSE_BUSY             (uint8_t)4
#define DISPENSE_UNKNOWN_FAIL     (uint8_t)5

/*------------------------------------------------------------------------------
 * Function Declarations
------------------------------------------------------------------------------*/
void      ASGC_System_Startup();
void      ASGC_System_ESTOP();
uint8_t   DispenseSeeds_HelperFunc(float holeX, float holeY);
uint16_t  ASGC_System_DispenseSeeds();
void      Reset_DispenseSeeds_FSM();

// TASK FUNCTIONS - These are all the functions that will be registered in Scheduler.c
SYS_RESULT AHT20_Request_Measurement_TASK();
SYS_RESULT AHT20_Get_Data_TASK();
SYS_RESULT SEN0169_Get_Data_TASK();
SYS_RESULT SEN0244_Get_Data_TASK();
SYS_RESULT AS7341_Get_Data_TASK();
SYS_RESULT AS7341_Is_Midnight_TASK();
SYS_RESULT CNC_Dispense_Seeds_TASK();
SYS_RESULT ILI9341_Change_Dashboard_Screen_TASK();
SYS_RESULT ILI9341_Update_Uptime_TASK();

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
