

#pragma once

#include "sys_types.h"
#include <driver/hal/hal_uart_types.h>
#include <driver/hal/hal_i2c_types.h>
#include <driver/hal/hal_pwm_types.h>
#include <driver/hal/hal_timer_types.h>
#include <driver/hal/hal_spi_types.h>
#include <driver/sys_pm_types.h>
#include <modules/pm.h>

#if defined(CONFIG_GPIO_CLOCK_PIN_SUPPORT)
#include <driver/hal/hal_clock_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	sys_hw_t *hw;
} sys_hal_t;


/**  Platform Start **/

//Platform

/** Platform Misc Start **/
bk_err_t sys_hal_init(void);
/** Platform Misc End **/

/** Platform USB Start **/
void sys_hal_usb_enable_clk(bool en);
void sys_hal_usb_analog_phy_en(bool en);
void sys_hal_usb_analog_speed_en(bool en);
void sys_hal_usb_analog_ckmcu_en(bool en);
void sys_hal_usb_analog_deepsleep_en(bool en);
void sys_hal_set_usb_analog_dp_capability(uint8_t capability);
void sys_hal_set_usb_analog_dn_capability(uint8_t capability);
void sys_hal_usb_enable_charge(bool en);
void sys_hal_usb_charge_vlcf_cal(void);
void sys_hal_usb_charge_icp_cal(void);
void sys_hal_usb_charge_vcv_cal(void);
void sys_hal_usb_charge_get_cal(void);

/** Platform USB End **/

/** Platform CAN Start **/
void sys_hal_can_set_sel_clk(uint32_t v);
uint32_t sys_hal_can_get_sel_clk(void);

/** Platform CAN End **/

/** Platform PWM Start **/
void sys_hal_pwm_set_clock(uint32_t mode, uint32_t param);
/** Platform PWM End **/

/** Platform flash Start **/
void sys_hal_flash_set_dco(void);

void sys_hal_flash_set_dpll(void);

void sys_hal_flash_set_clk(uint32_t value);

void sys_hal_flash_set_clk_div(uint32_t value);

uint32_t sys_hal_flash_get_clk_sel(void);

uint32_t sys_hal_flash_get_clk_div(void);

/** Platform flash End **/

/** Platform qspi Start **/
void sys_hal_set_qspi_vddram_voltage(uint32_t param);

void sys_hal_set_qspi_io_voltage(uint32_t param);

void sys_hal_qspi_clk_sel(uint32_t id, uint32_t param);

void sys_hal_qspi_set_src_clk_div(uint32_t id, uint32_t value);

/** Platform qspi End **/

/** Platform SDIO Start **/
void sys_hal_set_sdio_clk_en(uint32_t value);
void sys_hal_set_cpu0_sdio_int_en(uint32_t value);
void sys_hal_set_cpu1_sdio_int_en(uint32_t value);
void sys_hal_set_sdio_clk_div(uint32_t value);
uint32_t sys_hal_get_sdio_clk_div(void);
void sys_hal_set_sdio_clk_sel(uint32_t value);
uint32_t sys_hal_get_sdio_clk_sel(void);
/** Platform SDIO End **/

/*low power feature start*/
void sys_hal_enter_deep_sleep(void *param);
void sys_hal_enter_normal_sleep(uint32_t peri_clk);
void sys_hal_enter_normal_wakeup(void);
bk_err_t sys_hal_normal_sleep_mode_ctrl(pm_normal_sleep_mode_e normal_sleep,pm_cpu_freq_e cpu_freq);
#if defined(CONFIG_SOC_BK7256XX)
__attribute__((section(".itcm_sec_code"))) void sys_hal_enter_low_voltage(void);
#else
void sys_hal_enter_low_voltage(void);
#endif
void sys_hal_enter_cpu_wfi(void);
void sys_hal_module_power_ctrl(power_module_name_t module,power_module_state_t power_state);
void sys_hal_wakeup_interrupt_clear(wakeup_source_t interrupt_source);
void sys_hal_module_power_ctrl(power_module_name_t module,power_module_state_t power_state);
void sys_hal_module_RF_power_ctrl (module_name_t module,power_module_state_t power_state);
void sys_hal_cpu0_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state);
void sys_hal_cpu1_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state);
void sys_hal_set_cpu1_boot_address_offset(uint32_t address_offset);
void sys_hal_set_cpu1_reset(uint32_t reset_value);
void sys_hal_set_cpu2_boot_address_offset(uint32_t address_offset);
void sys_hal_set_cpu2_reset(uint32_t reset_value);
void sys_hal_enable_mac_wakeup_source(void);
void sys_hal_enable_bt_wakeup_source(void);

uint32_t sys_hal_all_modules_clk_div_get(clk_div_reg_e reg);
void sys_hal_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value);
void sys_hal_usb_wakeup_enable(uint8_t index);
void sys_hal_touch_wakeup_enable(uint8_t index);
void sys_hal_rtc_wakeup_enable(uint32_t value);
void sys_hal_rtc_ana_wakeup_enable(uint32_t period);
void sys_hal_gpio_ana_wakeup_enable(uint32_t count, uint32_t index, uint32_t type);
void sys_hal_gpio_state_switch(bool lock);
void sys_hal_cpu_clk_div_set(uint32_t core_index, uint32_t value);
uint32_t sys_hal_cpu_clk_div_get(uint32_t core_index);
void sys_hal_low_power_hardware_init(void);
int32 sys_hal_lp_vol_set(uint32_t value);
uint32_t sys_hal_lp_vol_get(void);
int32 sys_hal_rf_tx_vol_set(uint32_t value);
uint32_t sys_hal_rf_tx_vol_get(void);
int32 sys_hal_rf_rx_vol_set(uint32_t value);
uint32_t sys_hal_rf_rx_vol_get(void);
int32 sys_hal_module_power_state_get(power_module_name_t module);
int32 sys_hal_rosc_calibration(uint32_t rosc_cali_mode, uint32_t cali_interval);
int sys_hal_rosc_test_mode(bool enabled);
int32 sys_hal_bandgap_cali_set(uint32_t value);//increase or decrease the dvdddig voltage
uint32_t sys_hal_bandgap_cali_get(void);
bk_err_t sys_hal_switch_cpu_bus_freq(pm_cpu_freq_e cpu_bus_freq);
bk_err_t sys_hal_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core,uint32_t ckdiv_bus, uint32_t ckdiv_cpu0,uint32_t ckdiv_cpu1);
bk_err_t sys_hal_cpu_freq_dump(void);
#if defined(CONFIG_SOC_BK7236XX)
#define sys_hal_set_cpu0_rxevt_sel(param)     sys_ll_set_cpu0_int_halt_clk_op_cpu0_rxevt_sel(param)
#define sys_hal_set_cpu1_rxevt_sel(param)     sys_ll_set_cpu1_int_halt_clk_op_cpu1_rxevt_sel(param)
#define sys_hal_set_cpu2_rxevt_sel(param)     sys_ll_set_cpu2_int_halt_clk_op_cpu2_rxevt_sel(param)
#endif

/*low power feature end*/
uint32 sys_hal_get_chip_id(void);
uint32 sys_hal_get_device_id(void);
int32 sys_hal_usb_power_down(void);
int32 sys_hal_usb_power_up(void);
uint32_t sys_hal_sadc_get_int_en(void);
int32 sys_hal_int_disable(uint32 param);
int32 sys_hal_int_enable(uint32 param);
int32 sys_hal_int_group2_disable(uint32 param);
int32 sys_hal_int_group2_enable(uint32 param);

#if defined(CONFIG_SOC_BK7236_SMP_TEMP)
int32 sys_hal_core_int_group1_disable(uint32 core_id, uint32 param);
int32 sys_hal_core_int_group1_enable(uint32 core_id, uint32 param);
int32 sys_hal_core_int_group2_disable(uint32 core_id, uint32 param);
int32 sys_hal_core_int_group2_enable(uint32 core_id, uint32 param);
#endif

int32 sys_hal_fiq_disable(uint32 param);
int32 sys_hal_fiq_enable(uint32 param);
int32 sys_hal_global_int_disable(uint32 param);
int32 sys_hal_global_int_enable(uint32 param);
uint32 sys_hal_get_int_status(void);
uint32 sys_hal_get_int_group2_status(void);
int32 sys_hal_set_int_status(uint32 param);
/* REG_0x29:cpu0_int_32_63_status->cpu0_gpio_int_st: ,R,0x29[22]*/
uint32_t sys_hal_get_cpu0_gpio_int_st(void);
uint32 sys_hal_get_fiq_reg_status(void);
uint32 sys_hal_set_fiq_reg_status(uint32 param);
uint32 sys_hal_get_intr_raw_status(void);
uint32 sys_hal_set_intr_raw_status(uint32 param);
int32 sys_hal_set_jtag_mode(uint32 param);
uint32 sys_hal_get_jtag_mode(void);

/*clock power control start*/
void sys_hal_clk_pwr_ctrl(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up);
void sys_hal_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel);
dev_clk_select_t sys_hal_get_clk_select(dev_clk_select_id_t dev);
//DCO divider is valid for all of the peri-devices.
void sys_hal_set_dco_div(dev_clk_dco_div_t div);
//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_hal_get_dco_div(void);
/*clock power control end*/

/* UART select clock    DIRTY **/
void sys_hal_uart_select_clock(uart_id_t id, uart_src_clk_t mode);
/* UART select clock    DIRTY **/

void sys_hal_i2c_select_clock(i2c_id_t id, i2c_src_clk_t mode);
void sys_hal_arm_wakeup_enable(uint32_t param);
void sys_hal_arm_wakeup_disable(uint32_t param);
uint32_t sys_hal_get_arm_wakeup(void);

void aon_pmu_hal_set_wakeup_source_reg(uint32_t param);
uint32_t aon_pmu_hal_get_wakeup_source_reg(void);

void sys_hal_set_cksel_sadc(uint32_t value);
void sys_hal_set_cksel_pwm(uint32_t value);
uint32_t sys_hal_uart_select_clock_get(uart_id_t id);
uint32_t sys_hal_i2c_select_clock_get(i2c_id_t id);
uint32_t sys_hal_sadc_get_int_en(void);
void sys_hal_sadc_int_enable(void);
void sys_hal_sadc_int_disable(void);
void sys_hal_sadc_pwr_up(void);
void sys_hal_sadc_pwr_down(void);
void sys_hal_set_saradc_config(void);
void sys_hal_set_sdmadc_config(void);
void sys_hal_set_clksel_spi(uint32_t value);
void sys_hal_timer_select_clock(sys_sel_timer_t num, timer_src_clk_t mode);
uint32_t sys_hal_timer_select_clock_get(sys_sel_timer_t id);
void sys_hal_spi_select_clock(spi_id_t num, spi_src_clk_t mode);
/* PWM select clock    DIRTY **/
void sys_hal_pwm_select_clock(sys_sel_pwm_t num, pwm_src_clk_t mode);
/* PWM select clock    DIRTY **/

void sys_hal_en_tempdet(uint32_t value);
void sys_hal_set_temp_mode(bool high_volt_mode);

uint32_t sys_hal_nmi_wdt_get_clk_div(void);
void sys_hal_nmi_wdt_set_clk_div(uint32_t value);
void sys_hal_trng_disckg_set(uint32_t value);
/**  Platform End **/




/**  BT Start **/
//BT
uint32_t sys_hal_mclk_mux_get(void);
uint32_t sys_hal_mclk_div_get(void);
void sys_hal_mclk_select(uint32_t value);
void sys_hal_mclk_div_set(uint32_t value);

void sys_hal_bt_power_ctrl(bool power_up);

void sys_hal_bt_clock_ctrl(bool en);
void sys_hal_xvr_clock_ctrl(bool en);

uint32_t sys_hal_interrupt_status_get(void);
void sys_hal_interrupt_status_set(uint32_t value);

void sys_hal_btdm_interrupt_ctrl(bool en);
void sys_hal_ble_interrupt_ctrl(bool en);
void sys_hal_bt_interrupt_ctrl(bool en);

void sys_hal_bt_rf_ctrl(bool en);
uint32_t sys_hal_bt_rf_status_get(void);
void sys_hal_bt_sleep_exit_ctrl(bool en);

void sys_hal_rf_ctrl(uint8_t type);
uint8_t sys_hal_rf_ctrl_type_get(void);

/**  BT End **/




/**  Audio Start **/
//Audio
/**  Audio End **/



/**  Video Start **/
void sys_hal_lcd_disp_clk_en(uint8_t clk_src_sel, uint8_t clk_div_l, uint8_t clk_div_h,uint8_t clk_always_on);
void sys_hal_lcd_disp_close(void);

void sys_hal_set_jpeg_dec_disckg(uint32_t value);
void sys_hal_dma2d_clk_en(uint8_t clk_always_on);

void sys_hal_set_slcd_clk_en(bool enable);
void sys_hal_set_slcd_sw_bias(uint32_t value);
void sys_hal_set_slcd_crb(uint32_t value);
void sys_hal_set_slcd_com_enable(uint32_t value);
void sys_hal_set_slcd_seg_enable(uint32_t value);
uint32_t sys_hal_get_slcd_seg_enable_status(void);
void sys_hal_set_slcd_enable(bool enable);

/**  Video End **/




/**  WIFI Start **/
//WIFI
void sys_hal_cali_dpll_spi_trig_disable(void);
void sys_hal_cali_dpll_spi_trig_enable(void);
void sys_hal_cali_dpll_spi_detect_disable(void);
void sys_hal_cali_dpll_spi_detect_enable(void);
uint32_t sys_hal_bias_reg_clean(uint32_t param);
uint32_t sys_hal_bias_reg_set(uint32_t param);
uint32_t sys_hal_bias_reg_read(void);
uint32_t sys_hal_bias_reg_write(uint32_t param);
uint32_t sys_hal_analog_reg1_get(void);
uint32_t sys_hal_analog_reg2_get(void);
uint32_t sys_hal_analog_reg4_get(void);
uint32_t sys_hal_analog_reg6_get(void);
uint32_t sys_hal_analog_reg7_get(void);
uint32_t sys_hal_analog_reg2_set(uint32_t param);
uint32_t sys_hal_analog_reg4_set(uint32_t param);
void sys_hal_set_xtalh_ctune(uint32_t value);
void sys_hal_set_ana_reg1_value(uint32_t value);
void sys_hal_set_ana_reg2_value(uint32_t value);
void sys_hal_set_ana_reg3_value(uint32_t value);
void sys_hal_set_ana_reg4_value(uint32_t value);
void sys_hal_set_ana_reg12_value(uint32_t value);
void sys_hal_set_ana_reg13_value(uint32_t value);
void sys_hal_set_ana_reg14_value(uint32_t value);
void sys_hal_set_ana_reg15_value(uint32_t value);
void sys_hal_set_ana_reg16_value(uint32_t value);
void sys_hal_set_ana_reg17_value(uint32_t value);
void sys_hal_set_ana_reg18_value(uint32_t value);
void sys_hal_set_ana_reg19_value(uint32_t value);
void sys_hal_set_ana_reg20_value(uint32_t value);
void sys_hal_set_ana_reg21_value(uint32_t value);
void sys_hal_set_ana_reg27_value(uint32_t value);
void sys_hal_analog_reg4_bits_or(uint32_t value);
void sys_hal_set_ana_reg6_value(uint32_t value);
void sys_hal_set_ana_reg7_value(uint32_t value);
uint32_t sys_hal_get_xtalh_ctune(void);
uint32_t sys_hal_cali_dpll(uint32_t first_time);
uint32_t sys_hal_cali_bgcalm(void);
uint32_t sys_hal_get_bgcalm(void);
void sys_hal_set_bgcalm(uint32_t value);
void sys_hal_set_audioen(uint32_t value);
void sys_hal_set_dpll_div_cksel(uint32_t value);
void sys_hal_set_dpll_reset(uint32_t value);
void sys_hal_set_gadc_ten(uint32_t value);
void sys_hal_analog_set(analog_reg_t reg, uint32_t value);
void sys_hal_analog_set_iram(analog_reg_t reg, uint32_t value);
uint32_t sys_hal_analog_get(analog_reg_t reg);
//Yantao Add Start
void sys_hal_modem_core_reset(void);
void sys_hal_mpif_invert(void);
void sys_hal_modem_subsys_reset(void);
void sys_hal_mac_subsys_reset(void);
void sys_hal_usb_subsys_reset(void);
void sys_hal_dsp_subsys_reset(void);
void sys_hal_mac_power_ctrl(bool power_up);
void sys_hal_modem_power_ctrl(bool power_up);
void sys_hal_pta_ctrl(bool pta_en);
void sys_hal_modem_bus_clk_ctrl(bool clk_en);
void sys_hal_modem_clk_ctrl(bool clk_en);
void sys_hal_mac_bus_clk_ctrl(bool clk_en);
void sys_hal_mac_clk_ctrl(bool clk_en);
void sys_hal_set_vdd_value(uint32_t param);
uint32_t sys_hal_get_vdd_value(void);
void sys_hal_block_en_mux_set(uint32_t param);
void sys_hal_enable_mac_gen_int(void);
void sys_hal_enable_mac_prot_int(void);
void sys_hal_enable_mac_tx_trigger_int(void);
void sys_hal_enable_mac_rx_trigger_int(void);
void sys_hal_enable_mac_txrx_misc_int(void);
void sys_hal_enable_mac_txrx_timer_int(void);
void sys_hal_enable_modem_int(void);
void sys_hal_enable_modem_rc_int(void);
void sys_hal_enable_hsu_int(void);
//Yantao Add End

void sys_hal_set_ana_vtempsel(uint32_t value);
/**  WIFI End **/

/**  MISC Start  */

void sys_hal_set_ioldo_lp(uint32_t value);

/**  MISC End  */

/**  Audio Start  **/

void sys_hal_aud_select_clock(uint32_t value);
void sys_hal_aud_clock_en(uint32_t value);
void sys_hal_aud_vdd1v_en(uint32_t value);
void sys_hal_aud_vdd1v5_en(uint32_t value);
void sys_hal_aud_mic1_en(uint32_t value);
void sys_hal_aud_mic2_en(uint32_t value);
void sys_hal_aud_audpll_en(uint32_t value);
void sys_hal_aud_aud_en(uint32_t value);
void sys_hal_aud_dac_drv_en(uint32_t value);
void sys_hal_aud_bias_en(uint32_t value);
void sys_hal_aud_dacr_en(uint32_t value);
void sys_hal_aud_dacl_en(uint32_t value);
void sys_hal_aud_diffen_en(uint32_t value);
void sys_hal_aud_rvcmd_en(uint32_t value);
void sys_hal_aud_lvcmd_en(uint32_t value);
void sys_hal_aud_micbias1v_en(uint32_t value);
void sys_hal_aud_micbias_trim_set(uint32_t value);
void sys_hal_aud_mic_rst_set(uint32_t value);
void sys_hal_aud_mic1_gain_set(uint32_t value);
void sys_hal_aud_mic2_gain_set(uint32_t value);
void sys_hal_aud_mic1_single_en(uint32_t value);
void sys_hal_aud_mic2_single_en(uint32_t value);
void sys_hal_aud_dacg_set(uint32_t value);
uint32_t sys_hal_aud_dacg_get(void);
void sys_hal_aud_dcoc_en(uint32_t value);
void sys_hal_aud_lmdcin_set(uint32_t value);
void sys_hal_aud_audbias_en(uint32_t value);
void sys_hal_aud_adcbias_en(uint32_t value);
void sys_hal_aud_micbias_en(uint32_t value);
void sys_hal_aud_dac_bias_en(uint32_t value);
void sys_hal_aud_idac_en(uint32_t value);
void sys_hal_aud_int_en(uint32_t value);
void sys_hal_sbc_int_en(uint32_t value);
void sys_hal_aud_power_en(uint32_t value);
void sys_hal_aud_dac_dcoc_en(uint32_t value);
void sys_hal_aud_dac_idac_en(uint32_t value);
void sys_hal_aud_dac_bypass_dwa_en(uint32_t value);
void sys_hal_aud_dac_dacmute_en(uint32_t value);

/**  Audio End  **/

void sys_hal_set_sys2flsh_2wire(uint32_t value);

/**  FFT Start  **/

void sys_hal_fft_disckg_set(uint32_t value);
void sys_hal_cpu_fft_int_en(uint32_t value);

/**  FFT End  **/

/**  I2S Start  **/
void sys_hal_i2s_select_clock(uint32_t value);
void sys_hal_i2s_clock_en(uint32_t value);
void sys_hal_i2s1_clock_en(uint32_t value);
void sys_hal_i2s2_clock_en(uint32_t value);

void sys_hal_i2s_disckg_set(uint32_t value);
void sys_hal_i2s_int_en(uint32_t value);
void sys_hal_i2s1_int_en(uint32_t value);
void sys_hal_i2s2_int_en(uint32_t value);
void sys_hal_apll_en(uint32_t value);
void sys_hal_cb_manu_val_set(uint32_t value);
void sys_hal_ana_reg11_vsel_set(uint32_t value);
void sys_hal_apll_cal_val_set(uint32_t value);
void sys_hal_apll_spi_trigger_set(uint32_t value);
void sys_hal_i2s0_ckdiv_set(uint32_t value);
void sys_hal_apll_config_set(uint32_t value);
void sys_hal_dmic_clk_div_set(uint32_t value);
/**  I2S End  **/


/**  Touch Start **/
void sys_hal_touch_power_down(uint32_t value);
void sys_hal_touch_sensitivity_level_set(uint32_t value);
void sys_hal_touch_scan_mode_enable(uint32_t value);
void sys_hal_touch_detect_threshold_set(uint32_t value);
void sys_hal_touch_detect_range_set(uint32_t value);
void sys_hal_touch_calib_enable(uint32_t value);
void sys_hal_touch_manul_mode_calib_value_set(uint32_t value);
void sys_hal_touch_manul_mode_enable(uint32_t value);
void sys_hal_touch_scan_mode_chann_set(uint32_t value);
void sys_hal_touch_scan_mode_chann_sel(uint32_t value);
void sys_hal_touch_serial_cap_enable(void);
void sys_hal_touch_serial_cap_disable(void);
void sys_hal_touch_serial_cap_sel(uint32_t value);
void sys_hal_touch_spi_lock(void);
void sys_hal_touch_spi_unlock(void);
void sys_hal_touch_test_period_set(uint32_t value);
void sys_hal_touch_test_number_set(uint32_t value);
void sys_hal_touch_calib_period_set(uint32_t value);
void sys_hal_touch_calib_number_set(uint32_t value);
void sys_hal_touch_int_set(uint32_t value);
void sys_hal_touch_int_clear(uint32_t value);
void sys_hal_touch_int_enable(uint32_t value);

#if defined(CONFIG_SOC_BK7256XX)
void sys_hal_touch_adc_mode_enable(uint32_t value);
#endif

/**  Touch End **/


/** jpeg start **/
void sys_hal_mclk_mux_set(uint32_t value);
void sys_hal_set_jpeg_clk_sel(uint32_t value);
void sys_hal_set_clk_div_mode1_clkdiv_jpeg(uint32_t value);
void sys_hal_set_jpeg_disckg(uint32_t value);
void sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value);
void sys_hal_video_power_en(uint32_t value);
void sys_hal_set_auxs_cis_clk_sel(uint32_t value);
void sys_hal_set_auxs_cis_clk_div(uint32_t value);
void sys_hal_set_jpeg_clk_en(uint32_t value);
void sys_hal_set_cis_auxs_clk_en(uint32_t value);

/** jpeg end **/

/** h264 Start **/
void sys_hal_set_h264_clk_en(uint32_t value);

/** h264 End **/

/**  psram Start **/
void sys_hal_set_ana_vddgpio_sel(uint32_t value);
void sys_hal_psram_volstage_sel(uint32_t enable);
void sys_hal_psram_xtall_osc_enable(uint32_t enable);
void sys_hal_psram_doc_enable(uint32_t enable);
void sys_hal_psram_dpll_enable(uint32_t enable);
void sys_hal_psram_ldo_enable(uint32_t enable);
uint32_t sys_hal_psram_ldo_status(void);
void sys_hal_psram_clk_sel(uint32_t value);
void sys_hal_psram_set_clkdiv(uint32_t value);
void sys_hal_psram_psldo_vsel(uint32_t value);
void sys_hal_psram_psldo_vset(uint32_t output_voltage, uint32_t is_add_200mv);
void sys_hal_psram_psram_disckg(uint32_t value);

/**  psram End **/

uint32_t sys_hal_get_cpu_storage_connect_op_select_flash_sel(void);
void sys_hal_set_cpu_storage_connect_op_select_flash_sel(uint32_t value);

/**  Misc Start **/
//Misc
/**  Misc End **/



/** Temp reserve heare  */

#if 1
void sys_hal_set_btdm_clk_en(bool value);
uint32 sys_hal_get_btdm_clk_en(void);
void sys_hal_set_xvr_clk_en(bool value);
uint32 sys_hal_get_xvr_clk_en(void);
#endif


#if 1
void sys_hal_set_power_on_btsp(bool value);
uint32 sys_hal_get_power_on_btsp(void);
#endif


#if 1
void sys_hal_set_bts_wakeup_platform_en(bool value);
uint32 sys_hal_get_bts_wakeup_platform_en(void);
void sys_hal_set_bts_sleep_exit_req(bool value);
uint32 sys_hal_get_bts_sleep_exit_req(void);
#endif

void sys_hal_set_ana_reg_spi_latch1v(uint32_t v);
void sys_hal_set_ioldo_bypass(uint32_t v);
void sys_hal_set_ioldo_volt(uint32_t v);
void sys_hal_set_analdo_volt(uint32_t v);
void sys_hal_set_analdo_sel(uint32_t v);
void sys_hal_set_ana_trxt_tst_enable(uint32_t value);
void sys_hal_set_ana_scal_en(uint32_t value);
void sys_hal_set_ana_gadc_buf_ictrl(uint32_t value);
void sys_hal_set_ana_gadc_cmp_ictrl(uint32_t value);
void sys_hal_set_ana_pwd_gadc_buf(uint32_t value);
void sys_hal_set_ana_vref_sel(uint32_t value);
void sys_hal_set_ana_cb_cal_manu(uint32_t value);
void sys_hal_set_ana_cb_cal_trig(uint32_t value);
UINT32 sys_hal_get_ana_cb_cal_manu_val(void);
void sys_hal_set_ana_cb_cal_manu_val(uint32_t value);
void sys_hal_set_ana_vlsel_ldodig(uint32_t value);
void sys_hal_set_ana_vhsel_ldodig(uint32_t value);
void sys_hal_set_ana_vctrl_sysldo(uint32_t value);
void sys_hal_enable_eth_int(uint32_t value);

void sys_hal_set_yuv_buf_clock_en(uint32_t value);
void sys_hal_set_h264_clock_en(uint32_t value);
void sys_hal_set_ana_reg11_apfms(uint32_t value);
void sys_hal_set_ana_reg12_dpfms(uint32_t value);
void sys_hal_set_ana_reg5_pwd_rosc_spi(uint32_t value);
#if defined(CONFIG_ANA_RTC) || defined(CONFIG_ANA_GPIO)
void sys_hal_set_ana_reg8_spi_latch1v(uint32_t value);
#if defined(CONFIG_BK7239N_MP) && (CONFIG_BK7239N_MP == 1)
void sys_hal_set_ana_reg8_spi_latch1v_iram(uint32_t value);
void sys_hal_set_ana_reg8_rst_timerwks1v(uint32_t value);
void sys_hal_set_ana_reg8_rst_gpiowks(uint32_t value);
#else
void sys_hal_set_ana_reg8_spi_latch1v(uint32_t value);
void sys_hal_set_ana_reg8_rst_wks1v(uint32_t value);
#endif
void sys_hal_set_ana_reg5_adc_div(uint32_t value);
void sys_hal_set_ana_reg5_rosc_disable(uint32_t value);
void sys_hal_set_ana_reg7_timer_wkrstn(uint32_t value);
void sys_hal_set_ana_reg7_clk_sel(uint32_t value);
void sys_hal_set_ana_reg8_lvsleep_wkrst(uint32_t value);
void sys_hal_set_ana_reg8_gpiowk_rstn(uint32_t value);
void sys_hal_set_ana_reg8_rtcwk_rstn(uint32_t value);
void sys_hal_set_ana_reg8_ensfsdd(uint32_t value);
void sys_hal_set_ana_reg8_vlden(uint32_t value);
void sys_hal_set_ana_reg8_pwdovp1v(uint32_t value);
void sys_hal_set_ana_reg9_spi_timerwken(uint32_t value);
void sys_hal_set_ana_reg9_spi_byp32pwd(uint32_t value);
uint32_t sys_hal_get_ana_reg11_gpiowk(void);
void sys_hal_set_ana_reg11_gpiowk(uint32_t value);
uint32_t sys_hal_get_ana_reg11_rtcsel(void);
void sys_hal_set_ana_reg11_rtcsel(uint32_t value);
uint32_t sys_hal_get_ana_reg11_timersel(void);
void sys_hal_set_ana_reg11_timersel(uint32_t value);
uint32_t sys_hal_get_ana_reg12_timersel(void);
void sys_hal_set_ana_reg12_timersel(uint32_t value);
uint32_t sys_hal_get_ana_reg13_rtcsel(void);
void sys_hal_set_ana_reg13_rtcsel(uint32_t value);
void sys_hal_enable_ana_rtc_int(void);
void sys_hal_disable_ana_rtc_int(void);
void sys_hal_enable_ana_gpio_int(void);
void sys_hal_disable_ana_gpio_int(void);
#endif


/* generated codes by python tool with address mapping */
#if 0	//just for reference

/* REG_0x00 */

uint32_t sys_hal_get_device_id_value(sys_hw_t *hw);

/* REG_0x00:device_id->DeviceID: ,RO,0x0[31:0]*/
uint32_t sys_hal_get_device_id_deviceid(sys_hw_t *hw);

/* REG_0x01 */

uint32_t sys_hal_get_version_id_value(sys_hw_t *hw);

/* REG_0x01:version_id->VersionID: ,RO,0x1[31:0]*/
uint32_t sys_hal_get_version_id_versionid(sys_hw_t *hw);

/* REG_0x02 */

uint32_t sys_hal_get_cpu_current_run_status_value(sys_hw_t *hw);

/* REG_0x02:cpu_current_run_status->core0_halted:core0 halt indicate,RO,0x2[0]*/
uint32_t sys_hal_get_cpu_current_run_status_core0_halted(sys_hw_t *hw);

/* REG_0x02:cpu_current_run_status->core1_halted:core1 halt indicate,RO,0x2[1]*/
uint32_t sys_hal_get_cpu_current_run_status_core1_halted(sys_hw_t *hw);

/* REG_0x02:cpu_current_run_status->cpu0_sw_reset:cpu0_sw_reset indicate,RO,0x2[4]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu0_sw_reset(sys_hw_t *hw);

/* REG_0x02:cpu_current_run_status->cpu1_sw_reset:cpu1_sw_reset indicate,RO,0x2[5]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu1_sw_reset(sys_hw_t *hw);

/* REG_0x02:cpu_current_run_status->cpu0_pwr_dw_state:cpu0_pwr_dw_state,RO,0x2[8]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu0_pwr_dw_state(sys_hw_t *hw);

/* REG_0x02:cpu_current_run_status->cpu1_pwr_dw_state:cpu1_pwr_dw_state,RO,0x2[9]*/
uint32_t sys_hal_get_cpu_current_run_status_cpu1_pwr_dw_state(sys_hw_t *hw);

/* REG_0x03 */

uint32_t sys_hal_get_cpu_storage_connect_op_select_value(sys_hw_t *hw);

void sys_hal_set_cpu_storage_connect_op_select_value(sys_hw_t *hw, uint32_t value);

/* REG_0x03:cpu_storage_connect_op_select->boot_mode:0:ROM boot 1:FLASH boot,R/W,0x3[0]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_boot_mode(sys_hw_t *hw);

void sys_hal_set_cpu_storage_connect_op_select_boot_mode(sys_hw_t *hw, uint32_t value);

/* REG_0x03:cpu_storage_connect_op_select->jtag_core_sel:0:jtag connect core0, 1:jtag connect core1,R/W,0x3[8]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_jtag_core_sel(sys_hw_t *hw);

void sys_hal_set_cpu_storage_connect_op_select_jtag_core_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x03:cpu_storage_connect_op_select->flash_sel:0: normal flash operation 1:flash download by spi,R/W,0x3[9]*/
uint32_t sys_hal_get_cpu_storage_connect_op_select_flash_sel(sys_hw_t *hw);

void sys_hal_set_cpu_storage_connect_op_select_flash_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x04 */

uint32_t sys_hal_get_cpu0_int_halt_clk_op_value(sys_hw_t *hw);

void sys_hal_set_cpu0_int_halt_clk_op_value(sys_hw_t *hw, uint32_t value);

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_sw_rst:reserved,R/W,0x4[0]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_sw_rst(sys_hw_t *hw);

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_sw_rst(sys_hw_t *hw, uint32_t value);

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_pwr_dw:reserved,R/W,0x4[1]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(sys_hw_t *hw);

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(sys_hw_t *hw, uint32_t value);

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_int_mask:cpu0 int mask,R/W,0x4[2]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_int_mask(sys_hw_t *hw);

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_int_mask(sys_hw_t *hw, uint32_t value);

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_halt:core0 halt indicate,R/W,0x4[3]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_halt(sys_hw_t *hw);

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_halt(sys_hw_t *hw, uint32_t value);

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_clk_div:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x4[7:4]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_clk_div(sys_hw_t *hw);

void sys_hal_set_cpu0_int_halt_clk_op_cpu0_clk_div(sys_hw_t *hw, uint32_t value);

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_offset:reserved,RO,0x4[31:8]*/
uint32_t sys_hal_get_cpu0_int_halt_clk_op_cpu0_offset(sys_hw_t *hw);

/* REG_0x05 */

uint32_t sys_hal_get_cpu1_int_halt_clk_op_value(sys_hw_t *hw);

void sys_hal_set_cpu1_int_halt_clk_op_value(sys_hw_t *hw, uint32_t value);

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_sw_rst:reserved,R/W,0x5[0]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_sw_rst(sys_hw_t *hw);

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_sw_rst(sys_hw_t *hw, uint32_t value);

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_pwr_dw:reserved,R/W,0x5[1]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(sys_hw_t *hw);

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(sys_hw_t *hw, uint32_t value);

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_int_mask:cpu1 int mask,R/W,0x5[2]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_int_mask(sys_hw_t *hw);

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_int_mask(sys_hw_t *hw, uint32_t value);

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_halt:core1 halt indicate,R/W,0x5[3]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_halt(sys_hw_t *hw);

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_halt(sys_hw_t *hw, uint32_t value);

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_clk_div:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x5[7:4]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_clk_div(sys_hw_t *hw);

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_clk_div(sys_hw_t *hw, uint32_t value);

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_offset:reserved,R/W,0x5[31:8]*/
uint32_t sys_hal_get_cpu1_int_halt_clk_op_cpu1_offset(sys_hw_t *hw);

void sys_hal_set_cpu1_int_halt_clk_op_cpu1_offset(sys_hw_t *hw, uint32_t value);

/* REG_0x06 */

/* REG_0x08 */

uint32_t sys_hal_get_cpu_clk_div_mode1_value(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_value(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clkdiv_core:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x8[3:0]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_core(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clkdiv_core(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_core:0: clk_DCO	  1 : XTAL		2 : 320M	  3 : 480M,R/W,0x8[5:4]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_core(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_core(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clkdiv_bus:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x8[6]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_bus(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart0:Frequency division :	0:/1  1:/2	2:/4  3:/8,R/W,0x8[9:8]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_uart0(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clkdiv_uart0(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clksel_uart0:0:XTAL				1:APLL,R/W,0x8[10]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clksel_uart0(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clksel_uart0(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart1:Frequency division :	0:/1  1:/2	2:/4  3:/8,R/W,0x8[12:11]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_uart1(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clkdiv_uart1(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_uart1:0:XTAL			   1:APLL,R/W,0x8[13]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_uart1(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_uart1(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart2:Frequency division :	0:/1  1:/2	2:/4  3:/8,R/W,0x8[15:14]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_uart2(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clkdiv_uart2(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_uart2:0:XTAL			   1:APLL,R/W,0x8[16]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_uart2(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_uart2(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_sadc:0:XTAL			  1:APLL,R/W,0x8[17]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_sadc(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_sadc(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm0:0:clk32			   1:XTAL,R/W,0x8[18]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_pwm0(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_pwm0(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm1:0:clk32			   1:XTAL,R/W,0x8[19]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_pwm1(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_pwm1(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_timer0:0:clk32 			 1:XTAL,R/W,0x8[20]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_timer0(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_timer0(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_timer1:0:clk32 			 1:XTAL,R/W,0x8[21]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_timer1(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_timer1(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_timer2:0:clk32 			 1:XTAL,R/W,0x8[22]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_timer2(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_timer2(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_i2s:0:XTAL 			 1:APLL,R/W,0x8[24]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_i2s(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_i2s(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_aud:0:XTAL 			 1:APLL,R/W,0x8[25]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_aud(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_aud(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clkdiv_jpeg:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x8[29:26]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_jpeg(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clkdiv_jpeg(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->cksel_jpeg:0:clk_320M	  1:clk_480M,R/W,0x8[30]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_cksel_jpeg(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_cksel_jpeg(sys_hw_t *hw, uint32_t value);

/* REG_0x08:cpu_clk_div_mode1->clkdiv_disp_l:Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),R/W,0x8[31]*/
uint32_t sys_hal_get_cpu_clk_div_mode1_clkdiv_disp_l(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode1_clkdiv_disp_l(sys_hw_t *hw, uint32_t value);

/* REG_0x09 */

uint32_t sys_hal_get_cpu_clk_div_mode2_value(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_value(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->clkdiv_disp_h:Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),R/W,0x9[2:0]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_clkdiv_disp_h(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_clkdiv_disp_h(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->cksel_disp:0:clk_320M	  1:clk_480M,R/W,0x9[3]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_disp(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_cksel_disp(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->ckdiv_psram:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x9[4]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_psram(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_ckdiv_psram(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->cksel_psram:0:clk_320M	   1:clk_480M,R/W,0x9[5]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_psram(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_cksel_psram(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->ckdiv_qspi0:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x9[9:6]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_qspi0(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_ckdiv_qspi0(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->ckdiv_sdio:0:/1	1:/2  2:/4	3:/8  4:/16  5:/32	6:/64  7:/256,R/W,0x9[16:14]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_sdio(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_ckdiv_sdio(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->cksel_sdio:0：XTAL			1：320M,R/W,0x9[17]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_sdio(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_cksel_sdio(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->ckdiv_auxs:Frequency division : F/(1+N), N is the data of the reg value,R/W,0x9[21:18]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_auxs(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_ckdiv_auxs(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->cksel_auxs:0:DCO 			 1:APLL 			   2:320M					  4:480M,R/W,0x9[23:22]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_auxs(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_cksel_auxs(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->cksel_flash:0:XTAL			   1:APLL				1x :clk_120M,R/W,0x9[25:24]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_cksel_flash(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_cksel_flash(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->ckdiv_flash:0:/1  1:/2  2:/4  3:/8,R/W,0x9[27:26]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_flash(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_ckdiv_flash(sys_hw_t *hw, uint32_t value);

/* REG_0x09:cpu_clk_div_mode2->ckdiv_i2s0:0:/1	1:/2  2:/4	3:/8  4:/16  5:/32	6:/64  7:/256,R/W,0x9[30:28]*/
uint32_t sys_hal_get_cpu_clk_div_mode2_ckdiv_i2s0(sys_hw_t *hw);

void sys_hal_set_cpu_clk_div_mode2_ckdiv_i2s0(sys_hw_t *hw, uint32_t value);

/* REG_0x0a */

uint32_t sys_hal_get_cpu_26m_wdt_clk_div_value(sys_hw_t *hw);

void sys_hal_set_cpu_26m_wdt_clk_div_value(sys_hw_t *hw, uint32_t value);

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_26m:0:/1  1:/2  2:/4  3:/8,R/W,0xa[1:0]*/
uint32_t sys_hal_get_cpu_26m_wdt_clk_div_ckdiv_26m(sys_hw_t *hw);

void sys_hal_set_cpu_26m_wdt_clk_div_ckdiv_26m(sys_hw_t *hw, uint32_t value);

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_wdt:0:/2 1:/4 2:/8 3:/16,R/W,0xa[3:2]*/
uint32_t sys_hal_get_cpu_26m_wdt_clk_div_ckdiv_wdt(sys_hw_t *hw);

void sys_hal_set_cpu_26m_wdt_clk_div_ckdiv_wdt(sys_hw_t *hw, uint32_t value);

/* REG_0x0b */

uint32_t sys_hal_get_cpu_anaspi_freq_value(sys_hw_t *hw);

void sys_hal_set_cpu_anaspi_freq_value(sys_hw_t *hw, uint32_t value);

/* REG_0x0b:cpu_anaspi_freq->anaspi_freq: ,R/W,0xb[5:0]*/
uint32_t sys_hal_get_cpu_anaspi_freq_anaspi_freq(sys_hw_t *hw);

void sys_hal_set_cpu_anaspi_freq_anaspi_freq(sys_hw_t *hw, uint32_t value);

/* REG_0x0c */

uint32_t sys_hal_get_cpu_device_clk_enable_value(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_value(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->i2c0_cken:1:i2c0_clk enable,R/W,0xc[0]*/
uint32_t sys_hal_get_cpu_device_clk_enable_i2c0_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_i2c0_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->spi0_cken:1:spi0_clk enable ,R/W,0xc[1]*/
uint32_t sys_hal_get_cpu_device_clk_enable_spi0_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_spi0_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->uart0_cken:1:uart0_clk enable,R/W,0xc[2]*/
uint32_t sys_hal_get_cpu_device_clk_enable_uart0_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_uart0_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->pwm0_cken:1:pwm0_clk enable ,R/W,0xc[3]*/
uint32_t sys_hal_get_cpu_device_clk_enable_pwm0_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_pwm0_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->tim0_cken:1:tim0_clk enable ,R/W,0xc[4]*/
uint32_t sys_hal_get_cpu_device_clk_enable_tim0_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_tim0_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->sadc_cken:1:sadc_clk enable ,R/W,0xc[5]*/
uint32_t sys_hal_get_cpu_device_clk_enable_sadc_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_sadc_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->irda_cken:1:irda_clk enable ,R/W,0xc[6]*/
uint32_t sys_hal_get_cpu_device_clk_enable_irda_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_irda_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->efuse_cken:1:efuse_clk enable,R/W,0xc[7]*/
uint32_t sys_hal_get_cpu_device_clk_enable_efuse_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_efuse_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->i2c1_cken:1:i2c1_clk enable ,R/W,0xc[8]*/
uint32_t sys_hal_get_cpu_device_clk_enable_i2c1_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_i2c1_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->spi1_cken:1:spi1_clk enable ,R/W,0xc[9]*/
uint32_t sys_hal_get_cpu_device_clk_enable_spi1_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_spi1_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->uart1_cken:1:uart1_clk enable,R/W,0xc[10]*/
uint32_t sys_hal_get_cpu_device_clk_enable_uart1_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_uart1_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->uart2_cken:1:uart2_clk enable,R/W,0xc[11]*/
uint32_t sys_hal_get_cpu_device_clk_enable_uart2_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_uart2_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->pwm1_cken:1:pwm1_clk enable ,R/W,0xc[12]*/
uint32_t sys_hal_get_cpu_device_clk_enable_pwm1_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_pwm1_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->tim1_cken:1:tim1_clk enable ,R/W,0xc[13]*/
uint32_t sys_hal_get_cpu_device_clk_enable_tim1_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_tim1_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->tim2_cken:1:tim2_clk enable ,R/W,0xc[14]*/
uint32_t sys_hal_get_cpu_device_clk_enable_tim2_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_tim2_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->otp_cken:1:otp_clk enable  ,R/W,0xc[15]*/
uint32_t sys_hal_get_cpu_device_clk_enable_otp_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_otp_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->i2s_cken:1:i2s_clk enable  ,R/W,0xc[16]*/
uint32_t sys_hal_get_cpu_device_clk_enable_i2s_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_i2s_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->usb_cken:1:usb_clk enable  ,R/W,0xc[17]*/
uint32_t sys_hal_get_cpu_device_clk_enable_usb_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_usb_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->can_cken:1:can_clk enable  ,R/W,0xc[18]*/
uint32_t sys_hal_get_cpu_device_clk_enable_can_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_can_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->psram_cken:1:psram_clk enable,R/W,0xc[19]*/
uint32_t sys_hal_get_cpu_device_clk_enable_psram_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_psram_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->qspi0_cken:1:qspi0_clk enable,R/W,0xc[20]*/
uint32_t sys_hal_get_cpu_device_clk_enable_qspi0_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_qspi0_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->qspi1_cken:1:qspi1_clk enable,R/W,0xc[21]*/
uint32_t sys_hal_get_cpu_device_clk_enable_qspi1_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_qspi1_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->sdio_cken:1:sdio_clk enable ,R/W,0xc[22]*/
uint32_t sys_hal_get_cpu_device_clk_enable_sdio_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_sdio_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->auxs_cken:1:auxs_clk enable ,R/W,0xc[23]*/
uint32_t sys_hal_get_cpu_device_clk_enable_auxs_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_auxs_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->btdm_cken:1:btdm_clk enable ,R/W,0xc[24]*/
uint32_t sys_hal_get_cpu_device_clk_enable_btdm_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_btdm_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->xvr_cken:1:xvr_clk enable  ,R/W,0xc[25]*/
uint32_t sys_hal_get_cpu_device_clk_enable_xvr_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_xvr_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->mac_cken:1:mac_clk enable  ,R/W,0xc[26]*/
uint32_t sys_hal_get_cpu_device_clk_enable_mac_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_mac_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->phy_cken:1:phy_clk enable  ,R/W,0xc[27]*/
uint32_t sys_hal_get_cpu_device_clk_enable_phy_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_phy_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->jpeg_cken:1:jpeg_clk enable ,R/W,0xc[28]*/
uint32_t sys_hal_get_cpu_device_clk_enable_jpeg_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_jpeg_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->disp_cken:1:disp_clk enable ,R/W,0xc[29]*/
uint32_t sys_hal_get_cpu_device_clk_enable_disp_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_disp_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->aud_cken:1:aud_clk enable  ,R/W,0xc[30]*/
uint32_t sys_hal_get_cpu_device_clk_enable_aud_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_aud_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0c:cpu_device_clk_enable->wdt_cken:1:wdt_clk enable  ,R/W,0xc[31]*/
uint32_t sys_hal_get_cpu_device_clk_enable_wdt_cken(sys_hw_t *hw);

void sys_hal_set_cpu_device_clk_enable_wdt_cken(sys_hw_t *hw, uint32_t value);

/* REG_0x0d */

/* REG_0x0e */

uint32_t sys_hal_get_cpu_mode_disckg1_value(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_value(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->aon_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[0]*/
uint32_t sys_hal_get_cpu_mode_disckg1_aon_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_aon_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->sys_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[1]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sys_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_sys_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->dma_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[2]*/
uint32_t sys_hal_get_cpu_mode_disckg1_dma_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_dma_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->flash_disckg:BUS_CLK	ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[3]*/
uint32_t sys_hal_get_cpu_mode_disckg1_flash_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_flash_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->wdt_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[4]*/
uint32_t sys_hal_get_cpu_mode_disckg1_wdt_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_wdt_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->tim_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[5]*/
uint32_t sys_hal_get_cpu_mode_disckg1_tim_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_tim_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->urt_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[6]*/
uint32_t sys_hal_get_cpu_mode_disckg1_urt_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_urt_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->pwm_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[7]*/
uint32_t sys_hal_get_cpu_mode_disckg1_pwm_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_pwm_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->i2c_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[8]*/
uint32_t sys_hal_get_cpu_mode_disckg1_i2c_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_i2c_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->spi_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[9]*/
uint32_t sys_hal_get_cpu_mode_disckg1_spi_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_spi_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->sadc_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[10]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sadc_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_sadc_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->efs_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[11]*/
uint32_t sys_hal_get_cpu_mode_disckg1_efs_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_efs_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->irda_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[12]*/
uint32_t sys_hal_get_cpu_mode_disckg1_irda_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_irda_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->trng_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[13]*/
uint32_t sys_hal_get_cpu_mode_disckg1_trng_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_trng_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->sdio_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[14]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sdio_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_sdio_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->LA_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[15]*/
uint32_t sys_hal_get_cpu_mode_disckg1_la_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_la_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->tim1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[16]*/
uint32_t sys_hal_get_cpu_mode_disckg1_tim1_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_tim1_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->urt1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[17]*/
uint32_t sys_hal_get_cpu_mode_disckg1_urt1_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_urt1_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->urt2_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[18]*/
uint32_t sys_hal_get_cpu_mode_disckg1_urt2_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_urt2_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->pwm1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[19]*/
uint32_t sys_hal_get_cpu_mode_disckg1_pwm1_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_pwm1_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->i2c1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[20]*/
uint32_t sys_hal_get_cpu_mode_disckg1_i2c1_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_i2c1_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->spi1_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[21]*/
uint32_t sys_hal_get_cpu_mode_disckg1_spi1_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_spi1_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->usb_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[22]*/
uint32_t sys_hal_get_cpu_mode_disckg1_usb_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_usb_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->can_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[23]*/
uint32_t sys_hal_get_cpu_mode_disckg1_can_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_can_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->qspi0_disckg:BUS_CLK	ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[24]*/
uint32_t sys_hal_get_cpu_mode_disckg1_qspi0_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_qspi0_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->psram_disckg:BUS_CLK	ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[25]*/
uint32_t sys_hal_get_cpu_mode_disckg1_psram_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_psram_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->fft_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[26]*/
uint32_t sys_hal_get_cpu_mode_disckg1_fft_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_fft_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->sbc_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[27]*/
uint32_t sys_hal_get_cpu_mode_disckg1_sbc_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_sbc_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->aud_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[28]*/
uint32_t sys_hal_get_cpu_mode_disckg1_aud_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_aud_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->i2s_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[29]*/
uint32_t sys_hal_get_cpu_mode_disckg1_i2s_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_i2s_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->jpeg_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[30]*/
uint32_t sys_hal_get_cpu_mode_disckg1_jpeg_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_jpeg_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0e:cpu_mode_disckg1->jpeg_dec_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xe[31]*/
uint32_t sys_hal_get_cpu_mode_disckg1_jpeg_dec_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg1_jpeg_dec_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0f */

uint32_t sys_hal_get_cpu_mode_disckg2_value(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg2_value(sys_hw_t *hw, uint32_t value);

/* REG_0x0f:cpu_mode_disckg2->disp_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[0]*/
uint32_t sys_hal_get_cpu_mode_disckg2_disp_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg2_disp_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0f:cpu_mode_disckg2->dma2d_disckg:BUS_CLK	ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[1]*/
uint32_t sys_hal_get_cpu_mode_disckg2_dma2d_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg2_dma2d_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0f:cpu_mode_disckg2->btdm_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[3]*/
uint32_t sys_hal_get_cpu_mode_disckg2_btdm_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg2_btdm_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0f:cpu_mode_disckg2->xver_disckg:BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[4]*/
uint32_t sys_hal_get_cpu_mode_disckg2_xver_disckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg2_xver_disckg(sys_hw_t *hw, uint32_t value);

/* REG_0x0f:cpu_mode_disckg2->btdm_bps_ckg:BUS_CLK	ENABLE,0: bus clock open when module is select,1:bus clock always open,R/W,0xf[8:5]*/
uint32_t sys_hal_get_cpu_mode_disckg2_btdm_bps_ckg(sys_hw_t *hw);

void sys_hal_set_cpu_mode_disckg2_btdm_bps_ckg(sys_hw_t *hw, uint32_t value);

/* REG_0x10 */

uint32_t sys_hal_get_cpu_power_sleep_wakeup_value(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_value(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem1:0:power on of mem1 	 ,RW,0x10[0]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_mem1(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_mem1(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem2:0:power on of mem2 	 ,RW,0x10[1]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_mem2(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_mem2(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem3:0:power on of mem3 	 ,RW,0x10[2]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_mem3(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_mem3(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_encp:0:power on of encp 	 ,RW,0x10[3]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_encp(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_encp(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_bakp:0:power on of bakp 	 ,RW,0x10[4]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_bakp(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_bakp(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_ahbp:0:power on of ahbp 	 ,RW,0x10[5]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_ahbp(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_ahbp(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_audp:0:power on of audp 	 ,RW,0x10[6]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_audp(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_audp(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_vidp:0:power on of vidp 	 ,RW,0x10[7]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_vidp(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_vidp(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_btsp:0:power on of btsp 	 ,RW,0x10[8]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_btsp(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_btsp(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_mac:0:power on of wifp_mac  ,RW,0x10[9]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_wifp_mac(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_wifp_mac(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_phy:0:power on of wifp_phy  ,RW,0x10[10]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_pwd_wifp_phy(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_pwd_wifp_phy(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_flash_idle:0:sleep_en of flash_idle,RW,0x10[16]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu1_wfi:0:sleep_en of cpu1_wfi  ,RW,0x10[17]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu0_wfi:0:sleep_en of cpu0_wfi  ,RW,0x10[18]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_global:0:sleep_en of global	,RW,0x10[19]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_sleep_en_global(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_sleep_en_global(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->wifi_wakeup_platform_en:0:wifi_wakeup_en		,RW,0x10[20]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->bts_wakeup_platform_en:0:bts_wakeup_en		   ,RW,0x10[21]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(sys_hw_t *hw, uint32_t value);

/* REG_0x10:cpu_power_sleep_wakeup->bts_sleep_exit_req:0:bt sleep exit request ,RW,0x10[22]*/
uint32_t sys_hal_get_cpu_power_sleep_wakeup_bts_sleep_exit_req(sys_hw_t *hw);

void sys_hal_set_cpu_power_sleep_wakeup_bts_sleep_exit_req(sys_hw_t *hw, uint32_t value);

/* REG_0x11 */

/* REG_0x20 */

uint32_t sys_hal_get_cpu0_int_0_31_en_value(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_value(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_bmc32_int_en: ,R/W,0x20[0]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_bmc32_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_bmc32_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_irq_en: ,R/W,0x20[1]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_host_0_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_host_0_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_sec_irq_en: ,R/W,0x20[2]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer_int_en: ,R/W,0x20[3]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_timer_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_timer_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart_int_en: ,R/W,0x20[4]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_uart_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_uart_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_pwm_int_en: ,R/W,0x20[5]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_pwm_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_pwm_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c_int_en: ,R/W,0x20[6]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_i2c_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_i2c_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi_int_en: ,R/W,0x20[7]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_spi_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_spi_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_sadc_int_en: ,R/W,0x20[8]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_sadc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_sadc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_irda_int_en: ,R/W,0x20[9]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_irda_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_irda_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_sdio_int_en: ,R/W,0x20[10]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_sdio_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_sdio_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_gdma_int_en: ,R/W,0x20[11]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_gdma_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_gdma_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_la_int_en: ,R/W,0x20[12]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_la_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_la_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer1_int_en: ,R/W,0x20[13]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_timer1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_timer1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c1_int_en: ,R/W,0x20[14]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart1_int_en: ,R/W,0x20[15]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_uart1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_uart1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart2_int_en: ,R/W,0x20[16]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_uart2_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_uart2_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi1_int_en: ,R/W,0x20[17]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_spi1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_spi1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_can_int_en: ,R/W,0x20[18]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_can_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_can_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_usb_int_en: ,R/W,0x20[19]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_usb_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_usb_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_qspi_int_en: ,R/W,0x20[20]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_qspi_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_qspi_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_fft_int_en: ,R/W,0x20[21]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_fft_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_fft_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_sbc_int_en: ,R/W,0x20[22]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_sbc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_sbc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_aud_int_en: ,R/W,0x20[23]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_aud_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_aud_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2s_int_en: ,R/W,0x20[24]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_i2s_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_i2s_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegenc_int_en: ,R/W,0x20[25]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegdec_int_en: ,R/W,0x20[26]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_lcd_int_en: ,R/W,0x20[27]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_lcd_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_lcd_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_int_phy_en: ,R/W,0x20[29]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_mac_int_tx_rx_timer_en: ,R/W,0x20[30]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw, uint32_t value);

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_mac_int_tx_rx_misc_en: ,R/W,0x20[31]*/
uint32_t sys_hal_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21 */

uint32_t sys_hal_get_cpu0_int_32_63_en_value(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_value(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_rx_trigger_en: ,R/W,0x21[0]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_tx_trigger_en: ,R/W,0x21[1]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_port_trigger_en: ,R/W,0x21[2]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_port_trigger_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_port_trigger_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_gen_en: ,R/W,0x21[3]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_hsu_irq_en: ,R/W,0x21[4]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_int_mac_wakeup_en: ,R/W,0x21[5]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_dm_irq_en: ,R/W,0x21[7]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_dm_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_dm_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_ble_irq_en: ,R/W,0x21[8]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_ble_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_ble_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_bt_irq_en: ,R/W,0x21[9]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_bt_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_bt_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox0_int_en: ,R/W,0x21[15]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox1_int_en: ,R/W,0x21[16]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_bmc64_int_en: ,R/W,0x21[17]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_touched_int_en: ,R/W,0x21[19]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_touched_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_touched_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_usbplug_int_en: ,R/W,0x21[20]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_rtc_int_en: ,R/W,0x21[21]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_rtc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_rtc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x21:cpu0_int_32_63_en->cpu0_gpio_int_en: ,R/W,0x21[22]*/
uint32_t sys_hal_get_cpu0_int_32_63_en_cpu0_gpio_int_en(sys_hw_t *hw);

void sys_hal_set_cpu0_int_32_63_en_cpu0_gpio_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22 */

uint32_t sys_hal_get_cpu1_int_0_31_en_value(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_value(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_bmc32_int_en: ,R/W,0x22[0]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_bmc32_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_bmc32_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_irq_en: ,R/W,0x22[1]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_host_0_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_host_0_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_sec_irq_en: ,R/W,0x22[2]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer_int_en: ,R/W,0x22[3]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_timer_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_timer_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart_int_en: ,R/W,0x22[4]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_uart_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_uart_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_pwm_int_en: ,R/W,0x22[5]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_pwm_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_pwm_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c_int_en: ,R/W,0x22[6]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_i2c_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_i2c_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi_int_en: ,R/W,0x22[7]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_spi_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_spi_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_sadc_int_en: ,R/W,0x22[8]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_sadc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_sadc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_irda_int_en: ,R/W,0x22[9]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_irda_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_irda_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_sdio_int_en: ,R/W,0x22[10]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_sdio_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_sdio_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_gdma_int_en: ,R/W,0x22[11]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_gdma_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_gdma_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_la_int_en: ,R/W,0x22[12]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_la_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_la_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer1_int_en: ,R/W,0x22[13]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_timer1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_timer1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c1_int_en: ,R/W,0x22[14]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart1_int_en: ,R/W,0x22[15]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_uart1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_uart1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart2_int_en: ,R/W,0x22[16]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_uart2_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_uart2_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi1_int_en: ,R/W,0x22[17]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_spi1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_spi1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_can_int_en: ,R/W,0x22[18]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_can_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_can_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_usb_int_en: ,R/W,0x22[19]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_usb_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_usb_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_qspi_int_en: ,R/W,0x22[20]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_qspi_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_qspi_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_fft_int_en: ,R/W,0x22[21]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_fft_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_fft_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_sbc_int_en: ,R/W,0x22[22]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_sbc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_sbc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_aud_int_en: ,R/W,0x22[23]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_aud_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_aud_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2s_int_en: ,R/W,0x22[24]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_i2s_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_i2s_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegenc_int_en: ,R/W,0x22[25]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegdec_int_en: ,R/W,0x22[26]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_lcd_int_en: ,R/W,0x22[27]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_lcd_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_lcd_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_int_phy_en: ,R/W,0x22[29]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_mac_int_tx_rx_timer_en: ,R/W,0x22[30]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw, uint32_t value);

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_mac_int_tx_rx_misc_en: ,R/W,0x22[31]*/
uint32_t sys_hal_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23 */

uint32_t sys_hal_get_cpu1_int_32_63_en_value(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_value(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_rx_trigger_en: ,R/W,0x23[0]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_tx_trigger_en: ,R/W,0x23[1]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_port_trigger_en: ,R/W,0x23[2]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_port_trigger_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_port_trigger_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_gen_en: ,R/W,0x23[3]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_hsu_irq_en: ,R/W,0x23[4]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_int_mac_wakeup_en: ,R/W,0x23[5]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_dm_irq_en: ,R/W,0x23[7]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_dm_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_dm_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_ble_irq_en: ,R/W,0x23[8]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_ble_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_ble_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_bt_irq_en: ,R/W,0x23[9]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_bt_irq_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_bt_irq_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox0_int_en: ,R/W,0x23[15]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox1_int_en: ,R/W,0x23[16]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_bmc64_int_en: ,R/W,0x23[17]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_touched_int_en: ,R/W,0x23[19]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_touched_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_touched_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_usbplug_int_en: ,R/W,0x23[20]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_rtc_int_en: ,R/W,0x23[21]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_rtc_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_rtc_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x23:cpu1_int_32_63_en->cpu1_gpio_int_en: ,R/W,0x23[22]*/
uint32_t sys_hal_get_cpu1_int_32_63_en_cpu1_gpio_int_en(sys_hw_t *hw);

void sys_hal_set_cpu1_int_32_63_en_cpu1_gpio_int_en(sys_hw_t *hw, uint32_t value);

/* REG_0x28 */

uint32_t sys_hal_get_cpu0_int_0_31_status_value(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_bmc32_int_st: ,R,0x28[0]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_bmc32_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_irq_st: ,R,0x28[1]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_host_0_irq_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_sec_irq_st: ,R,0x28[2]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_host_0_sec_irq_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer_int_st: ,R,0x28[3]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_timer_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart_int_st: ,R,0x28[4]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_uart_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_pwm_int_st: ,R,0x28[5]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_pwm_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c_int_st: ,R,0x28[6]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_i2c_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi_int_st: ,R,0x28[7]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_spi_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_sadc_int_st: ,R,0x28[8]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_sadc_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_irda_int_st: ,R,0x28[9]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_irda_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_sdio_int_st: ,R,0x28[10]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_sdio_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_gdma_int_st: ,R,0x28[11]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_gdma_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_la_int_st: ,R,0x28[12]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_la_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer1_int_st: ,R,0x28[13]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_timer1_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c1_int_st: ,R,0x28[14]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart1_int_st: ,R,0x28[15]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_uart1_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart2_int_st: ,R,0x28[16]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_uart2_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi1_int_st: ,R,0x28[17]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_spi1_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_can_int_st: ,R,0x28[18]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_can_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_usb_int_st: ,R,0x28[19]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_usb_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_qspi_int_st: ,R,0x28[20]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_qspi_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_fft_int_st: ,R,0x28[21]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_fft_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_sbc_int_st: ,R,0x28[22]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_sbc_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_aud_int_st: ,R,0x28[23]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_aud_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2s_int_st: ,R,0x28[24]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_i2s_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegenc_int_st: ,R,0x28[25]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegdec_int_st: ,R,0x28[26]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_lcd_int_st: ,R,0x28[27]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_lcd_int_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_int_phy_st: ,R,0x28[29]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_mac_int_tx_rx_timer_st: ,R,0x28[30]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(sys_hw_t *hw);

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_mac_int_tx_rx_misc_st: ,R,0x28[31]*/
uint32_t sys_hal_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_misc_st(sys_hw_t *hw);

/* REG_0x29 */

uint32_t sys_hal_get_cpu0_int_32_63_status_value(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_rx_trigger_st: ,R,0x29[0]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_tx_trigger_st: ,R,0x29[1]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_port_trigger_st: ,R,0x29[2]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_port_trigger_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_gen_st: ,R,0x29[3]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_hsu_irq_st: ,R,0x29[4]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_int_mac_wakeup_st: ,R,0x29[5]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_dm_irq_st: ,R,0x29[7]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_dm_irq_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_ble_irq_st: ,R,0x29[8]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_ble_irq_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_bt_irq_st: ,R,0x29[9]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_bt_irq_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox0_int_st: ,R,0x29[15]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox1_int_st: ,R,0x29[16]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_bmc64_int_st: ,R,0x29[17]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_touched_int_st: ,R,0x29[19]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_touched_int_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_usbplug_int_st: ,R,0x29[20]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_rtc_int_st: ,R,0x29[21]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_rtc_int_st(sys_hw_t *hw);

/* REG_0x29:cpu0_int_32_63_status->cpu0_gpio_int_st: ,R,0x29[22]*/
uint32_t sys_hal_get_cpu0_int_32_63_status_cpu0_gpio_int_st(sys_hw_t *hw);

/* REG_0x2a */

uint32_t sys_hal_get_cpu1_int_0_31_status_value(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_bmc32_int_st: ,R,0x2a[0]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_bmc32_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_irq_st: ,R,0x2a[1]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_host_0_irq_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_sec_irq_st: ,R,0x2a[2]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_host_0_sec_irq_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer_int_st: ,R,0x2a[3]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_timer_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart_int_st: ,R,0x2a[4]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_uart_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_pwm_int_st: ,R,0x2a[5]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_pwm_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c_int_st: ,R,0x2a[6]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_i2c_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi_int_st: ,R,0x2a[7]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_spi_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sadc_int_st: ,R,0x2a[8]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_sadc_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_irda_int_st: ,R,0x2a[9]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_irda_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sdio_int_st: ,R,0x2a[10]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_sdio_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_gdma_int_st: ,R,0x2a[11]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_gdma_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_la_int_st: ,R,0x2a[12]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_la_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer1_int_st: ,R,0x2a[13]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_timer1_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c1_int_st: ,R,0x2a[14]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart1_int_st: ,R,0x2a[15]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_uart1_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart2_int_st: ,R,0x2a[16]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_uart2_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi1_int_st: ,R,0x2a[17]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_spi1_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_can_int_st: ,R,0x2a[18]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_can_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_usb_int_st: ,R,0x2a[19]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_usb_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_qspi_int_st: ,R,0x2a[20]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_qspi_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_fft_int_st: ,R,0x2a[21]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_fft_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sbc_int_st: ,R,0x2a[22]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_sbc_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_aud_int_st: ,R,0x2a[23]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_aud_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2s_int_st: ,R,0x2a[24]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_i2s_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegenc_int_st: ,R,0x2a[25]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegdec_int_st: ,R,0x2a[26]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_lcd_int_st: ,R,0x2a[27]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_lcd_int_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_int_phy_st: ,R,0x2a[29]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_mac_int_tx_rx_timer_st: ,R,0x2a[30]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(sys_hw_t *hw);

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_mac_int_tx_rx_misc_st: ,R,0x2a[31]*/
uint32_t sys_hal_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_misc_st(sys_hw_t *hw);

/* REG_0x2b */

uint32_t sys_hal_get_cpu1_int_32_63_status_value(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_rx_trigger_st: ,R,0x2b[0]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_tx_trigger_st: ,R,0x2b[1]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_port_trigger_st: ,R,0x2b[2]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_port_trigger_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_gen_st: ,R,0x2b[3]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_hsu_irq_st: ,R,0x2b[4]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_int_mac_wakeup_st: ,R,0x2b[5]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_dm_irq_st: ,R,0x2b[7]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_dm_irq_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_ble_irq_st: ,R,0x2b[8]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_ble_irq_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bt_irq_st: ,R,0x2b[9]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_bt_irq_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox0_int_st: ,R,0x2b[15]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox1_int_st: ,R,0x2b[16]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bmc64_int_st: ,R,0x2b[17]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_touched_int_st: ,R,0x2b[19]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_touched_int_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_usbplug_int_st: ,R,0x2b[20]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_rtc_int_st: ,R,0x2b[21]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_rtc_int_st(sys_hw_t *hw);

/* REG_0x2b:cpu1_int_32_63_status->cpu1_gpio_int_st: ,R,0x2b[22]*/
uint32_t sys_hal_get_cpu1_int_32_63_status_cpu1_gpio_int_st(sys_hw_t *hw);

/* REG_0x30 */

uint32_t sys_hal_get_gpio_config0_value(sys_hw_t *hw);

void sys_hal_set_gpio_config0_value(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio0:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[3:0]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio0(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio0(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio1:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[7:4]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio1(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio1(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio2:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[11:8]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio2(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio2(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio3:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[15:12]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio3(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio3(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio4:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[19:16]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio4(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio4(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio5:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[23:20]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio5(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio5(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio6:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[27:24]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio6(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio6(sys_hw_t *hw, uint32_t value);

/* REG_0x30:gpio_config0->sys_gpio7:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x30[31:28]*/
uint32_t sys_hal_get_gpio_config0_sys_gpio7(sys_hw_t *hw);

void sys_hal_set_gpio_config0_sys_gpio7(sys_hw_t *hw, uint32_t value);

/* REG_0x31 */

uint32_t sys_hal_get_gpio_config1_value(sys_hw_t *hw);

void sys_hal_set_gpio_config1_value(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio8:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[3:0]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio8(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio8(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio9:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[7:4]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio9(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio9(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio10:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[11:8]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio10(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio10(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio11:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[15:12]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio11(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio11(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio12:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[19:16]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio12(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio12(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio13:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[23:20]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio13(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio13(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio14:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[27:24]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio14(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio14(sys_hw_t *hw, uint32_t value);

/* REG_0x31:gpio_config1->sys_gpio15:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x31[31:28]*/
uint32_t sys_hal_get_gpio_config1_sys_gpio15(sys_hw_t *hw);

void sys_hal_set_gpio_config1_sys_gpio15(sys_hw_t *hw, uint32_t value);

/* REG_0x32 */

uint32_t sys_hal_get_gpio_config2_value(sys_hw_t *hw);

void sys_hal_set_gpio_config2_value(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio16:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[3:0]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio16(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio16(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio17:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[7:4]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio17(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio17(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio18:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[11:8]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio18(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio18(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio19:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[15:12]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio19(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio19(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio20:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[19:16]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio20(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio20(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio21:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[23:20]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio21(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio21(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio22:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[27:24]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio22(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio22(sys_hw_t *hw, uint32_t value);

/* REG_0x32:gpio_config2->sys_gpio23:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x32[31:28]*/
uint32_t sys_hal_get_gpio_config2_sys_gpio23(sys_hw_t *hw);

void sys_hal_set_gpio_config2_sys_gpio23(sys_hw_t *hw, uint32_t value);

/* REG_0x33 */

uint32_t sys_hal_get_gpio_config3_value(sys_hw_t *hw);

void sys_hal_set_gpio_config3_value(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio24:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[3:0]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio24(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio24(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio25:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[7:4]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio25(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio25(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio26:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[11:8]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio26(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio26(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio27:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[15:12]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio27(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio27(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio28:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[19:16]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio28(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio28(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio29:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[23:20]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio29(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio29(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio30:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[27:24]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio30(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio30(sys_hw_t *hw, uint32_t value);

/* REG_0x33:gpio_config3->sys_gpio31:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x33[31:28]*/
uint32_t sys_hal_get_gpio_config3_sys_gpio31(sys_hw_t *hw);

void sys_hal_set_gpio_config3_sys_gpio31(sys_hw_t *hw, uint32_t value);

/* REG_0x34 */

uint32_t sys_hal_get_gpio_config4_value(sys_hw_t *hw);

void sys_hal_set_gpio_config4_value(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio32:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[3:0]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio32(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio32(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio33:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[7:4]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio33(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio33(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio34:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[11:8]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio34(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio34(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio35:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[15:12]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio35(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio35(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio36:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[19:16]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio36(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio36(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio37:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[23:20]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio37(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio37(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio38:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[27:24]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio38(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio38(sys_hw_t *hw, uint32_t value);

/* REG_0x34:gpio_config4->sys_gpio39:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x34[31:28]*/
uint32_t sys_hal_get_gpio_config4_sys_gpio39(sys_hw_t *hw);

void sys_hal_set_gpio_config4_sys_gpio39(sys_hw_t *hw, uint32_t value);

/* REG_0x35 */

uint32_t sys_hal_get_gpio_config5_value(sys_hw_t *hw);

void sys_hal_set_gpio_config5_value(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio40:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[3:0]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio40(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio40(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio41:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[7:4]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio41(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio41(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio42:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[11:8]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio42(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio42(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio43:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[15:12]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio43(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio43(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio44:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[19:16]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio44(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio44(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio45:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[23:20]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio45(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio45(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio46:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[27:24]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio46(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio46(sys_hw_t *hw, uint32_t value);

/* REG_0x35:gpio_config5->sys_gpio47:0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,R/W,0x35[31:28]*/
uint32_t sys_hal_get_gpio_config5_sys_gpio47(sys_hw_t *hw);

void sys_hal_set_gpio_config5_sys_gpio47(sys_hw_t *hw, uint32_t value);

/* REG_0x38 */

uint32_t sys_hal_get_sys_debug_config0_value(sys_hw_t *hw);

void sys_hal_set_sys_debug_config0_value(sys_hw_t *hw, uint32_t value);

/* REG_0x38:sys_debug_config0->dbug_config0: ,R/W,0x38[31:0]*/
uint32_t sys_hal_get_sys_debug_config0_dbug_config0(sys_hw_t *hw);

void sys_hal_set_sys_debug_config0_dbug_config0(sys_hw_t *hw, uint32_t value);

/* REG_0x39 */

uint32_t sys_hal_get_sys_debug_config1_value(sys_hw_t *hw);

void sys_hal_set_sys_debug_config1_value(sys_hw_t *hw, uint32_t value);

/* REG_0x39:sys_debug_config1->dbug_config1:0: btsp_debug[0:32] 	   1: btsp_debug[32+:32]		   2: btsp_debug[64+:32]  4:btsp_debug[96+:6]		5:wifip_mac_dbg[31:0]			6: wifip_phy_dbg[31:0]							  default:	dbug_config0				   ,R/W,0x39[31:0]*/
uint32_t sys_hal_get_sys_debug_config1_dbug_config1(sys_hw_t *hw);

void sys_hal_set_sys_debug_config1_dbug_config1(sys_hw_t *hw, uint32_t value);

/* REG_0x40 */

void sys_hal_set_ana_reg0_value(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->ck2652sel:1:26MHz/0:52MHz,W,0x40[0]*/
void sys_hal_set_ana_reg0_ck2652sel(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->cp:cp curent control 0to 350uA 50uA step,W,0x40[3:1]*/
void sys_hal_set_ana_reg0_cp(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->spideten:unlock detect enable fron spi 1:enable,W,0x40[4]*/
void sys_hal_set_ana_reg0_spideten(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->hvref:high vth control for unlock detect 00:0.85V;01:0.9V;10:0.95V;11:1.05V,W,0x40[6:5]*/
void sys_hal_set_ana_reg0_hvref(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->lvref:low vth control for unlock detect 00:0.2V;01:0.3V;10:0.35V;11:0.4V,W,0x40[8:7]*/
void sys_hal_set_ana_reg0_lvref(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->Rzctrl26M:Rz ctrl in 26M mode:1:normal;0:add 14K,W,0x40[9]*/
void sys_hal_set_ana_reg0_rzctrl26m(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->LoopRzctrl:Rz ctrl:2K to 17K,1K step,W,0x40[13:10]*/
void sys_hal_set_ana_reg0_looprzctrl(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->rpc:second pole Rp ctrl:00:30K;01:10K;10:22K;11:2K,W,0x40[15:14]*/
void sys_hal_set_ana_reg0_rpc(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->nsyn:N divider rst,W,0x40[16]*/
void sys_hal_set_ana_reg0_nsyn(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->cksel:0:26M;1:40M;2:24M;3:19.2M,W,0x40[18:17]*/
void sys_hal_set_ana_reg0_cksel(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->spitrig:SPI band selection trigger signal,W,0x40[19]*/
void sys_hal_set_ana_reg0_spitrig(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->band:band manual value/band[0] ,W,0x40[24:20]*/
void sys_hal_set_ana_reg0_band(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->bandmanual:1:band manual;0:band auto,W,0x40[25]*/
void sys_hal_set_ana_reg0_bandmanual(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->dsptrig:band selection trigger signal,W,0x40[26]*/
void sys_hal_set_ana_reg0_dsptrig(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->lpen_dpll:dpll low power mode enable,W,0x40[27]*/
void sys_hal_set_ana_reg0_lpen_dpll(sys_hw_t *hw, uint32_t value);

/* REG_0x40:ana_reg0->xamp:xtal OSC amp control/xamp<0> shared with pll_cktst_en,W,0x40[31:28]*/
void sys_hal_set_ana_reg0_xamp(sys_hw_t *hw, uint32_t value);

/* REG_0x41 */

void sys_hal_set_ana_reg1_value(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->dpll_vrefsel:dpll ldo reference voltage selection  0:vbg_aon/1:vbg_cal,W,0x41[1]*/
void sys_hal_set_ana_reg1_dpll_vrefsel(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->msw:set the frequency of DCO manual,W,0x41[10:2]*/
void sys_hal_set_ana_reg1_msw(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->ictrl:controlling the bias cuttent of DCO core,W,0x41[13:11]*/
void sys_hal_set_ana_reg1_ictrl(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->osc_trig:reset the DCO core by spi to make it oscillate again,W,0x41[14]*/
void sys_hal_set_ana_reg1_osc_trig(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->osccal_trig:trigger the action of callibration in the DCO,W,0x41[15]*/
void sys_hal_set_ana_reg1_osccal_trig(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->cnti:set the controlling work of calibration in the DCO block to get the different frequency,W,0x41[24:16]*/
void sys_hal_set_ana_reg1_cnti(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->spi_rst:reset the calibration block of DCO by spi,W,0x41[25]*/
void sys_hal_set_ana_reg1_spi_rst(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->amsel:disable the calibration function of the DCO,set the frequency of DCO manual,W,0x41[26]*/
void sys_hal_set_ana_reg1_amsel(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->divctrl:controlling the value of divider in the DCO to get the different frequency,W,0x41[29:27]*/
void sys_hal_set_ana_reg1_divctrl(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->dco_tsten:dco test enable,W,0x41[30]*/
void sys_hal_set_ana_reg1_dco_tsten(sys_hw_t *hw, uint32_t value);

/* REG_0x41:ana_reg1->rosc_tsten:rosc test enable,W,0x41[31]*/
void sys_hal_set_ana_reg1_rosc_tsten(sys_hw_t *hw, uint32_t value);

/* REG_0x42 */

void sys_hal_set_ana_reg2_value(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->pwmscmen:buck nmos disable,W,0x42[0]*/
void sys_hal_set_ana_reg2_pwmscmen(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->buck_fasten:buck EA fast transient enable(=1),W,0x42[1]*/
void sys_hal_set_ana_reg2_buck_fasten(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->cls:buck current limit setting,W,0x42[4:2]*/
void sys_hal_set_ana_reg2_cls(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->pfms:buck freewheeling damping enable(=1) ,W,0x42[9:5]*/
void sys_hal_set_ana_reg2_pfms(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->ripc:buck pfm mode voltage ripple control setting,W,0x42[12:10]*/
void sys_hal_set_ana_reg2_ripc(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->rampc:buck ramping compensation setting,W,0x42[16:13]*/
void sys_hal_set_ana_reg2_rampc(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->rampcen:buck ramping compensation enable(=1),W,0x42[17]*/
void sys_hal_set_ana_reg2_rampcen(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->dpfmen:buck pfm mode current reduce enable(=1),W,0x42[18]*/
void sys_hal_set_ana_reg2_dpfmen(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->pfmen:buck pfm mode enable(=1),W,0x42[19]*/
void sys_hal_set_ana_reg2_pfmen(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->forcepfm:buck force pfm mode(=1),W,0x42[20]*/
void sys_hal_set_ana_reg2_forcepfm(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->swrsten:buck freewheeling damping enable(=1) ,W,0x42[21]*/
void sys_hal_set_ana_reg2_swrsten(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->tmposel:buck mpo pulse width control 0--shortest	 3---longest,W,0x42[23:22]*/
void sys_hal_set_ana_reg2_tmposel(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->mpoen:buck mpo mode enable( =1),W,0x42[24]*/
void sys_hal_set_ana_reg2_mpoen(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->spi_latchb:spi latch disable 0:latch;1:no latch,W,0x42[25]*/
void sys_hal_set_ana_reg2_spi_latchb(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->ldosel:ldo/buck select, 0:buck;1:LDO,W,0x42[26]*/
void sys_hal_set_ana_reg2_ldosel(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->iovoc:ioldo output voltage select 0:2.9V,….7:3.6V,W,0x42[29:27]*/
void sys_hal_set_ana_reg2_iovoc(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->vbpbuf_hp:vbspbuffer high power enable,W,0x42[30]*/
void sys_hal_set_ana_reg2_vbpbuf_hp(sys_hw_t *hw, uint32_t value);

/* REG_0x42:ana_reg2->bypassen:ioldo bypass enable,W,0x42[31]*/
void sys_hal_set_ana_reg2_bypassen(sys_hw_t *hw, uint32_t value);

/* REG_0x43 */

void sys_hal_set_ana_reg3_value(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->zcdta:buck zcd delay tune setting,W,0x43[4:0]*/
void sys_hal_set_ana_reg3_zcdta(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->zcdcala:buck zcd offset cali setting,W,0x43[10:5]*/
void sys_hal_set_ana_reg3_zcdcala(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->zcdmen:buck zcd manual cali enable(=1),W,0x43[11]*/
void sys_hal_set_ana_reg3_zcdmen(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->zcdcalen:buck zcd calibration enable(=1),W,0x43[12]*/
void sys_hal_set_ana_reg3_zcdcalen(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->zcdcal_tri:buck zcd auto cali triggle(0-->1),W,0x43[13]*/
void sys_hal_set_ana_reg3_zcdcal_tri(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->mroscsel:buck oscillator manual cali. enable(=1),W,0x43[14]*/
void sys_hal_set_ana_reg3_mroscsel(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->mfsel:buck oscillator manual fsel  ,W,0x43[17:15]*/
void sys_hal_set_ana_reg3_mfsel(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->mroscbcal:buck oscillator manual cap_cal	0xA---500k 0xB--1M 0x9---2M,W,0x43[21:18]*/
void sys_hal_set_ana_reg3_mroscbcal(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->osccaltrig:buck oscillator manual cali. enable(=1),W,0x43[22]*/
void sys_hal_set_ana_reg3_osccaltrig(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->ckintsel:buck clock source select  1-- ring oscillator   0--divider,W,0x43[23]*/
void sys_hal_set_ana_reg3_ckintsel(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->ckfs:buck output clock freq. select	0--500k 1---1M	2--2M  3--4M,W,0x43[25:24]*/
void sys_hal_set_ana_reg3_ckfs(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->vlsel_ldodig:digldo output voltage select(low power)	0:0.6V,…..7:1.4V,W,0x43[28:26]*/
void sys_hal_set_ana_reg3_vlsel_ldodig(sys_hw_t *hw, uint32_t value);

/* REG_0x43:ana_reg3->vhsel_ldodig:digldo output voltage select(high power)  0:0.6V,…..7:1.4V,W,0x43[31:29]*/
void sys_hal_set_ana_reg3_vhsel_ldodig(sys_hw_t *hw, uint32_t value);

/* REG_0x44 */

void sys_hal_set_ana_reg4_value(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->cb_manu_val:CB Calibration Manual Value,W,0x44[9:5]*/
void sys_hal_set_ana_reg4_cb_manu_val(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->cb_cal_trig:CB Calibration Trigger,W,0x44[10]*/
void sys_hal_set_ana_reg4_cb_cal_trig(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->cb_cal_manu:CB Calibration Manual Mode ,W,0x44[11]*/
void sys_hal_set_ana_reg4_cb_cal_manu(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->rosc_cal_intval:Rosc Calibration Interlval 0.25s~2s,W,0x44[14:12]*/
void sys_hal_set_ana_reg4_rosc_cal_intval(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->manu_cin:Rosc Calibration Manual Cin,W,0x44[21:15]*/
void sys_hal_set_ana_reg4_manu_cin(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->manu_fin:Rosc Calibration Manual Fin,W,0x44[26:22]*/
void sys_hal_set_ana_reg4_manu_fin(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->rosc_cal_mode:Rosc Calibration Mode:; 0x1: 32K; 0x0: 31.25K,W,0x44[27]*/
void sys_hal_set_ana_reg4_rosc_cal_mode(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->rosc_cal_trig:Rosc Calibration Trigger,W,0x44[28]*/
void sys_hal_set_ana_reg4_rosc_cal_trig(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->rosc_cal_en:Rosc Calibration Enable,W,0x44[29]*/
void sys_hal_set_ana_reg4_rosc_cal_en(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->rosc_manu_en:Rosc Calibration Manual Mode ,W,0x44[30]*/
void sys_hal_set_ana_reg4_rosc_manu_en(sys_hw_t *hw, uint32_t value);

/* REG_0x44:ana_reg4->rosc_tsten:Rosc test enable,W,0x44[31]*/
void sys_hal_set_ana_reg4_rosc_tsten(sys_hw_t *hw, uint32_t value);

/* REG_0x45 */

void sys_hal_set_ana_reg5_value(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->vref_scale:gadc reference voltage scale enable,W,0x45[0]*/
void sys_hal_set_ana_reg5_vref_scale(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->dccal_en:gadc DC calibration enable,W,0x45[1]*/
void sys_hal_set_ana_reg5_dccal_en(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->xtalh_ctune:xtalh load cap tuning,W,0x45[8:2]*/
void sys_hal_set_ana_reg5_xtalh_ctune(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->cktst_sel:clock test signal selection rosc/xtall/dco/dpll,W,0x45[10:9]*/
void sys_hal_set_ana_reg5_cktst_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->ck_tst_enbale:system clock test enable,W,0x45[11]*/
void sys_hal_set_ana_reg5_ck_tst_enbale(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->trxt_tst_enable:wifi trx test enable,W,0x45[12]*/
void sys_hal_set_ana_reg5_trxt_tst_enable(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->encb:global central bias enable,W,0x45[13]*/
void sys_hal_set_ana_reg5_encb(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->vctrl_dpllldo:dpll ldo output selection,W,0x45[15:14]*/
void sys_hal_set_ana_reg5_vctrl_dpllldo(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->vctrl_sysldo:sys ldo output selection,W,0x45[17:16]*/
void sys_hal_set_ana_reg5_vctrl_sysldo(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->temptst_en:tempdet test enable,W,0x45[18]*/
void sys_hal_set_ana_reg5_temptst_en(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->gadc_tsel:gadc test signal selection,W,0x45[21:19]*/
void sys_hal_set_ana_reg5_gadc_tsel(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->xtalh_ictrl:xtalh current control,W,0x45[22]*/
void sys_hal_set_ana_reg5_xtalh_ictrl(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->bgcalm:bandgap calibration manual setting,W,0x45[28:23]*/
void sys_hal_set_ana_reg5_bgcalm(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->bgcal_trig:bandgap calibrarion trig,W,0x45[29]*/
void sys_hal_set_ana_reg5_bgcal_trig(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->bgcal_manu:bandgap calibration manual mode enable,W,0x45[30]*/
void sys_hal_set_ana_reg5_bgcal_manu(sys_hw_t *hw, uint32_t value);

/* REG_0x45:ana_reg5->bgcal_en:bandgap calibration enable,W,0x45[31]*/
void sys_hal_set_ana_reg5_bgcal_en(sys_hw_t *hw, uint32_t value);

/* REG_0x46 */

void sys_hal_set_ana_reg6_value(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->itune_xtall:xtall core current control,W,0x46[3:0]*/
void sys_hal_set_ana_reg6_itune_xtall(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->xtall_ten:xtall test enable,W,0x46[4]*/
void sys_hal_set_ana_reg6_xtall_ten(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->psldo_vsel:ps ldo output voltage selection,0:VIO /1:1.8V,W,0x46[5]*/
void sys_hal_set_ana_reg6_psldo_vsel(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_usb:usb phy enable,W,0x46[6]*/
void sys_hal_set_ana_reg6_en_usb(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_xtall:xtall oscillator enable,W,0x46[7]*/
void sys_hal_set_ana_reg6_en_xtall(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_dco:dco enable,W,0x46[8]*/
void sys_hal_set_ana_reg6_en_dco(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_psram_ldo:psram ldo enable,W,0x46[9]*/
void sys_hal_set_ana_reg6_en_psram_ldo(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_tempdet:tempreture det enable,W,0x46[10]*/
void sys_hal_set_ana_reg6_en_tempdet(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_audpll:audio pll enable,W,0x46[11]*/
void sys_hal_set_ana_reg6_en_audpll(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_dpll:dpll enable,W,0x46[12]*/
void sys_hal_set_ana_reg6_en_dpll(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_sysldo:sysldo enable,W,0x46[13]*/
void sys_hal_set_ana_reg6_en_sysldo(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->pwd_gadc_buf:gadc input buffer pwd,W,0x46[17]*/
void sys_hal_set_ana_reg6_pwd_gadc_buf(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->xtal_hpsrr_en:xtal high psrr buffer enable,W,0x46[18]*/
void sys_hal_set_ana_reg6_xtal_hpsrr_en(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_xtal2rf:xtal clock to rfpll gate enable ,W,0x46[19]*/
void sys_hal_set_ana_reg6_en_xtal2rf(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->en_sleep:xtal sleep enable,W,0x46[20]*/
void sys_hal_set_ana_reg6_en_sleep(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->clkbuf_hd:xtal lpsrr clock buffer high power mode ,W,0x46[21]*/
void sys_hal_set_ana_reg6_clkbuf_hd(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->clkbuf_dsel_manu:xtal lpsrr clock buffer power mode selection 0: auto /1:manu ,W,0x46[22]*/
void sys_hal_set_ana_reg6_clkbuf_dsel_manu(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->xtal_lpmode_ctrl:xtal core low power mode enable,W,0x46[23]*/
void sys_hal_set_ana_reg6_xtal_lpmode_ctrl(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->rxtal_lp:xtal bias current setting at low power mode ,W,0x46[27:24]*/
void sys_hal_set_ana_reg6_rxtal_lp(sys_hw_t *hw, uint32_t value);

/* REG_0x46:ana_reg6->rxtal_hp:xtal26m bias current setting at high power mode ,W,0x46[31:28]*/
void sys_hal_set_ana_reg6_rxtal_hp(sys_hw_t *hw, uint32_t value);

/* REG_0x47 */

void sys_hal_set_ana_reg7_value(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->rng_tstck_sel:trng setting,W,0x47[0]*/
void sys_hal_set_ana_reg7_rng_tstck_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->rng_tsten:trng setting,W,0x47[1]*/
void sys_hal_set_ana_reg7_rng_tsten(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->itune_ref:trng setting,W,0x47[4:2]*/
void sys_hal_set_ana_reg7_itune_ref(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->itune_opa:trng setting,W,0x47[7:5]*/
void sys_hal_set_ana_reg7_itune_opa(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->itune_cmp:trng setting,W,0x47[10:8]*/
void sys_hal_set_ana_reg7_itune_cmp(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->Rnooise_sel:trng setting,W,0x47[11]*/
void sys_hal_set_ana_reg7_rnooise_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->Fslow_sel:trng setting,W,0x47[14:12]*/
void sys_hal_set_ana_reg7_fslow_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->Ffast_sel:trng setting,W,0x47[18:15]*/
void sys_hal_set_ana_reg7_ffast_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->gadc_cal_sel:gadc calibration mode selection,W,0x47[20:19]*/
void sys_hal_set_ana_reg7_gadc_cal_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->gadc_ten:gadc test enable,W,0x47[21]*/
void sys_hal_set_ana_reg7_gadc_ten(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->gadc_cmp_ictrl:gadc comparaor current select ,W,0x47[25:22]*/
void sys_hal_set_ana_reg7_gadc_cmp_ictrl(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->gadc_buf_ictrl:gadc buffer current select ,W,0x47[29:26]*/
void sys_hal_set_ana_reg7_gadc_buf_ictrl(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->vref_sel:gadc input reference select, 0：bandgap signal 1:GPIO voltage divided,W,0x47[30]*/
void sys_hal_set_ana_reg7_vref_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x47:ana_reg7->scal_en:gadc reference scale enable, 0:normal mode,1: scale mode ,W,0x47[31]*/
void sys_hal_set_ana_reg7_scal_en(sys_hw_t *hw, uint32_t value);

/* REG_0x48 */

void sys_hal_set_ana_reg8_value(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->cap_calspi:manul mode ,input cap calibretion value,W,0x48[8:0]*/
void sys_hal_set_ana_reg8_cap_calspi(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->gain_s:Sensitivity level selection,W,0x48[10:9]*/
void sys_hal_set_ana_reg8_gain_s(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->pwd_td:power down touch module,W,0x48[11]*/
void sys_hal_set_ana_reg8_pwd_td(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->en_fsr:low power mode ,enable fast response,W,0x48[12]*/
void sys_hal_set_ana_reg8_en_fsr(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->en_scm:scan mode enable,W,0x48[13]*/
void sys_hal_set_ana_reg8_en_scm(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->en_adcmode:adc mode enable,W,0x48[14]*/
void sys_hal_set_ana_reg8_en_adcmode(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->en_lpmode:low power mode enable,W,0x48[15]*/
void sys_hal_set_ana_reg8_en_lpmode(sys_hw_t *hw, uint32_t value);

/* REG_0x48:ana_reg8->chs_scan:scan mode chan selection,W,0x48[31:16]*/
void sys_hal_set_ana_reg8_chs_scan(sys_hw_t *hw, uint32_t value);

/* REG_0x49 */

void sys_hal_set_ana_reg9_value(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->en_otp_spi:otp ldo spi enable,W,0x49[0]*/
void sys_hal_set_ana_reg9_en_otp_spi(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->digovr_en:digldo over voltage reset enable,W,0x49[13]*/
void sys_hal_set_ana_reg9_digovr_en(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->usbpen:usb dp driver capability control,W,0x49[17:14]*/
void sys_hal_set_ana_reg9_usbpen(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->usbnen:usb dn driver capability control,W,0x49[21:18]*/
void sys_hal_set_ana_reg9_usbnen(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->usb_speed:usb speed selection,W,0x49[22]*/
void sys_hal_set_ana_reg9_usb_speed(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->usb_deepsleep:usb deepsleep mode enable by spi,W,0x49[23]*/
void sys_hal_set_ana_reg9_usb_deepsleep(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->man_mode:manul mode enable,W,0x49[24]*/
void sys_hal_set_ana_reg9_man_mode(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->crg:detect range selection :8pF/12pF/19pF/27pF,W,0x49[26:25]*/
void sys_hal_set_ana_reg9_crg(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->vrefs:detect threshold selection ,W,0x49[29:27]*/
void sys_hal_set_ana_reg9_vrefs(sys_hw_t *hw, uint32_t value);

/* REG_0x49:ana_reg9->en_cal:calibretion enable,W,0x49[31]*/
void sys_hal_set_ana_reg9_en_cal(sys_hw_t *hw, uint32_t value);

/* REG_0x4a */

void sys_hal_set_ana_reg10_value(sys_hw_t *hw, uint32_t value);

/* REG_0x4a:ana_reg10->sdm_val:audio pll sdm value,W,0x4a[29:0]*/
void sys_hal_set_ana_reg10_sdm_val(sys_hw_t *hw, uint32_t value);

/* REG_0x4a:ana_reg10->vco_hfreq_enb:audio pll vco high frequency enb,W,0x4a[30]*/
void sys_hal_set_ana_reg10_vco_hfreq_enb(sys_hw_t *hw, uint32_t value);

/* REG_0x4a:ana_reg10->cal_refen:cal_ref enable of audio pll,W,0x4a[31]*/
void sys_hal_set_ana_reg10_cal_refen(sys_hw_t *hw, uint32_t value);

/* REG_0x4b */

void sys_hal_set_ana_reg11_value(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->int_mod:DPLL integer mode enable; 0: fractional mode; 1: integer mode,W,0x4b[0]*/
void sys_hal_set_ana_reg11_int_mod(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->Nsyn:DPLL Ncoutner reset ,W,0x4b[1]*/
void sys_hal_set_ana_reg11_nsyn(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->selpol:DPLL PFD polarity control,W,0x4b[2]*/
void sys_hal_set_ana_reg11_selpol(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->reset:DPLL reset,W,0x4b[3]*/
void sys_hal_set_ana_reg11_reset(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->Ioffset:DPLL  charge pump offset current control,W,0x4b[6:4]*/
void sys_hal_set_ana_reg11_ioffset(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->LPFRz:DPLL Rz control of LPF,W,0x4b[10:7]*/
void sys_hal_set_ana_reg11_lpfrz(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->Rp2:DPLL Rp control of LPF,W,0x4b[13:11]*/
void sys_hal_set_ana_reg11_rp2(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->vsel_cal:DPLL vtrl selection during VCO band calibration,W,0x4b[14]*/
void sys_hal_set_ana_reg11_vsel_cal(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->kctrl:DPLL Kvco control,W,0x4b[16:15]*/
void sys_hal_set_ana_reg11_kctrl(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->ckref_loop_sel:polarity selection of referenc clock	to SDM,W,0x4b[17]*/
void sys_hal_set_ana_reg11_ckref_loop_sel(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->spi_trigger:DPLL band calibration spi trigger,W,0x4b[18]*/
void sys_hal_set_ana_reg11_spi_trigger(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->manual:DPLL VCO band manual enable; 0: auto mode; 1: manual mode,W,0x4b[19]*/
void sys_hal_set_ana_reg11_manual(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->Icp:DPLL charge pump current control; 000: 5uA; 001: 10uA; 010: 15uA; 011: 20uA; 100: 25uA; 101: 30uA; 110: 35uA; 111: 40uA,W,0x4b[22:20]*/
void sys_hal_set_ana_reg11_icp(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->Rsel:DPLL reference clock selection; 0: 13M; 1: 6.5M,W,0x4b[23]*/
void sys_hal_set_ana_reg11_rsel(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->ck26Men:xtal26M clock for audio enable,W,0x4b[24]*/
void sys_hal_set_ana_reg11_ck26men(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->ckaudio_outen:DPLL clock output to PAD enable,W,0x4b[25]*/
void sys_hal_set_ana_reg11_ckaudio_outen(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->divctrl:DPLL divider control; 000: div1; 001: div2; 010: div4; 011: div8; 1xx: div16,W,0x4b[28:26]*/
void sys_hal_set_ana_reg11_divctrl(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->cksel:DPLL divider control; 0: div3; 1: div4,W,0x4b[29]*/
void sys_hal_set_ana_reg11_cksel(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->usben:DPLL clock for USB enable,W,0x4b[30]*/
void sys_hal_set_ana_reg11_usben(sys_hw_t *hw, uint32_t value);

/* REG_0x4b:ana_reg11->audioen:DPLL clock for audio enable,W,0x4b[31]*/
void sys_hal_set_ana_reg11_audioen(sys_hw_t *hw, uint32_t value);

/* REG_0x4c */

void sys_hal_set_ana_reg12_value(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->digmic_ckinv:digmic clock inversion enable,W,0x4c[2]*/
void sys_hal_set_ana_reg12_digmic_ckinv(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->enmicdig:digmic enable,w,0x4c[3]*/
void sys_hal_set_ana_reg12_enmicdig(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->audck_rlcen:audio clock re-latch enable,W,0x4c[4]*/
void sys_hal_set_ana_reg12_audck_rlcen(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->lchckinven:audio clock re-latch clock inversion enable,W,0x4c[5]*/
void sys_hal_set_ana_reg12_lchckinven(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->ldo1v_vsel1v:audio 1.0V LDO selection, 000=0.8, 1X1=1.0,W,0x4c[8:6]*/
void sys_hal_set_ana_reg12_ldo1v_vsel1v(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->ldo1v_adj:audio 1.0V LDO output trimming, 00000=min, 11111=max,W,0x4c[13:9]*/
void sys_hal_set_ana_reg12_ldo1v_adj(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->audvdd_trm1v:audio 1.5V LDO selection, 00=min, 11=max,W,0x4c[15:14]*/
void sys_hal_set_ana_reg12_audvdd_trm1v(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->audvdd_voc1v:audio 1.5V LDO output trimming, 00000=min, 11111=max,W,0x4c[20:16]*/
void sys_hal_set_ana_reg12_audvdd_voc1v(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->enaudvdd1v:audio 1.0V LDO enable,W,0x4c[21]*/
void sys_hal_set_ana_reg12_enaudvdd1v(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->loadhp:audio 1.5V LDO, 1=good stability with small loading,W,0x4c[22]*/
void sys_hal_set_ana_reg12_loadhp(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->enaudvdd1v5:audio 1.5V LDO enable,W,0x4c[23]*/
void sys_hal_set_ana_reg12_enaudvdd1v5(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->enmicbias1v:micbias enable,W,0x4c[24]*/
void sys_hal_set_ana_reg12_enmicbias1v(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->micbias_trim:micbias output selection, 00=min, 11=max,W,0x4c[26:25]*/
void sys_hal_set_ana_reg12_micbias_trim(sys_hw_t *hw, uint32_t value);

/* REG_0x4c:ana_reg12->micbias_voc1v:micbias output trimming, 00000=min, 11111=max,W,0x4c[31:27]*/
void sys_hal_set_ana_reg12_micbias_voc1v(sys_hw_t *hw, uint32_t value);

/* REG_0x4d */

void sys_hal_set_ana_reg13_value(sys_hw_t *hw, uint32_t value);

/* REG_0x4d:ana_reg13->byp_dwaadc:adc dwa pass enable,W,0x4d[8]*/
void sys_hal_set_ana_reg13_byp_dwaadc(sys_hw_t *hw, uint32_t value);

/* REG_0x4d:ana_reg13->rst:rst,W,0x4d[9]*/
void sys_hal_set_ana_reg13_rst(sys_hw_t *hw, uint32_t value);

/* REG_0x4d:ana_reg13->adcdwa_mode:adc dwa model sel,W,0x4d[10]*/
void sys_hal_set_ana_reg13_adcdwa_mode(sys_hw_t *hw, uint32_t value);

/* REG_0x4d:ana_reg13->vodadjspi:adc reference manual spi control,W,0x4d[15:11]*/
void sys_hal_set_ana_reg13_vodadjspi(sys_hw_t *hw, uint32_t value);

/* REG_0x4d:ana_reg13->refvsel:0= high reference; 1=small reference,W,0x4d[21]*/
void sys_hal_set_ana_reg13_refvsel(sys_hw_t *hw, uint32_t value);

/* REG_0x4d:ana_reg13->capsw1v:munual value for cap trimming,W,0x4d[27:23]*/
void sys_hal_set_ana_reg13_capsw1v(sys_hw_t *hw, uint32_t value);

/* REG_0x4d:ana_reg13->adcckinven:audio adc clock inversion enable,W,0x4d[30]*/
void sys_hal_set_ana_reg13_adcckinven(sys_hw_t *hw, uint32_t value);

/* REG_0x4e */

void sys_hal_set_ana_reg14_value(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->isel:adc bias trimming,W,0x4e[1:0]*/
void sys_hal_set_ana_reg14_isel(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micdcocdin:adc micmode dcoc din,W,0x4e[9:2]*/
void sys_hal_set_ana_reg14_micdcocdin(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micdcocvc:adc micmode dcoc control,W,0x4e[11:10]*/
void sys_hal_set_ana_reg14_micdcocvc(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micdcocen_n:adc micmode dcoc enable,W,0x4e[12]*/
void sys_hal_set_ana_reg14_micdcocen_n(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micdcocen_p:adc micmode dcoc enable,W,0x4e[13]*/
void sys_hal_set_ana_reg14_micdcocen_p(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micsingleEn:adc micmode, single_end enable,W,0x4e[14]*/
void sys_hal_set_ana_reg14_micsingleen(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micGain:adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,W,0x4e[18:15]*/
void sys_hal_set_ana_reg14_micgain(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micdacen:adc micmode micdac enable,W,0x4e[19]*/
void sys_hal_set_ana_reg14_micdacen(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micdaciH:adc micmode, micdac input ,W,0x4e[27:20]*/
void sys_hal_set_ana_reg14_micdacih(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micdacit:adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,W,0x4e[29:28]*/
void sys_hal_set_ana_reg14_micdacit(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->hcen:adc 1stg op current trimming,W,0x4e[30]*/
void sys_hal_set_ana_reg14_hcen(sys_hw_t *hw, uint32_t value);

/* REG_0x4e:ana_reg14->micEn:mic1 mode enable,W,0x4e[31]*/
void sys_hal_set_ana_reg14_micen(sys_hw_t *hw, uint32_t value);

/* REG_0x4f */

void sys_hal_set_ana_reg15_value(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->isel:adc bias trimming,W,0x4f[1:0]*/
void sys_hal_set_ana_reg15_isel(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micdcocdin:adc micmode dcoc din,W,0x4f[9:2]*/
void sys_hal_set_ana_reg15_micdcocdin(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micdcocvc:adc micmode dcoc control,W,0x4f[11:10]*/
void sys_hal_set_ana_reg15_micdcocvc(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micdcocen_n:adc micmode dcoc enable,W,0x4f[12]*/
void sys_hal_set_ana_reg15_micdcocen_n(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micdcocen_p:adc micmode dcoc enable,W,0x4f[13]*/
void sys_hal_set_ana_reg15_micdcocen_p(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micsingleEn:adc micmode, single_end enable,W,0x4f[14]*/
void sys_hal_set_ana_reg15_micsingleen(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micGain:adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,W,0x4f[18:15]*/
void sys_hal_set_ana_reg15_micgain(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micdacen:adc micmode micdac enable,W,0x4f[19]*/
void sys_hal_set_ana_reg15_micdacen(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micdaciH:adc micmode, micdac input ,W,0x4f[27:20]*/
void sys_hal_set_ana_reg15_micdacih(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micdacit:adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,W,0x4f[29:28]*/
void sys_hal_set_ana_reg15_micdacit(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->hcen:adc 1stg op current trimming,W,0x4f[30]*/
void sys_hal_set_ana_reg15_hcen(sys_hw_t *hw, uint32_t value);

/* REG_0x4f:ana_reg15->micEn:mic2 mode enable,W,0x4f[31]*/
void sys_hal_set_ana_reg15_micen(sys_hw_t *hw, uint32_t value);

/* REG_0x50 */

void sys_hal_set_ana_reg16_value(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->hpdac:class ab driver high current mode. "1" high current. ,W,0x50[0]*/
void sys_hal_set_ana_reg16_hpdac(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->vcmsdac:1stg OP input common model voltage selection. "1" low common mode voltage,W,0x50[1]*/
void sys_hal_set_ana_reg16_vcmsdac(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->oscdac:threshold current setting for over current protection . "3" maximum current. "0" minimum current,W,0x50[3:2]*/
void sys_hal_set_ana_reg16_oscdac(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->ocendac:over current protection enable. "1" enable.,W,0x50[4]*/
void sys_hal_set_ana_reg16_ocendac(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->isel_idac:idac current sel,W,0x50[5]*/
void sys_hal_set_ana_reg16_isel_idac(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->adjdacref:audio dac reference voltage adjust.,W,0x50[10:6]*/
void sys_hal_set_ana_reg16_adjdacref(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->dcochg:dcoc high gain selection. "1" high gain,W,0x50[12]*/
void sys_hal_set_ana_reg16_dcochg(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->diffen:enable differential mode. "1" enable,W,0x50[13]*/
void sys_hal_set_ana_reg16_diffen(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->endaccal:enable offset calibration process. "1" enable.,W,0x50[14]*/
void sys_hal_set_ana_reg16_endaccal(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->rendcoc:R-channel dcoc dac enablel. "1" enable,W,0x50[15]*/
void sys_hal_set_ana_reg16_rendcoc(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->lendcoc:L-channel Dcoc dac enable. "1" enable,W,0x50[16]*/
void sys_hal_set_ana_reg16_lendcoc(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->renvcmd:R-channel common mode output buffer enable."1" enable,W,0x50[17]*/
void sys_hal_set_ana_reg16_renvcmd(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->lenvcmd:L-channel common mode output buffer enable. "1" enable,W,0x50[18]*/
void sys_hal_set_ana_reg16_lenvcmd(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->dacdrven:dac output driver enable."1" enable,W,0x50[19]*/
void sys_hal_set_ana_reg16_dacdrven(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->dacRen:dac R-channel enable. "1"  enable,W,0x50[20]*/
void sys_hal_set_ana_reg16_dacren(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->dacLen:dac L-channel enable. "1" enable,W,0x50[21]*/
void sys_hal_set_ana_reg16_daclen(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->dacG:dac gain setting: 000=0dB, 111=8dB,W,0x50[24:22]*/
void sys_hal_set_ana_reg16_dacg(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->ck4xsel:dac clock sel ,W,0x50[25]*/
void sys_hal_set_ana_reg16_ck4xsel(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->dacmute:dac mute enable. "1" mute enable,W,0x50[26]*/
void sys_hal_set_ana_reg16_dacmute(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->dwamode:dac dwa mode sel,W,0x50[27]*/
void sys_hal_set_ana_reg16_dwamode(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->ckposel:dac sample clock edge selection,W,0x50[28]*/
void sys_hal_set_ana_reg16_ckposel(sys_hw_t *hw, uint32_t value);

/* REG_0x50:ana_reg16->byldo:bypass 1v8 LDO,W,0x50[31]*/
void sys_hal_set_ana_reg16_byldo(sys_hw_t *hw, uint32_t value);

/* REG_0x51 */

void sys_hal_set_ana_reg17_value(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->lmdcin:l-cnannel offset cancel dac maumual input.,W,0x51[7:0]*/
void sys_hal_set_ana_reg17_lmdcin(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->rmdcin:r-channel offset cancel dac manmual input ,W,0x51[15:8]*/
void sys_hal_set_ana_reg17_rmdcin(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->spirst_ovc:ovc rst,W,0x51[16]*/
void sys_hal_set_ana_reg17_spirst_ovc(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->hc2s0v9:0=current is half,W,0x51[20]*/
void sys_hal_set_ana_reg17_hc2s0v9(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->lvcmsel:low vcm sel,W,0x51[21]*/
void sys_hal_set_ana_reg17_lvcmsel(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->loop2sel:2rd loop sel,W,0x51[22]*/
void sys_hal_set_ana_reg17_loop2sel(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->enbias:dac bias enable,W,0x51[23]*/
void sys_hal_set_ana_reg17_enbias(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->calck_sel0v9:offset calibration clock selection. "1" high clock.,W,0x51[24]*/
void sys_hal_set_ana_reg17_calck_sel0v9(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->bpdwa0v9:bypss audio dac dwa. "1" bypass.,W,0x51[25]*/
void sys_hal_set_ana_reg17_bpdwa0v9(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->looprst0v9:audio dac integrator capacitor reset. "1" reset.,W,0x51[26]*/
void sys_hal_set_ana_reg17_looprst0v9(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->oct0v9:over current delay time setting."11" maximum time. "00" minimum current.,W,0x51[28:27]*/
void sys_hal_set_ana_reg17_oct0v9(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->sout0v9:short output with 600ohm resistor. "1" short output.,W,0x51[29]*/
void sys_hal_set_ana_reg17_sout0v9(sys_hw_t *hw, uint32_t value);

/* REG_0x51:ana_reg17->hc0v9:dac current trimming, 00=minimum current, 11=maximum current,W,0x51[31:30]*/
void sys_hal_set_ana_reg17_hc0v9(sys_hw_t *hw, uint32_t value);

/* REG_0x52 */

void sys_hal_set_ana_reg18_value(sys_hw_t *hw, uint32_t value);

/* REG_0x52:ana_reg18->ictrl_dsppll:26M PLL setting,W,0x52[3:0]*/
void sys_hal_set_ana_reg18_ictrl_dsppll(sys_hw_t *hw, uint32_t value);

/* REG_0x52:ana_reg18->FBdivN:26M PLL setting,W,0x52[13:4]*/
void sys_hal_set_ana_reg18_fbdivn(sys_hw_t *hw, uint32_t value);

/* REG_0x52:ana_reg18->N_mcudsp:26M PLL setting,W,0x52[18:14]*/
void sys_hal_set_ana_reg18_n_mcudsp(sys_hw_t *hw, uint32_t value);

/* REG_0x52:ana_reg18->mode:26M PLL setting,W,0x52[19]*/
void sys_hal_set_ana_reg18_mode(sys_hw_t *hw, uint32_t value);

/* REG_0x52:ana_reg18->iamsel:26M PLL setting,W,0x52[20]*/
void sys_hal_set_ana_reg18_iamsel(sys_hw_t *hw, uint32_t value);

/* REG_0x52:ana_reg18->hvref:26M PLL setting,W,0x52[22:21]*/
void sys_hal_set_ana_reg18_hvref(sys_hw_t *hw, uint32_t value);

/* REG_0x52:ana_reg18->lvref:26M PLL setting,W,0x52[24:23]*/
void sys_hal_set_ana_reg18_lvref(sys_hw_t *hw, uint32_t value);

/* REG_0x53 */

void sys_hal_set_ana_reg19_value(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->amsel:26M PLL setting,W,0x53[0]*/
void sys_hal_set_ana_reg19_amsel(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->msw:26M PLL setting,W,0x53[9:1]*/
void sys_hal_set_ana_reg19_msw(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->tstcken_dpll:26M PLL setting,W,0x53[10]*/
void sys_hal_set_ana_reg19_tstcken_dpll(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->osccal_trig:26M PLL setting,W,0x53[11]*/
void sys_hal_set_ana_reg19_osccal_trig(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->cnti:26M PLL setting,W,0x53[20:12]*/
void sys_hal_set_ana_reg19_cnti(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->spi_rst:26M PLL setting,W,0x53[22]*/
void sys_hal_set_ana_reg19_spi_rst(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->closeloop_en:26M PLL setting,W,0x53[23]*/
void sys_hal_set_ana_reg19_closeloop_en(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->caltime:26M PLL setting,W,0x53[24]*/
void sys_hal_set_ana_reg19_caltime(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->LPFRz:26M PLL setting,W,0x53[26:25]*/
void sys_hal_set_ana_reg19_lpfrz(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->ICP:26M PLL setting,W,0x53[30:27]*/
void sys_hal_set_ana_reg19_icp(sys_hw_t *hw, uint32_t value);

/* REG_0x53:ana_reg19->CP2ctrl:26M PLL setting,W,0x53[31]*/
void sys_hal_set_ana_reg19_cp2ctrl(sys_hw_t *hw, uint32_t value);

#endif

void sys_hal_set_ana_trxt_tst_enable(uint32_t value);
void sys_hal_set_ana_scal_en(uint32_t value);
void sys_hal_set_ana_gadc_buf_ictrl(uint32_t value);
void sys_hal_set_ana_gadc_cmp_ictrl(uint32_t value);
void sys_hal_set_ana_pwd_gadc_buf(uint32_t value);
void sys_hal_set_ana_hres_sel0v9(uint32_t value);
void sys_hal_set_ana_vref_sel(uint32_t value);
void sys_hal_set_ana_cb_cal_manu(uint32_t value);
void sys_hal_set_ana_cb_cal_trig(uint32_t value);
UINT32 sys_hal_get_ana_cb_cal_manu_val(void);
void sys_hal_set_ana_cb_cal_manu_val(uint32_t value);
void sys_hal_set_ana_vlsel_ldodig(uint32_t value);
void sys_hal_set_ana_vhsel_ldodig(uint32_t value);
void sys_hal_set_ana_vctrl_sysldo(uint32_t value);
void sys_hal_enable_eth_int(uint32_t value);

void sys_hal_set_yuv_buf_clock_en(uint32_t value);
void sys_hal_set_h264_clock_en(uint32_t value);
void sys_hal_set_ana_reg11_apfms(uint32_t value);
void sys_hal_set_ana_reg12_dpfms(uint32_t value);

#if defined(CONFIG_HAL_DEBUG_SYS)
void sys_struct_dump(uint32_t start, uint32_t end);
#else
static inline void __sys_struct_dump(uint32_t start, uint32_t end)
{
	HAL_LOGI("start=%x, end-%x, please generate the hal_debug.c file!\r\n", start, end);
}
#define sys_struct_dump(start, end) __sys_struct_dump(start, end)
#endif

int sys_hal_set_buck(sys_buck_type_t buck, bool ena);
int sys_hal_set_buck_pfm(sys_buck_type_t buck, bool ena);
int sys_hal_set_buck_burst(sys_buck_type_t buck, bool ena);
int sys_hal_set_buck_mpo(sys_buck_type_t buck, bool ena);

int sys_hal_set_ldo_self_lp(sys_ldo_type_t ldo, bool ena);
int sys_hal_set_ldo_current_limit(sys_ldo_type_t ldo, bool ena);
int sys_hal_set_aon_power(sys_aon_power_t power);
int sys_hal_set_aon_ldo_volt(uint32_t volt);
int sys_hal_set_io_ldo_volt(uint32_t volt);
int sys_hal_set_ana_ldo_volt(bool trsw_ena, uint32_t rx_volt, uint32_t tx_volt);
int sys_hal_set_digital_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt);
int sys_hal_set_core_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt);
int sys_hal_set_lv_ctrl_pd(bool lp_ena);
int sys_hal_set_lv_ctrl_hf(bool lp_ena);
int sys_hal_set_lv_ctrl_flash(bool lp_ena);
int sys_hal_set_lv_ctrl_core(bool lp_ena);
int sys_hal_set_lpo_src(sys_lpo_src_t src);
void sys_hal_dump_ctrl(void);
void sys_hal_early_init(void);
void sys_hal_set_rott_int_en(uint32_t value);
void sys_hal_enter_low_analog(void);
void sys_hal_exit_low_analog(void);
void sys_hal_set_7816_int_en(uint32_t value);
void sys_hal_set_scr_clk(uint32_t value);

void sys_hal_set_ana_reg5_pwd_rosc_spi(uint32_t value);
#if defined(CONFIG_ANA_RTC) || defined(CONFIG_ANA_GPIO)
void sys_hal_set_ana_reg8_spi_latch1v(uint32_t value);
#if defined(CONFIG_BK7239N_MP) && (CONFIG_BK7239N_MP == 1)
void sys_hal_set_ana_reg8_spi_latch1v_iram(uint32_t value);
void sys_hal_set_ana_reg8_rst_timerwks1v(uint32_t value);
void sys_hal_set_ana_reg8_rst_gpiowks(uint32_t value);
#else
void sys_hal_set_ana_reg8_spi_latch1v(uint32_t value);
void sys_hal_set_ana_reg8_rst_wks1v(uint32_t value);
#endif
void sys_hal_set_ana_reg5_adc_div(uint32_t value);
void sys_hal_set_ana_reg5_rosc_disable(uint32_t value);
void sys_hal_set_ana_reg7_timer_wkrstn(uint32_t value);

void sys_hal_set_ana_reg8_lvsleep_wkrst(uint32_t value);
void sys_hal_set_ana_reg8_gpiowk_rstn(uint32_t value);
void sys_hal_set_ana_reg8_rtcwk_rstn(uint32_t value);
void sys_hal_set_ana_reg8_ensfsdd(uint32_t value);
void sys_hal_set_ana_reg8_vlden(uint32_t value);
void sys_hal_set_ana_reg8_pwdovp1v(uint32_t value);
void sys_hal_set_ana_reg9_spi_timerwken(uint32_t value);
void sys_hal_set_ana_reg9_spi_byp32pwd(uint32_t value);
uint32_t sys_hal_get_ana_reg11_gpiowk(void);
void sys_hal_set_ana_reg11_gpiowk(uint32_t value);
uint32_t sys_hal_get_ana_reg11_rtcsel(void);
void sys_hal_set_ana_reg11_rtcsel(uint32_t value);
uint32_t sys_hal_get_ana_reg11_timersel(void);
void sys_hal_set_ana_reg11_timersel(uint32_t value);
uint32_t sys_hal_get_ana_reg12_timersel(void);
void sys_hal_set_ana_reg12_timersel(uint32_t value);
uint32_t sys_hal_get_ana_reg13_rtcsel(void);
void sys_hal_set_ana_reg13_rtcsel(uint32_t value);
void sys_hal_enable_ana_rtc_int(void);
void sys_hal_disable_ana_rtc_int(void);
void sys_hal_enable_ana_gpio_int(void);
void sys_hal_disable_ana_gpio_int(void);
#endif

uint64_t sys_hal_get_low_voltage_sleep_duration_us(void);
void sys_hal_set_low_voltage_sleep_duration_us(uint64_t sleep_duration);
uint64_t sys_hal_get_low_voltage_wakeup_time_us(void);
void sys_hal_set_low_voltage_wakeup_time_us(uint64_t wakeup_time);
#if defined(CONFIG_GPIO_CLOCK_PIN_SUPPORT)
/**  System Clock Start **/
void sys_hal_clk_pin_ana_open(clk_pin_ana_src_t src, clk_pin_div_t div, bool en);
void sys_hal_clk_pin_dig_open(clk_pin_dig_src_t src, clk_pin_div_t div, bool en);
/**  System Clock End **/
#endif

#ifdef __cplusplus
}
#endif


