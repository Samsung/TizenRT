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
#include "pm_debug.h"
#include "sys_driver.h"
#include "aon_pmu_driver.h"


#define PM_HIGHEST_CPU_FREQ                     (CONFIG_PM_CPU_FRQ_HIGHEST)

static uint8_t s_pm_cpu_freq[PM_DEV_ID_MAX];
static pm_cpu_freq_e s_pm_current_cpu_freq;

typedef struct
{
	pm_cb_extern32k_cfg_t cfg[PM_32K_MODULE_MAX];
	uint32_t module_count;
} pm_cb_module_cfg_t;

static pm_cb_module_cfg_t s_pm_cb_module_cfg;
static beken_semaphore_t s_sync_sema = NULL;


/*=========================CLK/FREQ CTRL START========================*/
bk_err_t pm_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core, uint32_t ckdiv_bus, uint32_t ckdiv_cpu0, uint32_t ckdiv_cpu1)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	sys_drv_core_bus_clock_ctrl(cksel_core, ckdiv_core, ckdiv_bus, ckdiv_cpu0, ckdiv_cpu1);
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

pm_cpu_freq_e bk_pm_current_max_cpu_freq_get()
{
	return s_pm_current_cpu_freq;
}

pm_cpu_freq_e bk_pm_module_current_cpu_freq_get(pm_dev_id_e module)
{
	return s_pm_cpu_freq[module];
}

bk_err_t bk_pm_module_vote_cpu_freq(pm_dev_id_e module, pm_cpu_freq_e cpu_freq)
{
	bk_err_t ret            = BK_OK;
	uint32_t i              = 0;
	uint32_t freq_max       = 0;
	uint32_t freq_max_index = 0;
	bool bcpu_freq_highest  = false;

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	#if CONFIG_PM_CPU_FIXED_FREQ_ENABLE
	cpu_freq = (pm_cpu_freq_e)CONFIG_PM_CPU_FIXED_FREQ;
	#endif

	/*save the cpu freq first*/
	if (PM_CPU_FRQ_DEFAULT == cpu_freq)
	{
		cpu_freq = PM_CPU_FRQ_XTAL; // it will use the PM_DEV_ID_DEFAULT vote cpu frequency
	}

	/*Appli do not need to be concerned with the specific CPU frequency*/
	if(cpu_freq == PM_CPU_FRQ_HIGHEST)
	{
		cpu_freq = PM_HIGHEST_CPU_FREQ;
		bcpu_freq_highest = true;
	}
	else
	{
		bcpu_freq_highest = false;
	}

	s_pm_cpu_freq[module] = (uint8_t)cpu_freq;

	/*get the max cpu freq*/
	freq_max = s_pm_cpu_freq[0];
	for (i = 1; i < PM_DEV_ID_MAX; i++)
	{
		if (freq_max < s_pm_cpu_freq[i])
		{
			freq_max = s_pm_cpu_freq[i];
			freq_max_index = i;
		}
	}
	if(bcpu_freq_highest == true)
	{
		s_pm_cpu_freq[module] = (uint8_t)PM_CPU_FRQ_HIGHEST;
	}

	/*dpd need cpu freq 120M, when dpd calibration, it force dpd need cpu freq, if dpd cali finish, restore it*/
	if((module == PM_DEV_ID_PHY_DPD_CALI)&&(cpu_freq != PM_CPU_FRQ_XTAL))
	{
		freq_max = cpu_freq;
	}
	else
	{
		//when dpd cali finish, it will use the highest cpu freq in votes cpu freq.
		if(freq_max == PM_CPU_FRQ_HIGHEST)
		{
			freq_max = PM_HIGHEST_CPU_FREQ;
		}
	}
	/*updete the current cpu frequency*/
	if (s_pm_current_cpu_freq == freq_max)
	{
		GLOBAL_INT_RESTORE();
		return BK_OK;
	}
	else
	{
		ret = sys_drv_switch_cpu_bus_freq(freq_max);
	}
	if (ret == BK_OK)
	{
		if(bcpu_freq_highest == true)
		{
			s_pm_current_cpu_freq = PM_CPU_FRQ_HIGHEST;
		}
		else
		{
			s_pm_current_cpu_freq = freq_max;
		}
	}

	GLOBAL_INT_RESTORE();

	if (ret != BK_OK)
	{
		LOGI("switch cpu freq error\r\n");
		return ret;
	}
	//if (s_debug_en & 0x2)
	LOGI("Switch cpu freq %d %d\r\n", freq_max, freq_max_index);
	return BK_OK;
}

bk_err_t bk_pm_clock_ctrl(pm_dev_clk_e module, pm_dev_clk_pwr_e clock_state)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	sys_drv_dev_clk_pwr_up(module, clock_state);
	GLOBAL_INT_RESTORE();
	return BK_OK;
}

pm_lpo_src_e bk_pm_lpo_src_get()
{
	return (pm_lpo_src_e)aon_pmu_drv_lpo_src_get();
}

bk_err_t bk_pm_lpo_src_set(pm_lpo_src_e lpo_src)
{
	aon_pmu_drv_lpo_src_set((uint32_t)lpo_src);
	return BK_OK;
}

bk_err_t bk_pm_rosc_calibration(pm_rosc_cali_mode_e rosc_cali_mode, uint32_t cali_interval)
{
	bk_err_t ret = BK_OK;
	ret = sys_drv_rosc_calibration(rosc_cali_mode, cali_interval);
	if (ret != BK_OK)
	{
		BK_LOGD(NULL, "set rosc calibration parameter error %d %d \r\n", rosc_cali_mode, cali_interval);
		return ret;
	}
	return BK_OK;
}

bk_err_t pm_clk_32k_source_notify_cb(void)
{
	bk_err_t ret;

	ret = rtos_set_semaphore(&s_sync_sema);

	return ret;
}

bk_err_t pm_extern32k_register_cb(pm_cb_extern32k_cfg_t *cfg)
{
	if (cfg == NULL)
		return BK_FAIL;

	s_pm_cb_module_cfg.cfg[cfg->cb_module].cb_func = cfg->cb_func;

	return BK_OK;
}

bk_err_t pm_extern32k_unregister_cb(pm_cb_extern32k_cfg_t *cfg)
{
	if (cfg == NULL)
		return BK_FAIL;

	s_pm_cb_module_cfg.cfg[cfg->cb_module].cb_func = NULL;

	return BK_OK;
}

bk_err_t pm_extern32k_cb_exec(pm_sw_step_e sw_step, pm_lpo_src_e lpo_src)
{
	int i;
	s_pm_cb_module_cfg.module_count = 0;

	for (i = 0; i < PM_32K_MODULE_MAX; i++)
	{
		if (s_pm_cb_module_cfg.cfg[i].cb_func)
		{
			/*call cb and register notify callback*/
			s_pm_cb_module_cfg.module_count++;
			s_pm_cb_module_cfg.cfg[i].cb_func(sw_step, lpo_src, pm_clk_32k_source_notify_cb);
		}
	}

	return BK_OK;
}

bk_err_t pm_clk_32k_source_switch(pm_lpo_src_e lpo_src)
{
#if CONFIG_EXTERN_32K_PIN_MULT
	pm_lpo_src_e clk_src;
	bk_err_t ret;
	uint32_t count;

	clk_src = bk_clk_32k_customer_config_get();

	if (pm_debug_mode() & 0x2)
	{
		BK_LOGD(NULL, "customer set clk %d\r\n", clk_src);
	}

	if (clk_src != PM_LPO_SRC_X32K)
	{
		return BK_OK;
	}

	clk_src = bk_pm_lpo_src_get();
	if (pm_debug_mode() & 0x2)
	{
		BK_LOGD(NULL, "current clk %d,set clk %d\r\n", clk_src, lpo_src);
	}

	if (clk_src == lpo_src)
	{
		return BK_OK;
	}

	if (NULL == s_sync_sema)
	{
		rtos_init_semaphore(&s_sync_sema, PM_32K_MODULE_MAX);
	}

	/*when close DVP,wait for the external clock to stabilize*/
	if (lpo_src == PM_LPO_SRC_X32K)
	{
		aon_pmu_drv_lpo_src_extern32k_enable();
		rtos_delay_milliseconds(SWITCH_32K_DELAY);
	}

	/*Before clock switch,execute BTWF callback and wait BTWF notify*/
	pm_extern32k_cb_exec(PM_32K_STEP_BEGIN, lpo_src);

	count = s_pm_cb_module_cfg.module_count;
	while (count)
	{
		/*Wait for the BTWF clock switch to complete*/
		ret = rtos_get_semaphore(&s_sync_sema, SWITCH_32K_WAIT);
		if (ret < 0)
		{
			BK_LOGD(NULL, " ERR: wait sema timeout\r\n");
			rtos_deinit_semaphore(&s_sync_sema);
			return BK_FAIL;
		}
		count--;
	}

	/*switch 32k source*/
	bk_pm_lpo_src_set(lpo_src);

	/*after clock switch,execute BTWF callback and wait BTWF notify*/
	pm_extern32k_cb_exec(PM_32K_STEP_FINISH, lpo_src);

	rtos_deinit_semaphore(&s_sync_sema);
#endif
	return BK_OK;
}
/*=========================CLK/FREQ CTRL END========================*/
