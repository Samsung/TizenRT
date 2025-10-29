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
#include "driver/flash.h"
#include "flash_driver.h"
#include "iot_flash.h"
#include "os/mem.h"

#define SPI_FLASH_BLOCK_SIZE  0x10000   //64KB

static IotFlashInfo_t s_flash_info;
static bool s_flash_drv_open;
static uint32_t s_flash_write_size;
static uint32_t s_flash_read_size;
extern int hal_flash_lock();
extern int hal_flash_unlock();

static inline bool prv_is_alligned_4k( uint32_t dw_address)
{
    return (dw_address & FLASH_SECTOR_SIZE_MASK) == 0;
}

IotFlashHandle_t iot_flash_open(int32_t lFlashInstance)
{
    if ((lFlashInstance == -1) || (s_flash_drv_open == true)) {
        return NULL;
    }

    // Since architecture is based on common driver, flash has already initialized
    if((bk_flash_is_driver_inited() == true)) {
        s_flash_drv_open = true;
    }
    else
    {
#if defined(CONFIG_FLASH)
        bk_flash_driver_init();
        extern int hal_flash_init();
        hal_flash_init();
        s_flash_drv_open = true;
#endif
    }

    return (IotFlashHandle_t) 1;
}

IotFlashInfo_t *iot_flash_getinfo(IotFlashHandle_t const pxFlashHandle)
{
    if (pxFlashHandle == NULL) {
        return NULL;
    }

    s_flash_info.ulFlashSize = bk_flash_get_current_total_size();
    s_flash_info.ulBlockSize = SPI_FLASH_BLOCK_SIZE;
    s_flash_info.ulSectorSize = FLASH_SECTOR_SIZE;
    s_flash_info.ulPageSize = FLASH_PAGE_SIZE;
    s_flash_info.ulLockSupportSize = FLASH_SECTOR_SIZE;
    s_flash_info.ucAsyncSupported = false;

    return &s_flash_info;
}

void iot_flash_set_callback(IotFlashHandle_t const pxFlashHandle, IotFlashCallback_t xCallback, void *pvUserContext)
{
    // Not supported for BK7235
    return;
}

int32_t iot_flash_ioctl(IotFlashHandle_t const pxFlashHandle, IotFlashIoctlRequest_t xRequest, void *const pvBuffer)
{
    int32_t dw_rtn = IOT_FLASH_INVALID_VALUE;

    if (pxFlashHandle == NULL || (int) xRequest < 0) {
        return IOT_FLASH_INVALID_VALUE;
    }

    switch (xRequest){

        case eGetFlashStatus:
            if(pvBuffer == NULL){
                return IOT_FLASH_INVALID_VALUE;
            }

            hal_flash_lock();
            uint16_t status_value = bk_flash_read_status_reg();
            hal_flash_unlock();

            os_memcpy(pvBuffer,&status_value,sizeof(status_value));
            dw_rtn = IOT_FLASH_SUCCESS;
        break;
        case eGetFlashTxNoOfbytes:
            if(pvBuffer == NULL){
                return IOT_FLASH_INVALID_VALUE;
            }

            os_memcpy(pvBuffer,&s_flash_write_size,sizeof(s_flash_write_size));
            dw_rtn = IOT_FLASH_SUCCESS;
        break;
        case eGetFlashRxNoOfbytes:
            if(pvBuffer == NULL){
                return IOT_FLASH_INVALID_VALUE;
            }

            os_memcpy(pvBuffer,&s_flash_read_size,sizeof(s_flash_read_size));
            dw_rtn = IOT_FLASH_SUCCESS;
        break;
        case eSetFlashBlockProtection:
        case eGetFlashBlockProtection:
        case eSuspendFlashProgramErase:
        case eResumeFlashProgramErase:
        default:
            dw_rtn = IOT_FLASH_FUNCTION_NOT_SUPPORTED;
        break;
    }

    return dw_rtn;
}

int32_t iot_flash_erase_sectors(IotFlashHandle_t const pxFlashHandle, uint32_t ulAddress, size_t lSize)
{
    uint32_t erase_times = 0;
    uint32_t dw_loop = 0;
    uint32_t dw_rtn = 0;
    flash_protect_type_t project_type;

    if ((pxFlashHandle == NULL) || !(prv_is_alligned_4k(ulAddress)) || (lSize % FLASH_SECTOR_SIZE)) {
        return IOT_FLASH_INVALID_VALUE;
    }

    erase_times = lSize / FLASH_SECTOR_SIZE;
    hal_flash_lock();
    bk_flash_write_enable();

    project_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    for(dw_loop = 0; dw_loop < erase_times; dw_loop++){
        dw_rtn = bk_flash_erase_sector(ulAddress + dw_loop * FLASH_SECTOR_SIZE);

        if(dw_rtn != BK_OK){
            bk_flash_set_protect_type(project_type);
            bk_flash_write_disable();
            hal_flash_unlock();
            return IOT_FLASH_INVALID_VALUE;
        }
    }
    bk_flash_set_protect_type(project_type);

    bk_flash_write_disable();
    hal_flash_unlock();
    return IOT_FLASH_SUCCESS;
}

int32_t iot_flash_erase_chip(IotFlashHandle_t const pxFlashHandle)
{
    if (pxFlashHandle == NULL) {
        return IOT_FLASH_INVALID_VALUE;
    }

    return IOT_FLASH_FUNCTION_NOT_SUPPORTED;
}

int32_t iot_flash_write_sync(IotFlashHandle_t const pxFlashHandle, uint32_t ulAddress, uint8_t *const pvBuffer, size_t xBytes)
{
    flash_protect_type_t project_type;
    if (pxFlashHandle == NULL || (int) ulAddress < 0 || pvBuffer == NULL) {
        return IOT_FLASH_INVALID_VALUE;
    }

    s_flash_write_size = xBytes;
    hal_flash_lock();
    bk_flash_write_enable();

    project_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_err_t dw_rtn = bk_flash_write_bytes(ulAddress, (const uint8_t *)pvBuffer, xBytes);
    bk_flash_set_protect_type(project_type);

    bk_flash_write_disable();
    hal_flash_unlock();

    return (dw_rtn == BK_OK) ? IOT_FLASH_SUCCESS : IOT_FLASH_INVALID_VALUE;
}

int32_t iot_flash_read_sync(IotFlashHandle_t const pxFlashHandle, uint32_t ulAddress, uint8_t *const pvBuffer, size_t xBytes)
{
    if (pxFlashHandle == NULL || (int) ulAddress < 0 || pvBuffer == NULL) {
        return IOT_FLASH_INVALID_VALUE;
    }

    s_flash_read_size = xBytes;
    hal_flash_lock();
    bk_err_t dw_rtn = bk_flash_read_bytes(ulAddress, (uint8_t *)pvBuffer, xBytes);
    hal_flash_unlock();

    return (dw_rtn == BK_OK) ? IOT_FLASH_SUCCESS : IOT_FLASH_INVALID_VALUE;
}

int32_t iot_flash_write_async(IotFlashHandle_t const pxFlashHandle, uint32_t ulAddress, uint8_t *const pvBuffer, size_t xBytes)
{
    return IOT_FLASH_FUNCTION_NOT_SUPPORTED;
}

int32_t iot_flash_read_async(IotFlashHandle_t const pxFlashHandle, uint32_t ulAddress, uint8_t *const pvBuffer, size_t xBytes)
{
    return IOT_FLASH_FUNCTION_NOT_SUPPORTED;
}


int32_t iot_flash_close(IotFlashHandle_t const pxFlashHandle)
{
    if (s_flash_drv_open != true) {
        return IOT_FLASH_INVALID_VALUE;
    }
    s_flash_drv_open = false;
    return IOT_FLASH_SUCCESS;
}