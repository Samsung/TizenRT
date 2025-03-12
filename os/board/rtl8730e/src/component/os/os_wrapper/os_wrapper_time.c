/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba.h"
#include "os_wrapper.h"

uint64_t GenericTimerFreq;
#define pdTICKS_TO_CNT	(GenericTimerFreq/TICK_PER_SEC)

void rtos_time_delay_ms(uint32_t ms)
{
	up_mdelay(ms);
}

void rtos_time_delay_us(uint32_t us)
{
	up_udelay(us);
}

uint32_t rtos_time_get_current_system_time_ms(void)
{
	return (uint32_t)(clock()/TICK_PER_MSEC);
}

uint32_t rtos_time_get_current_system_time_us(void)
{
	return (rtos_time_get_current_system_time_ns() / 1000);
}

uint64_t rtos_time_get_current_system_time_ns(void)
{
	uint64_t time_ns = 0;
	irqstate_t flags = tizenrt_critical_enter();
	uint32_t time_ms = rtos_time_get_current_system_time_ms();
	uint64_t pass_value = 0;
	uint64_t compare_value = arm_arch_timer_compare();
	uint64_t current_value = arm_arch_timer_count();
	if (compare_value >= current_value) {
		pass_value = pdTICKS_TO_CNT - (compare_value - current_value);
	} else {
		pass_value = pdTICKS_TO_CNT + (current_value - compare_value);
	}
	time_ns = time_ms * 1000000ULL + (pass_value * 1000) / 50;  // arm_arch_timer_count is 50 Mhz
	tizenrt_critical_exit(flags);
	return time_ns;
}
