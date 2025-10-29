/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ITS_FLASH_H__
#define __ITS_FLASH_H__

#include "its_utils.h"
#include "tfm_hal_its.h"
#include "tfm_hal_ps.h"

/* Include the correct flash interface implementation for ITS */
#ifdef ITS_RAM_FS
/* RAM FS: use a buffer to emulate storage in RAM */
#include "its_flash_ram.h"
extern uint8_t its_block_data[];
#define ITS_FLASH_DEV its_block_data
#define ITS_FLASH_ALIGNMENT TFM_HAL_ITS_PROGRAM_UNIT
#define ITS_FLASH_OPS its_flash_fs_ops_ram

#elif (TFM_HAL_ITS_PROGRAM_UNIT > 16)
/* NAND flash: each filesystem block is buffered and then programmed in one
 * shot, so no filesystem data alignment is required.
 */
#include "its_flash_nand.h"
extern struct its_flash_nand_dev_t its_flash_nand_dev;
#define ITS_FLASH_DEV its_flash_nand_dev
#define ITS_FLASH_ALIGNMENT 1
#define ITS_FLASH_OPS its_flash_fs_ops_nand

#else
/* NOR flash: no write buffering, require each file in the filesystem to be
 * aligned to the program unit.
 */
#include "its_flash_nor.h"
#define ITS_FLASH_DEV TFM_HAL_ITS_FLASH_DRIVER
#define ITS_FLASH_ALIGNMENT TFM_HAL_ITS_PROGRAM_UNIT
#define ITS_FLASH_OPS its_flash_fs_ops_nor
#endif

/* Include the correct flash interface implementation for PS */
#ifdef TFM_PARTITION_PROTECTED_STORAGE
#ifdef PS_RAM_FS
/* RAM FS: use a buffer to emulate storage in RAM */
#include "its_flash_ram.h"
extern uint8_t ps_block_data[];
#define PS_FLASH_DEV ps_block_data
#define PS_FLASH_ALIGNMENT TFM_HAL_PS_PROGRAM_UNIT
#define PS_FLASH_OPS its_flash_fs_ops_ram

#elif (TFM_HAL_PS_PROGRAM_UNIT > 16)
/* NAND flash: each filesystem block is buffered and then programmed in one
 * shot, so no filesystem data alignment is required.
 */
#include "its_flash_nand.h"
extern struct its_flash_nand_dev_t ps_flash_nand_dev;
#define PS_FLASH_DEV ps_flash_nand_dev
#define PS_FLASH_ALIGNMENT 1
#define PS_FLASH_OPS its_flash_fs_ops_nand

#else
/* NOR flash: no write buffering, require each file in the filesystem to be
 * aligned to the program unit.
 */
#include "its_flash_nor.h"
#define PS_FLASH_DEV TFM_HAL_PS_FLASH_DRIVER
#define PS_FLASH_ALIGNMENT TFM_HAL_PS_PROGRAM_UNIT
#define PS_FLASH_OPS its_flash_fs_ops_nor
#endif
#else /* TFM_PARTITION_PROTECTED_STORAGE */
#define PS_FLASH_ALIGNMENT 1
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

/**
 * \brief Provides a compile-time constant for the maximum program unit required
 *        by any flash device that can be accessed through this interface.
 */
#define ITS_FLASH_MAX_ALIGNMENT ITS_UTILS_MAX(ITS_FLASH_ALIGNMENT, \
                                              PS_FLASH_ALIGNMENT)

#endif /* __ITS_FLASH_H__ */
