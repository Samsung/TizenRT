/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */
#include <stdio.h>
#include "system.h"
#include "bl_bk_reg.h"

#define REG_SET(r, l, h, v) do { \
	uint32_t reg_v = *(volatile uint32_t*)(r);\
	reg_v &= ~((( 1 << ((h) + 1)) - 1) & (~( (1 << (l)) - 1)));\
	reg_v |= ((v) << (l));\
	*(volatile uint32_t*)(r) = reg_v;\
} while(0)

#define TIMER0_REG_SET(reg_id, l, h, v) REG_SET((SOC_TIMER0_REG_BASE + ((reg_id) << 2)), (l), (h), (v))
#define TIMER0_PERIOD (0xFFFFFFFF)

uint32_t timer_init(uint32_t us)
{
	CKSEL_TIM0;//cksel_tim0 : XTL
	TIM0_ENABLE;//tim0_clk enable
	REG_WRITE(SOC_TIMER0_REG_BASE + 2 * 4,(REG_READ(SOC_TIMER0_REG_BASE + 2 * 4) | (1 << 0)));//tim0 soft_reset

	uint32_t delay_cycle = UART_CLOCK_FREQ_40M / 1000 / 1000 * us;

	if ((REG_READ(SOC_TIMER0_REG_BASE + (7 << 2)) & BIT(0)) == 0) {
		TIMER0_REG_SET(7, 0, 0, 1);
	}
	return delay_cycle;
}

uint32_t timer_hal_get_timer0_cnt(void)
{
	uint32_t cnt = 0;

	TIMER0_REG_SET(8, 2, 3, 0);
	TIMER0_REG_SET(8, 0, 0, 1);
	while (REG_READ((SOC_TIMER0_REG_BASE + (8 << 2))) & BIT(0));

	cnt = REG_READ(SOC_TIMER0_REG_BASE + (9 << 2));

	return cnt;
}

uint32_t timer_hal_diff(uint32_t begin, uint32_t end)
{
	if (end > begin) {
		return end - begin;
	} else {
		return (TIMER0_PERIOD - begin + end);
	}
}

void timer_delay_us(uint32_t us)
{
	uint32_t delay_cycle = timer_init(us);
	uint32_t begin = timer_hal_get_timer0_cnt();
	uint32_t end, diff;

	do {
		end = timer_hal_get_timer0_cnt();
		diff = timer_hal_diff(begin, end);
	} while (diff < delay_cycle);
}

void timer_delay_ms(uint32_t ms)
{
	timer_delay_us(1000 * ms);
}
