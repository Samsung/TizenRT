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

#ifndef __ARCH_ARM_INCLUDE_S5J_S5J_EFUSE_H
#define __ARCH_ARM_INCLUDE_S5J_S5J_EFUSE_H

#include <arch/chip/s5j_types.h>

#define EFUSE_BASE_ADDR 0x80000000

#define rEFUSE_PRODUCT_ID         EFUSE_BASE_ADDR + 0x0
#define rEFUSE_SFR_APB            EFUSE_BASE_ADDR + 0x100
#define rEFUSE_CHIPID_0           EFUSE_BASE_ADDR + 0x204
#define rEFUSE_CHIPID_1           EFUSE_BASE_ADDR + 0x208
#define rEFUSE_CHIPID_2           EFUSE_BASE_ADDR + 0x20C
#define rEFUSE_CHIPID_3           EFUSE_BASE_ADDR + 0x210
#define rEFUSE_PRODUCT_VER        EFUSE_BASE_ADDR + 0x220
#define rEFUSE_N_SECU_RD_DATA0    EFUSE_BASE_ADDR + 0x318
#define rEFUSE_N_SECU_RD_DATA1    EFUSE_BASE_ADDR + 0x31C
#define rEFUSE_N_SECU_RD_DATA2    EFUSE_BASE_ADDR + 0x320
#define rEFUSE_N_SECU_RD_DATA3    EFUSE_BASE_ADDR + 0x324
#define rEFUSE_CONTROL            EFUSE_BASE_ADDR + 0x328
#define rEFUSE_CONFIG             EFUSE_BASE_ADDR + 0x32C
#define rEFUSE_PROG_DATA0         EFUSE_BASE_ADDR + 0x334
#define rEFUSE_PROG_DATA1         EFUSE_BASE_ADDR + 0x338
#define rEFUSE_PROG_DATA2         EFUSE_BASE_ADDR + 0x33C
#define rEFUSE_PROG_DATA3         EFUSE_BASE_ADDR + 0x340
#define rEFUSE_PROG_DEBUG         EFUSE_BASE_ADDR + 0x370
#define rEFUSE_SENSING_DEBUG      EFUSE_BASE_ADDR + 0x374
#define rEFUSE_INT                EFUSE_BASE_ADDR + 0x3FC
#define rEFUSE_INT_STATUS         EFUSE_BASE_ADDR + 0x400
#define rEFUSE_INT_EN             EFUSE_BASE_ADDR + 0x404
#define rEFUSE_TIME_PARA0         EFUSE_BASE_ADDR + 0x408
#define rEFUSE_TIME_PARA1         EFUSE_BASE_ADDR + 0x40C
#define rEFUSE_STATUS             EFUSE_BASE_ADDR + 0x428
#define rEFUSE_SECU_SPARE0        EFUSE_BASE_ADDR + 0x42C
#define rEFUSE_N_SECU_SPARE0      EFUSE_BASE_ADDR + 0x434
#define rEFUSE_SECKEY0            EFUSE_BASE_ADDR + 0x500
#define rEFUSE_SECKEY1            EFUSE_BASE_ADDR + 0x504
#define rEFUSE_SECKEY2            EFUSE_BASE_ADDR + 0x508
#define rEFUSE_SECKEY3            EFUSE_BASE_ADDR + 0x50C
#define rEFUSE_SECKEY4            EFUSE_BASE_ADDR + 0x510
#define rEFUSE_SECKEY5            EFUSE_BASE_ADDR + 0x514
#define rEFUSE_SECKEY6            EFUSE_BASE_ADDR + 0x518
#define rEFUSE_SECKEY7            EFUSE_BASE_ADDR + 0x51C

//Addr
#define FSOURCE0_BIRA_MCU_KEY_ADDR            0
#define FSOURCE0_BIRA_WLBT_ADDR               0x80
#define FSOURCE0_CHIPID_ADDR                  0x100
#define FSOURCE0_AP_HW_OPTION_ADDR            0x180
#define FSOURCE0_SSS_CM0_ROLLBACK_CNT_ADDR    0x200
#define FSOURCE0_SSS_ROOT_ENC_KEY_ADDR        0x400
#define FSOURCE0_SSS_BACK_ENC_KEY_ADDR        0x500
#define FSOURCE0_SSS_CM0_SECURE_BOOT_KEY_ADDR 0x600
#define FSOURCE0_SSS_PRIVATE_KEY_ADDR         0x800
#define FSOURCE0_SECURE_KEY_ADDR              0xC00
#define FSOURCE0_SW_RW_RESION_ADDR            0xD00
#define FSOURCE1_SW_RW_RESION_ADDR            0x2000

//Size
#define FSOURCE0_BIRA_MCU_KEY_SIZE            0x80
#define FSOURCE0_BIRA_WLBT_SIZE               0x80
#define FSOURCE0_CHIPID_SIZE                  0x80
#define FSOURCE0_AP_HW_OPTION_SIZE            0x80
#define FSOURCE0_SSS_CM0_ROLLBACK_CNT_SIZE    0x20
#define FSOURCE0_SSS_ROOT_ENC_KEY_SIZE        0x100
#define FSOURCE0_SSS_BACK_ENC_KEY_SIZE        0x100
#define FSOURCE0_SSS_CM0_SECURE_BOOT_KEY_SIZE 0x200
#define FSOURCE0_SSS_PRIVATE_KEY_SIZE         0x200
#define FSOURCE0_SECURE_KEY_SIZE              0x100
#define FSOURCE0_SW_RW_RESION_SIZE            0x400
#define FSOURCE1_SW_RW_RESION_SIZE            0x1000

struct bank_info_s {
	u8  Bnak_name[30];
	u32 StartAddr;
	u32 bank_size;
	u32 Fusing_owner;
	u32 SW_read_enable;
	u32 HW_output_enable;
};
typedef struct bank_info_s BANK_INFO;

enum {
	BIRA_MCU_KEY = 0,
	BIRA_WLBT_KEY,
	CHIPID,
	AP_HW_OPTION,
	SSS_CM0_ROLLBACK_CNT,
	SSS_ROOT_ENC_KEY,
	SSS_BACK_ENC_KEY,
	SSS_CM0_SECURE_BOOT_KEY,
	SSS_PRIVATE_KEY,
	SECURE_KEY,
	SW_RW_RESION_0,
	SW_RW_RESION_1,
	BANK_MAX
} BANK;

enum {
	EDS = 0,
	CUSTOMER,
} FUSE_OWNER;

enum {
	SW_READ = 0,
	SW_READ_DIRECT,
	NO_SW_READ,
} SW_READ_EN;

enum {
	HW_OUTPUT = 0,
	NO_HW_OUTPUT,
} HW_OUTPUT_EN;

enum {
	PGM_VERIFY_DONE_INT = 0,
	PGM_VERIFY_FAIL_INT,
	PGM_DONE_INT,
	READ_DONE_INT
} INT_EN;

enum {
	PGM_VERIFY_DONE = 0,
	PGM_VERIFY_FAIL,
	PGM_DONE,
	READ_DONE
} INT_STATUS;

enum {
	SW_RESET = 0,
	PGM_VERIFY,
	PGM,
	READ
};

enum {
	EFUSE_RCS = 0,
	EFUSE_READ_SIZE,
	EFUSE_PGM_SIZE,
	EFUSE_SW_PRE_READING,
	EFUSE_ADDRESS
};

#endif							/* __ARCH_ARM_INCLUDE_S5J_S5J_EFUSE_H */
