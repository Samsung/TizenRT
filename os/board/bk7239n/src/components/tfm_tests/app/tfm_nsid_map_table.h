/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_NSID_MAP_TABLE_H__
#define __TFM_NSID_MAP_TABLE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Get NSID with the name of thread */
int32_t nsid_mgr_get_thread_nsid(const char* t_name);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NSID_MAP_TABLE_H__ */
