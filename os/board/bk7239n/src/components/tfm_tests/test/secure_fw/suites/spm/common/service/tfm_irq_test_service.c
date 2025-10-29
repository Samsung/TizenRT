/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "spm_test_defs.h"
#include "tfm_irq_test_service.h"
#include "tfm_plat_test.h"
#include "psa/service.h"

#ifdef TEST_NS_FLIH_IRQ
/* The execution flow ensures there are no race conditions for test_type */
static int32_t test_type = TFM_FLIH_TEST_CASE_INVALID;
/*
 * Records times of triggered
 *
 * The test cases do not care about exact value of flih_timer_triggered.
 * They only needs to know if it has reached a certain value.
 * And it is a single-read-single-writer model.
 * So the race condition of accessing flih_timer_triggered between the Partition
 * thread and IRS is acceptable.
 */
volatile uint32_t flih_timer_triggered = 0;

psa_flih_result_t tfm_flih_test_timer_handler(void)
{
    tfm_plat_test_secure_timer_clear_intr();

    switch (test_type) {
    case TFM_FLIH_TEST_CASE_1:
        flih_timer_triggered += 1;
        return PSA_FLIH_NO_SIGNAL;
    case TFM_FLIH_TEST_CASE_2:
        flih_timer_triggered += 1;
        if (flih_timer_triggered == 10) {
            return PSA_FLIH_SIGNAL;
        } else {
            return PSA_FLIH_NO_SIGNAL;
        }
        break;
    default:
        psa_panic();
        break;
    }

    return PSA_FLIH_NO_SIGNAL;
}

void flih_test_case_1(const psa_msg_t *msg, psa_signal_t timer_irq_signal)
{
    test_type = msg->type;

    flih_timer_triggered = 0;

    psa_irq_enable(timer_irq_signal);

    tfm_plat_test_secure_timer_start();

    while (flih_timer_triggered < 10);
    tfm_plat_test_secure_timer_stop();

    psa_irq_disable(timer_irq_signal);
}

void flih_test_case_2(const psa_msg_t *msg, psa_signal_t timer_irq_signal)
{
    test_type = msg->type;

    flih_timer_triggered = 0;

    psa_irq_enable(timer_irq_signal);

    tfm_plat_test_secure_timer_start();

    if (psa_wait(timer_irq_signal, PSA_BLOCK) != timer_irq_signal) {
        psa_panic();
    }
    tfm_plat_test_secure_timer_stop();

    psa_reset_signal(timer_irq_signal);
    psa_irq_disable(timer_irq_signal);
}
#endif /* TEST_NS_FLIH_IRQ */

#ifdef TEST_NS_SLIH_IRQ
static void slih_test_timer_handler(psa_signal_t timer_irq_signal)
{
    tfm_plat_test_secure_timer_stop();
    psa_irq_disable(timer_irq_signal);
    psa_eoi(timer_irq_signal);
}

void slih_test_case_1(const psa_msg_t *msg, psa_signal_t timer_irq_signal)
{
    psa_irq_enable(timer_irq_signal);

    tfm_plat_test_secure_timer_start();

    if (psa_wait(timer_irq_signal, PSA_BLOCK) != timer_irq_signal) {
        psa_panic();
    }
    slih_test_timer_handler(timer_irq_signal);
}

#endif /* TEST_NS_SLIH_IRQ */
