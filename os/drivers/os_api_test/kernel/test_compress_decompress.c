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
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>

#include <tinyara/os_api_test_drv.h>
#include <tinyara/binfmt/compression/compress_read.h>
#include <tinyara/kmalloc.h>
#include <tinyara/common_logs/common_logs.h>

#define READSIZE 2048

/****************************************************************************
 * Private Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function
 ****************************************************************************/

static int test_compress_decompress_function(unsigned long arg)
{
	int i;
	FILE *filefp;
	int filefd;
	int ret;
	off_t filelen;
	unsigned int size;
	size_t readsize = READSIZE;
	uint8_t *dst_buffer;
	struct s_header *compression_header;

	filefp = fopen("/mnt/myfile_comp", "r");
	filefd = fileno(filefp);

	if (filefd < 0) {
		int errval = get_errno();
		berr("%s %d\n", clog_message_str[CMN_LOG_FILE_OPEN_ERROR],errval);
		return -errval;
	}

	ret = compress_init(filefd, 0, &filelen);

	if (ret != OK) {
		berr("%s %d\n", clog_message_str[CMN_LOG_FILE_READ_ERROR],ret);
		return ret;
	}

	compression_header = get_compression_header();

	dst_buffer = (uint8_t *)kmm_malloc(READSIZE * sizeof(uint8_t));
	if (dst_buffer == NULL) {
		berr("%s\n", clog_message_str[CMN_LOG_ALLOC_FAIL]);
		compress_uninit();
		return ERROR;
	}


	for (i = 0; i < (compression_header->sections - 1); i++) {
		size = compress_read(filefd, 0, dst_buffer, readsize, i * READSIZE);
		if (size != READSIZE) {
			berr("Read for compressed block %d failed\n", i);
			compress_uninit();
			kmm_free(dst_buffer);
			return ERROR;
		}
	}

	compress_uninit();
	kmm_free(dst_buffer);

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_compress_decompress(int cmd, unsigned long arg)
{
	int ret = -EINVAL;

	switch (cmd) {
	case TESTIOC_COMPRESSION_TEST:
		ret = test_compress_decompress_function(arg);
		break;
	}
	return ret;
}
