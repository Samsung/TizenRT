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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CFG				(0x000)
#define G_CNT_L				(0x100)
#define G_CNT_U				(0x104)
#define G_CNT_WSTAT			(0x110)
#define G_COMP_L(x)			(x)
#define G_COMP_U(x)			(4 + x)
#define G_COMP_ADD_INCR(x)		(8 + x)
#define G_TCON				(0x240)
#define G_INT_CSTAT			(0x244)
#define G_INT_ENB			(0x248)
#define G_WSTAT				(0x24C)

#define L_MASK				(0xffffff00)

#define L_TCNTB(x)			(x + 0x00)
#define L_TCNTO(x)			(x + 0x04)
#define L_ICNTB(x)			(x + 0x08)
#define L_ICNTO(x)			(x + 0x0C)
#define L_FRCNTB(x)			(x + 0x10)
#define L_FRCNTO(x)			(x + 0x14)
#define L_TCON(x)			(x + 0x20)
#define L_INT_CSTAT(x)			(x + 0x30)
#define L_INT_ENB(x)			(x + 0x34)
#define L_WSTAT(x)			(x + 0x40)

#define G_TCON_START_FRC		(1 << 8)

#define G_TCON_AUTO_INCR_ENABLE(x)	(1 << ((2 * x) + 1))
#define G_TCON_COMP_ENABLE(x)		(1 << ((2 * x) + 0))

#define L_TCON_FRC_START		(1 << 3)
#define L_TCON_INTERVAL_MODE		(1 << 2)
#define L_TCON_INT_START		(1 << 1)
#define L_TCON_TIMER_START		(1 << 0)

#define WSTAT_G_CNT_U			(1 << 1)
#define WSTAT_G_CNT_L			(1 << 0)

#define WSTAT_G_COMP_ADD_INCR(x)	(1 << ((x * 4) + 2))
#define WSTAT_G_COMP_U(x)		(1 << ((x * 4) + 1))
#define WSTAT_G_COMP_L(x)		(1 << ((x * 4) + 0))

#define WSTAT_G_TCON			(1 << 16)

#define WSTAT_L_TCON			(1 << 3)
#define WSTAT_L_FRCCNTB			(1 << 2)
#define WSTAT_L_ICNTB			(1 << 1)
#define WSTAT_L_TCNTB			(1 << 0)

#define G_INT_OFFS(x)			((x>>4)&0xf)

#define mct_check_wstat(wstat, mct_id) \
do { \
	if (__raw_readl(mct_base + wstat) & mct_id) { \
		__raw_writel(mct_id, mct_base + wstat); \
		break; \
	} \
} while (1)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static volatile bool bmct_init;
static const u32 clk_rate = CONFIG_FIN_HZ;
static u32 divided_clk_rate = 1;
void *mct_base = (void *)MCT0_BASE;

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
* Name: s5j_mct_local_int_ctrl
*
* Description:
*   Local timer interrupt control.
*
* Input Parameters:
*   mct_id - mct timer ID
*   flag - interrupt enable/disable: LINT_NONE, LINT_INT, LINT_FRC.
*
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_int_ctrl(MCT_CHANNEL mct_id, MCT_LINT flag)
{
	Assert(mct_id >= MCT_L0);
	__raw_writel(flag, mct_base + L_INT_ENB(mct_id));
}

/****************************************************************************
* Name: s5j_mct_global_get_cmp_cnt
*
* Description:
*   Get Global timer channel current compare value.
*
* Input Parameters:
*   mct_id - mct timer ID
*
*
* Returned Value:
*   Compare value
*
****************************************************************************/
u64 s5j_mct_global_get_comp_cnt(MCT_CHANNEL mct_id)
{
	u32 low, high;

	Assert(mct_id <= MCT_G3);

	low = __raw_readl(mct_base + G_COMP_L(mct_id));
	high = __raw_readl(mct_base + G_COMP_U(mct_id));

	return (u64)(((u64) high << 32) | low);
}

/****************************************************************************
* Name: s5j_mct_global_get_auto_incr
*
* Description:
*   Get Global timer channel auto increment value.
*
* Input Parameters:
*   mct_id - mct timer ID
*
*
* Returned Value:
*   Increment value
*
****************************************************************************/
u32 s5j_mct_global_get_auto_incr(MCT_CHANNEL mct_id)
{
	Assert(mct_id <= MCT_G3);
	return (u32)(__raw_readl(mct_base + G_COMP_ADD_INCR(mct_id)));
}

/****************************************************************************
* Name: s5j_mct_global_set_auto_incr
*
* Description:
*   Global timer channel compare value auto increment set.
*
* Input Parameters:
*   mct_id - mct timer ID
*   count - increment value
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_global_set_auto_incr(MCT_CHANNEL mct_id, u32 count)
{
	Assert(mct_id <= MCT_G3);

	if (count == s5j_mct_global_get_auto_incr(mct_id)) {
		return;
	}

	__raw_writel(count, mct_base + G_COMP_ADD_INCR(mct_id));
	mct_check_wstat(G_WSTAT, WSTAT_G_COMP_ADD_INCR(mct_id));
}

/****************************************************************************
* Name: s5j_mct_local_set_int_cnt_auto
*
* Description:
*   Set Local interrupt counter value after counter reaches 0 value.
*
* Input Parameters:
*   mct_id - mct timer ID
*   count - initial value
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_set_int_cnt_auto(MCT_CHANNEL mct_id, u32 count)
{
	Assert(mct_id >= MCT_L0);
	/* clear manual update bit (ICNTB[31]) */
	count &= ~(1 << 31);
	__raw_writel(count, mct_base + L_ICNTB(mct_id));
	mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_ICNTB);
}

/****************************************************************************
* Name: s5j_mct_local_set_int_cnt_manual
*
* Description:
*   Set Local interrupt counter value immediately.
*
* Input Parameters:
*   mct_id - mct timer ID
*   count - initial value
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_set_int_cnt_manual(MCT_CHANNEL mct_id, u32 count)
{
	Assert(mct_id >= MCT_L0);
	/* set manual update bit (ICNTB[31]) */
	count |= 1 << 31;
	__raw_writel(count, mct_base + L_ICNTB(mct_id));
	mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_ICNTB);
}

/****************************************************************************
* Name: s5j_mct_local_set_tick_cnt
*
* Description:
*   Set Local TICK counter value
*
* Input Parameters:
*   mct_id - mct timer ID
*   count - initial value
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_set_tick_cnt(MCT_CHANNEL mct_id, u32 count)
{
	Assert(mct_id >= MCT_L0);
	__raw_writel(count, mct_base + L_TCNTB(mct_id));
	mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCNTB);
}

/****************************************************************************
* Name: s5j_mct_local_get_int_cnt
*
* Description:
*   Return current Local interrupt counter value
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   counter value.
*
****************************************************************************/
u32 s5j_mct_local_get_int_cnt(MCT_CHANNEL mct_id)
{
	Assert(mct_id >= MCT_L0);
	return (u32)(__raw_readl(mct_base + L_ICNTO(mct_id)) & 0x7FFFFFFF);
}

/****************************************************************************
* Name: s5j_mct_local_get_tick_cnt
*
* Description:
*   Return current Local TICK counter value
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   counter value.
*
****************************************************************************/
u32 s5j_mct_local_get_tick_cnt(MCT_CHANNEL mct_id)
{
	Assert(mct_id >= MCT_L0);
	return (u32) __raw_readl(mct_base + L_TCNTO(mct_id));
}

/****************************************************************************
* Name: s5j_mct_local_set_frc_cnt
*
* Description:
*   Set Local FRC counter value.
*
* Input Parameters:
*   mct_id - mct timer ID
*   count - value to set
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_set_frc_cnt(MCT_CHANNEL mct_id, u32 count)
{
	Assert(mct_id >= MCT_L0);
	__raw_writel(count, mct_base + L_FRCNTB(mct_id));
	mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_FRCCNTB);
}

/****************************************************************************
* Name: s5j_mct_local_get_frc_cnt
*
* Description:
*   Return current Local FRC counter value
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   counter value.
*
****************************************************************************/
u32 s5j_mct_local_get_frc_cnt(MCT_CHANNEL mct_id)
{
	Assert(mct_id >= MCT_L0);
	return (u32) __raw_readl(mct_base + L_FRCNTO(mct_id));
}

/****************************************************************************
* Name: s5j_mct_ispending
*
* Description:
*   Check if selected MCT interrupt is pending or not.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   true - pending, false - not pending.
*
****************************************************************************/
bool s5j_mct_ispending(MCT_CHANNEL mct_id)
{
	uregister reg;

	if (mct_id > MCT_G3) {
		reg = __raw_readl(mct_base + L_INT_CSTAT(mct_id)) & 3;
	} else {
		reg = __raw_readl(mct_base + G_INT_CSTAT) & (1 << G_INT_OFFS(mct_id));
	}

	if (!reg) {
		return false;
	} else {
		return true;
	}
}

/****************************************************************************
* Name: s5j_mct_global_start_frc
*
* Description:
*   Start global FRC counter
*
* Input Parameters:
*   None
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_global_start_frc(void)
{
	uregister reg;

	reg = __raw_readl(mct_base + G_TCON);
	reg |= (1 << 8);
	__raw_writel(reg, mct_base + G_TCON);
	mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
}

/****************************************************************************
* Name: s5j_mct_global_stop_frc
*
* Description:
*   Stop global FRC counter
*
* Input Parameters:
*   None
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_global_stop_frc(void)
{
	uregister reg;

	reg = __raw_readl(mct_base + G_TCON);
	if (reg & G_TCON_START_FRC) {
		reg &= ~(G_TCON_START_FRC);
		__raw_writel(reg, mct_base + G_TCON);
		mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_global_set_frc_cnt
*
* Description:
*   Preset initial value of global FRC
*
* Input Parameters:
*   count - initial value
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_global_set_frc_cnt(cycle_t count)
{
	u32 low, high;

	if (count == s5j_mct_global_get_frc_cnt()) {
		return;
	}

	high = (u32)((count >> 32) & 0xFFFFFFFF);
	low = (u32)(count & 0xFFFFFFFF);

	__raw_writel(high, mct_base + G_CNT_U);
	mct_check_wstat(G_CNT_WSTAT, WSTAT_G_CNT_U);

	__raw_writel(low, mct_base + G_CNT_L);
	mct_check_wstat(G_CNT_WSTAT, WSTAT_G_CNT_L);
}

/****************************************************************************
* Name: s5j_mct_clk_rate
*
* Description:
*   Set MCT prescaler/divider values.
*
* Input Parameters:
*   prescaler - precsaler value
*   divider - divider value
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_clk_rate(u32 prescaler, eMCT_DIVIDER divider)
{
	u32 reg;

	divided_clk_rate = clk_rate / (prescaler + 1) / (1 << divider);

	reg = __raw_readl(mct_base + CFG);
	reg = (reg & ~(0xFF << 0)) | (prescaler << 0);
	reg = (reg & ~(0x7 << 8)) | (divider << 8);
	__raw_writel(reg, mct_base + CFG);
}

/****************************************************************************
* Name: s5j_mct_local_set_interval
*
* Description:
*   Local timer enambe autoreload mode.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_set_interval(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id >= MCT_L0);

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (!(reg & L_TCON_INTERVAL_MODE)) {
		reg |= L_TCON_INTERVAL_MODE;
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_local_clear_interval
*
* Description:
*   Local timer disable autoreload mode.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_clear_interval(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id >= MCT_L0);

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (reg & L_TCON_INTERVAL_MODE) {
		reg &= ~(L_TCON_INTERVAL_MODE);
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_local_start_int
*
* Description:
*   Start Local timer interrupt counter.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_start_int(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id >= MCT_L0);

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (!(reg & L_TCON_INT_START)) {
		reg |= L_TCON_INT_START;
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_local_stop_int
*
* Description:
*   Stop Local timer interrupt counter.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_stop_int(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id >= MCT_L0);

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (reg & L_TCON_INT_START) {
		reg &= ~(L_TCON_INT_START);
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_local_start_timer
*
* Description:
*   Start Local timer tick counter.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_start_timer(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id >= MCT_L0);

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (!(reg & L_TCON_TIMER_START)) {
		reg |= L_TCON_TIMER_START;
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_local_stop_timer
*
* Description:
*   Stop Local timer tick counter.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_stop_timer(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id >= MCT_L0);

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (reg & L_TCON_TIMER_START) {
		reg &= ~(L_TCON_TIMER_START);
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_local_start_frc
*
* Description:
*   Start Local timer FRC counter.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_start_frc(MCT_CHANNEL mct_id)
{
	uregister reg;

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (!(reg & L_TCON_FRC_START)) {
		reg |= L_TCON_FRC_START;
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_local_stop_frc
*
* Description:
*   Stop Local timer FRC counter.
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_local_stop_frc(MCT_CHANNEL mct_id)
{
	uregister reg;

	reg = __raw_readl(mct_base + L_TCON(mct_id));
	if (reg & L_TCON_FRC_START) {
		reg &= ~(L_TCON_FRC_START);
		__raw_writel(reg, mct_base + L_TCON(mct_id));
		mct_check_wstat(L_WSTAT(mct_id), WSTAT_L_TCON);
	}
}

/****************************************************************************
* Name: s5j_mct_global_enable_int
*
* Description:
*   Enable interrupt for global counter channel
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_global_enable_int(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id <= MCT_G3);

	reg = __raw_readl(mct_base + G_INT_ENB);
	reg |= 1 << G_INT_OFFS(mct_id);
	__raw_writel(reg, mct_base + G_INT_ENB);
}

/****************************************************************************
* Name: s5j_mct_global_disable_int
*
* Description:
*   Disable interrupt for global counter channel
*
* Input Parameters:
*   mct_id - mct timer ID
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_global_disable_int(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id <= MCT_G3);

	reg = __raw_readl(mct_base + G_INT_ENB);
	reg &= ~(1 << G_INT_OFFS(mct_id));
	__raw_writel(reg, mct_base + G_INT_ENB);
}

/****************************************************************************
* Name: s5j_mct_global_set_comp_cnt
*
* Description:
*   Sets Compare value for global times channel
*
* Input Parameters:
*   mct_id - mct timer ID
*   count - 64 bit unsigned value for compare with FRC
*
* Returned Value:
*   None
*
****************************************************************************/
void s5j_mct_global_set_comp_cnt(MCT_CHANNEL mct_id, u64 count)
{
	u32 low, high;

	Assert(mct_id <= MCT_G3);

	if (count == s5j_mct_global_get_comp_cnt(mct_id)) {
		return;
	}

	low = (u32)(count & 0xFFFFFFFF);
	high = (u32)((count >> 32) & 0xFFFFFFFF);

	__raw_writel(low, mct_base + G_COMP_L(mct_id));
	mct_check_wstat(G_WSTAT, WSTAT_G_COMP_L(mct_id));
	__raw_writel(high, mct_base + G_COMP_U(mct_id));
	mct_check_wstat(G_WSTAT, WSTAT_G_COMP_U(mct_id));
}

/****************************************************************************
 * Name: s5j_mct_global_get_frc_cnt
 *
 * Description:
 *   Returns current Free Running Counter value
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   64 bit value of FRC
 *
 ****************************************************************************/

cycle_t s5j_mct_global_get_frc_cnt(void)
{
	cycle_t count;
	u32 low, high, high1;

	high = __raw_readl(mct_base + G_CNT_U);
	low = __raw_readl(mct_base + G_CNT_L);
	high1 = __raw_readl(mct_base + G_CNT_U);
	if (high != high1) {
		low = __raw_readl(mct_base + G_CNT_L);
	}

	count = (cycle_t) low;
	if (sizeof(cycle_t) > sizeof(u32)) {
		count |= ((cycle_t) high1 << 32);
	}

	return count;
}

/****************************************************************************
 * Name: s5j_mct_global_disable_comp
 *
 * Description:
 *   Disable compare operation for selected global timer
 *
 * Input Parameters:
 *   mct_id - mct timer ID
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void s5j_mct_global_disable_comp(MCT_CHANNEL mct_id)
{
	uregister reg;

	Assert(mct_id <= MCT_G3);

	reg = __raw_readl(mct_base + G_TCON);
	if (reg & G_TCON_COMP_ENABLE(mct_id)) {
		reg &= ~(1 << (G_INT_OFFS(mct_id) * 2));
		__raw_writel(reg, mct_base + G_TCON);
		mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
	}
}

/****************************************************************************
 * Name: s5j_mct_global_enable_comp
 *
 * Description:
 *   Enable compare operation for selected global timer
 *
 * Input Parameters:
 *   mct_id - mct timer ID
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void s5j_mct_global_enable_comp(MCT_CHANNEL mct_id)
{
	uregister reg;

	reg = __raw_readl(mct_base + G_TCON);
	if (!(reg & G_TCON_COMP_ENABLE(mct_id))) {
		reg |= G_TCON_COMP_ENABLE(mct_id);
		__raw_writel(reg, mct_base + G_TCON);
		mct_check_wstat(G_WSTAT, WSTAT_G_TCON);
	}
}

/****************************************************************************
 * Name: s5j_mct_reset
 *
 * Description:
 *   Stops operationof selected MCT timer.
 *
 * Input Parameters:
 *   mct_id - mct timer ID
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void s5j_mct_reset(MCT_CHANNEL mct_id)
{

	s5j_mct_clear_pending(mct_id);

	if (mct_id <= MCT_G3) {
		s5j_mct_global_disable_comp(mct_id);
		s5j_mct_global_disable_int(mct_id);
		s5j_mct_global_set_auto_incr(mct_id, 0);
		s5j_mct_global_set_comp_cnt(mct_id, 0);
	} else {
		s5j_mct_local_int_ctrl(mct_id, LINT_NONE);
		s5j_mct_local_stop_frc(mct_id);
		s5j_mct_local_stop_timer(mct_id);
		s5j_mct_local_stop_int(mct_id);
		s5j_mct_local_clear_interval(mct_id);
		s5j_mct_local_set_tick_cnt(mct_id, 0);
		s5j_mct_local_set_int_cnt_manual(mct_id, 0);
	}
}

/****************************************************************************
 * Name: s5j_mct_clear_pending
 *
 * Description:
 *   Clear pending interript flag.
 *
 * Input Parameters:
 *   mct_id - mct timer ID
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void s5j_mct_clear_pending(MCT_CHANNEL mct_id)
{
	if (mct_id > MCT_G3) {
		__raw_writel(3, mct_base + L_INT_CSTAT(mct_id));
	} else {
		__raw_writel(1 << G_INT_OFFS(mct_id), mct_base + G_INT_CSTAT);
	}
}
