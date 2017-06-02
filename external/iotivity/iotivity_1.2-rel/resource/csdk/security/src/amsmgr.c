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
#include "oic_malloc.h"
#include "amsmgr.h"
#include "resourcemanager.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "logger.h"
#include "ocrandom.h"
#include "aclresource.h"
#include "amaclresource.h"
#include "srmutility.h"
#include "base64.h"
#include "secureresourcemanager.h"
#include "doxmresource.h"
#include "policyengine.h"
#include "oic_string.h"
#include "caremotehandler.h"

#define TAG "OIC_SRM_AMSMGR"

 //Callback for AMS service multicast discovery request.
static OCStackApplicationResult AmsMgrDiscoveryCallback(void *ctx, OCDoHandle handle,
                         OCClientResponse * clientResponse);

//Callback for unicast secured port discovery request.
static OCStackApplicationResult SecurePortDiscoveryCallback(void *ctx, OCDoHandle handle,
                         OCClientResponse * clientResponse);

//Callback for unicast ACL request
static OCStackApplicationResult AmsMgrAclReqCallback(void *ctx, OCDoHandle handle,
    OCClientResponse * clientResponse);

OCStackResult DiscoverAmsService(PEContext_t *context)
{
    OIC_LOG(INFO, TAG, "IN DiscoverAmsService");

    OCStackResult ret = OC_STACK_ERROR;
    const char DOXM_DEVICEID_QUERY_FMT[] = "%s?%s=%s";
    char uri[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    OCCallbackData cbData = {.context=NULL};

    VERIFY_NON_NULL(TAG, context, ERROR);
    snprintf(uri, sizeof(uri), DOXM_DEVICEID_QUERY_FMT, OIC_RSRC_DOXM_URI,
                                       OIC_JSON_DEVICE_ID_NAME,
                                       context->amsMgrContext->amsDeviceId.id);

    cbData.cb = &AmsMgrDiscoveryCallback;
    cbData.context = (void*)context;

    /* TODO
     * If no good response was received for this discovery request,
     * PE would be blocked forever waiting for AMS service to respond with the ACE.
     * Need logic to reset the PE state and send ACCESS_DENIED response,
     * when discovery response from AMS service is not received within certain time.
     */
    OIC_LOG_V(INFO, TAG,"AMS Manager Sending Multicast Discovery with URI = %s", uri);
    ret = OCDoResource(NULL, OC_REST_DISCOVER, uri, NULL, NULL,
                       CT_DEFAULT, OC_LOW_QOS, &cbData, NULL, 0);

exit:
    OIC_LOG(INFO, TAG, "Leaving DiscoverAmsService");
    return ret;
}

static OCStackApplicationResult AmsMgrDiscoveryCallback(void *ctx, OCDoHandle handle,
                         OCClientResponse * clientResponse)
{
    OIC_LOG_V(INFO, TAG, "%s Begin", __func__ );

    if (!ctx ||
        !clientResponse ||
        !clientResponse->payload||
        (PAYLOAD_TYPE_SECURITY != clientResponse->payload->type)||
        (OC_STACK_OK != clientResponse->result))
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid Response ", __func__);
        return OC_STACK_KEEP_TRANSACTION;
    }

    (void)handle;
    PEContext_t *context = (PEContext_t *) ctx;
    if (context->state != AWAITING_AMS_RESPONSE)
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid PE State ", __func__);
        return OC_STACK_DELETE_TRANSACTION;
    }

    OicSecDoxm_t *doxm = NULL;

    OIC_LOG_V(INFO, TAG, "Doxm DeviceId Discovery response = %s\n",
          ((OCSecurityPayload*)clientResponse->payload)->securityData);
    uint8_t *payload = ((OCSecurityPayload*)clientResponse->payload)->securityData;
    size_t size = ((OCSecurityPayload*)clientResponse->payload)->payloadSize;

    //As doxm is NULL amsmgr can't test if response from trusted AMS service
    //so keep the transaction.
    if (OC_STACK_OK == CBORPayloadToDoxm(payload, size, &doxm))
    {
        OIC_LOG_V(ERROR, TAG, "%s : Unable to convert CBOR to Binary",__func__);
        return OC_STACK_KEEP_TRANSACTION;
    }

    OicUuid_t deviceId = {.id={0}};
    memcpy(&deviceId, &doxm->deviceID, sizeof(deviceId));
    DeleteDoxmBinData(doxm);

    /* TODO : By assuming that the first response received is the actual
     * AMS service, a 'bad device' can cause DoS attack.
     */
    if (memcmp(&context->amsMgrContext->amsDeviceId, &deviceId,
            sizeof(context->amsMgrContext->amsDeviceId)) == 0)
    {
        OIC_LOG(INFO, TAG, "AMS Manager Sending unicast discovery to get secured port info");
        //Sending Unicast discovery to get secure port information
        if (OC_STACK_OK == SendUnicastSecurePortDiscovery(context, &clientResponse->devAddr,
                clientResponse->connType))
        {
            context->retVal = ACCESS_WAITING_FOR_AMS;
            return OC_STACK_DELETE_TRANSACTION;
        }
    }
    context->retVal = ACCESS_DENIED_AMS_SERVICE_ERROR;
    SRMSendResponse(context->retVal);
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult SendUnicastSecurePortDiscovery(PEContext_t *context,OCDevAddr *devAddr,
                                      OCConnectivityType connType)
{
    OIC_LOG(INFO, TAG, "IN SendUnicastSecurePortDiscovery");

    const char RES_DOXM_QUERY_FMT[] = "%s?%s=%s";
    OCCallbackData cbData = {.context=NULL};
    char uri[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    snprintf(uri, sizeof(uri), RES_DOXM_QUERY_FMT, OC_RSRVD_WELL_KNOWN_URI,
            OC_RSRVD_RESOURCE_TYPE, OIC_RSRC_TYPE_SEC_DOXM);

    cbData.cb = &SecurePortDiscoveryCallback;
    cbData.context = context;

    OIC_LOG_V(INFO, TAG, "AMS Manager Sending Unicast Discovery with URI = %s", uri);

    return  OCDoResource(NULL, OC_REST_DISCOVER, uri, devAddr, NULL,
                         connType, OC_LOW_QOS, &cbData, NULL, 0);
}

static OCStackApplicationResult SecurePortDiscoveryCallback(void *ctx, OCDoHandle handle,
                         OCClientResponse * clientResponse)
{
    OIC_LOG(INFO, TAG, "In SecurePortDiscoveryCallback");

    if (!ctx ||
        !clientResponse ||
        !clientResponse->payload||
        (PAYLOAD_TYPE_DISCOVERY != clientResponse->payload->type)||
        (OC_STACK_OK != clientResponse->result))
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid Response ", __func__);
        SRMSendResponse(ACCESS_DENIED_AMS_SERVICE_ERROR);
        return OC_STACK_DELETE_TRANSACTION;
    }

    PEContext_t *context = (PEContext_t *) ctx;

    (void)handle;
    if (context->state != AWAITING_AMS_RESPONSE)
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid PE State ", __func__);
        context->retVal = ACCESS_DENIED_AMS_SERVICE_ERROR;
        SRMSendResponse(context->retVal);
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCResourcePayload *resPayload = ((OCDiscoveryPayload*)clientResponse->payload)->resources;

    //Verifying if the ID of the sender is an AMS service that this device trusts.
    if(resPayload &&
       memcmp(context->amsMgrContext->amsDeviceId.id,
            ((OCDiscoveryPayload*)clientResponse->payload)->sid,
            // resPayload->sid,
                    sizeof(context->amsMgrContext->amsDeviceId.id)) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid AMS device", __func__);
        context->retVal = ACCESS_DENIED_AMS_SERVICE_ERROR;
        SRMSendResponse(context->retVal);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (resPayload && resPayload->secure)
    {
        if(OC_STACK_OK == SendAclReq(context, &clientResponse->devAddr, clientResponse->connType,
                resPayload->port))
        {
            return OC_STACK_DELETE_TRANSACTION;
        }
    }
    OIC_LOG(INFO, TAG, "Can not find secure port information");

    context->retVal = ACCESS_DENIED_AMS_SERVICE_ERROR;
    SRMSendResponse(context->retVal);
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult SendAclReq(PEContext_t *context, OCDevAddr *devAddr, OCConnectivityType connType,
        uint16_t securedPort)
{
    OCStackResult ret = OC_STACK_ERROR;
    const char GET_ACE_QUERY_FMT[] = "%s?%s=%s;%s=%s";
    char uri[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    OCCallbackData cbData = {.context=NULL};
    OCDevAddr destAddr = {.adapter = OC_ADAPTER_IP};
    char *subID = NULL;

    VERIFY_NON_NULL(TAG, context, ERROR);
    VERIFY_NON_NULL(TAG, devAddr, ERROR);

    ret = ConvertUuidToStr(&context->subject, &subID);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "SendAclReq : Failed to canonical UUID encoding");
        return OC_STACK_ERROR;
    }

    snprintf(uri, sizeof(uri), GET_ACE_QUERY_FMT, OIC_RSRC_ACL_URI,
                                    OIC_JSON_SUBJECTID_NAME, subID,
                                    OIC_JSON_RESOURCES_NAME, context->resource);
    OICFree(subID);

    cbData.cb = &AmsMgrAclReqCallback;
    cbData.context = context;

    destAddr = *devAddr;
    //update port info
    destAddr.flags = (OCTransportFlags)(destAddr.flags | OC_FLAG_SECURE);
    destAddr.port = securedPort;

    OIC_LOG_V(INFO, TAG, "AMS Manager Sending Unicast ACL request with URI = %s", uri);
    ret = OCDoResource(NULL, OC_REST_GET, uri, &destAddr, NULL,
            connType, OC_LOW_QOS, &cbData, NULL, 0);

exit:
    OIC_LOG_V(INFO, TAG, "%s returns %d ", __func__, ret);
    return ret;
}

static OCStackApplicationResult AmsMgrAclReqCallback(void *ctx, OCDoHandle handle,
    OCClientResponse * clientResponse)
{
    OIC_LOG_V(INFO, TAG, "%s Begin", __func__ );

    (void)handle;
    PEContext_t *context = (PEContext_t *) ctx;
    SRMAccessResponse_t rsps;

    if (!ctx ||
        !clientResponse ||
        !clientResponse->payload||
        (PAYLOAD_TYPE_SECURITY != clientResponse->payload->type) ||
        (clientResponse->result != OC_STACK_OK))
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid Response ", __func__);
        SRMSendResponse(ACCESS_DENIED_AMS_SERVICE_ERROR);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (context->state != AWAITING_AMS_RESPONSE)
    {
        OIC_LOG_V(ERROR, TAG, "%s Invalid State ", __func__);
        context->retVal = ACCESS_DENIED_AMS_SERVICE_ERROR;
        SRMSendResponse(context->retVal);
        return OC_STACK_DELETE_TRANSACTION;
    }

    // Verify before installing ACL if the ID of the sender of this ACL is an AMS
    //service that this device trusts.
    rsps = ACCESS_DENIED;
    if((UUID_LENGTH == clientResponse->identity.id_length) &&
        memcmp(context->amsMgrContext->amsDeviceId.id, clientResponse->identity.id,
                       sizeof(context->amsMgrContext->amsDeviceId.id)) == 0)
    {
        size_t size = ((OCSecurityPayload*)clientResponse->payload)->payloadSize;
        OCStackResult ret =
                AppendACL(((OCSecurityPayload*)clientResponse->payload)->securityData, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OIC_LOG_V(INFO, TAG, "%s : Calling checkPermission", __func__);
        rsps = CheckPermission(context, &context->subject, context->resource, context->permission);
        VERIFY_SUCCESS(TAG, (true == IsAccessGranted(rsps)), ERROR);

        OIC_LOG_V(INFO, TAG, "%sAccess granted, Calling SRMCallCARequestHandler", __func__);
        context->retVal = ACCESS_GRANTED;
        SRMSendResponse(context->retVal);
        return OC_STACK_DELETE_TRANSACTION;
    }

exit:
    context->retVal = ACCESS_DENIED_AMS_SERVICE_ERROR;
    SRMSendResponse(context->retVal);
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult UpdateAmsMgrContext(PEContext_t *context, const CAEndpoint_t *endpoint,
                        const CARequestInfo_t *requestInfo)
{
    OCStackResult ret = OC_STACK_INVALID_PARAM;
    VERIFY_NON_NULL(TAG, context->amsMgrContext, ERROR);
    ret = OC_STACK_ERROR;

    //The AmsMgr context endpoint and requestInfo will be free from ,
    //AmsMgrAclReqCallback function
    if (context->amsMgrContext->endpoint)
    {
        OICFree(context->amsMgrContext->endpoint);
        context->amsMgrContext->endpoint = NULL;
    }
    context->amsMgrContext->endpoint = (CAEndpoint_t *)OICCalloc(1, sizeof(CAEndpoint_t ));
    VERIFY_NON_NULL(TAG, context->amsMgrContext->endpoint, ERROR);
    *context->amsMgrContext->endpoint = *endpoint;

    if (context->amsMgrContext->requestInfo)
    {
        FreeCARequestInfo(context->amsMgrContext->requestInfo);
        context->amsMgrContext->requestInfo = NULL;
    }
    context->amsMgrContext->requestInfo = CACloneRequestInfo(requestInfo);
    VERIFY_NON_NULL(TAG, context->amsMgrContext->requestInfo, ERROR);
    ret = OC_STACK_OK;
exit:
    return ret;
}

void FreeCARequestInfo(CARequestInfo_t *requestInfo)
{
    if (NULL == requestInfo)
    {
        OIC_LOG_V(DEBUG, TAG, "%s: Can't free memory. Received NULL requestInfo", __func__);
        return;
    }
    OICFree(requestInfo->info.token);
    OICFree(requestInfo->info.options);
    OICFree(requestInfo->info.payload);
    OICFree(requestInfo->info.resourceUri);
    OICFree(requestInfo);
}


//This method checks for Amacl resource. If Amacl is found then it fills up
//context->amsMgrContext->amsDeviceId with amsID of the Amacl else leaves it empty.
bool FoundAmaclForRequest(PEContext_t *context)
{
    OIC_LOG_V(INFO, TAG, "%s:no ACL found. Searching for AMACL",__func__);

    bool ret = false;
    VERIFY_NON_NULL(TAG, context, ERROR);
    memset(&context->amsMgrContext->amsDeviceId, 0, sizeof(context->amsMgrContext->amsDeviceId));

    //Call amacl resource function to get the AMS service deviceID for the resource
    if (OC_STACK_OK == AmaclGetAmsDeviceId(context->resource, &context->amsMgrContext->amsDeviceId))
    {
        OIC_LOG_V(INFO, TAG, "%s:AMACL found for the requested resource %s",
                __func__, context->resource);
        ret = true;
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "%s:AMACL found for the requested resource %s",
                __func__, context->resource);
        ret = false;
    }

 exit:
     return ret;
}

void ProcessAMSRequest(PEContext_t *context)
{
    OicUuid_t  emptyUuid = {.id={0}};
    OIC_LOG_V(INFO, TAG, "Entering %s", __func__);
    if (NULL != context)
    {
        if((ACCESS_GRANTED != context->retVal) && (false == context->amsProcessing))
        {
            context->amsProcessing = true;

            //Checking if context AMS deviceId is empty
            if(memcmp(&context->amsMgrContext->amsDeviceId, &emptyUuid, sizeof(OicUuid_t)) != 0 )
            {
                if(OC_STACK_OK == DiscoverAmsService(context))
                {
                    context->retVal = ACCESS_WAITING_FOR_AMS;
                    OIC_LOG_V(INFO, TAG, "Leaving %s(WAITING_FOR_AMS)", __func__);
                    context->state = AWAITING_AMS_RESPONSE;
                }
                else
                {
                    context->retVal = ACCESS_DENIED_AMS_SERVICE_ERROR;
                }
            }
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "Leaving %s(context is NULL)", __func__);
    }
}
