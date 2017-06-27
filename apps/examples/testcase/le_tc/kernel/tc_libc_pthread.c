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

/// @file tc_libc_pthread.c

/// @brief Test Case Example for Libc Pthread API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <tinyara/pthread.h>
#include "tc_internal.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define STACKSIZE    PTHREAD_STACK_DEFAULT
#define PRIORITY     PTHREAD_DEFAULT_PRIORITY
#define POLICY       PTHREAD_DEFAULT_POLICY
#define INHERITSCHED PTHREAD_EXPLICIT_SCHED

/****************************************************************************
 * Global Variables
 ****************************************************************************/

struct race_cond_s {
	sem_t *sem1;
	sem_t *sem2;
	pthread_rwlock_t *rw_lock;
};

/****************************************************************************
 * Public Functions
 **************************************************************************/

static int g_race_cond_thread_pos;

/****************************************************************************
  * Private Functions
 ****************************************************************************/

/**
* @fn                   :tc_libc_pthread_pthread_attr_init
* @brief                :This tc test pthread_attr_init()
* @Scenario             :If pthread_attr is NULL, ENOMEM is returned.
*                        Else, it return OK and pthread_attr is set to default value
* @API'scovered         :pthread_attr_init
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_init(void)
{
	pthread_attr_t attr;
	int ret_chk;

	ret_chk = pthread_attr_init(NULL);
	TC_ASSERT_EQ("pthread_attr_init", ret_chk, ENOMEM);

	ret_chk = pthread_attr_init(&attr);
	TC_ASSERT_EQ("pthread_attr_init", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_init", attr.stacksize, PTHREAD_STACK_DEFAULT);
	TC_ASSERT_EQ("pthread_attr_init", attr.priority, PTHREAD_DEFAULT_PRIORITY);
	TC_ASSERT_EQ("pthread_attr_init", attr.policy, PTHREAD_DEFAULT_POLICY);
	TC_ASSERT_EQ("pthread_attr_init", attr.inheritsched, PTHREAD_EXPLICIT_SCHED);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_destroy
* @brief                :This tc test pthread_attr_destroy()
* @Scenario             :If pthread_attr is NULL, EINVAL is returned.
*                        Else, it return OK and pthread_attr is set to zero
* @API'scovered         :pthread_attr_destroy
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_destroy(void)
{
	pthread_attr_t attr;
	pthread_attr_t destroyed_attr;
	int ret_chk;

	memset(&destroyed_attr, 0, sizeof(pthread_attr_t));

	ret_chk = pthread_attr_destroy(NULL);
	TC_ASSERT_EQ("pthread_attr_destroy", ret_chk, EINVAL);

	ret_chk = pthread_attr_destroy(&attr);
	TC_ASSERT_EQ("pthread_attr_destroy", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_destroy", memcmp(&attr, &destroyed_attr, sizeof(pthread_attr_t)), 0);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_getstacksize
* @brief                :This tc test pthread_attr_getstacksize()
* @Scenario             :If pthread_attr or stacksize parameter is NULL, EINVAL is returned.
*                        Else, it return OK and stacksize is set to the stack size of pthread_attr
* @API'scovered         :pthread_attr_getstacksize
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_getstacksize(void)
{
	pthread_attr_t attr;
	long stacksize;
	int ret_chk;

	attr.stacksize = STACKSIZE;

	ret_chk = pthread_attr_getstacksize(NULL, &stacksize);
	TC_ASSERT_EQ("pthread_attr_getstacksize", ret_chk, EINVAL);

	ret_chk = pthread_attr_getstacksize(&attr, NULL);
	TC_ASSERT_EQ("pthread_attr_getstacksize", ret_chk, EINVAL);

	ret_chk = pthread_attr_getstacksize(&attr, &stacksize);
	TC_ASSERT_EQ("pthread_attr_getstacksize", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_getstacksize", stacksize, attr.stacksize);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_setstacksize
* @brief                :This tc test pthread_attr_setstacksize()
* @Scenario             :If pthread_attr is NULL or stacksize is under PTHREAD_STACK_MIN , EINVAL is returned.
*                        Else, it return OK and stacksize of pthread_attr is set to stacksize parameter.
* @API'scovered         :pthread_attr_setstacksize
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_setstacksize(void)
{
	pthread_attr_t attr;
	long stacksize = STACKSIZE;
	int ret_chk;

	ret_chk = pthread_attr_setstacksize(NULL, stacksize);
	TC_ASSERT_EQ("pthread_attr_setstacksize", ret_chk, EINVAL);

	ret_chk = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN - 1);
	TC_ASSERT_EQ("pthread_attr_setstacksize", ret_chk, EINVAL);

	ret_chk = pthread_attr_setstacksize(&attr, stacksize);
	TC_ASSERT_EQ("pthread_attr_setstacksize", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_setstacksize", attr.stacksize, stacksize);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_getschedparam
* @brief                :This tc test pthread_attr_getschedparam()
* @Scenario             :If pthread_attr or sched_param parameter is NULL, EINVAL is returned.
*                        Else, it return OK and sched_priority of sched_param is set to the priority of pthread_attr
* @API'scovered         :pthread_attr_getschedparam
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_getschedparam(void)
{
	pthread_attr_t attr;
	struct sched_param param;
	int ret_chk;

	attr.priority = PRIORITY;

	ret_chk = pthread_attr_getschedparam(NULL, &param);
	TC_ASSERT_EQ("pthread_attr_getschedparam", ret_chk, EINVAL);

	ret_chk = pthread_attr_getschedparam(&attr, NULL);
	TC_ASSERT_EQ("pthread_attr_getschedparam", ret_chk, EINVAL);

	ret_chk = pthread_attr_getschedparam(&attr, &param);
	TC_ASSERT_EQ("pthread_attr_getschedparam", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_getschedparam", param.sched_priority, attr.priority);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_setschedparam
* @brief                :This tc test pthread_attr_setschedparam()
* @Scenario             :If pthread_attr or sched_param parameter is NULL, EINVAL is returned.
*                        Else, it return OK and sched_priority of sched_param is set to the priority of pthread_attr
* @API'scovered         :pthread_attr_setschedparam
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_setschedparam(void)
{
	pthread_attr_t attr;
	struct sched_param param;
	int ret_chk;

	param.sched_priority = PRIORITY;

	ret_chk = pthread_attr_setschedparam(NULL, &param);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, EINVAL);

	ret_chk = pthread_attr_setschedparam(&attr, NULL);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, EINVAL);

	ret_chk = pthread_attr_setschedparam(&attr, &param);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_setschedparam", attr.priority, param.sched_priority);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_getschedpolicy
* @brief                :This tc test pthread_attr_getschedpolicy()
* @Scenario             :If pthread_attr or policy is NULL, EINVAL is returned.
*                        Else, it return OK and policy is set to policy of pthread_attr_t
* @API'scovered         :pthread_attr_getschedpolicy
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_getschedpolicy(void)
{
	pthread_attr_t attr;
	int policy;
	int ret_chk;

	attr.policy = POLICY;

	ret_chk = pthread_attr_getschedpolicy(NULL, &policy);
	TC_ASSERT_EQ("pthread_attr_getschedpolicy", ret_chk, EINVAL);

	ret_chk = pthread_attr_getschedpolicy(&attr, NULL);
	TC_ASSERT_EQ("pthread_attr_getschedpolicy", ret_chk, EINVAL);

	ret_chk = pthread_attr_getschedpolicy(&attr, &policy);
	TC_ASSERT_EQ("pthread_attr_getschedpolicy", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_getschedpolicy", policy, attr.policy);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_setschedpolicy
* @brief                :This tc test pthread_attr_setschedpolicy()
* @Scenario             :If pthread_attr is NULL or policy parameter is invalid, EINVAL is returned.
*                        Else, it return OK and inheritsched of pthread_attr is set to inheritsched
* @API'scovered         :pthread_attr_setschedpolicy
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_setschedpolicy(void)
{
	pthread_attr_t attr;
	int policy;
	int ret_chk;

	policy = SCHED_FIFO;
	ret_chk = pthread_attr_setschedpolicy(NULL, policy);
	TC_ASSERT_EQ("pthread_attr_setschedpolicy", ret_chk, EINVAL);

	policy = -1;
	ret_chk = pthread_attr_setschedpolicy(&attr, policy);
	TC_ASSERT_EQ("pthread_attr_setschedpolicy", ret_chk, EINVAL);

	policy = SCHED_FIFO;
	ret_chk = pthread_attr_setschedpolicy(&attr, policy);
	TC_ASSERT_EQ("pthread_attr_setschedpolicy", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_setschedpolicy", attr.policy, policy);

	policy = SCHED_RR;
	ret_chk = pthread_attr_setschedpolicy(&attr, policy);
#if CONFIG_RR_INTERVAL > 0
	TC_ASSERT_EQ("pthread_attr_setschedpolicy", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_setschedpolicy", attr.policy, policy);
#else
	TC_ASSERT_EQ("pthread_attr_setschedpolicy", ret_chk, EINVAL);
#endif

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_getinheritsched
* @brief                :This tc test pthread_attr_getinheritsched()
* @Scenario             :If pthread_attr or inheritsched is NULL, EINVAL is returned.
*                        Else, it return OK and inheritsched is set to inheritsched of pthread_attr_t
* @API'scovered         :pthread_attr_getinheritsched
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_getinheritsched(void)
{
	pthread_attr_t attr;
	int inheritsched;
	int ret_chk;

	attr.inheritsched = INHERITSCHED;

	ret_chk = pthread_attr_getinheritsched(NULL, &inheritsched);
	TC_ASSERT_EQ("pthread_attr_getinheritsched", ret_chk, EINVAL);

	ret_chk = pthread_attr_getinheritsched(&attr, NULL);
	TC_ASSERT_EQ("pthread_attr_getinheritsched", ret_chk, EINVAL);

	ret_chk = pthread_attr_getinheritsched(&attr, &inheritsched);
	TC_ASSERT_EQ("pthread_attr_getinheritsched", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_getinheritsched", inheritsched, attr.inheritsched);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_setinheritsched
* @brief                :This tc test pthread_attr_setinheritsched()
* @Scenario             :If pthread_attr is NULL or inheritsched parameter is invalid, EINVAL is returned.
*                        Else, it return OK and inheritsched of pthread_attr is set to inheritsched
* @API'scovered         :pthread_attr_setinheritsched
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_setinheritsched(void)
{
	pthread_attr_t attr;
	int inheritsched;
	int ret_chk;

	inheritsched = PTHREAD_INHERIT_SCHED;
	ret_chk = pthread_attr_setinheritsched(NULL, inheritsched);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, EINVAL);

	inheritsched = -1;
	ret_chk = pthread_attr_setinheritsched(&attr, inheritsched);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, EINVAL);

	inheritsched = PTHREAD_INHERIT_SCHED;
	ret_chk = pthread_attr_setinheritsched(&attr, inheritsched);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_setschedparam", attr.inheritsched, inheritsched);

	inheritsched = PTHREAD_EXPLICIT_SCHED;
	ret_chk = pthread_attr_setinheritsched(&attr, inheritsched);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, OK);
	TC_ASSERT_EQ("pthread_attr_setschedparam", attr.inheritsched, inheritsched);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_barrierattr_init
* @brief                :This tc test pthread_barrierattr_init()
* @Scenario             :If pthread_barrierattr is NULL, EINVAL is returned.
*                        Else, it return OK and pthread_barrierattr is set to default value
* @API'scovered         :pthread_barrierattr_init
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_barrierattr_init(void)
{
	pthread_barrierattr_t attr;
	int ret_chk;

	ret_chk = pthread_barrierattr_init(NULL);
	TC_ASSERT_EQ("pthread_barrierattr_init", ret_chk, EINVAL);

	ret_chk = pthread_barrierattr_init(&attr);
	TC_ASSERT_EQ("pthread_barrierattr_init", ret_chk, OK);
	TC_ASSERT_EQ("pthread_barrierattr_init", attr.pshared, PTHREAD_PROCESS_PRIVATE);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_barrierattr_destroy
* @brief                :This tc test pthread_barrierattr_destroy()
* @Scenario             :If pthread_barrierattr_destroy is NULL, EINVAL is returned.
*                        Else, it return OK and pthread_barrierattr is set to default value
* @API'scovered         :pthread_barrierattr_destroy
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_barrierattr_destroy(void)
{
	pthread_barrierattr_t attr;
	int ret_chk;

	ret_chk = pthread_barrierattr_destroy(NULL);
	TC_ASSERT_EQ("pthread_barrierattr_destroy", ret_chk, EINVAL);

	ret_chk = pthread_barrierattr_destroy(&attr);
	TC_ASSERT_EQ("pthread_barrierattr_destroy", ret_chk, OK);
	TC_ASSERT_EQ("pthread_barrierattr_destroy", attr.pshared, PTHREAD_PROCESS_PRIVATE);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_barrierattr_getpshared
* @brief                :This tc test pthread_barrierattr_getpshared()
* @Scenario             :If pthread_barrierattr or psahred is NULL, EINVAL is returned.
*                        Else, it return OK and pshared is set to psahred of pthread_barrierattr
* @API'scovered         :pthread_barrierattr_getpshared
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_barrierattr_getpshared(void)
{
	pthread_barrierattr_t attr;
	int pshared;
	int ret_chk;

	ret_chk = pthread_barrierattr_getpshared(NULL, &pshared);
	TC_ASSERT_EQ("pthread_barrierattr_getpshared", ret_chk, EINVAL);

	ret_chk = pthread_barrierattr_getpshared(&attr, NULL);
	TC_ASSERT_EQ("pthread_barrierattr_getpshared", ret_chk, EINVAL);

	attr.pshared = PTHREAD_PROCESS_PRIVATE;
	ret_chk = pthread_barrierattr_getpshared(&attr, &pshared);
	TC_ASSERT_EQ("pthread_barrierattr_getpshared", ret_chk, OK);
	TC_ASSERT_EQ("pthread_barrierattr_getpshared", pshared, attr.pshared);

	attr.pshared = PTHREAD_PROCESS_SHARED;
	ret_chk = pthread_barrierattr_getpshared(&attr, &pshared);
	TC_ASSERT_EQ("pthread_barrierattr_getpshared", ret_chk, OK);
	TC_ASSERT_EQ("pthread_barrierattr_getpshared", pshared, attr.pshared);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_barrierattr_setpshared
* @brief                :This tc test pthread_attr_setinheritsched()
* @Scenario             :If pthread_attr is NULL or inheritsched parameter is invalid, EINVAL is returned.
*                        Else, it return OK and pshared of pthread_barrierattr is set to pshared.
* @API'scovered         :pthread_barrierattr_setpshared
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_barrierattr_setpshared(void)
{
	pthread_barrierattr_t attr;
	int pshared;
	int ret_chk;

	pshared = PTHREAD_PROCESS_PRIVATE;
	ret_chk = pthread_barrierattr_setpshared(NULL, pshared);
	TC_ASSERT_EQ("pthread_barrierattr_setpshared", ret_chk, EINVAL);

	pshared = -1;
	ret_chk = pthread_barrierattr_setpshared(&attr, pshared);
	TC_ASSERT_EQ("pthread_barrierattr_setpshared", ret_chk, EINVAL);

	pshared = PTHREAD_PROCESS_PRIVATE;
	ret_chk = pthread_barrierattr_setpshared(&attr, pshared);
	TC_ASSERT_EQ("pthread_barrierattr_setpshared", ret_chk, OK);
	TC_ASSERT_EQ("pthread_barrierattr_setpshared", attr.pshared, pshared);

	pshared = PTHREAD_PROCESS_SHARED;
	ret_chk = pthread_barrierattr_setpshared(&attr, pshared);
	TC_ASSERT_EQ("pthread_barrierattr_setpshared", ret_chk, OK);
	TC_ASSERT_EQ("pthread_barrierattr_setpshared", attr.pshared, pshared);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_condattr_init
* @brief                :This tc test pthread_condattr_init()
* @Scenario             :If pthread_condattr is NULL, EINVAL is returned.
*                        Else, it return OK and pthread_condattr is set to 0 value
* @API'scovered         :pthread_condattr_init
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_condattr_init(void)
{
	pthread_condattr_t attr;
	int ret_chk;

	ret_chk = pthread_condattr_init(NULL);
	TC_ASSERT_EQ("pthread_condattr_init", ret_chk, EINVAL);

	ret_chk = pthread_condattr_init(&attr);
	TC_ASSERT_EQ("pthread_condattr_init", ret_chk, OK);
	TC_ASSERT_EQ("pthread_condattr_init", attr, 0);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_condattr_destroy
* @brief                :This tc test pthread_condattr_destroy()
* @Scenario             :If pthread_condattr_destroy is NULL, EINVAL is returned.
*                        Else, it return OK
* @API'scovered         :pthread_condattr_destroy
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_condattr_destroy(void)
{
	pthread_condattr_t attr;
	int ret_chk;

	ret_chk = pthread_condattr_destroy(NULL);
	TC_ASSERT_EQ("pthread_condattr_destroy", ret_chk, EINVAL);

	ret_chk = pthread_condattr_destroy(&attr);
	TC_ASSERT_EQ("pthread_condattr_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_mutexattr_init
* @brief                :This tc test pthread_mutexattr_init()
* @Scenario             :If pthread_mutexattr is NULL, EINVAL is returned.
*                        Else, it return OK and pthread_mutexattr is set to default value
* @API'scovered         :pthread_mutexattr_init
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_mutexattr_init(void)
{
	pthread_mutexattr_t attr;
	int ret_chk;

	ret_chk = pthread_mutexattr_init(NULL);
	TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, EINVAL);

	ret_chk = pthread_mutexattr_init(&attr);
	TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, OK);
	TC_ASSERT_EQ("pthread_mutexattr_init", attr.pshared, 0);
#ifdef CONFIG_PTHREAD_MUTEX_TYPES
	TC_ASSERT_EQ("pthread_mutexattr_init", attr.type, PTHREAD_MUTEX_DEFAULT);
#endif
#ifdef CONFIG_PTHREAD_MUTEX_BOTH
#ifdef CONFIG_PTHREAD_MUTEX_DEFAULT_UNSAFE
	TC_ASSERT_EQ("pthread_mutexattr_init", attr.robust, PTHREAD_MUTEX_STALLED);
#else
	TC_ASSERT_EQ("pthread_mutexattr_init", attr.robust, PTHREAD_MUTEX_ROBUST);
#endif
#endif

	TC_SUCCESS_RESULT();
	return;
}

/**
* @fn                   :tc_libc_pthread_pthread_mutexattr_destroy
* @brief                :This tc test pthread_mutexattr_destroy()
* @Scenario             :If pthread_condattr_destroy is NULL, EINVAL is returned.
*                        Else, it return OK
* @API'scovered         :pthread_mutexattr_destroy
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_libc_pthread_pthread_mutexattr_destroy(void)
{
	pthread_mutexattr_t attr;
	int ret_chk;

	ret_chk = pthread_mutexattr_destroy(NULL);
	TC_ASSERT_EQ("pthread_mutexattr_destroy", ret_chk, EINVAL);

	ret_chk = pthread_mutexattr_destroy(&attr);
	TC_ASSERT_EQ("pthread_mutexattr_destroy", ret_chk, OK);
	TC_ASSERT_EQ("pthread_mutexattr_destroy", attr.pshared, 0);

	TC_SUCCESS_RESULT();
	return;
}

/****************************************************************************
 * Name: Test Case API's for libc_pthread_rwlock
 ****************************************************************************/
static void *race_cond_thread1(void *data)
{
	// Runs 1st
	int status;
	struct race_cond_s *rc = (struct race_cond_s *)data;

	TC_ASSERT_EQ_RETURN("race_cond_thread1", g_race_cond_thread_pos++, OK, NULL);

	status = pthread_rwlock_wrlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_wrlock", status, OK, NULL);

	sem_post(rc->sem2);
	sem_wait(rc->sem1);
	// Context Switch -> Runs 3rd

	TC_ASSERT_EQ_RETURN("race_cond_thread1", g_race_cond_thread_pos++, 2, NULL);

	status = pthread_rwlock_unlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_unlock", status, OK, NULL);

	status = pthread_rwlock_rdlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_rdlock", status, OK, NULL);

	sem_wait(rc->sem1);
	// Context Switch - Runs 5th

	TC_ASSERT_EQ_RETURN("race_cond_thread1", g_race_cond_thread_pos++, 4, NULL);

	status = pthread_rwlock_unlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_unlock", status, OK, NULL);

	status = pthread_rwlock_rdlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_rdlock", status, OK, NULL);

	sem_post(rc->sem2);
	sem_wait(rc->sem1);

	/* Context switch - Runs 7th */

	TC_ASSERT_EQ_RETURN("race_cond_thread1", g_race_cond_thread_pos++, 6, NULL);

	status = pthread_rwlock_unlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_unlock", status, OK, NULL);

	return NULL;

}

static void *race_cond_thread2(void *data)
{
	int status;
	struct race_cond_s *rc = (struct race_cond_s *)data;

	status = sem_wait(rc->sem2);
	// Runs 2nd
	TC_ASSERT_EQ_RETURN("sem_wait", status, OK, NULL);

	TC_ASSERT_EQ_RETURN("race_cond_thread2", g_race_cond_thread_pos++, 1, NULL);

	status = pthread_rwlock_tryrdlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_tryrdlock", status, EBUSY, NULL);

	status = pthread_rwlock_trywrlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_trywrlock", status, EBUSY, NULL);

	sem_post(rc->sem1);
	status = pthread_rwlock_rdlock(rc->rw_lock);
	// Context - Switch Runs 4th
	TC_ASSERT_EQ_RETURN("pthread_rwlock_rdlock", status, OK, NULL);

	TC_ASSERT_EQ_RETURN("race_cond_thread2", g_race_cond_thread_pos++, 3, NULL);

	status = pthread_rwlock_unlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_unlock", status, OK, NULL);

	sem_post(rc->sem1);
	sem_wait(rc->sem2);

	/* Context switch Runs 6th */

	TC_ASSERT_EQ_RETURN("race_cond_thread2", g_race_cond_thread_pos++, 5, NULL);

	sem_post(rc->sem1);
	status = pthread_rwlock_wrlock(rc->rw_lock);

	/* Context switch runs 8th */

	TC_ASSERT_EQ_RETURN("pthread_rwlock_wrlock", status, OK, NULL);

	TC_ASSERT_EQ_RETURN("race_cond_thread2", g_race_cond_thread_pos++, 7, NULL);

	status = pthread_rwlock_unlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_unlock", status, OK, NULL);

	return NULL;

}

static void test_two_threads(void)
{
	pthread_t thread1;
	pthread_t thread2;
	int status;
	pthread_rwlock_t rw_lock;
	sem_t sem1;
	sem_t sem2;
	struct race_cond_s rc;

	status = pthread_rwlock_init(&rw_lock, NULL);
	TC_ASSERT_EQ("pthread_rwlock_init", status, OK);

	status = sem_init(&sem1, 0, 0);
	TC_ASSERT_EQ("sem_init", status, OK);

	status = sem_init(&sem2, 0, 0);
	TC_ASSERT_EQ("sem_init", status, OK);

	rc.sem1 = &sem1;
	rc.sem2 = &sem2;
	rc.rw_lock = &rw_lock;

	g_race_cond_thread_pos = 0;
	status = pthread_create(&thread1, NULL, race_cond_thread1, &rc);
	status = pthread_create(&thread2, NULL, race_cond_thread2, &rc);
	(void)pthread_join(thread1, NULL);
	(void)pthread_join(thread2, NULL);

}

static void *timeout_thread1(FAR void *data)
{
	FAR struct race_cond_s *rc = (FAR struct race_cond_s *)data;
	int status;

	status = pthread_rwlock_wrlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_wrlock", status, OK, NULL);

	sem_wait(rc->sem1);

	status = pthread_rwlock_unlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_unlock", status, OK, NULL);

	return NULL;
}

static void *timeout_thread2(FAR void *data)
{
	FAR struct race_cond_s *rc = (FAR struct race_cond_s *)data;
	struct timespec time;
	int status;

	status = clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += 2;

	status = pthread_rwlock_timedwrlock(rc->rw_lock, &time);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_timedwrlock", status, ETIMEDOUT, NULL);

	status = clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += 2;

	status = pthread_rwlock_timedrdlock(rc->rw_lock, &time);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_timedrdlock", status, ETIMEDOUT, NULL);

	status = clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += 2;

	sem_post(rc->sem1);
	status = pthread_rwlock_timedrdlock(rc->rw_lock, &time);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_timedrdlock", status, OK, NULL);

	status = pthread_rwlock_unlock(rc->rw_lock);
	TC_ASSERT_EQ_RETURN("pthread_rwlock_unlock", status, OK, NULL);

	return NULL;
}

/**
* @fn                   :tc_libc_pthread_pthread_rwlock_rdlock_wrlock
* @Description          :creates and initializes a new read-write lock object with specified attributes and \
*                        creates two threads using pthread_create and tries acquiring read & write locks referenced rwlock \
*                        destroys the read-write lock object referenced by rwlock and release any resources used by the lock started with pthread_rwlock_init.
* API's covered         :pthread_rwlock_init, pthread_rwlock_wrlock, pthread_rwlock_rdlock, pthread_rwlock_trywrlock, pthread_rwlock_tryrdlock, pthread_rwlock_unlock
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_libc_pthread_pthread_rwlock_rdlock_wrlock(void)
{
	int status;
	pthread_rwlock_t rw_lock;

	status = pthread_rwlock_init(&rw_lock, NULL);
	TC_ASSERT_EQ("pthread_rwlock_init", status, OK);

	status = pthread_rwlock_trywrlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_trywrlock", status, OK);

	status = pthread_rwlock_trywrlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_trywrlock", status, EBUSY);

	status = pthread_rwlock_tryrdlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_tryrdlock", status, EBUSY);

	status = pthread_rwlock_unlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_unlock", status, OK);

	test_two_threads();

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_pthread_pthread_rwlock_test_timeout
* @Description          :creates and initializes a new read-write lock object with specified attributes and \
*                        tries acquiring timed read & write lock referenced rwlock multple times and checks whether it times out correctly \
*                        destroys the read-write lock object referenced by rwlock and release any resources used by the lock started with pthread_rwlock_init.
* API's covered         :pthread_rwlock_init, pthread_rwlock_timedwrlock, pthread_rwlock_timedrdlock, pthread_rwlock_unlock
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_libc_pthread_pthread_rwlock_test_timeout(void)
{

	pthread_rwlock_t rw_lock;
	struct race_cond_s rc;
	pthread_t thread1;
	pthread_t thread2;
	int status;
	sem_t sem1;
	sem_t sem2;

	status = pthread_rwlock_init(&rw_lock, NULL);
	TC_ASSERT_EQ("pthread_rwlock_init", status, OK);

	status = sem_init(&sem1, 0, 0);
	TC_ASSERT_EQ("sem_init", status, OK);

	status = sem_init(&sem2, 0, 0);
	TC_ASSERT_EQ("sem_init", status, OK);

	rc.sem1 = &sem1;
	rc.sem2 = &sem2;
	rc.rw_lock = &rw_lock;

	status = pthread_create(&thread1, NULL, timeout_thread1, &rc);
	status = pthread_create(&thread2, NULL, timeout_thread2, &rc);

	(void)pthread_join(thread1, NULL);
	(void)pthread_join(thread2, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_pthread_pthread_rwlock_init_unlock_destroy
* @Description          :creates and initializes a new read-write lock object with specified attributes and \
*			 unlock releases the lock held on the read-write lock object referenced by rwlock and then \
*                        destroys the read-write lock object referenced by rwlock and release any resources used by the lock started with pthread_rwlock_init.
* API's covered         :pthread_rwlock_init, pthread_rwlock_wrlock, pthread_rwlock_unlock, pthread_rwlock_destroy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_libc_pthread_pthread_rwlock_init_unlock_destroy(void)
{
	int status;
	pthread_rwlock_t rw_lock;
	pthread_rwlockattr_t attr;

	status = pthread_rwlock_init(NULL, &attr);
	TC_ASSERT_EQ("pthread_rwlock_init", status, EINVAL);

	status = pthread_rwlock_init(&rw_lock, NULL);
	TC_ASSERT_EQ("pthread_rwlock_init", status, OK);

	status = pthread_rwlock_wrlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_wrlock", status, OK);

	status = pthread_rwlock_unlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_unlock", status, OK);

	status = pthread_rwlock_destroy(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_destroy", status, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_pthread_pthread_rwlock_tryrdlock
* @Description          :creates and initializes a new read-write lock object with specified attributes and \
*                        pthread_rwlock_tryrdlock applies a timed read lock to the read-write lock referenced by rwlock \
*			 The calling thread acquires the read lock if a writer does not hold the lock and there are no writers blocked on the lock and then \
*                        unlocks & destroys the rw lock object referenced by rwlock & release any resources used by the lock started with pthread_rwlock_init.
* API's covered         :pthread_rwlock_init, pthread_rwlock_tryrdlock, pthread_rwlock_unlock, pthread_rwlock_destroy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_libc_pthread_pthread_rwlock_tryrdlock(void)
{
	int status;
	pthread_rwlock_t rw_lock;

	status = pthread_rwlock_init(&rw_lock, NULL);
	TC_ASSERT_EQ("pthread_rwlock_init", status, OK);

	status = pthread_rwlock_tryrdlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_tryrdlock", status, OK);

	status = pthread_rwlock_unlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_unlock", status, OK);

	status = pthread_rwlock_destroy(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_destroy", status, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_pthread_pthread_rwlock_trywrlock
* @Description          :creates and initializes a new read-write lock object with specified attributes and \
*                        pthread_rwlock_trywrlock applies a timed write lock to the read-write lock referenced by rwlock \
*			 If the lock cannot be acquired without waiting for other threads to unlock the lock \
*			 this wait shall be terminated when the specified timeout expires and then \
*                        unlocks & destroys the read-write lock object referenced by rwlock and release any resources used by the lock started with pthread_rwlock_init.
* API's covered         :pthread_rwlock_init, pthread_rwlock_trywrlock, pthread_rwlock_unlock, pthread_rwlock_destroy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_libc_pthread_pthread_rwlock_trywrlock(void)
{
	int status;
	pthread_rwlock_t rw_lock;

	status = pthread_rwlock_init(&rw_lock, NULL);
	TC_ASSERT_EQ("pthread_rwlock_init", status, OK);

	status = pthread_rwlock_trywrlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_trywrlock", status, OK);

	status = pthread_rwlock_unlock(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_unlock", status, OK);

	status = pthread_rwlock_destroy(&rw_lock);
	TC_ASSERT_EQ("pthread_rwlock_destroy", status, OK);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_pthread
 ****************************************************************************/

int libc_pthread_main(void)
{
	tc_libc_pthread_pthread_attr_init();
	tc_libc_pthread_pthread_attr_destroy();
	tc_libc_pthread_pthread_attr_getstacksize();
	tc_libc_pthread_pthread_attr_setstacksize();
	tc_libc_pthread_pthread_attr_getschedparam();
	tc_libc_pthread_pthread_attr_setschedparam();
	tc_libc_pthread_pthread_attr_getschedpolicy();
	tc_libc_pthread_pthread_attr_setschedpolicy();
	tc_libc_pthread_pthread_attr_getinheritsched();
	tc_libc_pthread_pthread_attr_setinheritsched();
	tc_libc_pthread_pthread_barrierattr_init();
	tc_libc_pthread_pthread_barrierattr_destroy();
	tc_libc_pthread_pthread_barrierattr_getpshared();
	tc_libc_pthread_pthread_barrierattr_setpshared();
	tc_libc_pthread_pthread_condattr_init();
	tc_libc_pthread_pthread_condattr_destroy();
	tc_libc_pthread_pthread_mutexattr_init();
	tc_libc_pthread_pthread_mutexattr_destroy();
	tc_libc_pthread_pthread_rwlock_init_unlock_destroy();
	tc_libc_pthread_pthread_rwlock_tryrdlock();
	tc_libc_pthread_pthread_rwlock_trywrlock();
	tc_libc_pthread_pthread_rwlock_rdlock_wrlock();
	tc_libc_pthread_pthread_rwlock_test_timeout();

	return 0;
}
