/*
 * Copyright (c) 2017-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <common/bl_common.h>
#include <cortex_a55.h>
#include <cpu_macros.S>
#include <plat_macros.S>

/* Hardware handled coherency */
#if HW_ASSISTED_COHERENCY == 0
#error "Cortex-A55 must be compiled with HW_ASSISTED_COHERENCY enabled"
#endif

	/* --------------------------------------------------
	 * Errata Workaround for Cortex A55 Errata #768277.
	 * This applies only to revision r0p0 of Cortex A55.
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r3
	 * --------------------------------------------------
	 */
func errata_a55_768277_wa
	/*
	 * Compare r0 against revision r0p0
	 */
	mov		r2, lr
	bl		check_errata_768277
	mov		lr, r2
	cmp		r0, #ERRATA_NOT_APPLIES
	beq		1f
	ldcopr16	r0, r1, CORTEX_A55_CPUACTLR
	orr64_imm	r0, r1, CORTEX_A55_CPUACTLR_DISABLE_DUAL_ISSUE
	stcopr16	r0, r1, CORTEX_A55_CPUACTLR
	isb
1:
	bx		lr
endfunc errata_a55_768277_wa

func check_errata_768277
	mov	r1, #0x00
	b	cpu_rev_var_ls
endfunc check_errata_768277

	/* ------------------------------------------------------------------
	 * Errata Workaround for Cortex A55 Errata #778703.
	 * This applies only to revision r0p0 of Cortex A55 where L2 cache is
	 * not configured.
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r3
	 * ------------------------------------------------------------------
	 */
func errata_a55_778703_wa
	/*
	 * Compare r0 against revision r0p0 and check that no private L2 cache
	 * is configured
	 */
	mov		r2, lr
	bl		check_errata_778703
	mov		lr, r2
	cmp		r0, #ERRATA_NOT_APPLIES
	beq		1f
	ldcopr16	r0, r1, CORTEX_A55_CPUECTLR
	orr64_imm	r0, r1, CORTEX_A55_CPUECTLR_L1WSCTL
	stcopr16	r0, r1, CORTEX_A55_CPUECTLR
	ldcopr16	r0, r1, CORTEX_A55_CPUACTLR
	orr64_imm	r0, r1, CORTEX_A55_CPUACTLR_DISABLE_WRITE_STREAMING
	stcopr16	r0, r1, CORTEX_A55_CPUACTLR
	isb
1:
	bx		lr
endfunc errata_a55_778703_wa

func check_errata_778703
	mov	r3, lr
	mov	r1, #0x00
	bl	cpu_rev_var_ls
	/*
	 * Check that no private L2 cache is configured
	 */
	ldcopr	r1, CORTEX_A55_CLIDR
	and	r1, r1, #CORTEX_A55_CLIDR_CTYPE3
	cmp	r1, #0
	movne	r0, #ERRATA_NOT_APPLIES
	bx	r3
endfunc check_errata_778703

	/* --------------------------------------------------
	 * Errata Workaround for Cortex A55 Errata #798797.
	 * This applies only to revision r0p0 of Cortex A55.
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r3
	 * --------------------------------------------------
	 */
func errata_a55_798797_wa
	/*
	 * Compare r0 against revision r0p0
	 */
	mov		r2, lr
	bl		check_errata_798797
	mov		lr, r2
	cmp		r0, #ERRATA_NOT_APPLIES
	beq		1f
	ldcopr16	r0, r1, CORTEX_A55_CPUACTLR
	orr64_imm	r0, r1, CORTEX_A55_CPUACTLR_DISABLE_L1_PAGEWALKS
	stcopr16	r0, r1, CORTEX_A55_CPUACTLR
	isb
1:
	bx		lr
endfunc errata_a55_798797_wa

func check_errata_798797
	mov	r1, #0x00
	b	cpu_rev_var_ls
endfunc check_errata_798797

	/* --------------------------------------------------------------------
	 * Errata Workaround for Cortex A55 Errata #846532.
	 * This applies only to revisions <= r0p1 of Cortex A55.
	 * Disabling dual-issue has a small impact on performance. Disabling a
	 * power optimization feature is an alternate workaround with no impact
	 * on performance but with an increase in power consumption (see errata
	 * notice).
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r3
	 * --------------------------------------------------------------------
	 */
func errata_a55_846532_wa
	/*
	 * Compare r0 against revision r0p1
	 */
	mov		r2, lr
	bl		check_errata_846532
	mov		lr, r2
	cmp		r0, #ERRATA_NOT_APPLIES
	beq		1f
	ldcopr16	r0, r1, CORTEX_A55_CPUACTLR
	orr64_imm	r0, r1, CORTEX_A55_CPUACTLR_DISABLE_DUAL_ISSUE
	stcopr16	r0, r1, CORTEX_A55_CPUACTLR
	isb
1:
	bx		lr
endfunc errata_a55_846532_wa

func check_errata_846532
	mov	r1, #0x01
	b	cpu_rev_var_ls
endfunc check_errata_846532

	/* -----------------------------------------------------
	 * Errata Workaround for Cortex A55 Errata #903758.
	 * This applies only to revisions <= r0p1 of Cortex A55.
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r3
	 * -----------------------------------------------------
	 */
func errata_a55_903758_wa
	/*
	 * Compare r0 against revision r0p1
	 */
	mov		r2, lr
	bl		check_errata_903758
	mov		lr, r2
	cmp		r0, #ERRATA_NOT_APPLIES
	beq		1f
	ldcopr16	r0, r1, CORTEX_A55_CPUACTLR
	orr64_imm	r0, r1, CORTEX_A55_CPUACTLR_DISABLE_L1_PAGEWALKS
	stcopr16	r0, r1, CORTEX_A55_CPUACTLR
	isb
1:
	bx		lr
endfunc errata_a55_903758_wa

func check_errata_903758
	mov	r1, #0x01
	b	cpu_rev_var_ls
endfunc check_errata_903758

	/* -----------------------------------------------------
	 * Errata Workaround for Cortex A55 Errata #1221012.
	 * This applies only to revisions <= r1p0 of Cortex A55.
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r3
	 * -----------------------------------------------------
	 */
func errata_a55_1221012_wa
	/*
	 * Compare r0 against revision r1p0
	 */
	mov		r2, lr
	bl		check_errata_1221012
	mov		lr, r2
	cmp		r0, #ERRATA_NOT_APPLIES
	beq		1f
	mov 		r0, #0x0000
	movt		r0, #0x0850
	mov		r1, #0
	stcopr16	r0, r1, CPUPOR
	mov		r0, #0x0000
	mov		r0, #0x1FF0
	mov		r1, #0x2
	stcopr16	r0, r1, CPUPMR
	mov		r0, #0x03FD
	movt		r0, #0x0110
	mov		r1, #0x0
	stcopr16	r0, r1, CPUPCR
	mov 		r0, #0x1
	stcopr 		r0, CPUPSELR
	mov		r0, #0x0040
	movt		r0, #0x08D0
	mov		r1, #0
	stcopr16	r0, r1, CPUPOR
	mov 		r0, #0x0040
	movt 		r0, #0x1FF0
	mov 		r1, #0
	stcopr16	r0, r1, CPUPMR
	mov		r0, #0x03FD
	movt		r0, #0x0110
	mov		r1, #0x0
	stcopr16	r0, r1, CPUPCR
	isb
1:
	bx		lr
endfunc errata_a55_1221012_wa

func check_errata_1221012
	mov	r1, #0x10
	b	cpu_rev_var_ls
endfunc check_errata_1221012


func cortex_a55_reset_func
	mov	r5, lr

#if ERRATA_DSU_798953
	bl	errata_dsu_798953_wa
#endif

#if ERRATA_DSU_936184
	bl	errata_dsu_936184_wa
#endif

	bl	cpu_get_rev_var
	mov	r4, r0

#if ERRATA_A55_768277
	mov	r0, r4
	bl	errata_a55_768277_wa
#endif

#if ERRATA_A55_778703
	mov	r0, r4
	bl	errata_a55_778703_wa
#endif

#if ERRATA_A55_798797
	mov	r0, r4
	bl	errata_a55_798797_wa
#endif

#if ERRATA_A55_846532
	mov	r0, r4
	bl	errata_a55_846532_wa
#endif

#if ERRATA_A55_903758
	mov	r0, r4
	bl	errata_a55_903758_wa
#endif

#if ERRATA_A55_1221012
	mov	r0, r4
	bl	errata_a55_1221012_wa
#endif

	bx	r5
endfunc cortex_a55_reset_func

	/* ---------------------------------------------
	 * HW will do the cache maintenance while powering down
	 * ---------------------------------------------
	 */
func cortex_a55_core_pwr_dwn
	/* ---------------------------------------------
	 * Enable CPU power down bit in power control register
	 * ---------------------------------------------
	 */
	ldcopr	r0, CORTEX_A55_CPUPWRCTLR
	orr	r0, r0, #CORTEX_A55_CORE_PWRDN_EN_MASK
	stcopr	r0, CORTEX_A55_CPUPWRCTLR
	isb
	bx	lr
endfunc cortex_a55_core_pwr_dwn

#if REPORT_ERRATA
/*
 * Errata printing function for Cortex A55. Must follow AAPCS.
 */
func cortex_a55_errata_report
	push	{r12, lr}

	bl	cpu_get_rev_var
	mov	r4, r0

	/*
	 * Report all errata. The revision-variant information is passed to
	 * checking functions of each errata.
	 */
	report_errata ERRATA_DSU_798953, cortex_a55, dsu_798953
	report_errata ERRATA_DSU_936184, cortex_a55, dsu_936184
	report_errata ERRATA_A55_768277, cortex_a55, 768277
	report_errata ERRATA_A55_778703, cortex_a55, 778703
	report_errata ERRATA_A55_798797, cortex_a55, 798797
	report_errata ERRATA_A55_846532, cortex_a55, 846532
	report_errata ERRATA_A55_903758, cortex_a55, 903758
	report_errata ERRATA_A55_1221012, cortex_a55, 1221012

	pop	{r12, lr}
	bx	lr
endfunc cortex_a55_errata_report
#endif

declare_cpu_ops cortex_a55, CORTEX_A55_MIDR, \
	cortex_a55_reset_func, \
	cortex_a55_core_pwr_dwn
