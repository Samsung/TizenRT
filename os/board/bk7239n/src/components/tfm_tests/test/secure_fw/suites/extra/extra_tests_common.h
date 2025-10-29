/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __EXTRA_TESTS_COMMON_H__
#define __EXTRA_TESTS_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define EXTRA_TEST_SUCCESS 0
#define EXTRA_TEST_FAILED -1

struct extra_tests_t
{
    int32_t (*test_entry)(void); /* Entry function to call */
    int32_t expected_ret;        /* Expected return value */
};

/**
 * \brief A function for extra test to init.
 *
 * \param[out] internal_test_t An internal pointer used to copy platform test.
 *
 * \returns Return EXTRA_TEST_SUCCESS if succeeds. Otherwise, return
 *          EXTRA_TEST_FAILED.
 */
int32_t extra_tests_init(struct extra_tests_t *internal_test_t);

/**
 * \brief A function for platform to register test entry.
 *
 * \param[out] internal_test_t An internal pointer used to copy platform test.
 * \param[in]  platform_test_t An external pointer to transfer platform test.
 *
 * \returns Return EXTRA_TEST_SUCCESS if succeeds. Otherwise, return
 *          EXTRA_TEST_FAILED.
 */
int32_t register_extra_tests(struct extra_tests_t *internal_test_t,
                     const struct extra_tests_t *platform_test_t);

#ifdef __cplusplus
}
#endif

#endif /* __EXTRA_TESTS_COMMON_H__ */
