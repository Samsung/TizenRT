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
#include "pm_interface.h"

#include "pm_debug.h"

/* 1. 核心电源管理API */
uint64_t bk_pm_suppress_ticks_and_sleep(uint32_t sleep_ticks)
{
#if CONFIG_PM_ENABLE
	return pm_management(sleep_ticks);
#endif
}
#if CONFIG_PM_WAKE_FUNC

void bk_pm_enter_sleep(void)
{
	uint32_t sleep_ticks = 0;
	LOGD("bk_pm_enter_sleep: sleep_ticks=%d (weak default)\n", sleep_ticks);
	bk_pm_suppress_ticks_and_sleep(sleep_ticks);
}
/* 用于 CONFIG_PM_ENABLE=n 和 CONFIG_PM_CLIENT=y 时的默认实现 */
/*=====================WEAK FUNCTION START=====================*/
/* 2. MCU电源管理 */
bk_err_t __attribute__((weak)) bk_pm_mcu_pm_ctrl(uint32_t power_state)
{
	LOGD("bk_pm_mcu_pm_ctrl: power_state=%d (weak default)\n", power_state);
	return BK_OK;
}

uint32_t __attribute__((weak)) bk_pm_mcu_pm_state_get(void)
{
	LOGD("bk_pm_mcu_pm_state_get: returning 0x1 (weak default)\n");
	return 0x1; /* 禁用MCU电源管理 */
}

/* 3. 睡眠模式管理 */
bk_err_t __attribute__((weak)) bk_pm_sleep_mode_set(pm_sleep_mode_e sleep_mode)
{
	LOGD("bk_pm_sleep_mode_set: sleep_mode=%d (weak default)\n", sleep_mode);
	return BK_OK;
}

/* 4. 唤醒源管理 */
bk_err_t __attribute__((weak)) bk_pm_wakeup_source_set(pm_wakeup_source_e wakeup_source, void* source_param)
{
	LOGD("bk_pm_wakeup_source_set: wakeup_source=%d, source_param=%p (weak default)\n", wakeup_source, source_param);
	return BK_OK;
}

pm_wakeup_source_e __attribute__((weak)) bk_pm_deep_sleep_wakeup_source_get(void)
{
	LOGD("bk_pm_deep_sleep_wakeup_source_get: returning PM_WAKEUP_SOURCE_INT_NONE (weak default)\n");
	return PM_WAKEUP_SOURCE_INT_NONE;
}

pm_wakeup_source_e __attribute__((weak)) bk_pm_exit_low_vol_wakeup_source_get(void)
{
	LOGD("bk_pm_exit_low_vol_wakeup_source_get: returning PM_WAKEUP_SOURCE_INT_NONE (weak default)\n");
	return PM_WAKEUP_SOURCE_INT_NONE;
}

bk_err_t __attribute__((weak)) bk_pm_exit_low_vol_wakeup_source_clear(void)
{
	LOGD("bk_pm_exit_low_vol_wakeup_source_clear: (weak default)\n");
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_exit_low_vol_wakeup_source_set(void)
{
	LOGD("bk_pm_exit_low_vol_wakeup_source_set: (weak default)\n");
	return BK_OK;
}

/* 5. 模块投票控制 */
bk_err_t __attribute__((weak)) bk_pm_module_vote_sleep_ctrl(pm_sleep_module_name_e module, uint32_t sleep_state, uint32_t sleep_time)
{
	LOGD("bk_pm_module_vote_sleep_ctrl: module=%d, sleep_state=%d, sleep_time=%d (weak default)\n", module, sleep_state, sleep_time);
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_module_vote_power_ctrl(pm_power_module_name_e module, pm_power_module_state_e power_state)
{
	LOGD("bk_pm_module_vote_power_ctrl: module=%d, power_state=%d (weak default)\n", module, power_state);
	return BK_OK;
}

/* 6. 时钟和频率管理 */
bk_err_t __attribute__((weak)) bk_pm_module_vote_cpu_freq(pm_dev_id_e module, pm_cpu_freq_e cpu_freq)
{
	LOGD("bk_pm_module_vote_cpu_freq: module=%d, cpu_freq=%d (weak default)\n", module, cpu_freq);
	return BK_OK;
}

pm_cpu_freq_e __attribute__((weak)) bk_pm_module_current_cpu_freq_get(pm_dev_id_e module)
{
	LOGD("bk_pm_module_current_cpu_freq_get: module=%d, returning PM_CPU_FRQ_DEFAULT (weak default)\n", module);
	return PM_CPU_FRQ_DEFAULT;
}

pm_cpu_freq_e __attribute__((weak)) bk_pm_current_max_cpu_freq_get(void)
{
	LOGD("bk_pm_current_max_cpu_freq_get: returning PM_CPU_FRQ_DEFAULT (weak default)\n");
	return PM_CPU_FRQ_DEFAULT;
}

bk_err_t __attribute__((weak)) bk_pm_clock_ctrl(pm_dev_clk_e module, pm_dev_clk_pwr_e clock_state)
{
	LOGD("bk_pm_clock_ctrl: module=%d, clock_state=%d (weak default)\n", module, clock_state);
	return BK_OK;
}

/* 7. 电压管理 */
bk_err_t __attribute__((weak)) bk_pm_lp_vol_set(uint32_t lp_vol)
{
	LOGD("bk_pm_lp_vol_set: lp_vol=%d (weak default)\n", lp_vol);
	return BK_OK;
}

uint32_t __attribute__((weak)) bk_pm_lp_vol_get(void)
{
	LOGD("bk_pm_lp_vol_get: returning 0 (weak default)\n");
	return 0; /* 默认0.6V */
}

bk_err_t __attribute__((weak)) bk_pm_rf_tx_vol_set(uint32_t tx_vol)
{
	LOGD("bk_pm_rf_tx_vol_set: tx_vol=%d (weak default)\n", tx_vol);
	return BK_OK;
}

uint32_t __attribute__((weak)) bk_pm_rf_tx_vol_get(void)
{
	LOGD("bk_pm_rf_tx_vol_get: returning 0 (weak default)\n");
	return 0; /* 默认1.25V */
}

bk_err_t __attribute__((weak)) bk_pm_rf_rx_vol_set(uint32_t rx_vol)
{
	LOGD("bk_pm_rf_rx_vol_set: rx_vol=%d (weak default)\n", rx_vol);
	return BK_OK;
}

uint32_t __attribute__((weak)) bk_pm_rf_rx_vol_get(void)
{
	LOGD("bk_pm_rf_rx_vol_get: returning 0 (weak default)\n");
	return 0; /* 默认1.25V */
}

/* 9. 32K时钟源管理 */
bk_err_t __attribute__((weak)) bk_pm_lpo_src_set(pm_lpo_src_e lpo_src)
{
	LOGD("bk_pm_lpo_src_set: lpo_src=%d (weak default)\n", lpo_src);
	return BK_OK;
}

pm_lpo_src_e __attribute__((weak)) bk_pm_lpo_src_get(void)
{
	LOGD("bk_pm_lpo_src_get: returning PM_LPO_SRC_X32K (weak default)\n");
	return PM_LPO_SRC_X32K;
}

bk_err_t __attribute__((weak)) pm_clk_32k_source_switch(pm_lpo_src_e lpo_src)
{
	return BK_OK;
}

bk_err_t __attribute__((weak)) pm_extern32k_register_cb(pm_cb_extern32k_cfg_t *cfg)
{
	return BK_OK;
}

bk_err_t __attribute__((weak)) pm_extern32k_unregister_cb(pm_cb_extern32k_cfg_t *cfg)
{
	return BK_OK;
}

/* 9. 模块状态查询 */
int32_t __attribute__((weak)) bk_pm_module_sleep_state_get(pm_sleep_module_name_e module)
{
	return 0; /* 默认active状态 */
}

int32_t __attribute__((weak)) bk_pm_module_power_state_get(pm_power_module_name_e module)
{
	return 0; /* 默认ON状态 */
}

uint32_t __attribute__((weak)) bk_pm_get_video_vote_pwr_state(void)
{
	return 0;
}

uint32_t __attribute__((weak)) bk_pm_get_audio_vote_pwr_state(void)
{
	return 0;
}

/* 10. 回调函数注册 */
bk_err_t __attribute__((weak)) bk_pm_sleep_register_cb(pm_sleep_mode_e sleep_mode, pm_dev_id_e dev_id, pm_cb_conf_t *enter_config, pm_cb_conf_t *exit_config)
{
	LOGD("bk_pm_sleep_register_cb: sleep_mode=%d, dev_id=%d (weak default)\n", sleep_mode, dev_id);
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_sleep_unregister_cb(pm_sleep_mode_e sleep_mode, pm_dev_id_e dev_id, bool enter_cb, bool exit_cb)
{
	LOGD("bk_pm_sleep_unregister_cb: sleep_mode=%d, dev_id=%d, enter_cb=%d, exit_cb=%d (weak default)\n", sleep_mode, dev_id, enter_cb, exit_cb);
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_light_sleep_register_cb(pm_cb_conf_t *enter_config, pm_cb_conf_t *exit_config)
{
	LOGD("bk_pm_light_sleep_register_cb: enter_config=%p, exit_config=%p (weak default)\n", enter_config, exit_config);
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_light_sleep_unregister_cb(bool enter_cb, bool exit_cb)
{
	LOGD("bk_pm_light_sleep_unregister_cb: enter_cb=%d, exit_cb=%d (weak default)\n", enter_cb, exit_cb);
	return BK_OK;
}

/* 11. 低电压超时管理 */
bk_err_t __attribute__((weak)) bk_pm_enter_lv_time_out_register_callback(pm_enter_lv_timeout_cb_t* lv_timeout_cb)
{
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_check_enter_lv_time_out(void)
{
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_module_lv_sleep_state_set(void)
{
	PM_LOGD("bk_pm_module_lv_sleep_state_set: (weak default)\n");
	return BK_OK;
}

uint64_t __attribute__((weak)) bk_pm_module_lv_sleep_state_get(pm_dev_id_e module)
{
	LOGD("bk_pm_module_lv_sleep_state_get: module=%d, returning 0 (weak default)\n", module);
	return 0;
}

bk_err_t __attribute__((weak)) bk_pm_module_lv_sleep_state_clear(pm_dev_id_e module)
{
	LOGD("bk_pm_module_lv_sleep_state_clear: module=%d (weak default)\n", module);
	return BK_OK;
}

/* 12. 自动电源管理 */
uint32_t __attribute__((weak)) bk_pm_cp1_auto_power_down_state_get(void)
{
	return 0; /* 默认禁用 */
}

bk_err_t __attribute__((weak)) bk_pm_cp1_auto_power_down_state_set(uint32_t value)
{
	return BK_OK;
}

uint32_t __attribute__((weak)) bk_pm_cp1_boot_flag_get(void)
{
	return 0; /* 默认未启动 */
}

pm_mem_auto_ctrl_e __attribute__((weak)) bk_pm_mem_auto_power_down_state_get(void)
{
	LOGD("bk_pm_mem_auto_power_down_state_get: returning PM_MEM_AUTO_CTRL_DISABLE (weak default)\n");
	return PM_MEM_AUTO_CTRL_DISABLE;
}

bk_err_t __attribute__((weak)) bk_pm_mem_auto_power_down_state_set(pm_mem_auto_ctrl_e value)
{
	LOGD("bk_pm_mem_auto_power_down_state_set: value=%d (weak default)\n", value);
	return BK_OK;
}

/* 13. 校准和特殊功能 */
bk_err_t __attribute__((weak)) bk_pm_rosc_calibration(pm_rosc_cali_mode_e rosc_cali_mode, uint32_t cali_interval)
{
	return BK_OK;
}

bk_err_t __attribute__((weak)) bk_pm_external_ldo_ctrl(uint32_t value)
{
	return BK_OK;
}

bool __attribute__((weak)) bk_pm_phy_cali_state_get(void)
{
	return 0; /* 默认未校准 */
}

bool __attribute__((weak)) bk_pm_phy_reinit_flag_get(void)
{
	return false; /* 默认未重初始化 */
}

void __attribute__((weak)) bk_pm_phy_reinit_flag_clear(void)
{
	/* 空实现 */
}

uint32_t __attribute__((weak)) bk_pm_phy_pm_state_get(void)
{
	return 0; /* 默认关闭 */
}

uint32_t __attribute__((weak)) bk_pm_wakeup_from_lowvol_consume_time_get(void)
{
	return 0;
}

/* 14. WiFi专用接口 */
void __attribute__((weak)) bk_pm_wifi_rtc_set(uint32_t tick, void *callback)
{
	/* 空实现 */
}

void __attribute__((weak)) bk_pm_wifi_rtc_clear(void)
{
	/* 空实现 */
}

/* 15. 深度睡眠模块配置 */
bk_err_t __attribute__((weak)) bk_pm_clear_deep_sleep_modules_config(pm_power_module_name_e module_name)
{
	return BK_OK;
}

/* 16. 硬件初始化 */
void __attribute__((weak)) pm_hardware_init(void)
{
	/* 空实现 */
}

/* 17. Mailbox 初始化 */
bk_err_t __attribute__((weak)) bk_pm_mailbox_init(void)
{
	return BK_OK;
}
#endif
/*=====================WEAK FUNCTION END=====================*/
