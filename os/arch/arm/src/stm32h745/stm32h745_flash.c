/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/stm32h745/stm32h745_flash.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>

#include <tinyara/init.h>
#include <tinyara/kmalloc.h>

#include <stm32h7xx_hal.h>
#include <string.h>

/****************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
#define STM32H745_FLASH_TOTAL_SIZE   (1024 * 1024 * 2)  /*Should be changed as a using*/
#define STM32H745_FLASH_BLOCK_SIZE   (32)           /*Should be changed as a using*/
#define STM32H745_FLASH_ERASE_SIZE   (128 * 1024)   /*Should be changed as a using*/
#define STM32H745_FLASH_SECTOR_NB    (16)           /*Total bank number*/
#define STM32H745_FLASH_SECTOR_NB_EACH_BANK (8)     /*each banks number*/

#define STM32H745_FLASH_BASE_ADDRESS  (0x08000000)   /*Should be changed as a using*/
#define STM32H745_FLASH_MAX_ADDRESS   (STM32H745_FLASH_BASE_ADDRESS + STM32H745_FLASH_TOTAL_SIZE)

#define STM32H745_FLASH_SECTOR_SIZE   (128 * 1024)

/************************************************************************************
 * Private Types
 ************************************************************************************/

/* This type represents the state of the MTD device.  The struct mtd_dev_s must
 * appear at the beginning of the definition so that you can freely cast between
 * pointers to struct mtd_dev_s.
 */
struct stm32h745_mtd_dev_s 
{
    struct mtd_dev_s mtd;       /* MTD interface */
    int nsectors;               /* number of erase sectors */
};

static struct stm32h745_mtd_dev_s g_dev_s;
/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* MTD driver methods */
static int stm32h745_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks);
static ssize_t stm32h745_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buf);
static ssize_t stm32h745_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buf);
static ssize_t stm32h745_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer);
static int stm32h745_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg);

#if defined(CONFIG_MTD_BYTE_WRITE)
static ssize_t stm32h745_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: stm32h745_erase
 * Description: 
 *   startblock BANK1 : 0 ~ 7
 *   startblock BANK2 : 8 ~ 15
 ************************************************************************************/\
static int stm32h745_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
    ssize_t result=OK;
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError = 0;
    int Banks;
    int Sector;

    if(nblocks < 1)
    {
        return ERROR;
    }

    if(startblock < STM32H745_FLASH_SECTOR_NB_EACH_BANK)
    {
        Banks  = FLASH_BANK_1;
        Sector = startblock;
    }
    else if(startblock < STM32H745_FLASH_SECTOR_NB)
    {
        Banks  = FLASH_BANK_2;
        Sector = startblock - STM32H745_FLASH_SECTOR_NB_EACH_BANK;
    }
    else
    {
        return ERROR;
    }

    __DSB();
    __ISB();

    __disable_irq();
//    stm32h745_irq_clear_pending_all();
    HAL_FLASH_Unlock();
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_2;
    EraseInitStruct.Banks         = Banks;
    EraseInitStruct.Sector        = Sector;
    EraseInitStruct.NbSectors     = nblocks;

    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        //Error_Handler();
        result = ERROR;
    }
    HAL_FLASH_Lock();
    __enable_irq();
    return result;
}

/************************************************************************************
 * Name: stm32h745_bread
 ************************************************************************************/
static ssize_t stm32h745_bread(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR uint8_t *buf)
{
    __DSB();
    __ISB();

    memcpy(buf,     /* dest buf */
           (uint8_t *)(STM32H745_FLASH_BASE_ADDRESS + (startblock * STM32H745_FLASH_BLOCK_SIZE)), /* src address*/
           (nblocks * STM32H745_FLASH_BLOCK_SIZE)); /* size */

    return nblocks;
}

/************************************************************************************
 * Name: stm32h745_bwrite
 ************************************************************************************/
static ssize_t stm32h745_bwrite(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks, FAR const uint8_t *buf)
{
    ssize_t result=OK;
    uint32_t address;
    __DSB();
    __ISB();

    //__disable_irq();
    HAL_FLASH_Unlock();

    address = STM32H745_FLASH_BASE_ADDRESS + (startblock * STM32H745_FLASH_BLOCK_SIZE);

    uint8_t *buf2 = (uint8_t *)buf;

    for(int i=0; i<nblocks; i++)
    {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, address, (uint32_t)buf2) != HAL_OK)
        {
            result = ERROR;
            break;
        }
        buf2 = buf2 + STM32H745_FLASH_BLOCK_SIZE;
        address = address + STM32H745_FLASH_BLOCK_SIZE;
    }
    HAL_FLASH_Lock();
    //__enable_irq();
    result = nblocks;
    return result;
}

/************************************************************************************
 * Name: stm32h745_read
 ************************************************************************************/
static ssize_t stm32h745_read(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
    __DSB();
    __ISB();
    memcpy(buffer,     /* dest buf */
           (uint8_t *)(STM32H745_FLASH_BASE_ADDRESS + offset), /* src address*/
           nbytes); /* size */

    return nbytes;
}


/************************************************************************************
 * Name: stm32h745_ioctl
 ************************************************************************************/
static int stm32h745_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
    int ret = ERROR;
    FAR struct stm32h745_mtd_dev_s *priv = (FAR struct stm32h745_mtd_dev_s *)dev;

    switch (cmd) 
    {
        case MTDIOC_GEOMETRY: 
        {
            FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)((uintptr_t) arg);
            if (geo) 
            {
                geo->blocksize = STM32H745_FLASH_BLOCK_SIZE;
                geo->erasesize = STM32H745_FLASH_ERASE_SIZE;
                geo->neraseblocks = priv->nsectors;
                ret = OK;
            }
        }
        break;

        case MTDIOC_BULKERASE: 
        {
            /* Erase the entire device */
            ret = stm32h745_erase(dev, 0, priv->nsectors);
        }
        break;

        case MTDIOC_XIPBASE:
        default:
            ret = ERROR;
            break;
    }

    return ret;
}

#if defined(CONFIG_MTD_BYTE_WRITE)
/************************************************************************************
 * Name: stm32h745_write
 ************************************************************************************/
static ssize_t stm32h745_write(FAR struct mtd_dev_s *dev, off_t offset, size_t nbytes, FAR uint8_t *buffer)
{
    /* Not supported */
    
    return ERROR;
}
#endif



/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: up_flashinitialize
 *
 * Description:
 *   Create an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/

FAR struct mtd_dev_s *up_flashinitialize(void)
{
    g_dev_s.nsectors   = STM32H745_FLASH_SECTOR_NB;
    g_dev_s.mtd.erase  = stm32h745_erase;
    g_dev_s.mtd.bread  = stm32h745_bread;
    g_dev_s.mtd.bwrite = stm32h745_bwrite;
    g_dev_s.mtd.read   = stm32h745_read;
#if defined(CONFIG_MTD_BYTE_WRITE)    
    g_dev_s.mtd.write  = stm32h745_write;
#endif    
    g_dev_s.mtd.ioctl  = stm32h745_ioctl;
    g_dev_s.mtd.name   = "stm32h745 flash";

    return (FAR struct mtd_dev_s *)&g_dev_s;
}

/************************************************************************************
 * Name: stm32h745_flash_ecc_handler
 ************************************************************************************/

void stm32h745_flash_ecc_handler(void)
{
    int ecc_sector;

    if(__HAL_FLASH_GET_FLAG_BANK1(FLASH_FLAG_DBECCERR_BANK1))
    {
        ecc_sector = (int)((int)(READ_REG(FLASH->ECC_FA1) * STM32H745_FLASH_BLOCK_SIZE) / STM32H745_FLASH_SECTOR_SIZE);
        
        __HAL_FLASH_CLEAR_FLAG_BANK1(FLASH_FLAG_SNECCERR_BANK1);
        __HAL_FLASH_CLEAR_FLAG_BANK1(FLASH_FLAG_DBECCERR_BANK1);
        
        stm32h745_erase(NULL, ecc_sector, 1);
        lldbg("ECC Error recovery handler\n");
        lldbg("BANK1 Sector erased:%d\n", ecc_sector);
    }

    if(__HAL_FLASH_GET_FLAG_BANK2(FLASH_FLAG_DBECCERR_BANK2))
    {
        ecc_sector = (int)((int)(READ_REG(FLASH->ECC_FA2) * STM32H745_FLASH_BLOCK_SIZE) / STM32H745_FLASH_SECTOR_SIZE);
        ecc_sector = ecc_sector + STM32H745_FLASH_SECTOR_NB_EACH_BANK;
        
        __HAL_FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_SNECCERR_BANK2);
        __HAL_FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_DBECCERR_BANK2);

        stm32h745_erase(NULL, ecc_sector, 1);
        lldbg("ECC Error recovery handler\n");
        lldbg("BANK2 Sector erased:%d\n", ecc_sector);
    }
}












