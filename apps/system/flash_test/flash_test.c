/****************************************************************************
 *
 * Copyright 2016, 2021 Samsung Electronics All Rights Reserved.
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
 * apps/system/flash_test/flash_test.c
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>

#include <tinyara/fs/mtd.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define BUF_SIZE   1024

#define READ_CMD    "read"
/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int flash_read(unsigned long addr, int size)
{
	int ret;
	int offset;
	int buf_idx;
	int read_size;
	int remaining;
	FAR uint8_t *buffer;
	FAR struct mtd_dev_s *dev_mtd = NULL;

	if (addr == 0 || size <= 0) {
		printf("Invalid arguments, offset 0x%x, read size %d\n", addr, size);
		return ERROR;
	}

	dev_mtd = up_flashinitialize();
	if (!dev_mtd) {
		printf("up_flashinitialize Failed\n");
		return ERROR;
	}

	buffer = (uint8_t *)malloc(BUF_SIZE);
	if (!buffer) {
		printf("Failed to allocate buffer %d\n", BUF_SIZE);
		return ERROR;
	}

	offset = 0;
	remaining = size;

	printf("Read 0x%x: ", addr);

	while (remaining > 0) {
		read_size = BUF_SIZE < remaining ? BUF_SIZE : remaining;
		ret = MTD_READ(dev_mtd, addr + offset, read_size, buffer);
		if (ret <= 0) {
			printf("Read Failed : %d\n", ret);
			free(buffer);
			return ERROR;
		}

		/* Print read buffer */
		for (buf_idx = 0; buf_idx < ret; buf_idx++) {
			if ((buf_idx % 16) == 0) {
				printf("\n");
			}
			printf("0x%02x ", buffer[buf_idx]);
		}
		offset += ret;
		remaining -= ret;
	}
	printf("\n");

	free(buffer);

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int flash_test_main(int argc, char *argv[])
#endif
{
	off_t offset;
	int read_size;

	if (argc == 4 && !strncmp(argv[1], READ_CMD, sizeof(READ_CMD) + 1)) {
		offset = strtoul(argv[2], NULL, 16);
		read_size = (int)atoi(argv[3]);

		/* Read an address in flash by size */
		return flash_read(offset, read_size);
	}

	printf("Usage: flash_test read <offset> <size(bytes)>\n");
	printf("Read flash by 'size' from 'flash base address + offset'\n");
	printf(" * Flash base address is board-specific \n");

	return ERROR;
}
