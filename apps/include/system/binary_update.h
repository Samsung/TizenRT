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

/***************************************************************************
 * Included Files
 ***************************************************************************/

#include <tinyara/binary_manager.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/**
 * @brief The structure of binary information
 */
struct binary_info_s {
	int part_size;
	char name[BIN_NAME_MAX];
	char version[BIN_VERSION_MAX];
	char dev_path[BINMGR_DEVNAME_LEN];
};
typedef struct binary_info_s binary_info_t;

/**
 * @brief The structure of binaries' information list
 */
struct binary_info_list_s {
	binary_info_t binary_info;
	struct binary_info_list_s* next;
};
typedef struct binary_info_list_s binary_info_list_t;


/**
 * @brief Loads the binary after downloading a new binary.
 * @details @b #include <apps/system/binary_update.h>\n
 *  This function loads a new binary after downloading the binary.\n
 * It requests the binary manager to load the input binary name.
 * @param[in] binary_name The name of a new binary to be loaded
 * @return 0 (OK) On success. -1 (ERROR) is returned on failure.
 * @since TizenRT v3.0
 */
int load_new_binary(char *binary_name);

/**
 * @brief Get the binary information.
 * @details @b #include <apps/system/binary_update.h>\n
 *  It requests the binary manager to get the binary information through input binary name.
 * @param[in] binary_name The binary name which to get
 * @param[out] binary_info The address value to receive the binary information
 * @return 0 (OK) On success. -1 (ERROR) is returned on failure.
 * @since TizenRT v3.0
 */
int get_binary_info(char *binary_name, binary_info_t *binary_info);

/**
 * @brief Get the all binaries information.
 * @details @b #include <apps/system/binary_update.h>\n
 *  It requests the binary manager to get the all of binaries' information.
 * @param[out] binary_info_list The address value to receive the binary information list
 * @return 0 (OK) On success. -1 (ERROR) is returned on failure.
 * @since TizenRT v3.0
 */
int get_binary_info_all(binary_info_list_t *binary_info_list);

/**
 * @brief Clean the binary_info_list
 * @details @b #include <apps/system/binary_update.h>\n
 *  This function recursively clears all information in the list.
 * @param[out] binary_info_list The address value to clean
 * @return None
 * @since TizenRT v3.0
 */
void clean_binary_info_list(binary_info_list_t *binary_info_list);
