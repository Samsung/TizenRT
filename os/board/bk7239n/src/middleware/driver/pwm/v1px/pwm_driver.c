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
#include "pwm_hal_v1px.h"
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

static void pwm_isr_common(pwm_id_t id);
static void pwm0_isr(void);
static void pwm1_isr(void);

#define PWM_GPIO_MODE_MAX               4
#define PWM_CHAN_MASK                   0x3f    // 6 channels
#define PWM_GROUP_NUM_PER_UNIT          SOC_PWM_GROUP_NUM
#define PWM_GROUP_NUM                   (PWM_GROUP_NUM_PER_UNIT * SOC_PWM_UNIT_NUM)
#define PWM_CHAN_TO_UNIT_ID(chan)       (chan / SOC_PWM_CHAN_NUM_PER_UNIT)
#define PWM_CHAN_ID_TO_HW_CHAN_ID(chan) (chan % SOC_PWM_CHAN_NUM_PER_UNIT)
#define PWM_GROUP_TO_UNIT_ID(group)     (group / SOC_PWM_GROUP_NUM)
#define PWM_GROUP_TO_HW_GROUP_ID(group) (group % SOC_PWM_GROUP_NUM)
#define PWM_INIT_LEVEL_HIGH             1
#define PWM_INIT_LEVEL_LOW              0
#define PWM_CAPTURE_CACHE_NUM           50

typedef struct {
	pwm_chan_t chan1;
	pwm_chan_t chan2;
	bool is_valid;
	uint32_t period_cycle;     /**< PWM Group period cycle */
	uint32_t chan1_duty_cycle; /**< Duty cycle of chan1 */
	uint32_t chan2_duty_cycle; /**< Duty cycle of chan2 */
	uint32_t dead_cycle;
	bool is_param_need_update;
	bool is_flip_mode_need_update;
} pwm_group_info_t;

typedef struct {
	pwm_hal_t hal;
	//Important notes: currently no lock for bits
	//Concurrently operation is NOT allowed!!!s_pwm_pin_id_map
	uint32_t chan_init_bits;
	uint32_t chan_init_signal_level;
	volatile uint32_t phase_shift_chan_init_level;
#if defined(CONFIG_PWM_PM_CB_SUPPORT)
	uint32_t pm_backup[PWM_PM_BACKUP_REG_NUM];
	uint8_t pm_bakeup_is_valid;
#endif
	beken_semaphore_t capture_sema;
} pwm_driver_t;

typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t gpio_dev;
} pwm_gpio_map_t;

typedef enum {
	PWM_SINGLE_CHAN_MODE = (1 << 0),
	PWM_GROUP_MODE = (1 << 1),
	PWM_PHASE_SHIFT_MODE = (1 << 2),
} pwm_func_mode_t;

static pwm_driver_t s_pwm[SOC_PWM_UNIT_NUM] = {0};
static pwm_isr_t s_pwm_isr[SOC_PWM_UNIT_NUM][SOC_PWM_CHAN_NUM_PER_UNIT] = {NULL};
static bool s_pwm_driver_is_init = false;
static const pwm_gpio_map_t s_pwm_pin_id_map[SOC_PWM_UNIT_NUM][SOC_PWM_CHAN_NUM_PER_UNIT] = GPIO_PWM_MAP_TABLE;
static pwm_group_info_t s_pwm_groups[PWM_GROUP_NUM];
static uint8_t s_pwm_mode = 0;
#if defined(CONFIG_PWM_PHASE_SHIFT)
static pwm_chan_t s_phase_shift_multi_hw_ch[SOC_PWM_UNIT_NUM] = {0};
static uint32_t s_phase_shift_first_valid_ch = 0;
static pwm_phase_shift_config_t *s_shift_config = NULL;
#endif

#define PWM_RETURN_ON_INVALID_CHAN(hw_ch) do {\
		if ((hw_ch) >= SOC_PWM_CHAN_NUM_PER_UNIT) {\
			return BK_ERR_PWM_CHAN_ID;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_CHAN_CLOCK(clock) do {\
		if ((clock) >= PWM_CHAN_CLOCK_MAX) {\
			return BK_ERR_PWM_CHAN_CLOCK;\
		}\
	} while(0)

#define PWM_RETURN_ON_NOT_INIT() do {\
		if (!s_pwm_driver_is_init) {\
			return BK_ERR_PWM_NOT_INIT;\
		}\
	} while(0)

#define PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch) do {\
		if (!(s_pwm[id].chan_init_bits & BIT((hw_ch)))) {\
			return BK_ERR_PWM_CHAN_NOT_INIT;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_GROUP(hw_ch) do {\
		if (hw_ch >= PWM_GROUP_NUM_PER_UNIT) {\
			return BK_ERR_PWM_GROUP_ID;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_GPIO_MODE(hw_ch) do {\
			if (hw_ch >= PWM_GPIO_MODE_MAX) {\
				return BK_ERR_PWM_INVALID_GPIO_MODE;\
			}\
		} while(0)

#define PWM_RETURN_ON_INVALID_CHAN_NUM(shift_config) do {\
	if (shift_config->chan_num < 2) {\
		return BK_ERR_PWM_PHASE_SHIFT_CHAN_NUM;\
	}\
} while(0)

#define PWM_PHASE_SHIFT_RETURN_ON_INVALID_DUTY(shift_config) do {\
	for (uint32_t index = 0; index < shift_config->chan_num; index++) {\
		if ((shift_config->duty_config[index].duty_cycle == 1) ||\
			(shift_config->duty_config[index].duty_cycle > shift_config->period_cycle)) {\
			PWM_LOGE("invalid duty cycle, index:%d, period_cycle:%d, duty_cycle:%d\r\n",\
					index,\
					shift_config->period_cycle,\
					shift_config->duty_config[index].duty_cycle);\
			return BK_ERR_PWM_PERIOD_DUTY;\
		}\
	}\
} while(0)

#if defined(CONFIG_PWM_PM_CB_SUPPORT)
#define PWM_PM_CHECK_RESTORE(id) do {\
	GLOBAL_INT_DECLARATION();\
	GLOBAL_INT_DISABLE();\
	switch (id) {\
	case PWM_ID_0:\
		if (bk_pm_module_lv_sleep_state_get(PM_DEV_ID_PWM_1)) {\
			bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_ON);\
			pwm_pm_restore(0, (void *)id);\
			bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_PWM_1);\
		}\
		break;\
	case PWM_ID_1:\
		if (bk_pm_module_lv_sleep_state_get(PM_DEV_ID_PWM_2)) {\
			bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM1, PM_POWER_MODULE_STATE_ON);\
			pwm_pm_restore(0, (void *)id);\
			bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_PWM_2);\
		}\
		break;\
	default:\
		break;\
	}\
	GLOBAL_INT_RESTORE();\
} while(0)
#else
#define PWM_PM_CHECK_RESTORE(id)
#endif

static void pwm_sw_ch_to_hw_id_ch(pwm_ch_t sw_ch, pwm_id_t *id, pwm_ch_t *hw_ch)
{
	if(id)
		*id = sw_ch / SOC_PWM_CHAN_NUM_PER_UNIT;
	if(hw_ch)
		*hw_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_UNIT;
}

static void pwm_chan_init_gpio(pwm_ch_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	gpio_dev_unmap(s_pwm_pin_id_map[id][hw_ch].gpio_id);
	gpio_dev_map(s_pwm_pin_id_map[id][hw_ch].gpio_id, s_pwm_pin_id_map[id][hw_ch].gpio_dev);
	bk_gpio_pull_up(s_pwm_pin_id_map[id][hw_ch].gpio_id);
}

static void pwm_chan_enable_interrupt_common(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	if(id == PWM_ID_0)
		sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
	else
		sys_drv_int_enable(PWM1_INTERRUPT_CTRL_BIT);

	pwm_hal_enable_interrupt(&s_pwm[id].hal, hw_ch);
}

/* Once the global PWM interrupt is enabled, we never disable it.
 * It should be OK, because if all PWM are stop, there will be no
 * PWM interrupt even though the global PWM interrupt is enabled.
 *
 * We can choose to disable the global PWM interrupt if all PWM
 * channels are stopped, but it make the code complex!
 **/
static void pwm_chan_disable_interrupt_common(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

#if 0
	pwm_hal_disable_interrupt(&s_pwm[id].hal, hw_ch);
	pwm_hal_clear_chan_interrupt_status(&s_pwm[id].hal, hw_ch);
#endif
}

/* Init common to PWM/Capture init
 * 1. Set channel init bit
 * 2. Power up channel
 * 3. Set channel specific clock
 */
static void pwm_chan_init_common(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;
	uint32_t soft_reset = 0;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	s_pwm[id].chan_init_bits |= BIT(hw_ch);

	if(id == PWM_ID_0) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_UP);
		sys_drv_pwm_select_clock(SYS_SEL_PWM0, PWM_SCLK_XTAL);
		sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
	}

	if(id == PWM_ID_1) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_UP);
		sys_drv_pwm_select_clock(SYS_SEL_PWM1, PWM_SCLK_XTAL);
		sys_drv_int_group2_enable(PWM1_INTERRUPT_CTRL_BIT);
	}

	pwm_chan_init_gpio(sw_ch);
	soft_reset = pwm_hal_get_cg_reset_soft_reset(&s_pwm[id].hal);
	if (0 == soft_reset) {
		pwm_hal_set_cg_reset_soft_reset(&s_pwm[id].hal, 1);
	}
}

static void pwm_chan_deinit_common(pwm_chan_t sw_ch)
{
	pwm_id_t unit_id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &unit_id, &hw_ch);
	s_pwm[unit_id].chan_init_bits &= ~BIT(hw_ch);
	pwm_hal_set_chan_enable(&s_pwm[unit_id].hal, hw_ch, false);
	//all of the channels deinit, then power-down
	if ((unit_id == 0) && ((s_pwm[unit_id].chan_init_bits & PWM_CHAN_MASK)== 0)) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_DOWN);
	}
	if ((unit_id == 1) && ((s_pwm[unit_id].chan_init_bits & PWM_CHAN_MASK)== 0)) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_DOWN);
	}
}

#if (defined(CONFIG_PWM_PM_CB_SUPPORT))
static int pwm_pm_backup(uint64_t sleep_time, void *args)
{
	PWM_RETURN_ON_NOT_INIT();
	pwm_id_t id = (pwm_id_t)args;
	if (!s_pwm[id].pm_bakeup_is_valid)
	{
		pwm_hal_pm_backup(&s_pwm[id].hal, s_pwm[id].pm_backup);
		s_pwm[id].pm_bakeup_is_valid = 1;
	}
	if (id == PWM_ID_0) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_DOWN);
	} else if (id == PWM_ID_1) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_DOWN);
	}
	return BK_OK;
}

static int pwm_pm_restore(uint64_t sleep_time, void *args)
{
	PWM_RETURN_ON_NOT_INIT();
	pwm_id_t id = (pwm_id_t)args;
	if (id == PWM_ID_0) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_UP);
	} else if (id == PWM_ID_1) {
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_UP);
	}
	if (s_pwm[id].pm_bakeup_is_valid)
	{
		pwm_hal_pm_restore(&s_pwm[id].hal, s_pwm[id].pm_backup);
		s_pwm[id].pm_bakeup_is_valid = 0;
	}
	return BK_OK;
}
#endif

bk_err_t bk_pwm_driver_init(void)
{
	if (s_pwm_driver_is_init)
		return BK_OK;

	os_memset(&s_pwm, 0, sizeof(s_pwm));
	os_memset(&s_pwm_isr, 0, sizeof(s_pwm_isr));

	bk_int_isr_register(INT_SRC_PWM, pwm0_isr, NULL);
	bk_int_isr_register(INT_SRC_PWM1, pwm1_isr, NULL);

#if (defined(CONFIG_PWM_PM_CB_SUPPORT))
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_ON);
#if (SOC_PWM_UNIT_NUM > 1)
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM1, PM_POWER_MODULE_STATE_ON);
#endif
#endif

	for (uint32_t uint_id = 0; uint_id < SOC_PWM_UNIT_NUM; uint_id++) {
		s_pwm[uint_id].hal.id = uint_id;
		pwm_hal_init(&s_pwm[uint_id].hal);
	}

	s_pwm_driver_is_init = true;
	return BK_OK;
}

bk_err_t bk_pwm_driver_deinit(void)
{
	if (!s_pwm_driver_is_init)
		return BK_OK;

	for (int sw_ch = 0; sw_ch < (SOC_PWM_CHAN_NUM_PER_UNIT * SOC_PWM_UNIT_NUM); sw_ch++)
		pwm_chan_deinit_common(sw_ch);

#if (defined(CONFIG_PWM_PM_CB_SUPPORT))
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_OFF);
#if (SOC_PWM_UNIT_NUM > 1)
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM1, PM_POWER_MODULE_STATE_OFF);
#endif
#endif

	s_pwm_driver_is_init = false;
	return BK_OK;
}

static void pwm_adjust_init_signal_via_duty(pwm_chan_t sw_ch, uint32_t period,
		uint32_t duty1, uint32_t duty2, uint32_t duty3)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	/** If duty ratio is 0% and initial signal is high,
	 * then the PWM signal will always be high.
	 * To avoid that, we manually configure init signal to low if
	 * duty ratio is 0%.
	 */
	if (duty1 == 0) {
		pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
		pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 4);
	}


	/** If duty ratio is 100% and initial signal is low,
	 * then the PWM signal will always be low.
	 * To avoid that, we manually configure init signal to high if
	 * duty ratio is 100%.
	 */
	else if (duty1 == period) {
		pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
		pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 4);
	}
}

static inline bool pwm_driver_duty_is_valid(uint32_t period, uint32_t duty1, uint32_t duty2,
										uint32_t duty3)
{
	if (period == 0) {
		PWM_LOGE("period must be more than 1\r\n");
		return false;
	}

	//Not consider (duty1 + duty2 + duty3) overflow
	if ((duty1 + duty2 + duty3) > period)
		return false;

	return true;
}

static void pwm_output_mode_config(pwm_chan_t sw_ch, int end_val, int psc, int cc, int ddt)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	switch(hw_ch)
	{
		case 0:	//PWM_CHANNEL0:
		case 1:
			pwm_hal_set_pwm_cr1_arpe1(&s_pwm[id].hal, 1) ;             //enable arr1 preload
			pwm_hal_set_tim1_arr_tim1_arr(&s_pwm[id].hal, end_val);				 //end value arr1 0x60
			pwm_hal_set_prescaler_psc1(&s_pwm[id].hal, psc) ;          //prescale is 5
			pwm_hal_set_ccr1_value(&s_pwm[id].hal, cc);
			pwm_hal_set_pwm_ccmr_oc1m(&s_pwm[id].hal, 0x3) ;		     //toggle at ccr1 and arr1
			//pwm_hal_set_pwm_dier_cc1ie(&s_pwm[id].hal, true) ;			 //enable ccr1 int
			pwm_hal_set_pwm_ccmr_ch1e(&s_pwm[id].hal, true);              //enable pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch2e(&s_pwm[id].hal, true);              //enable pwm_o[1]
			pwm_hal_set_dt_dt1(&s_pwm[id].hal, ddt) ;				 //set deadtime as 5 cycle
			pwm_hal_set_pwm_ccmr_ch1p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[1] as complementary of pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch2p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[1] as complementary of pwm_o[0]
			pwm_hal_set_pwm_edtr_ug1(&s_pwm[id].hal, 0x01);				 //update ccr1 and arr1
			break;
		case 2:	//PWM_CHANNEL2:
		case 3:
			pwm_hal_set_pwm_cr1_arpe2(&s_pwm[id].hal, 1) ;             //enable arr1 preload
			pwm_hal_set_tim2_arr_tim2_arr(&s_pwm[id].hal, end_val);				 //end value arr1 0x60
			pwm_hal_set_prescaler_psc2(&s_pwm[id].hal, psc) ;          //prescale is 5
			pwm_hal_set_ccr4_value(&s_pwm[id].hal, cc);
			pwm_hal_set_pwm_ccmr_oc2m(&s_pwm[id].hal, 0x3) ;		     //toggle at ccr1 and arr1
			//pwm_hal_set_pwm_dier_cc3ie(&s_pwm[id].hal, true) ;			 //enable ccr1 int
			pwm_hal_set_pwm_ccmr_ch3e(&s_pwm[id].hal, true);              //enable pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch4e(&s_pwm[id].hal, true);              //enable pwm_o[1]
			pwm_hal_set_dt_dt2(&s_pwm[id].hal, ddt) ;				 //set deadtime as 5 cycle
			pwm_hal_set_pwm_ccmr_ch3p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[1] as complementary of pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch4p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[1] as complementary of pwm_o[0]
			pwm_hal_set_pwm_edtr_ug2(&s_pwm[id].hal, 0x01);				 //update ccr1 and arr1
			break;
		case 4:	//PWM_CHANNEL4:
		case 5:
			pwm_hal_set_pwm_cr1_arpe3(&s_pwm[id].hal, 1) ;             //enable arr1 preload
			pwm_hal_set_tim3_arr_tim3_arr(&s_pwm[id].hal, end_val);				 //end value arr1 0x60
			pwm_hal_set_prescaler_psc3(&s_pwm[id].hal, psc) ;          //prescale is 5
			pwm_hal_set_ccr7_value(&s_pwm[id].hal, cc);
			pwm_hal_set_pwm_ccmr_oc3m(&s_pwm[id].hal, 0x3) ;		     //toggle at ccr1 and arr1
			//pwm_hal_set_pwm_dier_cc7ie(&s_pwm[id].hal, true) ;			 //enable ccr1 int
			pwm_hal_set_pwm_ccmr_ch5e(&s_pwm[id].hal, true);              //enable pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch6e(&s_pwm[id].hal, true);              //enable pwm_o[1]
			pwm_hal_set_dt_dt3(&s_pwm[id].hal, ddt) ;				 //set deadtime as 5 cycle
			pwm_hal_set_pwm_ccmr_ch5p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[1] as complementary of pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch6p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[1] as complementary of pwm_o[0]
			pwm_hal_set_pwm_edtr_ug3(&s_pwm[id].hal, 0x01);				 //update ccr1 and arr1
			break;
		default:
			break;
	}
}

static void pwm_group_set_complementary_mode(pwm_chan_t sw_chan)
{
	pwm_id_t id = 0;
	pwm_ch_t hw_ch = 0;

	pwm_sw_ch_to_hw_id_ch(sw_chan, &id, &hw_ch);
	PWM_LOGI("group pwm_id:%d, chan:%d\r\n", id, hw_ch);

	switch (hw_ch) {
		case PWM_CH_0:	//PWM_CHANNEL0:
		case PWM_CH_1:
			pwm_hal_set_pwm_ccmr_ch1p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[1] as complementary of pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch2p(&s_pwm[id].hal, 0x01) ;
			break;
		case PWM_CH_2:	//PWM_CHANNEL2:
		case PWM_CH_3:
			pwm_hal_set_pwm_ccmr_ch3p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[2] as complementary of pwm_o[3]
			pwm_hal_set_pwm_ccmr_ch4p(&s_pwm[id].hal, 0x01) ;
			break;
		case PWM_CH_4:	//PWM_CHANNEL4:
		case PWM_CH_5:
			pwm_hal_set_pwm_ccmr_ch5p(&s_pwm[id].hal, 0x00) ;			 //set pwm_o[4] as complementary of pwm_o[5]
			pwm_hal_set_pwm_ccmr_ch6p(&s_pwm[id].hal, 0x01) ;
			break;
		default:
			break;
	}
}

static void pwm_group_output_mode_config(const pwm_group_init_config_t *config, int psc, int ddt)
{
	pwm_output_mode_config(config->chan1, config->period_cycle - 1, psc, config->chan1_duty_cycle + ddt, ddt);
	pwm_output_mode_config(config->chan2, config->period_cycle - 1, psc, config->chan2_duty_cycle + ddt, ddt);
	pwm_group_set_complementary_mode(config->chan1);
	pwm_group_set_complementary_mode(config->chan2);
}

static void pwm_set_channel_config(pwm_ch_t sw_ch, const pwm_period_duty_config_t *config)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_LOGD("hw:%d, period_cycle:%d, duty_cycle:%d, duty2_cycle:%d, duty3_cycle:%d\r\n",
			hw_ch,
			config->period_cycle,
			config->duty_cycle,
			config->duty2_cycle,
			config->duty3_cycle);

	switch(hw_ch) {
		case 0:
			pwm_hal_set_pwm_ccmr_tim1ccm(&s_pwm[id].hal, 0);
			pwm_hal_set_tim1_arr_tim1_arr(&s_pwm[id].hal, config->period_cycle - 1);
			pwm_hal_set_ccr1_value(&s_pwm[id].hal, config->duty_cycle);
			pwm_hal_set_ccr2_value(&s_pwm[id].hal, config->duty2_cycle);
			pwm_hal_set_ccr3_value(&s_pwm[id].hal, config->duty3_cycle);
			pwm_hal_set_prescaler_psc1(&s_pwm[id].hal, config->psc);
			pwm_hal_set_pwm_ccmr_ch1e(&s_pwm[id].hal, true);
			break;
		case 1:
			pwm_hal_set_pwm_ccmr_tim1ccm(&s_pwm[id].hal, 0);
			pwm_hal_set_tim1_arr_tim1_arr(&s_pwm[id].hal, config->period_cycle - 1);
			pwm_hal_set_ccr1_value(&s_pwm[id].hal, config->duty_cycle);
			pwm_hal_set_ccr2_value(&s_pwm[id].hal, config->duty2_cycle);
			pwm_hal_set_ccr3_value(&s_pwm[id].hal, config->duty3_cycle);
			pwm_hal_set_prescaler_psc1(&s_pwm[id].hal, config->psc);
			pwm_hal_set_pwm_ccmr_ch2e(&s_pwm[id].hal, true);
			break;
		case 2:
			pwm_hal_set_pwm_ccmr_tim2ccm(&s_pwm[id].hal, 0);
			pwm_hal_set_tim2_arr_tim2_arr(&s_pwm[id].hal, config->period_cycle - 1);
			pwm_hal_set_ccr4_value(&s_pwm[id].hal, config->duty_cycle);
			pwm_hal_set_ccr5_value(&s_pwm[id].hal, config->duty2_cycle);
			pwm_hal_set_ccr6_value(&s_pwm[id].hal, config->duty3_cycle);
			pwm_hal_set_prescaler_psc2(&s_pwm[id].hal, config->psc);
			pwm_hal_set_pwm_ccmr_ch3e(&s_pwm[id].hal, true);
			break;
		case 3:
			pwm_hal_set_pwm_ccmr_tim2ccm(&s_pwm[id].hal, 0);
			pwm_hal_set_tim2_arr_tim2_arr(&s_pwm[id].hal, config->period_cycle - 1);
			pwm_hal_set_ccr4_value(&s_pwm[id].hal, config->duty_cycle);
			pwm_hal_set_ccr5_value(&s_pwm[id].hal, config->duty2_cycle);
			pwm_hal_set_ccr6_value(&s_pwm[id].hal, config->duty3_cycle);
			pwm_hal_set_prescaler_psc2(&s_pwm[id].hal, config->psc);
			pwm_hal_set_pwm_ccmr_ch4e(&s_pwm[id].hal, true);
			break;
		case 4:
			pwm_hal_set_pwm_ccmr_tim3ccm(&s_pwm[id].hal, 0);
			pwm_hal_set_tim3_arr_tim3_arr(&s_pwm[id].hal, config->period_cycle - 1);
			pwm_hal_set_ccr7_value(&s_pwm[id].hal, config->duty_cycle);
			pwm_hal_set_ccr8_value(&s_pwm[id].hal, config->duty2_cycle);
			pwm_hal_set_ccr9_value(&s_pwm[id].hal, config->duty3_cycle);
			pwm_hal_set_prescaler_psc3(&s_pwm[id].hal, config->psc);
			pwm_hal_set_pwm_ccmr_ch5e(&s_pwm[id].hal, true);
			break;
		case 5:
			pwm_hal_set_pwm_ccmr_tim3ccm(&s_pwm[id].hal, 0);
			pwm_hal_set_tim3_arr_tim3_arr(&s_pwm[id].hal, config->period_cycle - 1);
			pwm_hal_set_ccr7_value(&s_pwm[id].hal, config->duty_cycle);
			pwm_hal_set_ccr8_value(&s_pwm[id].hal, config->duty2_cycle);
			pwm_hal_set_ccr9_value(&s_pwm[id].hal, config->duty3_cycle);
			pwm_hal_set_prescaler_psc3(&s_pwm[id].hal, config->psc);
			pwm_hal_set_pwm_ccmr_ch6e(&s_pwm[id].hal, true);
			break;
		default:
			break;
	}
}

bk_err_t bk_pwm_init(pwm_chan_t sw_ch, const pwm_init_config_t *config)
{
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);

	pwm_id_t id;
	pwm_ch_t hw_ch;
	pwm_period_duty_config_t pwm_config;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_PM_CHECK_RESTORE(id);

	if (!pwm_driver_duty_is_valid(config->period_cycle, config->duty_cycle,
		config->duty2_cycle, config->duty3_cycle))
		return BK_ERR_PWM_PERIOD_DUTY;

#if (defined(CONFIG_PWM_PM_CB_SUPPORT))
	pm_cb_conf_t enter_cfg = {pwm_pm_backup, (void *)id};

	if (id == PWM_ID_0) {
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_ON);
		bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_PWM_1, &enter_cfg, NULL);
		bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_PWM_1);
	} else if (id == PWM_ID_1) {
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM1, PM_POWER_MODULE_STATE_ON);
		bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_PWM_2, &enter_cfg, NULL);
		bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_PWM_2);
	}
#endif

	pwm_chan_init_common(sw_ch);
	pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
	pwm_hal_set_new_config_way(&s_pwm[id].hal, hw_ch, 1);

	/* duty ratio is 0% */
	if (config->duty_cycle == 0) {
		s_pwm[id].chan_init_signal_level &= ~BIT(hw_ch);
		pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 4);
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_config.psc = config->psc;
		pwm_set_channel_config(sw_ch, &pwm_config);
		return BK_OK;
	}

	/* duty ratio is 100% */
	if (config->duty_cycle ==  config->period_cycle) {
		s_pwm[id].chan_init_signal_level |= BIT(hw_ch);
		pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 2);
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_config.psc = config->psc;
		pwm_set_channel_config(sw_ch, &pwm_config);
		return BK_OK;
	}

	s_pwm[id].chan_init_signal_level &= ~BIT(hw_ch);
	pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 1);
	pwm_config.period_cycle = config->period_cycle;
	pwm_config.duty_cycle = config->period_cycle - config->duty_cycle;
	pwm_config.duty2_cycle = config->period_cycle;
	pwm_config.duty3_cycle = 0;
	pwm_config.psc = config->psc;
	/* CCR cannot be set to 1 */
	if (pwm_config.duty_cycle == 1) {
		pwm_config.duty_cycle = 2;
	}
	pwm_set_channel_config(sw_ch, &pwm_config);

	return BK_OK;
}

bk_err_t bk_pwm_deinit(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;
	pwm_period_duty_config_t pwm_config = {0};

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

#if defined(CONFIG_PWM_PM_CB_SUPPORT)
	if (id == PWM_ID_0) {
		bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_PWM_1, true, false);
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM0, PM_POWER_MODULE_STATE_OFF);
	} else if (id == PWM_ID_1) {
		bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_PWM_2, true, false);
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_PWM1, PM_POWER_MODULE_STATE_OFF);
	}
#endif

	pwm_config.period_cycle = 1;
	bk_pwm_set_period_duty(sw_ch, &pwm_config);
	extern void delay_ms(UINT32 ms);
	delay_ms(1);
	bk_pwm_stop(sw_ch);
	pwm_hal_set_single_chan_tim_enable(&s_pwm[id].hal, hw_ch, false);
	s_pwm[id].chan_init_bits &= ~BIT(hw_ch);

	return BK_OK;
}

bk_err_t bk_pwm_start(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_PM_CHECK_RESTORE(id);
	pwm_hal_set_chan_enable(&s_pwm[id].hal, hw_ch, true);
	pwm_hal_set_single_chan_tim_enable(&s_pwm[id].hal, hw_ch, true);

	return BK_OK;
}

bk_err_t bk_pwm_stop(pwm_chan_t sw_ch)
{
	pwm_id_t unit_id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &unit_id, &hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(unit_id, hw_ch);
	PWM_PM_CHECK_RESTORE(unit_id);
	pwm_hal_set_chan_enable(&s_pwm[unit_id].hal, hw_ch, false);

	return BK_OK;
}

bk_err_t bk_pwm_register_isr(pwm_chan_t sw_ch, pwm_isr_t isr)
{
	PWM_RETURN_ON_NOT_INIT();

	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_pwm_isr[id][hw_ch] = isr;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

/* The PWM channel should be started before interrupt can be enabled.
 */
bk_err_t bk_pwm_enable_interrupt(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	pwm_chan_enable_interrupt_common(sw_ch);
	return BK_OK;
}

bk_err_t bk_pwm_disable_interrupt(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	pwm_chan_disable_interrupt_common(hw_ch);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_set_period_duty(pwm_chan_t sw_ch, pwm_period_duty_config_t *config)
{
	BK_RETURN_ON_NULL(config);

	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	if (!pwm_driver_duty_is_valid(config->period_cycle, config->duty_cycle,
		config->duty2_cycle, config->duty3_cycle))
		return BK_ERR_PWM_PERIOD_DUTY;

	PWM_PM_CHECK_RESTORE(id);
	pwm_hal_set_new_config_way(&s_pwm[id].hal, hw_ch, 1);

	/* duty ratio is 0% */
	if (config->duty_cycle == 0) {
		s_pwm[id].chan_init_signal_level &= ~BIT(hw_ch);
		pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 4);
		config->duty_cycle = 0;
		config->duty2_cycle = 0;
		config->duty3_cycle = 0;
		pwm_set_channel_config(sw_ch, config);
		return BK_OK;
	}

	/* duty ratio is 100% */
	if (config->duty_cycle ==  config->period_cycle) {
		s_pwm[id].chan_init_signal_level |= BIT(hw_ch);
		pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 2);
		config->duty_cycle = 0;
		config->duty2_cycle = 0;
		config->duty3_cycle = 0;
		pwm_set_channel_config(sw_ch, config);
		return BK_OK;
	}

	if (s_pwm[id].chan_init_signal_level & BIT(hw_ch)) {
		config->duty2_cycle = config->period_cycle;
		config->duty3_cycle = 0;
	} else {
		config->duty_cycle = config->period_cycle - config->duty_cycle;
		config->duty2_cycle = config->period_cycle;
		config->duty3_cycle = 0;
	}

	/* CCR cannot be set to 1 */
	if (config->duty_cycle == 1) {
		config->duty_cycle = 2;
	}
	pwm_set_channel_config(sw_ch, config);
	pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 1);

	return BK_OK;
}

static bool pwm_is_0_duty_ratio(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	return (pwm_hal_get_tim_ccr1(&s_pwm[id].hal, hw_ch) == 0);
}

static bool pwm_is_100_duty_ratio(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	return (pwm_hal_get_tim_ccr1(&s_pwm[id].hal, hw_ch) == pwm_hal_get_tim_arr(&s_pwm[id].hal, hw_ch));
}

bk_err_t bk_pwm_set_init_signal_low(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	if (pwm_is_100_duty_ratio(sw_ch))
		pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
	else
		pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);

	return BK_OK;
}

bk_err_t bk_pwm_set_init_signal_high(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	if (pwm_is_0_duty_ratio(hw_ch))
		pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
	else
		pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);

	return BK_OK;
}

//TODO finish it
bk_err_t bk_pwm_set_gpio(pwm_chan_t sw_ch, uint32 mode)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_RETURN_ON_INVALID_GPIO_MODE(id);

	pwm_chan_init_gpio(sw_ch);

	return BK_OK;
}

gpio_id_t bk_pwm_get_gpio(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	return s_pwm_pin_id_map[id][hw_ch].gpio_id;
}

static bool is_all_ones_or_zeros(uint32_t arr[], uint32_t num)
{
	if (num == 0) {
		return false;
	}

	uint32_t first_value = arr[0];
	for (int i = 0; i < num; i++) {
		if (arr[i] != first_value) {
			return false;
		}
	}

	return ((first_value == 0) || (first_value == 1));
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

	pwm_id_t id;
	pwm_ch_t hw_ch;
	bk_err_t ret = 0;

	pwm_sw_ch_to_hw_id_ch(chan, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	pwm_chan_init_common(chan);
	pwm_hal_init_capture(chan, config->edge);
	s_pwm_isr[id][hw_ch] = config->isr;
	if (s_pwm[id].capture_sema == NULL) {
		ret = rtos_init_semaphore(&(s_pwm[id].capture_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}

	return BK_OK;
}

bk_err_t bk_pwm_capture_deinit(pwm_chan_t chan)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(chan, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	pwm_hal_set_single_chan_tim_enable(&s_pwm[id].hal, hw_ch, 0);
	pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 0);
	s_pwm_isr[id][hw_ch] = NULL;
	if (s_pwm[id].capture_sema) {
		rtos_deinit_semaphore(&(s_pwm[id].capture_sema));
		s_pwm[id].capture_sema = NULL;
	}

	return BK_OK;
}

bk_err_t bk_pwm_capture_start(pwm_chan_t chan)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(chan, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	pwm_hal_set_single_chan_tim_enable(&s_pwm[id].hal, hw_ch, 1);
	pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 1);

	return BK_OK;
}

bk_err_t bk_pwm_capture_stop(pwm_chan_t chan)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(chan, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	pwm_hal_set_single_chan_tim_enable(&s_pwm[id].hal, hw_ch, 0);
	pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 0);

	return BK_OK;
}

uint32_t bk_pwm_capture_get_value(pwm_chan_t chan)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(chan, &id, &hw_ch);
	return pwm_hal_get_ccr1_shadow(&s_pwm[id].hal, hw_ch);
}

uint32_t bk_pwm_capture_get_period_duty_cycle(pwm_chan_t chan, uint32_t timeout_ms)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;
	gpio_id_t pwm_gpio = 0;
	bk_err_t ret = BK_OK;
	uint32_t capture_edge = 0;
	uint32_t ccr1_shadow = 0;
	uint32_t gpio_reg_val = 0;
	uint8_t gpio_input_value = 0;
	uint32_t max_count = 0;
	uint32_t most_freq = 0;
	uint32_t most_level = 0;
	uint32_t ccr_shadow_array[PWM_CAPTURE_CACHE_NUM] = {0};
	uint32_t level_array[PWM_CAPTURE_CACHE_NUM] = {0};
	static uint32_t period_cycle = 0;

	pwm_sw_ch_to_hw_id_ch(chan, &id, &hw_ch);

	capture_edge = pwm_hal_get_ccmr_chan_polarity(&s_pwm[id].hal, hw_ch);
	if (capture_edge == PWM_CAPTURE_POS || capture_edge == PWM_CAPTURE_NEG) {
		for (int i = 0; i < PWM_CAPTURE_CACHE_NUM; i++) {
			pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 1);
			ret = rtos_get_semaphore(&s_pwm[id].capture_sema, timeout_ms);
			if (ret != kNoErr) {
				break;
			}
			ccr1_shadow = pwm_hal_get_ccr1_shadow(&s_pwm[id].hal, hw_ch);
			ccr_shadow_array[i] = ccr1_shadow;
		}
		pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 0);
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
		pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 1);
		ret = rtos_get_semaphore(&s_pwm[id].capture_sema, timeout_ms);
		if (ret != kNoErr) {
			break;
		}
		ccr1_shadow = pwm_hal_get_ccr1_shadow(&s_pwm[id].hal, hw_ch);

		pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 1);
		ret = rtos_get_semaphore(&s_pwm[id].capture_sema, timeout_ms);
		if (ret != kNoErr) {
			break;
		}
		ccr1_shadow = pwm_hal_get_ccr1_shadow(&s_pwm[id].hal, hw_ch);

		pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 1);
		ret = rtos_get_semaphore(&s_pwm[id].capture_sema, timeout_ms);
		if (ret != kNoErr) {
			break;
		}
		ccr1_shadow = pwm_hal_get_ccr1_shadow(&s_pwm[id].hal, hw_ch);

		pwm_gpio = bk_pwm_get_gpio(chan);
		gpio_reg_val = REG_READ(SOC_AON_GPIO_REG_BASE + pwm_gpio * 4);
		REG_WRITE(SOC_AON_GPIO_REG_BASE + pwm_gpio * 4, 0xc);
		gpio_input_value = bk_gpio_get_input(pwm_gpio);
		REG_WRITE(SOC_AON_GPIO_REG_BASE + pwm_gpio * 4, gpio_reg_val);

		ccr_shadow_array[i] = ccr1_shadow;
		level_array[i] = gpio_input_value;
	}

	pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 0);

	most_freq = find_most_freq(ccr_shadow_array, PWM_CAPTURE_CACHE_NUM, &max_count);
	most_level = find_most_freq(level_array, PWM_CAPTURE_CACHE_NUM, &max_count);

	if (most_level) {
		return period_cycle - most_freq;
	} else {
		return most_freq;
	}
}

static pwm_group_t pwm_group_find_channel(pwm_chan_t sw_ch)
{
	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (s_pwm_groups[i].is_valid &&
		  ((s_pwm_groups[i].chan1 == sw_ch) || (s_pwm_groups[i].chan2 == sw_ch)))
			return i;
	}

	return PWM_GROUP_ID_INVALID;
}

static pwm_group_t pwm_group_find(pwm_chan_t chan1, pwm_chan_t chan2)
{
	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (s_pwm_groups[i].is_valid) {
			if ((s_pwm_groups[i].chan1 == chan1) &&
				(s_pwm_groups[i].chan2 == chan2))
				return i;

			if ((s_pwm_groups[i].chan1 == chan2) &&
				(s_pwm_groups[i].chan2 == chan1))
				return i;
		}
	}

	return PWM_GROUP_ID_INVALID;
}

static bool pwm_group_is_existed(pwm_group_t group)
{
	if (group >= PWM_GROUP_NUM) {
		return false;
	}

	return (s_pwm_groups[group].is_valid == true);
}

static pwm_group_t pwm_group_add(pwm_chan_t chan1, pwm_chan_t chan2)
{
	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (!s_pwm_groups[i].is_valid) {
			s_pwm_groups[i].is_valid = true;
			s_pwm_groups[i].chan1 = chan1;
			s_pwm_groups[i].chan2 = chan2;
			return (i);
		}
	}

	return PWM_GROUP_ID_INVALID;
}

static void pwm_group_delete(pwm_group_t group)
{
	if (group >= PWM_GROUP_NUM) {
		PWM_LOGE("pwm group delete error!\r\n");
		return;
	}

	if (group < PWM_GROUP_NUM) {
		s_pwm_groups[group].is_valid = false;
		s_pwm_groups[group].chan1 = 0;
		s_pwm_groups[group].chan2 = 0;
	}
}

static bk_err_t pwm_group_validate_param(pwm_chan_t sw_ch1, pwm_chan_t sw_ch2,
		uint32_t period_cycle, uint32_t chan1_duty_cycle, uint32_t chan2_duty_cycle)

{
	pwm_id_t id1, id2;
	pwm_ch_t hw_ch1, hw_ch2;

	pwm_sw_ch_to_hw_id_ch(sw_ch1, &id1, &hw_ch1);
	pwm_sw_ch_to_hw_id_ch(sw_ch2, &id2, &hw_ch2);

	PWM_RETURN_ON_INVALID_CHAN(hw_ch1);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch2);

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

	if ((chan1_duty_cycle <= 1))
		return BK_ERR_PWM_GROUP_DUTY;

	if (period_cycle < (chan1_duty_cycle + chan2_duty_cycle))
		return BK_ERR_PWM_GROUP_DUTY;

	return BK_OK;
}

static const pwm_group_info_t s_default_groups[PWM_GROUP_NUM_PER_UNIT] = {
	{.chan1 = 0, .chan2 = 1},
	{.chan1 = 2, .chan2 = 3},
	{.chan1 = 4, .chan2 = 5},
};

static bool pwm_chan_is_default_group(pwm_chan_t chan1, pwm_chan_t chan2)
{
	pwm_id_t id1, id2;
	pwm_ch_t hw_ch1, hw_ch2;

	pwm_sw_ch_to_hw_id_ch(chan1, &id1, &hw_ch1);
	pwm_sw_ch_to_hw_id_ch(chan2, &id2, &hw_ch2);

	if (id1 != id2) {
		return false;
	}

	for (int i = 0; i < PWM_GROUP_NUM_PER_UNIT; i++) {
		if ((s_default_groups[i].chan1 == hw_ch1 && s_default_groups[i].chan2 == hw_ch2) ||
			(s_default_groups[i].chan1 == hw_ch2 && s_default_groups[i].chan2 == hw_ch1)) {
			return true;
		}
	}

	return false;
}

static void pwm_group_init_with_any_channel(const pwm_group_init_config_t *config, int dead_cycle)
{
	pwm_id_t id1, id2;
	pwm_ch_t hw_ch1, hw_ch2;
	pwm_period_duty_config_t pwm_config;

	os_memset(&pwm_config, 0, sizeof(pwm_config));
	pwm_sw_ch_to_hw_id_ch(config->chan1, &id1, &hw_ch1);
	pwm_sw_ch_to_hw_id_ch(config->chan2, &id2, &hw_ch2);

	pwm_config.psc = 0;
	pwm_config.period_cycle = config->period_cycle;
	pwm_config.duty_cycle = config->chan1_duty_cycle;
	pwm_config.duty2_cycle = 0;
	pwm_config.duty3_cycle = 0;
	pwm_hal_set_new_config_way(&s_pwm[id1].hal, hw_ch1, 0);
	pwm_set_channel_config(config->chan1, &pwm_config);
	pwm_hal_set_flip_mode(&s_pwm[id1].hal, hw_ch1, 3);
	pwm_hal_set_init_signal_high(&s_pwm[id1].hal, hw_ch1);

	pwm_config.duty_cycle = config->chan1_duty_cycle + dead_cycle;
	pwm_config.duty2_cycle = config->chan2_duty_cycle + pwm_config.duty_cycle;
	pwm_hal_set_new_config_way(&s_pwm[id2].hal, hw_ch2, 0);
	pwm_set_channel_config(config->chan2, &pwm_config);
	pwm_hal_set_flip_mode(&s_pwm[id2].hal, hw_ch2, 1);
	pwm_hal_set_init_signal_low(&s_pwm[id2].hal, hw_ch2);
	extern void delay_ms(UINT32 ms);
	delay_ms(50);
}

bk_err_t bk_pwm_group_init(const pwm_group_init_config_t *config, pwm_group_t *group)
{
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(group);
	PWM_PM_CHECK_RESTORE(PWM_ID_0);
	PWM_PM_CHECK_RESTORE(PWM_ID_1);

	uint32_t dead_cycle = 0;
	int ret;

	*group = PWM_GROUP_ID_INVALID;
	ret = pwm_group_validate_param(config->chan1, config->chan2, config->period_cycle,
								   config->chan1_duty_cycle, config->chan2_duty_cycle);
	if (ret != BK_OK) {
		PWM_LOGE("pwm_group_validate_param, ret:-0x%x\r\n", -ret);
		return ret;
	}

	s_pwm_mode |= PWM_GROUP_MODE;
	dead_cycle = (config->period_cycle - config->chan1_duty_cycle - config->chan2_duty_cycle) >> 1;
	pwm_chan_init_common(config->chan1);
	pwm_chan_init_common(config->chan2);
	if (pwm_chan_is_default_group(config->chan1, config->chan2)) {
		PWM_LOGI("use hardware default group\r\n");
		if (dead_cycle >= 0x400) {
			PWM_LOGE("pwm_group_validate_dead_cycle, dead_cycle:%d\r\n", dead_cycle);
			return BK_ERR_PWM_GROUP_DUTY;
		}
		pwm_group_output_mode_config(config, 0, dead_cycle);
	} else {
		PWM_LOGI("configure channels into group mode manually\r\n");
		pwm_group_init_with_any_channel(config, dead_cycle);
	}
	*group = pwm_group_add(config->chan1, config->chan2);
	PWM_LOGI("group(%d) period=%d chan1_duty=%d chan2_duty=%d, dead_cycle=%d\n",
			 *group, config->period_cycle, config->chan1_duty_cycle, config->chan2_duty_cycle, dead_cycle);

	s_pwm_groups[*group].period_cycle = config->period_cycle;
	s_pwm_groups[*group].chan1_duty_cycle = config->chan1_duty_cycle;
	s_pwm_groups[*group].chan2_duty_cycle = config->chan2_duty_cycle;
	s_pwm_groups[*group].dead_cycle = dead_cycle;
	s_pwm_groups[*group].is_param_need_update = false;
	s_pwm_groups[*group].is_flip_mode_need_update = false;

	return BK_OK;
}

bk_err_t bk_pwm_group_deinit(pwm_group_t group)
{
	if (!pwm_group_is_existed(group))
		return BK_ERR_PWM_GROUP_NOT_EXIST;

	PWM_LOGI("group(%d) deinit\r\n", group);

	s_pwm_mode &= ~PWM_GROUP_MODE;
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PWM_2, PM_CPU_FRQ_DEFAULT);
	bk_pwm_group_stop(group);
	pwm_chan_deinit_common(s_pwm_groups[group].chan1);
	pwm_chan_deinit_common(s_pwm_groups[group].chan2);
	pwm_group_delete(group);

	return BK_OK;
}

bk_err_t bk_pwm_group_start(pwm_group_t group)
{
	if (!pwm_group_is_existed(group)) {
		PWM_LOGE("pwm group(%d) not existed\r\n", group);
		return BK_ERR_PWM_GROUP_NOT_EXIST;
	}
	PWM_PM_CHECK_RESTORE(PWM_ID_0);
	PWM_PM_CHECK_RESTORE(PWM_ID_1);
	PWM_LOGI("group(%d) start\r\n", group);

	pwm_id_t id1, id2;
	pwm_ch_t hw_ch1, hw_ch2;

	pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan1, &id1, &hw_ch1);
	pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan2, &id2, &hw_ch2);

	PWM_LOGI("group(%d) chan1:%d, chan2:%d\r\n", group, s_pwm_groups[group].chan1, s_pwm_groups[group].chan2);

	pwm_hal_group_start(id1, id2, hw_ch1, hw_ch2);

	return BK_OK;
}

bk_err_t bk_pwm_group_stop(pwm_group_t group)
{
	if (!pwm_group_is_existed(group)) {
		return BK_ERR_PWM_GROUP_NOT_EXIST;
	}
	PWM_PM_CHECK_RESTORE(PWM_ID_0);
	PWM_PM_CHECK_RESTORE(PWM_ID_1);
	PWM_LOGI("group(%d) stop\r\n", group);

	pwm_id_t id1, id2;
	pwm_ch_t hw_ch1, hw_ch2;

	pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan1, &id1, &hw_ch1);
	pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan2, &id2, &hw_ch2);

	pwm_hal_set_single_chan_tim_enable(&s_pwm[id1].hal, hw_ch1, 0);
	pwm_hal_set_single_chan_tim_enable(&s_pwm[id2].hal, hw_ch2, 0);

	return BK_OK;
}

bk_err_t bk_pwm_group_set_config(pwm_group_t group, const pwm_group_config_t *config)
{
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);
	if (!pwm_group_is_existed(group)) {
		return BK_ERR_PWM_GROUP_NOT_EXIST;
	}
	PWM_PM_CHECK_RESTORE(PWM_ID_0);
	PWM_PM_CHECK_RESTORE(PWM_ID_1);

	pwm_id_t id1, id2;
	pwm_ch_t hw_ch1, hw_ch2;
	uint32_t dead_cycle = 0;

	pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan1, &id1, &hw_ch1);
	pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan2, &id2, &hw_ch2);

	if (s_pwm_groups[group].chan1 == s_pwm_groups[group].chan2)
		return BK_ERR_PWM_GROUP_SAME_CHAN;

	if (config->period_cycle == 0 ||
		config->period_cycle < (config->chan1_duty_cycle + config->chan2_duty_cycle))
		return BK_ERR_PWM_GROUP_DUTY;

	if (((config->chan1_duty_cycle == 0) && (config->chan2_duty_cycle != config->period_cycle)) ||
		((config->chan2_duty_cycle == 0) && (config->chan1_duty_cycle != config->period_cycle))) {
		return BK_ERR_PWM_GROUP_DUTY;
	}

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PWM_2, PM_CPU_FRQ_120M);

	dead_cycle = (config->period_cycle - config->chan1_duty_cycle - config->chan2_duty_cycle) >> 1;

	pwm_hal_set_new_config_way(&s_pwm[id1].hal, hw_ch1, 1);
	pwm_hal_set_new_config_way(&s_pwm[id2].hal, hw_ch2, 1);

	uint32_t int_level = rtos_disable_int();
	s_pwm_groups[group].period_cycle = config->period_cycle;
	s_pwm_groups[group].chan1_duty_cycle = config->chan1_duty_cycle;
	s_pwm_groups[group].chan2_duty_cycle = config->chan2_duty_cycle;
	s_pwm_groups[group].dead_cycle = dead_cycle;
	s_pwm_groups[group].is_param_need_update = true;
	s_pwm_groups[group].is_flip_mode_need_update = true;
	pwm_hal_set_uie(&s_pwm[id1].hal, hw_ch1, 1);
	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t bk_pwm_group_set_init_signal(pwm_group_t group, const pwm_group_config_t *config)
{
	return BK_OK;
}

bk_err_t bk_pwm_set_mode_timer(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	pwm_hal_set_mode_timer(&s_pwm[id].hal, hw_ch);
#endif
	return BK_OK;
}

#if defined(CONFIG_PWM_PHASE_SHIFT)

static bk_err_t pwm_init_with_phase(pwm_chan_t sw_ch, const pwm_init_config_t *config)
{
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);

	pwm_id_t id;
	pwm_ch_t hw_ch;
	pwm_period_duty_config_t pwm_config;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	if ((config->duty_cycle == 1) ||
		(config->duty_cycle > config->period_cycle) ||
		(config->duty2_cycle > config->period_cycle) ||
		(config->duty3_cycle > config->period_cycle)) {
		PWM_LOGE("invalid duty cycle!\r\n");
		return BK_ERR_PWM_PERIOD_DUTY;
	}

	pwm_chan_init_common(sw_ch);
	pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 1);

	/* duty ratio is 0% */
	if (config->duty_cycle == 0 &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_config.period_cycle = config->period_cycle;
		pwm_config.duty_cycle = 0;
		pwm_config.duty2_cycle = 0;
		pwm_config.duty3_cycle = 0;
		pwm_config.psc = config->psc;
		pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
		pwm_set_channel_config(sw_ch, &pwm_config);
		pwm_hal_set_chan_enable(&s_pwm[id].hal, hw_ch, true);
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
		pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
		pwm_set_channel_config(sw_ch, &pwm_config);
		pwm_hal_set_chan_enable(&s_pwm[id].hal, hw_ch, true);
		return BK_OK;
	}

	pwm_config.period_cycle = config->period_cycle;
	pwm_config.duty_cycle = config->duty_cycle;
	pwm_config.duty2_cycle = config->duty2_cycle;
	pwm_config.duty3_cycle = config->duty3_cycle;;
	pwm_config.psc = config->psc;
	pwm_set_channel_config(sw_ch, &pwm_config);
	pwm_hal_set_chan_enable(&s_pwm[id].hal, hw_ch, true);

	return BK_OK;
}

static bk_err_t pwm_set_period_duty_with_phase(pwm_chan_t sw_ch, pwm_period_duty_config_t *config)
{
	BK_RETURN_ON_NULL(config);

	if ((config->duty_cycle == 1) ||
		(config->duty_cycle > config->period_cycle) ||
		(config->duty2_cycle > config->period_cycle) ||
		(config->duty3_cycle > config->period_cycle)) {
		PWM_LOGE("invalid duty cycle!\r\n");
		return BK_ERR_PWM_PERIOD_DUTY;
	}

	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	pwm_hal_set_flip_mode(&s_pwm[id].hal, hw_ch, 1);

	/* duty ratio is 0% */
	if (config->duty_cycle == 0 &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		pwm_set_channel_config(sw_ch, config);
		return BK_OK;
	}

	/* duty ratio is 100% */
	if (config->duty_cycle == config->period_cycle &&
		config->duty2_cycle == 0 &&
		config->duty3_cycle == 0) {
		config->duty_cycle = 0;
		config->duty2_cycle = 0;
		config->duty3_cycle = 0;
		pwm_set_channel_config(sw_ch, config);
		return BK_OK;
	}

	pwm_set_channel_config(sw_ch, config);

	return BK_OK;
}

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

static void pwm_phase_shift_set_duty(void)
{
	if (s_shift_config == NULL) {
		return;
	}

	__attribute__((unused)) bk_err_t ret = 0;
	pwm_id_t id;
	pwm_ch_t hw_ch;
	uint32_t ccr1 = 0, ccr2 = 0, duty_cycle = 0;
	pwm_period_duty_config_t pwm_config = {0};
	static uint8_t enter_isr_count = 0;

	enter_isr_count++;
	uint32_t first_valid_index = pwm_phase_shift_find_first_valid_index(s_shift_config);
	pwm_sw_ch_to_hw_id_ch(s_shift_config->duty_config[0].chan, &id, &hw_ch);
	pwm_hal_set_uie(&s_pwm[id].hal, hw_ch, 0);

	if (enter_isr_count == 1) {
		for (uint32_t index = 0; index < s_shift_config->chan_num; index++) {
			pwm_sw_ch_to_hw_id_ch(s_shift_config->duty_config[index].chan, &id, &hw_ch);
			if (s_shift_config->duty_config[index].duty_cycle == 0) {
				pwm_config.period_cycle = s_shift_config->period_cycle;
				pwm_config.duty_cycle = 0;
				pwm_config.duty2_cycle = 0;
				pwm_config.duty3_cycle = 0;
				pwm_config.psc = s_shift_config->psc;
				ret = pwm_set_period_duty_with_phase(s_shift_config->duty_config[index].chan, &pwm_config);
				if ((s_pwm[id].phase_shift_chan_init_level & BIT(hw_ch))) {
					pwm_hal_set_chan_enable(&s_pwm[id].hal, hw_ch, false);
				}
			} else if (s_shift_config->duty_config[index].duty_cycle == s_shift_config->period_cycle) {
				pwm_config.period_cycle = s_shift_config->period_cycle;
				pwm_config.duty_cycle = s_shift_config->period_cycle;
				pwm_config.duty2_cycle = 0;
				pwm_config.duty3_cycle = 0;
				pwm_config.psc = s_shift_config->psc;
				ret = pwm_set_period_duty_with_phase(s_shift_config->duty_config[index].chan, &pwm_config);
			} else if (index == first_valid_index) {
				if ((s_pwm[id].phase_shift_chan_init_level & BIT(hw_ch))) {
					ccr1 = s_shift_config->period_cycle;
				} else {
					ccr1 = 2;
				}
				ccr2 = s_shift_config->duty_config[index].duty_cycle;
				pwm_config.period_cycle = s_shift_config->period_cycle;
				pwm_config.duty_cycle = ccr1;
				pwm_config.duty2_cycle = ccr2;
				pwm_config.duty3_cycle = 0;
				pwm_config.psc = s_shift_config->psc;
				ret = pwm_set_period_duty_with_phase(s_shift_config->duty_config[index].chan, &pwm_config);
			} else {
				ccr1 = ccr2;
				duty_cycle = s_shift_config->duty_config[index].duty_cycle;
				pwm_config.psc = s_shift_config->psc;
				pwm_config.period_cycle = s_shift_config->period_cycle;
				if ((ccr2 + duty_cycle) > s_shift_config->period_cycle) {
					ccr2 = ccr2 + s_shift_config->duty_config[index].duty_cycle - s_shift_config->period_cycle;
				} else {
					ccr2 = ccr2 + s_shift_config->duty_config[index].duty_cycle;
				}
				pwm_config.duty_cycle = ccr1;
				pwm_config.duty2_cycle = ccr2;
				pwm_config.duty3_cycle = 0;
				ret = pwm_set_period_duty_with_phase(s_shift_config->duty_config[index].chan, &pwm_config);
			}
		}
		pwm_sw_ch_to_hw_id_ch(s_shift_config->duty_config[0].chan, &id, &hw_ch);
		pwm_hal_set_uie(&s_pwm[id].hal, hw_ch, 1);
	} else {
		enter_isr_count = 0;
		for (uint32_t index = 0; index < s_shift_config->chan_num; index++) {
			pwm_sw_ch_to_hw_id_ch(s_shift_config->duty_config[index].chan, &id, &hw_ch);
			if (s_shift_config->duty_config[index].duty_cycle == 0) {
				if ((s_pwm[id].phase_shift_chan_init_level & BIT(hw_ch))) {
					pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
					pwm_hal_set_chan_enable(&s_pwm[id].hal, hw_ch, true);
				}
				s_pwm[id].phase_shift_chan_init_level &= ~BIT(hw_ch);
			} else if (s_shift_config->duty_config[index].duty_cycle == s_shift_config->period_cycle) {
				if ((s_pwm[id].phase_shift_chan_init_level & BIT(hw_ch)) == 0) {
					pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
				}
				s_pwm[id].phase_shift_chan_init_level |= BIT(hw_ch);
			} else if (index == first_valid_index) {
				if ((s_pwm[id].phase_shift_chan_init_level & BIT(hw_ch))) {
					ccr1 = s_shift_config->period_cycle;
				} else {
					ccr1 = 2;
				}
				ccr2 = s_shift_config->duty_config[index].duty_cycle;
			} else {
				ccr1 = ccr2;
				duty_cycle = s_shift_config->duty_config[index].duty_cycle;
				if ((ccr2 + duty_cycle) > s_shift_config->period_cycle) {
					if ((s_pwm[id].phase_shift_chan_init_level & BIT(hw_ch)) == 0) {
						pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
					}
					ccr2 = ccr2 + s_shift_config->duty_config[index].duty_cycle - s_shift_config->period_cycle;
					s_pwm[id].phase_shift_chan_init_level |= BIT(hw_ch);
				} else {
					if (s_pwm[id].phase_shift_chan_init_level & BIT(hw_ch)) {
						pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
					}
					ccr2 = ccr2 + s_shift_config->duty_config[index].duty_cycle;
					s_pwm[id].phase_shift_chan_init_level &= ~BIT(hw_ch);
				}
			}
		}
	}
}

bk_err_t bk_pwm_phase_shift_init(const pwm_phase_shift_config_t *shift_config)
{
	BK_RETURN_ON_NULL(shift_config);
	PWM_RETURN_ON_INVALID_CHAN_NUM(shift_config);
	PWM_PHASE_SHIFT_RETURN_ON_INVALID_DUTY(shift_config);

	bk_err_t ret = 0;
	pwm_id_t id;
	pwm_ch_t hw_ch;
	uint32_t ccr1 = 0, ccr2 = 0, duty_cycle = 0;
	pwm_init_config_t pwm_init_config = {0};

	s_shift_config = (pwm_phase_shift_config_t *)os_malloc(sizeof(pwm_phase_shift_config_t));
	if (s_shift_config == NULL) {
		return BK_ERR_NO_MEM;
	}

	s_pwm_mode |= PWM_PHASE_SHIFT_MODE;
	os_memset(&s_phase_shift_multi_hw_ch, 0, sizeof(s_phase_shift_multi_hw_ch));
	memcpy(s_shift_config, shift_config, sizeof(pwm_phase_shift_config_t));
	uint32_t first_valid_index = pwm_phase_shift_find_first_valid_index(shift_config);
	s_phase_shift_first_valid_ch = shift_config->duty_config[0].chan;

	for (uint32_t index = 0; index < shift_config->chan_num; index++) {
		pwm_sw_ch_to_hw_id_ch(shift_config->duty_config[index].chan, &id, &hw_ch);
		pwm_hal_set_new_config_way(&s_pwm[id].hal, hw_ch, 1);
		if (shift_config->duty_config[index].duty_cycle == 0) {
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = 0;
			pwm_init_config.duty2_cycle = 0;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(shift_config->duty_config[index].chan, &pwm_init_config);
			s_pwm[id].phase_shift_chan_init_level &= ~BIT(hw_ch);
		} else if (shift_config->duty_config[index].duty_cycle == shift_config->period_cycle) {
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = shift_config->period_cycle;
			pwm_init_config.duty2_cycle = 0;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(shift_config->duty_config[index].chan, &pwm_init_config);
			s_pwm[id].phase_shift_chan_init_level |= BIT(hw_ch);
		} else if (index == first_valid_index) {
			ccr1 = 0;
			ccr2 = ccr1 + shift_config->duty_config[index].duty_cycle;
			pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = shift_config->period_cycle;
			pwm_init_config.duty2_cycle = ccr2;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(shift_config->duty_config[index].chan, &pwm_init_config);
			s_pwm[id].phase_shift_chan_init_level |= BIT(hw_ch);
		} else {
			ccr1 = ccr2;
			duty_cycle = shift_config->duty_config[index].duty_cycle;
			if ((ccr2 + duty_cycle) > shift_config->period_cycle) {
				pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
				ccr2 = ccr2 + shift_config->duty_config[index].duty_cycle - shift_config->period_cycle;
				s_pwm[id].phase_shift_chan_init_level |= BIT(hw_ch);
			} else {
				pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
				ccr2 = ccr2 + shift_config->duty_config[index].duty_cycle;
				s_pwm[id].phase_shift_chan_init_level &= ~BIT(hw_ch);
			}
			pwm_init_config.period_cycle = shift_config->period_cycle;
			pwm_init_config.duty_cycle = ccr1;
			pwm_init_config.duty2_cycle = ccr2;
			pwm_init_config.duty3_cycle = 0;
			pwm_init_config.psc = shift_config->psc;
			ret = pwm_init_with_phase(shift_config->duty_config[index].chan, &pwm_init_config);
		}
		s_phase_shift_multi_hw_ch[id] |= BIT(hw_ch);
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
	os_memset(&s_phase_shift_multi_hw_ch, 0, sizeof(s_phase_shift_multi_hw_ch));
	bk_pwm_phase_shift_stop();

	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_start(void)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(s_phase_shift_first_valid_ch, &id, &hw_ch);
	if (id == 1) {
		pwm_hal_set_multi_chan_tim_enable(&s_pwm[1].hal, s_phase_shift_multi_hw_ch[1], 1);
		pwm_hal_set_multi_chan_tim_enable(&s_pwm[0].hal, s_phase_shift_multi_hw_ch[0], 1);
	} else {
		pwm_hal_set_multi_chan_tim_enable(&s_pwm[0].hal, s_phase_shift_multi_hw_ch[0], 1);
		pwm_hal_set_multi_chan_tim_enable(&s_pwm[1].hal, s_phase_shift_multi_hw_ch[1], 1);
	}

	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_stop(void)
{
	for (uint32_t id = 0; id < SOC_PWM_UNIT_NUM; id++) {
		pwm_hal_set_multi_chan_tim_enable(&s_pwm[id].hal, s_phase_shift_multi_hw_ch[id], 0);
	}
	return BK_OK;
}

bk_err_t bk_pwm_phase_shift_set_duty_and_update(const pwm_phase_shift_config_t *shift_config)
{
	BK_RETURN_ON_NULL(shift_config);
	PWM_RETURN_ON_INVALID_CHAN_NUM(shift_config);
	PWM_PHASE_SHIFT_RETURN_ON_INVALID_DUTY(shift_config);

	bk_err_t ret = 0;
	pwm_id_t id;
	pwm_ch_t hw_ch;

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PWM_2, PM_CPU_FRQ_120M);
	pwm_sw_ch_to_hw_id_ch(shift_config->duty_config[0].chan, &id, &hw_ch);

	uint32_t int_level = rtos_disable_int();
	memcpy(s_shift_config, shift_config, sizeof(pwm_phase_shift_config_t));
	pwm_hal_set_uie(&s_pwm[id].hal, hw_ch, 1);
	rtos_enable_int(int_level);

	return ret;
}

bk_err_t bk_pwm_phase_shift_set_duty(pwm_chan_t chan, uint32_t duty_cycle)
{
	BK_RETURN_ON_NULL(s_shift_config);
	if (duty_cycle == 1 || duty_cycle > s_shift_config->period_cycle) {
		return BK_ERR_PWM_PERIOD_DUTY;
	}

	for (uint32_t index = 0; index < s_shift_config->chan_num; index++) {
		if (s_shift_config->duty_config[index].chan == chan) {
			s_shift_config->duty_config[index].duty_cycle = duty_cycle;
			return BK_OK;
		}
	}

	return BK_ERR_PWM_CHAN_ID;
}

bk_err_t bk_pwm_phase_shift_update_duty(void)
{
	BK_RETURN_ON_NULL(s_shift_config);

	pwm_id_t id;
	pwm_ch_t hw_ch;

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PWM_2, PM_CPU_FRQ_120M);
	pwm_sw_ch_to_hw_id_ch(s_shift_config->duty_config[0].chan, &id, &hw_ch);
	uint32_t int_level = rtos_disable_int();
	pwm_hal_set_uie(&s_pwm[id].hal, hw_ch, 1);
	rtos_enable_int(int_level);

	return BK_OK;
}

#endif

static void pwm_set_flip_mode(pwm_ch_t sw_ch, uint32_t mode)
{
	pwm_ch_t hw_ch;
	pwm_id_t unit_id;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &unit_id, &hw_ch);
	pwm_hal_set_flip_mode(&s_pwm[unit_id].hal, hw_ch, mode);
}

static uint32_t pwm_get_flip_mode(pwm_ch_t sw_ch)
{
	pwm_ch_t hw_ch;
	pwm_id_t unit_id;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &unit_id, &hw_ch);
	return pwm_hal_get_flip_mode(&s_pwm[unit_id].hal, hw_ch);
}

static uint32_t pwm_get_tim_arr(pwm_ch_t sw_ch)
{
	pwm_ch_t hw_ch;
	pwm_id_t unit_id;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &unit_id, &hw_ch);
	return pwm_hal_get_tim_arr(&s_pwm[unit_id].hal, hw_ch);
}

static void pwm_group_update_config(pwm_group_t group)
{
	pwm_ch_t hw_ch;
	pwm_id_t unit_id;
	uint32_t chan1_flip_mode;
	uint32_t chan2_flip_mode;
	pwm_period_duty_config_t config1 = {0};
	pwm_period_duty_config_t config2 = {0};

	chan1_flip_mode = pwm_get_flip_mode(s_pwm_groups[group].chan1);
	chan2_flip_mode = pwm_get_flip_mode(s_pwm_groups[group].chan2);

	pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan1, &unit_id, &hw_ch);
	pwm_hal_t *hal = &s_pwm[unit_id].hal;

	/* chan1: duty ratio is 100%, chan2: duty ratio is 0% */
	if (s_pwm_groups[group].period_cycle == s_pwm_groups[group].chan1_duty_cycle &&
		s_pwm_groups[group].chan2_duty_cycle == 0) {
		if (chan1_flip_mode == 0x2 && chan2_flip_mode == 0x2) {
			pwm_hal_set_uie(hal, hw_ch, 0);
			pwm_set_flip_mode(s_pwm_groups[group].chan2, 4);
			pwm_set_flip_mode(s_pwm_groups[group].chan1, 4);
			s_pwm_groups[group].is_param_need_update = false;
			s_pwm_groups[group].is_flip_mode_need_update = false;
			return;
		}

		if (s_pwm_groups[group].is_flip_mode_need_update) {
			config1.period_cycle = pwm_get_tim_arr(s_pwm_groups[group].chan1) + 1;
			config1.duty_cycle = 0;
			config1.duty2_cycle = 0;
			config1.duty3_cycle = 0;

			config2.period_cycle = pwm_get_tim_arr(s_pwm_groups[group].chan2) + 1;
			config2.duty_cycle = 0;
			config2.duty2_cycle = 0;
			config2.duty3_cycle = 0;

			pwm_set_channel_config(s_pwm_groups[group].chan2, &config2);
			pwm_set_channel_config(s_pwm_groups[group].chan1, &config1);

			s_pwm_groups[group].is_flip_mode_need_update = false;
		} else {
			pwm_hal_set_uie(hal, hw_ch, 0);
			pwm_set_flip_mode(s_pwm_groups[group].chan1, 4);
			pwm_set_flip_mode(s_pwm_groups[group].chan2, 4);
			s_pwm_groups[group].is_param_need_update = false;
		}
	} else if ((s_pwm_groups[group].chan1_duty_cycle == 0) &&
		(s_pwm_groups[group].chan2_duty_cycle == s_pwm_groups[group].period_cycle)) {
		/* chan1: duty ratio is 0%, chan2: duty ratio is 100% */
		if (chan1_flip_mode == 0x4 && chan2_flip_mode == 0x4) {
			pwm_hal_set_uie(hal, hw_ch, 0);
			pwm_set_flip_mode(s_pwm_groups[group].chan1, 2);
			pwm_set_flip_mode(s_pwm_groups[group].chan2, 2);
			s_pwm_groups[group].is_param_need_update = false;
			s_pwm_groups[group].is_flip_mode_need_update = false;
			return;
		}

		if (s_pwm_groups[group].is_flip_mode_need_update) {
			pwm_set_flip_mode(s_pwm_groups[group].chan1, 1);
			pwm_set_flip_mode(s_pwm_groups[group].chan2, 3);
			s_pwm_groups[group].is_flip_mode_need_update = false;
		} else {
			pwm_hal_set_uie(hal, hw_ch, 0);
			pwm_set_flip_mode(s_pwm_groups[group].chan1, 2);
			pwm_set_flip_mode(s_pwm_groups[group].chan2, 2);
			s_pwm_groups[group].is_param_need_update = false;
		}
	} else if (chan1_flip_mode == 0x4 && chan2_flip_mode == 0x4) {
		if (s_pwm_groups[group].is_flip_mode_need_update) {
			config1.period_cycle = s_pwm_groups[group].period_cycle;
			config1.duty_cycle = s_pwm_groups[group].chan1_duty_cycle;
			config1.duty2_cycle = 0;
			config1.duty3_cycle = 0;

			config2.period_cycle = s_pwm_groups[group].period_cycle;
			config2.duty_cycle = s_pwm_groups[group].chan1_duty_cycle + s_pwm_groups[group].dead_cycle;
			config2.duty2_cycle = config2.duty_cycle + s_pwm_groups[group].chan2_duty_cycle;
			config2.duty3_cycle = 0;

			pwm_set_channel_config(s_pwm_groups[group].chan2, &config2);
			pwm_set_channel_config(s_pwm_groups[group].chan1, &config1);

			s_pwm_groups[group].is_flip_mode_need_update = false;
		} else {
			pwm_hal_set_uie(hal, hw_ch, 0);
			pwm_set_flip_mode(s_pwm_groups[group].chan1, 3);
			pwm_set_flip_mode(s_pwm_groups[group].chan2, 1);
			s_pwm_groups[group].is_param_need_update = false;
		}
	} else if (chan1_flip_mode == 0x2 && chan2_flip_mode == 0x2) {
		if (s_pwm_groups[group].is_flip_mode_need_update) {
			config1.period_cycle = s_pwm_groups[group].period_cycle;
			config1.duty_cycle = 0;
			config1.duty2_cycle = 0;
			config1.duty3_cycle = 0;

			config2.period_cycle = s_pwm_groups[group].period_cycle;
			config2.duty_cycle = s_pwm_groups[group].period_cycle - s_pwm_groups[group].dead_cycle;
			config2.duty2_cycle = 0;
			config2.duty3_cycle = 0;

			pwm_set_channel_config(s_pwm_groups[group].chan2, &config2);
			pwm_set_channel_config(s_pwm_groups[group].chan1, &config1);

			s_pwm_groups[group].is_flip_mode_need_update = false;
		} else {
			pwm_set_flip_mode(s_pwm_groups[group].chan1, 3);
			pwm_set_flip_mode(s_pwm_groups[group].chan2, 1);
			config1.period_cycle = s_pwm_groups[group].period_cycle;
			config1.duty_cycle = s_pwm_groups[group].chan1_duty_cycle;
			config1.duty2_cycle = 0;
			config1.duty3_cycle = 0;

			config2.period_cycle = s_pwm_groups[group].period_cycle;
			config2.duty_cycle = s_pwm_groups[group].chan1_duty_cycle + s_pwm_groups[group].dead_cycle;
			config2.duty2_cycle = config2.duty_cycle + s_pwm_groups[group].chan2_duty_cycle;
			config2.duty3_cycle = 0;

			pwm_set_channel_config(s_pwm_groups[group].chan2, &config2);
			pwm_set_channel_config(s_pwm_groups[group].chan1, &config1);
			pwm_hal_set_uie(hal, hw_ch, 0);
			s_pwm_groups[group].is_param_need_update = false;
		}
	} else {
		config1.period_cycle = s_pwm_groups[group].period_cycle;
		config1.duty_cycle = s_pwm_groups[group].chan1_duty_cycle;
		config1.duty2_cycle = 0;
		config1.duty3_cycle = 0;

		config2.period_cycle = s_pwm_groups[group].period_cycle;
		config2.duty_cycle = s_pwm_groups[group].chan1_duty_cycle + s_pwm_groups[group].dead_cycle;
		config2.duty2_cycle = config2.duty_cycle + s_pwm_groups[group].chan2_duty_cycle;
		config2.duty3_cycle = 0;

		pwm_set_channel_config(s_pwm_groups[group].chan2, &config2);
		pwm_set_channel_config(s_pwm_groups[group].chan1, &config1);
		pwm_set_flip_mode(s_pwm_groups[group].chan1, 3);
		pwm_set_flip_mode(s_pwm_groups[group].chan2, 1);
		pwm_hal_set_uie(hal, hw_ch, 0);
		s_pwm_groups[group].is_param_need_update = false;
		s_pwm_groups[group].is_flip_mode_need_update = false;
	}
}

static void pwm_isr_common(pwm_id_t id)
{
	pwm_hal_t *hal = &s_pwm[id].hal;
	uint32_t int_status;
	pwm_ch_t hw_ch;
	pwm_id_t unit_id;

	int_status = pwm_hal_get_pwm_interrupt_status(hal);
	pwm_hal_clr_interrupt_status(hal, int_status);

#if defined(CONFIG_PWM_PHASE_SHIFT)
	if (s_pwm_mode & PWM_PHASE_SHIFT_MODE) {
		pwm_phase_shift_set_duty();
	} else
#endif
	{
		for (pwm_group_t group = 0; group < PWM_GROUP_NUM; group++) {
			pwm_sw_ch_to_hw_id_ch(s_pwm_groups[group].chan1, &unit_id, &hw_ch);
			hal = &s_pwm[unit_id].hal;
			if (pwm_hal_is_uif_triggered(hal, hw_ch, int_status)) {
				if (s_pwm_groups[group].is_valid && s_pwm_groups[group].is_param_need_update) {
					pwm_group_update_config(group);
				}
			}
		}

		hal = &s_pwm[id].hal;
		for (hw_ch = 0; hw_ch < SOC_PWM_CHAN_NUM_PER_UNIT; hw_ch++) {
			if (pwm_hal_get_pwm_sr_ccxif(hal, hw_ch, int_status)) {
				// pwm_hal_set_cc1ie(&s_pwm[id].hal, hw_ch, 0);
				if (s_pwm[id].capture_sema) {
					rtos_set_semaphore(&s_pwm[id].capture_sema);
				}
				if (s_pwm_isr[id][hw_ch])
					s_pwm_isr[id][hw_ch](hw_ch);
			}
		}
	}
}

static void pwm0_isr(void)
{
	PWM_LOGD("%s\n", __func__);
	pwm_isr_common(0);
}

static void pwm1_isr(void)
{
	PWM_LOGD("%s\n", __func__);
	pwm_isr_common(1);
}
