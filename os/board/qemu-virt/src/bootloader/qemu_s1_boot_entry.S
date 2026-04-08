
.syntax unified
.arch armv7-a

.section .vectors, "ax", %progbits
.global _start
.extern s1_boot
.extern __stack_top, __bss_start__, __bss_end__
_start:
	b Reset_Handler
	b Undefined_Handler
	b SVC_Handler
	b Prefetch_Abort_Handler
	b Data_Abort_Handler
	b .  /* Reserved */
	b IRQ_Handler
	b FIQ_Handler

.section .text
Reset_Handler:
	cpsid if, #31
	/* Set up stack pointer using the symbol from the linker script */
	ldr r0, =__stack_top
	mov sp, r0

	/* Clear .bss using symbols from the linker script */
	ldr r0, =__bss_start__
	ldr r1, =__bss_end__
	mov r2, #0
clear_bss:
	cmp r0, r1
	bge bss_cleared
	str r2, [r0], #4
	b clear_bss
bss_cleared:

	/* Call C code */
	bl s1_boot

	/* Loop forever if s1_boot returns */
loop:
	b loop

Undefined_Handler:
	b Undefined_Handler

SVC_Handler:
	b SVC_Handler

Prefetch_Abort_Handler:
	b Prefetch_Abort_Handler

Data_Abort_Handler:
	b Data_Abort_Handler

IRQ_Handler:
	b IRQ_Handler

FIQ_Handler:
	b FIQ_Handler
