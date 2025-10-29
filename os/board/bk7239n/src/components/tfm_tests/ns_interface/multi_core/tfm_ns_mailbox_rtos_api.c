/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This file is a reference implementation of Non-secure mailbox RTOS API.
 * This reference implementation is based on TF-M ROTS wrapper API.
 * It can be replaced by RTOS specific implementation.
 */

#include "cmsis_compiler.h"

#ifdef TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD
#include "os_wrapper/msg_queue.h"
#else
#include "os_wrapper/semaphore.h"
#endif
#include "os_wrapper/thread.h"

#include "tfm_ns_mailbox.h"

/*
 * Thread flag to manage wait/wake mechanism in mailbox.、
 * Thread flag can be RTOS specific.
 * The following example definition also covers the rule of CMSIS-RTOS2, which
 * requires the MSB of thread flags must be 0b0.
 */
#define MAILBOX_THREAD_FLAG            0x5FCA0000

#ifndef TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD
#define MAX_SEMAPHORE_COUNT            NUM_MAILBOX_QUEUE_SLOT

static void *ns_lock_handle = NULL;
#endif

const void *tfm_ns_mailbox_os_get_task_handle(void)
{
    return os_wrapper_thread_get_handle();
}

void tfm_ns_mailbox_os_wait_reply(void)
{
    os_wrapper_thread_wait_flag(MAILBOX_THREAD_FLAG, OS_WRAPPER_WAIT_FOREVER);
}

void tfm_ns_mailbox_os_wake_task_isr(const void *task_handle)
{
    os_wrapper_thread_set_flag_isr((void *)task_handle, MAILBOX_THREAD_FLAG);
}

/*
 * When NSPE mailbox only covers a single non-secure core, spinlock only
 * requires to disable IRQ.
 */
void tfm_ns_mailbox_os_spin_lock(void)
{
    __disable_irq();
}

/*
 * It is assumed that IRQ is always enabled when spinlock is acquired.
 * Otherwise, the waiting thread won't be woken up.
 */
void tfm_ns_mailbox_os_spin_unlock(void)
{
    __enable_irq();
}

#ifdef TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD
void *tfm_ns_mailbox_os_mq_create(size_t msg_size, uint8_t msg_count)
{
    return os_wrapper_msg_queue_create(msg_size, msg_count);
}

int32_t tfm_ns_mailbox_os_mq_send(void *mq_handle, const void *msg_ptr)
{
    int32_t ret;

    if (!mq_handle || !msg_ptr) {
        return MAILBOX_INVAL_PARAMS;
    }

    while (1) {
        ret = os_wrapper_msg_queue_send(mq_handle, msg_ptr);
        if (ret == OS_WRAPPER_SUCCESS) {
            return MAILBOX_SUCCESS;
        }
    }

    return MAILBOX_GENERIC_ERROR;
}

int32_t tfm_ns_mailbox_os_mq_receive(void *mq_handle, void *msg_ptr)
{
    int32_t ret;

    if (!mq_handle || !msg_ptr) {
        return MAILBOX_INVAL_PARAMS;
    }

    while (1) {
        ret = os_wrapper_msg_queue_receive(mq_handle, msg_ptr);
        if (ret == OS_WRAPPER_SUCCESS) {
            return MAILBOX_SUCCESS;
        }
    }

    return MAILBOX_GENERIC_ERROR;
}
#else /* TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD */
int32_t tfm_ns_mailbox_os_lock_init(void)
{
    ns_lock_handle = os_wrapper_semaphore_create(MAX_SEMAPHORE_COUNT,
                                                 MAX_SEMAPHORE_COUNT,
                                                 NULL);
    if (!ns_lock_handle) {
        return MAILBOX_GENERIC_ERROR;
    }

    return MAILBOX_SUCCESS;
}

int32_t tfm_ns_mailbox_os_lock_acquire(void)
{
    return os_wrapper_semaphore_acquire(ns_lock_handle,
                                        OS_WRAPPER_WAIT_FOREVER);
}

int32_t tfm_ns_mailbox_os_lock_release(void)
{
    return os_wrapper_semaphore_release(ns_lock_handle);
}
#endif /* TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD */
