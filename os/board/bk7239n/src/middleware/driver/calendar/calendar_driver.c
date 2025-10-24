// Copyright 2020-2021 Beken
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

#include <common/bk_include.h>
#include <os/mem.h>
#include <driver/calendar.h>
#include "calendar_driver.h"
#include "calendar_hal.h"

typedef struct {
	calendar_hal_t hal;
} calendar_driver_t;

#define CALENDAR_SRC_CLOCK_FREQ         32000
#define CALENDAR_CLOCKS_PER_SECOND      CALENDAR_SRC_CLOCK_FREQ
#define CALENDAR_PERIOD_US_PER_CLOCK    31.25f /* (1 / CALENDAR_SRC_CLOCK_FREQ) * 1000000 us */

#define CALENDAR_RETURN_ON_NOT_INIT() do {\
	if (!s_calendar_driver_is_init) {\
		return BK_ERR_CALENDAR_NOT_INIT;\
	}\
} while(0)

static calendar_driver_t s_calendar = {0};
static bool s_calendar_driver_is_init = false;

static void calendar_deinit_common(void)
{
	calendar_hal_disable(&s_calendar.hal);
	calendar_hal_reset_config_to_default(&s_calendar.hal);
}

bk_err_t bk_calendar_driver_init(void)
{
	if (s_calendar_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_calendar, 0, sizeof(s_calendar));
	calendar_hal_init(&s_calendar.hal);
	s_calendar_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_calendar_driver_deinit(void)
{
	if (!s_calendar_driver_is_init) {
		return BK_OK;
	}

	calendar_deinit_common();
	s_calendar_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_calendar_set_init_value(uint32_t init_val)
{
	CALENDAR_RETURN_ON_NOT_INIT();
	calendar_hal_set_init_value(&s_calendar.hal, init_val);

	return BK_OK;
}

uint64_t bk_calendar_get_time_us(void)
{
	uint64_t total_us_since_calendar_init = 0;
	uint32_t seconds, pre_seconds = 0;
	uint32_t clocks_of_us_part, pre_clocks_of_us_part = 0;
	uint32_t diff_s, diff_us = 0;

	while (1) {
		seconds = calendar_hal_get_second_cnt(&s_calendar.hal);
		clocks_of_us_part = calendar_hal_get_us_cnt(&s_calendar.hal);
		diff_s = seconds - pre_seconds;
		diff_us = clocks_of_us_part - pre_clocks_of_us_part;
		if ((0 == diff_s) && (0 == diff_us)) {
			break;
		}

		pre_seconds = seconds;
		pre_clocks_of_us_part = clocks_of_us_part;
	}

	/* The calendar source clock is lpo_clk(default 32k).
	 * notice:
	 * Please check lpo_clk_mux(sys_ctrl REG_0x40[0:1]) is whether rosc clock,
	 * If lpo_clk_mux select rosc clock, please check whether the rosc_clk is calibrated to 32k(sys_ctrl REG_0x4D[2])
	 * Please ensure the rosc_clk is calibrated to 32k if lpo_clk_mux select rosc_clk,
	 * otherwise the following formula is wrong if rosc_clk is calibrated to 31.25k.
	 */
	total_us_since_calendar_init = (seconds * CALENDAR_CLOCKS_PER_SECOND + clocks_of_us_part) * CALENDAR_PERIOD_US_PER_CLOCK;

	return total_us_since_calendar_init;
}

