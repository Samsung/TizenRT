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

/// @file semaphore.c
/// @brief Test Case Example for semaphore API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include "tc_internal.h"

#define HALF_SEC        500000
#define SEC_1           1

int g_pshared = 0;
int g_count = 1;

sem_t g_empty;
sem_t g_full;
sem_t g_mutex;
int g_loop_cnt = 5;

/**
* @fn                   :producer_func
* @description          :Function for tc_semaphore_sem_init_post_wait
* @return               :void*
*/
static void *producer_func(void *arg)
{
	int index;
	int ret_val;
	for (index = 0; index < g_loop_cnt; index++) {
		ret_val = sem_wait(&g_empty);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
		}
		ret_val = sem_wait(&g_mutex);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
		}
		//printf("Producer val %d\n", index);
		ret_val = sem_post(&g_mutex);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
		}
		ret_val = sem_post(&g_full);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
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
	int ret_val;
	for (index = 0; index < g_loop_cnt; index++) {
		ret_val = sem_wait(&g_full);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
		}
		ret_val = sem_wait(&g_mutex);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
		}
		ret_val = sem_post(&g_mutex);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
		}
		ret_val = sem_post(&g_empty);
		if (ret_val == ERROR) {
			printf("tc_semaphore_sem_init_post_wait FAIL, Error No: %d\n", errno);
			total_fail++;
			return NULL;
		}
	}
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :tc_semaphore_sem_init_post_wait
* @brief                :Unlocks the semaphore passed as argument.
* @scenario             :Unlocks the semaphore passed as argument.
* API's covered         :sem_init, sem_post, sem_wait
* Preconditions         :Semaphore should be initialised.
* Postconditions        :Semaphore should be destroyed.
* @return               :void
*/
static void tc_semaphore_sem_init_post_wait(void)
{
	int ret_val = ERROR;
	pthread_t pid;
	pthread_t cid;

	ret_val = sem_init(&g_empty, g_pshared, g_count);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_init_post_wait FAIL : sem_init FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_val = sem_init(&g_full, g_pshared, 0);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_init_post_wait FAIL : sem_init FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_val = sem_init(&g_mutex, g_pshared, g_count);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_init_post_wait FAIL : sem_init FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = pthread_create(&pid, NULL, producer_func, NULL);
	if (ret_val != OK) {
		printf("tc_semaphore_sem_init_post_wait, pthread_create FAIL Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_val = pthread_create(&cid, NULL, consumer_func, NULL);
	if (ret_val != OK) {
		printf("tc_semaphore_sem_init_post_wait, pthread_create FAIL Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = pthread_join(pid, NULL);
	if (ret_val != OK) {
		printf("tc_semaphore_sem_init_post_wait, pthread_join FAIL Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_val = pthread_join(cid, NULL);
	if (ret_val != OK) {
		printf("tc_semaphore_sem_init_post_wait, pthread_join FAIL Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_semaphore_sem_init_post_wait PASS\n");
	total_pass++;
	sem_destroy(&g_empty);
	sem_destroy(&g_full);
	sem_destroy(&g_mutex);
}

/**
* @fn                   :tc_semaphore_sem_trywait
* @brief                :sem_trywait() is the same as sem_wait(), except that if the decrement cannot be
*                        immediately performed, then call returns an error (errno set to EAGAIN)
*                        instead of blocking.
* @scenario             :sem_trywait() is the same as sem_wait(), except that if the decrement cannot be
*                        immediately performed, then call returns an error (errno set to EAGAIN)
*                        instead of blocking.
* API's covered         :sem_init, sem_trywait
* Preconditions         :Semaphore should be initialised and should be unlocked.
* Postconditions        :Semaphore should be destroyed.
* @return               :void
*/

static void tc_semaphore_sem_trywait(void)
{
	int ret_val = ERROR;
	sem_t sem_name;

	ret_val = sem_init(&sem_name, g_pshared, 0);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_trywait FAIL : sem_init FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* semname count is 0, sem_trywait will not block, but return immediately with error */
	ret_val = sem_trywait(&sem_name);
	if (ret_val != ERROR) {
		printf("tc_semaphore_sem_trywait FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	sem_destroy(&sem_name);

	ret_val = sem_init(&sem_name, g_pshared, g_count);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_trywait FAIL : sem_init FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_val = sem_trywait(&sem_name);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_trywait FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = sem_post(&sem_name);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_trywait FAIL : sem_post Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* locking semaphore to test sem_trywait */
	ret_val = sem_wait(&sem_name);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_trywait FAIL : Semaphore not locked so skipping scenario to test sem_trywait in locked semaphore state \n");
	} else {
		/* locking semaphore in locked state using sem_trywait will return ERROR on pass */
		ret_val = sem_trywait(&sem_name);
		if (ret_val != ERROR) {
			printf("tc_semaphore_sem_trywait FAIL \n");
			total_fail++;
			RETURN_ERR;
		}
	}

	printf("tc_semaphore_sem_trywait PASS \n");
	total_pass++;
	sem_destroy(&sem_name);
}

/**
* @fn                   :tc_semaphore_sem_timedwait
* @brief                :same as sem_wait(), except that abs_timeout specifies a limit on the amount of
*                        time that the call should block if the decrement cannot be immediately performed.
* @scenario             :same as sem_wait(), except that abs_timeout specifies a limit on the amount of
*                        time that the call should block if the decrement cannot be immediately performed.
* API's covered         :sem_init, sem_timedwait
* Preconditions         :Semaphore should be initialised and timespec structure should be specified.
* Postconditions        :Semaphore should be destroyed.
* @return               :void
*/
static void tc_semaphore_sem_timedwait(void)
{
	int ret_val = ERROR;
	struct timespec st_set_time;
	struct timespec st_init_time;
	struct timespec st_final_time;
	sem_t sem_name;
	ret_val = sem_init(&sem_name, g_pshared, g_count);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_timedwait FAIL : sem_init FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = clock_gettime(CLOCK_REALTIME, &st_init_time);
	if (ret_val != OK) {
		printf("tc_semaphore_sem_timedwait FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	st_set_time.tv_sec = st_init_time.tv_sec + SEC_1;

	ret_val = sem_timedwait(&sem_name, &st_set_time);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_timedwait FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = clock_gettime(CLOCK_REALTIME, &st_final_time);
	if (ret_val != OK) {
		printf("tc_semaphore_sem_timedwait FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if (g_count == 0) {
		if (st_init_time.tv_sec + SEC_1 != st_final_time.tv_sec) {
			printf("tc_semaphore_sem_timedwait FAIL, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	}
	printf("tc_semaphore_sem_timedwait PASS \n");
	total_pass++;
	sem_destroy(&sem_name);
}

/**
* @fn                   :tc_semaphore_sem_destroy
* @brief                :Destroy an unnamed semaphore
* @scenario             :Destroy an unnamed semaphore
* API's covered         :sem_init, sem_destroy
* Preconditions         :Semaphore should be initialised
* Postconditions        :Semaphore should be destroyed.
* @return               :void
*/
static void tc_semaphore_sem_destroy(void)
{
	int ret_val = ERROR;
	sem_t sem_name;
	ret_val = sem_init(&sem_name, g_pshared, g_count);
	if (ret_val == ERROR) {
		printf("tc_semaphore_sem_destroy FAIL : sem_init FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_val = sem_destroy(&sem_name);
	if (ret_val != OK) {
		printf("tc_semaphore_sem_destroy FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* The effect of subsequent use of the semaphore sem is undefined until sem is re-initialised
	   by another call to sem_init(). */
	printf("tc_semaphore_sem_destroy PASS\n");
	total_pass++;
}

/****************************************************************************
 * Name: semaphore
 ****************************************************************************/

int semaphore_main(void)
{
	tc_semaphore_sem_init_post_wait();
	tc_semaphore_sem_trywait();
	tc_semaphore_sem_timedwait();	/*tc fails if g_count is set 0 */
	tc_semaphore_sem_destroy();

	return 0;
}
