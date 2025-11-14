/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SFN_NS_TESTS_H__
#define __SFN_NS_TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "test_framework.h"

/**
 * \brief Register testsuite for sfn non-secure interface.
 *
 * \param[in] p_test_suite The test suite to be executed.
 */
void register_testsuite_ns_sfn_interface(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __SFN_NS_TESTS_H__ */
