/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MMIOVEC_TEST_H__
#define __MMIOVEC_TEST_H__

#include "test_framework.h"
#include "psa/client.h"

/**
 * \brief Mapping input vectors and unmapping them.
 *
 * \param[in] handle            Service handle to call the test service.
                                It can be a stateless handle or a handle
                                returned by psa_connect. Note this API does not
                                call psa_close after use of this handle.
 * \param[out] ret              Test results.
 */
void invec_map_unmap_test(struct test_result_t *ret, psa_handle_t handle);

/**
 * \brief Mapping output vectors and unmapping them.
 *
 * \param[in] handle            Service handle to call the test service.
                                It can be a stateless handle or a handle
                                returned by psa_connect. Note this API does not
                                call psa_close after use of this handle.
 * \param[out] ret              Test results.
 */
void outvec_map_unmap_test(struct test_result_t *ret, psa_handle_t handle);

/**
 * \brief Mapping input vectors and not unmapping them.
 *
 * \param[in] handle            Service handle to call the test service.
                                It can be a stateless handle or a handle
                                returned by psa_connect. Note this API does not
                                call psa_close after use of this handle.
 * \param[out] ret              Test results.
 */
void outvec_map_only_test(struct test_result_t *ret, psa_handle_t handle);

#endif /* __MMIOVEC_TEST_H__ */
