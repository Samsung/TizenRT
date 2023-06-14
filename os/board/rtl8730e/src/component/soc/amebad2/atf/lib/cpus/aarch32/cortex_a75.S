/*
 * Copyright (c) 2017-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <cortex_a75.h>
#include <cpu_macros.S>

/* Hardware handled coherency */
#if HW_ASSISTED_COHERENCY == 0
#error "Cortex-A75 must be compiled with HW_ASSISTED_COHERENCY enabled"
#endif

	/* --------------------------------------------------
	 * Errata Workaround for Cortex A75 Errata #764081.
	 * This applies only to revision r0p0 of Cortex A75.
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r2
	 * --------------------------------------------------
	 */
func errata_a75_764081_wa
	/*
	 * Compare r0 against revision r0p0
	 */
	mov	r2, lr
	bl	check_errata_764081
	mov	lr, r2
	cmp	r0, #ERRATA_NOT_APPLIES
	beq	1f
	ldcopr	r1, SCTLR
	orr	r1, r1 ,#SCTLR_IESB_BIT
	stcopr	r1, SCTLR
	isb
1:
	bx	lr
endfunc errata_a75_764081_wa

func check_errata_764081
	mov	r1, #0x00
	b	cpu_rev_var_ls
endfunc check_errata_764081

	/* --------------------------------------------------
	 * Errata Workaround for Cortex A75 Errata #790748.
	 * This applies only to revision r0p0 of Cortex A75.
	 * Inputs:
	 * r0: variant[4:7] and revision[0:3] of current cpu.
	 * Shall clobber: r0-r2
	 * --------------------------------------------------
	 */
func errata_a75_790748_wa
	/*
	 * Compare r0 against revision r0p0
	 */
	mov		r2, lr
	bl		check_errata_790748
	mov		lr, r2
	cmp		r0, #ERRATA_NOT_APPLIES
	beq		1f
	ldcopr16	r0, r1, CORTEX_A75_CPUACTLR
	orr		r0, r0 ,#(1 << 13)
	stcopr16	r0, r1, CORTEX_A75_CPUACTLR
	isb
1:
	bx		lr
endfunc errata_a75_790748_wa

func check_errata_790748
	mov	r1, #0x00
	b	cpu_rev_var_ls
endfunc check_errata_790748

	/* -------------------------------------------------
	 * The CPU Ops reset function for Cortex-A75.
	 * -------------------------------------------------
	 */
func cortex_a75_reset_func
	mov	r5, lr
	bl	cpu_get_rev_var
	mov	r4, r0

#if ERRATA_A75_764081
	mov	r0, r4
	bl	errata_a75_764081_wa
#endif

#if ERRATA_A75_790748
	mov	r0, r4
	bl	errata_a75_790748_wa
#endif

#if IMAGE_BL32 && WORKAROUND_CVE_2017_5715
	ldr	r0, =wa_cve_2017_5715_bpiall_vbar
	stcopr	r0, VBAR
	isb
#endif

#if WORKAROUND_CVE_2018_3639
	ldcopr16	r0, r1, CORTEX_A75_CPUACTLR
	orr64_imm	r0, r0, CORTEX_A75_CPUACTLR_DISABLE_LOAD_PASS_STORE
	stcopr16	r0, r1, CORTEX_A75_CPUACTLR
	isb
#endif

#if ERRATA_DSU_798953
	bl	errata_dsu_798953_wa
#endif

#if ERRATA_DSU_936184
	bl	errata_dsu_936184_wa
#endif

	bx	r5
endfunc cortex_a75_reset_func

func check_errata_cve_2017_5715
#if WORKAROUND_CVE_2017_5715
	mov	r0, #ERRATA_APPLIES
#else
	mov	r0, #ERRATA_MISSING
#endif
	bx	lr
endfunc check_errata_cve_2017_5715

func check_errata_cve_2018_3639
#if WORKAROUND_CVE_2018_3639
	mov	r0, #ERRATA_APPLIES
#else
	mov	r0, #ERRATA_MISSING
#endif
	bx	lr
endfunc check_errata_cve_2018_3639
	/* ---------------------------------------------
	 * HW will do the cache maintenance while powering down
	 * ---------------------------------------------
	 */
func cortex_a75_core_pwr_dwn
	/* ---------------------------------------------
	 * Enable CPU power down bit in power control register
	 * ---------------------------------------------
	 */
	ldcopr	r0, CORTEX_A75_CPUPWRCTLR
	orr	r0, r0, #CORTEX_A75_CORE_PWRDN_EN_MASK
	stcopr	r0, CORTEX_A75_CPUPWRCTLR
	isb
	bx	lr
endfunc cortex_a75_core_pwr_dwn

#if REPORT_ERRATA
/*
 * Errata printing function for Cortex A75. Must follow AAPCS.
 */
func cortex_a75_errata_report
	push	{r12, lr}

	bl	cpu_get_rev_var
	mov	r4, r0

	/*
	 * Report all errata. The revision-variant information is passed to
	 * checking functions of each errata.
	 */
	report_errata ERRATA_A75_764081, cortex_a75, 764081
	report_errata ERRATA_A75_790748, cortex_a75, 790748
	report_errata WORKAROUND_CVE_2017_5715, cortex_a75, cve_2017_5715
	report_errata WORKAROUND_CVE_2018_3639, cortex_a75, cve_2018_3639
	report_errata ERRATA_DSU_798953, cortex_a75, dsu_798953
	report_errata ERRATA_DSU_936184, cortex_a75, dsu_936184

	pop	{r12, lr}
	bx	lr
endfunc cortex_a75_errata_report
#endif

declare_cpu_ops cortex_a75, CORTEX_A75_MIDR, \
	cortex_a75_reset_func, \
	cortex_a75_core_pwr_dwn
