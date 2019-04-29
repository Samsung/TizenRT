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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>

#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/* Get index in binary table with binary id */
int binary_manager_get_index_with_binid(int bin_id)
{
	int bin_idx;
	int bin_count;

	if (bin_id <= 0) {
		bmdbg("Invalid bin_id %d\n", bin_id);
		return ERROR;
	}

	bin_count = binary_manager_get_binary_count();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (BIN_ID(bin_idx) == bin_id) {
			bmvdbg("FIND binary %d idx = %d\n", bin_id, bin_idx);
			return bin_idx;
		}
	}

	return ERROR;
}

/* Get index in binary table with binary id */
int binary_manager_get_index_with_name(char *bin_name)
{
	int bin_idx;
	int bin_count;

	if (bin_name == NULL) {
		bmdbg("Invalid binary name, NULL\n");
		return ERROR;
	}

	bin_count = binary_manager_get_binary_count();

	for (bin_idx = 1; bin_idx <= bin_count; bin_idx++) {
		if (!strncmp(BIN_NAME(bin_idx), bin_name, strlen(bin_name) + 1)) {
			bmvdbg("FIND binary %s idx = %d\n", bin_name, bin_idx);
			return bin_idx;
		}
	}

	return ERROR;
}

/* Get binary info with binary name */
int binary_manager_get_info_with_name(char *bin_name)
{
	return OK;
}

/* Get info of all registered binaries */
int binary_manager_get_info_all(void)
{
	return OK;
}
