/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 * examples/mmwave/example_mmwave.c
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
#include <tinyara/sensors/iwrl6432.h>
#include <tinyara/rtc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>
#include <errno.h>

#define MQ_PATH "mmwave_mq"
static bool g_terminate;
static mqd_t g_mmwave_mq;
static int mFd;

static int mmwave_start();
static int mmwave_stop();
static int mmwave_prepare();

static int mmwave_init()
{
	int ret;
	int buf_len;
	int buf_size;
	struct mq_attr attr;
	
	mFd = open("/dev/mmWave", O_RDONLY);
	if (mFd < 0) {
		printf("open failed errno : %d\n", errno);
		return -1;
	}

	/* Get Size of buffer, but we can skip this, we know size actually */
	ret = ioctl(mFd, SNIOC_GET_BUFSIZE, (unsigned long)&buf_size);
	if (ret < 0) {
		printf("get Buffer failed. errno : %d\n", errno);
		return -1;
	}
	/* Get Number of Buffer, but we can skip this, we know size actually */
	ret = ioctl(mFd, SNIOC_GET_BUFNUM, (unsigned long)&buf_len);
	if (ret < 0) {
		printf("get Buffer failed. errno : %d\n", errno);
		return -1;
	}

	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct iwrl6432_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;
	g_mmwave_mq = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0644, &attr);
	if (g_mmwave_mq == NULL) {
		printf("mq_open failed!!\n");
		return -1;
	}

	ret = ioctl(mFd, SNIOC_REGISTERMQ, (unsigned long)g_mmwave_mq);
	if (ret < 0) {
		printf("register mq failed. errno : %d\n", errno);
		return -1;
	}
}

static int mmwave_start()
{
	int ret;
	struct iwrl6432_msg_s msg;
	int prio;

	size_t size;
	/* Start Collect */
	ret = ioctl(mFd, SNIOC_START, NULL);
	if (ret < 0) {
		printf("start failed. errno : %d\n", errno);
		return -1;
	}

	while (!g_terminate)
	{
		ret = ioctl(mFd, SNIOC_GETDATA, NULL);
		if (ret < 0) {
			printf("get Buffer failed. errno : %d\n", errno);
			return -1;
		}
		size = mq_receive(g_mmwave_mq, (FAR char *)&msg, sizeof(msg), &prio);
		if (size != sizeof(msg)) {
			printf("wrong msg... size : %d sizeofmsg : %d\n", size, sizeof(msg));
			continue;
		}
		lldbg("@@@@@ msgId!! %d\n", msg.msgId);

		if (msg.msgId == IWRL6432_MSG_DEQUEUE) {
			/* memcpy data here */
			
		} else if (msg.msgId == IWRL6432_MSG_STOP_FORCELY) {
			mmwave_stop();
		} else if (msg.msgId == IWRL6432_MSG_UNDERRUN) {
			/* TODO Cancel AI Model here?? */
		}
	}
	mq_close(g_mmwave_mq);
	return OK;
}

static int mmwave_stop()
{
	int ret;
	/* Start Collect */
	ret = ioctl(mFd, SNIOC_STOP, NULL);
	if (ret < 0) {
		printf("stop failed. errno : %d\n", errno);
		return -1;
	}
	close(mFd);
	g_terminate = true;
	return OK;
}

static int mmwave_prepare()
{
	int ret;
	/* Start Collect */
	ret = ioctl(mFd, SNIOC_PREPARE, NULL);
	if (ret < 0) {
		printf("prepare failed. errno : %d\n", errno);
		return -1;
	}
	return OK;
}



static void show_usage(void)
{
	printf("usage: mmwave <command #>\n");
	printf("Excute mmwave testing or controling.\n\n");
	printf("    prepare : Prepare the mmwave operation \n");
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
	mmwave_init();
	g_terminate = false;
	if (argc == 2) {
		if (!strncmp(argv[1], "prepare", 8)) {
			return mmwave_prepare();
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
