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

/// @file tc_main.c

/// @brief Main Function for TestCase Example
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <semaphore.h>

#if defined(CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_ITC)
#define TC_ARASTORAGE_STACK       4096
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_FILESYSTEM
#define TC_FS_STACK       4096
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_KERNEL
#define TC_KERNEL_STACK   2048
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MPU
#define TC_MPU_STACK   2048
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_NETWORK
#define TC_NETWORK_STACK  2048
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TTRACE
#define TC_TTRACE_STACK  2048
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_ITC)
#define TC_SYSTEMIO_STACK 2048
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_DM_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_DM_ITC)
#define TC_DM_STACK  20480
#endif

sem_t tc_sem;
int working_tc;

/* Library&Environment Test Case as le_tc*/
extern int fs_main(int argc, char *argv[]);
extern int kernel_tc_main(int argc, char *argv[]);
extern int network_tc_main(int argc, char *argv[]);
extern int ttrace_tc_main(int argc, char *argv[]);

/* TinyAra Public API Test Case as ta_tc */
extern int utc_arastorage_main(int argc, char *argv[]);
extern int itc_arastorage_main(int argc, char *argv[]);
extern int utc_sysio_main(int argc, char *argv[]);
extern int itc_sysio_main(int argc, char *argv[]);
extern int utc_dm_main(int argc, char *argv[]);
extern int itc_dm_main(int argc, char *argv[]);

/* Not yet */
extern int mpu_tc_main(int argc, char *argv[]);

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_main(int argc, char *argv[])
#endif
{
	int pid;

	sem_init(&tc_sem, 0, 1);

#ifdef CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_UTC
	pid = task_create("arastorageutc", SCHED_PRIORITY_DEFAULT, TC_ARASTORAGE_STACK, utc_arastorage_main, argv);
	if (pid < 0) {
		printf("Arastorage utc is not started, err = %d\n", pid);
	}
#endif

#ifdef CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_ITC
	pid = task_create("arastorageitc", SCHED_PRIORITY_DEFAULT, TC_ARASTORAGE_STACK, itc_arastorage_main, argv);
	if (pid < 0) {
		printf("Arastorage itc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_FILESYSTEM
	pid = task_create("fstc", SCHED_PRIORITY_DEFAULT, TC_FS_STACK, fs_main, argv);
	if (pid < 0) {
		printf("FS tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_KERNEL
	pid = task_create("kerneltc", SCHED_PRIORITY_DEFAULT, TC_KERNEL_STACK, kernel_tc_main, argv);
	if (pid < 0) {
		printf("Kernel tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MPU
	pid = task_create("mputc", SCHED_PRIORITY_DEFAULT, TC_MPU_STACK, mpu_tc_main, argv);
	if (pid < 0) {
		printf("MPU tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_NETWORK
	pid = task_create("nettc", SCHED_PRIORITY_DEFAULT, TC_NETWORK_STACK, network_tc_main, argv);
	if (pid < 0) {
		printf("Network tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TTRACE
	pid = task_create("ttracetc", SCHED_PRIORITY_DEFAULT, TC_TTRACE_STACK, ttrace_tc_main, argv);
	if (pid < 0) {
		printf("T-trace tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_DM_UTC
	pid = task_create("dmapi", SCHED_PRIORITY_DEFAULT, TC_DM_STACK, utc_dm_main, argv);
	if (pid < 0) {
		printf("DM utc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_DM_ITC
	pid = task_create("dmapi", SCHED_PRIORITY_DEFAULT, TC_DM_STACK, itc_dm_main, argv);
	if (pid < 0) {
		printf("DM itc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_UTC
	pid = task_create("sysioutc", SCHED_PRIORITY_DEFAULT, TC_SYSTEMIO_STACK, utc_sysio_main, argv);
	if (pid < 0) {
		printf("System IO utc is not started, err = %d\n", pid);
	}
#endif

#ifdef CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_ITC
	pid = task_create("sysioitc", SCHED_PRIORITY_DEFAULT, TC_SYSTEMIO_STACK, itc_sysio_main, argv);
	if (pid < 0) {
		printf("System IO itc is not started, err = %d\n", pid);
	}
#endif
	do {
		sleep(5);
	} while (working_tc > 0);
	(void)sem_destroy(&tc_sem);

	printf("TC is finished\n");

	return 0;
}
