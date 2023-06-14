/*
 * Copyright (c) 2017-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CORTEX_A75_H
#define CORTEX_A75_H

#include <lib/utils_def.h>

/* Cortex-A75 MIDR */
#define CORTEX_A75_MIDR			U(0x410fd0a0)

/*******************************************************************************
 * CPU Extended Control register specific definitions.
 ******************************************************************************/
#define CORTEX_A75_CPUPWRCTLR		p15, 0, c15, c2, 7
#define CORTEX_A75_CPUECTLR		p15, 4, c15

/*******************************************************************************
 * CPU Auxiliary Control register specific definitions.
 ******************************************************************************/
#define CORTEX_A75_CPUACTLR		p15, 0, c15

#define CORTEX_A75_CPUACTLR_DISABLE_LOAD_PASS_STORE	(ULL(1) << 35)

/* Definitions of register field mask in CORTEX_A75_CPUPWRCTLR */
#define CORTEX_A75_CORE_PWRDN_EN_MASK	U(0x1)

#endif /* CORTEX_A75_H */
