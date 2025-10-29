/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "bitops.h"
#include "config_impl.h"
#include "critical_section.h"
#include "psa/lifecycle.h"
#include "psa/service.h"
#include "interrupt.h"
#include "spm_ipc.h"
#include "tfm_arch.h"
#include "tfm_core_utils.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/interrupt_defs.h"
#include "ffm/psa_api.h"
#include "utilities.h"
#include "ffm/backend.h"
#include "ffm/psa_api.h"
#include "ffm/spm_error_base.h"
#include "tfm_rpc.h"
#include "tfm_spm_hal.h"
#include "tfm_hal_interrupt.h"
#include "tfm_hal_platform.h"
#include "tfm_psa_call_pack.h"

#define GET_STATELESS_SERVICE(index)    (stateless_services_ref_tbl[index])
extern struct service_t *stateless_services_ref_tbl[];

#if PSA_FRAMEWORK_HAS_MM_IOVEC

/*
 * The MM-IOVEC status
 * The max total number of invec and outvec is 8.
 * Each invec/outvec takes 4 bit, 32 bits in total.
 *
 * The encoding format of the MM-IOVEC status:
 *--------------------------------------------------------------
 *|  Bit   |  31 - 28  |  27 - 24  | ... |  7 - 4   |  3 - 0   |
 *--------------------------------------------------------------
 *| Vector | outvec[3] | outvec[2] | ... | invec[1] | invec[0] |
 *--------------------------------------------------------------
 *
 * Take invec[0] as an example:
 *
 * bit 0:  whether invec[0] has been mapped.
 * bit 1:  whether invec[0] has been unmapped.
 * bit 2:  whether invec[0] has been accessed using psa_read(), psa_skip() or
 *         psa_write().
 * bit 3:  reserved for invec[0].
 */

#define IOVEC_STATUS_BITS              4   /* Each vector occupies 4 bits. */
#define OUTVEC_IDX_BASE                4   /*
                                            * Base index of outvec.
                                            * There are four invecs in front of
                                            * outvec.
                                            */
#define INVEC_IDX_BASE                 0   /* Base index of invec. */

#define IOVEC_MAPPED_BIT               (1U << 0)
#define IOVEC_UNMAPPED_BIT             (1U << 1)
#define IOVEC_ACCESSED_BIT             (1U << 2)

#define IOVEC_IS_MAPPED(handle, iovec_idx)      \
    ((((handle)->iovec_status) >> ((iovec_idx) * IOVEC_STATUS_BITS)) &  \
                               IOVEC_MAPPED_BIT)
#define IOVEC_IS_UNMAPPED(handle, iovec_idx)    \
    ((((handle)->iovec_status) >> ((iovec_idx) * IOVEC_STATUS_BITS)) &  \
                               IOVEC_UNMAPPED_BIT)
#define IOVEC_IS_ACCESSED(handle, iovec_idx)    \
    ((((handle)->iovec_status) >> ((iovec_idx) * IOVEC_STATUS_BITS)) &  \
                               IOVEC_ACCESSED_BIT)
#define SET_IOVEC_MAPPED(handle, iovec_idx)     \
    (((handle)->iovec_status) |= (IOVEC_MAPPED_BIT <<   \
                              ((iovec_idx) * IOVEC_STATUS_BITS)))
#define SET_IOVEC_UNMAPPED(handle, iovec_idx)   \
    (((handle)->iovec_status) |= (IOVEC_UNMAPPED_BIT << \
                              ((iovec_idx) * IOVEC_STATUS_BITS)))
#define SET_IOVEC_ACCESSED(handle, iovec_idx)   \
    (((handle)->iovec_status) |= (IOVEC_ACCESSED_BIT << \
                              ((iovec_idx) * IOVEC_STATUS_BITS)))

#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */

void spm_handle_programmer_errors(psa_status_t status)
{
    if (status == PSA_ERROR_PROGRAMMER_ERROR ||
        status == PSA_ERROR_CONNECTION_REFUSED) {
        if (!tfm_spm_is_ns_caller()) {
            tfm_core_panic();
        }
    }
}

uint32_t tfm_spm_get_lifecycle_state(void)
{
    /*
     * FixMe: return PSA_LIFECYCLE_UNKNOWN to the caller directly. It will be
     * implemented in the future.
     */
    return PSA_LIFECYCLE_UNKNOWN;
}

/* PSA Client API function body */

uint32_t tfm_spm_client_psa_framework_version(void)
{
    return PSA_FRAMEWORK_VERSION;
}

uint32_t tfm_spm_client_psa_version(uint32_t sid)
{
    struct service_t *service;
    bool ns_caller = tfm_spm_is_ns_caller();

    /*
     * It should return PSA_VERSION_NONE if the RoT Service is not
     * implemented.
     */
    service = tfm_spm_get_service_by_sid(sid);
    if (!service) {
        return PSA_VERSION_NONE;
    }

    /*
     * It should return PSA_VERSION_NONE if the caller is not authorized
     * to access the RoT Service.
     */
    if (tfm_spm_check_authorization(sid, service, ns_caller) != SPM_SUCCESS) {
        return PSA_VERSION_NONE;
    }

    return service->p_ldinf->version;
}

psa_status_t tfm_spm_client_psa_call(psa_handle_t handle,
                                     uint32_t ctrl_param,
                                     const psa_invec *inptr,
                                     psa_outvec *outptr)
{
    psa_invec invecs[PSA_MAX_IOVEC];
    psa_outvec outvecs[PSA_MAX_IOVEC];
    struct conn_handle_t *conn_handle;
    struct service_t *service;
    int i, j;
    int32_t client_id;
    uint32_t sid, version, index;
    uint32_t privileged;
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;
    bool ns_caller = tfm_spm_is_ns_caller();
    int32_t type = (int32_t)(int16_t)((ctrl_param & TYPE_MASK) >> TYPE_OFFSET);
    size_t in_num = (size_t)((ctrl_param & IN_LEN_MASK) >> IN_LEN_OFFSET);
    size_t out_num = (size_t)((ctrl_param & OUT_LEN_MASK) >> OUT_LEN_OFFSET);

    /* The request type must be zero or positive. */
    if (type < 0) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /* It is a PROGRAMMER ERROR if in_len + out_len > PSA_MAX_IOVEC. */
    if ((in_num > PSA_MAX_IOVEC) ||
        (out_num > PSA_MAX_IOVEC) ||
        (in_num + out_num > PSA_MAX_IOVEC)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /* It is a PROGRAMMER ERROR if the handle is a null handle. */
    if (handle == PSA_NULL_HANDLE) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    client_id = tfm_spm_get_client_id(ns_caller);

    /* Allocate space from handle pool for static handle. */
    if (IS_STATIC_HANDLE(handle)) {
        index = GET_INDEX_FROM_STATIC_HANDLE(handle);

        if (!IS_VALID_STATIC_HANDLE_IDX(index)) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }

        service = GET_STATELESS_SERVICE(index);
        if (!service) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }

        sid = service->p_ldinf->sid;

        /*
         * It is a PROGRAMMER ERROR if the caller is not authorized to access
         * the RoT Service.
         */
        if (tfm_spm_check_authorization(sid, service, ns_caller)
            != SPM_SUCCESS) {
            return PSA_ERROR_CONNECTION_REFUSED;
        }

        version = GET_VERSION_FROM_STATIC_HANDLE(handle);

        if (tfm_spm_check_client_version(service, version) != SPM_SUCCESS) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }

        CRITICAL_SECTION_ENTER(cs_assert);
        conn_handle = tfm_spm_create_conn_handle();
        CRITICAL_SECTION_LEAVE(cs_assert);

        if (!conn_handle) {
            return PSA_ERROR_CONNECTION_BUSY;
        }

        conn_handle->rhandle = NULL;
        handle = tfm_spm_to_user_handle(conn_handle);
    } else {
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
        /* It is a PROGRAMMER ERROR if an invalid handle was passed. */
        conn_handle = spm_get_handle_by_client_handle(handle, client_id);
        if (!conn_handle) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }

        /*
         * It is a PROGRAMMER ERROR if the connection is currently
         * handling a request.
         */
        if (conn_handle->status != TFM_HANDLE_STATUS_IDLE) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }

        service = conn_handle->service;

        if (!service) {
            /* FixMe: Need to implement a mechanism to resolve this failure. */
            return PSA_ERROR_PROGRAMMER_ERROR;
        }
#else
        return PSA_ERROR_PROGRAMMER_ERROR;
#endif
    }

    privileged = GET_CURRENT_PARTITION_PRIVILEGED_MODE();

    /*
     * Read client invecs from the wrap input vector. It is a PROGRAMMER ERROR
     * if the memory reference for the wrap input vector is invalid or not
     * readable.
     */
    if (tfm_memory_check(inptr, in_num * sizeof(psa_invec), ns_caller,
        TFM_MEMORY_ACCESS_RO, privileged) != SPM_SUCCESS) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /*
     * Read client outvecs from the wrap output vector and will update the
     * actual length later. It is a PROGRAMMER ERROR if the memory reference for
     * the wrap output vector is invalid or not read-write.
     */
    if (tfm_memory_check(outptr, out_num * sizeof(psa_outvec), ns_caller,
        TFM_MEMORY_ACCESS_RW, privileged) != SPM_SUCCESS) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    spm_memset(invecs, 0, sizeof(invecs));
    spm_memset(outvecs, 0, sizeof(outvecs));

    /* Copy the address out to avoid TOCTOU attacks. */
    spm_memcpy(invecs, inptr, in_num * sizeof(psa_invec));
    spm_memcpy(outvecs, outptr, out_num * sizeof(psa_outvec));

    /*
     * For client input vector, it is a PROGRAMMER ERROR if the provided payload
     * memory reference was invalid or not readable.
     */
    for (i = 0; i < in_num; i++) {
        if (tfm_memory_check(invecs[i].base, invecs[i].len, ns_caller,
            TFM_MEMORY_ACCESS_RO, privileged) != SPM_SUCCESS) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }
    }

    /*
     * Clients must never overlap input parameters because of the risk of a
     * double-fetch inconsistency.
     * Overflow is checked in tfm_memory_check functions.
     */
    for (i = 0; i + 1 < in_num; i++) {
        for (j = i+1; j < in_num; j++) {
            if (!((char *) invecs[j].base + invecs[j].len <=
                  (char *) invecs[i].base ||
                  (char *) invecs[j].base >=
                  (char *) invecs[i].base + invecs[i].len)) {
                return PSA_ERROR_PROGRAMMER_ERROR;
            }
        }
    }

    /*
     * For client output vector, it is a PROGRAMMER ERROR if the provided
     * payload memory reference was invalid or not read-write.
     */
    for (i = 0; i < out_num; i++) {
        if (tfm_memory_check(outvecs[i].base, outvecs[i].len,
            ns_caller, TFM_MEMORY_ACCESS_RW, privileged) != SPM_SUCCESS) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }
    }

    spm_fill_message(conn_handle, service, handle, type, client_id,
                     invecs, in_num, outvecs, out_num, outptr);

    return backend_instance.messaging(service, conn_handle);
}

/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1

psa_status_t tfm_spm_client_psa_connect(uint32_t sid, uint32_t version)
{
    struct service_t *service;
    struct conn_handle_t *conn_handle;
    int32_t client_id;
    psa_handle_t handle;
    bool ns_caller = tfm_spm_is_ns_caller();
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;

    /*
     * It is a PROGRAMMER ERROR if the RoT Service does not exist on the
     * platform.
     */
    service = tfm_spm_get_service_by_sid(sid);
    if (!service) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    /* It is a PROGRAMMER ERROR if connecting to a stateless service. */
    if (SERVICE_IS_STATELESS(service->p_ldinf->flags)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /*
     * It is a PROGRAMMER ERROR if the caller is not authorized to access the
     * RoT Service.
     */
    if (tfm_spm_check_authorization(sid, service, ns_caller) != SPM_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    /*
     * It is a PROGRAMMER ERROR if the version of the RoT Service requested is
     * not supported on the platform.
     */
    if (tfm_spm_check_client_version(service, version) != SPM_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    client_id = tfm_spm_get_client_id(ns_caller);

    /*
     * Create connection handle here since it is possible to return the error
     * code to client when creation fails.
     */
    CRITICAL_SECTION_ENTER(cs_assert);
    conn_handle = tfm_spm_create_conn_handle();
    CRITICAL_SECTION_LEAVE(cs_assert);
    if (!conn_handle) {
        return PSA_ERROR_CONNECTION_BUSY;
    }

    handle = tfm_spm_to_user_handle(conn_handle);
    /* No input or output needed for connect message */
    spm_fill_message(conn_handle, service, handle, PSA_IPC_CONNECT,
                     client_id, NULL, 0, NULL, 0, NULL);

    return backend_instance.messaging(service, conn_handle);
}

psa_status_t tfm_spm_client_psa_close(psa_handle_t handle)
{
    struct service_t *service;
    struct conn_handle_t *conn_handle;
    int32_t client_id;
    bool ns_caller = tfm_spm_is_ns_caller();

    /* It will have no effect if called with the NULL handle */
    if (handle == PSA_NULL_HANDLE) {
        return PSA_SUCCESS;
    }

    /* It is a PROGRAMMER ERROR if called with a stateless handle. */
    if (IS_STATIC_HANDLE(handle)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    client_id = tfm_spm_get_client_id(ns_caller);

    /*
     * It is a PROGRAMMER ERROR if an invalid handle was provided that is not
     * the null handle.
     */
    conn_handle = spm_get_handle_by_client_handle(handle, client_id);
    if (!conn_handle) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    service = conn_handle->service;
    if (!service) {
        /* FixMe: Need to implement one mechanism to resolve this failure. */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /*
     * It is a PROGRAMMER ERROR if the connection is currently handling a
     * request.
     */
    if (conn_handle->status == TFM_HANDLE_STATUS_ACTIVE) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    /* No input or output needed for close message */
    spm_fill_message(conn_handle, service, handle, PSA_IPC_DISCONNECT,
                     client_id, NULL, 0, NULL, 0, NULL);

    return backend_instance.messaging(service, conn_handle);
}

#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

/* PSA Partition API function body */

#if CONFIG_TFM_SPM_BACKEND_IPC == 1 \
    || CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1
psa_signal_t tfm_spm_partition_psa_wait(psa_signal_t signal_mask,
                                        uint32_t timeout)
{
    struct partition_t *partition = NULL;

    /*
     * Timeout[30:0] are reserved for future use.
     * SPM must ignore the value of RES.
     */
    timeout &= PSA_TIMEOUT_MASK;

    partition = GET_CURRENT_COMPONENT();

    /*
     * It is a PROGRAMMER ERROR if the signal_mask does not include any assigned
     * signals.
     */
    if ((partition->signals_allowed & signal_mask) == 0) {
        tfm_core_panic();
    }

    /*
     * backend_instance.wait() blocks the caller thread if no signals are
     * available. In this case, the return value of this function is temporary
     * set into runtime context. After new signal(s) are available, the return
     * value is updated with the available signal(s) and blocked thread gets
     * to run.
     */
    if (timeout == PSA_BLOCK) {
        return backend_instance.wait(partition, signal_mask);
    }

    return partition->signals_asserted & signal_mask;
}
#endif

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
psa_status_t tfm_spm_partition_psa_get(psa_signal_t signal, psa_msg_t *msg)
{
    struct conn_handle_t *handle = NULL;
    struct partition_t *partition = NULL;
    uint32_t privileged;

    /*
     * Only one message could be retrieved every time for psa_get(). It is a
     * fatal error if the input signal has more than a signal bit set.
     */
    if (!IS_ONLY_ONE_BIT_IN_UINT32(signal)) {
        tfm_core_panic();
    }

    partition = GET_CURRENT_COMPONENT();

    privileged = GET_PARTITION_PRIVILEGED_MODE(partition->p_ldinf);

    /*
     * Write the message to the service buffer. It is a fatal error if the
     * input msg pointer is not a valid memory reference or not read-write.
     */
    if (tfm_memory_check(msg, sizeof(psa_msg_t), false, TFM_MEMORY_ACCESS_RW,
        privileged) != SPM_SUCCESS) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the caller call psa_get() when no message has
     * been set. The caller must call this function after an RoT Service signal
     * is returned by psa_wait().
     */
    if (partition->signals_asserted == 0) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the RoT Service signal is not currently asserted.
     */
    if ((partition->signals_asserted & signal) == 0) {
        tfm_core_panic();
    }

    /*
     * Get message by signal from partition. It is a fatal error if getting
     * failed, which means the input signal is not correspond to an RoT service.
     */
    handle = spm_get_handle_by_signal(partition, signal);
    if (!handle) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    spm_memcpy(msg, &handle->msg, sizeof(psa_msg_t));

    return PSA_SUCCESS;
}
#endif

size_t tfm_spm_partition_psa_read(psa_handle_t msg_handle, uint32_t invec_idx,
                                  void *buffer, size_t num_bytes)
{
    size_t bytes;
    struct conn_handle_t *handle = NULL;
    uint32_t priv_mode;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
        return 0;
    }

    priv_mode = GET_PARTITION_PRIVILEGED_MODE(
                                        handle->service->partition->p_ldinf);

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (handle->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /* There was no remaining data in this input vector */
    if (handle->msg.in_size[invec_idx] == 0) {
        return 0;
    }

#if PSA_FRAMEWORK_HAS_MM_IOVEC
    /*
     * It is a fatal error if the input vector has already been mapped using
     * psa_map_invec().
     */
    if (IOVEC_IS_MAPPED(handle, (invec_idx + INVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    SET_IOVEC_ACCESSED(handle, (invec_idx + INVEC_IDX_BASE));
#endif

    /*
     * Copy the client data to the service buffer. It is a fatal error
     * if the memory reference for buffer is invalid or not read-write.
     */
    if (tfm_memory_check(buffer, num_bytes, false,
        TFM_MEMORY_ACCESS_RW, priv_mode) != SPM_SUCCESS) {
        tfm_core_panic();
    }

    bytes = num_bytes > handle->msg.in_size[invec_idx] ?
                        handle->msg.in_size[invec_idx] : num_bytes;

    spm_memcpy(buffer, handle->invec[invec_idx].base, bytes);

    /* There maybe some remaining data */
    handle->invec[invec_idx].base =
                                (char *)handle->invec[invec_idx].base + bytes;
    handle->msg.in_size[invec_idx] -= bytes;

    return bytes;
}

size_t tfm_spm_partition_psa_skip(psa_handle_t msg_handle, uint32_t invec_idx,
                                  size_t num_bytes)
{
    struct conn_handle_t *handle = NULL;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
        return 0;
    }

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (handle->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /* There was no remaining data in this input vector */
    if (handle->msg.in_size[invec_idx] == 0) {
        return 0;
    }

#if PSA_FRAMEWORK_HAS_MM_IOVEC
    /*
     * It is a fatal error if the input vector has already been mapped using
     * psa_map_invec().
     */
    if (IOVEC_IS_MAPPED(handle, (invec_idx + INVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    SET_IOVEC_ACCESSED(handle, (invec_idx + INVEC_IDX_BASE));
#endif

    /*
     * If num_bytes is greater than the remaining size of the input vector then
     * the remaining size of the input vector is used.
     */
    if (num_bytes > handle->msg.in_size[invec_idx]) {
        num_bytes = handle->msg.in_size[invec_idx];
    }

    /* There maybe some remaining data */
    handle->invec[invec_idx].base =
                            (char *)handle->invec[invec_idx].base + num_bytes;
    handle->msg.in_size[invec_idx] -= num_bytes;

    return num_bytes;
}

void tfm_spm_partition_psa_write(psa_handle_t msg_handle, uint32_t outvec_idx,
                                 const void *buffer, size_t num_bytes)
{
    struct conn_handle_t *handle = NULL;
    uint32_t priv_mode;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
        return;
    }

    priv_mode = GET_PARTITION_PRIVILEGED_MODE(
                                        handle->service->partition->p_ldinf);

    /*
     * It is a fatal error if message handle does not refer to a request
     * message
     */
    if (handle->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if outvec_idx is equal to or greater than
     * PSA_MAX_IOVEC
     */
    if (outvec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the call attempts to write data past the end of
     * the client output vector
     */
    if (num_bytes > handle->msg.out_size[outvec_idx] -
        handle->outvec[outvec_idx].len) {
        tfm_core_panic();
    }

#if PSA_FRAMEWORK_HAS_MM_IOVEC
    /*
     * It is a fatal error if the output vector has already been mapped using
     * psa_map_outvec().
     */
    if (IOVEC_IS_MAPPED(handle, (outvec_idx + OUTVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    SET_IOVEC_ACCESSED(handle, (outvec_idx + OUTVEC_IDX_BASE));
#endif

    /*
     * Copy the service buffer to client outvecs. It is a fatal error
     * if the memory reference for buffer is invalid or not readable.
     */
    if (tfm_memory_check(buffer, num_bytes, false,
        TFM_MEMORY_ACCESS_RO, priv_mode) != SPM_SUCCESS) {
        tfm_core_panic();
    }

    spm_memcpy((char *)handle->outvec[outvec_idx].base +
               handle->outvec[outvec_idx].len, buffer, num_bytes);

    /* Update the write number */
    handle->outvec[outvec_idx].len += num_bytes;
}

psa_status_t tfm_spm_partition_psa_reply(psa_handle_t msg_handle,
                                         psa_status_t status)
{
    struct service_t *service;
    struct conn_handle_t *handle;
    psa_status_t ret = PSA_SUCCESS;
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
        return PSA_ERROR_INVALID_HANDLE;
    }

    /*
     * RoT Service information is needed in this function, stored it in message
     * body structure. Only two parameters are passed in this function: handle
     * and status, so it is useful and simply to do like this.
     */
    service = handle->service;
    if (!service) {
        tfm_core_panic();
    }

    switch (handle->msg.type) {
    case PSA_IPC_CONNECT:
        /*
         * Reply to PSA_IPC_CONNECT message. Connect handle is returned if the
         * input status is PSA_SUCCESS. Others return values are based on the
         * input status.
         */
        if (status == PSA_SUCCESS) {
            ret = msg_handle;
        } else if (status == PSA_ERROR_CONNECTION_REFUSED) {
            /* Refuse the client connection, indicating a permanent error. */
            ret = PSA_ERROR_CONNECTION_REFUSED;
            handle->status = TFM_HANDLE_STATUS_TO_FREE;
        } else if (status == PSA_ERROR_CONNECTION_BUSY) {
            /* Fail the client connection, indicating a transient error. */
            ret = PSA_ERROR_CONNECTION_BUSY;
        } else {
            tfm_core_panic();
        }
        break;
    case PSA_IPC_DISCONNECT:
        /* Service handle is not used anymore */
        handle->status = TFM_HANDLE_STATUS_TO_FREE;

        /*
         * If the message type is PSA_IPC_DISCONNECT, then the status code is
         * ignored
         */
        break;
    default:
        if (handle->msg.type >= PSA_IPC_CALL) {

#if PSA_FRAMEWORK_HAS_MM_IOVEC

            /*
             * If the unmapped function is not called for an input/output vector
             * that has been mapped, the framework will remove the mapping.
             */
            int i;

            for (i = 0; i < PSA_MAX_IOVEC * 2; i++) {
                if (IOVEC_IS_MAPPED(handle, i) &&
                    (!IOVEC_IS_UNMAPPED(handle, i))) {
                    SET_IOVEC_UNMAPPED(handle, i);
                    /*
                     * Any output vectors that are still mapped will report that
                     * zero bytes have been written.
                     */
                    if (i >= OUTVEC_IDX_BASE) {
                        handle->outvec[i - OUTVEC_IDX_BASE].len = 0;
                    }
                }
            }

#endif
            /* Reply to a request message. Return values are based on status */
            ret = status;
            /*
             * The total number of bytes written to a single parameter must be
             * reported to the client by updating the len member of the
             * psa_outvec structure for the parameter before returning from
             * psa_call().
             */
            update_caller_outvec_len(handle);
            if (SERVICE_IS_STATELESS(service->p_ldinf->flags)) {
                handle->status = TFM_HANDLE_STATUS_TO_FREE;
            }
        } else {
            tfm_core_panic();
        }
    }

    if (ret == PSA_ERROR_PROGRAMMER_ERROR) {
        /*
         * If the source of the programmer error is a Secure Partition, the SPM
         * must panic the Secure Partition in response to a PROGRAMMER ERROR.
         */
        if (!TFM_CLIENT_ID_IS_NS(handle->msg.client_id)) {
            tfm_core_panic();
        }
    }

    /*
     * TODO: It can be optimized further by moving critical section protection
     * to mailbox. Also need to check implementation when secure context is
     * involved.
     */
    CRITICAL_SECTION_ENTER(cs_assert);
    ret = backend_instance.replying(handle, ret);
    CRITICAL_SECTION_LEAVE(cs_assert);

    if (handle->status == TFM_HANDLE_STATUS_TO_FREE) {
        tfm_spm_free_conn_handle(handle);
    } else {
        handle->status = TFM_HANDLE_STATUS_IDLE;
    }

    return ret;
}

#if CONFIG_TFM_DOORBELL_API == 1
void tfm_spm_partition_psa_notify(int32_t partition_id)
{
    struct partition_t *p_pt = tfm_spm_get_partition_by_id(partition_id);

    spm_assert_signal(p_pt, PSA_DOORBELL);
}

void tfm_spm_partition_psa_clear(void)
{
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;
    struct partition_t *partition = NULL;

    partition = GET_CURRENT_COMPONENT();

    /*
     * It is a fatal error if the Secure Partition's doorbell signal is not
     * currently asserted.
     */
    if ((partition->signals_asserted & PSA_DOORBELL) == 0) {
        tfm_core_panic();
    }

    CRITICAL_SECTION_ENTER(cs_assert);
    partition->signals_asserted &= ~PSA_DOORBELL;
    CRITICAL_SECTION_LEAVE(cs_assert);
}
#endif /* CONFIG_TFM_DOORBELL_API == 1 */

void tfm_spm_partition_psa_panic(void)
{
#ifdef CONFIG_TFM_HALT_ON_CORE_PANIC
    tfm_hal_system_halt();
#else
    /*
     * PSA FF recommends that the SPM causes the system to restart when a secure
     * partition panics.
     */
    tfm_hal_system_reset();
#endif
}

/* psa_set_rhandle is only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1

void tfm_spm_partition_psa_set_rhandle(psa_handle_t msg_handle, void *rhandle)
{
    struct conn_handle_t *handle;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
        return;
    }

    /* It is a PROGRAMMER ERROR if a stateless service sets rhandle. */
    if (SERVICE_IS_STATELESS(handle->service->p_ldinf->flags)) {
        tfm_core_panic();
        return;
    }

    handle->msg.rhandle = rhandle;
    handle->rhandle = rhandle;
}

#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

#if CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1
void tfm_spm_partition_psa_irq_enable(psa_signal_t irq_signal)
{
    struct partition_t *partition;
    struct irq_load_info_t *irq_info;

    partition = GET_CURRENT_COMPONENT();

    irq_info = get_irq_info_for_signal(partition->p_ldinf, irq_signal);
    if (!irq_info) {
        tfm_core_panic();
    }

    tfm_hal_irq_enable(irq_info->source);
}

psa_irq_status_t tfm_spm_partition_psa_irq_disable(psa_signal_t irq_signal)
{
    struct partition_t *partition;
    struct irq_load_info_t *irq_info;

    partition = GET_CURRENT_COMPONENT();

    irq_info = get_irq_info_for_signal(partition->p_ldinf, irq_signal);
    if (!irq_info) {
        tfm_core_panic();
    }

    tfm_hal_irq_disable(irq_info->source);

    return 1;
}

/* This API is only used for FLIH. */
#if CONFIG_TFM_FLIH_API == 1
void tfm_spm_partition_psa_reset_signal(psa_signal_t irq_signal)
{
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;
    struct irq_load_info_t *irq_info;
    struct partition_t *partition;

    partition = GET_CURRENT_COMPONENT();

    irq_info = get_irq_info_for_signal(partition->p_ldinf, irq_signal);
    if (!irq_info) {
        tfm_core_panic();
    }

    if (!irq_info->flih_func) {
        /* This API is for FLIH IRQs only */
        tfm_core_panic();
    }

    if ((partition->signals_asserted & irq_signal) == 0) {
        /* The signal is not asserted */
        tfm_core_panic();
    }

    CRITICAL_SECTION_ENTER(cs_assert);
    partition->signals_asserted &= ~irq_signal;
    CRITICAL_SECTION_LEAVE(cs_assert);
}
#endif

/* This API is only used for SLIH. */
#if CONFIG_TFM_SLIH_API == 1
void tfm_spm_partition_psa_eoi(psa_signal_t irq_signal)
{
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;
    struct irq_load_info_t *irq_info = NULL;
    struct partition_t *partition = NULL;

    partition = GET_CURRENT_COMPONENT();

    irq_info = get_irq_info_for_signal(partition->p_ldinf, irq_signal);
    /* It is a fatal error if passed signal is not an interrupt signal. */
    if (!irq_info) {
        tfm_core_panic();
    }

    if (irq_info->flih_func) {
        /* This API is for SLIH IRQs only */
        tfm_core_panic();
    }

    /* It is a fatal error if passed signal is not currently asserted */
    if ((partition->signals_asserted & irq_signal) == 0) {
        tfm_core_panic();
    }

    CRITICAL_SECTION_ENTER(cs_assert);
    partition->signals_asserted &= ~irq_signal;
    CRITICAL_SECTION_LEAVE(cs_assert);

    tfm_hal_irq_clear_pending(irq_info->source);
    tfm_hal_irq_enable(irq_info->source);
}
#endif
#endif /* CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1 */

#if PSA_FRAMEWORK_HAS_MM_IOVEC

const void *tfm_spm_partition_psa_map_invec(psa_handle_t msg_handle,
                                            uint32_t invec_idx)
{
    struct conn_handle_t *handle;
    uint32_t privileged;
    struct partition_t *partition = NULL;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
    }

    partition = handle->service->partition;
    privileged = GET_PARTITION_PRIVILEGED_MODE(partition->p_ldinf);

    /*
     * It is a fatal error if MM-IOVEC has not been enabled for the RoT
     * Service that received the message.
     */
    if (!SERVICE_ENABLED_MM_IOVEC(handle->service->p_ldinf->flags)) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if message handle does not refer to a request
     * message.
     */
    if (handle->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC.
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /* It is a fatal error if the input vector has length zero. */
    if (handle->msg.in_size[invec_idx] == 0) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the input vector has already been mapped using
     * psa_map_invec().
     */
    if (IOVEC_IS_MAPPED(handle, (invec_idx + INVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the input vector has already been accessed
     * using psa_read() or psa_skip().
     */
    if (IOVEC_IS_ACCESSED(handle, (invec_idx + INVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the memory reference for the wrap input vector is
     * invalid or not readable.
     */
    if (tfm_memory_check(handle->invec[invec_idx].base,
                         handle->invec[invec_idx].len,
                         false, TFM_MEMORY_ACCESS_RO,
                         privileged) != SPM_SUCCESS) {
        tfm_core_panic();
    }

    SET_IOVEC_MAPPED(handle, (invec_idx + INVEC_IDX_BASE));

    return handle->invec[invec_idx].base;
}

void tfm_spm_partition_psa_unmap_invec(psa_handle_t msg_handle,
                                       uint32_t invec_idx)
{
    struct conn_handle_t *handle;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if MM-IOVEC has not been enabled for the RoT
     * Service that received the message.
     */
    if (!SERVICE_ENABLED_MM_IOVEC(handle->service->p_ldinf->flags)) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if message handle does not refer to a request
     * message.
     */
    if (handle->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if invec_idx is equal to or greater than
     * PSA_MAX_IOVEC.
     */
    if (invec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if The input vector has not been mapped by a call to
     * psa_map_invec().
     */
    if (!IOVEC_IS_MAPPED(handle, (invec_idx + INVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the input vector has already been unmapped by a
     * call to psa_unmap_invec().
     */
    if (IOVEC_IS_UNMAPPED(handle, (invec_idx + INVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    SET_IOVEC_UNMAPPED(handle, (invec_idx + INVEC_IDX_BASE));
}

void *tfm_spm_partition_psa_map_outvec(psa_handle_t msg_handle,
                                       uint32_t outvec_idx)
{
    struct conn_handle_t *handle;
    uint32_t privileged;
    struct partition_t *partition = NULL;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
    }

    partition = handle->service->partition;
    privileged = GET_PARTITION_PRIVILEGED_MODE(partition->p_ldinf);

    /*
     * It is a fatal error if MM-IOVEC has not been enabled for the RoT
     * Service that received the message.
     */
    if (!SERVICE_ENABLED_MM_IOVEC(handle->service->p_ldinf->flags)) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if message handle does not refer to a request
     * message.
     */
    if (handle->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if outvec_idx is equal to or greater than
     * PSA_MAX_IOVEC.
     */
    if (outvec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /* It is a fatal error if the output vector has length zero. */
    if (handle->msg.out_size[outvec_idx] == 0) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the output vector has already been mapped using
     * psa_map_outvec().
     */
    if (IOVEC_IS_MAPPED(handle, (outvec_idx + OUTVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the output vector has already been accessed
     * using psa_write().
     */
    if (IOVEC_IS_ACCESSED(handle, (outvec_idx + OUTVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the output vector is invalid or not read-write.
     */
    if (tfm_memory_check(handle->outvec[outvec_idx].base,
                         handle->outvec[outvec_idx].len, false,
                         TFM_MEMORY_ACCESS_RW, privileged) != SPM_SUCCESS) {
        tfm_core_panic();
    }
    SET_IOVEC_MAPPED(handle, (outvec_idx + OUTVEC_IDX_BASE));

    return handle->outvec[outvec_idx].base;
}

void tfm_spm_partition_psa_unmap_outvec(psa_handle_t msg_handle,
                                        uint32_t outvec_idx, size_t len)
{
    struct conn_handle_t *handle;

    /* It is a fatal error if message handle is invalid */
    handle = spm_get_handle_by_msg_handle(msg_handle);
    if (!handle) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if MM-IOVEC has not been enabled for the RoT
     * Service that received the message.
     */
    if (!SERVICE_ENABLED_MM_IOVEC(handle->service->p_ldinf->flags)) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if message handle does not refer to a request
     * message.
     */
    if (handle->msg.type < PSA_IPC_CALL) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if outvec_idx is equal to or greater than
     * PSA_MAX_IOVEC.
     */
    if (outvec_idx >= PSA_MAX_IOVEC) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if len is greater than the output vector size.
     */
    if (len > handle->msg.out_size[outvec_idx]) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if The output vector has not been mapped by a call to
     * psa_map_outvec().
     */
    if (!IOVEC_IS_MAPPED(handle, (outvec_idx + OUTVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    /*
     * It is a fatal error if the output vector has already been unmapped by a
     * call to psa_unmap_outvec().
     */
    if (IOVEC_IS_UNMAPPED(handle, (outvec_idx + OUTVEC_IDX_BASE))) {
        tfm_core_panic();
    }

    SET_IOVEC_UNMAPPED(handle, (outvec_idx + OUTVEC_IDX_BASE));

    /* Update the write number */
    handle->outvec[outvec_idx].len = len;
}

#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */
