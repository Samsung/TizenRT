/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

/*
 * The address of Partition metadata, stored in TFM_SP_META_PTR region.
 * All privilege RW. Gets updated by Scheduler when scheduling.
 */
__attribute__((section(".bss.SP_META_PTR_SPRTL_INST")))
uintptr_t p_partition_metadata;
