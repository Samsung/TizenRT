/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MCUBOOT_INTEGRATION_TESTS_H
#define MCUBOOT_INTEGRATION_TESTS_H

#include "test_framework.h"

#ifdef __cplusplus
extern "C" {
#endif

void register_testsuite_mcuboot_integration(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* MCUBOOT_INTEGRATION_TESTS_H */
