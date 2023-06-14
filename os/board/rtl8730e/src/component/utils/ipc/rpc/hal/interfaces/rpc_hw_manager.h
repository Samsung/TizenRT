/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @addtogroup HAL
 * @{
 *
 * @brief Defines the structures and interfaces for the Hardware Abstraction Layer (HAL) module.
 *
 * @since 1.0
 * @version 1.0
 *
 */

/**
 * @file rpc_hw_manager.h
 *
 * @brief Declares the driver interfaces for managing RPC .
 *
 * @since 1.0
 * @version 1.0
 *
 */

#ifndef AMEBA_HARDWARE_INTERFACES_HARDWARE_RPC_RPC_HW_MANAGER_H
#define AMEBA_HARDWARE_INTERFACES_HARDWARE_RPC_RPC_HW_MANAGER_H

#include <stdint.h>

/**
  * @brief RPC Error Code Enumeration
  */
typedef enum {
	RPC_SUCCESS         =  0,
	RPC_ERROR           = -1,
	RPC_INVALID_CH      = -2,
	RPC_INVALID_SIZE    = -3,
	RPC_NO_MEMORY       = -4,
	RPC_NOT_INIT        = -5,
	RPC_TX_TIMEOUT      = -6,
	RPC_RX_TIMEOUT      = -7,
	RPC_TX_ERROR        = -8,
	RPC_RX_ERROR        = -9,
} rpc_error_t;


/**
 * @brief Provides interfaces for read and write data through RPC.
 */
struct RPCHwManager {
	/**
	 * @brief Write the data buffer.
	 *
	 * @param fd The write ipc queue id.
	 * @param opt The option for set BLOCK_MODE or others.
	 * @param buf The pointer of buffer for write.
	 * @param size The buffer size.
	 * @return Returns <b>0</b> if the buffer is write successfully;
	 *
	 * @since 1.0
	 * @version 1.0
	 */
	int32_t (*WriteBuffer)(int32_t id, int32_t opt, uint8_t *buf, int size);

	/**
	 * @brief Read the data buffer.
	 *
	 * @param fd The read ipc queue id.
	 * @param opt The option for set BLOCK_MODE or others.
	 * @param buf The pointer of buffer for read.
	 * @param size The buffer size.
	 * @return Returns <b>0</b> if the buffer is read successfully;
	 *
	 * @since 1.0
	 * @version 1.0
	 */
	int32_t (*ReadBuffer)(int32_t id, int32_t opt, uint8_t *buf, int size);
};

typedef struct RPCHwManager RPCHwManager;

/**
 * @brief Create the rpc hardware manager.
 *
 * @return Returns the pointer to the <b>RPCHwManager</b> object if the list is obtained; returns <b>NULL</b> otherwise.
 *
 * @since 1.0
 * @version 1.0
 */
struct RPCHwManager *GetRPCHwManager(void);

/**
 * @brief Destory the rpc hardware manager.
 *
 * @param manager Indicates the pointer to the rpc hardware manager.
 * @return Returns <b>0</b> if destroy successfully;
 *
 * @since 1.0
 * @version 1.0
 */
void DestoryRPCHwManager(struct RPCHwManager *manager);

#endif // AMEBA_HARDWARE_INTERFACES_HARDWARE_RPC_RPC_HW_MANAGER_H
/** @} */
