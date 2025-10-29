// Copyright 2020-2025 Beken
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
#include <os/os.h>
#include <os/mem.h>
#include <driver/wdt.h>
#include "wdt_driver.h"
#include "wdt_hal.h"
#include "reset_reason.h"
#include "bk_fake_clock.h"
#include "icu_driver.h"
#include "power_driver.h"
#include <components/system.h>
#include "sys_driver.h"
#include <driver/timer.h>
#include "bk_wdt.h"
#include "aon_pmu_driver.h"
#if defined(CONFIG_AON_RTC)
#include <driver/aon_rtc.h>
#endif

typedef struct {
	wdt_hal_t hal;
	uint8_t init_bits;
} wdt_driver_t;

#define DUMP_THREAD_WHEN_TASK_WDG_TIGGERED 1
#define DUMP_STACK_WHEN_TASK_WDG_TIGGERED 1

#define INT_WDG_FEED_PERIOD_TICK ((BK_MS_TO_TICKS(CONFIG_INT_WDT_PERIOD_MS)) >> 4)
#define TASK_WDG_PERIOD_TICK (BK_MS_TO_TICKS(CONFIG_TASK_WDT_PERIOD_MS))

#if defined(CONFIG_AON_RTC)
#define GET_TASK_CURRENT_TICK()  (BK_MS_TO_TICKS(bk_aon_rtc_get_us()/1000))
#else
#define GET_TASK_CURRENT_TICK()  (bk_get_tick())
#endif

#define WDT_RETURN_ON_DRIVER_NOT_INIT() do {\
		if (!s_wdt_driver_is_init) {\
			WDT_LOGE("WDT driver not init\r\n");\
			return BK_ERR_WDT_DRIVER_NOT_INIT;\
		}\
	} while(0)

#define WDT_RETURN_ON_NOT_INIT() do {\
		if (!(s_wdt.init_bits & BIT(0))) {\
			return BK_ERR_WDT_NOT_INIT;\
		}\
	} while(0)

#define WDT_RETURN_ON_INVALID_PERIOD(timeout) do {\
		if ((timeout) > WDT_F_PERIOD_V) {\
			WDT_LOGE("WDT invalid timeout\r\n");\
			return BK_ERR_WDT_INVALID_PERIOD;\
		}\
	} while(0)

#define WDT_BARK_TIME_MS    1000
#define NMI_WDT_CLK_DIV_16  3

static wdt_driver_t s_wdt = {0};
static bool s_wdt_driver_is_init = false;
static uint32_t s_wdt_period = CONFIG_INT_WDT_PERIOD_MS;

#if (defined(CONFIG_TASK_WDT))
static uint64_t s_last_task_wdt_feed_tick = 0;
static uint64_t s_last_task_wdt_log_tick = 0;
static bool s_task_wdt_enabled = true;
#endif

# if (defined(CONFIG_INT_WDT))
static uint32_t s_feed_watchdog_time = INT_WDG_FEED_PERIOD_TICK;
#endif

__IRAM_SEC static void wdt_init_common(void)
{
#if (defined(CONFIG_SYSTEM_CTRL))
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_WDG_CPU, CLK_PWR_CTRL_PWR_UP);
#else
	power_up_wdt();
#endif
}

__attribute__((section(".itcm_sec_code"))) static void wdt_deinit_common(void)
{
	s_wdt_period = CONFIG_INT_WDT_PERIOD_MS;
	wdt_hal_reset_config_to_default(&s_wdt.hal);
#if (defined(CONFIG_SYSTEM_CTRL))
	close_wdt();
	//sys_drv_dev_clk_pwr_up(CLK_PWR_ID_WDG_CPU, CLK_PWR_CTRL_PWR_DOWN);
#else
	power_down_wdt();
#endif
}

bk_err_t bk_wdt_driver_init(void)
{
	if (s_wdt_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_wdt, 0, sizeof(s_wdt));
	wdt_hal_init(&s_wdt.hal);
#if (defined(CONFIG_SYS_CPU0))
	// bk_timer_start(TIMER_ID2, WDT_BARK_TIME_MS, (timer_isr_t)bk_wdt_feed_handle);
	sys_drv_nmi_wdt_set_clk_div(NMI_WDT_CLK_DIV_16);
	aon_pmu_drv_wdt_rst_dev_enable();
#endif
	s_wdt_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_wdt_driver_deinit(void)
{
	if (!s_wdt_driver_is_init) {
		return BK_OK;
	}
	wdt_deinit_common();
#if (defined(CONFIG_SYS_CPU0))
	bk_timer_stop(TIMER_ID2);
#endif
	s_wdt_driver_is_init = false;

	return BK_OK;
}

/* 1. power up wdt
 * 2. set period
 */
__IRAM_SEC bk_err_t bk_wdt_start(uint32_t timeout_ms)
{
	WDT_RETURN_ON_DRIVER_NOT_INIT();
	WDT_RETURN_ON_INVALID_PERIOD(timeout_ms);

	if (!timeout_ms) {
		timeout_ms = CONFIG_INT_WDT_PERIOD_MS;
	}

	s_wdt_period = timeout_ms;
	wdt_init_common();
	wdt_hal_init_wdt(&s_wdt.hal, timeout_ms);
	s_wdt.init_bits |= BIT(0);
	WDT_LOGD("bk_wdt_start, s_wdt.init_bits:%x\n", s_wdt.init_bits);

	return BK_OK;
}

__attribute__((section(".itcm_sec_code"))) bk_err_t bk_wdt_stop(void)
{
	WDT_RETURN_ON_DRIVER_NOT_INIT();
	wdt_deinit_common();
	s_wdt.init_bits &= ~BIT(0);
	//WDT_LOGD("bk_wdt_stop, s_wdt.init_bits:%x\n", s_wdt.init_bits);
	return BK_OK;
}

bk_err_t bk_wdt_feed(void)
{
	WDT_RETURN_ON_DRIVER_NOT_INIT();
	WDT_RETURN_ON_NOT_INIT();

	wdt_hal_init_wdt(&s_wdt.hal, s_wdt_period);

	return BK_OK;
}

#if (defined(CONFIG_INT_WDT))
void bk_int_wdt_feed(void)
{
	static uint64_t s_last_int_wdt_feed_tick = 0;
	uint64_t current_tick = GET_TASK_CURRENT_TICK();

	if ((current_tick - s_last_int_wdt_feed_tick) >= s_feed_watchdog_time) {
		bk_wdt_feed();
		s_last_int_wdt_feed_tick = current_tick;
		//WDT_LOGD("feed interrupt watchdog, s_feed_watchdog_time = %u ms.\n", s_feed_watchdog_time);
	}
}

uint32_t bk_wdt_get_feed_time()
{
	return s_feed_watchdog_time;
}

void bk_wdt_set_feed_time(uint32_t dw_set_time)
{
	s_feed_watchdog_time = dw_set_time;
}

#endif

#if (defined(CONFIG_TASK_WDT))
__IRAM_SEC void bk_task_wdt_start()
{
	s_task_wdt_enabled = true;
}

__attribute__((section(".itcm_sec_code"))) void bk_task_wdt_stop()
{
	s_task_wdt_enabled = false;
}

void bk_task_wdt_feed(void)
{
	s_last_task_wdt_feed_tick = GET_TASK_CURRENT_TICK();
}

void bk_task_wdt_timeout_check(void)
{
	if (s_last_task_wdt_feed_tick && s_task_wdt_enabled) {
		const uint64_t c_last_feed_tick = s_last_task_wdt_feed_tick;
		const uint64_t current_tick = GET_TASK_CURRENT_TICK();

		if(current_tick > c_last_feed_tick) {
			if ((current_tick - c_last_feed_tick) > TASK_WDG_PERIOD_TICK) {
				if ((current_tick - s_last_task_wdt_log_tick) > TASK_WDG_PERIOD_TICK) {
					WDT_LOGW("task watchdog triggered\r\n");
					s_last_task_wdt_log_tick = current_tick;
					BK_ASSERT(0);
				}
			}
		}
	}
}

#endif

bool bk_wdt_is_driver_inited()
{
	return s_wdt_driver_is_init;
}

void bk_wdt_feed_handle(void)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

#if (defined(CONFIG_INT_WDT))
	bk_int_wdt_feed();
#endif

#if (defined(CONFIG_TASK_WDT))
	bk_task_wdt_timeout_check();
#endif
	GLOBAL_INT_RESTORE();
}

__attribute__((section(".itcm_sec_code"))) void close_wdt(void)
{
	wdt_hal_close();
}

void bk_wdt_force_feed(void)
{
	wdt_hal_force_feed();
}

void bk_wdt_force_reboot(void)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	aon_pmu_drv_r0_latch_to_r7b();
#endif
	wdt_hal_force_reboot();
	while(1);
	GLOBAL_INT_RESTORE();
}
