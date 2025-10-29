/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_internal_chacha20.h"

#include "cc_pal_types.h"
#include "cc_pal_mem.h"
#include "cc_pal_abort.h"

/** \file cc3xx_internal_chacha20.c
 *
 * This file contains the implementation of the internal functions to
 * perform symmetric encryption and decryption using the Chacha20
 * algorithm
 *
 */

/* This function interacts directly with the low level driver module */
static psa_status_t chacha_block(ChachaContext_t *ctx, const uint8_t *input,
                                 uint8_t *output, size_t size)
{
    drvError_t drvRc;
    CCBuffInfo_t inBuffInfo;
    CCBuffInfo_t outBuffInfo;

    drvRc = SetDataBuffersInfo(input, size, &inBuffInfo,
                               output, size, &outBuffInfo);
    if (drvRc != 0) {
        CC_PAL_LOG_ERR("Bad I/O buffers\r\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    drvRc = ProcessChacha(ctx, &inBuffInfo, &outBuffInfo, size);
    if (drvRc != 0) {
        CC_PAL_LOG_ERR("cc3xx_chacha20_update failed: %d\r\n", drvRc);
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return PSA_SUCCESS;
}

/** \defgroup internal_chacha20 Internal Chacha20 functions
 *
 *  Internal functions used by the driver to perform Chacha20 cipher encryption
 *  and decryption
 *
 *  @{
 */
void cc3xx_chacha20_init(ChachaContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\r\n");
        return;
    }

    CC_PalMemSet(ctx, 0, sizeof(ChachaContext_t));

    ctx->inputDataAddrType  = DLLI_ADDR;
    ctx->outputDataAddrType = DLLI_ADDR;

    /* valid range is [0; CHACHA_BLOCK_SIZE_BYTES), while
     * CHACHA_BLOCK_SIZE_BYTES indicates empty state
     */
    ctx->state.keystream_start = CHACHA_BLOCK_SIZE_BYTES;
}

void cc3xx_chacha20_free(ChachaContext_t *ctx)
{
    if (NULL == ctx) {
        CC_PAL_LOG_ERR("ctx cannot be NULL\r\n");
        return;
    }

    CC_PalMemSet(ctx, 0, sizeof(ChachaContext_t));
}

psa_status_t cc3xx_chacha20_setkey(
        ChachaContext_t *ctx,
        const uint8_t *key,
        size_t key_size)
{
    if (ctx == NULL || key == NULL || key_size != CHACHA_256_BIT_KEY_SIZE) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    CC_PalMemCopy(ctx->keyBuf, key, CHACHA_256_BIT_KEY_SIZE);

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_set_nonce(ChachaContext_t *ctx,
                                      const uint8_t *nonce,
                                      size_t nonce_size)
{
    if (ctx == NULL || nonce == NULL || nonce_size != CHACHA_IV_96_SIZE_BYTES) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    ctx->nonceSize = NONCE_SIZE_96;
    CC_PalMemCopy(ctx->nonceBuf, nonce, CHACHA_IV_96_SIZE_BYTES);

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_set_counter(ChachaContext_t *ctx,
                                        uint32_t counter)
{
    if (ctx == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    ctx->blockCounterLsb = counter;
    ctx->blockCounterMsb = 0;

    /* Calling set_counter has the effect of resetting any
     * ongoing operation
     */
    ctx->state.keystream_start = CHACHA_BLOCK_SIZE_BYTES;

    return PSA_SUCCESS;
}

psa_status_t cc3xx_chacha20_update(
        ChachaContext_t *ctx,
        const uint8_t *input,
        size_t input_len,
        uint8_t *output,
        size_t output_size,
        size_t *output_len)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    if (ctx == NULL || input == NULL || output == NULL || output_len == NULL) {
        CC_PAL_LOG_ERR("Null pointer exception\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    *output_len = 0;

    if (input_len == 0) {
        return PSA_SUCCESS;
    }

    if (output_size < input_len) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    /* If there is some outstanding state we need to process that first */
    uint8_t keystream_size =
        (CHACHA_BLOCK_SIZE_BYTES - ctx->state.keystream_start) > 0 ?
        (CHACHA_BLOCK_SIZE_BYTES - ctx->state.keystream_start) : 0;
    size_t size_to_process = keystream_size < input_len ?
                             keystream_size : input_len;
    if (size_to_process) {
        for (int i=0; i<size_to_process; i++) {
            output[i] =
                ctx->state.keystream[i+ctx->state.keystream_start] ^ input[i];
        }
        ctx->state.keystream_start += size_to_process;
        input_len -= size_to_process;
        (*output_len) += size_to_process;

        if (input_len == 0) {
            return PSA_SUCCESS;
        }
    }

    size_t process_len =
        (input_len/CHACHA_BLOCK_SIZE_BYTES)*CHACHA_BLOCK_SIZE_BYTES;
    if (process_len) {
        status = chacha_block(ctx, &input[size_to_process],
                              &output[size_to_process], process_len);
        if (status != PSA_SUCCESS) {
            *output_len = 0;
            return status;
        }
        (*output_len) += process_len;
    }

    /* Store the last part of the data that couldn't be processed as separate
     * block
     */
    if (input_len - process_len) {
        /* Generate a keystream block to keep as state */
        uint8_t all_zero_input[CHACHA_BLOCK_SIZE_BYTES] = {0};
        status = chacha_block(ctx, all_zero_input, ctx->state.keystream,
                              CHACHA_BLOCK_SIZE_BYTES);
        if (status != PSA_SUCCESS) {
            *output_len = 0;
            return status;
        }

        for (int i=0; i<input_len-process_len; i++) {
            output[i+size_to_process+process_len] =
                ctx->state.keystream[i] ^ input[i+size_to_process+process_len];
        }

        (*output_len) += (input_len-process_len);

        /* Update the pointer to the start of the unprocessed keystream */
        ctx->state.keystream_start = (input_len-process_len);
    }

    return status;
}

psa_status_t cc3xx_chacha20_finish(
        ChachaContext_t *ctx,
        uint8_t *output,
        size_t output_size,
        size_t *output_length)
{
    (void)ctx;
    (void)output;
    (void)output_size;
    if (output_length == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    *output_length = 0;

    return PSA_SUCCESS;
}
/** @} */ // end of internal_chacha20
