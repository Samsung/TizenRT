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
 * examples/sensor_test/sensor_main.c
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
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <tinyara/sensors/sensor.h>
#include <tinyara/rtc.h>

#ifdef CONFIG_SENSOR_AIS25BA
#define MEMS_SENSOR_PATH "/dev/sensor-mems"
#define MEMS_MQ_PATH "mems_mq"
#define EXAMPLE_SENSOR_FPS_TEST 5000

bool g_terminate;
static mqd_t g_mems_mq;
static int mems_fd;
static struct ais25ba_buf_s **gBuffer;
int buf_size; //size of segment buffer
int g_buf_num; //segment number of ais25ba

static bool is_sensor_prepared = false;

static void mems_teardown();

static void print_sensor_data(sensor_data_s *data)
{
	for (int i = 0; i < AIS25BA_BUFLENGTH; i++) {
		printf("x: %f, y: %f, z: %f\n", data[i].x, data[i].y, data[i].z);
	}
}

static int mems_sensor_init()
{
	struct mq_attr attr;
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct mems_sensor_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;
	int ret;

	mems_fd = open(MEMS_SENSOR_PATH, O_RDWR | O_SYNC, 0666);
	if (mems_fd < 0) {
		printf("ERROR: Failed to open sensor port error:%d\n", mems_fd);
		return ERROR;
	}

	ioctl(mems_fd, SENSOR_SET_SAMPRATE, AIS25BA_SAMPLE_RATE);
	g_mems_mq = mq_open(MEMS_MQ_PATH, O_RDWR | O_CREAT, 0644, &attr);
	if (g_mems_mq == NULL) {
		printf("ERROR: Sensor, mq_open failed!!\n");
		goto error_with_mq;
	}
	ret = ioctl(mems_fd, SENSOR_GET_BUFSIZE, (unsigned long)&buf_size);
	if (ret < 0) {
		printf("ERROR: Sensor, get Buffer size failed. errno : %d\n", errno);
		goto error_with_fd;
	}
	ret = ioctl(mems_fd, SENSOR_GET_BUFNUM, (unsigned long)&g_buf_num);
	if (ret < 0) {
		printf("ERROR: Sensor, get Buffer number failed. errno : %d\n", errno);
		goto error_with_fd;
	}

	ret = ioctl(mems_fd, SENSOR_REGISTERMQ, (unsigned long)g_mems_mq);
	if (ret < 0) {
		printf("ERROR: Sensor, register mq failed. errno : %d\n", errno);
		goto error_with_mq;
	}

	return OK;
error_with_mq:
	mq_close(MEMS_MQ_PATH);
error_with_fd:
	close(mems_fd);
	for (int j = 0; j < g_buf_num; j++) {
		if (gBuffer[j]) {
			free(gBuffer[j]);
			gBuffer[j] = NULL;
		}
	}
	if (gBuffer) {
		free(gBuffer);
		gBuffer = NULL;
	}
	return ERROR;
}

static int mems_sensor_prepare()
{
	if (is_sensor_prepared) {
			printf("Sensor already prepared, call sensor start\n");
			return;
	}
	g_terminate = true;
	int ret;
	mems_sensor_init();
	/* Alloc array of pointers to gBuffers */
	gBuffer = (FAR struct ais25ba_buf_s **)malloc(g_buf_num * sizeof(FAR void *));
	if (gBuffer == NULL) {
		printf("ERROR: Sensor, Alloc gBuffer failed\n");
		return ERROR;
	}

	for (int i = 0; i < g_buf_num; i++) {
		gBuffer[i] = (FAR struct ais25ba_buf_s *)malloc(sizeof(FAR struct ais25ba_buf_s) * AIS25BA_BUFLENGTH);
		if (gBuffer[i] == NULL) {
			mems_teardown();
			printf("gBuffer alloc failed\n");
			return ERROR;
		}
	}

	/* Share Buffer with Driver */
	for (int i = 0; i < g_buf_num; i++) {
		ret = ioctl(mems_fd, SENSOR_SENDBUFFER, (unsigned long)gBuffer[i]);
		if (ret != OK) {
			printf("get Buffer failed. errno : %d\n", errno);
			mems_teardown();
			return ERROR;
		}
	}
	is_sensor_prepared = true;
	printf("Sensor prepare success\n");
	return OK;
}

static int mems_sensor_start()
{
	if (!is_sensor_prepared) {
			printf("Sensor is not prepared, call sensor prepare\n");
			return ERROR;
	}
	int ret;
	struct mems_sensor_msg_s msg;
	int prio;
	size_t size;

	mems_sensor_init();
	/* Start Collect */
	ret = ioctl(mems_fd, SENSOR_START, NULL);
	if (ret < 0) {
		printf("ERROR: MEMS sensor start failed, errno: %d\n", errno);
		mems_teardown();
		return ERROR;
	}
	g_terminate = false;
	while (1) {
		if (g_terminate == true) {
			return OK;
		}

		size = mq_receive(g_mems_mq, (FAR char *)&msg, sizeof(msg), &prio);
		if (size != sizeof(msg)) {
			printf("ERROR: wrong msg, size: %d, sizeofmsg: %d\n", size, sizeof(msg));
			continue;
		}
		
		struct ais25ba_buf_s *buf = (struct ais25ba_buf_s *)msg.data;
		sensor_data_s *buffer = (sensor_data_s *)buf->data;
		print_sensor_data(buffer);
		//sleep(1);
		ret = ioctl(mems_fd, SENSOR_SENDBUFFER, (unsigned long)buf);
		if (ret != OK) {
			printf("get Buffer failed. errno : %d\n", errno);
			mems_teardown();
			return ERROR;
		}
	}

	ret = ioctl(mems_fd, SENSOR_STOP, NULL);
	if (ret != OK) {
		printf("Error: sensor stop failed. errno : %d\n", errno);
		mems_teardown();
		return ERROR;
	}
	mems_teardown();
	return OK;
}

static int mems_sensor_stop()
{
	if (is_sensor_prepared == false) {
			printf("ERROR: Sensor is not running\n");
			return ERROR;
	}
	is_sensor_prepared = false;
	mems_fd = open(MEMS_SENSOR_PATH, O_RDWR | O_SYNC, 0666);
	if (mems_fd < 0) {
		printf("ERROR: Failed to open sensor port error:%d\n", mems_fd);
		return;
	}
	int ret = ioctl(mems_fd, SENSOR_STOP, NULL);
	usleep(10000);
	mems_teardown();
	return ret;
}

static void mems_teardown()
{
	g_terminate = true;
	close(mems_fd);

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

static void show_usage(void)
{
	printf("\nusage: sensor <command #>\n");
	printf("Excute sensor testing or controling.\n\n");
	printf("	prepare : Prepare the sensor operation \n");
	printf("	start   : Start reading sensor data\n");
	printf("	stop	: Stop Reading sensor data\n");
	printf("	fpstest : Time it takes to get single data from sensor\n");
	printf("	readinfinite: Get data from sensor using read(), MQ not used\n");
	printf("	show    : Show sensor stats\n");
}

static int sensor_read()
{
	sensor_data_s *data = (sensor_data_s *)malloc(sizeof(sensor_data_s)*64);
	if (data == NULL) {
		printf("ERROR: Malloc failed in example app\n");
		return ERROR;
	}
	mems_fd = open(MEMS_SENSOR_PATH, O_RDWR | O_SYNC, 0666);
	if (mems_fd < 0) {
		printf("ERROR: Failed to open sensor port error:%d\n", mems_fd);
		return ERROR;
	}

	ioctl(mems_fd, SENSOR_SET_SAMPRATE, AIS25BA_SAMPLE_RATE);
	while (true) {
		read(mems_fd, data, 2);
		print_sensor_data(data);
	}
	close(mems_fd);
	return OK;
}

static int sensor_fps_test()
{
	int status;
	printf("Sensor FPS test start\n");
	printf("Waiting...\n");
	int fd_rtc = open("/dev/rtc0", O_RDWR);
	if (fd_rtc < 0) {
		printf("ERROR RTC\n");
		return ERROR;
	}
	struct rtc_time start_time = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	struct rtc_time end_time;

	sensor_data_s *data = (sensor_data_s *)malloc(sizeof(sensor_data_s)*64);
	mems_fd = open(MEMS_SENSOR_PATH, O_RDWR | O_SYNC, 0666);
	if (mems_fd < 0) {
		printf("ERROR: Failed to open sensor port error:%d\n", mems_fd);
		close(fd_rtc);
		return ERROR;
	}

	int count = EXAMPLE_SENSOR_FPS_TEST;
	ioctl(mems_fd, SENSOR_SET_SAMPRATE, AIS25BA_SAMPLE_RATE);
	status = ioctl(fd_rtc, RTC_RD_TIME, (unsigned long)&start_time);
	if (status < 0) {
		printf("ERROR: RTC ioctl failed RTC_RD_TIME, status: %d\n", status);
		close(fd_rtc);
		close(mems_fd);
		return ERROR;
	}
	while (count--) {
		read(mems_fd, data, 2);
	}
	status = ioctl(fd_rtc, RTC_RD_TIME, (unsigned long)&end_time);
	if (status < 0) {
		printf("ERROR: RTC ioctl failed RTC_RD_TIME, status: %d\n", status);
		close(fd_rtc);
		close(mems_fd);
		return ERROR;
	}

	printf("sensor fps test complete \n");
	close(mems_fd);
	close(fd_rtc);

	time_t start;
	time_t end;
	start = mktime((FAR struct tm *)&start_time);
	end = mktime((FAR struct tm *)&end_time);

	int time_elapsed = difftime(end, start);
	float fps = (time_elapsed * 1000) / EXAMPLE_SENSOR_FPS_TEST;
	printf("Total frames executed: %d, Total Time elapsed: %d\nTime elapsed per frame: %.2f ms\n\n", EXAMPLE_SENSOR_FPS_TEST, time_elapsed, fps);
	return OK;
}

static int sensor_show()
{
	mems_fd = open(MEMS_SENSOR_PATH, O_RDWR | O_SYNC, 0666);
	if (mems_fd < 0) {
		printf("ERROR: Failed to open sensor port error:%d\n", mems_fd);
		return ERROR;
	}

	sensor_info_s sensor_info;
	int status = OK;
	status = ioctl(mems_fd, SENSOR_SHOW, &sensor_info);
	if (status != OK) {
		printf("ERROR: IOCTL SENSOR_SHOW fail, errorno: %d\n", status);
		close(mems_fd);
		return ERROR;
	}

	printf("\nSENSOR INFO:\n");
	printf("Total count of Alive checks done on sensor: %d\n", sensor_info.alive_check_count);
	printf("Alive check fail count: %d\n", sensor_info.alive_check_fail_count);
	printf("Number of Frames received from sensor: %d\n", sensor_info.total_frame_receive_count);
	printf("Number of Frames received since last start: %d\n\n", sensor_info.total_frame_last_start);
	return OK;
}
#endif

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sensor_main(int argc, char *argv[])
#endif
{
	printf("Sensor test!!\n");
#ifdef CONFIG_SENSOR_AIS25BA

	if (argc <= 1 || !strncmp(argv[1], "-h", 2) || !strncmp(argv[1], "--help", 6)) {
		show_usage();
		return OK;
	}
	int status = OK;

	if (argc == 2) {
		if (!strncmp(argv[1], "prepare", 8)) {
			status = mems_sensor_prepare();
		} else if (!strncmp(argv[1], "start", 6)) {
			status = mems_sensor_start();
		} else if (!strncmp(argv[1], "stop", 5)) {
			status = mems_sensor_stop();
		} else if (!strncmp(argv[1], "fpstest", 8)) {
			status = sensor_fps_test();
		} else if (!strncmp(argv[1], "readinfinite", 13)) {
			status = sensor_read();
		} else if (!strncmp(argv[1], "show", 5)) {
			status = sensor_show();
		} else {
			show_usage();
		}
	} else {
		show_usage();
	}
	if (status != OK) {
		printf("Sensor test %s failed\n", argv[1]);
	}

#endif
	return 0;
}
