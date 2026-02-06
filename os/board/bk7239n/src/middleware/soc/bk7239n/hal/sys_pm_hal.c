// Copyright 2022-2025 Beken
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
#include "sys_hal.h"
#include "sys_ll.h"
#include "sys_ana_ll.h"
#include "aon_pmu_hal.h"
#include "sys_types.h"
#include <driver/aon_rtc.h>
#include <driver/hal/hal_spi_types.h>
#include "bk_arch.h"
#include "hal_port.h"
#include <os/os.h>
#include "sys_pm_hal.h"
#include "sys_pm_hal_ctrl.h"
#include "modules/pm.h"
#include <driver/pwr_clk.h>
#include "driver/flash.h"
#if defined(CONFIG_INT_WDT)
#include <driver/wdt.h>
#include <bk_wdt.h>
#endif
#if defined(CONFIG_CACHE_ENABLE)
#include "cache.h"
#endif
//#include "FreeRTOS.h"
#include "armstar.h"
#if defined(CONFIG_DEEP_LV)
#include "deep_lv.h"
#endif
#if defined(CONFIG_MPU)
#include "mpu.h"
#endif
#include "sys_driver.h"
#if CONFIG_TEMP_DETECT
#include "components/sensor.h"
#endif

#define portNVIC_SYSTICK_CTRL_REG             ( *( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG             ( *( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SHPR3_REG                    ( *( ( volatile uint32_t * ) 0xe000ed20 ) )

#define portNVIC_INT_CTRL_REG                 ( *( ( volatile uint32_t * ) 0xe000ed04 ) )

#define portNVIC_SYSTICK_ENABLE_BIT           ( 1UL << 0UL )
#define portNVIC_SYSTICK_INT_BIT              ( 1UL << 1UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT       ( 1UL << 16UL )

#define portNVIC_PENDSVSET_BIT                ( 1UL << 28UL )
#define portNVIC_PENDSVCLR_BIT                ( 1UL << 27UL )
#define portNVIC_SYSTICKSET_BIT               ( 1UL << 26UL )
#define portNVIC_SYSTICKCLR_BIT               ( 1UL << 25UL )

#define PM_DELAY_TICKS_PER_MS                 (32)
#define PM_EXIT_LOWVOL_SYSTICK_TIME           (32)      //1ms
#define PM_EXIT_LOWVOL_SYSTICK_RELOAD_TIME    (0xFFFFFF)//set max
#if defined(CONFIG_SOFT_CTRL_VOLT)
#define PM_LOW_VOL_VCOREHSEL_LDO_SEL          (6)       // 0.75V
#endif

extern void delay_us(uint32_t us);
extern void flush_all_dcache(void);
#define DELAY_US(us)						  delay_us(us)

#if defined(CONFIG_OTA_POSITION_INDEPENDENT_AB)
#define FLASH_BASE_ADDRESS                    (0x44030000)
#define FLASH_OFFSET_ADDR_BEGIN               (0x16)
#define FLASH_OFFSET_ADDR_END                 (0x17)
#define FLASH_ADDR_OFFSET                     (0x18)
#define FLASH_OFFSET_ENABLE                   (0x19)

typedef struct
{
	uint32_t flash_offset_enable_val;
	uint32_t offset_addr_begin_val;
	uint32_t offset_addr_end_val ;
	uint32_t flash_addr_offset_val;
}flash_ab_reg_t;

#endif

#if defined(CONFIG_GPIO_RETENTION_SUPPORT)
#define GPIO_RETENTION_MAP_SIZE            (8)
#define GPIO_NUM_MAX       (SOC_GPIO_NUM)
#define GPIO_RETENTION_MAP                 {GPIO_24, GPIO_6, GPIO_NUM_MAX, GPIO_NUM_MAX, GPIO_NUM_MAX, GPIO_NUM_MAX, GPIO_NUM_MAX, GPIO_NUM_MAX}
static uint32_t s_gpio_retention_map[GPIO_RETENTION_MAP_SIZE] = GPIO_RETENTION_MAP;
#endif

uint64_t low_voltage_exit_tick = 0;
uint64_t low_voltage_sleep_duration_us = 0;
uint64_t low_voltage_wakeup_time_us = 0;

static inline bool is_lpo_src_26m32k(void)
{
	return (aon_pmu_ll_get_r41_lpo_config() == SYS_LPO_SRC_26M32K);
}

static inline bool is_lpo_src_ext32k(void)
{
	return (aon_pmu_ll_get_r41_lpo_config() == SYS_LPO_SRC_EXTERNAL_32K);
}

static inline bool is_wifi_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_WIFI);
}

static inline bool is_gpio_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_GPIO);
}

static inline bool is_rtc_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_RTC);
}

static inline bool is_bt_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_BT);
}

static inline bool is_usbplug_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_USBPLUG);
}

static inline bool is_touch_ws_enabled(uint8_t ena_bits)
{
	return !!(ena_bits & WS_TOUCH);
}

static inline bool is_wifi_pd_poweron(uint32_t pd_bits)
{
	return !(pd_bits & PD_WIFI);
}

static inline bool is_btsp_pd_poweron(uint32_t pd_bits)
{
	return !(pd_bits & PD_BTSP);
}

static inline void sys_hal_restore_int(volatile uint32_t int_state1, volatile uint32_t int_state2)
{
	sys_ll_set_cpu0_int_0_31_en_value(int_state1);
	sys_ll_set_cpu0_int_32_63_en_value(int_state2);
}

static inline void sys_hal_enable_wakeup_int(void)
{
}

static inline void sys_hal_set_core_freq(volatile uint8_t cksel_core, volatile uint8_t clkdiv_core, volatile uint8_t clkdiv_bus)
{
    sys_ll_set_cpu_clk_div_mode1_cksel_core(cksel_core);
    sys_ll_set_cpu_clk_div_mode1_clkdiv_core(clkdiv_core);
    ;//sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(clkdiv_bus);
}

static inline void sys_hal_set_core_26m(void)
{
	sys_hal_set_core_freq(0, 0, 0);
}

static inline void sys_hal_backup_set_core_26m(volatile uint8_t *cksel_core, volatile uint8_t *clkdiv_core, volatile uint8_t *clkdiv_bus)
{
	IF_LV_CTRL_CORE() {
		*cksel_core = sys_ll_get_cpu_clk_div_mode1_cksel_core();
		*clkdiv_core = sys_ll_get_cpu_clk_div_mode1_clkdiv_core();
		;//*clkdiv_bus = sys_ll_get_cpu_clk_div_mode1_clkdiv_bus();
		sys_hal_set_core_freq(0, 0, 0);
	}
}

static inline void sys_hal_restore_core_freq(volatile uint8_t cksel_core, volatile uint8_t clkdiv_core, volatile uint8_t clkdiv_bus)
{
	IF_LV_CTRL_CORE() {
		;//sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(clkdiv_bus);
		sys_ll_set_cpu_clk_div_mode1_clkdiv_core(clkdiv_core);
		sys_ll_set_cpu_clk_div_mode1_cksel_core(cksel_core);
	}
}

static inline void sys_hal_set_flash_freq(volatile uint8_t cksel_flash, volatile uint8_t ckdiv_flash)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(cksel_flash);
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(ckdiv_flash);
}

static inline void sys_hal_set_flash_26m(void)
{
	sys_hal_set_flash_freq(0, 0);
}

static inline void sys_hal_backup_set_flash_26m(volatile uint8_t *cksel_flash, volatile  uint8_t *ckdiv_flash)
{
	IF_LV_CTRL_FLASH() {
		*cksel_flash = sys_ll_get_cpu_clk_div_mode2_cksel_flash();
		*ckdiv_flash = sys_ll_get_cpu_clk_div_mode2_ckdiv_flash();
		sys_ll_set_cpu_clk_div_mode2_cksel_flash(0);//eg:from the 80m to 26m, it need select clk source first
		sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(0);//then ckdiv
	}
}

static inline void sys_hal_set_flash_120m(void)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(2);
}

static inline void sys_hal_set_anaspi_freq(uint8_t anaspi_freq)
{
	sys_ll_set_cpu_anaspi_freq_value(anaspi_freq);
}

static inline void sys_hal_backup_set_anaspi_freq_26m(uint8_t *anaspi_freq)
{
	*anaspi_freq = sys_ll_get_cpu_anaspi_freq_value();
	sys_hal_set_anaspi_freq(1); //26M/4
}

static inline void sys_hal_restore_anaspi_freq(uint8_t anaspi_freq)
{
	sys_hal_set_anaspi_freq(anaspi_freq);
}

static inline void sys_hal_restore_flash_freq(volatile uint8_t cksel_flash, volatile uint8_t ckdiv_flash)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(ckdiv_flash);//eg:from the 26m to 80m, it need config clk div first
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(cksel_flash);//then clk source
}

static inline void sys_hal_mask_cpu0_int(void)
{
	sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(1);
}

static inline void sys_hal_enable_spi_latch(void)
{
	 sys_ana_ll_set_ana_reg8_spi_latch1v(1);
}

static inline void sys_hal_disable_spi_latch(void)
{
	 sys_ana_ll_set_ana_reg8_spi_latch1v(0);
}

static inline uint32_t sys_hal_disable_hf_clock(void)
{
	uint32_t val = sys_ana_ll_get_ana_reg5_value();
	uint32_t ret_val = val;

	val &= ~EN_ALL;

	if (is_lpo_src_ext32k()) {
	val |= EN_XTAL;
	}

	sys_ana_ll_set_ana_reg5_value(val);

	return ret_val;

}

static inline void sys_hal_restore_hf_clock(volatile uint32_t val)
{
	sys_ana_ll_set_ana_reg5_value(val);
}

/**
 * buck power supply switch
 *
 * uint32_t type input:
 *   0: close buck
 *   other: open buck
 *
 * note: please declare this function as static when buck is stable
*/
void sys_hal_buck_switch(uint32_t flag)
{
	volatile uint8_t cksel_core = 0, clkdiv_core = 0, clkdiv_bus = 0;

	if (flag == 0) {
		os_printf("disable buckA.\r\n");
		sys_hal_enable_spi_latch();
		sys_ana_ll_set_ana_reg10_aldosel(1);
		sys_hal_disable_spi_latch();
	} else if (flag == 1){
		os_printf("enable buckA.\r\n");
		//let the cpu frequency to 26m, in order to be successfully switch voltage provide from ldo to buck
		sys_hal_backup_set_core_26m(&cksel_core, &clkdiv_core, &clkdiv_bus);

		sys_hal_enable_spi_latch();
		sys_ana_ll_set_ana_reg10_aldosel(0);
		sys_hal_disable_spi_latch();

		sys_hal_restore_core_freq(cksel_core, clkdiv_core, clkdiv_bus);
	} else {
		os_printf("set buck power supply param %d must < 2 \r\n",flag);
	}
}


/**
 * high digital voltage
 *
 * uint32_t value input:
 *   voltage value
 *
 *
 * note: please declare this function
*/
void sys_hal_v_core_h_sel(uint32_t value)
{
	sys_hal_enable_spi_latch();
	if(sys_ana_ll_get_ana_reg8_vcorehsel() != value)
	{
		sys_ana_ll_set_ana_reg8_vcorehsel(value);
	}
	sys_hal_disable_spi_latch();
}

static inline void sys_hal_deep_sleep_set_buck(void)
{
	//TODO: fix me when bring up pm
	// sys_ll_set_ana_reg11_aldosel(1);
	// sys_ll_set_ana_reg12_dldosel(1);
}

static inline void sys_hal_deep_sleep_set_vldo(void)
{
	// sys_ll_set_ana_reg8_coreldo_hp(0);
	/*dldohp disabling causes OTP read failed!, so it can't set 0*/
	sys_ana_ll_set_ana_reg7_dldohp(1);

	sys_ana_ll_set_ana_reg7_aldohp(0); //20230423 tenglong
}

static inline void sys_hal_clear_wakeup_source(void)
{
	// aon_pmu_ll_set_r43_clr_wakeup(1);
	// aon_pmu_ll_set_r43_clr_wakeup(0);
}

static inline void sys_hal_set_halt_config(void)
{
	uint32_t v = aon_pmu_ll_get_r41();

	//halt_lpo = 1
	//halt_busrst = 0
	//halt_busiso = 1, halt_buspwd = 1
	//halt_blpiso = 1, halt_blppwd = 1
	//halt_wlpiso = 1, halt_wlppwd = 1
	v |= (0xFD << 24);
	aon_pmu_ll_set_r41(v);
}
static inline void sys_hal_power_on_and_select_rosc(pm_lpo_src_e lpo_src)
{
	volatile uint32_t count = PM_POWER_ON_ROSC_STABILITY_TIME;
	if((lpo_src == PM_LPO_SRC_ROSC)||(lpo_src == PM_LPO_SRC_DIVD))
	{
		if(sys_ana_ll_get_ana_reg5_pwd_rosc_spi() != 0x0)
		{
			sys_ana_ll_set_ana_reg5_pwd_rosc_spi(0x0);//power on rosc
			while(count--)//delay time for stability when power on rosc
			{
			}
		}

		if(aon_pmu_ll_get_r41_lpo_config() != PM_LPO_SRC_ROSC)
		{
			aon_pmu_ll_set_r41_lpo_config(PM_LPO_SRC_ROSC);
		}
	}
}
static inline void sys_hal_set_power_parameter(uint8_t sleep_mode)
{
	/*  r40[3:0]   wake1_delay = 0x1;
 	 *  r40[7:4]   wake2_delay = 0x1;
 	 *  r40[11:8]  wake3_delay = 0x1;
 	 *  r40[15:12] halt1_delay = 0x1;
 	 *  r40[19:16] halt2_delay = 0x1;
 	 *  r40[23:20] halt3_delay = 0x1;
 	 *  r40[24] halt_volt: deep = 0, lv = 1
 	 *  r40[25] halt_xtal = 1 //26M32K unable to wakeup system, disable during lv sleep
 	 *  r40[26] halt_core: deep = 1, lv = 0
 	 *  r40[27] halt_flash = 1
 	 *  r40[28] halt_rosc = 0
 	 *  r40[29] halt_resten: deep = 0, lv = 1
 	 *  r40[30] halt_isolat = 1
 	 *  r40[31] halt_clkena = 0
 	 **/
	if (sleep_mode == PM_MODE_DEEP_SLEEP)
	{
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
		aon_pmu_ll_set_r40(0x5E111111);//using the external 32k , it need more wake delay time
#else
		// OTP need more delay to recovery voltage
		aon_pmu_ll_set_r40(0x5E111111);
#endif
	}
	else
	{
		uint32_t val;
		#if CONFIG_LV_FLASH_ENTER_LP_ENABLE
		val = (0x63111000
			|(PM_CURRENT_LOW_VOLTAGE_WAKEUP1_DELAY&0xF)
			|((PM_CURRENT_LOW_VOLTAGE_WAKEUP2_DELAY&0xF)<<4)
			|((PM_CURRENT_LOW_VOLTAGE_WAKEUP3_DELAY&0xF)<<8));
		#else
		val = (0x6B111000
			|(PM_CURRENT_LOW_VOLTAGE_WAKEUP1_DELAY&0xF)
			|((PM_CURRENT_LOW_VOLTAGE_WAKEUP2_DELAY&0xF)<<4)
			|((PM_CURRENT_LOW_VOLTAGE_WAKEUP3_DELAY&0xF)<<8));
		#endif

		aon_pmu_ll_set_r40(val);
	}
}

static inline void sys_hal_set_sleep_condition(void)
{
	uint32_t v = sys_ll_get_cpu_power_sleep_wakeup_value();

	//sleep_bus_idle_bypass =  0
	//sleep_en_global = 1
	//sleep_en_need_cpu0_wfi = 0
	//sleep_en_need_flash_idle = 0
	v &= (~0xF0000);
	v |= (0x8 << 16);

	sys_ll_set_cpu_power_sleep_wakeup_value(v);
}

static inline void sys_hal_power_down_pd(volatile uint32_t *pd_reg_v)
{
	uint32_t value = 0;
	IF_LV_CTRL_PD() {
		uint32_t v = sys_ll_get_cpu_power_sleep_wakeup_value();
		*pd_reg_v = v;

		/*config power domain*/
		v |= PD_DOWN_DOMAIN;
#if 0
		/*config r41 bus wl,bl pw and iso*/
		sys_hal_set_halt_config();

		if (is_wifi_pd_poweron(v)) {
			aon_pmu_ll_set_r41_halt_wlpiso(1);
			aon_pmu_ll_set_r41_halt_wlppwd(0);
		}
		else {
			aon_pmu_ll_set_r41_halt_wlpiso(1);
			aon_pmu_ll_set_r41_halt_wlppwd(1);
		}

		if (is_btsp_pd_poweron(v)) {
			aon_pmu_ll_set_r41_halt_blpiso(1);
			aon_pmu_ll_set_r41_halt_blppwd(0);
		}
		else {
			aon_pmu_ll_set_r41_halt_blpiso(1);
			aon_pmu_ll_set_r41_halt_blppwd(1);
		}
#endif
		sys_ll_set_cpu_power_sleep_wakeup_value(v);

		/*config cpu0 subpwdm*/
		sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(1);
		value = sys_ll_get_cpu0_lv_sleep_cfg_value();
		/*bit2(1: fpu powerdown  when lv sleep & cpu0_subpwdm_en==1'b1)*/
		/*bit0(1: cache retension when lv sleep & cpu0_subpwdm_en==1'b1)*/
		// value |= (0x1 << 0);
		sys_ll_set_cpu0_lv_sleep_cfg_value(value);

#if defined(CONFIG_DEEP_LV)
		/* cpu is off during lv-sleep */
		aon_pmu_hal_set_halt_cpu(1);
		/* active when cpu off*/
		aon_pmu_ll_set_r41_halt_anareg(0);//must be 0, if set to 1 will reset ,analog bug
		/* PMU_REG0x40: BIT[29]=0, BIT[30]=1 */
		aon_pmu_ll_set_r40_halt_resten(0);
		aon_pmu_ll_set_r40_halt_isolat(1);
#endif
	}
}

static inline void sys_hal_power_on_pd(volatile uint32_t v_sys_r10)
{
	IF_LV_CTRL_PD() {
		sys_ll_set_cpu_power_sleep_wakeup_value(v_sys_r10);
	}
}

#if !defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
__attribute__((section(".iram"))) void sys_hal_gpio_state_switch(bool lock)
{
	/*pass aon_pmu_r0 to ana*/
	if (lock) {
		aon_pmu_ll_set_r0_gpio_sleep(1);
	} else {
		aon_pmu_ll_set_r0_gpio_sleep(0);
	}
	aon_pmu_ll_set_r25(0x424B55AA);
	aon_pmu_ll_set_r25(0xBDB4AA55);
}
#endif

__attribute__((section(".itcm_sec_code"))) void sys_hal_enter_deep_sleep(void *param)
{
	volatile uint32_t int_state1, int_state2;
	uint32_t systick_ctrl_value = 0;
	pm_lpo_src_e lpo_src        = PM_LPO_SRC_ROSC;
	bk_err_t sleep_allowd       = BK_OK;

	portNVIC_INT_CTRL_REG |= portNVIC_SYSTICKCLR_BIT;
	systick_ctrl_value = portNVIC_SYSTICK_CTRL_REG;
	portNVIC_SYSTICK_CTRL_REG = 0;//disable the systick, avoid it affect the enter deepsleep

	int_state1 = sys_ll_get_cpu0_int_0_31_en_value();
	int_state2 = sys_ll_get_cpu0_int_32_63_en_value();
	sys_ll_set_cpu0_int_0_31_en_value(0x0);
	sys_ll_set_cpu0_int_32_63_en_value(0x0);
	extern bk_err_t ana_rtc_wakeup_config(void *args);
	sleep_allowd = ana_rtc_wakeup_config(NULL);
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );

	/*confirm here hasn't external interrupt*/
	if ((check_IRQ_pending())
	 || (portNVIC_INT_CTRL_REG&portNVIC_SYSTICKSET_BIT)
	 || (sleep_allowd != BK_OK))
	{
		sys_ll_set_cpu0_int_0_31_en_value(int_state1);
		sys_ll_set_cpu0_int_32_63_en_value(int_state2);
		portNVIC_SYSTICK_CTRL_REG = systick_ctrl_value;
		return;
	}

	sys_hal_mask_cpu0_int();

/*----------config wakeup source  start--------------*/
	sys_drv_wakeup_source_gpio_clear();
	sys_drv_wakeup_source_rtc_clear();
#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
	extern bk_err_t gpio_enable_interrupt_mult_for_wake(void);
	gpio_enable_interrupt_mult_for_wake();
#endif
	sys_hal_enable_ana_rtc_int();
	sys_hal_enable_ana_gpio_int();
/*-----------config wakeup source  end --------------*/

	sys_hal_set_core_26m();
	sys_hal_set_flash_26m();

#if defined(CONFIG_INT_WDT)
	bk_wdt_stop();
	#if defined(CONFIG_TASK_WDT)
	bk_task_wdt_stop();
	#endif
#endif

#if defined(CONFIG_AON_PMU_POR_TIMING_SUPPORT)
	// disable it or it will cause error
	sys_ana_ll_set_ana_reg7_vporsel(0);
	sys_ana_ll_set_ana_reg10_vbg_rstrtc_en(1);
	//aon_pmu_ll_set_r0_saved_time(0);
#endif
	/*Enable fast boot*/
	aon_pmu_hal_set_r0_fast_boot(1);

	lpo_src = aon_pmu_ll_get_r41_lpo_config();
	/*set enter deep sleep mode flag*/
	bk_misc_set_reset_reason(RESET_SOURCE_SUPER_DEEP);

	sys_hal_set_sleep_condition();

	sys_hal_set_power_parameter(PM_MODE_DEEP_SLEEP);

	sys_hal_enable_spi_latch();

	sys_hal_disable_hf_clock();
	sys_ana_ll_set_ana_reg5_en_cb(0);

	if((lpo_src == PM_LPO_SRC_ROSC)||(lpo_src == PM_LPO_SRC_DIVD))
	{
		sys_hal_power_on_and_select_rosc(lpo_src);
		// levelshift latch for rc32k (keep current freq)
		sys_ana_ll_set_ana_reg4_spilatchb_rc32k(0);
		// spi_byp32pwd must be set to 1
		sys_ana_ll_set_ana_reg9_spi_byp32pwd(1);
	}

	;//sys_ll_set_ana_reg8_valoldosel(PM_LOW_VOL_AON_LDO_SEL); //0x4:0.8V aon voltage
	sys_hal_disable_spi_latch();

#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	aon_pmu_hal_set_gpio_sleep(1, false);
#else
	sys_hal_gpio_state_switch(true);
#endif

	sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(1);

#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	aon_pmu_hal_r0_latch_to_r7b();
#endif

	/*-----enter deep sleep-------*/
	arch_deep_sleep();
}

static inline void sys_hal_set_low_voltage(volatile uint32_t *ana_r0, volatile uint32_t *ana_r3, volatile uint32_t *ana_r7, volatile uint32_t *ana_r8, volatile uint32_t *ana_r9, volatile uint32_t *ana_r10)
{
	sys_hal_enable_spi_latch();

	*ana_r0 = sys_ana_ll_get_ana_reg0_value();
	*ana_r3 = sys_ana_ll_get_ana_reg3_value();
	*ana_r7 = sys_ana_ll_get_ana_reg7_value();
	// *ana_r8 = sys_ana_ll_get_ana_reg8_value();
	*ana_r9 = sys_ana_ll_get_ana_reg9_value();
	*ana_r10 = sys_ana_ll_get_ana_reg10_value();
#if !defined(CONFIG_DEEP_LV)
	sys_ana_ll_set_ana_reg3_en_xtalh_sleep(1);
#endif
	sys_ana_ll_set_ana_reg7_spi_pwd_regpow(0);
	sys_ana_ll_set_ana_reg7_vbspbuf_lp(1);
	sys_ana_ll_set_ana_reg7_envrefh1v(0);
	sys_ana_ll_set_ana_reg7_vanaldosel(2); // 0: 0.9V, 2: 1V
#if CONFIG_TEMP_DETECT
	float temp;
	bk_err_t err = bk_sensor_get_current_temperature(&temp);

	if((err != BK_OK) || (temp < 0)) // get temperature failed or temperature is less than 0
	{
		#if defined(CONFIG_LDO_SELF_LOW_POWER_MODE_ENA)
		sys_ana_ll_set_ana_reg7_dldohp(1); //will increase current 15uA
		sys_ana_ll_set_ana_reg7_aldohp(0);
		#endif
	}
	else
	{
		#if defined(CONFIG_LDO_SELF_LOW_POWER_MODE_ENA)
		sys_ana_ll_set_ana_reg7_dldohp(0);
		sys_ana_ll_set_ana_reg7_aldohp(0);
		#endif
	}
#else
	#if defined(CONFIG_LDO_SELF_LOW_POWER_MODE_ENA)
	sys_ana_ll_set_ana_reg7_dldohp(0);
	sys_ana_ll_set_ana_reg7_aldohp(0);
	#endif
#endif
	sys_ana_ll_set_ana_reg7_bypassen(1);//bit20
	sys_ana_ll_set_ana_reg7_ioldolp(1);//bit21

	// sys_ana_ll_set_ana_reg9_azcd_manu(0x20);
	// sys_ana_ll_set_ana_reg9_enzcdcalib(0);
	// sys_ana_ll_set_ana_reg9_zcdmsel(1);

	sys_ana_ll_set_ana_reg9_iburstsel(3);//bit[19:18]
	sys_ana_ll_set_ana_reg9_mrosci_cal(0);//bit[27:25]
	sys_ana_ll_set_ana_reg10_avea_sel(3);
	// sys_ana_ll_set_ana_reg10_arampc(0);//suggest by long.teng,20251105

	sys_hal_disable_spi_latch();
}

static inline void sys_hal_restore_voltage(volatile uint32_t ana_r8, volatile uint32_t core_low_voltage)
{
	sys_hal_enable_spi_latch();
	sys_ana_ll_set_ana_reg8_value(ana_r8);
	sys_hal_disable_spi_latch();
}

static void sys_hal_delay(volatile uint32_t times)
{
        while(times--);
}

void sys_hal_exit_low_voltage(void)
{
}

uint64_t sys_hal_get_exit_low_voltage_tick(void)
{
	return low_voltage_exit_tick;
}

void sys_hal_set_exit_low_voltage_tick(uint64_t tick)
{
	low_voltage_exit_tick= tick;

}

uint64_t sys_hal_get_low_voltage_sleep_duration_us(void)
{
	return low_voltage_sleep_duration_us;
}

void sys_hal_set_low_voltage_sleep_duration_us(uint64_t sleep_duration)
{
	low_voltage_sleep_duration_us= sleep_duration;
}

inline uint64_t sys_hal_get_low_voltage_wakeup_time_us(void)
{
	return low_voltage_wakeup_time_us;
}

inline void sys_hal_set_low_voltage_wakeup_time_us(uint64_t wakeup_time)
{
	low_voltage_wakeup_time_us = wakeup_time;
}

static inline void sys_hal_lv_set_buck(void)
{
	// fix it when bring up pm
#if 0
	if (sys_ll_get_ana_reg11_aldosel() == 0) {
		sys_ll_set_ana_reg11_aforcepfm(1);//tenglong20230417 modify buck mode(increase buck effect)
		sys_ll_set_ana_reg12_dforcepfm(1);//tenglong20230417
	}
#endif
}

static inline void sys_hal_lv_restore_buck(void)
{
	// fix it when bring up pm
#if 0
	if (sys_ll_get_ana_reg11_aldosel() == 0) {
		sys_ll_set_ana_reg11_aforcepfm(0);//tenglong20230417
		sys_ll_set_ana_reg12_dforcepfm(0);//tenglong20230417
	}
#endif
}

#if defined(CONFIG_OTA_POSITION_INDEPENDENT_AB)
//when enter lowvoltage need backup related flash information.
static inline void flash_ab_info_backup(flash_ab_reg_t *p_flash_ab_reg)
{
	if(!p_flash_ab_reg)
	{
		return;
	}

	p_flash_ab_reg->offset_addr_begin_val = REG_READ(FLASH_BASE_ADDRESS + FLASH_OFFSET_ADDR_BEGIN*4);
	p_flash_ab_reg->offset_addr_end_val = REG_READ(FLASH_BASE_ADDRESS + FLASH_OFFSET_ADDR_END*4);
	p_flash_ab_reg->flash_addr_offset_val = REG_READ(FLASH_BASE_ADDRESS + FLASH_ADDR_OFFSET*4);
	p_flash_ab_reg->flash_offset_enable_val = (REG_READ(FLASH_BASE_ADDRESS + FLASH_OFFSET_ENABLE*4) & 0x1);
}

//when exit lowvoltage need restore related flash information.
static inline void flash_ab_info_restore(flash_ab_reg_t *p_flash_ab_reg)
{
	if(!p_flash_ab_reg)
	{
		return;
	}

	REG_WRITE((FLASH_BASE_ADDRESS + FLASH_OFFSET_ADDR_BEGIN*4), p_flash_ab_reg->offset_addr_begin_val);
	REG_WRITE((FLASH_BASE_ADDRESS + FLASH_OFFSET_ADDR_END*4), p_flash_ab_reg->offset_addr_end_val);
	REG_WRITE((FLASH_BASE_ADDRESS + FLASH_ADDR_OFFSET*4), p_flash_ab_reg->flash_addr_offset_val);
	REG_WRITE((FLASH_BASE_ADDRESS + FLASH_OFFSET_ENABLE*4), p_flash_ab_reg->flash_offset_enable_val);
}
#endif

#if defined(CONFIG_DEEP_LV)
static uint32_t s_sys_saved_regs[12] = {0};
static uint32_t s_sys_ana_regs[16] = {0};
static uint32_t s_saved_sram[4] = {0};

__attribute__((section(".iram"))) static void _deep_lv_enter_(void)
{
	__asm volatile
	(
		" .syntax unified           \n"
		" cpsie i                   \n" /* Globally enable interrupts. */
		" cpsie f                   \n"
		" dsb                       \n"
		" isb                       \n"
		" svc %0                    \n"
		" nop                       \n"
		"                           \n"
		::"i"(portSVC_DEEP_LV_ENTER):"memory"
	);
}

__attribute__((section(".iram"))) void sys_hal_deep_lv_enter(void)
{
	_deep_lv_enter_();
	__asm volatile
	(
		" .syntax unified           \n"
		" nop                       \n"
	);
}

__attribute__((section(".iram"))) void sys_hal_regs_save(void)
{
	s_sys_saved_regs[0] = sys_ll_get_cpu_clk_div_mode1_value(); // reg_0x8
	s_sys_saved_regs[1] = sys_ll_get_cpu_clk_div_mode2_value(); // reg_0x9
	s_sys_saved_regs[2] = sys_ll_get_cpu_26m_wdt_clk_div_value(); // reg_0xa
	s_sys_saved_regs[3] = sys_ll_get_cpu_anaspi_freq_value(); // reg_0xb
	s_sys_saved_regs[4] = sys_ll_get_cpu_device_clk_enable_value(); // reg_0xc
	s_sys_saved_regs[5] = sys_ll_get_cpu_device_mem_ctrl2_value(); // reg_0xf
	s_sys_saved_regs[6] = sys_ll_get_cpu_power_sleep_wakeup_value(); // reg_0x10
	s_sys_saved_regs[7] = sys_ll_get_cpu0_lv_sleep_cfg_value(); // reg_0x11
	s_sys_saved_regs[8] = sys_ll_get_cpu_device_mem_ctrl3_value(); // reg_0x12
	s_sys_saved_regs[9] = sys_ll_get_cpu0_int_0_31_en_value(); // reg_0x20
	s_sys_saved_regs[10] = sys_ll_get_cpu0_int_32_63_en_value(); // reg_0x21

	for (uint32_t i = 0; i < 16; i++) {
		s_sys_ana_regs[i] = sys_hal_analog_get(ANALOG_REG0 + i);
	}

	// for (uint32_t i = 0; i < 4; i++) {
	// 	s_saved_sram[i] = REG_READ(0x2801FFF0 + (i << 2));
	// }
}

__attribute__((section(".iram")))  void sys_hal_regs_restore(void)
{
	s_sys_saved_regs[0] |= (0x1 << 4);
	s_sys_saved_regs[0] |= (0x3 << 0);
	sys_ll_set_cpu_clk_div_mode1_value(s_sys_saved_regs[0]); // reg_0x8
//	sys_ll_set_cpu_clk_div_mode2_value(s_sys_saved_regs[1]); // reg_0x9
	sys_ll_set_cpu_26m_wdt_clk_div_value(s_sys_saved_regs[2]); // reg_0xa
	sys_ll_set_cpu_anaspi_freq_value(s_sys_saved_regs[3]); // reg_0xb
	sys_ll_set_cpu_device_clk_enable_value(s_sys_saved_regs[4]); // reg_0xc
	sys_ll_set_cpu_device_mem_ctrl2_value(s_sys_saved_regs[5]); // reg_0xf
	sys_ll_set_cpu_power_sleep_wakeup_value(s_sys_saved_regs[6]); // reg_0x10
	sys_ll_set_cpu0_lv_sleep_cfg_value(s_sys_saved_regs[7]); // reg_0x11
	sys_ll_set_cpu_device_mem_ctrl3_value(s_sys_saved_regs[8]); // reg_0x12
	sys_ll_set_cpu0_int_0_31_en_value(s_sys_saved_regs[9]); // reg_0x20
	sys_ll_set_cpu0_int_32_63_en_value(s_sys_saved_regs[10]); // reg_0x21

	sys_ana_ll_set_ana_reg8_spi_latch1v(1);
	/* restore analog regs */
	for (uint32_t i = 0; i < 9; i++) {
		if(( i == 0)||( i == 5)||( i == 7)||( i == 8))
			continue;
		sys_hal_analog_set_iram(ANALOG_REG0 + i, s_sys_ana_regs[i]);
	}
	/**
	 * attention:
	 * SPI latch must be enable before ana_reg[8~13] modification
	 * and don't forget disable it after that.
	 */
	for (uint32_t i = 9; i < 16; i++) {
		sys_hal_analog_set_iram(ANALOG_REG0 + i, s_sys_ana_regs[i]);
	}
	sys_ana_ll_set_ana_reg8_spi_latch1v(0);

	for (uint32_t i = 0; i < 4; i++) {
 		REG_WRITE(0x2801FFF0 + (i << 2), s_saved_sram[i]);
	}
}
#endif

__attribute__((section(".iram"))) void sys_hal_enable_ana_rtc_int(void)
{
	sys_hal_int_group2_enable(ANA_RTC_INTERRUPT_CTRL_BIT);

	sys_hal_enable_spi_latch();
	#if defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
	sys_hal_set_ana_reg8_rtcwk_rstn(1);
	#else
	sys_hal_set_ana_reg8_rtcwk_rstn(1);
	sys_hal_set_ana_reg7_timer_wkrstn(1);
	#endif
	sys_hal_disable_spi_latch();
}

__attribute__((section(".iram")))  void sys_hal_disable_ana_rtc_int(void)
{
	sys_hal_int_group2_disable(ANA_RTC_INTERRUPT_CTRL_BIT);

	sys_hal_enable_spi_latch();
	#if defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
	sys_hal_set_ana_reg8_rtcwk_rstn(0);
	#else
	sys_hal_set_ana_reg7_timer_wkrstn(0);
	sys_hal_set_ana_reg8_rtcwk_rstn(0);
	#endif
	/*rest timer source flag*/
	sys_ana_ll_set_ana_reg8_rst_timerwks1v(1);
	sys_ana_ll_set_ana_reg8_rst_timerwks1v(0);
	sys_hal_disable_spi_latch();
}

__attribute__((section(".iram"))) void sys_hal_enable_ana_gpio_int(void)
{
	sys_hal_int_group2_enable(ANA_GPIO_INTERRUPT_CTRL_BIT);

	sys_hal_enable_spi_latch();
	sys_hal_set_ana_reg8_gpiowk_rstn(1);
	sys_hal_disable_spi_latch();
}

__attribute__((section(".iram"))) void sys_hal_disable_ana_gpio_int(void)
{
	sys_hal_int_group2_disable(ANA_GPIO_INTERRUPT_CTRL_BIT);

	sys_hal_enable_spi_latch();
	sys_hal_set_ana_reg8_gpiowk_rstn(0);
	/*rest gpio source flag*/
	sys_ana_ll_set_ana_reg8_rst_gpiowks(1);
	sys_ana_ll_set_ana_reg8_rst_gpiowks(0);
	sys_hal_disable_spi_latch();
}

__attribute__((section(".iram"))) void sys_drv_wakeup_source_gpio_clear(void)
{
	//reset source flag
	sys_ana_ll_set_ana_reg8_spi_latch1v(1);
	sys_ana_ll_set_ana_reg8_rst_gpiowks(1);
	sys_ana_ll_set_ana_reg8_rst_gpiowks(0);
	sys_ana_ll_set_ana_reg8_spi_latch1v(0);
}

__attribute__((section(".iram"))) void sys_drv_wakeup_source_rtc_clear(void)
{
	//reset source flag
	sys_ana_ll_set_ana_reg8_spi_latch1v(1);
	sys_ana_ll_set_ana_reg8_rst_timerwks1v(1);
	sys_ana_ll_set_ana_reg8_rst_timerwks1v(0);
	sys_ana_ll_set_ana_reg8_spi_latch1v(0);
}
__attribute__((section(".iram"))) void sys_hal_enter_low_voltage(void)
{
	//TODO: fix it when bring up pm
#if 1
	volatile uint32_t int_state1, int_state2;
	volatile uint8_t cksel_core = 0, clkdiv_core = 0, clkdiv_bus = 0;
	volatile uint8_t cksel_flash = 0, clkdiv_flash = 0;
	volatile uint32_t v_ana_r0,v_ana_r3, v_ana_r7, v_ana_r8,v_ana_r9, v_ana_r10;
	volatile uint32_t v_sys_r10    = 0;
	volatile uint32_t pmu_val               = 0;
	volatile uint32_t systick_ctrl_value    = 0;
	volatile uint32_t hf_reg_v = 0;
	volatile uint32_t systick_load_value    = 0;
	bk_err_t sleep_allowd       = BK_OK;

#if defined(CONFIG_OTA_POSITION_INDEPENDENT_AB)
	flash_ab_reg_t  ab_flash_reg   = {0};
#endif
	portNVIC_INT_CTRL_REG |= portNVIC_SYSTICKCLR_BIT;
	systick_ctrl_value = portNVIC_SYSTICK_CTRL_REG;
	systick_load_value = portNVIC_SYSTICK_LOAD_REG;
	portNVIC_SYSTICK_LOAD_REG = PM_EXIT_LOWVOL_SYSTICK_RELOAD_TIME;
	portNVIC_SYSTICK_CTRL_REG = 0;//disable the systick, avoid it affect the enter low voltage sleep

	int_state1 = sys_ll_get_cpu0_int_0_31_en_value();
	int_state2 = sys_ll_get_cpu0_int_32_63_en_value();
	sys_ll_set_cpu0_int_0_31_en_value(0x0);
	sys_ll_set_cpu0_int_32_63_en_value(0x0);
	extern bk_err_t ana_rtc_wakeup_config(void *args);
	sleep_allowd = ana_rtc_wakeup_config(NULL);

	if ((check_IRQ_pending())
	 || (sys_ll_get_cpu0_int_0_31_status_value() || sys_ll_get_cpu0_int_32_63_status_value())
	 || (portNVIC_INT_CTRL_REG&portNVIC_SYSTICKSET_BIT)
	 || (sleep_allowd != BK_OK))
	{
		sys_ll_set_cpu0_int_0_31_en_value(int_state1);
		sys_ll_set_cpu0_int_32_63_en_value(int_state2);
		portNVIC_SYSTICK_CTRL_REG = systick_ctrl_value;
		return;
	}

	portNVIC_SYSTICK_LOAD_REG = PM_EXIT_LOWVOL_SYSTICK_RELOAD_TIME;

	sys_hal_mask_cpu0_int();
	sys_drv_wakeup_source_gpio_clear();
	sys_drv_wakeup_source_rtc_clear();
/*----------config wakeup source  start--------------*/
#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
	extern bk_err_t gpio_enable_interrupt_mult_for_wake(void);
	gpio_enable_interrupt_mult_for_wake();
#endif
	sys_hal_enable_ana_rtc_int();
	sys_hal_enable_ana_gpio_int();
/*-----------config wakeup source  end --------------*/

	pmu_val =  aon_pmu_ll_get_r2();
	pmu_val |= BIT(BIT_SLEEP_FLAG_LOW_VOLTAGE);
	aon_pmu_ll_set_r2(pmu_val);

	bk_pm_module_lv_sleep_state_set();

	sys_hal_backup_set_core_26m(&cksel_core, &clkdiv_core, &clkdiv_bus);
	sys_hal_backup_set_flash_26m(&cksel_flash, &clkdiv_flash);

#if defined(CONFIG_INT_WDT)
	bk_wdt_stop();
	#if defined(CONFIG_TASK_WDT)
	bk_task_wdt_stop();
	#endif
#endif

#if defined(CONFIG_OTA_POSITION_INDEPENDENT_AB)
	;//flash_ab_info_backup(&ab_flash_reg);
#endif
#if 0

	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(0x1);
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(0x1);
#else
	//enable gpio ns interrupt
	sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(0x1);
#endif
	sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(0x1);
	sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(0x1);
#endif
	hf_reg_v = sys_hal_disable_hf_clock();

#if 0
	psram_state = sys_ll_get_ana_reg13_enpsram();
	chip_id = aon_pmu_hal_get_chipid();
	if(psram_state != 0x0)//when psram ldo enable(0x1:psram ldo enable)
	{
		if ((chip_id & PM_CHIP_ID_MASK) != (PM_CHIP_ID_MP_A & PM_CHIP_ID_MASK))
		{
			sys_ll_set_ana_reg10_vbspbuflp1v(0x0);
		}
	}
	else//when psram ldo disable
	{
		if ((chip_id & PM_CHIP_ID_MASK) != (PM_CHIP_ID_MP_A & PM_CHIP_ID_MASK))
		{
			sys_ll_set_ana_reg10_vbspbuflp1v(0x1);
		}
	}

	/*low voltage power optimization*/
	if(sys_ll_get_ana_reg11_enpowa() != 0x1)
	{
		sys_ll_set_ana_reg11_enpowa(0x1);
	}

	#if defined(CONFIG_PSRAM_POWER_DOMAIN_LV_DISABLE)
	if(psram_state != 0x0)
	{
		sys_ll_set_ana_reg13_enpsram(0x0);//the psram power domain need the app enable again
	}
	#endif

	if(sys_ll_get_ana_reg12_denburst() != 0x1)
	{
		sys_ll_set_ana_reg12_denburst(0x1);//buckD burst enable
	}

	if(sys_ll_get_ana_reg11_aenburst() != 0x1)
	{
		sys_ll_set_ana_reg11_aenburst(0x1);//buckA burst enable
	}
#endif
	sys_ana_ll_set_ana_reg5_en_cb(0);

	sys_hal_set_power_parameter(PM_MODE_LOW_VOLTAGE);
	sys_hal_set_sleep_condition();
	sys_hal_power_down_pd(&v_sys_r10);
	v_ana_r8 = sys_ana_ll_get_ana_reg8_value();
	/*step 1: set dig voltage to 0.7V*/
#if defined(CONFIG_SOFT_CTRL_VOLT)
	uint32_t vcorehsel             = 0;
	uint8_t  ustep                 = 0;
	sys_hal_enable_spi_latch();
	vcorehsel = sys_ana_ll_get_ana_reg8_vcorehsel();
	sys_ana_ll_set_ana_reg8_vcorehsel(PM_LOW_VOL_VCOREHSEL_LDO_SEL); //0x0:0.7V vcore
	sys_hal_disable_spi_latch();
#endif
	/*step 2: set ana voltage to 0.9V */
	sys_hal_set_low_voltage(&v_ana_r0, &v_ana_r3, &v_ana_r7, &v_ana_r8, &v_ana_r9, &v_ana_r10);

#if defined(CONFIG_LV_FLASH_ENTER_LP_ENABLE)
	bk_flash_enter_deep_sleep();
#endif

	volatile uint64_t before = bk_aon_rtc_get_us();
#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	aon_pmu_hal_set_gpio_sleep(1, true);
#else
	sys_hal_gpio_state_switch(true);
#endif

	bk_pm_sleep_wakeup_reason_clear();

/*----enter low voltage sleep-------*/
#if defined(CONFIG_DEEP_LV)
	/*Enable fast boot*/
	aon_pmu_hal_set_r0_fast_boot(1);
	sys_hal_regs_save();
	aon_pmu_hal_backup();
	//sys_hal_deep_lv_enter();
	__NOP();
	dlv_stack_frame_save_and_dlv(__get_LR());
	deep_lv_enter();
	if (dlv_is_startup_iram()) {
		arch_deep_sleep();
	}
#else
	arch_deep_sleep();
	DELAY_US(40); //must delay 40us to sampling rtc value
#endif
	bk_pm_sleep_wakeup_reason_set(check_IRQ_pending());

#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	aon_pmu_hal_set_gpio_sleep(0, true);
#else
	sys_hal_gpio_state_switch(false);
#endif

	uint64_t current = bk_aon_rtc_get_us();
	sys_hal_set_low_voltage_wakeup_time_us(current);
	sys_hal_set_low_voltage_sleep_duration_us(current - before);
#if defined(CONFIG_OTA_POSITION_INDEPENDENT_AB)
	;//flash_ab_info_restore(&ab_flash_reg);
#endif
	#if defined(CONFIG_LV_FLASH_ENTER_LP_ENABLE)
	bk_flash_exit_deep_sleep();
	#endif
/*--------------------wake up---------------------*/
#if defined(CONFIG_DEEP_LV)
	sys_hal_regs_restore();
	aon_pmu_hal_restore();
#endif

/*-----------restore voltage  start----------------*/
	sys_ana_ll_set_ana_reg8_spi_latch1v(1);

	sys_ana_ll_set_ana_reg0_value(v_ana_r0);
	sys_ana_ll_set_ana_reg3_value(v_ana_r3);
	/*step 1: restore ana voltage to 1.1V*/
	sys_ana_ll_set_ana_reg7_value(v_ana_r7);
	/*step 2: restore dig voltage to 0.875V by step */
#if defined(CONFIG_SOFT_CTRL_VOLT)
	/*vdddig voltage*/
	for(ustep = PM_LOW_VOL_VCOREHSEL_LDO_SEL+1; ustep <= vcorehsel; ustep++)
	{
		sys_ana_ll_set_ana_reg8_vcorehsel(ustep); //restore vdddig
	}
#endif
	sys_ana_ll_set_ana_reg8_value(v_ana_r8);
	sys_ana_ll_set_ana_reg9_value(v_ana_r9);
	sys_ana_ll_set_ana_reg10_value(v_ana_r10);

/*-------------restore voltage  end-----------------*/


/*----------restore analog clock  start--------------*/
	sys_ana_ll_set_ana_reg5_en_cb(1);

	sys_hal_restore_hf_clock(hf_reg_v);

/*-----------restore analog clock  end --------------*/

	sys_hal_power_on_pd(v_sys_r10);

	volatile uint64_t previous_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	sys_hal_set_exit_low_voltage_tick(previous_tick);

	//Use a function instead of delay
	void pm_low_voltage_bsp_restore(void);
	pm_low_voltage_bsp_restore();

/*----------reset wakeup source  start--------------*/
	#if defined(CONFIG_ANA_RTC_WAKUP_BY_RTC)
	sys_ana_ll_set_ana_reg8_rtcwk_rstn(0);
	#else
	sys_ana_ll_set_ana_reg7_timer_wkrstn(0);
	#endif
	sys_ana_ll_set_ana_reg8_gpiowk_rstn(0);
	sys_ana_ll_set_ana_reg8_lvsleep_wkrstn(1);
	sys_ana_ll_set_ana_reg8_spi_latch1v(0);
/*-----------reset wakeup source  end --------------*/

#if !defined(CONFIG_DEEP_LV)
/*---------------at least delay 190us-----------------*/
	volatile uint64_t current_tick  = 0;
	current_tick = previous_tick;
	while(((current_tick - previous_tick)) < (LOW_POWER_DPLL_STABILITY_DELAY_TIME*AON_RTC_MS_TICK_CNT))
	{
		current_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);
	}
#endif
/*---------------at least delay 190us end -----------------*/

	sys_hal_restore_core_freq(cksel_core, clkdiv_core, clkdiv_bus);

	sys_hal_restore_flash_freq(cksel_flash, clkdiv_flash);

	sys_hal_restore_int(int_state1, int_state2);

	portNVIC_SYSTICK_LOAD_REG = systick_load_value;

	portNVIC_SYSTICK_CTRL_REG = systick_ctrl_value;
#endif
}

void sys_hal_touch_wakeup_enable(uint8_t index)
{
}

void sys_hal_usb_wakeup_enable(uint8_t index)
{
}

void sys_hal_rtc_wakeup_enable(uint32_t value)
{
}
static bk_err_t sys_hal_normal_sleep_ultra_low_power_exit(pm_normal_sleep_mode_e normal_sleep,pm_cpu_freq_e cpu_freq)
{
	uint32_t value = 0;
	/*latch*/
	value = sys_ana_ll_get_reg8_value();
	value |= (0x1 << 9);
	sys_ana_ll_set_reg8_value(value);

	/*xtal sleep disable*/
	value = sys_ana_ll_get_reg3_value();
	if((value&(0x1 << 25)) == (0x1 << 25))
	{
		value &= ~(0x1 << 25);
	}
	sys_ana_ll_set_reg3_value(value);

	/*DPLL enable*/
	value = sys_ana_ll_get_reg5_value();
	if((value&(0x1 << 5)) != (0x1 << 5))
	{
		value |= (0x1 << 5);
	}
	sys_ana_ll_set_reg5_value(value);
	/*delay for dpll stability*/
	DELAY_US(200);

	/*vcore 0.875v*/
	value = sys_ana_ll_get_reg8_value();
	value &= ~(0xF << 16);
	value |= (0xB << 16);
	sys_ana_ll_set_reg8_value(value);
	/*delay for vddcore stability*/
	DELAY_US(100);

	/* cpu freq config*/
	sys_hal_switch_cpu_bus_freq(cpu_freq);

	/*Flash use 80M */
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(0);
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(2);
	DELAY_US(100);

	/*pwd_wifp_mac,rom_pgen enable*/
	sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(0);
	sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(0);
	return BK_OK;
}
static bk_err_t sys_hal_normal_sleep_ultra_low_power_enter(pm_normal_sleep_mode_e normal_sleep)
{
	uint32_t value = 0;
	/* cpu freq use 26M*/
	sys_ll_set_cpu_clk_div_mode1_cksel_core(0);
	/*Flash use 26M */
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(0);

	/*pwd_wifp_mac,rom_pgen*/
	sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(1);
	sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(1);

	/*latch*/
	value = sys_ana_ll_get_reg8_value();
	value |= (0x1 << 9);
	sys_ana_ll_set_reg8_value(value);

	/*vcore 0.75v*/
	value = sys_ana_ll_get_reg8_value();
	value &= ~(0xF << 16);
	value |= (0x6 << 16);
	sys_ana_ll_set_reg8_value(value);
	DELAY_US(100);

	/*DPLL disable*/
	value = sys_ana_ll_get_reg5_value();
	if((value&(0x1 << 5)) == (0x1 << 5))
	{
		value &= ~(0x1 << 5);
	}
	sys_ana_ll_set_reg5_value(value);
	DELAY_US(100);
	/*xtal sleep*/
	value = sys_ana_ll_get_reg3_value();
	if((value&(0x1 << 25)) != (0x1 << 25))
	{
		value |= (0x1 << 25);
	}
	sys_ana_ll_set_reg3_value(value);

	return BK_OK;
}
static bk_err_t sys_hal_normal_sleep_mode_1_enter(pm_normal_sleep_mode_e normal_sleep,pm_cpu_freq_e cpu_freq)
{
	uint32_t value = 0;
	/*latch*/
	value = sys_ana_ll_get_reg8_value();
	value |= (0x1 << 9);
	sys_ana_ll_set_reg8_value(value);

	/*xtal sleep disable*/
	value = sys_ana_ll_get_reg3_value();

	if((value&(0x1 << 25)) == (0x1 << 25))
	{
		value &= ~(0x1 << 25);
	}
	sys_ana_ll_set_reg3_value(value);

	/*DPLL enable*/
	value = sys_ana_ll_get_reg5_value();
	if((value&(0x1 << 5)) != (0x1 << 5))
	{
		value |= (0x1 << 5);
	}
	sys_ana_ll_set_reg5_value(value);
	/*delay for dpll stability*/
	DELAY_US(200);

	/* cpu freq config*/
	sys_hal_switch_cpu_bus_freq(cpu_freq);

	/*pwd_wifp_mac,rom_pgen*/
	sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(1);
	sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(1);

	/*Flash use 80M */
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(2);
	DELAY_US(100);

	return BK_OK;
}
bk_err_t sys_hal_normal_sleep_mode_ctrl(pm_normal_sleep_mode_e normal_sleep,pm_cpu_freq_e cpu_freq)
{
	switch(normal_sleep)
	{
		case PM_NORMAL_SLEEP_ULTRA_EXIT:
		{
			sys_hal_normal_sleep_ultra_low_power_exit(normal_sleep,cpu_freq);
		}
		break;
		case PM_NORMAL_SLEEP_ULTRA_ENTER:
		{
			sys_hal_normal_sleep_ultra_low_power_enter(normal_sleep);
		}
		break;
		case PM_NORMAL_SLEEP_MODE_1:
		{
			sys_hal_normal_sleep_mode_1_enter(normal_sleep,cpu_freq);
		}
		break;
		default:
		break;
	}
	return BK_OK;
}

void sys_hal_enter_cpu_wfi()
{
	// TODO - find out the reason:
	// When sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask() is called, the normal sleep can't wakedup,
	// need to find out!!!
	// sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(1);
	arch_sleep();
}
__attribute__((section(".iram"))) uint64_t pm_get_current_tick()
{
	uint32_t low_tick   = aon_pmu_hal_rtc_tick_l_get();
	uint32_t high_tick = aon_pmu_hal_rtc_tick_h_get()&0xF;
	uint64_t tick_val = ((uint64_t)high_tick << 32) + low_tick;
	return tick_val;
}

__attribute__((section(".iram"))) void pm_delay_us(volatile uint32_t us)
{
	uint64_t start = pm_get_current_tick();
	uint64_t delta;

	uint64_t thresh = (uint64_t)us * (uint64_t)PM_DELAY_TICKS_PER_MS;
	do {
		delta = pm_get_current_tick() - start;
	} while (delta * 1000ULL < thresh);
}

__attribute__((section(".iram"))) void sys_hal_enter_normal_sleep(uint32_t peri_clk)
{
	volatile uint32_t clken_peri = 0;
	volatile uint8_t cksel_core = 0, clkdiv_core = 0;
	volatile uint8_t cksel_flash = 0, clkdiv_flash = 0;
	volatile uint8_t vcoresel = 0;
	volatile bool en_dpll_dig = false;
	volatile bool en_dpll_ana = false;
	volatile bool xtal_sleep = false;
	volatile uint32_t systick_ctrl_value    = 0;
	volatile uint32_t systick_load_value    = 0;
	volatile uint32_t systick_val_before    = 0;
	volatile uint32_t pwd_rom_value         = 0;

	uint32_t systick_ticks_per_ms = 0;
	uint32_t adjusted_val = 0;

	clken_peri = sys_ll_get_cpu_device_clk_enable_value();
	sys_ll_set_cpu_device_clk_enable_mac_cken(0);
	sys_ll_set_cpu_device_clk_enable_thread_cken(0);
	sys_ll_set_cpu_device_clk_enable_phy_cken(0);
	sys_ll_set_cpu_device_clk_enable_rf_cken(0);
	/*Support BT,it can disable btdm,xvr clock*/
	//sys_ll_set_cpu_device_clk_enable_xvr_cken(0);
	//sys_ll_set_cpu_device_clk_enable_btdm_cken(0);


	/* Save SysTick state before entering sleep */
	portNVIC_INT_CTRL_REG |= portNVIC_SYSTICKCLR_BIT;
	systick_ctrl_value = portNVIC_SYSTICK_CTRL_REG;
	systick_load_value = portNVIC_SYSTICK_LOAD_REG;
	systick_val_before = portNVIC_SYSTICK_CURRENT_VALUE_REG;
	portNVIC_SYSTICK_CTRL_REG = 0;//disable the systick, avoid it affect the enter low voltage sleep

	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );
	__asm volatile( "nop" );

	/*confirm here hasn't external interrupt*/
	if (check_IRQ_pending()||(portNVIC_INT_CTRL_REG&portNVIC_SYSTICKSET_BIT))
	{
		sys_ll_set_cpu_device_clk_enable_value(clken_peri);

		portNVIC_SYSTICK_CTRL_REG = systick_ctrl_value;
		portNVIC_SYSTICK_LOAD_REG = systick_load_value & 0x00FFFFFF;

		return;
	}
	/* flush all dcache(data in sram and psram)*/
	flush_all_dcache();
	__asm volatile( "dsb" );
	__asm volatile( "isb" );


	/* cpu freq use 10M*/
	cksel_core = sys_ll_get_cpu_clk_div_mode1_cksel_core();
	clkdiv_core = sys_ll_get_cpu_clk_div_mode1_clkdiv_core();
	sys_ll_set_cpu_clk_div_mode1_cksel_core(0);
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(3);

	/*Flash use 26M */
	cksel_flash = sys_ll_get_cpu_clk_div_mode2_cksel_flash();
	clkdiv_flash = sys_ll_get_cpu_clk_div_mode2_ckdiv_flash();
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(0);
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(0);

	sys_ll_set_cpu_device_clk_enable_mac_cken(0);
	sys_ll_set_cpu_device_clk_enable_thread_cken(0);
	sys_ll_set_cpu_device_clk_enable_phy_cken(0);
	sys_ll_set_cpu_device_clk_enable_rf_cken(0);
	sys_ll_set_cpu_device_clk_enable_psram_cken(0);
	sys_ll_set_cpu_device_clk_enable_otp_cken(0);
	//sys_ll_set_cpu_device_clk_enable_xvr_cken(0);
	//sys_ll_set_cpu_device_clk_enable_btdm_cken(0);

	pwd_rom_value = sys_ll_get_cpu_power_sleep_wakeup_rom_pgen();
	sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(1);
	/* cpu halt*/
	sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(1);

	bk_pm_sleep_wakeup_reason_clear();

	pm_delay_us(30);
	/*DPLL_DIV disable*/
	en_dpll_dig = sys_ll_get_cpu_device_clk_enable_dplldiv_cken();
	sys_ll_set_cpu_device_clk_enable_dplldiv_cken(0);
	pm_delay_us(30);
	/*latch start*/

	sys_ana_ll_set_ana_reg8_spi_latch1v_iram(1);

	/*vcore 0.7v*/
	// Direct register access to avoid veneer
	vcoresel = (*(volatile uint32_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2)) >> 16) & 0xf;

	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 16, 0xf, 0x4);//0.7v
	pm_delay_us(30);

	/*DPLL disable*/
	// Direct register access to avoid veneer
	en_dpll_ana = (*(volatile uint32_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2)) >> 5) & 0x1;

	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 5, 0x1, 0);
	pm_delay_us(50);

	/*xtal sleep*/
	// Direct register access to avoid veneer
	xtal_sleep = (*(volatile uint32_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2)) >> 25) & 0x1;

	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 25, 0x1, 1);

	arch_deep_sleep();

	bk_pm_sleep_wakeup_reason_set(check_IRQ_pending());

	/*xtal sleep disable*/

	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 25, 0x1, xtal_sleep);

	pm_delay_us(50);

	/*restore vcore*/
	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 16, 0xf, vcoresel);
	/*delay for vddcore stability*/
	pm_delay_us(30);

	/*DPLL enable*/
	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 5, 0x1, en_dpll_ana);
	/*delay for dpll stability*/
	pm_delay_us(200);
	/*latch end*/
	sys_ana_ll_set_ana_reg8_spi_latch1v_iram(0);

	/*DPLL_DIV enable*/
	sys_ll_set_cpu_device_clk_enable_dplldiv_cken(en_dpll_dig);
	pm_delay_us(30);
	/*cpu freq restore*/
	sys_ll_set_cpu_clk_div_mode1_cksel_core(cksel_core);
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(clkdiv_core);
	pm_delay_us(30);
	/*Flash freq restore */
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(clkdiv_flash);
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(cksel_flash);

	/*delay for flash stability*/
	pm_delay_us(30);

	sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(pwd_rom_value);
	sys_ll_set_cpu_device_clk_enable_value(clken_peri);

	portNVIC_SYSTICK_LOAD_REG = systick_load_value;

	portNVIC_SYSTICK_CTRL_REG = systick_ctrl_value;
	portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT | portNVIC_SYSTICK_INT_BIT;
}

void sys_hal_enter_normal_wakeup()
{
}

void sys_hal_enable_mac_wakeup_source()
{
}

void sys_hal_enable_bt_wakeup_source()
{
}

void sys_hal_wakeup_interrupt_clear(wakeup_source_t interrupt_source)
{
}

int sys_hal_set_lpo_src(sys_lpo_src_t src)
{
	//TODO
	return BK_OK;
}

void sys_hal_enter_low_analog(void)
{
	sys_ana_ll_set_ana_reg8_spi_latch1v(1);
	#if CONFIG_TEMP_DETECT
	float temp;
	bk_err_t err = bk_sensor_get_current_temperature(&temp);
	if((err != BK_OK) || (temp < -10))
	{
		sys_ana_ll_set_ana_reg7_vanaldosel(0xa);
	}
	else if (temp < 20)
	{
		sys_ana_ll_set_ana_reg7_vanaldosel(0x7);
	}
	else if (temp < 60)
	{
		sys_ana_ll_set_ana_reg7_vanaldosel(0x7);
	}
	else
	{
		sys_ana_ll_set_ana_reg7_vanaldosel(0xa);
	}
	#else
	sys_ana_ll_set_ana_reg7_vanaldosel(0xa);
	#endif
	sys_ana_ll_set_ana_reg8_spi_latch1v(0);

	sys_ana_ll_set_ana_reg3_hpssren(0);
	/* tx low performence 0x45[11][9] = 1*/
	sys_ana_ll_set_ana_reg5_anabufsel_tx1(0);
	sys_ana_ll_set_ana_reg5_anabufsel_tx0(1);
	/* rx low performence 0x45[10][8] = 1*/
	sys_ana_ll_set_ana_reg5_anabufsel_rx1(0);
	sys_ana_ll_set_ana_reg5_anabufsel_rx0(1);


}

void sys_hal_exit_low_analog(void)
{
	sys_ana_ll_set_ana_reg8_spi_latch1v(1);
	sys_ana_ll_set_ana_reg7_vanaldosel(0xf);
	sys_ana_ll_set_ana_reg8_spi_latch1v(0);

	sys_ana_ll_set_ana_reg3_hpssren(1);
	/* tx high performence 0x45[11][9] = 3*/
	sys_ana_ll_set_ana_reg5_anabufsel_tx1(1);
	sys_ana_ll_set_ana_reg5_anabufsel_tx0(1);
	/* rx high performence 0x45[10][8] = 3*/
	sys_ana_ll_set_ana_reg5_anabufsel_rx1(1);
	sys_ana_ll_set_ana_reg5_anabufsel_rx0(1);

}

/**
 * set io ldo power mode
 *
 * uint32_t type input:
 *   0: high power mode
 *   1: low power mode
 *   other: undefine
*/
void sys_hal_set_ioldo_lp(uint32_t val)
{
	//TODO: fix it when bring up pm
}

void sys_hal_dco_switch_freq(dco_cali_speed_e speed)
{
	//TODO: fix it when bring up pm
#if 0
	//dco: default calibration is 240M
	switch (speed)
	{
		case DCO_CALIB_SPEED_480M:
		case DCO_CALIB_SPEED_320M:
			break;
		case DCO_CALIB_SPEED_240M:
			sys_ll_set_cpu_clk_div_mode1_cksel_core(0x1);// cpu clock source select dco
			break;
		case DCO_CALIB_SPEED_120M:
			sys_ll_set_ana_reg1_divctrl(0x1);
			sys_ll_set_cpu_clk_div_mode1_cksel_core(0x1);
			break;
		case DCO_CALIB_SPEED_80M:
			sys_ll_set_ana_reg1_divctrl(0x2);
			sys_ll_set_cpu_clk_div_mode1_cksel_core(0x1);
			;// cpu use dco
			break;
		case DCO_CALIB_SPEED_60M:
			sys_ll_set_ana_reg1_divctrl(0x3);
			sys_ll_set_cpu_clk_div_mode1_cksel_core(0x1);
			break;
		default:
			break;
	}
#endif
}

static int sys_hal_config_32k_source_default(void)
{
	pm_lpo_src_e lpo_src = PM_LPO_SRC_ROSC;

	lpo_src = bk_clk_32k_customer_config_get();
	if(lpo_src == PM_LPO_SRC_X32K)
	{
		sys_ana_ll_set_ana_reg5_en_xtall(0x1);
		if(sys_ana_ll_get_ana_reg5_itune_xtall() != 0xF)
		{
			sys_ana_ll_set_ana_reg5_itune_xtall(0xF);
		}

		sys_ana_ll_set_ana_reg5_itune_xtall(0xA);//0x0 provide highest current for external 32k,because the signal path long
		sys_ana_ll_set_ana_reg5_itune_xtall(0x4);

		aon_pmu_hal_lpo_src_set(PM_LPO_SRC_X32K);

		sys_ana_ll_set_ana_reg9_ckintsel(1);//select buck clock source(0x1: extern 32k)
	}
	else if(lpo_src == PM_LPO_SRC_DIVD)
	{
		aon_pmu_hal_lpo_src_set(PM_LPO_SRC_DIVD);
	}
	else
	{
		aon_pmu_hal_lpo_src_set(PM_LPO_SRC_ROSC);
	}

	return 0;
}

static int sys_hal_enable_buck(void)
{
	volatile uint8_t cksel_core = 0, clkdiv_core = 0, clkdiv_bus = 0;
	sys_hal_backup_set_core_26m(&cksel_core, &clkdiv_core, &clkdiv_bus);//let the cpu frequency to 26m, in order to be successfully switch voltage provide from ldo to buck

	sys_hal_enable_spi_latch();
	sys_ana_ll_set_ana_reg10_aldosel(0);
	DELAY_US(1);
	sys_hal_disable_spi_latch();

	sys_hal_restore_core_freq(cksel_core, clkdiv_core, clkdiv_bus);
	return 0;
}
static int sys_hal_power_config_default(void)
{
	//TODO: fix it when bring up pm
	/*config the power domain*/
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_BTSP,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_WIFIP_MAC,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_WIFI_PHY,POWER_MODULE_STATE_OFF);

#if 0
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_ENCP,POWER_MODULE_STATE_OFF);
#endif
	//sys_hal_module_power_ctrl(POWER_MODULE_NAME_BAKP,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_AUDP,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_BTSP,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_WIFIP_MAC,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_WIFI_PHY,POWER_MODULE_STATE_OFF);
	//sys_hal_module_power_ctrl(POWER_MODULE_NAME_MEM5,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_CPU1,POWER_MODULE_STATE_OFF);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_CPU2,POWER_MODULE_STATE_OFF);

	/*config the analog power*/
	/*1.disable audio test mode save 2ma*/
	sys_ll_set_ana_reg25_test_ckaudio_en(0x0);
	sys_ll_set_ana_reg25_audioen(0x0);

	sys_ll_set_ana_reg6_manu_cin(0x0);

	/*2.psram sel mode to save power consumption*/
	sys_ll_set_ana_reg13_vpsramsel(0x1);
	sys_ll_set_ana_reg13_pwdovp1v(0x1);

#if defined(CONFIG_VBSPBUFLP1V_ENABLE)
	uint32_t chip_id = aon_pmu_hal_get_chipid();
	if ((chip_id & PM_CHIP_ID_MASK) != (PM_CHIP_ID_MP_A & PM_CHIP_ID_MASK)){
		sys_ll_set_ana_reg10_vbspbuflp1v(0x1);
	}
#endif

	/*decrease the buck ripple wave,which good for wifi evm from hardware and analog reply */
	sys_ll_set_ana_reg9_spi_latch1v(1);
	sys_ll_set_ana_reg12_dswrsten(0x0);
	sys_ll_set_ana_reg10_aswrsten(0x0);
	sys_ll_set_ana_reg9_spi_latch1v(0);
#endif
	return 0;
}
static void sys_hal_wakeup_source_init(void)
{
	sys_hal_set_ana_reg5_pwd_rosc_spi(1);//bit14
	sys_hal_set_ana_reg8_spi_latch1v(1);//bit9
	sys_hal_set_ana_reg8_lvsleep_wkrst(0);//bit12
	sys_hal_set_ana_reg8_gpiowk_rstn(0);//bit13
	sys_hal_set_ana_reg8_rtcwk_rstn(0);//bit14
	sys_hal_set_ana_reg7_timer_wkrstn(0);
	sys_hal_set_ana_reg5_pwd_rosc_spi(0);//bit14
	sys_hal_set_ana_reg5_rosc_disable(0);//bit12
	sys_hal_set_ana_reg8_pwdovp1v(1);//bit24
	#if defined(CONFIG_KEEP_VDDDIG_VOLT_IN_LV)
	sys_hal_set_ana_reg8_vlden(0);//bit23
	#else
	sys_hal_set_ana_reg8_vlden(1);//bit23
	#endif
	sys_hal_set_ana_reg8_spi_latch1v(0);
}
#if defined(CONFIG_GPIO_RETENTION_SUPPORT)
static void sys_hal_gpio_retention_reset(void)
{
	uint32_t bitmap = aon_pmu_ll_get_r7b_gpio_retention_bitmap();
	if (bitmap != 0){
		const size_t retention_map_size = sizeof(s_gpio_retention_map)/sizeof(s_gpio_retention_map[0]);
		for (size_t i = 0; i < retention_map_size; i++) {
			const gpio_id_t gpio_id = s_gpio_retention_map[i];
			if (gpio_id >= GPIO_NUM_MAX) continue;
			const bool is_high = (bitmap & BIT(i));
			if (is_high) {
				GPIO_UP(gpio_id);
			} else {
				GPIO_DOWN(gpio_id);
			}
		}
	}
}
#endif
__attribute__((section(".iram"))) void sys_hal_flash_aes_clock_gate(void)
{
	/* aes flash clock gate*/
	uint32_t aes_flash = REG_READ(SOC_FLASH_REG_BASE + 0xa*4);
	aes_flash |= 0x1 << 28;
	REG_WRITE(SOC_FLASH_REG_BASE + 0xa*4, aes_flash);
}
void sys_hal_low_power_hardware_init()
{
	sys_hal_flash_aes_clock_gate();

	#if !defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	/*recover aon pmu reg0*/
	uint32_t reg = aon_pmu_ll_get_r7b();
	aon_pmu_ll_set_r0(reg);
	#endif

	/*set GPIO retetnion function continue*/
#if defined(CONFIG_GPIO_RETENTION_SUPPORT)
	sys_hal_gpio_retention_reset();
#endif

	/*gpio state unlock for shutdown wakeup*/
#if defined(CONFIG_AON_PMU_REG0_REFACTOR_DEV)
	aon_pmu_hal_set_gpio_sleep(0, true);
#else
	sys_hal_gpio_state_switch(false);
#endif

	/*set memery bypass*/
	aon_pmu_ll_set_r0_memchk_bps(1);
	aon_pmu_ll_set_r0_fast_boot(0);

	/*set wakeup source*/
	sys_hal_wakeup_source_init();

	/*enable the buck*/
	#if defined(CONFIG_BUCK_ENABLE)
	sys_hal_enable_buck();
	#endif

	/*select lowpower lpo clk source*/
	sys_hal_config_32k_source_default();

	/*default to config the power */
	sys_hal_power_config_default();

	/*set the lp voltage*/
	sys_hal_lp_vol_set(CONFIG_LP_VOL);

	/*set rosc modify 2s interval*/
	#if !defined(CONFIG_ROSC_CALIB_SW)
	sys_hal_rosc_calibration(2, 0x8);
	#endif

	/*rtc on bk7236N and bk7239n requires spi_timerwken*/
	sys_hal_set_ana_reg9_spi_timerwken(1);

	#if CONFIG_PM_LOWEST_POWER_MODE == 1
	bk_phy_set_cali_flag(0x0);
	#endif

}
