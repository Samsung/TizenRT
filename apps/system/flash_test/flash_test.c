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
#include <sys/time.h>
#include <arch/board/board.h>
#include <tinyara/spi/spi.h>
#include <tinyara/fs/mtd.h>
#include <time.h>
#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>
#include <fcntl.h>
/****************************************************************************
 * Definitions
 ****************************************************************************/
#define BUF_SIZE   65536
#define SIZE_1MB   1048576

#define READ_CMD    "read"
#define WRITE_CMD   "write"
#define ERASE_CMD   "erase"
#define BCDEV_CMD	"bch"

#define FLASH_TEST_PART_NAME "flash_test"
#define FLASH_TEST_PART_TYPE "ftl"

/****************************************************************************
 * Private Data
 ****************************************************************************/
enum flash_e {
	FLASH_INTERNAL = 0,
	FLASH_EXT = 1
};
typedef enum flash_e flash_t;

static FAR struct mtd_geometry_s geo;
static long sys_start_time = 0;
static long sys_end_time = 0;
static long systick_time_ms = 0;

static char blockname[32];

static inline long get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
static int count_occurrences(const char* str, char ch)
{
	int cnt = 0;
	const char *p = strchr(str, ch);
	while(p != NULL) {
		cnt++;
		p = strchr(p+1, ch);
	}
	return cnt;
}

static char* moveto_partno(const char* str, int partno)
{
	int cnt = partno;
	char ch = ',';
	char *p = strchr(str, ch);
	while (cnt != 1) {
		cnt--;
		p = strchr(p + 1, ch);
		p = p + 1;
	}
	return p;
}

static int verify_partition_type(flash_t flash, int partno)
{
	char *part_type;
	char *part_found = NULL;
	int ret = ERROR;
	unsigned int size = 0;

	size =  sizeof(CONFIG_FLASH_PART_TYPE);
	if(flash == FLASH_EXT)	{
		size +=  sizeof(CONFIG_SECOND_FLASH_PART_TYPE);
	}
	part_type = (char *)malloc(size);
	if(part_type == NULL) {
		printf("Memory allocation failed\n");
		return ret;
	}
	strcpy(part_type,CONFIG_FLASH_PART_TYPE);
	if(flash == FLASH_EXT)	{
		strcat(part_type,CONFIG_SECOND_FLASH_PART_TYPE);
	}
	part_found = moveto_partno(part_type,partno);
	if(part_found == NULL) {
		printf("ERROR: flash_test partition not found\n");
		goto error_out;
	}
	if(strncmp(part_found,FLASH_TEST_PART_TYPE,sizeof(FLASH_TEST_PART_TYPE) != 0)) {
		printf("ERROR: flash_test partition type not matched\n");
		goto error_out;
	}
	ret = OK;
error_out:
	free(part_type);	
	return ret;
}

static int find_bch(flash_t flash, int* part_no)
{
	char *part_name;
	char *part_found = NULL;
	int ret = ERROR;
	unsigned int size = 0;

	size =  sizeof(CONFIG_FLASH_PART_NAME);
#ifdef CONFIG_SECOND_FLASH_PARTITION
	size +=  sizeof(CONFIG_SECOND_FLASH_PART_NAME);
#endif
	part_name = (char *)malloc(size);
	if(part_name == NULL) {
		printf("Memory allocation failed\n");
		return ret;
	}
	if (flash == FLASH_INTERNAL) {
		part_found = strstr(CONFIG_FLASH_PART_NAME, FLASH_TEST_PART_NAME);
		if(part_found == NULL) {
			printf("No partition found for flash_test\n");
			goto error_out;
		}
		strncpy(part_name,CONFIG_FLASH_PART_NAME,(part_found-CONFIG_FLASH_PART_NAME));
		*part_no = count_occurrences(part_name, ',');
	}
	else
	{
		*part_no = count_occurrences(CONFIG_FLASH_PART_NAME, ',');

		part_found = strstr(CONFIG_SECOND_FLASH_PART_NAME,FLASH_TEST_PART_NAME);
		if(part_found== NULL){
			printf("Not found flash_test in second partition\n");
			goto error_out;
		}
		strncpy(part_name,CONFIG_SECOND_FLASH_PART_NAME,(part_found-CONFIG_SECOND_FLASH_PART_NAME));
		*part_no += count_occurrences(part_name, ',');
	}
	ret = verify_partition_type(flash, *part_no);
	if(ret != OK) {
		printf("ERROR: verify partition type failed\n");
		goto error_out;
	}
	ret = OK;

error_out:
	free(part_name);
	return ret;
}

static int prepare_bch(flash_t flash, int * partno)
{
	int ret = ERROR;
	int fd;

	ret = find_bch(flash, partno);
	if (ret!= OK) {
		printf("ERROR: check the partition config of flash_test is set properly\n");
		return ret;
	}
	snprintf(blockname, 32, "/dev/mtdblock%d", *partno);

	fd = open(blockname, O_RDWR);
	if (fd < 0) {
		printf("ERROR: open bch %s\n", blockname);
		return ret;
	}
	ret = ioctl(fd, MTDIOC_GEOMETRY, (unsigned long)&geo);
	if (ret < 0) {
		printf("ERROR : Not able to get the BCH Flash Geometry:\n");
		goto out_close_fd;
	}
	printf("BCH Flash Geometry:\n");
	printf("  blocksize:      %lu\n", (unsigned long)geo.blocksize);
	printf("  erasesize:      %lu\n", (unsigned long)geo.erasesize);
	printf("  neraseblocks:   %lu\n", (unsigned long)geo.neraseblocks);
	ret = OK;

out_close_fd:	
	close(fd);
	return ret;
}

static void fill_buffer(FAR uint32_t *buff)
{
	off_t offset = 0;
	for (int i = 0; i < BUF_SIZE / sizeof(uint32_t); i++) {
		buff[i] = offset;
		offset += 4;
	}
}

static int bch_write(flash_t flash)
{
	FAR uint32_t *buffer;
	int fd;
	ssize_t nbytes;
	off_t offset;
	int ret = ERROR;
	int g_partno = 0;

	/* Allocate a buffer */
	buffer = (FAR uint32_t *)malloc(BUF_SIZE);

	if (!buffer) {
		printf("ERROR: failed to allocate a sector buffer\n");
		return ret;
	}

	ret = prepare_bch(flash, &g_partno);
	if(ret != OK) {
		printf("prepare_bch fail\n");
		goto out_free_buffer;
	}
	/* Open the master MTD FLASH character driver for writing */

	fd = open(blockname, O_WRONLY);
	if (fd < 0) {
		printf("ERROR: open /dev/mtd%d failed: %d\n", g_partno, errno);
		goto out_free_buffer;
	}
	/* Now write the offset into every block */
	printf("Writing to media: buff Size %d, times %d\n", BUF_SIZE, (SIZE_1MB/BUF_SIZE));

	offset = 0;
	/* Fill the block with the offset */
	fill_buffer(buffer);

	sys_start_time = get_time();
	printf("BCH Write start\n");

	/* And write it using the character driver */
	for (int i = 0; i < (SIZE_1MB/BUF_SIZE); i++) {
		nbytes = write(fd, buffer, (size_t)(BUF_SIZE));
		if (nbytes < 0) {
			printf("ERROR: write to /dev/mtd%d failed: %d\n", g_partno, errno);
			goto out_close_fd;
		}
	}
	printf("BCH Write End\n");
	sys_end_time = get_time();
	printf("\nSystick time taken for Flash bch Write %ld(ms)\n", (sys_end_time - sys_start_time));
	ret = OK;

out_close_fd:
	close(fd);
out_free_buffer:
	free(buffer);
	return ret;
}

static int bch_Read(flash_t flash)
{
	FAR uint32_t *buffer;
	FAR uint32_t *rd_buffer;
	int fd;
	ssize_t nbytes;
	off_t offset;
	int g_partno = 0;
	int ret = ERROR;

	/* Allocate a buffer */
	buffer = (FAR uint32_t *)malloc(BUF_SIZE);

	if (!buffer) {
		printf("ERROR: failed to allocate a sector buffer\n");
		return ret;
	}
	rd_buffer = (FAR uint32_t *)malloc(BUF_SIZE);
	if (!buffer) {
		printf("ERROR: failed to allocate a sector buffer\n");
		free(buffer);
		return ret;
	}

	ret = prepare_bch(flash, &g_partno);
	if(ret != OK) {
		printf("prepare_bch fail\n");
		goto out_free_buffer;
	}
	/* Open the master MTD FLASH character driver for writing */

	fd = open(blockname, O_RDONLY);
	if (fd < 0) {
		printf("ERROR: open /dev/mtd%d failed: %d\n", g_partno, errno);
		goto out_free_buffer;
	}

	/* Now Read the offset into every block */
	printf("Reading from media: buff Size %d, times %d\n", BUF_SIZE, (SIZE_1MB/BUF_SIZE));

	offset = 0;
	systick_time_ms = 0;
	/* Fill the block with the offset */

	fill_buffer(buffer);

	/* And write it using the character driver */
	printf("BCH Read start\n");
	for (int j = 0; j < (SIZE_1MB/BUF_SIZE); j++) {
		sys_start_time = get_time();
		nbytes = read(fd, rd_buffer, (size_t)(BUF_SIZE));
		if (nbytes < 0) {
			printf("ERROR: Read to /dev/mtd%d failed: %d\n", g_partno, errno);
			close(fd);
			goto out_close_fd;
		}
		sys_end_time = get_time();
		systick_time_ms += sys_end_time - sys_start_time;
		if (memcmp(rd_buffer, buffer, nbytes)) {
			printf("ERROR: Read value comparison failed to /dev/mtd%d failed: %d\n", g_partno, errno);
			close(fd);
			goto out_close_fd;
		}
	}
	printf("BCH Read End\n");
	printf("\nSystick time taken for Flash bch Read %ld(ms)\n", systick_time_ms);
	ret = OK;

out_close_fd:
	close(fd);
out_free_buffer:
	free(buffer);
	free(rd_buffer);
	return ret;
}

static int bch_erase(flash_t flash)
{
	int ret = ERROR;
	int fd;
	int g_partno = 0;

	ret = prepare_bch(flash, &g_partno);
	if(ret != OK) {
		printf("prepare_bch fail\n");
		return ret;
	}

	fd = open(blockname, O_RDWR);
	if (fd < 0) {
		printf("ERROR: open failed for: %s\n", blockname);
		return ret;
	}
	sys_start_time = get_time();
	printf("BCH Erase start\n");
	ret = ioctl(fd, MTDIOC_BULKERASE, 0);
	if (ret < 0) {
		fdbg("ERROR: ioctl ERASE failed: %d\n", ret);
		goto out_close_fd;
	}
	printf("BCH Erase End\n");
	sys_end_time = get_time();
	printf("\nSystick time taken for Flash bch Erase %ld(ms)\n", (sys_end_time - sys_start_time));
	ret = OK;

out_close_fd:
	close(fd);
	return ret;
}

#ifdef CONFIG_BUILD_FLAT
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int mtd_read(unsigned long addr, int size, flash_t flash)
{
	int ret = ERROR;
	int offset;
	int buf_idx;
	int read_size;
	int remaining;
	FAR uint32_t *wr_buffer;
	FAR uint8_t *rd_buffer;
	FAR struct mtd_dev_s *dev_mtd = NULL;
	long long total_duration_ms = 0;
	systick_time_ms = 0;

	if (addr == 0 || size <= 0) {
		printf("Invalid arguments, offset 0x%x, read size %d\n", addr, size);
		return ret;
	}
	if (flash == FLASH_INTERNAL) {
		printf("Received Flash Read cmd for internal flash offset 0x%x, read size %d\n", addr, size);
		dev_mtd = up_flashinitialize();
		if (!dev_mtd) {
			printf("up_flashinitialize Failed\n");
			return ret;
		}

	}
#ifdef CONFIG_SECOND_FLASH_PARTITION

	else if (flash == FLASH_EXT) {
		printf("Received Flash Read cmd for external flash offset 0x%x, read size %d\n", addr, size);
		struct spi_dev_s *spi = up_spiinitialize(1);

#ifdef CONFIG_MTD_JEDEC
		dev_mtd = jedec_initialize(spi);
		if (dev_mtd == NULL) {
			printf("Jedec Init failed\n");
			return ret;
		}
#elif defined(CONFIG_MTD_W25)
		dev_mtd = w25_initialize(spi);
		if (dev_mtd == NULL) {
			printf("w25 Init failed\n");
			return ret;
		}
		printf("w25_initialize: done for secondary flash \n");
#endif
	}
#endif
	/* Get the geometry of the FLASH device */
	ret = dev_mtd->ioctl(dev_mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t)&geo));
	if (ret < 0) {
		printf("ERROR: mtd->ioctl failed: %d\n", ret);
		return ret;
	}

	printf("Flash Geometry:\n");
	printf("  blocksize:      %lu\n", (unsigned long)geo.blocksize);
	printf("  erasesize:      %lu\n", (unsigned long)geo.erasesize);
	printf("  neraseblocks:   %lu\n", (unsigned long)geo.neraseblocks);

	/* Allocate a buffer */
	wr_buffer = (FAR uint32_t *)malloc(BUF_SIZE);
	if (!wr_buffer) {
		printf("Failed to allocate wr_buffer %d\n", BUF_SIZE);
		return ret;
	}

	rd_buffer = (FAR uint8_t *)malloc(BUF_SIZE);

	if (!rd_buffer) {
		printf("ERROR: failed to allocate a sector buffer\n");
		free(wr_buffer);
		return ret;
	}
	offset = 0;
	remaining = size;

	fill_buffer(wr_buffer);
	printf("Read size %d at addr 0x%x: \n",size, addr);
	printf("Flash Read start\n");

	while (remaining > 0) {
		sys_start_time = get_time();
		
		read_size = BUF_SIZE < remaining ? BUF_SIZE : remaining;
		ret = MTD_READ(dev_mtd, addr + offset, read_size, (uint8_t *)rd_buffer);
		if (ret != read_size) {
			printf("Read Failed : %d\n", ret);
			ret = ERROR;
			goto error_out;
		}
		sys_end_time = get_time();
		systick_time_ms += sys_end_time - sys_start_time;
		/* Compare the contents */
		if (memcmp(rd_buffer, wr_buffer, read_size)) {
			printf("ERROR: Flash Read value comparison failed at %p \n", addr + offset);
			ret = ERROR;
			goto error_out;
		}
		/* Print read buffer */
		for (buf_idx = 0; buf_idx < ret; buf_idx++) {
			if ((buf_idx % 16) == 0) {
				printf("\n");
			}
			printf("0x%02x ", (uint8_t *) rd_buffer[buf_idx]);
		}
		offset += ret;
		remaining -= ret;
	}
	printf("Flash Read End\n");
	printf("\n Systick time taken %ld(ms)\n", systick_time_ms);
	printf("\n");
	ret = OK;
error_out:	
	free(wr_buffer);
	free(rd_buffer);
	return ret;
}

static int mtd_write(unsigned long addr, int size, flash_t flash)
{
	int ret;
	int offset;
	int write_size;
	int remaining;
	FAR uint32_t *buffer;
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
			printf("Jedec Init failed\n");
			return ERROR;
		}
#elif defined(CONFIG_MTD_W25)
		dev_mtd = w25_initialize(spi);
		if (dev_mtd == NULL) {
			printf("w25 Init failed\n");
			return ERROR;
		}
#endif
	}
#endif

	buffer = (FAR uint32_t *)malloc(BUF_SIZE);
	if (!buffer) {
		printf("Failed to allocate buffer %d\n", BUF_SIZE);
		return ERROR;
	}
	fill_buffer(buffer);

	offset = 0;
	remaining = size;

	printf("Write 0x%x: \n", addr);
	sys_start_time = get_time();
	printf("Flash Write Start\n");
	while (remaining > 0) {
		write_size = BUF_SIZE < remaining ? BUF_SIZE : remaining;
		ret = MTD_WRITE(dev_mtd, addr + offset, write_size, (const uint8_t *)buffer);
		if (ret <= 0) {
			printf("Write Failed : %d\n", ret);
			free(buffer);
			return ERROR;
		}
		offset += ret;
		remaining -= ret;
	}
	printf("Flash Write End\n");
	sys_end_time = get_time();
	printf("\nSystick time taken for Flash Write %ld(ms)\n", (sys_end_time - sys_start_time));

	printf("\n");
	free(buffer);
	return OK;
}

static int mtd_erase(unsigned long addr, int size, flash_t flash)
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
			printf("Jedec Init failed\n");
			return ERROR;
		}
#elif defined(CONFIG_MTD_W25)
		dev_mtd = w25_initialize(spi);
		if (dev_mtd == NULL) {
			printf("w25 Init failed\n");
			return ERROR;
		}
#endif
	}
#endif
	/* Get the geometry of the FLASH device */
	ret = dev_mtd->ioctl(dev_mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t)&geo));
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

	printf("Erase addr 0x%x & Secors 0x%x: \n", addr, erase_sectors);
	sys_start_time = get_time();
	printf("Flash Erase Start\n");
	ret = MTD_ERASE(dev_mtd, erase_sector_st, erase_sectors);
	if (ret < 0) {
		printf("Erase Failed: %d Erase sector is : %d\n", ret, erase_sector_st);
		return ERROR;
	}
	printf("Flash Erase End\n");
	sys_end_time = get_time();
	printf("\nSystick time taken for Flash Erase %ld(ms)\n", (sys_end_time - sys_start_time));
	printf("\n");
	return OK;
}
#endif
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

	if ((argc == 4) && (!strncmp(argv[1], BCDEV_CMD, sizeof(BCDEV_CMD) + 1))) {
		flash_type = (flash_t)atoi(argv[3]);
#if !defined(CONFIG_SECOND_FLASH_PARTITION)
		if (flash_type == FLASH_EXT) {
			printf("\nERROR: Enable External Flash related configs to perform the test.\n");
			return ERROR;
		}
#endif
		if (!strncmp(argv[2], READ_CMD, sizeof(READ_CMD) + 1)) {
			return bch_Read(flash_type);
		} else if (!strncmp(argv[2], WRITE_CMD, sizeof(WRITE_CMD) + 1)) {
			return bch_write(flash_type);
		} else if (!strncmp(argv[2], ERASE_CMD, sizeof(ERASE_CMD) + 1)) {
			return bch_erase(flash_type);
		}
	}
#ifdef CONFIG_BUILD_FLAT /* Mtd support only on flat build*/
	else if (argc == 5) {
		offset = strtoul(argv[2], NULL, 16);
		size = (int)atoi(argv[3]);
		flash_type = (flash_t)atoi(argv[4]);

#if !defined(CONFIG_SECOND_FLASH_PARTITION)
		if (flash_type == FLASH_EXT) {
			printf("\nERROR: Enable External Flash related configs to perform the test.\n");
			return ERROR;
		}
#endif
		if (!strncmp(argv[1], READ_CMD, sizeof(READ_CMD) + 1)) {
			/* Read an address in flash by size */
			return mtd_read(offset, size, flash_type);
		} else if (!strncmp(argv[1], WRITE_CMD, sizeof(WRITE_CMD) + 1)) {
			/* Write an address in flash by size */
			return mtd_write(offset, size, flash_type);
		} else if (!strncmp(argv[1], ERASE_CMD, sizeof(ERASE_CMD) + 1)) {
			/* Erase an address in flash by size */
			return mtd_erase(offset, size, flash_type);
		}
	}
#endif //CONFIG_BUILD_FLAT	
	else {
		printf("Usage: flash_test read,write & erase only work on flat build\n");
		printf("Usage: flash_test read <offset> <size(bytes)> <Flash_type>(0: Internal, 1: External)\n");
		printf("Usage: flash_test write <offset> <size(bytes)> <Flash_type>(0: Internal, 1: External)\n");
		printf("Usage: flash_test erase <offset> <size(bytes)> <Flash_type>(0: Internal, 1: External)\n");
		printf("Usage: flash_test bch read  <Flash_type>(0: Internal, 1: External)\n");
		printf("Usage: flash_test bch write <Flash_type>(0: Internal, 1: External)\n");
		printf("Usage: flash_test bch erase <Flash_type>(0: Internal, 1: External)\n");

		printf("Read flash by 'size' from 'flash base address + offset'\n");
		printf(" * Flash base address is board-specific \n");
		printf(" * Make sure to give the sector aligned address for the Flash erase \n");
	}

	return ERROR;
}
