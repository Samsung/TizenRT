/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "thread.h"
#include "tfm_arch.h"
#include "utilities.h"

/* Declaration of current thread pointer. */
struct thread_t *p_curr_thrd;

/* Force ZERO in case ZI(bss) clear is missing. */
static struct thread_t *p_thrd_head = NULL; /* Point to the first thread. */
static struct thread_t *p_rnbl_head = NULL; /* Point to the first runnable. */

/* Define Macro to fetch global to support future expansion (PERCPU e.g.) */
#define LIST_HEAD   p_thrd_head
#define RNBL_HEAD   p_rnbl_head

struct thread_t *thrd_next(void)
{
    struct thread_t *p_thrd = RNBL_HEAD;
    /*
     * First runnable thread has highest priority since threads are
     * sorted by priority.
     */
    while (p_thrd && p_thrd->state != THRD_STATE_RUNNABLE) {
        p_thrd = p_thrd->next;
    }

    return p_thrd;
}

static void insert_by_prior(struct thread_t **head, struct thread_t *node)
{
    if (*head == NULL || (node->priority <= (*head)->priority)) {
        node->next = *head;
        *head = node;
    } else {
        struct thread_t *iter = *head;

        while (iter->next && (node->priority > iter->next->priority)) {
            iter = iter->next;
        }

        node->next = iter->next;
        iter->next = node;
    }
}

void thrd_start(struct thread_t *p_thrd, thrd_fn_t fn, thrd_fn_t exit_fn)
{
    TFM_CORE_ASSERT(p_thrd != NULL);

    /* Insert a new thread with priority */
    insert_by_prior(&LIST_HEAD, p_thrd);

    tfm_arch_init_context(p_thrd->p_context_ctrl, (uintptr_t)fn, NULL,
                          (uintptr_t)exit_fn);

    /* Mark it as RUNNABLE after insertion */
    thrd_set_state(p_thrd, THRD_STATE_RUNNABLE);
}

void thrd_set_state(struct thread_t *p_thrd, uint32_t new_state)
{
    TFM_CORE_ASSERT(p_thrd != NULL);

    p_thrd->state = new_state;

    /*
     * Set first runnable thread as head to reduce enumerate
     * depth while searching for a first runnable thread.
     */
    if ((p_thrd->state == THRD_STATE_RUNNABLE) &&
        ((RNBL_HEAD == NULL) || (p_thrd->priority < RNBL_HEAD->priority))) {
        RNBL_HEAD = p_thrd;
    } else {
        RNBL_HEAD = LIST_HEAD;
    }
}

uint32_t thrd_start_scheduler(struct thread_t **ppth)
{
    struct thread_t *pth = thrd_next();

    tfm_arch_trigger_pendsv();

    if (ppth) {
        *ppth = pth;
    }

    return tfm_arch_refresh_hardware_context(pth->p_context_ctrl);
}

void thrd_set_wait(struct sync_obj_t *p_sync_obj, struct thread_t *pth)
{
    TFM_CORE_ASSERT(p_sync_obj && p_sync_obj->magic == THRD_SYNC_MAGIC);

    p_sync_obj->owner = pth;
    thrd_set_state(pth, THRD_STATE_BLOCK);
}

void thrd_wake_up(struct sync_obj_t *p_sync_obj, uint32_t ret_val)
{
    TFM_CORE_ASSERT(p_sync_obj && p_sync_obj->magic == THRD_SYNC_MAGIC);

    if (p_sync_obj->owner && p_sync_obj->owner->state == THRD_STATE_BLOCK) {
        thrd_set_state(p_sync_obj->owner, THRD_STATE_RUNNABLE);
        tfm_arch_set_context_ret_code(p_sync_obj->owner->p_context_ctrl,
                                      ret_val);
        p_sync_obj->owner = NULL;
    }
}
