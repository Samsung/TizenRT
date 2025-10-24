/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "trng.h"
#include <string.h>

int32_t bl1_trng_generate_random(uint8_t *output, size_t output_size)
{
    if (output_size == 0) {
        return 0;
    }

    if (output == NULL) {
        return -1;
    }

    memset(output, 4, output_size);
    return 0;
}

unsigned char fih_delay_random_uchar(void)
{
    return 4;
}
