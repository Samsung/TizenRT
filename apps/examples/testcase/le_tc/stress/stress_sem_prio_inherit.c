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
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PI_STACK    2048
#define PI_H_PRIO   80
#define PI_A_PRIO   100
#define PI_B_PRIO   120
#define PI_C_PRIO   140
#define PI_SHORT_US 100000
#define PI_HOLD_US  600000
#define PI_T4_H_PRIO  50
#define PI_T4_A1_PRIO 100
#define PI_T4_A2_PRIO 95
#define PI_T4_B_PRIO  80

#define PI_T5_MAIN_PRIO       160
#define PI_T5_LOW_PRIO        110
#define PI_T5_A_PRIO          120
#define PI_T5_B_PRIO          130
#define PI_T5_C_PRIO          140
#define PI_T5_D_PRIO          150
#define PI_T5_STACK           4096
#define PI_T5_TIMEOUT_SECONDS 2
#define PI_T5_CPU             0

#define PI_T5_STAGE_INITIAL   0
#define PI_T5_STAGE_READY     1
#define PI_T5_STAGE_WAITING   2
#define PI_T5_STAGE_ACQUIRED  3
#define PI_T5_STAGE_ERROR    -1

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if !defined(CONFIG_BUILD_KERNEL) && defined(CONFIG_PRIORITY_INHERITANCE)

static sem_t g_t1_sem;
static pid_t g_t1_holder = -1;

static sem_t g_t2_sem_a;
static sem_t g_t2_sem_b;
static sem_t g_t2_sem_c;
static pid_t g_t2_holder = -1;

static sem_t g_t3_sem1;
static sem_t g_t3_sem2;
static sem_t g_t3_sem3;
static pid_t g_t3_holder = -1;

static sem_t g_t4_sem_a;
static sem_t g_t4_sem_b;
static pid_t g_t4_holder = -1;

static sem_t g_t5_target_sem;
static sem_t g_t5_a_hold_sem;
static sem_t g_t5_b_hold_sem;
static sem_t g_t5_c_hold_sem;
static sem_t g_t5_d_hold_sem;
static sem_t g_t5_acquired_sem;
static sem_t g_t5_start_sem[4];
static volatile int g_t5_a_stage;
static volatile int g_t5_b_stage;
static volatile int g_t5_c_stage;
static volatile int g_t5_d_stage;

static int g_pi_pass;
static int g_pi_fail;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int pi_getprio(pid_t pid)
{
	struct sched_param param;

	if (sched_getparam(pid, &param) < 0) {
		printf("pi: sched_getparam(%d) failed errno=%d\n", pid, errno);
		return ERROR;
	}

	return param.sched_priority;
}

static const char *pi_result(int actual, int expected)
{
	if (actual == expected) {
		g_pi_pass++;
		return "PASS";
	}

	g_pi_fail++;
	return "FAIL";
}

static void pi_check(const char *test, const char *tag, pid_t pid, int expected)
{
	int p = pi_getprio(pid);
	printf("[%s] %s: priority=%d expected=%d %s\n",
		   test, tag, p, expected, pi_result(p, expected));
}

static void pi_stay_alive(const char *name)
{
	printf("pi: %s done, staying alive\n", name);
	while (1) {
		sleep(1);
	}
}

static int pi_t1_holder_main(int argc, char *argv[])
{
	int p;

	sem_wait(&g_t1_sem);

	p = pi_getprio(0);
	printf("[t1] holder acquired, priority=%d base=%d\n", p, PI_H_PRIO);

	usleep(PI_HOLD_US);

	p = pi_getprio(0);
	printf("[t1] holder before post, priority=%d expected=%d\n", p, PI_C_PRIO);

	sem_post(&g_t1_sem);

	p = pi_getprio(0);
	printf("[t1] holder after post, priority=%d expected=%d\n", p, PI_H_PRIO);

	usleep(PI_HOLD_US);
	pi_stay_alive("t1_holder");
	return OK;
}

static int pi_t1_waiter_main(int argc, char *argv[])
{
	const char *name = (argc > 0 && argv[0]) ? argv[0] : "t1_w";

	printf("[t1] %s waiting\n", name);
	sem_wait(&g_t1_sem);
	printf("[t1] %s acquired, releasing\n", name);
	sem_post(&g_t1_sem);
	pi_stay_alive(name);
	return OK;
}

static int pi_t1_test(void)
{
	FAR char *argv_a[] = { "t1_wa", NULL };
	FAR char *argv_b[] = { "t1_wb", NULL };
	FAR char *argv_c[] = { "t1_wc", NULL };

	printf("\n=== Test 1: single sem, three waiters ===\n");
	printf("holder=%d waiter_a=%d waiter_b=%d waiter_c=%d\n",
		   PI_H_PRIO, PI_A_PRIO, PI_B_PRIO, PI_C_PRIO);

	if (sem_init(&g_t1_sem, 0, 1) < 0) {
		printf("[t1] sem_init failed errno=%d\n", errno);
		return ERROR;
	}

	g_t1_holder = task_create("t1_holder", PI_H_PRIO, PI_STACK,
						  pi_t1_holder_main, NULL);
	if (g_t1_holder < 0) {
		printf("[t1] task_create(holder) failed errno=%d\n", errno);
		return ERROR;
	}

	usleep(PI_SHORT_US);
	pi_check("t1", "after holder acquire", g_t1_holder, PI_H_PRIO);

	if (task_create("t1_wa", PI_A_PRIO, PI_STACK, pi_t1_waiter_main,
					(FAR char *const *)argv_a) < 0) {
		printf("[t1] task_create(wa) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t1", "after waiter_a blocks", g_t1_holder, PI_A_PRIO);

	if (task_create("t1_wb", PI_B_PRIO, PI_STACK, pi_t1_waiter_main,
					(FAR char *const *)argv_b) < 0) {
		printf("[t1] task_create(wb) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t1", "after waiter_b blocks", g_t1_holder, PI_B_PRIO);

	if (task_create("t1_wc", PI_C_PRIO, PI_STACK, pi_t1_waiter_main,
					(FAR char *const *)argv_c) < 0) {
		printf("[t1] task_create(wc) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t1", "after waiter_c blocks", g_t1_holder, PI_C_PRIO);

	usleep(PI_HOLD_US + PI_SHORT_US);
	pi_check("t1", "after holder post", g_t1_holder, PI_H_PRIO);

	usleep(PI_HOLD_US);
	printf("=== Test 1 done ===\n");
	return OK;
}

static int pi_t2_holder_main(int argc, char *argv[])
{
	int p;

	sem_wait(&g_t2_sem_a);
	sem_wait(&g_t2_sem_b);
	sem_wait(&g_t2_sem_c);

	p = pi_getprio(0);
	printf("[t2] holder acquired all three, priority=%d base=%d\n",
		   p, PI_H_PRIO);

	usleep(PI_HOLD_US);

	p = pi_getprio(0);
	printf("[t2] before posts, priority=%d expected=%d\n", p, PI_C_PRIO);

	sem_post(&g_t2_sem_c);
	p = pi_getprio(0);
	printf("[t2] after post sem_c, priority=%d expected=%d\n", p, PI_B_PRIO);

	usleep(PI_SHORT_US);
	sem_post(&g_t2_sem_b);
	p = pi_getprio(0);
	printf("[t2] after post sem_b, priority=%d expected=%d\n", p, PI_A_PRIO);

	usleep(PI_SHORT_US);
	sem_post(&g_t2_sem_a);
	p = pi_getprio(0);
	printf("[t2] after post sem_a, priority=%d expected=%d\n", p, PI_H_PRIO);

	pi_stay_alive("t2_holder");
	return OK;
}

static int pi_t2_waiter_a_main(int argc, char *argv[])
{
	printf("[t2] waiter_a waiting on sem_a\n");
	sem_wait(&g_t2_sem_a);
	printf("[t2] waiter_a acquired sem_a, releasing\n");
	sem_post(&g_t2_sem_a);
	pi_stay_alive("t2_wa");
	return OK;
}

static int pi_t2_waiter_b_main(int argc, char *argv[])
{
	printf("[t2] waiter_b waiting on sem_b\n");
	sem_wait(&g_t2_sem_b);
	printf("[t2] waiter_b acquired sem_b, releasing\n");
	sem_post(&g_t2_sem_b);
	pi_stay_alive("t2_wb");
	return OK;
}

static int pi_t2_waiter_c_main(int argc, char *argv[])
{
	printf("[t2] waiter_c waiting on sem_c\n");
	sem_wait(&g_t2_sem_c);
	printf("[t2] waiter_c acquired sem_c, releasing\n");
	sem_post(&g_t2_sem_c);
	pi_stay_alive("t2_wc");
	return OK;
}

static int pi_t2_test(void)
{
	printf("\n=== Test 2: three sems, one waiter each, LIFO release ===\n");
	printf("holder=%d waiter_a=%d waiter_b=%d waiter_c=%d\n",
		   PI_H_PRIO, PI_A_PRIO, PI_B_PRIO, PI_C_PRIO);

	if (sem_init(&g_t2_sem_a, 0, 1) < 0 ||
		sem_init(&g_t2_sem_b, 0, 1) < 0 ||
		sem_init(&g_t2_sem_c, 0, 1) < 0) {
		printf("[t2] sem_init failed errno=%d\n", errno);
		return ERROR;
	}

	g_t2_holder = task_create("t2_holder", PI_H_PRIO, PI_STACK,
						  pi_t2_holder_main, NULL);
	if (g_t2_holder < 0) {
		printf("[t2] task_create(holder) failed errno=%d\n", errno);
		return ERROR;
	}

	usleep(PI_SHORT_US);
	pi_check("t2", "after holder acquires all", g_t2_holder, PI_H_PRIO);

	if (task_create("t2_wa", PI_A_PRIO, PI_STACK, pi_t2_waiter_a_main, NULL) < 0) {
		printf("[t2] task_create(wa) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t2", "after waiter_a blocks on sem_a", g_t2_holder, PI_A_PRIO);

	if (task_create("t2_wb", PI_B_PRIO, PI_STACK, pi_t2_waiter_b_main, NULL) < 0) {
		printf("[t2] task_create(wb) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t2", "after waiter_b blocks on sem_b", g_t2_holder, PI_B_PRIO);

	if (task_create("t2_wc", PI_C_PRIO, PI_STACK, pi_t2_waiter_c_main, NULL) < 0) {
		printf("[t2] task_create(wc) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t2", "after waiter_c blocks on sem_c", g_t2_holder, PI_C_PRIO);

	usleep(PI_HOLD_US + PI_SHORT_US * 4);
	pi_check("t2", "after all done", g_t2_holder, PI_H_PRIO);

	usleep(PI_HOLD_US);
	printf("=== Test 2 done ===\n");
	return OK;
}

static int pi_t3_holder_main(int argc, char *argv[])
{
	int p;

	sem_wait(&g_t3_sem1);
	sem_wait(&g_t3_sem2);
	sem_wait(&g_t3_sem3);

	p = pi_getprio(0);
	printf("[t3] holder acquired sem1+sem2+sem3, priority=%d base=%d\n",
		   p, PI_H_PRIO);

	usleep(PI_HOLD_US);

	p = pi_getprio(0);
	printf("[t3] before posts, priority=%d expected=%d\n", p, PI_C_PRIO);

	sem_post(&g_t3_sem1);
	usleep(PI_SHORT_US);
	sem_post(&g_t3_sem2);
	usleep(PI_SHORT_US);
	sem_post(&g_t3_sem3);

	usleep(PI_HOLD_US);

	p = pi_getprio(0);
	printf("[t3] after all posts, priority=%d expected=%d\n", p, PI_H_PRIO);

	pi_stay_alive("t3_holder");
	return OK;
}

static int pi_t3_task_main(int argc, char *argv[])
{
	const char *name = (argc > 0 && argv[0]) ? argv[0] : "t3_task";

	printf("[t3] %s: waiting sem1\n", name);
	sem_wait(&g_t3_sem1);

	printf("[t3] %s: got sem1, waiting sem2\n", name);
	sem_wait(&g_t3_sem2);

	printf("[t3] %s: got sem2, waiting sem3\n", name);
	sem_wait(&g_t3_sem3);

	printf("[t3] %s: got all three, releasing\n", name);
	sem_post(&g_t3_sem3);
	sem_post(&g_t3_sem2);
	sem_post(&g_t3_sem1);

	pi_stay_alive(name);
	return OK;
}

static int pi_t3_test(void)
{
	FAR char *argv_a[] = { "t3_ta", NULL };
	FAR char *argv_b[] = { "t3_tb", NULL };
	FAR char *argv_c[] = { "t3_tc", NULL };

	printf("\n=== Test 3: three tasks each acquire all three sems ===\n");
	printf("holder=%d task_a=%d task_b=%d task_c=%d\n",
		   PI_H_PRIO, PI_A_PRIO, PI_B_PRIO, PI_C_PRIO);

	if (sem_init(&g_t3_sem1, 0, 1) < 0 ||
		sem_init(&g_t3_sem2, 0, 1) < 0 ||
		sem_init(&g_t3_sem3, 0, 1) < 0) {
		printf("[t3] sem_init failed errno=%d\n", errno);
		return ERROR;
	}

	g_t3_holder = task_create("t3_holder", PI_H_PRIO, PI_STACK,
						  pi_t3_holder_main, NULL);
	if (g_t3_holder < 0) {
		printf("[t3] task_create(holder) failed errno=%d\n", errno);
		return ERROR;
	}

	usleep(PI_SHORT_US);
	pi_check("t3", "after holder acquires all", g_t3_holder, PI_H_PRIO);

	if (task_create("t3_ta", PI_A_PRIO, PI_STACK, pi_t3_task_main,
					(FAR char *const *)argv_a) < 0) {
		printf("[t3] task_create(ta) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t3", "after task_a blocks on sem1", g_t3_holder, PI_A_PRIO);

	if (task_create("t3_tb", PI_B_PRIO, PI_STACK, pi_t3_task_main,
					(FAR char *const *)argv_b) < 0) {
		printf("[t3] task_create(tb) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t3", "after task_b blocks on sem1", g_t3_holder, PI_B_PRIO);

	if (task_create("t3_tc", PI_C_PRIO, PI_STACK, pi_t3_task_main,
					(FAR char *const *)argv_c) < 0) {
		printf("[t3] task_create(tc) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t3", "after task_c blocks on sem1", g_t3_holder, PI_C_PRIO);

	usleep(PI_HOLD_US + PI_SHORT_US * 6 + PI_HOLD_US);
	pi_check("t3", "after all done", g_t3_holder, PI_H_PRIO);

	usleep(PI_HOLD_US);
	printf("=== Test 3 done ===\n");
	return OK;
}

static int pi_t4_holder_main(int argc, char *argv[])
{
	int p;

	printf("[t4-HOLDER] base=%d, acquiring sem_a then sem_b\n", PI_T4_H_PRIO);
	sem_wait(&g_t4_sem_a);
	sem_wait(&g_t4_sem_b);

	p = pi_getprio(0);
	printf("[t4-HOLDER] after acquire: priority=%d (expected %d)\n", p, PI_T4_H_PRIO);

	usleep(PI_HOLD_US);

	p = pi_getprio(0);
	printf("[t4-HOLDER] before posts: priority=%d (expected %d)\n", p, PI_T4_A1_PRIO);

	printf("[t4-HOLDER] posting sem_a ...\n");
	sem_post(&g_t4_sem_a);

	p = pi_getprio(0);
	printf("[t4-HOLDER] after post sem_a: priority=%d expected=%d %s\n",
		   p, PI_T4_B_PRIO, pi_result(p, PI_T4_B_PRIO));

	usleep(PI_SHORT_US);

	printf("[t4-HOLDER] posting sem_b ...\n");
	sem_post(&g_t4_sem_b);

	p = pi_getprio(0);
	printf("[t4-HOLDER] after post sem_b: priority=%d expected=%d %s\n",
		   p, PI_T4_H_PRIO, pi_result(p, PI_T4_H_PRIO));

	pi_stay_alive("t4_holder");
	return OK;
}

static int pi_t4_waiter_a1_main(int argc, char *argv[])
{
	int p;

	printf("[t4-WA1(100)] Task starting, priority=%d (should be 100)\n", pi_getprio(0));
	printf("[t4-WA1(100)] About to block on sem_a\n");
	sem_wait(&g_t4_sem_a);
	p = pi_getprio(0);
	printf("[t4-WA1(100)] *** ACQUIRED sem_a! priority=%d, now releasing ***\n", p);
	printf("[t4-WA1(100)] This means holder released waiter_a1 (got priority %d)\n", p);
	sem_post(&g_t4_sem_a);
	printf("[t4-WA1(100)] Released sem_a (this will unblock waiter_a2)\n");
	pi_stay_alive("t4_wa1");
	return OK;
}

static int pi_t4_waiter_a2_main(int argc, char *argv[])
{
	int p;

	printf("[t4-WA2(95)] Task starting, priority=%d (should be 95)\n", pi_getprio(0));
	printf("[t4-WA2(95)] About to block on sem_a (waiter_a1 is ahead)\n");
	sem_wait(&g_t4_sem_a);
	p = pi_getprio(0);
	printf("[t4-WA2(95)] *** ACQUIRED sem_a! priority=%d ***\n", p);
	printf("[t4-WA2(95)] This means: holder released, waiter_a1 released, now a2's turn\n");
	printf("[t4-WA2(95)] Now releasing sem_a\n");
	sem_post(&g_t4_sem_a);
	printf("[t4-WA2(95)] Released sem_a\n");
	pi_stay_alive("t4_wa2");
	return OK;
}

static int pi_t4_waiter_b_main(int argc, char *argv[])
{
	int p;

	printf("[t4-WB(80)] Task starting, priority=%d (should be 80)\n", pi_getprio(0));
	printf("[t4-WB(80)] About to block on sem_b (DIFFERENT semaphore!)\n");
	sem_wait(&g_t4_sem_b);
	p = pi_getprio(0);
	printf("[t4-WB(80)] *** ACQUIRED sem_b! priority=%d ***\n", p);
	printf("[t4-WB(80)] Now releasing sem_b\n");
	sem_post(&g_t4_sem_b);
	printf("[t4-WB(80)] Released sem_b\n");
	pi_stay_alive("t4_wb");
	return OK;
}

static int pi_t4_test(void)
{
	printf("\n=== Test 4: cross-semaphore contamination (per-sem tagging fix) ===\n");
	printf("holder=%d waiter_a1=%d waiter_a2=%d waiter_b=%d\n",
		   PI_T4_H_PRIO, PI_T4_A1_PRIO, PI_T4_A2_PRIO, PI_T4_B_PRIO);
	printf("[t4] holder(50) holds sem_a and sem_b\n");
	printf("[t4] a1(100),a2(95) wait on sem_a ; b(80) waits on sem_b\n");
	printf("[t4] aggregate demands = [100,95(sem_a), 80(sem_b)], sched=100\n");
	printf("[t4] post sem_a: remove only sem_a demands -> remain [80(sem_b)]\n");
	printf("[t4]   expected after sem_a = 80 (b still waits on held sem_b)\n");
	printf("[t4]   expected after sem_b = 50 (base)\n\n");

	if (sem_init(&g_t4_sem_a, 0, 1) < 0 ||
		sem_init(&g_t4_sem_b, 0, 1) < 0) {
		printf("[t4] sem_init failed errno=%d\n", errno);
		return ERROR;
	}

	g_t4_holder = task_create("t4_holder", PI_T4_H_PRIO, PI_STACK,
						  pi_t4_holder_main, NULL);
	if (g_t4_holder < 0) {
		printf("[t4] task_create(holder) failed errno=%d\n", errno);
		return ERROR;
	}

	usleep(PI_SHORT_US);
	pi_check("t4", "after holder acquire", g_t4_holder, PI_T4_H_PRIO);

	if (task_create("t4_wa1", PI_T4_A1_PRIO, PI_STACK, pi_t4_waiter_a1_main, NULL) < 0) {
		printf("[t4] task_create(wa1) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t4", "after waiter_a1(100) blocks on sem_a", g_t4_holder, PI_T4_A1_PRIO);

	if (task_create("t4_wa2", PI_T4_A2_PRIO, PI_STACK, pi_t4_waiter_a2_main, NULL) < 0) {
		printf("[t4] task_create(wa2) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t4", "after waiter_a2(95) blocks on sem_a", g_t4_holder, PI_T4_A1_PRIO);

	if (task_create("t4_wb", PI_T4_B_PRIO, PI_STACK, pi_t4_waiter_b_main, NULL) < 0) {
		printf("[t4] task_create(wb) failed errno=%d\n", errno);
		return ERROR;
	}
	usleep(PI_SHORT_US);
	pi_check("t4", "after waiter_b(80) blocks on sem_b", g_t4_holder, PI_T4_A1_PRIO);

	usleep(PI_HOLD_US + PI_SHORT_US * 4);
	pi_check("t4", "after all done", g_t4_holder, PI_T4_H_PRIO);

	usleep(PI_HOLD_US);
	printf("=== Test 4 done ===\n");
	return OK;
}

static int pi_t5_set_self_priority(int priority)
{
	struct sched_param param;

	param.sched_priority = priority;
	return sched_setparam(0, &param);
}

static int pi_t5_run_until_blocked(void)
{
	/* The controller and workers are pinned to one CPU.  Once the controller
	 * lowers its priority, the newly-created task runs until it blocks.  The
	 * controller can only resume after that blocking point has been reached.
	 */

	if (pi_t5_set_self_priority(PI_T5_LOW_PRIO) < 0) {
		return ERROR;
	}

	return pi_t5_set_self_priority(PI_T5_MAIN_PRIO);
}

static pid_t pi_t5_create_task(const char *name, main_t entry, int priority)
{
	return task_create(name, priority, PI_T5_STACK, entry, NULL);
}

static int pi_t5_prepare_task(int index, volatile int *stage)
{
#ifdef CONFIG_SMP
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(PI_T5_CPU, &cpuset);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) < 0) {
		*stage = PI_T5_STAGE_ERROR;
		return ERROR;
	}
#endif

#ifdef CONFIG_CANCELLATION_POINTS
	(void)task_setcancelstate(TASK_CANCEL_ENABLE, NULL);
	(void)task_setcanceltype(TASK_CANCEL_ASYNCHRONOUS, NULL);
#endif

	if (sem_wait(&g_t5_start_sem[index]) < 0) {
		*stage = PI_T5_STAGE_ERROR;
		return ERROR;
	}

	return OK;
}

static int pi_t5_wait_for_acquisition(void)
{
	struct timespec abstime;

	if (clock_gettime(CLOCK_REALTIME, &abstime) < 0) {
		return ERROR;
	}

	abstime.tv_sec += PI_T5_TIMEOUT_SECONDS;
	do {
		if (sem_timedwait(&g_t5_acquired_sem, &abstime) == OK) {
			return OK;
		}
	} while (errno == EINTR);

	return ERROR;
}

static void pi_t5_delete_task(pid_t pid)
{
	(void)task_delete(pid);
}

static void pi_t5_check(const char *name, int actual, int expected)
{
	printf("[t5] %s=%d expected=%d %s\n",
		   name, actual, expected, pi_result(actual, expected));
}

static int pi_t5_task_a(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (pi_t5_prepare_task(0, &g_t5_a_stage) < 0) {
		return ERROR;
	}

	if (sem_wait(&g_t5_target_sem) < 0 || sem_wait(&g_t5_target_sem) < 0) {
		g_t5_a_stage = PI_T5_STAGE_ERROR;
		return ERROR;
	}

	g_t5_a_stage = PI_T5_STAGE_READY;
	(void)sem_wait(&g_t5_a_hold_sem);
	return OK;
}

static int pi_t5_task_b(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (pi_t5_prepare_task(1, &g_t5_b_stage) < 0) {
		return ERROR;
	}

	if (sem_wait(&g_t5_target_sem) < 0) {
		g_t5_b_stage = PI_T5_STAGE_ERROR;
		return ERROR;
	}

	g_t5_b_stage = PI_T5_STAGE_READY;
	(void)sem_wait(&g_t5_b_hold_sem);
	return OK;
}

static int pi_t5_task_c(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (pi_t5_prepare_task(2, &g_t5_c_stage) < 0) {
		return ERROR;
	}

	g_t5_c_stage = PI_T5_STAGE_WAITING;
	if (sem_wait(&g_t5_target_sem) < 0) {
		g_t5_c_stage = PI_T5_STAGE_ERROR;
		return ERROR;
	}

	g_t5_c_stage = PI_T5_STAGE_ACQUIRED;
	(void)sem_post(&g_t5_acquired_sem);
	(void)sem_wait(&g_t5_c_hold_sem);
	return OK;
}

static int pi_t5_task_d(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	if (pi_t5_prepare_task(3, &g_t5_d_stage) < 0) {
		return ERROR;
	}

	g_t5_d_stage = PI_T5_STAGE_WAITING;
	if (sem_wait(&g_t5_target_sem) < 0) {
		g_t5_d_stage = PI_T5_STAGE_ERROR;
		return ERROR;
	}

	g_t5_d_stage = PI_T5_STAGE_ACQUIRED;
	(void)sem_post(&g_t5_acquired_sem);
	(void)sem_wait(&g_t5_d_hold_sem);
	return OK;
}

static int pi_t5_test(void)
{
	FAR sem_t *semaphores[] = {
		&g_t5_target_sem,
		&g_t5_a_hold_sem,
		&g_t5_b_hold_sem,
		&g_t5_c_hold_sem,
		&g_t5_d_hold_sem,
		&g_t5_acquired_sem,
		&g_t5_start_sem[0],
		&g_t5_start_sem[1],
		&g_t5_start_sem[2],
		&g_t5_start_sem[3],
	};
	const unsigned int initial_values[] = { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	const char *names[] = {
		"pi_t5_a",
		"pi_t5_b",
		"pi_t5_c",
		"pi_t5_d",
	};
	main_t entries[] = {
		pi_t5_task_a,
		pi_t5_task_b,
		pi_t5_task_c,
		pi_t5_task_d,
	};
	const int priorities[] = {
		PI_T5_A_PRIO,
		PI_T5_B_PRIO,
		PI_T5_C_PRIO,
		PI_T5_D_PRIO,
	};
	volatile int *stages[] = {
		&g_t5_a_stage,
		&g_t5_b_stage,
		&g_t5_c_stage,
		&g_t5_d_stage,
	};
	const int expected_stages[] = {
		PI_T5_STAGE_READY,
		PI_T5_STAGE_READY,
		PI_T5_STAGE_WAITING,
		PI_T5_STAGE_WAITING,
	};
	pid_t tasks[4];
	bool created[4] = { false, false, false, false };
	struct sched_param original_param;
	int initial_failures = g_pi_fail;
	int initialized = 0;
	int target_value = 0;
	int acquired_count = 0;
	int ret;
	int i;
#ifdef CONFIG_SMP
	cpu_set_t original_affinity;
	cpu_set_t test_affinity;
	bool affinity_saved = false;
#endif

	printf("\n=== Test 5: release all counts held by deleted task ===\n");
	printf("main=%d A=%d B=%d C=%d D=%d\n",
		   PI_T5_MAIN_PRIO, PI_T5_A_PRIO, PI_T5_B_PRIO,
		   PI_T5_C_PRIO, PI_T5_D_PRIO);

	if (sched_getparam(0, &original_param) < 0) {
		printf("[t5] sched_getparam(main) failed errno=%d\n", errno);
		g_pi_fail++;
		return ERROR;
	}

#ifdef CONFIG_SMP
	if (sched_getaffinity(0, sizeof(cpu_set_t), &original_affinity) < 0) {
		printf("[t5] sched_getaffinity(main) failed errno=%d\n", errno);
		g_pi_fail++;
		return ERROR;
	}
	affinity_saved = true;
	CPU_ZERO(&test_affinity);
	CPU_SET(PI_T5_CPU, &test_affinity);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &test_affinity) < 0) {
		printf("[t5] sched_setaffinity(main) failed errno=%d\n", errno);
		g_pi_fail++;
		return ERROR;
	}
#endif

	if (pi_t5_set_self_priority(PI_T5_MAIN_PRIO) < 0) {
		printf("[t5] set main priority failed errno=%d\n", errno);
		g_pi_fail++;
		goto cleanup;
	}

	g_t5_a_stage = PI_T5_STAGE_INITIAL;
	g_t5_b_stage = PI_T5_STAGE_INITIAL;
	g_t5_c_stage = PI_T5_STAGE_INITIAL;
	g_t5_d_stage = PI_T5_STAGE_INITIAL;

	for (i = 0; i < (int)(sizeof(semaphores) / sizeof(semaphores[0])); i++) {
		if (sem_init(semaphores[i], 0, initial_values[i]) < 0) {
			printf("[t5] sem_init[%d] failed errno=%d\n", i, errno);
			g_pi_fail++;
			goto cleanup;
		}
		initialized++;
	}

	for (i = 0; i < 4; i++) {
		tasks[i] = pi_t5_create_task(names[i], entries[i], priorities[i]);
		if (tasks[i] < 0) {
			printf("[t5] task_create(%c) failed errno=%d\n", 'A' + i, errno);
			g_pi_fail++;
			goto cleanup;
		}
		created[i] = true;

		if (sem_post(&g_t5_start_sem[i]) < 0) {
			printf("[t5] start task %c failed errno=%d\n", 'A' + i, errno);
			g_pi_fail++;
			goto cleanup;
		}

		if (pi_t5_run_until_blocked() < 0 || *stages[i] != expected_stages[i]) {
			printf("[t5] task %c did not reach its blocking point\n", 'A' + i);
			g_pi_fail++;
			goto cleanup;
		}
	}

	(void)sem_getvalue(&g_t5_target_sem, &target_value);
	printf("[t5] before delete: semcount=%d A-prio=%d B-prio=%d\n",
		   target_value, pi_getprio(tasks[0]), pi_getprio(tasks[1]));
	if (target_value != -2 || pi_getprio(tasks[1]) != PI_T5_D_PRIO) {
		printf("[t5] precondition mismatch; expected semcount=-2 B-prio=150\n");
		g_pi_fail++;
		goto cleanup;
	}

	ret = task_delete(tasks[0]);
	if (ret < 0) {
		printf("[t5] task_delete(A) failed errno=%d\n", errno);
		g_pi_fail++;
		goto cleanup;
	}
	created[0] = false;

	if (pi_t5_wait_for_acquisition() == OK) {
		acquired_count++;
	}
	if (pi_t5_wait_for_acquisition() == OK) {
		acquired_count++;
	}

	(void)sem_getvalue(&g_t5_target_sem, &target_value);
	printf("[t5] after delete: semcount=%d acquired=%d C=%s D=%s B-prio=%d\n",
		   target_value, acquired_count,
		   g_t5_c_stage == PI_T5_STAGE_ACQUIRED ? "yes" : "no",
		   g_t5_d_stage == PI_T5_STAGE_ACQUIRED ? "yes" : "no",
		   pi_getprio(tasks[1]));

	pi_t5_check("semcount", target_value, 0);
	pi_t5_check("acquired waiters", acquired_count, 2);
	pi_t5_check("B priority", pi_getprio(tasks[1]), PI_T5_B_PRIO);

cleanup:
	(void)pi_t5_set_self_priority(PI_T5_MAIN_PRIO);
	for (i = 3; i >= 0; i--) {
		if (created[i]) {
			pi_t5_delete_task(tasks[i]);
		}
	}

	while (initialized > 0) {
		initialized--;
		(void)sem_destroy(semaphores[initialized]);
	}

	(void)sched_setparam(0, &original_param);
#ifdef CONFIG_SMP
	if (affinity_saved) {
		(void)sched_setaffinity(0, sizeof(cpu_set_t), &original_affinity);
	}
#endif

	printf("=== Test 5 done: %s ===\n",
		   g_pi_fail == initial_failures ? "PASS" : "FAIL");
	return g_pi_fail == initial_failures ? OK : ERROR;
}

static void pi_summary(void)
{
	printf("\n=== SUMMARY ===\n");
	printf("PASS: %d\n", g_pi_pass);
	printf("FAIL: %d\n", g_pi_fail);
}

#endif

/****************************************************************************
 * Name: stress_sem_prio_inherit_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int stress_sem_prio_inherit_main(int argc, char *argv[])
#endif
{
#if defined(CONFIG_BUILD_KERNEL)
	printf("pi: semaphore priority inheritance test not enabled in kernel build mode\n");
#elif defined(CONFIG_PRIORITY_INHERITANCE)
	g_pi_pass = 0;
	g_pi_fail = 0;
	(void)pi_t1_test();
	(void)pi_t2_test();
	(void)pi_t3_test();
	(void)pi_t4_test();
	(void)pi_t5_test();
	pi_summary();
#else
	printf("pi: CONFIG_PRIORITY_INHERITANCE is disabled\n");
#endif
	return 0;
}
