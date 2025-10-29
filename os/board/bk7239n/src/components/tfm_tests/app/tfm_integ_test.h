/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_INTEG_TEST_H__
#define __TFM_INTEG_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Simple macro to mark UNUSED variables
 *
 */
#define UNUSED_VARIABLE(X) ((void)(X))

#if defined(TEST_FRAMEWORK_NS) || defined(TEST_FRAMEWORK_S)
/**
 * \brief Main test application for the RTX-TFM core
 *        integration tests
 *
 */
void test_app(void *argument);
#endif /* TEST_FRAMEWORK_NS || TEST_FRAMEWORK_S */

#ifdef __cplusplus
}
#endif

#endif /* __TFM_INTEG_TEST_H__ */
