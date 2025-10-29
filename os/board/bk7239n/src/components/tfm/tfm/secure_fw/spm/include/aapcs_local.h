/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_AAPCS_LOCAL_H__
#define __TFM_AAPCS_LOCAL_H__

#include <stdint.h>

/*
 * AAPCS: Procedure Call Standard for the Arm Architecture(IHI0042).
 *
 * This file complies with AAPCS rules for the local project(TF-M)
 * convenience. Append a 'local' postfix here in case the toolchain
 * makes 'AAPCS' as one built-in word.
 */

/*
 * Returning values in multiple argument registers:
 *
 * If a caller in assembly code requires multiple return values from a
 * callee, returning in argument registers is more convenient. This needs
 * the callee to assemble multiple return values into one fundamental data
 * type whose size exceeds the default data width, such as returning a 64-bit
 * integer on a 32-bit system separates the 64-bit into two 32-bit values
 * and gets returned in argument register 0 and 1.
 *
 * The following is the implementation for a generic 32-bit system. To
 * be expanded when new systems get involved.
 *
 * Method (MACRO) uses 'a(x)', which stands for AAPCS 'argument register'.
 * Implementation uses 'r(x)', which is the implementation name for
 * 'a(x)' in the 32-bit system.
 */
union u64_in_u32_regs_t {
    struct {
        uint32_t    r0;
        uint32_t    r1;
    } u32_regs;

    uint64_t        u64_val;
};

#define AAPCS_DUAL_U32_T  union u64_in_u32_regs_t

#define AAPCS_DUAL_U32_SET(v, a0, a1)                           \
                        do {                                    \
                            (v).u32_regs.r0 = (uint32_t)(a0);   \
                            (v).u32_regs.r1 = (uint32_t)(a1);   \
                        } while (0)

#define AAPCS_DUAL_U32_SET_A0(v, a0)                            \
                            (v).u32_regs.r0 = (uint32_t)(a0)

#define AAPCS_DUAL_U32_SET_A1(v, a1)                            \
                            (v).u32_regs.r1 = (uint32_t)(a1)

#define AAPCS_DUAL_U32_AS_U64(v) (v).u64_val

#endif /* __TFM_AAPCS_LOCAL_H__ */
