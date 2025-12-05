/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __SERVICE_API_H__
#define __SERVICE_API_H__

#include <stdint.h>
#include "tfm_boot_status.h"

/**
 * \brief Retrieve secure partition related data from shared memory area, which
 *        stores shared data between bootloader and runtime firmware.
 *
 * \param[in]  major_type  Major type.
 * \param[out] boot_data   Pointer to boot data.
 * \param[in]  len         The length of the boot data.
 */
int32_t tfm_core_get_boot_data(uint8_t major_type,
                               struct tfm_boot_data *boot_data,
                               uint32_t len);

#endif /* __SERVICE_API_H__ */
