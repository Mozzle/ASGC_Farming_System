/*-----------------------------------------------------------------------------
 *
 * 	FSM.h
 *
 * 		The main (Finite State Machine) state controller of the ASGC Farming
 *      Robot
 *
 *  Created on: Sep 3, 2025
 *
-----------------------------------------------------------------------------*/

#ifndef INC_FSM_H
#define INC_FSM_H

/*-----------------------------------------------------------------------------
INCLUDES
-----------------------------------------------------------------------------*/
#include "functionality_mngmnt.h"
/*-----------------------------------------------------------------------------
DEFINES
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
TYPEDEFS
-----------------------------------------------------------------------------*/

typedef struct FSM_State_Struct{
    bool stateActivated;                        /* Has state activation function */
                                                /* ran yet?                      */
    SYS_RESULT (*state_activation_funciton)();  /* Function to run when          */
                                                /* transitioning to this state   */
    SYS_RESULT (*transition_check_function)();  /* Periodic function to check for*/
                                                /* transitions out of this state */
}FSM_State_Struct_t;

typedef uint16_t FSM_State;
enum {
    FSM_STATE_INIT,
    FSM_STATE_WAITING_ON_START,
    FSM_STATE_FILL_RESERVOIR,
    // ...
    FSM_STATE_ESTOP_PRESSED,
    NUM_FSM_STATES
};

/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/
SYS_RESULT FSM_Init();
void FSM_Update();


#endif /* INC_FSM_H */