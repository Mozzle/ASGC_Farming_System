/*-----------------------------------------------------------------------------
 *
 * 	Scheduler.h
 *
 * 		File used for scheudling tasks.
 *
 *  Created on: Sep 2, 2025
 *
-----------------------------------------------------------------------------*/

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

/*-----------------------------------------------------------------------------
INCLUDES
-----------------------------------------------------------------------------*/
#include "main.h"
#include "AHT20.h"
#include "FAN_pwm_intf.h"
#include "Buttons.h"
#include "timer.h"
#include "CNC.h"
#include "SEN0169.h"
#include "SEN0244.h"
#include "mixing_motor.h"
#include "gpio_switching_intf.h"
#include "Adafruit_AS7341.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "vl53l1_api.h"


/*-----------------------------------------------------------------------------
DEFINES
-----------------------------------------------------------------------------*/
#define AHT20_TASK_DEFAULT_INTERVAL_MS			30000
#define SEN0169_TASK_DEFAULT_INTERVAL_MS		30000
#define SEN0244_TASK_DEFAULT_INTERVAL_MS		30000
#define AS7341_TASK_DEFAULT_INTERVAL_MS			30000

/*-----------------------------------------------------------------------------
TYPEDEFS
-----------------------------------------------------------------------------*/

// Task structure
typedef struct Scheduler_Task{
	bool enabled;
	uint32_t interval_ms;
	uint64_t last_run_timestamp;
	uint8_t num_consecutive_failures;
	SYS_RESULT (*task_function)();
	SYS_RESULT (*failure_handler)(struct Scheduler_Task*);
}Scheduler_Task_t;

typedef uint8_t Scheduler_Task_ID_t;
enum {
	AHT20_GET_DATA_TASK,
	SEN0169_GET_DATA_TASK,
	SEN0244_GET_DATA_TASK,
	AS7341_GET_DATA_TASK, // Many more to add!
	NUM_SCHEDULER_TASKS
};

/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/
void Scheduler_Init();
void Scheduler_Update();
void Scheduler_Enable_Task(Scheduler_Task_ID_t task_id, uint32_t ms_delay);
void Scheduler_Disable_Task(Scheduler_Task_ID_t task_id);
void Scheduler_Set_Task_Interval(Scheduler_Task_ID_t task_id, uint32_t interval_ms);
void Scheduler_Set_Task_Function(Scheduler_Task_ID_t task_id, SYS_RESULT (*task_function)());
void Scheduler_Set_Task_Failure_Handler(Scheduler_Task_ID_t task_id, SYS_RESULT (*failure_handler)(struct Scheduler_Task*));


#endif /* INC_SCHEDULER_H_ */
