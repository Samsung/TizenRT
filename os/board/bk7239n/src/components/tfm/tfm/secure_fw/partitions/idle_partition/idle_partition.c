/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "fih.h"
#include "psa/service.h"

void tfm_idle_thread(void)
{
    while (1) {
        /*
         * There could be other Partitions becoming RUNABLE after wake up.
         * This is a dummy psa_wait to let SPM check possible scheduling.
         * It does not expect any signals.
         */
        if (psa_wait(PSA_WAIT_ANY, PSA_POLL) == 0) {
            __WFI();
        }
    }

#ifdef TFM_FIH_PROFILE_ON
    fih_delay();

    while (1) {
        /*
         * There could be other Partitions becoming RUNABLE after wake up.
         * This is a dummy psa_wait to let SPM check possible scheduling.
         * It does not expect any signals.
         */
        if (psa_wait(PSA_WAIT_ANY, PSA_POLL) == 0) {
            __WFI();
        }
    }
#endif
}
