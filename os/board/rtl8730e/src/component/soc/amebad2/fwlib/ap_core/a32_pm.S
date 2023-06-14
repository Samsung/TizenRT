
#define CPSR_MODE_MASK	0x1f
#define CPSR_MODE_USR	0x10
#define CPSR_MODE_FIQ	0x11
#define CPSR_MODE_IRQ	0x12
#define CPSR_MODE_SVC	0x13
#define CPSR_MODE_MON	0x16
#define CPSR_MODE_ABT	0x17
#define CPSR_MODE_UND	0x1b
#define CPSR_MODE_SYS	0x1f


	.macro mov_imm _reg, _val
		.if ((\_val) & 0xffff0000) == 0
			mov	\_reg, #(\_val)
		.else
			movw	\_reg, #((\_val) & 0xffff)
			movt	\_reg, #((\_val) >> 16)
		.endif
	.endm

	.macro save_regs mode
		cps	\mode
		mrs	r2, spsr
		str	r2, [r0], #4
		str	sp, [r0], #4
		str	lr, [r0], #4
	.endm

	.macro restore_regs mode
		cps	\mode
		ldr	r2, [r0], #4
		ldr	sp, [r0], #4
		ldr	lr, [r0], #4
		msr	spsr_fsxc, r2
	.endm


.align 4
.global	cpu_suspend
.type cpu_suspend, %function
cpu_suspend:
	stmfd	sp!, {r4 - r11, lr}
	mov	r5, sp					@ current SP
	stmfd	sp!, {r0, r1}		@ save suspend func arg and pointer
	mov	r0, r5					@ SP
	bl	cpu_suspend_save
	ldmfd	sp!, {r0, pc}		@ call suspend fn


.align 4
.global	cpu_do_suspend
.type cpu_do_suspend, %function
cpu_do_suspend:
	stmfd	sp!, {r4 - r11, lr}
	cps #CPSR_MODE_SVC
	mrc	p15, 0, r4, c13, c0, 0				@ save FCSE
	mrc	p15, 0, r5, c13, c0, 3				@ save TPIDR
	stmia	r0!, {r4 - r5}
	mrc	p15, 0, r4, c12, c0, 0				@ save VBAR
	mrc	p15, 0, r6, c10, c2, 0				@ save MAIR0. LPAE is used so MAIR0 need to be saved.
	mrrc	p15, 0, r5, r7, c2				@ save TTBR0. TTBR1 is not used so not necessary to save it
	mrc	p15, 0, r11, c2, c0, 2				@ save TTBCR
	mrc	p15, 0, r8, c1, c0, 0				@ save SCTLR
	mrc	p15, 0, r9, c1, c0, 1				@ save ACTLR
	mrc	p15, 0, r10, c1, c0, 2				@ save CPACR
	stmia	r0!, {r4 - r11}

	save_regs	#CPSR_MODE_IRQ				@ save spsr, lr ,sp for irq, fiq, abt, und, and svc mode
	save_regs	#CPSR_MODE_FIQ
	save_regs	#CPSR_MODE_ABT
	save_regs	#CPSR_MODE_UND
	save_regs	#CPSR_MODE_SVC
	cps	#CPSR_MODE_SYS

	ldmfd	sp!, {r4 - r11, pc}


.align 4
.global	cpu_resume
.type cpu_resume, %function
cpu_resume:
	ldr	r1, = ctx							@ get ctx addr directly because mmu is 1:1 mapping
	add	r0, r1, #0x4
	mov	ip, #0
	mcr	p15, 0, ip, c7, c5, 0				@ invalidate I cache
	mcr	p15, 0, ip, c13, c0, 1				@ set context id to 0
	ldmia	r0!, {r4 - r5}
	mcr	p15, 0, r4, c13, c0, 0				@ restore FCSE
	mcr	p15, 0, r5, c13, c0, 3				@ restore TPIDR
	ldmia	r0!, {r4 - r11}

	mcr	p15, 0, r4, c12, c0, 0				@ restore VBAR
	mcr	p15, 0, ip, c8, c7, 0				@ invalidate TLBs
	mcr	p15, 0, r6, c10, c2, 0				@ restore MAIR0
	mcrr	p15, 1, r1, ip, c2				@ set TTBR1 to 0 because it is not used
	mcrr	p15, 0, r5, r7, c2				@ restore TTBR0
	mcr	p15, 0, r11, c2, c0, 2				@ restore TTBCR
	mcr	p15, 0, r9, c1, c0, 1				@ restore ACTLR
	mcr	p15, 0, r10, c1, c0, 2				@ restore CPACR

	isb
	dsb

	mcr	p15, 0, r8, c1, c0, 0				@ restore SCTLR. cache and mmu will be enabled. it is safe because of 1:1 mapping

	restore_regs	#CPSR_MODE_IRQ			@ restore spsr, lr, sp for irq, fiq, abt, und, svc mode.
	restore_regs	#CPSR_MODE_FIQ
	restore_regs	#CPSR_MODE_ABT
	restore_regs	#CPSR_MODE_UND
	restore_regs	#CPSR_MODE_SVC

	isb
	dsb

	cps	#CPSR_MODE_SYS						@ freertos task is running in sys mode.
	ldr	r1, = ctx
	ldr	sp, [r1]

	isb
	mov r0, #0
	ldmfd	sp!, {r4 - r11, pc}				@ come back to where to sleep.





