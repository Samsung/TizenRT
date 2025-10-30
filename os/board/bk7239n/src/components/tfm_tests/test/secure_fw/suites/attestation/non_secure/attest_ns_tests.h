/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ATTESTATION_NS_TESTS_H__
#define __ATTESTATION_NS_TESTS_H__

#include "test_framework.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Register testsuite for the initial attestation service.
 *
 * \param[in] p_test_suite The test suite to be executed.
 */
void
register_testsuite_ns_attestation_interface(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __ATTESTATION_NS_TESTS_H__ */
