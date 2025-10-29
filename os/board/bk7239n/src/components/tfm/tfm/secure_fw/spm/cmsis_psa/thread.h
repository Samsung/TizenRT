/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __M_THREAD_H__ /* Add an extra M as thread.h is common. */
#define __M_THREAD_H__

#include <stddef.h>
#include <stdint.h>

/* State codes */
#define THRD_STATE_CREATING       0
#define THRD_STATE_RUNNABLE       1
#define THRD_STATE_BLOCK          2
#define THRD_STATE_DETACH         3
#define THRD_STATE_INVALID        4

/* Priorities. Lower value has higher priority */
#define THRD_PRIOR_HIGHEST        0x0
#define THRD_PRIOR_HIGH           0xF
#define THRD_PRIOR_MEDIUM         0x1F
#define THRD_PRIOR_LOW            0x7F
#define THRD_PRIOR_LOWEST         0xFF

/* Error codes */
#define THRD_SUCCESS              0
#define THRD_ERR_GENERIC          1

/* Thread entry function type */
typedef void (*thrd_fn_t)(void *);

/* An address causes exceptions (invalid address and security bit). */
#define THRD_GENERAL_EXIT  ((thrd_fn_t)(0xFFFFFFFE))

/* Thread context */
struct thread_t {
    uint8_t         priority;           /* Priority                          */
	char *name;
    uint8_t         state;              /* State                             */
    uint16_t        flags;              /* Flags and align, DO NOT REMOVE!   */
    void            *p_context_ctrl;    /* Context control (sp, splimit, lr) */
    struct thread_t *next;              /* Next thread in list               */
};

/*
 * Definition for the current thread and its access helper preprocessor.
 * The definition needs to be declared in one of the sources.
 */
extern struct thread_t *p_curr_thrd;
#define CURRENT_THREAD p_curr_thrd

/*
 * Initialize the thread_t struct with the given inputs.
 *
 * Parameters:
 *  p_thrd         -    Pointer of caller provided thread_t struct to be init
 *  p_ctx_ctrl     -    Initial Context control (sp, splimit, lr)
 *  priority       -    Initial priority
 */
#define THRD_INIT(p_thrd, p_ctx_ctrl, prio) do {                         \
                        (p_thrd)->priority       = (uint8_t)(prio);      \
                        (p_thrd)->state          = THRD_STATE_CREATING;  \
                        (p_thrd)->flags          = 0;                    \
                        (p_thrd)->p_context_ctrl = p_ctx_ctrl;           \
                    } while (0)

/*
 * Set thread priority.
 *
 * Parameters :
 *  p_thrd         -     Pointer of thread_t struct
 *  priority       -     Priority value (0~255)
 *
 * Note :
 *  The new priority may not take effect immediately.
 */
#define THRD_SET_PRIORITY(p_thrd, priority) \
                                        p_thrd->priority = (uint8_t)(priority)

/*
 * Update current thread's bound context pointer.
 *
 * Parameters :
 *  x              -     Context pointer to be bound with the current thread.
 */
#define THRD_UPDATE_CUR_CTXCTRL(x)          \
                                CURRENT_THREAD->p_context_ctrl = (void *)(x)

/*
 * Check if a schedule is under expectation by measuring on a given thread.
 *
 * Return :
 *  `true` if schedule is under expectation. `false` if not.
 */
#define THRD_EXPECTING_SCHEDULE() (!(thrd_next() == CURRENT_THREAD))

/*
 * Set thread state, and updates the runnable head.
 *
 * Parameters :
 *  p_thrd         -     Pointer of thread_t struct
 *  new_state      -     New state of thread
 */
void thrd_set_state(struct thread_t *p_thrd, uint32_t new_state);

/*
 * Prepare thread context with given info and insert it into schedulable list.
 *
 * Parameters :
 *  p_thrd         -     Pointer of thread_t struct
 *  fn             -     Thread entry function
 *  exit_fn        -     The function to go when 'fn' exited
 *
 * Note :
 *  - thrd_fn_t does not have parameters but param is still set into R0 for some
 *    special usages, for example the NS agent.
 *  - The thread is not "started" immediately.
 */
void thrd_start(struct thread_t *p_thrd, thrd_fn_t fn, thrd_fn_t exit_fn);

/*
 * Get the next thread to run in list.
 *
 * Return :
 *  Pointer of next thread to run.
 */
struct thread_t *thrd_next(void);

/*
 * Start scheduling.
 *
 * ppth [out]       -     The first runnable thread
 *
 * Return :
 *  The EXC_RETURN payload of the first runnable thread for caller usage.
 */
uint32_t thrd_start_scheduler(struct thread_t **ppth);

/* Sync object */

/* A magic for corruption detecting */
#ifndef NDEBUG
#define THRD_SYNC_MAGIC                0x736f626a   /* 'sobj' */
#endif

struct sync_obj_t {
#ifndef NDEBUG
    uint32_t magic;             /* The magic for corruption detecting */
#endif
    struct thread_t *owner;     /* The owner thread of the sync object */
};

#ifndef NDEBUG
#define THRD_SYNC_INIT(p_sync_obj) do {                            \
                            (p_sync_obj)->magic = THRD_SYNC_MAGIC; \
                            (p_sync_obj)->owner = NULL;            \
                        } while (0)
#else
#define THRD_SYNC_INIT(p_sync_obj) (p_sync_obj)->owner = NULL
#endif

/*
 * Block the thread to wait on an sync object. The thread becomes the onwer of
 * sync object.
 *
 * Parameters:
 *  p_sync_obj  -    The pointer of sync object allocated by the caller
 *  pth         -    The thread_t which waits on p_sync_obj.
 *
 */
void thrd_set_wait(struct sync_obj_t *p_sync_obj, struct thread_t *pth);

/*
 * Wake up the sync object owner thread and set the return value of the function
 * that caused the "waiting".
 *
 * Parameters :
 *  p_sync_obj   -   The sync object that the thread is waiting on
 *  ret_val      -   Value to be returned to owner
 */
void thrd_wake_up(struct sync_obj_t *p_sync_obj, uint32_t ret_val);

#endif /* __M_THREAD_H__ */
