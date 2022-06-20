/*
 * Copyright(c) 2020, Realtek Semiconductor Corp. All rights reserved.
 *
 * Author: CTC PSP Software
 *
 */

#ifndef RV32_CRASHDUMP_H
#define RV32_CRASHDUMP_H

#include <stdint.h>
#include <stdio.h>
#include "platform_autoconf.h"

// Maximum number of instructions check while backtrace
#define MAX_INSTR_CHECK (1024)

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
				uint32_t *reg);

#endif
