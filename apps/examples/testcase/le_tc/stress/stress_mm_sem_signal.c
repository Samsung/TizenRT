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
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MM_SEM_TEST_FILL_COUNT   (CONFIG_PREALLOC_MQ_MSGS + 8)
#define MM_SEM_TEST_LOG_INTERVAL 100000UL

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
#define MM_SEM_TEST_DELAY_COUNT 8192U
#define MM_SEM_TEST_TARGET_CPU  0
#define MM_SEM_TEST_SENDER_CPU  1
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_SIGNALS) && !defined(CONFIG_DISABLE_PTHREAD)

static mqd_t g_mm_signal_queue;
static volatile int g_mm_signal_sent;
static volatile int g_mm_signal_done;
static volatile int g_mm_signal_errors;

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
static sem_t g_mm_start_sem;
static sem_t g_mm_ready_sem;
static sem_t g_mm_done_sem;
static volatile unsigned int g_mm_signal_delay;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
static int mm_sem_wait_nointr(sem_t *sem)
{
	int ret;

	do {
		ret = sem_wait(sem);
	} while (ret != 0 && errno == EINTR);

	return ret;
}

static int mm_sem_set_cpu(int cpu)
{
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	return sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}

static int mm_sem_sync_initialize(void)
{
	if (sem_init(&g_mm_start_sem, 0, 0) != 0) {
		return -1;
	}

	if (sem_init(&g_mm_ready_sem, 0, 0) != 0) {
		sem_destroy(&g_mm_start_sem);
		return -1;
	}

	if (sem_init(&g_mm_done_sem, 0, 0) != 0) {
		sem_destroy(&g_mm_ready_sem);
		sem_destroy(&g_mm_start_sem);
		return -1;
	}

	return 0;
}

static void mm_sem_sync_destroy(void)
{
	sem_destroy(&g_mm_done_sem);
	sem_destroy(&g_mm_ready_sem);
	sem_destroy(&g_mm_start_sem);
}
#endif // defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1

static void mm_signal_handler(int signo)
{
	char message = 's';

	if (mq_send(g_mm_signal_queue, &message, 1, 0) != 0) {
		g_mm_signal_errors++;
	}
	g_mm_signal_done++;

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	sem_post(&g_mm_done_sem);
#endif
}

static void *mm_signal_sender(void *arg)
{
	pthread_t target = *(pthread_t *)arg;

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	unsigned int delay = 0;
	volatile unsigned int spin;

	if (mm_sem_set_cpu(MM_SEM_TEST_SENDER_CPU) != 0) {
		printf("MMSEM WARN: sender affinity errno=%d\n", errno);
	}

	for (;;) {
		if (mm_sem_wait_nointr(&g_mm_start_sem) != 0) {
			g_mm_signal_errors++;
			continue;
		}

		g_mm_signal_delay = delay;
		sem_post(&g_mm_ready_sem);
		for (spin = 0; spin < delay; spin++) {
		}

		g_mm_signal_sent++;
		if (pthread_kill(target, SIGUSR1) != 0) {
			g_mm_signal_sent--;
			g_mm_signal_errors++;
			sem_post(&g_mm_done_sem);
		}

		delay++;
		if (delay >= MM_SEM_TEST_DELAY_COUNT) {
			delay = 0;
		}
	}
#else // defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	for (;;) {
		if (g_mm_signal_sent == g_mm_signal_done) {
			if (pthread_kill(target, SIGUSR1) == 0) {
				g_mm_signal_sent++;
			} else {
				g_mm_signal_errors++;
			}
		} else {
			sched_yield();
		}
	}
#endif // defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1

	return NULL;
}

static int mm_sem_signal_test(void)
{
	char fill_name[32];
	char main_name[32];
	char signal_name[32];
	struct mq_attr fill_attr;
	struct mq_attr work_attr;
	struct sigaction action;
	struct sigaction old_action;
	sigset_t signal_set;
	pthread_t sender;
	pthread_t target;
	char message = 'm';
	char received;
	unsigned long i;
	int create_ret;
	mqd_t fill_queue;
	mqd_t main_queue;

	memset(&fill_attr, 0, sizeof(fill_attr));
	memset(&work_attr, 0, sizeof(work_attr));
	fill_attr.mq_maxmsg = MM_SEM_TEST_FILL_COUNT;
	fill_attr.mq_msgsize = 1;
	work_attr.mq_maxmsg = 1;
	work_attr.mq_msgsize = 1;
	snprintf(fill_name, sizeof(fill_name), "mm_sem_fill_%d", getpid());
	snprintf(main_name, sizeof(main_name), "mm_sem_main_%d", getpid());
	snprintf(signal_name, sizeof(signal_name), "mm_sem_signal_%d", getpid());

	mq_unlink(fill_name);
	mq_unlink(main_name);
	mq_unlink(signal_name);

	fill_queue = mq_open(fill_name, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &fill_attr);
	if (fill_queue == (mqd_t)-1) {
		printf("MMSEM FAIL: fill mq_open errno=%d\n", errno);
		return -1;
	}

	for (i = 0; i < MM_SEM_TEST_FILL_COUNT; i++) {
		if (mq_send(fill_queue, &message, 1, 0) != 0) {
			printf("MMSEM FAIL: fill mq_send errno=%d\n", errno);
			mq_close(fill_queue);
			mq_unlink(fill_name);
			return -1;
		}
	}
	mq_close(fill_queue);

	g_mm_signal_queue = mq_open(signal_name, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &fill_attr);
	if (g_mm_signal_queue == (mqd_t)-1) {
		printf("MMSEM FAIL: signal mq_open errno=%d\n", errno);
		mq_unlink(fill_name);
		return -1;
	}

	main_queue = mq_open(main_name, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &work_attr);
	if (main_queue == (mqd_t)-1) {
		printf("MMSEM FAIL: main mq_open errno=%d\n", errno);
		mq_close(g_mm_signal_queue);
		mq_unlink(signal_name);
		mq_unlink(fill_name);
		return -1;
	}

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	if (mm_sem_sync_initialize() != 0) {
		printf("MMSEM FAIL: synchronization init errno=%d\n", errno);
		mq_close(main_queue);
		mq_close(g_mm_signal_queue);
		mq_unlink(main_name);
		mq_unlink(signal_name);
		mq_unlink(fill_name);
		return -1;
	}
#endif

	memset(&action, 0, sizeof(action));
	action.sa_handler = mm_signal_handler;
	sigemptyset(&action.sa_mask);
	if (sigaction(SIGUSR1, &action, &old_action) != 0) {
		printf("MMSEM FAIL: sigaction errno=%d\n", errno);
#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
		mm_sem_sync_destroy();
#endif
		mq_close(main_queue);
		mq_close(g_mm_signal_queue);
		mq_unlink(main_name);
		mq_unlink(signal_name);
		mq_unlink(fill_name);
		return -1;
	}

	target = pthread_self();
	g_mm_signal_sent = 0;
	g_mm_signal_done = 0;
	g_mm_signal_errors = 0;
	sigemptyset(&signal_set);
	sigaddset(&signal_set, SIGUSR1);
	sigprocmask(SIG_UNBLOCK, &signal_set, NULL);

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	if (mm_sem_set_cpu(MM_SEM_TEST_TARGET_CPU) != 0) {
		printf("MMSEM WARN: target affinity errno=%d\n", errno);
	}
#endif

	create_ret = pthread_create(&sender, NULL, mm_signal_sender, &target);
	if (create_ret != 0) {
		printf("MMSEM FAIL: pthread_create ret=%d\n", create_ret);
		sigaction(SIGUSR1, &old_action, NULL);
#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
		mm_sem_sync_destroy();
#endif
		mq_close(main_queue);
		mq_close(g_mm_signal_queue);
		mq_unlink(main_name);
		mq_unlink(signal_name);
		mq_unlink(fill_name);
		return -1;
	}

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
	printf("MMSEM START pid=%d phase_sweep=%u\n", getpid(), MM_SEM_TEST_DELAY_COUNT);
#else
	printf("MMSEM START pid=%d\n", getpid());
#endif
	for (i = 0; ; i++) {
#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
		sem_post(&g_mm_start_sem);
		mm_sem_wait_nointr(&g_mm_ready_sem);
#endif

		if (mq_send(main_queue, &message, 1, 0) != 0) {
			printf("MMSEM ERROR: main mq_send errno=%d\n", errno);
			g_mm_signal_errors++;
		}

		while (mq_receive(main_queue, &received, 1, NULL) >= 0) {
		}

#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
		mm_sem_wait_nointr(&g_mm_done_sem);
#endif

		while (mq_receive(g_mm_signal_queue, &received, 1, NULL) >= 0) {
		}
		if (i % MM_SEM_TEST_LOG_INTERVAL == 0) {
#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1
			printf("MMSEM RUNNING iter=%lu sent=%d signals=%d errors=%d delay=%u\n",
					i, (int)g_mm_signal_sent, (int)g_mm_signal_done,
					(int)g_mm_signal_errors, (unsigned int)g_mm_signal_delay);
#else
			printf("MMSEM RUNNING iter=%lu sent=%d signals=%d errors=%d\n",
					i, (int)g_mm_signal_sent, (int)g_mm_signal_done,
					(int)g_mm_signal_errors);
#endif
		}
	}
}

#endif // !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_SIGNALS) && !defined(CONFIG_DISABLE_PTHREAD)

/****************************************************************************
 * Name: stress_mm_sem_signal_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int stress_mm_sem_signal_main(int argc, char *argv[])
#endif
{
#if !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_SIGNALS) && !defined(CONFIG_DISABLE_PTHREAD)
	return mm_sem_signal_test();
#else
	printf("MMSEM unavailable\n");
	return -1;
#endif
}
