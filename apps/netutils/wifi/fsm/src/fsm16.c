/*!
        Copyright (C) CCL, CSR plc 2008
        All rights reserved

\file   fsm16.c

\brief  The 16bit version of the FSM.
*/

#include "fsm_private.h"
//#include "slsi_wifi_p2p_fsm_definitions.h"
#ifdef CONFIG_DEBUG_WLAN_API_DEBUG
extern const char *const slsi_wifi_p2p_state_names[];
extern const char *const slsi_wifi_p2p_event_names[];
#endif
/*! \brief fsm_16bit_run

    The engine function for a 16 bit bitfiled sparse state machine.
    Adheres to the "shake it until it stops rattling" principle,
    driving the state machine until there are no more events.

    \returns fsm_result_t - FSM_RESULT_OK if the event was processed
    FSM_RESULT_FAIL otherwise
*/
fsm_result_t fsm_16bit_run(const FSM_SPARSE16_DEFINITION_T *p_fsm,	/* The FSM definition */
						   fsm16_state_t *p_state,	/* State variable */
						   fsm_event_t event,	/* Event to process */
						   void *pv_arg1,	/* Two void pointer arguments */
						   void *pv_arg2)
{
	const FSM_SPARSE16_TRANSITION_T *p_transition, *p_end_trans;
	fsm16_action_t action_index;
	uint16_t orig_state;

#ifdef FSM_POINTER_CHECK
	/* Make sure p_state is in RAM as we need to write to it */
	if ((uintptr) p_state < (uintptr) RAM_START) {
		panic(PANIC_FSM_BAD_POINTER);
	}
#endif

	while (event != FSM_EVENT_NULL) {
		/* Point to the first transition defined for the current state */
		/* Search the current state's transition list for the event,
		 * terminating the search at FSM_EVENT_NULL
		 */
		for (p_transition = p_fsm->p_table[*p_state].p_transitions, p_end_trans = p_fsm->p_table[*p_state + 1].p_transitions;; p_transition++) {
			/* Have we reached the end of the list */
			if (p_transition == p_end_trans) {
				BTLOG_FSM("fsm_16bit_run - failed (state=%u, event=%u)", *p_state, event);
				/* Unhandled event */
				return FSM_RESULT_FAIL;
			}
			if (TRANSITION16_FIELD(EVENT, p_transition->trans) == event) {
				break;
			}
		}

		/* Cache the action index, we need it a couple of times. */
		action_index = TRANSITION16_FIELD(ACTION, p_transition->trans);

		orig_state = *p_state;
		/* State transition */
		*p_state = TRANSITION16_FIELD(NEW_STATE, p_transition->trans);
#ifdef CONFIG_DEBUG_WLAN_API_DEBUG
		printf("##### FSM State change [%s => %s] via event %s #####\n", slsi_wifi_p2p_state_names[orig_state], slsi_wifi_p2p_state_names[*p_state], slsi_wifi_p2p_event_names[event]);
#endif
		BTLOG_FSM("fsm_16bit_run (state=[%u->%u], event=%u, action=%u)", orig_state, *p_state, event, action_index);

		/* Call the action function if one is specified */
		if (action_index != FSM16_ACTION_NULL) {
			event = INDIRECT_CALL("fsm_action", p_fsm->p_actions[action_index](pv_arg1, pv_arg2));
		} else {
			event = FSM_EVENT_NULL;
		}
	}

	return FSM_RESULT_OK;
}
