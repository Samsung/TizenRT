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

#define STACKSIZE    PTHREAD_STACK_DEFAULT
#define PRIORITY     PTHREAD_DEFAULT_PRIORITY
#define POLICY       PTHREAD_DEFAULT_POLICY
#define INHERITSCHED PTHREAD_EXPLICIT_SCHED

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

	return 0;
}
