#include <stdio.h>
#include <pthread.h>
#include "ssMutex.h"
#include "thingsmalloc.h"
#include "thingslogger.h"

#define TAG ("SS_MUTEX")

ss_mutex_t* ssMutexNew(void)
{
    ss_mutex_t *mutexInfo = (ss_mutex_t*)DAMalloc(sizeof(ss_mutex_t));
    if (NULL != mutexInfo)
    {
        // create the mutex with the attributes set
        int ret=pthread_mutex_init(&(mutexInfo->mutex), PTHREAD_MUTEX_DEFAULT);
        if (0 != ret)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Failed to initialize mutex : %d", __func__, ret);
            DAFree(mutexInfo);
            mutexInfo = NULL;
        }
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Failed to allocate mutex!", __func__);
    }
    return mutexInfo;
}

bool ssMutexFree(ss_mutex_t* mutexInfo)
{
    bool bRet=false;
    if (mutexInfo)
    {
        int ret = pthread_mutex_destroy(&mutexInfo->mutex);
        if (0 == ret)
        {
            DAFree(mutexInfo);
            bRet=true;
        }
        else
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Failed to free mutex : %d", __func__, ret);
        }
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Invalid mutex !", __func__);
    }
    return bRet;
}

void ssMutexLock(ss_mutex_t* mutexInfo)
{
    if (mutexInfo)
    {
        int ret = pthread_mutex_lock(&mutexInfo->mutex);
        if(ret != 0)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Pthread Mutex lock failed: %d", ret);
        }
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s Invalid mutex !", __func__);
    }
}

void ssMutexUnlock(ss_mutex_t* mutexInfo)
{
    if (mutexInfo)
    {
        int ret = pthread_mutex_unlock(&mutexInfo->mutex);
        if(ret != 0)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Pthread Mutex unlock failed: %d", ret);
        }
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "%s: Invalid mutex !", __func__);
    }
}

