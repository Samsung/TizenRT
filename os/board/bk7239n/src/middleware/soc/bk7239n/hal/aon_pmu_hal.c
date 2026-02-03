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
#include "aon_pmu_hal.h"
#include "aon_pmu_ll.h"
#include "sys_ana_ll.h"
#include "system_hw.h"
#include "sys_types.h"
#include "modules/pm.h"
#include <soc/bk7239n/soc.h>

bk_err_t aon_pmu_hal_init(void)
{
	aon_pmu_ll_set_r41_gpio_clksel(1);

	return BK_OK;
}

__FLASH_BOOT_CODE uint32_t aon_pmu_hal_get_chipid(void)
{
	return aon_pmu_ll_get_r7c_id();
}

__IRAM_SEC void aon_pmu_hal_clear_wakeup_source(wakeup_source_t value)
{
}

void aon_pmu_hal_set_wakeup_source(wakeup_source_t value)
{
}

void aon_pmu_hal_usbplug_int_en(uint32_t value)
{
	// uint32_t int_state = 0;
	// int_state = aon_pmu_ll_get_r1_usbplug_int_en();
	// int_state |= value;
	// aon_pmu_ll_set_r1_usbplug_int_en(int_state);
}

void aon_pmu_hal_touch_int_en(uint32_t value)
{
	// uint32_t int_state = 0;
	// int_state = aon_pmu_ll_get_r1_touch_int_en();
	// int_state |= value;
	// aon_pmu_ll_set_r1_touch_int_en(int_state);
}

uint32_t aon_pmu_hal_get_touch_int_status(void)
{
	// return aon_pmu_ll_get_reg73_mul_touch_int();
	return 0;
}

uint32_t aon_pmu_hal_get_cap_cal(void)
{
	// return aon_pmu_ll_get_reg73_cap_cal();
	return 0;
}

uint32_t aon_pmu_hal_get_touch_state(void)
{
	// return aon_pmu_ll_get_reg73_mul_touch_int();
	return 0;
}

uint32_t aon_pmu_hal_get_adc_cal()
{
	return aon_pmu_ll_get_r7d_adc_cal();
}

__FLASH_BOOT_CODE uint32_t aon_pmu_hal_get_dpll_unlock(uint32_t *unlockL, uint32_t *unlockH)
{
    aon_pmu_r7d_t r7d;

    r7d.v = aon_pmu_ll_get_r7d();
    if (NULL != unlockL) {
        *unlockL = r7d.dpll_unlockL;
    }
    if (NULL != unlockH) {
        *unlockH = r7d.dpll_unlockH;
    }

	return r7d.dpll_unlockL || r7d.dpll_unlockH;
}

/* Flash version for early boot (in FLASH) */
__FLASH_BOOT_CODE uint32_t aon_pmu_hal_get_dpll_band_flash()
{
	return aon_pmu_ll_get_r7d_dpll_band();
}

uint32_t aon_pmu_hal_get_dpll_band()
{
	return aon_pmu_ll_get_r7d_dpll_band();
}

__IRAM_SEC void aon_pmu_hal_reg_set(pmu_reg_e reg, uint32_t value)
{
    uint32_t pmu_reg_addr = SOC_AON_PMU_REG_BASE + reg * 4;
	REG_WRITE(pmu_reg_addr, value);
}
__IRAM_SEC uint32_t aon_pmu_hal_reg_get(pmu_reg_e reg)
{
    uint32_t pmu_reg_addr = SOC_AON_PMU_REG_BASE + reg * 4;
	return REG_READ(pmu_reg_addr);
}


void aon_pmu_hal_wdt_rst_dev_enable()
{
	uint32_t aon_pmu_r2 = 0;
	aon_pmu_r2 = aon_pmu_ll_get_r2();
	aon_pmu_r2 &= ~0x7;
	aon_pmu_r2 |= 0x6;

	aon_pmu_ll_set_r2(aon_pmu_r2);
}

void aon_pmu_hal_rtc_samp_sel(uint32_t value)
{
	aon_pmu_ll_set_r2_rtc_value_samp_sel(value);
}

void aon_pmu_hal_lpo_src_extern32k_enable(void)
{
	//Empty function
}

void aon_pmu_hal_lpo_src_set(uint32_t lpo_src)
{
	volatile uint32_t count = PM_POWER_ON_ROSC_STABILITY_TIME;
	if(lpo_src == PM_LPO_SRC_ROSC)
	{
		if(sys_ana_ll_get_ana_reg5_pwd_rosc_spi() != 0x0)
		{
			sys_ana_ll_set_ana_reg5_pwd_rosc_spi(0x0);//power on rosc
			while(count--)//delay time for stability when power on rosc
			{
			}
		}
	}

	if(aon_pmu_ll_get_r41_lpo_config() != lpo_src)
	{
		aon_pmu_ll_set_r41_lpo_config(lpo_src);
	}

	if(lpo_src == PM_LPO_SRC_ROSC)
	{
		if(sys_ana_ll_get_ana_reg5_en_xtall() == 0x1)
		{
			sys_ana_ll_set_ana_reg5_en_xtall(0x0);
		}

		if(sys_ana_ll_get_ana_reg5_itune_xtall() != 0xF)
		{
			sys_ana_ll_set_ana_reg5_itune_xtall(0xF);
		}
	}
}

__IRAM_SEC uint32_t aon_pmu_hal_lpo_src_get()
{
	return aon_pmu_ll_get_r41_lpo_config();
}

uint32_t aon_pmu_hal_bias_cal_get()
{
	return aon_pmu_ll_get_r7e_cbcal();
}

__IRAM_SEC uint32_t aon_pmu_hal_band_cal_get()
{
	return aon_pmu_ll_get_r7e_band_cal();
}
__IRAM_SEC uint32_t aon_pmu_hal_rtc_tick_h_get(void)
{
	return aon_pmu_ll_get_r78_rtc_tick_h();
}

__IRAM_SEC uint32_t aon_pmu_hal_rtc_tick_l_get(void)
{
	return aon_pmu_ll_get_r79_rtc_tick_l();
}

uint32_t aon_pmu_hal_get_wakeup_source(void)
{
	return aon_pmu_ll_get_r71_wakeup_source();
}

void aon_pmu_hal_r0_latch_to_r7b(void)
{
	aon_pmu_ll_set_r25(0x424B55AA);
	aon_pmu_ll_set_r25(0xBDB4AA55);
}

#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
uint32_t aon_pmu_hal_get_dlv_startup(void)
{
	return aon_pmu_ll_get_r7b_dlv_startup();
}

__IRAM_SEC uint32_t aon_pmu_hal_get_dlv_startup_iram(void)
{
	return aon_pmu_ll_get_r7b_dlv_startup();
}

__IRAM_SEC void aon_pmu_hal_set_dlv_startup(uint32_t value)
{
	uint32_t r0 = aon_pmu_ll_get_r0();
	uint32_t r7b = aon_pmu_ll_get_r7b();
	aon_pmu_ll_set_r0(r7b);
	aon_pmu_ll_set_r0_dlv_startup(value);
	aon_pmu_hal_r0_latch_to_r7b();
	aon_pmu_ll_set_r0(r0);
	aon_pmu_ll_set_r0_dlv_startup(value);
}

uint32_t aon_pmu_hal_get_reset_reason(void)
{
	return aon_pmu_ll_get_r7b_reset_reason();
}

void aon_pmu_hal_set_reset_reason(uint32_t value, bool write_immediately)
{
	if (write_immediately) {
		uint32_t r0 = aon_pmu_ll_get_r0();
		uint32_t r7b = aon_pmu_ll_get_r7b();
		aon_pmu_ll_set_r0(r7b);
		aon_pmu_ll_set_r0_reset_reason(value);
		aon_pmu_hal_r0_latch_to_r7b();
		delay_us(1);    //add delay to make sure ana value is set successfully
		aon_pmu_ll_set_r0(r0);
		aon_pmu_ll_set_r0_reset_reason(value);
	} else {
		aon_pmu_ll_set_r0_reset_reason(value);
	}
}

uint32_t aon_pmu_hal_get_gpio_sleep(void)
{
	return aon_pmu_ll_get_r7b_gpio_sleep();
}

__attribute__((section(".iram"))) void aon_pmu_hal_set_gpio_sleep(uint32_t value, bool write_immediately)
{
	if (write_immediately) {
		uint32_t r0 = aon_pmu_ll_get_r0();
		uint32_t r7b = aon_pmu_ll_get_r7b();
		aon_pmu_ll_set_r0(r7b);
		aon_pmu_ll_set_r0_gpio_sleep(value);
		aon_pmu_hal_r0_latch_to_r7b();
		aon_pmu_ll_set_r0(r0);
		aon_pmu_ll_set_r0_gpio_sleep(value);
	} else {
		aon_pmu_ll_set_r0_gpio_sleep(value);
	}
}
#else
uint32_t aon_pmu_hal_get_dlv_startup(void)
{
	uint32_t val = aon_pmu_ll_get_r7b();
	return (val & BIT(23)) >> 23;
}

__IRAM_SEC uint32_t aon_pmu_hal_get_dlv_startup_iram(void)
{
	uint32_t val = aon_pmu_ll_get_r7b();
	return (val & BIT(23)) >> 23;
}

__IRAM_SEC void aon_pmu_hal_set_dlv_startup(uint32_t value)
{
	aon_pmu_ll_set_r0_dlv_startup(value);
	aon_pmu_ll_set_r25(0x424B55AA);
	aon_pmu_ll_set_r25(0xBDB4AA55);
}

uint32_t aon_pmu_hal_get_reset_reason(void)
{
	return aon_pmu_ll_get_r0_reset_reason();
}

void aon_pmu_hal_set_reset_reason(uint32_t value)
{
	aon_pmu_ll_set_r0_reset_reason(value);
	aon_pmu_ll_set_r25(0x424B55AA);
	aon_pmu_ll_set_r25(0xBDB4AA55);
}
#endif

void aon_pmu_hal_gpio_clksel_set(uint32_t value)
{
	aon_pmu_ll_set_r41_gpio_clksel(value);
}

uint32_t aon_pmu_hal_gpio_clksel_get(void)
{
	return aon_pmu_ll_get_r41_gpio_clksel();
}

void aon_pmu_hal_set_halt_cpu(uint32_t value)
{
	aon_pmu_ll_set_r41_halt_cpu(value);
}

static uint32_t s_pmu_saved_regs[3] = {0};

__IRAM_SEC void aon_pmu_hal_backup(void)
{
	aon_pmu_ll_set_r25(0x424B55AA);
	aon_pmu_ll_set_r25(0xBDB4AA55);
	s_pmu_saved_regs[0] = aon_pmu_ll_get_r40();
	s_pmu_saved_regs[1] = aon_pmu_ll_get_r41();
	s_pmu_saved_regs[2] = aon_pmu_ll_get_r2();
}

__IRAM_SEC void aon_pmu_hal_restore(void)
{
	uint32_t reg = aon_pmu_ll_get_r7b();
	aon_pmu_ll_set_r0(reg);
	aon_pmu_ll_set_r40(s_pmu_saved_regs[0]);
	aon_pmu_ll_set_r41(s_pmu_saved_regs[1]);
	aon_pmu_ll_set_r2(s_pmu_saved_regs[2]);
}

#if CONFIG_AON_PMU_POR_RTC_RESET
void aon_pmu_hal_save_time(bool is_startup)
{
	uint32_t time = aon_pmu_ll_get_r79_rtc_tick_l() / 3200;
	aon_pmu_ll_set_r0_saved_time(time);
}

uint32_t aon_pmu_hal_get_por_timing()
{
	uint32_t time = aon_pmu_ll_get_r0_saved_time();
	aon_pmu_ll_set_r0_saved_time(0);
	return time;
}
#else
void aon_pmu_hal_save_time(bool is_startup)
{
	uint32_t time = aon_pmu_ll_get_r79_rtc_tick_l() / 3200; // unit=100ms
	uint32_t _time = aon_pmu_ll_get_r7b_saved_time();

	// use bit10 to indicate time value is over 0x1FF
	if (time > 0x1FF)
	{
		time = (time & 0x1FF) | BIT(10);
	}

	// no need to rewrite the same value into r7b
	if (time == _time)
	{
		return;
	}

	if (is_startup)
	{
		if (time < _time) {
			// indicate the real power off
			aon_pmu_ll_set_r0_saved_time(_time);
		}
		else {
			aon_pmu_ll_set_r0_saved_time(time);
		}
	}
	else
	{
		uint32_t r0 = aon_pmu_ll_get_r0();
		uint32_t r7b = aon_pmu_ll_get_r7b();
		aon_pmu_ll_set_r0(r7b);
		aon_pmu_ll_set_r0_saved_time(time);
		aon_pmu_hal_r0_latch_to_r7b();
		aon_pmu_ll_set_r0(r0);
		aon_pmu_ll_set_r0_saved_time(time);
	}
}

uint32_t aon_pmu_hal_get_por_timing(void)
{
	uint32_t time_pre = aon_pmu_ll_get_r7b_saved_time();
	uint32_t time_cur = aon_pmu_ll_get_r0_saved_time();
	int32_t delta_time = time_cur - time_pre;

	// the maxim valid timing range is 512 * 100ms = 51.2s
	if (delta_time < 0)
	{
		delta_time += 0x1FF;
	}

	return delta_time >= 0 ? delta_time : 0;
}
#endif

uint32_t aon_pmu_hal_get_ana_gpio_status(void)
{
	return aon_pmu_ll_get_r77_ana_gpio_sta();
}

uint32_t aon_pmu_hal_set_r0_fast_boot(uint32_t value)
{
	aon_pmu_ll_set_r0_fast_boot(value);
	return 0;
}
