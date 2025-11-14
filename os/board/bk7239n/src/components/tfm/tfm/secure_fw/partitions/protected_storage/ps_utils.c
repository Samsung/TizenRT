/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "ps_utils.h"
#include "tfm_memory_utils.h"

psa_status_t ps_utils_check_contained_in(uint32_t superset_size,
                                         uint32_t subset_offset,
                                         uint32_t subset_size)
{
    /* Check that subset_offset is valid */
    if (subset_offset > superset_size) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check that subset_offset + subset_size fits in superset_size.
     * The previous check passed, so we know that subset_offset <= superset_size
     * and so the right hand side of the inequality cannot underflow.
     */
    if (subset_size > (superset_size - subset_offset)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return PSA_SUCCESS;
}
