/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

#ifndef IOVEC_LEN
#define IOVEC_LEN(x) (uint32_t)ARRAY_SIZE(x)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __ARRAY_H__ */
