/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <dirent.h>
#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
#include <pthread.h>
#endif
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <errno.h>
#include <sys/mount.h>
#endif
#include <sys/types.h>
#include <tinyara/clock.h>
#include <tinyara/sched.h>
#include <tinyara/fs/fs.h>

#include "kdbg_utils.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
#define CPULOADMON_PREFIX "CPUload Monitor: "

#define CPULOADMONITOR_STACKSIZE 1024

#ifndef CONFIG_CPULOADMONITOR_PRIORITY
#define CONFIG_CPULOADMONITOR_PRIORITY 100
#endif

#ifndef CONFIG_CPULOADMONITOR_INTERVAL
#define CONFIG_CPULOADMONITOR_INTERVAL 5
#endif
#endif

extern volatile uint32_t g_cpuload_timeconstant[SCHED_NCPULOAD];

/****************************************************************************
 * Private Data
 ****************************************************************************/
#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
static volatile bool is_started = false;
static pthread_t cpuloadmon;
#endif

#define CPULOAD_BUFLEN 64

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int cpuload_read_proc(FAR struct dirent *entryp, FAR void *arg)
{
	int ret;
	char *filepath;
	char buf[CPULOAD_BUFLEN];

	printf("  %s	", entryp->d_name);
	asprintf(&filepath, "%s/%s/%s", PROCFS_MOUNT_POINT, entryp->d_name, "loadavg");
	ret = kdbg_readfile(filepath, buf, CPULOAD_BUFLEN, NULL);
	free(filepath);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		return ERROR;
	}
	printf("\n");

	return OK;
}

static void cpuload_print_tasklist(void)
{
	int cpuload_idx;

#ifdef CONFIG_SCHED_MULTI_CPULOAD
	printf("=====================================\n");
#else
	printf("=================\n");
#endif
	printf(" CPU USAGE\n");
#ifdef CONFIG_SCHED_MULTI_CPULOAD
	printf("=====================================\n");
#else
	printf("=================\n");
#endif
	printf(" PID");
	for (cpuload_idx = 0; cpuload_idx < SCHED_NCPULOAD; cpuload_idx++) {
		printf("%9ds", g_cpuload_timeconstant[cpuload_idx]);
	}
#ifdef CONFIG_SCHED_MULTI_CPULOAD
	printf("\n-------------------------------------\n");
#else
	printf("\n-----------------\n");
#endif

	/* Print cpu load for each task */
	kdbg_proc_pid_foreach(cpuload_read_proc);

#ifdef CONFIG_SCHED_MULTI_CPULOAD
	printf("=====================================\n");
#else
	printf("=================\n");
#endif
}

#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
static void *cpuloadmonitor_daemon(void *args)
{
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	int ret;
	bool is_mounted;

	is_mounted = false;

	/* Mount Procfs to use */
	ret = mount(NULL, PROCFS_MOUNT_POINT, PROCFS_FSTYPE, 0, NULL);
	if (ret == ERROR) {
		if (errno == EEXIST) {
			is_mounted = true;
		} else {
			printf("Failed to mount procfs : %d\n", errno);
			return NULL;
		}
	}
#endif

	/* Loop until we detect that there is a request to stop. */
	while (is_started) {
		cpuload_print_tasklist();
		sleep(CONFIG_CPULOADMONITOR_INTERVAL);
	}

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif

	return NULL;
}

static void cpuload_daemon_start(void)
{
	int ret;
	pthread_attr_t attr;

	if (is_started) {
		printf(CPULOADMON_PREFIX "Already started\n");
		return;
	}

	pthread_attr_init(&attr);
	attr.stacksize = CPULOADMONITOR_STACKSIZE;
	attr.priority = CONFIG_CPULOADMONITOR_PRIORITY;
	attr.inheritsched = PTHREAD_EXPLICIT_SCHED;
	
	ret = pthread_create(&cpuloadmon, &attr, cpuloadmonitor_daemon, NULL);
	if (ret != OK) {
		printf(CPULOADMON_PREFIX "Failed to start the cpuload monitor: %d\n", ret);
		return;
	}
	pthread_setname_np(cpuloadmon, "CPULoadMonitor");

	ret = pthread_detach(cpuloadmon);
	if (ret != OK) {
		printf(CPULOADMON_PREFIX "Failed to detach the cpuload monitor: %d\n", ret);
		pthread_cancel(cpuloadmon);
		return;
	}

	is_started = true;
}

static void cpuload_daemon_stop(void)
{
	if (is_started) {
		/* Stopped */
		pthread_cancel(cpuloadmon);
		is_started = false;
		printf(CPULOADMON_PREFIX "CPU load Monitor Stopped\n");
	} else {
		printf(CPULOADMON_PREFIX "CPU load Monitor is not activated.\n");
	}
}
#endif

static void show_usage(void)
{
	printf("\nUsage: cpuload\n");
#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
	printf("    Or, cpuload <start|stop>\n");
#endif
	printf("Print CPU load\n");
#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
	printf("Or start/stop CPU load monitor daemon\n");
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int kdbg_cpuload(int argc, char **args)
{
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	int ret;
	bool is_mounted;

	is_mounted = false;
#endif
	if (argc > 2) {
		show_usage();
		return ERROR;
	}

	if (argc == 1) {
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
		/* Mount Procfs to use */
		ret = mount(NULL, PROCFS_MOUNT_POINT, PROCFS_FSTYPE, 0, NULL);
		if (ret == ERROR) {
			if (errno == EEXIST) {
				is_mounted = true;
			} else {
				printf("Failed to mount procfs : %d\n", errno);
				return ERROR;
			}
		}
#endif
		/* Print CPU load once */
		cpuload_print_tasklist();

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
		if (!is_mounted) {
			/* Detach mounted Procfs */
			(void)umount(PROCFS_MOUNT_POINT);
		}
#endif
#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
	} else if (!strncmp(args[1], "start", strlen("start") + 1)) {
		/* Start the cpuload monitor */
		cpuload_daemon_start();
	} else if (!strncmp(args[1], "stop", strlen("stop") + 1)) {
		/* Stop the cpuload monitor */
		cpuload_daemon_stop();
#endif
	} else {
		show_usage();
		return ERROR;
	}
	return OK;
}
