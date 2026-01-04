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
#include <modules/pm.h>
#include "sys_driver.h"
#include <driver/rosc_32k.h>
#include <driver/pwr_clk.h>
#include <driver/aon_rtc.h>
#include <driver/gpio.h>
#include "gpio_driver.h"
#include "wdt_driver.h"
#include "driver/flash.h"
#include <os/os.h>

#include "pm_sleep.h"
#include "pm_power.h"
#include "pm_psram.h"
#include "pm_debug.h"
#include "pm_wakeup_source.h"
#include "pm_interface.h"

#define PM_LOWVOL_CB_SIZE                               (0x2)
#define PM_SLEEP_CB_ENTER_LOWVOL_INDEX                  (0x0)
#define PM_SLEEP_CB_EXIT_LOWVOL_INDEX                   (0x1)
#define PM_DEEPSLEEP_CB_SIZE                            (10)
#define PM_SLEEP_CB_IND_PRI_0                           (0)
#define PM_SLEEP_CB_IND_PRI_1                           (6)


static __attribute__((section(".dtcm_sec_data "))) pm_cb_conf_t s_pm_lowvol_enter_exit_cb_conf[PM_LOWVOL_CB_SIZE][PM_DEV_ID_MAX];
static __attribute__((section(".dtcm_sec_data "))) pm_sleep_cb_t s_pm_deepsleep_enter_cb_conf[PM_DEEPSLEEP_CB_SIZE];
#if CONFIG_PM_SUPER_DEEP_SLEEP
static __attribute__((section(".dtcm_sec_data "))) pm_sleep_cb_t s_pm_superdeep_enter_cb_conf[PM_DEEPSLEEP_CB_SIZE];
#endif

static __attribute__((section(".dtcm_sec_data "))) uint8_t s_pm_deepsleep_enter_cb_cnt[2] = {PM_SLEEP_CB_IND_PRI_0, PM_SLEEP_CB_IND_PRI_1};
#if CONFIG_PM_SUPER_DEEP_SLEEP
static __attribute__((section(".dtcm_sec_data "))) uint8_t s_pm_superdeep_enter_cb_cnt[2] = {PM_SLEEP_CB_IND_PRI_0, PM_SLEEP_CB_IND_PRI_1};
#endif

static volatile  uint64_t s_pm_module_lv_sleep_state               = 0;

static uint64_t s_pm_check_lv_enter_time_out	        = 0;
static pm_enter_lv_timeout_cb_t s_pm_lv_timeout_cb_arr[PM_ENTER_LV_TIME_OUT_MODULE_MAX]= {0};


static void pm_enter_cpu_wfi(void);
static void pm_enter_normal_sleep(void);
static void pm_enter_low_voltage(void);
static void pm_enter_deep_sleep(void);
static void pm_enter_super_deep_sleep(void);
static int pm_low_voltage_resource_set(void);
static void pm_low_voltage_resource_restore(void);


/*=========================SLEEP/WAKEUP FUNCTION START========================*/
uint64_t pm_cpu_wfi_process()
{
	volatile uint32_t int_level = 0;
	int_level = pm_disable_int();

	uint64_t sleep_tick         = 0ULL;
	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t entry_tick         = 0ULL;
	entry_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	#endif

	pm_enter_cpu_wfi();

	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t exit_tick          = 0ULL;
	exit_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	if(exit_tick - entry_tick < 0)
	{
		sleep_tick = 0ULL;
	}
	else
	{
		sleep_tick = exit_tick - entry_tick;
	}

	#endif
	pm_enable_int(int_level);

	return sleep_tick;
}

uint64_t pm_normal_sleep_process()
{
	volatile uint32_t int_level = 0;
	uint64_t sleep_tick         = 0ULL;

	int_level = pm_disable_int();
	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t entry_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	#endif
	#if CONFIG_PM_PROTECT_TIME_CHECK
	if (0 == bk_pm_module_power_state_get(POWER_MODULE_NAME_BTSP))
	{
		/*When check bt wakeup time comming ,return, not sleep*/
		if (!pm_check_protect_time(entry_tick, entry_tick))
		{
			return sleep_tick;
		}
	}
	#endif

	/*stop wdt*/
	bk_wdt_stop();
	close_wdt();

	/* Execute pre-sleep callbacks */
	//bk_pm_pre_sleep_callback_execute();

	pm_enter_normal_sleep();

	/* Execute post-sleep (wakeup) callbacks */
	//bk_pm_post_sleep_callback_execute();

	/*restore wdt*/
	extern int wdt_init(void);
	wdt_init();
	bk_wdt_feed();
	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t exit_tick          = 0ULL;
	exit_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	if(exit_tick - entry_tick < 0)
	{
		sleep_tick = 0ULL;
	}
	else
	{
		sleep_tick = exit_tick - entry_tick;
	}
	#else
	sleep_tick = 0ULL;
	#endif
	#if CONFIG_CKMN
	bk_rosc_32k_ckest_prog(32);
	#endif
	pm_enable_int(int_level);
	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_TICK_COMP,0x0,0x0);
	return sleep_tick;
}

uint64_t pm_low_voltage_process()
{
	volatile uint32_t int_level = 0;
	uint64_t sleep_tick         = 0ULL;

	int_level = pm_disable_int();
	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t entry_tick         = 0ULL;
	entry_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	#endif
	if(pm_low_voltage_resource_set() != 0)
	{
		pm_enable_int(int_level);
		return sleep_tick;
	}

	/*Debug pd,lpo,psram start*/
	pm_debug_lowvol_sleep_hook();
	/*Debug pd,lpo,psram end*/

	/* Execute pre-sleep callbacks */
	bk_pm_pre_sleep_callback_execute();

	pm_enter_low_voltage();

	/* Execute post-sleep (wakeup) callbacks */
	bk_pm_post_sleep_callback_execute();

	/*Debug pd,lpo,psram start*/
	pm_debug_low_vol_wakeup_hook();
	/*Debug pd,lpo,psram end*/

	pm_low_voltage_resource_restore();

	if (pm_debug_mode() & 0x2)
		LOGD(NULL, "low voltage int open before\r\n");

#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t exit_tick          = 0ULL;
	exit_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	if(exit_tick - entry_tick < 0)
	{
		sleep_tick = 0ULL;
	}
	else
	{
		sleep_tick = exit_tick - entry_tick;
	}
#endif
	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_TICK_COMP,0x0,0x0);
	pm_enable_int(int_level);

	/* Execute post-sleep (wakeup) callbacks */
	bk_pm_post_sleep_callback_execute();

	if (pm_debug_mode() & 0x2)
		LOGI("low voltage int open after 0x%x 0x%x \r\n", /*int_mie,*/ int_level, bk_pm_lp_vol_get());

	return sleep_tick;
}

void pm_deep_sleep_prepare()
{
	for (uint8_t i = 0; i < PM_SLEEP_CB_IND_PRI_1; i++)
	{
		if (s_pm_deepsleep_enter_cb_conf[i].cfg.cb != NULL)
		{
			s_pm_deepsleep_enter_cb_conf[i].cfg.cb(0, s_pm_deepsleep_enter_cb_conf[i].cfg.args);
		}
	}
}

uint64_t pm_deep_sleep_process()
{
	uint64_t sleep_tick         = 0ULL;
	volatile uint32_t int_level = 0;
	int_level = pm_disable_int();
	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t entry_tick         = 0ULL;
	entry_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	#endif

	for (uint8_t i = s_pm_deepsleep_enter_cb_cnt[PM_CB_PRIORITY_1]; i < PM_DEEPSLEEP_CB_SIZE; i++)
	{
		if (s_pm_deepsleep_enter_cb_conf[i].cfg.cb != NULL)
		{
			s_pm_deepsleep_enter_cb_conf[i].cfg.cb(0, s_pm_deepsleep_enter_cb_conf[i].cfg.args);
		}
	}

#if CONFIG_GPIO_WAKEUP_SUPPORT
	gpio_enter_low_power((void *)0);
#endif

	if (bk_pm_wakeup_source_get() & (0x1 << PM_WAKEUP_SOURCE_INT_RTC))
	{
		pm_rtc_wakeup_deep_sleep();
	}

	if (bk_pm_wakeup_source_get() & (0x1 << PM_WAKEUP_SOURCE_INT_TOUCHED))
	{
		pm_touched_wakeup_deep_sleep();
	}

	/* Execute pre-sleep callbacks (after legacy callbacks) */
	bk_pm_pre_sleep_callback_execute();

	pm_enter_deep_sleep();

	/* Execute post-sleep (wakeup) callbacks (first thing after wakeup) */
	bk_pm_post_sleep_callback_execute();
	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t exit_tick          = 0ULL;
	exit_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	if(exit_tick - entry_tick < 0)
	{
		sleep_tick = 0ULL;
	}
	else
	{
		sleep_tick = exit_tick - entry_tick;
	}
	#else
		sleep_tick = 0ULL;
	#endif

	pm_enable_int(int_level);
	return sleep_tick;	
}

#if CONFIG_PM_SUPER_DEEP_SLEEP
void pm_super_deep_sleep_prepare()
{
	for (uint8_t i = 0; i < PM_SLEEP_CB_IND_PRI_1; i++)
	{
		if (s_pm_superdeep_enter_cb_conf[i].cfg.cb != NULL)
		{
			// BK_LOGD(NULL, "%d %d\r\n", i, s_pm_superdeep_enter_cb_conf[i].id);
			s_pm_superdeep_enter_cb_conf[i].cfg.cb(0, s_pm_superdeep_enter_cb_conf[i].cfg.args);
		}
	}
}

void pm_super_deep_sleep_process()
{
	for (uint8_t i = s_pm_superdeep_enter_cb_cnt[PM_CB_PRIORITY_1]; i < PM_DEEPSLEEP_CB_SIZE; i++)
	{
		if (s_pm_superdeep_enter_cb_conf[i].cfg.cb != NULL)
		{
			s_pm_superdeep_enter_cb_conf[i].cfg.cb(0, s_pm_superdeep_enter_cb_conf[i].cfg.args);
		}
	}

#if CONFIG_GPIO_RETENTION_SUPPORT
	gpio_retention_sync(false);
#endif

#if CONFIG_GPIO_WAKEUP_SUPPORT
	uint64_t skip_io = BIT64(16); // workaround fix for super deep gpio wakeup
#if CONFIG_GPIO_RETENTION_SUPPORT
	skip_io |= gpio_retention_map_get();
#endif
	gpio_hal_switch_to_low_power_status(skip_io);
#endif

	pm_enter_super_deep_sleep();
}
#endif
/*=========================SLEEP/WAKEUP FUNCTION END========================*/

/*=========================LOW_VOLTAGE_PS PRIVATE FUNCTION START========================*/

static int pm_low_voltage_resource_set()
{
	pm_dev_id_e dev_id = 0;
	#if !CONFIG_PM_CLIENT
	#if CONFIG_PSRAM
	pm_psram_malloc_state_and_power_ctrl();
	#endif
	pm_lv_enter_time_out_clear();
	#if CONFIG_PM_LV_WDT_PROTECTION
		#if CONFIG_AON_WDT
		bk_aon_wdt_feed();
		#endif
	#endif
	#endif

	bk_pm_exit_low_vol_wakeup_source_clear();

	for (dev_id = 0; dev_id < PM_DEV_ID_MAX; dev_id++)
	{
		if (s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_ENTER_LOWVOL_INDEX][dev_id].cb != NULL)
		{
			s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_ENTER_LOWVOL_INDEX][dev_id].cb(0, s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_ENTER_LOWVOL_INDEX][dev_id].args);
		}
	}

#if CONFIG_PM_PROTECT_TIME_CHECK
	#if CONFIG_AON_RTC || CONFIG_ANA_RTC
	uint64_t current_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	#endif
	if (0 == bk_pm_module_power_state_get(POWER_MODULE_NAME_BTSP))
	{
		/*When check bt wakeup time comming ,return, not sleep*/
		if (!pm_check_protect_time(current_tick, current_tick))
		{
			for (dev_id = 0; dev_id < PM_DEV_ID_MAX; dev_id++)
			{
				if (s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].cb != NULL)
				{
					s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].cb(0, s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].args);
				}
			}
			return -1;
		}
	}
#endif

#if CONFIG_GPIO_WAKEUP_SUPPORT
	gpio_enter_low_power((void *)0);
#endif

	if (bk_pm_wakeup_source_get() & (0x1 << PM_WAKEUP_SOURCE_INT_TOUCHED))
	{
		pm_touched_wakeup_low_voltage();
	}

#if CONFIG_BAKP_POWER_DOMAIN_PM_CONTROL
	bk_pm_module_vote_power_ctrl(POWER_SUB_MODULE_NAME_BAKP_PM, PM_POWER_MODULE_STATE_OFF);
#endif

#if CONFIG_PHY_PWD
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_BT, PM_POWER_MODULE_STATE_OFF);
#endif
	/*stop wdt to prevent wdt timeout*/
	bk_wdt_stop();
	close_wdt();

	/*flash line mode 4->2 when enter low voltage*/
	bk_flash_set_line_mode(2);

	return 0;
}

void pm_low_voltage_bsp_restore(void)
{
	/*flash line mode 2->4 when exit low voltage*/
	bk_flash_set_line_mode(bk_flash_get_line_mode());

#if CONFIG_CKMN
	bk_rosc_32k_ckest_prog(32);
#endif
	/*restore wdt*/
	extern int wdt_init(void);
	wdt_init();
	bk_wdt_feed();

#if CONFIG_PM_LV_WDT_PROTECTION
	#if CONFIG_AON_WDT
		bk_aon_wdt_stop();
	#endif
#endif

	bk_pm_exit_low_vol_wakeup_source_set();
}

static void pm_low_voltage_resource_restore()
{
	pm_dev_id_e dev_id = 0;

#if CONFIG_BAKP_POWER_DOMAIN_PM_CONTROL
	bk_pm_module_vote_power_ctrl(POWER_SUB_MODULE_NAME_BAKP_PM, PM_POWER_MODULE_STATE_ON);
#endif

#if CONFIG_GPIO_WAKEUP_SUPPORT
	gpio_exit_low_power((void *)0);
#endif

	for (dev_id = 0; dev_id < PM_DEV_ID_MAX; dev_id++)
	{
		if (s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].cb != NULL)
		{
			s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].cb(0, s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].args);
		}
	}
}

uint64_t bk_pm_module_lv_sleep_state_get(pm_dev_id_e module)
{
	return s_pm_module_lv_sleep_state & (0x1ULL << module);
}
__attribute__((section(".itcm_sec_code"))) bk_err_t bk_pm_module_lv_sleep_state_set()
{
	s_pm_module_lv_sleep_state = 0xFFFFFFFFFFFFFFFF;
	return BK_OK;
}
bk_err_t bk_pm_module_lv_sleep_state_clear(pm_dev_id_e module)
{
	s_pm_module_lv_sleep_state &= ~(0x1ULL << module);
	return BK_OK;
}

bk_err_t pm_debug_lv_state(void)
{
	LOGD(NULL, "pm_module_lv_sleep_state:0x%llx\r\n",s_pm_module_lv_sleep_state);
	return BK_OK;
}

bk_err_t bk_pm_enter_lv_time_out_register_callback(pm_enter_lv_timeout_cb_t* lv_timeout_cb)
{
	bk_err_t ret = BK_OK;
	if(lv_timeout_cb == NULL)
	{
		return BK_FAIL;
	}
	if(lv_timeout_cb->module >= PM_ENTER_LV_TIME_OUT_MODULE_MAX)
	{
		return BK_FAIL;
	}
	s_pm_lv_timeout_cb_arr[lv_timeout_cb->module].module = lv_timeout_cb->module;
	s_pm_lv_timeout_cb_arr[lv_timeout_cb->module].cfg.cb = lv_timeout_cb->cfg.cb;
	s_pm_lv_timeout_cb_arr[lv_timeout_cb->module].cfg.args = lv_timeout_cb->cfg.args;
	return ret;
}

bk_err_t bk_pm_check_enter_lv_time_out()
{
	bk_err_t ret = BK_OK;
	uint64_t cur_tick = 0;
	if (true == bk_pm_module_sleep_state_get(PM_SLEEP_MODULE_NAME_APP))
	{
		cur_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
		if(s_pm_check_lv_enter_time_out == 0)
		{
			s_pm_check_lv_enter_time_out = cur_tick;
		}
		else
		{
			if((cur_tick - s_pm_check_lv_enter_time_out) > CONFIG_PM_ENTER_LV_TIME_OUT_PERIOD_MS*AON_RTC_MS_TICK_CNT)
			{
				for(int index = 0; index < PM_ENTER_LV_TIME_OUT_MODULE_MAX; index++)
				{
					if(s_pm_lv_timeout_cb_arr[index].cfg.cb != NULL)
					{
						s_pm_lv_timeout_cb_arr[index].cfg.cb(s_pm_lv_timeout_cb_arr[index].cfg.args);
					}
				}
			}
		}
	}
	return ret;
}

bk_err_t pm_lv_enter_time_out_clear()
{
	s_pm_check_lv_enter_time_out = 0;
	return BK_OK;
}
/*=========================LOW_VOLTAGE_PS PRIVATE FUNCTION END========================*/

/*=========================SLEEP CB REGISTER API START========================*/
static void pm_sleep_cb_push_item(pm_sleep_cb_t cb_arr[], uint8_t *cb_cnt_p, pm_sleep_cb_t cb_item)
{
	if (*cb_cnt_p == PM_DEEPSLEEP_CB_SIZE)
	{
		if (pm_debug_mode() & 0x2)
			LOGD(NULL, "call back function overflow, dev %d regist fail!\r\n", cb_item.id);
		if (pm_debug_mode() & 0x1)
		{
			LOGD(NULL, "cb functions dump: [ ");
			for (uint8_t i = 0; i < PM_DEEPSLEEP_CB_SIZE; i++)
				LOGD(NULL, "%d ", cb_arr[i].id);
			LOGD(NULL, "]\r\n");
		}
		return;
	} else if (cb_arr[*cb_cnt_p].cfg.cb != NULL) {
		if (pm_debug_mode() & 0x2)
			LOGD(NULL, "cb functions have overlap warning, dev %d -> %d\r\n", cb_item.id, cb_arr[*cb_cnt_p].id);
	}

	for (uint8_t i = 0; i < *cb_cnt_p; i++)
	{
		if (cb_arr[i].id == cb_item.id)
		{
			cb_arr[i].cfg.cb   = cb_item.cfg.cb;
			cb_arr[i].cfg.args = cb_item.cfg.args;
			return;
		}
	}

	cb_arr[*cb_cnt_p].id       = cb_item.id;
	cb_arr[*cb_cnt_p].cfg.cb   = cb_item.cfg.cb;
	cb_arr[*cb_cnt_p].cfg.args = cb_item.cfg.args;

	(*cb_cnt_p)++;
}

static void pm_sleep_cb_pop_item(pm_sleep_cb_t cb_arr[], uint8_t *cb_cnt_p, uint8_t item_ind)
{
	uint8_t tail_ind = (*cb_cnt_p) - 1;

	cb_arr[item_ind].id        = cb_arr[tail_ind].id;
	cb_arr[item_ind].cfg.cb    = cb_arr[tail_ind].cfg.cb;
	cb_arr[item_ind].cfg.args  = cb_arr[tail_ind].cfg.args;

	cb_arr[tail_ind].id       = PM_DEV_ID_MAX;
	cb_arr[tail_ind].cfg.cb   = NULL;
	cb_arr[tail_ind].cfg.args = NULL;

	(*cb_cnt_p)--;
}

bk_err_t bk_pm_sleep_register_cb(pm_sleep_mode_e sleep_mode, pm_dev_id_e dev_id, pm_cb_conf_t *enter_config, pm_cb_conf_t *exit_config)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	if (sleep_mode == PM_MODE_LOW_VOLTAGE)
	{
		if (enter_config != NULL)
		{
			if ((enter_config->cb != NULL) && (dev_id < PM_DEV_ID_MAX))
			{
				s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_ENTER_LOWVOL_INDEX][dev_id].cb = enter_config->cb;
				s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_ENTER_LOWVOL_INDEX][dev_id].args = enter_config->args;
			}
		}

		if (exit_config != NULL)
		{
			if ((exit_config->cb != NULL) && (dev_id < PM_DEV_ID_MAX))
			{
				s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].cb = exit_config->cb;
				s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].args = exit_config->args;
			}
		}
	}
	else if (sleep_mode == PM_MODE_DEEP_SLEEP)
	{
		if (enter_config != NULL)
		{
			pm_sleep_cb_t cb_item = {dev_id, *enter_config};
			// use exit_config to indicate the execution priority when entering sleep
			if ((enter_config->cb != NULL) && (exit_config != NULL) && ((pm_cb_priority_e)exit_config->args < PM_CB_PRIORITY_MAX))
			{
				pm_sleep_cb_push_item(s_pm_deepsleep_enter_cb_conf, &s_pm_deepsleep_enter_cb_cnt[(pm_cb_priority_e)exit_config->args], cb_item);
			}
			else if ((enter_config->cb != NULL) && (dev_id < PM_DEV_ID_DEFAULT))
			{
				pm_sleep_cb_push_item(s_pm_deepsleep_enter_cb_conf, &s_pm_deepsleep_enter_cb_cnt[PM_CB_PRIORITY_0], cb_item);
			}
			else if ((enter_config->cb != NULL) && (dev_id < PM_DEV_ID_MAX))
			{
				pm_sleep_cb_push_item(s_pm_deepsleep_enter_cb_conf, &s_pm_deepsleep_enter_cb_cnt[PM_CB_PRIORITY_1], cb_item);
			}
		}
	}
#if CONFIG_PM_SUPER_DEEP_SLEEP
	else if (sleep_mode == PM_MODE_SUPER_DEEP_SLEEP)
	{
		if (enter_config != NULL)
		{
			pm_sleep_cb_t cb_item = {dev_id, *enter_config};
			if ((enter_config->cb != NULL) && (dev_id < PM_DEV_ID_DEFAULT))
			{
				pm_sleep_cb_push_item(s_pm_superdeep_enter_cb_conf, &s_pm_superdeep_enter_cb_cnt[PM_CB_PRIORITY_0], cb_item);
			}
			else if ((enter_config->cb != NULL) && (dev_id < PM_DEV_ID_MAX))
			{
				pm_sleep_cb_push_item(s_pm_superdeep_enter_cb_conf, &s_pm_superdeep_enter_cb_cnt[PM_CB_PRIORITY_1], cb_item);
			}
		}
	}
#endif
	else
	{
		LOGD(NULL, "The sleep mode[%d] not support register call back \r\n", sleep_mode);
	}
	GLOBAL_INT_RESTORE();
	return BK_OK;
}

bk_err_t bk_pm_sleep_unregister_cb(pm_sleep_mode_e sleep_mode, pm_dev_id_e dev_id, bool enter_cb, bool exit_cb)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	if (sleep_mode == PM_MODE_LOW_VOLTAGE)
	{
		if ((enter_cb == true) && (dev_id < PM_DEV_ID_MAX))
		{
			s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_ENTER_LOWVOL_INDEX][dev_id].cb = NULL;
			s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_ENTER_LOWVOL_INDEX][dev_id].args = NULL;
		}

		if (exit_cb == true)
		{
			s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].cb = NULL;
			s_pm_lowvol_enter_exit_cb_conf[PM_SLEEP_CB_EXIT_LOWVOL_INDEX][dev_id].args = NULL;
		}
	}
	else if (sleep_mode == PM_MODE_DEEP_SLEEP)
	{
		if ((enter_cb == true) && (dev_id < PM_DEV_ID_DEFAULT))
		{
			for (uint8_t i = PM_SLEEP_CB_IND_PRI_0; i < PM_SLEEP_CB_IND_PRI_1; i++)
			{
				if (s_pm_deepsleep_enter_cb_conf[i].id == dev_id)
				{
					pm_sleep_cb_pop_item(s_pm_deepsleep_enter_cb_conf, &s_pm_deepsleep_enter_cb_cnt[PM_CB_PRIORITY_0], i);
					break;
				}
			}
		}
		else if ((enter_cb == true) && (dev_id < PM_DEV_ID_MAX))
		{
			for (uint8_t i = PM_SLEEP_CB_IND_PRI_1; i < PM_DEEPSLEEP_CB_SIZE; i++)
			{
				if (s_pm_deepsleep_enter_cb_conf[i].id == dev_id)
				{
					pm_sleep_cb_pop_item(s_pm_deepsleep_enter_cb_conf, &s_pm_deepsleep_enter_cb_cnt[PM_CB_PRIORITY_1], i);
					break;
				}
			}
		}
	}
#if CONFIG_PM_SUPER_DEEP_SLEEP
	else if (sleep_mode == PM_MODE_SUPER_DEEP_SLEEP)
	{
		if ((enter_cb == true) && (dev_id < PM_DEV_ID_DEFAULT))
		{
			for (uint8_t i = PM_SLEEP_CB_IND_PRI_0; i < PM_SLEEP_CB_IND_PRI_1; i++)
			{
				if (s_pm_superdeep_enter_cb_conf[i].id == dev_id)
				{
					pm_sleep_cb_pop_item(s_pm_superdeep_enter_cb_conf, &s_pm_superdeep_enter_cb_cnt[PM_CB_PRIORITY_0], i);
					break;
				}
			}
		}
		else if ((enter_cb == true) && (dev_id < PM_DEV_ID_MAX))
		{
			for (uint8_t i = PM_SLEEP_CB_IND_PRI_1; i < PM_DEEPSLEEP_CB_SIZE; i++)
			{
				if (s_pm_superdeep_enter_cb_conf[i].id == dev_id)
				{
					pm_sleep_cb_pop_item(s_pm_superdeep_enter_cb_conf, &s_pm_superdeep_enter_cb_cnt[PM_CB_PRIORITY_1], i);
					break;
				}
			}
		}
	}
#endif
	else
	{
		LOGD(NULL, "The sleep mode[%d] not support unregister call back \r\n", sleep_mode);
	}
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_pm_light_sleep_register_cb(pm_cb_conf_t *enter_config, pm_cb_conf_t *exit_config)
{
#if !defined(CONFIG_PM_LIGHT_SLEEP)
	return BK_ERR_NOT_SUPPORT;
#else
	if (enter_config != NULL)
	{
		s_pm_light_sleep_enter_cb_conf.cb = enter_config->cb;
		s_pm_light_sleep_enter_cb_conf.args = enter_config->args;
	}
	if (exit_config != NULL)
	{
		s_pm_light_sleep_exit_cb_conf.cb = exit_config->cb;
		s_pm_light_sleep_exit_cb_conf.args = exit_config->args;
	}
	return BK_OK;
#endif
}

bk_err_t bk_pm_light_sleep_unregister_cb(bool enter_cb, bool exit_cb)
{
#if !defined(CONFIG_PM_LIGHT_SLEEP)
	return BK_ERR_NOT_SUPPORT;
#else
	if (enter_cb)
	{
		s_pm_light_sleep_enter_cb_conf.cb = NULL;
	}

	if (exit_cb)
	{
		s_pm_light_sleep_exit_cb_conf.cb = NULL;
	}

	return BK_OK;
#endif
}
/*=========================SLEEP CB REGISTER API END========================*/

/*=========================WIFI ALARM START========================*/

bk_err_t bk_pm_wifi_rtc_set(uint32_t tick, void *callback)
{
	bk_err_t ret = BK_OK;
	alarm_info_t low_valtage_alarm = {
									PM_WIFI_RTC_ALARM_NAME,
									tick,
									1,
									callback,
									NULL
									};
	//force unregister previous if doesn't finish.
	bk_alarm_unregister(AON_RTC_ID_1, low_valtage_alarm.name);
	ret = bk_alarm_register(AON_RTC_ID_1, &low_valtage_alarm);
	if(ret < 0)
	{
		LOGI(NULL, "wifi_rtc register failed, ret:%d\r\n", ret);
		return ret;
	}
	bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_RTC, NULL);
	return ret;
}

bk_err_t bk_pm_wifi_rtc_clear(void)
{
	bk_err_t ret = BK_OK;
	ret = bk_alarm_unregister(AON_RTC_ID_1, (uint8_t *)PM_WIFI_RTC_ALARM_NAME);
	return ret;
}
/*=========================WIFI ALARM END========================*/

/*=========================ENTER SLEEP FUNCTION START========================*/
static void pm_enter_cpu_wfi()
{
	sys_drv_enter_cpu_wfi();
}

static void pm_enter_normal_sleep()
{
	sys_drv_enter_normal_sleep(0);
}

static void pm_enter_low_voltage()
{
	sys_drv_enter_low_voltage();
}

static void pm_enter_deep_sleep()
{
	sys_drv_enter_deep_sleep(NULL);
}

#if CONFIG_PM_SUPER_DEEP_SLEEP
static void pm_enter_super_deep_sleep()
{
	uint8_t use_super_deep = 1;
	bk_misc_set_reset_reason(RESET_SOURCE_SUPER_DEEP);
	sys_drv_enter_deep_sleep(&use_super_deep);
}
#endif
/*=========================ENTER SLEEP FUNCTION END========================*/
