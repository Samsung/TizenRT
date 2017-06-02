//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
//*********************************************************************

/**
 * @file
 * This file provides APIs related to mutex with no operation
 * for Singlethread implementation.
 */
#include "octhread.h"

/**
 * TAG
 * Logging tag for module name
 */
#define TAG "UMUTEX"

typedef struct _tagMutexInfo_t
{
} oc_mutex_internal;

typedef struct _tagEventInfo_t
{
} oc_cond_internal;

typedef struct _tagThreadInfo_t
{
} oc_thread_internal;


/**
 * @var g_mutexInfo
 * @brief This is used to return a non NULL value for oc_mutex_new().
 */
static oc_mutex_internal g_mutexInfo = { 0 };

/**
 * @var g_condInfo
 * @brief This is used to return a non NULL value for oc_cond_new().
 */
static oc_cond_internal g_condInfo = { 0 };

OCThreadResult_t oc_thread_new(oc_thread *t, void *(*start_routine)(void *), void *arg)
{
    return OC_THREAD_CREATE_FAILURE;
}

OCThreadResult_t oc_thread_free(oc_thread t)
{
    return OC_THREAD_INVALID;
}

OCThreadResult_t oc_thread_wait(oc_thread t)
{
    return OC_THREAD_INVALID;
}

oc_mutex oc_mutex_new(void)
{
    return (oc_mutex)&g_mutexInfo;
}

bool oc_mutex_free(oc_mutex mutex)
{
    return true;
}

void oc_mutex_lock(oc_mutex mutex)
{
    return;
}

void oc_mutex_unlock(oc_mutex mutex)
{
    return;
}

oc_cond oc_cond_new(void)
{
    return (oc_cond)&g_condInfo;
}

void oc_cond_free(oc_cond cond)
{
    return;
}

void oc_cond_signal(oc_cond cond)
{
    return;
}

void oc_cond_broadcast(oc_cond cond)
{
    return;
}

void oc_cond_wait(oc_cond cond, oc_mutex mutex)
{
    return;
}

OCWaitResult_t oc_cond_wait_for(oc_cond cond, oc_mutex mutex, uint64_t microseconds)
{
    return OC_WAIT_SUCCESS;
}

