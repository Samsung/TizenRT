/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sched.h>
#include <unistd.h>

/****************************************************************************
 * Private Function
 ****************************************************************************/

#if !defined(CONFIG_BUILD_KERNEL)
static int stress_task_create_term_exit(int argc, char *argv[])
{
	return 0;
}

static int stress_task_create_term_sleep(int argc, char *argv[])
{
	while (1) {
		sleep(10);
	}

	return 0;
}

static int stress_task_create_term_loop(int cpu)
{
	pid_t pid;

#ifdef CONFIG_SMP
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) < 0) {
		return -1;
	}
#endif

	while (1) {
		pid = task_create("stress_task_create_term_exit", SCHED_PRIORITY_DEFAULT, 1024, stress_task_create_term_exit, NULL);
		if (pid < 0) {
			return -1;
		}

		sched_yield();
		task_delete(pid);

		pid = task_create("stress_task_create_term_sleep", SCHED_PRIORITY_DEFAULT, 1024, stress_task_create_term_sleep, NULL);
		if (pid < 0) {
			return -1;
		}

		task_delete(pid);
	}
}

static int stress_task_create_term_cpu0(int argc, char *argv[])
{
	return stress_task_create_term_loop(0);
}

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
static int stress_task_create_term_cpu1(int argc, char *argv[])
{
	return stress_task_create_term_loop(1);
}
#endif
#endif

/****************************************************************************
 * Name: stress_task_create_term_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int stress_task_create_term_main(int argc, char *argv[])
#endif
{
#if !defined(CONFIG_BUILD_KERNEL)
	pid_t cpu0_pid;
#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	pid_t cpu1_pid;
#endif

	cpu0_pid = task_create("stress_task_create_term_cpu0", SCHED_PRIORITY_DEFAULT, 1024, stress_task_create_term_cpu0, NULL);
	if (cpu0_pid < 0) {
		return -1;
	}

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	cpu1_pid = task_create("stress_task_create_term_cpu1", SCHED_PRIORITY_DEFAULT, 1024, stress_task_create_term_cpu1, NULL);
	if (cpu1_pid < 0) {
		task_delete(cpu0_pid);
		return -1;
	}
#endif

	while (1) {
		sleep(10);
	}
#else
	return -1;
#endif

	return 0;
}
