/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "runtime_defs.h"
#include "sprt_partition_metadata_indicator.h"

#include "psa/error.h"
#include "psa/service.h"

void common_sfn_thread(void)
{
    psa_signal_t sig_asserted, signal_mask, sig;
    psa_msg_t msg = {0};
    struct runtime_metadata_t *meta;
    service_fn_t *p_sfn_table;
    sfn_init_fn_t sfn_init;

    meta = PART_METADATA();
    sfn_init = (sfn_init_fn_t)meta->entry;
    p_sfn_table = (service_fn_t *)meta->sfn_table;
    signal_mask = (1 << meta->n_sfn) - 1;

    if (sfn_init && sfn_init() != PSA_SUCCESS) {
        psa_panic();
    }

    while (1) {
        sig_asserted = psa_wait(signal_mask, PSA_BLOCK);
        /* Handle signals */
        for (int i = 0; sig_asserted != 0 && i < meta->n_sfn; i++) {
            sig = 1 << i;
            if (sig_asserted & sig) {
                /* The i bit signal asserted, index of SFN is i as well */
                if (!p_sfn_table[i]) {
                    /* No corresponding SFN */
                    psa_panic();
                    return;
                }

                psa_get(sig, &msg);
                psa_reply(msg.handle, ((service_fn_t)p_sfn_table[i])(&msg));
                sig_asserted &= ~sig;
            }
        }

        if (sig_asserted != 0) {
            /* Wrong signal asserted */
            psa_panic();
        }
    }
}
