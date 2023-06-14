/*
 * Copyright (c) 2013-2016, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Cortina Access. All rights reserved.
 * Copyright (c) 2017, Realtek. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <asm_macros.S>
#include <platform_def.h>

	.weak	plat_report_exception
	.weak	plat_crash_console_init
	.weak	plat_crash_console_putc
	.weak	plat_reset_handler
	.weak	plat_disable_acp
	.weak	bl1_plat_prepare_exit
	.weak	plat_error_handler
	.weak	plat_panic_handler
        .weak   plat_cache_reset

        .set    noreorder
	/* -----------------------------------------------------
	 * Placeholder function which should be redefined by
	 * each platform.
	 * -----------------------------------------------------
	 * k0 : epc , k1 : cause, a0: which exception vector we came from
	 */
func plat_report_exception
    /* store away the arguement which tells us where we came from */
    move  $s6, $a0

    la    $s2, plat_err_str
    jal   asm_print_str
    nop

    la    $s2, exception_epc_str
    jal   asm_print_str
    nop

    move  $s2, $k0                /*print EPC */
    jal   asm_print_hex_32
    nop

    la    $s2, exception_cause_str
    jal   asm_print_str
    nop

    move  $s2, $k1                /*print CAUSE */
    jal   asm_print_hex_32
    nop

    /* go to stall to halt the cpu, when we return from exception context */
    la  $t0, stall
    rfe
    jr $t0
    nop
stall:
    j stall
    nop

endfunc plat_report_exception

	/* -----------------------------------------------------
	 * Placeholder function which should be redefined by
	 * each platform.
	 * -----------------------------------------------------
	 */
func plat_crash_console_init
    li      $v0, 0
    jr	    $ra
    nop
endfunc plat_crash_console_init

	/* -----------------------------------------------------
	 * Placeholder function which should be redefined by
	 * each platform.
	 * -----------------------------------------------------
	 */
func plat_crash_console_putc
    jr	    $ra
    nop
endfunc plat_crash_console_putc

	/* -----------------------------------------------------
	 * Placeholder function which should be redefined by
	 * each platform.
	 * -----------------------------------------------------
	 */
func plat_reset_handler
    jr	    $ra
    nop
endfunc plat_reset_handler

	/* -----------------------------------------------------
	 * Placeholder function which should be redefined by
	 * each platform.
	 * -----------------------------------------------------
	 */
func plat_disable_acp
    jr	    $ra
    nop
endfunc plat_disable_acp

	/* -----------------------------------------------------
	 * void bl1_plat_prepare_exit(entry_point_info_t *ep_info);
	 * Called before exiting BL1. Default: do nothing
	 * -----------------------------------------------------
	 */
func bl1_plat_prepare_exit
    jr	    $ra
    nop
endfunc bl1_plat_prepare_exit

	/* -----------------------------------------------------
	 * void plat_error_handler(int err) __dead2;
	 * Endless loop by default.
	 * -----------------------------------------------------
	 */
func plat_error_handler
    jal  plat_error_handler
    nop
endfunc plat_error_handler

	/* -----------------------------------------------------
	 * void plat_panic_handler(void) __dead2;
	 * Endless loop by default.
	 * -----------------------------------------------------
	 */
func plat_panic_handler
	jal plat_panic_handler
    nop
endfunc plat_panic_handler

	/* -----------------------------------------------------
	 * Placeholder function which should be redefined by
	 * each platform.
	 * -----------------------------------------------------
	 */
func plat_cache_reset
    jr		$ra
    nop
endfunc plat_cache_reset

.section .rodata.rev_err_str, "aS"
    plat_err_str:
        .asciz "\nPlatform exception reporting: "
    exception_epc_str:
        .asciz "\nEPC: "
    exception_cause_str:
        .asciz "\nCAUSE: "

