#ifndef _BK_PHY_ADAPTER_H_
#define _BK_PHY_ADAPTER_H_

#include <common/bk_include.h>

#include "driver/hal/hal_adc_types.h"
#include "driver/flash_partition.h"
#include "driver/flash.h"

#include "drv_model_pub.h"


/* modules/wifi/... */
typedef struct {

    uint32_t _version;

    /* modules/wifi/... */
    void * (*_mpb_reg_api)(void);
    void * (*_crm_reg_api)(void);
    void * (*_riu_reg_api)(void);
    void * (*_mix_funcs)(void);
    uint32_t (*_bk_misc_get_reset_reason)(void);
    void (*_rs_init)(UINT32 channel, INT32 band, UINT32 mode);
    void (*_rs_bypass_mac_init)(UINT32 channel, INT32 band, UINT32 mode);
    void (*_rs_deinit)(void);
    void (*_evm_init)(UINT32 channel, INT32 band, UINT32 bandwidth);
    void (*_evm_bypass_mac_init)(UINT32 frequency, INT32 band, UINT32 bandwidth);

    void (*_nv_phy_reg_set_hook)(void *hook);
    void (*_evm_clear_ke_evt_mac_bit)(void);
    void (*_evm_set_ke_evt_mac_bit)(void);
    //
    void (*_tx_evm_set_chan_ctxt_pop)(void * chan_info);

    UINT32 (*_save_info_item)(UINT32 item, UINT8 *ptr0, UINT8 *ptr1, UINT8 *ptr2);
    UINT32 (*_get_info_item)(UINT32 item, UINT8 *ptr0, UINT8 *ptr1, UINT8 *ptr2);

    ////
    void (*_delay)(int32 num);
    void (*_delay_us)(UINT32 us);
    UINT32 (*_ddev_control)(UINT32 handle, UINT32 cmd, VOID *param);
    int (*_bk_wdt_stop)(void);

    bool (*_temp_detect_is_init)(void);
    int (*_temp_detect_deinit)(void);
    int (*_temp_detect_init)(uint32_t init_temperature);
    int (*_temp_detect_get_temperature)(uint32_t *temperature);
    int (*_bk_feature_temp_detect_enable)(void);

    bool (*_ate_is_enabled)(void);
    int (*_volt_single_get_current_voltage)(uint32_t *volt_value);
    float (*_bk_adc_data_calculate)(uint16_t adc_val, uint8_t adc_chan);

    int (*_bk_pm_clock_ctrl_saradc_pwrup)(void);
    int (*_bk_pm_clock_ctrl_phy_pwrup)(void);
    int (*_bk_pm_module_vote_power_ctrl_phy)(int32_t value);
    int (*_bk_pm_module_vote_cpu_freq)(uint32_t module, uint32_t cpu_freq);

    uint32_t (*_aon_pmu_drv_bias_cal_get)(void);
    uint32_t (*_aon_pmu_drv_band_cal_get)(void);
    uint32_t (*_aon_pmu_drv_get_adc_cal)(void);
    uint32_t (*_aon_pmu_hal_get_chipid)(void);
    void (*_sys_drv_set_pwd_anabuf_lownoise)(uint32_t value);
    uint32_t (*_sys_drv_rfband)(uint32_t rfband);
    void (*_sys_drv_rf_ctrl)(uint8_t role);
    void (*_sys_drv_set_ana_cb_cal_trig)(uint32_t value);
    void (*_sys_drv_set_ana_cb_cal_manu)(uint32_t value);
    void (*_sys_ll_set_ana_reg5_adc_div)(uint32_t value);
    uint32_t (*_sys_ll_get_ana_reg5_adc_div)(void);
    void (*_sys_ll_set_ana_reg8_ioldo_lp)(uint32_t value);
    void (*_sys_ll_set_ana_reg9_vcorehsel)(uint32_t value);
    void (*_sys_drv_set_spi_latch1v)(uint32_t value);
    void (*_sys_ll_set_ana_reg10_iobyapssen)(uint32_t value);
    void (*_sys_ll_set_ana_reg11_aldosel)(uint32_t value);
    void (*_sys_ll_set_ana_reg12_dldosel)(uint32_t value);
    void (*_sys_drv_set_iobyapssen)(uint32_t value);
    void (*_sys_drv_set_ana_ioldo_lp)(uint32_t value);
    void (*_sys_drv_optim_dpd_tx)(uint32_t vbias);

    uint32_t (*_sys_drv_cali_bias)(uint32_t update);
    uint32_t (*_sys_drv_cali_dpll)(uint32_t param);
    void (*_sys_drv_set_bias)(uint32_t value);
    uint32_t (*_sys_drv_cali_bgcalm)(void);
    uint32_t (*_sys_drv_analog_get_xtalh_ctune)(void);
    uint32_t (*_sys_drv_analog_set_xtalh_ctune)(uint32_t param);
    int32 (*_sys_drv_module_power_state_get)(uint32_t module);
    void (*_phy_sys_drv_modem_bus_clk_ctrl_on)(void);
    void (*_phy_sys_drv_modem_clk_ctrl_on)(void);

    int (*_bk_adc_channel_raw_read)(int32_t adc_channel, uint16_t* buf, uint32_t size, uint32_t timeout);

    int (*_bk_cal_saradc_start)(int32_t adc_channel, int32_t adc_clk, int32_t steady_time);
    int (*_bk_cal_saradc_stop)(int32_t adc_channel);

    int (*_bk_saradc_start)(uint8_t adc_channel, uint32_t sample_rate, uint32_t div,
                                        uint32_t saradc_clk, uint32_t saradc_mode, uint32_t saradc_xtal, uint32_t saturate_mode, uint32_t steady_time);
    int (*_bk_saradc_stop)(uint8_t adc_channel);

    int (*_bk_flash_read_bytes)(uint32_t address, uint8_t *user_buf, uint32_t size);
    int (*_bk_flash_erase_sector)(uint32_t address);
    int (*_bk_flash_write_bytes)(uint32_t address, const uint8_t *user_buf, uint32_t size);

    bk_err_t (*_bk_flash_set_protect_type_protect_none)(void);
    bk_err_t (*_bk_flash_set_protect_type_unprotect_last_block)(void);
    uint32_t (*_bk_flash_partition_get_rf_firmware_info)(void);

    int (*_bk_otp_apb_read)(uint32_t item, uint8_t* buf, uint32_t size);
    int (*_bk_otp_apb_update)(uint32_t item, uint8_t* buf, uint32_t size);

    void (*_log)(int level, char *tag, const char *fmt, ...);
    void (*_log_raw)(int level, char *tag, const char *fmt, ...);
    uint32_t (*_get_time)(void);

    void (*_bk_printf)(const char *fmt, ...);
    void (*_bk_null_printf)(const char *fmt, ...);
    void (*_shell_set_log_level)(int level);

    void *(*_os_malloc)(uint32_t size);
    void (*_os_free)(void *mem_ptr);
    int32_t (*_os_memcmp)(const void *s, const void *s1, uint32_t n);
    void *(*_os_memset)(void *b, int c, UINT32 len);
    void *(*_os_zalloc)(size_t size);
    UINT32 (*_os_strtoul)(const char *nptr, char **endptr, int base);
    int(*_os_strcasecmp)(const char *s1, const char *s2);
    int32_t (*_os_strcmp)(const char *s1, const char *s2);
    int32_t (*_os_strncmp)(const char *s1, const char *s2, const uint32_t n);
    void *(*_os_memcpy)(void *out, const void *in, uint32_t n);

    void (*_rtos_assert)(uint32_t exp);
    void (*_rtos_enable_int)(uint32_t int_level);
    uint32_t (*_rtos_disable_int)(void);
    uint32_t (*_rtos_get_time)(void);
    int (*_rtos_start_timer)(void *timer);
    int(*_rtos_init_timer)(void *timer, uint32_t time_ms, void *function, void *arg);
    int (*_rtos_deinit_timer)(void *timer);
    bk_err_t (*_rtos_reload_timer)(void *timer);
    int (*_rtos_delay_milliseconds)(uint32_t num_ms);
    bk_err_t (*_rtos_deinit_mutex)(void** mutex);
    bk_err_t (*_rtos_init_mutex)(void** mutex);
    bk_err_t (*_rtos_lock_mutex)(void** mutex);
    bk_err_t (*_rtos_unlock_mutex)(void** mutex);
    bool     (*_rtos_is_critical_section)(void);
    
////for bk7256
    uint64 (*_riscv_get_mtimer)(void);

    void (*_sys_drv_set_ana_vctrl_sysldo)(uint32_t value);
    uint32_t (*_sys_drv_analog_reg3_set)(uint32_t value);
    uint32_t (*_sys_drv_analog_reg2_set)(uint32_t value);
    void (*_sys_drv_flash_cksel)(uint32_t value);
    void (*_sys_drv_set_ana_scal_en)(uint32_t value);
    void (*_sys_drv_set_ana_gadc_buf_ictrl)(uint32_t value);
    void (*_sys_drv_set_ana_gadc_cmp_ictrl)(uint32_t value);
    void (*_sys_drv_set_ana_vtempsel)(uint32_t value);
    void (*_sys_drv_set_ana_vref_sel)(uint32_t value);
    void (*_sys_drv_set_ana_vhsel_ldodig)(uint32_t value);
    void (*_sys_drv_set_ana_pwd_gadc_buf)(uint32_t value);
    uint32_t (*_sys_drv_set_bgcalm)(uint32_t value);
    uint32_t (*_sys_drv_get_bgcalm)(void);
    uint32_t (*_sys_drv_set_vdd_value)(uint32_t value);
    uint32_t (*_sys_drv_get_vdd_value)(void);


    uint32_t (*_aon_pmu_hal_get_reg0x7c)(void);
    uint32_t (*_aon_pmu_get_device_id)(void);

    int (*_gpio_dev_unmap)(uint32_t gpio_id);
    int (*_gpio_dev_map_txen)(uint32_t gpio_id);
    int (*_bk_gpio_pull_down)(uint32_t gpio_id);

    uint32_t (*_bk_get_hardware_chip_id_version)(void);
    int (*_bk_efuse_read_byte)(uint8_t addr, uint8_t *data);

    /* For SARADC */
    void (*_bk_set_g_saradc_flag)(UINT8 flag);
    int (*_bk_set_temp_callback)(void *callback, int resigter);
    int (*_bk_set_volt_callback)(void *callback, int resigter);

    /* For BT */
    bk_err_t (*_ble_in_dut_mode)(void);
    uint8_t (*_get_tx_pwr_idx)(void);
    void (*_txpwr_max_set_bt_polar)(void);
    void (*_ble_tx_testmode_retrig)(void);
    uint8_t(*_ble_adv_txpwr_set_feature_enable)(void);

    int (*_gpio_dev_map_rxen)(uint32_t gpio_id);
    int (*_bk_set_env_enhance)(const char *key, const void *value, int value_len);
    int (*_bk_get_env_enhance)(const char *key, void *value, int value_len);
    uint8_t (*_bk_phy_get_wifi_media_mode_config)(void);
    int (*_bk_feature_save_rfcali_to_otp_enable)(void);
    int (*_bk_otp_ahb_read)(uint32_t item, uint8_t* buf, uint32_t size);
    int (*_bk_otp_ahb_update)(uint32_t item, uint8_t* buf, uint32_t size);
    int (*_bk_get_otp_ahb_rfcali_item)(void);
    void (*_sys_ll_set_ana_reg8_violdosel)(uint32_t value);
    void (*_sys_ll_set_ana_reg8_iocurlim)(uint32_t value);
    int(*_bk_feature_volt_5v)(void);
    int(*_bk_feature_phy_log_enable)(void);
    void *(*_os_malloc_sram)(uint32_t size);
    int (*_cli_printf)(const char *fmt, ...);
    int(*_bk_feature_temp_high_volt_enable)(void);
    uint32_t (*_bk_phy_get_cali_flag)(void);
    bool(*_me_is_connect_with_instrument)(void);
} phy_os_funcs_t;


typedef struct {

    uint32_t _saradc_autotest;

    uint32_t _rf_cfg_tssi_item;
    uint32_t _rf_cfg_mode_item;
    uint32_t _rf_cfg_tssi_b_item;

    uint32_t _cmd_tl410_clk_pwr_up;
    uint32_t _pwd_ble_clk_bit;

    uint32_t _pm_cpu_frq_60m;
    uint32_t _pm_cpu_frq_80m;
    uint32_t _pm_cpu_frq_120m;
    uint32_t _pm_cpu_frq_160m;
    uint32_t _pm_cpu_frq_240m;
    uint32_t _pm_cpu_frq_default;

    uint32_t _pm_dev_id_phy;
    uint32_t _pm_dev_id_phy_dpd_cali;



    uint32_t _dd_dev_type_ble;
    uint32_t _dd_dev_type_sctrl;
    uint32_t _dd_dev_type_icu;
    uint32_t _dd_dev_type_flash;


    uint32_t _chip_version_a;
    uint32_t _chip_version_b;
    uint32_t _chip_version_c;
    uint32_t _chip_version_default;

#if (CONFIG_SOC_BK7256XX)
    uint32_t _pm_chip_id_high_pos;
    uint32_t _pm_chip_id_high_value;
#else

#if (CONFIG_SOC_BK7239XX)
    uint32_t _pm_chip_id_mask;
    uint32_t _pm_chip_id_mpw_v2;
    uint32_t _pm_chip_id_mpw_v3;
#else
    uint32_t _pm_chip_id_mask;
    uint32_t _pm_chip_id_mpw_v2_3;
    uint32_t _pm_chip_id_mpw_v4;
    uint32_t _pm_chip_id_mp_A;
#endif
#endif

    uint32_t _cmd_get_device_id;
    uint32_t _cmd_sctrl_ble_powerdown;
    uint32_t _cmd_sctrl_ble_powerup;

#if (!CONFIG_SOC_BK7231)
    uint32_t _cmd_ble_rf_bit_set;
    uint32_t _cmd_ble_rf_bit_clr;
#endif

    uint32_t _cmd_sctrl_get_vdd_value;
    uint32_t _cmd_sctrl_set_vdd_value;


    int32_t  _param_xtalh_ctune_mask;
    uint32_t _param_aud_dac_gain_mask;

    uint32_t _pm_power_module_state_on;
    uint32_t _pm_power_module_state_off;
    uint32_t _pm_power_module_state_none;

    uint32_t _adc_temp_10degree_per_dbpwr;
    uint32_t _adc_temp_buffer_size;
    uint32_t _ADC_TEMP_SENSER_CHANNEL;
    uint32_t _ADC_VOLT_SENSER_CHANNEL;
    uint32_t _adc_temp_lsb_per_10degree;
    uint32_t _adc_temp_val_min;
    uint32_t _adc_temp_val_max;
    uint32_t _ADC_TEMP_CODE_DFT_25DEGREE;
    uint32_t _adc_xtal_dist_intial_val;
    uint32_t _adc_temp_dist_intial_val;

    uint32_t _ieee80211_band_2ghz ;
    uint32_t _ieee80211_band_5ghz ;
    uint32_t _ieee80211_band_6ghz ;
    uint32_t _ieee80211_band_60ghz;
    uint32_t _ieee80211_num_bands ;

    uint32_t _OTP_MAC_ADDRESS1;
    uint32_t _OTP_RFCALI1;
    uint32_t _OTP_VDDDIG_BANDGAP;
    uint32_t _OTP_DIA;
    uint32_t _OTP_GADC_TEMPERATURE;
    uint32_t _OTP_CHIP_RESERVED;
} phy_os_variable_t;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void bk_phy_adapter_init(void);
extern void rwnx_cal_mac_sleep_rc_clr(void);
extern void rwnx_cal_mac_sleep_rc_recover(void);


#endif
