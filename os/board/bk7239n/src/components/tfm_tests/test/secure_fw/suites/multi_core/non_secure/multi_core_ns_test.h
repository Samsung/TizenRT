/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MULTI_CORE_NS_TESTS_H__
#define __MULTI_CORE_NS_TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "test_framework.h"

/**
 * \brief Register testsuite for multi-core topology.
 *
 * \param[in] p_test_suite The test suite to be executed.
 */
void register_testsuite_multi_core_ns_interface(
                                             struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __MULTI_CORE_NS_TESTS_H__ */
