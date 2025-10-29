/**
 ****************************************************************************************
 *
 * @file boot_handlers.s
 *
 * @brief ARM Exception Vector handler functions.
 *
 * Copyright (C) RivieraWaves 2011-2016
 *
 ****************************************************************************************
 */	
	.globl entry_main
	.globl boot_reset
	.globl boot_swi
	.globl boot_undefined
	.globl boot_pabort
	.globl boot_dabort
	.globl boot_reserved
	.globl irq_handler
	.globl fiq_handler
	.globl do_irq
	.globl do_fiq
	.globl do_swi
	.globl do_undefined
	.globl do_pabort
	.globl do_dabort
	.globl do_reserved
	.globl bk_trap_udef
	.globl bk_trap_pabt
	.globl bk_trap_dabt
	.globl bk_trap_resv
	.globl __svc_stack_top
	.globl __irq_stack_top
	.globl __fiq_stack_top
	.extern _arm_exception_irq_handler
	.extern _arm_exception_fiq_handler
	.extern _arm_exception_swi_handler
    
#include <common/sys_config.h>

/* ========================================================================
 *                                Macros
 * ======================================================================== */
#define ARM_MODE_MASK                     0x1F
#define ARM_MODE_USR                      0x10
#define ARM_MODE_FIQ                      0x11
#define ARM_MODE_IRQ                      0x12
#define ARM_MODE_SVC                      0x13
#define ARM_MODE_ABT                      0x17
#define ARM_MODE_UND                      0x1B
#define ARM_MODE_SYS                      0x1F
#define BOOT_FIQ_IRQ_MASK                 0xC0
#define BOOT_IRQ_MASK                     0x80

		.align	5
boot_swi:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400008
	LDR    r0, [R1]
	BX	   r0

		.align	5
irq_handler:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400000
	LDR    r0, [R1]
	BX	   r0

		.align	5
fiq_handler:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400004
	LDR    r0, [R1]
	BX	   r0

	.align	5 
do_swi:
	LDMFD	SP!, {R0-R1}
    B HalExceptSwiHdl

	.align	5 
do_irq:
	LDMFD	SP!, {R0-R1}
	B HalExceptIrqHdl

	.align	5    
do_fiq:
	LDMFD	SP!, {R0-R1}
	B HalExceptFiqHdl
	
/*EOF*/

