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
#include <common/bk_compiler.h>
#include <os/mem.h>
#include "icu_driver.h"
#include "timer_driver.h"
#include "timer_hal.h"
#include <driver/timer.h>
#include "clock_driver.h"
#include "power_driver.h"
#include <driver/int.h>
#include "sys_driver.h"
#include "timer_driver.h"

#if (SOC_TIMER_INTERRUPT_NUM > 1)
static void timer1_isr(void) ;
#endif
static void timer_isr(void) ;

typedef struct {
    timer_hal_t hal;
#if defined(CONFIG_TIMER_PM_CB_SUPPORT)
    uint32_t pm_backup[SOC_TIMER_GROUP_NUM][TIMER_PM_BACKUP_REG_NUM];
    uint8_t pm_bakeup_is_valid[SOC_TIMER_GROUP_NUM];
#endif
} timer_driver_t;

static timer_driver_t s_timer = {0};
static timer_isr_t s_timer_isr[SOC_TIMER_CHAN_NUM_PER_UNIT] = {NULL};
static bool s_timer_driver_is_init = false;

#define TIMER_RETURN_ON_NOT_INIT() do {\
        if (!s_timer_driver_is_init) {\
            return BK_ERR_TIMER_NOT_INIT;\
        }\
    } while(0)

#define TIMER_RETURN_ON_INVALID_ID(id) do {\
        if ((id) >= SOC_TIMER_CHAN_NUM_PER_UNIT) {\
            return BK_ERR_TIMER_ID;\
        }\
    } while(0)

#define TIMER_RETURN_ON_IS_RUNNING(id, status) do {\
        if ((status) & BIT((id))) {\
            return BK_ERR_TIMER_IS_RUNNING;\
        }\
    } while(0)

#define TIMER_RETURN_TIMER_ID_IS_ERR(id) do {\
        if ((~CONFIG_TIMER_SUPPORT_ID_BITS) & BIT((id))) {\
            return BK_ERR_TIMER_ID_ON_DEFCONFIG;\
        }\
    } while(0)

#if defined(CONFIG_TIMER_PM_CB_SUPPORT)
static bk_err_t timer_pm_restore(uint64_t sleep_time, void *args);
static void timer_restore_regs(uint32_t group_id)
{
    switch (group_id) {
    case 0:
#if defined(CONFIG_DEEP_LV)
        if (bk_pm_module_lv_sleep_state_get(PM_DEV_ID_TIMER_0)) {
            timer_pm_restore(0, (void *)group_id);
            bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_TIMER_0);
        }
#endif
        break;
    case 1:
        if (bk_pm_module_lv_sleep_state_get(PM_DEV_ID_TIMER_1)) {
            bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_TIMER1, PM_POWER_MODULE_STATE_ON);
            timer_pm_restore(0, (void *)group_id);
            bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_TIMER_1);
        }
        break;
    default:
        break;
    }
}

#define TIMER_PM_CHECK_RESTORE(id) do {\
    uint32_t group_id;\
    GLOBAL_INT_DECLARATION();\
    GLOBAL_INT_DISABLE();\
    group_id = id / SOC_TIMER_CHAN_NUM_PER_GROUP;\
    timer_restore_regs(group_id);\
    GLOBAL_INT_RESTORE();\
}while(0)
#else
#define TIMER_PM_CHECK_RESTORE(id)
#endif

void bk_timer_clock_select(timer_id_t id, timer_src_clk_t mode)
{
	uint32_t group_index = 0;

	group_index = id / SOC_TIMER_CHAN_NUM_PER_GROUP;
	switch(group_index)
	{
		case 0:
			sys_drv_timer_select_clock(SYS_SEL_TIMER0, mode);
			break;
		case 1:
			sys_drv_timer_select_clock(SYS_SEL_TIMER1, mode);
			break;
		default:
			break;
	}
}

uint32_t bk_timer_get_clock_src(timer_id_t timer_id)
{
	uint32_t group_index;
    uint32_t timer_clock = 0xff;

	group_index = timer_id / SOC_TIMER_CHAN_NUM_PER_GROUP;
	switch(group_index)
	{
		case 0:
			timer_clock = sys_hal_timer_select_clock_get(SYS_SEL_TIMER0);
			break;
		case 1:
			timer_clock = sys_hal_timer_select_clock_get(SYS_SEL_TIMER1);
			break;
		default:
			break;
	}

    return timer_clock;
}

static void timer_clock_enable(timer_id_t id)
{
	uint32_t group_index = 0;

	group_index = id / SOC_TIMER_CHAN_NUM_PER_GROUP;
	switch(group_index)
	{
		case 0:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_TIMER_1, CLK_PWR_CTRL_PWR_UP);
			break;
		case 1:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_TIMER_2, CLK_PWR_CTRL_PWR_UP);
			break;
		case 2:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_TIMER_3, CLK_PWR_CTRL_PWR_UP);
			break;
		default:
			break;
	}
}

static void timer_clock_disable(timer_id_t id)
{
	uint32_t group_index = 0;

	group_index = id / SOC_TIMER_CHAN_NUM_PER_GROUP;
	switch(group_index)
	{
		case 0:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_TIMER_1, CLK_PWR_CTRL_PWR_DOWN);
			break;
		case 1:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_TIMER_2, CLK_PWR_CTRL_PWR_DOWN);
			break;
		case 2:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_TIMER_3, CLK_PWR_CTRL_PWR_DOWN);
			break;
		default:
			break;
	}
}

static void timer_interrupt_enable(timer_id_t id)
{
	uint32_t group_index = 0;

	group_index = id / SOC_TIMER_CHAN_NUM_PER_GROUP;
	switch(group_index)
	{
		case 0:
			sys_drv_int_enable(TIMER_INTERRUPT_CTRL_BIT);
			break;

		case 1:
			sys_drv_int_enable(TIMER1_INTERRUPT_CTRL_BIT);
			break;
		default:
			break;
	}
}

static void timer_chan_init_common(timer_id_t timer_id)
{
	timer_clock_enable(timer_id);
}

static void timer_chan_deinit_common(timer_id_t timer_id)
{
    timer_hal_stop_common(&s_timer.hal, timer_id);
    timer_hal_reset_config_to_default(&s_timer.hal, timer_id);
	timer_clock_disable(timer_id);
}

static void timer_chan_enable_interrupt_common(timer_id_t timer_id)
{
	timer_interrupt_enable(timer_id);
    timer_hal_enable_interrupt(&s_timer.hal, timer_id);
}

#if defined(CONFIG_TIMER_PM_CB_SUPPORT)
__IRAM_SEC static bk_err_t timer_pm_backup(uint64_t sleep_time, void *args)
{
    uint32_t group_id = (uint32_t)args;
    TIMER_RETURN_ON_NOT_INIT();

    if (group_id > SOC_TIMER_GROUP_NUM - 1)
        return BK_FAIL;

    if (!s_timer.pm_bakeup_is_valid[group_id])
    {
        timer_hal_backup(&s_timer.hal, group_id, &s_timer.pm_backup[group_id][0]);
        s_timer.pm_bakeup_is_valid[group_id] = 1;
    }

    return BK_OK;
}

__IRAM_SEC static bk_err_t timer_pm_restore(uint64_t sleep_time, void *args)
{
    uint32_t group_id = (uint32_t)args;
    TIMER_RETURN_ON_NOT_INIT();

    if (group_id > SOC_TIMER_GROUP_NUM - 1)
        return BK_FAIL;

    if (s_timer.pm_bakeup_is_valid[group_id])
    {
        timer_hal_restore(&s_timer.hal, group_id, &s_timer.pm_backup[group_id][0]);
        s_timer.pm_bakeup_is_valid[group_id] = 0;
    }

    return BK_OK;
}

static void timer_register_lvsleep_cb(uint32_t group_id)
{
    pm_cb_conf_t timer_enter_config = {
        .cb = (pm_cb)timer_pm_backup,
        .args = (void *)group_id
    };

    pm_cb_conf_t timer_exit_config = {
        .cb = (pm_cb)timer_pm_restore,
        .args = (void *)group_id
    };

    switch(group_id)
    {
        case 0:
#if defined(CONFIG_DEEP_LV)
            bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_TIMER_0, &timer_enter_config, &timer_exit_config);
            bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_TIMER_0);
#endif
            break;
        case 1:
            bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_TIMER1, PM_POWER_MODULE_STATE_ON);
            bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_TIMER_1, &timer_enter_config, &timer_exit_config);
            bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_TIMER_1);
            break;
        default:
            break;
    }
}

static void timer_unregister_lvsleep_cb(uint32_t group_id)
{
    switch(group_id)
    {
        case 0:
#if defined(CONFIG_DEEP_LV)
            bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_TIMER_0, true, true);
#endif
            break;
        case 1:
            bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_TIMER_1, true, true);
            bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_TIMER1, PM_POWER_MODULE_STATE_OFF);
            break;
        default:
            break;
    }
}
#endif

static void timer_isr(void);

bk_err_t bk_timer_driver_init(void)
{
    if (s_timer_driver_is_init) {
        return BK_OK;
    }

    /* the default clock src of timer0 timer1*/
    bk_timer_clock_select(SYS_SEL_TIMER0, TIMER_SCLK_XTAL);
    bk_timer_clock_select(SYS_SEL_TIMER1, TIMER_SCLK_XTAL);

    os_memset(&s_timer, 0, sizeof(s_timer));
    os_memset(&s_timer_isr, 0, sizeof(s_timer_isr));

#if defined(CONFIG_TIMER_PM_CB_SUPPORT)
    for(uint32_t group_id = 0; group_id < SOC_TIMER_GROUP_NUM; group_id ++)
    {
        timer_register_lvsleep_cb(group_id);
    }
#endif

    bk_int_isr_register(INT_SRC_TIMER, timer_isr, NULL);
#if (SOC_TIMER_INTERRUPT_NUM > 1)
    bk_int_isr_register(INT_SRC_TIMER1, timer1_isr, NULL);
#endif
    timer_hal_init(&s_timer.hal);

    s_timer_driver_is_init = true;

    return BK_OK;
}

bk_err_t bk_timer_driver_deinit(void)
{
    if (!s_timer_driver_is_init) {
        return BK_OK;
    }

#if defined(CONFIG_TIMER_PM_CB_SUPPORT)
    for (uint32_t group_id = 1; group_id < SOC_TIMER_GROUP_NUM; group_id++)
    {
        timer_unregister_lvsleep_cb(group_id);
    }
#endif

    for (int chan = 0; chan < SOC_TIMER_CHAN_NUM_PER_UNIT; chan++) {
        timer_chan_deinit_common(chan);
    }

    s_timer_driver_is_init = false;

    return BK_OK;
}

extern void bk_delay(int num);//TODO fix me

bk_err_t bk_timer_start_without_callback(timer_id_t timer_id, uint32_t time_ms)
{
    uint32_t en_status = 0;

#if defined(CONFIG_TIMER_US)
	if (timer_id == TIMER_ID0) {
		TIMER_LOGE("timer0 is reserved for us timer!\r\n");
	}
#endif

    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    timer_chan_init_common(timer_id);

#if !defined(CONFIG_RTC_TIMER_PRECISION_TEST)
    timer_chan_enable_interrupt_common(timer_id);
#endif

    en_status = timer_hal_get_enable_status(&s_timer.hal);
    if (en_status & BIT(timer_id)) {
        TIMER_LOGD("timer(%d) is running, stop it\r\n", timer_id);
        timer_hal_disable(&s_timer.hal, timer_id);
        /* Delay to fix the bug that timer counter becomes bigger than
         * timer period. Once timer counter becomes bigger than timer period,
         * the timer will never timeout, or takes very very long time to
         * timeout.
         *
         * This issue is firstly observed in HOS tick timer. HOS restarts
         * the tick timer with different time_ms(timer period) again and again,
         * without the delay, the tick timer counter becomes bigger than timer
         * period very soon, then the tick interrupt will never be triggered.
         * */
        bk_delay(4);
    }

    timer_hal_init_timer(&s_timer.hal, timer_id, time_ms, TIMER_UNIT_MS);
    timer_hal_start_common(&s_timer.hal, timer_id);

    return BK_OK;
}

bk_err_t bk_timer_start(timer_id_t timer_id, uint32_t time_ms, timer_isr_t callback)
{
#if defined(CONFIG_TIMER_US)
	if (timer_id == TIMER_ID0) {
		TIMER_LOGE("timer0 is reserved for us timer!\r\n");
	}
#endif

#if defined(CONFIG_TIMER_SUPPORT_ID_BITS)
    TIMER_RETURN_TIMER_ID_IS_ERR(timer_id);
#endif
    BK_LOG_ON_ERR(bk_timer_start_without_callback(timer_id, time_ms));

    if (timer_id < SOC_TIMER_CHAN_NUM_PER_UNIT){
        s_timer_isr[timer_id] = callback;
    }

    return BK_OK;
}

bk_err_t bk_timer_stop(timer_id_t timer_id)
{
    TIMER_RETURN_ON_NOT_INIT();
#if defined(CONFIG_TIMER_SUPPORT_ID_BITS)
    TIMER_RETURN_TIMER_ID_IS_ERR(timer_id);
#endif
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    timer_hal_stop_common(&s_timer.hal, timer_id);

    return BK_OK;
}

bk_err_t bk_timer_cancel(timer_id_t timer_id)
{
    TIMER_RETURN_ON_NOT_INIT();
#if defined(CONFIG_TIMER_SUPPORT_ID_BITS)
    TIMER_RETURN_TIMER_ID_IS_ERR(timer_id);
#endif
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    timer_ll_reset_config_to_default(s_timer.hal.hw, timer_id);

    s_timer_isr[timer_id] = NULL;

    return BK_OK;
}

uint32_t bk_timer_get_cnt(timer_id_t timer_id)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    return timer_hal_get_count(&s_timer.hal, timer_id);
}

bk_err_t bk_timer_enable(timer_id_t timer_id)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    timer_hal_enable(&s_timer.hal, timer_id);
    return BK_OK;
}

bk_err_t bk_timer_disable(timer_id_t timer_id)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    timer_hal_disable(&s_timer.hal, timer_id);
    return BK_OK;
}

uint32_t bk_timer_get_period(timer_id_t timer_id)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    return timer_hal_get_end_count(&s_timer.hal, timer_id);
}

uint32_t bk_timer_get_enable_status(void)
{
    TIMER_RETURN_ON_NOT_INIT();
    return timer_hal_get_enable_status(&s_timer.hal);
}

bool bk_timer_is_interrupt_triggered(timer_id_t timer_id)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);

    TIMER_PM_CHECK_RESTORE(timer_id);

    uint32_t int_status = timer_hal_get_interrupt_status(&s_timer.hal);
    return timer_hal_is_interrupt_triggered(&s_timer.hal, timer_id, int_status);
}

uint32_t timer_clear_isr_status(void)
{
	uint32_t int_status;
    timer_hal_t *hal = &s_timer.hal;

    int_status = timer_hal_get_interrupt_status(hal);
    timer_hal_clear_interrupt_status(hal, int_status);

	return int_status;
}

static void timer_isr(void)
{
    uint32_t int_status;
    timer_hal_t *hal = &s_timer.hal;

    int_status = timer_clear_isr_status();

#if (SOC_TIMER_INTERRUPT_NUM > 1)
     for(int chan = 0; chan < SOC_TIMER_CHAN_NUM_PER_GROUP; chan++) {
#else
    for(int chan = 0; chan < SOC_TIMER_CHAN_NUM_PER_UNIT; chan++) {
#endif
        if(timer_hal_is_interrupt_triggered(hal, chan, int_status)) {
            if(s_timer_isr[chan]) {
                s_timer_isr[chan](chan);
            }
        }
    }
}

#if (SOC_TIMER_INTERRUPT_NUM > 1)
static void timer1_isr(void)
{
    uint32_t int_status;
    timer_hal_t *hal = &s_timer.hal;

    int_status = timer_clear_isr_status();

    for(int chan = SOC_TIMER_CHAN_NUM_PER_GROUP; chan < SOC_TIMER_CHAN_NUM_PER_UNIT; chan++) {
        if(timer_hal_is_interrupt_triggered(hal, chan, int_status)) {
            if(s_timer_isr[chan]) {
                s_timer_isr[chan](chan);
            }
        }
    }
}
#endif

uint64_t bk_timer_get_time(timer_id_t timer_id, uint32_t div, uint32_t last_count, timer_value_unit_t unit_type)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);
    TIMER_PM_CHECK_RESTORE(timer_id);

    uint64_t current_time = 0;
    uint64_t unit_factor = 1;
    uint64_t current_count;
	uint32_t timer_clock = TIMER_SCLK_XTAL;

    current_count = timer_hal_get_count(&s_timer.hal, timer_id) + last_count;
    if (div == 0) {
        div = 1;
    }

    timer_clock = bk_timer_get_clock_src(timer_id);
    unit_factor = (unit_type == TIMER_UNIT_MS) ? 1 : 1000;

	if(timer_clock == TIMER_SCLK_XTAL) {
        current_time = unit_factor * current_count * (uint64_t)div / TIMER_CLOCK_FREQ_XTAL;
	} else {
        current_time = unit_factor * current_count * (uint64_t)div / TIMER_CLOCK_FREQ_32K;
	}

    return current_time;
}

bk_err_t bk_timer_delay_with_callback(timer_id_t timer_id, uint64_t time_us, timer_isr_t callback)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);

#if defined(CONFIG_TIMER_SUPPORT_ID_BITS)
    TIMER_RETURN_TIMER_ID_IS_ERR(timer_id);
#endif
    uint64_t current_count = 0;
    uint64_t delta_count = 0;
    uint64_t end_count = 0;

    TIMER_PM_CHECK_RESTORE(timer_id);

    timer_chan_init_common(timer_id);
    timer_chan_enable_interrupt_common(timer_id);

    current_count = timer_hal_get_count(&s_timer.hal, timer_id);
    delta_count = timer_hal_cal_end_count(timer_id, time_us, 1, TIMER_UNIT_US);
    end_count = current_count + delta_count;

    if(end_count > 0xFFFFFFFFFFFFFFFF){
        end_count = 0xFFFFFFFFFFFFFFFF;
    }

    timer_ll_set_end_count(s_timer.hal.hw, timer_id, (uint32_t)end_count);
    timer_ll_set_clk_div(s_timer.hal.hw, timer_id, 0);
    timer_ll_clear_chan_interrupt_status(s_timer.hal.hw, timer_id);

    timer_hal_start_common(&s_timer.hal, timer_id);

    s_timer_isr[timer_id] = callback;

    return BK_OK;
}

#if defined(CONFIG_TIMER_US)
__IRAM_SEC void bk_timer_delay_us(uint32_t us)
{
	timer_hal_delay_us(us);
}
#endif

bk_err_t bk_timer_start_us(timer_id_t timer_id, uint64_t time_us, timer_isr_t callback)
{
    TIMER_RETURN_ON_NOT_INIT();
    TIMER_RETURN_ON_INVALID_ID(timer_id);
    TIMER_PM_CHECK_RESTORE(timer_id);
#if CONFIG_TIMER_SUPPORT_ID_BITS
    TIMER_RETURN_TIMER_ID_IS_ERR(timer_id);
#endif

    uint32_t en_status = 0;

#if CONFIG_TIMER_US
    if (timer_id == TIMER_ID0) {
        TIMER_LOGE("timer0 is reserved for us timer!\r\n");
    }
#endif

    timer_chan_init_common(timer_id);
#if !CONFIG_RTC_TIMER_PRECISION_TEST
    timer_chan_enable_interrupt_common(timer_id);
#endif

    en_status = timer_hal_get_enable_status(&s_timer.hal);
    if (en_status & BIT(timer_id)) {
        TIMER_LOGD("timer(%d) is running, stop it\r\n", timer_id);
        timer_hal_disable(&s_timer.hal, timer_id);
        delay(4);
    }

    timer_hal_init_timer(&s_timer.hal, timer_id, time_us, TIMER_UNIT_US);
    uint32_t int_level = rtos_enter_critical();
    timer_hal_start_common(&s_timer.hal, timer_id);
    if (timer_id < SOC_TIMER_CHAN_NUM_PER_UNIT){
        s_timer_isr[timer_id] = callback;
    }
    rtos_exit_critical(int_level);

    return BK_OK;
}
