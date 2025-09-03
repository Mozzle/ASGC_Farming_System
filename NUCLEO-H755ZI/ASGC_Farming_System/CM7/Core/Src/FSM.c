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

/*-----------------------------------------------------------------------------
STATIC VARIABLES
-----------------------------------------------------------------------------*/
struct FSM_State_Struct FSM_STATES[NUM_FSM_STATES];
FSM_State currentFSMState;

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
    FSM_STATES[FSM_STATE_INIT].transition_check_function = NULL;

    FSM_STATES[FSM_STATE_WAITING_ON_START].stateActivated = false;
    FSM_STATES[FSM_STATE_WAITING_ON_START].state_activation_funciton = NULL;
    FSM_STATES[FSM_STATE_WAITING_ON_START].transition_check_function = NULL;

    FSM_STATES[FSM_STATE_FILL_RESERVOIR].stateActivated = false;
    FSM_STATES[FSM_STATE_FILL_RESERVOIR].state_activation_funciton = NULL;
    FSM_STATES[FSM_STATE_FILL_RESERVOIR].transition_check_function = NULL;

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

SYS_RESULT FSM_State_Init_TCF() {

}
