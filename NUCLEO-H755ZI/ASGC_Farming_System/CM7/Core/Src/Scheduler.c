/*-----------------------------------------------------------------------------
 *
 * 	Scheduler.c
 *
 * 		File used for scheudling tasks.
 *
 *  Created on: Sep 2, 2025
 *
-----------------------------------------------------------------------------*/

#include "Scheduler.h"

/*-----------------------------------------------------------------------------
STATIC VARIABLES
-----------------------------------------------------------------------------*/
struct Scheduler_Task Task_List[NUM_SCHEDULER_TASKS];

/*-----------------------------------------------------------------------------
 *
 * 		Scheduler_Init()
 *
 * 		Initializes the scheduler module and the task list.
 *
 ----------------------------------------------------------------------------*/
 void Scheduler_Init() {
	// AHT20 Data Retrieval Task
	Task_List[AHT20_GET_DATA_TASK].enabled = false;
	Task_List[AHT20_GET_DATA_TASK].interval_ms = AHT20_TASK_DEFAULT_INTERVAL_MS;
	Task_List[AHT20_GET_DATA_TASK].last_run_timestamp = 0;
	Task_List[AHT20_GET_DATA_TASK].num_consecutive_failures = 0;
	Task_List[AHT20_GET_DATA_TASK].task_function = AHT20_Get_Data_TASK;
	Task_List[AHT20_GET_DATA_TASK].failure_handler = NULL; // Add failure handler later


	// SEN0169 pH Data Retrieval Task
	Task_List[SEN0169_GET_DATA_TASK].enabled = false;
	Task_List[SEN0169_GET_DATA_TASK].interval_ms = SEN0169_TASK_DEFAULT_INTERVAL_MS;
	Task_List[SEN0169_GET_DATA_TASK].last_run_timestamp = 0;
	Task_List[SEN0169_GET_DATA_TASK].num_consecutive_failures = 0;
	Task_List[SEN0169_GET_DATA_TASK].task_function = SEN0169_Get_Data_TASK;
	Task_List[SEN0169_GET_DATA_TASK].failure_handler = NULL; // Add failure handler later

	// SEN0244 TDS Data Retrieval Task
	Task_List[SEN0244_GET_DATA_TASK].enabled = false;
	Task_List[SEN0244_GET_DATA_TASK].interval_ms = SEN0244_TASK_DEFAULT_INTERVAL_MS;
	Task_List[SEN0244_GET_DATA_TASK].last_run_timestamp = 0;
	Task_List[SEN0244_GET_DATA_TASK].num_consecutive_failures = 0;
	Task_List[SEN0244_GET_DATA_TASK].task_function = SEN0244_Get_Data_TASK;
	Task_List[SEN0244_GET_DATA_TASK].failure_handler = NULL; // Add failure handler later

	// AS7341 Spectral Data Retrieval and DLI Calculation Task
	Task_List[AS7341_GET_DATA_TASK].enabled = false;
	Task_List[AS7341_GET_DATA_TASK].interval_ms = AS7341_TASK_DEFAULT_INTERVAL_MS;
	Task_List[AS7341_GET_DATA_TASK].last_run_timestamp = 0;
	Task_List[AS7341_GET_DATA_TASK].num_consecutive_failures = 0;
	Task_List[AS7341_GET_DATA_TASK].task_function = AS7341_Get_Data_TASK;
	Task_List[AS7341_GET_DATA_TASK].failure_handler = NULL; // Add failure handler later

	Task_List[CNC_DISPENSE_SEEDS_TASK].enabled = false;
	Task_List[CNC_DISPENSE_SEEDS_TASK].interval_ms = CNC_DISPENSE_SEEDS_TASK_DEFAULT_INTERVAL_MS;
	Task_List[CNC_DISPENSE_SEEDS_TASK].last_run_timestamp = 0;
	Task_List[CNC_DISPENSE_SEEDS_TASK].num_consecutive_failures = 0;
	Task_List[CNC_DISPENSE_SEEDS_TASK].task_function = CNC_Dispense_Seeds_TASK;
	Task_List[CNC_DISPENSE_SEEDS_TASK].failure_handler = NULL; // Add failure handler later
 }

/*-----------------------------------------------------------------------------
 *
 * 		Scheduler_Update()
 *
 * 		The main update function for the scheduler, called in the main loop.
 *
 ----------------------------------------------------------------------------*/
 void Scheduler_Update() {
	/*-------------------------------------------------------------------------
	Local Variables
	-------------------------------------------------------------------------*/
	Scheduler_Task_ID_t i;

	/*-------------------------------------------------------------------------
	For every task:
	-------------------------------------------------------------------------*/
	for (i = 0; i < NUM_SCHEDULER_TASKS; i++) {
		// If the task is enabled
		if ( Task_List[i].enabled ) {

			// If the task interval has elapsed since the last time it was ran successfully
			if ( (getTimestamp() - Task_List[i].last_run_timestamp) >= Task_List[i].interval_ms ) {

				// Run the task function. If the task function succeeds, update the last run timestamp
				if (Task_List[i].task_function() == SYS_SUCCESS) {
					Task_List[i].last_run_timestamp = getTimestamp();
					Task_List[i].num_consecutive_failures = 0;
				}
				// If the task function fails
				else {
					Task_List[i].num_consecutive_failures++;
				}

				// If the task has failed 5 times in a row, handle the failure
				if (Task_List[i].num_consecutive_failures >= 5) {
					// Call a task failure handler function here

					// This is just a placeholder to reset the timestamp for now
					Task_List[i].last_run_timestamp = getTimestamp();
					Task_List[i].num_consecutive_failures = 0;
				}
				
			}
		}
	}
 }


 /*-----------------------------------------------------------------------------
 *
 * 		Scheduler_Enable_Task()
 *
 * 		Enables a task in the scheduler.
 * 		task_id: The ID of the task to enable, e.g.: AHT20_GET_DATA_TASK
 * 		ms_delay: The delay in milliseconds before the task is first run.
 * 		If ms_delay is 0, the task will run immediately the next time 
 *    	Scheduler_Update() is called.
 *
 ----------------------------------------------------------------------------*/
 void Scheduler_Enable_Task(Scheduler_Task_ID_t task_id, uint32_t ms_delay) {
	 /*------------------------------------------------------------------------
	   LOCAL VARIABLES
	  -----------------------------------------------------------------------*/
	 uint64_t curTime;
	 uint64_t newLastRunTime;

	// If the task ID is valid
	if (task_id < NUM_SCHEDULER_TASKS) {
		curTime = getTimestamp();
		newLastRunTime = curTime - Task_List[task_id].interval_ms;

		// Protect from underflow of last_run_timestamp
		if (newLastRunTime > curTime) {
			newLastRunTime = ms_delay;
		}
		else {
			newLastRunTime = newLastRunTime + ms_delay;
		}

		Task_List[task_id].enabled = true;
		Task_List[task_id].last_run_timestamp = newLastRunTime;
		Task_List[task_id].num_consecutive_failures = 0;
	}
 }


/*-----------------------------------------------------------------------------
 *
 * 		Scheduler_Disable_Task()
 *
 * 		Prevents a task from being run by the scheduler.
 *
 ----------------------------------------------------------------------------*/
void Scheduler_Disable_Task(Scheduler_Task_ID_t task_id) {
	// If the task ID is valid
	if (task_id < NUM_SCHEDULER_TASKS) {
		Task_List[task_id].enabled = false;
	}
 }


 /*-----------------------------------------------------------------------------
 *
 * 		Scheduler_Set_Task_Interval()
 *
 * 		Sets the interval of a task in the scheduler.
 *
 ----------------------------------------------------------------------------*/
 void Scheduler_Set_Task_Interval(Scheduler_Task_ID_t task_id, uint32_t interval_ms) {
	// If the task ID is valid
	if (task_id < NUM_SCHEDULER_TASKS) {
		Task_List[task_id].interval_ms = interval_ms;
	}
 }


 /*-----------------------------------------------------------------------------
 *
 * 		Scheduler_Set_Task_Function()
 *
 * 		Sets the task function of a task in the scheduler.
 * 		task_id: The ID of the task to modify, e.g.: AHT20_GET_DATA_TASK
 * 		task_function: A pointer to a _TASK function (returns SYS_RESULT, 
 * 		takes no args)
 *
 ----------------------------------------------------------------------------*/
 void Scheduler_Set_Task_Function(Scheduler_Task_ID_t task_id, SYS_RESULT (*task_function)()) {
	// If the task ID is valid
	if (task_id < NUM_SCHEDULER_TASKS) {
		Task_List[task_id].task_function = task_function;
	}
 }


 /*-----------------------------------------------------------------------------
 *
 * 		Scheduler_Set_Task_Failure_Handler()
 *
 * 		Sets the task function of a task in the scheduler.
 * 		task_id: The ID of the task to modify, e.g.: AHT20_GET_DATA_TASK
 * 		failure_handler: A pointer to a failure handler function (returns 
 * 		SYS_RESULT, takes a pointer to the failed Scheduler_Task_t as an argument)
 *
 ----------------------------------------------------------------------------*/
 void Scheduler_Set_Task_Failure_Handler(Scheduler_Task_ID_t task_id, SYS_RESULT (*failure_handler)(struct Scheduler_Task*)) {
	// If the task ID is valid
	if (task_id < NUM_SCHEDULER_TASKS) {
		Task_List[task_id].failure_handler = failure_handler;
	}
 }
