/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/kernel_test_drv.h>
#include <tinyara/mpu_test.h>

static void *assert_thread(void *index)
{
	int type;
	int tc_fd = open(KERNEL_TEST_DRVPATH, O_WRONLY);
	struct mputest_arg_s obj;
	int ret;

	if (tc_fd < 0) {
		printf("\nFAILED to open kernel test driver %s\n", KERNEL_TEST_DRVPATH);
		return NULL;
	}

	type = getpid() % 3;
	if (type == 0) {
		/* PANIC */
		printf("[%d] %dth thread, PANIC!\n", getpid(), (int)index);
		sleep(1);
		PANIC();
	} else if (type == 1) {
		/* Access kernel code */
		obj.type = MPUTEST_KERNEL_CODE;
		ret = ioctl(tc_fd, TESTIOC_MPUTEST, (unsigned long)&obj);
		if (ret < 0) {
			printf("ERROR: Failed to obtain address from kernel\n");
			close(tc_fd);
			return NULL;
		}

		printf("[%d] %dth thread, Write kernel code space 0x%x\n", getpid(), (int)index, obj.addr);
		sleep(1);
		*(obj.addr) = 0xdeadbeef;
	} else {
		/* Access another binary 'micom' address */
		obj.type = MPUTEST_APP_ADDR;
		ret = ioctl(tc_fd, TESTIOC_MPUTEST, (unsigned long)&obj);
		if (ret < 0) {
			printf("ERROR: Failed to obtain address from kernel\n");
			close(tc_fd);
			return NULL;
		}

		printf("[%d] %dth thread, Write another app space 0x%x\n", getpid(), (int)index, obj.addr);
		sleep(1);
		*(obj.addr) = 0xdeadbeef;
	}

	close(tc_fd);
	return 0;
}

static void *normal_thread(void *index)
{
	printf("[%d] %dth thread, normal thread\n", getpid(), (int)index);

	while (1);
	return 0;
}

static int assert_group_main_task(int argc, char *argv[])
{
	int count;
	pthread_t thd;
	pthread_attr_t attr;

	printf("[%d] assert_group_main_task \n", getpid());

	pthread_attr_init(&attr);

	for (count = 0; count < 2; count++) {
		pthread_create(&thd, &attr, (pthread_startroutine_t)normal_thread, (pthread_addr_t)count);
	}
	pthread_create(&thd, &attr, (pthread_startroutine_t)assert_thread, (pthread_addr_t)count);

	while (1);

	return 0;
}

static int normal_task(int argc, char *argv[])
{
	printf("[%d] normal_task \n", getpid());

	while (1);

	return 0;
}

static int make_children_task(int argc, char *argv[])
{
	int pid;

	printf("[%d] make_children_task \n", getpid());

	pid = task_create("normal", 100, 1024, normal_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}

	pid = task_create("assert_group_main", 100, 1024, assert_group_main_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}

	while (1);

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void recovery_test(void)
{
	int pid;

	pid = task_create("mkchildren", 100, 1024, make_children_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return;
	}
	printf("I'm RECOVERY main! create mkchildren task %d\n", pid);
}
