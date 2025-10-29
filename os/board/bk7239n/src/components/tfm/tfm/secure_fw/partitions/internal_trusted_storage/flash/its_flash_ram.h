/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file its_flash_ram.h
 *
 * \brief Implementations of the flash interface functions for an emulated flash
 *        device using RAM. See its_flash_fs_ops_t for full documentation of
 *        functions.
 */

#ifndef __ITS_FLASH_RAM_H__
#define __ITS_FLASH_RAM_H__

#ifdef __cplusplus
extern "C" {
#endif

extern const struct its_flash_fs_ops_t its_flash_fs_ops_ram;

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_RAM_H__ */
