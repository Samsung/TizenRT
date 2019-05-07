/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
/************************************************************************************
 * os/arch/arm/src/imxrt/imxrt_flash.h
 *
 *   Copyright (C) 2011 Uros Platise. All rights reserved.
 *   Author: Uros Platise <uros.platise@isotel.eu>
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_IMXRT_IMXRT_FLASH_H
#define __ARCH_ARM_SRC_IMXRT_IMXRT_FLASH_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/progmem.h>

#include "chip.h"
#include "imxrt_config.h"

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "chip/imxrt102x_config.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "chip/imxrt105x_config.h"
#else
#error Unrecognized i.MX RT architecture
#endif

#ifndef __ASSEMBLY__

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#define IMXRT_FLEXSPI FLEXSPI
#define IMXRT_FLASH_SIZE 0x2000 /* 64Mb/KByte */
#define IMXRT_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define IMXRT_FLASH_PAGE_SIZE 256
#define IMXRT_SECTOR 0
#define IMXRT_FLASH_BASE  0x60000000
#define IMXRT_SECTOR_SIZE 0x1000 /* 4K */
#define IMXRT_BLOCK_SIZE 0x8000
#define IMXRT_FLASH_TOTAL_SIZE (IMXRT_FLASH_SIZE * 1024)
#define IMXRT_FLEXSPI_CLOCK kCLOCK_FlexSpi

#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL 7
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST 13
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD 0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS 1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE 2
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR 3
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD 4
#define NOR_CMD_LUT_SEQ_IDX_READID 8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG 9
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI 10
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI 11
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG 12
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP 5

#define CUSTOM_LUT_LENGTH 60
#define FLASH_BUSY_STATUS_POL 1
#define FLASH_BUSY_STATUS_OFFSET 0
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#define IMXRT_FLEXSPI FLEXSPI
#define IMXRT_FLASH_SIZE 0x10000
#define IMXRT_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define IMXRT_FLASH_PAGE_SIZE 512
#define IMXRT_SECTOR_SIZE 0x40000
#define IMXRT_BLOCK_SIZE 0x40000
#define IMXRT_FLASH_BASE 0x60000000
#define IMXRT_FLEXSPI_CLOCK kCLOCK_FlexSpi
#define IMXRT_FLASH_TOTAL_SIZE (IMXRT_FLASH_SIZE * 1024)
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA 0
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS 2
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE 4
#define HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR 6
#define HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM 10
#define CUSTOM_LUT_LENGTH 48
#else
#error Unrecognized i.MX RT architecture
#endif

/************************************************************************************
 * Public Data
 ************************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Name: imxrt_flexspi_nor_hyperbus_read
 *
 * Description:
 *   Check if bus is busy and wait until exiting busy
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_hyperbus_read(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes);

/************************************************************************************
 * Name: imxrt_flexspi_nor_hyperbus_write
 *
 * Description:
 *   Check if bus is busy and wait until exiting busy
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_hyperbus_write(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes);

#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
/************************************************************************************
 * Name: flexspi_nor_enable_quad_mode
 *
 * Description:
 *   Enable Quad mode in flexspi
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);

/************************************************************************************
 *
 * Description:
 *   Get vendor id in nor flash
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);

#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
/************************************************************************************
 * Name: imxrt_flexspi_nor_hyperflash_cfi
 *
 * Description:
 *   Check the configuration in hyperflash
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_hyperflash_cfi(FLEXSPI_Type *base);
#endif

/************************************************************************************
 * Name: imxrt_flexspi_nor_write_enable
 *
 * Description:
 *   Write data to hyper flash using flexspi interface
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr);

/************************************************************************************
 * Name: imxrt_flexspi_nor_wait_bus_busy
 *
 * Description:
 *   Check if bus is busy and wait until exiting busy
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_wait_bus_busy(FLEXSPI_Type *base);

/************************************************************************************
 * Name: imxrt_flexspi_nor_flash_erase_sector
 *
 * Description:
 *   Erase data to hyper flash using flexspi interface
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);

/************************************************************************************
 * Name: imxrt_flexspi_nor_flash_page_program
 *
 * Description:
 *   Write data to hyper flash using flexspi interface
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src);

/************************************************************************************
 * Name: imxrt_flash_init
 *
 * Description:
 *   Initialize flash 
 *
 ************************************************************************************/
void imxrt_flash_init(void);

FAR struct mtd_dev_s *imxrt_mtd_initialize(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */

#endif							/* __ARCH_ARM_SRC_IMXRT_IMXRT_FLASH_H */
