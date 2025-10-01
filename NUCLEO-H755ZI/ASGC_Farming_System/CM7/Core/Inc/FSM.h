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
#include "main.h"
/*-----------------------------------------------------------------------------
DEFINES
-----------------------------------------------------------------------------*/

extern uint32_t unixTimeRefSec;
#define FSM_STATE_FILL_RESERVOIR_DWELL_TIME         120000

/*-----------------------------------------------------------------------------
TYPEDEFS
-----------------------------------------------------------------------------*/

typedef struct FSM_State_Struct{
    bool stateActivated;                        /* Has state activation function */
                                                /* ran yet?                      */
    uint64_t stateStartTimestamp;               /* Time that state was started   */
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
    FSM_STATE_CNC_HOMING,
    FSM_STATE_SEED_DISPENSE,
    FSM_STATE_GROWTH_MONITORING,
    // ...
    FSM_STATE_ESTOP_PRESSED,
    NUM_FSM_STATES
};

/*-----------------------------------------------------------------------------
FUNCTION DECLARATIONS
-----------------------------------------------------------------------------*/
SYS_RESULT FSM_Init();
void FSM_Update();

/* FSM_STATE_INIT */
SYS_RESULT FSM_State_INIT_TCF();

/* FSM_STATE_WAITING_ON_START */
SYS_RESULT FSM_State_WAITING_ON_START_SAF();
SYS_RESULT FSM_State_WAITING_ON_START_TCF();

/* FSM_STATE_FILL_RESERVOIR */
SYS_RESULT FSM_State_FILL_RESERVOIR_SAF();
SYS_RESULT FSM_State_FILL_RESERVOIR_TCF();

/* FSM_STATE_CNC_HOMING */
SYS_RESULT FSM_State_CNC_HOMING_SAF();
SYS_RESULT FSM_State_CNC_HOMING_TCF();

/* FSM_STATE_SEED_DISPENSE */
SYS_RESULT FSM_State_SEED_DISPENSE_SAF();
SYS_RESULT FSM_State_SEED_DISPENSE_TCF();

/* FSM_STATE_GROWTH_MONITORING */
SYS_RESULT FSM_State_GROWTH_MONITORING_SAF();
SYS_RESULT FSM_State_GROWTH_MONITORING_TCF();

uint64_t FSM_GetSystemUptime();

#endif /* INC_FSM_H */
