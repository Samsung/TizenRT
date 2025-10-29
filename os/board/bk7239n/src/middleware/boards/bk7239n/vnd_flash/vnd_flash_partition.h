/**
 *********************************************************************************
 * @file vnd_flash_partition.h
 * @brief This file provides all the headers of Flash operation functions..
 *********************************************************************************
 *
 *Copyright 2020-2025 Beken
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 *********************************************************************************
*/
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_OVERRIDE_FLASH_PARTITION)

#include <vendor_flash_partition.h>

#else //#if CONFIG_OVERRIDE_FLASH_PARTITION

#define BK_PARTITION_BOOTLOADER_TEMP 0
#define BK_PARTITION_APPLICATION_TEMP 1
#define BK_PARTITION_OTA_TEMP 2
#define BK_PARTITION_APPLICATION1_TEMP 3
#define BK_PARTITION_MATTER_FLASH_TEMP 4
#define BK_PARTITION_RF_FIRMWARE_TEMP 5
#define BK_PARTITION_NET_PARAM_TEMP 6
#define BK_PARTITION_USR_CONFIG_TEMP 7
#define BK_PARTITION_OTA_FINA_EXECUTIVE_TEMP 8
#define BK_PARTITION_APPLICATION2_TEMP 9
#define BK_PARTITION_EASYFLASH_TEMP 10
#define BK_PARTITION_MAX_TEMP 11

#ifdef BK_PARTITION_BOOTLOADER_TEMP
#define BK_PARTITION_BOOTLOADER_USER BK_PARTITION_BOOTLOADER_TEMP
#endif
#ifdef BK_PARTITION_APPLICATION_TEMP
#define BK_PARTITION_APPLICATION_USER BK_PARTITION_APPLICATION_TEMP
#endif
#ifdef BK_PARTITION_OTA_TEMP
#define BK_PARTITION_OTA_USER BK_PARTITION_OTA_TEMP
#endif
#ifdef BK_PARTITION_USR_CONFIG_TEMP
#define BK_PARTITION_USR_CONFIG_USER BK_PARTITION_USR_CONFIG_TEMP
#endif
#ifdef BK_PARTITION_MATTER_FACTORY_TEMP
#define BK_PARTITION_MATTER_FACTORY_USER BK_PARTITION_MATTER_FACTORY_TEMP
#endif
#ifdef BK_PARTITION_EASYFLASH_TEMP
#define BK_PARTITION_EASYFLASH_USER BK_PARTITION_EASYFLASH_TEMP
#endif
#ifdef BK_PARTITION_MATTER_TEMP
#define BK_PARTITION_MATTER_USER BK_PARTITION_MATTER_TEMP
#endif
#ifdef BK_PARTITION_RF_FIRMWARE_TEMP
#define BK_PARTITION_RF_FIRMWARE_USER BK_PARTITION_RF_FIRMWARE_TEMP
#endif
#ifdef BK_PARTITION_NET_PARAM_TEMP
#define BK_PARTITION_NET_PARAM_USER BK_PARTITION_NET_PARAM_TEMP
#endif
#ifdef BK_PARTITION_LITTLSFS_TEMP
#define BK_PARTITION_LITTLSFS_USER BK_PARTITION_LITTLSFS_TEMP
#endif

typedef enum
{
    BK_PARTITION_START_USER = BK_PARTITION_MAX_TEMP - 1,
#ifndef BK_PARTITION_BOOTLOADER_USER
    BK_PARTITION_BOOTLOADER_USER,
#endif
#ifndef BK_PARTITION_APPLICATION_USER
    BK_PARTITION_APPLICATION_USER,
#endif
#ifndef BK_PARTITION_OTA_USER
    BK_PARTITION_OTA_USER,
#endif
#ifndef BK_PARTITION_USR_CONFIG_USER
    BK_PARTITION_USR_CONFIG_USER,
#endif
#ifndef BK_PARTITION_MATTER_FACTORY_USER
    BK_PARTITION_MATTER_FACTORY_USER,
#endif
#ifndef BK_PARTITION_EASYFLASH_USER
    BK_PARTITION_EASYFLASH_USER,
#endif
#ifndef BK_PARTITION_MATTER_USER
    BK_PARTITION_MATTER_USER,
#endif
#ifndef BK_PARTITION_RF_FIRMWARE_USER
    BK_PARTITION_RF_FIRMWARE_USER,
#endif
#ifndef BK_PARTITION_NET_PARAM_USER
    BK_PARTITION_NET_PARAM_USER,
#endif
#ifndef BK_PARTITION_LITTLSFS_USER
    BK_PARTITION_LITTLSFS_USER,
#endif
    BK_PARTITION_MAX_USER,
}bk_partition_user_t;

#endif // #if CONFIG_OVERRIDE_FLASH_PARTITION

#ifdef __cplusplus
}
#endif
