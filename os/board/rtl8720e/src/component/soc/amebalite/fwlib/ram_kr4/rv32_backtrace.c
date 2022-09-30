/*
 * Copyright(c) 2020, Realtek Semiconductor Corp. All rights reserved.
 *
 * Author: CTC PSP Software
 *
 */

#include "rv32_backtrace.h"

// String IO Interface Definition
//extern int bsp_string_output(const char *format, ...);
#define bsp_string_output printf

#ifdef CONFIG_DEBUG_TINY_CRASH_DUMP
#define error_printf(fmt, args...) bsp_string_output(fmt, ##args);
#define trace_printf(fmt, args...) bsp_string_output(fmt, ##args);
#define info_printf(fmt, args...) bsp_string_output(fmt, ##args);

#else
#define error_printf(fmt, args...) bsp_string_output("[ERROR] [%s::%d] " fmt, __FUNCTION__, __LINE__, ##args);

#define trace_printf(fmt, args...) bsp_string_output("[TRACE] [%s::%d] " fmt, __FUNCTION__, __LINE__, ##args);

#define info_printf(fmt, args...) bsp_string_output("[INFO] [%s::%d] " fmt, __FUNCTION__, __LINE__, ##args);

#endif /* CONFIG_DEBUG_TINY_CRASH_DUMP */

#ifdef BACKTRACE_DEBUG
#define debug_printf(fmt, args...) bsp_string_output(fmt, ##args);
#else
#define debug_printf(fmt, args...)

#endif

// Concept of stack backtracing
//                                                            ┌───────────────┐
//                                                            │Code of frame#3│
//                                                            ├───────────────┴────────────┐
// ┌──────────────┐                                           │addiu sp, sp, -<stack size> │
// │High addresses│                                           │sw    ra, <ra offset>(sp)   │
// └───────┬──────┴──────────────┬──                          │....                        │
//         │ return address (ra) │                        ┌───▶ra of frame#2               │
//         ├─────────────────────┤                        │   │....                        │
//         │                     │                        │   │lw    ra, <ra offset>(sp)   │
//┌───────┐│   local variables   │   ╔═══════╗            │   │jr    ra                    │
//│Frame#3││                     │   ║Step #4║            │   │addiu sp, sp, <stack size>  │
//└───────┘├─────────────────────┤   ╠═══════╩════════════╩═╗ └────────────────────────────┘
//         │                     │ ┌─╣Scanning code         ║
//         │      arguments      │ │ ║Fetching offset of sp ║ ┌───────────────┐
//         │                     │ │ ║Fetching offset of ra ║ │Code of frame#2│
// ────────┼─────────────────────┼◀┘ ╚══════════════════════╝ ├───────────────┴────────────┐
//         │ return address (ra) │                            │addiu sp, sp, -<stack size> │
//         ├─────────────────────┤                            │sw    ra, <ra offset>(sp)   │
//         │                     │                            │....                        │
//┌───────┐│   local variables   │                        ┌───▶ra of frame#1               │
//│Frame#2││                     │                        │   │....                        │
//└───────┘├─────────────────────┤                        │   │lw    ra, <ra offset>(sp)   │
//         │                     │   ╔═══════╗            │   │jr    ra                    │
//         │      arguments      │   ║Step #3║            │   │addiu sp, sp, <stack size>  │
//         │                     │   ╠═══════╩════════════╩═╗ └────────────────────────────┘
// ────────┼─────────────────────┼◀──╣Scanning code         ║
//         │ return address (ra) │   ║Fetching offset of sp ║ ┌───────────────┐
//         ├─────────────────────┤   ║Fetching offset of ra ║ │Code of frame#1│
//         │                     │   ╚══════════════════════╝ ├───────────────┴────────────┐
//┌───────┐│   local variables   │                            │addiu sp, sp, -<stack size> │
//│Frame#1││                     │                            │sw    ra, <ra offset>(sp)   │
//└───────┘├─────────────────────┤                            │....                        │
//         │                     │                        ┌───▶ra of frame#0               │
//         │      arguments      │                        │   │....                        │
//         │                     │                        │   │lw    ra, <ra offset>(sp)   │
// ────────┼─────────────────────┼◀┐ ╔═══════╗            │   │jr    ra                    │
//         │ return address (ra) │ │ ║Step #2║            │   │addiu sp, sp, <stack size>  │
//         ├─────────────────────┤ │ ╠═══════╩════════════╩═╗ └────────────────────────────┘
//         │                     │ └─╣Scanning code         ║
//┌───────┐│   local variables   │   ║Fetching offset of sp ║
//│Frame#0││                     │   ║Fetching offset of ra ║ ┌───────────────┐
//└───────┘├─────────────────────┤   ╚══════════════════════╝ │Code of frame#0│
//         │                     │   ╔═══════╗                ├───────────────┴────────────┐
//         │      arguments      │   ║Step #1║                │addiu sp, sp, -<stack size> │
//         │                     │   ╠═══════╩══════╗         │sw    ra, <ra offset>(sp)   │
// ┌───────┴──────┬──────────────┘◀──╣Fetching $sp  ║         │....                        │
// │Low addresses │                  ║Fetching $epc ║─────────┼▶EPC                        │
// └──────────────┘                  ╚══════════════╝         │....                        │
//                                                            │lw    ra, <ra offset>(sp)   │
//                                                            │jr    ra                    │
//                                                            │addiu sp, sp, <stack size>  │
//                                                            └────────────────────────────┘

// Type definition for RV32I S-Format(SW) instruction
// ┌─────────┬───────┬───────┬─────┬───────┬──────────┐
// │imm[11:5]│  rs2  │  rs1  │funct│  imm  │  opcode  │
// │    7    │   5   │   5   │  3  │ [4:0] │    7     │
// └─────────┴───────┴───────┴─────┴───────┴──────────┘
// Type definition for RV32 I-Format(ADDIW) instruction
// ┌──────────────┬────────┬──────┬────────┬──────────┐
// │  imm[11:0]   │  rs1   │funct3│   rd   │  opcode  │
// │      12      │   5    │  3   │   5    │    7     │
// └──────────────┴────────┴──────┴────────┴──────────┘
typedef union {
	struct {
		uint32_t opcode: 7; /*!< bit:  0..6    opcode */
		uint32_t imm0: 5;  /*!< bit:  7..11   imm[4:0] */
		uint32_t funct3: 3; /*!< bit:  12..14  funct3 */
		uint32_t rs1: 5;   /*!< bit:  15..19  rs1 */
		uint32_t rs2: 5;   /*!< bit:  20..24  rs2 */
		uint32_t imm1: 7;  /*!< bit:  25..31  imm[11:5] */
	} s_format;            /*!< Structure used for bit  access */
	struct {
		uint32_t opcode: 7; /*!< bit:  0..6    opcode */
		uint32_t rd: 5;    /*!< bit:  7..11   dest */
		uint32_t funct3: 3; /*!< bit:  12..14  funct3 */
		uint32_t rs1: 5;   /*!< bit:  15..19  rs1 */
		uint32_t imm: 12;  /*!< bit:  20..31  imm[11:0] */
	} i_format;            /*!< structure used for bit  access */
	struct {
		uint32_t opcode: 7; /*!< bit:  0..6    opcode */
		uint32_t other0: 5; /*!< bit:  7..11   others */
		uint32_t funct3: 3; /*!< bit:  12..14  others */
		uint32_t other1;   /*!< bit:  15..31  others */
	} g_format;            /*!< structure used for bit  access */
	uint32_t w;            /*!< type      used for word access */
} rv32i_instruction;

// Type definition for RV32C CI c.addi instruction
// ┌──────┬────────┬───────────┬──────────┬──────┐
// │ 000  │nzimm[5]│  rs1/rd   │nzimm[4:0]│  01  │
// │15..13│   12   │   11..7   │   6..2   │ 1..0 │
// └──────┴────────┴───────────┴──────────┴──────┘
// Type definition for RV32C CI c.addi16sp instruction
// ┌──────┬────────┬───────────┬────────────────┬──────┐
// │ 011  │nzimm[9]│  rs1/rd   │nzimm[4|6|8:7|5]│  01  │
// │15..13│   12   │   11..7   │      6..2      │ 1..0 │
// └──────┴────────┴───────────┴────────────────┴──────┘
// Type definition for RV32C CSS c.swsp instruction
// ┌──────┬───────────────────┬───────────┬──────┐
// │ 110  │   uimm[5:2|7:6]   │    rs2    │  10  │
// │15..13│       12..7       │   6..2    │ 1..0 │
// └──────┴───────────────────┴───────────┴──────┘
typedef union {
	struct {
		uint16_t opcode0: 2; /*!< bit:  0..1     opcode0     */
		uint16_t nzimm0: 5; /*!< bit:  2..6     nzimm[4:0]  */
		uint16_t rs1rd: 5;  /*!< bit:  7..11    rs1/rd != 0 */
		uint16_t nzimm1: 1; /*!< bit:  12..12   nzimm[5]    */
		uint16_t opcode1: 3; /*!< bit:  13..15   opcode1     */
	} ci_format;
	struct {
		uint16_t opcode0: 2; /*!< bit:  0..1     opcode0     */
		uint16_t nzimm0: 1; /*!< bit:  2..2     nzimm[5]    */
		uint16_t nzimm1: 2; /*!< bit:  3..4     nzimm[8:7]  */
		uint16_t nzimm2: 1; /*!< bit:  5..5     nzimm[6]    */
		uint16_t nzimm3: 1; /*!< bit:  6..6     nzimm[4]    */
		uint16_t rs1rd: 5;  /*!< bit:  7..11    rs1/rd != 0 */
		uint16_t nzimm4: 1; /*!< bit:  12..12   nzimm[9]    */
		uint16_t opcode1: 3; /*!< bit:  13..15   opcode1     */
	} ci_addi16sp_format;
	struct {
		uint16_t opcode0: 2; /*!< bit:  0..1     opcode0     */
		uint16_t rs2: 5;    /*!< bit:  2..6     rs2         */
		uint16_t uimm0: 2;  /*!< bit:  7..8     uimm[7:6]   */
		uint16_t uimm1: 4;  /*!< bit:  9..12    uimm[5:2]   */
		uint16_t opcode1: 3; /*!< bit:  13..15   opcode1     */
	} css_format;
	uint16_t hw;           /*!< type      used for half word access */
} rv32c_instruction;

// Store $ra, ex. <sw ra, 12(sp>
#define OP_STORE        (0x23)
#define FUNC_SW         (0x2)

// Store for RV32C, CSS c.swsp
#define OP_RV32C_SW1    (0x6)
#define OP_RV32C_SW0    (0x2)

// Push $sp, ex. <addi sp, sp, -48>
#define OP_IMM          (0x13)
#define FUNC_ADDIW      (0x0)

// Push for RV32C $sp, CI c.addi
#define OP_RV32C_ADDI1  (0x0)
#define OP_RV32C_ADDI0  (0x1)

// Push for RV32C $sp, CI c.addi16sp
#define OP_RV32C_ADDI1_16SP  (0x3)
#define OP_RV32C_ADDI0_16SP  (0x1)

// RET:: I-format jalr x0, x1, 0
#define OP_RET          (0x00008067)

// RET_RV32C::
#define OP_RV32C_RET    (0x8082)

// utils macro
#define ABS(x) ((x<0)?-x:x)

// RISC-V register
enum {
	REG_ZERO = 0,
	REG_RA,
	REG_SP,
	REG_GP,
	REG_TP,
	REG_T0,
	REG_T1,
	REG_T2,
	REG_S0,
	REG_S1,
	REG_A0,
	REG_A1,
	REG_A2,
	REG_A3,
	REG_A4,
	REG_A5,
#if !defined( __riscv_32e ) || ( __riscv_32e == 0 )
	REG_A6,
	REG_A7,
	REG_S2,
	REG_S3,
	REG_S4,
	REG_S5,
	REG_S6,
	REG_S7,
	REG_S8,
	REG_S9,
	REG_S10,
	REG_S11,
	REG_T3,
	REG_T4,
	REG_T5,
	REG_T6,
#endif
	REG_END
};

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
uint32_t unwind_frame_rv32i(stackframe *stack_frame,
							const uint32_t max_instr_check)
{
	int32_t ra_offset = 0;
	int32_t stack_size = 0;
	uint32_t *addr;
	rv32i_instruction ip;
	rv32c_instruction ipc1, ipc2;

	if (stack_frame->pc == 0 || stack_frame->sp == 0) {
		debug_printf("pc(%p) / sp(%p) is NULL\n", stack_frame->pc, stack_frame->sp);
		return -1;
	}

	// Start to search instruction for stack allocating and ra storing
	for (addr = (uint32_t *)stack_frame->pc; ((void *)(addr + max_instr_check) > stack_frame->pc)
		 && (!ra_offset || !stack_size) && (addr != 0); --addr) {

		// fetch next instruction
		if ((uint32_t)addr & 2) {
			ip.w = ((*(uint16_t *)addr)) | (((*(uint16_t *)(((uint32_t)addr) + 2))) << 16);
		} else {
			ip.w = *(uint32_t *)addr;
		}

		ipc1.hw = *(uint16_t *)addr;
		ipc2.hw = *(uint16_t *)(((uint32_t)addr) + 2);

		debug_printf("[%p] -> %lx\n", addr, ip.w);

		if (ip.g_format.opcode == OP_IMM && ip.g_format.funct3 == FUNC_ADDIW) {
			if (ip.i_format.rd == REG_SP && ip.i_format.rs1 == REG_SP) {
				// Found pushing $sp instruction
				struct {
					int x: 12;
				} sp_imm;
				int32_t stack_offset;
				stack_offset = sp_imm.x = ip.i_format.imm;
				debug_printf("Found pushing $sp instruction, stack frame size -> %ld (%lx)\n",
							 ABS(stack_offset), ABS(stack_offset));

				if (stack_offset > 0) {
					/* This marks the end of the previous function,
					   which means we overran. */
					error_printf("stack_size(%ld) is positive ??\n", stack_size);
					return -1;
				}
				stack_size = ABS(stack_offset);
			}
		} else if (ip.g_format.opcode == OP_STORE && ip.g_format.funct3 == FUNC_SW) {
			if (ip.s_format.rs1 == REG_SP && ip.s_format.rs2 == REG_RA) {
				// Found storing $ra instruction
				struct {
					int x: 12;
				} ra_imm;
				ra_offset = ra_imm.x = (ip.s_format.imm1 << 5 | ip.s_format.imm0);
				debug_printf("Found storing $ra instruction, ra offset -> %ld (0x%lx)\n", ra_offset, ra_offset);

				if (ra_offset < 0) {
					/* This shouldn't happen. */
					error_printf("ra_offset(%ld) is negative??", ra_offset);
					return -1;
				}
			}
		} else if (ip.w == OP_RET) {
			// Hit return instruction of previous function, this should be leaf function or compiler optimized function
			debug_printf("Hit return instruction of previous function, abort traceing %lx\n", ip.w);
			break;
		} else if (ipc1.hw == OP_RV32C_RET || ipc2.hw == OP_RV32C_RET) {
			debug_printf("Hit rv32c return instruction of previous function, abort traceing %lx\n", ip.w);
			return -1;
		}
	}

	// Update the return address and stack size
	stack_frame->ra = (!ra_offset) ? 0 : (void *) * (uint32_t *)(stack_frame->sp + ra_offset);
	stack_frame->stack_size = (!stack_size) ? 0 : stack_size;

	if (!ra_offset || !stack_size) {
		// Missing return address or stack size
		debug_printf("ra_offset(%lx) or stack_size(%lx) is zero, can't backtrace, is it RV32C?\n", ra_offset, stack_size);
		return -1;
	}

	return 0;
}

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
uint32_t unwind_frame_rv32c(stackframe *stack_frame,
							const uint32_t max_instr_check)
{
	int32_t ra_offset = 0;
	int32_t stack_size = 0;
	uint16_t *addr;
	rv32c_instruction ip;

	if (stack_frame->pc == 0 || stack_frame->sp == 0 || (uint32_t)stack_frame->pc % 2) {
		debug_printf("pc(%p) / sp(%p) is NULL or not 2 bytes aligned\n", stack_frame->pc, stack_frame->sp);
		return -1;
	}

	// Start to search instruction for stack allocating and ra storing
	for (addr = (uint16_t *)stack_frame->pc; ((void *)(addr + max_instr_check) > stack_frame->pc)
		 && (!ra_offset || !stack_size) && (addr != 0); --addr) {

		// fetch next instruction
		ip.hw = *addr;
		debug_printf("[%p] -> %x\n", addr, *addr);

		debug_printf("\t op1-> %x, op0-> %x\n", ip.css_format.opcode1, ip.css_format.opcode0);
		if (ip.ci_format.opcode1 == OP_RV32C_ADDI1 &&
			ip.ci_format.opcode0 == OP_RV32C_ADDI0 &&
			ip.ci_format.rs1rd == REG_SP) {
			// Found pushing $sp instruction
			struct {
				int x: 6;
			} sp_imm;
			int32_t stack_offset;
			stack_offset = sp_imm.x = ip.ci_format.nzimm1 << 5 | ip.ci_format.nzimm0;
			debug_printf("Found pushing $sp instruction, stack frame size -> %ld (%lx)\n",
						 ABS(stack_offset), ABS(stack_offset));

			if (stack_offset > 0) {
				/* This marks the end of the previous function,
				   which means we overran. */
				error_printf("stack_size(%ld) is positive ??\n", stack_size);
				return -1;
			}
			stack_size = ABS(stack_offset);
		} else if (ip.ci_format.opcode1 == OP_RV32C_ADDI1_16SP &&
				   ip.ci_format.opcode0 == OP_RV32C_ADDI0_16SP &&
				   ip.ci_format.rs1rd == REG_SP) {
			// Found pushing $sp instruction
			struct {
				int x: 10;
			} sp_imm;
			int32_t stack_offset;
			// addi16sp: ADD imm*16 to SP
			stack_offset = sp_imm.x = ip.ci_addi16sp_format.nzimm4 << 9 |
									  ip.ci_addi16sp_format.nzimm3 << 4 |
									  ip.ci_addi16sp_format.nzimm2 << 6 |
									  ip.ci_addi16sp_format.nzimm1 << 7 |
									  ip.ci_addi16sp_format.nzimm0 << 5;
			debug_printf("Found pushing $sp instruction, stack frame size -> %ld (%lx)\n",
						 ABS(stack_offset), ABS(stack_offset));

			if (stack_offset > 0) {
				/* This marks the end of the previous function,
				   which means we overran. */
				error_printf("stack_size(%ld) is positive ??\n", stack_size);
				return -1;
			}
			stack_size = ABS(stack_offset);
		} else if (ip.css_format.opcode1 == OP_RV32C_SW1 &&
				   ip.css_format.opcode0 == OP_RV32C_SW0 &&
				   ip.css_format.rs2 == REG_RA) {
			// Found storing $ra instruction
			struct {
				int x: 8;
			} ra_imm;
			ra_offset = ra_imm.x = (ip.css_format.uimm0 << 6 | ip.css_format.uimm1 << 2);
			debug_printf("Found storing $ra instruction, ra offset -> %ld (0x%lx)\n", ra_offset, ra_offset);

			if (ra_offset < 0) {
				/* This shouldn't happen. */
				error_printf("ra_offset(%ld) is negative??", ra_offset);
				return -1;
			}
		} else if (ip.hw == OP_RV32C_RET) {
			// Hit return instruction of previous function, this should be leaf function or compiler optimized function
			debug_printf("Hit return instruction of previous function, abort traceing %x\n", ip.hw);
			break;
		}
	}

	// Update the return address and stack size
	stack_frame->ra = (!ra_offset) ? 0 : (void *) * (uint32_t *)(stack_frame->sp + ra_offset);
	stack_frame->stack_size = (!stack_size) ? 0 : stack_size;

	if (!ra_offset || !stack_size) {
		// Missing return address or stack size
		debug_printf("ra_offset(%lx) or stack_size(%lx) is zero, can't backtrace\n", ra_offset, stack_size);
		return -1;
	}

	return 0;
}

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
							uint32_t depth)
{
	uint32_t frame_count = 0;
	uint32_t *current_sp = 0;
	uint32_t *current_pc = 0;
	stackframe frame = { .pc = pc,
						 .sp = sp
					   };

	// If caller pass pc/sp as 0, let's do self stack backtrace.
	if (pc == NULL || sp == NULL) {
		asm volatile("addi %0, sp, 0" : "=r"(current_sp) ::);
		asm volatile("auipc %0, 0" : "=r"(current_pc) ::);

		frame.pc = current_pc;
		frame.sp = current_sp;
	}

	trace_printf("Start stack backtracing for sp %p, pc %p\n", frame.sp, frame.pc);

	while (depth--) {
		if (unwind_frame_rv32i(&frame, MAX_INSTR_CHECK)) {
			// Can't backtrace stack for rv32i, maybe it's rv32c? Take a shoot
			if (unwind_frame_rv32c(&frame, MAX_INSTR_CHECK)) {
				// Can't backtrace stack for rv32c neither, end processing.
				break;
			}
		}
		// TODO: Should we use function pointer for user to process frame we found.
		trace_printf("[frame #%ld] sp-> %p, pc-> %p, stack_size-> %ld, ra-> %p\n",
					 frame_count++, frame.sp, frame.pc, frame.stack_size, frame.ra);

		// Update the sp and pc for next stack frame searching
		frame.pc = frame.ra;
		frame.sp += frame.stack_size;
	}

	return frame_count;
}

void freertos_task_stack_backtrace(uint32_t *pc, uint32_t *ra,
								   uint32_t *sp, uint32_t *stack_limit)
{
	uint32_t frame_found = 0;

	trace_printf("========== Stack Trace ==========\n");
	frame_found = do_stack_backtrace((uint32_t *)pc, sp, 10);
	if (frame_found == 0) {
		// No any stack frame found?? Maybe it's leaf function, use ra to take a shoot
		// Please make sure the address of *reg* is from x0, otherwise need to make
		// corresponding changes to get RA address for stack backtrace.
		//
		// Sometimes compiler will push $sp, but not store ra into stack in leaf function
		// So we need to get the stack frame size first
		stackframe frame_rv32i = { .pc = pc, .sp = sp };
		stackframe frame_rv32c = { .pc = pc, .sp = sp };
		uint32_t stack_size = 0;

		unwind_frame_rv32i(&frame_rv32i, MAX_INSTR_CHECK);
		unwind_frame_rv32c(&frame_rv32c, MAX_INSTR_CHECK);
		stack_size = (frame_rv32i.stack_size) ? frame_rv32i.stack_size : frame_rv32c.stack_size;

		sp = sp + (stack_size >> 2);
		if (sp < stack_limit) {
			trace_printf("Is this leaf function? Using RA as PC to do backtrace, stack_size(%ld/%ld)\n",
						 frame_rv32i.stack_size, frame_rv32c.stack_size);
			do_stack_backtrace(ra, sp, 10);
		}
	}
	trace_printf("========== End of Stack Trace ==========\n");
}
