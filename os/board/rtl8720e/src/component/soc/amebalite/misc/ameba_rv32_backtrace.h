/*
 * Copyright(c) 2020, Realtek Semiconductor Corp. All rights reserved.
 *
 * Author: CTC PSP Software
 *
 */

#ifndef RV32_BACKTRACE_H
#define RV32_BACKTRACE_H

#include <stdint.h>
#include <stdio.h>
#include "platform_autoconf.h"

//
// Type definition for stack frame
typedef struct {
	void *sp;
	void *pc;
	void *ra;
	uint32_t stack_size;
} stackframe;

/**
 * @brief    stack backtrace for RV32I
 *
 * @details  Parsing code on the fly to unwind stack frame
 *
 * @param    stackframe.sp: input, stack pointer
 *           stackframe.pc: input, program counter
 *           stackframe.ra: output, return address found in stack
 *           stackframe.stack_size: output, size of current stack frame
 *
 * @return   0 for success, others for failed
 */
uint32_t __attribute__((optimize("O0"))) unwind_frame_rv32i(stackframe *stack_frame,
		const uint32_t max_instr_check);

/**
 * @brief    stack backtrace for RV32C
 *
 * @details  Parsing code on the fly to unwind stack frame
 *
 * @param    [in] stackframe.sp: input, stack pointer
 *           [in] stackframe.pc: input, program counter
 *           [out] stackframe.ra: output, return address found in stack
 *           [out] stackframe.stack_size: output, size of current stack frame
 *
 * @return   0 for success, others for failed
 */
uint32_t __attribute__((optimize("O0"))) unwind_frame_rv32c(stackframe *stack_frame,
		const uint32_t max_instr_check);

// Maximum number of instructions check while backtrace
#define MAX_INSTR_CHECK (1024)

/**
 * @brief    do_stack_backtrace
 *
 * @details  Backtracing the whole call stack for RV32I/RV32C
 *           This function can be used for three purpose
 *           1. Dump callstack for crash
 *           2. Dump callstack if you want to know the caller for current function
 *              just pass NULL for pc and sp.
 *           3. Dump callstack for RTOS task with pc/sp in TCB.
 *
 * @param    [in] pc: the program counter of current context
 *           [in] sp: the stack pointer of current context
 *           [in] depth: how many frame want to trace
 *
 * @return   The number of stack frame found
 */
uint32_t do_stack_backtrace(uint32_t *pc,
							uint32_t *sp,
							uint32_t depth);

void freertos_task_stack_backtrace(uint32_t *pc, uint32_t *ra,
								   uint32_t *sp, uint32_t *stack_limit);

#endif /* RV32_BACKTRACE_H */
