/*
 * Copyright(c) 2020, Realtek Semiconductor Corp. All rights reserved.
 *
 * Author: CTC PSP Software
 *
 */

#include "ameba_rv32_crashdump.h"
#ifdef CONFIG_DEBUG_RXI_DUMP
#include "rxi300_bus.h"
#endif
#ifdef CONFIG_DEBUG_BACK_TRACE
#include "ameba_rv32_backtrace.h"
#endif
#ifdef CONFIG_DEBUG_NWATCH
#include <nwatch.h>
#endif

// String IO Interface Definition
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

// definition for index of RISC-V CSR register
#define REG_MSTATUS     (0x300)
#define REG_MISA        (0x301)
#define REG_MEDELEG     (0x302)
#define REG_MIDELEG     (0x303)
#define REG_MIE         (0x304)
#define REG_MTVEC       (0x305)
#define REG_MSCRATCH    (0x340)
#define REG_MEPC        (0x341)
#define REG_MCAUSE      (0x342)
#define REG_MTVAL       (0x343)
#define REG_MIP         (0x344)

enum {
	EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED = 0,
	EXCEPTION_INSTRUCTION_ACCESS_FAULT,
	EXCEPTION_ILLEGAL_INSTRUCTION,
	EXCEPTION_BREAKPOINT,
	EXCEPTION_LOAD_ADDRESS_MISALIGNED,
	EXCEPTION_LOAD_ACCESS_FAULT,
	EXCEPTION_STORE_ADDRESS_MISALIGNED,
	EXCEPTION_STORE_ADDRESS_FAULT,
	EXCEPTION_ENVIRONMENT_CALL_FROM_U_MODE,
	EXCEPTION_ENVIRONMENT_CALL_FROM_S_MODE,
	EXCEPTION_RESERVED1,
	EXCEPTION_ENVIRONMENT_CALL_FROM_M_MODE,
	EXCEPTION_INSTRUCTION_PAGE_FAULT,
	EXCEPTION_LOAD_PAGE_FAULT,
	EXCEPTION_RESERVED2,
	EXCEPTION_STORE_PAGE_FAULT,
	EXCEPTION_END,
};

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

#ifndef CONFIG_DEBUG_TINY_CRASH_DUMP
static const char *const exception_code_string[] = {
	[EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED]  = "Instruction address misaligned",
	[EXCEPTION_INSTRUCTION_ACCESS_FAULT]        = "Instruction access fault",
	[EXCEPTION_ILLEGAL_INSTRUCTION]             = "Illegal instruction",
	[EXCEPTION_BREAKPOINT]                      = "Breakpoint",
	[EXCEPTION_LOAD_ADDRESS_MISALIGNED]         = "Load addess misaligned",
	[EXCEPTION_LOAD_ACCESS_FAULT]               = "Load access fault",
	[EXCEPTION_STORE_ADDRESS_MISALIGNED]        = "Store address misaligned",
	[EXCEPTION_STORE_ADDRESS_FAULT]             = "Store address fault",
	[EXCEPTION_ENVIRONMENT_CALL_FROM_U_MODE]    = "Environment call from U mode",
	[EXCEPTION_ENVIRONMENT_CALL_FROM_S_MODE]    = "Environment call from S mode",
	[EXCEPTION_ENVIRONMENT_CALL_FROM_M_MODE]    = "Environment call from M mode",
	[EXCEPTION_INSTRUCTION_PAGE_FAULT]          = "Instruction page fault",
	[EXCEPTION_LOAD_PAGE_FAULT]                 = "Load page fault",
	[EXCEPTION_STORE_PAGE_FAULT]                = "Store page fault",
	[EXCEPTION_RESERVED1]                       = "Reserved exception",
};
#endif /* CONFIG_DEBUG_TINY_CRASH_DUMP */

static const char *const register_string[] = {
	[REG_ZERO]      = "zero",
	[REG_RA]        = "ra",
	[REG_SP]        = "sp",
	[REG_GP]        = "gp",
	[REG_TP]        = "tp",
	[REG_T0]        = "t0",
	[REG_T1]        = "t1",
	[REG_T2]        = "t2",
	[REG_S0]        = "s0/fp",
	[REG_S1]        = "s1",
	[REG_A0]        = "a0",
	[REG_A1]        = "a1",
	[REG_A2]        = "a2",
	[REG_A3]        = "a3",
	[REG_A4]        = "a4",
	[REG_A5]        = "a5",
#if !defined( __riscv_32e ) || ( __riscv_32e == 0 )
	[REG_A6]        = "a6",
	[REG_A7]        = "a7",
	[REG_S2]        = "s2",
	[REG_S3]        = "s3",
	[REG_S4]        = "s4",
	[REG_S5]        = "s5",
	[REG_S6]        = "s6",
	[REG_S7]        = "s7",
	[REG_S8]        = "s8",
	[REG_S9]        = "s9",
	[REG_S10]       = "s10",
	[REG_S11]       = "s11",
	[REG_T3]        = "t3",
	[REG_T4]        = "t4",
	[REG_T5]        = "t5",
	[REG_T6]        = "t6",
#endif
};

#ifndef CONFIG_DEBUG_TINY_CRASH_DUMP
typedef struct {
	char *cause_string;
	char *mtval_string;
} exception_cause_string_t, *pexception_cause_string_t;

static const exception_cause_string_t exception_cause_0_string[] = {
	{.cause_string = "Branch target misaligned", .mtval_string = "Target address"},
	{.cause_string = "Instruction address misaligned", .mtval_string = "Instruction address"},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_1_string[] = {
	{.cause_string = "Instruction PMP check fail", .mtval_string = "Instruction address"},
	{.cause_string = "Instruction Page walk bus error", .mtval_string = "Instruction address"},
	{.cause_string = "Instruction bus error", .mtval_string = "Instruction address"},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_2_string[] = {
	{.cause_string = "Illegal instruction", .mtval_string = "Instruction opcode"},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_4_string[] = {
	{.cause_string = "Load address misaligned", .mtval_string = "Load address"},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_5_string[] = {
	{.cause_string = "Load PMP check fail", .mtval_string = "Load address"},
	{.cause_string = "Load Page walk bus error", .mtval_string = "Load address"},
	{.cause_string = "Load bus error", .mtval_string = NULL},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_6_string[] = {
	{.cause_string = "Store/AMO address misaligned", .mtval_string = "Store address"},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_7_string[] = {
	{.cause_string = "Srore/AMO PMP check fail", .mtval_string = "Store address"},
	{.cause_string = "Store/AMO page walk bus error", .mtval_string = "Store address"},
	{.cause_string = "Store bus error", .mtval_string = NULL},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_12_string[] = {
	{.cause_string = "Instruction page walk check fail", .mtval_string = "Instruction address"},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_13_string[] = {
	{.cause_string = "Load page walk check fail", .mtval_string = "Load address"},
	{NULL, NULL}
};

static const exception_cause_string_t exception_cause_15_string[] = {
	{.cause_string = "Sore/AMO page walk check fail", .mtval_string = "Store address"},
	{NULL, NULL}
};

static const pexception_cause_string_t exception_cause_string_tbl[] = {
	[EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED]  = (pexception_cause_string_t)exception_cause_0_string,
	[EXCEPTION_INSTRUCTION_ACCESS_FAULT]        = (pexception_cause_string_t)exception_cause_1_string,
	[EXCEPTION_ILLEGAL_INSTRUCTION]             = (pexception_cause_string_t)exception_cause_2_string,
	[EXCEPTION_BREAKPOINT]                      = NULL,
	[EXCEPTION_LOAD_ADDRESS_MISALIGNED]         = (pexception_cause_string_t)exception_cause_4_string,
	[EXCEPTION_LOAD_ACCESS_FAULT]               = (pexception_cause_string_t)exception_cause_5_string,
	[EXCEPTION_STORE_ADDRESS_MISALIGNED]        = (pexception_cause_string_t)exception_cause_6_string,
	[EXCEPTION_STORE_ADDRESS_FAULT]             = (pexception_cause_string_t)exception_cause_7_string,
	[EXCEPTION_ENVIRONMENT_CALL_FROM_U_MODE]    = NULL,
	[EXCEPTION_ENVIRONMENT_CALL_FROM_S_MODE]    = NULL,
	[EXCEPTION_RESERVED1]                       = NULL,
	[EXCEPTION_ENVIRONMENT_CALL_FROM_M_MODE]    = NULL,
	[EXCEPTION_INSTRUCTION_PAGE_FAULT]          = (pexception_cause_string_t)exception_cause_12_string,
	[EXCEPTION_LOAD_PAGE_FAULT]                 = (pexception_cause_string_t)exception_cause_13_string,
	[EXCEPTION_RESERVED2]                       = NULL,
	[EXCEPTION_STORE_PAGE_FAULT]                = (pexception_cause_string_t)exception_cause_15_string,
};
#endif /* CONFIG_DEBUG_TINY_CRASH_DUMP */

#define READ_CSR(result, csr_num)   asm volatile("csrr %0, %1" : "=r"(result) : "I"(csr_num))

/**
 * @brief    Dump crash informtion
 *
 * @details  Dump registers exception handler stored and trigger stack backtrace
 *
 * @param    [in] pc: The program counter while crash
 *           [in] sp: The stack pointer while crash
 *           [in] reg: The pointer to memory which exception handler store the registers
 *
 * @return   NULL
 */
void crash_dump(uint32_t *pc,
				uint32_t *sp,
				uint32_t *reg)
{
	uint32_t mscratch  = 0;
	uint32_t mepc      = 0;
	uint32_t mcause    = 0;
	uint32_t mtval     = 0;
	int32_t nr_runs = CONFIG_DEBUG_CRASH_DUMP_RUNS;

	READ_CSR(mscratch, REG_MSCRATCH);
	READ_CSR(mepc, REG_MEPC);
	READ_CSR(mcause, REG_MCAUSE);
	READ_CSR(mtval, REG_MTVAL);

#ifdef CONFIG_DEBUG_TINY_CRASH_DUMP
	(void)(pc);
#endif

	while (nr_runs > 0) {
#ifndef CONFIG_DEBUG_TINY_CRASH_DUMP
		pexception_cause_string_t excp_cause;
		trace_printf("========== Crash Dump ==========\n");
		trace_printf("Exception caught on %p with reason [0x%lx] -> [%s]\n",
					 pc, mcause, (mcause < EXCEPTION_END) ? exception_code_string[mcause] : "Unknown exception");

		if (mcause < EXCEPTION_END) {
			excp_cause = exception_cause_string_tbl[mcause];
			if (excp_cause != NULL) {
				trace_printf("Exception cause could be:\r\n");
				while (excp_cause->cause_string != NULL) {
					if (excp_cause->mtval_string != NULL) {
						trace_printf("\t%s, %s = 0x%lx\r\n", excp_cause->cause_string, \
									 excp_cause->mtval_string, mtval);
					} else {
						trace_printf("\t%s\r\n", excp_cause->cause_string);
					}
					excp_cause++;
				}
			}
		}
#endif /* CONFIG_DEBUG_TINY_CRASH_DUMP */

		trace_printf("========== Register Dump ==========\n")
		trace_printf("[mscratch] 0x%8.8lx\n", mscratch);
		trace_printf("[mepc]     0x%8.8lx\n", mepc);
		trace_printf("[mcause]   0x%8.8lx\n", mcause);
		trace_printf("[mtval]    0x%8.8lx\n", mtval);

		// Dump general register information
		// Interrupr/exception handler need to help saving general registers at beginning
		for (int32_t i = REG_ZERO ; i < REG_END ; i++) {
			trace_printf("[x%2.2ld -> %5s] 0x%8.8lx\n", i, register_string[i], reg[i]);
		}

#ifdef CONFIG_DEBUG_NWATCH
		nwatch_t nwatch;
		trace_printf("========== Trigger Dump ==========\n");
		for (uint32_t i = 0; i < 8; i++) {
			nwatch.index = i;
			if (nwatch_match(&nwatch)) {
				continue;
			}

			if (nwatch.hit) {
				trace_printf("[%8d] 0x%8.8lx\n", i, nwatch.address);
				nwatch.hit = 0;
			}
		}
#endif

#ifndef CONFIG_DEBUG_BACK_TRACE
		// Dump stack content while crash
		trace_printf("========== Stack Dump ==========\n");
		int j;
		for (j = 0; (CONFIG_STACK_DUMP_DEPTH - j) >= 4; j += 4) {
			trace_printf("[%8d] 0x%8.8lx 0x%8.8lx 0x%8.8lx 0x%8.8lx\n", j, sp[j], sp[j + 1],
						 sp[j + 2], sp[j + 3]);
		}

		if ((CONFIG_STACK_DUMP_DEPTH - j) > 0) {
			trace_printf("[%8d] ", j);

			while ((CONFIG_STACK_DUMP_DEPTH - j) > 0) {
				trace_printf("0x%8.8lx ", sp[j]);
				j++;
			}
			trace_printf("\n");
		}
#else
		trace_printf("========== Stack Trace ==========\n");
		uint32_t frame_found = 0;
		frame_found = do_stack_backtrace(pc, sp, 10);
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

			trace_printf("Is this leaf function? Using RA as PC to do backtrace, stack_size(%ld/%ld)\n",
						 frame_rv32i.stack_size, frame_rv32c.stack_size);
			do_stack_backtrace((uint32_t *)reg[REG_RA], sp + (stack_size >> 2), 10);
		}
		trace_printf("========== End of Stack Trace ==========\n");
#endif /* CONFIG_DEBUG_TINY_CRASH_DUMP */

#ifdef CONFIG_DEBUG_RXI_DUMP
		// dump bus fault error log if current exception could be caused by a bus error
		switch (mcause) {
		case EXCEPTION_INSTRUCTION_ACCESS_FAULT:
		case EXCEPTION_LOAD_ACCESS_FAULT:
		case EXCEPTION_STORE_ADDRESS_FAULT:
		case EXCEPTION_LOAD_PAGE_FAULT:
		case EXCEPTION_STORE_PAGE_FAULT:
		case EXCEPTION_INSTRUCTION_PAGE_FAULT:
			dump_rxi_bus_err_log();
			break;

		default:
			break;
		}
#endif
		trace_printf("========== End of Crash Dump ==========\n");
		trace_printf("\r\n\r\n");
		nr_runs--;
	}
}
