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

static const uint64_t USECS_PER_MSEC = 1000;

typedef struct _tagMutexInfo_t
{
    CRITICAL_SECTION mutex;
} oc_mutex_internal;

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
        CloseHandle(threadInfo->handle);
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
    if (WAIT_OBJECT_0 != joinres)
    {
        OIC_LOG(ERROR, TAG, "Failed to join thread");
        res = OC_THREAD_WAIT_FAILURE;
    }
    else
    {
        CloseHandle(threadInfo->handle);
    }
    return res;
}

oc_mutex oc_mutex_new(void)
{
    oc_mutex retVal = NULL;
    oc_mutex_internal *mutexInfo = (oc_mutex_internal*) OICMalloc(sizeof(oc_mutex_internal));
    if (NULL != mutexInfo)
    {
        InitializeCriticalSection(&mutexInfo->mutex);
        retVal = (oc_mutex)mutexInfo;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to allocate mutex!", __func__);
    }

    return retVal;
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
        LeaveCriticalSection(&mutexInfo->mutex);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid mutex !", __func__);
    }
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

    return retVal;
}

