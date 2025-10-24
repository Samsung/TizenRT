/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/framework_feature.h"
#include "test_framework.h"

/**
 * \brief Retrieve the version of the PSA Framework API.
 */
void tfm_sfn_test_1001(struct test_result_t *ret);

/**
 * \brief Retrieve the version of an RoT Service.
 */
void tfm_sfn_test_1002(struct test_result_t *ret);

/**
 * \brief Request a connection-based RoT Service
 */
void tfm_sfn_test_1003(struct test_result_t *ret);

/**
 * \brief Request a stateless RoT Service
 */
void tfm_sfn_test_1004(struct test_result_t *ret);

#if PSA_FRAMEWORK_HAS_MM_IOVEC
/**
 * \brief Mapping input vectors and unmapping them.
 *
 * \note Test psa_map_invec() and psa_unmap_invec() functionality.
 */
void tfm_sfn_test_1005(struct test_result_t *ret);

/**
 * \brief Mapping output vectors and unmapping them.
 *
 * \note Test psa_map_outvec() and psa_unmap_outvec() functionality.
 */
void tfm_sfn_test_1006(struct test_result_t *ret);

/**
 * \brief Mapping output vectors and not unmapping them.
 *
 * \note RoT services map outvecs and does not unmap outvecs, the service caller
 *       should get a zero out length.
 */
void tfm_sfn_test_1007(struct test_result_t *ret);

#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */
