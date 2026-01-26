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
#include "adc_driver.h"
#include "common/bk_err.h"

#if defined(CONFIG_SARADC_V1P2_COMPATIBLE_MODE)
static adc_chan_t s_current_channel_id = 0;
#endif

/* These functions will be deprecated; obsolete interfaces*/
bk_err_t bk_adc_init(adc_chan_t adc_chan)
{
    return BK_OK;
}

bk_err_t bk_adc_deinit(adc_chan_t chan)
{
    #if defined(CONFIG_SARADC_V1P2_COMPATIBLE_MODE)
    return bk_adc_channel_deinit(chan);
    #else
    return BK_OK;
    #endif
}

bk_err_t bk_adc_acquire(void)
{
    return BK_OK;
}

bk_err_t bk_adc_release(void)
{
    return BK_OK;
}

bk_err_t bk_adc_stop(void)
{
    return BK_OK;
}

bk_err_t bk_adc_start(void)
{
    return BK_OK;
}

bk_err_t bk_adc_enable_bypass_clalibration(void)
{
    return BK_OK;
}

bk_err_t bk_adc_disable_bypass_clalibration(void)
{
    return BK_OK;
}

bk_err_t bk_adc_set_config(adc_config_t *config)
{
    #if defined(CONFIG_SARADC_V1P2_COMPATIBLE_MODE)
    s_current_channel_id = config->chan;
    return bk_adc_channel_init(config);
    #else
    return BK_OK;
    #endif
}

bk_err_t bk_adc_set_channel(adc_chan_t adc_chan)
{
    #if defined(CONFIG_SARADC_V1P2_COMPATIBLE_MODE)
    s_current_channel_id = adc_chan;
    return BK_OK;
    #else
    return BK_OK;
    #endif
}

bk_err_t bk_adc_read_raw(uint16_t* buf, uint32_t size, uint32_t timeout)
{
    #if defined(CONFIG_SARADC_V1P2_COMPATIBLE_MODE)
    return bk_adc_channel_raw_read(s_current_channel_id, buf, size, timeout);
    #else
    return BK_OK;
    #endif
}

bk_err_t bk_adc_read(uint16_t* data, uint32_t timeout)
{
    #if defined(CONFIG_SARADC_V1P2_COMPATIBLE_MODE)
    return bk_adc_channel_read(s_current_channel_id, data, timeout);
    #else
    return BK_OK;
    #endif
}
// eof

