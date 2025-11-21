/*
 * Copyright (c) 2021, Nordic Semiconductor ASA. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EXCEPTION_INFO_H__
#define __EXCEPTION_INFO_H__

#include <stdint.h>

#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define TRUSTZONE_PRESENT
#endif

#if defined(__ARM_ARCH_8_1M_MAIN__) || defined(__ARM_ARCH_8M_MAIN__) \
    || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#define FAULT_STATUS_PRESENT
#endif

/* Arguments to EXCEPTION_INFO() */
#define EXCEPTION_TYPE_SECUREFAULT 0
#define EXCEPTION_TYPE_HARDFAULT   1
#define EXCEPTION_TYPE_MEMFAULT    2
#define EXCEPTION_TYPE_BUSFAULT    3
#define EXCEPTION_TYPE_USAGEFAULT  4

/* This level of indirection is needed to fully resolve exception info when it's
 * a macro
 */
#define _STRINGIFY(exception_info) #exception_info

/* Store context for an exception, and print an error message with the context.
 *
 * @param[in]  exception_type  One of the EXCEPTION_TYPE_* values defined above. Any
 *                             other value will result in printing "Unknown".
 */
#ifdef TFM_EXCEPTION_INFO_DUMP
#define EXCEPTION_INFO(exception_type)                  \
    __ASM volatile(                                     \
        "PUSH   {R4-R11}\n"                             \
        "MOV     r0, lr\n"                              \
        "MRS     r1, MSP\n"                             \
        "MRS     r2, PSP\n"                             \
        "MOVS    r3, #" _STRINGIFY(exception_type) "\n" \
        "BL      store_and_dump_context\n"              \
    )

/* Store context for an exception, then print the info.
 * Call EXCEPTION_INFO() instead of calling this directly.
 */
void store_and_dump_context(uint32_t LR_in, uint32_t MSP_in, uint32_t PSP_in,
                            uint32_t exception_type);

void store_and_dump_copy_context(uint32_t *exc_frame_copy);
#else
#define EXCEPTION_INFO(exception_type)
#endif

#endif /* __EXCEPTION_INFO_H__ */
