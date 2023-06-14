
/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

/**
 * @file asm_vectors.s
 *
 * This file contains the initial vector table for the Cortex A35 processor
 *
 */

	.org 0
	.text
	.section .vectors,"ax"

	.globl	_vector_table
	.extern _boot
	.extern FreeRTOS_SWI_Handler
	.extern FreeRTOS_IRQ_Handler
	.extern UndefinedExceptionAddr
	.extern DataAbortAddr
	.extern PrefetchAbortAddr

	
_vector_table:
	B	_boot
	B	FreeRTOS_UndefinedHandler
	B	FreeRTOS_SWI_Handler
	B	FreeRTOS_PrefetchAbortHandler
	B	FreeRTOS_DataAbortHandler
	B	.
	B	FreeRTOS_IRQ_Handler
	B	FreeRTOS_FIQHandler


	.align	4
FreeRTOS_FIQHandler:				@ FIQ vector handler */
	STMDB	SP!, {R0-R3, R12, LR}	@ state save from compiled code

	BL	FIQHandler					@ FIQ vector

	LDMIA	SP!, {R0-R3, R12, LR}	@ state restore from compiled code
	SUBS	PC, LR, #4				@ adjust return


	.align	4
FreeRTOS_UndefinedHandler:			@ Undefined handler
	STMDB	SP!, {R0-R3, R12, LR}	@ state save from compiled code
	LDR		R0, =UndefinedExceptionAddr
	SUB		R1, LR, #4
	STR		R1, [R0]            	@ store address of instruction causing exception

	BL	UndefinedExceptionHandler	@ call C function here

	LDMIA	SP!, {R0-R3, R12, LR}	@ state restore from compiled code
	MOVS	PC, LR

	.align	4
FreeRTOS_DataAbortHandler:			@ Data Abort handler
	STMDB	SP!, {R0-R3, R12, LR}	@ state save from compiled code
	LDR		R0, =DataAbortAddr
	SUB		R1, LR, #8
	STR		R1, [R0]            	@ stores instruction causing data abort

	bl		DataAbortHandler		@ call C function

	LDMIA	SP!, {R0-R3, R12, LR}	@ state restore from compiled code
	SUBS	PC, LR, #8				@ points to the instruction that caused the exception

	.align	4
FreeRTOS_PrefetchAbortHandler:		@ Prefetch Abort handler
	STMDB	SP!, {R0-R3, R12, LR}	@ state save from compiled code
	LDR		R0, =PrefetchAbortAddr
	SUB		R1, LR, #4
	STR		R1, [R0]	 			@ stores instruction causing prefetch abort

	BL		PrefetchAbortHandler	@ call C function

	LDMIA	SP!, {R0-R3, R12, LR}	@ state restore from compiled code
	SUBS	PC, LR, #4				@ points to the instruction that caused the exception

.end
