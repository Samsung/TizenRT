/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include "log_dump/log_dump.h"

#include <tinyara/log_dump/log_dump.h>
#include <tinyara/compression.h>
#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/
/****************************************************************************
 * Public Variables
 ****************************************************************************/
/****************************************************************************
 * Private Variables
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/

int log_dump_compress(char *out_buf, char *in_buf, long unsigned int in_buf_size)
{
	int compressed_size = CONFIG_LOG_DUMP_CHUNK_SIZE;
	int comp_ret;

	comp_ret = compress_block((unsigned char *)out_buf, &compressed_size, (unsigned char*)in_buf, in_buf_size);
	if (comp_ret != LOG_DUMP_OK) {
		compressed_size = LOG_DUMP_COMPRESS_FAIL;
	}

	return compressed_size;
}
