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
#include "bk_pm_internal_api.h"
#include <driver/pwr_clk.h>
#include <sys/types.h>

#include "pm_mailbox.h"
#include "pm_debug.h"


#define PM_CP1_SOC_AON_RTC_REG_BASE          (SOC_AON_RTC_REG_BASE)
#define PM_AON_RTC_CNT_VAL_L_OFFSET          (0x3*4)
#define PM_AON_RTC_CNT_VAL_H_OFFSET          (0xa*4)

#define PM_AON_RTC_CNT_VAL_L_ADDR            (PM_CP1_SOC_AON_RTC_REG_BASE + PM_AON_RTC_CNT_VAL_L_OFFSET)
#define PM_AON_RTC_CNT_VAL_H_ADDR            (PM_CP1_SOC_AON_RTC_REG_BASE + PM_AON_RTC_CNT_VAL_H_OFFSET)

#define PM_SEND_CMD_CP0_RESPONSE_TIME_OUT    (100) //100ms
#define PM_SEND_CMD_CP1_RESPONSE_TIEM        (100)  //100ms
#define PM_BOOT_CP1_WAITING_TIEM             (500) // 0.5s


static uint8_t  s_debug_en               = 0;


#if CONFIG_MAILBOX
static uint64_t pm_cp1_aon_rtc_counter_get();
#endif


bk_err_t bk_pm_module_vote_power_ctrl(pm_power_module_name_e module, pm_power_module_state_e power_state)
{
#if CONFIG_MAILBOX
	uint64_t previous_tick  = 0;
	uint64_t current_tick   = 0;
	mb_chnl_cmd_t mb_cmd = {0};
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	bk_pm_cp1_pwr_ctrl_state_set(PM_MAILBOX_COMMUNICATION_INIT);
	mb_cmd.hdr.cmd = PM_POWER_CTRL_CMD;
	mb_cmd.param1 = module;
	mb_cmd.param2 = power_state;
	mb_cmd.param3 = 0;
	mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();

	previous_tick = pm_cp1_aon_rtc_counter_get();
	current_tick = previous_tick;
	while((current_tick - previous_tick) < (PM_SEND_CMD_CP0_RESPONSE_TIME_OUT*PM_AON_RTC_DEFAULT_TICK_COUNT))
	{
	    if (bk_pm_cp1_pwr_ctrl_state_get()) // wait the cp0 response
	    {
			break;
	    }
	    current_tick = pm_cp1_aon_rtc_counter_get();
	}

	if(!bk_pm_cp1_pwr_ctrl_state_get())
	{
	    BK_LOGD(NULL, "cp1 power_C:%d time out\r\n",module);
	}

	if (s_debug_en & 0x2)
		BK_LOGD(NULL, "cp1 vote power\r\n");
#endif
	return BK_OK;
}

bk_err_t bk_pm_module_vote_sleep_ctrl(pm_sleep_module_name_e module, uint32_t sleep_state, uint32_t sleep_time)
{
#if CONFIG_MAILBOX
	uint64_t previous_tick  = 0;
	uint64_t current_tick   = 0;

	if(module == PM_SLEEP_MODULE_NAME_LOG)
	{
		return BK_OK;
	}

	mb_chnl_cmd_t mb_cmd = {0};
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	bk_pm_cp1_sleep_ctrl_state_set(PM_MAILBOX_COMMUNICATION_INIT);
	mb_cmd.hdr.cmd = PM_SLEEP_CTRL_CMD;
	mb_cmd.param1 = module;
	mb_cmd.param2 = sleep_state;
	mb_cmd.param3 = sleep_time;
	mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();

	previous_tick = pm_cp1_aon_rtc_counter_get();
	current_tick = previous_tick;
	while((current_tick - previous_tick) < (PM_SEND_CMD_CP0_RESPONSE_TIME_OUT*PM_AON_RTC_DEFAULT_TICK_COUNT))
	{
	    if (bk_pm_cp1_sleep_ctrl_state_get()) // wait the cp0 response
	    {
			break;
	    }
	    current_tick = pm_cp1_aon_rtc_counter_get();
	}

	if(!bk_pm_cp1_sleep_ctrl_state_get())
	{
	    BK_LOGD(NULL, "cp1 wait cp0 vote sleep[%d] time out\r\n",module);
	}
#endif

	return BK_OK;
}

bk_err_t bk_pm_clock_ctrl(pm_dev_clk_e module, pm_dev_clk_pwr_e clock_state)
{
#if CONFIG_MAILBOX
	uint64_t previous_tick  = 0;
	uint64_t current_tick   = 0;
	mb_chnl_cmd_t mb_cmd    = {0};

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	bk_pm_cp1_clk_ctrl_state_set(PM_MAILBOX_COMMUNICATION_INIT);
	mb_cmd.hdr.cmd = PM_CLK_CTRL_CMD;
	mb_cmd.param1 = module;
	mb_cmd.param2 = clock_state;
	mb_cmd.param3 = 0;
	mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();

	previous_tick = pm_cp1_aon_rtc_counter_get();
	current_tick = previous_tick;
	while((current_tick - previous_tick) < (PM_SEND_CMD_CP0_RESPONSE_TIME_OUT*PM_AON_RTC_DEFAULT_TICK_COUNT))
	{
	    if (bk_pm_cp1_clk_ctrl_state_get()) // wait the cp0 response
	    {
			break;
	    }
	    current_tick = pm_cp1_aon_rtc_counter_get();
	}

	if(!bk_pm_cp1_clk_ctrl_state_get())
	{
	    BK_LOGD(NULL, "cp1 vote freq[%d] time out\r\n",module);
	}
#endif

	return BK_OK;
}

bk_err_t bk_pm_module_vote_cpu_freq(pm_dev_id_e module, pm_cpu_freq_e cpu_freq)
{
#if CONFIG_MAILBOX
	uint64_t previous_tick  = 0;
	uint64_t current_tick   = 0;

	mb_chnl_cmd_t mb_cmd    = {0};
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	bk_pm_cp1_cpu_freq_ctrl_state_set(PM_MAILBOX_COMMUNICATION_INIT);
	mb_cmd.hdr.cmd = PM_CPU_FREQ_CTRL_CMD;
	mb_cmd.param1 = module;
	mb_cmd.param2 = cpu_freq;
	mb_cmd.param3 = 0;
	mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();

	previous_tick = pm_cp1_aon_rtc_counter_get();
	current_tick = previous_tick;
	BK_LOGD(NULL, "cp1 vote freq_B[%lld]\r\n",previous_tick);
	while((current_tick - previous_tick) < (PM_SEND_CMD_CP0_RESPONSE_TIME_OUT*PM_AON_RTC_DEFAULT_TICK_COUNT))
	{
	    if (bk_pm_cp1_cpu_freq_ctrl_state_get()) // wait the cp0 response
	    {
			break;
	    }
	    current_tick = pm_cp1_aon_rtc_counter_get();
	}

	if(!bk_pm_cp1_cpu_freq_ctrl_state_get())
	{
	    BK_LOGD(NULL, "cp1 vote freq[%d]time out\r\n",module);
	}
	BK_LOGD(NULL, "cp1 vote freq_E[%lld]\r\n",current_tick);

	if(s_debug_en&0x2)
		BK_LOGD(NULL, "cpu1 vote cpu freq\r\n");
#endif

	return BK_OK;
}

bk_err_t bk_pm_module_vote_psram_ctrl(pm_power_psram_module_name_e module,pm_power_module_state_e power_state)
{
#if CONFIG_MAILBOX
	uint64_t previous_tick  = 0;
	uint64_t current_tick   = 0;

	bk_err_t ret = BK_OK;
	mb_chnl_cmd_t mb_cmd = {0};
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	bk_pm_cp1_psram_power_state_set(PM_MAILBOX_COMMUNICATION_INIT);
	mb_cmd.hdr.cmd = PM_CTRL_PSRAM_POWER_CMD;
	mb_cmd.param1 = module;
	mb_cmd.param2 = power_state;
	ret = mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();
	BK_LOGD(NULL, "cp1 vote psram_P B:%d\r\n",ret);
	while(ret != BK_OK)
	{
		ret = mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
		rtos_delay_milliseconds(4);
	}

	previous_tick = pm_cp1_aon_rtc_counter_get();
	current_tick = previous_tick;
	while((current_tick - previous_tick) < (PM_SEND_CMD_CP1_RESPONSE_TIEM*PM_AON_RTC_DEFAULT_TICK_COUNT))
	{
	    if (bk_pm_cp1_psram_power_state_get()) // wait the cp0 response
	    {
			break;
	    }
	    current_tick = pm_cp1_aon_rtc_counter_get();
	}

	if(!bk_pm_cp1_psram_power_state_get())
	{
	    BK_LOGD(NULL, "cp1 get psram sta timeout\r\n");
	}

	BK_LOGD(NULL, "cp1 vote psram_P E\r\n");
#endif

	return BK_OK;
}

bk_err_t bk_pm_module_vote_ctrl_external_ldo(gpio_ctrl_ldo_module_e module,gpio_id_t gpio_id,gpio_output_state_e value)
{
#if CONFIG_MAILBOX
	uint64_t previous_tick  = 0;
	uint64_t current_tick   = 0;

	mb_chnl_cmd_t mb_cmd = {0};
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	bk_pm_cp1_external_ldo_ctrl_state_set(PM_MAILBOX_COMMUNICATION_INIT);
	mb_cmd.hdr.cmd = PM_CTRL_EXTERNAL_LDO_CMD;
	mb_cmd.param1 = module;
	mb_cmd.param2 = gpio_id;
	mb_cmd.param3 = value;
	mb_chnl_write(MB_CHNL_PWC, &mb_cmd);
	GLOBAL_INT_RESTORE();

	previous_tick = pm_cp1_aon_rtc_counter_get();
	current_tick = previous_tick;
	while((current_tick - previous_tick) < (PM_BOOT_CP1_WAITING_TIEM*PM_AON_RTC_DEFAULT_TICK_COUNT))
	{
	    if (bk_pm_cp1_external_ldo_ctrl_state_get()) // wait the cp0 response
	    {
			break;
	    }
	    current_tick = pm_cp1_aon_rtc_counter_get();
	}

	if(!bk_pm_cp1_external_ldo_ctrl_state_get())
	{
	    BK_LOGD(NULL, "cp1 ctr extLdo timeout\r\n");
	}

	BK_LOGD(NULL, "cp1 vote ctr_extLdo\r\n");
#endif

	return BK_OK;
}

#if CONFIG_MAILBOX
static uint64_t pm_cp1_aon_rtc_counter_get()
{
	volatile uint32_t val = REG_READ(PM_AON_RTC_CNT_VAL_L_ADDR);
	volatile uint32_t val_hi = REG_READ(PM_AON_RTC_CNT_VAL_H_ADDR);

	while (REG_READ(PM_AON_RTC_CNT_VAL_L_ADDR) != val
		|| REG_READ(PM_AON_RTC_CNT_VAL_H_ADDR) != val_hi)
	{
		val = REG_READ(PM_AON_RTC_CNT_VAL_L_ADDR);
		val_hi = REG_READ(PM_AON_RTC_CNT_VAL_H_ADDR);
	}
	return (((uint64_t)(val_hi) << 32) + val);
}
#endif

/*=========================DEBUG/TEST CTRL START========================*/
uint32_t pm_debug_mode()
{
	return s_debug_en;
}

void pm_debug_ctrl(uint32_t debug_en)
{
	s_debug_en = debug_en;
}
/*=========================DEBUG/TEST CTRL END========================*/