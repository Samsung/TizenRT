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
 * os/arch/arm/src/imxrt/imxrt_hyperflash.c
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
#include "imxrt_log.h"

#include "cache.h"
#include "up_arch.h"
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/spi/spi.h>

#include "imxrt_clock.h"
#include "imxrt_iomuxc.h"
#include "chip/imxrt_memorymap.h"


/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Private Data
 ************************************************************************************/
FAR struct mtd_dev_s *flash_dev = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
	.flexspiRootClk = 42000000, /* 42MHZ SPI serial clock */
	.isSck2Enabled = false,
	.flashSize = IMXRT_FLASH_SIZE,
	.CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
	.CSInterval = 2,
	.CSHoldTime = 0,
	.CSSetupTime = 3,
	.dataValidTime = 1,
	.columnspace = 3,
	.enableWordAddress = true,
	.AWRSeqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA,
	.AWRSeqNumber = 1,
	.ARDSeqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA,
	.ARDSeqNumber = 1,
	.AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
	.AHBWriteWaitInterval = 20,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
	/* Read Data */
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA + 1] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04),

	/* Write Data */
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA + 1] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x02),
	/* Read Status */
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 1] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 2] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 3] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x70), // DATA 0x70
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 4] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 5] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x0B),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 6] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x0),

	/* Write Enable */
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 1] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 2] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 3] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // DATA 0xAA
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 4] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 5] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 6] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x02),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 7] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),

	/* Erase Sector  */
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 1] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 2] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 3] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x80), // DATA 0x80
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 4] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 5] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 6] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 7] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 8] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 9] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 10] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x02),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 11] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 12] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 13] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 14] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x30, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

	/* program page */
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 2] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 3] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0), // DATA 0xA0
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 4] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
	[4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 5] = FLEXSPI_LUT_SEQ(
		kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x80),
};

/************************************************************************************
 * Private Functions
 ************************************************************************************/
/************************************************************************************
 * Name: flexspi_pins_init
 *
 * Description:
 *   Initialize pin setting in flexspi
 *
 ************************************************************************************/
static inline void flexspi_pins_init(void)
{
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
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02,   /* GPIO_SD_B1_01 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01,   /* GPIO_SD_B1_02 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00,   /* GPIO_SD_B1_03 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK,     /* GPIO_SD_B1_04 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 PAD functional properties : */
		0x0130F1u);                             /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Pull
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Enabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													MCR0 Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
	imxrt_iomuxc_setpinconfig(
		IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 PAD functional properties : */
		0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
													Drive Strength Field: R0/6
													Speed Field: max(200MHz)
													Open Drain Enable Field: Open Drain Disabled
													Pull / Keep Enable Field: Pull/Keeper Enabled
													Pull / Keep Select Field: Keeper
													Pull Up / Down Config. Field: 100K Ohm Pull Down
													Hyst. Enable Field: Hysteresis Disabled */
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
	imxrt_clock_initusb1pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332MHZ. */
	imxrt_clock_setmux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
	imxrt_clock_setdiv(kCLOCK_FlexspiDiv, 3);   /* flexspi clock 83M, DDR mode, internal clock 42M. */
}

/************************************************************************************
 * Name: flexspi_clock_update
 *
 * Description:
 *   Update clock setting in flexspi
 *
 ************************************************************************************/
static inline void flexspi_clock_update(void)
{
	/* Program finished, speed the clock to 166M. */
	imxrt_flexspi_enable(IMXRT_FLEXSPI, false);
	imxrt_clock_disableclock(IMXRT_FLEXSPI_CLOCK);
	imxrt_clock_setdiv(kCLOCK_FlexspiDiv, 0); /* flexspi clock 332M, DDR mode, internal clock 166M. */
	imxrt_clock_enableclock(IMXRT_FLEXSPI_CLOCK);
	imxrt_flexspi_enable(IMXRT_FLEXSPI, true);
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/
/****************************************************************************
 * Public Types
 ****************************************************************************/
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
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA;
	flashXfer.data = buffer;
	flashXfer.dataSize = bytes;
	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	if (status != kStatus_Success) {
		return status;
	}

	return status;
}

/************************************************************************************
 * Name: imxrt_flexspi_nor_hyperbus_write
 *
 * Description:
 *   Check if bus is busy and wait until exiting busy
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_hyperbus_write(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
	flexspi_transfer_t flashXfer;
	status_t status;

	flashXfer.deviceAddress = addr * 2;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Write;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA;
	flashXfer.data = buffer;
	flashXfer.dataSize = bytes;
	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	if (status != kStatus_Success) {
		return status;
	}

	return status;
}

/************************************************************************************
 * Name: imxrt_flexspi_nor_hyperflash_cfi
 *
 * Description:
 *   Initialize clock setting in flexspi
 *
 ************************************************************************************/
status_t imxrt_flexspi_nor_hyperflash_cfi(FLEXSPI_Type *base)
{
	/*
		* Read ID-CFI Parameters
		*/
	// CFI Entry
	status_t status;
	uint32_t buffer[2];
	uint32_t data = 0x9800;
	status = imxrt_flexspi_nor_hyperbus_write(base, 0x555, &data, 2);
	if (status != kStatus_Success) {
		return status;
	}

	// ID-CFI Read
	// Read Query Unique ASCII String
	status = imxrt_flexspi_nor_hyperbus_read(base, 0x10, &buffer[0], sizeof(buffer));
	if (status != kStatus_Success) {
		return status;
	}
	buffer[1] &= 0xFFFF;
	// Check that the data read out is  unicode "QRY" in big-endian order
	if ((buffer[0] != 0x52005100) || (buffer[1] != 0x5900)) {
		status = kStatus_Fail;
		return status;
	}
	// ASO Exit
	data = 0xF000;
	status = imxrt_flexspi_nor_hyperbus_write(base, 0x0, &data, 2);
	if (status != kStatus_Success) {
		return status;
	}

	return status;
}

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
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE;

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
	flashXfer.SeqNumber = 2;
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS;
	flashXfer.data = &readValue;
	flashXfer.dataSize = 2;

	do {
		status = imxrt_flexspi_transferblocking(base, &flashXfer);

		if (status != kStatus_Success) {
			return status;
		}
		if (readValue & 0x8000) {
			isBusy = false;
		} else {
			isBusy = true;
		}

		if (readValue & 0x3200) {
			status = kStatus_Fail;
			break;
		}

	} while (isBusy);

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
	status = imxrt_flexspi_nor_write_enable(base, address);

	if (status != kStatus_Success) {
		irqrestore(flags);
		return status;
	}

	flashXfer.deviceAddress = address;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Command;
	flashXfer.SeqNumber = 4;
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR;
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
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
	flashXfer.data = (uint32_t *)src;
	flashXfer.dataSize = IMXRT_FLASH_PAGE_SIZE;
	status = imxrt_flexspi_transferblocking(base, &flashXfer);

	if (status != kStatus_Success) {
		irqrestore(flags);
		return status;
	}

	status = imxrt_flexspi_nor_wait_bus_busy(base);

	arch_flush_dcache(address + IMXRT_FLASH_BASE, address + IMXRT_FLASH_BASE + IMXRT_FLASH_PAGE_SIZE);

	irqrestore(flags);

	return status;
}

/* The smallest integer type that can hold the IOMUX encoding */

typedef uint16_t iomux_pinset_t;
#ifdef CONFIG_MTD_PROGMEM
size_t up_progmem_pagesize(size_t page)
{
	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_pagesize called: page[%d] ", page);
	IMXLOG(tempLog);
	return IMXRT_FLASH_PAGE_SIZE;
}

size_t up_progmem_blocksize(void)
{
	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_blocksize called ");
	IMXLOG(tempLog);
	return IMXRT_BLOCK_SIZE;
}

ssize_t up_progmem_getpage(size_t addr)
{
	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_getpage called: addr[%d] ", addr);
	IMXLOG(tempLog);

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
	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_getaddress called: page[%d] size_t[%d]", page, sizeof(size_t));
	IMXLOG(tempLog);

	if (page >= (IMXRT_FLASH_TOTAL_SIZE/IMXRT_FLASH_PAGE_SIZE)) {
		return SIZE_MAX;
	}

	return page * IMXRT_FLASH_PAGE_SIZE + IMXRT_FLASH_BASE;
}

size_t up_progmem_npages(void)
{
	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_npages called ");
	IMXLOG(tempLog);

	return (IMXRT_FLASH_TOTAL_SIZE/IMXRT_FLASH_PAGE_SIZE);
}

bool up_progmem_isuniform(void)
{
	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_isuniform called ");
	IMXLOG(tempLog);
#ifdef IMXRT_FLASH_PAGE_SIZE
	return true;
#else
	return false;
#endif							/* def IMXRT_FLASH_PAGE_SIZE */
}

ssize_t up_progmem_erasepage(size_t page)
{
	status_t status;
	flexspi_transfer_t flashXfer;
	uint32_t address;

	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_erasepage called: page[%d] ", page);
	IMXLOG(tempLog);

	address = up_progmem_getaddress(page);

	/* Write enable */
	status = imxrt_flexspi_nor_write_enable(IMXRT_FLEXSPI, address);

	if (status != kStatus_Success) {
		return status;
	}

	flashXfer.deviceAddress = address;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Command;
	flashXfer.SeqNumber = 4;
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR;
	status = imxrt_flexspi_transferblocking(IMXRT_FLEXSPI, &flashXfer);

	if (status != kStatus_Success) {
		return status;
	}

	status = imxrt_flexspi_nor_wait_bus_busy(IMXRT_FLEXSPI);

	return up_progmem_blocksize();
}

ssize_t up_progmem_ispageerased(size_t page)
{
	size_t addr;
	size_t count;
	size_t bwritten = 0;

	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_ispageerased called : page[%d]", page);
	IMXLOG(tempLog);

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
	flexspi_transfer_t flashXfer;
	status_t status;

	char tempLog[128];

	snprintf(tempLog, sizeof(tempLog), " up_progmem_write called :addr[%d]", addr);
	IMXLOG(tempLog);

	flashXfer.deviceAddress = addr * 2;
	flashXfer.port = kFLEXSPI_PortA1;
	flashXfer.cmdType = kFLEXSPI_Write;
	flashXfer.SeqNumber = 1;
	flashXfer.seqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA;
	flashXfer.data = (uint32_t *)buf;
	flashXfer.dataSize = count;
	status = imxrt_flexspi_transferblocking(IMXRT_FLEXSPI, &flashXfer);

	if (status != kStatus_Success) {
		return status;
	}

	return count;
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

	flexspi_pins_init();

	flexspi_clock_init();

	/*Get FLEXSPI default settings and configure the flexspi. */
	imxrt_flexspi_getdefaultconfig(&config);
	
	/*Set AHB buffer size for reading data through AHB bus. */
	config.ahbConfig.enableAHBPrefetch = true;
	/*Allow AHB read start address do not follow the alignment requirement. */
	config.ahbConfig.enableReadAddressOpt = true;
	config.ahbConfig.enableAHBBufferable = true;
	config.ahbConfig.enableAHBCachable = true;
	/* enable diff clock and DQS */
	config.enableSckBDiffOpt = true;
	config.rxSampleClock = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
	config.enableCombination = true;
	imxrt_flexspi_init(IMXRT_FLEXSPI, &config);

	/* Configure flash settings according to serial flash feature. */
	imxrt_flexspi_setflashconfig(IMXRT_FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

	/* Update LUT table. */
	imxrt_flexspi_updatelut(IMXRT_FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

	/* Do software reset. */
	imxrt_flexspi_softwarereset(IMXRT_FLEXSPI);
}

