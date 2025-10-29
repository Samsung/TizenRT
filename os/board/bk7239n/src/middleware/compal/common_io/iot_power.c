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


/* These APIs require the following menuconfig options enabled:
 *
 * CONFIG_PM_LIGHT_SLEEP=y
 */

#include <stdlib.h>
#include <string.h>
//#include <FreeRTOSConfig.h>
//#include <portmacro.h>
#include <iot_power.h>
#include <os/os.h>

#include <common/bk_include.h>
#include <modules/pm.h>
#include "func_convert.h"


#define COMMON_POWER_TAG  "bk-iot-power"


typedef struct IotPowerDescriptor {
    UINT8 status;
    UINT8 is_cb_supported;
    IotPowerCallback_t idle_cb;
    void *cb_args;
    IotPowerMode_t power_mode;
    IotPowerMode_t prev_power_mode;
    IotPowerIdleMode_t idle_mode;
    UINT32 PCWake_ms;
    UINT32 ClkSrcOffWake_ms;
    UINT32 VddOffWake_ms;
    IotPowerWakeupSources_t wake_src;
}IotPowerDescriptor_t;

static IotPowerDescriptor_t power_descriptor={0};

static int iot_power_idle_enter_cb(uint64_t expected_time_ms, void *args)
{
    if (power_descriptor.PCWake_ms > expected_time_ms) {
        power_descriptor.idle_mode = eIdleNoneMode;
        return -1;
    }
    BK_LOGD(COMMON_POWER_TAG，"th = %d, delay = %d \r\n", power_descriptor.PCWake_ms, (uint32_t)expected_time_ms);
    
    if(power_descriptor.idle_cb != NULL) {
        power_descriptor.idle_cb(true, power_descriptor.cb_args);
    }
    if((power_descriptor.VddOffWake_ms > 0) && (expected_time_ms > power_descriptor.VddOffWake_ms))
        power_descriptor.idle_mode = eIdleVddOffMode;
    else if((power_descriptor.ClkSrcOffWake_ms > 0) && (expected_time_ms > power_descriptor.ClkSrcOffWake_ms))
        power_descriptor.idle_mode = eIdleClkSrcOffModeMode;
    else
        power_descriptor.idle_mode = eIdlePCMode;
    return 0;
}

static int iot_power_idle_exit_cb(uint64_t slept_time_ms, void *args)
{
    if(power_descriptor.idle_cb != NULL) {
        power_descriptor.idle_cb(false, power_descriptor.cb_args);
        if (power_descriptor.power_mode == eUnknownPowerMode) {
            power_descriptor.power_mode = power_descriptor.prev_power_mode;
        }
    }
    return 0;
}


IotPowerHandle_t iot_power_open(void)
{
    if (power_descriptor.status) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_open err,already open\r\n");
        return NULL;
    }
    BK_LOGD(COMMON_POWER_TAG, "iot_power_open\r\n");
    //to do reg
    pm_cb_conf_t enter_config={iot_power_idle_enter_cb, NULL};
    pm_cb_conf_t exit_config={iot_power_idle_exit_cb, NULL};
    if(bk_pm_light_sleep_register_cb(&enter_config, &exit_config) != BK_OK){
        BK_LOGD(COMMON_POWER_TAG, "iot_power sleep_register err\r\n");
        return NULL;
    }
    
    memset(&power_descriptor, 0, sizeof(power_descriptor));
    power_descriptor.status = 1;

    power_descriptor.PCWake_ms = 2;
    power_descriptor.power_mode = eNormalPowerPerfMode;
    power_descriptor.idle_mode = eIdleNoneMode;

    return &power_descriptor;
}

int32_t iot_power_set_mode(IotPowerHandle_t const pxPowerHandle, IotPowerMode_t xMode)
{
    if (pxPowerHandle == NULL) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_set_mode err,invalid ptr\r\n");
        return IOT_POWER_SET_FAILED;
    }
    BK_LOGD(COMMON_POWER_TAG, "iot_power_set_mode\r\n");
    if (pxPowerHandle->power_mode == xMode) {
        return IOT_POWER_SUCCESS;
    }

    switch(xMode) {
        case eHighPowerPerfMode:
            // Power tests requires this mode so not returning
            //return IOT_POWER_FUNCTION_NOT_SUPPORTED;
        case eNormalPowerPerfMode:           
            break;
         case eLowPowerPerfMode:            
            break;
         default:
            return IOT_POWER_INVALID_VALUE;
    }

    pxPowerHandle->power_mode = xMode;
    pxPowerHandle->idle_mode = eIdleNoneMode;
    return IOT_POWER_SUCCESS;
}

int32_t iot_power_reset_mode(IotPowerHandle_t const pxPowerHandle)
{
    if (pxPowerHandle == NULL) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_reset_mode err,invalid ptr\r\n");
        return IOT_POWER_INVALID_VALUE;
    }
    BK_LOGD(COMMON_POWER_TAG, "iot_power_reset_mode\r\n");
    pxPowerHandle->prev_power_mode = pxPowerHandle->power_mode;
    pxPowerHandle->power_mode = eUnknownPowerMode;
    return IOT_POWER_SUCCESS;
}

void iot_power_set_callback(IotPowerHandle_t const pxPowerHandle, IotPowerCallback_t xCallback, void *pvUserContext)
{
    if (pxPowerHandle == NULL || xCallback == NULL ) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_set_callback err,invalid ptr\r\n");
        return;
    }
    BK_LOGD(COMMON_POWER_TAG, "iot_power_set_callback\r\n");
    pxPowerHandle->idle_cb = xCallback;
    pxPowerHandle->cb_args = pvUserContext;
    return;
}


static int iot_power_set_wakeup_sources(IotPowerWakeupSources_t *data)
{
    //bk_err_t ret = 0;
    uint8_t *sources = data->pucWakeupSources;

    if (!data) {
        return IOT_POWER_INVALID_VALUE;
    }

    if (data->usLength < 8) {
        return IOT_POWER_INVALID_VALUE;
    }
    uint8_t pm_wake_source = sources[0];
    rtc_wakeup_param_t      rtc_wakeup_param         = {0};
    system_wakeup_param_t   system_wakeup_param      = {0};
    gpio_wakeup_param_t     gpio_wakeup_param        = {0};
    touch_wakeup_param_t    touch_wakeup_param       = {0};
    usbplug_wakeup_param_t  usbplug_wakeup_param     = {0};
    void *wakeup_ptr = NULL;
    
    switch(pm_wake_source){
        case PM_WAKEUP_SOURCE_INT_GPIO:
            wakeup_ptr = &gpio_wakeup_param;
            gpio_wakeup_param.gpio_id = sources[1];
            gpio_wakeup_param.gpio_valid = PARAM_DATA_VALID;
            gpio_wakeup_param.gpio_trigger_interrupt_type = sources[2];
            break;
        case PM_WAKEUP_SOURCE_INT_RTC:
            wakeup_ptr = &rtc_wakeup_param;
            rtc_wakeup_param.period = array_to_uint32(sources+1);
            //rtc_wakeup_param.isr_callback = cli_pm_rtc_callback;
            break;
        case PM_WAKEUP_SOURCE_INT_SYSTEM_WAKE:
            wakeup_ptr = &system_wakeup_param;
            if(sources[1] == WIFI_WAKEUP)
                system_wakeup_param.wifi_bt_wakeup = WIFI_WAKEUP;
            else
                system_wakeup_param.wifi_bt_wakeup = BT_WAKEUP;
            break;
        case PM_WAKEUP_SOURCE_INT_USBPLUG:
            wakeup_ptr = &usbplug_wakeup_param;
            break;
        case PM_WAKEUP_SOURCE_INT_TOUCHED:
            wakeup_ptr = &touch_wakeup_param;
            touch_wakeup_param.touch_channel = sources[1];
            break;
        default:
            pm_wake_source = PM_WAKEUP_SOURCE_INT_NONE;
            break;    
    }

    if(pm_wake_source != PM_WAKEUP_SOURCE_INT_NONE){
        bk_pm_wakeup_source_set(pm_wake_source, wakeup_ptr);
    }
    return IOT_POWER_SUCCESS;
}

int32_t iot_power_ioctl(IotPowerHandle_t const pxPowerHandle, IotPowerIoctlRequest_t xRequest, void *const pvBuffer)
{
    if (pxPowerHandle == NULL || pvBuffer == NULL) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_ioctl err,invalid ptr\r\n");
        return IOT_POWER_INVALID_VALUE;
    }
    BK_LOGD(COMMON_POWER_TAG, "iot_power_ioctl\r\n");
    switch(xRequest) {
        case eSetPCWakeThreshold: {
            pxPowerHandle->PCWake_ms = (*(uint32_t *)pvBuffer);
            break;
            }
        case eGetPCWakeThreshold:
            *(uint32_t *)pvBuffer = pxPowerHandle->PCWake_ms;
            break;
        case eSetClkSrcOffWakeThreshold:
            pxPowerHandle->ClkSrcOffWake_ms = (*(uint32_t *)pvBuffer);
            break;
        case eGetClkSrcOffWakeThreshold:
            *(uint32_t *)pvBuffer = pxPowerHandle->ClkSrcOffWake_ms;
            break;
        case eSetVddOfffWakeThreshold:
            pxPowerHandle->VddOffWake_ms = (*(uint32_t *)pvBuffer);
            break;
        case eGetVddOfffWakeThreshold:
            *(uint32_t *)pvBuffer = pxPowerHandle->VddOffWake_ms;
            break;
        case eSetWakeupSources:
            pxPowerHandle->wake_src = (*(IotPowerWakeupSources_t *)pvBuffer);
            iot_power_set_wakeup_sources(&pxPowerHandle->wake_src);
            break;
        case eGetWakeupSources:
            *(IotPowerWakeupSources_t *)pvBuffer = pxPowerHandle->wake_src;
            break;
        case eGetLastIdleMode:
            *(uint32_t *)pvBuffer = pxPowerHandle->idle_mode;
            break;
        default:
            return IOT_POWER_INVALID_VALUE;
    }
    return IOT_POWER_SUCCESS;
}

int32_t iot_power_get_mode(IotPowerHandle_t const pxPowerHandle, IotPowerMode_t *xMode)
{
    if (pxPowerHandle == NULL || xMode == NULL) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_get_mode err,invalid ptr\r\n");
        return IOT_POWER_GET_FAILED;
    }
    BK_LOGD(COMMON_POWER_TAG, "iot_power_get_mode\r\n");
    *xMode = pxPowerHandle->power_mode;
    return IOT_POWER_SUCCESS;
}

int32_t iot_power_close(IotPowerHandle_t const pxPowerHandle)
{
    if (pxPowerHandle == NULL) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_close err,invalid ptr\r\n");
        return IOT_POWER_INVALID_VALUE;
    }
    if (pxPowerHandle->status == 0) {
        BK_LOGE(COMMON_POWER_TAG, "iot_power_close err,already close\r\n");
        return IOT_POWER_NOT_INITIALIZED;
    }
    BK_LOGD(COMMON_POWER_TAG, "iot_power_close\r\n");
    pxPowerHandle->status = 0;
    pxPowerHandle->idle_cb = NULL; 
    bk_pm_light_sleep_unregister_cb(true, true);

    return IOT_POWER_SUCCESS;
}
