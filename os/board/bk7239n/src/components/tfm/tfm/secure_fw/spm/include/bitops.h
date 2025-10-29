/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __BITOPS_H__
#define __BITOPS_H__

#include <stdint.h>

/* Check if there is only one bit availiable in a 32bit number */
#define IS_ONLY_ONE_BIT_IN_UINT32(n)                          \
                  ((uint32_t)(n) && !((uint32_t)(n) & ((uint32_t)(n)-1)))
#endif /* __BITOPS_H__ */
