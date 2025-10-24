/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __PSA_SERVICE_H__
#define __PSA_SERVICE_H__

#ifndef __ASSEMBLY__

#include "psa_common.h"
#include "psa_interface_redirect.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PSA_POLL                            (0x00000000u)
#define PSA_BLOCK                           (0x80000000u)
#define PSA_DOORBELL                        (0x00000008u)
#define PSA_WAIT_ANY                        (~0u)

/* message type */
#define PSA_IPC_CONNECT                     (1)
#define PSA_IPC_CALL                        (2)
#define PSA_IPC_DISCONNECT                  (3)

typedef uint32_t psa_signal_t;
typedef struct psa_msg_t {
    uint32_t type;
    psa_handle_t handle;
    int32_t client_id;
    void *rhandle;
    size_t in_size[PSA_MAX_IOVEC];
    size_t out_size[PSA_MAX_IOVEC];
} psa_msg_t;

typedef struct psa_msg_t psa_msg_t;

/*
 * Get the message which corresponds to a given RoT Service signal and remove
 * the message from the RoT Service queue
 * Parameters
 *     psa_signal_t signal: The signal value for an asserted RoT Service
 *     psa_msg_t *msg: Pointer to psa_msg_t object for receiving the message
 * Return
 *     PSA_SUCCESS Success
 *     PSA_ERROR_DOES_NOT_EXIST The message could not be delivered
 * */
psa_status_t psa_get(psa_signal_t signal, psa_msg_t *msg);

/*
 * Completes handling of a specific message and unblocks the client
 * Parameters
 *     psa_handle_t msg_handle: Handle for the client’s message or the null handle
 *     psa_status_t status: Return value to be reported to the client
 * Return
 *     void Success
 *     Does not return The call is invalid, one or more of the following are true:
 *         • msg_handle is invalid and is not the null handle
 *         • An invalid return code is specified for the type of message (see below)
 * */
void psa_reply(psa_handle_t msg_handle, psa_status_t status);

/*
 * Read a message parameter or part of a message parameter from the client input
 * vector.
 * Parameters
 *    a_handle_t msg_handle: Handle for the client’s message
 *    uint32_t invec_idx: Index of the input vector to read from. Must be less
 * than PSA_MAX_IOVEC
 *    void *buffer:Buffer in the Secure Partition to copy the requested data to
 *    size_t num_bytes:Maximum number of bytes to read from the client input vector
 * Return
 *     > 0 Number of bytes copied
 *     0 There was no remaining data in this input vector
 *     Does not return The call is invalid, one or more of the following are true:
 *         • msg_handle is invalid
 *         • msg_handle does not refer to a PSA_IPC_CALL message
 *         • invec_idx is equal to or greater than PSA_MAX_IOVEC
 *         • the memory reference for buffer is invalid or not writable
 * */
size_t psa_read(psa_handle_t msg_handle, uint32_t invec_idx,
                            void *buffer, size_t num_bytes);

/*
 * Writes a message response to a client output vector.
 * Parameters
 *     psa_handle_t msg_handle: Handle for the client’s message
 *     uint32_t outvec_idx: Index of the output vector to write to. Must be less
 * than PSA_MAX_IOVEC
 *     const void *buffer: Buffer with the data to write
 *     size_t num_bytes: Number of bytes to write to the client output vector
 * Return
 *     void Success
 *     Does not return The call is invalid, one or more of the following are true:
 *     • msg_handle is invalid
 *     • msg_handle does not refer to a PSA_IPC_CALL message
 *     • outvec_idx is equal to or greater than PSA_MAX_IOVEC
 *     • the memory reference for buffer is invalid
 *     • the call attempts to write data past the end of the client output vector
 * */
void psa_write(psa_handle_t msg_handle, uint32_t outvec_idx,
                           const void *buffer, size_t num_bytes);

/*
 * Skip over part of a client input vector
 * Parameters
 *     psa_handle_t msg_handle: Handle for the client’s message
 *     uint32_t invec_idx: Index of the input vector to skip from. Must be less
 * than PSA_MAX_IOVEC
 *     size_t num_bytes: Maximum number of bytes to skip in the client input vector
 * Return
 * > 0 Number of bytes skipped
 * 0 There was no remaining data in this input vector
 * Does not return The call is invalid, one or more of the following are true:
 *     • msg_handle is invalid
 *     • msg_handle does not refer to a PSA_IPC_CALL message
 *     • invec_idx is equal to or greater than PSA_MAX_IOVEC
 * */
size_t psa_skip(psa_handle_t msg_handle, uint32_t invec_idx,
                            size_t num_bytes);

/*
 * Associates some caller-provided private data with a specified client connection
 * Parameters
 *      psa_handle_t msg_handle: Handle for the client’s message
 *      void *rhandle: Reverse handle allocated by the RoT Service
 * Return
 *     void Success, rhandle will be provided with all subsequent messages
 * delivered on this connection
 *     Does not return msg_handle is invalid
 **/
void psa_set_rhandle(psa_handle_t msg_handle, void *rhandle);

/* Returns the Secure Partition signals that have been asserted from
 * the subset of signals indicated in the bitmask provided. The mask must contain
 * the set of signals the caller is interested in handling.
 * Parameters
 *     psa_signal_t signal_mask: A set of interrupt and doorbell signals to query.
 * Signals that are not in this set will be ignored.
 *     uint32_t timeout: A timeout bitfield value
 * Return
 *     > 0 At least one signal is asserted
 *     0 No signals are asserted. This case is only seen if caller used a
 * polling timeout
 *     Does not return The call is invalid, one or more of the following are true:
 *     • signal_mask does not include any interrupt or doorbell signals
 *     • signal_mask includes one or more RoT Service signals
 * */
uint32_t psa_wait(uint32_t signal_mask, uint32_t timeout);

void psa_notify(int32_t partition_id);
void psa_clear(void);
void psa_eoi(uint32_t irq_signal);

#ifdef __cplusplus
}
#endif

#endif /* __ASSEMBLY__ */

#endif /* __PSA_SERVICE_H__ */
