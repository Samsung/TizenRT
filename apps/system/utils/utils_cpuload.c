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
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
#include <errno.h>
#include <sys/mount.h>
#endif
#include <sys/ioctl.h>
#include <sys/types.h>
#include <tinyara/clock.h>
#include <tinyara/cpuload.h>
#include <tinyara/sched.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include "utils_proc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CPULOADMON_PREFIX "CPUload Monitor: "

#define CPULOADMONITOR_STACKSIZE 2048

#ifndef CONFIG_CPULOADMONITOR_PRIORITY
#define CONFIG_CPULOADMONITOR_PRIORITY 100
#endif

#ifndef CONFIG_CPULOADMONITOR_INTERVAL
#define CONFIG_CPULOADMONITOR_INTERVAL 5
#endif

#define CPULOAD_BUFLEN 128
#define CPULOADMON_RUNNING_FOREVER -1

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct cpuload_pidhash_s {
	int pid;
	int count;
	bool active_flag;
};

enum cpuload_mode {
	CPULOAD_NORMAL,
	CPULOAD_SNAPSHOT,
};

static volatile bool is_started = false;
static pthread_t cpuloadmon;
static int cpuload_snapfd;
static int cpuload_mode;
static int cpuload_count;
static int cpuload_interval;
static int cpuload_snapintval;
static pid_t *cpuload_snaparr;
static int cpuload_snaparr_size;
struct cpuload_pidhash_s cpuload_pidhash[CONFIG_MAX_TASKS + 1];

/* The last index is for dead threads */
#define CPULOAD_INACTIVE_IDX CONFIG_MAX_TASKS

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void cpuload_stop(void)
{
	if (is_started) {
		/* Stopped */
		if (cpuload_snapfd > 0) {
			(void)ioctl(cpuload_snapfd, CPULOADIOC_STOP, (unsigned long)NULL);
			free(cpuload_snaparr);
		}
		is_started = false;
		printf(CPULOADMON_PREFIX "CPU load Monitor Stopped\n");
		pthread_cancel(cpuloadmon);
	} else {
		printf(CPULOADMON_PREFIX "CPU load Monitor is not activated.\n");
	}
}

static void cpuload_print_pid_value(char *buf, void *arg)
{
	int i;
	int pid;
	int pid_hash;
	double load_ratio;
	stat_data stat_info[PROC_STAT_MAX];

	stat_info[0] = buf;

	for (i = 0; i < PROC_STAT_MAX - 1; i++) {
		stat_info[i] = strtok_r(stat_info[i], " ", &stat_info[i + 1]);
	}

	printf("%3s | %3s |", stat_info[PROC_STAT_PID], stat_info[PROC_STAT_PRIORITY]);
	if (cpuload_mode == CPULOAD_SNAPSHOT) {
		pid = atoi(stat_info[PROC_STAT_PID]);
		pid_hash = PIDHASH(pid);
		cpuload_pidhash[pid_hash].active_flag = true;
		if (pid != cpuload_pidhash[pid_hash].pid) {
			cpuload_pidhash[CPULOAD_INACTIVE_IDX].count += cpuload_pidhash[pid_hash].count;
			cpuload_pidhash[pid_hash].count = 0;
		}
		load_ratio = (double)(100 * cpuload_pidhash[pid_hash].count) / cpuload_snaparr_size;
		printf(" %3d(%4.1f) |", cpuload_pidhash[pid_hash].count, load_ratio);
	} else {
#ifdef CONFIG_SCHED_MULTI_CPULOAD
		printf(" %5s | %5s | %5s |", stat_info[PROC_STAT_CPULOAD_SHORT], stat_info[PROC_STAT_CPULOAD_MID], stat_info[PROC_STAT_CPULOAD_LONG]);
#else
		printf(" %5s |", stat_info[PROC_STAT_CPULOAD]);
#endif
	}
#if (CONFIG_TASK_NAME_SIZE > 0)
	printf(" %s", stat_info[PROC_STAT_NAME]);
#endif
}

static int cpuload_read_proc(FAR struct dirent *entryp, FAR void *arg)
{
	int ret;
	char *filepath;
	char buf[CPULOAD_BUFLEN];

	asprintf(&filepath, "%s/%s/%s", PROCFS_MOUNT_POINT, entryp->d_name, "stat");
	ret = utils_readfile(filepath, buf, CPULOAD_BUFLEN, cpuload_print_pid_value, NULL);
	free(filepath);
	if (ret < 0) {
		printf("Failed to read %s\n", filepath);
		return ERROR;
	}
	printf("\n");
	return OK;
}

static void cpuload_print_normal(void)
{
	/* Print titles */
	printf("PID | Pri |");
#ifdef CONFIG_SCHED_MULTI_CPULOAD
	printf("%5ds | %4ds | %4ds |", CONFIG_SCHED_CPULOAD_TIMECONSTANT_SHORT, CONFIG_SCHED_CPULOAD_TIMECONSTANT_MID, CONFIG_SCHED_CPULOAD_TIMECONSTANT_LONG);
#else
	printf("%5ds |", CONFIG_SCHED_CPULOAD_TIMECONSTANT);
#endif
	printf("\n--------------------------------------------------\n");

	/* Print cpuload for each task */
	utils_proc_pid_foreach(cpuload_read_proc, NULL);
	printf("--------------------------------------------------\n");
}

static void cpuload_print_snapshot(void)
{
	int ret;
	int pid_idx;
	int tick_idx;

	/* Get snapshot measurement */
	ret = ioctl(cpuload_snapfd, CPULOADIOC_GETVALUE, (unsigned long)cpuload_snaparr);
	if (ret < 0) {
		cpuload_stop();
	}

	/* Initialize tick counts for threads */
	for (pid_idx = 0; pid_idx <= CONFIG_MAX_TASKS; pid_idx++) {
		cpuload_pidhash[pid_idx].active_flag = false;
		cpuload_pidhash[pid_idx].count = 0;
	}

	/* Caculate ticks of each thread */
	for (tick_idx = 0; tick_idx < cpuload_snaparr_size; tick_idx++) {
		pid_idx = PIDHASH(cpuload_snaparr[tick_idx]);
		cpuload_pidhash[pid_idx].pid = cpuload_snaparr[tick_idx];
		cpuload_pidhash[pid_idx].count++;
	}

	/* Print titles */
	printf("PID | Pri |  Snap ticks  |");
	printf("\n--------------------------------------------------\n");

	/* Print cpuload for each task */
	utils_proc_pid_foreach(cpuload_read_proc, NULL);

	/* Caclate uncounted ticks for dead thread when reading proc entry */
	for (pid_idx = 0; pid_idx < CONFIG_MAX_TASKS; pid_idx++) {
		if (!cpuload_pidhash[pid_idx].active_flag) {
			cpuload_pidhash[CPULOAD_INACTIVE_IDX].count += cpuload_pidhash[pid_idx].count;
		}
	}

	if (cpuload_pidhash[CPULOAD_INACTIVE_IDX].count > 0) {
		printf(" * Ticks of dead threads : %d\n", cpuload_pidhash[CPULOAD_INACTIVE_IDX].count);
	}
	printf(" * Snapshot interval : %ds (%d ticks)\n", cpuload_snapintval, cpuload_snaparr_size);
	printf("--------------------------------------------------\n");
}

static void *cpuload_monitor(void *args)
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
	/* Start to print data after snapshot interval(s) when snapshot mode. */
	if (cpuload_mode == CPULOAD_SNAPSHOT) {
		sleep(cpuload_snapintval);
	}

	/* Loop until we detect that there is a request to stop. */
	while (is_started) {
		if (cpuload_mode == CPULOAD_SNAPSHOT) {
			cpuload_print_snapshot();
		} else {
			cpuload_print_normal();
		}
		if (cpuload_count > 0) {
			if (--cpuload_count == 0) {
				cpuload_stop();
				break;
			}
		}
		sleep(cpuload_interval);
	}

#if !defined(CONFIG_FS_AUTOMOUNT_PROCFS)
	if (!is_mounted) {
		/* Detach mounted Procfs */
		(void)umount(PROCFS_MOUNT_POINT);
	}
#endif

	return NULL;
}

static int cpuload_start(void)
{
	int ret;
	pthread_attr_t attr;

	cpuload_snapfd = -1;
	cpuload_snaparr = NULL;

	if (cpuload_mode == CPULOAD_SNAPSHOT) {
		/* Allocate data buffer for CPU load measurements in time interval */
		cpuload_snaparr_size = cpuload_snapintval * CLOCKS_PER_SEC;
		cpuload_snaparr = (pid_t *)malloc(cpuload_snaparr_size * sizeof(pid_t));
		if (cpuload_snaparr == NULL) {
			printf("Fail to allocate buffer for snapshot, errno %d\n", errno);
			return ERROR;
		}
		/* Open cpuload driver */
		cpuload_snapfd = open(CPULOAD_DRVPATH, O_RDWR);
		if (cpuload_snapfd < 0) {
			printf("Fail to open cpuload driver. errno %d\n", errno);
			goto errout_with_free;
		}
		/* Start snapshot measurement */
		ret = ioctl(cpuload_snapfd, CPULOADIOC_START, (unsigned long)cpuload_snapintval);
		if (ret < 0) {
			printf("Fail to start cpuload measurement. errno %d\n", errno);
			close(cpuload_snapfd);
			goto errout_with_free;
		}
	}

	/* Create cpuload moniter thread */
	pthread_attr_init(&attr);
	attr.stacksize = CPULOADMONITOR_STACKSIZE;
	attr.priority = CONFIG_CPULOADMONITOR_PRIORITY;

	ret = pthread_create(&cpuloadmon, &attr, cpuload_monitor, NULL);
	if (ret != OK) {
		printf(CPULOADMON_PREFIX "Failed to start the cpuload monitor: %d\n", ret);
		goto errout_with_close;
	}
	ret = pthread_detach(cpuloadmon);
	if (ret != OK) {
		printf(CPULOADMON_PREFIX "Failed to detach the cpuload monitor: %d\n", ret);
		pthread_cancel(cpuloadmon);
		goto errout_with_close;
	}
	pthread_setname_np(cpuloadmon, "CPULoadMonitor");

	if (cpuload_mode == CPULOAD_SNAPSHOT) {
		printf("CPU monitor will started after %ds with interval %d.\n", cpuload_snapintval, cpuload_interval);
	} else {
		printf("Started CPU monitor with interval %d.\n", cpuload_interval);
	}

	return OK;
errout_with_close:
	if (cpuload_snapfd > 0) {
		(void)ioctl(cpuload_snapfd, CPULOADIOC_STOP, (unsigned long)NULL);
		close(cpuload_snapfd);
	}
errout_with_free:
	if (cpuload_snaparr != NULL) {
		free(cpuload_snaparr);
		cpuload_snaparr = NULL;
	}
	return ERROR;
}

static void cpuload_show_usage(void)
{
	printf("\nUsage: cpuload [-s <snapshot interval(s)>] [-i <print interval(s)>] [-n <iterations>]\n");
	printf("    Or, cpuload stop\n");
	printf("Start/Stop CPU load monitor daemon\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int utils_cpuload(int argc, char **args)
{
	int opt;
	int ret;
	long value;

	if (argc >= 2) {
		if (!strncmp(args[1], "--help", strlen("--help") + 1)) {
			goto show_usage;
		}

		if (!strncmp(args[1], "stop", strlen("stop") + 1)) {
			/* Stop the cpuload monitor */
			cpuload_stop();
			return OK;
		}
	}

	if (is_started) {
		printf(CPULOADMON_PREFIX "Already started\n");
		return ERROR;
	}

	cpuload_mode = CPULOAD_NORMAL;
	cpuload_interval = CONFIG_CPULOADMONITOR_INTERVAL;
	cpuload_count = CPULOADMON_RUNNING_FOREVER;

	if (argc > 1) {
		/*
		 * -s [snapshot interval] : set snapshot interval (s)
		 * -i [interval] : set interval of cpuload daemon (s)
		 * -n [iterations] : set count of iterations
		 *
		 * For example,
		 *  TASH >> cpuload -s 60 -i 10
		 *  CPU monitor starts with snapshot mode and shows measured values every 10 seconds.
		 */
		while ((opt = getopt(argc, args, "s:i:n:")) != ERROR) {
			switch (opt) {
			case 's':
				/* set snapshot interval */
				cpuload_snapintval = atoi(optarg);
				if (cpuload_snapintval <= 0 || cpuload_snapintval == LONG_MAX) {
					printf("Invalid input");
					goto show_usage;
				}
				cpuload_mode = CPULOAD_SNAPSHOT;
				break;
			case 'i':
				/* set interval for showing measured data */
				value = atoi(optarg);
				if (value <= 0 || value == LONG_MAX) {
					printf("Invalid input");
					goto show_usage;
				}
				cpuload_interval = value;
				break;
			case 'n':
				/* set count of iterations */
				value = atoi(optarg);
				if (value <= 0 || value == LONG_MAX) {
					printf("Invalid input");
					goto show_usage;
				}
				cpuload_count = value;
				break;
			default:
				printf("Invalid input");
				goto show_usage;
			}
		}
	}
	/* Set started flag to avoid creation more than 1 daemon by another request */
	is_started = true;

	/* Start cpuload monitor */
	ret = cpuload_start();
	if (ret < 0) {
		is_started = false;
	}
	return ret;
show_usage:
	cpuload_show_usage();

	return ERROR;
}
