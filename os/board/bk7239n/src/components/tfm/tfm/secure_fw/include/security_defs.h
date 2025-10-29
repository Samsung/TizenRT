/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SECURITY_DEFS_H__
#define __SECURITY_DEFS_H__

/* Generic security-related definitions */

/*
 * Stack SEAL is involved since Security Extension exists, it becomes
 * a generic security defs used by both SPM and Partitions.
 */
#define STACK_SEAL_PATTERN    0xFEF5EDA5

#endif /* __SECURITY_DEFS_H__ */
