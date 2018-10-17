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
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
#include <pthread.h>
#endif
#include <sys/types.h>
#include <tinyara/clock.h>
#include <tinyara/sched.h>
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

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void cpuload_print_task(struct tcb_s *tcb, void *arg)
{
	struct cpuload_s cpuload;
	uint32_t intpart;
	uint32_t fracpart;
	int cpuload_idx;
	uint32_t tmp;

	printf("  %d    ", tcb->pid);
	for (cpuload_idx = 0; cpuload_idx < SCHED_NCPULOAD; cpuload_idx++) {

		(void)clock_cpuload(tcb->pid, cpuload_idx, &cpuload);

		if (cpuload.total > 0) {
			tmp = (1000 * cpuload.active) / cpuload.total;
			intpart = tmp / 10;
			fracpart = tmp - 10 * intpart;
		} else {
			intpart = 0;
			fracpart = 0;
		}
		printf("%3d.%d %%   ", intpart, fracpart);
	}
	printf("\n");
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
	sched_foreach(cpuload_print_task, NULL);

#ifdef CONFIG_SCHED_MULTI_CPULOAD
	printf("=====================================\n");
#else
	printf("=================\n");
#endif
}

#ifdef CONFIG_ENABLE_CPULOAD_MONITOR
static void *cpuloadmonitor_daemon(void *args)
{
	/* Loop until we detect that there is a request to stop. */
	while (is_started) {
		cpuload_print_tasklist();		
		sleep(CONFIG_CPULOADMONITOR_INTERVAL);
	}

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
	if (argc > 2) {
		show_usage();
		return ERROR;
	}

	if (argc == 1) {
		/* Print CPU load once */
		cpuload_print_tasklist();
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
