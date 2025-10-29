/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifdef TFM_PSA_API
#include "psa/service.h"
#include "psa_manifest/tfm_ps_test_service.h"
#else
#include "psa/client.h"
#endif

#include "ps_object_system.h"

psa_status_t tfm_ps_test_prepare(psa_invec *in_vec, size_t in_len,
                                 psa_outvec *out_vec, size_t out_len)
{
    (void)in_vec;
    (void)in_len;
    (void)out_vec;
    (void)out_len;

    return ps_system_prepare();
}

#ifdef TFM_PSA_API
static psa_status_t tfm_ps_test_handle_msg(const psa_msg_t *msg)
{
    psa_status_t status = PSA_ERROR_PROGRAMMER_ERROR;

    switch (msg->type) {
    case PSA_IPC_CONNECT:
    case PSA_IPC_DISCONNECT:
        status = PSA_SUCCESS;
        break;
    case PSA_IPC_CALL:
        status = ps_system_prepare();
        break;
    }
    return status;
}

#if (TFM_SP_PS_TEST_MODEL_SFN == 1)
psa_status_t tfm_ps_test_prepare_sfn(const psa_msg_t* msg)
{
    return tfm_ps_test_handle_msg(msg);
}
#elif (TFM_SP_PS_TEST_MODEL_IPC == 1)
psa_status_t tfm_ps_test_init(void)
{
    psa_msg_t msg;
    while (1) {
        (void)psa_wait(TFM_PS_TEST_PREPARE_SIGNAL, PSA_BLOCK);
        (void)psa_get(TFM_PS_TEST_PREPARE_SIGNAL, &msg);
        psa_reply(msg.handle, tfm_ps_test_handle_msg(&msg));
    }
    return PSA_SUCCESS;
}
#endif /* TFM_SP_PS_TEST_MODEL_SFN == 1 */
#else /* TFM_PSA_API */
psa_status_t tfm_ps_test_init(void)
{
    return PSA_SUCCESS;
}
#endif /* TFM_PSA_API */
