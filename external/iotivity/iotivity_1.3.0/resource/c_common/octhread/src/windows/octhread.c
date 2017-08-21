/* *****************************************************************
*
* Copyright 2016 Intel Corporation
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************/


/**
 * @file
 * This file provides APIs related to mutex, semaphores, and threads.
 */
#include "iotivity_config.h"
#include "octhread.h"
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <stdio.h>
#include <errno.h>
#include <oic_malloc.h>

#include "logger.h"
#include "iotivity_debug.h"

#define TAG "OIC_UMUTEX"

static const uint64_t USECS_PER_MSEC = 1000;

typedef struct _tagMutexInfo_t
{
    CRITICAL_SECTION mutex;

    /**
     * Catch some of the incorrect mutex usage, by tracking the mutex owner,
     * on Debug builds.
     */
#ifndef NDEBUG
    DWORD owner;
    uint32_t recursionCount;
#endif
} oc_mutex_internal;

#ifndef NDEBUG
static DWORD oc_get_current_thread_id()
{
    DWORD id = GetCurrentThreadId();
    assert(OC_INVALID_THREAD_ID != id);
    return id;
}
#endif

typedef struct _tagEventInfo_t
{
    CONDITION_VARIABLE cond;
} oc_cond_internal;

typedef struct _tagThreadInfo_t
{
    HANDLE handle;
} oc_thread_internal;

OCThreadResult_t oc_thread_new(oc_thread *t, void *(*start_routine)(void *), void *arg)
{
    OCThreadResult_t res = OC_THREAD_SUCCESS;
    oc_thread_internal *threadInfo = (oc_thread_internal*)OICMalloc(sizeof(oc_thread_internal));
    if (NULL != threadInfo)
    {
        threadInfo->handle = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
        if (threadInfo->handle == NULL)
        {
            res = OC_THREAD_CREATE_FAILURE;
            *t = NULL;
            OICFree(threadInfo);
            OIC_LOG_V(ERROR, TAG, "%s: CreateThread failed: %i", __func__, GetLastError());
        }
        else
        {
            *t = (oc_thread)threadInfo;
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to allocate thread!", __func__);
        *t = NULL;
        res = OC_THREAD_ALLOCATION_FAILURE;
    }

   return res;
}

OCThreadResult_t oc_thread_free(oc_thread t)
{
    OCThreadResult_t res = OC_THREAD_INVALID_PARAMETER;
    oc_thread_internal *threadInfo = (oc_thread_internal*) t;
    if (threadInfo)
    {
        OC_VERIFY(CloseHandle(threadInfo->handle));
        OICFree(threadInfo);
        res = OC_THREAD_SUCCESS;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid thread !", __func__);
    }
    return res;
}

OCThreadResult_t oc_thread_wait(oc_thread t)
{
    OCThreadResult_t res = OC_THREAD_SUCCESS;
    oc_thread_internal *threadInfo = (oc_thread_internal*) t;
    DWORD joinres = WaitForSingleObject(threadInfo->handle, INFINITE);
    assert(WAIT_OBJECT_0 == joinres);
    if (WAIT_OBJECT_0 != joinres)
    {
        OIC_LOG(ERROR, TAG, "Failed to join thread");
        res = OC_THREAD_WAIT_FAILURE;
    }
    return res;
}

oc_mutex oc_mutex_new(void)
{
    oc_mutex retVal = NULL;
    oc_mutex_internal *mutexInfo = (oc_mutex_internal*) OICMalloc(sizeof(oc_mutex_internal));
    if (NULL != mutexInfo)
    {
#ifndef NDEBUG
        mutexInfo->owner = OC_INVALID_THREAD_ID;
        mutexInfo->recursionCount = 0;
#endif
        InitializeCriticalSection(&mutexInfo->mutex);
        retVal = (oc_mutex)mutexInfo;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to allocate mutex!", __func__);
    }

    return retVal;
}

oc_mutex oc_mutex_new_recursive(void)
{
    return oc_mutex_new();
}

bool oc_mutex_free(oc_mutex mutex)
{
    bool bRet = false;
    oc_mutex_internal *mutexInfo = (oc_mutex_internal*) mutex;
    if (mutexInfo)
    {
        DeleteCriticalSection(&mutexInfo->mutex);
        OICFree(mutexInfo);
        bRet=true;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid mutex !", __func__);
    }

    return bRet;
}

void oc_mutex_lock(oc_mutex mutex)
{
    oc_mutex_internal *mutexInfo = (oc_mutex_internal*) mutex;
    if (mutexInfo)
    {
        EnterCriticalSection(&mutexInfo->mutex);

#ifndef NDEBUG
        /**
         * Updating the recursionCount and owner fields must be performed
         * while owning the lock, to solve race conditions with other
         * threads using the same lock.
         */
        if (mutexInfo->recursionCount != 0)
        {
            oc_mutex_assert_owner(mutex, true);
        }
        else
        {
            mutexInfo->owner = oc_get_current_thread_id();
        }

        mutexInfo->recursionCount++;
#endif
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid mutex !", __func__);
    }
}

void oc_mutex_unlock(oc_mutex mutex)
{
    oc_mutex_internal *mutexInfo = (oc_mutex_internal*) mutex;

    if (mutexInfo)
    {
#ifndef NDEBUG
        oc_mutex_assert_owner(mutex, true);

        /**
         * Updating the recursionCount and owner fields must be performed
         * while owning the lock, to solve race conditions with other
         * threads using the same lock.
         */
        mutexInfo->recursionCount--;

        if (mutexInfo->recursionCount == 0)
        {
            mutexInfo->owner = OC_INVALID_THREAD_ID;
        }
#endif

        LeaveCriticalSection(&mutexInfo->mutex);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid mutex !", __func__);
    }
}

void oc_mutex_assert_owner(const oc_mutex mutex, bool currentThreadIsOwner)
{
#ifdef NDEBUG
    (void)(mutex);
    (void)(currentThreadIsOwner);
#else
    assert(NULL != mutex);
    const oc_mutex_internal *mutexInfo = (const oc_mutex_internal*) mutex;

    DWORD currentThreadID = oc_get_current_thread_id();
    if (currentThreadIsOwner)
    {
        assert(mutexInfo->owner == currentThreadID);
        assert(mutexInfo->recursionCount != 0);
    }
    else
    {
        assert(mutexInfo->owner != currentThreadID);
    }
#endif
}

oc_cond oc_cond_new(void)
{
    oc_cond retVal = NULL;
    oc_cond_internal *eventInfo = (oc_cond_internal*) OICMalloc(sizeof(oc_cond_internal));
    if (NULL != eventInfo)
    {
        InitializeConditionVariable(&eventInfo->cond);
        retVal = (oc_cond) eventInfo;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to allocate condition variable!", __func__);
    }

    return retVal;
}

void oc_cond_free(oc_cond cond)
{
    oc_cond_internal *eventInfo = (oc_cond_internal*) cond;
    if (eventInfo != NULL)
    {
        OICFree(cond);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid parameter", __func__);
    }
}

void oc_cond_signal(oc_cond cond)
{
    oc_cond_internal *eventInfo = (oc_cond_internal*) cond;
    if (eventInfo != NULL)
    {
        WakeConditionVariable(&eventInfo->cond);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid parameter", __func__);
    }
}

void oc_cond_broadcast(oc_cond cond)
{
    oc_cond_internal* eventInfo = (oc_cond_internal*) cond;
    if (eventInfo != NULL)
    {
        WakeAllConditionVariable(&eventInfo->cond);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid parameter", __func__);
    }
}

void oc_cond_wait(oc_cond cond, oc_mutex mutex)
{
    oc_cond_wait_for(cond, mutex, 0L);
}

OCWaitResult_t oc_cond_wait_for(oc_cond cond, oc_mutex mutex, uint64_t microseconds)
{
    OCWaitResult_t retVal = OC_WAIT_INVAL;

    oc_cond_internal *eventInfo = (oc_cond_internal*) cond;
    oc_mutex_internal *mutexInfo = (oc_mutex_internal*) mutex;

    if (NULL == mutexInfo)
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid mutex", __func__);
        return OC_WAIT_INVAL;
    }

    if (NULL == eventInfo)
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid condition", __func__);
        return OC_WAIT_INVAL;
    }

    DWORD milli = 0;
    if (microseconds > 0)
    {
        milli = (DWORD)(microseconds / USECS_PER_MSEC);
    }
    else
    {
        milli = INFINITE;
    }

#ifndef NDEBUG
    // Recursively-acquired locks are not supported for use with condition variables.
    oc_mutex_assert_owner(mutex, true);
    assert(mutexInfo->recursionCount == 1);
    mutexInfo->recursionCount = 0;
    mutexInfo->owner = OC_INVALID_THREAD_ID;
#endif

    // Wait for the given time        
    if (!SleepConditionVariableCS(&eventInfo->cond, &mutexInfo->mutex, milli))
    {
        if (GetLastError() == ERROR_TIMEOUT)
        {
            retVal = OC_WAIT_TIMEDOUT;
        }
        else
        {
            OIC_LOG_V(ERROR, TAG, "SleepConditionVariableCS() failed %i", GetLastError());
            retVal = OC_WAIT_INVAL;
        }
    }
    else
    {
        retVal = OC_WAIT_SUCCESS;
    }

#ifndef NDEBUG
    oc_mutex_assert_owner(mutex, false);
    assert(mutexInfo->recursionCount == 0);
    mutexInfo->recursionCount = 1;
    mutexInfo->owner = oc_get_current_thread_id();
#endif

    return retVal;
}
