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

extern struct sadc_device *saradc_dev_ptr;

uint32_t bk_saradc_read_raw_data(uint32_t timeout);
bk_err_t adc_hw_activate(struct sadc_device *dev);
bk_err_t adc_activate_config(struct sadc_device *dev, adc_config_t *config);
bk_err_t adc_channel_config(struct sadc_device *dev, adc_chan_t adc_chan);

void saradc_hal_start_enable(void)
{
    struct sadc_device *dev = saradc_dev_ptr;
    adc_hal_t *hal_ptr = &dev->hal;

    adc_hal_start_commom(hal_ptr);
}

int saradc_hal_is_fifo_empty(adc_hal_t *hal_ptr)
{
    return (!adc_hal_is_fifo_empty(hal_ptr));
}

bk_err_t saradc_start_int_disable(void)
{
    struct sadc_device *dev = saradc_dev_ptr;
    adc_hal_t *hal_ptr = &dev->hal;

    if(adc_hal_check_adc_busy(hal_ptr))
        return BK_ERR_ADC_BUSY;
    if (adc_hal_check_adc_enable(hal_ptr))
        return BK_OK;

    sys_drv_sadc_int_disable();

    return BK_OK;
}

void bk_adc_read_for_ate(adc_chan_t chan_id, uint32_t saradc_num, uint16_t *saradc_buf)
{
    uint32_t i;
    int irq_level;
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_context *ctx = &dev->ctx;
    adc_hal_t *hal_ptr = &dev->hal;

    BK_LOG_ON_ERR(saradc_start_int_disable());

#if 0
    saradc_hal_start_enable();
#else
    bk_err_t ret = BK_OK;
    adc_config_t *channel_config_ptr;
    channel_config_ptr = dev->channel_cfg[chan_id];

    ret = (adc_channel_config(dev, chan_id));
    ret = (adc_activate_config(dev, channel_config_ptr));
    ret = (adc_hw_activate(dev));
    (void)ret;
#endif

    adc_context_lock(ctx);
    irq_level = rtos_disable_int();
    for(i = 0; i < saradc_num; i++)
    {
        if(saradc_hal_is_fifo_empty(hal_ptr))
        {
            saradc_buf[i] = bk_saradc_read_raw_data(1000);
        }
        else
        {
            i--;
            continue;
        }
    }
    rtos_enable_int(irq_level);
    adc_context_release(ctx);
}

void test_adc_for_ate(adc_chan_t channel, adc_mode_t mode,
                      uint32_t pre_div, uint32_t samp_rate,
                      uint32_t filter, uint32_t sta_ctrl,
                      uint32_t usCount, uint16_t *pDataBuffer)
{
    adc_config_t config = {0};
    uint32_t adc_clk;

    adc_clk = 26000000/2/(pre_div + 1);

    config.chan = channel;
    config.adc_mode = mode;
    config.clk = adc_clk;
    config.src_clk = 1;
    config.saturate_mode = 4;
    config.sample_rate = samp_rate;
    config.steady_ctrl= sta_ctrl;
    config.adc_filter = filter;

    BK_LOG_ON_ERR(bk_adc_channel_init(&config));

    bk_adc_read_for_ate(channel, usCount, pDataBuffer);
    BK_LOG_ON_ERR(bk_adc_channel_deinit(channel));
}

bk_err_t bk_adc_cont_mode_cont_read(struct sadc_device *dev, uint16_t *data, uint32_t count)
{
    adc_hal_t *hal_ptr = &dev->hal;
    uint32_t got_data_cnt = 0;

    if(NULL == data) {
        return BK_FAIL;
    }

    while(got_data_cnt < count) {
        if(!adc_hal_is_fifo_empty(hal_ptr)) {
            data[got_data_cnt ++] = adc_hal_get_adc_data(hal_ptr);
        }
    }

    return BK_OK;
}

bk_err_t bk_adc_cont_start(adc_config_t *config, uint8_t chan_id, uint16_t *data, uint32_t count)
{
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_context *ctx_ptr;
    adc_config_t *channel_config_ptr;
    bk_err_t ret = BK_OK;

    BK_LOG_ON_ERR(bk_adc_channel_init(config));

    channel_config_ptr = dev->channel_cfg[chan_id];
    if(NULL == channel_config_ptr) {
        ret = BK_ERR_ADC_CHAN_NOT_INIT;
        goto cont_rd_exit;
    }

    ctx_ptr = &dev->ctx;
    BK_LOG_ON_ERR(adc_context_lock(ctx_ptr));
    BK_LOG_ON_ERR(adc_channel_config(dev, chan_id));
    BK_LOG_ON_ERR(adc_activate_config(dev, channel_config_ptr));
    BK_LOG_ON_ERR(adc_hw_activate(dev));
    adc_hal_stop_commom(&dev->hal);

    BK_LOG_ON_ERR(adc_context_release(ctx_ptr));

cont_rd_exit:
    return ret;
}

bk_err_t bk_adc_cont_get_raw(uint8_t chan_id, uint16_t *data, uint32_t count)
{
    struct sadc_device *dev = saradc_dev_ptr;
    struct sadc_context *ctx_ptr;
    bk_err_t ret = BK_OK;
    int int_level;

    ctx_ptr = &dev->ctx;
    BK_LOG_ON_ERR(adc_context_lock(ctx_ptr));

    int_level = rtos_disable_int();
    adc_hal_start_commom(&dev->hal);
    BK_LOG_ON_ERR(bk_adc_cont_mode_cont_read(dev, data, count));
    rtos_enable_int(int_level);

    sys_drv_set_ana_pwd_gadc_buf(0);
    BK_LOG_ON_ERR(adc_context_release(ctx_ptr));

    return ret;
}

#if SARADC_AUTOTEST
bk_err_t bk_saradc_set_config(adc_config_t *config, uint32_t div)
{
    adc_hal_t *hal_ptr = &saradc_dev_ptr->hal;

    BK_RETURN_ON_NULL(config);
    ADC_RETURN_ON_INVALID_MODE(config->adc_mode);
    ADC_RETURN_ON_INVALID_SRC_CLK(config->src_clk);
    ADC_RETURN_ON_INVALID_CHAN(config->chan);

    adc_hal_set_div(hal_ptr, div);
    adc_hal_set_mode(hal_ptr, config->adc_mode);
    adc_hal_set_steady_ctrl(hal_ptr, config->steady_ctrl);
    adc_hal_set_saturate_mode(hal_ptr, config->saturate_mode);

    if(config->adc_mode == ADC_CONTINUOUS_MODE)
    {
        os_printf("config->sample_rate = %d\r\n",config->sample_rate);
        adc_hal_set_sample_rate(hal_ptr, config->sample_rate);
        adc_hal_set_adc_filter(hal_ptr, config->adc_filter);
    }

    return BK_OK;
}
#endif
// eof

