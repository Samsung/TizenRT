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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <pthread.h>

static void *assert_thread(void *index)
{
	printf("[%d] %dth thread, assert!\n", getpid(), (int)index);

	PANIC();
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

	pid	= task_create("normal", 100, 1024, normal_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}

	pid	= task_create("assert_group_main", 100, 1024, assert_group_main_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}

	while (1);

	return 0;
}


#ifdef CONFIG_EXAMPLES_MPU_TEST

#if !defined(CONFIG_MPUTEST_KERNEL_CODE_ADDR) || !defined(CONFIG_MPUTEST_KERNEL_DATA_ADDR) || !defined(CONFIG_MPUTEST_APP_ADDR)
#error "Address not defined for MPU TEST"
#endif

void perform_mpu_test()
{
#ifdef CONFIG_EXAMPLES_MPU_TEST
	volatile uint32_t *address;
	char ch;

	printf("\nPress R - For Read Test\n");
	printf("Press W - For Write Test\n");

	ch = getchar();
	if (ch == 'R' || ch == 'r') {
		printf("\nPress A for app MPU test\n");
		printf("Press K for kernel MPU test\n");

		ch = getchar();

		if (ch == 'A' || ch == 'a') {
			address = CONFIG_MPUTEST_APP_ADDR;

			uint32_t dest;

			printf("\n************************************************\n");
			printf("************************************************\n");
			printf("************************************************\n");
			printf("* Test to verify protection of Micom app space *\n");
			printf("************************************************\n");
			printf("************************************************\n");
			printf("************************************************\n");

			printf("INFO: Read Micom app space: 0x%x\n", address);
			sleep(1);
			dest = *address;

			printf("ERROR: Wifi Task made invalid access to Micom app space\n");
		} else {

			printf("\nPress C - For Read Code Test\n");
			printf("Press D - For Read Data Test\n");

			ch = getchar();
			if (ch == 'C' || ch == 'c') {
				address = CONFIG_MPUTEST_KERNEL_CODE_ADDR;

				uint32_t dest;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel code     *\n");
				printf("* User Tasks should not be allowed to read     *\n");
				printf("* kernel code space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Read Code: 0x%x\n", address);

				sleep(1);
				dest = *address;

				printf("ERROR: User Task made invalid access to Kernel code space\n");
			} else if (ch == 'D' || ch == 'd') {
			       address = CONFIG_MPUTEST_KERNEL_DATA_ADDR;

				uint32_t dest;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel data     *\n");
				printf("* User Tasks should not be allowed to read     *\n");
				printf("* kernel data space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Read Data: 0x%x\n", address);

				sleep(1);
				dest = *address;


				printf("ERROR: User Task made invalid access to Kernel data space\n");
			}
		}
	} else if (ch == 'W' || ch == 'w') {

		printf("\nPress A for app MPU test\n");
		printf("Press K for kernel MPU test\n");

		ch = getchar();

		if (ch == 'A' || ch == 'a') {
			address = CONFIG_MPUTEST_APP_ADDR;

			uint32_t dest = 0xdeadbeef;

			printf("\n************************************************\n");
			printf("************************************************\n");
			printf("************************************************\n");
			printf("* Test to verify protection of Micom app space *\n");
			printf("************************************************\n");
			printf("************************************************\n");
			printf("************************************************\n");
			printf("INFO: Write Micom app space: 0x%x\n", address);

			sleep(1);
			*address = dest;

			printf("ERROR: Wifi Task made invalid access to Micom app space\n");
		} else {

			printf("\nPress C - For Write Kernel Code Test\n");
			printf("Press D - For Write Kernel Data Test\n");

			ch = getchar();
			if (ch == 'C' || ch == 'c') {
				address = CONFIG_MPUTEST_KERNEL_CODE_ADDR;
				uint32_t dest = 0xdeadbeef;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel code     *\n");
				printf("* User Tasks should not be allowed to write    *\n");
				printf("* kernel code space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Write kernel space: 0x%x\n", address);

				sleep(1);
				*address = dest;

				printf("ERROR: User Task made invalid access to Kernel code space\n");
			} else if (ch == 'D' || ch == 'd') {
				address = CONFIG_MPUTEST_KERNEL_DATA_ADDR;
				uint32_t dest = 0xdeadbeef;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel data     *\n");
				printf("* User Tasks should not be allowed to write    *\n");
				printf("* kernel data space. MPU shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Write kernel space: 0x%x\n", address);

				sleep(1);
				*address = dest;

				printf("ERROR: User Task made invalid access to Kernel data space\n");
			}
		}
	}

#endif

}

#else

#define perform_mpu_test()
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, char **argv)
{
	int pid;

	perform_mpu_test();

	pid = task_create("mkchildren", 100, 1024, make_children_task, (FAR char *const *)NULL);
	if (pid < 0) {
		printf("task create FAIL\n");
		return 0;
	}
	printf("I'm WIFI main! create mkchildren task %d\n", pid);

	while (1) {
		sleep(10);
		printf("[%d] WIFI ALIVE\n", getpid());
	}
	return 0;
}
