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
#include "common/bk_include.h"
#include "common/bk_err.h"
#include "temp_detect_pub.h"
#include "iot_tsensor.h"
#include "os/mem.h"
#include <stdbool.h>

#define INIT_TEMPERATUE_VLAUE   25//the raw temperature 

static bool s_temp_active_flag;
static bool s_temp_en_disable_flag;

IotTsensorHandle_t iot_tsensor_open(int32_t lTsensorInstance)
{
    uint32_t dw_rtn = 0;

    if((lTsensorInstance > 0) || (s_temp_active_flag == true)){
        return NULL;
    }
    else if (temp_detect_is_init() == true){
        s_temp_active_flag = true;
        return (IotTsensorHandle_t) 1;
    }
    else{
        dw_rtn = temp_detect_init(INIT_TEMPERATUE_VLAUE);

        if(dw_rtn != BK_OK){
            return NULL;
        }
        else{
            s_temp_active_flag = true;
            return (IotTsensorHandle_t) 1;
        }
    }
}

void iot_tsensor_set_callback(IotTsensorHandle_t const xTsensorHandle,
                              IotTsensorCallback_t xCallback,
                              void * pvUserContext)
{
    //Not support call back.
}

int32_t iot_tsensor_enable(IotTsensorHandle_t const xTsensorHandle)
{
    if((xTsensorHandle == NULL) || (s_temp_active_flag == false)){
        return IOT_TSENSOR_INVALID_VALUE;
    }

    s_temp_en_disable_flag = true;

    return IOT_TSENSOR_SUCCESS;
}

int32_t iot_tsensor_disable(IotTsensorHandle_t const xTsensorHandle)
{
     if((xTsensorHandle == NULL) || (s_temp_active_flag == false)){
        return IOT_TSENSOR_INVALID_VALUE;
    }
    s_temp_en_disable_flag = false;
    return IOT_TSENSOR_SUCCESS;
}

int32_t iot_tsensor_get_temp(IotTsensorHandle_t const xTsensorHandle,
                             int32_t * lTemp)
{
    int32_t temperature = 0;

    if((xTsensorHandle == NULL) || (lTemp == NULL) || (s_temp_active_flag == false)){
        return IOT_TSENSOR_INVALID_VALUE;
    }

    if(s_temp_en_disable_flag == false){
        return IOT_TSENSOR_DISABLED;
    }

    if(BK_OK == temp_detect_get_temperature((uint32_t *)&temperature)){
        os_memcpy(lTemp,&temperature,sizeof(int32_t));
        return IOT_TSENSOR_SUCCESS;
    }
    else{
        return IOT_TSENSOR_GET_TEMP_FAILED;
    }
}

int32_t iot_tsensor_ioctl(IotTsensorHandle_t const xTsensorHandle,
                          IotTsensorIoctlRequest_t xRequest,
                          void * const pvBuffer)
{
    uint32_t temperature = 0;

    if((xTsensorHandle == NULL) || 
        ((xRequest != eTsensorPerformCalibration) && (pvBuffer == NULL))){
        return IOT_TSENSOR_INVALID_VALUE;
    }

    if(s_temp_active_flag == false){
        return IOT_TSENSOR_CLOSED;
    }

    switch (xRequest)
    {
        case eTsensorGetMinThreshold:
            temperature = ADC_TEMP_VAL_MIN;
            os_memcpy( pvBuffer, &temperature, sizeof(temperature));
            break;
        case eTsensorGetMaxThreshold:
            temperature = ADC_TEMP_VAL_MAX;
            os_memcpy( pvBuffer, &temperature, sizeof(temperature));
            break;
        case eTsensorSetMinThreshold:
        case eTsensorSetMaxThreshold:
        case eTsensorPerformCalibration:
            return IOT_TSENSOR_NOT_SUPPORTED;
            break;
        default:
            return IOT_TSENSOR_INVALID_VALUE;
    }

    return IOT_TSENSOR_SUCCESS;
}

int32_t iot_tsensor_close( IotTsensorHandle_t const xTsensorHandle)
{
    if((xTsensorHandle == NULL) || (s_temp_active_flag == false)){
        return IOT_TSENSOR_INVALID_VALUE;
    }
    iot_tsensor_disable(xTsensorHandle);//before close we need disable the sesor.
    s_temp_active_flag = false;

    return IOT_TSENSOR_SUCCESS;
}