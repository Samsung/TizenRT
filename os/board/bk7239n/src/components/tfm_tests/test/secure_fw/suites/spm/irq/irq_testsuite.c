/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "psa_manifest/sid.h"
#include "psa/client.h"
#include "test_framework.h"
#include "tfm_peripherals_def.h"
#include "tfm_plat_test.h"
#include "irq_test.h"

#ifdef TEST_NS_SLIH_IRQ
static void tfm_irq_test_slih_case_1(struct test_result_t *ret)
{
    irq_test_slih_case_1(ret, TFM_SLIH_TEST_CASE_HANDLE);
}
#endif /* TEST_NS_SLIH_IRQ */

#ifdef TEST_NS_FLIH_IRQ
static void tfm_irq_test_flih_case_1(struct test_result_t *ret)
{
    irq_test_flih_case_1(ret, TFM_FLIH_TEST_CASE_HANDLE);
}

static void tfm_irq_test_flih_case_2(struct test_result_t *ret)
{
    irq_test_flih_case_2(ret, TFM_FLIH_TEST_CASE_HANDLE);
}
#endif /* TEST_NS_FLIH_IRQ */

static struct test_t irq_test_cases[] = {
#ifdef TEST_NS_SLIH_IRQ
    {&tfm_irq_test_slih_case_1, "TFM_NS_IRQ_TEST_SLIH_1001",
     "SLIH HANDLING Case 1"},
#endif /* TEST_NS_SLIH_IRQ */
#ifdef TEST_NS_FLIH_IRQ
    {&tfm_irq_test_flih_case_1, "TFM_NS_IRQ_TEST_FLIH_1101",
     "FLIH HANDLING not returning signal"},
    {&tfm_irq_test_flih_case_2, "TFM_NS_IRQ_TEST_FLIH_1102",
     "FLIH HANDLING returning Signal"},
#endif /* TEST_NS_FLIH_IRQ */
};

void register_testsuite_irq_test(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(irq_test_cases) / sizeof(irq_test_cases[0]));

    set_testsuite("TFM IRQ Test (TFM_IRQ_TEST_1xxx)",
                  irq_test_cases, list_size, p_test_suite);
}
