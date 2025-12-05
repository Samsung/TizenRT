/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* Data types and API definitions in NSPE mailbox library */

#ifndef __TFM_NS_MAILBOX_H__
#define __TFM_NS_MAILBOX_H__

#include <stdbool.h>
#include <stdint.h>

#include "tfm_mailbox.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(TFM_MULTI_CORE_NS_OS) && (NUM_MAILBOX_QUEUE_SLOT > 1)
#error "NUM_MAILBOX_QUEUE_SLOT should be set to 1 for NS bare metal environment"
#endif

/**
 * \brief NSPE mailbox initialization
 *
 * \param[in] queue             The base address of NSPE mailbox queue to be
 *                              initialized.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_init(struct ns_mailbox_queue_t *queue);

/**
 * \brief Send PSA client call to SPE via mailbox. Wait and fetch PSA client
 *        call result.
 *
 * \param[in] call_type         PSA client call type
 * \param[in] params            Parameters used for PSA client call
 * \param[in] client_id         Optional client ID of non-secure caller.
 *                              It is required to identify the non-secure caller
 *                              when NSPE OS enforces non-secure task isolation.
 * \param[out] reply            The buffer written with PSA client call result.
 *
 * \retval MAILBOX_SUCCESS      The PSA client call is completed successfully.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_client_call(uint32_t call_type,
                                   const struct psa_client_params_t *params,
                                   int32_t client_id,
                                   int32_t *reply);

#ifdef TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD
/**
 * \brief Handling PSA client calls in a dedicated NS mailbox thread.
 *        This function constructs NS mailbox messages, transmits them to SPE
 *        mailbox and returns the results to NS PSA client.
 *
 * \param[in] args              The pointer to the structure of PSA client call
 *                              parameters.
 */
void tfm_ns_mailbox_thread_runner(void *args);
#else /* TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD */
#define tfm_ns_mailbox_thread_runner(args)        do {} while (0)
#endif /* TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD */

/**
 * \brief Platform specific NSPE mailbox initialization.
 *        Invoked by \ref tfm_ns_mailbox_init().
 *
 * \param[in] queue             The base address of NSPE mailbox queue to be
 *                              initialized.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_hal_init(struct ns_mailbox_queue_t *queue);

/**
 * \brief Notify SPE to deal with the PSA client call sent via mailbox
 *
 * \note The implementation depends on platform specific hardware and use case.
 *
 * \retval MAILBOX_SUCCESS      Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_ns_mailbox_hal_notify_peer(void);

/**
 * \brief Enter critical section of NSPE mailbox.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_enter_critical(void);

/**
 * \brief Exit critical section of NSPE mailbox.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_exit_critical(void);

/**
 * \brief Enter critical section of NSPE mailbox in IRQ handler.
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_enter_critical_isr(void);

/**
 * \brief Enter critical section of NSPE mailbox in IRQ handler
 *
 * \note The implementation depends on platform specific hardware and use case.
 */
void tfm_ns_mailbox_hal_exit_critical_isr(void);

#ifdef TFM_MULTI_CORE_NS_OS
/**
 * \brief Initialize the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the non-secure use scenario.
 *
 * \return \ref MAILBOX_SUCCESS on success
 * \return \ref MAILBOX_GENERIC_ERROR on error
 */
int32_t tfm_ns_mailbox_os_lock_init(void);

/**
 * \brief Acquire the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the non-secure use scenario.
 *
 * \return \ref MAILBOX_SUCCESS on success
 * \return \ref MAILBOX_GENERIC_ERROR on error
 */
int32_t tfm_ns_mailbox_os_lock_acquire(void);

/**
 * \brief Release the multi-core lock for synchronizing PSA client call(s)
 *        The actual implementation depends on the non-secure use scenario.
 *
 * \return \ref MAILBOX_SUCCESS on success
 * \return \ref MAILBOX_GENERIC_ERROR on error
 */
int32_t tfm_ns_mailbox_os_lock_release(void);

/**
 * \brief Get the handle of the current non-secure task executing mailbox
 *        functionalities
 *
 * \note This function should be implemented according to NS OS and
 *       actual use scenario.
 *       This function can be ignored or return NULL if sleep/wake-up mechanism
 *       is not required in PSA Client API implementation.
 *
 * \return Return the handle of task.
 */
const void *tfm_ns_mailbox_os_get_task_handle(void);

/**
 * \brief Performs use scenario and NS OS specific waiting mechanism to wait for
 *        the reply to be returned from SPE.
 *
 * \note This function is implemented by NS OS specific waiting mechanism
 *       according to use scenario.
 */
void tfm_ns_mailbox_os_wait_reply(void);

/*
 * \brief Performs use scenario and NS OS specific mechanism in a mailbox IRQ
 *        handler, to wake up a sleeping task which is waiting for its mailbox
 *        message reply.
 *
 * \note The underlying NS OS specific function called inside this function
 *       should be able to work in an IRQ handler.
 *
 * \note This function is implemented by NS OS specific waiting
 *       mechanism according to use scenario.
 *
 * \param[in] task_handle       The handle to the task to be woken up.
 */
void tfm_ns_mailbox_os_wake_task_isr(const void *task_handle);

/**
 * \brief Create and initialize a message queue
 *
 * \param[in] msg_size        The maximum message size in bytes
 * \param[in] msg_count       The maximum number of messages in queue
 *
 * \return Returns handle of the message queue created, or NULL in case of error
 */
void *tfm_ns_mailbox_os_mq_create(size_t msg_size, uint8_t msg_count);

/**
 * \brief Send a request via message queue
 *
 * \param[in] mq_handle       The handle of message queue
 * \param[in] msg_ptr         The pointer to the message to be sent
 *
 * \note The message size must be the same as the value set in
 *       \ref tfm_ns_mailbox_os_mq_create.
 *
 * \return \ref MAILBOX_SUCCESS if the message is successfully sent, or
 *         other return code in case of error
 */
int32_t tfm_ns_mailbox_os_mq_send(void *mq_handle, const void *msg_ptr);

/**
 * \brief Receive a request from message queue
 *
 * \param[in] mq_handle       The handle of message queue
 * \param[in] msg_ptr         The pointer to buffer for message to be received
 *
 * \return \ref MAILBOX_SUCCESS if the message is successfully received, or
 *         other return code in case of error
 *
 * \note The message size is the same as the value set in
 *       \ref tfm_ns_mailbox_os_mq_create.
 *
 * \note The function should be blocked until a message is received from message
 *       queue, unless a fatal error occurs.
 */
int32_t tfm_ns_mailbox_os_mq_receive(void *mq_handle, void *msg_ptr);

/**
 * \brief Go through mailbox messages already replied by SPE mailbox and
 *        wake up the owner tasks of replied mailbox messages.
 *        This function is intended to be called inside platform specific
 *        notification IRQ handler.
 *
 * \return MAILBOX_SUCCESS       The tasks of replied mailbox messages
 *                               were found and wake-up signals were sent.
 * \return MAILBOX_NO_PEND_EVENT No replied mailbox message is found.
 * \return Other return code     Failed with an error code
 */
int32_t tfm_ns_mailbox_wake_reply_owner_isr(void);

/**
 * \brief NSPE local mailbox spin lock.
 *        Implemented by NS RTOS specific implementation.
 */
void tfm_ns_mailbox_os_spin_lock(void);

/**
 * \brief NSPE local mailbox spin unlock.
 *        Implemented by NS RTOS specific implementation.
 */
void tfm_ns_mailbox_os_spin_unlock(void);
#else /* TFM_MULTI_CORE_NS_OS */
#define tfm_ns_mailbox_os_wait_reply()         do {} while (0)

#define tfm_ns_mailbox_os_wake_task_isr(task)  do {} while (0)

static inline int32_t tfm_ns_mailbox_os_lock_init(void)
{
    return MAILBOX_SUCCESS;
}

static inline int32_t tfm_ns_mailbox_os_lock_acquire(void)
{
    return MAILBOX_SUCCESS;
}

static inline int32_t tfm_ns_mailbox_os_lock_release(void)
{
    return MAILBOX_SUCCESS;
}

static inline const void *tfm_ns_mailbox_os_get_task_handle(void)
{
    return NULL;
}

static inline int32_t tfm_ns_mailbox_wake_reply_owner_isr(void)
{
    return MAILBOX_NO_PEND_EVENT;
}

/*
 * Local spinlock is implemented as a dummy one when integrating with NS bare
 * metal environment since interrupt is not required in NS mailbox.
 */
#define tfm_ns_mailbox_os_spin_lock()   do {} while (0)

#define tfm_ns_mailbox_os_spin_unlock() do {} while (0)
#endif /* TFM_MULTI_CORE_NS_OS */

/* The following inline functions configure non-secure mailbox queue status */
static inline void clear_queue_slot_empty(struct ns_mailbox_queue_t *queue_ptr,
                                          uint8_t idx)
{
    if (idx < NUM_MAILBOX_QUEUE_SLOT) {
        queue_ptr->empty_slots &= ~(1UL << idx);
    }
}

static inline void set_queue_slot_pend(struct ns_mailbox_queue_t *queue_ptr,
                                       uint8_t idx)
{
    if (idx < NUM_MAILBOX_QUEUE_SLOT) {
        queue_ptr->pend_slots |= (1UL << idx);
    }
}

static inline void clear_queue_slot_all_replied(
                                           struct ns_mailbox_queue_t *queue_ptr,
                                           mailbox_queue_status_t status)
{
    queue_ptr->replied_slots &= ~status;
}

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NS_MAILBOX_H__ */
