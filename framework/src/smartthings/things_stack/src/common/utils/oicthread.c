
#include <stdio.h>
#include <stdlib.h>

#include "thingslogger.h"
#include "thingsmalloc.h"

#include "oicthread.h"

#define TAG "[oicthread]"

int OICThread_create(pthread_t * __newthread,
                     pthread_attr_t * __attr,
                     void *(*__start_routine) (void *),
                     void * __arg)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
    int ret = 0;
#ifndef OCF_RTOS
    int ret = -1;
    pthread_attr_t attr;
    pthread_attr_t* pAttr;

    if( __newthread == NULL || __start_routine == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Input invalied.(mandatory var: __newthread=0x%X, __start_routine=0x%X", __newthread, __start_routine);
        return ret;
    }

    pAttr = __attr;

    if( __attr == NULL )
    {
        if( pthread_attr_init(&attr) != 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "pthread_attr_init() is failed.");
            return ret;
        }

        if( pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "pthread_attr_setdetachstate() is failed.");
            return ret;
        }

        pAttr = &attr;
    }

    ret = pthread_create(__newthread, pAttr, __start_routine, __arg );

    if( __attr == NULL )
    {
        if( pthread_attr_destroy(&attr) != 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "pthread_attr_destroy() is failed.");
        }
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
#endif
    return ret;
}

