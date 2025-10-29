/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CLIENT_API_TESTS_H__
#define __CLIENT_API_TESTS_H__

#include <stdint.h>

#include "test_framework.h"
#include "psa/client.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Test against the psa_framework_version() API */
void psa_framework_version_test(struct test_result_t *ret);

/* Test against the psa_version() API */
void psa_version_test(uint32_t sid, struct test_result_t *ret);

/*
 * Call psa_call with the given ``handle`` with inputs and outputs.
 * The handle can be either connection-based handle returned by psa_connect()
 * or stateless handle.
 *
 * The RoT Services should read out the invecs and validate the values.
 * And then write some data to outvecs.
 * The outputs are then validated to see if they are set to the expected values
 */
void request_rot_service_test(psa_handle_t handle,
                              struct test_result_t *ret);

#ifdef __cplusplus
}
#endif

#endif /* __CLIENT_API_TESTS_H__ */
