/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/clock.h>

#include <arch/board/board.h>

#include "chip.h"
#include "up_internal.h"
#include "up_arch.h"

#include "chip/s5jt200_mct.h"
#include "s5j_mct.h"
#include "s5j_clock.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint32_t mct_getreg32(FAR struct s5j_mct_priv_s *priv, uint32_t offset)
{
	return getreg32(priv->base_addr + offset);
}

static void mct_putreg32(FAR struct s5j_mct_priv_s *priv, uint32_t offset,
				uint32_t value)
{
	putreg32(value, priv->base_addr + offset);
}

static int s5j_mct_setclock(uint32_t freq)
{
	uint32_t tclkb = s5j_clk_get_rate(CLK_DFT_OSCCLK);
	uint32_t cfg = getreg32(S5J_MCT_CFG) & 0xFFFFF800;
	uint32_t prescaler = (tclkb / freq - 1);

	if (prescaler > 0xFF) {
		return -EPERM;
	}

	/* set value what are divide(1), and prescaler */
	putreg32(prescaler | cfg, S5J_MCT_CFG);

	return OK;
}

static void s5j_mct_wait_wstat(uint32_t base, uint32_t bitmask)
{
	uint32_t reg = base + S5J_MCT_L_WSTAT_OFFSET;

	while ((getreg32(reg) & bitmask) == 0) {
		/* poll */
	}

	putreg32(bitmask, reg);
}

static void s5j_mct_global_wait_wstat(uint32_t base, uint32_t wstat, uint32_t bitmask)
{
	uint32_t reg = base + wstat;

	while ((getreg32(reg) & bitmask) == 0) {
		/* poll */
	}

	putreg32(bitmask, reg);
}

/****************************************************************************
 * Name: s5j_mct_global_get_comp_cnt
 *
 * Description:
 *   Return current counter value of comparator
 *
 ****************************************************************************/

static u64 s5j_mct_global_get_comp_cnt(FAR struct s5j_mct_priv_s *priv, int mct_id, uint32_t gmct_offset)
{
	u32 low;
	u32 high;

	DEBUGASSERT(mct_id == S5J_MCT_GLOBAL);

	low = getreg32(priv->base_addr + S5J_MCT_G_COMP_L(gmct_offset));
	high = getreg32(priv->base_addr + S5J_MCT_G_COMP_U(gmct_offset));

	return (u64)(((u64)high << 32) | low);
}

/****************************************************************************
 * Name: s5j_mct_global_disable_int
 *
 * Description:
 *	Disable Global MCT interrupt.
 *
 ****************************************************************************/

static void s5j_mct_global_disable_int(FAR struct s5j_mct_priv_s *priv, uint32_t gmct_int)
{
	unsigned int reg;

	DEBUGASSERT(gmct_int >= MCT_G0 && gmct_int <= MCT_G3);

	reg = getreg32(priv->base_addr + S5J_MCT_G_INT_ENB);
	reg &= ~(1 << gmct_int);
	putreg32(reg, priv->base_addr + S5J_MCT_G_INT_ENB);
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
#ifdef CONFIG_S5J_GTIMER
static FAR struct s5j_mct_priv_s s5j_gmct_priv = {
	.base_addr = S5J_MCT_BASE,
	.irq_id    = IRQ_MCT_G0,
};
#endif

#ifdef CONFIG_S5J_TIMER0
static FAR struct s5j_mct_priv_s s5j_mct0_priv = {
	.base_addr = (S5J_MCT_BASE + 0x300),
	.irq_id    = IRQ_MCT_L0,
};
#endif

#ifdef CONFIG_S5J_TIMER1
static FAR struct s5j_mct_priv_s s5j_mct1_priv = {
	.base_addr = (S5J_MCT_BASE + 0x400),
	.irq_id    = IRQ_MCT_L1,
};
#endif

#ifdef CONFIG_S5J_TIMER2
static FAR struct s5j_mct_priv_s s5j_mct2_priv = {
	.base_addr = (S5J_MCT_BASE + 0x500),
	.irq_id    = IRQ_MCT_L2,
};
#endif

#ifdef CONFIG_S5J_TIMER3
static FAR struct s5j_mct_priv_s s5j_mct3_priv = {
	.base_addr = (S5J_MCT_BASE + 0x600),
	.irq_id    = IRQ_MCT_L3,
};
#endif

static bool g_mct_initialized;

/****************************************************************************
 * Pubic Functions
 ****************************************************************************/
/****************************************************************************
 * Name: s5j_mct_global_get_frc_cnt
 *
 * Description:
 *	Read all 64-bits of global free running counter.
 *	Returns the number of cycles in the global counter.
 *
 *	Input Parameters:
 *		priv - mct device structure
 *
 *	Returned Value:
 *		64-bit Global FRC value.
 *
 ****************************************************************************/

u64 s5j_mct_global_get_frc_cnt(FAR struct s5j_mct_priv_s *priv)
{
	u32 low;
	u32 high;
	u32 high1 = getreg32(priv->base_addr + S5J_MCT_G_CNT_U);

	do {
		high = high1;
		low = getreg32(priv->base_addr + S5J_MCT_G_CNT_L);
		high1 = getreg32(priv->base_addr + S5J_MCT_G_CNT_U);
	} while (high != high1);

	return sizeof(u64) > sizeof(u32) ? ((u64)high << 32) | low : (u64)low;
}

/****************************************************************************
 * Name: s5j_mct_global_set_frc_cnt
 *
 * Description:
 *	Set FRC counter value
 *
 *	Input Parameters:
 *		priv - mct device structure
 *		count - 64-bit count value
 *
 *	Return:
 *		None
 *
 ****************************************************************************/

void s5j_mct_global_set_frc_cnt(FAR struct s5j_mct_priv_s *priv, u64 count)
{
	u32 low;
	u32 high;

	if (count == s5j_mct_global_get_frc_cnt(priv)) {
		return;
	}

	high = (u32)((count >> 32) & 0xFFFFFFFF);
	low = (u32)(count & 0xFFFFFFFF);

	putreg32(high, priv->base_addr + S5J_MCT_G_CNT_U);
	s5j_mct_global_wait_wstat(priv->base_addr, S5J_MCT_G_CNT_WSTAT, WSTAT_G_CNT_U);

	putreg32(low, priv->base_addr + S5J_MCT_G_CNT_L);
	s5j_mct_global_wait_wstat(priv->base_addr, S5J_MCT_G_CNT_WSTAT, WSTAT_G_CNT_L);
}

/****************************************************************************
 * Name: s5j_mct_global_start_frc
 *
 * Description:
 *	Start Global Free-Running Counter
 *
 *	Input Parameters:
 *		priv - mct device structure
 *
 *	Return:
 *		None
 *
 ****************************************************************************/

void s5j_mct_global_start_frc(FAR struct s5j_mct_priv_s *priv)
{
	unsigned int reg;

	reg = getreg32(priv->base_addr + S5J_MCT_G_TCON);
	reg |= G_TCON_START_FRC;
	putreg32(reg, priv->base_addr + S5J_MCT_G_TCON);
	s5j_mct_global_wait_wstat(priv->base_addr, S5J_MCT_G_WSTAT, WSTAT_G_TCON);
}

/****************************************************************************
 * Name: s5j_mct_global_set_comp_cnt
 *
 * Description:
 *	Set global timer comparation value.
 *	MCT will set interrupt if reach to timer count.
 *	FRC in global timer is an up-counter.
 *	After it starts, it increments until 64'hffff_ffff_ffff_ffff and returns to 0.
 *
 *	Input Parameters:
 *		priv - mct device structure
 *		mct_id - global mct device id (S5J_MCT_GLOBAL)
 *		gmct_offset - offset to comparator counter register from the mct base address
 *		count - 64-bit count value
 *
 *	Return:
 *		None
 *
 ****************************************************************************/

void s5j_mct_global_set_comp_cnt(FAR struct s5j_mct_priv_s *priv, int mct_id, uint32_t gmct_offset, u64 count)
{
	u32 low;
	u32 high;

	DEBUGASSERT(mct_id == S5J_MCT_GLOBAL);

	if (count == s5j_mct_global_get_comp_cnt(priv, mct_id, gmct_offset)) {
		return;
	}

	low = (u32)(count & 0xFFFFFFFF);
	high = (u32)((count >> 32) & 0xFFFFFFFF);

	putreg32(low, priv->base_addr + S5J_MCT_G_COMP_L(gmct_offset));
	s5j_mct_global_wait_wstat(priv->base_addr, S5J_MCT_G_WSTAT,  WSTAT_G_COMP_L(gmct_offset));

	putreg32(high, priv->base_addr + S5J_MCT_G_COMP_U(gmct_offset));
	s5j_mct_global_wait_wstat(priv->base_addr, S5J_MCT_G_WSTAT, WSTAT_G_COMP_U(gmct_offset));
}

/****************************************************************************
 * Name: s5j_mct_global_disable_comp
 *
 * Description:
 *	Disable the comparison between the global FRC and the 64-bit comparaton in MCT channel.
 *
 *	Input Parameters:
 *		priv - mct device structure
 *		mct_id - global mct device id (S5J_MCT_GLOBAL)
 *		gmct_offset - offset to comparator enable field in control register
 *
 *	Return:
 *		None
 *
 ****************************************************************************/

void s5j_mct_global_disable_comp(FAR struct s5j_mct_priv_s *priv, int mct_id, uint32_t gmct_offset)
{
	unsigned int reg;

	DEBUGASSERT(mct_id == S5J_MCT_GLOBAL);

	reg = getreg32(priv->base_addr + S5J_MCT_G_TCON);
	if (reg & G_TCON_COMP_ENABLE(gmct_offset)) {
		reg &= ~(1 << (gmct_offset * 2));
		putreg32(reg, priv->base_addr + S5J_MCT_G_TCON);
		s5j_mct_global_wait_wstat(priv->base_addr, S5J_MCT_G_WSTAT, WSTAT_G_TCON);
	}
}

/****************************************************************************
 * Name: s5j_mct_global_enable_comp
 *
 * Description:
 *	Enable the comparison between the global FRC and the 64-bit comparaton in MCT channel.
 *
 *	Input Parameters:
 *		priv - mct device structure
 *		mct_id - global mct device id (S5J_MCT_GLOBAL)
 *		gmct_offset - offset to comparator enable field in control register
 *
 *	Return:
 *		None
 *
 ****************************************************************************/

void s5j_mct_global_enable_comp(FAR struct s5j_mct_priv_s *priv, int mct_id, uint32_t gmct_offset)
{
	unsigned int reg;

	DEBUGASSERT(mct_id == S5J_MCT_GLOBAL);

	reg = getreg32(priv->base_addr + S5J_MCT_G_TCON);
	if (!(reg & G_TCON_COMP_ENABLE(gmct_offset))) {
		reg |= G_TCON_COMP_ENABLE(gmct_offset);
		putreg32(reg, priv->base_addr + S5J_MCT_G_TCON);
		s5j_mct_global_wait_wstat(priv->base_addr, S5J_MCT_G_WSTAT, WSTAT_G_TCON);
	}
}

/****************************************************************************
 * Name: s5j_mct_clear_pending
 *
 * Description:
 *	Clear interupt pending register in MCT.
 *
 *	Input Parameters:
 *		priv - mct device structure
 *		mct_id - global mct device id (S5J_MCT_GLOBAL)
 *		gmct_offset - offset to interrupt status
 *
 *	Return:
 *		None
 *
 ****************************************************************************/

void s5j_mct_clear_pending(FAR struct s5j_mct_priv_s *priv, int mct_id, uint32_t gmct_offset)
{
	if (mct_id == S5J_MCT_GLOBAL) {
		putreg32(1 << gmct_offset, priv->base_addr + S5J_MCT_G_INT_CSTAT);
	}
}

/****************************************************************************
 * Name: s5j_mct_global_enable_int
 *
 * Description:
 *	Enable Global MCT interrupt.
 *
 *	Input Parameters:
 *		priv - mct device structure
 *		mct_id - global mct interrupt number
 *
 *	Return:
 *		None
 *
 ****************************************************************************/

void s5j_mct_global_enable_int(FAR struct s5j_mct_priv_s *priv, uint32_t gmct_int)
{
	unsigned int reg;

	DEBUGASSERT(gmct_int >= 0 && gmct_int < 4);

	reg = getreg32(priv->base_addr + S5J_MCT_G_INT_ENB);
	reg |= 1 << gmct_int;
	putreg32(reg, priv->base_addr + S5J_MCT_G_INT_ENB);
}

void s5j_mct_ack_irq(FAR struct s5j_mct_priv_s *priv)
{
	unsigned int cstat = S5J_MCT_INT_CSTAT_FRC | S5J_MCT_INT_CSTAT_ICNT;

	mct_putreg32(priv, S5J_MCT_L_INT_CSTAT_OFFSET, cstat);
}

void s5j_mct_setmode(FAR struct s5j_mct_priv_s *priv, bool oneshot)
{
	unsigned int value = mct_getreg32(priv, S5J_MCT_L_TCON_OFFSET);

	if (oneshot) {
		mct_putreg32(priv, S5J_MCT_L_TCON_OFFSET, value & ~S5J_MCT_L_TCON_INTERVAL_MODE);
	} else {
		mct_putreg32(priv, S5J_MCT_L_TCON_OFFSET, value | S5J_MCT_L_TCON_INTERVAL_MODE);
	}

	s5j_mct_wait_wstat(priv->base_addr, S5J_MCT_WSTAT_TCON);
}

void s5j_mct_enable(FAR struct s5j_mct_priv_s *priv)
{
	unsigned int value;

	/* Start timer and enable interrupt */
	value = mct_getreg32(priv,  S5J_MCT_L_TCON_OFFSET);
	value |=  S5J_MCT_L_TCON_INT_START |  S5J_MCT_L_TCON_TIMER_START;
	mct_putreg32(priv,  S5J_MCT_L_TCON_OFFSET, value);
	s5j_mct_wait_wstat(priv->base_addr, S5J_MCT_WSTAT_TCON);
}

void s5j_mct_disable(FAR struct s5j_mct_priv_s *priv)
{
	unsigned int value;

	/* Stop timer and disable interrupt */
	value = mct_getreg32(priv,  S5J_MCT_L_TCON_OFFSET);
	value &= ~(S5J_MCT_L_TCON_TIMER_START | S5J_MCT_L_TCON_INT_START);
	mct_putreg32(priv,  S5J_MCT_L_TCON_OFFSET, value);
	s5j_mct_wait_wstat(priv->base_addr, S5J_MCT_WSTAT_TCON);
}

int s5j_mct_setisr(FAR struct s5j_mct_priv_s *priv, xcpt_t handler, void *arg)
{
	irq_attach(priv->irq_id, handler, arg);

	/* Enable SysTick interrupts */
	up_enable_irq(priv->irq_id);

	return OK;
}

void s5j_mct_setperiod(FAR struct s5j_mct_priv_s *priv, uint32_t period)
{
	/* Write the appropriate values to TCNTB SFR to set the tick counter. */
	mct_putreg32(priv,  S5J_MCT_L_TCNTB_OFFSET, period);
	s5j_mct_wait_wstat(priv->base_addr, S5J_MCT_WSTAT_TCNTB);

	/*
	 * Write ICNTB SFR with ICNTB[31] = 1 to
	 * update the 31-bit internal interrupt counter.
	 */
	mct_putreg32(priv,  S5J_MCT_L_ICNTB_OFFSET, 1 << 31);
	s5j_mct_wait_wstat(priv->base_addr, S5J_MCT_WSTAT_ICNTB);
}

void s5j_mct_enableint(FAR struct s5j_mct_priv_s *priv)
{
	mct_putreg32(priv,  S5J_MCT_L_INT_ENB_OFFSET, S5J_MCT_INTR_ICNT);
}

void s5j_mct_disableint(FAR struct s5j_mct_priv_s *priv)
{
	mct_putreg32(priv,  S5J_MCT_L_INT_ENB_OFFSET, 0);
}

FAR struct s5j_mct_priv_s *s5j_mct_init(int timer)
{
	FAR struct s5j_mct_priv_s *priv = NULL;

	DEBUGASSERT(timer < CONFIG_S5J_MCT_NUM);

	switch (timer) {
	/* Global Timer */
#ifdef CONFIG_S5J_GTIMER
	case S5J_MCT_GLOBAL:
		priv = &s5j_gmct_priv;
		break;
#endif

	/* Local Timers */
#ifdef CONFIG_S5J_TIMER0
	case S5J_MCT_CHANNEL0:
		priv = &s5j_mct0_priv;
		break;
#endif

#ifdef CONFIG_S5J_TIMER1
	case S5J_MCT_CHANNEL1:
		priv = &s5j_mct1_priv;
		break;
#endif

#ifdef CONFIG_S5J_TIMER2
	case S5J_MCT_CHANNEL2:
		priv = &s5j_mct2_priv;
		break;
#endif

#ifdef CONFIG_S5J_TIMER3
	case S5J_MCT_CHANNEL3:
		priv = &s5j_mct3_priv;
		break;
#endif
	}

	if (!g_mct_initialized && priv) {
		s5j_mct_setclock(USEC_PER_SEC);
		g_mct_initialized = true;
		lldbg("MCT Initilaized\n");
	}

	return priv;
}
