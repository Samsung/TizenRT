/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#define UART_BASE_ADDRESS		0x4200C000

#ifndef FIQ_STACK_SIZE
#define FIQ_STACK_SIZE			1024
#endif
#ifndef IRQ_STACK_SIZE
#define IRQ_STACK_SIZE			1024
#endif
#ifndef SUPERVISOR_STACK_SIZE
#define SUPERVISOR_STACK_SIZE	2048
#endif
#ifndef ABORT_STACK_SIZE
#define ABORT_STACK_SIZE		1024
#endif
#ifndef UNDEF_STACK_SIZE
#define UNDEF_STACK_SIZE		1024
#endif
#ifndef SYS_STACK_SIZE
#define SYS_STACK_SIZE			1024
#endif

#define FIQ_STACK_SIZE_TOTAL	(FIQ_STACK_SIZE * CONFIG_CPUS_NUM)
#define IRQ_STACK_SIZE_TOTAL	(IRQ_STACK_SIZE * CONFIG_CPUS_NUM)
#define SUPERVISOR_STACK_SIZE_TOTAL	(SUPERVISOR_STACK_SIZE * CONFIG_CPUS_NUM)
#define ABORT_STACK_SIZE_TOTAL	(ABORT_STACK_SIZE * CONFIG_CPUS_NUM)
#define UNDEF_STACK_SIZE_TOTAL	(UNDEF_STACK_SIZE * CONFIG_CPUS_NUM)
#define SYS_STACK_SIZE_TOTAL	(SYS_STACK_SIZE * CONFIG_CPUS_NUM)

#ifndef HEAP_SIZE
#define HEAP_SIZE				0x1000
#endif

	.section .stack, "aw"
	.align 3
__fiq_stack:
	.space FIQ_STACK_SIZE_TOTAL
	.size __fiq_stack, . - __fiq_stack

__irq_stack:
	.space IRQ_STACK_SIZE_TOTAL
	.size __irq_stack, . - __irq_stack

__supervisor_stack:
	.space SUPERVISOR_STACK_SIZE_TOTAL
	.size __supervisor_stack, . - __supervisor_stack

__abort_stack:
	.space ABORT_STACK_SIZE_TOTAL
	.size __abort_stack, . - __abort_stack

__undef_stack:
	.space UNDEF_STACK_SIZE_TOTAL
	.size __undef_stack, . - __undef_stack

__sys_stack:
	.space SYS_STACK_SIZE_TOTAL
	.size __sys_stack, . - __sys_stack

	.section .heap, "aw"
	.align 3
__heap:
	.space HEAP_SIZE
	.size __heap, . - __heap

	.section .boot,"ax"
	.global	_boot
	.extern _vector_table
	.extern vPortSecondaryStart
	.extern app_start
/* this initializes the various processor modes */
_prestart:
_boot:
	/* set VBAR to the _vector_table address in linker script */
	ldr	r0, =_vector_table
	mcr	p15, 0, r0, c12, c0, 0

	/* Invalidate caches and TLBs */
	mov	r0, #0						@ r0 = 0
	mcr	p15, 0, r0, c8, c7, 0		@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0		@ invalidate icache
	mcr	p15, 0, r0, c7, c5, 6		@ invalidate branch predictor array
	dsb	sy
	isb	sy
	bl	invalidate_dcache

	/* Disable MMU, if enabled */
	/* clear I/C to disable I/D Cache */
	/* clear M to disable MMU, clear Z to disable branch prediction */
	mrc	p15, 0, r0, c1, c0, 0		@ read CP15 register 1
	bic	r0, r0, #0x00002000			@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007			@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002			@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800			@ set bit 11 (Z---) BTB
	bic	r0, r0, #0x00001000			@ clear bit 12 (I) I-cache
	mcr	p15, 0, r0, c1, c0, 0		@ write value back

	/* Read mpidr */
	mrc	p15, 0, r0, c0, c0, 5
	and	r3, r0, #0xff
	add	r3, r3, #0x1

	/* set stack */
	/* cpsr[4:0] = mode */
	/* cpsr[5] = 0 , ARM state */
	/* cpsr[6] = 1 , Disable FIQ */
	/* cpsr[7] = 1 , Disable IRQ */
	mrs	r0, cpsr					@ get the current PSR
	mvn	r1, #0x1f					@ set up the FIQ stack pointer
	and	r2, r1, r0
	orr	r2, r2, #0x11				@ FIQ mode
	msr	cpsr, r2
	mov	r0, #FIQ_STACK_SIZE
	mul	r1, r0, r3
	ldr	r2, =__fiq_stack
	add	r13, r1, r2					@ FIQ stack pointer

	mrs	r0, cpsr					@ get the current PSR
	mvn	r1, #0x1f					@ set up the irq stack pointer
	and	r2, r1, r0
	orr	r2, r2, #0x12				@ IRQ mode
	msr	cpsr, r2
	mov	r0, #IRQ_STACK_SIZE
	mul	r1, r0, r3
	ldr	r2, =__irq_stack
	add	r13, r1, r2					@ IRQ stack pointer

	mrs	r0, cpsr					@ get the current PSR
	mvn	r1, #0x1f					@ set up the Abort stack pointer
	and	r2, r1, r0
	orr	r2, r2, #0x17				@ Abort mode
	msr	cpsr, r2
	mov	r0, #ABORT_STACK_SIZE
	mul	r1, r0, r3
	ldr	r2, =__abort_stack
	add	r13, r1, r2					@ Abort stack pointer

	mrs	r0, cpsr					@ get the current PSR
	mvn	r1, #0x1f					@ set up the Undefine stack pointer
	and	r2, r1, r0
	orr	r2, r2, #0x1b				@ Undefine mode
	msr	cpsr, r2
	mov	r0, #UNDEF_STACK_SIZE
	mul	r1, r0, r3
	ldr	r2, =__undef_stack
	add	r13, r1, r2					@ Undefine stack pointer

	mrs	r0, cpsr					@ get the current PSR
	mvn	r1, #0x1f					@ set up the system stack pointer
	and	r2, r1, r0
	orr	r2, r2, #0x1f				@ SYS mode
	msr	cpsr, r2
	mov	r0, #SYS_STACK_SIZE
	mul	r1, r0, r3
	ldr	r2, =__sys_stack
	add	r13, r1, r2					@ SYS stack pointer

	mrs	r0, cpsr					@ get the current PSR
	mvn	r1, #0x1f					@ set up the supervisor stack pointer
	and	r2, r1, r0
	orr	r2, r2, #0x13				@ Supervisor mode
	msr	cpsr, r2
	mov	r0, #SUPERVISOR_STACK_SIZE
	mul	r1, r0, r3
	ldr	r2, =__supervisor_stack
	add	r13, r1, r2					@ Supervisor stack pointer

	/* write to ACTLR */
	mrc	p15, 0, r0, c1, c0, 1		@ Read ACTLR
	orr	r0, r0, #(0x01 << 6)		@ set SMP bit
	orr	r0, r0, #(0x01 << 0)		@ Cache/TLB maintenance broadcast
	mcr	p15, 0, r0, c1, c0, 1		@ Write ACTLR

	/* Read mpidr */
	mrc	p15, 0, r0, c0, c0, 5
	and	r0, r0, #0xff
	add	r0, r0, #0x1

	/* enable MMU and cache */
	bl    setupMMUTable

	ldr	r0, =0x55555555				@ set all domains to client
	mcr	p15, 0, r0, c3, c0, 0

	/* enable mmu, icahce and dcache */
	mrc	p15, 0, r0, c1, c0, 0		@ read SCTLR
	bic	r0, r0, #(0x1 << 13)
	bic	r0, r0, #(0x1 << 1)			@ disable alignment check
	orr	r0, r0, #(0x1 << 12)		@ eanble I-cache
	orr	r0, r0, #(0x1 << 2)			@ enable D-cache
	orr	r0, r0, #(0x1 << 0)			@ enable MMU
	dsb								@ dsb allow the MMU to start up
	mcr	p15, 0, r0, c1, c0, 0		@ enable cache and MMU
	isb								@ isb flush prefetch buffer

	/* initialize L2 cache */

	/* if floating point and simd, Set CPACR */
	mrc	p15, 0, r1, c1, c0, 2		/* read cp access control register (CPACR) into r1 */
	orr	r1, r1, #(0xf << 20)		/* enable full access for cp10 & cp11 */
	mcr	p15, 0, r1, c1, c0, 2		/* write back into CPACR */

	/* enable vfp */
	fmrx	r1, FPEXC				@ read the exception register
	orr		r1,r1, #(0x1 << 30)		@ set VFP enable bit, leave the others in orig state
	fmxr	FPEXC, r1				@ write back the exception register

#if (CONFIG_CPUS_NUM > 1)
	/* only Core 0 calls main() */
	/* Read mpidr */
	mrc	p15, 0, r0, c0, c0, 5
	and	r3, r0, #0xff
	add	r3, r3, #0x1
	cmp	r3, #1
	bgt	vPortSecondaryStart
#endif

	bl	app_start
	bl	exit

	/* should never get here */
	.global exit
exit:
	b 	_exit

/*
 * invalidate_dcache - invalidate the entire d-cache by set/way
 */
invalidate_dcache:
	mrc	p15, 1, r0, c0, c0, 1		@ read CLIDR
	ands	r3, r0, #0x7000000
	mov	r3, r3, lsr #24				@ cache level value (naturally aligned)
	beq	finished
	mov	r10, #0						@ start with level 0
loop1:
	add	r2, r10, r10, lsr #1		@ work out 3xcachelevel
	mov	r1, r0, lsr r2				@ bottom 3 bits are the Cache type for this level
	and	r1, r1, #7					@ get those 3 bits alone
	cmp	r1, #2
	blt	skip						@ no cache or only instruction cache at this level
	mcr	p15, 2, r10, c0, c0, 0		@ write the Cache Size selection register
	isb								@ isb to sync the change to the CacheSizeID reg
	mrc	p15, 1, r1, c0, c0, 0		@ reads current Cache Size ID register
	and	r2, r1, #7					@ extract the line length field
	add	r2, r2, #4					@ add 4 for the line length offset (log2 16 bytes)
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3		@ r4 is the max number on the way size (right aligned)
	clz	r5, r4						@ r5 is the bit position of the way size increment
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13		@ r7 is the max number of the index size (right aligned)
loop2:
	mov	r9, r4						@ r9 working copy of the max way size (right aligned)
loop3:
	orr	r11, r10, r9, lsl r5		@ factor in the way number and cache number into r11
	orr	r11, r11, r7, lsl r2		@ factor in the index number
	mcr	p15, 0, r11, c7, c6, 2		@ invalidate by set/way
	subs	r9, r9, #1				@ decrement the way number
	bge	loop3
	subs	r7, r7, #1				@ decrement the index
	bge	loop2
skip:
	add	r10, r10, #2				@ increment the cache number
	cmp	r3, r10
	bgt	loop1

finished:
	mov	r10, #0						@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	dsb
	isb

	bx	lr


	.global	__PutCharacter
	.type __PutCharacter, %function
__PutCharacter:
	ldr r1, =UART_BASE_ADDRESS
	/* Check the input parameter */
	cmp r1, #0
	beq putc_error
	/* Prepend '\r' to '\n' */
	cmp r0, #0xA
	bne 2f
1:
	ldr r2, [r1, #0x14] /* LSR[23]: TP4F_NOT_FULL */
	tst r2, #0x800000
	beq 1b
	mov r2, #0xD
	str r2, [r1, #0x68] /* UART transmitter holding register 4: FIFO entry for Cortex-A*/
2:
	ldr r2, [r1, #0x14]
	tst r2, #0x800000
	beq 2b
	str r0, [r1, #0x68]
	bx	lr
putc_error:
	mov r0, #-1
	bx	lr

.end

