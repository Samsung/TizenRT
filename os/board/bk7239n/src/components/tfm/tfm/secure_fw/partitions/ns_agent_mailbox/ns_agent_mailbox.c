/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/service.h"
#include "psa_manifest/ns_agent_mailbox.h"
#include "tfm_hal_multi_core.h"
#include "tfm_hal_platform.h"
#include "tfm_multi_core.h"
#include "tfm_rpc.h"
#include "tfm_sp_log.h"

static void boot_ns_core(void)
{
    /* Boot up non-secure core */
    LOG_DBGFMT("Enabling non-secure core...\r\n");

    tfm_hal_boot_ns_cpu(tfm_hal_get_ns_VTOR());
    tfm_hal_wait_for_ns_cpu_ready();
}

void ns_agent_mailbox_entry(void)
{
    psa_signal_t signals = 0;

    boot_ns_core();

    tfm_inter_core_comm_init();

    psa_irq_enable(MAILBOX_SIGNAL);

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & MAILBOX_SIGNAL) {
            psa_eoi(MAILBOX_SIGNAL);
            tfm_rpc_client_call_handler();
        } else {
            psa_panic();
        }
    }
}
