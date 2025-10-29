/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MCUBOOT_SUITES_H
#define MCUBOOT_SUITES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "test_framework_error_codes.h"

enum test_suite_err_t run_mcuboot_testsuite(void);

#ifdef __cplusplus
}
#endif

#endif /* MCUBOOT_SUITES_H */
