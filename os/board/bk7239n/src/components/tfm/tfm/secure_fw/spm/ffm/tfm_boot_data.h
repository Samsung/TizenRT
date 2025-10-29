/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_BOOT_DATA_H__
#define __TFM_BOOT_DATA_H__

#include <stdint.h>

/**
 * \brief Retrieve secure partition related data from shared memory area, which
 *        stores shared data between bootloader and runtime firmware.
 *
 * \param[in] args  Pointer to stack frame, which carries input parameters.
 */
void tfm_core_get_boot_data_handler(uint32_t args[]);

/**
 * \brief Validate the content of shared memory area, which stores the shared
 *        data between bootloader and runtime firmware.
 */
void tfm_core_validate_boot_data(void);

#endif /* __TFM_BOOT_DATA_H__ */
