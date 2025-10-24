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
#include <iot_efuse.h>


#include <common/bk_include.h>
#include <driver/efuse.h>
#include <os/mem.h>
#include <os/os.h>
#include <components/log.h>


#define COMMON_EFUSE_TAG "common_io_efuse"



typedef struct IotEfuseDescriptor {
    UINT32 status;
} IotEfuseDescriptor_t;

static IotEfuseDescriptor_t efuse_desc={0};


IotEfuseHandle_t iot_efuse_open( void )
{
    BK_LOGD(COMMON_EFUSE_TAG, "efuse_open\r\n");
    bk_efuse_driver_init();
    if(efuse_desc.status){
        BK_LOGE(COMMON_EFUSE_TAG, "efuse_already_open\r\n");
        return NULL;
    }
    else{
        efuse_desc.status = 1;
        return &efuse_desc;
    }
}

int32_t iot_efuse_read_32bit_word( IotEfuseHandle_t const pxEfuseHandle,
                                   uint32_t ulIndex,
                                   uint32_t * ulValue )
{
    uint32_t idx=0, temp=0;
    if (pxEfuseHandle == NULL || ulIndex > 7 || ulValue == NULL){
        BK_LOGE(COMMON_EFUSE_TAG, "Invalid arguments\r\n");
        return IOT_EFUSE_INVALID_VALUE;
    }
    BK_LOGD(COMMON_EFUSE_TAG, "efuse_read32\r\n");
    *ulValue = 0;
    for(idx=0; idx<4; idx++){
        if(bk_efuse_read_byte((ulIndex * 4 + idx), (uint8 *)&temp) == BK_OK){
            *ulValue |= (temp << (idx * 8));
        }
        else
            return IOT_EFUSE_READ_FAIL;
    }
    return IOT_EFUSE_SUCCESS;
}

int32_t iot_efuse_write_32bit_word( IotEfuseHandle_t const pxEfuseHandle,
                                    uint32_t ulIndex,
                                    uint32_t ulValue )
{
    uint32_t idx=0;
    if (pxEfuseHandle == NULL || ulIndex > 7){
        BK_LOGE(COMMON_EFUSE_TAG, "Invalid arguments\r\n");
        return IOT_EFUSE_INVALID_VALUE;
    }
    BK_LOGD(COMMON_EFUSE_TAG, "efuse_write32\r\n");
    for(idx=0; idx<4; idx++){
        if(bk_efuse_write_byte((ulIndex * 4 + idx), ulValue) == BK_OK){
            ulValue = ulValue>>8;
        }
        else
            return IOT_EFUSE_WRITE_FAIL;
    }
    return IOT_EFUSE_SUCCESS;
}

int32_t iot_efuse_read_16bit_word( IotEfuseHandle_t const pxEfuseHandle,
                                   uint32_t ulIndex,
                                   uint16_t * ulValue )
{
    uint32_t idx=0, temp=0;
    if (pxEfuseHandle == NULL || ulIndex > 15 || ulValue == NULL){
        BK_LOGE(COMMON_EFUSE_TAG, "Invalid arguments\r\n");
        return IOT_EFUSE_INVALID_VALUE;
    }
    BK_LOGD(COMMON_EFUSE_TAG, "efuse_read16\r\n");
    *ulValue = 0;
    for(idx=0; idx<2; idx++){
        if(bk_efuse_read_byte((ulIndex * 2 + idx), (uint8 *)&temp) == BK_OK){
            *ulValue |= (temp << (idx * 8));
        }
        else
            return IOT_EFUSE_READ_FAIL;
    }
    return IOT_EFUSE_SUCCESS;
}

int32_t iot_efuse_write_16bit_word( IotEfuseHandle_t const pxEfuseHandle,
                                    uint32_t ulIndex,
                                    uint16_t value )
{
    uint32_t idx=0;
    if (pxEfuseHandle == NULL || ulIndex > 15){
        BK_LOGE(COMMON_EFUSE_TAG, "Invalid arguments\r\n");
        return IOT_EFUSE_INVALID_VALUE;
    }
    BK_LOGD(COMMON_EFUSE_TAG, "efuse_write16\r\n");
    for(idx=0; idx<2; idx++){
        if(bk_efuse_write_byte((ulIndex * 2 + idx), value) == BK_OK){
            value = value>>8;
        }
        else
            return IOT_EFUSE_WRITE_FAIL;
    }
    return IOT_EFUSE_SUCCESS;
}

int32_t iot_efuse_close( IotEfuseHandle_t const pxEfuseHandle )
{
    if (pxEfuseHandle == NULL){
        BK_LOGE(COMMON_EFUSE_TAG, "Invalid arguments\r\n");
        return IOT_EFUSE_INVALID_VALUE;
    }
    BK_LOGD(COMMON_EFUSE_TAG, "efuse_close\r\n");
    if(pxEfuseHandle->status){
        pxEfuseHandle->status = 0;
        return IOT_EFUSE_SUCCESS;
    }
    else
        return IOT_EFUSE_INVALID_VALUE;
}

