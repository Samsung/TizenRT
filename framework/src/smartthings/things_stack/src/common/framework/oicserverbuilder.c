//#******************************************************************
//#
//# Copyright 2015 SAMSUNG ELECTRONICS All Rights Reserved.
//#
//#******************************************************************
#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "oicdef.h"
#include "oiccommon.h"
#include "thingslogger.h"
#include "thingsmalloc.h"
#include "oicstringutil.h"
#include "ocpayload.h"
#include "oicresource.h"
#include "oicserverbuilder.h"
#include "resourcehandler.h"

#include "oicthread.h"
#ifdef OCF_RTOS
#include "thingsRtosUtil.h"
#endif
#define TAG "[oicsvrbldr]"

#define DEFAULT_DATA_MODEL_VERSIONS "res.1.1.0,sh.1.1.0"

#define KEY_ATTR_DEVICE_NAME                "n"
#define KEY_ATTR_PLATFORM_VENDERID          "vid"


static pthread_t gthreadId_Server;
static pthread_t gthreadId_Presence;

static int gQuitFlag = 0;
static uint gPresenceFlag = 0;
static int gPresenceDuration = 20;
static volatile bool isPresence = false;

OICServerBuilder* gBuilder = NULL;

void* ServerExecuteLoop(void *param)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    while (!gQuitFlag)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "OCProcess Error");
            //[JAY] need to insert error handling logic from here
        }
        //[JAY] The proper time period for looping need to be decided..
        usleep(10*1000);
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return NULL;
}

void* PresenceNotiLoop(void *param)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if (OCStartPresence(0) == OC_STACK_OK)
    {
/*        while (!gQuitFlag)
        {
            gPresenceFlag++;
            //THINGS_LOG_D(THINGS_DEBUG, TAG, "COUNT : %d", gPresenceFlag);
            if (gPresenceFlag > gPresenceDuration)
            {
                gPresenceFlag = 0;
                OCStopPresence();
                THINGS_LOG(THINGS_INFO, TAG, "Stop Sending Presence Packet");
                isPresence = false;
                break;
            }
            sleep(1);
        }
*/
    }
    else
    {
         THINGS_LOG_ERROR(THINGS_ERROR, TAG, "OCStartPresence Error");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return NULL;
}

void RegisterReqHandler(struct OICServerBuilder* builder, RequestHandlerCB handler)
{
    if(handler == NULL || builder == NULL)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Param");
        return;
    }
    builder->handler = handler;
}

struct OicResource* CreateResource(struct OICServerBuilder* builder,
                                   char* uri,
                                   char* type,
                                   char* interface,
                                   int isDiscoverable,
                                   int isObserable,
                                   int isSecure)
{
    OicResource* res = NULL;
    OCResourceHandle hd = NULL;
    uint8_t RSC_Properties = OC_DISCOVERABLE;

    if(builder->handler == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG,"Handler for serverbuilder is not registered");
        return res;
    }

    if(1 != isDiscoverable)
    {
        RSC_Properties = OC_ACTIVE;
    }

    if(1 == isObserable)
    {
        RSC_Properties |= OC_OBSERVABLE;
    }

    // if( strstr(uri, URI_ACCESSPOINTLIST) == 0
    //     && strstr(uri, URI_PROVINFO) == 0 )
    if(1 == isSecure)
    {
#ifdef __SECURED__
        RSC_Properties |= OC_SECURE;
#else
        THINGS_LOG(THINGS_DEBUG, TAG,"Stack is in UNSECURED Mode");
#endif
    }

    OCStackResult ret = OCCreateResource( &hd,
                                          type,
                                          interface,
                                          uri,
                                          builder->handler,
                                          NULL,
                                          RSC_Properties);

    if(ret != OC_STACK_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Resource Creation Failed - ret = %d, %s", ret, uri);
        return NULL;
    }

    res = CreateResourceInst(NULL, hd, NULL, NULL);

    if(NULL == res)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG,"CreateResourceInst is failed");
        return res;
    }

    res->resType = type;

    builder->gResArr[builder->res_num++] = res;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Created hd [%x], prop [0x%X] uri : %s", res->resourceHandle, RSC_Properties, res->uri);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "DISCOVERABLE : %s",(isDiscoverable == 1 ? "YES":"NO"));
    THINGS_LOG_D(THINGS_DEBUG, TAG, "OBSERABLE : %s",(isObserable == 1 ? "YES":"NO"));
    THINGS_LOG_D(THINGS_DEBUG, TAG, "SECURE : %s",(isSecure == 1 ? "YES":"NO"));
    return res;
}

// struct OicResource* CreateResource(struct OICServerBuilder* builder,
//                                    char* uri,
//                                    char* type,
//                                    char* interface,
//                                    int isObserable,
//                                    int isSecure)
// {
//     OicResource* res = NULL;
//     OCResourceHandle hd = NULL;
//     uint8_t RSC_Properties = OC_DISCOVERABLE;

//     if(builder->handler == NULL)
//     {
//         THINGS_LOG_ERROR(THINGS_ERROR, TAG,"Handler for serverbuilder is not registered");
//         return res;
//     }

//     if(1 == isObserable)
//     {
//         RSC_Properties |= OC_OBSERVABLE;
//     }

//     // if( strstr(uri, URI_ACCESSPOINTLIST) == 0
//     //     && strstr(uri, URI_PROVINFO) == 0 )
//     if(1 == isSecure)
//     {
// #ifdef __SECURED__
//         RSC_Properties |= OC_SECURE;
// #else
//         THINGS_LOG(THINGS_DEBUG, TAG,"Stack is in UNSECURED Mode");
// #endif
//     }

//     OCStackResult ret = OCCreateResource( &hd,
//                                           type,
//                                           interface,
//                                           uri,
//                                           builder->handler,
//                                           NULL,
//                                           RSC_Properties);

//     if(ret != OC_STACK_OK)
//     {
//         THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Resource Creation Failed - ret = %d, %s", ret, uri);
//         return NULL;
//     }

//     res = CreateResourceInst(NULL, hd, NULL, NULL);

//     if(NULL == res)
//     {
//         THINGS_LOG_ERROR(THINGS_ERROR, TAG,"CreateResourceInst is failed");
//         return res;
//     }

//     res->resType = type;

//     builder->gResArr[builder->res_num++] = res;

//     THINGS_LOG_D(THINGS_DEBUG, TAG, "Created hd [%x], prop [0x%X] uri : %s", res->resourceHandle, RSC_Properties, res->uri);
//     THINGS_LOG_D(THINGS_DEBUG, TAG, "DISCOVERABLE : YES, OBSERABLE : %s, SECURE : %s",(isObserable == 1 ? "YES":"NO"), (isSecure == 1 ? "YES":"NO"));
//     return res;
// }

// struct OicResource * CreateActiveResource (struct OICServerBuilder* builder,
//                                          char* uri,
//                                          char* type,
//                                          char* interface,
//                                          int isObserable,
//                                          int isSecure)
// {
//     OicResource* res = NULL;
//     OCResourceHandle hd = NULL;
//     uint8_t RSC_Properties = OC_ACTIVE ;

//     if(builder->handler == NULL)
//     {
//         THINGS_LOG_ERROR(THINGS_ERROR, TAG,"Handler for serverbuilder is not registered");
//         return res;
//     }

//     if(1 == isObserable)
//     {
//         RSC_Properties |= OC_OBSERVABLE;
//     }
//     if(1 == isSecure)
//     {
// #ifdef __SECURED__
//         RSC_Properties |= OC_SECURE;
// #else
//         THINGS_LOG(THINGS_DEBUG, TAG,"Stack is in UNSECURED Mode");
// #endif
//     }

//     OCStackResult ret = OCCreateResource( &hd,
//                                           type,
//                                           interface,
//                                           uri,
//                                           builder->handler,
//                                           NULL,
//                                           RSC_Properties);

//     if(ret != OC_STACK_OK)
//     {
//         THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Active Resource Creation Failed - ret = %d, %s", ret, uri);
//         return NULL;
//     }

//     res = CreateResourceInst(NULL, hd, NULL, NULL);

//     if(NULL == res)
//     {
//         THINGS_LOG_ERROR(THINGS_ERROR, TAG,"CreateActiveResource is failed");
//         return res;
//     }

//     res->resType = type;

//     builder->gResArr[builder->res_num++] = res;

//     THINGS_LOG_D(THINGS_DEBUG, TAG, "Created hd [%x], prop [0x%X] uri : %s", res->resourceHandle, RSC_Properties, res->uri);

//     return res;
// }


struct OicResource* CreateCollectionResource(struct OICServerBuilder* builder,
    char* uri,
    char* type,
    char* interface)
{
    OicResource* res = NULL;
    OCResourceHandle hd = NULL;
    uint8_t RSC_Properties = OC_DISCOVERABLE | OC_OBSERVABLE;


    if(builder->handler == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG,"Handler for serverbuilder is not registered");
        return res;
    }

#ifdef __SECURED__

    RSC_Properties |= OC_SECURE;

#endif //#ifdef __SECURED__

    OCStackResult ret = OCCreateResource( &hd,
                                            uri,
                                          type,
                                          uri,
                                          builder->handler,
                                          NULL,
                                          RSC_Properties);

    if(ret != OC_STACK_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Resource Creation Failed - ret = %d, %s", ret, uri);
        return NULL;
    }

    // [Jay] Baseline interface will be binded automatically by IoTivity Stack.

   

    res = CreateResourceInst(NULL, hd, NULL, NULL);

    if(NULL == res)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG,"CreateResourceInst is failed");
        return res;
    }

    res->resType = type;

    builder->gResArr[builder->res_num++] = res;

    THINGS_LOG_D(THINGS_INFO, TAG, "Created hd [%x], prop [0x%X] uri : %s", res->resourceHandle, RSC_Properties, uri);

    return res;
}

void DeleteResource(struct OICServerBuilder* builder)
{
    for(size_t iter = 0 ; iter < builder->res_num ; iter++ )
    {
        OCStackResult ret = OCDeleteResource((OCResourceHandle)(builder->gResArr[iter]->resourceHandle));
        if(ret != OC_STACK_OK)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to delete the resource");
        }
    }
}

int AddInterfaceType(OicResource* resource, char* interface)
{
    if(NULL != resource
        && NULL != interface)
    {
        if(NULL != resource->resourceHandle)
        {
            if(OC_STACK_OK ==
                OCBindResourceInterfaceToResource(resource->resourceHandle,
                    interface) )
            {
                return OC_STACK_OK;
            }
        }
    }

    return OC_STACK_ERROR;
}

int AddResourceType(OicResource* resource, char* type)
{
    if(NULL != resource
        && NULL != type)
    {
        if(NULL != resource->resourceHandle)
        {
            if(OC_STACK_OK ==
                OCBindResourceTypeToResource(resource->resourceHandle,
                    type) )
            {
                return OC_STACK_OK;
            }
        }
    }

    return OC_STACK_ERROR;
}

void Bind(struct OicResource* res, struct OicResource* bind)
{
    if(res == NULL || bind == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid Resource");
        return;
    }

    if(res->resourceHandle == bind->resourceHandle)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "It's identical resource");
        return;
    }

    OCStackResult ret = OCBindResource((OCResourceHandle)(res->resourceHandle),
                                       (OCResourceHandle)(bind->resourceHandle));
    if(ret != OC_STACK_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Bind Failed ");
    }
}

void BindAll(struct OicResource* res, struct OicResource* bind[], size_t size)
{
    for(size_t iter = 0 ; iter < size ; iter++)
    {
        Bind(res, bind[iter]);
        // [JAY] This time value achieved via testing
        //       May need to re-test to find the opt. value for give platform.
        usleep(100 * 1000);
    }
}

struct OicResource* GetResource(OICServerBuilder* builder, const char* uri)
{
    OicResource* ret = NULL;
    for(size_t iter = 0 ; iter < builder->res_num ; iter++ )
    {
        const char* rURI = OCGetResourceUri((OCResourceHandle)(builder->gResArr[iter]->resourceHandle));

        if(CompareStrings(rURI, uri) == 0)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG,"URI Compare : %s , %s", uri, rURI);
            ret = builder->gResArr[iter];
            break;
        }
    }
    return ret;
}

int BroadcastPresence(OICServerBuilder* builder, int maxCnt)
{
    if( maxCnt <= 0 )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "check your maxCnt value.(must maxCnt > 0)");
        return 1;
    }

    if(!isPresence)
    {
        isPresence = true;
        gPresenceFlag = 0;
        gPresenceDuration = maxCnt;
#ifdef OCF_RTOS
        pthread_create_rtos(&gthreadId_Presence, NULL, PresenceNotiLoop, (void *)NULL, THINGS_STACK_PRESENCE_NOTI_THREAD);
#else
        OICThread_create (&gthreadId_Presence, NULL, PresenceNotiLoop, (void *)NULL);
#endif
    }
    else if( (uint)gPresenceDuration < gPresenceFlag+((uint)maxCnt) )
    {
        gPresenceDuration = maxCnt;
        gPresenceFlag = 0;
    }

    return 0;
}

void InitBuilder(struct OICServerBuilder* builder, RequestHandlerCB cb)
{
    //if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
    //if(OC_STACK_OK != OCInit1 (OC_CLIENT_SERVER,OC_IP_USE_V4, OC_IP_USE_V4) )
    OCTransportAdapter mTransport = ( OC_ADAPTER_IP |  OC_ADAPTER_TCP ); 
    if(OC_STACK_OK != OCInit2 (OC_CLIENT_SERVER, OC_IP_USE_V4, OC_IP_USE_V4, mTransport) )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "RESOURCE SERVER START FAILED");
        return ;
    }

    THINGS_LOG(THINGS_INFO, TAG, "Resource Server IS NOW STARTING...");

    gQuitFlag = 0;


#ifdef OCF_RTOS
    pthread_create_rtos(&gthreadId_Server, NULL, ServerExecuteLoop, (void *)NULL, THINGS_STACK_SERVEREXCETUE_LOOP_THREAD);
#else
    OICThread_create (&gthreadId_Server, NULL, ServerExecuteLoop, (void *)NULL);
#endif

    RegisterReqHandler(builder, cb);
}

void DeinitBuilder(OICServerBuilder* builder)
{
    if(builder != NULL)
    {
        gQuitFlag = 1;
        pthread_cancel(gthreadId_Server);
        pthread_join(gthreadId_Server, NULL);
        pthread_detach(gthreadId_Server);
        gthreadId_Server = NULL;

        pthread_cancel(gthreadId_Presence);
        pthread_join(gthreadId_Presence, NULL);
        pthread_detach(gthreadId_Presence);
        gthreadId_Presence = NULL;

        // 1. [JAY] Need to unregister those registered resource in the Stack
        // 2. [JAY] Free the payload of each resources
        for(size_t iter = 0 ; iter < builder->res_num ; iter++ )
        {

            if(builder->gResArr[iter]->rep != NULL)
            {
                ReleaseRepresentationInst(builder->gResArr[iter]->rep);
            }
        }
    }

    if (OCStop() != OC_STACK_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "OCStack process error");
    }
}

void SetDeviceInfo(OICServerBuilder* builder, char* deviceName, char* deviceType)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/d] name :%s",deviceName);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/d] type :%s",deviceType);

    OCDeviceInfo deviceInfo;
    deviceInfo.deviceName = NULL;
    deviceInfo.types = NULL;
    deviceInfo.specVersion = NULL;
    deviceInfo.dataModelVersions = NULL;

    DuplicateString(deviceName,  &deviceInfo.deviceName);
    DuplicateString(OC_SPEC_VERSION ,&deviceInfo.specVersion);
    deviceInfo.dataModelVersions = OCCreateOCStringLL(DEFAULT_DATA_MODEL_VERSIONS);
    deviceInfo.types = OCCreateOCStringLL(deviceType);
    OCResourcePayloadAddStringLL(&deviceInfo.types, OC_RSRVD_RESOURCE_TYPE_DEVICE);
    OCSetDeviceInfo(deviceInfo);

    // OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, KEY_ATTR_DEVICE_NAME, deviceName);

    free (deviceInfo.deviceName);
    free (deviceInfo.specVersion);
    OCFreeOCStringLL (deviceInfo.dataModelVersions);
    OCFreeOCStringLL (deviceInfo.types);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

void SetPlatformInfo(OICServerBuilder* builder, char* modelNum, char* pver,
                        char* osver, char* hwver, char* fwver, char* venderid)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if(modelNum == NULL || strlen(modelNum) < 1)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid input for registering platform Info");
        return;
    }

    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Manufacturer :%s",MANUFACTURER_NAME);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Model Name :%s",modelNum);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. Plaform :%s",pver);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. OS :%s",osver);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. HW :%s",hwver);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. FW :%s",fwver);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. vid :%s",venderid);

    OCPlatformInfo platformInfo;

    platformInfo.platformID = (char*)OCGetServerInstanceIDString();
    platformInfo.manufacturerName = MANUFACTURER_NAME;
    platformInfo.modelNumber = modelNum;
    platformInfo.platformVersion = pver;
    platformInfo.operatingSystemVersion = osver;
    platformInfo.hardwareVersion = hwver;
    platformInfo.firmwareVersion = fwver;

    platformInfo.manufacturerUrl = MANUFACTURER_URL;
    platformInfo.dateOfManufacture = NULL;
    platformInfo.supportUrl = NULL;
    platformInfo.systemTime = NULL;

    OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, KEY_ATTR_PLATFORM_VENDERID, venderid);

    OCSetPlatformInfo(platformInfo);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}


OICServerBuilder* GetBuilderInstance()
{
    if(gBuilder == NULL)
    {
        gBuilder = (OICServerBuilder*) DAMalloc (sizeof(OICServerBuilder));
        if(gBuilder != NULL)
        {
            gBuilder->InitModule = &InitBuilder;
            gBuilder->DeinitModule = &DeinitBuilder;
            gBuilder->SetDeviceInfo = &SetDeviceInfo;
            gBuilder->SetPlatformInfo = &SetPlatformInfo;
            gBuilder->CreateResource = &CreateResource;
            // gBuilder->CreateActiveResource = &CreateActiveResource;
            gBuilder->CreateCollectionResource = &CreateCollectionResource;
            gBuilder->GetResource = &GetResource;
            gBuilder->DeleteResource = &DeleteResource;
            gBuilder->AddInterfaceType = &AddInterfaceType;
            gBuilder->AddResourceType = &AddResourceType;
            gBuilder->Bind = &Bind;
            gBuilder->BindAll = &BindAll;
            gBuilder->BroadcastPresence = &BroadcastPresence;
            gBuilder->res_num = 0;
            gBuilder->handler = NULL;

            isPresence = false;

            return gBuilder;
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Not enough Memory for Builder Instance");
            return NULL;
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Builder Instance Already Created");
        return gBuilder;
    }
}

void ReleaseBuilderInstance(OICServerBuilder* builder)
{
    if(builder)
    {
        // [JAY] Thread should be terminated first..
        if(!gQuitFlag)
        {
            builder->DeinitModule(builder);
        }

        if(builder->res_num > 0)
        {
            for(size_t iter = 0 ; iter < builder->res_num ; iter++)
            {
                if(builder->gResArr[iter] != NULL)
                {
                   /*! Added by DA for memory Leak fix
                    */
                    ReleaseResourceInstImpl(builder->gResArr[iter]);
                    builder->gResArr[iter] = NULL;
                }
            }
        }

        DAFree(builder);
        gBuilder = NULL;
        isPresence = false;
    }
}
