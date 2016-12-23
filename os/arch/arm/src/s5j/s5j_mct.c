/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_mct.c
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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

#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>

#include <arch/chip/chip_types.h>
#include <arch/irq.h>
#include <chip.h>
#include <tinyara/clock.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static u32 mct_local_get_int_cnt(int mct_id);
static u32 mct_local_get_tick_cnt(int mct_id);
u32 mct_local_get_frc_cnt(int mct_id);
static cycle_t usec2ticks(u64 usec);
//static bool mct_is_global(int mct_id);
//static void mct_global_clear_int(int mct_id);
//static void mct_global_disable_auto_incr(int mct_id);
//static bool mct_is_global(int mct_id);
//static void mct_global_clear_int(int mct_id);
//static void mct_global_disable_auto_incr(int mct_id);

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CFG                (0x000)
#define G_CNT_L            (0x100)
#define G_CNT_U            (0x104)
#define G_CNT_WSTAT        (0x110)
#define G_COMP_L(x)        (0x200 + (0x10 * x))
#define G_COMP_U(x)        (0x204 + (0x10 * x))
#define G_COMP_ADD_INCR(x) (0x208 + (0x10 * x))
#define G_TCON             (0x240)
#define G_INT_CSTAT        (0x244)
#define G_INT_ENB          (0x248)
#define G_WSTAT            (0x24C)

#define L_BASE(x)     (0x300 + (0x100 * x))
#define L_MASK        (0xffffff00)

#define L_TCNTB(x)      (L_BASE(x) + 0x00)
#define L_TCNTO(x)      (L_BASE(x) + 0x04)
#define L_ICNTB(x)      (L_BASE(x) + 0x08)
#define L_ICNTO(x)      (L_BASE(x) + 0x0C)
#define L_FRCNTB(x)     (L_BASE(x) + 0x10)
#define L_FRCNTO(x)     (L_BASE(x) + 0x14)
#define L_TCON(x)       (L_BASE(x) + 0x20)
#define L_INT_CSTAT(x)  (L_BASE(x) + 0x30)
#define L_INT_ENB(x)    (L_BASE(x) + 0x34)
#define L_WSTAT(x)      (L_BASE(x) + 0x40)

#define G_TCON_START_FRC      (1 << 8)

#define G_TCON_AUTO_INCR_ENABLE(x) (1 << ((2 * x) + 1))
#define G_TCON_COMP_ENABLE(x)      (1 << ((2 * x) + 0))

#define L_TCON_FRC_START      (1 << 3)
#define L_TCON_INTERVAL_MODE  (1 << 2)
#define L_TCON_INT_START      (1 << 1)
#define L_TCON_TIMER_START    (1 << 0)

#define L_INTR_FRC          (1 << 1)
#define L_INTR_ICNT         (1 << 0)

#define WSTAT_G_CNT_U       (1 << 1)
#define WSTAT_G_CNT_L       (1 << 0)

#define WSTAT_G_COMP_ADD_INCR(x) (1 << ((x * 4) + 2))
#define WSTAT_G_COMP_U(x)        (1 << ((x * 4) + 1))
#define WSTAT_G_COMP_L(x)        (1 << ((x * 4) + 0))

#define WSTAT_G_TCON        (1 << 16)

#define WSTAT_L_TCON        (1 << 3)
#define WSTAT_L_FRCCNTB     (1 << 2)
#define WSTAT_L_ICNTB       (1 << 1)
#define WSTAT_L_TCNTB       (1 << 0)

#define mct_check_wstat(wstat, offset) \
	do { \
		if (__raw_readl(mct_base + wstat) & offset) { \
			__raw_writel(offset, mct_base + wstat); \
			break; \
		} \
	} while (1)

#define MCT_DRIVER_FROM_EXCITE

/****************************************************************************
 * Private Types
 ****************************************************************************/
enum {
	MCT_GLOBAL,
	MCT_LOCAL,
};

struct mct_desc {
	char *name;
	int offset;
	unsigned irq_id;
	mct_cb_t callback;
	void *param;
};

struct mct_desc mct_device[] = {
	/* global */
	[MCT_G0] = {
		.name = "mct-g.0",
		.irq_id = IRQ_MCT_G0,
	},
	[MCT_G1] = {
		.name = "mct-g.1",
		.irq_id = IRQ_MCT_G1,
	},
	[MCT_G2] = {
		.name = "mct-g.2",
		.irq_id = IRQ_MCT_G2,
	},
	[MCT_G3] = {
		.name = "mct-g.3",
		.irq_id = IRQ_MCT_G3,
	},
	/* local */
	[MCT_L0] = {
		.name = "mct-l.0",
		.irq_id = IRQ_MCT_L0,
	},
	[MCT_L1] = {
		.name = "mct-l.1",
		.irq_id = IRQ_MCT_L1,
	},
	[MCT_L2] = {
		.name = "mct-l.2",
		.irq_id = IRQ_MCT_L2,
	},
	[MCT_L3] = {
		.name = "mct-l.3",
		.irq_id = IRQ_MCT_L3,
	},
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static volatile bool bmct_init;
static const u32 clk_rate = CONFIG_FIN_HZ;
static u32 divided_clk_rate = 1;

/****************************************************************************
 * Public Data
 ****************************************************************************/
void *mct_base = (void *)MCT0_BASE;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int mct_get_offset_by_id(int mct_id)
{
	char *p;
	int i;
	struct mct_desc *mct = &mct_device[mct_id];

	/*  Assert(mct_id >= 0 && mct_id < MCT_END); */

	for (i = 0, p = &mct->name[0]; i < strlen(mct->name); i++, p++)
		if (*p == '.') {
			break;
		}

	return (int)(*(++p) - '0');
}

static int mct_get_type_by_id(int mct_id)
{
	Assert(mct_id >= 0 && mct_id < MCT_END);

	if (mct_id < MCT_L0) {
		return MCT_GLOBAL;
	} else {
		return MCT_LOCAL;
	}

	return -EINVAL;
}

static int mct_get_id_by_name(char *name)
{
	struct mct_desc *mct;
	int i;

	for (i = 0; i < ARRAY_SIZE(mct_device); i++) {
		mct = &mct_device[i];
		if (!strcmp(mct->name, name)) {
			return i;
		}
	}

	return -EINVAL;
}

static bool mct_is_valid(int mct_id)
{
	return (mct_id >= 0 && mct_id < MCT_END) ? true : false;
}

static void mct_global_disable_int(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + G_INT_ENB);
	reg &= ~(1 << offset);
	__raw_writel(reg, mct_base + G_INT_ENB);
}

static void mct_local_enable_int(int mct_id, int flag)
{
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	offset = mct_get_offset_by_id(mct_id);

	if (flag & 0xFFFFFFFC) {
		lldbg("invalid mct local interrupt flag\n");
		goto out;
	}

	__raw_writel(flag, mct_base + L_INT_ENB(offset));
out:
	return;
}

static void mct_local_disable_int(int mct_id, int flag)
{
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	offset = mct_get_offset_by_id(mct_id);

	if (flag & 0xFFFFFFFC) {
		lldbg("invalid mct local interrupt flag\n");
		goto out;
	}

	__raw_writel(flag, mct_base + L_INT_ENB(offset));

out:
	return;
}

static u64 mct_global_get_comp_cnt(int mct_id)
{
	int offset;
	u32 low, high;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	offset = mct_get_offset_by_id(mct_id);

	low = __raw_readl(mct_base + G_COMP_L(offset));
	high = __raw_readl(mct_base + G_COMP_U(offset));

	return (u64)(((u64) high << 32) | low);
}

static u32 mct_global_get_auto_incr(int mct_id)
{
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	offset = mct_get_offset_by_id(mct_id);

	return (u32)(__raw_readl(mct_base + G_COMP_ADD_INCR(offset)));
}

static void mct_global_set_auto_incr(int mct_id, u32 count)
{
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	if (count == mct_global_get_auto_incr(mct_id)) {
		return;
	}

	offset = mct_get_offset_by_id(mct_id);

	__raw_writel(count, mct_base + G_COMP_ADD_INCR(offset));
	mct_check_wstat(G_WSTAT, WSTAT_G_COMP_ADD_INCR(offset));
}

static void mct_local_set_int_cnt(int mct_id, u32 count)
{
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	if (count == mct_local_get_int_cnt(mct_id)) {
		return;
	}

	offset = mct_get_offset_by_id(mct_id);

	if (count >> 31) {
		lldbg("invalid count value: count[31] used for a manual update bit.\n");
		goto out;
	}

	/* set a manual update bit (ICNTB[31]) */
	count |= 1 << 31;
	__raw_writel(count, mct_base + L_ICNTB(offset));
	mct_check_wstat(L_WSTAT(offset), WSTAT_L_ICNTB);
out:
	return;
}

static void mct_local_set_tick_cnt(int mct_id, u32 count)
{
	int offset;

	if (count == mct_local_get_tick_cnt(mct_id)) {
		return;
	}

	offset = mct_get_offset_by_id(mct_id);

	__raw_writel(count, mct_base + L_TCNTB(offset));
	mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCNTB);
}

static u32 mct_local_get_int_cnt(int mct_id)
{
	int offset;

	offset = mct_get_offset_by_id(mct_id);

	return (u32)(__raw_readl(mct_base + L_ICNTO(offset)) & 0x7FFFFFFF);
}

static u32 mct_local_get_tick_cnt(int mct_id)
{
	int offset;

	offset = mct_get_offset_by_id(mct_id);

	return (u32)__raw_readl(mct_base + L_TCNTO(offset));
}

u32 mct_local_get_frc_cnt(int mct_id)
{
	int offset;

	offset = mct_get_offset_by_id(mct_id);

	return (u32)__raw_readl(mct_base + L_FRCNTO(offset));
}

static bool mct_ispending(int mct_id)
{
	uregister reg;
	int offset;

	offset = mct_get_offset_by_id(mct_id);

	if (mct_get_type_by_id(mct_id) == MCT_LOCAL) {
		reg = __raw_readl(mct_base + L_INT_CSTAT(offset)) & 0x3;
	} else {
		reg = __raw_readl(mct_base + G_INT_CSTAT) & (1 << offset);
	}

	if (!reg) {
		return false;
	} else {
		return true;
	}
}

static void mct_global_start_frc(void)
{
	uregister reg;

	reg = __raw_readl(mct_base + G_TCON);
	reg |= (1 << 8);
	__raw_writel(reg, mct_base + G_TCON);
	mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
}

static void mct_global_stop_frc(void)
{
	uregister reg;

	reg = __raw_readl(mct_base + G_TCON);
	if (reg & G_TCON_START_FRC) {
		reg &= ~(G_TCON_START_FRC);
		__raw_writel(reg, mct_base + G_TCON);
		mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
	}
}

static void mct_global_set_frc_cnt(cycle_t count)
{
	u32 low;
	u32 high;

	if (count == mct_global_get_frc_cnt()) {
		return;
	}

	high = (u32)((count >> 32) & 0xFFFFFFFF);
	low = (u32)(count & 0xFFFFFFFF);

	__raw_writel(high, mct_base + G_CNT_U);
	mct_check_wstat(G_CNT_WSTAT, WSTAT_G_CNT_U);

	__raw_writel(low, mct_base + G_CNT_L);
	mct_check_wstat(G_CNT_WSTAT, WSTAT_G_CNT_L);
}

static cycle_t mct_get_timer(cycle_t start)
{
	return mct_global_get_frc_cnt() - start;
}

static void mct_udelay(u64 usec)
{
	cycle_t end = mct_get_timer(0) + usec2ticks(usec);
	while (mct_get_timer(0) < end) ;
}

static void mct_clk_rate(u32 prescaler, eMCT_DIVIDER divider)
{
	u32 reg;

	divided_clk_rate = clk_rate / (prescaler + 1) / (1 << divider);

	reg = __raw_readl(mct_base + CFG);
	reg = (reg & ~(0xFF << 0)) | (prescaler << 0);
	reg = (reg & ~(0x7 << 8)) | (divider << 8);
	__raw_writel(reg, mct_base + CFG);
}

static cycle_t usec2ticks(u64 usec)
{
	return (usec * divided_clk_rate) / USEC_PER_SEC;
}

static u64 ticks2usec(cycle_t tick)
{
	return (tick * USEC_PER_SEC) / divided_clk_rate;
}

static void mct_local_set_interval(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (!(reg & L_TCON_INTERVAL_MODE)) {
		reg |= L_TCON_INTERVAL_MODE;
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

static void mct_local_clear_interval(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (reg & L_TCON_INTERVAL_MODE) {
		reg &= ~(L_TCON_INTERVAL_MODE);
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

static void mct_local_start_int(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (!(reg & L_TCON_INT_START)) {
		reg |= L_TCON_INT_START;
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

static void mct_local_stop_int(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (reg & L_TCON_INT_START) {
		reg &= ~(L_TCON_INT_START);
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

static void mct_local_start_timer(int mct_id)
{
	uregister reg;
	int offset;

	/*  Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL); */

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (!(reg & L_TCON_TIMER_START)) {
		reg |= L_TCON_TIMER_START;
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

static void mct_local_stop_timer(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_LOCAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (reg & L_TCON_TIMER_START) {
		reg &= ~(L_TCON_TIMER_START);
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

static void mct_set_callback(int mct_id, mct_cb_t mct_cb, void *param)
{
	struct mct_desc *mct = &mct_device[mct_id];

	mct->callback = mct_cb;
	mct->param = param;
}

static void mct_handler(int mct_id)
{
	/*  struct mct_desc *mct = &mct_device[mct_id]; */
	mct_clear_pending(mct_id);
}

static __irq void mct_isr_gt0_handler(void)
{
	mct_handler(MCT_G0);
}

static __irq void mct_isr_gt1_handler(void)
{
	mct_handler(MCT_G1);
}

static __irq void mct_isr_gt2_handler(void)
{
	mct_handler(MCT_G2);
}

static __irq void mct_isr_gt3_handler(void)
{
	mct_handler(MCT_G3);
}

static void mct_reset(int mct_id)
{
	int type = mct_get_type_by_id(mct_id);

	mct_clear_pending(mct_id);

	if (type == MCT_GLOBAL) {
		mct_global_disable_comp(mct_id);
		mct_global_disable_int(mct_id);
		mct_global_set_auto_incr(mct_id, 0);
		mct_global_set_comp_cnt(mct_id, 0);
	} else if (type == MCT_LOCAL) {
		mct_local_disable_int(mct_id, (L_INTR_FRC | L_INTR_ICNT));
		mct_local_stop_frc(mct_id);
		mct_local_stop_timer(mct_id);
		mct_local_stop_int(mct_id);
		mct_local_clear_interval(mct_id);
		mct_local_set_tick_cnt(mct_id, 0);
		mct_local_set_int_cnt(mct_id, 0);
	}

	mct_set_callback(mct_id, NULL, NULL);
}

static int mct_open(char *name)
{
	int mct_id;

	mct_id = mct_get_id_by_name(name);

	if (!mct_is_valid(mct_id)) {
		lldbg("invalid mct id\n");
		return -1;
	}

	mct_reset(mct_id);

	return mct_id;
}

static void mct_close(int mct_id)
{
	mct_reset(mct_id);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
 *
 * @brief    enable mctX timer function, call with mct_id will enabel mctX
 * @param    int mct_id  : channel number of MCT
 * @return
 * @note     note here
 */
void mct_global_enable_int(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + G_INT_ENB);
	reg |= 1 << offset;
	__raw_writel(reg, mct_base + G_INT_ENB);
}

/**
 *
 * @brief    mct has globan and local timer, this function enable local timer0 function
 * @return
 * @note     note here
 */
void mct_local_start_timer0(void)
{
	int mct = MCT_L0;
	int tick = (26 * USEC_PER_TICK);	/*  Fin*TargetPeriod  */
	int icnt = 0;
	mct_local_enable_int(mct, L_INTR_ICNT);
	mct_local_set_tick_cnt(mct, tick);
	mct_local_set_int_cnt(mct, icnt);

	mct_local_set_interval(mct);
	mct_local_start_int(mct);
	mct_local_start_timer(mct);
}

/**
 *
 * @brief    Initialize global timer for tick
 * @return
 * @note     note here
 */
void mct_init(void)
{

#ifndef TEMP_TEST
	if (bmct_init == true) {
		return;
	}
#endif

	mct_base = (void *)MCT0_BASE;
	mct_global_set_frc_cnt(0);
	mct_global_start_frc();

	bmct_init = true;
}

/**
 *
 * @brief    set global timer comparation value, MCT will set interrupt if reach to timer count. FRC in global timer is an up-counter. After it starts, it increments until 64'hffff_ffff_ffff_ffff and returns to 0. Comparator continuously see the value. When it matches with two values, interrupt occurs.
 * @param    int mct_id : channel of mct
 * @param    u64 count : 64 bit comparation value
 * @note     note here
 */
void mct_global_set_comp_cnt(int mct_id, u64 count)
{
	int offset;
	u32 low;
	u32 high;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	if (count == mct_global_get_comp_cnt(mct_id)) {
		return;
	}

	offset = mct_get_offset_by_id(mct_id);

	low = (u32)(count & 0xFFFFFFFF);
	high = (u32)((count >> 32) & 0xFFFFFFFF);

	__raw_writel(low, mct_base + G_COMP_L(offset));
	mct_check_wstat(G_WSTAT, WSTAT_G_COMP_L(offset));
	__raw_writel(high, mct_base + G_COMP_U(offset));
	mct_check_wstat(G_WSTAT, WSTAT_G_COMP_U(offset));
}

/**
 *
 * @brief    return value of free running counter
 * @return   cycle_t count : 64bit value of FRC
 * @note     note here
 */
cycle_t mct_global_get_frc_cnt(void)
{
	cycle_t count;
	u32 low;
	u32 high;
	u32 high1;

	high = __raw_readl(mct_base + G_CNT_U);
	low = __raw_readl(mct_base + G_CNT_L);
	high1 = __raw_readl(mct_base + G_CNT_U);
	if (high != high1) {
		low = __raw_readl(mct_base + G_CNT_L);
	}

	count = (cycle_t) low;
	if (sizeof(cycle_t) > sizeof(u32)) {
		count |= ((cycle_t)high1 << 32);
	}

	return count;
}

/**
 *
 * @brief    disable the comparison between the global FRC and the 64-bit comparaton in MCT channel.
 * @param    int mct_id : channel of MCT
 * @note     note here
 */
void mct_global_disable_comp(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + G_TCON);
	if (reg & G_TCON_COMP_ENABLE(offset)) {
		reg &= ~(1 << (offset * 2));
		__raw_writel(reg, mct_base + G_TCON);
		mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
	}
}

/**
 *
 * @brief    ensable the comparison between the global FRC and the 64-bit comparaton in MCT channel.
 * @param    int mct_id : channel of MCT
 * @note     note here
 */
void mct_global_enable_comp(int mct_id)
{
	uregister reg;
	int offset;

	Assert(mct_get_type_by_id(mct_id) == MCT_GLOBAL);

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + G_TCON);
	if (!(reg & G_TCON_COMP_ENABLE(offset))) {
		reg |= G_TCON_COMP_ENABLE(offset);
		__raw_writel(reg, mct_base + G_TCON);
		mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
	}
}

/**
 *
 * @brief    when FRC in global timer are matched with comparation value, interrupt is asserted. this function will clear interupt pending register in MCT.
 * @param    int mct_id : channel of MCT
 * @note     note here
 */
void mct_clear_pending(int mct_id)
{
	int offset;

	offset = mct_get_offset_by_id(mct_id);

	if (mct_get_type_by_id(mct_id) == MCT_LOCAL) {
		__raw_writel(3, mct_base + L_INT_CSTAT(offset));
	} else if (mct_get_type_by_id(mct_id) == MCT_GLOBAL) {
		__raw_writel(1 << offset, mct_base + G_INT_CSTAT);
	}
}

void mct_local_start_frc(int mct_id)
{
	uregister reg;
	int offset;

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (!(reg & L_TCON_FRC_START)) {
		reg |= L_TCON_FRC_START;
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

void mct_local_stop_frc(int mct_id)
{
	uregister reg;
	int offset;

	offset = mct_get_offset_by_id(mct_id);

	reg = __raw_readl(mct_base + L_TCON(offset));
	if (reg & L_TCON_FRC_START) {
		reg &= ~(L_TCON_FRC_START);
		__raw_writel(reg, mct_base + L_TCON(offset));
		mct_check_wstat(L_WSTAT(offset), WSTAT_L_TCON);
	}
}

__irq void mct_isr_lt0_handler(void)
{
	mct_handler(MCT_L0);
}

__irq void mct_isr_lt1_handler(void)
{
	mct_handler(MCT_L1);
}

__irq void mct_isr_lt2_handler(void)
{
	mct_handler(MCT_L2);
}

__irq void mct_isr_lt3_handler(void)
{
	mct_handler(MCT_L3);
}

void mct_local_start_frc_all(void)
{
	mct_local_start_frc(0);
	irq_attach(IRQ_MCT_L0, (xcpt_t) mct_isr_lt0_handler);
	/* Enable SysTick interrupts */
	up_enable_irq(IRQ_MCT_L0);

}
