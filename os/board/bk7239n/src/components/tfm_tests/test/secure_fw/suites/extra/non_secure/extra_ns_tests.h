/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#ifndef __EXTRA_NS_TESTS_H__
#define __EXTRA_NS_TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "extra_tests_common.h"
#include "test_framework.h"

/**
 * \brief Entry function used for extra non-secure test.
 *
 * \param[out] ret Returns error code as specified in \ref test_result_t.
 */
void extra_ns_tests_entry(struct test_result_t *ret);

/**
 * \brief Register testsuite for the extra non-secure test.
 *
 * \param[in] p_test_suite The test suite to be executed.
 */
void register_testsuite_extra_ns_interface(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __EXTRA_NS_TESTS_H__ */
