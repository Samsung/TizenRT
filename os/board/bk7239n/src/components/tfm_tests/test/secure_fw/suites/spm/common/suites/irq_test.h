/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __IRQ_TEST_H__
#define __IRQ_TEST_H__

#include "test_framework.h"
#include "psa/client.h"

#ifdef TEST_NS_SLIH_IRQ
/**
 * Test process:
 *    - NSPE starts testing
 *    - Test Partition starts timer
 *    - Test Partition waits for the timer signal
 *    - Test Partition receives the signal and stop timer
 *    - Test Partition returns to NSPE
 *    - Test finishes
 */
void irq_test_slih_case_1(struct test_result_t *ret, psa_handle_t handle);
#endif /* TEST_NS_SLIH_IRQ */

#ifdef TEST_NS_FLIH_IRQ
/**
 * Test process:
 *    - NSPE starts testing
 *    - Test Partition starts timer
 *    - Test Partition waits for the timer to be trigger for a certain count by
 *      reading the global count in a while loop
 *    - In the handling function, the count is increased
 *    - The count reaches the value and test Partition stops timer
 *    - Test Partition returns to NSPE
 *    - Test finishes
 */
void irq_test_flih_case_1(struct test_result_t *ret, psa_handle_t handle);

/**
 * Test process:
 *    - NSPE starts testing
 *    - Test Partition starts timer
 *    - Test Partition waits for the timer signal
 *    - In the handling function, the timer trigger count is increased
 *    - The count reaches a certain value and the ISR returns PSA_FLIH_SIGNAL
 *    - Test Partition receives the signal, stops timer and returns to NSPE
 *    - Test finishes
 */
void irq_test_flih_case_2(struct test_result_t *ret, psa_handle_t handle);
#endif /* TEST_NS_FLIH_IRQ */

#endif /* __IRQ_TEST_H__ */
