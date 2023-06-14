/*
 * Copyright (c) 2017-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CORTEX_A55_H
#define CORTEX_A55_H

#include <lib/utils_def.h>

/* Cortex-A55 MIDR for revision 0 */
#define CORTEX_A55_MIDR			U(0x410fd050)

/*******************************************************************************
 * CPU Extended Control register specific definitions.
 ******************************************************************************/
#define CORTEX_A55_CPUPWRCTLR		p15, 0, c15, c2, 7
#define CORTEX_A55_CPUECTLR		p15, 4, c15

#define CORTEX_A55_CPUECTLR_L1WSCTL	(ULL(3) << 25)

/*******************************************************************************
 * CPU Auxiliary Control register specific definitions.
 ******************************************************************************/
#define CORTEX_A55_CPUACTLR				p15, 0, c15

#define CORTEX_A55_CPUACTLR_DISABLE_WRITE_STREAMING	(ULL(1) << 24)
#define CORTEX_A55_CPUACTLR_DISABLE_DUAL_ISSUE		(ULL(1) << 31)
#define CORTEX_A55_CPUACTLR_DISABLE_L1_PAGEWALKS	(ULL(1) << 49)

/*******************************************************************************
 * CPU Identification register specific definitions.
 ******************************************************************************/
#define CORTEX_A55_CLIDR				p15, 1, c0, c0, 1

#define CORTEX_A55_CLIDR_CTYPE3				(U(7) << 6)

/* Definitions of register field mask in CORTEX_A55_CPUPWRCTLR */
#define CORTEX_A55_CORE_PWRDN_EN_MASK	U(0x1)

/* Instruction patching registers */
#define CPUPSELR	p15, 6, c15, c8, 0
#define CPUPCR		p15, 8, c15
#define CPUPOR		p15, 9, c15
#define CPUPMR		p15, 10, c15

#endif /* CORTEX_A55_H */
