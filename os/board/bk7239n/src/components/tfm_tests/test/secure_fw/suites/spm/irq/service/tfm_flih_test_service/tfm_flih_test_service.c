/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_test.h"
#include "spm_test_defs.h"
#include "tfm_irq_test_service.h"
#include "tfm_sp_log.h"
#include "psa/service.h"
#include "psa_manifest/tfm_flih_test_service.h"

psa_flih_result_t tfm_timer0_irq_flih(void)
{
    return tfm_flih_test_timer_handler();
}

static psa_status_t flih_test_handle_msg(const psa_msg_t *msg)
{
    psa_status_t status;

    switch (msg->type) {
    case TFM_FLIH_TEST_CASE_1:
        flih_test_case_1(msg, TFM_TIMER0_IRQ_SIGNAL);
        status = PSA_SUCCESS;
        break;
    case TFM_FLIH_TEST_CASE_2:
        flih_test_case_2(msg, TFM_TIMER0_IRQ_SIGNAL);
        status = PSA_SUCCESS;
        break;
    default:
        LOG_ERRFMT("FLIH test service: Invalid message type: 0x%x\r\n",
            msg->type);
        status = PSA_ERROR_PROGRAMMER_ERROR;
        break;
    }

    return status;
}

#if TFM_SP_FLIH_TEST_MODEL_IPC == 1
static void flih_test_get_msg(psa_signal_t signal, psa_msg_t *msg)
{
    psa_status_t status;

    status = psa_get(signal, msg);
    if (status != PSA_SUCCESS) {
        psa_panic();
    }
}

void tfm_flih_test_service_entry(void)
{
    psa_signal_t signals = 0;
    psa_msg_t msg;

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_FLIH_TEST_CASE_SIGNAL) {
            flih_test_get_msg(TFM_FLIH_TEST_CASE_SIGNAL, &msg);
            psa_reply(msg.handle, flih_test_handle_msg(&msg));
        }
    }
}
#elif TFM_SP_FLIH_TEST_MODEL_SFN == 1
psa_status_t tfm_flih_test_case_sfn(const psa_msg_t *msg)
{
    return flih_test_handle_msg(msg);
}
#endif
