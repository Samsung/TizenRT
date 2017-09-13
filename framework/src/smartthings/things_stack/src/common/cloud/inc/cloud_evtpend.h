#ifndef CLOUD_EVENTDATA_PENDING_PROCESS_BY_EUNSEOK_KIM
#define CLOUD_EVENTDATA_PENDING_PROCESS_BY_EUNSEOK_KIM

#include "escommon.h"

typedef struct ESCloudPendingEvent {
    bool exist;
    bool isThereCIProv;
    int  usingCount;
    ESCloudProvData event;
} ESCloudPendingEvent;


void CICPInitEventPend(void);

ESCloudProvData * CICPGetPendedData(void);

bool CICPGetIsThereCP(void);

bool CICPCASIsThereCPIFfalse(void);

void CICPDelPendedData(void);

void CICPDelIsThereCP(void);

int CICPPendEventData(ESCloudProvData * eventData);

int CICPEnterPenddataBlock(void);

int CICPReleasePenddataBlock(void);

#endif // CLOUD_EVENTDATA_PENDING_PROCESS_BY_EUNSEOK_KIM
