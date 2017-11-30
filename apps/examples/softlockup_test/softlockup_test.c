/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <unistd.h>
#include <tinyara/config.h>
#include <pthread.h>
#include <tinyara/wqueue.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define HOG_PTHREAD_PRIORITY     200
#define NON_HOG_PTHREAD_PRIORITY HOG_PTHREAD_PRIORITY + 1

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: hogging_function
 *
 * Description:
 *   This functions starts the infinte loop to simulate the hogging scenario.
 *
 * Input parameters:
 *   void
 *
 * Returned Value:
 *   void
 *
 ****************************************************************************/

static void *hogging_function(void *parameter)
{
	pthread_t tid = getpid();
	pthread_setname_np(tid, "hog_thread");
	for (;;) {
		/* Do Nothing and Hog */
	}
	return NULL;
}

/****************************************************************************
 * Name: thread_function
 *
 * Description:
 *   This function terminates after a finite period say 15 seconds.
 *
 * Input parameters:
 *   void
 *
 * Returned Value:
 *   void
 *
 ****************************************************************************/

static void *non_hogging_function(void *parameter)
{
	int i;
	for (i = 0; i < 15; i++) {
		/* Sleep for 1 sec */
		sleep(1);
	}
	return NULL;
}

/****************************************************************************
 * Name: test_low_prio_hog_thread
 *
 * Description:
 *   This function creates two pthreads. One pthread is of high priority and
 *   terminates after a finite period and second pthread runs in an infinite
 *   loop.
 *
 * Input parameters:
 *   void
 *
 * Returned Value:
 *   void
 *
 ****************************************************************************/

void test_low_prio_hog_thread(void)
{
	pthread_t thread1;
	pthread_t thread2;
	pthread_attr_t attr1;
	pthread_attr_t attr2;
	struct sched_param sparam;

	(void)pthread_attr_init(&attr1);
	sparam.sched_priority = NON_HOG_PTHREAD_PRIORITY;
	(void)pthread_attr_setschedparam(&attr1, & sparam);
	(void)pthread_create(&thread1, &attr1, non_hogging_function, NULL);

	(void)pthread_attr_init(&attr2);
	sparam.sched_priority = HOG_PTHREAD_PRIORITY;
	(void)pthread_attr_setschedparam(&attr2, & sparam);
	(void)pthread_create(&thread2, &attr2, hogging_function, NULL);

	/* Detach the threads */
	pthread_detach(thread1);
	pthread_detach(thread2);
}

/****************************************************************************
 * Name: test_normal_prio_hog_thread
 *
 * Description:
 *   This function creates a single pthread which is runing in an infinite
 *   loop.
 *
 * Input parameters:
 *   void
 *
 * Returned Value:
 *   void
 *
 ****************************************************************************/

void test_normal_prio_hog_thread(void)
{
	pthread_t thread1;
	pthread_attr_t attr;
	struct sched_param sparam;

	(void)pthread_attr_init(&attr);
	sparam.sched_priority = HOG_PTHREAD_PRIORITY;
	(void)pthread_attr_setschedparam(&attr, & sparam);
	(void)pthread_create(&thread1, &attr, hogging_function, NULL);
	pthread_detach(thread1);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int softlockup_test_main(int argc, char **argv)
#endif
{
	char ch;
	printf("\nPress A - For Normal Priority Hogging Thread Testcase\n");
	printf("Press B - For Low Priority Hogging Thread Testcase\n");

	ch = getchar();
	printf("%c", ch);
	if (ch == 'A' || ch == 'a') {
		printf("\nStarting normal priority hogging thread testcase\n");
		sleep(1);
		test_normal_prio_hog_thread();
	} else if (ch == 'B' || ch == 'b') {
		printf("\nStarting low priority hogging thread testcase along with high priority non hogging thread\n");
		sleep(1);
		test_low_prio_hog_thread();
	} else {
		printf("\nInvalid Option, Exiting!!\n");
	}

	return 0;
}
