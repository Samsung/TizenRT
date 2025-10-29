/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "utilities.h"

#define GET_MEM_ADDR_BIT0(x)        ((x) & 0x1)
#define GET_MEM_ADDR_BIT1(x)        ((x) & 0x2)

union tfm_mem_addr_t {
    uintptr_t uint_addr;        /* Address          */
    uint8_t *p_byte;            /* Byte copy        */
    uint16_t *p_dbyte;          /* Double byte copy */
    uint32_t *p_qbyte;          /* Quad byte copy   */
};

void *spm_memcpy(void *dest, const void *src, size_t n)
{
    union tfm_mem_addr_t p_dest, p_src;

    p_dest.uint_addr = (uintptr_t)dest;
    p_src.uint_addr = (uintptr_t)src;

    /* Byte copy for unaligned address. check the last bit of address. */
    while (n && (GET_MEM_ADDR_BIT0(p_dest.uint_addr) ||
           GET_MEM_ADDR_BIT0(p_src.uint_addr))) {
        *p_dest.p_byte++ = *p_src.p_byte++;
        n--;
    }

    /*
     * Double byte copy for aligned address.
     * Check the 2nd last bit of address.
     */
    while (n >= sizeof(uint16_t) && (GET_MEM_ADDR_BIT1(p_dest.uint_addr) ||
           GET_MEM_ADDR_BIT1(p_src.uint_addr))) {
        *(p_dest.p_dbyte)++ = *(p_src.p_dbyte)++;
        n -= sizeof(uint16_t);
    }

    /* Quad byte copy for aligned address. */
    while (n >= sizeof(uint32_t)) {
        *(p_dest.p_qbyte)++ = *(p_src.p_qbyte)++;
        n -= sizeof(uint32_t);
    }

    /* Byte copy for the remaining bytes. */
    while (n--) {
        *p_dest.p_byte++ = *p_src.p_byte++;
    }

    return dest;
}

void *spm_memset(void *s, int c, size_t n)
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
