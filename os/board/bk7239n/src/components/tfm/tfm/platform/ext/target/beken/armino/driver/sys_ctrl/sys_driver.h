#ifndef _SYS_DRV_H_
#define _SYS_DRV_H_

#include <common/sys_config.h>
#include <common/bk_include.h>
#include <soc/soc.h>
#include <components/log.h>
#include "sys_types.h"
#include "sys_hal.h"

#define SYS_DRV_DEBUG   0

#if SYS_DRV_DEBUG
#define SYS_DRV_PRT      os_printf
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


/**  Platform Start **/
//Platform

/* Platform Misc Start **/
void sys_drv_init();
/* Platform Misc End **/


#if CONFIG_USB    //功能宏开关
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

void sys_drv_qspi_clk_sel(uint32_t param);

void sys_drv_qspi_set_src_clk_div(uint32_t value);

void sys_drv_nmi_wdt_set_clk_div(uint32_t value);

uint32_t sys_drv_nmi_wdt_get_clk_div(void);

void sys_drv_trng_disckg_set(uint32_t value);

#if CONFIG_SDIO_V2P0
void sys_driver_set_sdio_clk_en(uint32_t value);

void sys_driver_set_cpu0_sdio_int_en(uint32_t value);
void sys_driver_set_cpu1_sdio_int_en(uint32_t value);
void sys_driver_set_sdio_clk_div(uint32_t value);
uint32_t sys_driver_get_sdio_clk_div();
void sys_driver_set_sdio_clk_sel(uint32_t value);
uint32_t sys_driver_get_sdio_clk_sel();
#endif

/*for low power  function start*/
void sys_drv_core_bus_clock_ctrl(high_clock_module_name_t core, uint32_t clksel,uint32_t clkdiv, high_clock_module_name_t bus,uint32_t bus_clksel,uint32_t bus_clkdiv);

void sys_drv_cpu0_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state);

void sys_drv_cpu1_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state);

void sys_drv_set_cpu1_boot_address_offset(uint32_t address_offset);

void sys_drv_set_cpu1_reset(uint32_t reset_value);

void sys_drv_enable_mac_wakeup_source();
void sys_drv_enable_bt_wakeup_source();
void sys_drv_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value);
uint32_t sys_drv_all_modules_clk_div_get(clk_div_reg_e reg);
void sys_drv_touch_wakeup_enable(uint8_t index);
void sys_drv_usb_wakeup_enable(uint8_t index);
void sys_drv_cpu_clk_div_set(uint32_t core_index, uint32_t value);
uint32_t sys_drv_cpu_clk_div_get(uint32_t core_index);
void sys_drv_low_power_hardware_init();
int32 sys_drv_lp_vol_set(uint32_t value);
uint32_t sys_drv_lp_vol_get();
int32 sys_drv_rosc_calibration(uint32_t rosc_cali_mode, uint32_t cali_interval);
int32 sys_drv_bandgap_cali_set(uint32_t value);//increase or decrease the dvdddig voltage
int sys_drv_rosc_test_mode(bool enabled);
uint32_t sys_drv_bandgap_cali_get();
/*for  low power  function end*/
uint32 sys_drv_get_device_id(void); // CMD_GET_DEVICE_ID
uint32 sys_drv_get_chip_id(void); // CMD_GET_CHIP_ID


int32 sys_drv_int_disable(uint32 param);
int32 sys_drv_int_enable(uint32 param);
int32 sys_drv_int_group2_disable(uint32 param);
int32 sys_drv_int_group2_enable(uint32 param);
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
void sys_drv_set_clk_div(dev_clk_select_id_t dev, uint32_t clk_div);
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
void sys_drv_sadc_int_disable(void);
void sys_drv_sadc_pwr_up(void);
void sys_drv_sadc_pwr_down(void);
void sys_drv_en_tempdet(uint32_t value);

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
uint32_t sys_drv_aud_dacdrv_en(uint32_t value);
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
uint32_t sys_drv_aud_int_en(uint32_t value);
uint32_t sys_drv_sbc_int_en(uint32_t value);
uint32_t sys_drv_aud_power_en(uint32_t value);

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
uint32_t sys_drv_ana_reg10_sdm_val_set(uint32_t value);
uint32_t sys_drv_ana_reg11_spi_trigger_set(uint32_t value);
uint32_t sys_drv_i2s0_ckdiv_set(uint32_t value);
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
uint32_t sys_drv_touch_serial_cap_enable(void);
uint32_t sys_drv_touch_serial_cap_disable(void);
uint32_t sys_drv_touch_int_enable(uint32_t value);

/**  Touch End **/

/**  jPEG Start **/
uint32_t sys_drv_mclk_mux_set(uint32_t value);
uint32_t sys_drv_set_jpeg_clk_sel(uint32_t value);
uint32_t sys_drv_set_clk_div_mode1_clkdiv_jpeg(uint32_t value);
uint32_t sys_drv_set_jpeg_disckg(uint32_t value);
uint32_t sys_drv_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value);
uint32_t sys_drv_video_power_en(uint32_t value);
uint32_t sys_drv_set_auxs(uint32_t cksel, uint32_t ckdiv);
uint32_t sys_drv_set_jpeg_clk_en();

/**  jPEG End **/

/**  H264 Start **/
uint32_t sys_drv_set_h264_clk_sel(uint32_t value);
uint32_t sys_drv_set_clk_div_mode1_clkdiv_h264(uint32_t value);
uint32_t sys_drv_set_h264_clk_en();
uint32_t sys_drv_h264_set_mclk_div(uint32_t value);

/**  H264 End **/

/**  psram Start **/
uint32_t sys_drv_psram_volstage_sel(uint32_t value);
uint32_t sys_drv_psram_xtall_osc_enable(uint32_t value);
uint32_t sys_drv_psram_dco_enable(uint32_t value);
uint32_t sys_drv_psram_dpll_enable(uint32_t value);
uint32_t sys_drv_psram_ldo_enable(uint32_t value);
uint32_t sys_drv_psram_clk_sel(uint32_t value);
uint32_t sys_drv_psram_set_clkdiv(uint32_t value);
uint32_t sys_drv_psram_psldo_vsel(uint32_t value);
uint32_t sys_drv_psram_psldo_vset(uint32_t value, bool is_add_200mv);
uint32_t sys_drv_psram_psram_disckg(uint32_t value);

/**  psram End **/

uint32_t sys_drv_cali_dpll(uint32_t param);
uint32_t sys_drv_bias_reg_set(uint32_t param);
uint32_t sys_drv_bias_reg_clean(uint32_t param);
uint32_t sys_drv_bias_reg_read(void);
uint32_t sys_drv_bias_reg_write(uint32_t param);
#if 0
uint32_t sys_drv_bias_get_cali_out(void);
#endif
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
void sys_drv_set_ana_vlsel_ldodig(uint32_t value);
void sys_drv_set_ana_vhsel_ldodig(uint32_t value);
void sys_drv_set_ana_vctrl_sysldo(uint32_t value);
void sys_drv_set_ana_vtempsel(uint32_t value);

uint32_t sys_drv_get_cpu_storage_connect_op_select_flash_sel(void);
void sys_drv_set_cpu_storage_connect_op_select_flash_sel(uint32_t value);

/**  WIFI End **/

void sys_drv_yuv_buf_pwr_up(void);
void sys_drv_yuv_buf_pwr_down(void);
void sys_drv_h264_pwr_up(void);
void sys_drv_h264_pwr_down(void);

void sys_drv_early_init(void);
void sys_drv_set_sys2flsh_2wire(uint32_t value);

typedef struct {
	uint32_t int0;
	uint32_t int1;
} sys_lock_ctx_t;

void sys_drv_enable_int(sys_lock_ctx_t *ctx);
void sys_drv_disable_int(const sys_lock_ctx_t *ctx);
void sys_drv_set_base_addr(uint32_t addr);

#endif //_SYS_DRV_H_
// eof

