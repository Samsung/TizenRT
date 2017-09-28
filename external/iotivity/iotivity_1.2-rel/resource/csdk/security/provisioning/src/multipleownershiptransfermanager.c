/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <stdbool.h>
#include <string.h>

#include "utlist.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cacommon.h"
#include "cainterface.h"
#include "base64.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif

#include "srmresourcestrings.h"
#include "doxmresource.h"
#include "pstatresource.h"
#include "credresource.h"
#include "aclresource.h"
#include "ownershiptransfermanager.h"
#include "securevirtualresourcetypes.h"
#include "oxmjustworks.h"
#include "pmtypes.h"
#include "pmutility.h"
#include "srmutility.h"
#include "provisioningdatabasemanager.h"
#include "oxmrandompin.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "oxmjustworks.h"
#include "oxmpreconfpin.h"
#include "oxmrandompin.h"
#include "otmcontextlist.h"

#define TAG "OIC_MULTIPLE_OTM"

/**********************************************************************
 * API for Super Owner
 **********************************************************************/

/**
 * Callback handler of security resource's POST request.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult MOTUpdateSecurityResourceCB(void *ctx, OCDoHandle UNUSED,
                                                OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    (void)UNUSED;
    OTMContext_t *motCtx = (OTMContext_t*)ctx;
    VERIFY_NON_NULL(TAG, motCtx, ERROR);
    VERIFY_NON_NULL(TAG, motCtx->ctxResultCallback, ERROR);
    VERIFY_NON_NULL(TAG, motCtx->ctxResultArray, ERROR);

    if(clientResponse)
    {
        memcpy(motCtx->ctxResultArray[0].deviceId.id,
               motCtx->selectedDeviceInfo->doxm->deviceID.id,
               sizeof(OicUuid_t));
        motCtx->ctxResultArray[0].res = clientResponse->result;

        if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            motCtx->ctxHasError = false;
        }
        else
        {
            motCtx->ctxHasError = true;
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "SRPGetACLResourceCB received Null clientResponse");
        motCtx->ctxResultArray[0].res = OC_STACK_ERROR;
        motCtx->ctxHasError = true;
    }

    motCtx->ctxResultCallback(motCtx->userCtx, motCtx->ctxResultArraySize,
                              motCtx->ctxResultArray, motCtx->ctxHasError);

exit:
    if(motCtx)
    {
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Callback handler of security resource's DELETE request.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult MOTDeleteSecurityResourceCB(void *ctx, OCDoHandle UNUSED,
                                                OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);
    (void)UNUSED;
    OTMContext_t *motCtx = (OTMContext_t*)ctx;
    VERIFY_NON_NULL(TAG, motCtx, ERROR);
    VERIFY_NON_NULL(TAG, motCtx->ctxResultCallback, ERROR);
    VERIFY_NON_NULL(TAG, motCtx->ctxResultArray, ERROR);

    if(clientResponse)
    {
        memcpy(motCtx->ctxResultArray[0].deviceId.id,
               motCtx->selectedDeviceInfo->doxm->deviceID.id,
               sizeof(OicUuid_t));
        motCtx->ctxResultArray[0].res = clientResponse->result;

        if(OC_STACK_RESOURCE_DELETED == clientResponse->result)
        {
            motCtx->ctxHasError = false;
        }
        else
        {
            motCtx->ctxHasError = true;
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "SRPGetACLResourceCB received Null clientResponse");
        motCtx->ctxResultArray[0].res = OC_STACK_ERROR;
        motCtx->ctxHasError = true;
    }

    motCtx->ctxResultCallback(motCtx->userCtx, motCtx->ctxResultArraySize,
                              motCtx->ctxResultArray, motCtx->ctxHasError);

exit:
    if(motCtx)
    {
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }
    OIC_LOG_V(INFO, TAG, "Out %s", __func__);

    return OC_STACK_DELETE_TRANSACTION;

}

/**
 * Internal API to send POST doxm request
 */
static OCStackResult MOTSendPostDoxm(void *ctx,
                                     const OCProvisionDev_t *targetDeviceInfo,
                                     OCProvisionResultCB resultCallback,
                                     const OicSecDoxm_t* doxm)
{
    OCStackResult postMomRes = OC_STACK_ERROR;
    OCSecurityPayload* secPayload = NULL;
    OTMContext_t *motCtx = NULL;
    bool freeFlag = true;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    //Generate the security payload using updated doxm
    secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    VERIFY_NON_NULL(TAG, secPayload, ERROR);
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    postMomRes = DoxmToCBORPayload(doxm, &secPayload->securityData, &secPayload->payloadSize, true);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    OIC_LOG(DEBUG, TAG, "Created doxm payload to update doxm:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    bool queryGenRes = PMGenerateQuery(true,
                                       targetDeviceInfo->endpoint.addr,
                                       targetDeviceInfo->securePort,
                                       targetDeviceInfo->connType,
                                       query, sizeof(query), OIC_RSRC_DOXM_URI);
    VERIFY_SUCCESS(TAG, (true == queryGenRes), ERROR);
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    //Create the MOT Context to handle the response message
    motCtx = (OTMContext_t*)OICCalloc(1, sizeof(OTMContext_t));
    VERIFY_NON_NULL(TAG, motCtx, ERROR);
    motCtx->selectedDeviceInfo = targetDeviceInfo;
    motCtx->ctxResultCallback = resultCallback;
    motCtx->ctxResultArraySize = 1;
    motCtx->ctxHasError = false;
    motCtx->userCtx = ctx;
    motCtx->ctxResultArray= (OCProvisionResult_t*)OICCalloc(1, sizeof(OCProvisionResult_t));
    VERIFY_NON_NULL(TAG, motCtx->ctxResultArray, ERROR);

    //Send POST request
    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &MOTUpdateSecurityResourceCB;
    cbData.context = (void *)motCtx;
    OIC_LOG(DEBUG, TAG, "Sending POST 'doxm' request to resource server");
    postMomRes = OCDoResource(NULL, OC_REST_POST, query,
                              &targetDeviceInfo->endpoint, (OCPayload*)secPayload,
                              targetDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    freeFlag = false;

exit:
    //If POST request successfully sent, motCtx will be cleaned from response handler.
    if(freeFlag && motCtx)
    {
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return postMomRes;
}

/**
 * API to update 'doxm.mom' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] momType Mode of multiple ownership transfer (ref. oic.sec.mom)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'mom' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTChangeMode(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                            const OicSecMomType_t momType, OCProvisionResultCB resultCallback)
{
    OCStackResult postMomRes = OC_STACK_INVALID_PARAM;
    OicSecDoxm_t* doxm = NULL;
    uint8_t* doxmPayload = NULL;
    size_t doxmPayloadLen = 0;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    VERIFY_SUCCESS(TAG, (OIC_NUMBER_OF_MOM_TYPE > momType), ERROR);
    VERIFY_NON_NULL(TAG, targetDeviceInfo, ERROR);
    postMomRes = OC_STACK_INVALID_CALLBACK;
    VERIFY_NON_NULL(TAG, resultCallback, ERROR);

    //Dulpicate doxm resource to update the 'mom' property
    postMomRes = DoxmToCBORPayload(targetDeviceInfo->doxm, &doxmPayload, &doxmPayloadLen, false);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    postMomRes = CBORPayloadToDoxm(doxmPayload, doxmPayloadLen, &doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);
    VERIFY_NON_NULL(TAG, doxm, ERROR);

    if(NULL == doxm->mom)
    {
        postMomRes = OC_STACK_NO_MEMORY;
        doxm->mom = (OicSecMom_t*)OICCalloc(1, sizeof(OicSecMom_t));
        VERIFY_NON_NULL(TAG, (doxm->mom), ERROR);
    }
    doxm->mom->mode = momType;

    //Send POST reuqest for update doxm
    postMomRes = MOTSendPostDoxm(ctx, targetDeviceInfo, resultCallback, doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

exit:
    OICFree(doxmPayload);
    DeleteDoxmBinData(doxm);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return postMomRes;
}

/**
 * API to add 'doxm.oxms' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] newOxm  OxMs to be added (ref. oic.sec.oxm)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxms' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTAddMOTMethod(void *ctx, OCProvisionDev_t *targetDeviceInfo,
                                 const OicSecOxm_t newOxm, OCProvisionResultCB resultCallback)
{
    OCStackResult postOxmRes = OC_STACK_INVALID_PARAM;
    OicSecOxm_t* newOxms = NULL;
    uint8_t* doxmPayload = NULL;
    size_t doxmPayloadLen = 0;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    VERIFY_SUCCESS(TAG, (OIC_OXM_COUNT > newOxm || OIC_PRECONFIG_PIN == newOxm), ERROR);
    VERIFY_NON_NULL(TAG, targetDeviceInfo, ERROR);
    postOxmRes = OC_STACK_INVALID_CALLBACK;
    VERIFY_NON_NULL(TAG, resultCallback, ERROR);
    postOxmRes = OC_STACK_NO_MEMORY;

    for(size_t i = 0; i < targetDeviceInfo->doxm->oxmLen; i++)
    {
        if(targetDeviceInfo->doxm->oxm[i] == newOxm)
        {
            OIC_LOG_V(INFO, TAG, "[%d] OxM already supported", (int)newOxm);
            OCProvisionResult_t* resArr = (OCProvisionResult_t*)OICCalloc(1, sizeof(OCProvisionResult_t));
            VERIFY_NON_NULL(TAG, resArr, ERROR);
            resArr->res = OC_STACK_OK;
            memcpy(resArr->deviceId.id, targetDeviceInfo->doxm->deviceID.id, sizeof(resArr->deviceId.id));
            resultCallback(ctx, 1, resArr, false);
            return OC_STACK_OK;
        }
    }

    newOxms = (OicSecOxm_t*)OICMalloc(sizeof(OicSecOxm_t) * (targetDeviceInfo->doxm->oxmLen + 1));
    VERIFY_NON_NULL(TAG, newOxms , ERROR);

    for(size_t i = 0; i < targetDeviceInfo->doxm->oxmLen; i++)
    {
        newOxms[i] = targetDeviceInfo->doxm->oxm[i];
    }
    newOxms[targetDeviceInfo->doxm->oxmLen] = newOxm;
    targetDeviceInfo->doxm->oxmLen++;
    OICFree(targetDeviceInfo->doxm->oxm);
    targetDeviceInfo->doxm->oxm = newOxms;

    //Send POST reuqest for update doxm
    postOxmRes = MOTSendPostDoxm(ctx, targetDeviceInfo, resultCallback, targetDeviceInfo->doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postOxmRes), ERROR);

exit:
    OICFree(doxmPayload);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return postOxmRes;
}

/**
 * API to update 'doxm.oxmsel' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
  * @param[in] oxmSelValue Method of multiple ownership transfer (ref. oic.sec.oxm)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxmsel' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTSelectMOTMethod(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                                 const OicSecOxm_t oxmSelValue, OCProvisionResultCB resultCallback)
{
    OCStackResult postMomRes = OC_STACK_INVALID_CALLBACK;
    OicSecDoxm_t* doxm = NULL;
    uint8_t* doxmPayload = NULL;
    size_t doxmPayloadLen = 0;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    VERIFY_NON_NULL(TAG, resultCallback, ERROR);
    postMomRes = OC_STACK_INVALID_PARAM;
    VERIFY_NON_NULL(TAG, targetDeviceInfo, ERROR);

    bool isValidOxmsel = false;
    for(size_t i = 0; i < targetDeviceInfo->doxm->oxmLen; i++)
    {
        if(targetDeviceInfo->doxm->oxm[i] == oxmSelValue)
        {
            isValidOxmsel = true;
            break;
        }
    }
    VERIFY_SUCCESS(TAG, isValidOxmsel, ERROR);

    //Dulpicate doxm resource to update the 'oxmsel' property
    postMomRes = DoxmToCBORPayload(targetDeviceInfo->doxm, &doxmPayload, &doxmPayloadLen, false);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    postMomRes = CBORPayloadToDoxm(doxmPayload, doxmPayloadLen, &doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);
    VERIFY_NON_NULL(TAG, doxm, ERROR);

    doxm->oxmSel = oxmSelValue;

    //Send POST reuqest for update doxm
    postMomRes = MOTSendPostDoxm(ctx, targetDeviceInfo, resultCallback, doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

exit:
    OICFree(doxmPayload);
    DeleteDoxmBinData(doxm);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return postMomRes;
}

/**
 * API to provision preconfigured PIN to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfPIN Preconfig PIN which is used while multiple owner authentication
 * @param[in] preconfPINLen Byte length of preconfig PIN
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST credential request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTProvisionPreconfigPIN(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                                 const char* preconfPIN, size_t preconfPINLen, OCProvisionResultCB resultCallback)
{
    OCStackResult postCredRes = OC_STACK_INVALID_CALLBACK;
    bool freeFlag = true;
    OCSecurityPayload* secPayload = NULL;
    OTMContext_t *motCtx = NULL;
    OicSecCred_t* pinCred = NULL;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    VERIFY_NON_NULL(TAG, resultCallback, ERROR);
    postCredRes = OC_STACK_INVALID_PARAM;
    VERIFY_NON_NULL(TAG, targetDeviceInfo, ERROR);
    VERIFY_NON_NULL(TAG, preconfPIN, ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen), ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen && OXM_PRECONFIG_PIN_MAX_SIZE >= preconfPINLen), ERROR);

    postCredRes = OC_STACK_NO_MEMORY;
    //Generate PIN based credential
    pinCred = (OicSecCred_t*)OICCalloc(1, sizeof(OicSecCred_t));
    VERIFY_NON_NULL(TAG, pinCred, ERROR);

    pinCred->privateData.data = (uint8_t*)OICMalloc(preconfPINLen + 1);
    VERIFY_NON_NULL(TAG, pinCred->privateData.data, ERROR);

    memcpy(pinCred->privateData.data, preconfPIN, preconfPINLen);
    pinCred->privateData.data[preconfPINLen] = '\0';
    pinCred->privateData.len = preconfPINLen;
    pinCred->privateData.encoding = OIC_ENCODING_RAW;
    pinCred->credType = PIN_PASSWORD;
    OICStrcpy(pinCred->subject.id, sizeof(pinCred->subject.id), WILDCARD_SUBJECT_ID.id);

    //Generate the security payload using updated doxm
    secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    VERIFY_NON_NULL(TAG, secPayload, ERROR);
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    postCredRes = CredToCBORPayload(pinCred, &secPayload->securityData, &secPayload->payloadSize, false);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postCredRes), ERROR);

    OIC_LOG(DEBUG, TAG, "Created Credential payload to register PIN credential:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    bool queryGenRes = PMGenerateQuery(true,
                                       targetDeviceInfo->endpoint.addr,
                                       targetDeviceInfo->securePort,
                                       targetDeviceInfo->connType,
                                       query, sizeof(query), OIC_RSRC_CRED_URI);
    VERIFY_SUCCESS(TAG, (true == queryGenRes), ERROR);
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    //Create the MOT Context to handle the response message
    motCtx = (OTMContext_t*)OICCalloc(1, sizeof(OTMContext_t));
    VERIFY_NON_NULL(TAG, motCtx, ERROR);
    motCtx->selectedDeviceInfo= targetDeviceInfo;
    motCtx->ctxResultCallback = resultCallback;
    motCtx->ctxResultArraySize =1;
    motCtx->ctxHasError = false;
    motCtx->userCtx = ctx;
    motCtx->ctxResultArray = (OCProvisionResult_t*)OICCalloc(1, sizeof(OCProvisionResult_t));
    VERIFY_NON_NULL(TAG, motCtx->ctxResultArray, ERROR);

    //Send POST request
    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &MOTUpdateSecurityResourceCB;
    cbData.context = (void *)motCtx;
    OIC_LOG(DEBUG, TAG, "Sending POST Preconfiged PIN credenatial request to resource server");
    postCredRes = OCDoResource(NULL, OC_REST_POST, query,
                              &targetDeviceInfo->endpoint, (OCPayload*)secPayload,
                              targetDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postCredRes), ERROR);

    freeFlag = false;

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return postCredRes;

exit:
    //If POST request successfully sent, motCtx will be cleaned from response handler.
    if(freeFlag && motCtx)
    {
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }
    if(pinCred)
    {
        OICFree(pinCred->privateData.data);
        OICFree(pinCred);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s : %d", __func__, postCredRes);
    return postCredRes;
}


/**********************************************************************
 * API for Sub Owner
 **********************************************************************/

static OCStackResult StartMultipleOwnershipTransfer(OTMContext_t* motCtx,
                                                    OCProvisionDev_t* selectedDevice);

static OTMContext_t* g_MotCtx = NULL;

static bool IsComplete(OTMContext_t* otmCtx)
{
    for(size_t i = 0; i < otmCtx->ctxResultArraySize; i++)
    {
        if(OC_STACK_CONTINUE == otmCtx->ctxResultArray[i].res)
        {
            return false;
        }
    }

    return true;
}

/**
 * Function to save the result of multiple ownership transfer.
 *
 * @param[in,out] motCtx   Context instance of multiple ownership transfer.
 * @param[in] res   result of multiple ownership transfer.
 */
static void SetMOTResult(OTMContext_t* motCtx, const OCStackResult res)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s : %d ", __func__, res);
    VERIFY_NON_NULL(TAG, motCtx, ERROR);

    if(motCtx->selectedDeviceInfo)
    {
        //Revert psk_info callback in case of random PIN OxM
        if(OIC_RANDOM_DEVICE_PIN == motCtx->selectedDeviceInfo->doxm->oxmSel ||
           OIC_PRECONFIG_PIN == motCtx->selectedDeviceInfo->doxm->oxmSel)
        {
            if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskCredentials))
            {
                OIC_LOG(WARNING, TAG, "Failed to revert the DTLS credential handler.");
            }
            OicUuid_t emptyUuid = { .id={0}};
            SetUuidForPinBasedOxm(&emptyUuid);
        }

        OCStackResult pdmRetVal = PDMSetDeviceState(&motCtx->selectedDeviceInfo->doxm->deviceID,
                                                    PDM_DEVICE_ACTIVE);
        if (OC_STACK_OK != pdmRetVal)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to add device information into PDM_DB : %d", res);
        }

        for(size_t i = 0; i < motCtx->ctxResultArraySize; i++)
        {
            if(memcmp(motCtx->selectedDeviceInfo->doxm->deviceID.id,
                      motCtx->ctxResultArray[i].deviceId.id, UUID_LENGTH) == 0)
            {
                motCtx->ctxResultArray[i].res = res;
                if(OC_STACK_OK != res)
                {
                    motCtx->ctxHasError = true;
                }
            }
        }

        //Remove the current OTM Context from OTM queue
        RemoveOTMContext(motCtx->selectedDeviceInfo->endpoint.addr,
                         motCtx->selectedDeviceInfo->securePort);

        //If there is a request being performed, cancel it to prevent retransmission.
        if(motCtx->ocDoHandle)
        {
            if (OC_STACK_OK !=  OCCancel(motCtx->ocDoHandle, OC_HIGH_QOS, NULL, 0))
            {
                OIC_LOG(ERROR, TAG, "Failed to remove registered callback");
            }
            motCtx->ocDoHandle = NULL;
        }

        //If all request is completed, invoke the user callback.
        if(IsComplete(motCtx))
        {
            motCtx->ctxResultCallback(motCtx->userCtx, motCtx->ctxResultArraySize,
                                       motCtx->ctxResultArray, motCtx->ctxHasError);

            OICFree(motCtx->ctxResultArray);
            OICFree(motCtx);
            motCtx = NULL;
        }
        else
        {
            if(OC_STACK_OK != StartMultipleOwnershipTransfer(motCtx,
                                                     motCtx->selectedDeviceInfo->next))
            {
                OIC_LOG(ERROR, TAG, "Failed to StartMultipleOwnershipTransfer");
            }
        }
    }

exit:
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

/**
 * API to add preconfigured PIN to local SVR DB.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfPIN Preconfig PIN which is used while multiple owner authentication
 * @param[in] preconfPINLen Byte length of preconfig PIN
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST credential request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTAddPreconfigPIN(const OCProvisionDev_t *targetDeviceInfo,
                                 const char* preconfPIN, size_t preconfPINLen)
{
    OCStackResult addCredRes = OC_STACK_INVALID_PARAM;
    OicSecCred_t* pinCred = NULL;
    bool freeFlag = true;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    VERIFY_NON_NULL(TAG, targetDeviceInfo, ERROR);
    VERIFY_NON_NULL(TAG, preconfPIN, ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen), ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen && OXM_PRECONFIG_PIN_MAX_SIZE >= preconfPINLen), ERROR);

    OicSecCred_t* prevCred = GetCredResourceData(&targetDeviceInfo->doxm->deviceID);
    if(NULL != prevCred)
    {
        OIC_LOG(INFO, TAG, "PIN/PW Credential already exist!");
        return OC_STACK_OK;
    }

    addCredRes = OC_STACK_NO_MEMORY;
    //Generate PIN based credential
    pinCred = (OicSecCred_t*)OICCalloc(1, sizeof(OicSecCred_t));
    VERIFY_NON_NULL(TAG, pinCred, ERROR);

    pinCred->privateData.data = (uint8_t*)OICMalloc(preconfPINLen + 1);
    VERIFY_NON_NULL(TAG, pinCred->privateData.data, ERROR);

    memcpy(pinCred->privateData.data, preconfPIN, preconfPINLen);
    pinCred->privateData.data[preconfPINLen] = '\0';
    pinCred->privateData.len = preconfPINLen;
    pinCred->privateData.encoding = OIC_ENCODING_RAW;
    pinCred->credType = PIN_PASSWORD;
    memcpy(pinCred->subject.id, targetDeviceInfo->doxm->deviceID.id, sizeof(pinCred->subject.id));

    addCredRes = AddCredential(pinCred);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == addCredRes), ERROR);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return addCredRes;

exit:
    if(pinCred)
    {
        OICFree(pinCred->privateData.data);
        OICFree(pinCred);
    }
    OIC_LOG_V(DEBUG, TAG, "In %s : %d", __func__, addCredRes);
    return addCredRes;
}

/**
 * Function to save the SubOwner PSK.
 *
 * @param[in] selectedDeviceInfo   selected device information to performing provisioning.
 * @return  OC_STACK_OK on success
 */
static OCStackResult SaveSubOwnerPSK(OCProvisionDev_t *selectedDeviceInfo)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    OCStackResult res = OC_STACK_ERROR;

    CAEndpoint_t endpoint;
    memset(&endpoint, 0x00, sizeof(CAEndpoint_t));
    OICStrcpy(endpoint.addr, MAX_ADDR_STR_SIZE_CA, selectedDeviceInfo->endpoint.addr);
    endpoint.addr[MAX_ADDR_STR_SIZE_CA - 1] = '\0';
    endpoint.port = selectedDeviceInfo->securePort;
    endpoint.adapter = selectedDeviceInfo->endpoint.adapter;

    OicUuid_t ownerDeviceID = {.id={0}};
    if (OC_STACK_OK != GetDoxmDeviceID(&ownerDeviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving SubOwner's device ID");
        return res;
    }

    uint8_t ownerPSK[OWNER_PSK_LENGTH_128] = {0};
    OicSecKey_t ownerKey = {ownerPSK, OWNER_PSK_LENGTH_128};

    //Generating SubOwnerPSK
    CAResult_t pskRet = CAGenerateOwnerPSK(&endpoint,
            (uint8_t *)GetOxmString(selectedDeviceInfo->doxm->oxmSel),
            strlen(GetOxmString(selectedDeviceInfo->doxm->oxmSel)),
            ownerDeviceID.id, sizeof(ownerDeviceID.id),
            selectedDeviceInfo->doxm->deviceID.id, sizeof(selectedDeviceInfo->doxm->deviceID.id),
            ownerPSK, OWNER_PSK_LENGTH_128);

    if (CA_STATUS_OK == pskRet)
    {
        OIC_LOG(DEBUG, TAG, "SubOwner PSK dump:");
        OIC_LOG_BUFFER(DEBUG, TAG, ownerPSK, OWNER_PSK_LENGTH_128);
        //Generating new credential for provisioning tool
        OicSecCred_t *cred = GenerateCredential(&selectedDeviceInfo->doxm->deviceID,
                                      SYMMETRIC_PAIR_WISE_KEY, NULL,
                                      &ownerKey, &ownerDeviceID, &ownerDeviceID);
        VERIFY_NON_NULL(TAG, cred, ERROR);

        uint32_t outSize = 0;
        size_t b64BufSize = B64ENCODE_OUT_SAFESIZE((OWNER_PSK_LENGTH_128 + 1));
        char* b64Buf = (uint8_t *)OICCalloc(1, b64BufSize);
        VERIFY_NON_NULL(TAG, b64Buf, ERROR);
        b64Encode(cred->privateData.data, cred->privateData.len, b64Buf, b64BufSize, &outSize);

        OICFree( cred->privateData.data );
        cred->privateData.data = (uint8_t *)OICCalloc(1, outSize + 1);
        VERIFY_NON_NULL(TAG, cred->privateData.data, ERROR);

        strncpy(cred->privateData.data, b64Buf, outSize);
        cred->privateData.data[outSize] = '\0';
        cred->privateData.encoding = OIC_ENCODING_BASE64;
        cred->privateData.len = outSize;
        OICFree(b64Buf);

        //Add SubOwnerPSK
        res = AddCredential(cred);
        if(res != OC_STACK_OK)
        {
            DeleteCredList(cred);
            return res;
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "CAGenerateOwnerPSK failed");
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s : %d", __func__, res);
exit:
    return res;
}


/**
 * Response handler for update subowner crendetial request.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SubOwnerCredentialHandler(void *ctx, OCDoHandle UNUSED,
                                OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    (void)UNUSED;
    OTMContext_t* motCtx = (OTMContext_t*)ctx;

    if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        if(motCtx && motCtx->selectedDeviceInfo)
        {
            //Delete previous credential such as preconfigured-pin
            RemoveCredential(&(motCtx->selectedDeviceInfo->doxm->deviceID));
            OCStackResult res = SaveSubOwnerPSK(motCtx->selectedDeviceInfo);
            if(OC_STACK_OK == res)
            {
                //Close the temporal secure session to verify the owner credential
                CAEndpoint_t* endpoint = (CAEndpoint_t *)&motCtx->selectedDeviceInfo->endpoint;
                endpoint->port = motCtx->selectedDeviceInfo->securePort;
                CAResult_t caResult = CAcloseSslSession(endpoint);
                if(CA_STATUS_OK != caResult)
                {
                    OIC_LOG(ERROR, TAG, "Failed to close DTLS session");
                    SetMOTResult(motCtx, OC_STACK_ERROR);
                    return OC_STACK_DELETE_TRANSACTION;
                }

                // TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA_256 = 0xC037, /**< see RFC 5489 */
                caResult = CASelectCipherSuite(0xC037, endpoint->adapter);
                if(CA_STATUS_OK != caResult)
                {
                    OIC_LOG(ERROR, TAG, "Failed to select TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA_256");
                    SetMOTResult(motCtx, OC_STACK_ERROR);
                    return OC_STACK_DELETE_TRANSACTION;
                }

                SetMOTResult(motCtx, OC_STACK_OK);
            }
            else
            {
                OIC_LOG(ERROR, TAG, "Failed to save the SubOwner PSK.");
                SetMOTResult(motCtx, res);
                return OC_STACK_DELETE_TRANSACTION;
            }
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "SubOwnerCredentialHandler : Unexpected result %d",
                  clientResponse->result);
        SetMOTResult(motCtx, clientResponse->result);
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

exit:
    return  OC_STACK_DELETE_TRANSACTION;
}


static OCStackResult PostSubOwnerCredential(OTMContext_t* motCtx)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if(!motCtx || !motCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = motCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};

    if(!PMGenerateQuery(true,
                        deviceInfo->endpoint.addr, deviceInfo->securePort,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_CRED_URI))
    {
        OIC_LOG(ERROR, TAG, "PostSubOwnerCredential : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);
    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    //Generate sub-owner credential for new device
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    /**
     * Because of the deadlock issue, we can not get a server's session information at this time.
     * So use the dumpy owner credential instance to send POST credential request.
     */
    OicUuid_t ownerId = {.id={0}};
    if(OC_STACK_OK == GetDoxmDeviceID(&ownerId))
    {
        OicSecCred_t newCredential;
        memset(&newCredential, 0x0, sizeof(OicSecCred_t));
        newCredential.next = NULL;
        newCredential.credType = SYMMETRIC_PAIR_WISE_KEY;

        //Set subject ID as SubOwner's ID
        memcpy(&(newCredential.subject), &ownerId, sizeof(OicUuid_t));

        //Set eowner ID as SubOwner's ID
        if(NULL == newCredential.eownerID)
        {
            newCredential.eownerID = OICCalloc(1, sizeof(OicUuid_t));
            if(NULL == newCredential.eownerID)
            {
                return OC_STACK_NO_MEMORY;
            }
        }
        memcpy(newCredential.eownerID->id, ownerId.id, sizeof(ownerId.id));

        //Fill private data as empty string
        newCredential.privateData.data = "";
        newCredential.privateData.len = 0;
        newCredential.privateData.encoding = OIC_ENCODING_BASE64;

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        newCredential.publicData.data = NULL;
        newCredential.publicData.len = 0;
        newCredential.publicData.encoding = OIC_ENCODING_RAW;
#endif
        //Send owner credential to new device : POST /oic/sec/cred [ owner credential ]
        if (OC_STACK_OK != CredToCBORPayload(&newCredential, &secPayload->securityData,
                                        &secPayload->payloadSize, 0))
        {
            OICFree(secPayload);
            OIC_LOG(ERROR, TAG, "Error while converting bin to cbor.");
            return OC_STACK_ERROR;
        }
        OIC_LOG(DEBUG, TAG, "Cred Payload:");
        OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);
        OICFree(newCredential.eownerID);

        OCCallbackData cbData;
        cbData.cb = &SubOwnerCredentialHandler;
        cbData.context = (void *)motCtx;
        cbData.cd = NULL;
        OCStackResult res = OCDoResource(NULL, OC_REST_POST, query,
                                         &deviceInfo->endpoint, (OCPayload*)secPayload,
                                         deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
        if (res != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "OCStack resource error");
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed to read DOXM device ID.");
        return OC_STACK_NO_RESOURCE;
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return OC_STACK_OK;
}


/**
 * Function to handle the handshake result in MOT.
 * This function will be invoked after DTLS handshake
 * @param   endPoint  [IN] The remote endpoint.
 * @param   errorInfo [IN] Error information from the endpoint.
 * @return  NONE
 */
static void MOTDtlsHandshakeCB(const CAEndpoint_t *endpoint, const CAErrorInfo_t *info)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if(NULL != endpoint && NULL != info)
    {
        OIC_LOG_V(INFO, TAG, "Received status from remote device(%s:%d) : %d",
                 endpoint->addr, endpoint->port, info->result);

        OTMContext_t* motCtx = GetOTMContext(endpoint->addr, endpoint->port);
        if(motCtx)
        {
            OicSecDoxm_t* newDevDoxm = motCtx->selectedDeviceInfo->doxm;

            if(NULL != newDevDoxm)
            {
                OicUuid_t emptyUuid = {.id={0}};

                //Make sure the address matches.
                if(strncmp(motCtx->selectedDeviceInfo->endpoint.addr,
                   endpoint->addr,
                   sizeof(endpoint->addr)) == 0 &&
                   motCtx->selectedDeviceInfo->securePort == endpoint->port)
                {
                    OCStackResult res = OC_STACK_ERROR;

                    //If temporal secure sesstion established successfully
                    if(CA_STATUS_OK == info->result)
                    {
                        //POST sub owner credential to new device.
                        res = PostSubOwnerCredential(motCtx);
                        if(OC_STACK_OK != res)
                        {
                            OIC_LOG(ERROR, TAG,
                                    "Failed to send POST request for SubOwner Credential");
                            SetMOTResult(motCtx, res);
                        }
                    }
                    //In case of authentication failure
                    else if(CA_DTLS_AUTHENTICATION_FAILURE == info->result)
                    {
                        //in case of error from wrong PIN, re-start the ownership transfer
                        if(OIC_RANDOM_DEVICE_PIN == newDevDoxm->oxmSel)
                        {
                            OIC_LOG(ERROR, TAG, "The PIN number may incorrect.");

                            motCtx->attemptCnt++;

                            if(WRONG_PIN_MAX_ATTEMP > motCtx->attemptCnt)
                            {
                                res = StartMultipleOwnershipTransfer(motCtx, motCtx->selectedDeviceInfo);
                                if(OC_STACK_OK != res)
                                {
                                    SetMOTResult(motCtx, res);
                                    OIC_LOG(ERROR, TAG, "Failed to Re-StartOwnershipTransfer");
                                }
                            }
                            else
                            {
                                OIC_LOG(ERROR, TAG, "User has exceeded the number of authentication attempts.");
                                SetMOTResult(motCtx, OC_STACK_AUTHENTICATION_FAILURE);
                            }
                        }
                        else
                        {
                            OIC_LOG(ERROR, TAG, "Failed to establish DTLS session.");
                            SetMOTResult(motCtx, OC_STACK_AUTHENTICATION_FAILURE);
                        }
                    }
                }
            }
        }
        else
        {
            OIC_LOG_V(ERROR, TAG, "Can not find the [%s:%d]'s OTMContext for MOT", endpoint->addr, endpoint->port);
        }
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

static OCStackResult StartMultipleOwnershipTransfer(OTMContext_t* motCtx,
                                                    OCProvisionDev_t* selectedDevice)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    OCStackResult res = OC_STACK_INVALID_PARAM;
    OicUuid_t myUuid = {.id={0}};

    VERIFY_NON_NULL(TAG, motCtx, ERROR);
    VERIFY_NON_NULL(TAG, selectedDevice, ERROR);
    VERIFY_NON_NULL(TAG, selectedDevice->doxm, ERROR);
    motCtx->selectedDeviceInfo = selectedDevice;

    res = GetDoxmDeviceID(&myUuid);
    if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to GetDoxmDeviceID");
        SetMOTResult(motCtx, res);
        return res;
    }
    if(memcmp(selectedDevice->doxm->owner.id, myUuid.id, sizeof(myUuid.id)) == 0)
    {
        res = OC_STACK_INVALID_DEVICE_INFO;
        OIC_LOG(ERROR, TAG, "Owner cannot be registered as sub-owner.");
        SetMOTResult(motCtx, res);
        return res;
    }
    if (NULL == selectedDevice->doxm->mom ||
        (selectedDevice->doxm->mom &&
         OIC_MULTIPLE_OWNER_DISABLE == selectedDevice->doxm->mom->mode))
    {
        res = OC_STACK_NOT_ACCEPTABLE;
        OIC_LOG(ERROR, TAG, "Selected device's MOT is disabled.");
        SetMOTResult(motCtx, res);
        return res;
    }

    //Checking duplication of Device ID.
    char* strUuid = NULL;
    PdmDeviceState_t deviceState = PDM_DEVICE_UNKNOWN;
    res = PDMGetDeviceState(&selectedDevice->doxm->deviceID, &deviceState);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    res = ConvertUuidToStr(&selectedDevice->doxm->deviceID, &strUuid);
    if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to convert UUID to str");
        SetMOTResult(motCtx, res);
        return res;
    }

    if(PDM_DEVICE_STALE == deviceState)
    {
        OIC_LOG_V(WARNING, TAG, "Detected duplicated UUID in stale status, "
                           "[%s] will be removed from PDM", strUuid);

        res = PDMDeleteDevice(&selectedDevice->doxm->deviceID);
        if(OC_STACK_OK != res)
        {
            OIC_LOG(ERROR, TAG, "Internal error in PDMDeleteDevice");
            OICFree(strUuid);
            SetMOTResult(motCtx, res);
        }
    }
    else if(PDM_DEVICE_INIT == deviceState)
    {
        OIC_LOG_V(ERROR, TAG, "[%s]'s multiple owner transfer process is already started.", strUuid);
        OICFree(strUuid);
        SetMOTResult(motCtx, OC_STACK_DUPLICATE_REQUEST);
        return OC_STACK_OK;
    }

    res = PDMAddDevice(&selectedDevice->doxm->deviceID);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(INFO, TAG, "Error in PDMAddDevice for [%s]", strUuid);
        OICFree(strUuid);
        SetMOTResult(motCtx, res);
        return res;
    }
    OICFree(strUuid);

    //Register DTLS event handler to catch the dtls event while handshake
    if(CA_STATUS_OK != CAregisterSslHandshakeCallback(MOTDtlsHandshakeCB))
    {
        OIC_LOG(WARNING, TAG, "StartOwnershipTransfer : Failed to register DTLS handshake callback.");
    }

    OicSecOxm_t oxmSel = selectedDevice->doxm->oxmSel;
    OIC_LOG_V(DEBUG, TAG, "Multiple Ownership Transfer method = %d", (int)oxmSel);

    if(OIC_PRECONFIG_PIN != oxmSel && OIC_RANDOM_DEVICE_PIN != oxmSel)
    {
        OIC_LOG(ERROR, TAG, "Unsupported OxM");
        return OC_STACK_ERROR;
    }

    res = OTMSetOTCallback(selectedDevice->doxm->oxmSel, &motCtx->otmCallback);
    if(OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Error in OTMSetOTCallback : %d", res);
        return res;
    }
    //Only two functions required for MOT
    VERIFY_NON_NULL(TAG, motCtx->otmCallback.loadSecretCB, ERROR);
    VERIFY_NON_NULL(TAG, motCtx->otmCallback.createSecureSessionCB, ERROR);

    if(OIC_RANDOM_DEVICE_PIN == oxmSel)
    {
        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskForRandomPinOxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to register DTLS credential handler for Random PIN OxM.");
        }
    }

    //Save the current context instance to use on the dtls handshake callback
    res = AddOTMContext(motCtx, selectedDevice->endpoint.addr, selectedDevice->securePort);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    res = motCtx->otmCallback.loadSecretCB(motCtx);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    res = motCtx->otmCallback.createSecureSessionCB(motCtx);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

exit:
    return res;
}

OCStackResult MOTDoOwnershipTransfer(void* ctx,
                                     OCProvisionDev_t *selectedDevicelist,
                                     OCProvisionResultCB resultCallback)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    OCStackResult res = OC_STACK_INVALID_PARAM;
    OTMContext_t* motCtx = NULL;
    OCProvisionDev_t* pCurDev = NULL;

    VERIFY_NON_NULL(TAG, selectedDevicelist, ERROR);
    VERIFY_NON_NULL(TAG, resultCallback, ERROR);

    res = OC_STACK_NO_MEMORY;
    motCtx = (OTMContext_t*)OICCalloc(1,sizeof(OTMContext_t));
    VERIFY_NON_NULL(TAG, motCtx, ERROR);

    motCtx->ctxResultCallback = resultCallback;
    motCtx->ctxHasError = false;
    motCtx->userCtx = ctx;
    motCtx->ctxResultArraySize = 0;
    LL_FOREACH(selectedDevicelist, pCurDev)
    {
        motCtx->ctxResultArraySize++;
    }

    motCtx->ctxResultArray =
        (OCProvisionResult_t*)OICCalloc(motCtx->ctxResultArraySize, sizeof(OCProvisionResult_t));
    VERIFY_NON_NULL(TAG, motCtx->ctxResultArray, ERROR);

    //Fill the device UUID for result array.
    size_t devIdx = 0;
    pCurDev = NULL;
    LL_FOREACH(selectedDevicelist, pCurDev)
    {
        memcpy(motCtx->ctxResultArray[devIdx].deviceId.id,
               pCurDev->doxm->deviceID.id,
               UUID_LENGTH);
        motCtx->ctxResultArray[devIdx].res = OC_STACK_CONTINUE;
        devIdx++;
    }

    motCtx->selectedDeviceInfo = selectedDevicelist;
    res = StartMultipleOwnershipTransfer(motCtx, selectedDevicelist);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return res;
exit:
    if(OC_STACK_OK != res)
    {
        if(motCtx)
        {
            OICFree(motCtx->ctxResultArray);
            OICFree(motCtx);
        }
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s : %d", __func__, res);

    return res;
}

OCStackResult MOTRemoveSubOwner(void* ctx,
                                const OCProvisionDev_t *targetDeviceInfo,
                                const OicUuid_t* subOwner,
                                OCProvisionResultCB resultCallback)
{
    OCStackResult deleteSubOwnerRes = OC_STACK_INVALID_CALLBACK;
    OTMContext_t *motCtx = NULL;
    char* strUuid = NULL;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    VERIFY_NON_NULL(TAG, resultCallback, ERROR);
    deleteSubOwnerRes = OC_STACK_INVALID_PARAM;
    VERIFY_NON_NULL(TAG, targetDeviceInfo, ERROR);
    VERIFY_NON_NULL(TAG, subOwner, ERROR);

    deleteSubOwnerRes = OC_STACK_NO_MEMORY;
    //Generate the qurey to delete sub-owner
    if (memcmp(subOwner->id, WILDCARD_SUBJECT_ID.id, sizeof(WILDCARD_SUBJECT_ID.id)) == 0)
    {
        strUuid = OICStrdup(WILDCARD_RESOURCE_URI);
        VERIFY_NON_NULL(TAG, strUuid, ERROR);
    }
    else
    {
        VERIFY_SUCCESS(TAG, (OC_STACK_OK == ConvertUuidToStr(subOwner, &strUuid)), ERROR);
    }
    char url[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    snprintf(url, sizeof(url), "%s?%s=%s", OIC_RSRC_DOXM_URI, OIC_JSON_SUBOWNERID_NAME, strUuid);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    bool queryGenRes = PMGenerateQuery(true,
                                       targetDeviceInfo->endpoint.addr,
                                       targetDeviceInfo->securePort,
                                       targetDeviceInfo->connType,
                                       query, sizeof(query), url);
    VERIFY_SUCCESS(TAG, (true == queryGenRes), ERROR);

    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    //Create the MOT Context to handle the response message
    motCtx = (OTMContext_t*)OICCalloc(1, sizeof(OTMContext_t));
    VERIFY_NON_NULL(TAG, motCtx, ERROR);
    motCtx->selectedDeviceInfo= targetDeviceInfo;
    motCtx->ctxResultCallback = resultCallback;
    motCtx->ctxResultArraySize =1;
    motCtx->ctxHasError = false;
    motCtx->userCtx = ctx;
    motCtx->ctxResultArray = (OCProvisionResult_t*)OICCalloc(1, sizeof(OCProvisionResult_t));
    VERIFY_NON_NULL(TAG, motCtx->ctxResultArray, ERROR);

    //Send POST request
    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &MOTDeleteSecurityResourceCB;
    cbData.context = (void *)motCtx;
    OIC_LOG(DEBUG, TAG, "Sending DELETE sub-owner request to resource server");
    deleteSubOwnerRes = OCDoResource(NULL, OC_REST_DELETE, query,
                                     &targetDeviceInfo->endpoint, NULL,
                                     targetDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == deleteSubOwnerRes), ERROR);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    OICFree(strUuid);

    return deleteSubOwnerRes;

exit:
    //If DELETE request successfully sent, motCtx will be cleaned from response handler.
    OICFree(strUuid);
    if (motCtx)
    {
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s : %d", __func__, deleteSubOwnerRes);
    return deleteSubOwnerRes;
}

