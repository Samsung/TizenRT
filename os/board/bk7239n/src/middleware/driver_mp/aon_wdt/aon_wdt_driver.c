// Copyright 2021-2025 Beken
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

#include <os/mem.h>
#include "reset_reason.h"
#include "bk_fake_clock.h"

#include "driver/aon_wdt_types.h"
#include "aon_wdt_hal.h"
#include <driver/aon_wdt.h>
#include "aon_wdt_driver.h"
#include "bk_aon_wdt.h"

#define INT_AON_WDG_FEED_PERIOD_TICK ((BK_MS_TO_TICKS(CONFIG_INT_AON_WDT_PERIOD_MS)) >> 4)

#if (defined(CONFIG_INT_AON_WDT))
	static uint32_t s_feed_aon_watchdog_time = INT_AON_WDG_FEED_PERIOD_TICK;
#endif

static uint32_t s_aon_wdt_period = CONFIG_INT_AON_WDT_PERIOD_MS;

#define AON_WDT_RETURN_ON_INVALID_PERIOD(timeout) do {\
		if ((timeout) > AON_WDT_F_PERIOD_V) {\
			AON_WDT_LOGE("AON_WDT invalid timeout\r\n");\
			return BK_ERR_AON_WDT_INVALID_PERIOD;\
		}\
	} while(0)

uint32_t bk_aon_wdt_get_period(void)
{
	return aon_wdt_hal_get_period();
}

bk_err_t bk_aon_wdt_set_period(uint32_t timeout_ms)
{
	AON_WDT_RETURN_ON_INVALID_PERIOD(timeout_ms);

	if (!timeout_ms) {
		timeout_ms = CONFIG_INT_AON_WDT_PERIOD_MS;
	}

	s_aon_wdt_period = timeout_ms;
	aon_wdt_hal_set_period(timeout_ms);
	AON_WDT_LOGD("bk_aon_wdt set period: %d.\r\n", timeout_ms);

	return BK_OK;
}

static void aon_wdt_deinit_common(void)
{
	s_aon_wdt_period = CONFIG_INT_AON_WDT_PERIOD_MS;
#if (defined(CONFIG_SYSTEM_CTRL))
	aon_wdt_hal_stop_aon_wdt();
#endif
}

bk_err_t bk_aon_wdt_stop(void)
{
	aon_wdt_deinit_common();
	AON_WDT_LOGD("bk_aon_wdt_stop.\r\n");

	return BK_OK;
}

bk_err_t bk_aon_wdt_feed(void)
{
	aon_wdt_hal_set_period(s_aon_wdt_period);

	bk_misc_set_reset_reason(RESET_SOURCE_WATCHDOG);

	return BK_OK;
}

#if (defined(CONFIG_INT_AON_WDT))
void bk_int_aon_wdt_feed(void)
{
	static uint64_t s_last_int_aon_wdt_feed_tick = 0;
	uint64_t current_tick = bk_get_tick();
	if ((current_tick - s_last_int_aon_wdt_feed_tick) >= s_feed_aon_watchdog_time) {
		bk_aon_wdt_feed();
		s_last_int_aon_wdt_feed_tick = current_tick;
		AON_WDT_LOGD("feed interrupt watchdog\n");
	}
}

uint32_t bk_aon_wdt_get_feed_time(void)
{
	return s_feed_aon_watchdog_time;
}

bk_err_t bk_aon_wdt_set_feed_time(uint32_t feed_time)
{
	s_feed_aon_watchdog_time = feed_time;
	return BK_OK;
}

#endif /* #if (CONFIG_INT_AON_WDT) */

void bk_aon_wdt_feed_handle(void)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

#if (defined(CONFIG_INT_AON_WDT))
	bk_int_aon_wdt_feed();
#endif

	GLOBAL_INT_RESTORE();
}

