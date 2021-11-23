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
 * examples/hello/hello_main.c
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
#include <tinyara/config.h>

#include <sys/time.h>
#include <time.h>
#include <tinyara/timer.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
/****************************************************************************
 * hello_main
 ****************************************************************************/

#define REQ_URL ""
#define WRITE_SIZE 512

struct message {
	int fd;
	int received;
	int check;
};

static int g_num = 0;
static char g_buf[128 * 100]; // 51KB?
static int g_write_size = 16;
static int g_islseek = 0;
static int g_write_log = 0;
static int g_td;
static char g_char = 0xee;

static int _write_flash(int fd, char *buf, int buf_size)
{
	int left = buf_size;
	int offset = 0;
	while (left > 0) {

		/* struct timespec start, end; */
		/* clock_gettime(CLOCK_MONOTONIC, &start); */
		/* printf("start: %f, %f\n", (double)start.tv_sec, (double)start.tv_nsec); */

		/* int len = write(fd, buf + offset, buf_size - offset); */
		/* clock_gettime(CLOCK_MONOTONIC, &end); */
		/* double diffTime = ((double)end.tv_sec + 1.0e-9 * end.tv_nsec) - ((double)start.tv_sec + 1.0e-9 * start.tv_nsec); */
		/* printf("Diff Time: %f\n", diffTime); */
		struct timer_status_s start, end;
		uint32_t elapsed;
		ioctl(g_td, TCIOC_SETMODE, MODE_FREERUN);
		ioctl(g_td, TCIOC_START, 0);
		ioctl(g_td, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&start);
		int len = write(fd, buf + offset, buf_size - offset);
		ioctl(g_td, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&end);
		ioctl(g_td, TCIOC_STOP, 0);
		elapsed = end.timeleft - start.timeleft;
		if (g_write_log == 1) {
			printf("Diff time(FLT) %u ms\n", elapsed);
		}
		if (len == -1) {
			int err_no = get_errno();
			if (err_no == EINTR || err_no == EAGAIN) {
				continue;
			}
			lldbg("errno %d\n", err_no);
			assert(0);
		}

		offset += len;
		left -= len;
		//printf("written (%d/%d)\n", len, buf_size);
	}
	return 0;
}

static size_t curl_callback(void *contents, size_t size, size_t nmemb,
							void *userp)
{
	printf("size %lu  nmemb %lu\n", size, nmemb);
	struct message *msg = (struct message *)userp;
	size_t left = size * nmemb;
	int offset = 0;
	msg->received += (size * nmemb);

	while (left > 0) {
		int buf_size = left > WRITE_SIZE ? WRITE_SIZE : left;
		_write_flash(msg->fd, contents + offset, buf_size);
		//printf("written (%d/%d)\n", offset, size * nmemb);
		offset += buf_size;
		left -= buf_size;
	}
	printf("written (%d/%d)\n", size * nmemb, msg->received);
	return size * nmemb;
}

int _download(void)
{
	CURL *curl;
	CURLcode res;
	struct message msg = {0, 0, 0};

	int fd = open("/dev/mtdblock12", O_RDWR, 0666);
	if (fd == -1) {
		printf("[pkbuild] open fail %d %s:%d\n", errno, __FUNCTION__, __LINE__);
		return -1;
	}
	msg.fd = fd;
	msg.check = 0;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
		curl_easy_setopt(curl, CURLOPT_URL, REQ_URL);
		struct curl_slist *list = NULL;
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1); // get response header?
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&msg);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		} else {
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
		curl_slist_free_all(list);
	}
	curl_global_cleanup();
	close(fd);
	return 0;
}

static int _write_flashing(void)
{
	int fd = open("/dev/mtdblock12", O_RDWR, 0666);
	if (fd == -1) {
		printf("[pkbuild] open fail %d %s:%d\n", errno, __FUNCTION__, __LINE__);
		return -1;
	}

	int left = sizeof(g_buf);
	int offset = 0;
	printf("write %d bytes write size %d lseek %d\n", left, g_write_size, g_islseek);
	while (left > 0) {
		int buf_size = left > g_write_size ? g_write_size : left;
		if (g_islseek != 0) {
			int ret = lseek(fd, offset, SEEK_SET);
			if (ret == -1) {
				lldbg("lseek fail %d\n", errno);
				assert(0);
			}
		}
		_write_flash(fd, g_buf + offset, buf_size);
		offset += buf_size;
		left -= buf_size;
		if (g_write_log == 1) {
			//printf("flash write (%d/%d/%d)\n", buf_size, offset, sizeof(g_buf));
		}
	}
	printf("written (%d/%d)\n", offset, sizeof(g_buf));
	close(fd);
	return 0;
}

void _wt_process(int argc, char *argv[])
{
#if 0
	for (int i = 0; i < g_num; i++) {
		int res = _download();
		printf("=================================\n\n");
		printf("Count (%d/%d) res(%d)\n\n", i + 1, g_num, res);
		printf("=================================\n\n");
	}
#endif
	for (int i = 0; i < sizeof(g_buf); i++) {
		g_buf[i] = g_char;
	}
	printf("=================================\n\n");
	printf("Flashing Start total %d buf_size %d write_size %d lseek(%s) log(%s)\n\n",
		   g_num, sizeof(g_buf), g_write_size,
		   (g_islseek == 1 ? "YES" : "NO"),
		   (g_write_log == 1 ? "YES" : "NO"));
	printf("=================================\n\n");

	g_td = open("/dev/timer0", O_RDONLY);
	if (g_td < 0) {
		assert(0);
	}

	for (int i = 0; i < g_num; i++) {
		//struct timeval start, end;
		//gettimeofday(&start, NULL);
		int res = _write_flashing();
		//gettimeofday(&end, NULL);
		printf("=================================\n\n");
		printf("Flashing Count (%d/%d) res(%d) \n\n",
			   i + 1, g_num, res);
		printf("=================================\n\n");
	}
	close(g_td);
}

int _read_flash(int fd, char *buf, int buf_size)
{
	int left = buf_size;
	int offset = 0;
	while (left > 0) {
		int len = read(fd, buf + offset, buf_size - offset);
		if (len == -1) {
			int err_no = get_errno();
			if (err_no == EINTR || err_no == EAGAIN) {
				continue;
			}
			lldbg("errno %d\n", err_no);
			assert(0);
		}

		offset += len;
		left -= len;
	}
	return 0;
}

void _flash_read(void)
{
	printf("=======================\n");
	printf("_read flash\n");
	printf("=======================\n\n");

	char buf[128];
	int buf_size = 128;
	int fd = open("/dev/mtdblock12", O_RDWR, 0666);
	if (fd == -1) {
		printf("[pkbuild] open fail %d %s:%d\n", errno, __FUNCTION__, __LINE__);
		return;
	}

	int left = sizeof(g_buf);
	int offset = 0;
	while (left > 0) {
		int read_size = left > buf_size ? buf_size : left;
		(void)_read_flash(fd, buf, read_size);
		offset += read_size;
		left -= read_size;
		for (int i = 0; i < read_size; i++) {
			printf("%02x ", buf[i]);
		}
		printf("\n");
	}
	close(fd);
	return;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	if (argc == 1) {
		printf("Hello, World!!\n");
		return 1;
	}

	/*  curl test */
	if (argc == 2) {
		g_num = atoi(argv[1]);
	}
	else if (argc == 3) {
		_flash_read();
	}
	/*  flashing test */
	/*  command count lseek(0, 1) write_size(Bytes) log(0, 1) character(to write)*/
	else if (argc == 6) {
		g_num = atoi(argv[1]);
		g_islseek = atoi(argv[2]);
		if (g_islseek != 1 && g_islseek != 0) {
			assert(0);
		}
		g_write_size = atoi(argv[3]);
		g_write_log = atoi(argv[4]);
		g_char = atoi(argv[5]);
		task_create("test sample", 100, 1024 * 10, (main_t)_wt_process, argv);
	}
	return 0;
}
