// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

/**
 * This macro defines the flash sector size.
 */
#define HAL_FLASH_SECTOR_SIZE (4096)

HAL_API hal_ret_t hal_flash_init(void);
HAL_API void hal_flash_cleanup(void);

HAL_API hal_ret_t hal_flash_read(hal_addr_t addr, uint8_t *data, size_t size);
HAL_API hal_ret_t hal_flash_write(hal_addr_t addr,
                                  const uint8_t *data,
                                  size_t size);
HAL_API hal_ret_t hal_flash_raw_write(hal_addr_t addr,
                                      const uint8_t *data,
                                      size_t size);
HAL_API hal_ret_t hal_flash_sector_write(hal_addr_t sector_addr,
                                         const uint8_t *data,
                                         size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_FLASH_H__ */