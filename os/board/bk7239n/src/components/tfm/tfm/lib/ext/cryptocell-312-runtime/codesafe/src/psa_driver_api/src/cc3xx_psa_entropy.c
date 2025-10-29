/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_entropy.c
 *
 * This file contains the implementation of the entry points associated
 * to the entropy capability as described by the PSA Cryptoprocessor
 * Driver interface specification
 *
 */

#include "cc3xx_psa_entropy.h"
#include "cc_pal_log.h"
#include "cc_pal_mem.h"
#include "cc_rnd_common.h"
#include "cc_rng_plat.h"
#include "llf_rnd_trng.h"

/** \defgroup psa_entropy PSA driver entry points for entropy collection
 *
 *  Entry points for entropy collection from the TRNG source as described by the
 *  PSA Cryptoprocessor Driver interface specification. The TRNG
 *  operates by sampling the output of a fast free-running ring oscillator in a
 *  different (slower) clock domain
 *
 *  @{
 */
psa_status_t cc3xx_get_entropy(uint32_t flags, size_t *estimate_bits,
                               uint8_t *output, size_t output_size)
{
    CCRndWorkBuff_t rndWorkBuff;
    CCRndState_t rndState;
    CCRndParams_t trngParams;
    CCError_t cc_err;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    uint32_t *entrSource_ptr;
    uint32_t estimate_bytes;

    if (output == NULL) {
        CC_PAL_LOG_ERR("output cannot be NULL\n");
        status = PSA_ERROR_INVALID_ARGUMENT;
        goto end;
    }

    if (estimate_bits == NULL) {
        CC_PAL_LOG_ERR("estimate_bits cannot be NULL\n");
        status = PSA_ERROR_INVALID_ARGUMENT;
        goto end;
    }

    if (output_size == 0) {
        CC_PAL_LOG_ERR("output_size cannot be 0\n");
        status = PSA_ERROR_INVALID_ARGUMENT;
        goto end;
    }

    CC_PalMemSetZero(&rndWorkBuff, sizeof(CCRndWorkBuff_t));
    CC_PalMemSetZero(&rndState, sizeof(CCRndState_t));
    CC_PalMemSetZero(&trngParams, sizeof(CCRndParams_t));

    cc_err = RNG_PLAT_SetUserRngParameters(&trngParams);
    if (cc_err != CC_OK) {
        CC_PAL_LOG_ERR("Error: RNG_PLAT_SetUserRngParameters() failed.\n");
        status = PSA_ERROR_HARDWARE_FAILURE;
        goto end;
    }

    cc_err = LLF_RND_GetTrngSource(&rndState,   /*in/out*/
                                   &trngParams, /*in/out*/
                                   0,           /*in  -  isContinued - false*/
                                   NULL,        /*unused*/
                                   &entrSource_ptr,             /*out*/
                                   (uint32_t *)&estimate_bytes, /*out*/
                                   (uint32_t *)&rndWorkBuff,    /*in*/
                                   0 /*unused*/);
    if (cc_err != CC_OK) {
        status = PSA_ERROR_HARDWARE_FAILURE;
    } else {

        CC_PalMemCopy(output,
                      entrSource_ptr + CC_RND_TRNG_SRC_INNER_OFFSET_WORDS,
                      output_size);
        *estimate_bits = PSA_BYTES_TO_BITS(estimate_bytes);

        status = PSA_SUCCESS;

        CC_PalMemSetZero(&rndWorkBuff, sizeof(CCRndWorkBuff_t));
        CC_PalMemSetZero(&rndState, sizeof(CCRndState_t));
        CC_PalMemSetZero(&trngParams, sizeof(CCRndParams_t));
    }

end:
    return status;
}
/** @} */ // end of psa_entropy
