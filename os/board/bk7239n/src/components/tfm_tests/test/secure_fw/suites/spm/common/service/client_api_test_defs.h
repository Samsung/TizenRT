/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CLIENT_API_TEST_DEFS_H__
#define __CLIENT_API_TEST_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Test error codes */
#define CLIENT_API_TEST_ERROR_FAIL     (-1)

/* Test service types */
#define CLIENT_API_TEST_TYPE_REQUEST_SRV    (0x1000)

/* Test data */
#define CLIENT_API_TEST_INPUT_DATA     0xDEADBEEF
#define CLIENT_API_TEST_OUTPUT_DATA    0xCAFEBABE

#ifdef __cplusplus
}
#endif

#endif /* __CLIENT_API_TEST_DEFS_H__ */
