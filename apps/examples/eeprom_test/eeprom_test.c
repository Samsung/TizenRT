/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * apps/examples/eeprom_test/eeprom_test.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/statfs.h>
#include <sys/stat.h>
#include <tinyara/fs/fs_utils.h>
#ifdef CONFIG_TASH
#include <apps/shell/tash.h>
#endif
#ifndef CONFIG_DISABLE_MOUNTPOINT
#include <sys/mount.h>
#ifdef CONFIG_FS_SMARTFS
#include <tinyara/fs/mksmartfs.h>
#endif
#ifdef CONFIG_RAMDISK
#include <tinyara/fs/ramdisk.h>
#endif
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define EE_TEST_PRI      100
#define EE_TEST_STAKSIZE 1024

static int eeprom_test(int argc, char **args)
{
	int rd_fd;
	int wr_fd;
	int i;
	char *array1, *array2;

	/* Allocate the memory */
	array1 = (char *)malloc(524);
	if (!array1) {
		goto err_1;
	}

	array2 = (char *)malloc(524);
	if (!array2) {
		goto err_2;
	}

	rd_fd = open("/dev/eeprom", O_RDONLY);
	if (rd_fd < 0) {
		goto err_3;
	}

	wr_fd = open("/dev/eeprom", O_WRONLY);
	if (rd_fd < 0) {
		goto err_4;
	}

	if (read(rd_fd, array1, 512) != 512) {
		goto err_6;
	}
	lseek(rd_fd, 0, SEEK_SET);

	for (i = 0; i < 512; i++) {
		array1[i] += i;
	}

	if (write(wr_fd, array1, 512) != 512) {
		goto err_5;
	}

	if (read(rd_fd, array2, 512) != 512) {
		goto err_6;
	}

	if (memcmp(array1, array2, 512) == 0) {
		printf("\nTEST1 PASS\n");
	}

	for (i = 0; i < 512; i++) {
		array1[i] += i + 1;
	}

	lseek(wr_fd, 0, SEEK_SET);
	lseek(rd_fd, 0, SEEK_SET);

	if (write(wr_fd, array1, 1) != 1) {
		goto err_5;
	}

	if (write(wr_fd, array1 + 1, 510) != 510) {
		goto err_5;
	}

	if (write(wr_fd, array1 + 511, 1) != 1) {
		goto err_5;
	}

	if (read(rd_fd, array2, 1) != 1) {
		goto err_6;
	}

	if (read(rd_fd, array2 + 1, 510) != 510) {
		goto err_6;
	}

	if (read(rd_fd, array2 + 511, 1) != 1) {
		goto err_6;
	}

	if (memcmp(array1, array2, 512) == 0) {
		printf("\nTEST2 PASS\n");
	} else {
		printf("\nTEST2 FAIL\n");
	}

	close(rd_fd);
	close(wr_fd);
	free(array2);
	free(array1);

	return 0;

err_6:
err_5:
	close(wr_fd);
err_4:
	close(rd_fd);
err_3:
	free(array2);
err_2:
	free(array1);
err_1:
	printf("\nERROR, test not passed");
	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * ee_test_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ee_test_main(int argc, char **args)
#endif
{
#ifdef CONFIG_TASH
	tash_cmd_install("ee_test", eeprom_test, TASH_EXECMD_SYNC);
#endif
	return 0;
}
