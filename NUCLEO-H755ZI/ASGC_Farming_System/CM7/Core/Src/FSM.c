/*-----------------------------------------------------------------------------
 *
 * 	FSM.c
 *
 * 		The main (Finite State Machine) state controller of the ASGC Farming
 *      Robot
 *
 *  Created on: Sep 3, 2025
 *
-----------------------------------------------------------------------------*/

#include "FSM.h"
#include "timer.h"
#include "Scheduler.h"
#include "gpio_switching_intf.h"
#include "ILI9341/ILI9341_GFX.h"
#include "RPI_UART.h"

/*-----------------------------------------------------------------------------
STATIC VARIABLES
-----------------------------------------------------------------------------*/
struct FSM_State_Struct FSM_STATES[NUM_FSM_STATES];
FSM_State currentFSMState;

extern bool SYSTEM_START_STATE;

/*-----------------------------------------------------------------------------
 *
 * 		FSM_Init()
 *
 * 		Initializes the finite state machine's states
 *
 ----------------------------------------------------------------------------*/
SYS_RESULT FSM_Init() {
    currentFSMState = FSM_STATE_INIT;

    FSM_STATES[FSM_STATE_INIT].stateActivated = true;
    FSM_STATES[FSM_STATE_INIT].state_activation_funciton = NULL;
    FSM_STATES[FSM_STATE_INIT].transition_check_function = FSM_State_INIT_TCF;

    FSM_STATES[FSM_STATE_WAITING_ON_START].stateActivated = false;
    FSM_STATES[FSM_STATE_WAITING_ON_START].state_activation_funciton = FSM_State_WAITING_ON_START_SAF;
    FSM_STATES[FSM_STATE_WAITING_ON_START].transition_check_function = FSM_State_WAITING_ON_START_TCF;

    FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateActivated = false;
    FSM_STATES[FSM_STATE_FILL_RESERVOIR].state_activation_funciton = FSM_State_FILL_RESERVOIR_SAF;
    FSM_STATES[FSM_STATE_FILL_RESERVOIR].transition_check_function = FSM_State_FILL_RESERVOIR_TCF;

    FSM_STATES[FSM_STATE_CNC_HOMING].stateActivated = false;
    FSM_STATES[FSM_STATE_CNC_HOMING].state_activation_funciton = FSM_State_CNC_HOMING_SAF;
    FSM_STATES[FSM_STATE_CNC_HOMING].transition_check_function = FSM_State_CNC_HOMING_TCF;

    FSM_STATES[FSM_STATE_SEED_DISPENSE].stateActivated = false;
    FSM_STATES[FSM_STATE_SEED_DISPENSE].state_activation_funciton = FSM_State_SEED_DISPENSE_SAF;
    FSM_STATES[FSM_STATE_SEED_DISPENSE].transition_check_function = FSM_State_SEED_DISPENSE_TCF;

    FSM_STATES[FSM_STATE_GROWTH_MONITORING].stateActivated = false;
    FSM_STATES[FSM_STATE_GROWTH_MONITORING].state_activation_funciton = FSM_State_GROWTH_MONITORING_SAF;
    FSM_STATES[FSM_STATE_GROWTH_MONITORING].transition_check_function = FSM_State_GROWTH_MONITORING_TCF;

    FSM_STATES[FSM_STATE_ESTOP_PRESSED].stateActivated = false;
    FSM_STATES[FSM_STATE_ESTOP_PRESSED].state_activation_funciton = NULL;
    FSM_STATES[FSM_STATE_ESTOP_PRESSED].transition_check_function = NULL;

    return SYS_SUCCESS;
}


/*-----------------------------------------------------------------------------
 *
 * 		FSM_Update()
 *
 * 		The main update function of the Finite State Machine
 *
 ----------------------------------------------------------------------------*/
void FSM_Update() {
    if (currentFSMState >= NUM_FSM_STATES) {
        // This is probably an unrecoverable hard fault, unless if a lot of
        // resources are invested in making compelling fault recovery

        // Idea: Make a state that is very similar to E-STOP state, but with
        // "Unrecoverable fault" displayed
        return;
    }

    // If the current state's activation function has not been executed
    if (!FSM_STATES[currentFSMState].stateActivated && FSM_STATES[currentFSMState].state_activation_funciton != NULL) {
        FSM_STATES[currentFSMState].state_activation_funciton();
        FSM_STATES[currentFSMState].stateActivated = true;
    }

    // Run the current state's transition check
    if (FSM_STATES[currentFSMState].transition_check_function != NULL) {
        FSM_STATES[currentFSMState].transition_check_function();
    }

}

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    FSM_STATE_INIT

    Transitions into this state: 
        NONE, this is the entry point of the FSM

    Action Upon State Activation:
        NONE, this is the entry point of the FSM

    Transitions out of this state:
        -> FSM_STATE_WAITING_ON_START
            Unconditionally transition to this state once the FSM is
            initialized.

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

/*-----------------------------------------------------------------------------
 *
 * 		FSM_State_INIT_TCF()
 *
 * 		The transition check function for FSM_STATE_INIT
 *      This TCF simply unconditionally moves to FSM_STATE_WAITING_ON_START
 *
 ----------------------------------------------------------------------------*/
SYS_RESULT FSM_State_INIT_TCF() {
    currentFSMState = FSM_STATE_WAITING_ON_START;

    return SYS_SUCCESS;
}


/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    FSM_STATE_WAITING_ON_START

    Transitions into this state: 
        -> FSM_STATE_INIT

    Action Upon State Activation:
        Display the start screen on the ILI9341, we are waiting on the 'Start'
        button to be pressed

    Transitions out of this state:
        -> FSM_STATE_FILL_RESERVOIR
            When the start button has been pressed, transition to the first
            'real' state in the system

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/


SYS_RESULT FSM_State_WAITING_ON_START_SAF() {
    Display_StartupScreen();
    return SYS_SUCCESS;
}

SYS_RESULT FSM_State_WAITING_ON_START_TCF() {
    if (SYSTEM_START_STATE == SYSTEM_ON) {
        currentFSMState = FSM_STATE_FILL_RESERVOIR;
        FSM_STATES[FSM_STATE_WAITING_ON_START].stateActivated = false;
    }

    return SYS_SUCCESS;
}

/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    FSM_STATE_FILL_RESERVOIR

    Transitions into this state: 
        -> FSM_STATE_WAITING_ON_START

    Action Upon State Activation:
        Open Fill Valve, Enable AHT20 Task, Enable SEN0169 Task, Enable SEN0244
        Task, Enable AS7341 Task, enable ILI9341 dashboard tasks

    Transitions out of this state:
        -> CNC_HOMING
            After 120 Seconds have elapsed, transition into CNC_HOMING

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

SYS_RESULT FSM_State_FILL_RESERVOIR_SAF() {
    FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateStartTimestamp = getTimestamp();

    GPIO_set_fill_valve(VALVE_OPEN);

    ILI9341_Update_PumpStatus(PUMP_OFF);
    ILI9341_Update_Uptime(FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateStartTimestamp);
    ILI9431_Set_Current_Dashboard_Page(DASHBOARD_PAGE_TDS_PH_HUMIDITY);
    Write_Logo();
    Display_Dashboard();

    /*-------------------------------------------------------------------------
    Experimentally determined task time to complete, as of the commit AFTER 3757dc0:
    AHT20_REQUEST_MEASUREMENT_TASK: ~1ms
    AHT20_GET_DATA_TASK: ~1ms
    SEN0169_GET_DATA_TASK: ~2ms
    SEN0244_GET_DATA_TASK: ~1ms
    AS7341_GET_DATA_TASK: ~535ms
    Placing 35ms offset between tasks, as tasks may grow with implementation of
    data sharing with Raspberry Pi and Display.
    See the 'SW Task Timing' sheet in the ASGC_Automated_Farming_System 
    spreadsheet for a visualization of the task scheduling.
    -------------------------------------------------------------------------*/ 
    Scheduler_Enable_Task(AHT20_REQUEST_MEASUREMENT_TASK, 0);
    Scheduler_Enable_Task(SEN0169_GET_DATA_TASK, 5);
    Scheduler_Enable_Task(SEN0244_GET_DATA_TASK, 40);
    Scheduler_Enable_Task(AHT20_GET_DATA_TASK, 81);
    Scheduler_Enable_Task(ILI9341_CHANGE_DASHBOARD_SCREEN_TASK, 100);
    Scheduler_Enable_Task(AS7341_GET_DATA_TASK, 116);
    Scheduler_Enable_Task(ILI9341_UPDATE_UPTIME_TASK, 2);


    return SYS_SUCCESS;
}


SYS_RESULT FSM_State_FILL_RESERVOIR_TCF() {

    // If FSM_STATE_FILL_RESERVOIR_DWELL_TIME time has elapsed since entering state
    if ( ( getTimestamp() - FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateStartTimestamp ) >= FSM_STATE_FILL_RESERVOIR_DWELL_TIME) {
        currentFSMState = FSM_STATE_CNC_HOMING;
        FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateActivated = false;
    }

    return SYS_SUCCESS;
}


/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    FSM_STATE_CNC_HOMING

    Transitions into this state: 
        -> FSM_STATE_FILL_RESERVOIR

    Action Upon State Activation:
        Turn on circulating pump, close fill valve, send CNC Homing command to
        Raspberry Pi

    Transitions out of this state:
        -> XXXXX
            XXXX

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

SYS_RESULT FSM_State_CNC_HOMING_SAF() {
    FSM_STATES[FSM_STATE_CNC_HOMING].stateStartTimestamp = getTimestamp();

    GPIO_set_fill_valve(VALVE_CLOSED);
    GPIO_set_circulating_pump(PUMP_ON);
    ILI9341_Update_PumpStatus(PUMP_ON);
    CNC_Home_Command();

    return SYS_SUCCESS;
}

SYS_RESULT FSM_State_CNC_HOMING_TCF() {
    // Wait a short amount of time.
    // Send command to Raspberry Pi to verify homing is complete.
    // If homing is complete, transition to next state.
    // otherwise, wait another short amount of time and check again.
    return SYS_SUCCESS;
}


/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    FSM_STATE_SEED_DISPENSE

    Transitions into this state: 
        -> FSM_STATE_CNC_HOMING

    Action Upon State Activation:
        [NOT YET IMPLEMENTED]

    Transitions out of this state:
        -> XXXXX
            XXXX

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

SYS_RESULT FSM_State_SEED_DISPENSE_SAF() {
    FSM_STATES[FSM_STATE_SEED_DISPENSE].stateStartTimestamp = getTimestamp();

    // Request the unix time reference from the Raspberry Pi
    RPI_UART_Send_RPI_UNIX_TIME_REQUEST_Pkt(5);

    return SYS_SUCCESS;
}

SYS_RESULT FSM_State_SEED_DISPENSE_TCF() {
    /* IMPLEMENT ME */
    return SYS_SUCCESS;
}


/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    FSM_STATE_GROWTH_MONITORING

    Transitions into this state: 
        -> FSM_STATE_CNC_HOMING

    Action Upon State Activation:
        Enable the AS7341 midnight checker task

    Transitions out of this state:
        -> XXXXX
            XXXX

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

SYS_RESULT FSM_State_GROWTH_MONITORING_SAF() {
    FSM_STATES[FSM_STATE_SEED_DISPENSE].stateStartTimestamp = getTimestamp();

    // Once we are monitoring the seeds growth, we begin checking if we've hit midnight (for DLI integral purposes)
    Scheduler_Enable_Task(AS7341_CHECK_FOR_MIDNIGHT_TASK, 0);

    return SYS_SUCCESS;
}

SYS_RESULT FSM_State_GROWTH_MONITORING_TCF() {
    /* IMPLEMENT ME */
    return SYS_SUCCESS;
}


/*-----------------------------------------------------------------------------
 *
 * 		FSM_GetSystemUptime()
 *
 * 		Returns the number of milliseconds since FSM_STATE_FILL_RESERVOIR was
 *      activated. This state start time is what is displayed on the ILI9341
 *      as uptime.
 *
 ----------------------------------------------------------------------------*/

uint64_t FSM_GetSystemUptime() {
    return getTimestamp() - FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateStartTimestamp;
}
