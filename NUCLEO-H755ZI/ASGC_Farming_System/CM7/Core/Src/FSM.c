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
        NONE, we are waiting on the 'Start' button to be pressed

    Transitions out of this state:
        -> FSM_STATE_FILL_RESERVOIR
            When the start button has been pressed, transition to the first
            'real' state in the system

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/


SYS_RESULT FSM_State_WAITING_ON_START_SAF() {
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
        Task, Enable AS7341 Task

    Transitions out of this state:
        -> CNC_HOMING
            After 120 Seconds have elapsed, transition into CNC_HOMING

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

SYS_RESULT FSM_State_FILL_RESERVOIR_SAF() {
    FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateStartTimestamp = getTimestamp();
    // Open Fill Valve, Enable AHT20 Task, Enable SEN0169 Task, Enable SEN0244
    // Task, Enable AS7341 Task
    return SYS_SUCCESS;
}


SYS_RESULT FSM_State_FILL_RESERVOIR_TCF() {

    // If FSM_STATE_FILL_RESERVOIR_DWELL_TIME time has elapsed since entering state
    if ( ( getTimestamp() - FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateStartTimestamp ) >= FSM_STATE_FILL_RESERVOIR_DWELL_TIME) {
        // currentFSMState = [NEXT STATE]
        FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateActivated = false;
    }

    return SYS_SUCCESS;
}


/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    FSM_STATE_CNC_HOMING

    Transitions into this state: 
        -> FSM_STATE_FILL_RESERVOIR

    Action Upon State Activation:
        Send CNC Homing command to Raspberry Pi, Turn on circulating pump, ...

    Transitions out of this state:
        -> XXXXX
            XXXX

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

SYS_RESULT FSM_State_CNC_HOMING_SAF() {
    FSM_STATES[FSM_STATE_CNC_HOMING].stateStartTimestamp = getTimestamp();
    // Send CNC Homing command to Raspberry Pi, Turn on circulating pump, ...

    return SYS_SUCCESS;
}

SYS_RESULT FSM_State_CNC_HOMING_TCF() {

    return SYS_SUCCESS;
}
