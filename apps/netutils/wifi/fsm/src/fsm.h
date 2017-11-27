/*!
        Copyright (C) CCL, CSR plc 2008
        All rights reserved

\file   fsm.h

\brief  This file defines the interface for two state machine
        engines. The most appropriate one should be chosen for each
        state machine based upon criteria such as code size and speed
        of execution. Both FSMs are table-driven.

        For in-depth information on how this works, see fsm_private.h.
*/

#ifndef _FSM_H
#define _FSM_H

//#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

//#include "types.h"

//#define FSM_DEBUG_NAMES 1
#define MXLOG_STRING(label, text) \
	static const char label[] = text

/**
 * Verify compile time constants
 *
 * Evaluate an expression at compile time and force the compilation
 * to abort if the expression is false.
 *
 * expr is the expression to evaluate. msg is a symbol name to try to
 * get into the error message (works only on some compilers).
 */
/*lint -emacro(506,COMPILE_TIME_ASSERT) */
/*lint -emacro(641,COMPILE_TIME_ASSERT) */
#define COMPILE_TIME_ASSERT(expr, msg) struct compile_time_assert_ ## msg \
{ \
    int compile_time_assert_ ## msg [(expr) ? 1 : -1]; \
}

#define INDIRECT_CALL(class, func) (func)
#define INDIRECT_CALL_TARGET(class) ((void)0)
#define CALL_ROOT(label) ((void)0)

/* Keep the magic numbers in one place */
#define TRANSITION32_NEW_STATE_SHIFT 0
#define TRANSITION32_NEW_STATE_BITS 9
#define TRANSITION32_ACTION_SHIFT \
    (TRANSITION32_NEW_STATE_SHIFT+TRANSITION32_NEW_STATE_BITS)
#define TRANSITION32_ACTION_BITS 9
#define TRANSITION32_EVENT_SHIFT (TRANSITION32_ACTION_SHIFT+TRANSITION32_ACTION_BITS)
#define TRANSITION32_EVENT_BITS (32 - TRANSITION32_EVENT_SHIFT)

/* The RE-POST event */
#define FSM32_EVENT_REPOST ((fsm_event_t) ((1<<TRANSITION32_EVENT_BITS)-2))
/* The ABORT event - for testing, cancel state-change */
#define FSM32_EVENT_ABORT ((fsm_event_t) ((1<<TRANSITION32_EVENT_BITS)-3))

/* The NULL event */
#define FSM32_EVENT_UNPAUSE ((fsm_event_t) 200)

/* The NULL action */
#define FSM32_ACTION_NULL ((fsm32_action_t) ((1<<TRANSITION32_ACTION_BITS)-1))
#define FSM32_ACTION_UNDEF FSM32_ACTION_NULL

#define TRANSITION16_NEW_STATE_SHIFT 0
#define TRANSITION16_NEW_STATE_BITS 4
#define TRANSITION16_ACTION_SHIFT \
    (TRANSITION16_NEW_STATE_SHIFT+TRANSITION16_NEW_STATE_BITS)
#define TRANSITION16_ACTION_BITS 6
#define TRANSITION16_EVENT_SHIFT (TRANSITION16_ACTION_SHIFT+TRANSITION16_ACTION_BITS)
#define TRANSITION16_EVENT_BITS (16 - TRANSITION16_EVENT_SHIFT)

/* Definitions for New hand bitfielded FSM */

/* The NULL action */
#define FSM16_ACTION_NULL ((fsm16_action_t) ((1<<TRANSITION16_ACTION_BITS)-1))
#define FSM16_ACTION_UNDEF FSM16_ACTION_NULL

/* Definitions for New hand bitfielded FSM */
/* The NULL event */
#define FSM_EVENT_NULL ((fsm_event_t) 0xFFFF)

/* Original FSM definitions */
/* Engine return values */
#define FSM_RESULT_FAIL ((fsm_result_t) 0)	/* Event was not processed */
#define FSM_RESULT_OK ((fsm_result_t) 1)	/* Event was processed */

/* Macro used to declare action functions */
#define FSM_ACTION_FN(fn) fsm_event_t fn(void *pv_arg1, void *pv_arg2)

/* Common code to extract a field from a transition */
#define TRANSITION32_FIELD(field, transition) \
    (uint16_t) (((transition) >> TRANSITION32_ ## field ## _SHIFT & \
                 ((1<< TRANSITION32_ ## field ## _BITS)-1)))
#define BUILD_TRANSITION32(event, action, new_state) \
    ((uint32_t)(event)     << TRANSITION32_EVENT_SHIFT | \
     (uint32_t)(action)    << TRANSITION32_ACTION_SHIFT | \
     (uint32_t)(new_state) << TRANSITION32_NEW_STATE_SHIFT)

#define TRANSITION16_FIELD(field, transition) \
    (((transition) >> TRANSITION16_ ## field ## _SHIFT & \
                 ((1<< TRANSITION16_ ## field ## _BITS)-1)))
#define BUILD_TRANSITION16(event, action, new_state) \
    ((uint16_t)(event)     << TRANSITION16_EVENT_SHIFT | \
     (uint16_t)(action)    << TRANSITION16_ACTION_SHIFT | \
     (uint16_t)(new_state) << TRANSITION16_NEW_STATE_SHIFT)

/* FSM basic data types */
typedef uint16_t fsm_event_t;
typedef uint16_t fsm32_state_t;
typedef uint16_t fsm32_action_t;
typedef uint8_t fsm16_state_t;
typedef uint8_t fsm16_action_t;

/* Type for returning an indication of whether an event was processed by
 * the engine */
typedef uint8_t fsm_result_t;

/* Type for specifying whether logging is required */
typedef uint8_t fsm_logging_t;

/* Action function pointer type */
typedef FSM_ACTION_FN((*fsm_action_fn_t));

/*
 * Sparse state machine definitions
 */

#if defined(STATEWATCH)
#define FSM_NAME_LOGGING
#endif

#ifdef FSM_NAME_LOGGING
#define FSM_LOG_INFO(fn, an, sn, en) , fn, an, sn, en
#else
#define FSM_LOG_INFO(fn, an, sn, en)
#endif

/* Sparse transition definition - an array of these is defined per state, and
 * must be terminated with FSM_EVENT_NULL.
 * To save constant space this is now 3 fields inside a uint32_t, we can
 * not use bitfields anymore as they straddle a 16 bit boundary and the
 * complier puts it in a new 16 bit word.
 * So We now have a 32 bit word defined as
 *
 *  31:18 event (14 bits)
 *  17:9  action (9 bits)
 *  8:0   state (9 bits)
 */
typedef struct {
	uint32_t trans;
} FSM_SPARSE32_TRANSITION_T;

/* Sparse state definition - this is built into an array of pointers to the
 * transition list for each state.
 */
typedef struct {
	const FSM_SPARSE32_TRANSITION_T *p_transitions;
} FSM_SPARSE32_STATES_T;

/* Sparse state machine definition - a pointer to this structure is passed to
 * the engine.
 */
typedef struct {
	const FSM_SPARSE32_STATES_T *p_table;	/* Pointer to state table */
	const fsm_action_fn_t *p_actions;	/* Pointer to action fn table */

#ifdef FSM_NAME_LOGGING
	/* Logging information - can be NULL */
	const char *const fsm_name;	/* Name of state machine */
	const char *const *const action_names;	/* Array of state names */
	const char *const *const state_names;	/* Array of state names */
	const char *const *const event_names;	/* Array of event names */
#endif
} FSM_SPARSE32_DEFINITION_T;

/* Sparse transition definition - an array of these is defined per state, and
 * must be terminated with FSM_EVENT_NULL.
 * To save constant space this is now 3 fields inside a uint16_t, we can
 * not use bitfields due to C99.
 * So we now have a 16 bit word defined as
 *
 *  15:10 event  (6 bits)
 *  9:4   action (6 bits)
 *  3:0   state  (4 bits)
 */
typedef struct {
	uint16_t trans;
} FSM_SPARSE16_TRANSITION_T;

/* Sparse state definition - this is built into an array of pointers to the
 * transition list for each state.
 */
typedef struct {
	const FSM_SPARSE16_TRANSITION_T *p_transitions;
} FSM_SPARSE16_STATES_T;

typedef struct {
	const FSM_SPARSE16_STATES_T *p_table;	/* Pointer to state table */
	const fsm_action_fn_t *p_actions;	/* Pointer to action fn table */

#ifdef FSM_NAME_LOGGING
	/* Logging information - can be NULL */
	const char *const fsm_name;	/* Name of state machine */
	const char *const *const action_names;	/* Array of state names */
	const char *const *const state_names;	/* Array of state names */
	const char *const *const event_names;	/* Array of event names */
#endif
} FSM_SPARSE16_DEFINITION_T;

/*! \brief fsm_32bit_run

    The engine function for a 32 bit bitfiled sparse state machine.
    Adheres to the "shake it until it stops rattling" principle,
    driving the state machine until there are no more events.

    \returns fsm_result_t - FSM_RESULT_OK if the event was processed
    FSM_RESULT_FAIL otherwise.

    Remember to tag the body of action functions with:
            INDIRECT_CALL_TARGET("fsm_action");
*/
extern fsm_result_t fsm_32bit_run(const FSM_SPARSE32_DEFINITION_T *p_fsm,	/* The FSM definition */
								  fsm32_state_t *p_state,	/* State variable */
								  fsm_event_t event,	/* Event to process */
								  void *pv_arg1,	/* Two void pointer arguments */
								  void *pv_arg2);

/*! \brief fsm_16bit_run

    The engine function for a 16 bit bitfiled sparse state machine.
    Adheres to the "shake it until it stops rattling" principle,
    driving the state machine until there are no more events.

    \returns fsm_result_t - FSM_RESULT_OK if the event was processed
    FSM_RESULT_FAIL otherwise.

    Remember to tag the body of action functions with:
            INDIRECT_CALL_TARGET("fsm_action");
*/
extern fsm_result_t fsm_16bit_run(const FSM_SPARSE16_DEFINITION_T *p_fsm,	/* The FSM definition */
								  fsm16_state_t *p_state,	/* State variable */
								  fsm_event_t event,	/* Event to process */
								  void *pv_arg1,	/* Two void pointer arguments */
								  void *pv_arg2);

#ifdef __cplusplus
}
#endif
#endif
