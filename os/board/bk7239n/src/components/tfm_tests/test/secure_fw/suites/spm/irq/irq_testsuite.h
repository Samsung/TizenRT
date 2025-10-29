/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __IRQ_TESTSUITE_H__
#define __IRQ_TESTSUITE_H__

#include "test_framework.h"

#ifdef __cplusplus
extern "C" {
#endif

void register_testsuite_irq_test(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __IRQ_TESTSUITE_H__ */
