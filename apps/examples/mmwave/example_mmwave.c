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
static struct iwrl6432_buf_s **gBuffer;
int buf_size;  // size of segment buffer
int g_buf_num; // segment number of iwrl6432
static int cnt = 0;

static int mmwave_start();
static int mmwave_show();
static int mmwave_stop();
static int mmwave_prepare();
static void mmwave_teardown();

static int mmwave_init()
{
	int ret;

	struct mq_attr attr;
	int i;
	mFd = open("/dev/mmWave", O_RDONLY);
	if (mFd < 0) {
		printf("open failed errno : %d\n", errno);
		return -1;
	}

	/* Get Size of buffer, but we can skip this, we know size actually */
	ret = ioctl(mFd, SNIOC_GET_BUFSIZE, (unsigned long)&buf_size);
	if (ret < 0) {
		printf("get Buffer size failed. errno : %d\n", errno);
		goto error_with_fd;
	}
	/* Get Number of Buffer, but we can skip this, we know size actually */
	ret = ioctl(mFd, SNIOC_GET_BUFNUM, (unsigned long)&g_buf_num);
	if (ret < 0) {
		printf("get Buffer number failed. errno : %d\n", errno);
		goto error_with_fd;
	}

	/* Alloc array of pointers to gBuffers */
	gBuffer = (FAR struct iwrl6432_buf_s **)malloc(g_buf_num * sizeof(FAR void *));
	if (gBuffer == NULL) {
		printf("Alloc gBuffer failed\n");
		goto error_with_fd;
	}

	for (i = 0; i < g_buf_num; i++) {
		gBuffer[i] = (FAR struct iwrl6432_buf_s *)malloc(sizeof(FAR struct iwrl6432_buf_s));
		if (gBuffer[i] == NULL) {
			goto error_with_fd;
		}
	}

	/* Register mq between app & driver */
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct iwrl6432_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;
	g_mmwave_mq = mq_open(MQ_PATH, O_RDWR | O_CREAT, 0644, &attr);
	if (g_mmwave_mq == NULL) {
		printf("mq_open failed!!\n");
		goto error_with_fd;
	}

	ret = ioctl(mFd, SNIOC_REGISTERMQ, (unsigned long)g_mmwave_mq);
	if (ret < 0) {
		printf("register mq failed. errno : %d\n", errno);
		goto error_with_mq;
	}
	return OK;
error_with_mq:
	mq_close(MQ_PATH);
error_with_fd:
	close(mFd);
	for (int j = 0; j < i; j++) {
		if (gBuffer[j]) {
			free(gBuffer[j]);
			gBuffer[j] = NULL;
		}
	}
	if (gBuffer) {
		free(gBuffer);
		gBuffer = NULL;
	}
	return -1;
}

static void mmwave_teardown()
{
	close(mFd);

	mq_close(g_mmwave_mq);

	for (int i = 0; i < g_buf_num; i++) {
		if (gBuffer[i]) {
			free(gBuffer[i]);
			gBuffer[i] = NULL;
		}
	}
	if (gBuffer) {
		free(gBuffer);
		gBuffer = NULL;
	}
}

static int mmwave_show()
{
	int ret;
	FAR uint16_t *buffer = (FAR uint16_t *)malloc(4 * sizeof(uint16_t));
	ret = ioctl(mFd, SNIOC_SHOW, (unsigned long)buffer);
	if (ret < 0) {
		printf("show failed. errno : %d\n", errno);
		if (buffer) {
			free(buffer);
			buffer = NULL;
		}
		mmwave_teardown();
		return -1;
	}
	printf("=================================\n");
	printf("Last Succeed Frame Id: %d\n", buffer[0]);
	printf("Total Checksum error count: %d\n", buffer[1]);
	printf("Total timeout count: %d\n", buffer[2]);
	printf("Total Frame mismatch count: %d\n", buffer[3]);
	printf("=================================\n");
	if (buffer) {
		free(buffer);
		buffer = NULL;
	}
	return OK;
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
		mmwave_teardown();
		return -1;
	}

	while (!g_terminate) {

		size = mq_receive(g_mmwave_mq, (FAR char *)&msg, sizeof(msg), &prio);
		if (size != sizeof(msg)) {
			printf("wrong msg... size : %d sizeofmsg : %d\n", size, sizeof(msg));
			continue;
		}

		if (msg.msgId == IWRL6432_MSG_DEQUEUE) {
			// printf("Received value buffer : %x\n", msg.pData);
			struct iwrl6432_buf_s *buf = (struct iwrl6432_buf_s *)msg.pData;
			/* TODO memcpy data here to inference */
			if (buf->data[6] == 0x01) {
				printf("Received cloud point. Frame_id : %d\n", (buf->data[12] | buf->data[13] << 8));
			} else if (buf->data[6] == 0x02) {
				printf("Received cube no: %d. Frame_id: %d\n", buf->data[21] + 1, (buf->data[12] | buf->data[13] << 8));
			} else if (buf->data[6] == 0x03) {
				printf("Received Track data. Frame_id: %d\n",(buf->data[12] | buf->data[13] << 8));
			}
			// for (int i = 21;i < 1301; i += 13) {
			// printf("X Coordinate :%d Y Coordinate :%d Z Coordinate :%d Radial Velocity:%d\n", (buf->data[i] | buf->data[i+1] << 8), (buf->data[i+2] | buf->data[i+3] << 8), (buf->data[i+4] | buf->data[i+5] << 8), (buf->data[i+6] | buf->data[i+7] << 8));
			// }
			/* And then send buffer to driver again to enqueue buffer to queue */
			ret = ioctl(mFd, SNIOC_SENDBUFFER, (unsigned long)buf);
			if (ret < 0) {
				printf("get Buffer failed. errno : %d\n", errno);
				mmwave_teardown();
				return -1;
			}

		} else if (msg.msgId == IWRL6432_MSG_STOP_FORCELY) {
			printf("############# stop by forcely!!!!!!############\n");
		} else if (msg.msgId == IWRL6432_MSG_UNDERRUN) {
			/* TODO Cancel AI Model here?? */
		} else if (msg.msgId == IWRL6432_MSG_READY_TO_USE) {
			printf("############# We can use it again !!! ##############\n");
			ret = mmwave_prepare();
			if (ret < 0) {
				printf("prepare failed. errno : %d\n", errno);
				return -1;
			}
			ret = ioctl(mFd, SNIOC_START, NULL);
			if (ret < 0) {
				printf("start failed. errno : %d\n", errno);
				mmwave_teardown();
				return -1;
			}
		} else if (msg.msgId == IWRL6432_MSG_TIMEOUT) {
			printf("############# Timeout!!!!!!############\n");
		}
	}
	/* Stop Collect */
	ret = ioctl(mFd, SNIOC_STOP, NULL);
	if (ret < 0) {
		printf("stop failed. errno : %d\n", errno);
		mmwave_teardown();
		return -1;
	}
	mmwave_teardown();
	return OK;
}

static int mmwave_stop()
{
	g_terminate = true;
	return OK;
}

/** TODO below must be implemented
 * 1. App should alloc buffer based on result of ioctl(SNIOC_GET_BUFSIZE, SNIOC_GET_BUFNUM)
 * 2. App should share point of these buffers with driver. App can share more size than SNIOC_GET_BUFSIZE
 * 3. pBuffer in struct iwrl6432_dev_s should points these buffers.
 *    Because buffer in kernel can't be accessed by app, so driver should not have its own buffer.
 * 4. so some kind of SNIOC_ENQUEUE_BUFFER is required.
 */
static int mmwave_prepare()
{
	int ret;
	/* Share Buffer with Driver */
	for (int i = 0; i < g_buf_num; i++) {
		ret = ioctl(mFd, SNIOC_SENDBUFFER, (unsigned long)gBuffer[i]);
		if (ret < 0) {
			printf("get Buffer failed. errno : %d\n", errno);
			mmwave_teardown();
			return -1;
		}
	}

	/* Then prepare */
	ret = ioctl(mFd, SNIOC_PREPARE, NULL);
	if (ret < 0) {
		printf("prepare failed. errno : %d\n", errno);
		mmwave_teardown();
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
		} else if (!strncmp(argv[1], "show", 5)) {
			return mmwave_show();
		} else {
			show_usage();
		}
	}
	return OK;
}
