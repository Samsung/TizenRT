/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 * examples/smartfs_powercut/smartfs_powercut_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <crc16.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <tinyara/fs/ioctl.h>
#include <tinyara/config.h>

/* Below paths are for test file which use small size of buffer(same as st_blksize) of stat */
#define TEST_SMALL 			"/mnt/test_small"
#define TEST_SMALL_BACKUP 	"/mnt/test_small.backup"

#define TEST_LARGE 			"/mnt/test_large"
#define TEST_LARGE_BACKUP 	"/mnt/test_large.backup"
#define TEST_BUFSIZE		4096

struct buffer_data_s {
	uint16_t crc16;
	char *data;
};

void make_random_chars(char *buf, size_t len)
{
	for (int i = 0; i < len; i++) {
		buf[i] = rand() % len;
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int smartfs_powercut_main(int argc, char *argv[])
#endif
{
	int ret;
	struct stat st;
	int bufsize;
	int fd;
	struct buffer_data_s small;
	struct buffer_data_s large;
	
	/* Preset of Power cut Test */
	
	ret = stat("/mnt", &st);
	if (ret != OK) {
		printf("stat error... errno : %d\n", errno);
		return ERROR;
	}
	/* Set small size of buffer, same as data size in sector */
	bufsize = st.st_blksize - sizeof(small.crc16);
	small.data = (char *)malloc(sizeof(char) * bufsize);
	
	ret = stat(TEST_SMALL, &st);
	/* Create Test file if it is not exist */
	if (ret == -ENOENT) {
		fd = open(TEST_SMALL, O_WROK | O_CREAT);
		if (fd < 0) {
			printf("open error path : %s errno : %d\n", TEST_SMALL, errno);
			return ERROR;
		}
		memset(small.data, 0xff, bufsize);
		make_random_chars(small.data, bufsize);
		for (int i = 0; i < bufsize; i++) {
			printf("[%c]", small.data[i]);
			if (i != 0 && i % 20 == 0) {
				printf("\n");
			}
		}
	}

	while(1) {
		
	}
	return OK;
}
