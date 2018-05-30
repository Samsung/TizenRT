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

/// @file tc_roundrobin.c

/// @brief Test Case Example for Roundrobin Features

/****************************************************************************
* Included Files
****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include "../../../../../os/kernel/clock/clock.h"
#include "tc_internal.h"

/****************************************************************************
* Definitions
****************************************************************************/
#define LOOP     10
#define LOG_SIZE 400
#define TESTCASE 3
#if CONFIG_MAX_TASKS > 16
#define NTHREAD  5
#else
#define NTHREAD  3
#endif
#define MAX_TASKS_MASK      (CONFIG_MAX_TASKS-1)
#define PIDHASH(pid)        ((pid) & MAX_TASKS_MASK)
/****************************************************************************
* Private Data
****************************************************************************/
pid_t log_pid[LOG_SIZE];
int log_timeslice[LOG_SIZE];
volatile int logidx;
int start, created;
int pid_prio[CONFIG_MAX_TASKS];
#if CONFIG_MAX_TASKS > 16
int priority[TESTCASE][NTHREAD] = { {90, 100, 100, 100, 110}, {110, 120, 120, 120, 130}, {70, 80, 80, 80, 90} };
#else
int priority[TESTCASE][NTHREAD] = { {90, 100, 110}, {110, 120, 130}, {70, 80, 90} };
#endif

pthread_attr_t attr;
struct sched_param param;

/****************************************************************************
* Public Functions
****************************************************************************/

static int pthread_attr_set(void)
{
	int status;
	if (attr.stacksize == 512 && attr.policy == SCHED_RR && attr.inheritsched == PTHREAD_EXPLICIT_SCHED) {
		return OK;
	} else {
		status = pthread_attr_init(&attr);
		if (status != OK) {
			return ERROR;
		}

		status = pthread_attr_setstacksize(&attr, 512);
		if (status != OK) {
			return ERROR;
		}

		status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		if (status != OK) {
			return ERROR;
		}

		status = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
		if (status != OK) {
			return ERROR;
		}
		return OK;
	}
}

/* check_log looks log[i] and log[i+1]. When there is log[i], check_log judge whether log[i+1] is available. */
static int check_log(void)
{
	int chk_log_cnt;
	int cmp_log_cnt;
	int pid_loop_cnt;
	int pid_loop[CONFIG_MAX_TASKS];
	int order[NTHREAD];
	int idx = 0;
	order[idx] = log_pid[0];
	idx++;

	for (pid_loop_cnt = 0; pid_loop_cnt < CONFIG_MAX_TASKS; pid_loop_cnt++) {
		pid_loop[pid_loop_cnt] = 0;
	}
	for (chk_log_cnt = 0; chk_log_cnt < logidx - 1; chk_log_cnt++) {
		pid_loop[log_pid[chk_log_cnt]]++;
		if (log_timeslice[chk_log_cnt] > 1 && pid_loop[log_pid[chk_log_cnt]] < LOOP) {
			if (log_pid[chk_log_cnt] != log_pid[chk_log_cnt + 1]) {
				/* unexpected context switching occured */
				return ERROR;
			}
		} else if (pid_prio[log_pid[chk_log_cnt]] < pid_prio[log_pid[chk_log_cnt + 1]]) {
			/* low priority task comes before high priority task */
			return ERROR;
		} else if (pid_prio[log_pid[chk_log_cnt]] > pid_prio[log_pid[chk_log_cnt + 1]]) {
			for (cmp_log_cnt = 0; cmp_log_cnt < idx; cmp_log_cnt++) {
				if (pid_loop[order[cmp_log_cnt]] < LOOP) {
					/* there remains uncompleted high priority task */
					return ERROR;
				}
			}
			order[0] = log_pid[chk_log_cnt + 1];
			idx = 1;
		} else {
			/* if pid_prio[log_pid[i]] == pid_prio[log_pid[i+1]]
			 * order is order of roundrobin in same priority task */
			for (cmp_log_cnt = 0; cmp_log_cnt < idx; cmp_log_cnt++) {
				if (order[cmp_log_cnt] == log_pid[chk_log_cnt + 1]) {
					break;
				}
			}
			if (cmp_log_cnt == idx) {
				order[idx] = log_pid[chk_log_cnt + 1];
				idx++;
			}

			if (cmp_log_cnt == 0) {
				if (order[idx - 1] != log_pid[chk_log_cnt]) {
					/* the order of same priority task/pthread is wrong */
					return ERROR;
				}
			} else {
				if (order[cmp_log_cnt - 1] != log_pid[chk_log_cnt]) {
					/* the order of same priority task/pthread is wrong */
					return ERROR;
				}
			}
		}
	}
	pid_loop[log_pid[logidx - 1]]++;
	return OK;
}

static void write_log(void)
{
	int prev = -1;
	int timeslice_cnt = 0;
	volatile struct tcb_s *cur_tcb = sched_self();
	do {
		usleep(1);
	} while (start < created);
	while (timeslice_cnt < LOOP) {
		if (prev != cur_tcb->timeslice) {
			log_pid[logidx] = PIDHASH(cur_tcb->pid);
			log_timeslice[logidx] = cur_tcb->timeslice;
			logidx++;
			timeslice_cnt++;
			prev = cur_tcb->timeslice;
		}
	}
}

static pthread_addr_t pthread_func(void *arg)
{
	start++;
	write_log();
	pthread_exit((pthread_addr_t)OK);
	return (pthread_addr_t)OK;
}

static int task_func(int argc, char *argv[])
{
	start++;
	write_log();
	task_delete(0);
	return OK;
}

static int taskNpthread_func(int argc, char *argv[])
{
	int thread_cnt;
	int status;
	int tc = (int)(argv[1][0]);
	pthread_t pth[NTHREAD - 1];
	start++;
	for (thread_cnt = 0; thread_cnt < NTHREAD - 1; thread_cnt++) {
		param.sched_priority = priority[tc][thread_cnt + 1];
		status = pthread_attr_setschedparam(&attr, &param);
		if (status != OK) {
			printf("tc_roundrobin_rr_taskNpthread FAIL : pthread_attr_setschedparam failed, status=%d\n", status);
			break;
		}
		status = pthread_create(&pth[thread_cnt], &attr, pthread_func, NULL);
		if (status != OK) {
			printf("tc_roundrobin_rr_taskNpthread FAIL : pthread_create failed, status=%d\n", status);
			break;
		} else {
			created++;
			pid_prio[PIDHASH(pth[thread_cnt])] = priority[tc][thread_cnt + 1];
		}
	}
	write_log();
	for (thread_cnt = 0; thread_cnt < NTHREAD - 1; thread_cnt++) {
		pthread_join(pth[thread_cnt], 0);
	}
	task_delete(0);
	return OK;
}

static void tc_roundrobin_rr_pthread(void)
{
	int tc;
	int ret;
	ret = pthread_attr_set();
	TC_ASSERT_NEQ("pthread_attr_set", ret, ERROR);

	for (tc = 0; tc < TESTCASE; tc++) {
		int pthread_cnt;
		pthread_t pth[NTHREAD];
		logidx = 0;
		start = created = 0;
		sleep(1);
		for (pthread_cnt = 0; pthread_cnt < NTHREAD; pthread_cnt++) {
			param.sched_priority = priority[tc][pthread_cnt];
			ret = pthread_attr_setschedparam(&attr, &param);
			TC_ASSERT_EQ("pthread_attr_setschedparam", ret, OK);

			ret = pthread_create(&pth[pthread_cnt], &attr, pthread_func, NULL);
			TC_ASSERT_EQ("pthread_create ", ret, OK);

			created++;
			pid_prio[PIDHASH(pth[pthread_cnt])] = priority[tc][pthread_cnt];
		}

		for (pthread_cnt = 0; pthread_cnt < NTHREAD; pthread_cnt++) {
			pthread_join(pth[pthread_cnt], 0);
		}

		ret = check_log();
		TC_ASSERT_EQ("check_log", ret, OK);
	}

	TC_SUCCESS_RESULT();
}

static void tc_roundrobin_rr_task(void)
{
	int tc;
	int ret;
	ret = pthread_attr_set();
	TC_ASSERT_NEQ("pthread_attr_set", ret, ERROR);

	for (tc = 0; tc < TESTCASE; tc++) {
		int task_cnt;
		pid_t task_pid[NTHREAD];
		logidx = 0;
		start = created = 0;
		sleep(1);
		for (task_cnt = 0; task_cnt < NTHREAD; task_cnt++) {
			task_pid[task_cnt] = task_create("rrtask", priority[tc][task_cnt], 1024, task_func, NULL);
			TC_ASSERT_NEQ("task_create", task_pid[task_cnt], ERROR);

			created++;
			param.sched_priority = priority[tc][task_cnt];
			ret = sched_setscheduler(task_pid[task_cnt], SCHED_RR, &param);
			TC_ASSERT_EQ("sched_setscheduler", ret, OK);

			pid_prio[PIDHASH(task_pid[task_cnt])] = priority[tc][task_cnt];
		}

		for (task_cnt = 0; task_cnt < NTHREAD; task_cnt++) {
			if (task_pid[task_cnt] != ERROR) {
				FAR struct tcb_s *ptr;
				while ((ptr = sched_gettcb(task_pid[task_cnt])) != NULL) {
					usleep(100);
				}
			}
		}

		ret = check_log();
		TC_ASSERT_EQ("check_log", ret, OK);
	}

	TC_SUCCESS_RESULT();
}

static void tc_roundrobin_rr_taskNpthread(void)
{
	int tc;
	int ret;
	ret = pthread_attr_set();
	TC_ASSERT_NEQ("pthread_attr_set", ret, ERROR);

	for (tc = 0; tc < TESTCASE; tc++) {
		int task_pid;
		FAR char *argv[2];
		char arg[2] = { tc, '\0' };
		argv[0] = arg;
		argv[1] = NULL;
		logidx = 0;
		start = created = 0;
		sleep(1);
		task_pid = task_create("rrtaskpth", priority[tc][0], 1024, taskNpthread_func, (FAR char * const *)argv);
		TC_ASSERT_NEQ("task_create", task_pid, ERROR);

		created++;
		param.sched_priority = priority[tc][0];
		ret = sched_setscheduler(task_pid, SCHED_RR, &param);
		TC_ASSERT_EQ("sched_setscheduler", ret, OK);

		pid_prio[PIDHASH(task_pid)] = priority[tc][0];

		if (task_pid != ERROR) {
			FAR struct tcb_s *ptr;
			while ((ptr = sched_gettcb(task_pid)) != NULL) {
				usleep(100);
			}
		}

		ret = check_log();
		TC_ASSERT_EQ("check_log", ret, OK);
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
* Name: roundrobin
****************************************************************************/

int roundrobin_main(void)
{
	tc_roundrobin_rr_pthread();
	tc_roundrobin_rr_task();
	tc_roundrobin_rr_taskNpthread();
	return 0;
}
