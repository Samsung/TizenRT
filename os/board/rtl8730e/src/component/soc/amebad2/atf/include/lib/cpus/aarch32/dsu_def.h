/*
 * Copyright (c) 2018-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSU_DEF_H
#define DSU_DEF_H

#include <lib/utils_def.h>

/********************************************************************
 * DSU Cluster Configuration registers definitions
 ********************************************************************/
#define CLUSTERCFR		p15, 0, c15, c3, 0

#define CLUSTERCFR_ACP_SHIFT	U(11)

/********************************************************************
 * DSU Cluster Main Revision ID registers definitions
 ********************************************************************/
#define CLUSTERIDR		p15, 0, c15, c3, 1

#define CLUSTERIDR_REV_SHIFT	U(0)
#define CLUSTERIDR_REV_BITS	U(4)
#define CLUSTERIDR_VAR_SHIFT	U(4)
#define CLUSTERIDR_VAR_BITS	U(4)

/********************************************************************
 * DSU Cluster Auxiliary Control registers definitions
 ********************************************************************/
#define CLUSTERACTLR		p15, 0, c15, c3, 3

#define CLUSTERACTLR_DISABLE_CLOCK_GATING	(ULL(1) << 15)

/********************************************************************
 * Masks applied for DSU errata workarounds
 ********************************************************************/
#define DSU_ERRATA_936184_MASK	(U(0x3) << 15)

#endif /* DSU_DEF_H */
