/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * examples/lcd_test/example_lcd.c
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

#include <tinyara/config.h>
#include <tinyara/mmwave/mmwave_dev.h>
#include <tinyara/rtc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>

#define CUBE_PACKET_SIZE 16408
#define CUBE_DATA_SIZE 16384
#define NO_OF_SEGMENT 6
bool g_terminate;

static int mmwave_start()
{
	uint8_t **data = (uint8_t **)malloc(NO_OF_SEGMENT * sizeof(uint8_t *));
	if (!data) {
		lldbg("malloc failed\n");
	}
	for (int i = 0; i < NO_OF_SEGMENT; i++) {
		data[i] = (uint8_t *)malloc(CUBE_DATA_SIZE);
		if (!data[i]) {
			lldbg("malloc failed\n");
		}
	}

	int ret = OK;
	int fd = 0;
	int p = 0;
	char port[20] = {'\0'};
	g_terminate = false;

	sprintf(port, MMWAVE_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open mmwave port : %s error:%d\n", port, fd);
		return ERROR;
	}
	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	while (!g_terminate) {
		poll(fds, 1, 5000);
		if (fds[0].revents & POLLIN) {
			read(fd, data, NO_OF_SEGMENT * CUBE_DATA_SIZE);
			printf("=============One frame Received=========\n");
		}
	}
	// read(fd, cube_data, CUBE_PACKET_SIZE);

	close(fd);
	for (int i = 0; i < NO_OF_SEGMENT; i++) {
		free(data[i]);
	}
	free(data);
	return ret;
}

static int mmwave_stop()
{
	g_terminate = true;
	return OK;
}

static int mmwave_pause()
{
	int ret = OK;
	int fd = 0;
	int p = 0;
	char port[20] = {'\0'};

	sprintf(port, MMWAVE_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open mmwave port : %s error:%d\n", port, fd);
		return ERROR;
	}
	if (ioctl(fd, MMWAVE_PAUSE, NULL) != OK) {
		printf("Fail to MMWAVE_PAUSE %s, errno:%d\n", MMWAVE_DEV_PATH, get_errno());
	}
	close(fd);
	return ret;
}

static int mmwave_resume()
{
	int ret = OK;
	int fd = 0;
	int p = 0;
	char port[20] = {'\0'};

	sprintf(port, MMWAVE_DEV_PATH, p);
	fd = open(port, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open mmwave port : %s error:%d\n", port, fd);
		return ERROR;
	}
	if (ioctl(fd, MMWAVE_RESUME, NULL) != OK) {
		printf("Fail to MMWAVE_RESUME %s, errno:%d\n", MMWAVE_DEV_PATH, get_errno());
	}
	close(fd);
	return ret;
}

static void show_usage(void)
{
	printf("usage: mmwave <command #>\n");
	printf("Excute mmwave testing or controling.\n\n");
	printf("    pause   : Pause the mmwave operation \n");
	printf("    resume  : Resume the mmwave operation\n");
	printf("    start   : Start reading mmwave data\n");
	printf("    stop    : Stop Reading mmwave data\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mmwave_main(int argc, char *argv[])
#endif
{
	if (argc <= 1 || !strncmp(argv[1], "-h", 2) || !strncmp(argv[1], "--help", 6)) {
		show_usage();
		return OK;
	}
	if (argc == 2) {
		if (!strncmp(argv[1], "pause", 6)) {
			return mmwave_pause();
		} else if (!strncmp(argv[1], "resume", 7)) {
			return mmwave_resume();
		} else if (!strncmp(argv[1], "start", 6)) {
			return mmwave_start();
		} else if (!strncmp(argv[1], "stop", 5)) {
			return mmwave_stop();
		} else {
			show_usage();
		}
	}

	return OK;
}
