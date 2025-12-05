/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>

#include "tfm_ns_mailbox.h"

/* Thread woken up flag */
#define NOT_WOKEN        0x0
#define WOKEN_UP        0x5C

/*
 * The request contains the parameters which application threads share with
 * NS mailbox thread.
 */
struct ns_mailbox_req_t {
    uint32_t                         call_type;   /* PSA client call type */
    const struct psa_client_params_t *params_ptr; /* Pointer to PSA client call
                                                   * parameters.
                                                   */
    int32_t                          client_id;   /* Optional client ID of the
                                                   * non-secure caller.
                                                   * It is required to identify
                                                   * the non-secure task when
                                                   * NSPE OS enforces non-secure
                                                   * task isolation
                                                   */
    const void                       *owner;      /* Handle of owner task. */
    int32_t                          *reply;      /* Address of reply value
                                                   * belonging to owner task.
                                                   */

    uint8_t                          *woken_flag; /* Indicate that owner task
                                                   * has been or should be woken
                                                   * up, after the reply is
                                                   * received.
                                                   */
};

/* Message queue handle */
static void *msgq_handle = NULL;

/* The handle of the dedicated NS mailbox thread. */
static const void *ns_mailbox_thread_handle = NULL;

/* The pointer to NSPE mailbox queue */
static struct ns_mailbox_queue_t *mailbox_queue_ptr = NULL;

static inline void set_queue_slot_all_empty(mailbox_queue_status_t completed)
{
    mailbox_queue_ptr->empty_slots |= completed;
}

static inline void set_queue_slot_woken(uint8_t idx)
{
    if (idx < NUM_MAILBOX_QUEUE_SLOT) {
        *mailbox_queue_ptr->queue[idx].reply.woken_flag = WOKEN_UP;
    }
}

static uint8_t acquire_empty_slot(struct ns_mailbox_queue_t *queue)
{
    uint8_t idx;
    mailbox_queue_status_t status;

    while (1) {
        tfm_ns_mailbox_os_spin_lock();
        status = queue->empty_slots;
        tfm_ns_mailbox_os_spin_unlock();

        if (status) {
            break;
        }

        /* No empty slot */
        queue->is_full = true;
        /* DSB to make sure the thread sleeps after the flag is set */
        __DSB();

        /* Wait for an empty slot released by a completed mailbox message */
        tfm_ns_mailbox_os_wait_reply();
        queue->is_full = false;
    }

    for (idx = 0; idx < NUM_MAILBOX_QUEUE_SLOT; idx++) {
        if (status & (1 << idx)) {
            break;
        }
    }

    tfm_ns_mailbox_os_spin_lock();
    clear_queue_slot_empty(queue, idx);
    tfm_ns_mailbox_os_spin_unlock();

    return idx;
}

static int32_t mailbox_tx_client_call_msg(const struct ns_mailbox_req_t *req,
                                          uint8_t *slot_idx)
{
    struct mailbox_msg_t *msg_ptr;
    struct mailbox_reply_t *reply_ptr;
    uint8_t idx = NUM_MAILBOX_QUEUE_SLOT;

    idx = acquire_empty_slot(mailbox_queue_ptr);
    if (idx == NUM_MAILBOX_QUEUE_SLOT) {
        return MAILBOX_QUEUE_FULL;
    }

#ifdef TFM_MULTI_CORE_TEST
    tfm_ns_mailbox_tx_stats_update();
#endif

    /* Fill the mailbox message */
    msg_ptr = &mailbox_queue_ptr->queue[idx].msg;
    msg_ptr->call_type = req->call_type;
    memcpy(&msg_ptr->params, req->params_ptr, sizeof(msg_ptr->params));
    msg_ptr->client_id = req->client_id;

    /* Prepare the reply structure */
    reply_ptr = &mailbox_queue_ptr->queue[idx].reply;
    reply_ptr->owner = req->owner;
    reply_ptr->reply = req->reply;
    reply_ptr->woken_flag = req->woken_flag;

    /*
     * Memory check can be added here to prevent a malicious application
     * from providing addresses of other applications or privileged area.
     */

    tfm_ns_mailbox_hal_enter_critical();
    set_queue_slot_pend(mailbox_queue_ptr, idx);
    tfm_ns_mailbox_hal_exit_critical();

    tfm_ns_mailbox_hal_notify_peer();

    if (slot_idx) {
        *slot_idx = idx;
    }

    return MAILBOX_SUCCESS;
}

static inline void ns_mailbox_set_reply_isr(uint8_t idx)
{
    int32_t *reply_ptr = mailbox_queue_ptr->queue[idx].reply.reply;

    if (reply_ptr) {
        *reply_ptr = mailbox_queue_ptr->queue[idx].reply.return_val;
    }
}

static int32_t mailbox_wait_reply(const struct ns_mailbox_req_t *req)
{
    while (1) {
        /*
         * Check the completed flag to make sure that the current thread is
         * woken up by reply event, rather than other events.
         */
        if (*req->woken_flag == WOKEN_UP) {
            break;
        }

        /* Woken up from sleep */
        tfm_ns_mailbox_os_wait_reply();
    }

    return MAILBOX_SUCCESS;
}

int32_t tfm_ns_mailbox_client_call(uint32_t call_type,
                                   const struct psa_client_params_t *params,
                                   int32_t client_id,
                                   int32_t *reply)
{
    struct ns_mailbox_req_t req;
    uint8_t woken_flag = NOT_WOKEN;
    int32_t ret;

    if (!mailbox_queue_ptr) {
        return MAILBOX_INIT_ERROR;
    }

    if (!params || !reply) {
        return MAILBOX_INVAL_PARAMS;
    }

    req.call_type = call_type;
    req.params_ptr = params;
    req.reply = reply;
    req.woken_flag = &woken_flag;
    req.owner = tfm_ns_mailbox_os_get_task_handle();
    req.client_id = client_id;

    ret = tfm_ns_mailbox_os_mq_send(msgq_handle, &req);
    if (ret != MAILBOX_SUCCESS) {
        return ret;
    }

    ret = mailbox_wait_reply(&req);

    return ret;
}

void tfm_ns_mailbox_thread_runner(void *args)
{
    struct ns_mailbox_req_t req;
    int32_t ret;

    (void)args;

    ns_mailbox_thread_handle = tfm_ns_mailbox_os_get_task_handle();

    while (1) {
        ret = tfm_ns_mailbox_os_mq_receive(msgq_handle, &req);
        if (ret != MAILBOX_SUCCESS) {
            continue;
        }

        /*
         * Invalid client address. However, the pointer was already
         * checked previously and therefore just simply ignore this
         * client call request.
         */
        if (!req.params_ptr || !req.reply || !req.woken_flag) {
            continue;
        }

        mailbox_tx_client_call_msg(&req, NULL);
    }
}

int32_t tfm_ns_mailbox_wake_reply_owner_isr(void)
{
    uint8_t idx;
    const void *task_handle;
    mailbox_queue_status_t replied_status, complete_slots = 0x0;

    if (!mailbox_queue_ptr) {
        return MAILBOX_INIT_ERROR;
    }

    tfm_ns_mailbox_hal_enter_critical_isr();
    replied_status = mailbox_queue_ptr->replied_slots;
    clear_queue_slot_all_replied(mailbox_queue_ptr, replied_status);
    tfm_ns_mailbox_hal_exit_critical_isr();

    if (!replied_status) {
        return MAILBOX_NO_PEND_EVENT;
    }

    for (idx = 0; idx < NUM_MAILBOX_QUEUE_SLOT; idx++) {
        /*
         * The reply has already received from SPE mailbox but
         * the wake-up signal is not sent yet.
         */
        if (!(replied_status & (0x1UL << idx))) {
            continue;
        }

        /*
         * Write back the return result.
         * When TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD is enabled, a reply is
         * returned inside ns_mailbox_set_reply_isr().
         * When TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD is disabled, a reply is
         * returned inside mailbox_rx_client_reply(). ns_mailbox_set_reply_isr()
         * is defined as dummy function.
         */
        ns_mailbox_set_reply_isr(idx);

        /* Wake up the owner of this mailbox message */
        set_queue_slot_woken(idx);

        task_handle = mailbox_queue_ptr->queue[idx].reply.owner;
        if (task_handle) {
            tfm_ns_mailbox_os_wake_task_isr(task_handle);
        }

        complete_slots |= (1UL << idx);

        replied_status &= ~(0x1UL << idx);
        if (!replied_status) {
            break;
        }
    }

    set_queue_slot_all_empty(complete_slots);

    /*
     * Wake up the NS mailbox thread in case it is waiting for
     * empty slots.
     */
    if (mailbox_queue_ptr->is_full) {
        if (ns_mailbox_thread_handle) {
            tfm_ns_mailbox_os_wake_task_isr(ns_mailbox_thread_handle);
        }
    }

    return MAILBOX_SUCCESS;
}

static inline int32_t mailbox_req_queue_init(uint8_t queue_depth)
{
    msgq_handle = tfm_ns_mailbox_os_mq_create(sizeof(struct ns_mailbox_req_t),
                                              queue_depth);
    if (!msgq_handle) {
        return MAILBOX_GENERIC_ERROR;
    }

    return MAILBOX_SUCCESS;
}

int32_t tfm_ns_mailbox_init(struct ns_mailbox_queue_t *queue)
{
    int32_t ret;

    if (!queue) {
        return MAILBOX_INVAL_PARAMS;
    }

    /*
     * Further verification of mailbox queue address may be required according
     * to non-secure memory assignment.
     */

    memset(queue, 0, sizeof(*queue));

    /* Initialize empty bitmask */
    queue->empty_slots =
            (mailbox_queue_status_t)((1UL << (NUM_MAILBOX_QUEUE_SLOT - 1)) - 1);
    queue->empty_slots +=
            (mailbox_queue_status_t)(1UL << (NUM_MAILBOX_QUEUE_SLOT - 1));

    mailbox_queue_ptr = queue;

    /* Platform specific initialization. */
    ret = tfm_ns_mailbox_hal_init(queue);
    if (ret != MAILBOX_SUCCESS) {
        return ret;
    }

    ret = mailbox_req_queue_init(NUM_MAILBOX_QUEUE_SLOT);

#ifdef TFM_MULTI_CORE_TEST
    tfm_ns_mailbox_tx_stats_init(queue);
#endif

    return ret;
}
