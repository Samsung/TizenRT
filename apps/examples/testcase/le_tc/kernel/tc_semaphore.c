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

/// @file tc_semaphore.c

/// @brief Test Case Example for semaphore API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/clock.h>
#include <tinyara/semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include "tc_internal.h"

#define PSHARED			0
#define SEC_2			2
#define LOOP_CNT		5
#define SEM_PRIO_DEFAULT	3

static sem_t g_empty;
static sem_t g_full;
static sem_t g_sem;

/**
* @fn                   :producer_func
* @description          :Function for tc_semaphore_sem_init_post_wait
* @return               :void*
*/
static void *producer_func(void *arg)
{
	int index;
	int ret_chk;
	for (index = 0; index < LOOP_CNT; index++) {
		ret_chk = sem_wait(&g_empty);
		if (ret_chk == ERROR) {
			pthread_exit("sem_wait(&g_empty)");
		}

		ret_chk = sem_wait(&g_sem);
		if (ret_chk == ERROR) {
			pthread_exit("sem_wait(&g_mutex)");
		}

		ret_chk = sem_post(&g_sem);
		if (ret_chk == ERROR) {
			pthread_exit("sem_post(&g_mutex)");
		}

		ret_chk = sem_post(&g_full);
		if (ret_chk == ERROR) {
			pthread_exit("sem_post(&g_full)");
		}
	}
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :consumer_func
* @description          :Function for tc_semaphore_sem_init_post_wait
* @return               :void*
*/
static void *consumer_func(void *arg)
{
	int index;
	int ret_chk;
	for (index = 0; index < LOOP_CNT; index++) {
		ret_chk = sem_wait(&g_full);
		if (ret_chk == ERROR) {
			pthread_exit("sem_wait(&g_full)");
		}

		ret_chk = sem_wait(&g_sem);
		if (ret_chk == ERROR) {
			pthread_exit("sem_wait(&g_mutex)");
		}

		ret_chk = sem_post(&g_sem);
		if (ret_chk == ERROR) {
			pthread_exit("sem_post(&g_mutex)");
		}

		ret_chk = sem_post(&g_empty);
		if (ret_chk == ERROR) {
			pthread_exit("sem_post(&g_empty)");
		}
	}
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :tc_semaphore_sem_post_wait
* @brief                :this tc tests sem_post, sem_wait
* @scenario             :Unlocks the semaphore passed as argument.
* API's covered         :sem_post, sem_wait
* Preconditions         :none
* Postconditions        :Semaphore should be destroyed.
* @return               :void
*/
static void tc_semaphore_sem_post_wait(void)
{
	pthread_addr_t pexit_value = NULL;
	pthread_t pid;
	pthread_t cid;
	int ret_chk;

#ifndef CONFIG_DEBUG
	/* sem_wait test NULL case */

	ret_chk = sem_wait(NULL);
	TC_ASSERT_EQ("sem_wait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_wait", get_errno(), EINVAL);
#endif

	/* sem_post test NULL case */

	ret_chk = sem_post(NULL);
	TC_ASSERT_EQ("sem_post", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_post", get_errno(), EINVAL);

	/* sem_wait & sem post ping-pong test */

	ret_chk = sem_init(&g_empty, PSHARED, 1);
	TC_ASSERT_EQ_CLEANUP("sem_init", ret_chk, OK, goto cleanup);

	ret_chk = sem_init(&g_full, PSHARED, 0);
	TC_ASSERT_EQ_CLEANUP("sem_init", ret_chk, OK, goto cleanup);

	ret_chk = sem_init(&g_sem, PSHARED, 1);
	TC_ASSERT_EQ_CLEANUP("sem_init", ret_chk, OK, goto cleanup);

	ret_chk = pthread_create(&pid, NULL, producer_func, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, goto cleanup);

	ret_chk = pthread_create(&cid, NULL, consumer_func, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, goto cleanup);

	ret_chk = pthread_join(pid, &pexit_value);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret_chk, OK, goto cleanup);
	TC_ASSERT_EQ_CLEANUP("pthread_join", pexit_value, NULL, goto cleanup);

	ret_chk = pthread_join(cid, &pexit_value);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret_chk, OK, goto cleanup);
	TC_ASSERT_EQ_CLEANUP("pthread_join", pexit_value, NULL, goto cleanup);

	TC_SUCCESS_RESULT();

cleanup:
	sem_destroy(&g_empty);
	sem_destroy(&g_full);
	sem_destroy(&g_sem);
}


/**
* @fn                   :tc_semaphore_sem_trywait
* @brief                :this tc tests sem_trywait
* @scenario             :if sem is NULL, sem_trywait returns ERROR and set errno to EINVAL
*                        else if sem is not available, it returns ERROR and set errno to EAGAIN
*                        else(==sem is available), it returns OK and get sem
* API's covered         :sem_trywait
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_semaphore_sem_trywait(void)
{
	sem_t sem;
	int ret_chk;

#ifndef CONFIG_DEBUG
	/* test NULL case */

	ret_chk = sem_trywait(NULL);
	TC_ASSERT_EQ("sem_trywait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_trywait", get_errno(), EINVAL);
#endif

	/* sem_trywait can't get sem */

	ret_chk = sem_init(&sem, PSHARED, 0);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_trywait(&sem);
	TC_ASSERT_EQ("sem_trywait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_trywait", get_errno(), EAGAIN);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	/* sem_trywait can get sem */

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_trywait(&sem);
	TC_ASSERT_EQ("sem_trywait", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_semaphore_sem_timedwait
* @brief                :this tc tests sem_timedwait
* @scenario             :if parameters are invalid, sem_timedwait returns ERROR
*                        time that the call should block if the decrement cannot be immediately performed.
* API's covered         :sem_timedwait
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_semaphore_sem_timedwait(void)
{
	struct timespec abstime;
	struct timespec curtime;
	sem_t sem;
	int ret_chk;

	/* init sem count to 1 */

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);
	ret_chk = clock_gettime(CLOCK_REALTIME, &abstime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);

#ifdef CONFIG_DEBUG
	/* NULL case test */

	ret_chk = sem_timedwait(&sem, NULL);
	TC_ASSERT_EQ("sem_timedwait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_timedwait", get_errno(), EINVAL);

	ret_chk = sem_timedwait(NULL, &abstime);
	TC_ASSERT_EQ("sem_timedwait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_timedwait", get_errno(), EINVAL);
#endif

	/* success to get sem case test */

	ret_chk = clock_gettime(CLOCK_REALTIME, &abstime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);

	abstime.tv_sec = abstime.tv_sec + SEC_2;
	abstime.tv_nsec = 0;

	ret_chk = sem_timedwait(&sem, &abstime);
	TC_ASSERT_EQ("sem_timedwait", ret_chk, OK);

	ret_chk = clock_gettime(CLOCK_REALTIME, &curtime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);
	TC_ASSERT_NEQ("sem_timedwait", abstime.tv_sec, curtime.tv_sec);

	ret_chk = sem_post(&sem);
	TC_ASSERT_EQ("sem_post", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	/* init sem count to 0 */

	ret_chk = sem_init(&sem, PSHARED, 0);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	/* invalid time test */

	abstime.tv_nsec = -1;
	ret_chk = sem_timedwait(&sem, &abstime);
	TC_ASSERT_EQ("sem_timedwait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_timedwait", get_errno(), EINVAL);

	abstime.tv_nsec = 1000000000;
	ret_chk = sem_timedwait(&sem, &abstime);
	TC_ASSERT_EQ("sem_timedwait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_timedwait", get_errno(), EINVAL);

	/* expired time test */

	ret_chk = clock_gettime(CLOCK_REALTIME, &abstime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);

	ret_chk = sem_timedwait(&sem, &abstime);
	TC_ASSERT_EQ("sem_timedwait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_timedwait", get_errno(), ETIMEDOUT);

	/* fail to get sem case test */

	ret_chk = clock_gettime(CLOCK_REALTIME, &abstime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);

	abstime.tv_sec = abstime.tv_sec + SEC_2;
	abstime.tv_nsec = 0;

	ret_chk = sem_timedwait(&sem, &abstime);
	TC_ASSERT_EQ("sem_timedwait", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_timedwait", get_errno(), ETIMEDOUT);

	ret_chk = clock_gettime(CLOCK_REALTIME, &curtime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);
	TC_ASSERT_EQ("sem_timedwait", abstime.tv_sec, curtime.tv_sec);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_semaphore_sem_destroy
* @brief                :this tc tests sem_destroy function
* @scenario             :if sem is NULL, it returns ERROR
*                        else, it returns OK
* API's covered         :sem_destroy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_semaphore_sem_destroy(void)
{
	sem_t sem;
	int ret_chk;

	/* NULL parameter check */

	ret_chk = sem_destroy(NULL);
	TC_ASSERT_EQ("sem_destroy", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_destroy", get_errno(), EINVAL);

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);
	TC_ASSERT_EQ("sem_destroy", sem.semcount, 1);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_PRIORITY_INHERITANCE
/**
* @fn                   :tc_semaphore_sem_setprotocol
* @brief                :Set semaphore protocol attribute
* @scenario             :if sem protocol is SEM_PRIO_NONE/SEM_PRIO_INHERIT, it returns OK else it returns ERROR
* API's covered         :sem_setprotocol
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_semaphore_sem_setprotocol(void)
{
	sem_t sem;
	int ret_chk;

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	ret_chk = sem_setprotocol(&sem, SEM_PRIO_NONE);
	TC_ASSERT_EQ("sem_setprotocol", ret_chk, OK);

	ret_chk = sem_setprotocol(&sem, SEM_PRIO_INHERIT);
	TC_ASSERT_EQ("sem_setprotocol", ret_chk, OK);

	ret_chk = sem_setprotocol(&sem, SEM_PRIO_PROTECT);
	TC_ASSERT_EQ("sem_setprotocol", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_setprotocol", errno, ENOSYS);

	ret_chk = sem_setprotocol(&sem, SEM_PRIO_DEFAULT);
	TC_ASSERT_EQ("sem_setprotocol", ret_chk, ERROR);
	TC_ASSERT_EQ("sem_setprotocol", errno, EINVAL);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);
	TC_ASSERT_EQ("sem_destroy", sem.semcount, 1);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_semaphore_sem_tickwait
* @brief                :lighter weight version of sem_timedwait() - will lock the semaphore referenced by sem as in the sem_wait() function
* @scenario             :if parameters are invalid, sem_timedwait returns ERROR
*                        else, it returns OK
* API's covered         :sem_tickwait
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_semaphore_sem_tickwait(void)
{
	sem_t sem;
	int ret_chk;
	struct timespec abstime;
	struct timespec curtime;

	/* init sem count to 1 */

	ret_chk = sem_init(&sem, PSHARED, 1);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	/* success to get sem case test */

	ret_chk = clock_gettime(CLOCK_REALTIME, &abstime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);

	ret_chk = sem_tickwait(&sem, clock(), SEC_2);
	TC_ASSERT_EQ("sem_tickwait", ret_chk, OK);

	ret_chk = clock_gettime(CLOCK_REALTIME, &curtime);
	TC_ASSERT_EQ("clock_gettime", ret_chk, OK);
	TC_ASSERT_NEQ("sem_timedwait", abstime.tv_sec + SEC_2, curtime.tv_sec);

	ret_chk = sem_post(&sem);
	TC_ASSERT_EQ("sem_post", ret_chk, OK);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	/* init sem count to 0 */

	ret_chk = sem_init(&sem, PSHARED, 0);
	TC_ASSERT_EQ("sem_init", ret_chk, OK);

	/* expired time test */

	ret_chk = sem_tickwait(&sem, clock() - SEC_2, 0);
	TC_ASSERT_EQ("sem_tickwait", ret_chk, ERROR);

	ret_chk = sem_tickwait(&sem, clock() - SEC_2, 1);
	TC_ASSERT_EQ("sem_tickwait", ret_chk, ERROR);

	ret_chk = sem_tickwait(&sem, clock() - SEC_2, 3);
	TC_ASSERT_EQ("sem_tickwait", ret_chk, ERROR);

	ret_chk = sem_destroy(&sem);
	TC_ASSERT_EQ("sem_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}
/****************************************************************************
 * Name: semaphore
 ****************************************************************************/

int semaphore_main(void)
{
	tc_semaphore_sem_destroy();
	tc_semaphore_sem_post_wait();
#ifdef CONFIG_PRIORITY_INHERITANCE
	tc_semaphore_sem_setprotocol();
#endif
	tc_semaphore_sem_tickwait();
	tc_semaphore_sem_trywait();
	tc_semaphore_sem_timedwait();

	return 0;
}
