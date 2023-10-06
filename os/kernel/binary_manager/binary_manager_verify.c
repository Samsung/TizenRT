/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <fcntl.h>
#include <crc32.h>
#include <stdlib.h>
#include <errno.h>

#ifdef CONFIG_BINARY_SIGNING
#include <tinyara/signature.h>
#endif
#include <tinyara/mm/mm.h>
#include <tinyara/binary_manager.h>

#include "binary_manager/binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
/* The buffer size for checking crc of common library */
#define CMNLIB_CRC_BUFSIZE    512000

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*********************************************************************************************
 * Name: binary_manager_verify_header_data
 *
 * Description:
 *  This function verifies header value according to type.
 *
 *********************************************************************************************/
static int binary_manager_verify_header_data(int type, void *header_input)
{
	/* Verify header data */
	if (type == BINARY_KERNEL) {
		kernel_binary_header_t *header_data = (kernel_binary_header_t *)header_input;
		if (header_data->header_size != sizeof(kernel_binary_header_t) - CHECKSUM_SIZE || header_data->binary_size == 0 \
		|| header_data->version < KERNEL_BIN_VER_MIN || header_data->version > KERNEL_BIN_VER_MAX) {
			bmdbg("Invalid kernel header data : headersize %u, version %u, binary size %u\n", header_data->header_size, header_data->version, header_data->binary_size);
			return ERROR;
		}
		bmvdbg("Kernel binary header : %u %u %u %u \n", header_data->header_size, header_data->binary_size, header_data->version, header_data->secure_header_size);
	} else if (type == BINARY_USERAPP) {
		user_binary_header_t *header_data = (user_binary_header_t *)header_input;
		if (header_data->bin_type != BIN_TYPE_ELF || header_data->bin_ver == 0 \
		|| header_data->loading_priority == 0 || header_data->loading_priority >= BINARY_LOADPRIO_MAX \
		|| header_data->bin_ramsize == 0 || header_data->bin_size == 0) {
			bmdbg("Invalid user header data : headersize %u, binsize %u, ramsize %u, bintype %u\n", header_data->header_size, header_data->bin_size, header_data->bin_ramsize, header_data->bin_type);
			return ERROR;
		}
		bmvdbg("User binary header : %u %u %u %u %s %u %u %u\n", header_data->header_size, header_data->bin_type, header_data->bin_size, header_data->loading_priority, header_data->bin_name, header_data->bin_ver, header_data->bin_ramsize, header_data->kernel_ver);
	} else {
		/* Verify header data */
		common_binary_header_t *header_data = (common_binary_header_t *)header_input;
		if (header_data->header_size == 0 || header_data->bin_size == 0 ||\
			header_data->version < BM_VERSION_DATE_MIN || header_data->version > BM_VERSION_DATE_MAX) {
			bmdbg("Invalid common header data : headersize %u, binsize %u, version %u\n", header_data->header_size, header_data->bin_size, header_data->version);
			return ERROR;
		}
		bmvdbg("Common binary header : headersize %u, binsize %u, version %u\n", header_data->header_size, header_data->bin_size, header_data->version);
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/*****************************************************************************
 * Name: binary_manager_read_header
 *
 * Description:
 *  This function reads and verifies header value with CRC checking according to type.
 *
 *****************************************************************************/
int binary_manager_read_header(int type, char *devpath, void *header_data, bool crc_check)
{
	int fd;
	int ret;
	uint32_t read_size;
	uint32_t bin_size;
	uint32_t calculate_crc = 0;
	uint8_t *crc_buffer;
	uint32_t crc_hash;
	uint32_t crc_bufsize;
	int header_size = 0;

	if (type < BINARY_KERNEL || type >= BINARY_TYPE_MAX || !header_data) {
		bmdbg("Invalid parameter, type %d\n", type);
		return BINMGR_INVALID_PARAM;
	}
	
	if (type == BINARY_KERNEL) {
		header_size = sizeof(kernel_binary_header_t);
	} else if (type == BINARY_USERAPP) {
		header_size = sizeof(user_binary_header_t);
	} else if (type == BINARY_COMMON) {
		header_size = sizeof(common_binary_header_t);
	}

	memset(header_data, 0, header_size);
	crc_buffer = NULL;

	fd = open(devpath, O_RDONLY);
	if (fd < 0) {
		bmdbg("Fail to open %s: errno %d\n", devpath, errno);
		return BINMGR_OPERATION_FAIL;
	}

#ifdef CONFIG_BINARY_SIGNING
	if (type != BINARY_KERNEL) {
		ret = lseek(fd, USER_SIGN_PREPEND_SIZE, SEEK_SET);
		if (ret < 0) {
			bmdbg("Fail to set offset to skip signing header, errno : %d\n", errno);
			ret = BINMGR_OPERATION_FAIL;
			goto errout_with_fd;
		}
	}
#endif

	/* Read the binary header */
	ret = read(fd, (FAR uint8_t *)header_data, header_size);
	if (ret != header_size) {
		bmdbg("Fail to read %s, ret %d, errno %d\n", devpath, ret, errno);
		ret = BINMGR_OPERATION_FAIL;
		goto errout_with_fd;
	}

	/* Verify header data */
	ret = binary_manager_verify_header_data(type, header_data);
	if (ret < 0) {
		ret = BINMGR_NOT_FOUND;
		goto errout_with_fd;
	}

	if (crc_check) {
		if (type == BINARY_KERNEL) {
			crc_bufsize = ((kernel_binary_header_t *)header_data)->binary_size;
			bin_size = ((kernel_binary_header_t *)header_data)->binary_size;
			crc_hash = ((kernel_binary_header_t *)header_data)->crc_hash;
		} else if (type == BINARY_USERAPP) {
			crc_bufsize = ((user_binary_header_t *)header_data)->bin_ramsize;
			bin_size = ((user_binary_header_t *)header_data)->bin_size;
			crc_hash = ((user_binary_header_t *)header_data)->crc_hash;
		} else if (type == BINARY_COMMON) {
			crc_bufsize = CMNLIB_CRC_BUFSIZE;
			bin_size = ((common_binary_header_t *)header_data)->bin_size;
			crc_hash = ((common_binary_header_t *)header_data)->crc_hash;
		}
		size_t max_bufsize = kmm_get_largest_freenode_size() / 2;
		crc_bufsize = crc_bufsize < max_bufsize ? crc_bufsize : max_bufsize;
		crc_buffer = (uint8_t *)kmm_malloc(crc_bufsize);
		if (!crc_buffer) {
			bmdbg("Fail to malloc buffer for checking crc, size %u\n", crc_bufsize);
			ret = BINMGR_OUT_OF_MEMORY;
			goto errout_with_fd;
		}
		/* Calculate checksum and Verify it */
		calculate_crc = crc32part((uint8_t *)header_data + CHECKSUM_SIZE, header_size - CHECKSUM_SIZE, calculate_crc);
		while (bin_size > 0) {
			read_size = bin_size < crc_bufsize ? bin_size : crc_bufsize;
			ret = read(fd, (void *)crc_buffer, read_size);
			if (ret != read_size) {
				bmdbg("Fail to read header for checksum : ret %d, errno %d\n", ret, errno);
				ret = BINMGR_OPERATION_FAIL;
				goto errout_with_fd;
			}
			calculate_crc = crc32part(crc_buffer, read_size, calculate_crc);
			bin_size -= read_size;
		}

		if (calculate_crc != crc_hash) {
			bmdbg("Fail to crc check : %u != %u\n", calculate_crc, crc_hash);
			ret = BINMGR_NOT_FOUND;
			goto errout_with_fd;
		}
		kmm_free(crc_buffer);
	}
	close(fd);

	return BINMGR_OK;

errout_with_fd:
	close(fd);
	if (crc_buffer) {
		kmm_free(crc_buffer);
	}
	return ret;
}
