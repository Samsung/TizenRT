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
#include "tc_common.h"

#if defined(CONFIG_TASH) && !defined(CONFIG_BUILTIN_APPS)
#include <apps/shell/tash.h>
#else
#if defined(CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_ITC)
#define TC_ARASTORAGE_STACK       4096
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_AUDIO_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_AUDIO_ITC)
#define TC_AUDIO_STACK  4096
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_DM_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_DM_ITC)
#define TC_DM_STACK  2048
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_DRIVERS
#define TC_DRIVERS_STACK   2048
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_FILESYSTEM
#define TC_FS_STACK       4096
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_KERNEL
#define TC_KERNEL_STACK   2048
#endif
#ifdef CONFIG_LIBCXX_UTC
#define TC_LIBCXX_STACK  4096
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_MEDIA_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_MEDIA_ITC)	
#define TC_MEDIA_STACK  8192	
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MPU
#define TC_MPU_STACK   2048
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_MQTT_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_MQTT_ITC)
#define TC_MQTT_STACK 4096
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_NETWORK
#define TC_NETWORK_STACK  2048
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_ITC)
#define TC_SYSTEMIO_STACK 2048
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_TASK_MANAGER_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_TASK_MANAGER_ITC)
#define TC_TASK_MANAGER_STACK  2048
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TTRACE
#define TC_TTRACE_STACK  2048
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC) || defined(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC)
#define TC_WIFI_MANAGER_STACK  4096
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TCP_TLS_STRESS
#define TC_TCP_TLS_STACK 8192
#endif
#endif

sem_t tc_sem;
int working_tc;
int total_pass;
int total_fail;

/* Library&Environment Test Case as le_tc*/
extern int tc_drivers_main(int argc, char *argv[]);
extern int tc_filesystem_main(int argc, char *argv[]);
extern int tc_kernel_main(int argc, char *argv[]);
extern int tc_network_main(int argc, char *argv[]);
extern int tc_ttrace_main(int argc, char *argv[]);
extern int tc_tcp_tls_main(int agrc, char *agrv[]);

/* TinyAra Public API Test Case as ta_tc */
extern int utc_arastorage_main(int argc, char *argv[]);
extern int itc_arastorage_main(int argc, char *argv[]);
extern int utc_audio_main(int argc, char *argv[]);
extern int itc_audio_main(int argc, char *argv[]);
extern int utc_dm_main(int argc, char *argv[]);
extern int itc_dm_main(int argc, char *argv[]);
extern int utc_media_main(int argc, char *argv[]);
extern int itc_media_main(int argc, char *argv[]);
extern int utc_mqtt_main(int argc, char *argv[]);
extern int itc_mqtt_main(int argc, char *argv[]);
extern int utc_sysio_main(int argc, char *argv[]);
extern int itc_sysio_main(int argc, char *argv[]);
extern int utc_task_manager_main(int argc, char *argv[]);
extern int itc_task_manager_main(int argc, char *argv[]);
extern int utc_wifi_manager_main(int argc, char *argv[]);
extern int itc_wifi_manager_main(int argc, char *argv[]);

/* Not yet */
extern int tc_mpu_main(int argc, char *argv[]);

/* Libc++ Testcase */
#ifdef CONFIG_LIBCXX_UTC
extern int utc_libcxx_main(int argc, char *argv[]);
#endif

#if defined(CONFIG_TASH) && !defined(CONFIG_BUILTIN_APPS)
static const tash_cmdlist_t tc_cmds[] = {
#ifdef CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_UTC
	{"arastorage_utc", utc_arastorage_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_ITC
	{"arastorage_itc", itc_arastorage_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_AUDIO_UTC
	{"audio_utc", utc_audio_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_AUDIO_ITC
	{"audio_itc", itc_audio_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_DM_UTC
	{"dm_utc", utc_dm_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_DM_ITC
	{"dm_itc", itc_dm_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_DRIVERS
	{"drivers_tc", tc_drivers_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_FILESYSTEM
	{"filesystem_tc", tc_filesystem_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_KERNEL
	{"kernel_tc", tc_kernel_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_LIBCXX_UTC
	{"libcxx_utc", utc_libcxx_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MEDIA_UTC	
	{"media_utc", utc_media_main, TASH_EXECMD_ASYNC},	
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MEDIA_ITC	
	{"media_itc", itc_media_main, TASH_EXECMD_ASYNC},	
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MPU
	{"mpu_tc", tc_mpu_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MQTT_UTC
	{"mqtt_utc", utc_mqtt_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MQTT_ITC
	{"mqtt_itc", itc_mqtt_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_NETWORK
	{"network_tc", tc_network_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_UTC
	{"sysio_utc", utc_sysio_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_ITC
	{"sysio_itc", itc_sysio_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TASK_MANAGER_UTC
	{"taskmgr_utc", utc_task_manager_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TASK_MANAGER_ITC
	{"taskmgr_itc", itc_task_manager_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TTRACE
	{"ttrace_tc", tc_ttrace_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC
	{"wifi_manager_utc", utc_wifi_manager_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC
	{"wifi_manager_itc", itc_wifi_manager_main, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TCP_TLS_STRESS
	{"tcp_tls_stress", tc_tcp_tls_main, TASH_EXECMD_ASYNC},
#endif
	{NULL, NULL, 0}
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
int tc_handler(tc_op_type_t type, const char *tc_name)
{
	switch (type) {
	case TC_START:
		if (working_tc == 0) {
			sem_init(&tc_sem, 0, 1);
		}

		working_tc++;
		if (sem_wait(&tc_sem) == ERROR) {
			printf("Fail to start \"%s\" due to semaphore\n", tc_name);
			working_tc--;
			return ERROR;
		}

		total_pass = 0;
		total_fail = 0;

		printf("\n########## %s Start ##########\n", tc_name);
		break;
	case TC_END:
#ifndef CONFIG_EXAMPLES_TESTCASE_TCP_TLS_STRESS
		printf("\n########## %s End [PASS : %d, FAIL : %d] ##########\n", tc_name, total_pass, total_fail);
#endif
		working_tc--;
		sem_post(&tc_sem);

		if (working_tc == 0) {
			sem_destroy(&tc_sem);
		}
		break;
	default:
		printf("Invalid testcase execution type\n");
		return ERROR;
		break;
	}

	return OK;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_main(int argc, char *argv[])
#endif
{
#if defined(CONFIG_TASH)
#if !defined(CONFIG_BUILTIN_APPS)
	tash_cmdlist_install(tc_cmds);
#endif
	printf("\nTestcase registers TASH commands named \"<MODULE_NAME>_tc\".\nPlease find them using \"help\" and execute them in TASH\n");
#else // !CONFIG_TASH
	int pid;

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
#ifdef CONFIG_EXAMPLES_TESTCASE_AUDIO_UTC
	pid = task_create("audioutc", SCHED_PRIORITY_DEFAULT, TC_AUDIO_STACK, utc_audio_main, argv);
	if (pid < 0) {
		printf("Audio utc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_AUDIO_ITC
	pid = task_create("audioitc", SCHED_PRIORITY_DEFAULT, TC_AUDIO_STACK, itc_audio_main, argv);
	if (pid < 0) {
		printf("Audio itc is not started, err = %d\n", pid);
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
#ifdef CONFIG_EXAMPLES_TESTCASE_DRIVERS
	pid = task_create("driverstc", SCHED_PRIORITY_DEFAULT, TC_DRIVERS_STACK, tc_drivers_main, argv);
	if (pid < 0) {
		printf("Drivers tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_FILESYSTEM
	pid = task_create("fstc", SCHED_PRIORITY_DEFAULT, TC_FS_STACK, tc_filesystem_main, argv);
	if (pid < 0) {
		printf("FS tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_KERNEL
	pid = task_create("kerneltc", SCHED_PRIORITY_DEFAULT, TC_KERNEL_STACK, tc_kernel_main, argv);
	if (pid < 0) {
		printf("Kernel tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_LIBCXX_UTC
	pid = task_create("libcxxutc", SCHED_PRIORITY_DEFAULT, TC_LIBCXX_STACK, utc_libcxx_main, argv);
	if (pid < 0) {
		printf("Libcxx utc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MEDIA_UTC
	pid = task_create("mediautc", SCHED_PRIORITY_DEFAULT, TC_MEDIA_STACK, utc_media_main, argv);
	if (pid < 0) {
		printf("Media utc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MEDIA_ITC
	pid = task_create("mediaitc", SCHED_PRIORITY_DEFAULT, TC_MEDIA_STACK, itc_media_main, argv);
	if (pid < 0) {
		printf("Media itc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MPU
	pid = task_create("mputc", SCHED_PRIORITY_DEFAULT, TC_MPU_STACK, tc_mpu_main, argv);
	if (pid < 0) {
		printf("MPU tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MQTT_UTC
	pid = task_create("mqttutc", SCHED_PRIORITY_DEFAULT, TC_MQTT_STACK, utc_mqtt_main, argv);
	if (pid < 0) {
		printf("MQTT utc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_MQTT_ITC
	pid = task_create("mqttitc", SCHED_PRIORITY_DEFAULT, TC_MQTT_STACK, itc_mqtt_main, argv);
	if (pid < 0) {
		printf("MQTT itc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_NETWORK
	pid = task_create("nettc", SCHED_PRIORITY_DEFAULT, TC_NETWORK_STACK, tc_network_main, argv);
	if (pid < 0) {
		printf("Network tc is not started, err = %d\n", pid);
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
#ifdef CONFIG_EXAMPLES_TESTCASE_TASK_MANAGER_UTC
	pid = task_create("taskmgrutc", SCHED_PRIORITY_DEFAULT, TC_TASK_MANAGER_STACK, utc_task_manager_main, argv);
	if (pid < 0) {
		printf("Task Manager utc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TTRACE
	pid = task_create("ttracetc", SCHED_PRIORITY_DEFAULT, TC_TTRACE_STACK, tc_ttrace_main, argv);
	if (pid < 0) {
		printf("T-trace tc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC
	pid = task_create("wifimgrutc", SCHED_PRIORITY_DEFAULT, TC_WIFI_MANAGER_STACK, utc_wifi_manager_main, argv);
	if (pid < 0) {
		printf("Wi-Fi Manager utc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC
	pid = task_create("wifimgritc", SCHED_PRIORITY_DEFAULT, TC_WIFI_MANAGER_STACK, itc_wifi_manager_main, argv);
	if (pid < 0) {
		printf("Wi-Fi Manager itc is not started, err = %d\n", pid);
	}
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE_TCP_TLS_STRESS
	pid = task_create("tcptlstc", SCHED_PRIORITY_DEFAULT, TC_TCP_TLS_STACK, tc_tcp_tls_main, argv);
	if (pid < 0) {
		printf("TCP TLS STRESS testcase is not started, err %d\n", pid);
	}
#endif

	printf("All Testcases finished\n");
#endif

	return 0;
}
#ifdef __cplusplus
}
#endif
