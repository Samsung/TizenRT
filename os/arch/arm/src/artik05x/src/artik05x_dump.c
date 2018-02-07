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
#include <tinyara/version.h>

#define CRASHDUMP_COMMON_START_MAGIC		0x4B545241   //ARTK
#define CRASHDUMP_COMMON_END_MAGIC			0x454E4F44   //DONE
#define CRASHDUMP_MAGIC_LEN					4

#define CRASHDUMP_PARTITION_START_ADDR		(CONFIG_ARTIK05X_OTA_BIN_ADDR + S5J_FLASH_PADDR)
#define CRASHDUMP_PARTION_SIZE				(CONFIG_ARTIK05X_OTA_BIN_SIZE)

#define CRASHDUMO_START_ADDR				(CRASHDUMP_PARTITION_START_ADDR + 16)

#define MAX_COMMIT_LEN	41
#define MAX_BUILD_TIME	20
#define MAX_FILE_NAME	32


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

enum dump_flags_t {
	PRESENT_DUMP_INFO	= 0x01,
	PRESENT_BIN_INFO	= 0x02,
	PRESENT_REGS		= 0x04,
	PRESENT_STACK		= 0x08,
	PRESENT_TCB			= 0x10,
	PRESENT_RAM			= 0x20,
};

struct dump_header_s {
	uint32_t	magic;
	uint32_t	dump_flag;
	uint32_t	len;
};

struct dump_data_s {
	struct dump_header_s	h;
	uint32_t	*data;
	uint32_t	magic;
};

struct dump_info_s {
	uint32_t flag;
	uint32_t dfar;
	uint32_t dfsr;
};

struct dump_bin_info_s {
	char commit[MAX_COMMIT_LEN];
	char buildtime[MAX_BUILD_TIME];
	char file_name[MAX_FILE_NAME];
	int lineno;
} __attribute__ ((packed));

struct dump_task_info_s {
	pid_t		pid;
	char		name[CONFIG_TASK_NAME_SIZE + 1];
	uint32_t	sched_priority;
#ifdef CONFIG_PRIORITY_INHERITANCE
	uint8_t		base_priority;
#endif
	uint8_t		task_state;
	int32_t		errno_;
	uint32_t	stack_size;
	uint32_t	stack_used;
} __attribute__ ((packed));

#ifdef CONFIG_ARTIK05X_CRASHDUMP_REG
extern struct arm_mode_regs_s cpu_ctxt_regs;	/* Global variable to store the register context */
extern volatile uint32_t *current_regs;
#endif

#ifdef CONFIG_BOARD_CRASHDUMP
extern uint32_t g_dfar;
extern uint32_t g_dfsr;
#endif

#ifdef CONFIG_BOARD_CRASHDUMP
#ifdef CONFIG_ARTIK05X_CRASHDUMP_TCB
static void count_tcb(FAR struct tcb_s *tcb, FAR void *arg)
{
	uint32_t *count  = (uint32_t *)(arg);

	(*count)++;
}
static void write_tcb(FAR struct tcb_s *tcb, FAR void *arg)
{
	uint32_t *addr = (uint32_t *)arg;
	struct dump_task_info_s tinfo;

	memset(tinfo.name, 0, sizeof(tinfo.name));

	tinfo.pid = tcb->pid;
	snprintf(tinfo.name, CONFIG_TASK_NAME_SIZE, "%s", tcb->name);
	tinfo.sched_priority = tcb->sched_priority;
#ifdef CONFIG_PRIORITY_INHERITANCE
	tinfo.base_priority = tcb->base_priority;
#endif
	tinfo.task_state = tcb->task_state;
	tinfo.stack_size = (uint32_t)(tcb->adj_stack_size);
	tinfo.stack_used = (uint32_t)up_check_tcbstack(tcb);
	tinfo.errno_ = tcb->pterrno;

	s5j_direct_write((uint32_t)(*addr), &tinfo, sizeof(struct dump_task_info_s));

	*addr += sizeof(struct dump_task_info_s);
}
#endif

static int artik05x_dump_write(uint32_t addr, struct dump_data_s *data)
{
	s5j_direct_write(addr, &(data->h), sizeof(struct dump_header_s));
	addr += sizeof(struct dump_header_s);
	s5j_direct_write(addr, data->data, data->h.len);
	addr += data->h.len;
	s5j_direct_write(addr, &data->magic, CRASHDUMP_MAGIC_LEN);
	addr += CRASHDUMP_MAGIC_LEN;

	return addr;
}

void board_crashdump(uint32_t currentsp, void *tcb, uint8_t *filename, int lineno)
{
	struct dump_data_s data;
	struct dump_info_s info;
	uint32_t addr;

	s5j_watchdog_disable();

	lldbg("Erase Flash for RAM dump!!\n");
	s5j_direct_erase(CRASHDUMP_PARTITION_START_ADDR, CRASHDUMP_PARTION_SIZE);

	lldbg("Crash Dump info\n");
	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	data.h.dump_flag = PRESENT_DUMP_INFO;

	info.dfar = g_dfar;
	info.dfsr = g_dfsr;

	info.flag = 0;
#ifdef CONFIG_ARTIK05X_CRASHDUMP_BININFO
	info.flag |= PRESENT_BIN_INFO;
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_REG
	info.flag |= PRESENT_REGS;
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_STACK
	info.flag |= PRESENT_STACK;
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_TCB
	info.flag |= PRESENT_TCB;
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_RAM
	info.flag |= PRESENT_RAM;
#endif

	data.h.len = sizeof(struct dump_info_s);

	/*Write Crash dump information */
	addr = CRASHDUMO_START_ADDR;
	data.magic = CRASHDUMP_COMMON_END_MAGIC;
	data.data  = (uint32_t *) &info;
	addr = artik05x_dump_write(addr, &data);

#ifdef CONFIG_ARTIK05X_CRASHDUMP_BININFO
	struct dump_bin_info_s bin_info;

	lldbg("Bininfo to Flash....\n");

	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	data.h.dump_flag = PRESENT_BIN_INFO;
	data.h.len = sizeof(struct dump_bin_info_s);
	data.data = (uint32_t *)&bin_info;

	memset(&bin_info, 0, sizeof(struct dump_bin_info_s));

	strncpy(bin_info.file_name, (FAR const char *)filename, sizeof(bin_info.file_name));
	snprintf(bin_info.commit, sizeof(bin_info.commit), "%s", CONFIG_VERSION_BUILD);
	snprintf(bin_info.buildtime, sizeof(bin_info.buildtime), "%s", CONFIG_VERSION_BUILD_TIME);
	bin_info.lineno = lineno;

	addr = artik05x_dump_write(addr, &data);
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_REG
	lldbg("REG DUMP to Flash....\n");
	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	data.h.dump_flag = PRESENT_REGS;

	if (current_regs) {
		data.h.len = (REG_CPSR + 1) * 4;
		data.data = (uint32_t *)current_regs;
	} else {
		data.h.len = sizeof(struct arm_mode_regs_s);
		data.data = (uint32_t *) &cpu_ctxt_regs;
	}

	addr = artik05x_dump_write(addr, &data);
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_STACK
	lldbg("Stack DUMP to Flash....\n");
	extern const uint32_t g_idle_topstack;

	struct tcb_s *rtcb = (struct tcb_s *)tcb;
	uint32_t ustackbase;
	uint32_t ustacksize;
	uint32_t sp;
#ifdef CONFIG_MPU_STACKGUARD
	uint32_t uguardsize = 0;
#endif

	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	data.h.dump_flag = PRESENT_STACK;

	if (rtcb->pid == 0) {
		ustackbase = g_idle_topstack - 4;
		ustacksize = CONFIG_IDLETHREAD_STACKSIZE;
	} else {
		ustackbase = (uint32_t)rtcb->adj_stack_ptr;
		ustacksize = (uint32_t)rtcb->adj_stack_size;
#ifdef CONFIG_MPU_STACKGUARD
		uguardsize = (uint32_t)rtcb->guard_size;
#endif
	}

	sp  = currentsp  & ~0x1f;
	data.h.len = 0;

#ifdef CONFIG_MPU_STACKGUARD
	if (sp > (ustackbase - ustacksize - uguardsize) && data.data < ustackbase) {
#else
	if (sp > ustackbase - ustacksize && sp < ustackbase) {
#endif
		data.h.len = ustackbase - sp;
		data.data = (uint32_t *)sp;
		addr = artik05x_dump_write(addr, &data);
	}
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_TCB
	uint32_t tcb_cnt = 0;

	lldbg("TCB DUMP to Flash....\n");
	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	data.h.dump_flag = PRESENT_TCB;

	sched_foreach(count_tcb, &tcb_cnt);

	data.h.len = tcb_cnt * sizeof(struct dump_task_info_s);

	s5j_direct_write(addr, &(data.h), sizeof(struct dump_header_s));
	addr += sizeof(struct dump_header_s);

	sched_foreach(write_tcb, &addr);

	s5j_direct_write(addr, &data.magic, CRASHDUMP_MAGIC_LEN);
	addr += CRASHDUMP_MAGIC_LEN;
#endif

#ifdef CONFIG_ARTIK05X_CRASHDUMP_RAM
	lldbg("RAM DUMP to Flash....\n");
	data.h.magic = CRASHDUMP_COMMON_START_MAGIC;
	data.h.dump_flag = PRESENT_RAM;
	data.h.len = CONFIG_RAM_SIZE;
	data.data = (uint32_t *) CONFIG_RAM_START;
	addr = artik05x_dump_write(addr, &data);
#endif

	lldbg("Crash dump done\n");
#ifdef CONFIG_S5J_WATCHDOG
	s5j_watchdog_enable();
#endif
}
#endif
