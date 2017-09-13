#ifndef _OICREQUESTHANDLER_H_
#define _OICREQUESTHANDLER_H_

#include "ocstack.h"
#include "oiccommon.h"
#include "oicresource.h"
#include "thingsapi.h"
#include "octypes.h"

//jun
//#include <ocfapi/ocfresource.h>

// #ifdef __cplusplus
// extern "C" {
// #endif // __cplusplus

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

/*
 * @param reqType : Request Msg Type : Get(1), Put(2), Post(4), Delete(8), Observe(16), Invalid(0)
 * @desc Definition will be relocated to oiccommon.h when if some other modules uses this function type
 */
typedef int (*ReqMsgHandlingFuncType)  (int reqType, OICInfo** pList, int cnt, int isOutDoor, char* pCmdId);
typedef int (*GetDeviceStateFuncType)   (struct OICInfo* pTarget, struct OICInfo** pList, int* pCnt);

/*
 *@brief OIC_Info & OicResource data structure converting func
 */
typedef int (*ConvertResourceCB)        (OicResource* targetResource, OICInfo** pList, int listCnt);
typedef int (*ConvertRequestInfoCB)     (OicResource* targetResource, OICInfo** pList, int *listCnt);

typedef char*   (*GetNotifyObsURICB)    (const char* Uri, const char* query);

typedef OCEntityHandlerResult (*HandleRequestInterfaceCB)(OicResource **ppstTargetResource);


typedef int (*StopSoftAPFuncType)         (int value);

typedef struct OICRequestHandler
{
    RequestHandlerCB            EntityHandler;
    // RequestValidatorCB          validator;

    void (*InitModule)          (void);

    void (*DeinitModule)        (void);
    int (*NotifyOicObservers)   (const char* uri);

} OICRequestHandler;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

OICRequestHandler* GetHandlerInstance();
void ReleaseHandlerInstance(OICRequestHandler*);


int RegisterDawitRequestInterfaceCB(HandleRequestInterfaceCB func);

int RegisterStopSoftAPCB(StopSoftAPFuncType func);

//jun
typedef int (*HandleRequestFuncType)   (struct OicResource* res);

int RegisterHandleReqCB(HandleRequestFuncType func, char** list, int listCnt);

void NotifyResultOfReset(OicResource* targetResource, bool result);

//jun
int RegisterHandleRequestFunc(HandleRequestFuncType getFunc, HandleRequestFuncType setFunc);

//jun
//int RegisterHandleRequestCB(HandleGetRequestCallback onGetFunc, HandleSetRequestCallback onSetFunc);

// #ifdef __cplusplus
// }
// #endif // __cplusplus

#endif // _OICREQUESTHANDLER_H_
