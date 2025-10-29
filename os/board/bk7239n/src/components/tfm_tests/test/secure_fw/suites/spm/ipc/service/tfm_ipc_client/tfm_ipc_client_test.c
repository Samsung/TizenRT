/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <assert.h>
#include "psa/client.h"
#include "psa/service.h"
#include "psa_manifest/sid.h"
#include "psa_manifest/tfm_ipc_client_test.h"
#include "tfm_hal_isolation.h"
#include "tfm_secure_client_2_api.h"
#include "utilities.h"

/* Define the return status */
#define IPC_SP_TEST_SUCCESS     (1)
#define IPC_SP_TEST_FAILED      (-1)

/*
 * Create a global const data, so that it is stored in code
 * section which is read only.
 */
char const client_data_read_only = 'A';

/*
 * Fixme: Temporarily implement abort as infinite loop,
 * will replace it later.
 */
static void tfm_abort(void)
{
    while (1)
        ;
}

#ifdef TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM
static int ipc_isolation_2_psa_access_app_readonly_memory(void)
{
    psa_handle_t handle;
    psa_status_t status;
    char const *client_data_p = &client_data_read_only;
    struct psa_invec invecs[1] = {{&client_data_p, sizeof(client_data_p)}};

    handle = psa_connect(IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID,
                         IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_VERSION);

    if (handle <= 0) {
        return IPC_SP_TEST_FAILED;
    }

    status = psa_call(handle, PSA_IPC_CALL, invecs, 1, NULL, 0);

    /* The system should panic before here. */
    psa_close(handle);
    return IPC_SP_TEST_FAILED;
}
#endif

static int ipc_isolation_2_psa_access_app_memory(void)
{
    psa_handle_t handle;
    psa_status_t status;
    int32_t result = IPC_SP_TEST_FAILED;
    char client_data = 'A';
    char *client_data_p = &client_data;
    struct psa_invec invecs[1] = {{&client_data_p, sizeof(client_data_p)}};

    handle = psa_connect(IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SID,
                         IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_VERSION);

    if (handle <= 0) {
        return result;
    }

    status = psa_call(handle, PSA_IPC_CALL, invecs, 1, NULL, 0);

    if ((client_data == 'B') && (status >= 0)) {
        result = IPC_SP_TEST_SUCCESS;
    }

    psa_close(handle);
    return result;
}

#ifdef TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM
static int ipc_client_app_access_psa_mem_test(void)
{
    psa_handle_t handle;
    psa_status_t status;
    uint8_t *outvec_data[1] = {0};
    struct psa_outvec outvecs[1] = {{outvec_data, sizeof(outvec_data[0])}};

    handle = psa_connect(IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SID,
                         IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_VERSION);

    if (handle <= 0) {
        return IPC_SP_TEST_FAILED;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
    if (status >= 0) {
        /*
         * outvecs should contain the pointer pointed to ipc service partition
         * memory. Read the pointed memory should cause panic.
         */
        uint8_t *psa_data_p = outvec_data[0];
        if (psa_data_p) {
            (*psa_data_p)++;
        }
    }

    /* The system should panic before here. */
    psa_close(handle);
    return IPC_SP_TEST_FAILED;
}
#endif

#ifdef TFM_IPC_ISOLATION_2_MEM_CHECK
static int ipc_client_mem_check_test(void)
{
    psa_handle_t handle;
    psa_status_t status;
    uint8_t *outvec_data[1] = {0};
    struct psa_outvec outvecs[1] = {{outvec_data, sizeof(outvec_data[0])}};
    struct psa_invec invecs[1] = {{NULL, 0}};

    handle = psa_connect(IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SID,
                         IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_VERSION);

    if (handle <= 0) {
        return IPC_SP_TEST_FAILED;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
    if (status >= 0) {
        /*
         * outvecs should contain the pointer pointed to ipc service partition
         * memory. In psa_call, it checks whether the target partition has the
         * access right to the invecs indicated memory. If no, the system will
         * panic.
         */
        uint8_t *psa_data_p = outvec_data[0];
        if (psa_data_p) {
            invecs[0].base = psa_data_p;
            invecs[0].len = sizeof(psa_data_p);
            psa_call(handle, PSA_IPC_CALL, invecs, 1, NULL, 0);
        }
    }

    /* The system should panic before here. */
    psa_close(handle);
    return IPC_SP_TEST_FAILED;
}
#endif

#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
static int ipc_client_retrieve_app_mem_test(void)
{
    psa_handle_t handle;
    psa_status_t status;
    uint32_t attr = 0;
    uint8_t *outvec_data[1] = {0};
    struct psa_outvec outvecs[1] = {{outvec_data, sizeof(outvec_data[0])}};

    handle = psa_connect(TFM_SECURE_CLIENT_2_SID, TFM_SECURE_CLIENT_2_VERSION);

    if (handle <= 0) {
        return IPC_SP_TEST_FAILED;
    }

    status = psa_call(handle, TFM_SECURE_CLIENT_2_ID_RETRIEVE_APP_MEM, NULL, 0,
                      outvecs, 1);
    psa_close(handle);

    if (status == PSA_SUCCESS) {
        /*
         * outvecs should contain the pointer pointed to app service partition
         * memory. Read the pointed memory should cause panic.
         */
        uint8_t *psa_data_p = outvec_data[0];
        if (psa_data_p) {
            attr |= (TFM_HAL_ACCESS_READABLE | TFM_HAL_ACCESS_WRITABLE
                     | TFM_HAL_ACCESS_UNPRIVILEGED);
            if (tfm_hal_memory_has_access((uintptr_t)psa_data_p, 1, attr) ==
                                                      TFM_HAL_ERROR_MEM_FAULT) {
                return IPC_SP_TEST_SUCCESS;
            } else {
                return IPC_SP_TEST_FAILED;
            }
        }
    }

    return IPC_SP_TEST_FAILED;
}
#endif

static void ipc_client_handle_ser_req(psa_msg_t msg, uint32_t signals,
                                      int (*fn)(void))
{
    int32_t ret;

    switch (msg.type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        ret = (*fn)();
        if (msg.out_size[0] != 0) {
            psa_write(msg.handle, 0, &ret, sizeof(ret));
        }
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        /* cannot get here? [broken SPM]. TODO*/
        tfm_abort();
        break;
    }
}

void ipc_client_test_main(void)
{
    psa_msg_t msg;
    uint32_t signals = 0;

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        psa_get(signals, &msg);
        if (signals & IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SIGNAL) {
            ipc_client_handle_ser_req(msg,
                                     IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SIGNAL,
                                     &ipc_isolation_2_psa_access_app_memory);
#ifdef TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM
        } else if (signals &
                    IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL) {
            ipc_client_handle_ser_req(msg,
                           IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL,
                           &ipc_isolation_2_psa_access_app_readonly_memory);
#endif
#ifdef TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM
        } else if (signals & IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SIGNAL) {
            ipc_client_handle_ser_req(msg,
                                     IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SIGNAL,
                                     &ipc_client_app_access_psa_mem_test);
#endif
#ifdef TFM_IPC_ISOLATION_2_MEM_CHECK
        } else if (signals & IPC_CLIENT_TEST_MEM_CHECK_SIGNAL) {
            ipc_client_handle_ser_req(msg, IPC_CLIENT_TEST_MEM_CHECK_SIGNAL,
                                      &ipc_client_mem_check_test);
#endif
#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
        } else if (signals & IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SIGNAL) {
            ipc_client_handle_ser_req(msg,
            IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SIGNAL,
            &ipc_client_retrieve_app_mem_test);
#endif
        } else {
            /* Should not go here. */
            tfm_abort();
        }
    }
}
