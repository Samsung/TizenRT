/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_qspi.c
 *
 *   Copyright (C) 2009, 2014 Gregory Nutt. All rights reserved.
 *   Author:
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
#include <tinyara/arch.h>

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <tinyara/kmalloc.h>

#include <arch/chip/irq.h>
#include <tinyara/watchdog.h>
#include <arch/chip/chip_types.h>

#include "chip.h"
#include "up_arch.h"
#include "cache.h"

#include <tinyara/clock.h>
#include <clock/clock.h>

/****************************************************************************
 * Private Functions Prototypes
 ****************************************************************************/
static void qspi_open(void);
static void qspi_close(void);
static ssize_t qspi_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t qspi_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static long qspi_ioctl(struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Types
 ****************************************************************************/
static const struct file_operations drv_fops = {
	(void *)qspi_open,		/* open */
	(void *)qspi_close,		/* close */
	(void *)qspi_read,		/* read */
	(void *)qspi_write,		/* write */
	0,						/* seek */
	(void *)qspi_ioctl		/* ioctl */
};

/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
#define DATA_SIZE 0x1000		/* 4096 */
#define USE_MALLOC (1)

#define WL_SIZE 100
#define AREA_A 0
#define AREA_B 1
int g_base_address;

u32 a[DATA_SIZE], b[DATA_SIZE];

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_TEST_QSPI
#define qspitest printf
#else
#define qspitest(x...)
#endif

#ifdef CONFIG_WL_DEBUG_QSPI
#define qspi_wl_debug printf
#else
#define qspi_wl_debug(x...)
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void copy4B(u32 targetAddr, u32 sourceAddr, u32 sizeByte)
{
	u32 loopt;
	u32 uRoundup;
	int i;

	uRoundup = sizeByte % 4;
	if (uRoundup) {
		sizeByte += (4 - uRoundup);
	}

	loopt = sizeByte / 4;

	for (i = 0; i < loopt; i++) {
		Outp32(targetAddr + (4 * i), Inp32(sourceAddr + (4 * i)));
	}

	return;
}

void copy1B(u32 targetAddr, u32 sourceAddr, u32 sizeByte)
{
	u32 loopt;
	int i;
	loopt = sizeByte;

	for (i = 0; i < loopt; i++) {
		Outp8(targetAddr + (i), Inp8(sourceAddr + (i)));
	}

	return;
}

void QSPI_Set_GPIOPort(void)
{
	s32 gpio_sf_clk, gpio_sf_cs, gpio_sf_si, gpio_sf_so, gpio_sf_wp, gpio_sf_hld;

	gpio_sf_clk = s5j_gpio(GPP1, 0);
	gpio_sf_cs = s5j_gpio(GPP1, 1);
	gpio_sf_si = s5j_gpio(GPP1, 2);
	gpio_sf_so = s5j_gpio(GPP1, 3);
	gpio_sf_wp = s5j_gpio(GPP1, 4);
	gpio_sf_hld = s5j_gpio(GPP1, 5);

	gpio_cfg_pin(gpio_sf_clk, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_cs, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_si, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_so, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_wp, GPIO_FUNC(2));
	gpio_cfg_pin(gpio_sf_hld, GPIO_FUNC(2));

	gpio_set_pull(gpio_sf_clk, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_cs, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_si, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_so, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_wp, GPIO_PULL_UP);
	gpio_set_pull(gpio_sf_hld, GPIO_PULL_UP);

	return;
}

void QSPI_disable_wp(void)
{
	HW_REG32(0x80310000, 0x04) &= ~(0x1 << 31);
	HW_REG32(0x80310000, 0x04) |= (0x1 << 31);
}

void QSPI_enable_wp(void)
{
	HW_REG32(0x80310000, 0x04) &= ~(0x1 << 31);
}

void QSPI_Init(void)
{
	QSPI_Set_GPIOPort();
	HW_REG32(0x80310000, 0x04) = 0x8010001A;	/* disable WP */
	HW_REG32(0x80310000, 0x78) = 0x8;	//FLASH_IO_MODE
	HW_REG32(0x80310000, 0x74) = 0x4;	//QUAD_FAST_READ

	/* COMMNAD1 will be asserted by default value */

	cal_clk_setrate(d1_serialflash, 80000000);
}

void qspi_get_base(void)
{
	u32 ctrlInfo[DATA_SIZE];
	int countA;
	int countB;
	int flag;

	copy4B((u32) ctrlInfo, QSPI_SECURE_AREA_CTRL_ADDRESS, DATA_SIZE);
	countA = ctrlInfo[0];
	countB = ctrlInfo[1];
	flag = ctrlInfo[2];

	if (flag == AREA_A) {
		g_base_address = QSPI_SECURE_AREA_START_ADDRESS_A;
	} else if (flag == AREA_B) {
		g_base_address = QSPI_SECURE_AREA_START_ADDRESS_B;
	}

	qspi_wl_debug("flag:%d coutA:%d countB:%d\n", flag, countA, countB);
}

void QSPI_Set_Vendor(eQSPI_VENDOR vendor)
{
	SetBits(rSF_CON, 16, 0x3F, (u32)vendor);
	return;
}

u8 QSPI_Get_Vendor(void)
{
	return GetBits(rSF_CON, 16, 0x3F);
}

void QSPI_Set_Vendor_Command(u8 vendor)
{
	eQSPI_VENDOR id;
	switch (vendor) {
	case 0xC2:
		//MXIC
		id = QSPI_VENDOR_MACRONIX;
		break;

	case 0xEF:
		//Winbond
		id = QSPI_VENDOR_WINBOND;
		break;

	case 0x20:
		//ST Micronics (MICRO)
		id = QSPI_VENDOR_STMICRO;
		break;

	case 0xBF:
		//SST(Silicon Storage Technology)
		id = QSPI_VENDOR_SST;
		break;

	case 0x1F:
		//ATMEL
		id = QSPI_VENDOR_ATMEL;
		break;

	case 0x01:
		//AMD(Spansion)
		id = QSPI_VENDOR_AMD;
		break;

	default:
		id = (eQSPI_VENDOR) QSPI_Get_Vendor();
		break;
	}

	QSPI_Set_Vendor(id);
}

void QSPI_Set_ReadMode(eQSPI_MODE mode, u32 command)
{
	switch (mode) {
	case QSPI_SINGLE_IO:

		//Disable performance enhancement
		SetBits(rFLASH_IO_MODE, 0, 0xF, 0x8);

		//Set READ command manually.
		if (command != 0) {
			SetBits(rCOMMAND5, 0, 0xFFU, command);
		}
		break;

	case QSPI_DUAL_FAST:
		SetBits(rFLASH_IO_MODE, 0, 0xF, 0x2);
		SetBits(rFLASH_PERF_MODE, 3, 1, 0);

		//Set READ command manually.
		if (command != 0) {
			SetBits(rCOMMAND5, 16, 0xFFU, command);
		}

		break;

	case QSPI_DUAL_IO:
		SetBits(rFLASH_IO_MODE, 0, 0xF, 0x2);
		SetBits(rFLASH_PERF_MODE, 3, 1, 1);

		//Set READ command manually.
		if (command != 0) {
			SetBits(rCOMMAND5, 16, 0xFFU, command);
		}

		break;

	case QSPI_QUAD_FAST:

		SetBits(rFLASH_IO_MODE, 0, 0xF, 0x4);
		SetBits(rFLASH_PERF_MODE, 3, 1, 0);

		//Set READ command manually.
		if (command != 0) {
			SetBits(rCOMMAND5, 24, 0xFFU, command);
		}

		break;

	case QSPI_QUAD_IO:
		SetBits(rFLASH_IO_MODE, 0, 0xF, 0x4);
		SetBits(rFLASH_PERF_MODE, 3, 1, 1);

		//Set READ command manually.
		if (command != 0) {
			SetBits(rCOMMAND1, 24, 0xFFU, command);
		}

		break;
	}

	return;
}

void QSPI_Set_WriteMode(eQSPI_WR_MODE mode, u32 command)
{
	switch (mode) {
	case QSPI_WREN:

		SetBits(rCOMMAND4, 8, 0xFF, command);
		break;

	case QSPI_WRDI:
		SetBits(rCOMMAND4, 0, 0xFF, command);

		break;

	case QSPI_WRSR:
		SetBits(rCOMMAND3, 16, 0xFF, command);

		break;
	}

	return;
}

void QSPI_Set_Page_Size(eQSPI_PAGE_SIZE size)
{
	//Bytes per Page
	SetBits(rSF_CON, 8, 0xF, size);

	return;
}

void QSPI_Read_ID(u32 *vendor)
{
	*vendor = Inp32(rRDID);
	return;
}

sRead_Status_Register QSPI_Get_Status_Register(void)
{
	sRead_Status_Register reg = { 0 };

	reg.rdsr = Inp32(rRDSR);

	return reg;
}

void QSPI_Write_Status_Register(u16 wrsr)
{
	Outp16(rWRSR, wrsr);

	return;
}

void QSPI_Sector_Erase(u32 targetAddr)
{
	Outp32(rERASE_ADDRESS, targetAddr);

	Outp8(rSE, QSPI_DUMMY_DATA);

	arch_invalidate_dcache(targetAddr + CONFIG_S5J_FLASH_BASE, (targetAddr + CONFIG_S5J_FLASH_BASE + QSPI_SIZE_4KB));
}

void QSPI_Block_Erase(u32 targetAddr, eQSPI_BLOCK_SIZE unit)
{
	u32 blockEraseSize = 0;

	if (unit == BLOCK_64KB) {
		SetBits(rCOMMAND2, 16, 0xFF, COMMAND_ERASE_64KB);
		blockEraseSize = QSPI_SIZE_64KB;
	} else {
		SetBits(rCOMMAND2, 16, 0xFF, COMMAND_ERASE_32KB);
		blockEraseSize = QSPI_SIZE_32KB;
	}

	Outp32(rERASE_ADDRESS, targetAddr);

	Outp8(rBE, QSPI_DUMMY_DATA);

	arch_invalidate_dcache(targetAddr + CONFIG_S5J_FLASH_BASE, (targetAddr + CONFIG_S5J_FLASH_BASE + blockEraseSize));
}

void QSPI_Chip_Erase(void)
{
	Outp8(rCE, QSPI_DUMMY_DATA);
}

eERASE_UNIT Get_Erase_Unit(u32 offsetStart, u32 target)
{
	u32 sizeLeft;
	sizeLeft = target - offsetStart;

	if (offsetStart == 0) {
		if (sizeLeft >= QSPI_SIZE_64KB) {
			return TYPE_64KB;
		} else if (sizeLeft >= QSPI_SIZE_32KB) {
			return TYPE_32KB;
		} else if (sizeLeft >= QSPI_SIZE_4KB) {
			return TYPE_4KB;
		} else {
			return TYPE_ERR;
		}
	}

	if ((offsetStart / QSPI_SIZE_64KB) && (sizeLeft >= QSPI_SIZE_64KB) && !(offsetStart % QSPI_SIZE_64KB)) {
		return TYPE_64KB;
	} else if ((offsetStart / QSPI_SIZE_32KB) && (sizeLeft >= QSPI_SIZE_32KB) && !(offsetStart % QSPI_SIZE_32KB)) {
		return TYPE_32KB;
	} else if ((offsetStart / QSPI_SIZE_4KB) && (sizeLeft >= QSPI_SIZE_4KB) && !(offsetStart % QSPI_SIZE_4KB)) {
		return TYPE_4KB;
	} else {
		return TYPE_ERR;
	}
}

//size should 4KB aligned
bool QSPI_Erase(u32 targetAddr, u32 size)
{
	u32 temp = 0;
	u32 target;
	eERASE_UNIT type;

	targetAddr = targetAddr - CONFIG_S5J_FLASH_BASE;

	temp = targetAddr % QSPI_SIZE_4KB;
	if (temp) {
		return false;
	}

	//Check address alignment
	if ((size % QSPI_SIZE_4KB) != 0) {
		return false;
	}

	if (size < QSPI_SIZE_4KB) {
		return false;
	}

	//Erase Offset
	temp = targetAddr;
	target = temp + size;

	do {
		type = Get_Erase_Unit(temp, target);

		switch (type) {
		case TYPE_4KB:
			QSPI_Sector_Erase(temp);
			temp += QSPI_SIZE_4KB;
			continue;

		case TYPE_32KB:
			QSPI_Block_Erase(temp, BLOCK_32KB);
			temp += QSPI_SIZE_32KB;
			continue;

		case TYPE_64KB:
			QSPI_Block_Erase(temp, BLOCK_64KB);
			temp += QSPI_SIZE_64KB;
			continue;

		default:
			return false;
		}
	} while (temp < target);

	return true;
}

void QSPI_Read(u32 targetAddr, u32 sourceAddr, u32 sizeByte)
{
	if (targetAddr >= QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("Unable to access\n");
		return;
	}

	copy4B(sourceAddr, targetAddr, sizeByte);
	/* copy1B(sourceAddr, targetAddr, sizeByte); */
}

void QSPI_Secure_Read(u32 targetOffset, u32 sourceAddr, u32 sizeByte)
{
	u32 targetAddr;

	targetAddr = QSPI_SECURE_AREA_START_ADDRESS + targetOffset;

	if (targetAddr >= QSPI_FLASH_END_ADDRESS) {
		lldbg("out of flash: 0x%x\n", targetAddr);
		return;
	}
	if (targetAddr < QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("out of secure area\n");
		return;
	}

	copy4B(sourceAddr, targetAddr, sizeByte);
}

void QSPI_Secure_Read_WL(u32 targetOffset, u32 sourceAddr, u32 sizeByte)
{
	u32 targetAddr;

	targetAddr = g_base_address + targetOffset;

	if (targetAddr >= QSPI_FLASH_END_ADDRESS) {
		lldbg("out of flash: 0x%x\n", targetAddr);
		return;
	}
	if (targetAddr < QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("out of secure area\n");
		return;
	}

	copy4B(sourceAddr, targetAddr, sizeByte);
}

u32 wear_leveling(u32 targetAddr)
{
	/* Need to consider speed during swapping */
	int countA;
	int countB;
	int flag;
	int addrA = QSPI_SECURE_AREA_START_ADDRESS_A;
	int addrB = QSPI_SECURE_AREA_START_ADDRESS_B;
#if USE_MALLOC
	u32 *ctrlInfo;
	ctrlInfo = (u32 *)malloc(4 * DATA_SIZE);

	if (ctrlInfo == NULL) {
		return 0;
	}
#else
	u32 ctrlInfo[DATA_SIZE];
#endif

	/*Read ctrA & ctrB & flag */
	if (g_base_address == addrA) {
		QSPI_Secure_Read_WL((2 * QSPI_SECURE_AREA_SIZE), (u32)ctrlInfo, DATA_SIZE);
	} else if (g_base_address == addrB) {
		QSPI_Secure_Read_WL((QSPI_SECURE_AREA_SIZE), (u32)ctrlInfo, DATA_SIZE);
	}

	countA = ctrlInfo[0];
	countB = ctrlInfo[1];
	flag = ctrlInfo[2];

	qspi_wl_debug("%d %d %d\n", flag, countA, countB);

	if (flag == AREA_A) {
		if (countA == countB + WL_SIZE) {	/*Switch A to B */
			int i;
#if USE_MALLOC
			u8 *TempBuffer;
			TempBuffer = (u8 *)malloc(DATA_SIZE);
#else
			u8 TempBuffer[DATA_SIZE];
#endif

			/* copy A to B */
			qspi_wl_debug("swapping A to B\n");
			for (i = 0; i < 23; i++) {
				g_base_address = addrA;
				QSPI_Secure_Read_WL((i * DATA_SIZE), (u32)TempBuffer, DATA_SIZE);
				g_base_address = addrB;
				QSPI_Secure_Write_NoWL(i * DATA_SIZE, (u32)TempBuffer, DATA_SIZE);
			}
#if USE_MALLOC
			free(TempBuffer);
#endif

			/* ctrB++, flag = B, write ctrol */
			ctrlInfo[1] = countB + 1;
			ctrlInfo[2] = AREA_B;
			g_base_address = addrA;
			QSPI_Secure_Write_NoWL(2 * QSPI_SECURE_AREA_SIZE, (u32)ctrlInfo, DATA_SIZE);
			g_base_address = addrB;

#if USE_MALLOC
			free(ctrlInfo);
#endif
			return (targetAddr + QSPI_SECURE_AREA_SIZE);
		} else {
			/* ctrA++, write ctrol */
			ctrlInfo[0] = countA + 1;
			g_base_address = addrA;
			QSPI_Secure_Write_NoWL(2 * QSPI_SECURE_AREA_SIZE, (u32)ctrlInfo, DATA_SIZE);

#if USE_MALLOC
			free(ctrlInfo);
#endif
			return (targetAddr);
		}
	} else if (flag == AREA_B) {	/*Switch B to A */
		if (countB == countA + WL_SIZE) {
			int i;
#if USE_MALLOC
			u8 *TempBuffer;
			TempBuffer = (u8 *)malloc(DATA_SIZE);
#else
			u8 TempBuffer[DATA_SIZE];
#endif

			/* copy B to A */
			qspi_wl_debug("swapping B to A\n");
			for (i = 0; i < 23; i++) {
				g_base_address = addrB;
				QSPI_Secure_Read_WL((i * DATA_SIZE), (u32)TempBuffer, DATA_SIZE);
				g_base_address = addrA;
				QSPI_Secure_Write_NoWL(i * DATA_SIZE, (u32)TempBuffer, DATA_SIZE);

			}
#if USE_MALLOC
			free(TempBuffer);
#endif
			/* ctrA++, flag = A, write ctrol  */
			g_base_address = addrA;
			ctrlInfo[0] = countA + 1;
			ctrlInfo[2] = AREA_A;
			QSPI_Secure_Write_NoWL(2 * QSPI_SECURE_AREA_SIZE, (u32)ctrlInfo, DATA_SIZE);
#if USE_MALLOC
			free(ctrlInfo);
#endif
			return (targetAddr - QSPI_SECURE_AREA_SIZE);
		} else {
			/* ctrB++, write ctrol */
			ctrlInfo[1] = countB + 1;
			g_base_address = addrA;
			QSPI_Secure_Write_NoWL(2 * QSPI_SECURE_AREA_SIZE, (u32)ctrlInfo, DATA_SIZE);
			g_base_address = addrB;
#if USE_MALLOC
			free(ctrlInfo);
#endif
			return (targetAddr);
		}
	}
#if USE_MALLOC
	free(ctrlInfo);
#endif
	return targetAddr;
}

u32 QSPI_Write(u32 targetAddr, u32 sourceAddr, u32 sizeByte)
{
	if (targetAddr >= QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("Unable to access\n");
		return false;
	}

	//Disable WP(Write Protect)
	SetBits(rSF_CON, 31, 0x1U, 1U);

	//Page Programming
	SetBits(rSF_CON, 15, 0x1, 1);

	// Checks Address range (16MB)
	if (((targetAddr - CONFIG_S5J_FLASH_BASE) >> 24) > 0) {	// 16MB
		lldbg("Flash Address range over!(24bit range)\n");
		return false;
	}
	//Checks sector(4KB) align
	if ((targetAddr & 0xFFF) != 0) {
		lldbg("4KB alignment failure\n");
		return false;
	}
	//should be erased before writing.
	if (QSPI_Erase(targetAddr, sizeByte) == false) {
		lldbg("Erase Failure\n");
		return false;
	}
	copy4B(targetAddr, sourceAddr, sizeByte);

	return true;

}

u32 QSPI_Write_No_Erase(u32 targetAddr, u32 sourceAddr, u32 sizeByte)
{
	if (targetAddr >= QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("Unable to access\n");
		return false;
	}

	SetBits(rSF_CON, 31, 0x1U, 1U);
	SetBits(rSF_CON, 15, 0x1, 1);

	if (((targetAddr - CONFIG_S5J_FLASH_BASE) >> 24) > 0) {
		lldbg("Flash Address range over!(24bit range)\n");
		return false;
	}

	if ((targetAddr & 0xFFF) != 0) {
		return false;
	}

	copy4B(targetAddr, sourceAddr, sizeByte);

	return true;
}

u32 QSPI_Secure_Write(u32 targetOffset, u32 sourceAddr, u32 sizeByte)
{
	u32 targetAddr;
	u32 aligned_addr;
	u32 aligned_size;
	u32 aligned_offset;
	u32 remainder_offset;
	int i;
#if USE_MALLOC
	u8 *TempBuffer;
	TempBuffer = (u8 *)malloc(8 * 1024 + 32);
#else
	u8 TempBuffer[8 * 1024];
#endif

	targetAddr = QSPI_SECURE_AREA_START_ADDRESS + targetOffset;
	/* lldbg("\t\t\t\tw:0x%08x\n", targetOffset); */
	QSPI_disable_wp();

	//size check
	aligned_size = (targetAddr & 0xFFF) + sizeByte;
	if (aligned_size > 4 * 1024) {
		aligned_size = 8 * 1024;
	} else {
		aligned_size = 4 * 1024;
	}

	//addr align check
	aligned_addr = (targetAddr & ~(0xFFF));
	aligned_offset = (targetOffset & ~(0xFFF));
	remainder_offset = (targetOffset & 0xFFF);

	if (targetAddr < QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("out of secure area\n");
#if USE_MALLOC
		free(TempBuffer);
#endif
		QSPI_enable_wp();
		return false;
	}

	SetBits(rSF_CON, 31, 0x1U, 1U);
	SetBits(rSF_CON, 15, 0x1, 1);

	if (((targetAddr - CONFIG_S5J_FLASH_BASE) >> 24) > 0) {
		lldbg("Flash Address range over!(24bit range)\n");
#if USE_MALLOC
		free(TempBuffer);
#endif
		QSPI_enable_wp();
		return false;
	}

	QSPI_Secure_Read(aligned_offset, (u32)TempBuffer, aligned_size);
	for (i = 0; i < sizeByte; i++) {
		*((u8 *)TempBuffer + remainder_offset + i) = *((u8 *)sourceAddr + i);
	}

	aligned_addr = wear_leveling(aligned_addr);
	if (QSPI_Erase(aligned_addr, aligned_size) == false) {
#if USE_MALLOC
		free(TempBuffer);
#endif
		QSPI_enable_wp();
		return false;
	}

	copy4B(aligned_addr, (u32)TempBuffer, aligned_size);
	arch_flush_dcache(aligned_addr, (aligned_addr + aligned_size));
#if USE_MALLOC
	free(TempBuffer);
#endif
	QSPI_enable_wp();

	return true;
}

u32 QSPI_Secure_Write_WL(u32 targetOffset, u32 sourceAddr, u32 sizeByte)
{
	u32 targetAddr;
	targetAddr = g_base_address + targetOffset;

	if (targetAddr < QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("out of secure area: 0x%x\n", targetAddr);
		return false;
	}

	SetBits(rSF_CON, 31, 0x1U, 1U);
	SetBits(rSF_CON, 15, 0x1, 1);

	if (((targetAddr - CONFIG_S5J_FLASH_BASE) >> 24) > 0) {
		lldbg("Flash Address range over!(24bit range)\n");
		return false;
	}

	if ((targetAddr & 0xFFF) != 0) {
		return false;
	}

	targetAddr = wear_leveling(targetAddr);

	if (QSPI_Erase(targetAddr, sizeByte) == false) {
		return false;
	}

	copy4B(targetAddr, sourceAddr, sizeByte);

	return true;
}

u32 QSPI_Secure_Write_NoWL(u32 targetOffset, u32 sourceAddr, u32 sizeByte)
{
	u32 targetAddr;
	targetAddr = g_base_address + targetOffset;

	if (targetAddr < QSPI_SECURE_AREA_START_ADDRESS) {
		lldbg("out of secure area: 0x%x\n", targetAddr);
		return false;
	}

	SetBits(rSF_CON, 31, 0x1U, 1U);
	SetBits(rSF_CON, 15, 0x1, 1);

	if (((targetAddr - CONFIG_S5J_FLASH_BASE) >> 24) > 0) {
		lldbg("Flash Address range over!(24bit range)\n");
		return false;
	}

	if ((targetAddr & 0xFFF) != 0) {
		return false;
	}

	if (QSPI_Erase(targetAddr, sizeByte) == false) {
		return false;
	}

	copy4B(targetAddr, sourceAddr, sizeByte);

	return true;
}

u32 Nv_Write(u32 targetAddr, u32 sourceAddr, u32 sizeByte)
{
	if ((targetAddr < CONFIG_NVRAM_WIFI_START)
		|| ((targetAddr + sizeByte) > (CONFIG_NVRAM_WIFI_START + 8 * 1024))
		|| (sizeByte > (8 * 1024))) {
		lldbg("Unable to access\n");
		return false;
	}

	//Disable WP(Write Protect)
	SetBits(rSF_CON, 31, 0x1U, 1U);

	//Page Programming
	SetBits(rSF_CON, 15, 0x1, 1);

	// Checks Address range (16MB)
	if (((targetAddr - CONFIG_S5J_FLASH_BASE) >> 24) > 0) {	// 16MB
		lldbg("Flash Address range over!(24bit range)\n");
		return false;
	}

	memcpy((void *)targetAddr, (void *)sourceAddr, sizeByte);
	arch_flush_dcache(CONFIG_NVRAM_WIFI_START, CONFIG_NVRAM_WIFI_START + 8 * 1024);

	return true;
}

u32 Nv_Read(u32 targetAddr, u32 sourceAddr, u32 sizeByte)
{
	if ((sourceAddr < CONFIG_NVRAM_WIFI_START)
		|| ((sourceAddr + sizeByte) > (CONFIG_NVRAM_WIFI_START + 8 * 1024))
		|| (sizeByte > (8 * 1024))) {
		lldbg("Unable to access\n");
		return false;
	}

	memcpy((void *)targetAddr, (void *)sourceAddr, sizeByte);
	return true;
}

u32 Nv_Erase(u32 targetAddr, u32 sizeByte)
{
	if ((targetAddr < CONFIG_NVRAM_WIFI_START)
		|| ((targetAddr + sizeByte) > (CONFIG_NVRAM_WIFI_START + 8 * 1024))
		|| (sizeByte > (8 * 1024))) {
		lldbg("Unable to access\n");
		return false;
	}

	//Disable WP(Write Protect)
	SetBits(rSF_CON, 31, 0x1U, 1U);

	//Page Programming
	SetBits(rSF_CON, 15, 0x1, 1);

	// Checks Address range (16MB)
	if (((targetAddr - CONFIG_S5J_FLASH_BASE) >> 24) > 0) {	// 16MB
		lldbg("Flash Address range over!(24bit range)\n");
		return false;
	}
	//Checks sector(4KB) align
	if ((targetAddr & 0xFFF) != 0) {
		lldbg("4KB alignment failure\n");
		return false;
	}

	arch_flush_dcache(CONFIG_NVRAM_WIFI_START, CONFIG_NVRAM_WIFI_START + 8 * 1024);

	//should be erased before writing.
	if (QSPI_Erase(targetAddr, sizeByte) == false) {
		lldbg("Erase Failure\n");
		return false;
	}

	return true;
}

void QSPI_Set_User_Instruction(u8 command)
{
	SetBits(rCOMMAND2, 24, 0xFFU, command);
}

static void qspi_open(void)
{

}

static void qspi_close(void)
{

}

static ssize_t qspi_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{

	return 0;
}

static ssize_t qspi_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	return 0;
}

ssize_t qspi_Wtest(void)
{
	systime_t start = 0;
	systime_t elapsed = 0;
	int i;

	start = clock_systimer();
	for (i = 0; i < 128; i++) {
		QSPI_Write(0x4500000 + (DATA_SIZE * i), (u32)&a[0], DATA_SIZE);
	}
	elapsed = clock_systimer() - start;
	qspitest("w: %d\n", elapsed);
	return 0;
}

ssize_t qspi_Rtest(void)
{
	systime_t start = 0;
	systime_t elapsed = 0;
	int i;

	for (i = 0; i < DATA_SIZE; i++) {
		b[i] = 0;
	}

	start = clock_systimer();
	for (i = 0; i < 128; i++) {
		QSPI_Read(0x4500000 + (DATA_SIZE * i), (u32)&b[0], DATA_SIZE);
	}
	elapsed = clock_systimer() - start;
	qspitest("r: %d\n", elapsed);

	for (i = 0; i < 8; i++) {
		qspitest("0x%x, ", b[i]);
	}
	return 0;
}

ssize_t qspi_Etest(void)
{
	systime_t start = 0;
	systime_t elapsed = 0;
	int i;

	start = clock_systimer();
	for (i = 0; i < 256; i++) {	//4K
		QSPI_Erase(0x4500000 + (DATA_SIZE * i), DATA_SIZE);
	}
	for (i = 0; i < 128; i++) {	//16K
		QSPI_Erase(0x4500000 + (4 * DATA_SIZE * i), (4 * DATA_SIZE));
	}
	for (i = 0; i < 64; i++) {	//32K
		QSPI_Erase(0x4600000 + (8 * DATA_SIZE * i), (8 * DATA_SIZE));
	}
	for (i = 0; i < 32; i++) {	//64K
		QSPI_Erase(0x4600000 + (16 * DATA_SIZE * i), (16 * DATA_SIZE));
	}
	elapsed = clock_systimer() - start;
	qspitest("\ne: %d\n", elapsed);
	return 0;
}

#if 1
ssize_t qspi_Secure_Wtest(void)
{
	uint32_t start = 0;
	uint32_t elapsed = 0;
	int i;

	start = clock_systimer();
	for (i = 0; i < 23; i++)
		//QSPI_Secure_Write((DATA_SIZE*i), (u32)&a[0], DATA_SIZE);
	{
		QSPI_Secure_Write_WL((DATA_SIZE * i), (u32)&a[0], DATA_SIZE);
	}
	elapsed = clock_systimer() - start;
	qspitest("secure w: %d\n", elapsed);
	return 0;
}

ssize_t qspi_Secure_Rtest(void)
{
	uint32_t start = 0;
	uint32_t elapsed = 0;
	int i;

	for (i = 0; i < DATA_SIZE; i++) {
		b[i] = 0;
	}

	start = clock_systimer();
	for (i = 0; i < 23; i++)
		//QSPI_Secure_Read((DATA_SIZE*i), (u32)&b[0], DATA_SIZE);
	{
		QSPI_Secure_Read_WL((DATA_SIZE * i), (u32)&b[0], DATA_SIZE);
	}
	elapsed = clock_systimer() - start;
	qspitest("secure r: %d\n", elapsed);

	for (i = 0; i < 8; i++) {
		qspitest("%d, ", b[i]);
	}
	return 0;
}
#endif

static long qspi_ioctl(struct file *filep, int cmd, unsigned long arg)
{
	u32 data;
	u32 vendor_id = 0;
	u8 tempBuffer[DATA_SIZE] = { 0 };
	int i;

	switch (cmd) {
	case QSPI_READ_INFO:
		qspitest("\n: 0x%x, 0x%x, 0x%x", QSPI_SIZE_4KB, QSPI_SIZE_32KB, QSPI_SIZE_64KB);
		qspitest("0x%x\n", QSPI_SECURE_AREA_START_ADDRESS);
		qspitest("0x%x\n", QSPI_FLASH_END_ADDRESS);

		data = Inp32(rSF_CON);
		qspitest("0x%x: 0x%x\n", rSF_CON, data);
		data = Inp32(rERASE_ADDRESS);
		qspitest("0x%x: 0x%x\n", rERASE_ADDRESS, data);
		data = Inp32(rCOMMAND1);
		qspitest("0x%x: 0x%x\n", rCOMMAND1, data);
		data = Inp32(rCOMMAND2);
		qspitest("0x%x: 0x%x\n", rCOMMAND2, data);
		data = Inp32(rCOMMAND3);
		qspitest("0x%x: 0x%x\n", rCOMMAND3, data);
		data = Inp32(rCOMMAND4);
		qspitest("0x%x: 0x%x\n", rCOMMAND4, data);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		data = Inp32(rRDID);
		qspitest("0x%x: 0x%x\n", rRDID, data);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rRDSR, data);

		break;
	case QSPI_READ_ID:
		QSPI_Read_ID(&vendor_id);
		qspitest("vendor_id 0x%x\n", vendor_id);
		break;

	case QSPI_FEATURE_TEST:
		qspitest("Disable Write\n");
		QSPI_Set_WriteMode(QSPI_WRDI, 0x04);
		qspitest("Enable Write\n");
		QSPI_Set_WriteMode(QSPI_WREN, 0x06);
		qspitest("Reset Write Status Register\n");
		QSPI_Set_WriteMode(QSPI_WRSR, 0x01);

		qspitest("Write\n");
		qspi_Wtest();

		qspitest("Normal Read\n");
		QSPI_Set_ReadMode(QSPI_SINGLE_IO, 0x03);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		data = Inp32(rFLASH_IO_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_IO_MODE, data);
		data = Inp32(rFLASH_PERF_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_PERF_MODE, data);
		qspi_Rtest();

		qspitest("FAST READ\n");
		QSPI_Set_ReadMode(QSPI_SINGLE_IO, 0x0B);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		SetBits(rFLASH_IO_MODE, 0, 0xF, 0x1);
		data = Inp32(rFLASH_IO_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_IO_MODE, data);
		data = Inp32(rFLASH_PERF_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_PERF_MODE, data);
		qspi_Rtest();

		qspitest("X2 FAST READ\n");
		QSPI_Set_ReadMode(QSPI_DUAL_FAST, 0x3B);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		data = Inp32(rFLASH_IO_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_IO_MODE, data);
		data = Inp32(rFLASH_PERF_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_PERF_MODE, data);
		qspi_Rtest();

		qspitest("X4 FAST READ\n");
		QSPI_Set_ReadMode(QSPI_QUAD_FAST, 0x6B);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		data = Inp32(rFLASH_IO_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_IO_MODE, data);
		data = Inp32(rFLASH_PERF_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_PERF_MODE, data);
		qspi_Rtest();

		qspitest("X2 IO FAST READ\n");
		QSPI_Set_ReadMode(QSPI_DUAL_IO, 0xBB);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		data = Inp32(rFLASH_IO_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_IO_MODE, data);
		data = Inp32(rFLASH_PERF_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_PERF_MODE, data);
		qspi_Rtest();

		qspitest("X4 IO FAST READ\n");
		QSPI_Set_ReadMode(QSPI_QUAD_IO, 0xEB);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		data = Inp32(rFLASH_IO_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_IO_MODE, data);
		data = Inp32(rFLASH_PERF_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_PERF_MODE, data);
		qspi_Rtest();

		break;

	case QSPI_SECURE_TEST:
		qspitest("\nSecure Write\n");

		for (i = 0; i < 30; i++) {
			qspi_Secure_Wtest();
		}

		qspitest("Normal Read\n");
		QSPI_Set_ReadMode(QSPI_SINGLE_IO, 0x03);
		data = Inp32(rCOMMAND5);
		qspitest("0x%x: 0x%x\n", rCOMMAND5, data);
		data = Inp32(rFLASH_IO_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_IO_MODE, data);
		data = Inp32(rFLASH_PERF_MODE);
		qspitest("0x%x: 0x%x\n", rFLASH_PERF_MODE, data);
		qspi_Secure_Rtest();

		break;

	case QSPI_ERASE_TEST:
		SetBits(rSF_CON, 31, 0x1U, 1U);
		SetBits(rSF_CON, 15, 0x1, 1);

		qspi_Etest();
		/*		QSPI_Chip_Erase();   */

		break;

	case QSPI_RESET_WL_CTR:

		g_base_address = QSPI_SECURE_AREA_START_ADDRESS_A;
		QSPI_Secure_Write_NoWL(2 * QSPI_SECURE_AREA_SIZE, (u32)tempBuffer, DATA_SIZE);

		break;

	default:
		break;
	}

	return 0;
}

void QSPI_print_mode(void)
{
#ifdef CONFIG_DEBUG
	int data;
	bool bwp = (Inp32(rSF_CON) & (0x1 << 31)) >> 31;
	data = Inp32(rFLASH_IO_MODE);
	data &= 0xf;

	switch (data) {
	case 0x4:
		lldbg("FLASH IO MODE [QUAD_FAST_READ], WP[%s]\n", (bwp) ? "disable" : "enable");
		break;

	case 0x2:
		lldbg("FLASH IO MODE [DUAL_FAST_READ], WP[%s]\n", (bwp) ? "disable" : "enable");
		break;

	case 0x1:
		lldbg("FLASH IO MODE [FAST_READ], WP[%s]\n", (bwp) ? "disable" : "enable");
		break;

	case 0x0:
		lldbg("FLASH IO MODE [NORMAL_READ], WP[%s]\n", (bwp) ? "disable" : "enable");
		break;
	default:
		lldbg("FLASH IO MODE [UNKNOWN]\n");
		break;
	}
#endif
}

/**
 *
 * @brief               Register the QSPI devices file
 * @param               void
 * @return              void
 * @note
 */
void qspi_register(void)
{
	register_blockdriver("/dev/nor", (FAR const struct block_operations *)&drv_fops, 0666, NULL);
}
