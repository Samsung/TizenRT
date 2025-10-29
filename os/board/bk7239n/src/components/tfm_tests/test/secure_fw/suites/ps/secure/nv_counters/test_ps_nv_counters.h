/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TEST_PS_NV_COUNTERS_H__
#define __TEST_PS_NV_COUNTERS_H__

#include <stdint.h>
#include "psa/protected_storage.h"
#include "tfm_plat_nv_counters.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Reads the given non-volatile (NV) counter.
 *
 * \param[in]  counter_id  NV counter ID.
 * \param[out] val         Pointer to store the current NV counter value.
 *
 * \return  PSA_SUCCESS if the value is read correctly, otherwise
 *          PSA_ERROR_GENERIC_ERROR
 */
psa_status_t test_ps_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                      uint32_t *val);

/**
 * \brief Increments the given non-volatile (NV) counter.
 *
 * \param[in] counter_id  NV counter ID.
 *
 * \return  When the NV counter reaches its maximum value, the
 *          PSA_ERROR_GENERIC_ERROR error is returned to indicate the
 *          value cannot be incremented. Otherwise, PSA_SUCCESS.
 */
psa_status_t test_ps_increment_nv_counter(enum tfm_nv_counter_t counter_id);

/**
 * \brief Decrements the given non-volatile (NV) counter.
 *
 * \param[in] counter_id  NV counter ID.
 *
 * \return  When the NV counter reaches its minimum value, the
 *          PSA_ERROR_GENERIC_ERROR error is returned to indicate the
 *          value cannot be decremented. Otherwise, PSA_SUCCESS.
 */
psa_status_t test_ps_decrement_nv_counter(enum tfm_nv_counter_t counter_id);

/**
 * \brief Disables PS increment nv counter function to force
 *        PSA_ERROR_GENERIC_ERROR return value as an indication that NV
 *        counter reaches its maximum value.
 */
void test_ps_disable_increment_nv_counter(void);

/**
 * \brief Enables PS increment nv counter function to work normally.
 */
void test_ps_enable_increment_nv_counter(void);

/**
 * \brief Sets a new value into the given non-volatile (NV) counter.
 *
 * \param[in] counter_id  NV counter Id.
 * \param[in] value       New NV counter value.
 *
 * \return  When the NV counter reaches its maximum value, the
 *          PSA_ERROR_GENERIC_ERROR error is returned to indicate the
 *          value cannot be set. Otherwise, PSA_SUCCESS.
 */
psa_status_t test_ps_set_nv_counter(enum tfm_nv_counter_t counter_id,
                                     uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_PS_NV_COUNTERS_H__ */
