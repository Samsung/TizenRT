#include <unistd.h>
#include <pthread.h>

#include "thingslogger.h"
#include "cloud_evtpend.h"
#include "resourcehandler.h"

#define TAG "[cloudevtpend]"


static ESCloudPendingEvent PendingEvent = {false, false, false, };
static pthread_mutex_t gPendingmutex = PTHREAD_MUTEX_INITIALIZER;

void CICPInitEventPend(void)
{
    pthread_mutex_lock(&gPendingmutex);
    PendingEvent.exist = false;
    PendingEvent.isThereCIProv = false;
    PendingEvent.usingCount = 0;
    initESCloudProvData(&PendingEvent.event);
    pthread_mutex_unlock(&gPendingmutex);
}

ESCloudProvData * CICPGetPendedData(void)    // [GET] If there pending-Data, then return point of Data.
{
    ESCloudProvData * pEvent = NULL;

    pthread_mutex_lock(&gPendingmutex);
    if(PendingEvent.exist == true)
        pEvent = &PendingEvent.event;
    pthread_mutex_unlock(&gPendingmutex);

    return pEvent;
}

bool CICPGetIsThereCP(void)                    // [GET] return isThereCIProv value.
{
    bool isThere = false;

    pthread_mutex_lock(&gPendingmutex);
    isThere = PendingEvent.isThereCIProv;
    pthread_mutex_unlock(&gPendingmutex);

    return isThere;
}

bool CICPCASIsThereCPIFfalse(void)             // [CAS] If isThereCIProv value is false, then set true and return isThereCIProv value.
{
    bool isThere = false;

    pthread_mutex_lock(&gPendingmutex);
    isThere = PendingEvent.isThereCIProv;
    if( isThere == false )
        PendingEvent.isThereCIProv = true;
    pthread_mutex_unlock(&gPendingmutex);

    return isThere;
}

void CICPDelPendedData(void)                     // [SET] Setting false to Flag about existing of pending-Data.
{
    bool bContinue = true;

    while(bContinue)
    {
        pthread_mutex_lock(&gPendingmutex);
        if( PendingEvent.exist == false || PendingEvent.usingCount == 0)
        {
            PendingEvent.exist = false;
            bContinue = false;
        }
        pthread_mutex_unlock(&gPendingmutex);

        if( bContinue == true )
            usleep(100000);     // 0.1 seconds delay.
    }
}

void CICPDelIsThereCP(void)                    // [SET] Setting false to Flag about existing of new CP-data.
{
    pthread_mutex_lock(&gPendingmutex);
    PendingEvent.isThereCIProv = false;
    pthread_mutex_unlock(&gPendingmutex);
}

int CICPPendEventData(ESCloudProvData * eventData)   // [SET] Setting true all Flag and Store Event-Data to pending repo.
{
    if(eventData == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Event Data is NULL.");
        return 0;
    }

    bool bContinue = true;

    while(bContinue)
    {
        pthread_mutex_lock(&gPendingmutex);
        if( PendingEvent.usingCount == 0)
        {
            PendingEvent.exist = true;
            PendingEvent.isThereCIProv = true;

            if( eventData != &PendingEvent.event )
            {
                cloneESCloudProvData(&PendingEvent.event, eventData);
            }
            bContinue = false;
        }
        pthread_mutex_unlock(&gPendingmutex);

        if(bContinue == true )
            usleep(100000);     // 0.1 seconds delay.
    }

    return 1;
}

int CICPEnterPenddataBlock(void)
{
    int usingCnt = 0;

    pthread_mutex_lock(&gPendingmutex);
    PendingEvent.usingCount++;
    usingCnt = PendingEvent.usingCount;
    pthread_mutex_unlock(&gPendingmutex);

    return usingCnt;
}


int CICPReleasePenddataBlock(void)
{
    int usingCnt = 0;

    pthread_mutex_lock(&gPendingmutex);
    PendingEvent.usingCount--;
    usingCnt = PendingEvent.usingCount;
    pthread_mutex_unlock(&gPendingmutex);

    return usingCnt;
}
