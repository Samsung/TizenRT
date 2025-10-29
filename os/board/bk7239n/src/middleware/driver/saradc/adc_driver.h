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

#pragma once

#include <components/log.h>
#include <os/os.h>

#include <driver/adc.h>
#include "bk_saradc.h"
#include "adc_hal.h"

/* sadc maybe does not be used, when enable smp feature;
 * configure the macro based on system requirement.
 */
#if defined(CONFIG_FREERTOS_SMP) && (CONFIG_FREERTOS_SMP == 1)
#define CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE  CONFIG_FREERTOS_SMP
#endif

#if defined(CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE)
#include "spinlock.h"
#endif // CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE

#define ADC_TAG "adc"
#define ADC_LOGI(...) BK_LOGI(ADC_TAG, ##__VA_ARGS__)
#define ADC_LOGW(...) BK_LOGW(ADC_TAG, ##__VA_ARGS__)
#define ADC_LOGE(...) BK_LOGE(ADC_TAG, ##__VA_ARGS__)
#define ADC_LOGD(...) BK_LOGD(ADC_TAG, ##__VA_ARGS__)

#define DEFAULT_ADC_MODE             ADC_CONTINUOUS_MODE
#define DEFAULT_ADC_SAMPLE_RATE      0x20
#define DEFAULT_ADC_SCLK             ADC_SCLK_XTAL
#define DEFAULT_ADC_CLK              2600000
#define DEFAULT_ADC_STEADY_TIME           7
#define DEFAULT_ADC_SAMPLE_UNIT_BYTES     2
#define DEFAULT_AVERAGE_SAMPLE_SIZE       32
#define DEFAULT_SATURATE_MODE             ADC_SATURATE_MODE_3
#define ADC_SAMPLE_THRESHOLD_DEFAULT      32

struct sadc_context {
    beken_semaphore_t sync;
    beken_mutex_t lock;

    #if defined(CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE)
    volatile spinlock_t multicore_lock;
    #endif // CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE
};

struct sadc_data {
    uint16_t *buffer;
    uint16_t sampling_request;
    uint16_t sampling_index;

    uint16_t sadc_cali_val[SARADC_CALIBRATE_MAX];
};

struct sadc_calib_ana_context {
    uint32_t ana_reg2_val;
    uint32_t ana_reg4_val;
    uint32_t ana_reg5_val;
    uint32_t ana_reg10_val;
};

struct sadc_statistics {
    uint32_t adc_isr_cnt;
    uint32_t adc_rx_total_cnt;
    uint32_t adc_rx_succ_cnt;
    uint32_t adc_rx_drop_cnt;
};

struct sadc_device {
    adc_hal_t hal;
    adc_config_t *channel_cfg[ADC_MAX];
    #if (defined(CONFIG_SARADC_REVISION))
    uint16_t rf_active_patch;
    uint16_t rf_active_nums;
    adc_chan_t adc_chan;
    #endif

    uint8_t adc_cali_invalid;

    struct sadc_context ctx;
    struct sadc_data data;

    adc_isr_cb isr_cb;
    void *cb_arg;

    /* data callback, and the first parameter is buffer
     * it is designated for compal module;
     * typedef void ( * IotAdcCallback_t )( uint16_t * pusConvertedData,
                                            void * pvUserContext );
     */
    FUNC_2PV_PTR data_callback;
    void *cb_ctx;

    #if defined(CONFIG_ADC_STATIS)
    struct sadc_statistics stats;
    #endif
};

static inline bk_err_t adc_context_init(struct sadc_context *ctx)
{
    bk_err_t ret;
    ret = rtos_init_mutex(&ctx->lock);
    if (kNoErr != ret) {
        return BK_ERR_ADC_INIT_MUTEX;
    }

    ret = rtos_init_semaphore(&ctx->sync, 1);
    if (BK_OK != ret) {
        return BK_ERR_ADC_INIT_READ_SEMA;
    }

    #if defined(CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE)
    ctx->multicore_lock.owner = SPIN_LOCK_FREE;
    ctx->multicore_lock.count = 0;
    #endif // CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE

    return ret;
}

static inline bk_err_t adc_context_lock(struct sadc_context *ctx)
{
    #if defined(CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE)
    spin_lock(&ctx->multicore_lock);
    #endif // CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE

    return rtos_lock_mutex(&ctx->lock);
}

static inline bk_err_t adc_context_release(struct sadc_context *ctx)
{
    #if defined(CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE)
    spin_unlock(&ctx->multicore_lock);
    #endif // CONFIG_ENABLE_USING_SADC_IN_MULTI_CORE

    return rtos_unlock_mutex(&ctx->lock);
}

static inline bk_err_t adc_context_deinit(struct sadc_context *ctx)
{
    bk_err_t ret;

    ret = rtos_deinit_mutex(&ctx->lock);
    if (kNoErr != ret) {
        return BK_ERR_ADC_DEINIT_MUTEX;
    }
    ctx->lock = NULL;

    ret = rtos_deinit_semaphore(&ctx->sync);
    if (BK_OK != ret) {
        return BK_ERR_ADC_DEINIT_READ_SEMA;
    }
    ctx->sync = NULL;

    return ret;
}

bk_err_t bk_adc_channel_raw_read(adc_chan_t channel_id, uint16_t* buf, uint32_t sample_cnt, uint32_t timeout);
bk_err_t bk_adc_channel_deinit(adc_chan_t chan);
bk_err_t bk_adc_channel_init(adc_config_t *config);
bk_err_t bk_adc_get_config(uint32 adc_ch, adc_config_t **config);
bk_err_t bk_adc_is_valid_ch(uint32_t ch);
bk_err_t bk_adc_register_isr_callback_iot_callback(void *iot_callback, void *p_iot_context);
bk_err_t bk_adc_unregister_isr_iot_callback(void);

#if defined(CONFIG_SARADC_V1P2)
bk_err_t bk_adc_enter_calib_mode(void);
bk_err_t bk_adc_cont_get_raw(uint8_t chan_id, uint16_t *data, uint32_t count);
bk_err_t bk_adc_cont_start(adc_config_t *config, uint8_t chan_id, uint16_t *data, uint32_t count);
#endif // CONFIG_SARADC_V1P2
//eof

