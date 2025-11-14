/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file its_flash_nand.h
 *
 * \brief Implementations of the flash interface functions for a NAND flash
 *        device. See its_flash_fs_ops_t for full documentation of functions.
 */

#ifndef __ITS_FLASH_NAND_H__
#define __ITS_FLASH_NAND_H__

#include <stddef.h>
#include <stdint.h>

#include "driver/Driver_Flash.h"

#ifdef __cplusplus
extern "C" {
#endif

struct its_flash_nand_dev_t {
    ARM_DRIVER_FLASH *driver;
    /* Two write buffers are reserved as the metadata block and the file block
     * write can be mixed in the file system operation.
     */
    uint32_t buf_block_id_0;
    uint32_t buf_block_id_1;
    uint8_t *write_buf_0;
    uint8_t *write_buf_1;
    size_t buf_size;
};

extern const struct its_flash_fs_ops_t its_flash_fs_ops_nand;

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_NAND_H__ */
