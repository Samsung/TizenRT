/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_HAL_MEMORY_SYMBOLS_H__
#define __TFM_HAL_MEMORY_SYMBOLS_H__

#include <stdint.h>

#ifdef CONFIG_TFM_PARTITION_META
extern uintptr_t hal_mem_sp_meta_start;
extern uintptr_t hal_mem_sp_meta_end;
#endif

#endif /*__TFM_HAL_MEMORY_SYMBOLS_H__ */
