#ifndef _SYS_DRV_H_
#define _SYS_DRV_H_

#include <common/sys_config.h>
#include <common/bk_include.h>
#include <soc/soc.h>
#include <components/log.h>
#include "sys_types.h"
#include "sys_hal.h"
#include <modules/pm.h>

#define SYS_DRV_DEBUG   0

#if SYS_DRV_DEBUG
#define SYS_DRV_TAG "sys_drv"
#define SYS_DRV_LOGI(...) BK_LOGI(SYS_DRV_TAG, ##__VA_ARGS__)
#define SYS_DRV_LOGW(...) BK_LOGW(SYS_DRV_TAG, ##__VA_ARGS__)
#define SYS_DRV_LOGE(...) BK_LOGE(SYS_DRV_TAG, ##__VA_ARGS__)
#define SYS_DRV_LOGD(...) BK_LOGD(SYS_DRV_TAG, ##__VA_ARGS__)
#else
#define SYS_DRV_LOGI      os_null_printf
#endif

#if SYS_DRV_DEBUG
#define SYS_DRV_PRT      BK_LOGI
#else
#define SYS_DRV_PRT      os_null_printf
#endif

#define SYSTEM_TAG "system"
#define SYSTEM_LOGI(...) BK_LOGI(SYSTEM_TAG, ##__VA_ARGS__)
#define SYSTEM_LOGW(...) BK_LOGW(SYSTEM_TAG, ##__VA_ARGS__)
#define SYSTEM_LOGE(...) BK_LOGE(SYSTEM_TAG, ##__VA_ARGS__)
#define SYSTEM_LOGD(...) BK_LOGD(SYSTEM_TAG, ##__VA_ARGS__)

#define SYS_DRV_FAILURE        ((UINT32)-1)
#define SYS_DRV_SUCCESS        (0)

#define SYS_DRV_PWR_UP        (1)
#define SYS_DRV_PWR_DOWN      (0)

#define SYS_DRV_CLK_ON        (1)
#define SYS_DRV_CLK_OFF       (0)

static inline uint32_t sys_drv_enter_critical(void)
{
	return rtos_enter_critical();
}

static inline void sys_drv_exit_critical(uint32_t flags)
{
	rtos_exit_critical(flags);
}

/**  Platform Start **/
//Platform

/* Platform Misc Start **/
void sys_drv_init(void);
/* Platform Misc End **/


#if defined(CONFIG_USB)    //功能宏开关
void sys_drv_usb_power_down(void);

void sys_drv_usb_clock_ctrl(bool ctrl, void *arg);

uint32_t sys_drv_usb_analog_phy_en(bool ctrl, void *arg);

uint32_t sys_drv_usb_analog_speed_en(bool ctrl, void *arg);

uint32_t sys_drv_usb_analog_ckmcu_en(bool ctrl, void *arg);

uint32_t sys_drv_usb_analog_dp_capability(uint8_t value);

uint32_t sys_drv_usb_analog_dn_capability(uint8_t value);

uint32_t sys_drv_usb_analog_deepsleep_en(bool ctrl);

void sys_drv_usb_charge_ctrl(bool ctrl, void *arg);

void sys_drv_usb_charge_cal(sys_drv_charge_step_t step, void *arg);
#endif	//CONFIG_USB

/* Platform UART Start **/
void sys_drv_uart_select_clock(uart_id_t id, uart_src_clk_t mode);
/* Platform UART End **/

/* Platform I2C Start **/
void sys_drv_i2c_select_clock(i2c_id_t id, i2c_src_clk_t mode);
/* Platform I2C End **/

void sys_drv_pwm_set_clock(uint32_t mode, uint32_t param);

void sys_drv_pwm_select_clock(sys_sel_pwm_t num, pwm_src_clk_t mode);

void sys_drv_timer_select_clock(sys_sel_timer_t num, timer_src_clk_t mode);

void sys_drv_flash_set_dco(void);

void sys_drv_flash_set_dpll(void);

void sys_drv_flash_cksel(uint32_t value);

void sys_drv_flash_set_clk_div(uint32_t value);

uint32_t sys_drv_flash_get_clk_sel(void);

uint32_t sys_drv_flash_get_clk_div(void);

void sys_drv_set_qspi_vddram_voltage(uint32_t param);

void sys_drv_set_qspi_io_voltage(uint32_t param);

void sys_drv_qspi_clk_sel(uint32_t id, uint32_t param);

void sys_drv_qspi_set_src_clk_div(uint32_t id, uint32_t value);

void sys_drv_nmi_wdt_set_clk_div(uint32_t value);

uint32_t sys_drv_nmi_wdt_get_clk_div(void);

void sys_drv_trng_disckg_set(uint32_t value);

#if defined(CONFIG_SDIO_V2P0)
void sys_driver_set_sdio_clk_en(uint32_t value);

void sys_driver_set_cpu0_sdio_int_en(uint32_t value);
void sys_driver_set_cpu1_sdio_int_en(uint32_t value);
void sys_driver_set_sdio_clk_div(uint32_t value);
uint32_t sys_driver_get_sdio_clk_div();
void sys_driver_set_sdio_clk_sel(uint32_t value);
uint32_t sys_driver_get_sdio_clk_sel();
#endif

void sys_drv_enter_deep_sleep(void * param);

void sys_drv_enter_normal_sleep(uint32_t peri_clk);

void sys_drv_enter_normal_wakeup(void);
void sys_drv_enter_low_voltage(void);
void sys_drv_enter_cpu_wfi(void);

/*for low power  function start*/
void sys_drv_module_power_ctrl(power_module_name_t module,power_module_state_t power_state);
int32 sys_drv_module_power_state_get(power_module_name_t module);
void sys_drv_module_RF_power_ctrl (module_name_t module,power_module_state_t power_state);

void sys_drv_cpu0_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state);
void sys_drv_cpu1_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state);
void sys_drv_set_cpu1_reset(uint32_t reset_value);
void sys_drv_set_cpu1_boot_address_offset(uint32_t address_offset);
void sys_drv_set_cpu1_pwr_dw(uint32_t is_pwr_down);

#if CONFIG_CPU_CNT > 2
void sys_drv_set_cpu2_boot_address_offset(uint32_t address_offset);
void sys_drv_set_cpu2_pwr_dw(uint32_t is_pwr_down);
void sys_drv_set_cpu2_reset(uint32_t reset_value);
#endif

void sys_drv_wakeup_source_gpio_clear(void);
void sys_drv_wakeup_source_rtc_clear(void);
void sys_drv_enable_mac_wakeup_source(void);
void sys_drv_enable_bt_wakeup_source(void);

void sys_drv_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value);
uint32_t sys_drv_all_modules_clk_div_get(clk_div_reg_e reg);
void sys_drv_wakeup_interrupt_clear(wakeup_source_t interrupt_source);
void sys_drv_wakeup_interrupt_set(wakeup_source_t interrupt_source);
void sys_drv_touch_wakeup_enable(uint8_t index);
void sys_drv_usb_wakeup_enable(uint8_t index);
void sys_drv_rtc_ana_wakeup_enable(uint32_t period);
void sys_drv_gpio_ana_wakeup_enable(uint32_t count, uint32_t index, uint32_t type);
void sys_drv_cpu_clk_div_set(uint32_t core_index, uint32_t value);
uint32_t sys_drv_cpu_clk_div_get(uint32_t core_index);
void sys_drv_low_power_hardware_init(void);
int32 sys_drv_lp_vol_set(uint32_t value);
uint32_t sys_drv_lp_vol_get(void);
int32 sys_drv_rf_tx_vol_set(uint32_t value);
uint32_t sys_drv_rf_tx_vol_get(void);
int32 sys_drv_rf_rx_vol_set(uint32_t value);
uint32_t sys_drv_rf_rx_vol_get(void);
int32 sys_drv_rosc_calibration(uint32_t rosc_cali_mode, uint32_t cali_interval);
int32 sys_drv_bandgap_cali_set(uint32_t value);//increase or decrease the dvdddig voltage
int sys_drv_rosc_test_mode(bool enabled);
uint32_t sys_drv_bandgap_cali_get(void);
bk_err_t sys_drv_switch_cpu_bus_freq(pm_cpu_freq_e cpu_bus_freq);
bk_err_t sys_drv_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core,uint32_t ckdiv_bus, uint32_t ckdiv_cpu0,uint32_t ckdiv_cpu1);

/*for  low power  function end*/
uint32 sys_drv_get_device_id(void); // CMD_GET_DEVICE_ID
uint32 sys_drv_get_chip_id(void);   // CMD_GET_CHIP_ID

int32 sys_drv_int_disable(uint32 param);
int32 sys_drv_int_enable(uint32 param);
int32 sys_drv_int_group2_disable(uint32 param);
int32 sys_drv_int_group2_enable(uint32 param);

#if defined(CONFIG_SOC_SMP)
int32 sys_drv_core_intr_group1_disable(uint32 core_id, uint32 param);
int32 sys_drv_core_intr_group1_enable(uint32 core_id, uint32 param);
int32 sys_drv_core_intr_group2_disable(uint32 core_id, uint32 param);
int32 sys_drv_core_intr_group2_enable(uint32 core_id, uint32 param);
#endif

int32 sys_drv_fiq_disable(uint32 param);
int32 sys_drv_fiq_enable(uint32 param);
int32 sys_drv_global_int_disable(uint32 param);
int32 sys_drv_global_int_enable(uint32 param);
uint32 sys_drv_get_int_source_status(void);
uint32_t sys_drv_get_cpu0_gpio_int_st(void);
int32 sys_drv_set_int_source_status(uint32 param);
uint32 sys_drv_get_fiq_reg_status(void);
uint32 sys_drv_set_fiq_reg_status(uint32 param);
uint32 sys_drv_get_intr_raw_status(void);
uint32 sys_drv_set_intr_raw_status(uint32 param);
int32 sys_drv_set_jtag_mode(uint32 param);
uint32 sys_drv_get_jtag_mode(void);

/*clock power control start*/
void sys_drv_dev_clk_pwr_up(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up);
void sys_drv_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel);
//DCO divider is valid for all of the peri-devices.
void sys_drv_set_dco_div(dev_clk_dco_div_t div);
//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_drv_get_dco_div(void);

/*clock power control end*/

/*wake up control start*/
void sys_drv_arm_wakeup_enable(uint32_t param);
void sys_drv_arm_wakeup_disable(uint32_t param);
uint32_t sys_drv_get_arm_wakeup(void);
/*wake up control end*/

void sys_drv_sadc_int_enable(void);
uint32_t sys_drv_sadc_get_int_en(void);
void sys_drv_sadc_int_disable(void);
void sys_drv_sadc_pwr_up(void);
void sys_drv_sadc_pwr_down(void);
void sys_drv_en_tempdet(uint32_t value);
void sys_drv_set_temp_mode(bool high_volt_mode);

/**  Platform End **/


/**  BT Start **/
//BT
uint32_t sys_drv_mclk_mux_get(void);
uint32_t sys_drv_mclk_div_get(void);
void sys_drv_mclk_select(uint32_t value);
void sys_drv_mclk_div_set(uint32_t value);

void sys_drv_bt_power_ctrl(bool power_up);

void sys_drv_bt_clock_ctrl(bool en);
void sys_drv_xvr_clock_ctrl(bool en);

uint32_t sys_drv_interrupt_status_get(void);
void sys_drv_interrupt_status_set(uint32_t value);

void sys_drv_btdm_interrupt_ctrl(bool en);
void sys_drv_ble_interrupt_ctrl(bool en);
void sys_drv_bt_interrupt_ctrl(bool en);

void sys_drv_bt_rf_ctrl(bool en);
uint32_t sys_drv_bt_rf_status_get(void);

void sys_drv_bt_sleep_exit_ctrl(bool en);

/**  BT End **/




/**  Audio Start **/
//Audio
/**  Audio End **/


/**  Video Start **/
/**
  * @brief	lcd_disp  system config
  * param1: clk source sel 0:clk_320M      1:clk_480M,
  * param2: clk_div  F/(1+clkdiv_disp_l+clkdiv_disp_h*2)
  * param3: int_en eanble lcd cpu int
  * param4: clk_always_on, BUS_CLK ENABLE,0: bus clock open when module is select,1:bus clock always open,  0 by defult
  * return none
  */
uint32_t sys_drv_lcd_set(uint8_t clk_src_sel, uint8_t clk_div_l, uint8_t clk_div_h,uint8_t clk_always_on);

/**
  * @brief	lcd clk close and int disable, reg value recover default.
  * return none
  */
uint32_t  sys_drv_lcd_close(void);

/**
  * @brief	dma2d system config
  * param1: clk source sel 0:clk_320M	   1:clk_480M,
  * param2: clk_always_on  ENABLE,0: bus clock open when module is select,1:bus clock always open
  * return none
  */
uint32_t sys_drv_dma2d_set(uint8_t clk_always_on);

/**
  * @brief	jpeg dec system config
  * param1: clk_always_on  ENABLE,0: bus clock open when module is select,1:bus clock always open
  * return none
  */
uint32_t sys_drv_set_jpeg_dec_disckg(uint32_t value);

/**  Video End **/




/**  WIFI Start **/
//WIFI

//Yantao Add Start
//CMD_SCTRL_RESET_SET, CMD_SCTRL_RESET_CLR
uint32_t sys_drv_reset_ctrl(uint32_t value);
//CMD_SCTRL_MODEM_CORE_RESET
uint32_t sys_drv_modem_core_reset(void);
//CMD_SCTRL_MPIF_CLK_INVERT
uint32_t sys_drv_mpif_invert(void);
//CMD_SCTRL_MODEM_SUBCHIP_RESET
uint32_t sys_drv_modem_subsys_reset(void);
//CMD_SCTRL_MAC_SUBSYS_RESET
uint32_t sys_drv_mac_subsys_reset(void);
//CMD_SCTRL_USB_SUBSYS_RESET
uint32_t sys_drv_usb_subsys_reset(void);
//CMD_SCTRL_DSP_SUBSYS_RESET
uint32_t sys_drv_dsp_subsys_reset(void);
//CMD_SCTRL_MAC_POWERDOWN, CMD_SCTRL_MAC_POWERUP
uint32_t sys_drv_mac_power_ctrl(bool power_on);
//CMD_SCTRL_MODEM_POWERDOWN, CMD_SCTRL_MODEM_POWERUP
uint32_t sys_drv_modem_power_ctrl(bool power_on);\
//CMD_BLE_RF_PTA_EN, CMD_BLE_RF_PTA_DIS
uint32_t sys_drv_pta_ctrl(bool pta_en);
//CMD_SCTRL_MODEM_AHB_CLOCK_DISABLE, CMD_SCTRL_MODEM_AHB_CLOCK_ENABLE
uint32_t sys_drv_modem_bus_clk_ctrl(bool clk_en);
//CMD_SCTRL_MODEM_CLOCK480M_DISABLE, CMD_SCTRL_MODEM_CLOCK480M_ENABLE
uint32_t sys_drv_modem_clk_ctrl(bool clk_en);
//CMD_SCTRL_MAC_AHB_CLOCK_DISABLE, CMD_SCTRL_MAC_AHB_CLOCK_ENABLE
uint32_t sys_drv_mac_bus_clk_ctrl(bool clk_en);
//CMD_SCTRL_MAC_CLOCK480M_DISABLE,CMD_SCTRL_MAC_CLOCK480M_ENABLE
uint32_t sys_drv_mac_clk_ctrl(bool clk_en);
//CMD_SCTRL_SET_VDD_VALUE
uint32_t sys_drv_set_vdd_value(uint32_t param);
//CMD_SCTRL_GET_VDD_VALUE
uint32_t sys_drv_get_vdd_value(void);
//CMD_SCTRL_BLOCK_EN_MUX_SET
uint32_t sys_drv_block_en_mux_set(uint32_t param);
//enable MAC and PHY interrupt
uint32_t sys_drv_enable_mac_gen_int(void);
uint32_t sys_drv_enable_mac_prot_int(void);
uint32_t sys_drv_enable_mac_tx_trigger_int(void);
uint32_t sys_drv_enable_mac_rx_trigger_int(void);
uint32_t sys_drv_enable_mac_txrx_misc_int(void);
uint32_t sys_drv_enable_mac_txrx_timer_int(void);
uint32_t sys_drv_enable_modem_int(void);
uint32_t sys_drv_enable_modem_rc_int(void);
uint32_t sys_drv_enable_hsu_int(void);


//Yantao Add End

/**  Audio Start  **/
uint32_t  sys_drv_aud_select_clock(uint32_t value);
uint32_t  sys_drv_aud_clock_en(uint32_t value);
uint32_t sys_drv_aud_vdd1v_en(uint32_t value);
uint32_t sys_drv_aud_vdd1v5_en(uint32_t value);
uint32_t sys_drv_aud_mic1_en(uint32_t value);
uint32_t sys_drv_aud_mic2_en(uint32_t value);
uint32_t sys_drv_aud_audpll_en(uint32_t value);
uint32_t sys_drv_aud_aud_en(uint32_t value);
uint32_t sys_drv_aud_dac_drv_en(uint32_t value);
uint32_t sys_drv_aud_dac_bias_en(uint32_t value);
uint32_t sys_drv_aud_bias_en(uint32_t value);
uint32_t sys_drv_aud_dacr_en(uint32_t value);
uint32_t sys_drv_aud_dacl_en(uint32_t value);
uint32_t sys_drv_aud_diffen_en(uint32_t value);
uint32_t sys_drv_aud_rvcmd_en(uint32_t value);
uint32_t sys_drv_aud_lvcmd_en(uint32_t value);
uint32_t sys_drv_aud_micbias1v_en(uint32_t value);
uint32_t sys_drv_aud_micbias_trim_set(uint32_t value);
uint32_t sys_drv_aud_mic_rst_set(uint32_t value);
uint32_t sys_drv_aud_mic1_gain_set(uint32_t value);
uint32_t sys_drv_aud_mic2_gain_set(uint32_t value);
uint32_t sys_drv_aud_mic1_single_en(uint32_t value);
uint32_t sys_drv_aud_mic2_single_en(uint32_t value);
uint32_t sys_drv_aud_dacg_set(uint32_t value);
uint32_t sys_drv_aud_dacg_get(void);
uint32_t sys_drv_aud_int_en(uint32_t value);
uint32_t sys_drv_sbc_int_en(uint32_t value);
uint32_t sys_drv_aud_power_en(uint32_t value);
uint32_t sys_drv_aud_audbias_en(uint32_t value);
uint32_t sys_drv_aud_adcbias_en(uint32_t value);
uint32_t sys_drv_aud_micbias_en(uint32_t value);
uint32_t sys_drv_aud_dac_dcoc_en(uint32_t value);
uint32_t sys_drv_aud_dac_idac_en(uint32_t value);
uint32_t sys_drv_aud_dac_bypass_dwa_en(uint32_t value);
uint32_t sys_drv_aud_dac_dacmute_en(uint32_t value);
/**  Audio End  **/

/**  FFT Start  **/
uint32_t sys_drv_fft_disckg_set(uint32_t value);
uint32_t sys_drv_cpu_fft_int_en(uint32_t value);
/**  FFT End  **/

/**  I2S Start  **/
uint32_t  sys_drv_i2s_select_clock(uint32_t value);
uint32_t  sys_drv_i2s_clock_en(uint32_t value);
uint32_t  sys_drv_i2s1_clock_en(uint32_t value);
uint32_t  sys_drv_i2s2_clock_en(uint32_t value);
uint32_t sys_drv_i2s_disckg_set(uint32_t value);
uint32_t sys_drv_i2s_int_en(uint32_t value);
uint32_t sys_drv_i2s1_int_en(uint32_t value);
uint32_t sys_drv_i2s2_int_en(uint32_t value);
uint32_t sys_drv_apll_en(uint32_t value);
uint32_t sys_drv_cb_manu_val_set(uint32_t value);
uint32_t sys_drv_ana_reg11_vsel_set(uint32_t value);
uint32_t sys_drv_apll_cal_val_set(uint32_t value);
uint32_t sys_drv_apll_spi_trigger_set(uint32_t value);
uint32_t sys_drv_i2s0_ckdiv_set(uint32_t value);
uint32_t sys_drv_apll_config_set(uint32_t value);
uint32_t sys_drv_dmic_clk_div_set(uint32_t value);
/**  I2S End  **/


/**  Touch Start **/
uint32_t sys_drv_touch_power_down(uint32_t enable);
uint32_t sys_drv_touch_sensitivity_level_set(uint32_t value);
uint32_t sys_drv_touch_scan_mode_enable(uint32_t enable);
uint32_t sys_drv_touch_detect_threshold_set(uint32_t value);
uint32_t sys_drv_touch_detect_range_set(uint32_t value);
uint32_t sys_drv_touch_calib_enable(uint32_t enable);
uint32_t sys_drv_touch_manul_mode_calib_value_set(uint32_t value);
uint32_t sys_drv_touch_manul_mode_enable(uint32_t enable);
uint32_t sys_drv_touch_scan_mode_chann_set(uint32_t value);
uint32_t sys_drv_touch_scan_mode_chann_sel(uint32_t value);
uint32_t sys_drv_touch_serial_cap_enable(void);
uint32_t sys_drv_touch_serial_cap_disable(void);
uint32_t sys_drv_touch_serial_cap_sel(uint32_t value);
uint32_t sys_drv_touch_spi_lock(void);
uint32_t sys_drv_touch_spi_unlock(void);
uint32_t sys_drv_touch_test_period_set(uint32_t value);
uint32_t sys_drv_touch_test_number_set(uint32_t value);
uint32_t sys_drv_touch_calib_period(uint32_t value);
uint32_t sys_drv_touch_calib_number(uint32_t value);
uint32_t sys_drv_touch_int_set(uint32_t value);
uint32_t sys_drv_touch_int_clear(uint32_t value);
uint32_t sys_drv_touch_int_enable(uint32_t value);

#if defined(CONFIG_SOC_BK7256XX)
uint32_t sys_drv_touch_adc_mode_enable(uint32_t enable);
#endif

/**  Touch End **/

/** auxs Start **/
uint32_t sys_drv_set_auxs_cis(uint32_t cksel, uint32_t ckdiv);
uint32_t sys_drv_set_cis_auxs_clk_en(uint32_t enable);
/** auxs End **/

/**  jpeg Start **/
uint32_t sys_drv_jpeg_power_en(uint32_t enable);
uint32_t sys_drv_set_jpeg_clk_sel(uint32_t value);
uint32_t sys_drv_set_clk_div_mode1_clkdiv_jpeg(uint32_t value);
uint32_t sys_drv_set_jpeg_disckg(uint32_t value);
uint32_t sys_drv_set_jpeg_clk_en(uint32_t enable);
/**  jpeg End **/

/**  H264 Start **/
uint32_t sys_drv_h264_power_en(uint32_t enable);
uint32_t sys_drv_set_h264_clk_sel(uint32_t value);
uint32_t sys_drv_set_clk_div_mode1_clkdiv_h264(uint32_t value);
uint32_t sys_drv_set_h264_clk_en(void);
/**  H264 End **/

/**  yuv_buf Start **/
uint32_t sys_drv_yuv_buf_power_en(uint32_t enable);
uint32_t sys_drv_set_yuv_buf_clk_en(uint32_t enable);
/**  yuv_buf End **/

/**  psram Start **/
uint32_t sys_drv_psram_volstage_sel(uint32_t value);
uint32_t sys_drv_psram_xtall_osc_enable(uint32_t value);
uint32_t sys_drv_psram_dco_enable(uint32_t value);
uint32_t sys_drv_psram_dpll_enable(uint32_t value);
uint32_t sys_drv_psram_ldo_enable(uint32_t value);
uint32_t sys_drv_psram_clk_sel(uint32_t value);
uint32_t sys_drv_psram_set_clkdiv(uint32_t value);
uint32_t sys_drv_psram_psldo_vsel(uint32_t value);
uint32_t sys_drv_psram_psldo_vset(uint32_t psldo_swb, uint32_t vpsramsel);
uint32_t sys_drv_psram_psram_disckg(uint32_t value);
/**  psram End **/

uint32_t sys_drv_cali_dpll(uint32_t param);
uint32_t sys_drv_cali_bias(uint32_t update);
uint32_t sys_drv_bias_reg_set(uint32_t param);
uint32_t sys_drv_bias_reg_clean(uint32_t param);
uint32_t sys_drv_bias_reg_read(void);
uint32_t sys_drv_bias_reg_write(uint32_t param);
uint32_t sys_drv_analog_reg4_bits_or(uint32_t param);
uint32_t sys_drv_analog_ctrl9_real_set(uint32_t param);
uint32_t sys_drv_analog_set_xtalh_ctune(uint32_t param);
uint32_t sys_drv_analog_get_xtalh_ctune(void);
uint32_t sys_drv_analog_reg1_set(uint32_t param);
uint32_t sys_drv_analog_reg2_set(uint32_t param);
uint32_t sys_drv_analog_reg3_set(uint32_t param);
uint32_t sys_drv_analog_reg4_set(uint32_t param);
uint32_t sys_drv_analog_reg6_set(uint32_t param);
uint32_t sys_drv_analog_reg7_set(uint32_t param);
uint32_t sys_drv_analog_reg1_get(void);
uint32_t sys_drv_analog_reg2_get(void);
uint32_t sys_drv_analog_reg4_get(void);
uint32_t sys_drv_analog_reg6_get(void);
uint32_t sys_drv_analog_reg7_get(void);
uint32_t sys_drv_analog_reg12_set(uint32_t param);
uint32_t sys_drv_analog_reg13_set(uint32_t param);
uint32_t sys_drv_analog_reg14_set(uint32_t param);
uint32_t sys_drv_analog_reg15_set(uint32_t param);
uint32_t sys_drv_analog_reg16_set(uint32_t param);
uint32_t sys_drv_analog_reg17_set(uint32_t param);
uint32_t sys_drv_cali_bgcalm(void);
uint32_t sys_drv_get_bgcalm(void);
uint32_t sys_drv_set_bgcalm(uint32_t param);
uint32_t sys_drv_set_dpll_for_i2s(void);
uint32_t sys_drv_set_gadc_ten(uint32_t param);
void sys_drv_analog_set(analog_reg_t reg, uint32_t value);
uint32_t sys_drv_analog_get(analog_reg_t reg);
void sys_drv_set_ana_trxt_tst_enable(uint32_t value);
void sys_drv_set_ana_scal_en(uint32_t value);
void sys_drv_set_ana_gadc_buf_ictrl(uint32_t value);
void sys_drv_set_ana_gadc_cmp_ictrl(uint32_t value);
void sys_drv_set_ana_pwd_gadc_buf(uint32_t value);
void sys_drv_set_ana_vref_sel(uint32_t value);
void sys_drv_set_ana_cb_cal_manu(uint32_t value);
void sys_drv_set_ana_cb_cal_trig(uint32_t value);
UINT32 sys_drv_get_ana_cb_cal_manu_val(void);
void sys_drv_set_ana_cb_cal_manu_val(uint32_t value);
void sys_drv_set_ana_reg11_apfms(uint32_t value);
void sys_drv_set_ana_reg12_dpfms(uint32_t value);
void sys_drv_set_ana_vlsel_ldodig(uint32_t value);
void sys_drv_set_ana_vhsel_ldodig(uint32_t value);
void sys_drv_set_ana_vctrl_sysldo(uint32_t value);
void sys_drv_set_ana_vtempsel(uint32_t value);
void sys_drv_set_ana_ioldo_lp(uint32_t value);
void sys_drv_set_ana_hres_sel0v9(void);

uint32_t sys_drv_get_cpu_storage_connect_op_select_flash_sel(void);
void sys_drv_set_cpu_storage_connect_op_select_flash_sel(uint32_t value);
void sys_drv_set_sys2flsh_2wire(uint32_t value);

/**  WIFI End **/

void sys_drv_yuv_buf_pwr_up(void);
void sys_drv_yuv_buf_pwr_down(void);
void sys_drv_h264_pwr_up(void);
void sys_drv_h264_pwr_down(void);
void sys_drv_slcd_clock_enable(void);
void sys_drv_slcd_clock_disable(void);
void sys_drv_set_ana_lcd_enable(void);
void sys_drv_set_ana_lcd_disable(void);
void sys_drv_set_ana_sw_bias(uint32_t value);
void sys_drv_set_ana_crb(uint32_t value);
void sys_drv_set_ana_com_port_enable(uint32_t value);
void sys_drv_set_ana_seg_port_enable(uint32_t value);
uint32_t sys_drv_get_ana_seg_port_enable_status(void);

void sys_drv_early_init(void);

#endif //_SYS_DRV_H_
// eof

