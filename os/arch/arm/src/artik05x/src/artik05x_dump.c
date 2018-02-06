/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/artik05x/src/artik05x_dump.c
 *
 *   Copyright (C) 2010 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <tinyara/board.h>
#include <chip/s5j_memorymap.h>


#define CRASHDUMP_COMMON_START_MAGIC		0x49545241   //ARTI
#define CRASHDUMP_RAM_DUMP_START_MAGIC		0x4D41524B   //KRAM
#define CRASHDUMP_REG_DUMP_START_MAGIC		0x4745524B   //KREG
#define CRASHDUMP_COMMON_END_MAGIC			0x454E4F44   //DONE
#define CRASHDUMP_MAGIC_LEN					4

#define CRASHDUMP_PARTITION_START_ADDR		(CONFIG_ARTIK05X_OTA_BIN_ADDR + S5J_FLASH_PADDR)
#define CRASHDUMP_PARTION_SIZE				(CONFIG_ARTIK05X_OTA_BIN_SIZE)

#define CRASHDUMO_START_ADDR				(CRASHDUMP_PARTITION_START_ADDR + 16)



struct arm_mode_regs_s {
	/* ARM provides 37 General purpos registers as below */
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	/* When we enter exception mode, registers are saved in SVC mode
	 * so map svc registers bank registers to save first
	 * SVC mode has R13_SVC(sp), R14_SVC(lr) and SPSR_SVC as banked registers
	 */
	uint32_t sp_svc;
	uint32_t lr_svc;
	/* R15 and cpsr will have pc and current execution mode status */
	uint32_t pc;
	uint32_t spsr_svc;
	uint32_t cpsr;
	/* ARM provides other modes like USER/FIQ/IRQ/ABT/UND/SYS modes so save their
	 * banked registers for easy debugging.
	 * USR/SYS mode both uses same register set (USR mode:0x10 and SYS:0x1F)
	 */
	uint32_t sp_usr;
	uint32_t lr_usr;
	/* FIQ mode has R8_FIQ-R12_FIQ, R14_FIQ (lr), R13_FIQ(sp) and SPSR_FIQ */
	uint32_t r8_fiq;
	uint32_t r9_fiq;
	uint32_t r10_fiq;
	uint32_t r11_fiq;
	uint32_t r12_fiq;
	uint32_t sp_fiq;
	uint32_t lr_fiq;
	uint32_t spsr_fiq;
	/* IRQ mode has R13_IRQ(sp), R14_IRQ(lr) and SPSR_IRQ as banked registers */
	uint32_t sp_irq;
	uint32_t lr_irq;
	uint32_t spsr_irq;
	/* ABT mode has R13_ABT(sp), R14_ABT(lr) and SPSR_ABT as banked registers */
	uint32_t sp_abt;
	uint32_t lr_abt;
	uint32_t spsr_abt;
	/* UND mode has R13_UND(sp), R14_UND(lr) and SPSR_UND as banked registers */
	uint32_t sp_und;
	uint32_t lr_und;
	uint32_t spsr_und;
};

struct dump_header {
	uint32_t	magic;
	uint32_t	len;
};

struct dump_data {
	struct dump_header	h;
	uint32_t	*data;
	uint32_t	magic;
};

extern struct arm_mode_regs_s cpu_ctxt_regs;	/* Global variable to store the register context */
extern volatile uint32_t *current_regs;

#ifdef CONFIG_BOARD_CRASHDUMP
static int artik05x_dump_write(uint32_t addr, struct dump_data *data)
{

	s5j_direct_write(addr, &(data->h), sizeof(struct dump_header));
	addr += sizeof(struct dump_header);
	s5j_direct_write(addr, data->data, data->h.len);
	addr += data->h.len;
	s5j_direct_write(addr, &data->magic, CRASHDUMP_MAGIC_LEN);
	addr += CRASHDUMP_MAGIC_LEN;

	return addr;
}

void board_crashdump(uint32_t currentsp, void *tcb, uint8_t *filename, int lineno)
{
	struct dump_data data;
	uint32_t addr;

	s5j_watchdog_disable();

	lldbg("Erase Flash for RAM dump!!\n");
	s5j_direct_erase(CRASHDUMP_PARTITION_START_ADDR, CRASHDUMP_PARTION_SIZE);

	/*RAM dump to Flash */
	addr = CRASHDUMO_START_ADDR;
	lldbg("RAM DUMP to Flash....\n");
	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	data.h.len = CONFIG_RAM_SIZE;
	data.data = (uint32_t *) CONFIG_RAM_START;
	addr = artik05x_dump_write(addr, &data);

	/*Register dump to Flash */
	lldbg("REG DUMP to Flash....\n");
	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	if (current_regs) {
		data.h.len = REG_R15 * 4;
		data.data = (uint32_t *)current_regs;
	} else {
		data.h.len = sizeof(struct arm_mode_regs_s);
		data.data = (uint32_t *) &cpu_ctxt_regs;
	}
	addr = artik05x_dump_write(addr, &data);

	lldbg("Crash dump done\n");
#ifdef CONFIG_S5J_WATCHDOG
	s5j_watchdog_enable();
#endif
}
#endif
