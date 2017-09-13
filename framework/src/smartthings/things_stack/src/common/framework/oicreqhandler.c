//#******************************************************************
//#
//# Copyright 2015 SAMSUNG ELECTRONICS All Rights Reserved.
//#
//#******************************************************************
#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE // for the usleep
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "ocpayload.h"

#include "oicdef.h"
#include "oiccommon.h"
#include "oicqueue.h"
#include "thingslogger.h"
#include "thingsmalloc.h"
#include "cacommon.h"
#include "oicstringutil.h"
#include "oicresource.h"
#include "oicnetwork.h"
#include "thingsapi.h"
#include "oiceasysetup.h"
#include "oicserverbuilder.h"
#include "thingsdatamanager.h"
#include "oicreqhandler.h"
#include "oichashmap.h"

#include "oicthread.h"
#ifdef OCF_RTOS
#include "ctype.h"
#include "thingsRtosUtil.h"
#endif

#define TAG "[oicreqhdlr]"

static HandleRequestFuncType             gHandleRequestGetCB = NULL;
static HandleRequestFuncType             gHandleRequestSetCB = NULL;

extern OICServerBuilder*                    gBuilder;

static Queue *                              gpOicReqQueue;
static int                                  gHandleResCnt = 0;
static char**                               gHandleResList = NULL;
static HandleRequestFuncType                gHandleReqCB = NULL;
static GetNotifyObsURICB                    gGetNotifyObsURI = NULL;

//static OICGetAPSearchListFuncType           gGetAPList = NULL;
static HandleRequestInterfaceCB             gHandleRequestInterfaceCB = NULL;

static StopSoftAPFuncType                   gStopSoftAPCB = NULL;

static pthread_t                            gReqHandle;

static int gQuitFlag = 0;

OCEntityHandlerResult OICAbort(pthread_t* hThreadAbort, ESEnrolleeAbort level);

/**
 * Return : 0 (Invalid Request), 1 (Resource Supporting Interface)
 * Need refactoring later...
 */
static int VerifyRequest(OCEntityHandlerRequest* ehRequest, const char* uri, int reqType)
{
    int result = 0;

    if(gBuilder == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Server Builder is not registered..");
        goto EXIT_VALIDATION;
    }

    OicResource* resource = CreateResourceInst(ehRequest->requestHandle,
                                                 ehRequest->resource,
                                                 ehRequest->query,
                                                 ehRequest->payload); //gBuilder->GetResource(gBuilder, uri);
    OicResource* child = NULL;
    /*! Added by DA for memory Leak fix
     */
    OicResource *pstTempResource = resource;
    if(resource == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Resource Not found : %s ",uri);
        goto EXIT_VALIDATION;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Query in the Request : %s", ehRequest->query);

    // [JAY] Verify received request in valid or not..
    // 1. If there's no interface type in the request ..
    if( (strstr(ehRequest->query, "if=") == NULL)
        || (strstr(ehRequest->query, OIC_INTERFACE_BASELINE) != NULL)
        || ( true == resource->IsSupportingInterfaceType(resource, ehRequest->query) ) )
    {
        result = 1;
    }

    // 2. If request type == POST(OC_REST_POST == 4),
    //    then validate the attributes kyes in the request
    if(result == 1 && reqType == OC_REST_POST )
    {
        // [Jay] Reseting the result value to apply
        // the result of additional verification
        result = 0;

        // if(gValidator == NULL )
        // {
        //     THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Validator not registered");
        //     goto EXIT_VALIDATION;
        // }

        // [Jay] If the given resource does not have sensor nor read interface type..
        if( !(resource->IsSupportingInterfaceType(resource, OIC_INTERFACE_SENSOR))
            && !(resource->IsSupportingInterfaceType(resource, OC_RSRVD_INTERFACE_READ) ) )
        {
            // [Jay] If no query or..
            //       query with "supporting interface types"  then...
            if( (strstr(ehRequest->query, "if=") == NULL)
                ||
                ( (strstr(ehRequest->query, "if=") != NULL)
                   && (resource->IsSupportingInterfaceType(resource, ehRequest->query) )
                )
              )
            {
                // [Jay] Verify request(Attributes.) with Validator func implemented
                const int num = resource->GetNumOfResTypes(resource);

                THINGS_LOG_D(THINGS_DEBUG, TAG, "# of RT :%d", num);

                if(resource->size > 1)
                {
                    // Remove "rep" wrapper
                    OicResource* res = resource;
                    OicRepresentation *tRep = NULL;
                    OicRepresentation *tChildRep = NULL;

                    for(int iter = 0; iter < resource->size; ++iter)
                    {
                        tChildRep = CreateRepresentationInst(NULL);
                        res->GetRepresentation(res, &tRep);
                        tRep->GetObjectValue(tRep, OC_RSRVD_REPRESENTATION, tChildRep);
                        res->SetRepresentation(res,tChildRep);

                        res = res->Next(res);
                    }

                    for(int iter = 0; iter < resource->size; ++iter)
                    {
                        child = NULL;

                        if(resource->uri == NULL)
                        {
                            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Resource URI is NULL.");
                            result = OC_EH_ERROR;
                            goto EXIT_VALIDATION;
                        }

                        child = gBuilder->GetResource(gBuilder, resource->uri);

                        if(NULL == child)
                        {
                            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Not Existing Child Resource.");
                            result = OC_EH_ERROR;
                            goto EXIT_VALIDATION;
                        }
                        if(NULL == resource->rep )
                        {
                            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "OicRepresentation is NULL.");
                            result = OC_EH_ERROR;
                            goto EXIT_VALIDATION;

                        }

                        if(num > 0)
                        {
                            for(int i = 0; i < num; i++)
                            {

                                result |= DM_ValidateAttributeInRequest(resource->GetResType(child, i),
                                                                (const void*)/*ehRequest->payload*/resource->rep->payload );
                                if(!result)
                                {
                                    goto EXIT_VALIDATION;
                                }
                            }
                        }

                        resource = resource->Next(resource);

                        // if(targetUri != NULL)   DAFree(targetUri);
                    }
                }
                else
                {
                    if( num > 0 )
                    {
                        for(int iter = 0 ; iter < num ; iter++)
                        {
                            // [Jay] if it's okey in any case then....it's valid..
                            result |= DM_ValidateAttributeInRequest(resource->GetResType(resource, iter),
                                                                (const void*)ehRequest->payload );
                        }
                    }
                }
            }
        }
    }

EXIT_VALIDATION:

   /*! Added by DA for memory Leak fix
    */
    ReleaseResourceInst(pstTempResource);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Validation Result.(%d)", result);

    return result;
}

OCEntityHandlerResult SendResponse( OCRequestHandle requestHandle,
                                    OCResourceHandle resource,
                                    OCEntityHandlerResult result,
                                    const char* uri,
                                    void* payload)
{
    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    OCEntityHandlerResult ehResult = OC_EH_OK;
    OCEntityHandlerResponse response = { 0, 0, OC_EH_ERROR, 0, 0, { },{ 0 }, false };

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Creating Response with Request Handle : %x,Resource Handle : %x",requestHandle, resource);

    response.numSendVendorSpecificHeaderOptions = 0;
    memset(response.sendVendorSpecificHeaderOptions,
            0, sizeof response.sendVendorSpecificHeaderOptions);
    memset(response.resourceUri, 0, sizeof(response.resourceUri));

    if(NULL != requestHandle && NULL != resource)
    {
        response.requestHandle = requestHandle;//ehRequest->requestHandle;
        response.resourceHandle = resource;//ehRequest->resource;
        response.persistentBufferFlag = 0;
        response.ehResult = result;
        if(payload != NULL)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Payload is Not NULL");
            response.payload = (OCPayload*)payload;
        }
        else
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "No Payload");
            response.payload = NULL;
        }

        THINGS_LOG_V(THINGS_INFO, TAG, "\t\t\tRes. : %s ( %d )",
            (response.ehResult == OC_EH_OK ? "NORMAL":"ERROR"), response.ehResult);

        OCStackResult ret = OCDoResponse(&response);
        THINGS_LOG_V(THINGS_INFO, TAG, "\t\t\tMsg. Out? : (%s)", (ret == OC_STACK_OK) ? "SUCCESS":"FAIL" );
        if (ret != OC_STACK_OK)
        {
            ehResult = OC_EH_ERROR;
        }
    }
    else
    {
        ehResult = OC_EH_ERROR;
    }

    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ehResult;
}

//OCEntityHandlerResult
//EntityHandler (OCEntityHandlerFlag flag,
//        OCEntityHandlerRequest *entityHandlerRequest, void* callback)
//{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
//    OCEntityHandlerResult ehResult = OC_EH_ERROR;
//
//    // Validate pointer
//    if (!entityHandlerRequest)
//    {
//        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid request pointer");
//        return ehResult;
//    }
//
//    const char* uri = OCGetResourceUri(entityHandlerRequest->resource);
//
//    // Observe Request Handling
//    if (flag & OC_OBSERVE_FLAG)
//    {
//        if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
//        {
//            THINGS_LOG_V(THINGS_INFO, TAG, "Observe Requset on : %s ", uri);
//            // 1. Check whether it's Observe request on the Collection Resource
//            if(NULL != strstr(uri, URI_DEVICE_COL) )
//            {
//                //2. Check whether the query carriese the if=oic.if.b
//                if( (strstr(entityHandlerRequest->query, OIC_INTERFACE_BATCH) == NULL) )
//                {
//                    //3. If not batch then error
//                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Collection Resource Requires BATCH for Observing : %s", entityHandlerRequest->query);
//                    ehResult = OC_EH_BAD_REQ;
//                    goto RESPONSE_ERROR;
//                }
//                else
//                {
//                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Receiving Observe Request Collection Resource");
//                }
//            }
//        }
//        else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
//        {
//            THINGS_LOG_V(THINGS_INFO, TAG, "CancelObserve Request on : %s", uri);
//        }
//    }
//
//    // Get/Post Request Handling
//    if (flag & OC_REQUEST_FLAG)
//    {
//        THINGS_LOG_D(THINGS_DEBUG, TAG, "Req. URI  : %s", uri);
//        THINGS_LOG_D(THINGS_DEBUG, TAG, "Req. TYPE : %d", entityHandlerRequest->method);
//
//        if( OICGetResetMask(RST_CONTROL_MODULE_DISABLE) == true )
//        {
//            THINGS_LOG(THINGS_INFO, TAG, "Control Module Disable.");
//            ehResult = OC_EH_NOT_ACCEPTABLE;
//        }
//        else if( (OC_REST_GET == entityHandlerRequest->method)
//            || (OC_REST_POST == entityHandlerRequest->method) )
//        {
//            THINGS_LOG_D(THINGS_DEBUG, TAG, "Request Handle : %x, Resource Handle : %x",
//                        entityHandlerRequest->requestHandle, entityHandlerRequest->resource);
//            if( VerifyRequest(entityHandlerRequest, uri, (int)entityHandlerRequest->method) > 0 )
//            {
//                // [JAY] IoTivity Stack Destroy the payload after receving result from this function
//                //       Therefore, we need to copy/clone the payload for the later use..
//                OicResource* resource = CreateResourceInst(
//                                            entityHandlerRequest->requestHandle,
//                                            entityHandlerRequest->resource,
//                                            entityHandlerRequest->query,
//                                            entityHandlerRequest->payload);
//                resource->SetDevAddr(resource, &(entityHandlerRequest->devAddr));
//                //resource->SetUri(resource, uri);
//
//                THINGS_LOG_D(THINGS_DEBUG, TAG, "About to Queue a received Request~!!!!");
//
//                //gOicReqQueue.push(&gOicReqQueue, resource, entityHandlerRequest->method);
//                gpOicReqQueue->push(gpOicReqQueue, resource, entityHandlerRequest->method);
//                ehResult = OC_EH_SLOW;
//            }
//            else
//            {
//                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Query in the Request : %s",
//                    entityHandlerRequest->query);
//            }
//        }
//        else if( OC_REST_DELETE == entityHandlerRequest->method
//              || OC_REST_PUT == entityHandlerRequest->method )
//        {
//            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Delete/PUT Req. Handling is not supported Yet");
//        }
//        else
//        {
//            THINGS_LOG_D(THINGS_DEBUG, TAG, "Received unsupported method from client");
//        }
//
//    }
//
//RESPONSE_ERROR:
//
//    if (ehResult != OC_EH_SLOW)
//    {
//        // [JAY] If the result is OC_EH_ERROR, the stack will remove the
//        //       received request in the stack.
//        //       If the reusult is OC_EH_SLOW, then the request will be
//        //       stored in the stack till the response goes out
//        ehResult = SendResponse(entityHandlerRequest->requestHandle,
//                                entityHandlerRequest->resource,
//                                ehResult, uri, NULL);
//        //[JAY] Currently this code will not do anything...
//        //      Need to refactor later..
//    }
//
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
//
//    return ehResult;
//}

#if 0
static OCEntityHandlerResult SetActionResource(OicResource* targetResource)
{

    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    char* pQuery[MAX_QUERY_LEN] = {NULL};
    OICScheduleInfo** pScheduleInfo = NULL;
    int listCnt = 0;

    char* spliter[3] = {";","?","#"};
    bool isMultiQuery = false;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "targetResource->query is : %s", targetResource->query);

    if(targetResource->query != NULL)
    {
        for(int iter = 0 ; iter < 3 ; iter++)
        {
            if(NULL != strstr(targetResource->query, spliter[iter]) )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Query Spliter is : %s", spliter[iter]);
                if(false == RemoveQuery(targetResource->query, pQuery,
                                            spliter[iter], "if="))
                {

                    if(NULL != targetResource->query)
                        strncat(pQuery, targetResource->query,
                            MAX_QUERY_LEN - 1);

                }
                isMultiQuery = true;
                break;
            }
        }
        if(!isMultiQuery)
        {
            if(0 < strlen(targetResource->query))
            {
                strncat(pQuery, targetResource->query, strlen(targetResource->query));
            }
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "pQuery is : %s", pQuery);

    if(gCvtSchReqInfoCB)
    {
        pScheduleInfo = (OICScheduleInfo**)DAMalloc(sizeof(OICScheduleInfo*) * MAX_NUM_SCHEDULE);
        ehResult = gCvtSchReqInfoCB(targetResource, pScheduleInfo, &listCnt);
        //if(OC_EH_OK == gCvtSchReqInfoCB(targetResource, pScheduleInfo, &listCnt))
        if(ehResult == OC_EH_OK)
        {
            if(!gSetScheduleFunc)
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "SET SCHEDULE CB NOT REGISTERED~!!");
                ehResult = OC_EH_ERROR;
            }
            else
            {
                ehResult = gSetScheduleFunc(pQuery, pScheduleInfo, listCnt);
            }
        }
        else
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "SCHEDULE INFO CONVERTER FAILED~!!");
        }
        for (int iter = 0; iter< listCnt; iter++)
        {
            DAFree(pScheduleInfo[iter]);
        }
        DAFree(pScheduleInfo);
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "SCHEDULE INFO CONVERTER NOT REGISTERED~!!");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "SET ACTION RESULT : %s", (ehResult == OC_EH_OK ? "SUCCESS" : "FAIL"));

    return ehResult;

    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}
#endif

static bool isNumber(char* str)
{
    int len = strlen(str);
    for (int index = 0 ; index < len ; index++)
    {
        if(!isdigit(str[index]))
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "It's NOT NUMBER : %s", str);
            return false;
        }
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, "It is NUMBER : %s", str);
    return true;
}

#ifndef OCF_RTOS
static int GetIdValueFromQuery(char idvalue[], char* inputQuery, int size)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Input query => %s", inputQuery);

    char seperators[] = "?;#&";

    if (NULL == inputQuery)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Input query is NULL");
        return 0;
    }

    if (size > MAX_QUERY_LEN)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Input size bigger than maximum query size(%d)", MAX_QUERY_LEN);
        return 0;
    }

    /*[Jaehong] (Need to consider trim to remove space in query) */
    // Remove empty space
    char queries[MAX_QUERY_LEN + 1] = { 0, };
    int index = 0;
    for (int i = 0; i < strlen(inputQuery); i++) {
        if (*(inputQuery + i) == ' ') continue;
        queries[index] = *(inputQuery + i);
        index++;
    }

    // Get Query start with "id="
    char* idQuery = strstr(queries, "id=");
    if (NULL == idQuery)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "There is no \"id=\" in input query");
        return 0;
    }

    // Seperate idQuery with seperators [?, ;, #]
    for (int i = 0; i < strlen(idQuery); i++) {
        if (*(idQuery + i) == '?' || *(idQuery + i) == ';'
            || *(idQuery + i) == '#' || *(idQuery + i) == '&')
        {
            strtok(idQuery, seperators);
            break;
        }
    }

    for (int i = 3, k = 0; i < strlen(idQuery); i++)
    {
        if (idQuery[i] >= '0' && idQuery[i] <= '9')
        {
            idvalue[k++] = idQuery[i];
        }
        else
        {
            idvalue[0] = '\0';
            return 0;
        }
    }

    return 1;
}

static OCEntityHandlerResult StoreTransferedFile(char* path, uint8_t * byteValue, size_t size)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if( path == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Path : %s ", path);
    }
    else
    {
        if(byteValue == NULL)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Blob data is NULL(empty) ");
        }
        else
        {
            FILE * fp = fopen(path,"wb");
            if(NULL == fp)
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not able to open File : %s !!!", path);
            }
            else
            {
                fwrite( byteValue, 1, size, fp);

                fclose(fp);

                ehResult = OC_EH_OK;
            }
        }
    }

    return ehResult;
}
static OCEntityHandlerResult SetFileTransferResource(OicResource* targetResource)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    int filenum = -1;
    char* opType = NULL;
    char id[MAX_QUERY_LEN] = {0};

    // 2. Check whether is is POST with UPDATE request or not.
    // THINGS_LOG_D(THINGS_DEBUG, TAG, "POST with op = %s", opType);
    targetResource->GetQuery(targetResource, OIC_QUERY_KEY_OP, &opType);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "POST with op = %s", opType);
    if(NULL != opType && strlen(opType) > 0)
    {
        if(NULL == strcmp(opType, OIC_QUERY_VALUE_UPDATE))
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "POST with op = %s", opType);

            // 1. Get ID from the Query
            if(targetResource->query != NULL
                && strlen(targetResource->query) > 0)
            {
                if (GetIDValueFromQuery(id, targetResource->query, MAX_QUERY_LEN))
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Getting Id Value Success");
                }
                else
                {
                    memset(id, 0, MAX_QUERY_LEN);
                    THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Getting Id Value Failed");
                }
            }

            if(strlen(id) > 0
                && isNumber(id) )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Selected file ID to store is %s", id);

                filenum = atoi(id);

                // 2. Get File Path using the ID
                char* name = NULL;
                const char * file = DM_GetFileName(filenum);
                // 3. Check the payload & Open the file
                if(NULL != file && strlen(file) > 0)
                {
                    int num = 0;
                    size_t size = 0;
                    uint8_t * value = NULL;
                    OicRepresentation** array = NULL;

                    targetResource->rep->GetArrayValue(targetResource->rep, SEC_ATTRIBUTE_COMMON_ITEMS, &num, &array);

                    //for(int iter = 0 ; iter < num ; iter++)
                    if(num > 0)
                    {
                        value = NULL;
                        size = 0;
                        array[0]->GetValue(array[0], SEC_ATTRIBUTE_NAME, &name);
                        array[0]->GetByteValue(array[0], SEC_ATTRIBUTE_FILE_BLOB, &value, &size);

                        THINGS_LOG_V(THINGS_INFO, TAG, " NAME : %s\t", name);

                        // 4. Write file
                        ehResult = StoreTransferedFile(file, value, size);

                        // 5. Inform App thru Callback, and response back to the DA SW.
                        if(NULL != gNotifyFileUpdate)
                        {
                            gNotifyFileUpdate(id, file);
                        }
                        else
                        {
                            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "CB for Notifying file update not registered ");
                        }

                        if(name)
                        {
                            DAFree(name);
                            name = NULL;
                        }

                        /*! Added by DA for memory Leak fix
                         */
                        for(int iter = 0 ; iter < num ; iter++){
                          ReleaseRepresentationInst(array[iter]);
                        }
                        if(array) DAFree(array);
                    }
                    else
                    {
                        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "No items in the object array (Payload) %d", num);
                    }
                }
                else
                {
                    THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "No such file exist with the file id : %d", filenum);
                }
            }
            else
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Cannot handle the request without File ID");
            }
        }
        else
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Supported OP Type : %s", opType);
        }
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Cannot handle the request without op type in the query : %s", id);
    }
    if(opType) DAFree(opType);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ehResult;
}

static OCEntityHandlerResult GetFileListResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    int appIndex = 0;
    int index = DM_GetNumOfFiles();

    OicRepresentation * rep = targetResource->rep;

    // 0. Allocate memory for the file list.
    OICFile ** files = (OICFile**)DAMalloc(sizeof(OICFile*) * MAX_NUM_OICFILE );

    // 1. intialization of the variable
    for(int size = 0 ; size < MAX_NUM_OICFILE ; size++)
    {
        files[size] = NULL;
    }

    // 2. Get File list from mother Application
    if(gGetFileListCB)
    {
        if(1== gGetFileListCB(files, &appIndex) )
        {
            THINGS_LOG_V(THINGS_DEBUG, TAG, "File list Retrieved from the App. CNT : %d", appIndex);
        }
    }
    else
    {
         THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "File list Get CB not Registered");
    }

    // 3. Check if there's files to share
    if(index > 0 || appIndex > 0)
    {
        int num = 0;
        OicRepresentation * childRep[index + appIndex];

        // 4. Share information of the files described in the oic_info file
        for(num = 0 ; num < (index)  ; num++)
        {
            char * name = DM_GetFileName(num);
            int  fileID = DM_GetFileID(num);

            // 3. Create payload if target file exist
            if (NULL != name && strlen(name) > 0 )
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "id : %d \tname : %s", fileID, name);



                childRep[num] = CreateRepresentationInst(NULL);
                // childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_NAME, name);
                char * fileName = strrchr(name, '/')+1;
                if(NULL != fileName)
                {
                    childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_NAME, fileName);
                }
                else
                {
                    childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_NAME, name);
                }
                char sfileID[MAX_LEN_FILE_ID] = {};
                memset(sfileID, 0, MAX_LEN_FILE_ID);
                snprintf(sfileID, MAX_LEN_FILE_ID, "%d", fileID);
                childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_COMMON_ID, sfileID );
            }
            else
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Payload Creation with File list failed");
                goto EXIT_GET_FILE_LIST;
            }
        }

        // 5. Share information of the files informed from the mother application
        for(num ; num < (index + appIndex) ; num++)
        {
            // THINGS_LOG_V(THINGS_DEBUG, TAG, "INDEX [%d] == NUM : %d", (num - index), num);
            THINGS_LOG_V(THINGS_DEBUG, TAG, "id : %d \tname : %s", files[ num - index ]->id, files[ num - index ]->path);
            childRep[num] = CreateRepresentationInst(NULL);

            char sfileID[MAX_LEN_FILE_ID] = {};
            memset(sfileID, 0, MAX_LEN_FILE_ID);
            snprintf(sfileID, MAX_LEN_FILE_ID, "%d", files[ num - index ]->id);

            childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_COMMON_ID,   sfileID );
            // childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_NAME, files[ num - index ]->path);
            char * fileName = strrchr( (files[ num - index ]->path), '/')+1;
            if(NULL != fileName)
            {
                childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_NAME, fileName);
            }
            else
            {
                childRep[num]->SetValue(childRep[num], SEC_ATTRIBUTE_NAME, files[ num - index ]->path);
            }
        }

        // 6. Insert gathered information of files as an attribute value
        rep->SetArrayValue(rep, SEC_ATTRIBUTE_COMMON_ITEMS, index + appIndex, childRep);

        /*! Added by DA for memory Leak fix
         */
        for(num=0 ; num < (index + appIndex) ; num++) {
          ReleaseRepresentationInst(childRep[num]);
          childRep[num] = NULL;
        }
        ehResult = OC_EH_OK;
    }

EXIT_GET_FILE_LIST:

    if(files)
    {
        for(int index2 = 0 ; index2 < appIndex ; index2++)
        {
            if(files[index2]->path)
            {
                DAFree(files[index2]->path);
                files[index2]->path = NULL;
            }
            /*! Added by DA for memory Leak fix
             */
            if(files[index2])
            {
                DAFree(files[index2]);
                files[index2] = NULL;
            }
        }
        DAFree(files);
        files = NULL;
    }

    return ehResult;
}

static OCEntityHandlerResult CreatePayloadToTransferFile(OicResource* targetResource,
                                                        const char** path,
                                                        int num)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    OicRepresentation * rep = targetResource->rep;
    OicRepresentation * childRep[num];
    int repCnt = 0;

    for(int i = 0 ; i < num ; i++) childRep[i] = NULL;

    FILE *fp = NULL;

    if( NULL != path )
    {
        for(int index = 0 ; index < num ; index++)
        {
            fp = fopen(path[index],"rb");
            if(NULL == fp)
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not able to open File : %s !!!", path[index]);
            }
            else
            {
                //Read the files in byte..
                if( 0 == fseek(fp, 0L, SEEK_END) )
                {
                    size_t size = ftell(fp);
                    // long size = ftell(fp);
                    fseek(fp, 0L, SEEK_SET);

                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "File Size  : %d !!!", size);
                    if(size > 0)
                    {
                        uint8_t * value = (uint8_t *)DAMalloc(size + 1);

                        if(value)
                        {
                            // Check the read file length
                            size_t bytesRead = fread(value, 1, size, fp);
                            // Append string end
                            value[bytesRead] = '\0';

                            THINGS_LOG_V(THINGS_DEBUG, TAG, "\tFile Size [%d]\t Byte Read [%d]", size, bytesRead);

                            // Set byte value into the payload
                            if(NULL != rep)
                            {
                                char * fileName = strrchr((char*)path[index], '/')+1;

                                childRep[repCnt] = CreateRepresentationInst(NULL);
                                childRep[repCnt]->SetByteValue(childRep[repCnt], SEC_ATTRIBUTE_FILE_BLOB, value, size);
                                // childRep[repCnt]->SetValue(childRep[repCnt], SEC_ATTRIBUTE_NAME, (char*)path[index]);
                                if(NULL != fileName)
                                {
                                    childRep[repCnt]->SetValue(childRep[repCnt], SEC_ATTRIBUTE_NAME, fileName);
                                }
                                else
                                {
                                    childRep[repCnt]->SetValue(childRep[repCnt], SEC_ATTRIBUTE_NAME, (char*)path[index]);
                                }

                                repCnt++;

                                ehResult = OC_EH_OK;
                            }

                        }
                        else
                        {
                            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not enough memory !!!");
                        }
                        DAFree(value);
                    }
                    else
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invaild File(Empty) !!!");
                    }
                }
                else
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "File size is not deterministic !!!");
                }
                fclose(fp);
                fp = NULL;
            }
        }

        // [Jay] Send payload at least there's one file to transfer
        THINGS_LOG_V(THINGS_INFO, TAG, "[ %d ] number of file(s) to transfer", repCnt);
        if(repCnt > 0)
        {
            rep->SetArrayValue(rep, SEC_ATTRIBUTE_COMMON_ITEMS, repCnt, childRep);
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "NO File to transfer~!!!!!");
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "NO File to transfer~!!!!!");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ehResult;
}

static OCEntityHandlerResult GetFileBlobValueResult(OicResource* targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    int filenum = -1;

    char id[MAX_QUERY_LEN] = {0};

    // 1. Get id from query
    if(targetResource->query != NULL
        && strlen(targetResource->query) > 0)
    {
        if (GetIdValueFromQuery(id, targetResource->query, MAX_QUERY_LEN))
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Getting Id Value Success");
        }
        else
        {
            memset(id, 0, MAX_QUERY_LEN);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Getting Id Value Failed");
        }
    }

    if(strlen(id) > 0
        && isNumber(id) )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Selected file ID to transfer is %s", id);

        filenum = atoi(id);

        // 2. Check the file with ID
        char* list[1] = {};
        char * file = DM_GetFileName(filenum);
        // 3. Create payload if target file exist
        if(NULL != file && strlen(file) > 0)
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "File Found @ ocf_info");

            list[0] = (char*)DACalloc( (strlen(file) + 1), sizeof(char) );
            DAStrncpy(list[0], file, strlen(file));
        }
        else
        {
            // TODO : Need to ask Application to share the File.
            if(gGetFileListCB)
            {
                int appIndex = 0;
                OICFile ** files = (OICFile**)DAMalloc (sizeof(OICFile*) * MAX_NUM_OICFILE);

                if(1== gGetFileListCB(files, &appIndex) )
                {
                    if(appIndex > 0)
                    {
                        for (int index = 0 ; index < appIndex ; index++)
                        {
                            if(files[index]->id == filenum)
                            {
                                THINGS_LOG_D(THINGS_INFO, TAG, "File Found @ Application shared file list");

                                list[0] = (char*)DACalloc((strlen(files[index]->path) + 1), sizeof(char)  );
                                DAStrncpy(list[0], files[index]->path, strlen(files[index]->path) );
                                break;
                            }
                        }
                    }
                }

                if(files)
                {
                    for(int index2 = 0 ; index2 < appIndex ; index2++)
                    {
                      if(files[index2]->path) DAFree(files[index2]->path);
                        files[index2]->path = NULL;
                        /*! Added by DA for memory Leak fix
                         */
                        if(files[index2]) DAFree(files[index2]);
                        files[index2] = NULL;
                    }
                    DAFree(files);
                    files = NULL;
                }
            }
            else
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Get File List CB not Registered");
            }
        }

        if(NULL != list[0])
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "file ID[%s], path : %s", id, list[0]);

            ehResult = CreatePayloadToTransferFile(targetResource, list, 1);

            DAFree(list[0]);
            list[0] = NULL;
        }

        if(ehResult == OC_EH_ERROR)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "No such file exist with the file id : %s", id);
        }
    }
    else
    {
        int index = 0;
        int index2 = 0;
        int appIndex = 0;
        int num = DM_GetNumOfFiles();

        OICFile ** files = (OICFile**)DACalloc ( MAX_NUM_OICFILE , sizeof(OICFile*) );

        if(gGetFileListCB)
        {
            if(1 != gGetFileListCB(files, &appIndex) )
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to get file list from the Application");
                THINGS_LOG_D(THINGS_INFO, TAG, "Reset cnt of Application shared file list to Zero");
                appIndex = 0;
            }
        }

        char* list[num + appIndex];

        for(index2 = 0 ; index2 < (num + appIndex) ; index2++ )
        {
            list[index2] = NULL;
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "ID value not delivered. Sharing All the Files : OCFINFO [%d], APP [%d]", num, appIndex);

        for(index = 0 ; index < num ; index++)
        {
            char* name = DM_GetFileName(index);
            list[index] = (char*)DACalloc( (strlen(name) + 1) , sizeof(char) );
            DAStrncpy(list[index], name, strlen(name));

            THINGS_LOG_D(THINGS_DEBUG, TAG, "(1) file path : %s", list[index]);
        }

        for(index ; index < (num + appIndex) ; index++)
        {
            list[index] = (char*)DACalloc((strlen(files[index - num]->path) + 1), sizeof(char) );
            DAStrncpy(list[index], files[index - num]->path , strlen(files[index - num]->path) );

            THINGS_LOG_D(THINGS_DEBUG, TAG, "(2) file path : %s", list[index]);
        }

        ehResult = CreatePayloadToTransferFile(targetResource, list, num + appIndex);

        for(index2 = 0 ; index2 < (num + appIndex ); index2++)
        {
            /*! Added by DA for memory Leak fix
             */
            if(files[index2]->path) {
              DAFree(files[index2]->path);
              files[index2]->path = NULL;
            }
            if(files[index2]) DAFree(list[index2]);
            list[index2] = NULL;
        }
    }

    return ehResult;
}
#endif



/**
 * This functions will replace thie ConverAPInfo(~~ )
 */
 static OCEntityHandlerResult ConvertAPInfor(OicResource* targetResource, AccessPointInfo1** pList, int listCnt)
 {
 
     OCEntityHandlerResult ehResult = OC_EH_ERROR;
 
     OicRepresentation* rep = NULL;
 
     if( !(listCnt == 0 || pList == NULL) )
     {
 
         if(targetResource->rep == NULL)
         {
             rep = CreateRepresentationInst(NULL);
             targetResource->SetRepresentation(targetResource, rep);
         }
         else
         {
             rep = targetResource->rep;
         }
 
         OicRepresentation** childRep = (OicRepresentation**)DAMalloc(listCnt * sizeof(OicRepresentation*));
 
         for(int iter = 0 ; iter < listCnt ; iter++)
         {
 
             THINGS_LOG_D(THINGS_DEBUG, TAG, "ESSID : %s", pList[iter]->ESSID);
             THINGS_LOG_D(THINGS_DEBUG, TAG, "SecurityKey : %s", pList[iter]->SecurityKey);
             THINGS_LOG_D(THINGS_DEBUG, TAG, "AuthType : %s", pList[iter]->AuthType);
             THINGS_LOG_D(THINGS_DEBUG, TAG, "EncType : %s", pList[iter]->EncType);
             THINGS_LOG_D(THINGS_DEBUG, TAG, "Channel : %s", pList[iter]->Channel);
             THINGS_LOG_D(THINGS_DEBUG, TAG, "SignalLevel : %s", pList[iter]->SignalLevel);
             THINGS_LOG_D(THINGS_DEBUG, TAG, "BSSID : %s", pList[iter]->BSSID);
 
             childRep[iter] = CreateRepresentationInst(NULL);
             childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_AP_CHANNAL, pList[iter]->Channel);
             childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_AP_ENCTYPE, pList[iter]->EncType);
             childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_AP_MACADDR, pList[iter]->BSSID);
             childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_AP_MAXRATE, "MAX_RATE");
             childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_AP_RSSI, pList[iter]->SignalLevel);
             childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_AP_SECTYPE, pList[iter]->AuthType);
             childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_AP_SSID, pList[iter]->ESSID);
         }
 
         rep->SetArrayValue(rep, SEC_ATTRIBUTE_AP_ITEMS, listCnt, childRep);
 
         for (int iter = 0; iter< listCnt; iter++)
         {
             /*! Changed by DA for memory Leak fix
              */
             ReleaseRepresentationInst(childRep[iter]);
             childRep[iter] = NULL;
         }
         DAFree(childRep);
         childRep = NULL;
 
         ehResult = OC_EH_OK;
     }
 
     return ehResult;
 }

/**
 * This functions will be deprecated~!!!
 */
 static OCEntityHandlerResult GetAPScanResult(OicResource* targetResource)
 {
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    
    AccessPointInfo1** pList = NULL;
    int listCnt = 0;
    int result = 0;

    // pList = (AccessPointInfo1**)DAMalloc(sizeof(AccessPointInfo1*) * MAX_NUM_AP);

    result = OICGetAPList(&pList, &listCnt);
    if(result)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Get Access points list!!! %d ", listCnt);
        if(listCnt > 0)
        {
            //ehResult = ConvertAPInfo(targetResource, pList, listCnt);
            ehResult = ConvertAPInfor(targetResource, pList, listCnt);
        }
        else if(listCnt == 0)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "0 Access points!!!! %d ", listCnt);
            ehResult = OC_EH_OK;
        }
        else
        {
            ehResult = OC_EH_ERROR;
        }
    }
    else
    {
        THINGS_LOG_V(THINGS_ERROR, TAG, "GET AP Searching List FUNC Failed!!!!");
    }

    if(pList != NULL)
    {
        for(int i = 0; i< listCnt;i++)
        {
            DAFree(pList[i]);
        }
        DAFree(pList);
    }
    
    return ehResult;
 }

/**
 * This functions will replace the GetAPScanResult(~~)
 */
static OCEntityHandlerResult GetProvisioningInfo(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    // if(ehResult == OC_EH_OK)
    {

        const char * deviceID = OCGetServerInstanceIDString();
        const char * deviceRT = DM_GetOCFDeviceType(0);
        bool isOwned = false;
        bool isReset = OICGetResetMask(RST_ALL_FLAG);
		/* if(!isReset) { */
		/* 	isReset = 1; */
		/* } */

        if( OC_STACK_OK != OCGetDeviceOwnedState(&isOwned) )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to get device owned state, Informing as UNOWNED~!!!!");
            isOwned = false;
        }
        if(targetResource->rep == NULL)
        {
            /*! Added by DA for memory Leak fix
             */
            OicRepresentation *pstRep = NULL;
            pstRep = CreateRepresentationInst(NULL);
            targetResource->SetRepresentation(targetResource, pstRep);
        }

        OicRepresentation* childRep[1] = {NULL};

        childRep[0] = CreateRepresentationInst(NULL);
        childRep[0]->SetValue(childRep[0], SEC_ATTRIBUTE_PROV_TARGET_ID, deviceID);
        childRep[0]->SetValue(childRep[0], SEC_ATTRIBUTE_PROV_TARGET_RT, deviceRT);

        // childRep[0]->SetBValue(childRep[0], SEC_ATTRIBUTE_PROV_TARGET_OWNED, isOwned);
        childRep[0]->SetBValue(childRep[0], SEC_ATTRIBUTE_PROV_TARGET_PUBED, DM_IsRscPublished());

        targetResource->rep->SetArrayValue(targetResource->rep,
                                           SEC_ATTRIBUTE_PROV_TARGETS,
                                                 1,
                                           childRep);

        targetResource->rep->SetBValue(targetResource->rep,
                                       SEC_ATTRIBUTE_PROV_TARGET_OWNED,
                                       isOwned);

        targetResource->rep->SetValue(targetResource->rep,
                                      SEC_ATTRIBUTE_PROV_EZSETDI,
                                      deviceID);

        targetResource->rep->SetBValue(targetResource->rep,
                                       SEC_ATTRIBUTE_PROV_RESET,
                                       isReset);

        targetResource->rep->SetIValue(targetResource->rep,
                                       SEC_ATTRIBUTE_PROV_ABORT,
                                       0);
        /*! Added by DA for memory Leak fix
         */
        if(NULL != childRep[0]){ ReleaseRepresentationInst(childRep[0]); }

        ehResult = OC_EH_OK;
    }

    return ehResult;
}

#ifndef OCF_RTOS
static OCEntityHandlerResult GetDeviceInfoResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(gGetDeviceInfoCB)
    {
        OICDeviceInfo * info = (OICDeviceInfo*) DAMalloc ( sizeof(OICDeviceInfo) );

        memset(info->modelID,     0, MAX_ATTRVALUE_LEN);
        memset(info->description, 0, MAX_ATTRVALUE_LEN);
        memset(info->serialNum,   0, MAX_ATTRVALUE_LEN);
        memset(info->versionId,   0, MAX_ATTRVALUE_LEN);
        memset(info->approveState,0, MAX_ATTRVALUE_LEN);

        info->itemCnt = 0;

        for(int iter = 0 ; iter < MAX_VER_CNT ; iter++)
        {
            info->items[iter] = (OICVersionItem*) DAMalloc (sizeof(OICVersionItem) );

            memset(info->items[iter]->id,     0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->type,   0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->name,   0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->number, 0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->newVersionAvailable,   0, MAX_ATTRVALUE_LEN);
        }

        int iter = 0;
        int res = gGetDeviceInfoCB(&info);
        if(res != 1)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Error Occurred while getting device information (%d).", ehResult);
            ehResult = OC_EH_ERROR;
        }
        else
        {
            // if(0 != IsDeviceInfoExist(info))
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Device Information found!!!!");
                // TODO : Payload Creation with received device information
                if(info->modelID != NULL && strlen(info->modelID) > 0)
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "\tmodelID : %s",info->modelID);
                    targetResource->rep->SetValue(targetResource->rep,
                                                  SEC_ATTRIBUTE_MODEL_NUM,
                                                  info->modelID);
                }
                if(info->description != NULL && strlen(info->description) > 0)
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "\tdescription : %s",info->description);
                    targetResource->rep->SetValue(targetResource->rep,
                                                  SEC_ATTRIBUTE_DESCRIPTION,
                                                  info->description);
                }
                if(info->serialNum != NULL && strlen(info->serialNum) > 0)
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "\tserialNum : %s",info->serialNum);
                    targetResource->rep->SetValue(targetResource->rep,
                                                  SEC_ATTRIBUTE_SERIAL_NUM,
                                                  info->serialNum);
                }
                if(info->versionId != NULL && strlen(info->versionId) > 0)
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "\tversionId : %s",info->versionId);
                    targetResource->rep->SetValue(targetResource->rep,
                                                  SEC_ATTRIBUTE_VERSION_ID,
                                                  info->versionId);
                }
                if(info->approveState != NULL && strlen(info->approveState) > 0)
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "\tapproveState : %s",info->approveState);
                    targetResource->rep->SetValue(targetResource->rep,
                                                  SEC_ATTRIBUTE_APPROVED_STATE,
                                                  info->approveState);
                }


                // char sfileID[MAX_LEN_FILE_ID] = {};
                OicRepresentation** childRep  = NULL;

                if(info->itemCnt > 0)
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "Items CNT : %d!!!!",info->itemCnt);

                    childRep  = (OicRepresentation**)DAMalloc(sizeof(OicRepresentation*) * info->itemCnt);

                    for( iter = 0 ; iter < info->itemCnt; iter++)
                    {

                        THINGS_LOG_V(THINGS_DEBUG, TAG, "\tid : %s",info->items[iter]->id);
                        THINGS_LOG_V(THINGS_DEBUG, TAG, "\ttype : %s",info->items[iter]->type);
                        THINGS_LOG_V(THINGS_DEBUG, TAG, "\tname : %s",info->items[iter]->name);
                        THINGS_LOG_V(THINGS_DEBUG, TAG, "\tnumber : %s",info->items[iter]->number);
                        THINGS_LOG_V(THINGS_DEBUG, TAG, "\tavailable : %s",info->items[iter]->newVersionAvailable);


                        childRep[iter] = CreateRepresentationInst(NULL);
                        childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_ID,   info->items[iter]->id);
                        childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_TYPE, info->items[iter]->type);
                        childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_THINGS_NAME, info->items[iter]->name);
                        childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_NUMBER, info->items[iter]->number);
                        childRep[iter]->SetValue(childRep[iter], SEC_ATTRIBUTE_VERSION_AVAILABLE, info->items[iter]->newVersionAvailable);
                    }

                    targetResource->rep->SetArrayValue(targetResource->rep,
                                                  SEC_ATTRIBUTE_ITEMS,
                                                  info->itemCnt,
                                                  childRep);

                    THINGS_LOG_V(THINGS_DEBUG, TAG, "Items ++++++++++++++++++ Done !");
                }
                else
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "No Items  ++++++++++++++++++ Done !");
                }

                ehResult = OC_EH_OK;
            }
            // else
            // {
            //     THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Information not found!!!!");
            // }
        }

        // 3. De-allocated memory of OICDeviceInformation & its items
        for( iter = 0 ; iter < info->itemCnt; iter++)
        {
            DAFree(info->items[iter]);
            info->items[iter] = NULL;
        }
        DAFree(info);
        info = NULL;
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "GET DEVICE Information CB NOT REGISTERED~!!!!");
    }

    return ehResult;
}

static OCEntityHandlerResult SetDeviceInfoResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(gSetDeviceInfoCB)
    {
        int iter = 0;

        OICDeviceInfo * info = (OICDeviceInfo*) DAMalloc ( sizeof(OICDeviceInfo) );

        memset(info->modelID,     0, MAX_ATTRVALUE_LEN);
        memset(info->description, 0, MAX_ATTRVALUE_LEN);
        memset(info->serialNum,   0, MAX_ATTRVALUE_LEN);
        memset(info->versionId,   0, MAX_ATTRVALUE_LEN);
        memset(info->approveState,0, MAX_ATTRVALUE_LEN);

        info->itemCnt = 0;

        for(int iter = 0 ; iter < MAX_VER_CNT ; iter++)
        {
            info->items[iter] = (OICVersionItem*) DAMalloc (sizeof(OICVersionItem) );

            memset(info->items[iter]->id,     0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->type,   0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->name,   0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->number, 0, MAX_ATTRVALUE_LEN);
            memset(info->items[iter]->newVersionAvailable,   0, MAX_ATTRVALUE_LEN);
        }

        // 1. User Update Item Information
        OicRepresentation** array = NULL;
        char * value = NULL;

        targetResource->rep->GetArrayValue(targetResource->rep,
                                          SEC_ATTRIBUTE_ITEMS,
                                          &info->itemCnt,
                                          &array);

        THINGS_LOG_D(THINGS_DEBUG, TAG, "Items Cnt = %d", info->itemCnt);
        if(array != NULL)
        {

            for(iter = 0 ; iter < info->itemCnt ; iter++)
            {
                if(true == (array[iter])->GetValue(array, SEC_ATTRIBUTE_ID, &value) )
                {
                    memcpy(info->items[iter]->id, value, strlen(value) + 1);
                    if(NULL != value)
                    {
                        DAFree(value);
                        value = NULL;
                    }
                }
                if(true == (array[iter])->GetValue(array, SEC_ATTRIBUTE_TYPE, &value) )
                {
                    memcpy(info->items[iter]->type, value, strlen(value) + 1);
                    if(NULL != value)
                    {
                        DAFree(value);
                        value = NULL;
                    }
                }
                if(true == (array[iter])->GetValue(array, SEC_ATTRIBUTE_THINGS_NAME, &value) )
                {
                    memcpy(info->items[iter]->name, value, strlen(value) + 1);
                    if(NULL != value)
                    {
                        DAFree(value);
                        value = NULL;
                    }
                }
                if(true == (array[iter])->GetValue(array, SEC_ATTRIBUTE_NUMBER, &value) )
                {
                    memcpy(info->items[iter]->number, value, strlen(value) + 1);
                    if(NULL != value)
                    {
                        DAFree(value);
                        value = NULL;
                    }
                }
                if(true == (array[iter])->GetValue(array, SEC_ATTRIBUTE_VERSION_AVAILABLE, &value) )
                {
                    memcpy(info->items[iter]->newVersionAvailable, value, strlen(value) + 1);
                    if(NULL != value)
                    {
                        DAFree(value);
                        value = NULL;
                    }
                }
            }
            DAFree(array);
            array =NULL;
        }

        if(NULL != value)
        {
            DAFree(value);
            value = NULL;
        }

        // 2. Update Version ID & Approved State
        if(true == targetResource->rep->GetValue(targetResource->rep,
                                          SEC_ATTRIBUTE_VERSION_ID,
                                          &value) )
        {
            memcpy(info->versionId,value, strlen(value) + 1);
            if(NULL != value)
            {
                DAFree(value);
                value = NULL;
            }
        }

        // 2. Update Version ID & Approved State
        if(true == targetResource->rep->GetValue(targetResource->rep,
                                          SEC_ATTRIBUTE_APPROVED_STATE,
                                          &value) )
        {
            memcpy(info->approveState,value, strlen(value) + 1);
            if(NULL != value)
            {
                DAFree(value);
                value = NULL;
            }
        }

        int res = gSetDeviceInfoCB(&info);
        if(res != 1)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Error Occurred while Setting device information (%d).", ehResult);
        }
        else
        {
            ehResult = OC_EH_OK;
        }

        // 3. De-allocated memory of OICDeviceInformation & its items
        for( iter = 0 ; iter < info->itemCnt; iter++)
        {
            DAFree(info->items[iter]);
            info->items[iter] = NULL;
        }
        DAFree(info);
        info = NULL;
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "SET DEVICE Information CB NOT REGISTERED~!!!!");
    }

    return ehResult;
}
#endif
static OCEntityHandlerResult TriggerResetRequest(OicResource* targetResource, bool reset)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    OicResource* cloneResource = NULL;

    THINGS_LOG_V(THINGS_DEBUG, TAG, "==> RESET : %s", (reset == true ? "YES":"NO"));

    if( reset == true )
    {
        int res = -1;

        cloneResource = CloneResourceInst(targetResource);
        res = OICReset((void*)cloneResource, RST_NEED_CONFIRM);

        switch( res )
        {
        case 1:
            THINGS_LOG(THINGS_DEBUG, TAG, "Reset Thread create is success.");
            ehResult = OC_EH_SLOW;
            break;
        case 0 :
            THINGS_LOG(THINGS_INFO, TAG, "Already Run Reset-Process.");
            ehResult = OC_EH_NOT_ACCEPTABLE;
        default :
            ReleaseResourceInst(cloneResource);
            cloneResource = NULL;
            break;
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "reset = %d, So, can not reset start.", reset);
        ehResult = OC_EH_OK;
    }

    return ehResult;
}

static OCEntityHandlerResult TriggerStopAPRequest(OicResource* targetResource, bool stopAP)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    THINGS_LOG_V(THINGS_DEBUG, TAG, "==> STOP SOFT AP : %s",(stopAP == true ? "YES":"NO"));

    if( stopAP == true )
    {
        if(NULL != gStopSoftAPCB)
        {
            if(1 == gStopSoftAPCB(stopAP) )
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Stop Soft AP notified Successfully");
                ehResult = OC_EH_OK;
            }
            else
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Stop Soft AP notified, BUT DENIED");
                ehResult = OC_EH_ERROR;
            }
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "stopAP = %d, So, can not stop AP.", stopAP);
        ehResult = OC_EH_OK;
    }

    return ehResult;
}

static OCEntityHandlerResult TriggerAbortRequest(OicResource* targetResource, ESEnrolleeAbort abortES)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    static pthread_t hThreadAbort = NULL;
//    static pthread_t hThreadAbort[4] = {NULL, };

    THINGS_LOG_V(THINGS_DEBUG, TAG, "==> ABORT Easy Setup : %d",abortES);

    switch(abortES)
    {
    case ABORT_BEFORE_RESET_CONFIRM:        // before Reset Confirm.
    case ABORT_BEFORE_SEC_CONFIRM:          // After Reset Confirm & before Security Confirm.
    case ABORT_BEFORE_DATA_PROVISIONING:    // After Security Confirm
        THINGS_LOG_V(THINGS_DEBUG, TAG, "Forwarding abort-level to DA App.(Level: %d)", abortES);
        ehResult = OICAbort(&hThreadAbort, abortES);
//        ehResult = OICAbort(&hThreadAbort[abortES], abortES);
        break;
    default :
        THINGS_LOG_D(THINGS_DEBUG, TAG, "abortES = %d, So, Don't need any-process", abortES);
        ehResult = OC_EH_OK;
        break;
    }

    return ehResult;
}

static OCEntityHandlerResult SetProvisioningInfo(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    bool reset = false;
    bool stopAP = false;
    int64_t  abortES= 0;

    if( targetResource->rep->GetBValue(targetResource->rep, SEC_ATTRIBUTE_PROV_RESET, &reset) == true )
    {
        ehResult = TriggerResetRequest(targetResource, reset);
    }
    else if(targetResource->rep->GetBValue(targetResource->rep, SEC_ATTRIBUTE_PROV_TERMINATE_AP, &stopAP) == true)
    {
        ehResult = TriggerStopAPRequest(targetResource, stopAP);
	}
    else if(targetResource->rep->GetIValue(targetResource->rep, SEC_ATTRIBUTE_PROV_ABORT, &abortES) == true)
	{
        ehResult = TriggerAbortRequest(targetResource, (ESEnrolleeAbort)abortES);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Get Value is failed.(RESET NOR TERMINATE NOR ABORT)");
        return ehResult;
    }

    return ehResult;
}

#ifndef OCF_RTOS
static OCEntityHandlerResult GetDeviceInformation(OICInfo* target, OICInfo** list, int * listCnt)
{
    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    if(gGetDeviceCB)
    {
        //gGetDeviceCB("0", uri, list, listCnt);
        int res = gGetDeviceCB(target, list, listCnt);
        if(res != 1)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Error Occurred while getting device information (%d).", ehResult);
            ehResult = OC_EH_ERROR;
        }
        else if(list != NULL)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Information found!!!! %d ", *listCnt);
            if(*listCnt > 0)
            {
                ehResult = OC_EH_OK;
            }
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Information not found!!!!");
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "GET ALL DEVICE FUNC CB NOT REGISTERED~!!!!");
    }

    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ehResult;
}

static OCEntityHandlerResult GetContentsMetadataResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    // OICContents ** pConList = NULL;
    int conCnt = 0;
    char* action = NULL;
    OICContents ** pConList = ((OICContents**)DAMalloc( ( sizeof(OICContents*) * MAX_NUM_OICCONTENT ) ));

    if(strstr (targetResource->uri, URI_CONTENTS) != NULL
        && strstr (targetResource->uri, URI_PROVIDER) != NULL )
    {
        if(NULL != gGetProvContents)
        {
            THINGS_LOG_V(THINGS_DEBUG, TAG, "Get contents metadata to provide");
            gGetProvContents(pConList, &conCnt, &action);
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "GET PROVIDER CONTENTS METADATA FUNC CB NOT REGISTERED~!!!!");
        }
    }
    else if(strstr (targetResource->uri, URI_CONTENTS) != NULL
        && strstr (targetResource->uri, URI_RENDERER) != NULL )
    {
        if(NULL != gGetRendContents)
        {
            THINGS_LOG_V(THINGS_DEBUG, TAG, "Get contents metadata which is assigned for rendering");
            gGetRendContents(pConList, &conCnt);
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "GET RENDERING CONTENTS METADATA FUNC CB NOT REGISTERED~!!!!");
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Unknown Resource : %s", targetResource->uri);
    }

    THINGS_LOG_V(THINGS_INFO, TAG, "#[%d] of Contents Metadata retrieved", conCnt);

    if(conCnt > 0)
    {
        OicRepresentation** childRep = (OicRepresentation**)DAMalloc(sizeof(OicRepresentation*)*conCnt);

        for(int iter = 0 ; iter < conCnt ; iter++)
        {

            childRep[iter] = CreateRepresentationInst(NULL);

            if(pConList[iter]->rt != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "rt : \t%s", pConList[iter]->rt );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_RT, pConList[iter]->rt);
            }
            if(pConList[iter]->provider != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "provider : \t%s", pConList[iter]->provider );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_PROVIDER, pConList[iter]->provider);
            }
            if(pConList[iter]->host != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "host : \t%s", pConList[iter]->host );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_HOST, pConList[iter]->host);
            }
            if(pConList[iter]->path != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "path : \t%s", pConList[iter]->path );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_PATH, pConList[iter]->path);
            }
            if(pConList[iter]->query != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "query : \t%s", pConList[iter]->query );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_QUERY, pConList[iter]->query);
            }
            if(pConList[iter]->mimeType != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "mimeType : \t%s", pConList[iter]->mimeType );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_MIME_TYPE, pConList[iter]->mimeType);
            }
            if(pConList[iter]->encoding != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "encoding : \t%s", pConList[iter]->encoding );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_ENCODING, pConList[iter]->encoding);
            }
            if(pConList[iter]->payload != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "payload : \t%s", pConList[iter]->payload );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_PAYLOAD, pConList[iter]->payload);
            }
            if(pConList[iter]->datetime != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "datetime : \t%s", pConList[iter]->datetime );
                childRep[iter]->SetValue(childRep[iter],  SEC_ATTRIBUTE_CONTENTS_DATETIME, pConList[iter]->datetime);
            }
            if(pConList[iter]->transport != NULL)
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "# [%d] of tranports  ", pConList[iter]->transportCnt );
                for(int iter2 = 0 ; iter2 < pConList[iter]->transportCnt ; iter2++)
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "transport [%d] : \t%s", iter2, pConList[iter]->transport[iter2] );
                }
                childRep[iter]->SetSArrayValue(childRep[iter],
                                                SEC_ATTRIBUTE_CONTENTS_TRANSPORT,
                                                pConList[iter]->transportCnt,
                                                pConList[iter]->transport);
            }
        }

        targetResource->rep->SetArrayValue(targetResource->rep,
                                           SEC_ATTRIBUTE_CONTENTS,
                                           conCnt,
                                           childRep );

        /*! Added by DA for memory Leak fix
         */
        for(int iter = 0 ; iter < conCnt ; iter++) {
          ReleaseRepresentationInst(childRep[iter]);
          childRep[iter] = NULL;
        }
        if(childRep) DAFree(childRep);

        if(NULL != action)
        {
            THINGS_LOG_V(THINGS_DEBUG, TAG, "ACTION TYPE : %s", action);
            targetResource->rep->SetValue(targetResource->rep,
                                      SEC_ATTRIBUTE_CONTENTS_ACTION,
                                      action);
        }


        ehResult = OC_EH_OK;
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata to share");
    }

    if(pConList)
    {
        for(int iter3 = 0 ; iter3 < conCnt ; iter3++)
        {
            DAFree(pConList[iter3]->rt);
            DAFree(pConList[iter3]->provider);
            DAFree(pConList[iter3]->host);
            DAFree(pConList[iter3]->path);
            DAFree(pConList[iter3]->query);
            DAFree(pConList[iter3]->mimeType);
            DAFree(pConList[iter3]->encoding);
            DAFree(pConList[iter3]->payload);
            DAFree(pConList[iter3]->datetime);
            for(int iter4 = 0 ; iter4 < pConList[iter3]->transportCnt ; iter4++)
            {
                DAFree(pConList[iter3]->transport[iter4]);
            }
        }
        DAFree(pConList);
        pConList = NULL;
    }

    return ehResult;
}

static OCEntityHandlerResult SetContentsRendererMetadataResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(NULL != gSetRendContents)
    {
        int conCnt = 0;
        OicRepresentation ** childRep = NULL;
        char* actionType = NULL;
        char* capaType = NULL;

        if(true != targetResource->rep->GetValue(targetResource->rep, SEC_ATTRIBUTE_CONTENTS_ACTION, &actionType) )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_ACTION)");
        }
        else
        {
            if(true == targetResource->rep->GetArrayValue(targetResource->rep,
                                                        SEC_ATTRIBUTE_CONTENTS,
                                                        &conCnt,
                                                        &childRep) )
            {
                THINGS_LOG_V(THINGS_INFO, TAG, "#[%d] of Contents Metadata received", conCnt);

                OICContents** pConList = (OICContents**)DAMalloc(sizeof(OICContents*)*conCnt);

                for(int iter = 0 ; iter < conCnt ; iter++)
                {
                    pConList[iter] = (OICContents*)DAMalloc(sizeof(OICContents));
                    pConList[iter]->rt = NULL;
                    pConList[iter]->provider = NULL;
                    pConList[iter]->host = NULL;
                    pConList[iter]->path = NULL;
                    pConList[iter]->query = NULL;
                    pConList[iter]->mimeType = NULL;
                    pConList[iter]->encoding = NULL;
                    pConList[iter]->payload = NULL;
                    pConList[iter]->datetime = NULL;
                    pConList[iter]->transport = NULL;
                    pConList[iter]->transportCnt = 0;

                    // if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_RT, &pConList[iter]->rt) )
                    // {
                    //     THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_RT) ");
                    // }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_PROVIDER, &pConList[iter]->provider))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_PROVIDER)");
                    }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_HOST, &pConList[iter]->host))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_HOST)");
                    }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_PATH, &pConList[iter]->path))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_PATH)");
                    }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_QUERY, &pConList[iter]->query))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_QUERY)");
                    }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_MIME_TYPE, &pConList[iter]->mimeType))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_MIME_TYPE)");
                    }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_ENCODING, &pConList[iter]->encoding))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_ENCODEING)");
                    }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_PAYLOAD, &pConList[iter]->payload))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_PAYLOAD)");
                    }
                    if(true != childRep[iter]->GetValue(childRep[iter], SEC_ATTRIBUTE_CONTENTS_DATETIME, &pConList[iter]->datetime))
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_DATETIME)");
                    }
                    if(true != childRep[iter]->GetSArrayValue(childRep[iter],
                                                   SEC_ATTRIBUTE_CONTENTS_TRANSPORT,
                                                   &pConList[iter]->transportCnt,
                                                   &pConList[iter]->transport) )
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_TRANSPORT)");
                    }

                    // for testing
                    if(pConList[iter]->rt != NULL)          {THINGS_LOG_V(THINGS_DEBUG, TAG, "rt : \t%s", pConList[iter]->rt );}
                    if(pConList[iter]->provider != NULL)    {THINGS_LOG_V(THINGS_DEBUG, TAG, "provider : \t%s", pConList[iter]->provider );}
                    if(pConList[iter]->host != NULL)        {THINGS_LOG_V(THINGS_DEBUG, TAG, "host : \t%s", pConList[iter]->host );}
                    if(pConList[iter]->path != NULL)        {THINGS_LOG_V(THINGS_DEBUG, TAG, "path : \t%s", pConList[iter]->path );}
                    if(pConList[iter]->query != NULL)       {THINGS_LOG_V(THINGS_DEBUG, TAG, "query : \t%s", pConList[iter]->query );}
                    if(pConList[iter]->mimeType != NULL)    {THINGS_LOG_V(THINGS_DEBUG, TAG, "mimeType : \t%s", pConList[iter]->mimeType );}
                    if(pConList[iter]->encoding != NULL)    {THINGS_LOG_V(THINGS_DEBUG, TAG, "encoding : \t%s", pConList[iter]->encoding );}
                    if(pConList[iter]->payload != NULL)     {THINGS_LOG_V(THINGS_DEBUG, TAG, "payload : \t%s", pConList[iter]->payload );}
                    if(pConList[iter]->datetime != NULL)     {THINGS_LOG_V(THINGS_DEBUG, TAG, "datetime : \t%s", pConList[iter]->datetime );}
                    if(pConList[iter]->transport != NULL)
                    {
                        THINGS_LOG_V(THINGS_DEBUG, TAG, "# [%d] of tranports  ", pConList[iter]->transportCnt );
                        for(int iter2 = 0 ; iter2 < pConList[iter]->transportCnt ; iter2++)
                        {
                            THINGS_LOG_V(THINGS_DEBUG, TAG, "transport [%d] : \t%s", iter2, pConList[iter]->transport[iter2] );
                        }
                    }
                }

                if(true != targetResource->rep->GetValue(targetResource->rep, SEC_ATTRIBUTE_CONTENTS_CAPA_TYPE, &capaType) )
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_CAPA_TYPE)");
                }
                if(true != targetResource->rep->GetValue(targetResource->rep, SEC_ATTRIBUTE_CONTENTS_ACTION, &actionType) )
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_ACTION)");
                }

                if(gSetRendContents(pConList, conCnt, actionType, capaType))
                {
                    ehResult = OC_EH_OK;
                }

                if(pConList)
                {
                    for(int iter2 = 0 ; iter2 < conCnt ; iter2++)
                    {
                        DAFree(pConList[iter2]->rt);
                        DAFree(pConList[iter2]->provider);
                        DAFree(pConList[iter2]->host);
                        DAFree(pConList[iter2]->path);
                        DAFree(pConList[iter2]->query);
                        DAFree(pConList[iter2]->mimeType);
                        DAFree(pConList[iter2]->encoding);
                        DAFree(pConList[iter2]->payload);
                        DAFree(pConList[iter2]->datetime);
                        for(int iter3 = 0 ; iter3 < pConList[iter2]->transportCnt ; iter3++)
                        {
                            DAFree(pConList[iter2]->transport[iter3]);
                        }
                    }
                    DAFree(pConList);
                    pConList = NULL;
                }
            }
            else
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata deilvered");
            }
            if(capaType)
            {
                DAFree(capaType);
                capaType = NULL;
            }
            if(actionType)
            {
                DAFree(actionType);
                actionType = NULL;
            }
        }

        /*! Added by DA for memory Leak fix
         */
        for(int iter2 = 0 ; iter2 < conCnt ; iter2++)
        {
           ReleaseRepresentationInst(childRep[iter2]);
           childRep[iter2] = NULL;
        }
        if(childRep) DAFree(childRep);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Callback for Setting RENDERER CONTENTS METADATA NOT REGISTERED~!!!!");
    }

    return ehResult;
}


static OCEntityHandlerResult SetDeviceMaintenanceResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(gSetMaintenanceCB)
    {
        bool factoryReset = false;
        bool collectLogs = false;

        OICMaintenance value;
        value.fr = -1;
        value.collectLog = -1;
        value.logIds = NULL;
        value.idCnt = 0;


        if(true == targetResource->rep->GetBValue(targetResource->rep,
                                                  OIC_ATTRIBUTE_RESET_FACTORY,
                                                  &factoryReset) )
        {
            value.fr = (factoryReset == false ? 0 : 1);
        }

        if(true == targetResource->rep->GetBValue(targetResource->rep,
                                                  SEC_ATTRIBUTE_LOG_COLLECT,
                                                  &collectLogs) )
        {
            value.collectLog = (collectLogs == false ? 0 : 1);
        }

        if(1 == gSetMaintenanceCB(&value))
        {

            if(NULL != value.logIds
                && 0 < value.idCnt )
            {
                targetResource->rep->SetIArrayValue(targetResource->rep,
                                                    SEC_ATTRIBUTE_LOG_IDS,
                                                    value.idCnt,
                                                    value.logIds);

                if(value.logIds != NULL)
                {
                    DAFree(value.logIds);
                    value.logIds = NULL;
                }

                ehResult = OC_EH_OK;
            }
            else
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Lod Ids or Cnt[%d] Array Address : %x ", value.idCnt, value.logIds);
            }
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid device maintenace data received");
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Callback for Setting Set Device Maintenance NOT REGISTERED~!!!!");
    }

    return ehResult;
}

static OCEntityHandlerResult SetBixbyRequestResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    char* target = NULL;
    char* value = NULL;

    if(gBixbyHandleCB)
    {
        if(true != targetResource->rep->GetValue(targetResource->rep, SEC_ATTRIBUTE_COMMON_TARGET, &target) )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_ACTION)");
        }
        else
        {
            if(true != targetResource->rep->GetValue(targetResource->rep, SEC_ATTRIBUTE_COMMON_VALUE, &value) )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No contents metadata (SEC_ATTRIBUTE_CONTENTS_ACTION)");
            }
            else
            {
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Target : %s, Value : %s", target, value);

                if( (target != NULL && strlen(target) > 0)
                    && (value != NULL && strlen(value) > 0) )
                {
                    if( 1 == gBixbyHandleCB(target,value) )
                    {
                        ehResult = OC_EH_OK;
                    }
                }
            }
        }

        /*! Added by DA for memory Leak fix
         */
        if(target) free(target);
        if(value) free(value);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Callback for Bixby");
    }

    return ehResult;
}

static OCEntityHandlerResult GetDeviceMaintenanceResult(OicResource * targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(gGetMaintenanceCB)
    {
        OICMaintenance value;
        value.fr = -1;
        value.collectLog = -1;
        value.logIds = NULL;
        value.idCnt = 0;

        if(1 == gGetMaintenanceCB(&value) )
        {
            if(value.fr > -1 && value.collectLog > -1)
            {
                targetResource->rep->SetBValue(targetResource->rep,
                                               OIC_ATTRIBUTE_RESET_FACTORY,
                                               (value.fr == 0 ? false:true) );

                targetResource->rep->SetBValue(targetResource->rep,
                                               SEC_ATTRIBUTE_LOG_COLLECT,
                                               ( value.collectLog == 0 ? false:true) );
                if(NULL != value.logIds
                    && 0 < value.idCnt )
                {
                    targetResource->rep->SetIArrayValue(targetResource->rep,
                                                        SEC_ATTRIBUTE_LOG_IDS,
                                                        value.idCnt,
                                                        value.logIds);
                }

                if(value.logIds != NULL)
                {
                    DAFree(value.logIds);
                    value.logIds = NULL;
                }

                THINGS_LOG_V(THINGS_DEBUG, TAG, "Is Logging? [%d]", value.collectLog);
                if(0 < value.collectLog
                    && NULL != value.logIds) THINGS_LOG_V(THINGS_DEBUG, TAG, "File ID? [%d]", value.logIds[0]);

                ehResult = OC_EH_OK;
            }
            else
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid device maintenace data received");
            }
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to get device maintenace data");
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Callback for Setting Get Device Maintenance NOT REGISTERED~!!!!");
    }

    return ehResult;
}
#endif

static OCEntityHandlerResult GetDeviceOrPlatformInfoResult(OicResource * targetResource, char* deviceID)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(strlen(deviceID) > 0
        && isNumber(deviceID) )
    {
        int num = atoi(deviceID);

        // 1. Get the resource with uri
        OicResource * resource = DM_GetResourceInstance(targetResource->uri, num);
        if(resource)
        {
            // 2. Clone the Payload of the resource
            OCPayloadDestroy((OCPayload*)targetResource->rep->payload);
            targetResource->rep->payload = NULL;
            targetResource->rep->payload = OCRepPayloadClone(resource->rep->payload);

            ehResult = OC_EH_OK;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Device ID : %s", deviceID);
    }


    return ehResult;
}

#ifndef OCF_RTOS
static void SetCommonPropertiesIntoPayload(OicResource * targetResource)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    OicResource * resource = gBuilder->GetResource(gBuilder, targetResource->uri);
    if(resource != NULL)
    {
        if(NULL != targetResource->rep)
        {
            int rtnum = resource->GetNumOfResTypes(resource);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "# of RT %d", (int)rtnum);
            for(int index = 0 ; index < rtnum ; index++)
            {
                OCRepPayloadAddResourceType( (OCRepPayload*)(targetResource->rep->payload), resource->GetResType(resource, index) );
            }

            int ifnum = resource->GetNumOfInfTypes(resource);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "# of IF %d", (int)rtnum);
            for(int index = 0 ; index < ifnum ; index++)
            {
                OCRepPayloadAddInterface( (OCRepPayload*)(targetResource->rep->payload), resource->GetInfType(resource, index) );
            }
        }
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

static void SetCommonPropertiesIntoPayload2(void * hd,
                const char* uri, OicRepresentation * rep, OCDevAddr *devAddr)
{
    uint8_t rtnum = 0;
    uint8_t itnum = 0;
    uint8_t index = 0;

    if(hd != NULL
        && uri != NULL
        && rep != NULL)
    {
        // 1. Get/Set Uri of child resource
        rep->SetValue(rep, OC_RSRVD_HREF, uri);

        // 2. Get/Set IT of child resource
        OCGetNumberOfResourceInterfaces((OCResourceHandle)hd, &itnum);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "# of IT %d", (int)itnum);

        char * its[itnum];
        for(index = 0 ; index < itnum ; index++)
        {
            its[index] = OCGetResourceInterfaceName( (OCResourceHandle)hd, index);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "#[%d] of IT %s", index, its[index]);
            if(its[index] != NULL
                && strlen(its[index]) < 1)
            {
                its[index] = NULL;
            }
        }
        rep->SetSArrayValue(rep,
                            OC_RSRVD_INTERFACE,
                            itnum,
                            its);

        // 3. Get/Set RT of child resource
        OCGetNumberOfResourceTypes((OCResourceHandle)hd, &rtnum);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "# of RT %d", (int)rtnum);

        char * rts[rtnum];
        for(index = 0 ; index < rtnum ; index++)
        {
            rts[index] = OCGetResourceTypeName( (OCResourceHandle)hd, index);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "#[%d] of RT %s", index, rts[index]);
            if(rts[index] != NULL
                && strlen(rts[index])  < 1 )
            {
                rts[index] = NULL;
            }
        }
        rep->SetSArrayValue(rep,
                            OC_RSRVD_RESOURCE_TYPE,
                            rtnum,
                            rts);


        //// Create payload about the Policy Property("p").
        // 1. Get Resource Property using the resource handle.
        OCResourceProperty  p = OCGetResourceProperties((OCResourceHandle)hd);
        // 2. Create new Payload to set the properties.
        OicRepresentation   *policy = CreateRepresentationInst(NULL);

        if(policy)
        {
            // 3. Set the "bm".
            policy->SetIValue(policy, OC_RSRVD_BITMAP,
                                    ((p & OC_DISCOVERABLE)
                                            | (p & OC_OBSERVABLE)));

            if (p & OC_SECURE)
            {   // if it is secured resource.
                // 3-1. Set the "sec".
                // THINGS_LOG_D(THINGS_DEBUG, TAG, "Dev Adapter Type : %s",
                //                 (devAddr.adapter == OC_ADAPTER_IP) ?
                //                     "IP" : "NON-IP");
                policy->SetBValue(policy, OC_RSRVD_SECURE, p & OC_SECURE);
                uint16_t securePort = 0;
                if(NULL != devAddr)
                {
                    if (devAddr->adapter == OC_ADAPTER_IP)
                    {
                        if (devAddr->flags & OC_IP_USE_V6)
                        {
                            securePort = caglobals.ip.u6s.port;
                            THINGS_LOG_D(THINGS_DEBUG, TAG, "Use IPv6.");
                        }
                        else if (devAddr->flags & OC_IP_USE_V4)
                        {
                            securePort = caglobals.ip.u4s.port;
                            THINGS_LOG_D(THINGS_DEBUG, TAG, "Use IPv4.");
                        }
                    }
                }
                // 3-2. Set the "port".
                policy->SetIValue(policy, OC_RSRVD_HOSTING_PORT, securePort);
            } // (p & OC_SECURE)

            // 4. Set "p" payload(object) to parent payload.
            rep->SetObjectValue(rep, OC_RSRVD_POLICY, policy);
            ReleaseRepresentationInst(policy);
        }
        else
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG,
                "Faild to get Resource Policy. Please Check Resource Handle");
        }

    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Handle or OicRepresentation");
    }
}

OICInfo* CreateOicInfo(void* requestHandle, void* resourceHandle)
{
    OICInfo* target = (OICInfo*) DAMalloc( sizeof(OICInfo) * 1 );

    if(NULL != target)
    {
        memset(target->deviceId,  0 , MAX_DEVICEID_LEN);
        memset(target->resource,  0 , MAX_RESOURCE_LEN);
        memset(target->query,     0 , MAX_QUERY_LEN);
        memset(target->attribute, 0 , MAX_ATTRIBUTE_LEN);
        memset(target->type,      0 , MAX_VALUETYPE_LEN);
        memset(target->value,     0 , MAX_ATTRVALUE_LEN);
        memset(target->id,        0 , MAX_DEVICEID_LEN);

        target->resourceHandle = resourceHandle;
        target->requestHandle = requestHandle;
    }

    return target;
}

static OCEntityHandlerResult GetResourceData(OicResource* targetResource)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    char* spliter[3] = {";","?","#"};
    bool isMultiQuery = false;

    OICInfo target;
    OICInfo** pList = NULL;
    int listCnt = 0;

    // Initialize data structrue
    memset(target.deviceId,  0, MAX_DEVICEID_LEN);
    memset(target.resource,  0, MAX_RESOURCE_LEN);
    memset(target.query,     0, MAX_QUERY_LEN);
    memset(target.attribute, 0, MAX_ATTRIBUTE_LEN);
    memset(target.type,      0, MAX_VALUETYPE_LEN);
    memset(target.value,     0, MAX_ATTRVALUE_LEN);
    memset(target.id,        0, MAX_DEVICEID_LEN);

    if(NULL == targetResource->uri)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "No Target Resource URI ");
        return OC_EH_ERROR;
    }

    RemoveQuery(targetResource->query, target.query, ";", "if=");
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Received Query After RemoveInterfaceQuery : %s", targetResource->query);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "OICInfo target Query : %s", target.query);

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    char* deviceNum = NULL;
    char tempURI[MAX_RESOURCE_LEN] = { 0 };

    deviceNum = strrchr(targetResource->uri, '/');

    // 1. Temperature Resource type Sync ( Vender <--> OCF )
    if(strstr(targetResource->uri,URI_TEMPERATURE) != NULL)
    {

        strncat(DAStrncpy(tempURI, URI_TEMPERATURES, strlen(URI_TEMPERATURES)), URI_VENDER_SPECIFIC, strlen(URI_VENDER_SPECIFIC));

        strncat(tempURI,deviceNum,strlen(deviceNum));

        DAStrncpy(target.resource, tempURI, strlen(tempURI) );


        if(strstr(targetResource->uri,URI_FRIDGE_FREEZER) != NULL
            || strstr(targetResource->uri,URI_COOK) != NULL)
        {
            if(0 != strlen(target.query))
            {
                strncat(target.query,"?",1);
            }
            strncat(target.query,"id=0",strlen("id=0"));
        }
        else if(strstr(targetResource->uri,URI_FRIDGE_COOLER) != NULL
            || strstr(targetResource->uri,URI_OVEN_PROBE) != NULL)
        {
            if(0 != strlen(target.query))
            {
                strncat(target.query,"?",1);
            }
            strncat(target.query,"id=1",strlen("id=1"));
        }
        else if(strstr(targetResource->uri,URI_FRIDGE_CVROOM) != NULL)
        {
            if(0 != strlen(target.query))
            {
                strncat(target.query,"?",1);
            }
            strncat(target.query,"id=2",strlen("id=2"));
        }
    }
    // 2. Sensor Resource type Sync ( Vender <--> OCF )
    else if(strstr(targetResource->uri,URI_SENSOR) != NULL)
    {

        strncat(DAStrncpy(tempURI, URI_SENSOR, strlen(URI_SENSOR)), URI_VENDER_SPECIFIC, strlen(URI_VENDER_SPECIFIC));

        strncat(tempURI,deviceNum,strlen(deviceNum));

        DAStrncpy(target.resource, tempURI, strlen(tempURI) );
    }
    // 3. Door Resource type Sync ( Vender <--> OCF )
    else if(strstr(targetResource->uri,URI_DOOR) != NULL)
    {
        strncat(DAStrncpy(tempURI, URI_DOORS, strlen(URI_DOORS)), URI_VENDER_SPECIFIC, strlen(URI_VENDER_SPECIFIC));

        strncat(tempURI,deviceNum,strlen(deviceNum));

        DAStrncpy(target.resource, tempURI, strlen(tempURI) );


        if(strstr(targetResource->uri,URI_FRIDGE_FREEZER) != NULL)
        {
            if(0 != strlen(target.query))
            {
                strncat(target.query,"?",1);
            }
            strncat(target.query,"id=0",strlen("id=0"));
        }
        else if(strstr(targetResource->uri,URI_FRIDGE_COOLER) != NULL)
        {
            if(0 != strlen(target.query))
            {
                strncat(target.query,"?",1);
            }
            strncat(target.query,"id=1",strlen("id=1"));
        }
        else if(strstr(targetResource->uri,URI_FRIDGE_CVROOM) != NULL)
        {
            if(0 != strlen(target.query))
            {
                strncat(target.query,"?",1);
            }
            strncat(target.query,"id=2",strlen("id=2"));
        }
    }
    // 4. Device (Collection) Resource type Handling
    else if(strstr(targetResource->uri, URI_DEVICE_COL) != NULL
        && targetResource->IsSupportingResourceType(targetResource, SEC_RTYPE_THINGS_DEVICE) )
    {
        if( strstr(targetResource->query, OC_RSRVD_INTERFACE_BATCH) != NULL
            && strstr(targetResource->query, OC_RSRVD_INTERFACE_DEFAULT ) == NULL )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Batch Get on Collection Resource");

            DAStrncpy(target.resource, targetResource->uri, strlen(targetResource->uri) );
        }

        else if( strstr(targetResource->query, "if=") == NULL
            ||  strstr(targetResource->query, OC_RSRVD_INTERFACE_DEFAULT ) != NULL
            ||  strstr(targetResource->query, OC_RSRVD_INTERFACE_LL ) != NULL )
        {
            if( strstr(targetResource->query, OC_RSRVD_INTERFACE_DEFAULT ) != NULL )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Baseline Get on Collection Resource");

                // SetCommonPropertiesIntoPayload(targetResource);
            }
            else
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Link-List Get on Collection Resource");
            }
            // THINGS_LOG_D(THINGS_DEBUG, TAG, "Request with query %s", targetResource->query);

            if(deviceNum == NULL)
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No Device Number Extracted~!!!!!");
            }
            else
            {
                int index = 0;
                int foundResouces = 0;
                int devNum = -1;
                char id[MAX_QUERY_LEN] = {0};

                memcpy(id, &deviceNum[1], strlen(deviceNum));

                if(strlen(id) > 0
                    && isNumber(id) )
                {
                    devNum = atoi(id);

                    int childrenNum = DM_GetNumofChildren(devNum);
                    if(childrenNum > 0)
                    {

                        OicRepresentation * childrenResourceRep[childrenNum];

                        for(index = 0 ; index < childrenNum ; index++)
                        {
                            memset(tempURI, 0,MAX_RESOURCE_LEN);

                            const char* uri = DM_GetResourceUri(devNum, index);

                            if(uri != NULL && strlen(uri) > 0)
                            {
                                strncat(tempURI, uri, strlen(uri));
                                strncat(tempURI, deviceNum, strlen(deviceNum));

                                THINGS_LOG_D(THINGS_DEBUG, TAG, "#[ %d ] Child Resource : %s", index, tempURI);

                                OicResource * temp = gBuilder->GetResource(gBuilder, tempURI);

                                if(temp)
                                {

                                    childrenResourceRep[foundResouces] = CreateRepresentationInst(NULL);
                                    SetCommonPropertiesIntoPayload2(
                                        temp->resourceHandle,
                                        tempURI,
                                        childrenResourceRep[foundResouces],
                                        targetResource->GetDevAddr(targetResource));

                                    foundResouces++;

                                }
                                else
                                {
                                   THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No such resource exist in the ServerBuilder");
                                }
                            }
                        }
                        THINGS_LOG_V(THINGS_DEBUG, TAG, "Adding [ %d vs %d ] number of link information", foundResouces, childrenNum);

                        bool res = targetResource->rep->SetArrayValue(targetResource->rep,
                                                                    OC_RSRVD_LINKS,
                                                                    foundResouces,
                                                                    childrenResourceRep);

                        if(res == true )
                        {
                            ehResult = OC_EH_OK;

                        }
                        else
                        {
                            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to Set Links information~!!!!");
                        }

                        for(index = 0 ; index < childrenNum ; index++)
                        {
                            ReleaseRepresentationInst(childrenResourceRep[index]);
                            childrenResourceRep[index] = NULL;
                        }
                    }
                    else
                    {
                        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No children on this Collection Resource");
                    }
                }
                else
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to get device number from the URI");
                }
            }

            goto EXIT_GET_HANDLEING;
        }
        else
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Get on URI : %s with query : %s", targetResource->uri, targetResource->query);
            goto EXIT_GET_HANDLEING;
        }
    }
    else
    {
        DAStrncpy(target.resource, targetResource->uri, strlen(targetResource->uri) );
    }

    // Query COPY
    // [Jay] The query will be used for retrieving object array type resource attribute values..
    //       for those resource with simple data type will used index=0 by default.
    //       ex) get /tempereatures with "id=0" ==> cooler temperature
    //           get /tempereatures with "id=1" ==> freezer temperature
    //           get /sensors with "id=0"       ==> pm 10 sensor
    //           get /sensors with "id=1"       ==> pm 22.5 sensor
    //           get /sensors with "id=2"       ==> pm gas sensor
    //           get /sensors with "id=3"       ==> pm total sensor

    THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Get on URI : %s with query : %s", target.resource, target.query);
    pList = (OICInfo**)DAMalloc(sizeof(OICInfo*) *  MAX_NUM_OICINFO);  // MAX_NUM_OICINFO == 200
    ehResult = GetDeviceInformation(&target, pList, &listCnt);
    if(OC_EH_OK == ehResult)
    {
        // If Get Device information succeed
        for(int iter = 0 ; iter < listCnt ; iter++)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Device ID : %s", pList[iter]->deviceId);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "ID : %s", pList[iter]->id);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "URI : %s", pList[iter]->resource);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr Name : %s", pList[iter]->attribute);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr. Value : %s", pList[iter]->value);
        }

        ehResult = gConvertResource(targetResource, pList, listCnt);

        if(pList != NULL)
        {
            for(int i = 0; i< listCnt;i++)
            {
                DAFree(pList[i]);
            }
            DAFree(pList);
        }
    }
    else
    {
        // If get device failed..
        OicResource* temp = gBuilder->GetResource(gBuilder, targetResource->uri);
        const char* rt = temp->GetResType(temp, 0);

        // In the OCF, Temperature resource types has its only payload format for error returning..
        if (0 == strcmp(rt, SEC_RTYPE_TEMPERATURE) || 0 == strcmp(rt, OIC_RTYPE_TEMPERATURE))
        {
            if(NULL != strstr(target.query,"units="))
            {

                char tempStr[100] = {0,};
                memcpy(tempStr, target.query, MAX_QUERY_LEN);
                memset(target.query, 0 , MAX_QUERY_LEN);

                for(int iter = 0 ; iter < 3 ; iter++)
                {
                    if(NULL != strstr(tempStr, spliter[iter]) )
                    {
                        THINGS_LOG_D(THINGS_DEBUG, TAG, "Query Spliter is : %s", spliter[iter]);
                        RemoveQuery(tempStr, target.query, spliter[iter],"units=");

                        break;
                    }
                }

                THINGS_LOG_D(THINGS_DEBUG, TAG, "After RemoveUnitsQuery Query : %s", target.query);
            }

            if(OC_EH_OK == GetDeviceInformation(&target, pList, &listCnt))
            {
                for(int iter = 0 ; iter < listCnt ; iter++)
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Device ID : %s", pList[iter]->deviceId);
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "ID : %s", pList[iter]->id);
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "URI : %s", pList[iter]->resource);
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr Name : %s", pList[iter]->attribute);
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr. Value : %s", pList[iter]->value);
                }
                ehResult = gConvertResource(targetResource, pList, listCnt);

                if(ehResult != OC_EH_OK)
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "OICInfo Resource Convert Failed: (%d)", ehResult);
                }
            }

            if(pList != NULL)
            {
                for(int i = 0; i< listCnt;i++)
                {
                    DAFree(pList[i]);
                }
                DAFree(pList);
            }

            targetResource->SetError(targetResource, OC_EH_ERROR);
            ehResult = OC_EH_ERROR;
        }
    }

    // THINGS_LOG_D(THINGS_DEBUG, TAG, "Request with query %s", targetResource->query);
    //if(strstr(targetResource->query, "if=oic.if.baseline") != NULL)
    if(strstr(targetResource->query, "if=") != NULL
        && strstr(targetResource->query, OIC_INTERFACE_BATCH) != NULL)
    {
        // THINGS_LOG_D(THINGS_DEBUG, TAG, "\tAppend Common Properties.");
        OicResource* childResource = targetResource->Next(targetResource);
        for(int iter = 1; iter < targetResource->size; ++iter)
        {
            if(childResource != NULL)
            {
                // SetCommonPropertiesIntoPayload(childResource);
                OicResource* temp = gBuilder->GetResource(gBuilder, childResource->uri);
                if(temp != NULL)
                {
                    OicRepresentation * rep = childResource->rep;
                    childResource->rep = CreateRepresentationInst(NULL);
                    childResource->rep->SetObjectValue(childResource->rep, OC_RSRVD_REPRESENTATION, rep);

                    childResource->resourceHandle = temp->resourceHandle;
                    childResource->SetUri(childResource, temp->uri);
                    childResource = childResource->Next(childResource);
                }
                else
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "\t\tResource(%s) wasn't registered.",childResource->uri);
                }
            }
        }
    }

EXIT_GET_HANDLEING:

    return ehResult;
}

static OCEntityHandlerResult PostResourceData(OicResource** targetRes)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    int listCnt = 0;
    int stateListCnt = 0;
    char* tempURI = NULL;
    OICInfo     target;
    OICInfo**    pList = NULL;
    OICInfo**    pStateList = NULL;

    OicResource* targetResource = *targetRes;
    // 1. TODO : Transform OicResource -> List of OICInfo
    // Initialize data structrue
    memset(target.deviceId,  0, MAX_DEVICEID_LEN);
    memset(target.resource,  0, MAX_RESOURCE_LEN);
    memset(target.query,     0, MAX_QUERY_LEN);
    memset(target.attribute, 0, MAX_ATTRIBUTE_LEN);
    memset(target.type,      0, MAX_VALUETYPE_LEN);
    memset(target.value,     0, MAX_ATTRVALUE_LEN);

    if(NULL != targetResource->uri)
    {
        DAStrncpy(target.resource, targetResource->uri, MAX_RESOURCE_LEN - 1 );
    }
    RemoveQuery(targetResource->query, target.query, ";","if=");
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Get on URI : %s", target.resource);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Query : %s", target.query);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Requested Query : %s", targetResource->query);


    pList = (OICInfo**)DAMalloc(sizeof(OICInfo*) * MAX_NUM_OICINFO);  // MAX_NUM_OICINFO == 200

    if(strstr(targetResource->uri,URI_DEVICE_COL) != NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "URI has URI_DEVICE_COL");
        if(NULL != strstr(targetResource->query, "if="))
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "query has interface");
            if(NULL != strstr(targetResource->query, OC_RSRVD_INTERFACE_BATCH))
            {
                THINGS_LOG_ERROR(THINGS_ERROR, TAG, "query has batch interface");

                OicResource* res = targetResource;
                OicRepresentation *tRep = NULL;
                OicRepresentation *tChildRep = NULL;

                for(int iter = 0; iter < targetResource->size; ++iter)
                {
                    tChildRep = CreateRepresentationInst(NULL);

                    res->GetRepresentation(res, &tRep);

                    tRep->GetObjectValue(tRep, OC_RSRVD_REPRESENTATION, tChildRep);

                    res->SetRepresentation(res,tChildRep);

                    res = res->Next(res);

                }


            }
            else
            {
                targetResource->SetError(targetResource, OC_EH_ERROR);
                ehResult = OC_EH_ERROR;
                THINGS_LOG_D(THINGS_DEBUG, TAG, "\t Wrong query. It is collection post request. But query does not have type of batch interface.   : (%d)", ehResult);
                goto EXIT_POST_HANDLEING;
            }

        }
        else
        {
            targetResource->SetError(targetResource, OC_EH_ERROR);
            ehResult = OC_EH_ERROR;
            THINGS_LOG_D(THINGS_DEBUG, TAG, "\t Does not have query. It is collection post request. But query does not have type of interface.   : (%d)", ehResult);
            goto EXIT_POST_HANDLEING;
        }
    }

    if(OC_EH_OK != gConvertRequestInfo(targetResource, pList, &listCnt))
    {
        //return OC_EH_ERROR;
        ehResult = OC_EH_ERROR;
        goto EXIT_POST_HANDLEING;
    }

    for(int iter = 0 ; iter < listCnt ; iter++)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "ID : %s", pList[iter]->deviceId);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "URI : %s", pList[iter]->resource);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr Name : %s", pList[iter]->attribute);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr. Value : %s", pList[iter]->value);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Query : %s", pList[iter]->query);
    }

    // 2. Deliver List of OICInfo with Request Type(POST) to App.
    if(1 == gHandlerCB(targetResource->reqType, pList, listCnt, 0, "1") )
    {
        ehResult = OC_EH_OK;
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Post Processing Result : (%d)", ehResult);
        pStateList = pList;
        stateListCnt = listCnt;
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Post Processing Result : (%d)", ehResult);
        if(NULL != strstr(targetResource->GetResType(targetResource, 0),
                                OIC_RTYPE_TEMPERATURE))
        {
            if(OC_EH_OK == GetDeviceInformation(&target, pStateList, &stateListCnt))
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "\t Result of Get Temperature  : (%d)", ehResult);
            }
        }
        else
        {
            pStateList = pList;
            stateListCnt = listCnt;
        }

        targetResource->SetError(targetResource, OC_EH_ERROR);
        ehResult = OC_EH_ERROR;
        THINGS_LOG_D(THINGS_DEBUG, TAG, "\t Error Result of Get Temperature  : (%d)", ehResult);
    }

    // 3. Check the iterface type in the query.
    if(NULL != targetResource->query)
    {

        //if(NULL != strstr(targetResource->query, "if=oic.if.baseline"))
        if(NULL != strstr(targetResource->query, "if=")
            && NULL != strstr(targetResource->query, OC_RSRVD_INTERFACE_BATCH))
        {

            if(strstr(targetResource->uri,URI_DEVICE_COL) != NULL)
            {
                OicResource* parentResource = targetResource;
                OicResource* deviceResource = CreateResourceInst(
                                            targetResource->requestHandle,
                                            targetResource->resourceHandle,
                                            targetResource->query,
                                            NULL);

                OicRepresentation* deviceRep = CreateRepresentationInst(NULL);

                deviceResource->SetRepresentation(deviceResource, deviceRep);

                THINGS_LOG_D(THINGS_DEBUG, TAG, "### Get child Resources size = %d ###", targetResource->size);
                for (int i = 0; i < targetResource->size; i++)
                {
                    //tempURI = NULL;
                    if(NULL == parentResource)
                    {
                        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "No More Children~!!!!!!!!");
                        break;
                    }
                    //parentResource->GetUri(parentResource,&tempURI);

                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Get child Resources URI = %s", parentResource->uri);//tempURI);
                    if(NULL != parentResource->uri)//tempURI)
                    {
                        //if(strstr(tempURI,URI_DEVICE_COL) != NULL)
                        if(strstr(parentResource->uri, URI_DEVICE_COL) != NULL)
                        {
                            THINGS_LOG(THINGS_DEBUG, TAG, "Device URI~!!!!!!!!");
                            parentResource = parentResource->Next(parentResource);
                            continue;
                        }
                    }

                    OicResource* temp = gBuilder->GetResource(gBuilder, parentResource->uri);
                    OicResource* childResource = CreateResourceInst(NULL,//temp->requestHandle,
                                                                    temp->resourceHandle,
                                                                    targetResource->query,
                                                                    NULL);//temp->rep);

                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Get child Resources URI = %s", temp->uri);
                    // THINGS_LOG_D(THINGS_DEBUG, TAG, "Request with query %s", targetResource->query);

                    // childResource->SetUri(childResource,parentResource->uri);
                    childResource->rep = CreateRepresentationInst(NULL);
                    childResource->rep->SetObjectValue(childResource->rep, OC_RSRVD_REPRESENTATION, parentResource->rep);
                    childResource->SetUri(childResource, temp->uri);
                    SetCommonPropertiesIntoPayload(childResource);
                    // SetUriIntoPayload(childResource);

                    deviceResource->AddChild(deviceResource, childResource);
                    parentResource = parentResource->Next( parentResource );

                }
                deviceResource->SetCommandId(deviceResource, targetResource->cmdId) ;// deviceCmdId);

                ReleaseResourceInst(targetResource);
                *targetRes = deviceResource;

                goto EXIT_POST_HANDLEING;

            }
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "\t EntityHandler result before ConvertResource  : (%d)", ehResult);

    OicResource* resource = CreateResourceInst(
                                            targetResource->requestHandle,
                                            targetResource->resourceHandle,
                                            targetResource->query,
                                            NULL);
    //resource->SetUri(resource, targetResource->uri);
    resource->SetCommandId(resource, targetResource->cmdId);

    ReleaseResourceInst(targetResource);
    *targetRes = resource;

    for(int iter = 0 ; iter < stateListCnt ; iter++)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "ID : %s", pStateList[iter]->deviceId);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "URI : %s", pStateList[iter]->resource);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr Name : %s", pStateList[iter]->attribute);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr. Value : %s", pStateList[iter]->value);
    }

    ehResult = gConvertResource(*targetRes, pStateList, stateListCnt);

EXIT_POST_HANDLEING:

    if(pList != NULL)
    {
        for(int i = 0; i< listCnt;i++)
        {
            DAFree(pList[i]);
            pList[i] = NULL;
        }
        DAFree(pList);
        pList = NULL;
    }

    return ehResult;

}
#endif
static OCEntityHandlerResult ProcessPostRequest(OicResource** targetRes)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    OicResource* targetResource = *targetRes;

    if(strstr (targetResource->uri, URI_SEC) != NULL
    && strstr (targetResource->uri, URI_PROVINFO) != NULL)
    {
        // 1. Post request for the Easy-Setup reset
        ehResult = SetProvisioningInfo(targetResource);
    }
#ifndef OCF_RTOS
  else if(strstr (targetResource->uri, URI_INFORMATION) != NULL)
    {
        // X. Get request for the Access point list(Will be deprecated!!!!)
        ehResult = SetDeviceInfoResult(targetResource);
    }   
    else if(strstr (targetResource->uri, URI_ACTIONS) != NULL )
    {
        // 2. Post request on the schedule resource
        THINGS_LOG_D(THINGS_DEBUG, TAG, "#####00 IN ProcessPostRequest pQuery : %s", targetResource->query);
        ehResult = SetActionResource(targetResource);
    }
    else if (strstr(targetResource->uri, URI_FILE) != NULL
        && strstr(targetResource->uri, URI_TRANSFER) != NULL )
    {
        // 3. Post request on the sharable file(s) to update the blob data
        ehResult = SetFileTransferResource(targetResource);
    }
    else if(strstr (targetResource->uri, URI_CONTENTS) != NULL
        && strstr (targetResource->uri, URI_RENDERER) != NULL )
    {
        // 4. Post request for the storing Blob data of files
        ehResult = SetContentsRendererMetadataResult(targetResource);
    }
    else if(strstr (targetResource->uri, URI_OIC) != NULL
        && strstr (targetResource->uri, URI_WK_MAINTENANCE) != NULL )
    {
        // 5. Post request for the trigger device maintenance functions
        ehResult = SetDeviceMaintenanceResult(targetResource);
    }
    else if(strstr (targetResource->uri, URI_BIXBY) != NULL )
    {
        // 6. Post request for handling bixby request
        ehResult = SetBixbyRequestResult(targetResource);
    }
#endif
    else
    {
#ifndef OCF_RTOS
 // 7. Post request on the other reosurce(s)
   int handled = 0;
        if( NULL != gHandleResList )
        {
            int iter = 0;
            for(iter = 0 ; iter < gHandleResCnt ; iter++)
            {
                if( strstr(targetResource->uri , gHandleResList[iter]) )
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "Handling %s with OicResource Based CB", targetResource->uri);

                    int ret = gHandleReqCB(targetResource->reqType, targetResource);
                    if(1 == ret)
                    {
                        ehResult = OC_EH_OK;
                        handled = 1;
                    }
                    else
                    {
                        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Handled as ERROR from App.");
                    }

                    break;
                }
            }
        }
        if(handled == 0)
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "Handling %s with OICInfo Converting Based CB", targetResource->uri);

            ehResult = PostResourceData(targetRes);
        }
#else
//jun
        // Post request for application resources
        if (gHandleRequestSetCB != NULL) {
            int ret = gHandleRequestSetCB(targetResource);
            if(1 == ret)
            {
                ehResult = OC_EH_OK;
            }
            else
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Handled as ERROR from App.");
            }
        } else {
            THINGS_LOG_V(THINGS_ERROR, TAG, "gHandleRequestSetCB is not registered");
        }


//jun
        // if(gSetReqCB != NULL){
        //     gSetReqCB(targetRes);
        //     ehResult = OC_EH_OK;
        // }

//jun
/*
        if(gHandleRequestInterfaceCB != NULL) {
          ehResult = gHandleRequestInterfaceCB(targetRes);
        }
        */
#endif
    }

    return ehResult;
}

#if 0
//jun
RequestMessage* convertToRequestMessage(OicResource** targetRes){  
   RequestMessage* rep = (RequestMessage*) malloc (sizeof(RequestMessage));
    if(NULL == rep)
    {
        return NULL;
    }

    return rep;
}
#endif

static OCEntityHandlerResult ProcessGetRequest(OicResource* targetResource)
{
    
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    char* deviceID = NULL;

    deviceID = strrchr(targetResource->uri, '/')+1;
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Get Device ID in Resources URI = %s", deviceID);

    OicRepresentation* rep = CreateRepresentationInst(NULL);
    targetResource->SetRepresentation(targetResource, rep);

    if(strstr (targetResource->uri, URI_SEC) != NULL
        && strstr (targetResource->uri, URI_PROVINFO) != NULL)
    {
        // 1. Get request for the Access point list
        ehResult = GetProvisioningInfo(targetResource);
    }
#ifndef OCF_RTOS
    else if(strstr (targetResource->uri, URI_INFORMATION) != NULL)
    {
        // X. Get request for the Access point list(Will be deprecated!!!!)
        ehResult = GetDeviceInfoResult(targetResource);
    }
    else if(strstr (targetResource->uri, URI_SEC) != NULL
        && strstr (targetResource->uri, URI_ACCESSPOINTLIST) != NULL)
    {
        // X. Get request for the Access point list(Will be deprecated!!!!)
        ehResult = GetAPScanResult(targetResource);
    }
    else if(strstr (targetResource->uri, URI_FILE) != NULL
        && strstr (targetResource->uri, URI_LIST) != NULL )
    {
        // 3. Get request for the list of files which can be transfered to the Client
        ehResult = GetFileListResult(targetResource);
    }
    else if(strstr (targetResource->uri, URI_FILE) != NULL
        && strstr (targetResource->uri, URI_TRANSFER) != NULL )
    {
        // 4. Get request for the Blob data of files
        ehResult = GetFileBlobValueResult(targetResource);
    }
    else if(strstr (targetResource->uri, URI_ACTIONS) != NULL )
    {
        // 5. Get request for the list of schedules
        ehResult = GetActionResource(targetResource);
    }
    else if (
            (strstr (targetResource->uri, URI_CONTENTS) != NULL
             && strstr (targetResource->uri, URI_PROVIDER) != NULL )
        ||
            ( strstr (targetResource->uri, URI_CONTENTS) != NULL
             && strstr (targetResource->uri, URI_RENDERER) != NULL )
            )
    {
        // 7. Get request for the Blob data of files
        ehResult = GetContentsMetadataResult(targetResource);
    }
    else if(strstr (targetResource->uri, URI_OIC) != NULL
        && strstr (targetResource->uri, URI_WK_MAINTENANCE) != NULL )
    {
        // 8. Get request for the device maintenance information
        ehResult = GetDeviceMaintenanceResult(targetResource);
    }
#endif
    else if(strstr (targetResource->uri, URI_SEC) != NULL
    && strstr (targetResource->uri, URI_ACCESSPOINTLIST) != NULL)
    {
         // X. Get request for the Access point list(Will be deprecated!!!!)
         ehResult = GetAPScanResult(targetResource);
    }
    else if(strstr (targetResource->uri, OC_RSRVD_DEVICE_URI) != NULL
         || strstr (targetResource->uri, OC_RSRVD_PLATFORM_URI) != NULL )
    {
        // 9. Get request for the device maintenance information
        ehResult = GetDeviceOrPlatformInfoResult(targetResource, deviceID);
    }
    else
    {
#ifndef OCF_RTOS
        THINGS_LOG_D(THINGS_DEBUG, TAG, "HELLO WORLD");
        // 10. Get request on the other resources
        int handled = 0;
        if( NULL != gHandleResList )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "HELLO WORLD");
            int iter = 0;
            for(iter = 0 ; iter < gHandleResCnt ; iter++)
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "%s  : %s ", targetResource->uri, gHandleResList[iter]);
                if( strstr(targetResource->uri , gHandleResList[iter]) )
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "Handling %s with OicResource Based CB", targetResource->uri);
                    int ret = gHandleReqCB(targetResource->reqType, targetResource);
                    if(1 == ret)
                    {
                        ehResult = OC_EH_OK;
                        handled = 1;
                    }
                    else
                    {
                        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Handled as ERROR from App.");
                    }

                    break;
                }
            }
        }
        if(handled == 0)
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "Handling %s with OICInfo Converting Based CB", targetResource->uri);

            ehResult = GetResourceData(targetResource);
        }
#else
        //jun
          // Post request for application resources
          if (gHandleRequestGetCB != NULL) {

            int ret = gHandleRequestGetCB(targetResource);
            if(1 == ret)
            {
                ehResult = OC_EH_OK;
            }
            else
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Handled as ERROR from App.");
            }
        } else {
            THINGS_LOG_V(THINGS_ERROR, TAG, "gHandleRequestGetCB is not registered");
        }
#endif
    }

    return ehResult;
}

static bool isCanNotResponseCase(OicResource* targetResource, OCMethod reqType, OCEntityHandlerResult ehResult)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    bool res = false;

    if( targetResource == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid argument.(targetResource=NULL)");
        return true;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "targetResource->uri=%s", targetResource->uri);
    if(strstr (targetResource->uri, URI_SEC) != NULL
        && strstr (targetResource->uri, URI_PROVINFO) != NULL)
    {
        if( reqType == OC_REST_POST && ehResult == OC_EH_SLOW )
        {
            res = true;
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "result = %d", res);
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return res;
}

static void* MessageHandlingLoop(void *param)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int reqType = -1;
    // char* uri = NULL;
    OicResource* targetResource = NULL;
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    while (!gQuitFlag)
    {
        //THINGS_LOG_D(THINGS_INFO, TAG, "[Req.HDLoop] Looping.");

        //while(gOicReqQueue.length(&gOicReqQueue) > 0)
        while(gpOicReqQueue->length(gpOicReqQueue) > 0 )
        {
            ehResult = OC_EH_ERROR;

            //targetResource = gOicReqQueue.pop(&gOicReqQueue, &reqType);
            targetResource = gpOicReqQueue->pop(gpOicReqQueue, &reqType);
            if(NULL == targetResource)
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Request Item is NULL.");
                continue;
            }

            // RequestInfo* info = CreateRequestInfo(reqType, 0,
            //                             targetResource->requestHandle,
            //                             targetResource->resourceHandle);

            // DuplicateString (info->frameId, &targetResource->cmdId);

            THINGS_LOG_D(THINGS_DEBUG, TAG,
                "Request Handle : %x, Resource Handle : %x",
                        targetResource->requestHandle,
                        targetResource->resourceHandle);

            // THINGS_LOG_D(THINGS_DEBUG, TAG, "Request on cmdId. %s",
            //                         targetResource->cmdId);
            targetResource->reqType = reqType;

            if ( reqType == OC_REST_GET )
            {
                THINGS_LOG_V(THINGS_INFO, TAG, "\t\tReq. : GET on %s", targetResource->uri);
                THINGS_LOG_V(THINGS_INFO, TAG, "\t\tQuery: %s", targetResource->query);
                ehResult = ProcessGetRequest(targetResource);

            }
            else if (reqType == OC_REST_POST )
            {
                THINGS_LOG_V(THINGS_INFO, TAG, "\t\tReq. : POST on  %s", targetResource->uri);
                THINGS_LOG_V(THINGS_INFO, TAG, "\t\tQuery: %s", targetResource->query);
                ehResult = ProcessPostRequest(&targetResource);
            }
            else
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, " Invalid Request Received : %d", reqType);
            }
            THINGS_LOG_D(THINGS_DEBUG, TAG, " @@@@@ targetResource ->size : %d", targetResource->size);
            // THINGS_LOG_D(THINGS_DEBUG, TAG, " @@@@@ targetResource ->cmdId : %s", targetResource->cmdId);

            if( isCanNotResponseCase(targetResource, reqType, ehResult) == false)
            {
                if(ehResult != OC_EH_OK && ehResult != OC_EH_SLOW)
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Handing Request Failed, Sending ERROR Response");

                    OicResource* temp = gBuilder->GetResource(gBuilder, targetResource->uri);
                    const char* rt = temp->GetResType(temp, 0);
                    if (0 == strcmp(rt, SEC_RTYPE_TEMPERATURE) || 0 == strcmp(rt, OIC_RTYPE_TEMPERATURE))
                    {
                        OCRepPayload *repPayload = targetResource->GetRepPayload(targetResource);
                        SendResponse(targetResource->requestHandle,
                                 targetResource->resourceHandle,
                                 OC_EH_ERROR, targetResource->uri, repPayload);
                    }
                    else
                    {
                        SendResponse(targetResource->requestHandle,
                                 targetResource->resourceHandle,
                                 ehResult, targetResource->uri, NULL);
                    }
                }
                else
                {
                    OCRepPayload *repPayload = targetResource->GetRepPayload(targetResource);

                    ehResult = SendResponse(
                                    targetResource->requestHandle,   // reqInfo->reqHandle,
                                    targetResource->resourceHandle,  // reqInfo->resHandle,
                                    targetResource->error,
                                    targetResource->uri,
                                    repPayload);

                  /*! Added by DA for memory Leak fix
                   */
                	//if(OC_EH_ERROR == ehResult)
                	//{
                    	OCPayloadDestroy((OCPayload*)repPayload);
                    	repPayload = NULL;
                    //}
                }
            }

            // [Jay] Need to design How to release memory allocated for the OicResource list.
            ReleaseResourceInst(targetResource);

            usleep(100 * 1000);
        } // End Of While Loop

        OICControlQueueEmpty();

        usleep(100 * 1000);
    } // End Of While Loop
    THINGS_LOG_D(THINGS_DEBUG, TAG, "[Req.HDLoop] Leaving loop...");

    return NULL;
}

void NotifyResultOfReset(OicResource* targetResource, bool result)
{
    THINGS_LOG(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY );

    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if( targetResource == NULL )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "Not exist remote-client.");
        return ;
    }

    if( result == true )    // reset Success.
    {
        OCRepPayload *repPayload = targetResource->GetRepPayload(targetResource);

        ehResult = SendResponse(
                        targetResource->requestHandle,   // reqInfo->reqHandle,
                        targetResource->resourceHandle,  // reqInfo->resHandle,
                        targetResource->error,
                        targetResource->uri,
                        repPayload);

          /*! Added by DA for memory Leak fix
           */
//        if(OC_EH_ERROR == ehResult)
//        {
            OCPayloadDestroy((OCPayload*)repPayload);
            repPayload = NULL;
//        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Handing Request Failed, Sending ERROR Response");

        SendResponse(targetResource->requestHandle,
                 targetResource->resourceHandle,
                 ehResult, targetResource->uri, NULL);
    }

    ReleaseResourceInst(targetResource);

    THINGS_LOG(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT );
}

#ifndef OCF_RTOS
OCRepPayload* GetConvertData (const char* uri, const struct OICInfo** oicinfo, const int listCnt)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    OCEntityHandlerResult ehResult = OC_EH_OK;
    OicResource * temp = gBuilder->GetResource(gBuilder, uri);
    OicResource * targetResource = NULL;
    if(temp)
    {
        targetResource = CreateResourceInst(NULL,
                                                temp->resourceHandle,
                                                NULL,
                                                NULL);

        if(!targetResource)
            return NULL;

        OicRepresentation * rep = CreateRepresentationInst(NULL);
        if(!rep)
        {
            ReleaseResourceInst(targetResource);
            return NULL;
        }

        targetResource->rep = rep;
        targetResource->SetUri(targetResource, uri);

        for(int iter = 0 ; iter < listCnt ; iter++)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Device ID : %s", oicinfo[iter]->deviceId);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "ID : %s", oicinfo[iter]->id);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "URI : %s", oicinfo[iter]->resource);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr Name : %s", oicinfo[iter]->attribute);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Attr. Value : %s", oicinfo[iter]->value);
        }

        ehResult = gConvertResource(targetResource, oicinfo, listCnt);

        if(ehResult != OC_EH_OK)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "OICInfo Resource Convert Failed: (%d)", ehResult);
            return NULL;
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return (OCRepPayload*) targetResource->rep->payload;
}
#endif

int NotifyOicObservers(const char* uri, const char* query)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int res = 0;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "uri = %s", uri);
    if(NULL != uri)
    {
        int remainLen = MAX_RESOURCE_LEN - 1;
        char tempUri[MAX_RESOURCE_LEN] = { 0 };

        if (NULL != gGetNotifyObsURI)
        {
            char* temp = gGetNotifyObsURI(uri, query);
            if (NULL != temp)
            {
                DAStrncpy(tempUri, temp, remainLen);
                remainLen -= strnlen(tempUri, MAX_RESOURCE_LEN - 1);
                DAFree(temp);
            }
        }

        if (strnlen(tempUri, MAX_RESOURCE_LEN - 1) < 1)
        {
            DAStrncpy(tempUri, uri, remainLen);
            remainLen -= strnlen(tempUri, MAX_RESOURCE_LEN - 1);
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "%s resource notifies to observers.", tempUri);

        for (int iter = 0; iter < gBuilder->res_num; iter++)
        {
            if (strstr(gBuilder->gResArr[iter]->uri, tempUri) != NULL)
            {
                OCStackResult ret2 = OCNotifyAllObservers(
                                    (OCResourceHandle)gBuilder->gResArr[iter]->resourceHandle,
                                    OC_MEDIUM_QOS);

                THINGS_LOG_D(THINGS_DEBUG, TAG, "%s resource has notified to observers.",
                                            gBuilder->gResArr[iter]->uri);

                if (OC_STACK_OK == ret2)
                {
                    THINGS_LOG(THINGS_INFO, TAG, "Success: Sent notification to Observers");
                }
                else
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "Failed: No Observers to notify : %d ", ret2);
                }
                res = 1;
                break;
            }
        }

#ifndef OCF_RTOS
        // if(res)
        {
            // Need to refactor later support the number bigger then 9
            char * deviceNum = strrchr(uri, '/');
            char resUri[128] = {0};

            memset(resUri, 0, (size_t)128);
            strcat(resUri, URI_DEVICE_COL);
            strcat(resUri, deviceNum);

            THINGS_LOG_D(THINGS_DEBUG, TAG, "Target Collection URI : %s", resUri);

            OicResource* temp = gBuilder->GetResource(gBuilder, resUri);
            if(temp)
            {
                OCStackResult ret3 = OCNotifyAllObservers(
                            (OCResourceHandle)temp->resourceHandle,
                            OC_MEDIUM_QOS);

                if (OC_STACK_OK == ret3)
                {
                    THINGS_LOG(THINGS_INFO, TAG, "Sent notification to Observers");
                }
                else
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "No Observers to notify : %d ", ret3);
                }
            }
        }
#endif

    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return res;
}

OCEntityHandlerResult handleMessage(OicResource* targetResource)
{
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	if(NULL == targetResource)
	{
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Request Item is NULL.");
		return OC_EH_ERROR;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG,
		"Request Handle : %x, Resource Handle : %x",
				targetResource->requestHandle,
				targetResource->resourceHandle);

	// THINGS_LOG_D(THINGS_DEBUG, TAG, "Request on cmdId. %s",
	//                         targetResource->cmdId);


	if ( targetResource->reqType == OC_REST_GET )
	{
		THINGS_LOG_V(THINGS_INFO, TAG, "\t\tReq. : GET on %s", targetResource->uri);
		THINGS_LOG_V(THINGS_INFO, TAG, "\t\tQuery: %s", targetResource->query);
		ehResult = ProcessGetRequest(targetResource);

	}
	else if (targetResource->reqType == OC_REST_POST )
	{
		THINGS_LOG_V(THINGS_INFO, TAG, "\t\tReq. : POST on  %s", targetResource->uri);
		THINGS_LOG_V(THINGS_INFO, TAG, "\t\tQuery: %s", targetResource->query);
		ehResult = ProcessPostRequest(&targetResource);
	}
	else
	{
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, " Invalid Request Received : %d", targetResource->reqType);
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, " @@@@@ targetResource ->size : %d", targetResource->size);
	// THINGS_LOG_D(THINGS_DEBUG, TAG, " @@@@@ targetResource ->cmdId : %s", targetResource->cmdId);

	if( isCanNotResponseCase(targetResource, targetResource->reqType, ehResult) == false)
	{
		if(ehResult != OC_EH_OK && ehResult != OC_EH_SLOW)
		{
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Handing Request Failed, Sending ERROR Response");

			OicResource* temp = gBuilder->GetResource(gBuilder, targetResource->uri);
			const char* rt = temp->GetResType(temp, 0);
			if (0 == strcmp(rt, SEC_RTYPE_TEMPERATURE) || 0 == strcmp(rt, OIC_RTYPE_TEMPERATURE))
			{
				OCRepPayload *repPayload = targetResource->GetRepPayload(targetResource);
				SendResponse(targetResource->requestHandle,
						 targetResource->resourceHandle,
						 OC_EH_ERROR, targetResource->uri, repPayload);
			}
			else
			{
				SendResponse(targetResource->requestHandle,
						 targetResource->resourceHandle,
						 ehResult, targetResource->uri, NULL);
			}
			ehResult = OC_EH_OK;
		}
		else
		{
			OCRepPayload *repPayload = targetResource->GetRepPayload(targetResource);

			ehResult = SendResponse(
							targetResource->requestHandle,   // reqInfo->reqHandle,
							targetResource->resourceHandle,  // reqInfo->resHandle,
							targetResource->error,
							targetResource->uri,
							repPayload);

		  /*! Added by DA for memory Leak fix
		   */
			//if(OC_EH_ERROR == ehResult)
			//{
				OCPayloadDestroy((OCPayload*)repPayload);
				repPayload = NULL;
			//}
		}
	}

	// [Jay] Need to design How to release memory allocated for the OicResource list.
	ReleaseResourceInst(targetResource);

	return ehResult;
}


OCEntityHandlerResult EntityHandler (OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback)
{
    THINGS_LOG_D(THINGS_INFO, TAG, THINGS_FUNC_ENTRY);
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    // Validate pointer
    if (!entityHandlerRequest)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid request pointer");
        return ehResult;
    }

    const char* uri = OCGetResourceUri(entityHandlerRequest->resource);

    // Observe Request Handling
    if (flag & OC_OBSERVE_FLAG)
    {
        if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
        {
            THINGS_LOG_V(THINGS_INFO, TAG, "Observe Requset on : %s ", uri);
            // 1. Check whether it's Observe request on the Collection Resource
            if(NULL != strstr(uri, URI_DEVICE_COL) )
            {
                //2. Check whether the query carriese the if=oic.if.b
                if( (strstr(entityHandlerRequest->query, OIC_INTERFACE_BATCH) == NULL) )
                {
                    //3. If not batch then error
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Collection Resource Requires BATCH for Observing : %s", entityHandlerRequest->query);
                    ehResult = OC_EH_BAD_REQ;
                    goto RESPONSE_ERROR;
                }
                else
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Receiving Observe Request Collection Resource");
                }
            }
        }
        else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
        {
            THINGS_LOG_V(THINGS_INFO, TAG, "CancelObserve Request on : %s", uri);
        }
    }

    // Get/Post Request Handling
    if (flag & OC_REQUEST_FLAG)
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "Req. URI  : %s", uri);
        THINGS_LOG_D(THINGS_INFO, TAG, "Req. TYPE : %d", entityHandlerRequest->method);

        if( OICGetResetMask(RST_CONTROL_MODULE_DISABLE) == true )
        {
            THINGS_LOG(THINGS_INFO, TAG, "Control Module Disable.");
            ehResult = OC_EH_NOT_ACCEPTABLE;
        }
        else if( (OC_REST_GET == entityHandlerRequest->method)
            || (OC_REST_POST == entityHandlerRequest->method) )
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "Request Handle : %x, Resource Handle : %x",
                        entityHandlerRequest->requestHandle, entityHandlerRequest->resource);
            if( VerifyRequest(entityHandlerRequest, uri, (int)entityHandlerRequest->method) > 0 )
            {
                // [JAY] IoTivity Stack Destroy the payload after receving result from this function
                //       Therefore, we need to copy/clone the payload for the later use..
                OicResource* resource = CreateResourceInst(
                                            entityHandlerRequest->requestHandle,
                                            entityHandlerRequest->resource,
                                            entityHandlerRequest->query,
                                            entityHandlerRequest->payload);
                resource->SetDevAddr(resource, &(entityHandlerRequest->devAddr));
                resource->reqType = entityHandlerRequest->method;
                //resource->SetUri(resource, uri);

//                THINGS_LOG_D(THINGS_DEBUG, TAG, "About to Queue a received Request~!!!!");
//
//                //gOicReqQueue.push(&gOicReqQueue, resource, entityHandlerRequest->method);
//                gpOicReqQueue->push(gpOicReqQueue, resource, entityHandlerRequest->method);
//                ehResult = OC_EH_SLOW;
                ehResult = handleMessage(resource);
            }
            else
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Query in the Request : %s",
                    entityHandlerRequest->query);
            }
        }
        else if( OC_REST_DELETE == entityHandlerRequest->method
              || OC_REST_PUT == entityHandlerRequest->method )
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Delete/PUT Req. Handling is not supported Yet");
        }
        else
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Received unsupported method from client");
        }

    }

RESPONSE_ERROR:

    if (ehResult != OC_EH_SLOW && ehResult != OC_EH_OK)
    {
        // [JAY] If the result is OC_EH_ERROR, the stack will remove the
        //       received request in the stack.
        //       If the reusult is OC_EH_SLOW, then the request will be
        //       stored in the stack till the response goes out
        ehResult = SendResponse(entityHandlerRequest->requestHandle,
                                entityHandlerRequest->resource,
                                ehResult, uri, NULL);
        //[JAY] Currently this code will not do anything...
        //      Need to refactor later..
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return ehResult;
}




void InitHandler()
{
    gQuitFlag = false;


  
//#ifdef OCF_RTOS
//    pthread_create_rtos(&gReqHandle, NULL, MessageHandlingLoop, (void *)NULL, THINGS_STACK_MESSAGE_HANDLING_THREAD);
//#else
//   OICThread_create (&gReqHandle, NULL, MessageHandlingLoop, (void *)NULL);
//#endif

}

void DeinitHandler()
{
    gQuitFlag = true;


    int iter = 0;
    if(gHandleResList != NULL)
    {
        for(iter = 0 ; iter < gHandleResCnt ; iter++)
        {
            if(gHandleResList[iter] != NULL)
            {
                DAFree(gHandleResList[iter]);
                gHandleResList[iter] = NULL;
            }
        }
        DAFree(gHandleResList);
        gHandleResList = NULL;
    }
    gHandleResCnt = 0;
}

struct OICRequestHandler* GetHandlerInstance()
{
    struct OICRequestHandler* handler
        = (OICRequestHandler*) DAMalloc (sizeof (OICRequestHandler) );

    if(handler == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Not Enough Memory");
        return NULL;
    }
    else
    {
        handler->EntityHandler = &EntityHandler;
        handler->InitModule = &InitHandler;
        handler->DeinitModule = &DeinitHandler;
        handler->NotifyOicObservers = &NotifyOicObservers;
        return handler;
    }
}

void ReleaseHandlerInstance(struct OICRequestHandler* handler)
{
    if(handler)
    {
        handler->DeinitModule();
        DAFree(handler);
    }
}

#ifndef OCF_RTOS
int RegisterNotifyFileUpdateCB(NotifyFileUpdateCB func)
{
    if( NULL != func )
    {
        gNotifyFileUpdate = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterProvContentsMetadataGetCB(GetProvContentsMetadataCB func)
{
    if( NULL != func )
    {
        gGetProvContents = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterRendContentsMetadataGetCB(GetRendContentsMetadataCB func)
{
    if( NULL != func )
    {
        gGetRendContents = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterRendContentsMetadataSetCB(SetRendContentsMetadataCB func)
{
    if( NULL != func )
    {
        gSetRendContents = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterDeviceMaintenaceGetCB(GetDeviceMaintenanceCB func)
{
    if( NULL != func )
    {
        gGetMaintenanceCB = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterDeviceMaintenaceSetCB(SetDeviceMaintenanceCB func)
{
    if( NULL != func )
    {
        gSetMaintenanceCB = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterGetDeviceInfoFunc(GetDeviceInfoFuncType func)
{
    if( NULL != func )
    {
        gGetDeviceInfoCB = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterSetDeviceInfoFunc(SetDeviceInfoFuncType func)
{
    if( NULL != func )
    {
        gSetDeviceInfoCB = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterFileInfoGetCB(GetFileListCB func)
{
    if( NULL != func )
    {
        gGetFileListCB = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterBixbyHandleCB(BIXBYReqHandleCB func)
{
    if( NULL != func )
    {
        gBixbyHandleCB = func;
        return 1;
    }
    else
    {
        return 0;
    }
}
#else

int RegisterDawitRequestInterfaceCB(HandleRequestInterfaceCB func)
{
  if(NULL != func) {
    gHandleRequestInterfaceCB = func;
    return 1;
  }

  return 0;
}

#endif

int RegisterStopSoftAPCB(StopSoftAPFuncType func)
{
    if( NULL != func )
    {
        gStopSoftAPCB = func;
        return 1;
    }
    else
    {
        return 0;
    }
}

int RegisterHandleReqCB(HandleRequestFuncType func, char** list, int listCnt)
{
    // THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");
    int res = 0;
    int iter = 0;
    int num = 0;

    for(iter = 0 ; iter < gHandleResCnt ; iter++)
    {
        if(gHandleResList[iter] != NULL)
        {
            DAFree(gHandleResList[iter]);
            gHandleResList[iter] = NULL;
        }
    }
    DAFree(gHandleResList);
    gHandleResList = NULL;
    gHandleResCnt = 0;

    if( NULL != func
        && listCnt > 0 )
    {

        gHandleReqCB = func;
        gHandleResCnt = listCnt;
        gHandleResList = (char**)DAMalloc(listCnt);
        // THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");
        for(iter = 0 ; iter < gHandleResCnt ; iter++)
        {
            gHandleResList[iter] = NULL;
        }
        // THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");
        for(iter = 0 ; iter < gHandleResCnt ; iter++)
        {
            num = 0;
// THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");
            if(list[iter] != NULL
            && strlen(list[iter]) > 0)
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[%d] URI : %s", iter, list[iter]);
                gHandleResList[iter] = (char*)DAMalloc( strlen(list[iter]) + 1 );
                memset(gHandleResList[iter], 0,  strlen(list[iter]) + 1 );
                DAStrncpy(gHandleResList[iter], list[iter], strlen(list[iter]));
            }
            else
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Resource List ");
                res = 0;
                break;
            }
            res = 1;
        }
    }

    if(res == 0)
    {
// THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");
        for(iter = 0 ; iter < gHandleResCnt ; iter++)
        {
            if(gHandleResList[iter] != NULL)
            {
                DAFree(gHandleResList[iter]);
                gHandleResList[iter] = NULL;
            }
        }
// THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");
        DAFree(gHandleResList);
        gHandleResList = NULL;
        gHandleResCnt = 0;
    }
    // THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");
    return res;
}

int RegisterHandleRequestFunc(HandleRequestFuncType getFunc, HandleRequestFuncType setFunc)
{
    if( NULL != getFunc && NULL != setFunc)
    {
        gHandleRequestGetCB = getFunc;
        gHandleRequestSetCB = setFunc;
    }
    else
    {
        return 0;
    }
    return 1;    
}