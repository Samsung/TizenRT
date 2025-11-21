/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CRT_IMPL_PRIVATE_H__
#define __CRT_IMPL_PRIVATE_H__

#include <stdint.h>
#include <stddef.h>

#define GET_MEM_ADDR_BIT0(x)        ((x) & 0x1)
#define GET_MEM_ADDR_BIT1(x)        ((x) & 0x2)

union tfm_mem_addr_t {
    uintptr_t uint_addr;        /* Address          */
    uint8_t *p_byte;            /* Byte copy        */
    uint16_t *p_dbyte;          /* Double byte copy */
    uint32_t *p_qbyte;          /* Quad byte copy   */
};

#endif /* __CRT_IMPL_PRIVATE_H__ */
