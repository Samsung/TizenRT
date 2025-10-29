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
#include "sys_hal.h"
#include "sys_ll.h"
#include "aon_pmu_hal.h"
#include "sys_types.h"
#include <driver/aon_rtc.h>
#include <driver/hal/hal_spi_types.h>
#include <driver/hal/hal_qspi_types.h>
#include "timer_hal.h"
#include <driver/pwr_clk.h>
#include "bk_misc.h"

#define CLKSEL_CORE_480M                 (1)
#define CLKSEL_FLASH_80M                 (0x2)
#define SYS_SWITCH_VDDDIG_VOL_DELAY_TIME    (4000)

static sys_hal_t s_sys_hal;

extern uint32 sys_hal_get_int_group2_status(void);
extern bk_err_t sys_hal_ctrl_vdddig_h_vol(uint32_t vol_value);
extern uint32_t sys_hal_vdddig_h_vol_get();
extern void sys_hal_adjust_dpll(void);

static void sys_hal_delay(volatile uint32_t times);

/**  Platform Start **/
/** Platform Misc Start **/
bk_err_t sys_hal_init()
{
	s_sys_hal.hw = (sys_hw_t *)SYS_LL_REG_BASE;

	return BK_OK;
}

/** Platform PWM Start **/

/** Platform PWM End **/
void sys_hal_set_ana_reg_spi_latch1v(uint32_t v)
{
    sys_ll_set_ana_reg8_spi_latch1v(v);
}

void sys_hal_set_ioldo_bypass(uint32_t v)
{
	sys_ll_set_ana_reg7_bypassen(v);
}

void sys_hal_set_ioldo_volt(uint32_t v)
{
	sys_ll_set_ana_reg7_violdosel(v);
}

void sys_hal_set_analdo_volt(uint32_t v)
{
	sys_ll_set_ana_reg7_vanaldosel(v);
}

void sys_hal_set_analdo_sel(uint32_t v)
{
	sys_ll_set_ana_reg10_aldosel(v);
}

void sys_hal_flash_set_clk(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_flash(value);
}

void sys_hal_flash_set_clk_div(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_flash:0:XTAL  1:APLL  1x :clk_120M,R/W,0x9[25:24]*/
uint32_t sys_hal_flash_get_clk_sel(void)
{
	return sys_ll_get_cpu_clk_div_mode2_cksel_flash();
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_flash:0:/1  1:/2  2:/4  3:/8,R/W,0x9[27:26]*/
uint32_t sys_hal_flash_get_clk_div(void)
{
	return sys_ll_get_cpu_clk_div_mode2_ckdiv_flash();
}

/** Flash end **/

/*for low power function start*/

int sys_hal_rosc_calibration(uint32_t rosc_cali_mode, uint32_t cali_interval)
{
	sys_ll_set_ana_reg6_manu_ena(0);
	sys_ll_set_ana_reg6_modifi_auto(0);
	sys_ll_set_ana_reg6_calib_auto(0);
	sys_ll_set_ana_reg6_spi_trig(0);

	if (rosc_cali_mode == 0) { //Auto
		sys_ll_set_ana_reg6_cal_mode(1);
		sys_ll_set_ana_reg6_calib_auto(1);
		sys_ll_set_ana_reg6_xtal_wakeup_time(8);
		sys_ll_set_ana_reg6_calib_interval(cali_interval);
		sys_ll_set_ana_reg6_spi_trig(1);
		sys_ll_set_ana_reg5_spilatchb_rc32k(1);
	} else if (rosc_cali_mode == 1) { //Manual
		sys_ll_set_ana_reg6_manu_cin(cali_interval);
		sys_ll_set_ana_reg6_calib_interval(cali_interval >> 16);
		sys_ll_set_ana_reg6_manu_ena(1);
		sys_ll_set_ana_reg5_spilatchb_rc32k(1);
		sys_ll_set_ana_reg5_spilatchb_rc32k(0);
	} else if (rosc_cali_mode == 2) { //Modify
		sys_ll_set_ana_reg6_cal_mode(1);
		sys_ll_set_ana_reg6_modifi_auto(1);
		sys_ll_set_ana_reg6_xtal_wakeup_time(8);
		sys_ll_set_ana_reg6_modify_interval(cali_interval);
		sys_ll_set_ana_reg6_spi_trig(1);
		sys_ll_set_ana_reg5_spilatchb_rc32k(1);
	} else if (rosc_cali_mode == 3) { //Trigger
		sys_ll_set_ana_reg6_cal_mode(1);
		sys_ll_set_ana_reg6_spi_trig(1);
		sys_ll_set_ana_reg5_spilatchb_rc32k(1);
	} else { //Disable
		sys_ll_set_ana_reg5_spilatchb_rc32k(1);
		sys_ll_set_ana_reg5_spilatchb_rc32k(0);
	}

	return BK_OK;
}

int sys_hal_rosc_test_mode(bool enabled)
{
	// if (enabled) {
	// 	sys_ll_set_ana_reg4_ck_tst_enbale(1);
	// 	sys_ll_set_ana_reg4_cktst_sel(0);
	// 	sys_ll_set_ana_reg5_rosc_tsten(1);
	// 	sys_ll_set_ana_reg6_cal_mode(1);

	// 	REG_WRITE((SOC_AON_GPIO_REG_BASE + (24 << 2)), 0x40);
	// } else {
	// 	sys_ll_set_ana_reg4_ck_tst_enbale(0);
	// 	sys_ll_set_ana_reg4_cktst_sel(0);
	// 	sys_ll_set_ana_reg5_rosc_tsten(0);
	// 	sys_ll_set_ana_reg6_cal_mode(0);
	// 	sys_ll_set_ana_reg6_calib_auto(0);
	// }
	return BK_OK;
}

void sys_hal_cpu0_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state)
{
    sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask( clock_state);
}

void sys_hal_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value)
{
    clk_div_address_map_t clk_div_address_map_table[] = CLK_DIV_ADDRESS_MAP;
    clk_div_address_map_t *clk_div_addr = &clk_div_address_map_table[reg];

    uint32_t clk_div_reg_address = clk_div_addr->reg_address;

	REG_WRITE(clk_div_reg_address, value);
}

uint32_t sys_hal_all_modules_clk_div_get(clk_div_reg_e reg)
{
    clk_div_address_map_t clk_div_address_map_table[] = CLK_DIV_ADDRESS_MAP;
    clk_div_address_map_t *clk_div_addr = &clk_div_address_map_table[reg];

	 uint32_t clk_div_reg_address = clk_div_addr->reg_address;

	return REG_READ(clk_div_reg_address);
}

void sys_hal_cpu_clk_div_set(uint32_t core_index, uint32_t value)
{
	if(core_index == 0)//cpu0
	{
		sys_ll_set_cpu0_int_halt_clk_op_cpu0_speed(value);
	}
}

uint32_t sys_hal_cpu_clk_div_get(uint32_t core_index)
{
	uint32_t value = 2;

	if(core_index == 0)//cpu0
	{
		value = sys_ll_get_cpu0_int_halt_clk_op_cpu0_speed();
	}

	return value;
}

void sys_hal_set_iobyapssen(uint32_t enable) {
    sys_hal_set_ana_reg_spi_latch1v(1);
    if (enable) {
        //tenglong20250508: ioldo >= vbat(3.5V) before iobypass change
        sys_hal_set_ioldo_volt(6);
        sys_hal_set_ioldo_bypass(1);
    } else {
        sys_hal_set_ioldo_bypass(0);
        sys_hal_set_ioldo_volt(4); //shuguang20250512:vio=3.3V
    }
    sys_hal_set_ana_reg_spi_latch1v(0);
}

void sys_hal_set_violdosel(uint32_t flag) {
}

void sys_hal_lp_anabuf_set(uint32_t v) {
}

int32 sys_hal_lp_vol_set(uint32_t value)
{
	sys_ll_set_ana_reg8_spi_latch1v(1);
	sys_ll_set_ana_reg8_vcorelsel(value);
	sys_ll_set_ana_reg8_spi_latch1v(0);

	return 0;
}

uint32_t sys_hal_lp_vol_get()
{
	return sys_ll_get_ana_reg8_vcorelsel();
}

int32 sys_hal_rf_tx_vol_set(uint32_t value)
{
	sys_ll_set_ana_reg8_spi_latch1v(1);
	// sys_ll_set_ana_reg7_t_vanaldosel(value);
	sys_ll_set_ana_reg8_spi_latch1v(0);
	return 0;
}

uint32_t sys_hal_rf_tx_vol_get()
{
	// return sys_ll_get_ana_reg7_t_vanaldosel();
	return 0;
}

int32 sys_hal_rf_rx_vol_set(uint32_t value)
{
	sys_ll_set_ana_reg8_spi_latch1v(1);
	// sys_ll_set_ana_reg7_r_vanaldosel(value);
	sys_ll_set_ana_reg8_spi_latch1v(0);
	return 0;
}

uint32_t sys_hal_rf_rx_vol_get()
{
	// return sys_ll_get_ana_reg7_r_vanaldosel();
	return 0;
}

int32 sys_hal_bandgap_cali_set(uint32_t value)//increase or decrease the dvdddig voltage
{
	return 0;
}
uint32_t sys_hal_bandgap_cali_get()
{
	return 0;
}

int32 sys_hal_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core,uint32_t ckdiv_bus, uint32_t ckdiv_cpu0,uint32_t ckdiv_cpu1)
{
	uint32_t clk_param  = 0;
	if(cksel_core > 1)
	{
		BK_LOGD(NULL, "set dvfs cksel core > 3 invalid %d\r\n",cksel_core);
		return BK_FAIL;
	}

	if((ckdiv_core > FREQUNCY_DIV_MAX) || (ckdiv_bus > FREQUNCY_DIV_BUS_MAX)||(ckdiv_cpu1 > FREQUNCY_DIV_CPU_MAX)||(ckdiv_cpu0 > FREQUNCY_DIV_CPU_MAX))
	{
		BK_LOGD(NULL, "set dvfs ckdiv_core ckdiv_bus ckdiv_cpu0_1  ckdiv_cpu0_1  > 1 invalid\r\n");
		return BK_FAIL;
	}

	clk_param = 0;
	clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
	if(((clk_param >> 0x4)&0x1) > cksel_core)//when it from the higher frequency to lower frequency
	{
		/*1.core clk select*/
		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x1 << 4);
		clk_param |=  cksel_core << 4;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config bus and core clk div*/
		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0xF << 0);
		clk_param |=  ckdiv_core << 0;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*3.config cpu clk div*/
		sys_hal_cpu_clk_div_set(0,ckdiv_cpu0);
	}
	else//when it from the lower frequency to higher frequency
	{
		/*1.config bus and core clk div*/
		if(ckdiv_core == 0)
		{
			sys_hal_cpu_clk_div_set(0,ckdiv_cpu0);//avoid the bus freq > 240m
		}

		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0xF << 0);
		clk_param |=  ckdiv_core << 0;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config cpu clk div*/
		if(ckdiv_core != 0)
		{
			sys_hal_cpu_clk_div_set(0,ckdiv_cpu0);
		}

		/*3.core clk select*/

		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x1 << 4);
		clk_param |=  cksel_core << 4;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);
	}

	return BK_OK;
}

bk_err_t sys_hal_ctrl_vdddig_h_vol(uint32_t vol_value)
{
	if(sys_ll_get_ana_reg8_vcorehsel() != vol_value)
	{
		sys_ll_set_ana_reg8_spi_latch1v(1);
		sys_ll_set_ana_reg8_vcorehsel(vol_value);
		sys_ll_set_ana_reg8_spi_latch1v(0);
		sys_hal_delay(SYS_SWITCH_VDDDIG_VOL_DELAY_TIME);//when cpu0 max freq 240m ,it delay 10uS for voltage stability
	}

	return BK_OK;
}

uint32_t sys_hal_vdddig_h_vol_get()
{
	return sys_ll_get_ana_reg8_vcorehsel();
}

bk_err_t sys_hal_switch_cpu_bus_freq_high_to_low(cpu_freq_e cpu_bus_freq)
{
	bk_err_t ret = BK_OK;
	switch(cpu_bus_freq)
	{
		case CPU_FRQ_480M://cpu0:240m;cpu1:480m;cpu2:480m;bus:240m
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x0,0x0,0x0,0x1);
			sys_hal_ctrl_vdddig_h_vol(0xD);//0.925V

			break;
		case CPU_FRQ_240M://cpu0:240m;cpu1:240m;;cpu2:240m;bus:240m
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x1,0x0,0x1,0x1);
			sys_hal_ctrl_vdddig_h_vol(0xC);//0.9V

			break;
		case CPU_FRQ_160M://cpu0:160m;cpu1:xm;;cpu2:xm;bus:240m
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x2,0x0,0x1,0x1);
			sys_hal_ctrl_vdddig_h_vol(0xC);//0.95V
			break;
		case CPU_FRQ_120M://cpu0:120m;cpu1:120m;cpu2:120m;bus:120m
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x3,0x0,0x1,0x1);
			#if CONFIG_ATE_TEST
			sys_hal_ctrl_vdddig_h_vol(0x7);//0.775V
			#else
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			#endif

			break;
		case CPU_FRQ_80M://cpu0:80m;cpu1:80m;cpu2:80m;bus:80m
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x5,0x0,0x1,0x1);
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V

			break;
		case CPU_FRQ_60M://cpu0:60m;cpu1:60m;cpu2:60m;bus:60m
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x7,0x0,0x1,0x1);
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			break;
		case CPU_FRQ_30M://cpu0:30m;cpu1:30m;cpu2:30m;bus:30m
			ret = sys_hal_core_bus_clock_ctrl(0x1,0xF,0x0,0x1,0x1);
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			break;
		case CPU_FRQ_26M://cpu0:26m;cpu1:26m;cpu2:26m;bus:26m
			ret = sys_hal_core_bus_clock_ctrl(0x0,0x0,0x0,0x1,0x1);
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			break;
		default:
			break;
	}

	return ret;
}
bk_err_t sys_hal_switch_cpu_bus_freq_low_to_high(cpu_freq_e cpu_bus_freq)
{
	bk_err_t ret = BK_OK;
	switch(cpu_bus_freq)
	{
		case CPU_FRQ_480M://cpu0:240m;cpu1:480m;cpu2:480m;bus:240m
			sys_hal_ctrl_vdddig_h_vol(0xD);//0.925V
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x0,0x0,0x0,0x1);
			break;
		case CPU_FRQ_240M://cpu0:240m;cpu1:240m;;cpu2:240m;bus:240m
			sys_hal_ctrl_vdddig_h_vol(0xC);//0.9V
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x1,0x0,0x1,0x1);
			break;
		case CPU_FRQ_160M://cpu0:160m;cpu1:xm;;cpu2:xm;bus:240m
			sys_hal_ctrl_vdddig_h_vol(0xC);//0.95V
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x2,0x0,0x1,0x1);
			break;
		case CPU_FRQ_120M://cpu0:120m;cpu1:120m;cpu2:120m;bus:120m
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x3,0x0,0x1,0x1);
			break;
		case CPU_FRQ_80M://cpu0:80m;cpu1:80m;cpu2:80m;bus:80m
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x5,0x0,0x1,0x1);
			break;
		case CPU_FRQ_60M://cpu0:60m;cpu1:60m;cpu2:60m;bus:60m
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			ret = sys_hal_core_bus_clock_ctrl(0x1,0x7,0x0,0x1,0x1);
			break;
		case CPU_FRQ_30M://cpu0:30m;cpu1:30m;cpu2:30m;bus:30m
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			ret = sys_hal_core_bus_clock_ctrl(0x1,0xF,0x0,0x1,0x1);
			break;
		case CPU_FRQ_26M://cpu0:26m;cpu1:26m;cpu2:26m;bus:26m
			sys_hal_ctrl_vdddig_h_vol(0xB);//0.875V
			ret = sys_hal_core_bus_clock_ctrl(0x0,0x0,0x0,0x1,0x1);
			break;
		default:
			break;
	}
	return ret;
}
static cpu_freq_e s_pre_cpu_freq = CPU_FRQ_120M;
bk_err_t sys_hal_switch_cpu_bus_freq(cpu_freq_e cpu_bus_freq)
{
	bk_err_t ret = BK_OK;

	cpu_freq_e prev_freq = s_pre_cpu_freq;

	if(prev_freq == cpu_bus_freq)
		return BK_OK;

	if(prev_freq > cpu_bus_freq)// eg: 480->60
	{
		sys_hal_switch_cpu_bus_freq_high_to_low(cpu_bus_freq);
	}
	else // eg: 60-480
	{
		sys_hal_switch_cpu_bus_freq_low_to_high(cpu_bus_freq);
	}
	s_pre_cpu_freq = cpu_bus_freq;

	return ret;
}


/*for low power function end*/
/*sleep feature end*/

uint32 sys_hal_get_chip_id(void)
{
	return sys_ll_get_version_id_versionid();
}

uint32 sys_hal_get_device_id(void)
{
	return sys_ll_get_device_id_deviceid();
}

__IRAM_SEC int32 sys_hal_int_disable(uint32 param) //CMD_ICU_INT_DISABLE
{
	uint32 reg = 0;
	uint32 value = 0;

	reg = sys_ll_get_cpu0_int_0_31_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu0_int_0_31_en_value(reg);

	return value;
}

__IRAM_SEC int32 sys_hal_int_enable(uint32 param) //CMD_ICU_INT_ENABLE
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_0_31_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_0_31_en_value(reg);

	return 0;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
__IRAM_SEC int32 sys_hal_int_group2_disable(uint32 param)
{
	uint32 reg = 0;
	uint32 value = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

	return value;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
__IRAM_SEC int32 sys_hal_int_group2_enable(uint32 param)
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

	return 0;
}

#if CONFIG_SOC_BK7236_SMP_TEMP
int32 sys_hal_core_int_group1_disable(uint32 core_id, uint32 param)
{
	uint32 reg = 0;
	uint32 value = 0;

	if(CPU0_CORE_ID == core_id){
		reg = sys_ll_get_cpu0_int_0_31_en_value();
		value = reg;
		reg &= ~(param);
		sys_ll_set_cpu0_int_0_31_en_value(reg);
	}

	return value;
}

int32 sys_hal_core_int_group1_enable(uint32 core_id, uint32 param)
{
	uint32 reg = 0;

	if(CPU0_CORE_ID == core_id){
		reg = sys_ll_get_cpu0_int_0_31_en_value();
		reg |= (param);
		sys_ll_set_cpu0_int_0_31_en_value(reg);
	}

	return 0;
}

int32 sys_hal_core_int_group2_disable(uint32 core_id, uint32 param)
{
	uint32 reg = 0;
	uint32 value = 0;

	if(CPU0_CORE_ID == core_id){
		reg = sys_ll_get_cpu0_int_32_63_en_value();
		value = reg;
		reg &= ~(param);
		sys_ll_set_cpu0_int_32_63_en_value(reg);
	}

	return value;
}

int32 sys_hal_core_int_group2_enable(uint32 core_id, uint32 param)
{
	uint32 reg = 0;

	if(CPU0_CORE_ID == core_id){
		reg = sys_ll_get_cpu0_int_32_63_en_value();
		reg |= (param);
		sys_ll_set_cpu0_int_32_63_en_value(reg);
	}

	return 0;
}
#endif

int32 sys_hal_fiq_disable(uint32 param)
{
	uint32 reg = 0;
	uint32 value = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	value = reg;
	reg &= ~(param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

	return value;
}

int32 sys_hal_fiq_enable(uint32 param)
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_32_63_en_value();
	reg |= (param);
	sys_ll_set_cpu0_int_32_63_en_value(reg);

	return 0;
}

int32 sys_hal_global_int_disable(uint32 param)
{
	int32 ret = 0;

	return ret;
}

int32 sys_hal_global_int_enable(uint32 param)
{
	int32 ret = 0;

	return ret;
}

uint32 sys_hal_get_int_status(void)
{
	return sys_ll_get_cpu0_int_0_31_status_value();
}

__attribute__((section(".itcm_sec_code"))) uint32 sys_hal_get_int_group2_status(void)
{
	return sys_ll_get_cpu0_int_32_63_status_value();
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_gpio_int_st: ,R,0x29[22]*/
uint32_t sys_hal_get_cpu0_gpio_int_st(void)
{
    return sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_s_int_en();
}

//NOTICE:INT source status is read only and can't be set, other projects is error, we'll delete them.
int32 sys_hal_set_int_status(uint32 param)
{
	return 0;
}

uint32 sys_hal_get_fiq_reg_status(void)
{
	uint32 reg = 0;

	reg = sys_ll_get_cpu0_int_32_63_status_value();

	return reg;
}

uint32 sys_hal_set_fiq_reg_status(uint32 param)
{
	uint32 reg = 0;

	///TODO:this reg is read only

	return reg;
}

uint32 sys_hal_get_intr_raw_status(void)
{
	uint32 reg = 0;

	///TODO:
	reg = sys_ll_get_cpu0_int_0_31_status_value();

	return reg;
}

uint32 sys_hal_set_intr_raw_status(uint32 param)
{
	uint32 reg = 0;

	///TODO:this reg is read only

	return reg;
}

int32 sys_hal_set_jtag_mode(uint32 param)
{
	return 0;
}

uint32 sys_hal_get_jtag_mode(void)
{
	return 0;
}

/* NOTICE: NOTICE: NOTICE: NOTICE: NOTICE: NOTICE: NOTICE
 * BK7256 clock, power is different with previous products(2022-01-10).
 * Previous products peripheral devices use only one signal of clock enable.
 * BK7256 uses clock and power signal to control one device,
 * This function only enable clock signal, we needs to enable power signal also
 * if we want to enable one device.
 */
__IRAM_SEC void sys_hal_clk_pwr_ctrl(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up)
{
	uint32_t v = 0;

    BK_ASSERT(CLK_PWR_ID_NONE > dev);

	v = sys_ll_get_cpu_device_clk_enable_value();

	if(CLK_PWR_CTRL_PWR_UP == power_up)
		v |= (1 << dev);
	else
		v &= ~(1 << dev);

	sys_ll_set_cpu_device_clk_enable_value(v);
}

/* UART select clock **/
void sys_hal_uart_select_clock(uart_id_t id, uart_src_clk_t mode)
{
	int sel_xtal = 0;
	int sel_appl = 1;

	switch(id)
	{
		case UART_ID_0:
			{
				if(mode == UART_SCLK_APLL)
					sys_ll_set_cpu_clk_div_mode1_cksel_uart0(sel_appl);
				else
					sys_ll_set_cpu_clk_div_mode1_cksel_uart0(sel_xtal);
				break;
			}
		case UART_ID_1:
			{
				if(mode == UART_SCLK_APLL)
					sys_ll_set_cpu_clk_div_mode1_cksel_uart1(sel_appl);
				else
					sys_ll_set_cpu_clk_div_mode1_cksel_uart1(sel_xtal);
				break;
			}
		case UART_ID_2:
			{
				if(mode == UART_SCLK_APLL)
					sys_ll_set_cpu_clk_div_mode1_cksel_uart2(sel_appl);
				else
					sys_ll_set_cpu_clk_div_mode1_cksel_uart2(sel_xtal);
				break;
			}
		default:
			break;
	}

}

void sys_hal_pwm_select_clock(sys_sel_pwm_t num, pwm_src_clk_t mode)
{
	uint32_t sel_clk32 = 0;
	uint32_t sel_xtal = 1;

	switch(num)
	{
		case SYS_SEL_PWM0:
			if(mode == PWM_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(sel_clk32);
			break;
		case SYS_SEL_PWM1:
			if(mode == PWM_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(sel_clk32);
			break;

		default:
			break;
	}
}

void sys_hal_timer_select_clock(sys_sel_timer_t num, timer_src_clk_t mode)
{
	uint32_t sel_clk32 = 0;
	uint32_t sel_xtal = 1;

	switch(num)
	{
		case SYS_SEL_TIMER0:
			if(mode == TIMER_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_tim0(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_tim0(sel_clk32);
			break;
		case SYS_SEL_TIMER1:
			if(mode == TIMER_SCLK_XTAL)
				sys_ll_set_cpu_clk_div_mode1_cksel_tim1(sel_xtal);
			else
				sys_ll_set_cpu_clk_div_mode1_cksel_tim1(sel_clk32);
			break;

		default:
			break;
	}
}

uint32_t sys_hal_timer_select_clock_get(sys_sel_timer_t id)
{
	uint32_t ret = 0;

	switch(id)
	{
		case SYS_SEL_TIMER0:
		{
			ret = sys_ll_get_cpu_clk_div_mode1_cksel_tim0();
			break;
		}
		case SYS_SEL_TIMER1:
		{
			ret = sys_ll_get_cpu_clk_div_mode1_cksel_tim1();
			break;
		}
		default:
			break;
	}

	ret = (ret) ? TIMER_SCLK_XTAL : TIMER_SCLK_CLK32;

	return ret;
}

void sys_hal_spi_select_clock(spi_id_t num, spi_src_clk_t mode)
{
	uint32_t sel_xtal = 0;
	uint32_t sel_60m = 1;
	uint32_t sel_80m = 3;

	switch(num)
	{
		case SPI_ID_0:
			if (mode == SPI_CLK_XTAL) {
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(sel_xtal);
			} else if (mode == SPI_CLK_APLL) {
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(sel_60m);
			} else {
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(sel_80m);
			}
			break;
#if (SOC_SPI_UNIT_NUM > 1)
		case SPI_ID_1:
			if (mode == SPI_CLK_XTAL) {
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sel_xtal);
			} else if (mode == SPI_CLK_APLL) {
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sel_60m);
			} else {
				sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sel_80m);
			}
			break;
#endif
		default:
			break;
	}
}

void sys_hal_qspi_clk_sel(uint32_t param)
{
		sys_ll_set_cpu_clk_div_mode2_cksel_qspi0(param);
}

void sys_hal_qspi_set_src_clk_div(uint32_t value)
{
		sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(value);
}

void sys_hal_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel)
{
}

dev_clk_select_t sys_hal_get_clk_select(dev_clk_select_id_t dev)
{
	return 0;
}

//DCO divider is valid for all of the peri-devices.
void sys_hal_set_dco_div(dev_clk_dco_div_t div)
{
}

//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_hal_get_dco_div(void)
{
	return 0;
}

/*clock power control end*/
void sys_hal_set_cksel_sadc(uint32_t value)
{
}

void sys_hal_set_cksel_pwm0(uint32_t value)
{
}

void sys_hal_set_cksel_pwm1(uint32_t value)
{
}

void sys_hal_set_cksel_pwm(uint32_t value)
{
    sys_hal_set_cksel_pwm0(value);
    sys_hal_set_cksel_pwm1(value);
}

uint32_t sys_hal_uart_select_clock_get(uart_id_t id)
{
    uint32_t ret = 0;

    switch(id)
    {
        case UART_ID_0:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_uart0();
            break;
        }
        case UART_ID_1:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_uart1();
            break;
        }
        case UART_ID_2:
        {
            ret = sys_ll_get_cpu_clk_div_mode1_cksel_uart2();
            break;
        }
        default:
            break;
    }

    ret = (!ret)?UART_SCLK_XTAL_26M:UART_SCLK_APLL;

    return ret;
}

void sys_hal_sadc_int_enable(void)
{
    sys_hal_int_enable(SADC_INTERRUPT_CTRL_BIT);
}

void sys_hal_sadc_int_disable(void)
{
    sys_hal_int_disable(SADC_INTERRUPT_CTRL_BIT);
}

void sys_hal_sadc_pwr_up(void)
{
	sys_ll_set_cpu_device_clk_enable_sadc_cken(1);
}

void sys_hal_sadc_pwr_down(void)
{
	sys_ll_set_cpu_device_clk_enable_sadc_cken(0);
}

void sys_hal_set_saradc_config(void)
{
	/* yanxiang.wei@bekencorp.com, based on doc: bk7236n_v2_gadc register setting.xlms*/
	sys_ana_ll_set_ana_reg2_inbufen(1);
	sys_ana_ll_set_ana_reg2_gadc_calcap_ch(3);
	sys_ana_ll_set_ana_reg2_gadc_clk_inv(1);
	sys_ana_ll_set_ana_reg2_gadc_clk_sel(1);
	sys_ana_ll_set_ana_reg2_gadc_calintsaw_en(1);
	sys_ana_ll_set_ana_reg2_gadc_clk_rlten(1);
	sys_ana_ll_set_ana_reg2_gadc_vbg_sel(1);
	sys_ana_ll_set_ana_reg2_gadc_vpcalsaw(4);
	sys_ana_ll_set_ana_reg2_gadc_vncalsaw(4);
	sys_ana_ll_set_ana_reg2_gadc_bscalsaw(4);
	sys_ana_ll_set_ana_reg2_gadc_compisel(4);
	sys_ana_ll_set_ana_reg2_nc_10_10(1);
	sys_ana_ll_set_ana_reg2_gadc_inbufsel(1);
	sys_ana_ll_set_ana_reg2_sar_enspi(1);

	sys_ana_ll_set_ana_reg3_offset_en(0);
	sys_ana_ll_set_ana_reg3_vref_sel(1);
	sys_ana_ll_set_ana_reg3_refbuff_isel(4);
	sys_ana_ll_set_ana_reg3_preamp_isel(4);
}

void sys_hal_set_sdmadc_config(void)
{
	/*TODO*/
}

void sys_hal_set_clksel_spi0(uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(value);
}

void sys_hal_set_clksel_spi1(uint32_t value)
{
    sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(value);
}

void sys_hal_set_clksel_spi(uint32_t value)
{
    if((SPI_CLK_SRC_XTAL == value) || (SPI_CLK_SRC_APLL == value))
    {
        sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(value);
        sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(value);
    }
    else
    {
        //BK_LOGD(NULL, "spi cksel is not support on BK7256 in function:%s, line:%d\n", __FUNCTION__, __LINE__);
    }
}

void sys_hal_en_tempdet(uint32_t value)
{
    sys_ll_set_ana_reg5_en_temp(value);
}

uint32_t sys_hal_mclk_mux_get(void)
{
	UINT32 ret = 0;

	ret = sys_ll_get_cpu_clk_div_mode1_cksel_core();

	return ret;
}

void sys_hal_mclk_mux_set(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_cksel_core(value);
}

uint32_t sys_hal_mclk_div_get(void)
{
	UINT32 ret = 0;

	ret = sys_ll_get_cpu_clk_div_mode1_clkdiv_core();

	return ret;
}

void sys_hal_mclk_div_set(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode1_clkdiv_core(value);
}

/**  Platform End **/




/**  BT Start **/
//BT
void sys_hal_bt_power_ctrl(bool power_up)
{
    if (power_up)
    {
        sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(0);
    }
    else
    {
        sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(1);
    }
}

void sys_hal_bt_clock_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu_device_clk_enable_btdm_cken(1);
    }
    else
    {
        sys_ll_set_cpu_device_clk_enable_btdm_cken(0);
    }
}

void sys_hal_xvr_clock_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu_device_clk_enable_xvr_cken(1);
    }
    else
    {
        sys_ll_set_cpu_device_clk_enable_xvr_cken(0);
    }
}

void sys_hal_btdm_interrupt_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_int_en(1);
    }
    else
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_int_en(0);
    }
}

void sys_hal_ble_interrupt_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_int_en(1);
    }
    else
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_int_en(0);
    }
}

void sys_hal_bt_interrupt_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_int_en(1);
    }
    else
    {
        sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_int_en(0);
    }
}

void sys_hal_thread_interrupt_ctrl(bool en)
{
    if (en) {
        sys_ll_set_cpu0_int_32_63_en_cpu0_thread_int_en(1);
    } else {
        sys_ll_set_cpu0_int_32_63_en_cpu0_thread_int_en(0);
    }
}

void sys_hal_bt_rf_ctrl(bool en)
{
    if (en)
    {
        //todo
    }
    else
    {
        //todo
    }
}

void sys_hal_rf_ctrl(uint8_t type)
{
    if (type == RF_CTRL_PTA) {
        // rf ctrl by PTA
        sys_ll_set_cpu_storage_connect_op_select_rf_switch_manual_en(0);
    } else if (type == RF_CTRL_WIFI) {
        // rf ctrl by Wi-Fi
        sys_ll_set_cpu_storage_connect_op_select_rf_switch_manual_en(1);
        sys_ll_set_cpu_storage_connect_op_select_rf_source(RF_CTRL_WIFI);
    } else if (type == RF_CTRL_BT) {
        // rf ctrl by BT
        sys_ll_set_cpu_storage_connect_op_select_rf_switch_manual_en(1);
        sys_ll_set_cpu_storage_connect_op_select_rf_source(RF_CTRL_BT);
    } else if (type == RF_CTRL_THREAD) {
        // rf ctrl by thread
       sys_ll_set_cpu_storage_connect_op_select_rf_switch_manual_en(1);
       sys_ll_set_cpu_storage_connect_op_select_rf_source(RF_CTRL_THREAD);
    }
}

uint8_t sys_hal_rf_ctrl_type_get(void)
{
    uint8_t type = 0;
    if (sys_ll_get_cpu_storage_connect_op_select_rf_switch_manual_en()) {
        type = sys_ll_get_cpu_storage_connect_op_select_rf_source();
        if (type == RF_CTRL_PTA || type == RF_CTRL_WIFI) {
            type = RF_CTRL_WIFI;
        }
    } else {
        type = RF_CTRL_PTA;
    }

    return type;
}

uint32_t sys_hal_bt_rf_status_get(void)
{
    //todo
    return 0;
}

void sys_hal_bt_sleep_exit_ctrl(bool en)
{
    if (en)
    {
        sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(1);
    }
    else
    {
        sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(0);
    }
}

void sys_hal_set_bts_wakeup_platform_en(bool value)
{
	/*TODO
	sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(value);*/
	while(1){;}
}

uint32 sys_hal_get_bts_wakeup_platform_en()
{
	/*TODO
	sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en();*/
	while(1){;}
	return 0;
}

/**
  * @brief	dma2d system config
  * param1: clk source sel 0:clk_320M	   1:clk_480M,
  * param2: clk_always_on  ENABLE,0: bus clock auto open when module is select,1:bus clock always open
  * param1: int_en eanble lcd cpu int
  * return none
  */
void sys_hal_dma2d_clk_en(uint8_t clk_always_on)
{
}

void sys_hal_set_jpeg_dec_disckg(uint32_t value)
{
}

/**  Video End **/

/**  WIFI Start **/
//WIFI

//Yantao Add Start
void sys_hal_modem_core_reset(void)
{
	//TODO, 7256 NO modem core reset
}

void sys_hal_mpif_invert(void)
{
	//TODO, 7256 NO mpif_invert
}

void sys_hal_modem_subsys_reset(void)
{
	//TODO, 7256 NO subsys reset
}
void sys_hal_mac_subsys_reset(void)
{
	//TODO, 7256 NO subsys reset
}
void sys_hal_dsp_subsys_reset(void)
{
	//TODO, 7256 NO subsys reset
}
void sys_hal_mac_power_ctrl(bool power_up)
{
	//WARNING:the low-level is power-down
	sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(!(power_up));
}

void sys_hal_modem_power_ctrl(bool power_up)
{
	sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(!(power_up));
}

void sys_hal_pta_ctrl(bool pta_en)
{
	//TODO, 7256 NO pta enable
}

__IRAM_SEC void sys_hal_modem_bus_clk_ctrl(bool clk_en)
{
	//TODO, 7256 no bus clock enable
}

__IRAM_SEC void sys_hal_modem_clk_ctrl(bool clk_en)
{
	sys_ll_set_cpu_device_clk_enable_phy_cken(clk_en);
}


void sys_hal_mac_bus_clk_ctrl(bool clk_en)
{
	//TODO, 7256 no bus clock enable
}

void sys_hal_mac_clk_ctrl(bool clk_en)
{
	sys_ll_set_cpu_device_clk_enable_mac_cken(clk_en);
}



void sys_hal_set_vdd_value(uint32_t param)
{
	sys_hal_ctrl_vdddig_h_vol(param);
}

uint32_t sys_hal_get_vdd_value(void)
{
	return sys_ll_get_ana_reg8_vcorehsel();
}

//CMD_SCTRL_BLOCK_EN_MUX_SET
void sys_hal_block_en_mux_set(uint32_t param)
{
	//TODO 7256 no block en mux
}

__IRAM_SEC void sys_hal_set_sys2flsh_2wire(uint32_t value)
{
}

void sys_hal_enable_mac_gen_int(void)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_mac_int_gen_int_en(1);
}

void sys_hal_enable_mac_prot_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_port_trigger_int_en(1);
}

void sys_hal_enable_mac_tx_trigger_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_tx_trigger_int_en(1);
}

void sys_hal_enable_mac_rx_trigger_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_rx_trigger_int_en(1);
}

void sys_hal_enable_mac_txrx_misc_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_tx_rx_misc_int_en(1);
}

void sys_hal_enable_mac_txrx_timer_int(void)
{
	sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_tx_rx_timer_int_en(1);
}

void sys_hal_enable_modem_int(void)
{
}

void sys_hal_enable_modem_rc_int(void)
{
}

void sys_hal_enable_hsu_int(void)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_hsu_irq_int_en(1);
}

void sys_hal_disable_hsu_int(void)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_hsu_irq_int_en(0);
}

//Yantao Add End

void sys_hal_cali_dpll_spi_trig_disable(void)
{
	// NOT SUPPORT
}

void sys_hal_cali_dpll_spi_trig_enable(void)
{
	// NOT SUPPORT
}

void sys_hal_cali_dpll_spi_detect_disable(void)
{
    sys_ll_set_ana_reg1_osccal_trig(0);
}

void sys_hal_cali_dpll_spi_detect_enable(void)
{
    sys_ll_set_ana_reg1_osccal_trig(1);
}

void sys_hal_set_xtalh_ctune(uint32_t value)
{
    sys_ll_set_ana_reg2_xtalh_ctune(value);
}

__IRAM_SEC void sys_hal_analog_set(analog_reg_t reg, uint32_t value)
{
    uint32_t analog_reg_address;

    if ((reg < ANALOG_REG0) || (reg >= ANALOG_MAX)) {
        return;
    }

    analog_reg_address = SYS_ANA_REG0_ADDR + (reg - ANALOG_REG0) * 4;

	sys_ll_set_analog_reg_value(analog_reg_address, value);
}
uint32_t sys_hal_analog_get(analog_reg_t reg)
{
    uint32_t analog_reg_address;

    if ((reg < ANALOG_REG0) || (reg >= ANALOG_MAX)) {
        return 0;
    }

    analog_reg_address = SYS_ANA_REG0_ADDR + (reg - ANALOG_REG0) * 4;

	return sys_ll_get_analog_reg_value(analog_reg_address);
}
void sys_hal_set_ana_reg1_value(uint32_t value)
{
    sys_ll_set_ana_reg1_value(value);
}

void sys_hal_set_ana_reg2_value(uint32_t value)
{
    sys_ll_set_ana_reg2_value(value);
}

void sys_hal_set_ana_reg3_value(uint32_t value)
{
    sys_ll_set_ana_reg3_value(value);
}

void sys_hal_set_ana_reg4_value(uint32_t value)
{
    sys_ll_set_ana_reg4_value(value);
}

void sys_hal_set_ana_reg12_value(uint32_t value)
{
    sys_ll_set_ana_reg12_value(value);
}

void sys_hal_set_ana_reg13_value(uint32_t value)
{
    sys_ll_set_ana_reg13_value(value);
}

void sys_hal_set_ana_reg14_value(uint32_t value)
{
    // sys_ll_set_ana_reg14_value(value);
}

void sys_hal_set_ana_reg15_value(uint32_t value)
{
    // sys_ll_set_ana_reg15_value(value);
}

void sys_hal_set_ana_reg16_value(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_set_ana_reg17_value(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_set_ana_reg18_value(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_set_ana_reg19_value(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_set_ana_reg20_value(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_set_ana_reg21_value(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_set_ana_reg27_value(uint32_t value)
{
	// NOT SUPPORT
}

uint32_t sys_hal_bias_reg_read(void)
{
	///TODO
    return 0;
}
uint32_t sys_hal_bias_reg_write(uint32_t param)
{
	///TODO

	return 0;

}

uint32_t sys_hal_analog_reg2_get(void)
{
	///TODO

    return 0;
}

uint32_t sys_hal_bias_reg_set(uint32_t param)
{
	///TODO

	return 0;
}

uint32_t sys_hal_bias_reg_clean(uint32_t param)
{
	///TODO

	return 0;
}


uint32_t sys_hal_get_xtalh_ctune(void)
{
    return sys_ll_get_ana_reg2_xtalh_ctune();
}

uint32_t sys_hal_cali_bgcalm(void)
{
    uint32_t bandgap;

    sys_ll_set_ana_reg8_spi_latch1v(1);
    sys_ll_set_ana_reg7_pwd_bgcal(0);
    sys_ll_set_ana_reg7_vbgcalmode(0);
    sys_ll_set_ana_reg7_vbgcalstart(0);
    sys_ll_set_ana_reg7_vbgcalstart(1);
    delay_us(100); //100us
    sys_ll_set_ana_reg7_vbgcalstart(0);

    delay_us(2000); //2ms to avoid consistency issue

    bandgap = aon_pmu_ll_get_r7d_bgcal();

    sys_ll_set_ana_reg7_vbgcalmode(1);
    sys_ll_set_ana_reg7_pwd_bgcal(1);
    sys_ll_set_ana_reg8_spi_latch1v(0);

    return bandgap;
}

uint32_t sys_hal_get_bgcalm(void)
{
    return sys_ll_get_ana_reg7_bgcal();
}

void sys_hal_set_bgcalm(uint32_t value)
{
    sys_ll_set_ana_reg8_spi_latch1v(1);
    sys_ll_set_ana_reg7_bgcal(value);
    sys_ll_set_ana_reg8_spi_latch1v(0);
}

void sys_hal_set_audioen(uint32_t value)
{
}

void sys_hal_set_dpll_div_cksel(uint32_t value)
{
}

void sys_hal_set_dpll_reset(uint32_t value)
{
}

void sys_hal_set_gadc_ten(uint32_t value)
{
}
/**  WIFI End **/

/**  Audio Start  **/

void sys_hal_aud_select_clock(uint32_t value)
{
}

void sys_hal_aud_clock_en(uint32_t value)
{
}

void sys_hal_aud_mic1_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_dacl_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_diffen_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_mic_rst_set(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_mic1_gain_set(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_dcoc_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_lmdcin_set(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_audbias_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_adcbias_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_micbias_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_dac_bias_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_idac_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_dac_drv_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_mic1_single_en(uint32_t value)
{
}

void sys_hal_aud_mic2_single_en(uint32_t value)
{
}

void sys_hal_aud_int_en(uint32_t value)
{
}

void sys_hal_aud_power_en(uint32_t value)
{
}

void sys_hal_aud_lvcmd_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_micbias1v_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_micbias_trim_set(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_audpll_en(uint32_t value)
{
	//not support
}

void sys_hal_aud_dacr_en(uint32_t value)
{
}

void sys_hal_aud_vdd1v_en(uint32_t value)
{
	//not support
}

void sys_hal_aud_vdd1v5_en(uint32_t value)
{
	//not support
}

void sys_hal_aud_mic2_en(uint32_t value)
{
	//not support
}

void sys_hal_aud_mic2_gain_set(uint32_t value)
{
	//not support
}

void sys_hal_aud_dacg_set(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_aud_aud_en(uint32_t value)
{
	//not support
}

void sys_hal_aud_rvcmd_en(uint32_t value)
{
	//not support
}

void sys_hal_dmic_clk_div_set(uint32_t value)
{
}

void sys_hal_aud_dac_dacmute_en(uint32_t value)
{
	//TODO
}

/**  Audio End  **/

/**  FFT Start  **/

void sys_hal_fft_disckg_set(uint32_t value)
{
}

void sys_hal_cpu_fft_int_en(uint32_t value)
{
}

/**  FFT End  **/

/**  I2S Start  **/
void sys_hal_i2s_select_clock(uint32_t value)
{
}

void sys_hal_i2s_clock_en(uint32_t value)
{
	sys_ll_set_cpu_device_clk_enable_i2s_cken(value);
}

void sys_hal_i2s1_clock_en(uint32_t value)
{
}

void sys_hal_i2s2_clock_en(uint32_t value)
{
}

void sys_hal_i2s_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_i2s0_int_en(value);
}

void sys_hal_i2s1_int_en(uint32_t value)
{
}

void sys_hal_i2s2_int_en(uint32_t value)
{
}

void sys_hal_i2s_disckg_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_apll_en(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_cb_manu_val_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_ana_reg11_vsel_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_apll_cal_val_set(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_apll_spi_trigger_set(uint32_t value)
{
	// NOT SUPPORT
}

void sys_hal_i2s0_ckdiv_set(uint32_t value)
{
	//NOT SUPPORT
}

void sys_hal_apll_config_set(uint32_t value)
{
	// NOT SUPPORT
}


/**  I2S End  **/


/** jpeg start **/
void sys_hal_set_jpeg_clk_sel(uint32_t value)
{
}

void sys_hal_set_clk_div_mode1_clkdiv_jpeg(uint32_t value)
{
}

void sys_hal_set_jpeg_disckg(uint32_t value)
{
}

void sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value)
{
}

void sys_hal_video_power_en(uint32_t value)
{
}

void sys_hal_set_auxs_cis_clk_sel(uint32_t value)
{
}

void sys_hal_set_auxs_cis_clk_div(uint32_t value)
{
}

void sys_hal_set_jpeg_clk_en(uint32_t value)
{
}

void sys_hal_set_cis_auxs_clk_en(uint32_t value)
{
}

/** jpeg end **/

/** h264 Start **/
void sys_hal_set_h264_clk_en(uint32_t value)
{
}

/** h264 End **/

/**  psram Start **/
void sys_hal_psram_volstage_sel(uint32_t enable)
{
	sys_ana_ll_set_ana_reg4_swb(enable); //1-1.8v, 0-3.0v
}

void sys_hal_psram_xtall_osc_enable(uint32_t enable)
{
	sys_ll_set_ana_reg5_en_xtall(enable);
}

void sys_hal_psram_doc_enable(uint32_t enable)
{
	// NOT SUPPORT
}

void sys_hal_psram_ldo_enable(uint32_t enable)
{
	sys_ana_ll_set_ana_reg4_en_psramldo(enable);
}

void sys_hal_psram_dpll_enable(uint32_t enable)
{
	sys_ll_set_ana_reg5_en_dpll(enable);
}

void sys_hal_psram_clk_sel(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_psram(value);
}

void sys_hal_psram_set_clkdiv(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(value);
}

void sys_hal_psram_psldo_vsel(uint32_t value)
{
	sys_ana_ll_set_ana_reg4_vpsramsel(value);// 1.8V + step 50mv
}

void sys_hal_psram_psldo_vset(uint32_t output_voltage, bool is_add_200mv)
{
	// NOT SUPPORT
	// psldo_swb=1, vpsramsel=0 - 1.80v
	// psldo_swb=1, vpsramsel=1 - 1.85v
	// psldo_swb=1, vpsramsel=2 - 1.90v
	// psldo_swb=1, vpsramsel=3 - 1.95v
	// psldo_swb=0, vpsramsel=* - 3.00v
	sys_ana_ll_set_ana_reg4_swb(output_voltage); //1-1.8v, 0-3.0v
	sys_ana_ll_set_ana_reg4_vpsramsel(is_add_200mv);// 1.8V + step 50mv
}

void sys_hal_psram_psram_disckg(uint32_t value)
{
	// not support
}

/**  psram End **/

/* REG_0x03:cpu_storage_connect_op_select->flash_sel:0: normal flash operation 1:flash download by spi,R/W,0x3[9]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_flash_sel(void)
{
    return sys_ll_get_cpu_storage_connect_op_select_flash_sel();
}

void sys_hal_set_cpu_storage_connect_op_select_flash_sel(uint32_t value)
{
    sys_ll_set_cpu_storage_connect_op_select_flash_sel(value);
}

/** Ethernet start **/
#ifdef CONFIG_ETH
void sys_hal_enable_eth_int(uint32_t value)
{
}
#endif
/** Ethernet End**/

/**  Misc Start **/
//Misc
/**  Misc End **/

void sys_hal_set_ana_trxt_tst_enable(uint32_t value)
{
}

void sys_hal_set_ana_scal_en(uint32_t value)
{
}

void sys_hal_set_ana_gadc_buf_ictrl(uint32_t value)
{
}

void sys_hal_set_ana_gadc_cmp_ictrl(uint32_t value)
{
}

void sys_hal_set_ana_pwd_gadc_buf(uint32_t value)
{
}

void sys_hal_set_ana_vref_sel(uint32_t value)
{
}

void sys_hal_set_ana_cb_cal_manu(uint32_t value)
{
    sys_ll_set_ana_reg5_bcal_en(value);
}

void sys_hal_set_ana_cb_cal_trig(uint32_t value)
{
    sys_ll_set_ana_reg5_bcal_start(value);
}

void sys_hal_set_ana_vlsel_ldodig(uint32_t value)
{
}

void sys_hal_set_ana_vhsel_ldodig(uint32_t value)
{
}

void sys_hal_set_sdio_clk_en(uint32_t value)
{
	sys_ll_set_cpu_device_clk_enable_sdio_cken(value);
}

void sys_hal_set_cpu0_sdio_int_en(uint32_t value)
{
	sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(value);
}

void sys_hal_set_sdio_clk_div(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(value);
}

uint32_t sys_hal_get_sdio_clk_div()
{
	return sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio();
}

void sys_hal_set_sdio_clk_sel(uint32_t value)
{
	sys_ll_set_cpu_clk_div_mode2_cksel_sdio(value);
}

uint32_t sys_hal_get_sdio_clk_sel()
{
	return sys_ll_get_cpu_clk_div_mode2_cksel_sdio();
}


void sys_hal_set_ana_vctrl_sysldo(uint32_t value)
{
}

void sys_hal_set_yuv_buf_clock_en(uint32_t value)
{
}

void sys_hal_set_h264_clock_en(uint32_t value)
{
}

void sys_hal_nmi_wdt_set_clk_div(uint32_t value)
{
	sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(value);
}

__IRAM_SEC uint32_t sys_hal_nmi_wdt_get_clk_div(void)
{
	return sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt();
}


void sys_hal_set_ana_cb_cal_manu_val(uint32_t value)
{
    sys_ll_set_ana_reg5_vbias(value);
}

__IRAM_SEC void sys_hal_set_ana_reg11_apfms(uint32_t value) {
	// NOT SUPPORT
}

__IRAM_SEC void sys_hal_set_ana_reg12_dpfms(uint32_t value) {
	// NOT SUPPORT
}

#if CONFIG_ANA_RTC || CONFIG_ANA_GPIO
void sys_hal_set_ana_reg8_spi_latch1v(uint32_t value)
{
	sys_ll_set_ana_reg8_spi_latch1v(value);
}

void sys_hal_set_ana_reg5_adc_div(uint32_t value)
{
	// sys_ll_set_ana_reg5_adc_div(value);
}

void sys_hal_set_ana_reg5_pwd_rosc_spi(uint32_t value)
{
	sys_ll_set_ana_reg5_pwd_rosc_spi(value);
}
void sys_hal_set_ana_reg5_rosc_disable(uint32_t value)
{
	sys_ll_set_ana_reg5_rosc_disable(value);
}

void sys_hal_set_ana_reg7_timer_wkrstn(uint32_t value)
{
	sys_ll_set_ana_reg7_timer_wkrstn(value);
}

void sys_hal_set_ana_reg8_rst_wks1v(uint32_t value)
{
	sys_ll_set_ana_reg8_rst_wks1v(value);
}

__IRAM_SEC void sys_hal_set_ana_reg8_lvsleep_wkrst(uint32_t value)
{
	sys_ll_set_ana_reg8_lvsleep_wkrst(value);
}

__IRAM_SEC void sys_hal_set_ana_reg8_gpiowk_rstn(uint32_t value)
{
	sys_ll_set_ana_reg8_gpiowk_rstn(value);
}

__IRAM_SEC void sys_hal_set_ana_reg8_rtcwk_rstn(uint32_t value)
{
	sys_ll_set_ana_reg8_rtcwk_rstn(value);
}
void sys_hal_set_ana_reg8_ensfsdd(uint32_t value)
{
	sys_ll_set_ana_reg8_ensfsdd(value);
}

void sys_hal_set_ana_reg8_vlden(uint32_t value)
{
	sys_ll_set_ana_reg8_vlden(value);
}

void sys_hal_set_ana_reg8_pwdovp1v(uint32_t value)
{
	// sys_ll_set_ana_reg8_pwdovp1v(value);
}
void sys_hal_set_ana_reg9_spi_timerwken(uint32_t value)
{
	sys_ll_set_ana_reg9_spi_timerwken(value);
}

void sys_hal_set_ana_reg7_clk_sel(uint32_t value)
{
	sys_ll_set_ana_reg7_clk_sel(value);
}

void sys_hal_set_ana_reg9_spi_byp32pwd(uint32_t value)
{
	sys_ll_set_ana_reg9_spi_byp32pwd(value);
}

uint32_t sys_hal_get_ana_reg11_gpiowk(void)
{
	return sys_ll_get_ana_reg11_gpiowk();
}

void sys_hal_set_ana_reg11_gpiowk(uint32_t value)
{
	sys_ll_set_ana_reg11_gpiowk(value);
}

uint32_t sys_hal_get_ana_reg11_rtcsel(void)
{
	return sys_ll_get_ana_reg11_rtcsel();
}

void sys_hal_set_ana_reg11_rtcsel(uint32_t value)
{
	sys_ll_set_ana_reg11_rtcsel(value);
}

uint32_t sys_hal_get_ana_reg11_timersel(void)
{
	return sys_ll_get_ana_reg11_timersel();
}

void sys_hal_set_ana_reg11_timersel(uint32_t value)
{
	sys_ll_set_ana_reg11_timersel(value);
}

uint32_t sys_hal_get_ana_reg12_timersel(void)
{
	return sys_ll_get_ana_reg12_timersel();
}

void sys_hal_set_ana_reg12_timersel(uint32_t value)
{
	sys_ll_set_ana_reg12_timersel(value);
}

uint32_t sys_hal_get_ana_reg13_rtcsel(void)
{
	return sys_ll_get_ana_reg13_rtcsel();
}

void sys_hal_set_ana_reg13_rtcsel(uint32_t value)
{
	sys_ll_set_ana_reg13_rtcsel(value);
}
#endif

static void sys_hal_delay(volatile uint32_t times)
{
        while(times--);
}

void sys_hal_adjust_dpll(void)
{
    uint32_t unlock, unlockL = 0, unlockH = 0;
    int32_t dpll_band, count;

    unlock = aon_pmu_hal_get_dpll_unlock(&unlockL, &unlockH);
    if (!unlock) {
        return;
    }
    dpll_band = (int32_t)sys_ll_get_ana_reg1_bandmanual();
    for (count = 7; count > 0; count--) {
        if (unlockL) {
            dpll_band++;
        } else {
            dpll_band--;
        }
        if (dpll_band < 0) {
            break;
        } else if (dpll_band > 0x7F) {
            break;
        }
        sys_ll_set_ana_reg1_bandmanual(dpll_band);
    }
}

uint32_t sys_hal_cali_dpll(uint32_t first_time)
{
    uint32_t int_mask = sys_hal_int_group2_disable(DPLL_UNLOCK_INTERRUPT_CTRL_BIT);

    //liupeng20250519: disable unlock detecter and delay 20us when cali dpll
    sys_ll_set_ana_reg0_cben(1);
    sys_ll_set_ana_reg1_manual(0);
    sys_hal_delay(340);

    sys_hal_cali_dpll_spi_detect_disable();

    if (first_time)
    {
        sys_hal_delay(3400);
    }
    else
    {
        sys_hal_delay(340);
    }

	sys_hal_cali_dpll_spi_detect_enable();

    if (first_time)
    {
        sys_hal_delay(3400);
    }
    else
    {
        sys_hal_delay(340);
    }

    //ronghui20250604: toggle twice to avoid wrong value after 0x40<29:28>=1
    sys_hal_cali_dpll_spi_detect_disable();

    if (first_time)
    {
        sys_hal_delay(3400);
    }
    else
    {
        sys_hal_delay(340);
    }

	sys_hal_cali_dpll_spi_detect_enable();

    if (first_time)
    {
        sys_hal_delay(3400);
    }
    else
    {
        sys_hal_delay(340);
    }

    sys_ll_set_ana_reg1_bandmanual(aon_pmu_hal_get_dpll_band());
    sys_ll_set_ana_reg1_manual(1);
    sys_ll_set_ana_reg0_cben(0);

    if (int_mask & DPLL_UNLOCK_INTERRUPT_CTRL_BIT) {
        sys_hal_int_group2_enable(DPLL_UNLOCK_INTERRUPT_CTRL_BIT);
    }

    return BK_OK;
}

lpo_src_e bk_clk_32k_customer_config_get(void)
{
	#if CONFIG_EXTERN_32K
		return LPO_SRC_X32K;
	#elif CONFIG_LPO_SRC_26M32K
		return LPO_SRC_DIVD;
	#else
		return LPO_SRC_ROSC;
	#endif //CONFIG_EXTERN_32K
}


void sys_hal_dpll_cpu_flash_time_early_init(uint32_t chip_id)
{
	uint32_t coresel = 0;
	uint32_t corediv = 0;
	//zhangheng20231018: calibrate dpll before use
	/*0x2:320M, 0X3:480M*/

	sys_hal_cali_dpll(1);

	timer_hal_us_init();

	/*increase the anaspi_freq, current it only use for lpo: external 32k, 26m/32k*/
	if(bk_clk_32k_customer_config_get() != LPO_SRC_ROSC)
	{
		sys_ll_set_cpu_anaspi_freq_anaspi_freq(0x2);//ana_sck = bus_clk/((divd+1)*2))
	}
}

#if 1
static void sys_hal_pwd_rosc()
{
	/*when the lpo src select external 32k or 26m/32k, it disable the rosc;because make sure write the analog register successfully*/
	if(bk_clk_32k_customer_config_get() != LPO_SRC_ROSC)
	{
		if(aon_pmu_ll_get_r41_lpo_config() == LPO_SRC_ROSC)
		{
			aon_pmu_ll_set_r41_lpo_config(PM_LPO_SRC_DIVD);
		}

		if(sys_ll_get_ana_reg5_pwd_rosc_spi() != 0x1)
		{
			sys_ll_set_ana_reg5_pwd_rosc_spi(0x1);
		}
	}
	else
	{
		if(sys_ll_get_ana_reg5_pwd_rosc_spi() != 0x0)
		{
			sys_ll_set_ana_reg5_pwd_rosc_spi(0x0);

			sys_hal_delay(500);
		}
	}
}
#endif

//TODO the module owner can put the sys init to better place
void sys_hal_early_init(void)
{
	uint32_t chip_id = aon_pmu_hal_get_chipid();

	uint32_t val = sys_hal_analog_get(ANALOG_REG5);
	// power up dpll first
	val |= (0x1 << 5);
	sys_hal_analog_set(ANALOG_REG5,val);

	sys_hal_analog_set(ANALOG_REG0, 0x19219780);//liupeng202400807:<9:7>=7;zhiyin20250429<11>=0 enable dpll_unlock flag;zhiyin20250604<29:28>=1
	sys_hal_analog_set(ANALOG_REG1, 0xC02EFA00);
	sys_hal_analog_set(ANALOG_REG2, 0x0600D436);
	sys_hal_analog_set(ANALOG_REG3, 0xFDFC0BC8);//tenglong20240806:high psrr as default, low when sleep;ronghui20250520<27>=1 avoid dpll unlock detect issue when high temp
	sys_hal_analog_set(ANALOG_REG4, 0x40000000);
	sys_hal_analog_set(ANALOG_REG5, 0x84072160);
	sys_hal_analog_set(ANALOG_REG6, 0x80088100);//liupeng20250427
	sys_hal_analog_set(ANALOG_REG7, 0x57E697C6);//tenglong20250430:ldo current limit for EVM;//shuguang20250512:vio=3.3V;xinyuan20250611:<9:6>=F for better EVM
	sys_hal_analog_set(ANALOG_REG8, 0xF97C72E4);//tenglong20250519:<31:30>=3(0.9V), same with BK7236_SYS_ANA_4D<13:12>

	/**
	 * attention:
	 * SPI latch must be enable before ana_reg[8~13] modification
	 * and don't forget disable it after that.
	 */
	sys_ll_set_ana_reg8_spi_latch1v(1);
	sys_hal_analog_set(ANALOG_REG9, 0xB7153327);//tenglong20240805:2M buck for EVM
	sys_hal_analog_set(ANALOG_REG10, 0xF7EEB89A);
	sys_hal_analog_set(ANALOG_REG11, 0x00000000);
	sys_hal_analog_set(ANALOG_REG12, 0x00100000);
	sys_hal_analog_set(ANALOG_REG13, 0x00080000);
	sys_hal_analog_set(ANALOG_REG14, 0x00A80008);
	sys_hal_analog_set(ANALOG_REG15, 0x528809F6);
	sys_ll_set_ana_reg8_spi_latch1v(0);

	/*early init cpu flash time*/
	sys_hal_dpll_cpu_flash_time_early_init(chip_id);
}

void sys_hal_set_base_addr(uint32_t addr)
{
	s_sys_hal.hw = (sys_hw_t *)addr;
}

void sys_hal_switch_freq(uint32_t cksel_core, uint32_t ckdiv_core, uint32_t ckdiv_cpu0)
{

	uint32_t clk_param  = 0;
	clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
	if(((clk_param >> 0x4)&0x3) > cksel_core)//when it from the higher frequency to lower frequency
	{
		/*1.core clk select*/
		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x3 << 4);
		clk_param |=  cksel_core << 4;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config bus and core clk div*/
		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0xF << 0);
		clk_param |=  ckdiv_core << 0;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*3.config cpu clk div*/
		sys_hal_cpu_clk_div_set(0,ckdiv_cpu0);

	}
	else//when it from the lower frequency to higher frequency
	{
		/*1.config bus and core clk div*/
		if(ckdiv_core == 0)
		{
			sys_hal_cpu_clk_div_set(0,ckdiv_cpu0);//avoid the bus freq > 240m
		}

		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0xF << 0);
		clk_param |=  ckdiv_core << 0;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config cpu clk div*/
		if(ckdiv_core != 0)
		{
			sys_hal_cpu_clk_div_set(0,ckdiv_cpu0);
		}

		/*3.core clk select*/

		clk_param = 0;
		clk_param = sys_hal_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x3 << 4);
		clk_param |=  cksel_core << 4;
		sys_hal_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);
	}

}


/**  SCR End  */
