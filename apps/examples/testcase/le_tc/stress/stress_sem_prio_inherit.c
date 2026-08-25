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
	pi_summary();
#else
	printf("pi: CONFIG_PRIORITY_INHERITANCE is disabled\n");
#endif
	return 0;
}
