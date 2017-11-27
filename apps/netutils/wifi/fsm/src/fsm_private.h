/*!
        Copyright (C) CCL, CSR plc 2008
        All rights reserved

\file   fsm_private.h

\brief  This file defines the interface for two state machine
        engines. The most appropriate one should be chosen for each
        state machine based upon criteria such as code size and speed
        of execution. Both FSMs are table- driven.

\page   FSM

\section Sparse FSM

        A sparse FSM consists of lists of transitions, one list per
        state. Only active transitions need to be specified. The list
        must be terminated with an FSM_EVENT_NULL entry. Using a
        sparse FSM requires fewer transitions to be specified, but
        each transition is slightly larger than those in a
        fully-specified FSM. Therefore there is not always a space
        saving. Also, the correct transition is found by a linear
        search of the transition list for the current state. This may
        have performance implications.

        State numbers must be contiguous and start from zero. Event
        numbers are unrestricted other than by the size of the
        fsm_event_t type, and must not be equal to FSM_EVENT_NULL.

        Action functions are specified by an index into a table of
        action function pointers, making a significant saving in the
        size of the FSM table. So action numbers must also start from
        zero.

        Action functions are defined to take two void pointer
        arguments, and to return an FSM event (or FSM_EVENT_NULL to
        specify no event).

        To use the sparse FSM engine, define the following data
        structures:

        * An array of FSM_SPARSE16/32_TRANSITION_T structures for each state.
          Terminate the array with
          { FSM_EVENT_NULL, FSM_ACTION_NULL, <state> }
        * An array of FSM_SPARSE_STATES_T containing pointers to each
          of the state transition lists. These must be in order of state
        * An array of fsm_action_fn_t[NUM_ACTIONS] action function
          pointers
        * A single FSM_SPARSE16/32_DEFINITION_T structure containing
          pointers to the state array and action table.

        Calling fsm_sparse16/32_run() will pass the event and data into the
        state machine, and will loop around until an action function
        returns FSM_EVENT_NULL. The fsm_sparse16/32_run() function returns
        FSM_RESULT_OK if the event was processed by the state machine,
        otherwise FSM_RESULT_FAIL.

        See the section below for details on logging.

\section Logging

        Both engines support full control over any FSM logging.  Each
        state machine can choose whether or not to use logging, and if
        so, it can choose whether to display event names or numbers,
        or state names or numbers.

        The relevant FSM_xxxx_DEFINITION_T structure contains pointers
        to arrays of state names, event names, and the name of the
        state machine. Any or all of these may be set to NULL.

        Logging is controlled by the "log" argument to the relevant
        FSM engine function. If set to FSM_NO_LOG, no logging output
        is produced. If set to FSM_LOG, then logging output is
        produced showing the FSM name, the current state and event. If
        state/event names are not defined, then numbers are output
        instead.

        The FSM_LOG and FSM_NO_LOG defines provide ultimate
        flexibility by controlling logging per state machine. However,
        if you wish to be able to control state machine logging on
        some or all state machines from a single build option, use
        "FSM_LOGGING" for the "log" argument. You can then use the
        "FSM_DEBUG" build option to active logging for the relevant
        state machines.

\section Code generation

        Use the "Sparse Compact H.fsm" template for the
        sparse16/32 state machines.
*/

#ifndef __FSM_PRIVATE_H__
#define __FSM_PRIVATE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "fsm.h"

/* Define a macro for logging, as this is common to both engines */
#if defined(ON_CHIP_DEBUG) && defined(FSM_DEBUG)
#define FSM_OUTPUT_LOGGING() send_hci_debug_event(HCI_DEBUG_EV_FSM, \
        (void*)p_state, pv_arg2, 0x00, *p_state, event, 0x00, 0x00, 0x00, 0x00,0x00, 0x00)
#else
#define FSM_OUTPUT_LOGGING() ((void) 0)
#endif

#if defined(INSTALL_BTLOG) && defined(INSTALL_BTLOG_FSM)
#define BTLOG_FSM(...) \
    do \
    { \
        if (btlog_is_enabled(BTLOG_SOURCE_FSM)) \
        { \
            BTLOG(FSM, __VA_ARGS__); \
        } \
    } while (0)
#else
#define BTLOG_FSM(...)
#endif

#endif
