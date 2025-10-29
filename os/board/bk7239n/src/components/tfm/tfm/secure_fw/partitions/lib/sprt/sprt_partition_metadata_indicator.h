/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPRT_PARTITION_METADATA_INDICATOR_H__
#define __SPRT_PARTITION_METADATA_INDICATOR_H__

#include <stdint.h>

extern uintptr_t p_partition_metadata;

/* Partition Metadata pointer */
#define PART_METADATA() ((struct runtime_metadata_t *)p_partition_metadata)

#endif /* __SPRT_PARTITION_METADATA_INDICATOR_H__ */
