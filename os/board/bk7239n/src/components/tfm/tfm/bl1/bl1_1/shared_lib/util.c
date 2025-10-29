/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "util.h"

#include "trng.h"
#include "fih.h"

/* The average roll should be 4 by the CLT, and our secrets are usually 32
 * bytes. Do 8 + 3 so there's a bit of extra. Should always be odd so the reseed
 * oscillates between before and after the forward step.
 */
#define RNG_CHUNK_BYTES  (11)
/* Reverse every between 0 and 7 bytes */
#define SHUFFLE_MASK     (0x7)

fih_int bl_secure_memeql(const void *ptr1, const void *ptr2, size_t num)
{
    fih_int is_equal = FIH_SUCCESS;
    size_t block_start;
    size_t block_end;
    size_t curr = 0;
    uint8_t rnd[RNG_CHUNK_BYTES];
    size_t rnd_curr_idx = sizeof(rnd);

    /* Do comparison. Every n bytes (where n is random between 1 and 9),
     * reverse the direction.
     */
    while (curr < num) {
        /* Only generate more entropy if we've run out */
        if (rnd_curr_idx == sizeof(rnd)) {
            bl1_trng_generate_random(rnd, sizeof(rnd));
            rnd_curr_idx = 0;
        }

        /* Forward case. Always at least one byte */
        block_start = curr;
        block_end = curr + (rnd[rnd_curr_idx++] & SHUFFLE_MASK) + 1;

        if (block_end > num) {
            block_end = num;
        }

        for (; curr < block_end; curr++) {
            if (((uint8_t *)ptr1)[curr] != ((uint8_t *)ptr2)[curr]) {
                is_equal = FIH_FAILURE;
            }
        }


        /* Only generate more entropy if we've run out */
        if (rnd_curr_idx == sizeof(rnd)) {
            bl1_trng_generate_random(rnd, sizeof(rnd));
            rnd_curr_idx = 0;
        }

        /* Reverse case. Always at least one byte */
        block_start = curr;
        block_end = curr + (rnd[rnd_curr_idx++] & SHUFFLE_MASK) + 1;

        if (block_end > num) {
            block_end = num;
        }

        for (curr = block_end - 1; curr >= block_start; curr--) {
            if (((uint8_t *)ptr1)[curr] != ((uint8_t *)ptr2)[curr]) {
                is_equal = FIH_FAILURE;
            }
        }
        curr = block_end;
    }
    if (curr != num) {
        FIH_PANIC;
    }

    FIH_RET(is_equal);
}

fih_int bl_secure_memcpy(void *destination, const void *source, size_t num)
{
    size_t block_start;
    size_t block_end;
    int64_t curr = 0;
    uint8_t rnd[RNG_CHUNK_BYTES];
    size_t rnd_curr_idx = sizeof(rnd);

    /* Do copy. Every n bytes (where n is random between 1 and 17), reverse the
     * direction.
     */
    while (curr < num) {
        /* Only generate more entropy if we've run out */
        if (rnd_curr_idx == sizeof(rnd)) {
            bl1_trng_generate_random(rnd, sizeof(rnd));
            rnd_curr_idx = 0;
        }

        /* Forward case */
        bl1_trng_generate_random(rnd, sizeof(rnd));
        block_start = curr;
        block_end = curr + (rnd[rnd_curr_idx++] & SHUFFLE_MASK) + 1;

        if (block_end > num) {
            block_end = num;
        }

        for (; curr < block_end; curr++) {
            ((uint8_t *)destination)[curr] = ((uint8_t *)source)[curr];
        }


        /* Only generate more entropy if we've run out */
        if (rnd_curr_idx == sizeof(rnd)) {
            bl1_trng_generate_random(rnd, sizeof(rnd));
            rnd_curr_idx = 0;
        }

        /* Reverse case */
        block_start = curr;
        block_end = curr + (rnd[rnd_curr_idx++] & SHUFFLE_MASK) + 1;

        if (block_end > num) {
            block_end = num;
        }

        for (curr = block_end - 1; block_start <= curr; curr--) {
            ((uint8_t *)destination)[curr] = ((uint8_t *)source)[curr];
        }
        curr = block_end;
    }
    if (curr != num) {
        FIH_PANIC;
    }

    FIH_RET(FIH_SUCCESS);
}
