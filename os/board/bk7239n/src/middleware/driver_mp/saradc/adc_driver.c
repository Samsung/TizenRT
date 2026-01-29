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

#define TAG "sadc"

#include <common/bk_include.h>
#include <stdbool.h>
#include <common/bk_compiler.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include "gpio_driver.h"
#include "adc_statis.h"
#include "adc_driver.h"
#include <driver/int.h>
#include "icu_driver.h"
#include "power_driver.h"
#include "clock_driver.h"
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "sys_driver.h"
#include "iot_adc.h"
#include "common/bk_err.h"
#include <modules/pm.h>
#include "bk_rf_internal.h"
#include "aon_pmu_hal.h"
#include "temp_detect.h"
#include <driver/otp.h>

uint32_t rc_drv_get_rf_rxon(void);

struct sadc_device *saradc_dev_ptr = NULL;
static uint8_t s_saradc_use_calibration_val_flag = 0x0;

#define ADC_CALIBRATION_DATA_NUM    (10000)
#define __unused                    __attribute__((__unused__))

#define ADC_RETURN_ON_NOT_INIT() do {\
		if (NULL == saradc_dev_ptr) {\
			ADC_LOGE("adc driver not init\r\n");\
			return BK_ERR_ADC_NOT_INIT;\
		}\
	} while(0)

#define ADC_RETURN_ON_INVALID_CHAN(id) do {\
		if (!adc_hal_is_valid_channel(&saradc_dev_ptr->hal, (id))) {\
			ADC_LOGE("ADC id number(%d) is invalid\r\n", (id));\
			return BK_ERR_ADC_INVALID_CHAN;\
		}\
	} while(0)

#define ADC_RETURN_ON_INVALID_MODE(mode) do {\
		if ((mode) >= ADC_NONE_MODE) {\
			return BK_ERR_ADC_INVALID_MODE;\
		}\
	} while(0)

#define ADC_RETURN_ON_INVALID_SRC_CLK(src_clk) do {\
		if ((src_clk) >= ADC_SCLK_NONE) {\
			return BK_ERR_ADC_INVALID_SCLK_MODE;\
		}\
	} while(0)

static bool adc_try_load_otp_cwt(uint32_t *otp_cwt_buf)
{
    uint32_t otp_data_size;
    uint8_t *data = NULL;
    bk_err_t result = BK_FAIL;
    uint32_t required_size = ADC_CWT_COEF_NUM * sizeof(uint32_t);

#if CONFIG_SOC_BK7236XX
    otp_data_size = otp_map_1[OTP_GADC_CALIBRATION].allocated_size;
#else
    otp_data_size = otp_map_2[OTP_GADC_CALIBRATION].allocated_size;
#endif

    // Check if OTP data size is sufficient for CWT coefficients
    // For bk7236/bk7236n, otp_data_size is too small (4/8 bytes), so this will return false
    if ((otp_cwt_buf == NULL) || (otp_data_size < required_size)) {
        return false;
    }

    data = (uint8_t *)os_zalloc(otp_data_size);
    if (data == NULL) {
        ADC_LOGE("malloc otp data buffer failed\r\n");
        return false;
    }

#if CONFIG_SOC_BK7236XX
    result = bk_otp_apb_read(OTP_GADC_CALIBRATION, data, otp_data_size);
#else
    result = bk_otp_ahb_read(OTP_GADC_CALIBRATION, data, otp_data_size);
#endif
    if (result != BK_OK) {
        ADC_LOGW("read otp gadc calib failed:%d\r\n", result);
        os_free(data);
        return false;
    }

    for (uint32_t idx = 0; idx < ADC_CWT_COEF_NUM; idx++) {
        uint32_t otp_words;
        os_memcpy(&otp_words, &data[idx * 4], sizeof(uint32_t));
        otp_cwt_buf[idx] = otp_words & 0xFFFFFF;
    }

    if ((otp_cwt_buf[ADC_CWT_COEF_NUM - 1] == 0)) {//The last set of self-calibration values has the highest weight.
        ADC_LOGW("invalid otp cwt value:[%x]\r\n",otp_cwt_buf[ADC_CWT_COEF_NUM - 1]);
        os_free(data);
        return false;
    }

    os_free(data);
    return true;
}

static void adc_flush(struct sadc_device *dev)
{
    adc_hal_t *hal_ptr = &dev->hal;

    adc_hal_clear_int_status(hal_ptr);
    while(!adc_hal_is_fifo_empty(hal_ptr)) {
        adc_hal_get_adc_data(hal_ptr);
    }
}

static bk_err_t adc_init_gpio(struct sadc_device *dev, adc_chan_t chan)
{
    #if defined(CONFIG_SARADC_V1P1)
    if (adc_hal_is_analog_channel(&dev->hal, chan))
        return BK_OK;
    #endif // CONFIG_SARADC_V1P1

    adc_gpio_map_t adc_map_table[] = ADC_DEV_MAP;
    adc_gpio_map_t *adc_map = &adc_map_table[chan];

    #if defined(CONFIG_SARADC_V1P2)
    /*TODO Fixme:wangzhilei*/
    if (adc_hal_is_analog_channel(&dev->hal, chan)) {
        gpio_dev_unmap(adc_map->gpio_id);
        bk_gpio_disable_pull(adc_map->gpio_id);
        bk_gpio_disable_input(adc_map->gpio_id);
        bk_gpio_disable_output(adc_map->gpio_id);

        return BK_OK;
    }
    #endif // CONFIG_SARADC_V1P2

    uint32_t map_item_cnt = sizeof(adc_map_table) / sizeof(adc_map_table[0]);
    if(((uint32_t)chan >= map_item_cnt)
            || (MAP_INVALID_ITEM == adc_map->gpio_dev)
            || (MAP_INVALID_ITEM == adc_map->gpio_id)) {
        return BK_FAIL;
    }

    //TODO optimize it
    if(chan == 0) {
        uint32_t param = PARAM_SARADC_BT_TXSEL_BIT;

        sys_drv_analog_reg4_bits_or(param);// to do,need remove old interface after all adaption is finished
    }

    gpio_dev_map(adc_map->gpio_id, adc_map->gpio_dev);
    bk_gpio_disable_pull(adc_map->gpio_id);
    bk_gpio_disable_input(adc_map->gpio_id);
    bk_gpio_disable_output(adc_map->gpio_id);

    return BK_OK;
}

static void adc_deinit_gpio(struct sadc_device *dev, adc_chan_t chan)
{
    if (adc_hal_is_analog_channel(&dev->hal, chan))
        return;

    adc_gpio_map_t adc_map_table[] = ADC_DEV_MAP;
    adc_gpio_map_t *adc_map = &adc_map_table[chan];

    gpio_dev_unmap(adc_map->gpio_id);
}

static void adc_enable_block(void)
{
    #if (!defined(CONFIG_SOC_BK7231N))
    //TODO - optimize it after sysctrl driver optimized!
    uint32_t param = BLK_BIT_SARADC;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);
    #endif
}

static bk_err_t adc_chan_init_common(struct sadc_device *dev, adc_chan_t chan)
{
    bk_err_t ret;
    adc_hal_t *hal_ptr;

    hal_ptr = &dev->hal;
    adc_hal_stop_commom(hal_ptr);

    ret = adc_init_gpio(dev, chan);
    if(BK_OK != ret) {
        return ret;
    }
    adc_hal_sel_channel(hal_ptr, chan);

    return ret;
}

static bk_err_t adc_chan_deinit_common(struct sadc_device *dev, adc_chan_t chan)
{
    adc_hal_t *hal_ptr = &dev->hal;
    if(NULL == hal_ptr->hw) {
        return BK_OK;
    }

    adc_hal_stop_commom(hal_ptr);
    sys_drv_sadc_int_disable();

    adc_flush(dev);

    sys_drv_sadc_pwr_down();
    adc_deinit_gpio(dev, chan);

    return BK_OK;
}

static bk_err_t adc_config_rx_buf(struct sadc_data *data, uint16_t* buf, uint32_t size)
{
    data->buffer = buf;
    data->sampling_index = 0;
    data->sampling_request = size / DEFAULT_ADC_SAMPLE_UNIT_BYTES;

    return BK_OK;
}

static bk_err_t adc_enable_bypass_calibration(struct sadc_device *dev)
{
    __unused adc_hal_t *hal_ptr = &dev->hal;

    adc_hal_enable_bypass_calib(hal_ptr);
    return BK_OK;
}

static bk_err_t adc_disable_bypass_calibration(struct sadc_device *dev)
{
    __unused adc_hal_t *hal_ptr = &dev->hal;

    adc_hal_disable_bypass_calib(hal_ptr);
    return BK_OK;
}

static bk_err_t adc_set_channel(struct sadc_device *dev, adc_chan_t adc_chan)
{
    adc_hal_sel_channel(&dev->hal, adc_chan);

    return BK_OK;
}

bk_err_t adc_activate_config(struct sadc_device *dev, adc_config_t *config)
{
    adc_hal_t *hal_ptr = &dev->hal;

    adc_hal_set_clk(hal_ptr, config->src_clk, config->clk);
    adc_hal_set_mode(hal_ptr, config->adc_mode);

    adc_hal_set_steady_ctrl(hal_ptr, config->steady_ctrl);
    adc_hal_set_saturate_mode(hal_ptr, config->saturate_mode);

    if (config->vol_div == ADC_VOL_DIV_NONE) {
        if (config->chan == ADC_0) {
            config->vol_div = ADC_VOL_DIV_5;
            adc_hal_set_vol_div(config->chan, ADC_VOL_DIV_5);
        } else {
            config->vol_div = ADC_VOL_DIV_3;
            adc_hal_set_vol_div(config->chan, ADC_VOL_DIV_3);
        }
    } else {
        adc_hal_set_vol_div(config->chan, config->vol_div);
    }

    adc_set_channel(dev, config->chan);

    if (config->adc_mode == ADC_CONTINUOUS_MODE) {
        adc_hal_set_sample_rate(hal_ptr, config->sample_rate);
        adc_hal_set_adc_filter(hal_ptr, config->adc_filter);
    }

    if (0 == config->is_hw_using_cali_result) {
        adc_enable_bypass_calibration(dev);
    } else {
        adc_disable_bypass_calibration(dev);
    }

    return BK_OK;
}

uint32_t bk_adc_get_temp_code_dft_25degree(void)
{
    return adc_hal_get_temp_code_dft_25degree();
}

uint32_t bk_adc_get_temp_code_dft_threshold(void)
{
    return adc_hal_get_temp_code_dft_threshold();
}

uint32_t bk_adc_get_temp_lsb_per_10degree(void)
{
    return adc_hal_get_temp_lsb_per_10degree();
}

uint32_t bk_adc_get_1Volt_value(void)
{
    return adc_hal_get_1Volt_value();
}

uint32_t bk_adc_get_1Volt_threshold(void)
{
    return adc_hal_get_1Volt_threshold();
}

uint32_t bk_adc_get_2Volt_value(void)
{
    return adc_hal_get_2Volt_value();
}

uint32_t bk_adc_get_2Volt_threshold(void)
{
    return adc_hal_get_2Volt_threshold();
}

__IRAM_SEC static int adc_pm_restore_cb(uint64_t sleep_time, void *args)
{
    (void)sleep_time;
    (void)args;
    hal_calib_apply();
    return BK_OK;
}

static bk_err_t adc_set_default_cali_val(struct sadc_data *data)
{
    const uint16_t cali_val[SARADC_CALIBRATE_MAX] = {
        #if (defined(CONFIG_SOC_BK7236N) || defined(CONFIG_SOC_BK7236Q))
        /*TODO Fixme: wangzhilei*/
        0, 0x11AA, 0x2382, ADC_TEMP_CODE_DFT_25DEGREE, ADC_TMEP_LSB_PER_10DEGREE /* 1Volt, 2Volt, 25Degree, 10Step*/
        #elif (defined(CONFIG_SOC_BK7239XX))
        0x4ca, 0x1194, 0x23ea, ADC_TEMP_CODE_DFT_25DEGREE, ADC_TMEP_LSB_PER_10DEGREE /* 1Volt, 2Volt, 25Degree, 10Step*/
        #elif (defined(CONFIG_SOC_BK7259))
        0, 0x1481, 0x279d, ADC_TEMP_CODE_DFT_25DEGREE, ADC_TMEP_LSB_PER_10DEGREE /* 1Volt, 2Volt, 25Degree, 10Step*/
        #elif (defined(CONFIG_SOC_BK7236XX))
        0, 0x9C7,0x1358, ADC_TEMP_CODE_DFT_25DEGREE, ADC_TMEP_LSB_PER_10DEGREE /* 1Volt, 2Volt, 25Degree, 10Step*/
        #else
        0, 0x55, 0x354, ADC_TEMP_CODE_DFT_25DEGREE, ADC_TMEP_LSB_PER_10DEGREE
        #endif
    };

    os_memcpy(data->sadc_cali_val, cali_val, sizeof(cali_val));

    return BK_OK;
}

static bk_err_t adc_cli_register(void)
{
    #if defined(CONFIG_SADC_API_TEST)
    int bk_sadc_register_cli_api_test_feature(void);
    bk_sadc_register_cli_api_test_feature();
    #endif

    #if defined(CONFIG_SARADC_TEST)
    int bk_sadc_register_cli_test_feature(void);
    bk_sadc_register_cli_test_feature();
    #endif

    #if defined(CONFIG_ADC_API_TEST)
    int bk_adc_api_register_cli_test_feature(void);
    bk_adc_api_register_cli_test_feature();
    #endif

    return BK_OK;
}

static bk_err_t adc_init(struct sadc_device *dev)
{
    adc_hal_t *hal_ptr = &dev->hal;

    sys_drv_sadc_pwr_up();

    #if defined(CONFIG_SARADC_V1P1)
    adc_enable_block();
    #elif defined(CONFIG_SARADC_V1P2)
    sys_hal_set_saradc_config();
    #else
#error "config saradc asic version, please"
    #endif
    adc_hal_init(hal_ptr);

    adc_cli_register();

    return BK_OK;
}

static bk_err_t adc_set_sample_threshold(struct sadc_device *dev, uint32_t cnt)
{
    if ((cnt == 0) || (cnt > SOC_ADC_SAMPLE_CNT_MAX)) {
        cnt = SOC_ADC_SAMPLE_CNT_MAX;
    }

    adc_hal_set_fifo_threshold(&dev->hal, cnt);

    return BK_OK;
}

bk_err_t adc_channel_config(struct sadc_device *dev, adc_chan_t adc_chan)
{
    bk_err_t ret;

    ADC_RETURN_ON_NOT_INIT();
    ADC_RETURN_ON_INVALID_CHAN(adc_chan);

    #if (defined(CONFIG_WIFI_ENABLE) || defined(CONFIG_ARMINO_BLE))
    rf_module_vote_ctrl(RF_OPEN, RF_BY_SARADC_BIT);
    #endif
    adc_init(dev);

    #if (defined(CONFIG_SARADC_NEED_FLUSH))
    adc_flush(dev);
    #endif
    ret = adc_chan_init_common(dev, adc_chan);
    if(BK_OK != ret) {
        return ret;
    }

    adc_set_sample_threshold(dev, ADC_SAMPLE_THRESHOLD_DEFAULT);

    #if defined(CONFIG_SARADC_V1P1)
    sys_hal_set_saradc_config();
    #endif // CONFIG_SARADC_V1P1

    return BK_OK;
}

bk_err_t bk_adc_channel_deinit(adc_chan_t chan)
{
    struct sadc_device *dev = saradc_dev_ptr;
    adc_config_t *config_cache_ptr;

    ADC_RETURN_ON_NOT_INIT();
    ADC_RETURN_ON_INVALID_CHAN(chan);

    adc_chan_deinit_common(dev, chan);

    bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SADC, PM_POWER_MODULE_STATE_OFF);

    #if (defined(CONFIG_WIFI_ENABLE) || defined(CONFIG_ARMINO_BLE))
    rf_module_vote_ctrl(RF_CLOSE, RF_BY_SARADC_BIT);
    #endif

    config_cache_ptr = dev->channel_cfg[chan];
    if(config_cache_ptr) {
        os_free(config_cache_ptr);
        config_cache_ptr = NULL;

        dev->channel_cfg[chan] = NULL;
    }

    return BK_OK;
}

bk_err_t adc_hw_activate(struct sadc_device *dev)
{
    adc_hal_t *hal_ptr = &dev->hal;

    if(adc_hal_check_adc_busy(hal_ptr))
        return BK_ERR_ADC_BUSY;

    if (adc_hal_check_adc_enable(hal_ptr))
        return BK_OK;

    adc_hal_start_commom(hal_ptr);

    return BK_OK;
}

static bk_err_t adc_stop_conversion(struct sadc_device *dev)
{
    adc_hal_t *hal_ptr = &dev->hal;

    if (!adc_hal_check_adc_enable(hal_ptr))
        return BK_OK;

    sys_drv_sadc_int_disable();
    adc_flush(dev);
    adc_hal_stop_commom(hal_ptr); /* for power consumption*/

    return BK_OK;
}

static bk_err_t adc_wait_for_read_complete(struct sadc_device *dev, uint32_t timeout)
{
    bk_err_t ret = BK_OK;
    uint32 tick1 ;
    uint32 tick2;
    uint32 tick_duration;
    #if (defined(CONFIG_SARADC_REVISION))
    dev->rf_active_nums = rc_drv_get_rf_rxon();
    #endif
    adc_flush(dev);
    sys_drv_sadc_int_enable();
    tick1 = rtos_get_time();
    while(sys_drv_sadc_get_int_en())
    {
        tick2 = rtos_get_time();
        tick_duration = (tick2 >= tick1) ? (tick2 - tick1) : ((0xFFFFFFFF- tick1) + tick2) ;
        if (tick_duration >= 4)
        {
            bk_printf(" adc_wait for_read timeout ");
            break;
        }
    }
    ret = rtos_get_semaphore(&dev->ctx.sync, timeout);
    if(ret != kNoErr) {
        ret = BK_ERR_ADC_GET_READ_SEMA;
    }

    return ret;
}

bk_err_t bk_adc_single_read(uint16_t* data)
{
    adc_hal_t *hal_ptr = &saradc_dev_ptr->hal;

    *data = adc_hal_get_adc_data(hal_ptr);

    return BK_OK;
}

static bk_err_t adc_current_channel_convert(struct sadc_device *dev, uint16_t* buf, uint32_t buf_size, uint32_t timeout)
{
    bk_err_t ret;

    ret = adc_wait_for_read_complete(dev, timeout);

    adc_stop_conversion(dev);
    sys_drv_set_ana_pwd_gadc_buf(0);

    return ret;
}

static bk_err_t adc_revision_raw(struct sadc_device *dev, uint16_t* buf, uint32_t buf_size, uint32_t timeout)
{
    #if (defined(CONFIG_SARADC_REVISION))
    uint32_t average = 0;
    uint16_t index;
    uint16_t threshold;
    uint16_t rf_active_count = 0;
    uint16_t rf_idle_count = 0;
    uint16_t revision_count = 0;
    uint32_t rf_active_average = 0;
    uint32_t rf_idle_average = 0;
    uint32_t rf_active_threshold = 0;

    if (!dev->rf_active_patch) {
        return BK_OK;
    }

    if (adc_hal_is_analog_channel(&dev->hal, dev->adc_chan) && (dev->adc_chan != ADC_0)) {
        return BK_OK;
    }

    //0. just revision when rf_active
    if (dev->rf_active_nums == 0) {
        return BK_OK;
    }

    //1. estimate voltage(calc ADC average instead)
    if (dev->data.sampling_request == 0) {
        return BK_OK;
    }
    for (index = 0; index < dev->data.sampling_request; index++) {
        average += buf[index];
    }
    average = average / dev->data.sampling_request;

    //2. calc threshold between RF_ON and RF_ACTIVE: ~1.5%, change threshold to 0.5%
    threshold = average / 200;

    //3. split data with threshold
    for (index = 0; index < dev->data.sampling_request; index++) {
        if (buf[index] >= average + threshold) {
            rf_idle_count++;
            rf_idle_average += buf[index];
            //ADC_LOGE("+[%d]%d\r\n", index, buf[index]);
        } else if (buf[index] <= average - threshold) {
            rf_active_count++;
            rf_active_average += buf[index];
            //ADC_LOGE("-[%d]%d\r\n", index, buf[index]);
        } else {
            //ADC_LOGE("*[%d]%d\r\n", index, buf[index]);
        }
    }

    //ADC_LOGE("rf_active_nums=%d,idle=%d/%d,active=%d/%d,threshold=%d,average=%d\r\n", dev->rf_active_nums, rf_idle_average, rf_idle_count, rf_active_average, rf_active_count, threshold, average);
    //4. revision data if needed(ignore max/min value)
    if ((rf_idle_count > 1) || (rf_active_count > 1)) {
        //4.1. no mater RF_ACTIVE or RF_IDLE, revision partial data
        if (rf_idle_count > 1) {
            rf_idle_average /= rf_idle_count;
            rf_active_threshold = rf_idle_average - 2 * threshold;
        } else {
            rf_active_average /= rf_active_count;
            rf_active_threshold = rf_active_average + threshold;
        }
        for (index = 0; index < dev->data.sampling_request; index++) {
            if (buf[index] < rf_active_threshold) {
                //ADC_LOGE("[%d]%d->%d\r\n", index, buf[index], buf[index] + 3 * threshold);
                buf[index] += 3 * threshold;
                revision_count++;
            }
        }
    }
    if ((revision_count == 0) && (dev->rf_active_nums != 0)) {
        //4.2. revision all data
        for (index = 0; index < dev->data.sampling_request; index++) {
            //ADC_LOGE("[%d]%d->%d\r\n", index, buf[index], buf[index] + 3 * threshold);
            buf[index] += 3 * threshold;
        }
    } else {
        //4.3. keep all data
    }
    #endif

    return BK_OK;
}

bk_err_t bk_adc_get_raw(adc_chan_t chan_id, uint16_t* buf, uint32_t buf_size, uint32_t timeout)
{
    bk_err_t ret = BK_OK;
    struct sadc_device *dev;
    struct sadc_context *ctx_ptr;
    adc_config_t *channel_config_ptr;

    dev = saradc_dev_ptr;
    if((NULL == buf) || (NULL == dev)) {
        ret = BK_FAIL;
        goto get_exit;
    }

    if(0 == buf_size) {
        goto get_exit;
    }

    channel_config_ptr = dev->channel_cfg[chan_id];
    if(NULL == channel_config_ptr) {
        ret = BK_ERR_ADC_CHAN_NOT_INIT;
        goto get_exit;
    }
    ctx_ptr = &dev->ctx;
    BK_LOG_ON_ERR(adc_context_lock(ctx_ptr));

    #if (defined(CONFIG_SARADC_REVISION))
    dev->adc_chan = chan_id;
    #endif

    BK_LOG_ON_ERR(adc_channel_config(dev, chan_id));
    BK_LOG_ON_ERR(adc_activate_config(dev, channel_config_ptr));
    BK_LOG_ON_ERR(adc_config_rx_buf(&dev->data, buf, buf_size));
    BK_LOG_ON_ERR(adc_hw_activate(dev));
    BK_LOG_ON_ERR(adc_current_channel_convert(dev, buf,buf_size, timeout));
    BK_LOG_ON_ERR(adc_revision_raw(dev, buf,buf_size, timeout));

    BK_LOG_ON_ERR(adc_context_release(ctx_ptr));

get_exit:
    return ret;
}

bk_err_t bk_adc_channel_init(adc_config_t *config)
{
    adc_chan_t channel_id;
    adc_config_t *config_cache_ptr;
    struct sadc_device *dev = saradc_dev_ptr;

    BK_RETURN_ON_NULL(dev);
    BK_RETURN_ON_NULL(config);
    ADC_RETURN_ON_INVALID_MODE(config->adc_mode);
    ADC_RETURN_ON_INVALID_SRC_CLK(config->src_clk);
    ADC_RETURN_ON_INVALID_CHAN(config->chan);

    channel_id = config->chan;
    config_cache_ptr  = dev->channel_cfg[channel_id];
    if(NULL == config_cache_ptr) {
        ;
        config_cache_ptr = (adc_config_t *)os_malloc_sram(sizeof(*config));
        //config_cache_ptr = (adc_config_t *)os_malloc(sizeof(*config));
    }

    if(NULL == config_cache_ptr) {
        return BK_FAIL;
    }

    os_memcpy(config_cache_ptr, config, sizeof(*config));
    dev->channel_cfg[channel_id] = config_cache_ptr;

    #if defined(CONFIG_SARADC_PM_CB_SUPPORT)
    pm_cb_conf_t resume_cfg;
    resume_cfg.cb   = (pm_cb)adc_pm_restore_cb;
    resume_cfg.args = NULL;
    bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_SARADC, NULL, &resume_cfg);
    bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SADC, PM_POWER_MODULE_STATE_ON);
    #endif

    return BK_OK;
}

uint32_t bk_saradc_read_raw_data(uint32_t timeout)
{
    uint32_t raw_data;
    struct sadc_device *dev = saradc_dev_ptr;
    adc_hal_t *hal_ptr = &dev->hal;

    raw_data = adc_hal_get_adc_data(hal_ptr);

    return raw_data;
}

bk_err_t bk_adc_channel_raw_read(adc_chan_t channel_id, uint16_t* buf, uint32_t sample_cnt, uint32_t timeout)
{
    bk_err_t ret;

    ret = bk_adc_get_raw(channel_id, buf, sample_cnt * DEFAULT_ADC_SAMPLE_UNIT_BYTES, timeout);

    return ret;
}

bk_err_t bk_adc_channel_read(adc_chan_t chan_id, uint16_t *data, uint32_t timeout)
{
    int ret = BK_OK;
    uint32_t sum = 0;
    uint16_t samples[DEFAULT_AVERAGE_SAMPLE_SIZE] = {0};
    uint32_t num, average_sample_count;

    ret = bk_adc_get_raw(chan_id, samples, DEFAULT_AVERAGE_SAMPLE_SIZE * DEFAULT_ADC_SAMPLE_UNIT_BYTES, timeout);

    /* calculate the sample mean*/
    average_sample_count = DEFAULT_AVERAGE_SAMPLE_SIZE;
    num = average_sample_count/2 + average_sample_count%2;
    for (uint16_t i = num; i < average_sample_count; i++) {
        sum += samples[i];
    }

    if ((average_sample_count - num) > 0) {
        sum = sum / (average_sample_count - num);
    } else {
        sum = 0;
    }

    #if defined(CONFIG_SARADC_RANGE_DIVIDE)
    sum = sum >> 1;
    #endif

    *data = sum;

    return ret;
}

/* gadc input divider selection based on analog channel of adc
 */
bk_err_t bk_adc_set_vol_div(adc_chan_t adc_chan, adc_vol_div_t vol_div)
{
    ADC_RETURN_ON_INVALID_CHAN(adc_chan);

    adc_hal_set_vol_div(adc_chan, vol_div);

    return BK_OK;
}

static bk_err_t adc_handle_conversion_data(struct sadc_device *dev)
{
    uint32_t sample_id, sample_request_cnt;
    uint32_t is_drop = 0, is_recved = 0;
    struct sadc_context *ctx_ptr = &dev->ctx;
    struct sadc_data *data_ptr = &dev->data;
    uint16_t *buf_ptr = data_ptr->buffer;
    adc_hal_t *hal_ptr = &dev->hal;

    #if defined(CONFIG_ADC_STATIS)
    struct sadc_statistics *stats = &dev->stats;
    #endif
    ADC_STATIS_INC(stats->adc_isr_cnt);

    sample_id = data_ptr->sampling_index;
    sample_request_cnt = data_ptr->sampling_request;

    if(NULL == buf_ptr) {
        ADC_LOGI("ADC rx buf null\r\n");
        is_drop = 1;
    }

    while(!adc_hal_is_fifo_empty(hal_ptr)) {
        ADC_STATIS_INC(stats->adc_rx_total_cnt);

        if(sample_id >= sample_request_cnt) {
            is_drop = 1;
        }

        if(0 == is_drop) {
            #if (defined(CONFIG_SARADC_REVISION))
            if ((dev->rf_active_patch) && (!adc_hal_is_analog_channel(hal_ptr, dev->adc_chan) || (dev->adc_chan == ADC_0))) {
                //drop 2 in 3 to trigger ISR more times, and we could check RF_ACTIVE more correctly
                adc_hal_get_adc_data(hal_ptr);
                adc_hal_get_adc_data(hal_ptr);
            }
            #endif
            buf_ptr[sample_id ++] = adc_hal_get_adc_data(hal_ptr);
            is_recved ++;

            ADC_STATIS_INC(stats->adc_rx_succ_cnt);
        } else {
            if((adc_hal_get_mode(hal_ptr) == ADC_CONTINUOUS_MODE)) {
                adc_hal_get_adc_data(hal_ptr);

                ADC_STATIS_INC(stats->adc_rx_drop_cnt);
            } else {
                //software mode is NOT used!
            }
        }
    }

    if (is_recved) {
        data_ptr->sampling_index = sample_id;

        if(data_ptr->sampling_index >= data_ptr->sampling_request) {
            adc_stop_conversion(dev);
            rtos_set_semaphore(&(ctx_ptr->sync));
        }
    }

    if(dev->data_callback) {
        dev->data_callback(dev->data.buffer, dev->cb_ctx);
    }

    return BK_OK;
}

static void adc_isr(void)
{
    struct sadc_device *dev;

    dev = saradc_dev_ptr;
    BK_ASSERT(dev);

    adc_hal_clear_int_status(&dev->hal);
    #if (defined(CONFIG_SARADC_REVISION))
    dev->rf_active_nums += rc_drv_get_rf_rxon();
    #endif
    adc_handle_conversion_data(dev);

    if (dev->isr_cb) {
        dev->isr_cb((uint32_t)dev->cb_arg);
    }
}

bk_err_t bk_adc_driver_deinit(void)
{
    uint32_t i;
    bk_err_t ret;
    adc_config_t *chan_cfg;
    struct sadc_device *dev;

    dev = saradc_dev_ptr;
    BK_ASSERT(dev);

    for(i = 0; i < ADC_MAX; i ++) {
        chan_cfg = dev->channel_cfg[i];
        if(chan_cfg) {
            bk_adc_channel_deinit(i);
        }
    }

    ret = adc_hal_deinit(&dev->hal);
    bk_int_isr_unregister(INT_SRC_SARADC);

    ret = adc_context_deinit(&dev->ctx);
    if(BK_OK != ret) {
        return ret;
    }

    os_memset(dev, 0xff, sizeof(*dev));
    os_free(dev);
    dev = NULL;


    saradc_dev_ptr = NULL;

    return BK_OK;
}

bk_err_t bk_adc_driver_init(void)
{
    bk_err_t ret;
    struct sadc_device *dev;

    if(NULL != saradc_dev_ptr) {
        return BK_OK;
    }

    dev = (struct sadc_device *)os_zalloc(sizeof(*dev));
    if(NULL == dev) {
        return BK_ERR_ADC_INSUFFICIENT_MEM;
    }
    saradc_dev_ptr = dev;

    ret = adc_context_init(&dev->ctx);
    if(BK_OK != ret) {
        goto init_failed;
    }

    #if (defined(CONFIG_SARADC_REVISION))
    uint32_t chip_id = aon_pmu_hal_get_chipid();
    if ((chip_id & PM_CHIP_ID_MASK) <= (PM_CHIP_ID_MP_C & PM_CHIP_ID_MASK)) {
        dev->rf_active_patch = 1;
    }
    #endif

    adc_set_default_cali_val(&dev->data);
    bk_int_isr_register(INT_SRC_SARADC, adc_isr, NULL);

    #if defined(CONFIG_SARADC_V1P2)
    bk_adc_enter_calib_mode();
    #endif // CONFIG_SARADC_V1P2

    return BK_OK;

init_failed:
    bk_adc_driver_deinit();

    return ret;
}

bk_err_t bk_adc_is_valid_analog_channel(uint32_t channel_id)
{
    struct sadc_device *dev = saradc_dev_ptr;

    if (dev == NULL) {
        return BK_ERR_ADC_NOT_INIT;
    }

    if (!adc_hal_is_valid_channel(&dev->hal, channel_id))
    {
        ADC_LOGE("ADC id number(%d) is invalid\r\n", (channel_id));
        return BK_ERR_ADC_INVALID_CHAN;
    }

    return BK_OK;
}

bk_err_t bk_adc_register_isr_callback(adc_isr_t isr, uint32_t param)
{
    struct sadc_device *dev = saradc_dev_ptr;

    if(NULL == dev) {
        return BK_FAIL;
    }

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    dev->isr_cb = (adc_isr_cb)isr;
    dev->cb_arg = (void *)param;
    GLOBAL_INT_RESTORE();

    return BK_OK;
}

bk_err_t bk_adc_unregister_isr_callback(void)
{
    struct sadc_device *dev = saradc_dev_ptr;

    if(NULL == dev) {
        return BK_FAIL;
    }

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    dev->isr_cb = (adc_isr_cb)NULL;
    dev->cb_arg = NULL;
    GLOBAL_INT_RESTORE();

    return BK_OK;
}

bk_err_t bk_adc_register_isr_iot_callback(void* iot_callback, void *p_iot_context)
{
    struct sadc_device *dev = saradc_dev_ptr;

    if(NULL == dev) {
        return BK_FAIL;
    }

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    dev->data_callback = (FUNC_2PV_PTR)iot_callback;
    dev->cb_ctx = p_iot_context;
    GLOBAL_INT_RESTORE();

    return BK_OK;
}

bk_err_t bk_adc_unregister_isr_iot_callback(void)
{
    struct sadc_device *dev = saradc_dev_ptr;

    if(NULL == dev) {
        return BK_FAIL;
    }

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    dev->data_callback = (FUNC_2PV_PTR)NULL;
    dev->cb_ctx = NULL;
    GLOBAL_INT_RESTORE();

    return BK_OK;
}

void bk_adc_set_using_calibration_val_flag(void)
{
    s_saradc_use_calibration_val_flag = 1;
}

void bk_adc_clear_using_calibration_val_flag(void)
{
    s_saradc_use_calibration_val_flag = 0;
}

uint32_t bk_adc_is_using_calibration_val_flag(void)
{
    return (!!(s_saradc_use_calibration_val_flag));
}

float saradc_calculate(UINT16 adc_val)
{
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_data *data = &dev->data;
    float practical_voltage = 0;
    UINT16 cali_low_val = data->sadc_cali_val[SARADC_CALIBRATE_LOW];
    UINT16 cali_high_val = data->sadc_cali_val[SARADC_CALIBRATE_HIGH];

    #if defined(CONFIG_SARADC_V1P1)
    #if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX))
    UINT16 cali_300mV_val = data->sadc_cali_val[SARADC_CALIBRATE_EXT_LOW];
    if ((cali_300mV_val != 0) && (adc_val < cali_low_val)) {
        /* (adc_val - low) / (practical_voltage - 1Volt) = (low - ext_low) / 0.7Volt */
        /* practical_voltage = 0.7 * (adc_val - low) / (low - ext_low) + 1Volt */
        practical_voltage = 0.7 * (adc_val - cali_low_val);
        practical_voltage = (practical_voltage / (float)(cali_low_val - cali_300mV_val)) + 1;
    } else {
        /* (adc_val - low) / (practical_voltage - 1Volt) = (high - low) / 1Volt */
        /* practical_voltage = (adc_val - low) / (high - low) + 1Volt */
        practical_voltage = (float)(adc_val - cali_low_val);
        practical_voltage = (practical_voltage / (float)(cali_high_val - cali_low_val)) + 1;
    }
    #else
    practical_voltage = (adc_val -(cali_low_val - 4096));
    practical_voltage = practical_voltage/(cali_high_val  - (cali_low_val - 4096));
    practical_voltage = 2*practical_voltage;
    #endif
    #elif defined(CONFIG_SARADC_V1P2)
    #if (defined(CONFIG_SOC_BK7239XX)) //need adapt for 0.3v valtage
    UINT16 cali_300mV_val = data->sadc_cali_val[SARADC_CALIBRATE_EXT_LOW];
    if ((cali_300mV_val != 0) && (adc_val < cali_low_val)) {
        /* (adc_val - low) / (practical_voltage - 1Volt) = (low - ext_low) / 0.7Volt */
        /* practical_voltage = 0.7 * (adc_val - low) / (low - ext_low) + 1Volt */
        practical_voltage = 0.7 * (adc_val - cali_low_val);
        practical_voltage = (practical_voltage / (float)(cali_low_val - cali_300mV_val)) + 1;
    } else {
    /* (adc_val - low) / (practical_voltage - 1Volt) = (high - low) / 1Volt */
    /* practical_voltage = (adc_val - low) / (high - low) + 1Volt */
    practical_voltage = (float)(adc_val - cali_low_val);
    practical_voltage = (practical_voltage / (float)(cali_high_val - cali_low_val)) + 1;
    }
    #else
    practical_voltage = (float)(adc_val - cali_low_val);
    practical_voltage = (practical_voltage / (float)(cali_high_val - cali_low_val)) + 1;
    #endif
    #endif

    if (practical_voltage < 0) {
        practical_voltage = 0.0f;
    }

    return practical_voltage;
}

float temperature_calculate(UINT16 adc_val)
{
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_data *data = &dev->data;
    float practical_temperature;
    UINT16 cali_temp_val = data->sadc_cali_val[SARADC_CALIBRATE_TEMP_CODE25];
    UINT16 cali_temp_step = data->sadc_cali_val[SARADC_CALIBRATE_TEMP_STEP10];

    #if (defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7236N)) && (!defined(CONFIG_SOC_BK7259)) &&(!defined(CONFIG_SOC_BK7236Q))
    //46->46.5 for better practical
    (void)cali_temp_step;
    if (cali_temp_val >= adc_val)
    {
        practical_temperature = 25 + (cali_temp_val - adc_val) * 10 / 46.5;
    }
    else
    {
        practical_temperature = 25 - (adc_val - cali_temp_val) * 10 / 46.5;
    }
    if (practical_temperature > 90) {
        //temp > 90, thre=5.15
        practical_temperature = 90 + (INT16)((float)(practical_temperature - 90) * 46.5 / 51.5);
    }
    #else
    if (cali_temp_val >= adc_val)
    {
        practical_temperature = 25 + (cali_temp_val - adc_val) * 10 / cali_temp_step;
    }
    else
    {
        practical_temperature = 25 - (adc_val - cali_temp_val) * 10 / cali_temp_step;
    }
    #endif

    return practical_temperature;
}

float bk_adc_data_calculate(UINT16 adc_val, UINT8 adc_chan)
{
    float cali_value = 0;

    if (adc_chan == ADC_7) {
        return temperature_calculate(adc_val);
    }

    if (adc_chan == ADC_8 || adc_chan == ADC_9 || adc_chan == ADC_11) {
        ADC_LOGI("adc_chan %d has been used\r\n", adc_chan);
        return -1.0f;
    }

    cali_value = saradc_calculate(adc_val);
    if (adc_chan == ADC_0) {
        cali_value = cali_value * 4;
    }

    return cali_value;
}

UINT32 saradc_get_calibrate_val(uint16_t *value, SARADC_MODE mode)
{
    uint32_t irq_level;
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_context *ctx = &dev->ctx;
    struct sadc_data *data = &dev->data;

    if ((mode < SARADC_CALIBRATE_EXT_LOW) || (SARADC_CALIBRATE_MAX <= mode))
    {
        return SARADC_FAILURE;
    }

    adc_context_lock(ctx);
    irq_level = rtos_disable_int();

    *value = data->sadc_cali_val[mode];

    rtos_enable_int(irq_level);
    adc_context_release(ctx);

    return SARADC_SUCCESS;
}

UINT32 saradc_set_calibrate_val(uint16_t *value, SARADC_MODE mode)
{
    uint32_t irq_level;
    uint32_t threshold = 0;
    uint32_t dft_value = 0;
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_context *ctx = &dev->ctx;
    struct sadc_data *data = &dev->data;

    if ((mode < SARADC_CALIBRATE_EXT_LOW) || (SARADC_CALIBRATE_MAX <= mode))
    {
        return SARADC_FAILURE;
    }

    adc_context_lock(ctx);
    irq_level = rtos_disable_int();

    if (mode == SARADC_CALIBRATE_LOW) {
        threshold = bk_adc_get_1Volt_threshold();
        dft_value = bk_adc_get_1Volt_value();
        if ((*value + threshold < dft_value) || (dft_value + threshold < *value)) {
            dev->adc_cali_invalid = 1;
        }
        if (!dev->adc_cali_invalid) {
            data->sadc_cali_val[mode] = *value;
        }
    } else if (mode == SARADC_CALIBRATE_HIGH) {
        threshold = bk_adc_get_2Volt_threshold();
        dft_value = bk_adc_get_2Volt_value();
        if ((*value + threshold < dft_value) || (dft_value + threshold < *value)) {
            dev->adc_cali_invalid = 1;
        }
        if (!dev->adc_cali_invalid) {
            data->sadc_cali_val[mode] = *value;
        }
    } else if (mode == SARADC_CALIBRATE_TEMP_CODE25) {
        threshold = bk_adc_get_temp_code_dft_threshold();
        dft_value = bk_adc_get_temp_code_dft_25degree();
        if ((dft_value <= *value + threshold) && (*value <= dft_value + threshold)) {
            data->sadc_cali_val[mode] = *value;
        }
    } else {
        data->sadc_cali_val[mode] = *value;
    }

    rtos_enable_int(irq_level);
    adc_context_release(ctx);

    return SARADC_SUCCESS;
}

#if defined(CONFIG_ADC_STATIS)
void adc_statis_dump(void)
{
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_statistics *stats = &dev->stats;

    BK_LOGI(TAG, "dump adc statis:\r\n");
    BK_LOGI(TAG, "adc_isr_cnt:     %d\r\n", stats->adc_isr_cnt);
    BK_LOGI(TAG, "adc_rx_total_cnt:       %d\r\n", stats->adc_rx_total_cnt);
    BK_LOGI(TAG, "adc_rx_succ_cnt:       %d\r\n", stats->adc_rx_succ_cnt);
    BK_LOGI(TAG, "adc_rx_drop_cnt:       %d\r\n", stats->adc_rx_drop_cnt);
}
#endif

#if defined(CONFIG_SARADC_V1P2)
bk_err_t adc_calib_save_analog_context(struct sadc_calib_ana_context *ana_context)
{
    ana_context->ana_reg2_val = sys_ana_ll_get_reg2_value();
    ana_context->ana_reg4_val = sys_ana_ll_get_reg4_value();
    ana_context->ana_reg5_val = sys_ana_ll_get_reg5_value();
    ana_context->ana_reg10_val = sys_ana_ll_get_reg10_value();

    return BK_OK;
}

bk_err_t adc_calib_restore_analog_context(struct sadc_calib_ana_context *ana_context)
{
    sys_ana_ll_set_reg2_value(ana_context->ana_reg2_val);
    sys_ana_ll_set_reg4_value(ana_context->ana_reg4_val);
    sys_ana_ll_set_reg5_value(ana_context->ana_reg5_val);
    sys_ana_ll_set_reg10_value(ana_context->ana_reg10_val);

    return BK_OK;
}

bk_err_t bk_adc_enter_calib_mode(void)
{
    uint32_t otp_cwt[ADC_CWT_COEF_NUM] = {0};
    struct sadc_calib_ana_context ana_context;
    bk_err_t ret = BK_OK;
    int ana_context_saved = 0;
    int gadc_buf_enabled = 0;
    uint16_t *adc_raw_data_buf = NULL;

    if (adc_try_load_otp_cwt(otp_cwt)) {
        adc_calib_save_analog_context(&ana_context);
        ana_context_saved = 1;
        sys_drv_set_ana_pwd_gadc_buf(1);
        gadc_buf_enabled = 1;
        adc_hal_calib_init();
        adc_hal_set_cwt(otp_cwt);
        goto cleanup;
    }

    adc_config_t config = {0};
    adc_raw_data_buf = (uint16_t *)os_zalloc(ADC_CALIBRATION_DATA_NUM * sizeof(uint16_t));
    if (NULL == adc_raw_data_buf) {
        ADC_LOGE("adc_raw_data_buf malloc failed\r\n");
        ret = BK_ERR_NO_MEM;
        goto cleanup;
    }

    config.chan = 0;
    config.adc_mode = ADC_CONTINUOUS_MODE;
    config.src_clk = ADC_SCLK_XTAL;
    config.clk = 2;
    config.saturate_mode = 0;
    config.steady_ctrl= 0;
    config.adc_filter = 0;
    config.sample_rate = 0;

    os_memset(adc_raw_data_buf, 0, ADC_CALIBRATION_DATA_NUM * sizeof(uint16_t));

    adc_calib_save_analog_context(&ana_context);
    ana_context_saved = 1;
    sys_drv_set_ana_pwd_gadc_buf(1);
    gadc_buf_enabled = 1;

    ret = bk_adc_cont_start(&config, config.chan, adc_raw_data_buf, ADC_CALIBRATION_DATA_NUM);
    if (ret != BK_OK) {
        goto cleanup;
    }

    adc_hal_calib_init();
    ret = bk_adc_cont_get_raw(config.chan, adc_raw_data_buf, ADC_CALIBRATION_DATA_NUM);
    if (ret != BK_OK) {
        goto cleanup;
    }

    adc_hal_stop_commom(NULL);
    adc_hal_set_cwt_calib(adc_raw_data_buf, ADC_CALIBRATION_DATA_NUM);

cleanup:
    if (adc_raw_data_buf) {
        os_free(adc_raw_data_buf);
        adc_raw_data_buf = NULL;
    }
    if (gadc_buf_enabled) {
        sys_drv_set_ana_pwd_gadc_buf(0);
    }
    if (ana_context_saved) {
        adc_calib_restore_analog_context(&ana_context);
    }

    return ret;
}
#endif // CONFIG_SARADC_V1P2
// eof

