// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <driver/mpc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the MPC driver
 *
 * This API init the resoure common:
 *   - Init MPC driver control memory
 *
 * @attention 1. This API should be called before any other MPC APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_mpc_driver_init(void);

/**
 * @brief     Deinit the MPC driver
 *
 * This API free all resource related to MPC.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_mpc_driver_deinit(void);

/**
 * @brief     Get the block size, unit is byte
 *
 * @param dev the MPC device
 *
 * @return the block size
 */
uint32_t bk_mpc_get_block_size(mpc_dev_t dev);

/**
 * @brief     Get the block index max value
 *
 * @param dev the MPC device
 *
 * @return the max block index
 */
uint32_t bk_mpc_get_max_block_index(mpc_dev_t dev);

/**
 * @brief     Set a MPC secure attribute configuration on the memory passed as parameter for a number of blocks
 *
 * @param dev the MPC device
 * @param mem_addr_offset start memory address offset to configure
 *        (must be block size aligned)
 * @param block_num number of blocks to configure
 *        (block size is bk_mpc_get_block_size(dev))
 * @param secure_type the MPC secure type, with each element must be MPC_BLOCK_SECURE or MPC_BLOCK_NON_SECURE
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 *    - BK_ERR_MPC_BLOCK_INDEX_OUT_OF_RANGE: MPC block index is out of range
 *    - BK_ERR_MPC_INVALID_LUT_PARAM: MPC invalid lut parameter
 *    - BK_ERR_MPC_INVALID_DEV: MPC device id is invalid
 */
bk_err_t bk_mpc_set_secure_attribute(mpc_dev_t dev, uint32_t mem_addr_offset, uint32_t block_num, mpc_block_secure_type_t secure_type);

/**
 * @brief     Lockdown MPC. After locking, only read operations are allowed
 *
 * @attention Once we call this API, MPC will be unlocked until the next reset
 *
 * @param dev the MPC device
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 *    - BK_ERR_MPC_INVALID_DEV: MPC device id is invalid
 */
bk_err_t bk_mpc_lockdown(mpc_dev_t dev);

/**
 * @brief     Enable MPC secure exception.
 *
 * @attention When secure exception is enabled, access without permission will trigger bus fault.
 *            Otherwise just write/read ignore.
 *
 * @param dev the MPC device
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 *    - BK_ERR_MPC_INVALID_DEV: MPC device id is invalid
 */
bk_err_t bk_mpc_enable_secure_exception(mpc_dev_t dev);

/**
 * @brief     Disable MPC secure exception.
 *
 * @attention When secure exception is enabled, access without permission will trigger bus fault.
 *            Otherwise just write/read ignore.
 *
 * @param dev the MPC device
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 *    - BK_ERR_MPC_INVALID_DEV: MPC device id is invalid
 */
bk_err_t bk_mpc_disable_secure_exception(mpc_dev_t dev);

/**
 * @brief     Dump secure attributes of specified MPC device
 *
 * @param dev the MPC device
 *
 * @return
 *    - BK_OK: succeed
 */
bk_err_t bk_mpc_dump_secure_attribute(mpc_dev_t dev);
#ifdef __cplusplus
}
#endif

