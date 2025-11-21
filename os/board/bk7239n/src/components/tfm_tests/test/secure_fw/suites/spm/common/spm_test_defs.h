/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPM_COMMON_TEST_DEFS_H__
#define __TFM_SPM_COMMON_TEST_DEFS_H__

#include "psa/framework_feature.h"

/* MMIOVEC test service types [0x100, 0x1FF] */
#if PSA_FRAMEWORK_HAS_MM_IOVEC
#define INVEC_MAP_AND_UNMAP              (0x100)
#define OUTVEC_MAP_AND_UNMAP             (0x101)
#define OUTVEC_MAP_NOT_UNMAP             (0x102)

#define TFM_MMIOVEC_TEST_ERROR           (-257)

#define MMIOVEC_INPUT_DATA               (0xFFFFABCD)
#define MMIOVEC_OUTPUT_DATA              (0xA5)
#define MMIOVEC_TEST_VEC_LEN             (4)

#define MMIOVECT_TEST_INVEC    {MMIOVEC_INPUT_DATA, MMIOVEC_INPUT_DATA + 1, \
                                MMIOVEC_INPUT_DATA + 2, MMIOVEC_INPUT_DATA + 3}
#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */

/* IRQ test service types [0x200, 0x2FF] */
#ifdef TEST_NS_FLIH_IRQ
#define TFM_FLIH_TEST_CASE_INVALID          (0x200)
#define TFM_FLIH_TEST_CASE_1                (0x201)
#define TFM_FLIH_TEST_CASE_2                (0x202)
#endif /* TEST_NS_FLIH_IRQ */

#ifdef TEST_NS_SLIH_IRQ
#define TFM_SLIH_TEST_CASE_1                (0x203)
#endif /* TEST_NS_SLIH_IRQ */

#endif /* __TFM_SPM_COMMON_TEST_DEFS_H__ */
