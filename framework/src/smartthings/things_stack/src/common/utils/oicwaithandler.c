/*
 * oicwaithandler.c
 *
 *  Created on: 2016. 10. 31.
 *      Author: eunseok
 */
#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#include "thingsmalloc.h"
#include "thingslogger.h"
#include "oicwaithandler.h"

#include "oicthread.h"
#ifdef OCF_RTOS
#include "thingsRtosUtil.h"
#endif

#define TAG "[waithandler]"

#define MAX_RANDOM_NUM      2999

#define MAX_SENDHANDLES     25

typedef void*(*PthreadFunc)(void* param);

typedef struct TimeOutManager_s
{
    TimeOut_s TimeOut;
    pthread_t gthreadId;
    OCDoHandle handleVal;
    CheckTimeOutCallFunc funcName;
} TimeOutManager_s;

static int iSendHandle = 0;
static OCDoHandle gSendHandle[MAX_SENDHANDLES] = {NULL,};
static pthread_mutex_t gREQmutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gTimeOutmutex = PTHREAD_MUTEX_INITIALIZER;


OCDoHandle GetRequestHandle(void)
{
    int i = 0;
    OCDoHandle result = NULL;

    pthread_mutex_lock(&gREQmutex);
    for(i = iSendHandle-1; i >= 0; i--)     // latest gSendHandle Value return.
    {
        if(gSendHandle[i] != NULL)
        {
            result = gSendHandle[i];
            break;
        }
    }
    pthread_mutex_unlock(&gREQmutex);

    return result;
}

int CASRequestHandle(OCDoHandle EqualVal, OCDoHandle writeHandleVal)
{
    int i = 0;
    int res = 0;

    pthread_mutex_lock(&gREQmutex);
    for(i = iSendHandle-1; i >= 0; i--)     // latest gSendHandle Value check.
    {
        if( gSendHandle[i] == EqualVal )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Found request handler.(%d th : 0x%X)", i, gSendHandle[i]);
            gSendHandle[i] = writeHandleVal;
            res = 1;
            break;
        }
    }
    pthread_mutex_unlock(&gREQmutex);

    return res;
}

bool isEmptyRequestHandle(void)
{
    bool isEmpty = false;

    pthread_mutex_lock(&gREQmutex);
    if( iSendHandle < MAX_SENDHANDLES )
    {
        isEmpty = true;
    }
    pthread_mutex_unlock(&gREQmutex);

    return isEmpty;
}

OCDoHandle AddRequestHandle(OCDoHandle HandleVal)
{
    OCDoHandle result = NULL;

    pthread_mutex_lock(&gREQmutex);
    if(iSendHandle >= MAX_SENDHANDLES)
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "gSendHandle array is Full.");
    }
    else
    {
        gSendHandle[iSendHandle] = HandleVal;
        result = gSendHandle[iSendHandle];
        iSendHandle++;
    }
    pthread_mutex_unlock(&gREQmutex);

    return result;
}

void DelAllRequestHandle(void)
{
    int i = 0;

    pthread_mutex_lock(&gREQmutex);
    for(i = 0; i < iSendHandle; i++)
    {
        gSendHandle[i] = NULL;
    }
    iSendHandle = 0;
    pthread_mutex_unlock(&gREQmutex);
}

/************************************************************
 *
 * Thread for Time-Out Process for Handle base
 *
 * Time-Out : DEFAULT_TIMEOUT sec.
 *
 ************************************************************/
static void* HandleBaseTimeOutLoop( TimeOutManager_s* pTimeOutManager )
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    static int cntTimeOutThread = 0;

    pthread_mutex_lock(&gTimeOutmutex);
    cntTimeOutThread++;
    THINGS_LOG_D(THINGS_DEBUG, TAG, "[HD Start] Count of Time-Out thread is %d.", cntTimeOutThread);
    pthread_mutex_unlock(&gTimeOutmutex);

    sleep(1);

    if(pTimeOutManager == NULL || pTimeOutManager->gthreadId < 0 || pTimeOutManager->funcName == NULL || pTimeOutManager->TimeOut.cur_counter <= 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Parameter is invalid.");
        if(pTimeOutManager)
        {
            DAFree(pTimeOutManager);
        }

        return 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "handleVal=0x%X, timeCounter=%d, funcName=0x%X", pTimeOutManager->handleVal, pTimeOutManager->TimeOut.cur_counter, pTimeOutManager->funcName);

    while( GetRequestHandle() != NULL && pTimeOutManager->handleVal == GetRequestHandle() )
    {   // GetRequestHandle() != NULL will guarantee for Connecting to AP.
        sleep(1);

        if( --pTimeOutManager->TimeOut.cur_counter == 0 )
        {
            if( CASRequestHandle(pTimeOutManager->handleVal, NULL) == 1 )
            {   // CASRequestHandle(handle, NULL) == 1 will guarantee that AP is connected and handle is latest important request.
                    pTimeOutManager->funcName(&pTimeOutManager->TimeOut);
            }
            pTimeOutManager->handleVal = (void*)(-1);
        }
    }

    DAFree(pTimeOutManager);
    pthread_mutex_lock(&gTimeOutmutex);
    cntTimeOutThread--;
    THINGS_LOG_D(THINGS_DEBUG, TAG, "[HD End] Count of Time-Out thread is %d.", cntTimeOutThread);
    pthread_mutex_unlock(&gTimeOutmutex);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");

    return 0;
}

long long nextTimeOut( const long long ori_num , const long long cur_num )
{
    long long x = 0;
    long long nextT = -1;

    if( ori_num <= 0 || cur_num <= 0 )
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Not Enabled PDF re-try.");
        return nextT;
    }

    while(cur_num >= nextT )
    {
        x = (long long)rand() * (long long)(MAX_RANDOM_NUM+1);
// pkes        x = x / (long long)((unsigned int)MAX_RAND+(unsigned int)1);
		x = x / (long long)((unsigned int)RAND_MAX+(unsigned int)1);
        x = x + 500;

        if( x < 500 || x > MAX_RANDOM_NUM+500)
        {
            THINGS_LOG_V(THINGS_INFO, TAG, "X value invalid.");
            break;
        }

        nextT = ((long long)ori_num * ((long long)10000000000000 + x*x*x*x)) / (long long)10000000000000;
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Gaussian next Time-Out Number: nextT = %lld", nextT);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "next Time-Out: %lld", nextT);
    return nextT;
}

unsigned long int CreateTimeOutProcess( OCDoHandle hadler, CheckTimeOutCallFunc CallFunc, TimeOut_s* timeOut )
{
    TimeOutManager_s* pTimeOutManager = NULL;

    if( (pTimeOutManager = (TimeOutManager_s*)DAMalloc(sizeof(TimeOutManager_s))) == NULL || CallFunc == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory allocation is failed.");
        return 0;
    }

    pTimeOutManager->gthreadId = NULL;
    pTimeOutManager->handleVal = hadler;
    pTimeOutManager->funcName = CallFunc;
    if( timeOut == NULL )
    {
        pTimeOutManager->TimeOut.cur_counter = DEFAULT_TIMEOUT;  // default second.
        pTimeOutManager->TimeOut.ori_num     = DEFAULT_TIMEOUT;  // default second.
        pTimeOutManager->TimeOut.cur_num     = DEFAULT_TIMEOUT;  // default second.
    }
    else
    {
        pTimeOutManager->TimeOut.cur_counter = timeOut->cur_counter;
        pTimeOutManager->TimeOut.ori_num     = timeOut->ori_num;
        pTimeOutManager->TimeOut.cur_num     = timeOut->cur_num;
    }

#ifdef OCF_RTOS
    if( pthread_create_rtos(&pTimeOutManager->gthreadId, NULL, (PthreadFunc)HandleBaseTimeOutLoop, (void *)pTimeOutManager, THINGS_STACK_WAITHANDLER_THREAD) != 0 )
#else
    if( OICThread_create(&pTimeOutManager->gthreadId, NULL, (PthreadFunc)HandleBaseTimeOutLoop, (void *)pTimeOutManager) != 0 )
#endif
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create thread is failed.");
        DAFree(pTimeOutManager);
        return 0;
    }

    return pTimeOutManager->gthreadId;
}

