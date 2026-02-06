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

#include "bk_phy_adapter.h"

#include "driver/wdt.h"
#include "bk_wdt.h"
#include "bk_wifi.h"
#include "adc_driver.h"

#include "sys_driver.h"
#include "soc/soc.h"
#include "sys_ll.h"
#include "sys_ana_ll.h"

#include "sys_types.h"
#include "aon_pmu_hal.h"
#include "aon_pmu_driver.h"
#include "bk_rf_internal.h"
#include "bk_feature.h"
#include "bk_icu.h"
#include "temp_detect.h"
#include "volt_detect.h"
#include "bk_saradc.h"
#include <components/ate.h>
#include "bk_misc.h"
#include <modules/pm.h>
#include <modules/chip_support.h>

#include <components/system.h>
#include <os/mem.h>
#include <os/str.h>

#include <common/bk_assert.h>

#include "gpio_driver.h"
#include <driver/gpio.h>
#include <driver/efuse.h>
#include <driver/adc.h>
#include <driver/otp.h>

#if CONFIG_ARMINO_BLE
#include "bluetooth_internal.h"
#endif

#include "bk_rf_adapter.h"
#include "bk_wifi_phy_internal.h"
#include "bk_sys_ctrl.h"
#include "adc_hal.h"

#ifdef CONFIG_FREERTOS_SMP
#include "spinlock.h"
#endif // CONFIG_FREERTOS_SMP

#if CONFIG_EASY_FLASH
#if CONFIG_EASY_FLASH_V4
#include "bk_ef.h"
#endif
#endif

#include "bk_cli.h"

#define PHY_OSI_VERSION              0x00060006

UINT32 g_phy_cali_flag =             0x1;

void bk_phy_set_cali_flag(UINT32 flag)
{
    g_phy_cali_flag = flag;
}

UINT32 bk_phy_get_cali_flag(void)
{
    return g_phy_cali_flag;
}

static void bk_set_g_saradc_flag(UINT8 flag)
{
}

static uint32_t bk_get_hardware_chip_id_version_wrapper(void)
{
    return bk_get_hardware_chip_id_version();
}

static void shell_set_log_level_wrapper(int level)
{
#if (CONFIG_SHELL_ASYNCLOG)
    shell_set_log_level(level);
#endif
}

static void *os_malloc_wrapper(uint32_t size)
{
    return (void *)os_malloc(size);
}

static void *os_malloc_sram_wrapper(uint32_t size)
{
    return (void *)os_malloc_sram(size);
}

static void os_free_wrapper(void *mem_ptr)
{
    os_free(mem_ptr);
}
static int32_t os_memcmp_wrapper(const void *s, const void *s1, uint32_t n)
{
	return os_memcmp(s, s1, (unsigned int)n);
}
static void *os_memset_wrapper(void *b, int c, UINT32 len)
{
	return (void *)os_memset(b, c, (unsigned int)len);
}
static void *os_zalloc_wrapper(size_t size)
{
	return os_zalloc(size);
}
static UINT32 os_strtoul_wrapper(const char *nptr, char **endptr, int base)
{
	return os_strtoul(nptr, endptr, base);
}
static int os_strcasecmp_wrapper(const char *s1, const char *s2)
{
	return os_strcasecmp(s1, s2);
}
static int32_t os_strcmp_wrapper(const char *s1, const char *s2)
{
    return os_strcmp(s1, s2);
}
static int32_t os_strncmp_wrapper(const char *s1, const char *s2, const uint32_t n)
{
	return os_strncmp(s1, s2, n);
}
static void *os_memcpy_wrapper(void *out, const void *in, uint32_t n)
{
	return (void *)os_memcpy(out, in, n);
}



static int phy_gpio_dev_unmap_wrapper(uint32_t gpio_id)
{
    return gpio_dev_unmap(gpio_id);
}



static int phy_bk_gpio_pull_down_wrapper(uint32_t gpio_id)
{
    return bk_gpio_pull_down(gpio_id);
}

static int bk_set_temperature_callback_wrapper(void *callback, int resigter)
{
#if (CONFIG_TEMP_DETECT)
	if (resigter) {
	    return bk_sensor_register_temperature_callback((bk_sensor_callback)callback);
	} else {
	    return bk_sensor_unregister_temperature_callback((bk_sensor_callback)callback);
	}
#else
    return BK_ERR_NOT_SUPPORT;
#endif
}

static int bk_set_voltage_callback_wrapper(void *callback, int resigter)
{
#if (CONFIG_VOLT_DETECT)
	if (resigter) {
	    return bk_sensor_register_voltage_callback((bk_sensor_callback)callback);
	} else {
	    return bk_sensor_unregister_voltage_callback((bk_sensor_callback)callback);
	}
#else
    return BK_ERR_NOT_SUPPORT;
#endif
}

static int bk_otp_apb_read_wrapper(uint32_t item, uint8_t* buf, uint32_t size)
{
#if (!CONFIG_OTP)
    return BK_FAIL;
#else
    return bk_otp_apb_read(item, buf, size);
#endif
}

static int bk_otp_apb_update_wrapper(uint32_t item, uint8_t* buf, uint32_t size)
{
#if (!CONFIG_OTP)
    return BK_FAIL;
#else
    return bk_otp_apb_update(item, buf, size);
#endif
}

static int bk_otp_ahb_read_wrapper(uint32_t item, uint8_t* buf, uint32_t size)
{
#if (!CONFIG_OTP)
    return BK_FAIL;
#else
    return bk_otp_ahb_read(item, buf, size);
#endif
}

static int bk_otp_ahb_update_wrapper(uint32_t item, uint8_t* buf, uint32_t size)
{
#if (!CONFIG_OTP)
    return BK_FAIL;
#else
    return bk_otp_ahb_update(item, buf, size);
#endif
}

static int bk_get_otp_ahb_rfcali_item_wrapper(void)
{
#if (!CONFIG_OTP)
    return BK_FAIL;
#else
    return OTP_RFCALI1;
#endif
}

static uint32_t aon_pmu_hal_get_reg0x7c(void)
{
    return aon_pmu_hal_reg_get(PMU_REG0x7c);
}
static uint32_t aon_pmu_get_device_id(void)
{
#if CONFIG_SOC_BK7256XX
    UINT32 device_id;
    UINT8 device_addr_idx = 0xA;

    device_id = aon_pmu_hal_reg_get(device_addr_idx);
    return device_id;
#else
    return 0;
#endif
}

static int gpio_dev_map_rxen(uint32_t gpio_id)
{
    return gpio_dev_map(gpio_id, GPIO_DEV_RXEN);
}

static int gpio_dev_map_txen(uint32_t gpio_id)
{
    return gpio_dev_map(gpio_id, GPIO_DEV_TXEN);
}

static bk_err_t bk_pm_clock_ctrl_saradc_pwrup(void)
{
    int32_t temp = 0;
    temp = bk_pm_clock_ctrl(PM_CLK_ID_SARADC, PM_CLK_CTRL_PWR_UP);
    return temp;
}
static bk_err_t bk_pm_clock_ctrl_phy_pwrup(void)
{
    int32_t temp = 0;
    temp = bk_pm_clock_ctrl(PM_CLK_ID_PHY, PM_CLK_CTRL_PWR_UP);
    return temp;
}

static bk_err_t bk_pm_module_vote_power_ctrl_phy(int32_t on_off)
{
    int32_t temp = 0;
    temp = bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_PHY, on_off);
    return temp;
}

static bk_err_t bk_pm_module_vote_cpu_freq_phy(uint32_t module, uint32_t cpu_freq)
{
    return bk_pm_module_vote_cpu_freq((pm_dev_id_e)module, (pm_cpu_freq_e)cpu_freq);
}

static int32 sys_drv_module_power_state_get_wrapper(uint32_t module)
{
    return sys_drv_module_power_state_get(module);
}
static void phy_sys_drv_modem_bus_clk_ctrl_on(void)
{
    sys_drv_modem_bus_clk_ctrl(SYS_DRV_CLK_ON);
}
static void phy_sys_drv_modem_clk_ctrl_on(void)
{
    sys_drv_modem_clk_ctrl(SYS_DRV_CLK_ON);
}

static int bk_flash_read_bytes_wrapper(uint32_t address, uint8_t *user_buf, uint32_t size)
{
#if (CONFIG_OTP && CONFIG_PHY_RFCALI_TO_OTP)
    return BK_ERR_NOT_SUPPORT;
#else
    return bk_flash_read_bytes(address, user_buf, size);
#endif
}

static int bk_flash_erase_sector_wrapper(uint32_t address)
{
#if (CONFIG_OTP && CONFIG_PHY_RFCALI_TO_OTP)
    return BK_ERR_NOT_SUPPORT;
#else
    return bk_flash_erase_sector(address);
#endif
}

static int bk_flash_write_bytes_wrapper(uint32_t address, const uint8_t *user_buf, uint32_t size)
{
#if (CONFIG_OTP && CONFIG_PHY_RFCALI_TO_OTP)
    return BK_ERR_NOT_SUPPORT;
#else
    return bk_flash_write_bytes(address, user_buf, size);
#endif
}

static uint32_t bk_flash_partition_get_rf_firmware_info(void)
{
#if (CONFIG_OTP && CONFIG_PHY_RFCALI_TO_OTP)
    return 0;
#else
#if (CONFIG_RF_FIRMWARE_DYNAMIC_PARTITION)
    uint32_t rf_partition_start_addr = bk_flash_get_capacity_bytes() - FLASH_RF_FIRMWARE_OFFSET;
    //bk_printf("rf_partition_start_addr=%x\r\n",rf_partition_start_addr);
    return rf_partition_start_addr;
#else
    bk_logic_partition_t * pt = NULL;
    pt = bk_flash_partition_get_info(BK_PARTITION_RF_FIRMWARE);
    return pt->partition_start_addr;
#endif

#endif
}

static bk_err_t bk_flash_set_protect_type_protect_none(void)
{
#if (CONFIG_OTP && CONFIG_PHY_RFCALI_TO_OTP)
    return BK_ERR_NOT_SUPPORT;
#else
    return bk_flash_set_protect_type(FLASH_PROTECT_NONE);
#endif
}

static bk_err_t bk_flash_set_protect_type_unprotect_last_block(void)
{
#if (CONFIG_OTP && CONFIG_PHY_RFCALI_TO_OTP)
    return BK_ERR_NOT_SUPPORT;
#else
    return bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
#endif
}

static void rtos_assert_wrapper(uint32_t exp)
{
    BK_ASSERT(exp);
}

#ifdef CONFIG_FREERTOS_SMP
static volatile spinlock_t phy_spin_lock = SPIN_LOCK_INIT;
#endif // CONFIG_FREERTOS_SMP
static uint32_t rtos_disable_int_wrapper(void)
{
	uint32_t int_level = rtos_disable_int();
	#ifdef CONFIG_FREERTOS_SMP
	spin_lock(&phy_spin_lock);
	#endif // CONFIG_FREERTOS_SMP
	return int_level;
}
static void rtos_enable_int_wrapper(uint32_t int_level)
{
	#ifdef CONFIG_FREERTOS_SMP
	spin_unlock(&phy_spin_lock);
	#endif // CONFIG_FREERTOS_SMP
	rtos_enable_int(int_level);
}
static uint32_t rtos_get_time_wrapper(void)
{
    return rtos_get_time();
}
static bk_err_t rtos_reload_timer_wrapper(void *timer)
{
	return rtos_reload_timer(timer);
}
static bk_err_t rtos_start_timer_wrapper(void *timer)
{
	return rtos_start_timer(timer);
}
static bk_err_t rtos_init_timer_wrapper(void *timer, uint32_t time_ms, void *function, void *arg)
{
	return rtos_init_timer(timer, time_ms, function, arg);
}
static bk_err_t rtos_deinit_timer_wrapper(void *timer)
{
	return rtos_deinit_timer(timer);
}
static bk_err_t rtos_delay_milliseconds_wrapper(uint32_t num_ms)
{
	return rtos_delay_milliseconds(num_ms);
}

static bk_err_t rtos_deinit_mutex_wrapper(void **mutex)
{
	return rtos_deinit_mutex(mutex);
}

static bk_err_t rtos_init_mutex_wrapper(void **mutex)
{
	return rtos_init_mutex(mutex);
}

static bk_err_t rtos_lock_mutex_wrapper(void **mutex)
{
	return rtos_lock_mutex(mutex);
}
static bk_err_t rtos_unlock_mutex_wrapper(void ** mutex )
{
	return rtos_unlock_mutex(mutex);
}

static bk_err_t ble_in_dut_mode_wrapper(void)
{
#if CONFIG_ARMINO_BLE
    return ble_in_dut_mode();
#else
    return 0;
#endif
}

static void ble_rf_test_mode_retirg(void)
{
#if CONFIG_ARMINO_BLE
        bluetooth_rf_test_mode_retrig();
#endif
}

static uint8_t ble_adv_txpwr_set_feature_enable_wrapper(void)
{
#if CONFIG_SUPPORT_BLE_ADV_TXPWR_SET
        return 1;
#else
        return 0;
#endif
}

static uint8_t get_tx_pwr_idx_wrapper(void)
{
#if CONFIG_ARMINO_BLE
    return get_tx_pwr_idx();
#else
    return 0;
#endif
}

static void txpwr_max_set_bt_polar_wrapper(void)
{
#if CONFIG_ARMINO_BLE
#if (CONFIG_SOC_BK7239) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7286 || CONFIG_SOC_BK7258)
    txpwr_max_set_bt_polar();
#endif
#endif
}

void * mpb_reg_api_wrapper(void)
{
#if CONFIG_WIFI_ENABLE
    return mpb_reg_api();
#else
    return NULL;
#endif
}

void * crm_reg_api_wrapper(void)
{
#if CONFIG_WIFI_ENABLE
    return crm_reg_api();
#else
    return NULL;
#endif
}

void * riu_reg_api_wrapper(void)
{
#if (CONFIG_WIFI_ENABLE)
    return riu_reg_api();
#else
    return NULL;
#endif
}

void * mix_funcs_wrapper(void)
{
#if (CONFIG_WIFI_ENABLE)
    return mix_funcs();
#else
    return NULL;
#endif
}

static uint32_t bk_misc_get_reset_reason_wrapper(void)
{
    return bk_misc_get_reset_reason();
}

static void sys_drv_set_iobyapssen(uint32_t v) {
#if (CONFIG_SOC_BK7236N) || (CONFIG_SOC_BK7239XX)
    sys_hal_set_ana_reg_spi_latch1v(1);
    if (v) {
        //tenglong20250508: ioldo >= vbat(3.5V) before iobypass change
        sys_hal_set_ioldo_volt(6);
        sys_hal_set_ioldo_bypass(1);
    } else {
        sys_hal_set_ioldo_bypass(0);
        sys_hal_set_ioldo_volt(4); //shuguang20250512:vio=3.3V
    }
    sys_hal_set_ana_reg_spi_latch1v(0);
#elif (CONFIG_SOC_BK7259)
#elif (CONFIG_SOC_BK7236XX)
	sys_ll_set_ana_reg9_spi_latch1v(1);
	if (v) {
		sys_ll_set_ana_reg10_iobyapssen(1);
	} else {
		//shuguang20240718: use default current limit
		sys_ll_set_ana_reg10_iobyapssen(0);
	}
	sys_ll_set_ana_reg9_spi_latch1v(0);
#endif
}

static void sys_ll_set_ana_reg8_violdosel_wrapper(uint32_t flag) {
#if (CONFIG_SOC_BK7236N) || (CONFIG_SOC_BK7239XX)
	//TODO: fix me when bk7236n bring up
#elif (CONFIG_SOC_BK7259)
#elif (CONFIG_SOC_BK7236XX)
	sys_ll_set_ana_reg9_spi_latch1v(1);
	if (flag) {
		sys_ll_set_ana_reg8_violdosel(2);
	} else {
		//shuguang20240718: use default current limit
		sys_ll_set_ana_reg8_violdosel(4);
	}
	sys_ll_set_ana_reg9_spi_latch1v(0);
#endif
}

void sys_drv_set_pwd_anabuf_lownoise(uint32_t v) {
#if (CONFIG_SOC_BK7239XX)
    //ronghui20250514:lp for rx
	sys_ana_ll_set_ana_reg5_anabufsel_rx1(v);
	sys_ana_ll_set_ana_reg3_hpssren(v);
#endif
}

uint32_t sys_drv_rfband(uint32_t rfband) {
	return BK_OK;
}

static void sys_drv_rf_ctrl(uint8_t type)
{
    sys_hal_rf_ctrl(type);
}

static void sys_drv_optim_dpd_tx(uint32_t vbias)
{
#if (CONFIG_SOC_BK7236N)
    sys_hal_set_ana_reg_spi_latch1v(1);
    sys_hal_set_analdo_sel(1);
    sys_hal_set_analdo_volt(0xF);
    sys_hal_set_ana_cb_cal_manu_val(vbias);
#endif
}

static int bk_wdt_stop_wrapper(void) {
#if (CONFIG_TASK_WDT)
	bk_task_wdt_stop();
#endif
#if CONFIG_WDT_EN
	return bk_wdt_stop();
#else
    return BK_FAIL;
#endif
}

static bk_err_t bk_cal_saradc_start(int32_t adc_channel, int32_t adc_clk, int32_t steady_time)
{
	adc_config_t config = {0};

#if CONFIG_SARADC_V1P1
	BK_RETURN_ON_ERR(bk_adc_acquire());
	BK_LOG_ON_ERR(bk_adc_init(adc_channel));

	config.chan          = adc_channel;
	config.adc_mode      = ADC_CONTINUOUS_MODE;
	config.clk           = adc_clk;

	config.src_clk       = ADC_SCLK_XTAL_26M;

	config.saturate_mode = ADC_SATURATE_MODE_3;
	config.sample_rate   = 0;
	config.steady_ctrl   = steady_time;
	config.adc_filter    = 0;
	if (adc_channel == 8) {
#if (CONFIG_SOC_BK7236XX) && (!CONFIG_SOC_BK7236N)
        //donghui20231108:
        //CHx: 0:1/4 1:1/3 2:1/2 3:1/1
        //CH0: 0:1/7 1:1/5 2:1/3 3:1/1
        sys_ll_set_ana_reg5_adc_div(3);
#endif
		//config.vol_div = ADC_VOL_DIV_1;
	}

	BK_LOG_ON_ERR(bk_adc_set_config(&config));
	BK_LOG_ON_ERR(bk_adc_enable_bypass_clalibration());
	BK_LOG_ON_ERR(bk_adc_start());
#else
	config.chan          = adc_channel;
	config.adc_mode      = ADC_CONTINUOUS_MODE;
	config.clk           = adc_clk;
	config.src_clk       = ADC_SCLK_XTAL;
	config.saturate_mode = ADC_SATURATE_MODE_3;
	config.sample_rate   = 0;
	config.steady_ctrl   = steady_time;
	config.adc_filter    = 0;
	if (adc_channel == 8) {
		config.vol_div = ADC_VOL_DIV_1;
	}

	BK_LOG_ON_ERR(bk_adc_channel_init(&config));
#endif

	return BK_OK;
}

static bk_err_t bk_cal_saradc_stop(int32_t adc_channel)
{
#if CONFIG_SARADC_V1P1
	BK_LOG_ON_ERR(bk_adc_stop());
	BK_LOG_ON_ERR(bk_adc_deinit(adc_channel));
	if (adc_channel == 8) {
#if (CONFIG_SOC_BK7236XX) && (!CONFIG_SOC_BK7236N)
        //donghui20231108:
        //CHx: 0:1/4 1:1/3 2:1/2 3:1/1
        //CH0: 0:1/7 1:1/5 2:1/3 3:1/1
        sys_ll_set_ana_reg5_adc_div(1);
#endif
	}
	BK_LOG_ON_ERR(bk_adc_release());
#else
	BK_LOG_ON_ERR(bk_adc_channel_deinit(adc_channel));
#endif
	return BK_OK;
}

static bk_err_t bk_saradc_start(uint8_t adc_channel, uint32_t sample_rate, uint32_t div,
                                        uint32_t saradc_clk, uint32_t saradc_mode, uint32_t saradc_xtal, uint32_t saturate_mode, uint32_t steady_time)
{
#if SARADC_AUTOTEST
    extern bk_err_t bk_saradc_set_config(adc_config_t *config, uint32_t div);

	uint32_t adc_clk = 0;
	adc_config_t config = {0}; __maybe_unused_var(config);
    BK_RETURN_ON_ERR(bk_adc_acquire());
    BK_LOG_ON_ERR(bk_adc_init(adc_channel));
	adc_clk = saradc_clk/2/(div + 1);

    config.chan           = adc_channel;
    config.adc_mode       = saradc_mode;
    config.clk            = adc_clk;
    config.src_clk        = saradc_xtal;
    config.saturate_mode  = saturate_mode;
    config.sample_rate    = sample_rate;
    config.steady_ctrl    = steady_time;
    config.adc_filter     = 0;

    BK_LOG_ON_ERR(bk_saradc_set_config(&config, div));
    BK_LOG_ON_ERR(bk_adc_enable_bypass_clalibration());
    BK_LOG_ON_ERR(bk_adc_start());
    return BK_OK;
#else
    return 0;
#endif
}

static bk_err_t bk_saradc_stop(uint8_t adc_channel)
{
#if SARADC_AUTOTEST
	BK_LOG_ON_ERR(bk_adc_stop());
	BK_LOG_ON_ERR(bk_adc_deinit(adc_channel));
	BK_LOG_ON_ERR(bk_adc_release());
	return BK_OK;
#else
    return 0;
#endif
}

static int phy_set_env_enhance_wrapper(const char *key, const void *value, int value_len)
{
#if CONFIG_EASY_FLASH
#if CONFIG_EASY_FLASH_V4
    return bk_set_env_enhance(key,value,value_len);
#else
    return 0;
#endif
#else
    return 0;
#endif
}

static int phy_get_env_enhance_wrapper(const char *key, void *value, int value_len)
{
#if CONFIG_EASY_FLASH
#if CONFIG_EASY_FLASH_V4
    return bk_get_env_enhance(key,value,value_len);
#else
    return 0;
#endif
#else
    return 0;
#endif
}

static UINT8 bk_phy_get_wifi_media_mode_config_wrapper(void)
{
#if (CONFIG_SOC_BK7258)
    bool flag = 0;

    bk_wifi_get_wifi_media_mode_config(&flag);
    return flag;
#else
    return 0;
#endif
}

static int bk_adc_channel_raw_read_wrapper(int32_t channel_id, uint16_t* buf, uint32_t sample_cnt, uint32_t timeout)
{
#if CONFIG_SARADC_V1P1
    return bk_adc_read_raw(buf, sample_cnt, timeout);
#else
    return bk_adc_channel_raw_read(channel_id, buf, sample_cnt, timeout);
#endif
}

bool rtos_is_critical_section_wrapper(void)
{
    return rtos_local_irq_disabled() || rtos_is_in_interrupt_context();
}

extern uint64 riscv_get_mtimer(void);

const phy_os_funcs_t g_phy_os_funcs = {

    ._version = PHY_OSI_VERSION,
    ._mpb_reg_api              = mpb_reg_api_wrapper,
    ._crm_reg_api              = crm_reg_api_wrapper,
    ._riu_reg_api              = riu_reg_api_wrapper,
    ._mix_funcs                = mix_funcs_wrapper,
    ._bk_misc_get_reset_reason = bk_misc_get_reset_reason_wrapper,

#if CONFIG_WIFI_ENABLE
    /* modules/wifi/... */
    ._rs_init                  = rs_init,
    ._rs_bypass_mac_init       = rs_bypass_mac_init,
    ._rs_deinit                = rs_deinit,
    ._evm_init                 = evm_phy_init,
    ._evm_bypass_mac_init      = evm_bypass_mac_init,
    ._nv_phy_reg_set_hook      = bk_phy_set_nv_reg_hook,
    ._evm_clear_ke_evt_mac_bit = evm_clear_ke_evt_mac_bit,
    ._evm_set_ke_evt_mac_bit   = evm_set_ke_evt_mac_bit,
    ._tx_evm_set_chan_ctxt_pop = tx_evm_set_chan_ctxt_pop,
    ._save_info_item           = save_info_item,
    ._get_info_item            = get_info_item ,
#else
    ._nv_phy_reg_set_hook      = NULL,
#endif

    ////
    ._delay                    = bk_delay,
    ._delay_us                 = delay_us,
    ._bk_wdt_stop              = bk_wdt_stop_wrapper,

    ._temp_detect_is_init              = temp_detect_is_init,
    ._temp_detect_deinit               = temp_detect_deinit,
    ._temp_detect_init                 = temp_detect_init,
    ._temp_detect_get_temperature      = temp_detect_get_temperature,
    ._bk_feature_temp_detect_enable    = bk_feature_temp_detect_enable,

    ._ate_is_enabled                   = ate_is_enabled,
    ._volt_single_get_current_voltage  = volt_single_get_current_voltage,
    ._bk_adc_data_calculate            = bk_adc_data_calculate,

    ._bk_pm_clock_ctrl_saradc_pwrup    = bk_pm_clock_ctrl_saradc_pwrup,
    ._bk_pm_clock_ctrl_phy_pwrup       = bk_pm_clock_ctrl_phy_pwrup,
    ._bk_pm_module_vote_power_ctrl_phy = bk_pm_module_vote_power_ctrl_phy,
    ._bk_pm_module_vote_cpu_freq       = bk_pm_module_vote_cpu_freq_phy,

    ._aon_pmu_drv_bias_cal_get         = aon_pmu_drv_bias_cal_get,
    ._aon_pmu_drv_band_cal_get         = aon_pmu_drv_band_cal_get,
    ._aon_pmu_drv_get_adc_cal          = aon_pmu_drv_get_adc_cal,

#if (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
    ._aon_pmu_hal_get_chipid           = aon_pmu_hal_get_chipid,

    ._sys_drv_set_pwd_anabuf_lownoise  = sys_drv_set_pwd_anabuf_lownoise,
    ._sys_drv_rfband                   = sys_drv_rfband,
    ._sys_drv_rf_ctrl                  = sys_drv_rf_ctrl,
#if CONFIG_SOC_BK7236XX
    ._sys_drv_set_ana_cb_cal_trig      = sys_drv_set_ana_cb_cal_trig,
    ._sys_drv_set_ana_cb_cal_manu      = sys_drv_set_ana_cb_cal_manu,
    ._sys_ll_set_ana_reg5_adc_div      = sys_ll_set_ana_reg5_adc_div,
    ._sys_ll_get_ana_reg5_adc_div      = sys_ll_get_ana_reg5_adc_div,
    ._sys_ll_set_ana_reg8_ioldo_lp     = sys_ll_set_ana_reg8_ioldo_lp,
    ._sys_ll_set_ana_reg9_vcorehsel    = sys_ll_set_ana_reg9_vcorehsel,
    ._sys_drv_set_spi_latch1v          = sys_ll_set_ana_reg9_spi_latch1v,
    ._sys_ll_set_ana_reg10_iobyapssen  = sys_ll_set_ana_reg10_iobyapssen,
    ._sys_ll_set_ana_reg11_aldosel     = sys_ll_set_ana_reg11_aldosel,
	._sys_ll_set_ana_reg12_dldosel     = sys_ll_set_ana_reg12_dldosel,
#endif

    ._sys_drv_set_iobyapssen           = sys_drv_set_iobyapssen,
    ._sys_drv_set_ana_ioldo_lp         = sys_drv_set_ana_ioldo_lp,
    ._sys_drv_optim_dpd_tx             = sys_drv_optim_dpd_tx,
#endif
    ._sys_drv_cali_bias                 = sys_drv_cali_bias,
    ._sys_drv_cali_dpll                 = sys_drv_cali_dpll,
    ._sys_drv_set_bias                  = sys_drv_set_ana_cb_cal_manu_val,
    ._sys_drv_cali_bgcalm               = sys_drv_cali_bgcalm,
    ._sys_drv_analog_get_xtalh_ctune    = sys_drv_analog_get_xtalh_ctune,
    ._sys_drv_analog_set_xtalh_ctune    = sys_drv_analog_set_xtalh_ctune,
    ._sys_drv_module_power_state_get    = sys_drv_module_power_state_get_wrapper,
    ._phy_sys_drv_modem_bus_clk_ctrl_on = phy_sys_drv_modem_bus_clk_ctrl_on,
    ._phy_sys_drv_modem_clk_ctrl_on     = phy_sys_drv_modem_clk_ctrl_on,
    ._bk_adc_channel_raw_read           = (int (*)(int32_t , uint16_t*, uint32_t, uint32_t))bk_adc_channel_raw_read_wrapper,
    ._bk_cal_saradc_start               = bk_cal_saradc_start,
    ._bk_cal_saradc_stop                = bk_cal_saradc_stop,
    ._bk_saradc_start                   = bk_saradc_start,
    ._bk_saradc_stop                    = bk_saradc_stop,
    ._bk_flash_read_bytes               = bk_flash_read_bytes_wrapper,
    ._bk_flash_erase_sector                          = bk_flash_erase_sector_wrapper,
    ._bk_flash_write_bytes                           = bk_flash_write_bytes_wrapper,
    ._bk_flash_set_protect_type_protect_none         = bk_flash_set_protect_type_protect_none,
    ._bk_flash_set_protect_type_unprotect_last_block = bk_flash_set_protect_type_unprotect_last_block,
    ._bk_flash_partition_get_rf_firmware_info        = bk_flash_partition_get_rf_firmware_info,
    ._bk_otp_apb_read                                = bk_otp_apb_read_wrapper,
    ._bk_otp_apb_update                              = bk_otp_apb_update_wrapper,
    ._log                                  = bk_printf_ext,
    ._log_raw                              = bk_printf_raw,
    ._get_time                             = rtos_get_time_wrapper,
    ._bk_printf                            = bk_printf,
    ._bk_null_printf                       = bk_null_printf,
    ._shell_set_log_level                  = shell_set_log_level_wrapper,
    ._os_malloc                            = os_malloc_wrapper,
    ._os_free                              = os_free_wrapper,
    ._os_memcmp                            = os_memcmp_wrapper,
    ._os_memset                            = os_memset_wrapper,
    ._os_zalloc                            = os_zalloc_wrapper,
    ._os_strtoul                           = os_strtoul_wrapper,
    ._os_strcasecmp                        = os_strcasecmp_wrapper,
    ._os_strcmp                            = os_strcmp_wrapper,
    ._os_strncmp                           = os_strncmp_wrapper,
    ._os_memcpy                            = os_memcpy_wrapper,
    ._rtos_assert                          = rtos_assert_wrapper,
    ._rtos_enable_int                      = rtos_enable_int_wrapper,
    ._rtos_disable_int                     = rtos_disable_int_wrapper,
    ._rtos_get_time                        = rtos_get_time_wrapper,
    ._rtos_start_timer                     = rtos_start_timer_wrapper,
    ._rtos_init_timer                      = rtos_init_timer_wrapper,
    ._rtos_deinit_timer                    = rtos_deinit_timer_wrapper,
    ._rtos_reload_timer                    = rtos_reload_timer_wrapper,
    ._rtos_delay_milliseconds              = rtos_delay_milliseconds_wrapper,
    ._rtos_deinit_mutex                    = rtos_deinit_mutex_wrapper,
    ._rtos_init_mutex                      = rtos_init_mutex_wrapper,
    ._rtos_lock_mutex                      = rtos_lock_mutex_wrapper,
    ._rtos_unlock_mutex                    = rtos_unlock_mutex_wrapper,
    ._rtos_is_critical_section             = rtos_is_critical_section_wrapper,
    ._sys_drv_set_bgcalm                   = sys_drv_set_bgcalm,
    ._sys_drv_get_bgcalm                   = sys_drv_get_bgcalm,
    ._sys_drv_set_vdd_value                = sys_drv_set_vdd_value,
    ._sys_drv_get_vdd_value                = sys_drv_get_vdd_value,

    /* For SARADC */
    ._bk_set_g_saradc_flag                 = bk_set_g_saradc_flag,
    ._bk_set_temp_callback                 = bk_set_temperature_callback_wrapper,
    ._bk_set_volt_callback                 = bk_set_voltage_callback_wrapper,

    /* For BT */
    ._ble_in_dut_mode                      = ble_in_dut_mode_wrapper,
    ._get_tx_pwr_idx                       = get_tx_pwr_idx_wrapper,
    ._txpwr_max_set_bt_polar               = txpwr_max_set_bt_polar_wrapper,
    ._ble_tx_testmode_retrig               = ble_rf_test_mode_retirg,
    ._ble_adv_txpwr_set_feature_enable     = ble_adv_txpwr_set_feature_enable_wrapper,
    ._gpio_dev_map_rxen                    = gpio_dev_map_rxen,
    ._bk_set_env_enhance                   = phy_set_env_enhance_wrapper,
    ._bk_get_env_enhance                   = phy_get_env_enhance_wrapper,
    ._bk_phy_get_wifi_media_mode_config    = bk_phy_get_wifi_media_mode_config_wrapper,
    ._bk_feature_save_rfcali_to_otp_enable = bk_feature_save_rfcali_to_otp_enable,
    ._bk_otp_ahb_read                      = bk_otp_ahb_read_wrapper,
    ._bk_otp_ahb_update                    = bk_otp_ahb_update_wrapper,
    ._bk_get_otp_ahb_rfcali_item           = bk_get_otp_ahb_rfcali_item_wrapper,
#if (CONFIG_SOC_BK7236XX)
    ._sys_ll_set_ana_reg8_violdosel        = sys_ll_set_ana_reg8_violdosel_wrapper,
    ._sys_ll_set_ana_reg8_iocurlim         = sys_ll_set_ana_reg8_iocurlim,
    ._bk_feature_volt_5v                   = bk_feature_volt_5v,
#else
    ._sys_ll_set_ana_reg8_violdosel        = NULL,
    ._sys_ll_set_ana_reg8_iocurlim         = NULL,
    ._bk_feature_volt_5v                   = NULL,
#endif
    ._bk_feature_phy_log_enable            = bk_feature_phy_log_enable,
    ._os_malloc_sram                       = os_malloc_sram_wrapper,
    ._cli_printf                           = cli_printf,
    ._bk_feature_temp_high_volt_enable     = bk_feature_temp_high_volt_enable,
    ._bk_phy_get_cali_flag                 = bk_phy_get_cali_flag,
    ._me_is_connect_with_instrument        = me_is_connect_with_instrument,
};

const phy_os_variable_t g_phy_os_variable = {

    ._saradc_autotest        = SARADC_AUTOTEST,

    ._rf_cfg_tssi_item       = RF_CFG_TSSI_ITEM,
    ._rf_cfg_mode_item       = RF_CFG_MODE_ITEM,
    ._rf_cfg_tssi_b_item     = RF_CFG_TSSI_B_ITEM,

    ._cmd_tl410_clk_pwr_up   = CMD_TL410_CLK_PWR_UP,
    ._pwd_ble_clk_bit        = PWD_BLE_CLK_BIT,


    ._pm_cpu_frq_60m         = PM_CPU_FRQ_60M,
    ._pm_cpu_frq_80m         = PM_CPU_FRQ_80M,
    ._pm_cpu_frq_120m        = PM_CPU_FRQ_120M,
    ._pm_cpu_frq_160m        = PM_CPU_FRQ_160M,
    ._pm_cpu_frq_240m        = PM_CPU_FRQ_240M,
    ._pm_cpu_frq_default     = PM_CPU_FRQ_DEFAULT,

    ._pm_dev_id_phy          = PM_DEV_ID_PHY,
    ._pm_dev_id_phy_dpd_cali = PM_DEV_ID_PHY_DPD_CALI,

#if ((CONFIG_ARMINO_BLE))
    ._dd_dev_type_ble      = DD_DEV_TYPE_BLE,
#endif
    ._dd_dev_type_sctrl    = DD_DEV_TYPE_SCTRL,
    ._dd_dev_type_icu      = DD_DEV_TYPE_ICU,

    ._chip_version_a       = CHIP_VERSION_A,
    ._chip_version_b       = CHIP_VERSION_B,
    ._chip_version_c       = CHIP_VERSION_C,
    ._chip_version_default = CHIP_VERSION_DEFAULT,

#if (CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX)
    ._pm_chip_id_mask      = PM_CHIP_ID_MASK,
    ._pm_chip_id_mpw_v2    = PM_CHIP_ID_MPW_V2,
    ._pm_chip_id_mpw_v3    = PM_CHIP_ID_MPW_V3,
#else
    ._pm_chip_id_mask      = PM_CHIP_ID_MASK,
    ._pm_chip_id_mpw_v2_3  = PM_CHIP_ID_MPW_V2_3,
    ._pm_chip_id_mpw_v4    = PM_CHIP_ID_MPW_V4,
    ._pm_chip_id_mp_A      = PM_CHIP_ID_MP_A,
#endif
    ._cmd_get_device_id             = CMD_GET_DEVICE_ID,
    ._cmd_sctrl_ble_powerdown       = CMD_SCTRL_BLE_POWERDOWN,
    ._cmd_sctrl_ble_powerup         = CMD_SCTRL_BLE_POWERUP,
    ._cmd_ble_rf_bit_set            = CMD_BLE_RF_BIT_SET,
    ._cmd_ble_rf_bit_set            = CMD_BLE_RF_BIT_CLR,
    ._cmd_sctrl_get_vdd_value       = CMD_SCTRL_GET_VDD_VALUE,
    ._cmd_sctrl_set_vdd_value       = CMD_SCTRL_SET_VDD_VALUE,
    ._param_xtalh_ctune_mask        = PARAM_XTALH_CTUNE_MASK,
    ._param_aud_dac_gain_mask       = PARAM_AUD_DAC_GAIN_MASK,
    ._pm_power_module_state_on      = PM_POWER_MODULE_STATE_ON,
    ._pm_power_module_state_off     = PM_POWER_MODULE_STATE_OFF,
    ._pm_power_module_state_none    = PM_POWER_MODULE_STATE_NONE,
    ._adc_temp_10degree_per_dbpwr   = ADC_TMEP_10DEGREE_PER_DBPWR,
    ._adc_temp_buffer_size          = ADC_TEMP_BUFFER_SIZE,
    ._ADC_TEMP_SENSER_CHANNEL       = ADC_TEMP_SENSOR_CHANNEL,
    ._ADC_VOLT_SENSER_CHANNEL       = ADC_VOLT_SENSER_CHANNEL,
    ._adc_temp_lsb_per_10degree     = ADC_TMEP_LSB_PER_10DEGREE,
    ._adc_temp_val_min              = ADC_TEMP_VAL_MIN,
    ._adc_temp_val_max              = ADC_TEMP_VAL_MAX,
    ._ADC_TEMP_CODE_DFT_25DEGREE    = ADC_TEMP_CODE_DFT_25DEGREE,
    ._adc_xtal_dist_intial_val      = ADC_XTAL_DIST_INTIAL_VAL,
    ._adc_temp_dist_intial_val      = ADC_TMEP_DIST_INTIAL_VAL,

    ._ieee80211_band_2ghz           = IEEE80211_BAND_2GHZ ,
    ._ieee80211_band_5ghz           = IEEE80211_BAND_5GHZ ,
    ._ieee80211_band_6ghz           = IEEE80211_BAND_6GHZ ,
    ._ieee80211_band_60ghz          = IEEE80211_BAND_60GHZ,
    ._ieee80211_num_bands           = IEEE80211_NUM_BANDS ,

#if CONFIG_OTP
    ._OTP_MAC_ADDRESS1              = OTP_MAC_ADDRESS1,
    ._OTP_RFCALI1                   = OTP_RFCALI1,
    ._OTP_VDDDIG_BANDGAP            = OTP_VDDDIG_BANDGAP,
    ._OTP_DIA                       = OTP_DIA ,
    ._OTP_GADC_TEMPERATURE          = OTP_GADC_TEMPERATURE,
    #if CONFIG_SOC_BK7236XX
    ._OTP_CHIP_RESERVED             = OTP_CHIP_RESERVED,
    #endif
#endif
};

extern void phy_adapter_init(const void * phy_funcs, const void * phy_vars);

void bk_phy_adapter_init(void)
{
    phy_adapter_init(&g_phy_os_funcs, &g_phy_os_variable);
}

