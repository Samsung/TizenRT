/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "psa/service.h"
#include "psa_manifest/tfm_psa_proxy.h"
#include "tfm_pools.h"
#include "psa_manifest/sid.h"
#include "tfm_multi_core_api.h"
#include "tfm_ns_mailbox.h"
#include "psa_proxy_shared_mem_mngr.h"

#define NON_SECURE_CLIENT_ID            (-1)

/* Maximum number of connections supported, should be platform/configuration
 * specific */
#define SE_CONN_MAX_NUM                 (16)

TFM_POOL_DECLARE(forward_handle_pool, sizeof(psa_handle_t),
                 SE_CONN_MAX_NUM);

static inline void init_forward_handle_pool(void)
{
    tfm_pool_init(forward_handle_pool,
                  POOL_BUFFER_SIZE(forward_handle_pool),
                  sizeof(psa_handle_t),
                  SE_CONN_MAX_NUM);
}

static inline psa_handle_t * allocate_forward_handle(void)
{
    return (psa_handle_t *) tfm_pool_alloc(forward_handle_pool);
}

static inline void deallocate_forward_handle(psa_handle_t *h)
{
    tfm_pool_free(forward_handle_pool, h);
}

static psa_status_t forward_message_to_secure_enclave(psa_signal_t signal,
                                                       const psa_msg_t *msg)
{
    psa_status_t status;
    struct psa_client_params_t params;
    int32_t ret;

    /* Use stateless handle for stateless services. */
    switch (signal) {
    case TFM_CRYPTO_SIGNAL:
        params.psa_call_params.handle = TFM_CRYPTO_HANDLE;
        params.psa_call_params.type = msg->type;
        break;
    case TFM_PROTECTED_STORAGE_SERVICE_SIGNAL:
        params.psa_call_params.handle = TFM_PROTECTED_STORAGE_SERVICE_HANDLE;
        params.psa_call_params.type = msg->type;
        break;
    case TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_SIGNAL:
        params.psa_call_params.handle =
                                TFM_INTERNAL_TRUSTED_STORAGE_SERVICE_HANDLE;
        params.psa_call_params.type = msg->type;
        break;
    case TFM_ATTESTATION_SERVICE_SIGNAL:
        params.psa_call_params.handle = TFM_ATTESTATION_SERVICE_HANDLE;
        params.psa_call_params.type = msg->type;
        break;
    default:
        params.psa_call_params.handle = *((psa_handle_t *)msg->rhandle);
        params.psa_call_params.type = PSA_IPC_CALL;
        break;
    }

    status = psa_proxy_put_msg_into_shared_mem(msg, &params);

    if (status != PSA_SUCCESS) {
        return status;
    }

    ret = tfm_ns_mailbox_client_call(MAILBOX_PSA_CALL, &params,
                                     NON_SECURE_CLIENT_ID, (int32_t *)&status);
    if (ret != MAILBOX_SUCCESS) {
        status = PSA_ERROR_COMMUNICATION_FAILURE;
    }

    if (status == PSA_SUCCESS) {
        psa_proxy_write_back_results_from_shared_mem(msg);
    }

    return status;
}

static void psa_disconnect_from_secure_enclave(psa_msg_t *msg)
{
    psa_handle_t *forward_handle_ptr = (psa_handle_t *)msg->rhandle;
    struct psa_client_params_t params;
    int32_t reply;

    params.psa_close_params.handle = *forward_handle_ptr;

    (void)tfm_ns_mailbox_client_call(MAILBOX_PSA_CLOSE, &params,
                                     NON_SECURE_CLIENT_ID, &reply);

    deallocate_forward_handle(forward_handle_ptr);
}

static void get_sid_and_version_for_signal(psa_signal_t signal, uint32_t *sid,
                                           uint32_t *version)
{
    switch (signal) {
    case TFM_SP_PLATFORM_SYSTEM_RESET_SIGNAL:
        *sid = TFM_SP_PLATFORM_SYSTEM_RESET_SID;
        *version = TFM_SP_PLATFORM_SYSTEM_RESET_VERSION;
        break;
    case TFM_SP_PLATFORM_IOCTL_SIGNAL:
        *sid = TFM_SP_PLATFORM_IOCTL_SID;
        *version = TFM_SP_PLATFORM_IOCTL_VERSION;
        break;
    case TFM_SP_PLATFORM_NV_COUNTER_SIGNAL:
        *sid = TFM_SP_PLATFORM_NV_COUNTER_SID;
        *version = TFM_SP_PLATFORM_NV_COUNTER_VERSION;
        break;
    default:
        psa_panic();
        break;
    }
}

static psa_status_t psa_connect_to_secure_enclave(psa_signal_t signal,
                                                  psa_msg_t *msg)
{
    psa_handle_t *forward_handle_ptr;
    struct psa_client_params_t params;
    int32_t ret;

    forward_handle_ptr = allocate_forward_handle();

    if (forward_handle_ptr != NULL) {

        get_sid_and_version_for_signal(signal, &params.psa_connect_params.sid,
                                       &params.psa_connect_params.version);

        /* Fixme: All messages sent with the same client id */
        ret = tfm_ns_mailbox_client_call(MAILBOX_PSA_CONNECT, &params,
                                         NON_SECURE_CLIENT_ID,
                                         (int32_t *)forward_handle_ptr);
        if (ret != MAILBOX_SUCCESS) {
            *forward_handle_ptr = PSA_NULL_HANDLE;
        }

        if ( *forward_handle_ptr > 0) {
            psa_set_rhandle(msg->handle, (void *)forward_handle_ptr);
            return PSA_SUCCESS;
        } else {
            deallocate_forward_handle(forward_handle_ptr);
            return *forward_handle_ptr;
        }
    } else {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }
}

static void handle_signal(psa_signal_t signal)
{
    psa_msg_t msg;
    psa_status_t status;

    status = psa_get(signal, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        status = psa_connect_to_secure_enclave(signal, &msg);
        psa_reply(msg.handle, status);
        break;
    case PSA_IPC_DISCONNECT:
        psa_disconnect_from_secure_enclave(&msg);
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        status = forward_message_to_secure_enclave(signal, &msg);
        psa_reply(msg.handle, status);
        break;
    }
}

static psa_status_t psa_proxy_init(void)
{
    int32_t ret;

    if (tfm_platform_ns_wait_for_s_cpu_ready()) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    ret = tfm_ns_mailbox_init(psa_proxy_get_ns_mailbox_queue());
    if (ret != MAILBOX_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    init_forward_handle_pool();

    return PSA_SUCCESS;
}

psa_status_t psa_proxy_sp_init(void)
{
    psa_signal_t signal;
    psa_status_t err;

    err = psa_proxy_init();
    if ( err != PSA_SUCCESS ) {
        psa_panic();
    }

    while (1) {
        /* Control is given back to SPM */
        signal = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signal == TFM_PROXY_DUMMY_SIGNAL) {
            psa_panic();
        }

        handle_signal(signal);
    }

    return PSA_SUCCESS;
}
