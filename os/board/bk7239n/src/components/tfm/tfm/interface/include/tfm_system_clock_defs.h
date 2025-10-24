/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TFM_SYSTEM_CLOCK_DEFS_H
#define TFM_SYSTEM_CLOCK_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    TFM_SYSTEM_JTAG_CONNECT_CORE0 = (0u),
    TFM_SYSTEM_JTAG_CONNECT_CORE1,
    TFM_SYSTEM_ROM_BOOT,
    TFM_SYSTEM_FLASH_BOOT,
};

#ifdef __cplusplus
}
#endif

#endif /* TFM_SYSTEM_CLOCK_DEFS_H */
