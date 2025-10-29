/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crt_impl_private.h"

void *memset(void *s, int c, size_t n)
{
    union tfm_mem_addr_t p_mem;
    uint32_t quad_pattern;

    p_mem.p_byte = (uint8_t *)s;
    quad_pattern = (((uint8_t)c) << 24) | (((uint8_t)c) << 16) |
                   (((uint8_t)c) << 8) | ((uint8_t)c);

    while (n && (p_mem.uint_addr & (sizeof(uint32_t) - 1))) {
        *p_mem.p_byte++ = (uint8_t)c;
        n--;
    }

    while (n >= sizeof(uint32_t)) {
        *p_mem.p_qbyte++ = quad_pattern;
        n -= sizeof(uint32_t);
    }

    while (n--) {
        *p_mem.p_byte++ = (uint8_t)c;
    }

    return s;
}
