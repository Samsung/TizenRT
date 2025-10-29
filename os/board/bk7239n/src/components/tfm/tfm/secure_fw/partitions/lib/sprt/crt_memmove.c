/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "crt_impl_private.h"

static void *memcpy_r(void *dest, const void *src, size_t n)
{
    union tfm_mem_addr_t p_dest, p_src;

    p_dest.uint_addr = (uintptr_t)dest + n;
    p_src.uint_addr = (uintptr_t)src + n;

    /* Byte copy for unaligned address. check the last bit of address. */
    while (n && (GET_MEM_ADDR_BIT0(p_dest.uint_addr) ||
           GET_MEM_ADDR_BIT0(p_src.uint_addr))) {
        *(--p_dest.p_byte) = *(--p_src.p_byte);
        n--;
    }

    /* Double byte copy for aligned address.
     * Check the 2nd last bit of address.
     */
    while (n >= sizeof(uint16_t) && (GET_MEM_ADDR_BIT1(p_dest.uint_addr) ||
           GET_MEM_ADDR_BIT1(p_src.uint_addr))) {
        *(--p_dest.p_dbyte) = *(--p_src.p_dbyte);
        n -= sizeof(uint16_t);
    }

    /* Quad byte copy for aligned address. */
    while (n >= sizeof(uint32_t)) {
        *(--p_dest.p_qbyte) = *(--p_src.p_qbyte);
        n -= sizeof(uint32_t);
    }

    /* Byte copy for the remaining bytes. */
    while (n--) {
        *(--p_dest.p_byte) = *(--p_src.p_byte);
    }

    return dest;
}

/*
 * For overlapped memory area:
 * 1) overlapped: use reverse memory move.
 * 2) non-overlapped: use forward memory move.
 */
void *memmove(void *dest, const void *src, size_t n)
{
    /*
     * FixMe: Add a "assert (dest == NULL || src == NULL)" here
     * after "assert()" for sprtl is implemented.
     */
    if (src >= dest) {
        memcpy(dest, src, n);
    } else {
        memcpy_r(dest, src, n);
    }

    return dest;
}
