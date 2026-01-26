// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <soc/soc.h>

#define TIMER0_REG_SET(reg_id, l, h, v) REG_SET((SOC_TIMER0_REG_BASE + ((reg_id) << 2)), (l), (h), (v))
#define TIMER0_PERIOD 0xFFFFFFFF

__IRAM_SEC static inline uint32_t timer_hal_get_timer0_cnt(void)
{
	volatile uint32_t int_level = 0;
	uint32_t cnt = 0;
	int_level = rtos_enter_critical();
	TIMER0_REG_SET(8, 2, 3, 0);
	TIMER0_REG_SET(8, 0, 0, 1);
	while (REG_READ((SOC_TIMER0_REG_BASE + (8 << 2))) & BIT(0));

	cnt = REG_READ(SOC_TIMER0_REG_BASE + (9 << 2));
	rtos_exit_critical(int_level);
	return cnt;
}

__IRAM_SEC static inline uint32_t timer_hal_diff(uint32_t begin, uint32_t end)
{
	if (end > begin) {
		return end - begin;
	} else {
		return (TIMER0_PERIOD - begin + end);
	}
}

__FLASH_BOOT_CODE void timer_hal_us_init(void)
{
	TIMER0_REG_SET(2, 0, 0, 1);
	REG_WRITE((SOC_TIMER0_REG_BASE + (4 << 2)), TIMER0_PERIOD);
	TIMER0_REG_SET(7, 0, 0, 1);
	SYS_REG_SET(0xc, 4, 4, 1);
	SYS_REG_SET(8, 20, 20, 1);
}

/**
 * TODO optimize it!
 *
 * If we need very accurate delay, please write your owner delay!
 * 
 * Test Result:
 *  us          actual
 * --------------------------------
 *  1     |      8.8
 *  2     |      8.8
 *  3     |      8.4
 *  4     |      10
 *  5     |      9.6
 *  100   |      106
 *  1000  |      1007
 */
__IRAM_SEC void timer_hal_delay_us(uint32_t us)
{
	uint32_t delay_cycle = TIMER_CLOCK_FREQ_XTAL / 1000 * us;

	if ((REG_READ(SOC_TIMER0_REG_BASE + (7 << 2)) & BIT(0)) == 0) {
		TIMER0_REG_SET(7, 0, 0, 1);
	}

	uint32_t begin = timer_hal_get_timer0_cnt();
	uint32_t end;
	uint32_t diff;

	do {
		end = timer_hal_get_timer0_cnt();
		diff = timer_hal_diff(begin, end);
	} while (diff < delay_cycle);
}
