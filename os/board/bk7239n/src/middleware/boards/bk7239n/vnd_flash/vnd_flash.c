/**
 *********************************************************************************
 * @file vnd_flash_new.c
 * @brief This file provides all the headers of Flash operation functions..
 *********************************************************************************
 *
 * The MIT License
 * Copyright (c) 2017 BEKEN Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
 *********************************************************************************
*/
#include <common/bk_include.h>
#include <os/os.h>
#include <driver/flash_partition.h>
#include <common/bk_kernel_err.h>
#include <os/mem.h>
#include <vnd_flash_partition.h>
#include "partitions_gen_ns.h"

/* Logic partition on flash devices */
const bk_logic_partition_t bk_flash_partitions[BK_PARTITION_MAX_USER] = {
    [BK_PARTITION_BOOTLOADER_USER] = 
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description = "bootloader",
        .partition_start_addr = 0x0,
        .partition_length = 0x11000,
        .partition_options = PAR_OPT_EXECUTE_EN | PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
#if defined(CONFIG_PRIMARY_ALL_PHY_PARTITION_OFFSET)
    [BK_PARTITION_APPLICATION_USER] = 
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description = "application",
        .partition_start_addr = CONFIG_PRIMARY_ALL_PHY_PARTITION_OFFSET,
        .partition_length = CONFIG_PRIMARY_ALL_PHY_PARTITION_SIZE,
        .partition_options = PAR_OPT_EXECUTE_EN | PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
#endif
#if defined(CONFIG_OTA_PHY_PARTITION_OFFSET)
    [BK_PARTITION_OTA_USER] = 
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description = "ota",
        .partition_start_addr = CONFIG_OTA_PHY_PARTITION_OFFSET,
        .partition_length = CONFIG_OTA_PHY_PARTITION_SIZE,
        .partition_options = PAR_OPT_EXECUTE_DIS | PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
#endif
    [BK_PARTITION_USR_CONFIG] = 
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description = "usr_config",
        .partition_start_addr = CONFIG_USER_CONFIG_PHY_PARTITION_OFFSET,
        .partition_length = CONFIG_USER_CONFIG_PHY_PARTITION_SIZE,
        .partition_options = PAR_OPT_EXECUTE_DIS | PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
#if defined(CONFIG_EASYFLASH_PHY_PARTITION_OFFSET)
    [BK_PARTITION_EASYFLASH] = 
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description = "easyflash",
        .partition_start_addr = CONFIG_EASYFLASH_PHY_PARTITION_OFFSET,
        .partition_length = CONFIG_EASYFLASH_PHY_PARTITION_SIZE,
        .partition_options = PAR_OPT_EXECUTE_DIS | PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
#endif
#if defined(CONFIG_SYS_RF_PHY_PARTITION_OFFSET)
    [BK_PARTITION_RF_FIRMWARE] = 
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description = "rf_firmware",
        .partition_start_addr = CONFIG_SYS_RF_PHY_PARTITION_OFFSET,
        .partition_length = CONFIG_SYS_RF_PHY_PARTITION_SIZE,
        .partition_options = PAR_OPT_EXECUTE_DIS | PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
#endif
#if defined(CONFIG_SYS_NET_PHY_PARTITION_OFFSET)
    [BK_PARTITION_NET_PARAM] = 
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description = "net_param",
        .partition_start_addr = CONFIG_SYS_NET_PHY_PARTITION_OFFSET,
        .partition_length = CONFIG_SYS_NET_PHY_PARTITION_SIZE,
        .partition_options = PAR_OPT_EXECUTE_DIS | PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
#endif
};
