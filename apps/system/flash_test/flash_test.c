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
#include <arch/board/board.h>
#include <tinyara/spi/spi.h>
#include <tinyara/fs/mtd.h>
#include <time.h>
#include <tinyara/config.h>

//#include <board/common/common.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define BUF_SIZE   4096

#define READ_CMD    "read"
#define WRITE_CMD   "write"
#define ERASE_CMD   "erase"

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct timespec start_time;
struct timespec end_time;

enum flash_e {
	FLASH_INTERNAL = 0,
	FLASH_EXT = 1
};
typedef enum flash_e flash_t;

FAR struct mtd_geometry_s geo;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int flash_read(unsigned long addr, int size, flash_t flash)
{
	int ret;
	int offset;
	int buf_idx;
	int read_size;
	int remaining;
	FAR uint8_t *buffer;
	FAR struct mtd_dev_s *dev_mtd = NULL;

	long long total_duration_ms = 0;

	if (addr == 0 || size <= 0) {
		printf("Invalid arguments, offset 0x%x, read size %d\n", addr, size);
		return ERROR;
	}
	if (flash == FLASH_INTERNAL) {
		printf("Received Flash Read cmd for internal flash offset 0x%x, read size %d\n", addr, size);
		dev_mtd = up_flashinitialize();
		if (!dev_mtd) {
			printf("up_flashinitialize Failed\n");
			return ERROR;
		}

	}
#ifdef CONFIG_SECOND_FLASH_PARTITION

	else if (flash == FLASH_EXT) {
		printf("Received Flash Read cmd for external flash offset 0x%x, read size %d\n", addr, size);
		struct spi_dev_s *spi = up_spiinitialize(1);

#ifdef CONFIG_MTD_JEDEC
		dev_mtd = jedec_initialize(spi);
		if (dev_mtd == NULL) {
			lldbg("Jedec Init failed\n");
			return ERROR;
		}
#elif defined(CONFIG_MTD_W25)
		dev_mtd = w25_initialize(spi);
		if (dev_mtd == NULL) {
			lldbg("w25 Init failed\n");
			return ERROR;
		}
		printf("w25_initialize: done for secondary flash \n");
#endif
	}
#endif
	/* Get the geometry of the FLASH device */
	ret = dev_mtd->ioctl(dev_mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t) & geo));
	if (ret < 0) {
		fdbg("ERROR: mtd->ioctl failed: %d\n", ret);
	}

	printf("Flash Geometry:\n");
	printf("  blocksize:      %lu\n", (unsigned long)geo.blocksize);
	printf("  erasesize:      %lu\n", (unsigned long)geo.erasesize);
	printf("  neraseblocks:   %lu\n", (unsigned long)geo.neraseblocks);

	buffer = (uint8_t *)malloc(BUF_SIZE);
	if (!buffer) {
		printf("Failed to allocate buffer %d\n", BUF_SIZE);
		return ERROR;
	}

	offset = 0;
	remaining = size;

	printf("Read 0x%x: ", addr);

	while (remaining > 0) {
		clock_gettime(CLOCK_REALTIME, &start_time);
		read_size = BUF_SIZE < remaining ? BUF_SIZE : remaining;
		ret = MTD_READ(dev_mtd, addr + offset, read_size, buffer);
		if (ret <= 0) {
			printf("Read Failed : %d\n", ret);
			free(buffer);
			return ERROR;
		}
		clock_gettime(CLOCK_REALTIME, &end_time);

		total_duration_ms += ((end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec)) / 1000000;
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
	printf("\n time taken for Flash Read %lld(ms)\n", total_duration_ms);
	printf("\n");
	free(buffer);

	return OK;
}

static void fill_buffer(FAR uint8_t *buff)
{
	for (int i = 0; i < BUF_SIZE; i++) {
		buff[i] = i;
	}
}

static int flash_write(unsigned long addr, int size, flash_t flash)
{
	int ret;
	int offset;
	int buf_idx;
	int write_size;
	int remaining;
	FAR uint8_t *buffer;
	FAR struct mtd_dev_s *dev_mtd = NULL;

	if (addr == 0 || size <= 0) {
		printf("Invalid arguments, offset 0x%x, Write size %d\n", addr, size);
		return ERROR;
	}
	if (flash == FLASH_INTERNAL) {
		dev_mtd = up_flashinitialize();
		if (!dev_mtd) {
			printf("up_flashinitialize Failed\n");
			return ERROR;
		}
	}
#ifdef CONFIG_SECOND_FLASH_PARTITION
	else if (flash == FLASH_EXT) {
		struct spi_dev_s *spi = up_spiinitialize(1);

#ifdef CONFIG_MTD_JEDEC
		dev_mtd = jedec_initialize(spi);
		if (dev_mtd == NULL) {
			lldbg("Jedec Init failed\n");
			return ERROR;
		}
#elif defined(CONFIG_MTD_W25)
		dev_mtd = w25_initialize(spi);
		if (dev_mtd == NULL) {
			lldbg("w25 Init failed\n");
			return ERROR;
		}
#endif
	}
#endif

	buffer = (uint8_t *)malloc(BUF_SIZE);
	if (!buffer) {
		printf("Failed to allocate buffer %d\n", BUF_SIZE);
		return ERROR;
	}
	fill_buffer(buffer);

	offset = 0;
	remaining = size;

	printf("Write 0x%x: ", addr);
	clock_gettime(CLOCK_REALTIME, &start_time);
	while (remaining > 0) {
		write_size = BUF_SIZE < remaining ? BUF_SIZE : remaining;
		ret = MTD_WRITE(dev_mtd, addr + offset, write_size, buffer);
		if (ret <= 0) {
			printf("Write Failed : %d\n", ret);
			free(buffer);
			return ERROR;
		}
		offset += ret;
		remaining -= ret;
	}
	clock_gettime(CLOCK_REALTIME, &end_time);
	long duration = ((end_time.tv_sec - start_time.tv_sec) * 1000000000) + (end_time.tv_nsec - start_time.tv_nsec);
	duration = duration / 1000000;

	printf("\ntime taken for Flash Write %lld(ms)\n", ((end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec)) / 1000000);
	printf("\n");

	free(buffer);

	return OK;
}

static int flash_erase(unsigned long addr, int size, flash_t flash)
{
	int ret;
	int erase_sectors;
	int erase_sector_st;

	FAR struct mtd_dev_s *dev_mtd = NULL;

	if (addr == 0 || size <= 0) {
		printf("Invalid arguments, offset 0x%x, read size %d\n", addr, size);
		return ERROR;
	}

	if (flash == FLASH_INTERNAL) {
		dev_mtd = up_flashinitialize();
		if (!dev_mtd) {
			printf("up_flashinitialize Failed\n");
			return ERROR;
		}
	}
#ifdef CONFIG_SECOND_FLASH_PARTITION
	else if (flash == FLASH_EXT) {
		struct spi_dev_s *spi = up_spiinitialize(1);

#ifdef CONFIG_MTD_JEDEC
		dev_mtd = jedec_initialize(spi);
		if (dev_mtd == NULL) {
			lldbg("Jedec Init failed\n");
			return ERROR;
		}
#elif defined(CONFIG_MTD_W25)
		dev_mtd = w25_initialize(spi);
		if (dev_mtd == NULL) {
			lldbg("w25 Init failed\n");
			return ERROR;
		}
#endif
	}
#endif
	/* Get the geometry of the FLASH device */
	ret = dev_mtd->ioctl(dev_mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t) & geo));
	if (ret < 0) {
		fdbg("ERROR: mtd->ioctl failed: %d\n", ret);
	}

	printf("Flash Geometry:\n");
	printf("  blocksize:      %lu\n", (unsigned long)geo.blocksize);
	printf("  erasesize:      %lu\n", (unsigned long)geo.erasesize);
	printf("  neraseblocks:   %lu\n", (unsigned long)geo.neraseblocks);

	if (addr % geo.erasesize != 0) {
		printf("ERROR: Address not aligned with the Erase sector size\n");
		return ERROR;
	}
	erase_sectors = size / geo.erasesize;
	if (size % geo.erasesize != 0) {
		erase_sectors++;
	}
	erase_sector_st = addr / geo.erasesize;

	printf("Erase addr 0x%x & Secors 0x%x: ", addr, erase_sectors);
	clock_gettime(CLOCK_REALTIME, &start_time);

	ret = MTD_ERASE(dev_mtd, erase_sector_st, erase_sectors);
	if (ret < 0) {
		printf("Erase Failed: %d Erase sector is : %d\n", ret, erase_sector_st);
		return ERROR;
	}

	clock_gettime(CLOCK_REALTIME, &end_time);
	long duration = ((end_time.tv_sec - start_time.tv_sec) * 1000000000) + (end_time.tv_nsec - start_time.tv_nsec);
	duration = duration / 1000000;

	printf("\ntime taken for Flash Erase of %ld sectors of sector size %ld is  %lld(ms)\n", erase_sectors, geo.erasesize, ((end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec)) / 1000000);
	printf("\n");
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
	int size;
	flash_t flash_type;

	if (argc == 5) {
		offset = strtoul(argv[2], NULL, 16);
		size = (int)atoi(argv[3]);
		flash_type = (flash_t) atoi(argv[4]);

#if !defined(CONFIG_SECOND_FLASH_PARTITION)
		if (flash_type == FLASH_EXT) {
			printf("\nERROR: Enable External Flash related configs to perform the test.\n");
			return ERROR;
		}
#endif

		if (!strncmp(argv[1], READ_CMD, sizeof(READ_CMD) + 1)) {
			/* Read an address in flash by size */
			return flash_read(offset, size, flash_type);
		} else if (!strncmp(argv[1], WRITE_CMD, sizeof(WRITE_CMD) + 1)) {
			/* Write an address in flash by size */
			return flash_write(offset, size, flash_type);
		} else if (!strncmp(argv[1], ERASE_CMD, sizeof(ERASE_CMD) + 1)) {
			/* Erase an address in flash by size */
			return flash_erase(offset, size, flash_type);
		}
	} else {
		printf("Usage: flash_test read <offset> <size(bytes)> <Flash_type>(0: Internal, 1: External)\n");
		printf("Usage: flash_test write <offset> <size(bytes)> <Flash_type>(0: Internal, 1: External)\n");
		printf("Usage: flash_test erase <offset> <size(bytes)> <Flash_type>(0: Internal, 1: External)\n");

		printf("Read flash by 'size' from 'flash base address + offset'\n");
		printf(" * Flash base address is board-specific \n");
		printf(" * Make sure to give the sector aligned address for the Flash erase \n");
	}

	return ERROR;
}
