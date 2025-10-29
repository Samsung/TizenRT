/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FWU_S_TESTS_H__
#define __FWU_S_TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "test_framework.h"

/**
 * \brief Register testsuite for the PSA Firmware Update service secure
 *        interface tests.
 *
 * \param[in] p_test_suite  The test suite to be executed.
 */
void register_testsuite_s_psa_fwu_interface(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __FWU_S_TESTS_H__ */
