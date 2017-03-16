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
 * arch/arm/src/s5j/s5j_qspi.h
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

#ifndef __ARCH_ARM_SRC_S5J_S5J_QSPI_H
#define __ARCH_ARM_SRC_S5J_S5J_QSPI_H

#define QSPI_DUMMY_DATA     0x1

#define QSPI_SECTOR_OFFSET       (12)
#define QSPI_BLOCK32K_OFFSET     (15)
#define QSPI_BLOCK64K_OFFSET     (16)

#define QSPI_SIZE_64KB   (1<<QSPI_BLOCK64K_OFFSET)
#define QSPI_SIZE_32KB   (1<<QSPI_BLOCK32K_OFFSET)
#define QSPI_SIZE_4KB    (1<<QSPI_SECTOR_OFFSET)

#define COMMAND_ERASE_32KB  (0x52)
#define COMMAND_ERASE_64KB  (0xD8)

#define rSF_CON                                     (SFI_BASE + 0x0004)
#define rERASE_ADDRESS                              (SFI_BASE + 0x0010)
#define rCOMMAND1                                   (SFI_BASE + 0x001C)
#define rCOMMAND2                                   (SFI_BASE + 0x0020)
#define rCOMMAND3                                   (SFI_BASE + 0x0024)
#define rCOMMAND4                                   (SFI_BASE + 0x0028)
#define rCOMMAND5                                   (SFI_BASE + 0x002C)
#define rADDR_CMD                                   (SFI_BASE + 0x0059)
#define rSE                                         (SFI_BASE + 0x005E)
#define rFLASH_IO_MODE                              (SFI_BASE + 0x0074)
#define rFLASH_PERF_MODE                            (SFI_BASE + 0x0078)
#define rRDID                                       (SFI_BASE + 0x00AC)
#define rBE                                         (SFI_BASE + 0x00BE)
#define rCE                                         (SFI_BASE + 0x00CE)
#define rRDSR                                       (SFI_BASE + 0x00DC)
#define rWRDI                                       (SFI_BASE + 0x00DD)
#define rWRSR                                       (SFI_BASE + 0x00DE)
#define rWREN                                       (SFI_BASE + 0x00EE)

typedef enum {
	TYPE_ERR = 0,
	TYPE_4KB = 1,
	TYPE_32KB = 2,
	TYPE_64KB = 3,
} eERASE_UNIT;

typedef enum {
	QSPI_READ_INFO,
	QSPI_READ_ID,
	QSPI_FEATURE_TEST,
	QSPI_SECURE_TEST,
	QSPI_ERASE_TEST,
	QSPI_RESET_WL_CTR,
} eQSPI_IOCTL_CMD;

typedef enum {
	BLOCK_32KB,
	BLOCK_64KB,
} eQSPI_BLOCK_SIZE;

typedef enum {
	QSPI_SINGLE_IO,
	QSPI_DUAL_FAST,
	QSPI_DUAL_IO,
	QSPI_QUAD_FAST,
	QSPI_QUAD_IO,
} eQSPI_MODE;

typedef enum {
	QSPI_WREN,
	QSPI_WRDI,
	QSPI_WRSR,
} eQSPI_WR_MODE;

typedef enum {
	QSPI_VENDOR_SST = 0x1,
	QSPI_VENDOR_STMICRO = 0x2,
	QSPI_VENDOR_AMD = 0x4,
	QSPI_VENDOR_ATMEL = 0x8,
	QSPI_VENDOR_MACRONIX = 0x10,
	QSPI_VENDOR_WINBOND = 0x20,
} eQSPI_VENDOR;

typedef enum {
	QSPI_PAGE_4BYTES = 0x0,
	QSPI_PAGE_8BYTES = 0x1,
	QSPI_PAGE_16BYTES = 0x2,
	QSPI_PAGE_32BYTES = 0x3,
	QSPI_PAGE_64BYTES = 0x4,
	QSPI_PAGE_128BYTES = 0x5,
	QSPI_PAGE_256BYTES = 0x6,
} eQSPI_PAGE_SIZE;

typedef struct _status_register_t {
	u8 rdsr;
	struct {
		unsigned WIP: 1;
		unsigned WEL: 1;
		unsigned BP0: 1;
		unsigned BP1: 1;
		unsigned BP2: 1;
		unsigned BP3: 1;
		unsigned QE: 1;			//Quad enable
		unsigned SRWD: 1;		//Status Register Write Disable
	} b;
} flash_status_register;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void s5j_qspi_copy_4byte(unsigned int target_addr, unsigned int source_addr, unsigned int sizebyte);
static void s5j_qspi_copy_1byte(unsigned int target_addr, unsigned int source_addr, unsigned int sizebyte);
static void s5j_qspi_set_gpio(void);
static flash_status_register s5j_qspi_get_status_register(void);
eERASE_UNIT s5j_qspi_get_eraseunit(unsigned int offset_start, unsigned int target);
static void s5j_qspi_sector_erase(unsigned int target_addr);
static void s5j_qspi_block_erase(unsigned int target_addr, eQSPI_BLOCK_SIZE unit);
static void s5j_qspi_chip_erase(void);

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void s5j_qspi_disable_wp(void);
void s5j_qspi_enable_wp(void);
void s5j_qspi_take_sem(void);
void s5j_qspi_release_sem(void);
bool s5j_qspi_erase(unsigned int target_addr, unsigned int size);

#endif							/* __ARCH_ARM_SRC_S5J_S5J_QSPI_H */
