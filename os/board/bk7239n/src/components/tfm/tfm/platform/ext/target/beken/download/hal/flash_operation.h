/**
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and
 * any derivative work shall retain this copyright notice.
 *
 */

#ifndef __FLASH_OPERATION_H__
#define __FLASH_OPERATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"
#include "hal_src_internal.h"

#define FLASH_RW_MAX_DATA_SIZE (256)
#define FLASH_SECTOR_SIZE (0x1000)
#define FLASH_ERASE_BLOCK32_SIZE (0x8000)
#define FLASH_ERASE_BLOCK_SIZE (0x10000)
#define FLASH_MAX_SIZE (0x1000000)

extern hal_ret_t flash_op_read(hal_addr_t addr, uint8_t *buffer_p, size_t len);
extern hal_ret_t flash_op_write(hal_addr_t addr,
                                const uint8_t *buffer_p,
                                size_t len);
extern hal_ret_t flash_op_erase(hal_addr_t addr, size_t len);
extern hal_ret_t flash_op_raw_write(hal_addr_t addr,
                                    const uint8_t *buffer_p,
                                    size_t len);
extern hal_ret_t flash_op_sector_write(hal_addr_t addr,
                                       const uint8_t *buffer_p,
                                       size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_OPERATION_H__ */
