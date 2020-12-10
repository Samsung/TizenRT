/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/**
 * @defgroup Binary_Manager Binary_Manager
 * @ingroup Binary_Manager
 * @brief Provides APIs for Binary Manager
 * @{
 */

/**
 * @file binary_manager/binary_manager.h
 */
#ifndef __BINARY_MANAGER_H__
#define __BINARY_MANAGER_H__

/***************************************************************************
 * Included Files
 ***************************************************************************/
#include <tinyara/binary_manager.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_BINMGR_UPDATE
/**
 * @brief Loads the binary after downloading a new binary
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  This function loads a new binary after downloading the binary.\n
 * It requests the binary manager to load the input binary name.
 * @param[in] binary_name The name of a new binary to be loaded
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) On success. On failure, negative value is returned.
 * @since TizenRT v3.0
 */
binmgr_result_type_e binary_manager_update_binary(char *binary_name);

/**
 * @brief Get the binary information with name
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  It requests the binary manager to get the binary information through input binary name.
 * @param[in] binary_name The binary name which to get
 * @param[out] binary_info The address value to receive the binary information
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) On success. On failure, negative value is returned.
 * @since TizenRT v3.0
 */
binmgr_result_type_e binary_manager_get_update_info(char *binary_name, binary_update_info_t *binary_info);

/**
 * @brief Get the all binaries information
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  It requests the binary manager to get the all of binaries' information.
 * @param[out] binary_info_list The address value to receive the binary information list
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) On success. On failure, negative value is returned.
 * @since TizenRT v3.0
 */
binmgr_result_type_e binary_manager_get_update_info_all(binary_update_info_list_t *binary_info_list);
#endif

/**
 * @brief Notify that loaded binary is started to binary manager
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  It sends a message the binary manager to notify that loaded binary is started well.
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) On success. On failure, negative value is returned.
 * @since TizenRT v3.0
 */
binmgr_result_type_e binary_manager_notify_binary_started(void);

/**
 * @brief Set the callback which will be called when the states of binaries are changed
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  It sets callback for the changes of binary state.
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) On success. On failure, negative value is returned.
 * @since TizenRT v3.0
 */
binmgr_result_type_e binary_manager_register_state_changed_callback(binmgr_statecb_t handler, void *cb_data);

/**
 * @brief Clear the registered callback for the changes of binary state.
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  It clears registered callback for the changes of binary state.
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) On success. On failure, negative value is returned.
 * @since TizenRT v3.0
 */
binmgr_result_type_e binary_manager_unregister_state_changed_callback(void);

/**
 * @brief Get the path to download binary with version.
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  User can perform file operations with returned filepath : open, read, write, lseek, close.
 *  In case of kernel binary, it returns a path of inactive kernel partition.
 *  Otherwise, it creates empty file with version and returns a path of the file.
 * @param[in] binary_name The binary name to update
 * @param[in] version The binary version to update
 * @param[out] download_path The path to the download binary. The maximum length is BINARY_PATH_LEN.
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) on success. On failure, negative value is returned.
 * @since TizenRT v3.1 PRE
 */
binmgr_result_type_e binary_manager_get_download_path(char *binary_name, uint32_t version, char *download_path);

/**
 * @brief Get a state of binary
 * @details @b #include <binary_manager/binary_manager.h>\n
 *  It sends a message the binary manager to get a state of binary with binary name.
 * @param[in] binary_name The binary name to get state
 * @param[out] state The state of binary with binary_name
 * @return A defined value of binmgr_result_type_e in <tinyara/binary_manager.h>
 *         0 (BINMGR_OK) on success. On failure, negative value is returned.
 * @since TizenRT v3.1 PRE
 */
binmgr_result_type_e binary_manager_get_state(char *binary_name, int *state);

#endif
/**
 * @}
 */
