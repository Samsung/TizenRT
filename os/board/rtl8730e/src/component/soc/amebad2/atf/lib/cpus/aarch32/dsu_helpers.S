/*
 * Copyright (c) 2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <asm_macros.S>
#include <dsu_def.h>
#include <lib/cpus/errata_report.h>

	/* -----------------------------------------------------------------------
	 * DSU erratum 798953 check function
	 * Checks the DSU variant, revision and configuration to determine if
	 * the erratum applies. Erratum applies on all configurations of the
	 * DSU and if revision-variant is r0p0.
	 *
	 * The erratum was fixed in r0p1.
	 *
	 * This function is called from both assembly and C environment. So it
	 * follows AAPCS.
	 *
	 * Clobbers: r0-r1
	 * -----------------------------------------------------------------------
	 */
	.globl	check_errata_dsu_798953
	.globl	errata_dsu_798953_wa

func check_errata_dsu_798953
	/* Check if DSU is equal to r0p0 */
	ldcopr	r1, CLUSTERIDR

	/* DSU variant and revision bitfields in CLUSTERIDR are adjacent */
	ubfx	r0, r1, #CLUSTERIDR_REV_SHIFT,\
			#(CLUSTERIDR_REV_BITS + CLUSTERIDR_VAR_BITS)
	mov	r1, #(0x0 << CLUSTERIDR_REV_SHIFT)
	cmp	r0, r1
	movls	r0, #ERRATA_APPLIES
	movhi	r0, #ERRATA_NOT_APPLIES
	bx	lr
endfunc check_errata_dsu_798953

	/* --------------------------------------------------
	 * Errata Workaround for DSU erratum #798953.
	 *
	 * Can clobber only: x0-x17
	 * --------------------------------------------------
	 */
func errata_dsu_798953_wa
	mov	r4, lr
	bl	check_errata_dsu_798953
	cmp	r0, #ERRATA_NOT_APPLIES
	beq	1f

	/* If erratum applies, disable high-level clock gating */
	ldcopr	r0, CLUSTERACTLR
	orr	r0, #CLUSTERACTLR_DISABLE_CLOCK_GATING
	stcopr	r0, CLUSTERACTLR
	isb
1:
	bx	r4
endfunc errata_dsu_798953_wa

	/* -----------------------------------------------------------------------
	 * DSU erratum 936184 check function
	 * Checks the DSU variant, revision and configuration to determine if
	 * the erratum applies. Erratum applies if ACP interface is present
	 * in the DSU and revision-variant <= r1p0.
	 *
	 * The erratum was fixed in r2p0.
	 *
	 * This function is called from both assembly and C environment. So it
	 * follows AAPCS.
	 *
	 * Clobbers: r0-r1
	 * -----------------------------------------------------------------------
	 */
	.globl	check_errata_dsu_936184
	.globl	errata_dsu_936184_wa

func check_errata_dsu_936184
	/* Erratum applies only if DSU has the ACP interface */
	mov	r0, #ERRATA_NOT_APPLIES
	ldcopr	r1, CLUSTERCFR
	ubfx	r1, r1, #CLUSTERCFR_ACP_SHIFT, #1
	cmp	r1, #0
	beq	1f

	/* If ACP is present, check if DSU is older than r2p0 */
	ldcopr	r1, CLUSTERIDR

	/* DSU variant and revision bitfields in CLUSTERIDR are adjacent */
	ubfx	r0, r1, #CLUSTERIDR_REV_SHIFT,\
			#(CLUSTERIDR_REV_BITS + CLUSTERIDR_VAR_BITS)
	mov	r1, #(0x1 << CLUSTERIDR_VAR_SHIFT)
	cmp	r0, r1
	movls	r0, #ERRATA_APPLIES
	movhi	r0, #ERRATA_NOT_APPLIES
1:
	bx	lr
endfunc check_errata_dsu_936184

	/* --------------------------------------------------
	 * Errata Workaround for DSU erratum #936184.
	 *
	 * Can clobber only: x0-x17
	 * --------------------------------------------------
	 */
func errata_dsu_936184_wa
	mov	r4, lr
	bl	check_errata_dsu_936184
	cmp	r0, #ERRATA_NOT_APPLIES
	beq	1f

	/* If erratum applies, we set a mask to a DSU control register */
	ldcopr	r0, CLUSTERACTLR
	ldr	r1, =DSU_ERRATA_936184_MASK
	orr	r0, r0, r1
	stcopr	r0, CLUSTERACTLR
	isb
1:
	bx	r4
endfunc errata_dsu_936184_wa
