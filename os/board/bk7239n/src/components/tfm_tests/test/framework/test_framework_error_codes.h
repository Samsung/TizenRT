/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TEST_FRAMEWORK_ERROR_CODES_H__
#define __TEST_FRAMEWORK_ERROR_CODES_H__

enum test_suite_err_t {
    TEST_SUITE_ERR_NO_ERROR = 0,           /*!< No error */
    TEST_SUITE_ERR_INVALID_DATA = 1,       /*!< Invalid test suite if any of the
                                            *   pointers is NULL
                                            */
    TEST_SUITE_ERR_INVALID_TEST_DATA = 2,  /*!< Invalid test if any of the
                                            *  pointers is NULL
                                            */
    TEST_SUITE_ERR_TEST_FAILED  = 3,       /*!< Last executed test has failed */
};

#endif /* __TEST_FRAMEWORK_ERROR_CODES_H__ */
