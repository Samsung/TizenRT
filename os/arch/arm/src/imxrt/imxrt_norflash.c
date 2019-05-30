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
 * os/arch/arm/src/imxrt/imxrt_norflash.c
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

/* Provides standard flash access functions, to be used by the  flash mtd driver.
 * The interface is defined in the include/tinyara/progmem.h
 *
 * Requirements during write/erase operations on FLASH:
 *  - HSI must be ON.
 *  - Low Power Modes are not permitted during write/erase
 */

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <stdio.h>
#include <errno.h>

#include "imxrt_flexspi.h"
#include "imxrt_flash.h"
#include "imxrt_clock.h"

#include "up_arch.h"
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/spi/spi.h>

#include "imxrt_clock.h"
#include "imxrt_iomuxc.h"
#include "chip/imxrt_memorymap.h"
#include "imxrt_log.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
#define IMXRT_FLEXSPI FLEXSPI
#define IMXRT_FLASH_SIZE 0x2000 /* 64Mb/KByte */
#define IMXRT_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define IMXRT_FLASH_PAGE_SIZE 256
#define IMXRT_SECTOR 0
#define IMXRT_FLASH_BASE  0x60000000
#define IMXRT_SECTOR_SIZE 0x1000 /* 4K */
#define IMXRT_BLOCK_SIZE 0x8000
#define IMXRT_FLASH_TOTAL_SIZE (IMXRT_FLASH_SIZE*1024)
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


/************************************************************************************
 * Private Data
 ************************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
flexspi_device_config_t deviceconfig = {
	.flexspiRootClk = 133000000,
	.flashSize = IMXRT_FLASH_SIZE,
	.CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
	.CSInterval = 2,
	.CSHoldTime = 3,
	.CSSetupTime = 3,
	.dataValidTime = 0,
	.columnspace = 0,
	.enableWordAddress = 0,
	.AWRSeqIndex = 0,
	.AWRSeqNumber = 0,
	.ARDSeqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
	.ARDSeqNumber = 1,
	.AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
	.AHBWriteWaitInterval = 0,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
	/* Normal read mode -SDR */
	/* Normal read mode -SDR */
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Fast read mode - SDR */
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST + 1] = FLEXSPI_LUT_SEQ(
	    kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

	/* Fast read quad mode - SDR */
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 1] = FLEXSPI_LUT_SEQ(
	    kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x06, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),

	/* Read extend parameters */
	[4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x81, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

	/* Write Enable */
	[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Erase Sector  */
	[4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xD7, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

	/* Page Program - single mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Page Program - quad mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x32, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Read ID */
	[4 * NOR_CMD_LUT_SEQ_IDX_READID] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xAB, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READID + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Enable Quad mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),

	/* Enter QPI mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x35, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Exit QPI mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_EXITQPI] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0xF5, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Read status register */
	[4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

	/* Erase whole chip */
	[4 * NOR_CMD_LUT_SEQ_IDX_ERASECHIP] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC7, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
};
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
flexspi_device_config_t deviceconfig = {
	.flexspiRootClk = 120000000,
	.flashSize = IMXRT_FLASH_SIZE,
	.CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
	.CSInterval = 2,
	.CSHoldTime = 3,
	.CSSetupTime = 3,
	.dataValidTime = 0,
	.columnspace = 0,
	.enableWordAddress = 0,
	.AWRSeqIndex = 0,
	.AWRSeqNumber = 0,
	.ARDSeqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
	.ARDSeqNumber = 1,
	.AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
	.AHBWriteWaitInterval = 0,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
	/* Normal read mode -SDR */
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Fast read mode - SDR */
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST + 1] = FLEXSPI_LUT_SEQ(
	    kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

	/* Fast read quad mode - SDR */
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 1] = FLEXSPI_LUT_SEQ(
	    kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x06, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),

	/* Read extend parameters */
	[4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x81, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

	/* Write Enable */
	[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Erase Sector  */
	[4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xD7, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

	/* Page Program - single mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Page Program - quad mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x32, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Read ID */
	[4 * NOR_CMD_LUT_SEQ_IDX_READID] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xAB, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),
	[4 * NOR_CMD_LUT_SEQ_IDX_READID + 1] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Enable Quad mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),

	/* Enter QPI mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x35, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Exit QPI mode */
	[4 * NOR_CMD_LUT_SEQ_IDX_EXITQPI] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0xF5, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

	/* Read status register */
	[4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

	/* Erase whole chip */
	[4 * NOR_CMD_LUT_SEQ_IDX_ERASECHIP] =
	    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC7, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
};
#endif

/************************************************************************************
 * Private Functions
 ************************************************************************************/
/************************************************************************************
 * Name: flexspi_nor_enable_quad_mode
 *
 * Description:
 *   Enable Quad mode in flexspi
 *
 ************************************************************************************/
/************************************************************************************
 * Name: imxrt_flexspi_nor_write_enable
 *
 * Description:
 *   Write data to hyper flash using flexspi interface
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
{
	flexspi_transfer_t flashXfer;
	status_t status;

	/* Write enable */
	flashXfer.deviceAddress = baseAddr;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Command;
	flashXfer.SeqNumber = 2;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	return status;
}

/************************************************************************************
 * Name: imxrt_flexspi_nor_wait_bus_busy
 *
 * Description:
 *   Check if bus is busy and wait until exiting busy
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_wait_bus_busy(FLEXSPI_Type *base)
{
	/* Wait status ready. */
	bool isBusy;
	uint32_t readValue;
	status_t status;
	flexspi_transfer_t flashXfer;

	flashXfer.deviceAddress = 0;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Read;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUSREG;
	flashXfer.data = &readValue;
	flashXfer.dataSize = 1;

	do {
		status = imxrt_flexspi_transferblocking(base, &flashXfer);

		if (status != kStatus_Success) {
			return status;
		}
		if (FLASH_BUSY_STATUS_POL) {
			if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET)) {
				isBusy = true;
			} else {
				isBusy = false;
			}
		} else {
			if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET)) {
				isBusy = false;
			} else {
				isBusy = true;
			}
		}

	} while (isBusy);

	return status;
}

status_t imxrt_flexspi_nor_enable_quad_mode(FLEXSPI_Type *base)
{
	flexspi_transfer_t flashXfer;
	status_t status;
	uint32_t writeValue = 0x40;

	/* Write enable */
	status = imxrt_flexspi_nor_write_enable(base, 0);

	if (status != kStatus_Success) {
		return status;
	}

	/* Enable quad mode. */
	flashXfer.deviceAddress = 0;
	flashXfer.port = kFLEXSPI_PortB1;
	flashXfer.cmdType = kFLEXSPI_Write;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG;
	flashXfer.data = &writeValue;
	flashXfer.dataSize = 1;

	status = imxrt_flexspi_transferblocking(base, &flashXfer);
	if (status != kStatus_Success) {
		return status;
	}

	status = imxrt_flexspi_nor_wait_bus_busy(base);

	return status;
}

/************************************************************************************
 *
 * Description:
 *   Get vendor id in nor flash
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId)
{
	uint32_t temp;
	flexspi_transfer_t flashXfer;
	flashXfer.deviceAddress = 0;
	flashXfer.port = kFLEXSPI_PortB1;
	flashXfer.cmdType = kFLEXSPI_Read;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READID;
	flashXfer.data = &temp;
	flashXfer.dataSize = 1;

	status_t status = imxrt_flexspi_transferblocking(base, &flashXfer);

	*vendorId = temp;

	return status;
}

/************************************************************************************
 * Name: flexspi_pins_init
 *
 * Description:
 *   Initialize pin setting in flexspi
 *
 ************************************************************************************/
static inline void flexspi_pins_init(void)
{
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
	imxrt_clock_enableclock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS,     /* GPIO_SD_B1_05 is configured as FLEXSPI_A_DQS */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_05 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03,  /* GPIO_SD_B1_06 is configured as FLEXSPI_A_DATA03 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_06 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK,    /* GPIO_SD_B1_07 is configured as FLEXSPI_A_SCLK */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_07 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00,  /* GPIO_SD_B1_08 is configured as FLEXSPI_A_DATA00 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_08 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02,  /* GPIO_SD_B1_09 is configured as FLEXSPI_A_DATA02 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_09 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01,  /* GPIO_SD_B1_10 is configured as FLEXSPI_A_DATA01 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_10 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B,   /* GPIO_SD_B1_11 is configured as FLEXSPI_A_SS0_B */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_11 */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS,     /* GPIO_SD_B1_05 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03,  /* GPIO_SD_B1_06 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK,    /* GPIO_SD_B1_07 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00,  /* GPIO_SD_B1_08 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02,  /* GPIO_SD_B1_09 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01,  /* GPIO_SD_B1_10 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B,   /* GPIO_SD_B1_11 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
	imxrt_clock_enableclock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03,   /* GPIO_SD_B1_00 is configured as FLEXSPIB_DATA03 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_00 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02,   /* GPIO_SD_B1_01 is configured as FLEXSPIB_DATA02 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_01 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01,   /* GPIO_SD_B1_02 is configured as FLEXSPIB_DATA01 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_02 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00,   /* GPIO_SD_B1_03 is configured as FLEXSPIB_DATA00 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_03 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK,     /* GPIO_SD_B1_04 is configured as FLEXSPIB_SCLK */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_04 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 is configured as FLEXSPIA_DQS */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_05 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 is configured as FLEXSPIA_SS0_B */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_06 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 is configured as FLEXSPIA_SCLK */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_07 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 is configured as FLEXSPIA_DATA00 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_08 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 is configured as FLEXSPIA_DATA01 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_09 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 is configured as FLEXSPIA_DATA02 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_10 */
	imxrt_iomuxc_setpinmux(
		IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 is configured as FLEXSPIA_DATA03 */
		1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_11 */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03,   /* GPIO_SD_B1_00 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02,   /* GPIO_SD_B1_01 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01,   /* GPIO_SD_B1_02 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00,   /* GPIO_SD_B1_03 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK,     /* GPIO_SD_B1_04 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 PAD functional properties : */
		0x0130F1u);                             /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Pull
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Enabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * MCR0 Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
							 * Drive Strength Field: R0/6
							 * Speed Field: max(200MHz)
							 * Open Drain Enable Field: Open Drain Disabled
							 * Pull / Keep Enable Field: Pull/Keeper Enabled
							 * Pull / Keep Select Field: Keeper
							 * Pull Up / Down Config. Field: 100K Ohm Pull Down
							 * Hyst. Enable Field: Hysteresis Disabled */
	#endif
}

/************************************************************************************
 * Name: flexspi_clock_init
 *
 * Description:
 *   Initialize clock setting in flexspi
 *
 ************************************************************************************/
static inline void flexspi_clock_init(void)
{
	const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

	imxrt_clock_initusb1pll(&g_ccmConfigUsbPll);
	imxrt_clock_initusb1pfd(kCLOCK_Pfd0, 24);   /* Set PLL3 PFD0 clock 360MHZ. */
	imxrt_clock_setmux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
	imxrt_clock_setdiv(kCLOCK_FlexspiDiv, 2);   /* flexspi clock 120M. */
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/
/************************************************************************************
 * Name: imxrt_flexspi_nor_hyperbus_read
 *
 * Description:
 *   Check if bus is busy and wait until exiting busy
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_hyperbus_read(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
	flexspi_transfer_t flashXfer;
	status_t status;

	flashXfer.deviceAddress = addr * 2;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Read;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST;
	flashXfer.data = buffer;
	flashXfer.dataSize = bytes;
	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	if (status != kStatus_Success) {
		return status;
	}

	return status;
}

/************************************************************************************
 * Name: imxrt_flexspi_nor_flash_erase_sector
 *
 * Description:
 *   Erase data to hyper flash using flexspi interface
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
{
	status_t status;
	flexspi_transfer_t flashXfer;
	irqstate_t flags;

	flags = irqsave();

	/* Write enable */
	flashXfer.deviceAddress = address;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Command;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	if (status != kStatus_Success) {
		irqrestore(flags);
		return status;
	}

	flashXfer.deviceAddress = address;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Command;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	if (status != kStatus_Success) {
		irqrestore(flags);
		return status;
	}

	status = imxrt_flexspi_nor_wait_bus_busy(base);
	irqrestore(flags);

	return status;
}

/************************************************************************************
 * Name: imxrt_flexspi_nor_flash_page_program
 *
 * Description:
 *   Write data to hyper flash using flexspi interface
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src)
{
	status_t status;
	flexspi_transfer_t flashXfer;
	irqstate_t flags;

	flags = irqsave();

	/* Write enable */
	status = imxrt_flexspi_nor_write_enable(base, address);

	if (status != kStatus_Success) {
		irqrestore(flags);
		return status;
	}

	/* Prepare page program command */
	flashXfer.deviceAddress = address;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Write;
	flashXfer.SeqNumber = 2;
	flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD;
	flashXfer.data = (uint32_t *)src;
	flashXfer.dataSize = IMXRT_FLASH_PAGE_SIZE;
	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	if (status != kStatus_Success) {
		irqrestore(flags);
		return status;
	}

	status = imxrt_flexspi_nor_wait_bus_busy(base);
	irqrestore(flags);

	return status;
}


#ifdef CONFIG_MTD_PROGMEM
size_t up_progmem_pagesize(size_t page)
{
	printf("up_progmem_pagesize\n");
	return IMXRT_FLASH_PAGE_SIZE;
}

size_t up_progmem_blocksize(void)
{
	printf("up_progmem_blocksize\n");
	return IMXRT_BLOCK_SIZE;
}

ssize_t up_progmem_getpage(size_t addr)
{
	printf("up_progmem_getpage\n");

	if (addr < IMXRT_FLASH_BASE) {
		return 0;
	}

	if (addr >= IMXRT_FLASH_TOTAL_SIZE) {
		return -EFAULT;
	}

	return addr / IMXRT_FLASH_PAGE_SIZE;
}

size_t up_progmem_getaddress(size_t page)
{
	printf("up_progmem_getaddress\n");

	if (page >= (IMXRT_FLASH_TOTAL_SIZE/IMXRT_FLASH_PAGE_SIZE)) {
		return SIZE_MAX;
	}

	return page * IMXRT_FLASH_PAGE_SIZE + IMXRT_FLASH_BASE;
}

size_t up_progmem_npages(void)
{
	printf("up_progmem_npages\n");

	return (IMXRT_FLASH_TOTAL_SIZE/IMXRT_FLASH_PAGE_SIZE);
}

bool up_progmem_isuniform(void)
{
	printf("up_progmem_isuniform\n");
	return false;
}

ssize_t up_progmem_erasepage(size_t page)
{
	uint32_t address;

	printf("up_progmem_erasepage\n");

	address = up_progmem_getaddress(page);

	return imxrt_flexspi_nor_flash_erase_sector(IMXRT_FLEXSPI, address);
}

ssize_t up_progmem_ispageerased(size_t page)
{
	size_t addr;
	size_t count;
	size_t bwritten = 0;

	printf("up_progmem_erasepage\n");

	if (page >= IMXRT_FLASH_PAGE_SIZE) {
		return -EFAULT;
	}

	/* Verify */

	for (addr = up_progmem_getaddress(page), count = up_progmem_pagesize(page); count; count--, addr++) {
		if (getreg8(addr) != 0xff) {
			bwritten++;
		}
	}

	return bwritten;
}

ssize_t up_progmem_write(size_t addr, const void *buf, size_t count)
{
	printf("up_progmem_write\n");

	return 0;
}
#else
/************************************************************************************
 * Name: up_flashinitialize
 *
 * Description:
 *   Return an initialize MTD device instance.  MTD devices are not registered
 *   in the file system, but are created as instances that can be bound to
 *   other functions (such as a block or character driver front end).
 *
 ************************************************************************************/
FAR struct mtd_dev_s *up_flashinitialize(void)
{
	return (FAR struct mtd_dev_s *)imxrt_mtd_initialize();
}
#endif

/************************************************************************************
 * Name: imxrt_flash_init
 *
 * Description:
 *   Initialize flash 
 *
 ************************************************************************************/
void imxrt_flash_init(void)
{
	flexspi_config_t config;
	status_t status;

	flexspi_pins_init();
	flexspi_clock_init();

	/*Get FLEXSPI default settings and configure the flexspi. */
	imxrt_flexspi_getdefaultconfig(&config);

	/*Set AHB buffer size for reading data through AHB bus. */
	config.ahbConfig.enableAHBPrefetch = true;
	config.ahbConfig.enableAHBBufferable = true;
	config.ahbConfig.enableReadAddressOpt = true;
	config.ahbConfig.enableAHBCachable = true;
	config.rxSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
	imxrt_flexspi_init(IMXRT_FLEXSPI, &config);

	/* Configure flash settings according to serial flash feature. */
	imxrt_flexspi_setflashconfig(IMXRT_FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

	/* Update LUT table. */
	imxrt_flexspi_updatelut(IMXRT_FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

	/* Do software reset. */
	imxrt_flexspi_softwarereset(IMXRT_FLEXSPI);

	/* Enter quad mode. */
	status = imxrt_flexspi_nor_enable_quad_mode(IMXRT_FLEXSPI);
	if (status != kStatus_Success) {
		IMXLOG("flexspi_nor_enable_quad_mode error!!");
	}
}

