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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <common/bk_include.h>
#include <common/bk_compiler.h>
#include <os/mem.h>
#include <driver/pwm.h>
#include "pwm_hal_v2p2.h"
#include "pwm_driver.h"
#include "clock_driver.h"
#include "power_driver.h"
#include <driver/int.h>
#include "sys_driver.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#if defined(CONFIG_PWM_PM_CB_SUPPORT)
#include <modules/pm.h>
#endif

#define PWM_CAPTURE_CACHE_NUM           50

typedef struct {
	pwm_chan_t chan1;
	pwm_chan_t chan2;
	bool is_valid;
} pwm_group_info_t;

typedef struct {
	uint32_t chan_init_bits;
	uint32_t chan_init_signal_level;
	uint32_t group_chan_init_level;
#if defined(CONFIG_PWM_PHASE_SHIFT)
	volatile uint32_t phase_shift_chan_init_level;
#endif
	pwm_group_info_t groups[SOC_PWM_TIM_NUM];
#if defined(CONFIG_PWM_PM_CB_SUPPORT)
	uint32_t pm_backup[PWM_PM_BACKUP_REG_NUM];
	uint8_t pm_bakeup_is_valid;
#endif
} pwm_driver_t;

typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t gpio_dev;
} pwm_gpio_map_t;

typedef struct {
	pwm_chan_t sw_ch;
	pwm_chan_t tim_ch;
} pwm_tim_map_t;

typedef struct {
	uint8_t tim_id;
	pwm_chan_t tim_ch_p;
	pwm_chan_t tim_ch_n;
} pwm_group_tim_map_t;

typedef struct {
	pwm_chan_t sw_ch;
	pwm_chan_t tim_cap_ch;
} pwm_capture_tim_map_t;

typedef enum {
	PWM_SINGLE_CHAN_MODE = (1 << 0),
	PWM_GROUP_MODE = (1 << 1),
	PWM_PHASE_SHIFT_MODE = (1 << 2),
} pwm_func_mode_t;

#define PWM_RETURN_ON_NOT_INIT() do {\
	if (!s_pwm_driver_is_init) {\
		return BK_ERR_PWM_NOT_INIT;\
	}\
} while(0)

#define PWM_RETURN_ON_CHAN_NOT_INIT(sw_ch) do {\
	if (!(s_pwm.chan_init_bits & BIT((sw_ch)))) {\
		return BK_ERR_PWM_CHAN_NOT_INIT;\
	}\
} while(0)

#define PWM_RETURN_ON_INVALID_CHAN(sw_ch) do {\
	if ((sw_ch) >= SOC_PWM_CHAN_NUM_MAX) {\
		return BK_ERR_PWM_CHAN_ID;\
	}\
} while(0)

#define PWM_RETURN_ON_INVALID_CHAN_NUM(shift_config) do {\
	if (shift_config->chan_num < 2) {\
		return BK_ERR_PWM_PHASE_SHIFT_CHAN_NUM;\
	}\
} while(0)

#define PWM_PHASE_SHIFT_RETURN_ON_INVALID_DUTY(shift_config) do {\
	for (uint32_t index = 0; index < shift_config->chan_num; index++) {\
		if (shift_config->duty_config[index].duty_cycle > shift_config->period_cycle) {\
			PWM_LOGE("invalid duty cycle, index:%d, period_cycle:%d, duty_cycle:%d\r\n",\
					index,\
					shift_config->period_cycle,\
					shift_config->duty_config[index].duty_cycle);\
			return BK_ERR_PWM_PERIOD_DUTY;\
		}\
	}\
} while(0)

#if CONFIG_PWM_PM_CB_SUPPORT
#define PWM_PM_CHECK_RESTORE(id) do {\
	GLOBAL_INT_DECLARATION();\
	GLOBAL_INT_DISABLE();\
	if (bk_pm_module_lv_sleep_state_get(PM_DEV_ID_PWM_1)) {\
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_ON);\
		pwm_pm_restore(0, (void *)id);\
		bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_PWM_1);\
	}\
	GLOBAL_INT_RESTORE();\
} while(0)
#else
#define PWM_PM_CHECK_RESTORE(id)
#endif

static bool s_pwm_driver_is_init = false;
static pwm_driver_t s_pwm = {0};
static pwm_isr_t s_pwm_isr[SOC_PWM_CHAN_NUM_MAX] = {NULL};
static const pwm_gpio_map_t s_pwm_pin_id_map[SOC_PWM_CHAN_NUM_MAX] = GPIO_PWM_MAP_TABLE;
static uint8_t s_pwm_mode = 0;
#if defined(CONFIG_PWM_PHASE_SHIFT)
static pwm_chan_t s_phase_shift_multi_hw_ch = {0};
static uint32_t s_phase_shift_first_valid_ch = 0;
static pwm_phase_shift_config_t *s_shift_config = NULL;
#endif

static void pwm0_isr(void);

static void pwm_chan_init_gpio(pwm_chan_t sw_ch)
{
	gpio_dev_unmap(s_pwm_pin_id_map[sw_ch].gpio_id);
	gpio_dev_map(s_pwm_pin_id_map[sw_ch].gpio_id, s_pwm_pin_id_map[sw_ch].gpio_dev);
	bk_gpio_pull_up(s_pwm_pin_id_map[sw_ch].gpio_id);
}

static void pwm_chan_init_common(pwm_chan_t sw_ch)
{
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_UP);
	sys_drv_pwm_select_clock(SYS_SEL_PWM0, PWM_SCLK_XTAL);
	sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
	pwm_chan_init_gpio(sw_ch);
	s_pwm.chan_init_bits |= BIT(sw_ch);
}

static void pwm_chan_deinit_common(pwm_chan_t sw_ch)
{
	pwm_period_duty_config_t pwm_config = {0};

	pwm_config.period_cycle = 1;
	pwm_hal_set_chan_config(sw_ch, &pwm_config);
	pwm_hal_set_polarity(sw_ch, PWM_POLARITY_POSITIVE);
	pwm_hal_set_single_chan_enable(sw_ch, 0);
	s_pwm.chan_init_signal_level &= ~BIT(sw_ch);
	s_pwm.chan_init_bits &= ~BIT(sw_ch);
}

static inline bool pwm_driver_duty_is_valid(uint32_t period, uint32_t duty1, uint32_t duty2,
										uint32_t duty3)
{
	if (period == 0) {
		PWM_LOGE("period must be more than 0\r\n");
		return false;
	}

	// Not consider (duty1 + duty2 + duty3) overflow
	if ((duty1 + duty2 + duty3) > period) {
		PWM_LOGE("duty err, period:%d, duty1:%d, duty2:%d, duty3:%d\r\n", period, duty1, duty2, duty3);
		return false;
	}

	return true;
}

static pwm_group_t pwm_group_find(pwm_chan_t chan1, pwm_chan_t chan2)
{
	for (int i = 0; i < SOC_PWM_TIM_NUM; i++) {
		if (s_pwm.groups[i].is_valid) {
			if ((s_pwm.groups[i].chan1 == chan1) &&
				(s_pwm.groups[i].chan2 == chan2)) {
				return i;
			}

			if ((s_pwm.groups[i].chan1 == chan2) &&
				(s_pwm.groups[i].chan2 == chan1)) {
				return i;
			}
		}
	}

	return PWM_GROUP_ID_INVALID;
}

static pwm_group_t pwm_group_find_channel(pwm_chan_t sw_ch)
{
	for (int i = 0; i < SOC_PWM_TIM_NUM; i++) {
		if (s_pwm.groups[i].is_valid &&
		  ((s_pwm.groups[i].chan1 == sw_ch) || (s_pwm.groups[i].chan2 == sw_ch)))
			return i;
	}

	return PWM_GROUP_ID_INVALID;
}

static pwm_group_t pwm_group_add(pwm_chan_t chan1, pwm_chan_t chan2)
{
	for (uint32_t i = 0; i < SOC_PWM_TIM_NUM; i++) {
		if (!s_pwm.groups[i].is_valid) {
			s_pwm.groups[i].is_valid = true;
			s_pwm.groups[i].chan1 = chan1;
			s_pwm.groups[i].chan2 = chan2;
			return i;
		}
	}

	return PWM_GROUP_ID_INVALID;
}

static bk_err_t pwm_group_validate_param(pwm_chan_t sw_ch1,
										 pwm_chan_t sw_ch2,
										 uint32_t period_cycle,
										 uint32_t chan1_duty_cycle,
										 uint32_t chan2_duty_cycle)
{
	PWM_RETURN_ON_INVALID_CHAN(sw_ch1);
	PWM_RETURN_ON_INVALID_CHAN(sw_ch2);

	if (sw_ch1 == sw_ch2)
		return BK_ERR_PWM_GROUP_SAME_CHAN;

	if (PWM_GROUP_ID_INVALID != pwm_group_find(sw_ch1, sw_ch2))
		return BK_ERR_PWM_GROUP_EXIST;

	if (PWM_GROUP_ID_INVALID != pwm_group_find_channel(sw_ch1))
		return BK_ERR_PWM_GROUP_CHAN_USED;

	if (PWM_GROUP_ID_INVALID != pwm_group_find_channel(sw_ch2))
		return BK_ERR_PWM_GROUP_CHAN_USED;

	if (period_cycle == 0)
		return BK_ERR_PWM_GROUP_DUTY;

	if (period_cycle < (chan1_duty_cycle + chan2_duty_cycle))
		return BK_ERR_PWM_GROUP_DUTY;

	return BK_OK;
}

static void pwm_chan_enable_interrupt_common(pwm_chan_t sw_ch)
{
	sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
	pwm_hal_set_uie(sw_ch, 1);
}

static void pwm_chan_disable_interrupt_common(pwm_chan_t sw_ch)
{
	pwm_hal_set_uie(sw_ch, 0);
}

#if (CONFIG_PWM_PM_CB_SUPPORT)
static int pwm_pm_backup(uint64_t sleep_time, void *args)
{
	PWM_RETURN_ON_NOT_INIT();
	uint32_t chan = (uint32_t)args;
	if (!s_pwm.pm_bakeup_is_valid) {
		pwm_hal_pm_backup(chan, s_pwm.pm_backup, PWM_PM_BACKUP_REG_NUM);
		s_pwm.pm_bakeup_is_valid = 1;
	}
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_DOWN);

	return BK_OK;
}

static int pwm_pm_restore(uint64_t sleep_time, void *args)
{
	PWM_RETURN_ON_NOT_INIT();
	uint32_t chan = (uint32_t)args;

	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_UP);
	if (s_pwm.pm_bakeup_is_valid) {
		pwm_hal_pm_restore(chan, s_pwm.pm_backup, PWM_PM_BACKUP_REG_NUM);
		s_pwm.pm_bakeup_is_valid = 0;
	}

	return BK_OK;
}
#endif

bk_err_t bk_pwm_driver_init(void)
{
	if (s_pwm_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_pwm, 0, sizeof(s_pwm));
	os_memset(&s_pwm_isr, 0, sizeof(s_pwm_isr));
	bk_int_isr_register(INT_SRC_PWM, pwm0_isr, NULL);
#if (defined(CONFIG_PWM_PM_CB_SUPPORT))
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_ON);
#endif
	pwm_hal_init();
	s_pwm_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_pwm_driver_deinit(void)
{
	if (!s_pwm_driver_is_init)
		return BK_OK;

	for (int sw_ch = 0; sw_ch < (SOC_PWM_CHAN_NUM_PER_UNIT * SOC_PWM_UNIT_NUM); sw_ch++) {
		pwm_chan_deinit_common(sw_ch);
	}

#if (defined(CONFIG_PWM_PM_CB_SUPPORT))
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_OFF);
#endif
	pwm_hal_deinit();
	s_pwm_driver_is_init = false;
	return BK_OK;
}

bk_err_t bk_pwm_init(pwm_chan_t chan, const pwm_init_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_NOT_INIT();
	PWM_PM_CHECK_RESTORE(chan);

	pwm_period_duty_config_t pwm_config = {0};
	pwm_tim_map_t tim_map[] = PWM_CHAN_DEFAULT_TIM_MAP;
	bk_err_t ret = 0;

	if (!pwm_driver_duty_is_valid(config->period_cycle, config->duty_cycle,
								  config->duty2_cycle, config->duty3_cycle))
		return BK_ERR_PWM_PERIOD_DUTY;

#if (defined(CONFIG_PWM_PM_CB_SUPPORT))
	pm_cb_conf_t enter_cfg = {pwm_pm_backup, (void *)chan};

	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_ON);
	bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_PWM_1, &enter_cfg, NULL);
	bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_PWM_1);
#endif

	pwm_chan_init_common(chan);
	pwm_hal_chan_alloc_io(chan, tim_map[chan].tim_ch);
	pwm_hal_set_polarity(chan, PWM_POLARITY_POSITIVE);
	pwm_hal_set_init_level(chan, 0);
	pwm_hal_set_ocm(chan, 1);
	pwm_hal_set_ocpe(chan, 1);
	pwm_hal_set_arpe(chan, 1);
	pwm_hal_set_ocmpe(chan, 1);

	/* duty ratio is 0% */
	if (config->duty_cycle == 0 &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_config.psc = config->psc;
		pwm_hal_set_init_level(chan, 0);
		pwm_hal_set_chan_config(chan, &pwm_config);
		s_pwm.chan_init_signal_level &= ~BIT(chan);
		return BK_OK;
	}

	/* duty ratio is 100% */
	if (config->duty_cycle == config->period_cycle &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_config.psc = config->psc;
		pwm_hal_set_init_level(chan, 1);
		pwm_hal_set_chan_config(chan, &pwm_config);
		s_pwm.chan_init_signal_level |= BIT(chan);
		return BK_OK;
	}

	pwm_config.period_cycle = config->period_cycle;
	pwm_config.duty_cycle = 0;
	pwm_config.duty2_cycle = config->period_cycle - config->duty_cycle;
	pwm_config.duty3_cycle = 0;
	pwm_config.psc = config->psc;
	ret = pwm_hal_set_chan_config(chan, &pwm_config);
	s_pwm.chan_init_signal_level &= ~BIT(chan);

	return ret;
}

bk_err_t bk_pwm_deinit(pwm_chan_t chan)
{
	pwm_chan_deinit_common(chan);
#if defined(CONFIG_PWM_PM_CB_SUPPORT)
	bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_PWM_1, true, false);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_OFF);
#endif
	bk_pwm_stop(chan);

	return BK_OK;
}

bk_err_t bk_pwm_start(pwm_chan_t chan)
{
	PWM_PM_CHECK_RESTORE(chan);
	pwm_hal_set_single_chan_tim_enable(chan, 1);

	return BK_OK;
}

bk_err_t bk_pwm_stop(pwm_chan_t chan)
{
	PWM_PM_CHECK_RESTORE(chan);
	pwm_hal_set_single_chan_tim_enable(chan, 0);

	return BK_OK;
}

bk_err_t bk_pwm_set_period_duty(pwm_chan_t chan, pwm_period_duty_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(chan);
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);
	PWM_PM_CHECK_RESTORE(chan);

	if (!pwm_driver_duty_is_valid(config->period_cycle, config->duty_cycle,
								  config->duty2_cycle, config->duty3_cycle)) {
		return BK_ERR_PWM_PERIOD_DUTY;
	}

	bk_err_t ret = BK_OK;
	pwm_period_duty_config_t pwm_config = {0};

	os_memcpy(&pwm_config, config, sizeof(pwm_period_duty_config_t));

	/* duty ratio is 0% */
	if (config->duty_cycle == 0 &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_hal_set_init_level(chan, 0);
		pwm_hal_set_chan_config(chan, config);
		s_pwm.chan_init_signal_level &= ~BIT(chan);
		return BK_OK;
	}

	/* duty ratio is 100% */
	if (config->duty_cycle == config->period_cycle &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_hal_set_init_level(chan, 1);
		pwm_hal_set_chan_config(chan, &pwm_config);
		s_pwm.chan_init_signal_level |= BIT(chan);
		return BK_OK;
	}

	if (s_pwm.chan_init_signal_level & BIT(chan)) {
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = config->duty_cycle;
		pwm_config.duty3_cycle = 0;
	} else {
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = config->period_cycle - config->duty_cycle;
		pwm_config.duty3_cycle = 0;
	}
	PWM_LOGD("chan:%d, period_cycle:%d, duty_cycle:%d, duty2_cycle:%d, duty3_cycle:%d\r\n",
			chan,
			pwm_config.period_cycle,
			pwm_config.duty_cycle,
			pwm_config.duty2_cycle,
			pwm_config.duty3_cycle);
	ret = pwm_hal_set_chan_config(chan, &pwm_config);

	return ret;
}

bk_err_t bk_pwm_set_init_signal_low(pwm_chan_t chan)
{
	pwm_hal_set_init_level(chan, 0);
	return BK_OK;
}

bk_err_t bk_pwm_set_init_signal_high(pwm_chan_t chan)
{
	pwm_hal_set_init_level(chan, 1);
	return BK_OK;
}

bk_err_t bk_pwm_register_isr(pwm_chan_t chan, pwm_isr_t isr)
{
	uint32_t flags = rtos_disable_int();
	s_pwm_isr[chan] = isr;
	rtos_enable_int(flags);

	return BK_OK;
}

bk_err_t bk_pwm_enable_interrupt(pwm_chan_t chan)
{
	pwm_chan_enable_interrupt_common(chan);
	return BK_OK;
}

bk_err_t bk_pwm_disable_interrupt(pwm_chan_t chan)
{
	pwm_chan_disable_interrupt_common(chan);
	return BK_OK;
}

bk_err_t bk_pwm_group_init(const pwm_group_init_config_t *config, pwm_group_t *group)
{
	BK_RETURN_ON_NULL(config);
	BK_RETURN_ON_NULL(group);
	PWM_PM_CHECK_RESTORE(config->chan1);
	PWM_PM_CHECK_RESTORE(config->chan2);

	int ret = BK_OK;
	*group = PWM_GROUP_ID_INVALID;
	ret = pwm_group_validate_param(config->chan1, config->chan2, config->period_cycle,
								   config->chan1_duty_cycle, config->chan2_duty_cycle);
	if (ret != BK_OK) {
		return ret;
	}

	/* alloc free TIM */
	pwm_group_t group_id = pwm_group_add(config->chan1, config->chan2);
	pwm_group_tim_map_t group_tim_map[] = PWM_GROUP_DEFAULT_TIM_MAP;
	uint32_t dead_cycle = (config->period_cycle - config->chan1_duty_cycle - config->chan2_duty_cycle) >> 1;
	pwm_period_duty_config_t pwm_config = {0};

	pwm_chan_init_common(config->chan1);
	pwm_chan_init_common(config->chan2);
	pwm_hal_chan_alloc_io(config->chan1, group_tim_map[group_id].tim_ch_p);
	pwm_hal_chan_alloc_io(config->chan2, group_tim_map[group_id].tim_ch_n);
	pwm_hal_set_ocm(group_id, 1);
	pwm_hal_set_ocpe(group_id, 1);
	pwm_hal_set_arpe(group_id, 1);
	pwm_hal_set_ocmpe(group_id, 1);

	if ((config->chan1_duty_cycle == 0) && (config->chan2_duty_cycle == config->period_cycle)) {
		pwm_config.psc = config->psc;
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_hal_set_init_level(group_id, 0);
		pwm_hal_set_group_config(&pwm_config, group_id, 0);
		*group = group_id;
		s_pwm.group_chan_init_level &= ~BIT(group_id);
		return ret;
	}

	if ((config->chan1_duty_cycle == config->period_cycle) && (config->chan2_duty_cycle == 0)) {
		pwm_config.psc = config->psc;
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_hal_set_init_level(group_id, 1);
		pwm_hal_set_group_config(&pwm_config, group_id, 0);
		s_pwm.group_chan_init_level |= BIT(group_id);
		*group = group_id;
		return ret;
	}

	pwm_config.psc = config->psc;
	pwm_config.period_cycle = config->period_cycle;
	pwm_config.duty_cycle = config->chan2_duty_cycle + dead_cycle;
	pwm_config.duty2_cycle = config->period_cycle;
	pwm_hal_set_init_level(group_id, 0);
	pwm_hal_set_group_config(&pwm_config, group_id, dead_cycle);
	*group = group_id;
	s_pwm.group_chan_init_level &= ~BIT(group_id);
	PWM_LOGI("group(%d) period=%d chan1_duty=%d chan2_duty=%d, dead_cycle=%d\n",
			  group_id, config->period_cycle, config->chan1_duty_cycle, config->chan2_duty_cycle, dead_cycle);

	return ret;
}

bk_err_t bk_pwm_group_deinit(pwm_group_t group)
{
	bk_pwm_group_stop(group);

	return BK_OK;
}

bk_err_t bk_pwm_group_set_config(pwm_group_t group, const pwm_group_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	PWM_PM_CHECK_RESTORE(s_pwm.groups[group].chan1);
	PWM_PM_CHECK_RESTORE(s_pwm.groups[group].chan2);

	uint32_t dead_cycle = (config->period_cycle - config->chan1_duty_cycle - config->chan2_duty_cycle) >> 1;
	pwm_period_duty_config_t pwm_config = {0};

	if ((config->chan1_duty_cycle == 0) && (config->chan2_duty_cycle == config->period_cycle)) {
		pwm_config.psc = config->psc;
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_hal_set_init_level(group, 0);
		pwm_hal_set_group_config(&pwm_config, group, 0);
		s_pwm.group_chan_init_level &= ~BIT(group);
		return BK_OK;
	}

	if ((config->chan1_duty_cycle == config->period_cycle) && (config->chan2_duty_cycle == 0)) {
		pwm_config.psc = config->psc;
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_hal_set_init_level(group, 1);
		pwm_hal_set_group_config(&pwm_config, group, 0);
		s_pwm.group_chan_init_level |= BIT(group);
		return BK_OK;
	}

	if (s_pwm.group_chan_init_level & BIT(group)) {
		pwm_config.psc = config->psc;
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = config->chan1_duty_cycle + dead_cycle;
		pwm_config.duty2_cycle = config->period_cycle;
	} else {
		pwm_config.psc = config->psc;
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = config->chan2_duty_cycle + dead_cycle;
		pwm_config.duty2_cycle = config->period_cycle;
	}
	pwm_hal_set_group_config(&pwm_config, group, dead_cycle);

	PWM_LOGI("group(%d) period=%d chan1_duty=%d chan2_duty=%d, dead_cycle=%d\n",
			group, config->period_cycle, config->chan1_duty_cycle, config->chan2_duty_cycle, dead_cycle);

	return BK_OK;
}

bk_err_t bk_pwm_group_start(pwm_group_t group)
{
	PWM_PM_CHECK_RESTORE(s_pwm.groups[group].chan1);
	PWM_PM_CHECK_RESTORE(s_pwm.groups[group].chan2);

	pwm_hal_set_single_chan_tim_enable(group, 1);
	PWM_LOGI("group(%d) start\r\n", group);

	return BK_OK;
}

bk_err_t bk_pwm_group_stop(pwm_group_t group)
{
	PWM_PM_CHECK_RESTORE(s_pwm.groups[group].chan1);
	PWM_PM_CHECK_RESTORE(s_pwm.groups[group].chan2);

	pwm_hal_set_single_chan_tim_enable(group, 0);
	PWM_LOGI("group(%d) stop\r\n", group);

	return BK_OK;
}

static int compare(const void *a, const void *b)
{
	return (*(uint32_t *)a - *(uint32_t *)b);
}

static uint32_t find_most_freq(uint32_t arr[], uint32_t num, uint32_t *max_count)
{
	if (num == 0) {
		*max_count = 0;
		return 0xff;
	}

	qsort(arr, num, sizeof(uint32_t), compare);

	uint32_t max_freq_number = arr[0];
	uint32_t current_number = arr[0];
	uint32_t current_count = 1;
	*max_count = 1;

	for (uint32_t i = 0; i < num; i++) {
		if (arr[i] == current_number) {
			current_count++;
		} else {
			if (current_count > *max_count) {
				*max_count = current_count;
				max_freq_number = current_number;
			}
			current_number = arr[i];
			current_count = 1;
		}
	}

	if (current_count > *max_count) {
		*max_count = current_count;
		max_freq_number =  current_number;
	}

	return max_freq_number;
}

bk_err_t bk_pwm_capture_init(pwm_chan_t chan, const pwm_capture_init_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(chan);

	bk_err_t ret = 0;
	pwm_capture_tim_map_t cap_map[] = PWM_CAPTURE_DEFAULT_TIM_MAP;

	pwm_chan_init_common(chan);
	pwm_hal_chan_alloc_capture_io(cap_map[chan].sw_ch, cap_map[chan].tim_cap_ch);
	pwm_hal_chan_alloc_io(cap_map[chan].sw_ch, cap_map[chan].tim_cap_ch);
	pwm_hal_init_capture(chan, config->edge);
	s_pwm_isr[chan] = config->isr;
	if (s_pwm.capture_sema == NULL) {
		ret = rtos_init_semaphore(&(s_pwm.capture_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}

	return BK_OK;
}

bk_err_t bk_pwm_capture_deinit(pwm_chan_t chan)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);

	pwm_hal_set_single_chan_tim_enable(chan, 0);
	pwm_hal_set_cc1ie(chan, 0);
	s_pwm_isr[chan] = NULL;
	if (s_pwm.capture_sema) {
		rtos_deinit_semaphore(&(s_pwm.capture_sema));
		s_pwm.capture_sema = NULL;
	}

	return BK_OK;
}

bk_err_t bk_pwm_capture_start(pwm_chan_t chan)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);

	pwm_hal_set_single_chan_tim_enable(chan, 1);
	pwm_hal_set_cc1ie(chan, 1);
	return BK_OK;
}

bk_err_t bk_pwm_capture_stop(pwm_chan_t chan)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);

	pwm_hal_set_single_chan_tim_enable(chan, 0);
	pwm_hal_set_cc1ie(chan, 0);
	return BK_OK;
}

uint32_t bk_pwm_capture_get_value(pwm_chan_t chan)
{
	return pwm_hal_get_ccr1_shadow(chan);
}

uint32_t bk_pwm_capture_get_period_duty_cycle(pwm_chan_t chan, uint32_t timeout_ms)
{
	bk_err_t ret = BK_OK;
	uint32_t ccr1_shadow = 0;
	uint32_t capture_int_type = 0;
	uint32_t capture_edge = 0;
	uint32_t max_count = 0;
	uint32_t most_freq = 0;
	uint32_t most_level = 0;
	uint32_t ccr_shadow_array[PWM_CAPTURE_CACHE_NUM] = {0};
	uint32_t level_array[PWM_CAPTURE_CACHE_NUM] = {0};
	static uint32_t period_cycle = 0;

	capture_edge = pwm_hal_get_ccmr_chan_polarity(chan);
	if (capture_edge == PWM_CAPTURE_POS || capture_edge == PWM_CAPTURE_NEG) {
		for (int i = 0; i < PWM_CAPTURE_CACHE_NUM; i++) {
			pwm_hal_set_cc1ie(chan, 1);
			ret = rtos_get_semaphore(&s_pwm.capture_sema, timeout_ms);
			if (ret != kNoErr) {
				break;
			}
			ccr1_shadow = pwm_hal_get_ccr1_shadow(chan);
			ccr_shadow_array[i] = ccr1_shadow;
		}
		pwm_hal_set_cc1ie(chan, 0);
		period_cycle = find_most_freq(ccr_shadow_array, PWM_CAPTURE_CACHE_NUM, &max_count);
		return period_cycle;
	}

	if (period_cycle == 0) {
		PWM_LOGW("pwm_capture, period_cycle:0\r\n");
		return 0;
	}

	os_memset(&ccr_shadow_array, 0, sizeof(ccr_shadow_array));
	os_memset(&level_array, 0, sizeof(level_array));
	for (int i = 0; i < PWM_CAPTURE_CACHE_NUM; i++) {
		pwm_hal_set_cc1ie(chan, 1);
		ret = rtos_get_semaphore(&s_pwm.capture_sema, timeout_ms);
		if (ret != kNoErr) {
			break;
		}
		ccr1_shadow = pwm_hal_get_ccr1_shadow(chan);
		capture_int_type = pwm_hal_get_capture_int_type(chan);

		ccr_shadow_array[i] = ccr1_shadow;
		level_array[i] = capture_int_type;
	}

	pwm_hal_set_cc1ie(chan, 0);
	most_freq = find_most_freq(ccr_shadow_array, PWM_CAPTURE_CACHE_NUM, &max_count);
	most_level = find_most_freq(level_array, PWM_CAPTURE_CACHE_NUM, &max_count);

	if (most_level) {
		return most_freq;
	} else {
		return period_cycle - most_freq;
	}
}

#if defined(CONFIG_PWM_PHASE_SHIFT)

static uint32_t pwm_phase_shift_find_first_valid_index(const pwm_phase_shift_config_t *shift_config)
{
	for (uint32_t index = 0; index < shift_config->chan_num; index++) {
		if ((shift_config->duty_config[index].duty_cycle != 0) &&
			(shift_config->duty_config[index].duty_cycle != shift_config->period_cycle)) {
			return index;
		}
	}
	return 0xff;
}

static bk_err_t pwm_init_with_phase(pwm_chan_t sw_ch, const pwm_init_config_t *config)
{
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(sw_ch);

	pwm_period_duty_config_t pwm_config = {0};
	pwm_tim_map_t tim_map[] = PWM_CHAN_DEFAULT_TIM_MAP;
	bk_err_t ret = BK_OK;

	if ((config->duty_cycle > config->period_cycle) ||
		(config->duty2_cycle > config->period_cycle) ||
		(config->duty3_cycle > config->period_cycle)) {
		PWM_LOGE("invalid duty cycle!\r\n");
		return BK_ERR_PWM_PERIOD_DUTY;
	}

	pwm_chan_init_common(sw_ch);
	pwm_hal_chan_alloc_io(sw_ch, tim_map[sw_ch].tim_ch);
	pwm_hal_set_polarity(sw_ch, PWM_POLARITY_POSITIVE);
	pwm_hal_set_ocm(sw_ch, 1);
	pwm_hal_set_ocpe(sw_ch, 1);
	pwm_hal_set_arpe(sw_ch, 1);
	pwm_hal_set_ocmpe(sw_ch, 1);

	/* duty ratio is 0% */
	if (config->duty_cycle == 0 &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_config.psc = config->psc;
		pwm_hal_set_init_level(sw_ch, 0);
		pwm_hal_set_chan_config(sw_ch, &pwm_config);
		return BK_OK;
	}

	/* duty ratio is 100% */
	if (config->duty_cycle == config->period_cycle &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_config.psc = config->psc;
		pwm_hal_set_init_level(sw_ch, 1);
		pwm_hal_set_chan_config(sw_ch, &pwm_config);
		return BK_OK;
	}

	pwm_config.period_cycle = config->period_cycle;
	pwm_config.duty_cycle = config->duty_cycle;
	pwm_config.duty2_cycle = config->duty2_cycle;
	pwm_config.duty3_cycle = config->duty3_cycle;;
	pwm_config.psc = config->psc;
	ret = pwm_hal_set_chan_config(sw_ch, &pwm_config);

	return ret;
}

static bk_err_t pwm_set_period_duty_with_phase(pwm_chan_t sw_ch, pwm_period_duty_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(sw_ch);

	if ((config->duty_cycle > config->period_cycle) ||
		(config->duty2_cycle > config->period_cycle) ||
		(config->duty3_cycle > config->period_cycle)) {
		PWM_LOGE("invalid duty cycle!\r\n");
		return BK_ERR_PWM_PERIOD_DUTY;
	}

	bk_err_t ret = BK_OK;

	/* duty ratio is 0% */
	if (config->duty_cycle == 0 &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_hal_set_init_level(sw_ch, 0);
		pwm_hal_set_chan_config(sw_ch, config);
		return BK_OK;
	}

	/* duty ratio is 100% */
	if (config->duty_cycle == config->period_cycle &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		config->duty_cycle = 0;
		pwm_hal_set_init_level(sw_ch, 1);
		pwm_hal_set_chan_config(sw_ch, config);
		return BK_OK;
	}

	ret = pwm_hal_set_chan_config(sw_ch, config);

	return ret;
}

static void pwm_phase_shift_set_duty(void)
{
	if (s_shift_config == NULL) {
		return;
	}

	__attribute__((unused)) bk_err_t ret = 0;
	pwm_ch_t hw_ch;
	uint32_t ccr1 = 0, ccr2 = 0, duty_cycle = 0;
	pwm_period_duty_config_t pwm_config = {0};

	uint32_t first_valid_index = pwm_phase_shift_find_first_valid_index(s_shift_config);
	hw_ch = s_shift_config->duty_config[0].chan;

	for (uint32_t index = 0; index < s_shift_config->chan_num; index++) {
		hw_ch = s_shift_config->duty_config[index].chan;
		if (s_shift_config->duty_config[index].duty_cycle == 0) {
			pwm_config.period_cycle = s_shift_config->period_cycle;
			pwm_config.duty_cycle = 0;
			pwm_config.duty2_cycle = 0;
			pwm_config.duty3_cycle = 0;
			pwm_config.psc = s_shift_config->psc;
			ret = pwm_set_period_duty_with_phase(hw_ch, &pwm_config);
			s_pwm.phase_shift_chan_init_level &= ~BIT(hw_ch);
		} else if (s_shift_config->duty_config[index].duty_cycle == s_shift_config->period_cycle) {
			pwm_config.period_cycle = s_shift_config->period_cycle;
			pwm_config.duty_cycle = s_shift_config->period_cycle;
			pwm_config.duty2_cycle = 0;
			pwm_config.duty3_cycle = 0;
			pwm_config.psc = s_shift_config->psc;
			ret = pwm_set_period_duty_with_phase(hw_ch, &pwm_config);
			s_pwm.phase_shift_chan_init_level |= BIT(hw_ch);
		} else if (index == first_valid_index) {
			pwm_hal_set_init_level(hw_ch, 1);
			ccr1 = s_shift_config->period_cycle;
			ccr2 = s_shift_config->duty_config[index].duty_cycle;
			pwm_config.period_cycle = s_shift_config->period_cycle;
			pwm_config.duty_cycle = ccr1;
			pwm_config.duty2_cycle = ccr2;
			pwm_config.duty3_cycle = 0;
			pwm_config.psc = s_shift_config->psc;
			ret = pwm_set_period_duty_with_phase(hw_ch, &pwm_config);
			s_pwm.phase_shift_chan_init_level |= BIT(hw_ch);
		} else {
			ccr1 = ccr2;
			duty_cycle = s_shift_config->duty_config[index].duty_cycle;
			pwm_config.psc = s_shift_config->psc;
			pwm_config.period_cycle = s_shift_config->period_cycle;
			if ((ccr2 + duty_cycle) > s_shift_config->period_cycle) {
				pwm_hal_set_init_level(hw_ch, 1);
				ccr2 = ccr2 + s_shift_config->duty_config[index].duty_cycle - s_shift_config->period_cycle;
				s_pwm.phase_shift_chan_init_level |= BIT(hw_ch);
			} else {
				pwm_hal_set_init_level(hw_ch, 0);
				ccr2 = ccr2 + s_shift_config->duty_config[index].duty_cycle;
				s_pwm.phase_shift_chan_init_level &= ~BIT(hw_ch);
			}
			pwm_config.duty_cycle = ccr1;
			pwm_config.duty2_cycle = ccr2;
			pwm_config.duty3_cycle = 0;
			ret = pwm_set_period_duty_with_phase(s_shift_config->duty_config[index].chan, &pwm_config);
		}
	}
}

bk_err_t bk_pwm_phase_shift_init(const pwm_phase_shift_config_t *shift_config)
{
	BK_RETURN_ON_NULL(shift_config);
	PWM_RETURN_ON_INVALID_CHAN_NUM(shift_config);
	PWM_PHASE_SHIFT_RETURN_ON_INVALID_DUTY(shift_config);

	bk_err_t ret = 0;
	pwm_ch_t hw_ch;
	uint32_t ccr1 = 0, ccr2 = 0, duty_cycle = 0;
	pwm_init_config_t pwm_init_config = {0};

	if (s_shift_config == NULL) {
		s_shift_config = (pwm_phase_shift_config_t *)os_zalloc(sizeof(pwm_phase_shift_config_t));
	}
	if (s_shift_config == NULL) {
		return BK_ERR_NO_MEM;
	}

	s_pwm_mode |= PWM_PHASE_SHIFT_MODE;
	s_phase_shift_multi_hw_ch = 0;
	memcpy(s_shift_config, shift_config, sizeof(pwm_phase_shift_config_t));
	uint32_t first_valid_index = pwm_phase_shift_find_first_valid_index(shift_config);
	s_phase_shift_first_valid_ch = shift_config->duty_config[0].chan;

	for (uint32_t index = 0; index < shift_config->chan_num; index++) {
		hw_ch = shift_config->duty_config[index].chan;
		if (shift_config->duty_config[index].duty_cycle == 0) {
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = 0;
			pwm_init_config.duty2_cycle = 0;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(hw_ch, &pwm_init_config);
			s_pwm.phase_shift_chan_init_level &= ~BIT(hw_ch);
		} else if (shift_config->duty_config[index].duty_cycle == shift_config->period_cycle) {
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = shift_config->period_cycle;
			pwm_init_config.duty2_cycle = 0;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(hw_ch, &pwm_init_config);
			s_pwm.phase_shift_chan_init_level |= BIT(hw_ch);
		} else if (index == first_valid_index) {
			ccr1 = 0;
			ccr2 = ccr1 + shift_config->duty_config[index].duty_cycle;
			pwm_hal_set_init_level(hw_ch, 1);
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = shift_config->period_cycle;
			pwm_init_config.duty2_cycle = ccr2;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(hw_ch, &pwm_init_config);
			s_pwm.phase_shift_chan_init_level |= BIT(hw_ch);
		} else {
			ccr1 = ccr2;
			duty_cycle = shift_config->duty_config[index].duty_cycle;
			if ((ccr2 + duty_cycle) > shift_config->period_cycle) {
				pwm_hal_set_init_level(hw_ch, 1);
				ccr2 = ccr2 + shift_config->duty_config[index].duty_cycle - shift_config->period_cycle;
				s_pwm.phase_shift_chan_init_level |= BIT(hw_ch);
			} else {
				pwm_hal_set_init_level(hw_ch, 0);
				ccr2 = ccr2 + shift_config->duty_config[index].duty_cycle;
				s_pwm.phase_shift_chan_init_level &= ~BIT(hw_ch);
			}
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = ccr1;
			pwm_init_config.duty2_cycle = ccr2;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(hw_ch, &pwm_init_config);
		}
		s_phase_shift_multi_hw_ch |= BIT(hw_ch);
	}

	return ret;
}

bk_err_t bk_pwm_phase_shift_deinit(void)
{
	if (s_shift_config) {
		os_free(s_shift_config);
		s_shift_config = NULL;
	}
	s_pwm_mode &= ~PWM_PHASE_SHIFT_MODE;
	s_phase_shift_multi_hw_ch = 0;
	bk_pwm_phase_shift_stop();

	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_start(void)
{
	pwm_hal_set_multi_chan_tim_enable(s_phase_shift_multi_hw_ch, 1);
	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_stop(void)
{
	pwm_hal_set_multi_chan_tim_enable(s_phase_shift_multi_hw_ch, 0);
	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_set_duty(pwm_chan_t chan, uint32_t duty_cycle)
{
	BK_RETURN_ON_NULL(s_shift_config);

	if (duty_cycle > s_shift_config->period_cycle) {
		return BK_ERR_PWM_PERIOD_DUTY;
	}

	pwm_hal_set_sync_all_enable(1);

	for (uint32_t index = 0; index < s_shift_config->chan_num; index++) {
		if (s_shift_config->duty_config[index].chan == chan) {
			s_shift_config->duty_config[index].duty_cycle = duty_cycle;
			break;
		}
	}

	pwm_phase_shift_set_duty();

	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_update_duty(void)
{
	BK_RETURN_ON_NULL(s_shift_config);

	uint32_t int_level = rtos_disable_int();
	pwm_hal_sync_all_shadow();
	pwm_hal_set_sync_all_enable(0);
	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_set_duty_and_update(const pwm_phase_shift_config_t *shift_config)
{
	BK_RETURN_ON_NULL(shift_config);
	PWM_RETURN_ON_INVALID_CHAN_NUM(shift_config);
	PWM_PHASE_SHIFT_RETURN_ON_INVALID_DUTY(shift_config);

	pwm_hal_set_sync_all_enable(1);
	memcpy(s_shift_config, shift_config, sizeof(pwm_phase_shift_config_t));
	pwm_phase_shift_set_duty();
	uint32_t int_level = rtos_disable_int();
	pwm_hal_sync_all_shadow();
	pwm_hal_set_sync_all_enable(0);
	rtos_enable_int(int_level);

	return BK_OK;
}

#endif

static uint32_t pwm_adjust_fade_num(pwm_chan_t chan, pwm_fade_mode_t fade_mode)
{
	uint32_t ccr2 = pwm_hal_get_ccr2(chan);
	uint32_t arr = pwm_hal_get_tim_arr(chan);
	uint32_t fade_scale =  pwm_hal_get_fade_scale(chan);
	uint32_t fade_num_lower = (ccr2 - 0) / fade_scale;
	uint32_t fade_num_upper = (arr + 1 - ccr2) / fade_scale;
	uint32_t cur_fade_num = pwm_hal_get_fade_num(chan);
	uint32_t fade_num_to_set = cur_fade_num;

	PWM_LOGI("TIM%d CCR2=%d, ARR=%d\r\n", chan, ccr2, arr);

	if (s_pwm.chan_init_signal_level & BIT(chan)) {
		if (fade_mode == PWM_DUTY_DIR_INCREASE) {
			if (cur_fade_num > fade_num_upper) {
				PWM_LOGW("cur_fade_num=%d, max_fade_num=%d\r\n", cur_fade_num, fade_num_upper);
				fade_num_to_set = fade_num_upper;
			}
		} else {
			if (cur_fade_num > fade_num_lower) {
				PWM_LOGW("cur_fade_num=%d, max_fade_num=%d\r\n", cur_fade_num, fade_num_lower);
				fade_num_to_set = fade_num_lower;
			}
		}
	} else {
		if (fade_mode == PWM_DUTY_DIR_INCREASE) {
			if (cur_fade_num > fade_num_lower) {
				PWM_LOGW("cur_fade_num=%d, max_fade_num=%d\r\n", cur_fade_num, fade_num_lower);
				fade_num_to_set = fade_num_lower;
			}
		} else {
			if (cur_fade_num > fade_num_upper) {
				PWM_LOGW("cur_fade_num=%d, max_fade_num=%d\r\n", cur_fade_num, fade_num_upper);
				fade_num_to_set = fade_num_upper;
			}
		}
	}

	return fade_num_to_set;
}

bk_err_t bk_pwm_fade_init(pwm_chan_t chan, uint32_t fade_scale, uint32_t fade_intv_cycle, uint32_t fade_num)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);

	pwm_hal_set_fade_scale(chan, fade_scale);
	pwm_hal_set_fade_intv_cycle(chan, fade_intv_cycle);
	pwm_hal_set_fade_num(chan, fade_num);

	return BK_OK;
}

bk_err_t bk_pwm_fade_start(pwm_chan_t chan, pwm_fade_mode_t fade_mode)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);

	if (s_pwm.chan_init_signal_level & BIT(chan)) {
		pwm_hal_set_fade_inc_dec(chan, fade_mode);
	} else {
		pwm_hal_set_fade_inc_dec(chan, ~fade_mode);
	}
	uint32_t fade_num = pwm_adjust_fade_num(chan, fade_mode);
	pwm_hal_set_fade_num(chan, fade_num);
	PWM_LOGI("TIM%d mode=%d, fade_num=%d\r\n", chan, fade_mode, fade_num);
	pwm_hal_set_fade_enable(chan, 1);

	return BK_OK;
}

bk_err_t bk_pwm_fade_stop(pwm_chan_t chan)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);

	pwm_hal_set_fade_num(chan, 0);
	pwm_hal_set_fade_enable(chan, 0);

	return BK_OK;
}

static void pwm_isr_common(pwm_unit_t id)
{
	uint32_t int_status[SOC_PWM_GROUP_NUM] = {0};

	pwm_hal_get_pwm_interrupt_status(int_status, SOC_PWM_GROUP_NUM);
	pwm_hal_clear_interrupt_status(int_status, SOC_PWM_GROUP_NUM);

	for (uint32_t index = 0; index < SOC_PWM_TIM_NUM; index++) {
		if (pwm_hal_is_cc1if_triggered(index, int_status, SOC_PWM_GROUP_NUM)) {
			// pwm_hal_set_cc1ie(index, 0);
			if (s_pwm.capture_sema) {
				rtos_set_semaphore(&s_pwm.capture_sema);
			}
			if (s_pwm_isr[index]) {
				s_pwm_isr[index](index);
			}
		}

		if (pwm_hal_is_uif_triggered(index, int_status, SOC_PWM_GROUP_NUM)) {
			if (s_pwm_isr[index]) {
				s_pwm_isr[index](index);
			}
		}
	}
}

static void pwm0_isr(void)
{
	PWM_LOGD("%s\n", __func__);
	pwm_isr_common(0);
}

