/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SFN_S_TESTS_H__
#define __SFN_S_TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "test_framework.h"

/**
 * \brief Register testsuite for sfn Secure interface.
 *
 * \param[in] p_test_suite The test suite to be executed.
 */
void register_testsuite_s_sfn_interface(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __SFN_S_TESTS_H__ */
