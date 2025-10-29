/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>

/* Generates random bytes */
int32_t bl1_trng_generate_random(uint8_t *output, size_t output_size);
