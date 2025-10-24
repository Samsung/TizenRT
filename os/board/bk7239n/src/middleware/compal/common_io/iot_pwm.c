// Copyright 2020-2021 Beken
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


#include <stdint.h>
#include <string.h>
#include <iot_pwm.h>
#include <iot_board_gpio.h>
#include "iot_test_common_io_config.h"

#include <common/bk_include.h>
#include <driver/pwm.h>
#include <os/mem.h>
#include <os/os.h>
#include <components/log.h>


#define COMMON_PWM_TAG "common_io_pwm"
#define PWM_CLK_FREQ   (26000000UL)


typedef struct IotPwmDescriptor {
    UINT32 id;
    UINT8 set_flag;
    IotPwmConfig_t config;
} IotPwmDescriptor_t;
static volatile uint8_t pwm_bit_mask=0;


static UINT8 bk_conver_config_to_internal(const IotPwmConfig_t *config, void *internal, bool init)
{
    UINT32 cycle,duty;

    if( (config->ulPwmFrequency == 0) || (config->ucPwmDutyCycle == 0) || (config->ucPwmDutyCycle >= 100) )
        return 0;
    
    cycle = PWM_CLK_FREQ /config->ulPwmFrequency;
    duty = cycle * config->ucPwmDutyCycle / 100;
    if(init)
    {
        ((pwm_init_config_t *)internal)->period_cycle = cycle;
        ((pwm_init_config_t *)internal)->duty_cycle = duty;
    }
    else
    {
        ((pwm_period_duty_config_t *)internal)->period_cycle = cycle;
        ((pwm_period_duty_config_t *)internal)->duty_cycle = duty;
    }
    return 1;
}



IotPwmHandle_t iot_pwm_open( int32_t lPwmInstance )
{
    IotPwmDescriptor_t *pwm_ctx;
    pwm_init_config_t internal;
    if ( (lPwmInstance < 0) || ( lPwmInstance >= PWM_ID_MAX )  ) 
    {
        BK_LOGE(COMMON_PWM_TAG, "Invalid arguments\r\n");
        return NULL;
    }
    BK_LOGD(COMMON_PWM_TAG, "pwm_open\r\n");
    bk_pwm_driver_init();
    if(pwm_bit_mask & BIT(lPwmInstance))
        return NULL;
    
    pwm_ctx = (IotPwmDescriptor_t *) os_zalloc(sizeof(IotPwmDescriptor_t));
    if(pwm_ctx != NULL)
    {
        pwm_ctx->id = lPwmInstance;
        pwm_ctx->config.ulPwmFrequency = 2000;
        pwm_ctx->config.ucPwmDutyCycle = 20;
        pwm_ctx->config.ucPwmChannel = 0;
        memset(&internal, 0, sizeof(pwm_init_config_t));
        bk_conver_config_to_internal(&(pwm_ctx->config), &internal, 1);
        internal.psc = 25;
        if(bk_pwm_init(lPwmInstance, &internal) != BK_OK)
        {
            os_free(pwm_ctx);
            BK_LOGE(COMMON_PWM_TAG, "!!!bk_pwm_init error\r\n");
            return NULL;
        }
        bk_pwm_set_init_signal_low(lPwmInstance);
        pwm_bit_mask |= BIT(lPwmInstance);
    }
    else
    {
        BK_LOGE(COMMON_PWM_TAG,"!!!pwm zalloc fail \r\n");
    }
    return pwm_ctx;
}

int32_t iot_pwm_set_config( IotPwmHandle_t const pxPwmHandle, const IotPwmConfig_t xConfig )
{
    if (pxPwmHandle == NULL) 
    {
        BK_LOGE(COMMON_PWM_TAG, "Invalid arguments\r\n");
        return IOT_PWM_INVALID_VALUE;
    }
    BK_LOGD(COMMON_PWM_TAG, "pwm_set\r\n");
    pwm_period_duty_config_t internal;
    memset(&internal, 0 , sizeof(pwm_period_duty_config_t));
    if(bk_conver_config_to_internal(&xConfig, &internal, 0))
    {
        if(bk_pwm_set_period_duty(pxPwmHandle->id, &internal) != BK_OK)
        {
            BK_LOGE(COMMON_PWM_TAG, "!!!set_period_duty error\r\n");
            return IOT_PWM_INVALID_VALUE;
        }
        pxPwmHandle->config = xConfig;
        pxPwmHandle->set_flag = 1;
        return IOT_PWM_SUCCESS;
    }
    else
    {
        return IOT_PWM_INVALID_VALUE;
    }
}

IotPwmConfig_t * iot_pwm_get_config( IotPwmHandle_t const pxPwmHandle )
{
    if (pxPwmHandle == NULL) 
    {
        BK_LOGE(COMMON_PWM_TAG, "Invalid arguments\r\n");
        return NULL;
    }
    BK_LOGD(COMMON_PWM_TAG, "pwm_get\r\n");
    if(pxPwmHandle->set_flag)
        return &(pxPwmHandle->config);
    else
        return NULL;
}

int32_t iot_pwm_start( IotPwmHandle_t const pxPwmHandle )
{
    if (pxPwmHandle == NULL) 
    {
        BK_LOGE(COMMON_PWM_TAG, "Invalid arguments\r\n");
        return IOT_PWM_INVALID_VALUE;
    }
    BK_LOGD(COMMON_PWM_TAG, "pwm_start\r\n");
    if(pxPwmHandle->set_flag)
    {
        bk_pwm_start(pxPwmHandle->id);
        return IOT_PWM_SUCCESS;
    }
    else
        return IOT_PWM_NOT_CONFIGURED;    
}

int32_t iot_pwm_stop( IotPwmHandle_t const pxPwmHandle )
{
    if (pxPwmHandle == NULL) 
    {
        BK_LOGE(COMMON_PWM_TAG, "Invalid arguments\r\n");
        return IOT_PWM_INVALID_VALUE;
    }
    BK_LOGD(COMMON_PWM_TAG, "pwm_stop\r\n");
    bk_pwm_stop(pxPwmHandle->id);
    return IOT_PWM_SUCCESS;
}

int32_t iot_pwm_close( IotPwmHandle_t const pxPwmHandle )
{
    if (pxPwmHandle == NULL) 
    {
        BK_LOGE(COMMON_PWM_TAG, "Invalid arguments\r\n");
        return IOT_PWM_INVALID_VALUE;
    }
    BK_LOGD(COMMON_PWM_TAG, "pwm_close\r\n");
    if(pwm_bit_mask & BIT(pxPwmHandle->id))
    {
        bk_pwm_deinit(pxPwmHandle->id);
        os_free(pxPwmHandle);
        pwm_bit_mask &= (~ BIT(pxPwmHandle->id));
        return IOT_PWM_SUCCESS;
    }
    else
        return IOT_PWM_INVALID_VALUE;
}

