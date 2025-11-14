/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <assert.h>
#include <stdbool.h>
#include "psa/client.h"
#include "psa/service.h"
#include "psa_manifest/tfm_ipc_service_test.h"
#include "spm_test_defs.h"
#include "tfm_api.h"
#include "tfm_hal_isolation.h"
#include "tfm_secure_api.h"
#include "tfm_sp_log.h"
#include "client_api_test_defs.h"
#include "client_api_test_service.h"
#if PSA_FRAMEWORK_HAS_MM_IOVEC
#include "tfm_mmiovec_test_service.h"
#endif

#define IPC_SERVICE_BUFFER_LEN                          32

/* Define the global variable for the IPC_APP_ACCESS_PSA_MEM_SID service. */
uint8_t ipc_servic_data;
uint8_t *ipc_service_data_p = &ipc_servic_data;

/*
 * Fixme: Temporarily implement abort as infinite loop,
 * will replace it later.
 */
static void tfm_abort(void)
{
    while (1)
        ;
}

static void ipc_service_basic(void)
{
    psa_msg_t msg;
    psa_status_t status;

    status = psa_get(IPC_SERVICE_TEST_BASIC_SIGNAL, &msg);
    if (status != PSA_SUCCESS) {
        return;
    }

    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case CLIENT_API_TEST_TYPE_REQUEST_SRV:
        psa_reply(msg.handle, client_api_test_rot_srv(&msg));
        break;
    default:
        psa_reply(msg.handle, PSA_ERROR_NOT_SUPPORTED);
        break;
    }
}

static void ipc_service_psa_access_app_mem(void)
{
    psa_msg_t msg;
    char rec_data;
    uint32_t rec_buf;

    psa_get(IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        /*
         * rec_buf is a pointer pointed to a char type memory in client stack.
         */
        if (msg.in_size[0] != 0) {
            psa_read(msg.handle, 0, &rec_buf, 4);
            rec_data = *(char *)rec_buf;

            /* rec_data is assigned to 'A' by the client side. */
            if (rec_data != 'A') {
                psa_reply(msg.handle, -1);
                break;
            }

            /* Change the char type client stack memory to 'B'. */
            *((char *)rec_buf) = 'B';
        }
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        /* cannot get here? [broken SPM]. TODO*/
        tfm_abort();
        break;
    }
}

#ifdef TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM
static void ipc_service_psa_access_app_readonly_mem(void)
{
    psa_msg_t msg;
    char rec_data;
    uint32_t rec_buf, attr = 0;

    psa_get(IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
         /*
          * rec_buf is a pointer pointed to a char type memory in client
          * code section. Write the memory will cause MemManage fault.
          */
        if (msg.in_size[0] != 0) {
            psa_read(msg.handle, 0, &rec_buf, 4);
            rec_data = *(char *)rec_buf;

            /* rec_data is assigned to 'A' by the client side. */
            if (rec_data != 'A') {
                psa_reply(msg.handle, -1);
                break;
            }
            attr |= (TFM_HAL_ACCESS_READABLE | TFM_HAL_ACCESS_WRITABLE
                     | TFM_HAL_ACCESS_UNPRIVILEGED);
            if (tfm_hal_memory_has_access((uintptr_t)rec_buf, 4, attr) !=
                                                      TFM_HAL_ERROR_MEM_FAULT) {
                psa_reply(msg.handle, PSA_ERROR_GENERIC_ERROR);
                break;
            }
        }
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        /* cannot get here? [broken SPM]. TODO*/
        tfm_abort();
        break;
    }
}
#endif

#if defined TFM_IPC_ISOLATION_2_MEM_CHECK \
    || defined TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM
static void ipc_service_app_access_psa_mem(void)
{
    psa_msg_t msg;

    psa_get(IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        if (msg.out_size[0] != 0) {

            /*
             * Write a pointer to outvec. The pointer points to uint8_t
             * memory in ipc servive partition.
             */
            psa_write(msg.handle, 0, &ipc_service_data_p,
                      sizeof(ipc_service_data_p));
        }

        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        /* cannot get here? [broken SPM]. TODO*/
        tfm_abort();
        break;
    }
}
#endif

static void ipc_service_programmer_error(void)
{
    psa_msg_t msg;

    psa_get(IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        psa_reply(msg.handle, PSA_ERROR_PROGRAMMER_ERROR);
        break;
    default:
        /* cannot get here? [broken SPM]. TODO*/
        tfm_abort();
        break;
    }
}

static void ipc_service_stateless_rot(void)
{
    psa_status_t status;
    psa_msg_t msg;

    /* Retrieve the message corresponding to the example service signal */
    status = psa_get(IPC_SERVICE_TEST_STATELESS_ROT_SIGNAL, &msg);
    if (status != PSA_SUCCESS) {
        return;
    }

    /* Decode the message */
    switch (msg.type) {
    case CLIENT_API_TEST_TYPE_REQUEST_SRV:
        status = client_api_test_rot_srv(&msg);
        break;
    default:
        /* Invalid message type */
        status = PSA_ERROR_PROGRAMMER_ERROR;
        break;
    }

    /* Reply with the message result status to unblock the client */
    psa_reply(msg.handle, status);
}

#if PSA_FRAMEWORK_HAS_MM_IOVEC

static void ipc_service_mmiovec_test_handle(void)
{
    psa_status_t status;
    psa_msg_t msg;

    /* Retrieve the message corresponding to the MMIO test service signal */
    status = psa_get(IPC_SERVICE_TEST_MMIOVEC_SIGNAL, &msg);
    if (status != PSA_SUCCESS) {
        psa_panic();
    }

    /* Decode the message */
    switch (msg.type) {
    case INVEC_MAP_AND_UNMAP:
        status = test_service_mmiovec_invec(&msg);
        break;
    case OUTVEC_MAP_AND_UNMAP:
        status = test_service_mmiovec_outvec(&msg);
        break;
    case OUTVEC_MAP_NOT_UNMAP:
        status = test_service_outvec_not_unmap(&msg);
        break;
    default:
        /* Invalid message type */
        status = TFM_MMIOVEC_TEST_ERROR;
    }
    /* Reply with the message result status to unblock the client */
    psa_reply(msg.handle, status);
}

#endif

/* Test thread */
void ipc_service_test_main(void *param)
{
    uint32_t signals = 0;

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & IPC_SERVICE_TEST_BASIC_SIGNAL) {
            ipc_service_basic();
        } else if (signals & IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SIGNAL) {
            ipc_service_psa_access_app_mem();
#ifdef TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM
        } else if (signals
                   & IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL) {
            ipc_service_psa_access_app_readonly_mem();
#endif
#if defined TFM_IPC_ISOLATION_2_MEM_CHECK \
    || defined TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM
        } else if (signals & IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SIGNAL) {
            ipc_service_app_access_psa_mem();
#endif
        } else if (signals & IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_SIGNAL) {
            ipc_service_programmer_error();
        } else if (signals & IPC_SERVICE_TEST_STATELESS_ROT_SIGNAL) {
            ipc_service_stateless_rot();
#if PSA_FRAMEWORK_HAS_MM_IOVEC
        } else if (signals & IPC_SERVICE_TEST_MMIOVEC_SIGNAL) {
            ipc_service_mmiovec_test_handle();
#endif
        } else {
            /* Should not come here */
            tfm_abort();
        }
    }
}
