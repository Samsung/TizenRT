/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_ps_nv_counters.h"
#include <limits.h>
#include "nv_counters/ps_nv_counters.h"
#include "ps_utils.h"
#include "tfm_platform_api.h"
#include "tfm_plat_otp.h"
#include "cmsis_compiler.h"
#include <string.h>
#include "otp.h"

#define DISABLE_INCREMENT 0
#define ENABLE_INCREMENT  1

#define TOTAL_PS_NV_COUNTERS  3
#define INIT_NV_COUNTERS_VALUE 42

static uint8_t nv_increment_status = ENABLE_INCREMENT;
static uint32_t test_nv_counters[TOTAL_PS_NV_COUNTERS] = {
                                                [0] = INIT_NV_COUNTERS_VALUE,
                                                [1] = INIT_NV_COUNTERS_VALUE,
                                                [2] = INIT_NV_COUNTERS_VALUE
                };

uint32_t test_suite_is_testing(void);

static uint32_t get_nv_counter_position(enum tfm_nv_counter_t counter_id)
{
    switch (counter_id) {
    case TFM_PS_NV_COUNTER_1:
        return 0;
    case TFM_PS_NV_COUNTER_2:
        return 1;
    case TFM_PS_NV_COUNTER_3:
        return 2;
    default:
        return TOTAL_PS_NV_COUNTERS;
    }
}

psa_status_t ps_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                 uint32_t *val)
{
	if (test_suite_is_testing()){
	    uint32_t nv_pos;

	    nv_pos = get_nv_counter_position(counter_id);
	    if (nv_pos >= TOTAL_PS_NV_COUNTERS) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }

	    /* Reads counter value */
	    *val = test_nv_counters[nv_pos];
	}
	else{
	    enum tfm_platform_err_t err;

	    err = tfm_platform_nv_counter_read(counter_id, PS_NV_COUNTER_SIZE,
	                                       (uint8_t *)val);
	    if (err != TFM_PLATFORM_ERR_SUCCESS) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }
	}

    return PSA_SUCCESS;
}

psa_status_t ps_increment_nv_counter(enum tfm_nv_counter_t counter_id)
{
	if (test_suite_is_testing()){
	    uint32_t nv_pos;

	    if (nv_increment_status == DISABLE_INCREMENT) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }

	    nv_pos = get_nv_counter_position(counter_id);
	    if (nv_pos >= TOTAL_PS_NV_COUNTERS) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }

	    if (test_nv_counters[nv_pos] == UINT32_MAX) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }

	    /* Increments counter value */
	    test_nv_counters[nv_pos]++;
	}else{
	    enum tfm_platform_err_t err;

	    /* NOTE: tfm_plat_increment_nv_counter returns TFM_PLAT_ERR_MAX_VALUE when
	     *       the counter reaches its maximum value. The current PS
	     *       implementation treats this condition as an error as, from that
	     *       moment onwards, the rollback protection can not be achieved based
	     *       on the NV counters.
	     */
	    err = tfm_platform_nv_counter_increment(counter_id);
	    if (err != TFM_PLATFORM_ERR_SUCCESS) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }
	}

    return PSA_SUCCESS;
}

/* Implementation of PS NV counter interfaces defined by
 * test_ps_nv_counters.h
 */
void test_ps_disable_increment_nv_counter(void)
{
    nv_increment_status = DISABLE_INCREMENT;
}

void test_ps_enable_increment_nv_counter(void)
{
    nv_increment_status = ENABLE_INCREMENT;
}

psa_status_t test_ps_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                      uint32_t *val)
{
    return ps_read_nv_counter(counter_id, val);
}

psa_status_t test_ps_increment_nv_counter(enum tfm_nv_counter_t counter_id)
{
    return ps_increment_nv_counter(counter_id);
}

psa_status_t test_ps_decrement_nv_counter(enum tfm_nv_counter_t counter_id)
{
	if (test_suite_is_testing()){
	    uint32_t nv_pos;

	    nv_pos = get_nv_counter_position(counter_id);
	    if (nv_pos >= TOTAL_PS_NV_COUNTERS) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }

	    if (test_nv_counters[nv_pos] == 0) {
	        return PSA_ERROR_GENERIC_ERROR;
	    }

	    /* Decrements counter value */
	    test_nv_counters[nv_pos]--;
	}

    return PSA_SUCCESS;
}

psa_status_t test_ps_set_nv_counter(enum tfm_nv_counter_t counter_id,
                                    uint32_t value)
{
    uint32_t nv_pos;

    nv_pos = get_nv_counter_position(counter_id);
    if (nv_pos >= TOTAL_PS_NV_COUNTERS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Sets counter value */
    test_nv_counters[nv_pos] = value;

    return PSA_SUCCESS;
}
