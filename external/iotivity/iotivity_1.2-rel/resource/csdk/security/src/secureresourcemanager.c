//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <string.h>
#include "ocstack.h"
#include "logger.h"
#include "cainterface.h"
#include "resourcemanager.h"
#include "credresource.h"
#include "doxmresource.h"
#include "pstatresource.h"
#include "policyengine.h"
#include "srmutility.h"
#include "psinterface.h"
#include "amsmgr.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "securevirtualresourcetypes.h"
#include "secureresourcemanager.h"
#include "srmresourcestrings.h"
#include "ocresourcehandler.h"

#if defined( __WITH_TLS__) || defined(__WITH_DTLS__)
#include "pkix_interface.h"
#endif //__WITH_TLS__ or __WITH_DTLS__
#define TAG  "OIC_SRM"

//Request Callback handler
static CARequestCallback gRequestHandler = NULL;
//Response Callback handler
static CAResponseCallback gResponseHandler = NULL;
//Error Callback handler
static CAErrorCallback gErrorHandler = NULL;
//Persistent Storage callback handler for open/read/write/close/unlink
static OCPersistentStorage *gPersistentStorageHandler =  NULL;
//Provisioning response callback
static SPResponseCallback gSPResponseHandler = NULL;

/**
 * A single global Policy Engine context will suffice as long
 * as SRM is single-threaded.
 */
PEContext_t g_policyEngineContext;

/**
 * Function to register provisoning API's response callback.
 * @param respHandler response handler callback.
 */
void SRMRegisterProvisioningResponseHandler(SPResponseCallback respHandler)
{
    gSPResponseHandler = respHandler;
}

void SetResourceRequestType(PEContext_t *context, const char *resourceUri)
{
    context->resourceType = GetSvrTypeFromUri(resourceUri);
}

static void SRMSendUnAuthorizedAccessresponse(PEContext_t *context)
{
    CAResponseInfo_t responseInfo = {.result = CA_EMPTY};

    if (NULL == context ||
       NULL == context->amsMgrContext->requestInfo)
    {
        OIC_LOG_V(ERROR, TAG, "%s : NULL Parameter(s)",__func__);
        return;
    }

    memcpy(&responseInfo.info, &(context->amsMgrContext->requestInfo->info),
            sizeof(responseInfo.info));
    responseInfo.info.payload = NULL;
    responseInfo.result = CA_UNAUTHORIZED_REQ;
    responseInfo.info.dataType = CA_RESPONSE_DATA;

    if (CA_STATUS_OK == CASendResponse(context->amsMgrContext->endpoint, &responseInfo))
    {
        OIC_LOG(DEBUG, TAG, "Succeed in sending response to a unauthorized request!");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed in sending response to a unauthorized request!");
    }
}

void SRMSendResponse(SRMAccessResponse_t responseVal)
{
    OIC_LOG(DEBUG, TAG, "Sending response to remote device");

    if (IsAccessGranted(responseVal) && gRequestHandler)
    {
        OIC_LOG_V(INFO, TAG, "%s : Access granted. Passing Request to RI layer", __func__);
        if (!g_policyEngineContext.amsMgrContext->endpoint ||
            !g_policyEngineContext.amsMgrContext->requestInfo)
        {
            OIC_LOG_V(ERROR, TAG, "%s : Invalid arguments", __func__);
            SRMSendUnAuthorizedAccessresponse(&g_policyEngineContext);
            goto exit;
        }
        gRequestHandler(g_policyEngineContext.amsMgrContext->endpoint,
                g_policyEngineContext.amsMgrContext->requestInfo);
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "%s : ACCESS_DENIED.", __func__);
        SRMSendUnAuthorizedAccessresponse(&g_policyEngineContext);
    }

exit:
    //Resetting PE state to AWAITING_REQUEST
    SetPolicyEngineState(&g_policyEngineContext, AWAITING_REQUEST);
}

/**
 * Handle the request from the SRM.
 *
 * @param endPoint object from which the response is received.
 * @param requestInfo contains information for the request.
 */
void SRMRequestHandler(const CAEndpoint_t *endPoint, const CARequestInfo_t *requestInfo)
{
    OIC_LOG(DEBUG, TAG, "Received request from remote device");

    bool isRequestOverSecureChannel = false;
    if (!endPoint || !requestInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid arguments");
        return;
    }

    // Copy the subjectID
    OicUuid_t subjectId = {.id = {0}};
    memcpy(subjectId.id, requestInfo->info.identity.id, sizeof(subjectId.id));
    if (endPoint->flags & CA_SECURE)
    {
        OIC_LOG(INFO, TAG, "request over secure channel");
        isRequestOverSecureChannel = true;
    }

    //Check the URI has the query and skip it before checking the permission
    if (NULL == requestInfo->info.resourceUri)
    {
        OIC_LOG(ERROR, TAG, "Invalid resourceUri");
        return;
    }

    char *uri = strstr(requestInfo->info.resourceUri, "?");
    int position = 0;
    if (uri)
    {
        //Skip query and pass the resource uri
        position = uri - requestInfo->info.resourceUri;
    }
    else
    {
        position = strlen(requestInfo->info.resourceUri);
    }
    if (MAX_URI_LENGTH < position  || 0 > position)
    {
        OIC_LOG(ERROR, TAG, "Incorrect URI length");
        return;
    }
    SRMAccessResponse_t response = ACCESS_DENIED;
    char newUri[MAX_URI_LENGTH + 1];
    OICStrcpyPartial(newUri, MAX_URI_LENGTH + 1, requestInfo->info.resourceUri, position);

    SetResourceRequestType(&g_policyEngineContext, newUri);

     // Form a 'Error', 'slow response' or 'access deny' response and send to peer
    CAResponseInfo_t responseInfo = {.result = CA_EMPTY};
    memcpy(&responseInfo.info, &(requestInfo->info), sizeof(responseInfo.info));
    responseInfo.info.payload = NULL;
    responseInfo.info.dataType = CA_RESPONSE_DATA;

    OCResource *resPtr = FindResourceByUri(newUri);
    if (NULL != resPtr)
    {
        // All vertical secure resources and SVR resources other than DOXM & PSTAT should reject request
        // over coap.
        if ((((resPtr->resourceProperties) & OC_SECURE)
                            && (g_policyEngineContext.resourceType == NOT_A_SVR_RESOURCE))
                            || ((g_policyEngineContext.resourceType < OIC_SEC_SVR_TYPE_COUNT)
                            &&  (g_policyEngineContext.resourceType != OIC_R_DOXM_TYPE)
                            &&  (g_policyEngineContext.resourceType != OIC_R_PSTAT_TYPE)))
        {
           // if resource is secure and request is over insecure channel
            if (!isRequestOverSecureChannel)
            {
                // Reject all the requests over coap for secure resource.
                responseInfo.result = CA_FORBIDDEN_REQ;
                if (CA_STATUS_OK != CASendResponse(endPoint, &responseInfo))
                {
                    OIC_LOG(ERROR, TAG, "Failed in sending response to a unauthorized request!");
                }
                return;
            }
        }
    }
#ifdef MULTIPLE_OWNER
    /*
     * In case of ACL and CRED, The payload required to verify the payload.
     * Payload information will be used for subowner's permission verification.
     */
    g_policyEngineContext.payload = (uint8_t*)requestInfo->info.payload;
    g_policyEngineContext.payloadSize = requestInfo->info.payloadSize;
#endif //MULTIPLE_OWNER

    //New request are only processed if the policy engine state is AWAITING_REQUEST.
    if (AWAITING_REQUEST == g_policyEngineContext.state)
    {
        OIC_LOG_V(DEBUG, TAG, "Processing request with uri, %s for method, %d",
                requestInfo->info.resourceUri, requestInfo->method);
        response = CheckPermission(&g_policyEngineContext, &subjectId, newUri,
                GetPermissionFromCAMethod_t(requestInfo->method));
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "PE state %d. Ignoring request with uri, %s for method, %d",
                g_policyEngineContext.state, requestInfo->info.resourceUri, requestInfo->method);
    }

    if (IsAccessGranted(response) && gRequestHandler)
    {
        gRequestHandler(endPoint, requestInfo);
        return;
    }

    VERIFY_NON_NULL(TAG, gRequestHandler, ERROR);

    if (ACCESS_WAITING_FOR_AMS == response)
    {
        OIC_LOG(INFO, TAG, "Sending slow response");

        UpdateAmsMgrContext(&g_policyEngineContext, endPoint, requestInfo);
        responseInfo.result = CA_EMPTY;
        responseInfo.info.type = CA_MSG_ACKNOWLEDGE;
    }
    else
    {
        /*
         * TODO Enhance this logic more to decide between
         * CA_UNAUTHORIZED_REQ or CA_FORBIDDEN_REQ depending
         * upon SRMAccessResponseReasonCode_t
         */
        OIC_LOG(INFO, TAG, "Sending for regular response");
        responseInfo.result = CA_UNAUTHORIZED_REQ;
    }

    if (CA_STATUS_OK != CASendResponse(endPoint, &responseInfo))
    {
        OIC_LOG(ERROR, TAG, "Failed in sending response to a unauthorized request!");
    }
    return;
exit:
    responseInfo.result = CA_INTERNAL_SERVER_ERROR;
    if (CA_STATUS_OK != CASendResponse(endPoint, &responseInfo))
    {
        OIC_LOG(ERROR, TAG, "Failed in sending response to a unauthorized request!");
    }
}

/**
 * Handle the response from the SRM.
 *
 * @param endPoint points to the remote endpoint.
 * @param responseInfo contains response information from the endpoint.
 */
void SRMResponseHandler(const CAEndpoint_t *endPoint, const CAResponseInfo_t *responseInfo)
{
    OIC_LOG(DEBUG, TAG, "Received response from remote device");

    // isProvResponse flag is to check whether response is catered by provisioning APIs or not.
    // When token sent by CA response matches with token generated by provisioning request,
    // gSPResponseHandler returns true and response is not sent to RI layer. In case
    // gSPResponseHandler is null and isProvResponse is false response then the response is for
    // RI layer.
    bool isProvResponse = false;

    if (gSPResponseHandler)
    {
        isProvResponse = gSPResponseHandler(endPoint, responseInfo);
    }
    if (!isProvResponse && gResponseHandler)
    {
        gResponseHandler(endPoint, responseInfo);
    }
}

/**
 * Handle the error from the SRM.
 *
 * @param endPoint is the remote endpoint.
 * @param errorInfo contains error information from the endpoint.
 */
void SRMErrorHandler(const CAEndpoint_t *endPoint, const CAErrorInfo_t *errorInfo)
{
    OIC_LOG_V(INFO, TAG, "Received error from remote device with result, %d for request uri, %s",
            errorInfo->result, errorInfo->info.resourceUri);
    if (gErrorHandler)
    {
        gErrorHandler(endPoint, errorInfo);
    }
}

OCStackResult SRMRegisterHandler(CARequestCallback reqHandler,
                                 CAResponseCallback respHandler,
                                 CAErrorCallback errHandler)
{
    OIC_LOG(DEBUG, TAG, "SRMRegisterHandler !!");
    if( !reqHandler || !respHandler || !errHandler)
    {
        OIC_LOG(ERROR, TAG, "Callback handlers are invalid");
        return OC_STACK_INVALID_PARAM;
    }
    gRequestHandler = reqHandler;
    gResponseHandler = respHandler;
    gErrorHandler = errHandler;


#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    CARegisterHandler(SRMRequestHandler, SRMResponseHandler, SRMErrorHandler);
#else
    CARegisterHandler(reqHandler, respHandler, errHandler);
#endif /* __WITH_DTLS__ */
    return OC_STACK_OK;
}

OCStackResult SRMRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler)
{
    OIC_LOG(DEBUG, TAG, "SRMRegisterPersistentStorageHandler !!");
    if(!persistentStorageHandler)
    {
        OIC_LOG(ERROR, TAG, "The persistent storage handler is invalid");
        return OC_STACK_INVALID_PARAM;
    }

    gPersistentStorageHandler = persistentStorageHandler;

    // Check validity of Persistent Storage
    OCStackResult res = OC_STACK_SVR_DB_NOT_EXIST;
    res = CheckPersistentStorage(persistentStorageHandler);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Persistent storage is not normal");
        gPersistentStorageHandler = NULL;
    }
    return res;
}

OCPersistentStorage* SRMGetPersistentStorageHandler()
{
    return gPersistentStorageHandler;
}

OCStackResult SRMInitSecureResources()
{
    // TODO: temporarily returning OC_STACK_OK every time until default
    // behavior (for when SVR DB is missing) is settled.
    InitSecureResources();
    OCStackResult ret = OC_STACK_OK;
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    if (CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskCredentials))
    {
        OIC_LOG(ERROR, TAG, "Failed to revert TLS credential handler.");
        ret = OC_STACK_ERROR;
    }
    CAregisterPkixInfoHandler(GetPkixInfo);
    CAregisterGetCredentialTypesHandler(InitCipherSuiteList);
#endif // __WITH_DTLS__ or __WITH_TLS__
    return ret;
}

void SRMDeInitSecureResources()
{
    DestroySecureResources();
}

OCStackResult SRMInitPolicyEngine()
{
    return InitPolicyEngine(&g_policyEngineContext);
}

void SRMDeInitPolicyEngine()
{
    DeInitPolicyEngine(&g_policyEngineContext);
}

bool SRMIsSecurityResourceURI(const char* uri)
{
    if (!uri)
    {
        return false;
    }

    const char *rsrcs[] = {
        OIC_RSRC_SVC_URI,
        OIC_RSRC_AMACL_URI,
        OIC_RSRC_CRL_URI,
        OIC_RSRC_CRED_URI,
        OIC_RSRC_ACL_URI,
        OIC_RSRC_DOXM_URI,
        OIC_RSRC_PSTAT_URI,
        OIC_RSRC_PCONF_URI,
        OIC_RSRC_DPAIRING_URI,
        OIC_RSRC_VER_URI,
        OC_RSRVD_PROV_CRL_URL
    };

    // Remove query from Uri for resource string comparison
    size_t uriLen = strlen(uri);
    char *query = strchr (uri, '?');
    if (query)
    {
        uriLen = query - uri;
    }

    for (size_t i = 0; i < sizeof(rsrcs)/sizeof(rsrcs[0]); i++)
    {
        size_t svrLen = strlen(rsrcs[i]);

        if ((uriLen == svrLen) &&
            (strncmp(uri, rsrcs[i], svrLen) == 0))
        {
            return true;
        }
    }

    return false;
}

/**
 * Check whether persistent storage is valid
 * @return OC_STACK_OK if valid, other errors otherwise;
 */
OCStackResult CheckPersistentStorage(OCPersistentStorage* ps)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    FILE *fp = NULL;
    OCStackResult ret = OC_STACK_SVR_DB_NOT_EXIST;
    OicSecDoxm_t* doxm = NULL;
    OicSecPstat_t* pstat = NULL;
    uint8_t *data = NULL;
    size_t size = 0;

    if (NULL == ps)
    {
        OIC_LOG(DEBUG, TAG, "NULL PersistentStorage Parameter");
        ret = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    // Check whether the DB file exists
    fp = ps->open(SVR_DB_DAT_FILE_NAME, "rb");
    if (NULL == fp)
    {
        OIC_LOG(ERROR, TAG, "DB file cannot be opened");
        ret = OC_STACK_SVR_DB_NOT_EXIST;
        SetPSStatus(PS_OPEN_FAIL);
        goto exit;
    }
    ps->close(fp);

    OIC_LOG(DEBUG, TAG, "Checking doxm resource...");
    //Check DOXM resource
    ret = GetSecureVirtualDatabaseFromPS2(ps, OIC_JSON_DOXM_NAME, &data, &size);
    // If database read failed
    if (OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Can not find the DOXM Resource in SVR DB.");
        ret = OC_STACK_INCONSISTENT_DB;
        SetPSStatus(PS_PARSE_FAIL);
        goto exit;
    }
    if (data && 0 < size)
    {
        // Read DOXM resource from PS
        ret = CBORPayloadToDoxm(data, size, &doxm);
        if (OC_STACK_OK != ret)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to Convert CBOR to Doxm bin : %d", ret);
            ret = OC_STACK_INCONSISTENT_DB;
            SetPSStatus(PS_PARSE_FAIL);
            goto exit;
        }
    }
    else
    {
        ret = OC_STACK_INCONSISTENT_DB;
        SetPSStatus(PS_PARSE_FAIL);
        goto exit;
    }
    if (data)
    {
        OICFree(data);
        data = NULL;
    }

    OIC_LOG(DEBUG, TAG, "Checking pstat resource...");
    //Check PSTAT resource
    ret = GetSecureVirtualDatabaseFromPS2(ps, OIC_JSON_PSTAT_NAME, &data, &size);
    // If database read failed
    if (OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Can not find the PSTAT Resource in SVR DB.");
        ret = OC_STACK_INCONSISTENT_DB;
        SetPSStatus(PS_PARSE_FAIL);
        goto exit;
    }
    if (data && 0 < size)
    {
        // Read ACL resource from PS
        ret = CBORPayloadToPstat(data, size, &pstat);
        if (OC_STACK_OK != ret)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to Convert CBOR to PSTAT bin : %d", ret);
            ret = OC_STACK_INCONSISTENT_DB;
            SetPSStatus(PS_PARSE_FAIL);
            goto exit;
        }
    }
    else
    {
        ret = OC_STACK_INCONSISTENT_DB;
        SetPSStatus(PS_PARSE_FAIL);
        goto exit;
    }
    if (data)
    {
        OICFree(data);
        data = NULL;
    }

    SetPSStatus(PS_NORMAL);

    ret = OC_STACK_OK;
    OIC_LOG(DEBUG, TAG, "All Secure Virtual Resources are fine.");

exit:
    if (data)
    {
        OICFree(data);
    }
    if (doxm)
    {
        DeleteDoxmBinData(doxm);
    }
    if (pstat)
    {
        DeletePstatBinData(pstat);
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return ret;
}

/**
 * Get the Secure Virtual Resource (SVR) type from the URI.
 * @param   uri [IN] Pointer to URI in question.
 * @return  The OicSecSvrType_t of the URI passed (note: if not a Secure Virtual
            Resource, e.g. /a/light, will return "NOT_A_SVR_TYPE" enum value)
 */
static const char URI_QUERY_CHAR = '?';
OicSecSvrType_t GetSvrTypeFromUri(const char* uri)
{
    if (!uri)
    {
        return NOT_A_SVR_RESOURCE;
    }

    // Remove query from Uri for resource string comparison
    size_t uriLen = strlen(uri);
    char *query = strchr (uri, URI_QUERY_CHAR);
    if (query)
    {
        uriLen = query - uri;
    }

    size_t svrLen = 0;

    svrLen = strlen(OIC_RSRC_ACL_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_ACL_URI, svrLen))
        {
            return OIC_R_ACL_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_AMACL_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_AMACL_URI, svrLen))
        {
            return OIC_R_AMACL_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_CRED_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_CRED_URI, svrLen))
        {
            return OIC_R_CRED_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_CRL_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_CRL_URI, svrLen))
        {
            return OIC_R_CRL_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_DOXM_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_DOXM_URI, svrLen))
        {
            return OIC_R_DOXM_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_DPAIRING_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_DPAIRING_URI, svrLen))
        {
            return OIC_R_DPAIRING_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_PCONF_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_PCONF_URI, svrLen))
        {
            return OIC_R_PCONF_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_PSTAT_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_PSTAT_URI, svrLen))
        {
            return OIC_R_PSTAT_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_SVC_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_SVC_URI, svrLen))
        {
            return OIC_R_SVC_TYPE;
        }
    }

    svrLen = strlen(OIC_RSRC_SACL_URI);
    if(uriLen == svrLen)
    {
        if(0 == strncmp(uri, OIC_RSRC_SACL_URI, svrLen))
        {
            return OIC_R_SACL_TYPE;
        }
    }

    return NOT_A_SVR_RESOURCE;
}
