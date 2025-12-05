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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"
#include "bk_misc.h"
#include "aon_pmu_driver.h"

#define SYS_DRV_DELAY_TIME_10US	              120
#define SYS_DRV_DELAY_TIME_200US	          3400

/**  WIFI Start **/
//WIFI

void sys_drv_delay10us(void)
{
	volatile UINT32 i = 0;

	for (i = 0; i < SYS_DRV_DELAY_TIME_10US; i ++)
		;
}

void sys_drv_delay200us(void)
{
	volatile UINT32 i = 0;

	for (i = 0; i < SYS_DRV_DELAY_TIME_200US; i ++)
		;
}

void sys_drv_ps_dpll_delay(UINT32 time)
{
	volatile UINT32 i = 0;

	for (i = 0; i < time; i ++)
		;
}

uint32_t sys_drv_cali_dpll(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

    sys_hal_cali_dpll(param);

	sys_drv_exit_critical(int_level);

    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_bias_reg_set(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

	sys_hal_bias_reg_set(param);
    sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_bias_reg_clean(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

	sys_hal_bias_reg_clean(param);
    sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_bias_reg_read(void)
{
	uint32 ret;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_bias_reg_read();
    sys_drv_exit_critical(int_level);
    return ret;
}

uint32_t sys_drv_bias_reg_write(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

	sys_hal_bias_reg_write(param);
    sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_cali_bias(uint32_t update)
{
#define CFG_ROSC           0
    uint32 bias_cal;

#if !CFG_ROSC
    uint32_t pwd_rosc = sys_hal_analog_get(ANALOG_REG5) & (1 << 14);
    if (pwd_rosc != 0x0)
    {
		#if (defined(CONFIG_SOC_BK7236N) || defined(CONFIG_SOC_BK7239))
        sys_hal_set_ana_reg5_pwd_rosc_spi(0);
		#endif
        delay_us(2000);
    }
#endif

    sys_drv_set_ana_cb_cal_manu(1);
    sys_drv_set_ana_cb_cal_trig(0);
    sys_drv_set_ana_cb_cal_trig(1);
    delay_us(30);
    sys_drv_set_ana_cb_cal_trig(0);
    delay_us(3000);	//3ms as tenglong designed

    bias_cal = aon_pmu_drv_bias_cal_get();
    if (update) {
        sys_drv_set_ana_cb_cal_manu_val(bias_cal);
    }
    sys_drv_set_ana_cb_cal_manu(0);

#if !CFG_ROSC
    if (pwd_rosc != 0x0)
    {
		#if (defined(CONFIG_SOC_BK7236N) || defined(CONFIG_SOC_BK7239))
        sys_hal_set_ana_reg5_pwd_rosc_spi(1);
		#endif
    }
#endif

    return bias_cal;
}

uint32_t sys_drv_analog_reg4_bits_or(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_drv_exit_critical(int_level);
    //analog reg write only?
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_ctrl9_real_set(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_drv_exit_critical(int_level);
    //analog reg write only?
    return SYS_DRV_SUCCESS;
}


uint32_t sys_drv_analog_set_xtalh_ctune(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_xtalh_ctune(param);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_get_xtalh_ctune(void)
{
    uint32_t int_level = sys_drv_enter_critical();
    uint32 reg_value;
    reg_value = sys_hal_get_xtalh_ctune();
	sys_drv_exit_critical(int_level);
    return reg_value;
}
void sys_drv_analog_set(analog_reg_t reg, uint32_t value)
{
    sys_hal_analog_set(reg,value);
}
uint32_t sys_drv_analog_get(analog_reg_t reg)
{
    return sys_hal_analog_get(reg);
}
uint32_t sys_drv_analog_reg1_set(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_reg1_value(param);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg2_set(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_reg2_value(param);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg3_set(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_reg3_value(param);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg4_set(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_reg4_value(param);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg12_set(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_ana_reg12_value(param);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg13_set(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_ana_reg13_value(param);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg14_set(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_ana_reg14_value(param);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg15_set(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_ana_reg15_value(param);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg16_set(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_ana_reg16_value(param);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg17_set(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_ana_reg17_value(param);
	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg6_set(uint32_t param)
{
    //analog reg write only?
	uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_ana_reg6_value(param);
    sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg7_set(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_ana_reg7_value(param);
    sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg1_get(void)
{
	uint32 ret;
    uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_analog_reg1_get();
    sys_drv_exit_critical(int_level);
    return ret;
}
uint32_t sys_drv_analog_reg2_get(void)
{
	uint32 ret;
    uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_analog_reg2_get();
    sys_drv_exit_critical(int_level);
    return ret;
}
uint32_t sys_drv_analog_reg4_get(void)
{
	uint32 ret;
    uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_analog_reg4_get();
    sys_drv_exit_critical(int_level);
	return ret;
}


uint32_t sys_drv_analog_reg6_get(void)
{
	uint32 ret;
    uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_analog_reg6_get();
    sys_drv_exit_critical(int_level);
	return ret;
}


uint32_t sys_drv_analog_reg7_get(void)
{
	uint32 ret;
    uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_analog_reg7_get();
	sys_drv_exit_critical(int_level);
	return ret;
}

uint32_t sys_drv_cali_bgcalm(void)
{
    uint32_t ret;
    uint32_t int_level = sys_drv_enter_critical();

    ret = sys_hal_cali_bgcalm();
    sys_drv_exit_critical(int_level);

    return ret;
}

uint32_t sys_drv_get_bgcalm(void)
{
    uint32_t ret;
	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_get_bgcalm();
	sys_drv_exit_critical(int_level);
    return ret;
}

uint32_t sys_drv_set_bgcalm(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_bgcalm(param);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_dpll_for_i2s(void)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_audioen(1);
	sys_hal_set_dpll_div_cksel(1);
	sys_hal_set_dpll_reset(1);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_gadc_ten(uint32_t param)
{
    uint32_t int_level = sys_drv_enter_critical();

    sys_hal_set_gadc_ten(param);
	sys_drv_exit_critical(int_level);
    return SYS_DRV_SUCCESS;
}


//Yantao Add Start

//Unused
//CMD_SCTRL_RESET_SET, CMD_SCTRL_RESET_CLR
uint32_t sys_drv_reset_ctrl(uint32_t value)
{
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MODEM_CORE_RESET
uint32_t sys_drv_modem_core_reset(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_modem_core_reset();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MPIF_CLK_INVERT
uint32_t sys_drv_mpif_invert(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_mpif_invert();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MODEM_SUBCHIP_RESET
uint32_t sys_drv_modem_subsys_reset(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_modem_subsys_reset();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_MAC_SUBSYS_RESET
uint32_t sys_drv_mac_subsys_reset(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_mac_subsys_reset();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_USB_SUBSYS_RESET
uint32_t sys_drv_usb_subsys_reset(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_usb_subsys_reset();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_DSP_SUBSYS_RESET
uint32_t sys_drv_dsp_subsys_reset(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_dsp_subsys_reset();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MAC_POWERDOWN, CMD_SCTRL_MAC_POWERUP
uint32_t sys_drv_mac_power_ctrl(bool power_up)
{
	uint32_t int_level = sys_drv_enter_critical();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_mac_power_ctrl(power_up);

	if(!ret)
		ret = sys_amp_res_release();

	sys_drv_exit_critical(int_level);
	return ret;
}

//CMD_SCTRL_MODEM_POWERDOWN, CMD_SCTRL_MODEM_POWERUP
uint32_t sys_drv_modem_power_ctrl(bool power_up)
{
	uint32_t int_level = sys_drv_enter_critical();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_modem_clk_ctrl(power_up);

	if(!ret)
		ret = sys_amp_res_release();

	sys_drv_exit_critical(int_level);
	return ret;
}

//CMD_BLE_RF_PTA_EN, CMD_BLE_RF_PTA_DIS
uint32_t sys_drv_pta_ctrl(bool pta_en)
{
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_pta_ctrl(pta_en);

	sys_drv_exit_critical(int_level);
#endif
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MODEM_AHB_CLOCK_DISABLE, CMD_SCTRL_MODEM_AHB_CLOCK_ENABLE
uint32_t sys_drv_modem_bus_clk_ctrl(bool clk_en)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_modem_bus_clk_ctrl(clk_en);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_MODEM_CLOCK480M_DISABLE, CMD_SCTRL_MODEM_CLOCK480M_ENABLE
uint32_t sys_drv_modem_clk_ctrl(bool clk_en)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_modem_clk_ctrl(clk_en);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MAC_AHB_CLOCK_DISABLE, CMD_SCTRL_MAC_AHB_CLOCK_ENABLE
uint32_t sys_drv_mac_bus_clk_ctrl(bool clk_en)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_mac_bus_clk_ctrl(clk_en);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_MAC_CLOCK480M_DISABLE,CMD_SCTRL_MAC_CLOCK480M_ENABLE
uint32_t sys_drv_mac_clk_ctrl(bool clk_en)
{
	uint32_t int_level = sys_drv_enter_critical();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_mac_clk_ctrl(clk_en);

	if(!ret)
		ret = sys_amp_res_release();

	sys_drv_exit_critical(int_level);
	return ret;
}

//CMD_SCTRL_SET_VDD_VALUE
uint32_t sys_drv_set_vdd_value(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_set_vdd_value(param);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_GET_VDD_VALUE
uint32_t sys_drv_get_vdd_value(void)
{
	uint32 ret;

	uint32_t int_level = sys_drv_enter_critical();

	ret = sys_hal_get_vdd_value();

	sys_drv_exit_critical(int_level);
	return ret;
}

//CMD_SCTRL_BLOCK_EN_MUX_SET
uint32_t sys_drv_block_en_mux_set(uint32_t param)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_block_en_mux_set(param);

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_gen_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_mac_gen_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_prot_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();


	sys_hal_enable_mac_prot_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_tx_trigger_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_mac_tx_trigger_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_rx_trigger_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_mac_rx_trigger_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_txrx_misc_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_mac_txrx_misc_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_txrx_timer_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_mac_txrx_timer_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t  sys_drv_enable_modem_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_modem_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t  sys_drv_enable_modem_rc_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_modem_rc_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t  sys_drv_enable_hsu_int(void)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_enable_hsu_int();

	sys_drv_exit_critical(int_level);
	return SYS_DRV_SUCCESS;
}

//Yantao Add End
/**  WIFI End **/

