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
#include "srmresourcestrings.h"
#include "doxmresource.h"
#include "pstatresource.h"
#include "credresource.h"
#include "aclresource.h"
#include "ownershiptransfermanager.h"
#include "oxmjustworks.h"
#include "pmtypes.h"
#include "pmutility.h"
#include "pmutilityinternal.h"
#include "srmutility.h"
#include "provisioningdatabasemanager.h"
#include "oxmrandompin.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "oxmjustworks.h"
#include "oxmpreconfpin.h"
#include "oxmrandompin.h"
#include "otmcontextlist.h"
#include "ocstackinternal.h"
#include "mbedtls/ssl_ciphersuites.h"
#include "ocrandom.h"
#include "secureresourceprovider.h"

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
static OCStackApplicationResult MOTUpdateSecurityResourceCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OC_UNUSED(handle);

    OTMContext_t *motCtx = NULL;

    VERIFY_NOT_NULL(TAG, ctx, ERROR);
    VERIFY_NOT_NULL(TAG, ((Data_t *)ctx)->ctx, ERROR);

    motCtx = (OTMContext_t *)(((Data_t *)ctx)->ctx);

    VERIFY_NOT_NULL(TAG, motCtx, ERROR);
    VERIFY_NOT_NULL(TAG, motCtx->ctxResultCallback, ERROR);
    VERIFY_NOT_NULL(TAG, motCtx->ctxResultArray, ERROR);

    if (clientResponse)
    {
        memcpy(motCtx->ctxResultArray[0].deviceId.id,
               motCtx->selectedDeviceInfo->doxm->deviceID.id,
               sizeof(OicUuid_t));
        motCtx->ctxResultArray[0].res = clientResponse->result;
        if (OC_STACK_RESOURCE_CHANGED == clientResponse->result)
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
        OIC_LOG_V(ERROR, TAG, "%s: received Null clientResponse", __func__);
        motCtx->ctxResultArray[0].res = OC_STACK_ERROR;
        motCtx->ctxHasError = true;
    }

    motCtx->ctxResultCallback(motCtx->userCtx, motCtx->ctxResultArraySize,
                              motCtx->ctxResultArray, motCtx->ctxHasError);

    OIC_LOG_V(DEBUG, TAG, "%s: update ok", __func__);
exit:
    if (motCtx)
    {
        PMDeleteDeviceList(motCtx->selectedDeviceInfo);
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Restores pstat after PIN provisioning
 */
static OCStackApplicationResult RestoreProvCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OC_UNUSED(handle);
    OC_UNUSED(clientResponse);

    VERIFY_NOT_NULL_RETURN(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);

    if (OC_STACK_OK != SetDOS(ctx, DOS_RFNOP, MOTUpdateSecurityResourceCB))
    {
        OIC_LOG_V(ERROR, TAG, "%s: SetDOS", __func__);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult MOTSendPostDoxmCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OC_UNUSED(handle);
    OC_UNUSED(clientResponse);

    bool freeFlag = true;
    OTMContext_t *motCtx = NULL;
    OCStackResult postMomRes = OC_STACK_INVALID_PARAM;
    VERIFY_NOT_NULL(TAG, ctx, ERROR);

    OCSecurityPayload *secPayload = NULL;

    Data_t *data = (Data_t *) ctx;
    if (MOT_TYPE != data->type)
    {
        OIC_LOG(ERROR, TAG, "Invalid type");
        goto exit;
    }

    VERIFY_NOT_NULL(TAG, data->ctx, ERROR);
    motCtx = (OTMContext_t *) (data->ctx);

    //Generate the security payload using updated doxm
    secPayload = (OCSecurityPayload *)OICCalloc(1, sizeof(OCSecurityPayload));
    VERIFY_NOT_NULL(TAG, secPayload, ERROR);
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    // This function appears to be called for updating oxmSel, mom, and subOwners,
    // but it previously was including ALL writeable Properties.  So we include
    // at least those three.
    bool propertiesToInclude[DOXM_PROPERTY_COUNT];
    memset(propertiesToInclude, 0, sizeof(propertiesToInclude));
    propertiesToInclude[DOXM_OXMSEL] = true;
    propertiesToInclude[DOXM_SUBOWNER] = true;
    propertiesToInclude[DOXM_MOM] = true;

    postMomRes = DoxmToCBORPayloadPartial(motCtx->doxm, &secPayload->securityData,
                                          &secPayload->payloadSize, propertiesToInclude);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    OIC_LOG(DEBUG, TAG, "Created doxm payload to update doxm:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    bool queryGenRes = PMGenerateQuery(true,
                                       motCtx->selectedDeviceInfo->endpoint.addr,
                                       motCtx->selectedDeviceInfo->securePort,
                                       motCtx->selectedDeviceInfo->connType,
                                       query, sizeof(query), OIC_RSRC_DOXM_URI);
    VERIFY_SUCCESS(TAG, (true == queryGenRes), ERROR);
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    //Send POST request
    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(cbData));

    cbData.cb = &RestoreProvCB;
    cbData.context = ctx;
    cbData.cd = NULL;

    postMomRes = OCDoResource(NULL, OC_REST_POST, query,
                              &(motCtx->selectedDeviceInfo->endpoint), (OCPayload *)secPayload,
                              motCtx->selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    freeFlag = false;

    OIC_LOG_V(DEBUG, TAG, "%s: Send POST 'doxm' request to resource server", __func__);

exit:
    //If POST request successfully sent, motCtx will be cleaned from response handler.
    if (freeFlag && motCtx)
    {
        PMDeleteDeviceList(motCtx->selectedDeviceInfo);
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return postMomRes;
}

/**
 * Internal API to send POST doxm request
 */
static OCStackResult MOTSendPostDoxm(void *ctx,
                                     const OCProvisionDev_t *targetDeviceInfo,
                                     OCProvisionResultCB resultCallback,
                                     OicSecDoxm_t *doxm)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OTMContext_t *motCtx = NULL;
    OCProvisionDev_t *localTargetDeviceInfo = NULL;
    OCStackResult ret =  OC_STACK_ERROR;

    //Create the MOT Context to handle the response message
    motCtx = (OTMContext_t *)OICCalloc(1, sizeof(OTMContext_t));
    VERIFY_NOT_NULL(TAG, motCtx, ERROR);

    localTargetDeviceInfo = PMCloneOCProvisionDevList(targetDeviceInfo);
    VERIFY_NOT_NULL(TAG, localTargetDeviceInfo, ERROR);

    motCtx->selectedDeviceInfo = localTargetDeviceInfo;
    motCtx->ctxResultCallback = resultCallback;
    motCtx->ctxResultArraySize = 1;
    motCtx->ctxHasError = false;
    motCtx->doxm = doxm;
    motCtx->userCtx = ctx;
    motCtx->ctxResultArray = (OCProvisionResult_t *)OICCalloc(1, sizeof(OCProvisionResult_t));
    VERIFY_NOT_NULL(TAG, motCtx->ctxResultArray, ERROR);

    Data_t *data = (Data_t *) OICCalloc(1, sizeof(Data_t));
    VERIFY_NOT_NULL(TAG, data, ERROR);
    data->type = MOT_TYPE;
    data->ctx = motCtx;

    ret =  SetDOS(data, DOS_RFPRO, MOTSendPostDoxmCB);
    if (OC_STACK_OK != ret)
    {
        FreeData(data);
    }
exit:
    if (OC_STACK_OK != ret)
    {
        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return ret;
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
    OicSecDoxm_t *doxm = NULL;
    uint8_t *doxmPayload = NULL;
    size_t doxmPayloadLen = 0;

    OIC_LOG(DEBUG, TAG, "IN MOTChangeMode");

    VERIFY_SUCCESS(TAG, (OIC_NUMBER_OF_MOM_TYPE > momType), ERROR);
    VERIFY_NOT_NULL(TAG, targetDeviceInfo, ERROR);
    postMomRes = OC_STACK_INVALID_CALLBACK;
    VERIFY_NOT_NULL(TAG, resultCallback, ERROR);

    //Dulpicate doxm resource to update the 'mom' property
    postMomRes = DoxmToCBORPayload(targetDeviceInfo->doxm, &doxmPayload, &doxmPayloadLen);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    postMomRes = CBORPayloadToDoxm(doxmPayload, doxmPayloadLen, &doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);
    VERIFY_NOT_NULL(TAG, doxm, ERROR);

    if (NULL == doxm->mom)
    {
        postMomRes = OC_STACK_NO_MEMORY;
        doxm->mom = (OicSecMom_t *)OICCalloc(1, sizeof(OicSecMom_t));
        VERIFY_NOT_NULL(TAG, (doxm->mom), ERROR);
    }
    doxm->mom->mode = momType;

    //Send POST reuqest for update doxm
    postMomRes = MOTSendPostDoxm(ctx, targetDeviceInfo, resultCallback, doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    OIC_LOG(DEBUG, TAG, "OUT MOTChangeMode");

exit:
    OICFree(doxmPayload);
    DeleteDoxmBinData(doxm);
    return postMomRes;
}

/**
 * API to add 'doxm.oxms' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] newOxm  OxMs to be added (ref. oic.sec.doxmtype)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxms' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTAddMOTMethod(void *ctx, OCProvisionDev_t *targetDeviceInfo,
                              const OicSecOxm_t newOxm, OCProvisionResultCB resultCallback)
{
    OCStackResult postOxmRes = OC_STACK_INVALID_PARAM;
    OicSecOxm_t *newOxms = NULL;
    uint8_t *doxmPayload = NULL;

    OIC_LOG(DEBUG, TAG, "IN MOTAddMOTMethod");

    VERIFY_SUCCESS(TAG, (OIC_OXM_COUNT > newOxm || OIC_PRECONFIG_PIN == newOxm), ERROR);
    VERIFY_NOT_NULL(TAG, targetDeviceInfo, ERROR);
    postOxmRes = OC_STACK_INVALID_CALLBACK;
    VERIFY_NOT_NULL(TAG, resultCallback, ERROR);
    postOxmRes = OC_STACK_NO_MEMORY;

    for (size_t i = 0; i < targetDeviceInfo->doxm->oxmLen; i++)
    {
        if (targetDeviceInfo->doxm->oxm[i] == newOxm)
        {
            OIC_LOG_V(INFO, TAG, "[%d] OxM already supported", (int)newOxm);
            OCProvisionResult_t *resArr = (OCProvisionResult_t *)OICCalloc(1, sizeof(OCProvisionResult_t));
            VERIFY_NOT_NULL(TAG, resArr, ERROR);
            resArr->res = OC_STACK_OK;
            memcpy(resArr->deviceId.id, targetDeviceInfo->doxm->deviceID.id, sizeof(resArr->deviceId.id));
            resultCallback(ctx, 1, resArr, false);
            return OC_STACK_OK;
        }
    }

    newOxms = (OicSecOxm_t *)OICMalloc(sizeof(OicSecOxm_t) * (targetDeviceInfo->doxm->oxmLen + 1));
    VERIFY_NOT_NULL(TAG, newOxms, ERROR);

    for (size_t i = 0; i < targetDeviceInfo->doxm->oxmLen; i++)
    {
        newOxms[i] = targetDeviceInfo->doxm->oxm[i];
    }
    newOxms[targetDeviceInfo->doxm->oxmLen] = newOxm;
    targetDeviceInfo->doxm->oxmLen++;
    OICFree(targetDeviceInfo->doxm->oxm);
    targetDeviceInfo->doxm->oxm = newOxms;

    //Send POST request for doxm update
    postOxmRes = MOTSendPostDoxm(ctx, targetDeviceInfo, resultCallback, targetDeviceInfo->doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postOxmRes), ERROR);

    OIC_LOG(DEBUG, TAG, "OUT MOTAddMOTMethod");

exit:
    OICFree(doxmPayload);
    return postOxmRes;
}

/**
 * API to update 'doxm.oxmsel' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] oxmSelValue Method of multiple ownership transfer (ref. oic.sec.doxmtype)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxmsel' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTSelectMOTMethod(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                                 const OicSecOxm_t oxmSelValue, OCProvisionResultCB resultCallback)
{
    OCStackResult postMomRes = OC_STACK_INVALID_CALLBACK;
    OicSecDoxm_t *doxm = NULL;
    uint8_t *doxmPayload = NULL;
    size_t doxmPayloadLen = 0;

    OIC_LOG(DEBUG, TAG, "IN MOTSelectOTMethod");

    VERIFY_NOT_NULL(TAG, resultCallback, ERROR);
    postMomRes = OC_STACK_INVALID_PARAM;
    VERIFY_NOT_NULL(TAG, targetDeviceInfo, ERROR);

    bool isValidOxmsel = false;
    for (size_t i = 0; i < targetDeviceInfo->doxm->oxmLen; i++)
    {
        if (targetDeviceInfo->doxm->oxm[i] == oxmSelValue)
        {
            isValidOxmsel = true;
            break;
        }
    }
    VERIFY_SUCCESS(TAG, isValidOxmsel, ERROR);

    //Dulpicate doxm resource to update the 'oxmsel' property
    postMomRes = DoxmToCBORPayload(targetDeviceInfo->doxm, &doxmPayload, &doxmPayloadLen);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    postMomRes = CBORPayloadToDoxm(doxmPayload, doxmPayloadLen, &doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);
    VERIFY_NOT_NULL(TAG, doxm, ERROR);

    doxm->oxmSel = oxmSelValue;

    //Send POST reuqest for update doxm
    postMomRes = MOTSendPostDoxm(ctx, targetDeviceInfo, resultCallback, doxm);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postMomRes), ERROR);

    OIC_LOG(DEBUG, TAG, "OUT MOTSelectOTMethod");

exit:
    OICFree(doxmPayload);
    DeleteDoxmBinData(doxm);
    return postMomRes;
}

static OCStackApplicationResult MOTProvisionPreconfigPINCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OC_UNUSED(handle);
    OC_UNUSED(clientResponse);

    bool freeFlag = true;
    OTMContext_t *motCtx = NULL;
    OCStackResult postCredRes = OC_STACK_INVALID_PARAM;
    OCSecurityPayload *secPayload = NULL;

    VERIFY_NOT_NULL(TAG, ctx, ERROR);

    Data_t *data = (Data_t *) ctx;
    if (MOT_TYPE != data->type)
    {
        OIC_LOG(ERROR, TAG, "Invalid type");
        goto exit;
    }

    VERIFY_NOT_NULL(TAG, data->ctx, ERROR);
    motCtx = (OTMContext_t *) (data->ctx);

    //Generate the security payload using updated doxm
    secPayload = (OCSecurityPayload *)OICCalloc(1, sizeof(OCSecurityPayload));
    VERIFY_NOT_NULL(TAG, secPayload, ERROR);
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    postCredRes = CredToCBORPayload(motCtx->cred, &secPayload->securityData, &secPayload->payloadSize,
                                    false);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postCredRes), ERROR);

    OIC_LOG(DEBUG, TAG, "Created Credential payload to register PIN credential:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    bool queryGenRes = PMGenerateQuery(true,
                                       motCtx->selectedDeviceInfo->endpoint.addr,
                                       motCtx->selectedDeviceInfo->securePort,
                                       motCtx->selectedDeviceInfo->connType,
                                       query, sizeof(query), OIC_RSRC_CRED_URI);
    VERIFY_SUCCESS(TAG, (true == queryGenRes), ERROR);
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    //Send POST request
    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(cbData));

    cbData.cb = &RestoreProvCB;
    cbData.context = ctx;
    cbData.cd = NULL;

    OIC_LOG(DEBUG, TAG, "Sending POST Preconfiged PIN credenatial request to resource server");
    postCredRes = OCDoResource(NULL, OC_REST_POST, query,
                               &(motCtx->selectedDeviceInfo->endpoint), (OCPayload *)secPayload,
                               motCtx->selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == postCredRes), ERROR);

    freeFlag = false;

exit:
    //If POST request successfully sent, motCtx will be cleaned from response handler.
    if (freeFlag && motCtx)
    {
        PMDeleteDeviceList(motCtx->selectedDeviceInfo);

        if (motCtx->cred)
        {
            OICFree(motCtx->cred->credUsage);
            OICFree(motCtx->cred->privateData.data);
            OICFree(motCtx->cred);
        }

        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return postCredRes;
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
                                       const char *preconfPIN, size_t preconfPINLen, OCProvisionResultCB resultCallback)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OCStackResult postCredRes = OC_STACK_INVALID_CALLBACK;
    bool freeFlag = true;
    OTMContext_t *motCtx = NULL;
    OicSecCred_t *pinCred = NULL;
    Data_t *data = NULL;
    OCProvisionDev_t *localTargetDeviceInfo = NULL;


    VERIFY_NOT_NULL(TAG, resultCallback, ERROR);
    postCredRes = OC_STACK_INVALID_PARAM;
    VERIFY_NOT_NULL(TAG, targetDeviceInfo, ERROR);
    VERIFY_NOT_NULL(TAG, preconfPIN, ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen), ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen && OXM_PRECONFIG_PIN_MAX_SIZE >= preconfPINLen), ERROR);

    postCredRes = OC_STACK_NO_MEMORY;
    //Generate PIN based credential
    pinCred = (OicSecCred_t *)OICCalloc(1, sizeof(OicSecCred_t));
    VERIFY_NOT_NULL(TAG, pinCred, ERROR);

    pinCred->privateData.data = (uint8_t *)OICMalloc(preconfPINLen + 1);
    VERIFY_NOT_NULL(TAG, pinCred->privateData.data, ERROR);

    memcpy(pinCred->privateData.data, preconfPIN, preconfPINLen);
    pinCred->privateData.data[preconfPINLen] = '\0';
    pinCred->privateData.len = preconfPINLen;
    pinCred->privateData.encoding = OIC_ENCODING_RAW;
    pinCred->credType = PIN_PASSWORD;
    memcpy(&pinCred->subject, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t));

    pinCred->credUsage = (char *)OICCalloc(1, (strlen(PRECONFIG_PIN_CRED) + 1));
    VERIFY_NOT_NULL(TAG, pinCred->credUsage, ERROR);
    OICStrcpy(pinCred->credUsage, (strlen(PRECONFIG_PIN_CRED) + 1), PRECONFIG_PIN_CRED);

    //Create the MOT Context to handle the response message
    motCtx = (OTMContext_t *)OICCalloc(1, sizeof(OTMContext_t));
    VERIFY_NOT_NULL(TAG, motCtx, ERROR);

    localTargetDeviceInfo = PMCloneOCProvisionDevList(targetDeviceInfo);
    VERIFY_NOT_NULL(TAG, localTargetDeviceInfo, ERROR);

    motCtx->selectedDeviceInfo = localTargetDeviceInfo;
    motCtx->ctxResultCallback = resultCallback;
    motCtx->ctxResultArraySize = 1;
    motCtx->ctxHasError = false;
    motCtx->userCtx = ctx;
    motCtx->cred = pinCred;
    motCtx->ctxResultArray = (OCProvisionResult_t *)OICCalloc(1, sizeof(OCProvisionResult_t));
    VERIFY_NOT_NULL(TAG, motCtx->ctxResultArray, ERROR);

    data = (Data_t *) OICCalloc(1, sizeof(Data_t));
    VERIFY_NOT_NULL(TAG, data, ERROR);
    data->type = MOT_TYPE;
    data->ctx = motCtx;

    if (SetDOS(data, DOS_RFPRO, MOTProvisionPreconfigPINCB) != OC_STACK_OK)
    {
        FreeData(data);
        postCredRes = OC_STACK_ERROR;
        goto exit;
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
    return OC_STACK_OK;

exit:
    //If POST request successfully sent, motCtx will be cleaned from response handler.
    if (freeFlag && motCtx)
    {
        PMDeleteDeviceList(motCtx->selectedDeviceInfo);

        if (motCtx->cred)
        {
            OICFree(motCtx->cred->credUsage);
            OICFree(motCtx->cred->privateData.data);
            OICFree(motCtx->cred);
        }

        OICFree(motCtx->ctxResultArray);
        OICFree(motCtx);
    }

    OIC_LOG_V(ERROR, TAG, "OUT %s", __func__);

    return postCredRes;
}


/**********************************************************************
 * API for Sub Owner
 **********************************************************************/

static OCStackResult StartMultipleOwnershipTransfer(OTMContext_t *motCtx,
        OCProvisionDev_t *selectedDevice);

static bool IsComplete(OTMContext_t *otmCtx)
{
    for (size_t i = 0; i < otmCtx->ctxResultArraySize; i++)
    {
        if (OC_STACK_CONTINUE == otmCtx->ctxResultArray[i].res)
        {
            return false;
        }
    }

    return true;
}

int MOTIsSupportedOnboardingType(OicSecDoxm_t *ptrDoxm)
{
    OIC_LOG_V(DEBUG, TAG, "Multiple Ownership Transfer type: %u", ptrDoxm->oxmSel);

    if ((OIC_PRECONFIG_PIN != ptrDoxm->oxmSel) && (OIC_RANDOM_DEVICE_PIN != ptrDoxm->oxmSel))
    {
        OIC_LOG_V(ERROR, TAG, "Unsupported MOT device");
        return 1;
    }
    return 0;
}

/**
 * Function to save the result of multiple ownership transfer.
 *
 * @param[in,out] motCtx   Context instance of multiple ownership transfer.
 * @param[in] res   result of multiple ownership transfer.
 */
static void SetMOTResult(OTMContext_t *motCtx, const OCStackResult res)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s: %d ", __func__, res);

    VERIFY_NOT_NULL(TAG, motCtx, ERROR);

    if (motCtx->selectedDeviceInfo)
    {
        //Revert psk_info callback in case of random PIN OxM
        if (!MOTIsSupportedOnboardingType(motCtx->selectedDeviceInfo->doxm))
        {
            if (CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskCredentials))
            {
                OIC_LOG(WARNING, TAG, "Failed to revert the DTLS credential handler.");
            }
            OicUuid_t emptyUuid = { .id = {0}};
            SetUuidForPinBasedOxm(&emptyUuid);
        }

        OCStackResult pdmRetVal = PDMSetDeviceState(&motCtx->selectedDeviceInfo->doxm->deviceID,
                                  PDM_DEVICE_ACTIVE);
        if (OC_STACK_OK != pdmRetVal)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to add device information into PDM_DB : %d", res);
        }

        for (size_t i = 0; i < motCtx->ctxResultArraySize; i++)
        {
            if (memcmp(motCtx->selectedDeviceInfo->doxm->deviceID.id,
                       motCtx->ctxResultArray[i].deviceId.id, UUID_LENGTH) == 0)
            {
                motCtx->ctxResultArray[i].res = res;
                if (OC_STACK_OK != res)
                {
                    motCtx->ctxHasError = true;
                }
            }
        }

        //Remove the current OTM Context from OTM queue
        RemoveOTMContext(motCtx->selectedDeviceInfo->endpoint.addr,
                         motCtx->selectedDeviceInfo->securePort);

        //If there is a request being performed, cancel it to prevent retransmission.
        if (motCtx->ocDoHandle)
        {
            if (OC_STACK_OK !=  OCCancel(motCtx->ocDoHandle, OC_HIGH_QOS, NULL, 0))
            {
                OIC_LOG(ERROR, TAG, "Failed to remove registered callback");
            }
            motCtx->ocDoHandle = NULL;
        }

        //If all request is completed, invoke the user callback.
        if (IsComplete(motCtx))
        {
            motCtx->ctxResultCallback(motCtx->userCtx, motCtx->ctxResultArraySize,
                                      motCtx->ctxResultArray, motCtx->ctxHasError);

            OICFree(motCtx->ctxResultArray);
            OICFree(motCtx);
            motCtx = NULL;
        }
        else
        {
            if (OC_STACK_OK != StartMultipleOwnershipTransfer(motCtx,
                    motCtx->selectedDeviceInfo->next))
            {
                OIC_LOG(ERROR, TAG, "Failed to StartMultipleOwnershipTransfer");
            }
        }
    }

exit:
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
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
                                 const char *preconfPIN, size_t preconfPINLen)
{
    OCStackResult addCredRes = OC_STACK_INVALID_PARAM;
    OicSecCred_t *pinCred = NULL;

    OIC_LOG(DEBUG, TAG, "IN MOTAddPreconfigPIN");

    VERIFY_NOT_NULL(TAG, targetDeviceInfo, ERROR);
    VERIFY_NOT_NULL(TAG, preconfPIN, ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen), ERROR);
    VERIFY_SUCCESS(TAG, (0 != preconfPINLen && OXM_PRECONFIG_PIN_MAX_SIZE >= preconfPINLen), ERROR);

    OicSecCred_t *prevCred = GetCredResourceData(&targetDeviceInfo->doxm->deviceID);
    if (NULL != prevCred)
    {
        OIC_LOG(INFO, TAG, "PIN/PW Credential already exist!");
        return OC_STACK_OK;
    }

    addCredRes = OC_STACK_NO_MEMORY;
    //Generate PIN based credential
    pinCred = (OicSecCred_t *)OICCalloc(1, sizeof(OicSecCred_t));
    VERIFY_NOT_NULL(TAG, pinCred, ERROR);

    pinCred->privateData.data = (uint8_t *)OICMalloc(preconfPINLen + 1);
    VERIFY_NOT_NULL(TAG, pinCred->privateData.data, ERROR);

    memcpy(pinCred->privateData.data, preconfPIN, preconfPINLen);
    pinCred->privateData.data[preconfPINLen] = '\0';
    pinCred->privateData.len = preconfPINLen;
    pinCred->privateData.encoding = OIC_ENCODING_RAW;
    pinCred->credType = PIN_PASSWORD;
    memcpy(pinCred->subject.id, targetDeviceInfo->doxm->deviceID.id, sizeof(pinCred->subject.id));

    addCredRes = AddCredential(pinCred);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == addCredRes), ERROR);

exit:
    if (OC_STACK_OK != addCredRes)
    {
        OICFree(pinCred->privateData.data);
        OICFree(pinCred);
    }

    OIC_LOG(DEBUG, TAG, "OUT MOTAddPreconfigPIN");

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
    OIC_LOG(DEBUG, TAG, "IN SaveSubOwnerPSK");

    OCStackResult res = OC_STACK_ERROR;

    CAEndpoint_t endpoint;
    CopyDevAddrToEndpoint(&selectedDeviceInfo->endpoint, &endpoint);

    if (CA_ADAPTER_IP == endpoint.adapter)
    {
        endpoint.port = selectedDeviceInfo->securePort;
    }
#ifdef WITH_TCP
    else if (CA_ADAPTER_TCP == endpoint.adapter)
    {
        endpoint.port = selectedDeviceInfo->tcpSecurePort;
    }
#endif

    OicUuid_t ownerDeviceID = {.id = {0}};
    if (OC_STACK_OK != GetDoxmDeviceID(&ownerDeviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving SubOwner's device ID");
        return res;
    }

    uint8_t ownerPSK[OWNER_PSK_LENGTH_128] = {0};
    OicSecKey_t ownerKey;
    memset(&ownerKey, 0, sizeof(ownerKey));
    ownerKey.data = ownerPSK;
    ownerKey.len = OWNER_PSK_LENGTH_128;
    ownerKey.encoding = OIC_ENCODING_RAW;

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
        VERIFY_NOT_NULL(TAG, cred, ERROR);

        size_t outSize = 0;
        size_t b64BufSize = B64ENCODE_OUT_SAFESIZE((OWNER_PSK_LENGTH_128 + 1));
        char *b64Buf = (char *)OICCalloc(1, b64BufSize);
        VERIFY_NOT_NULL(TAG, b64Buf, ERROR);
        b64Encode(cred->privateData.data, cred->privateData.len, b64Buf, b64BufSize, &outSize);

        OICFree(cred->privateData.data);
        cred->privateData.data = (uint8_t *)OICCalloc(1, outSize + 1);
        VERIFY_NOT_NULL(TAG, cred->privateData.data, ERROR);

        strncpy((char *)(cred->privateData.data), b64Buf, outSize);
        cred->privateData.data[outSize] = '\0';
        cred->privateData.encoding = OIC_ENCODING_BASE64;
        cred->privateData.len = outSize;
        OICFree(b64Buf);

        //Add SubOwnerPSK
        res = AddCredential(cred);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "%s: cannot add credential", __func__);
            DeleteCredList(cred);
            goto exit;
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "CAGenerateOwnerPSK failed");
    }

exit:
    OIC_LOG(DEBUG, TAG, "OUT SaveSubOwnerPSK");
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
static OCStackApplicationResult SubOwnerCredentialHandler(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "IN SubOwnerCredentialHandler");

    OC_UNUSED(handle);

    VERIFY_NOT_NULL(TAG, clientResponse, WARNING);
    VERIFY_NOT_NULL(TAG, ctx, WARNING);
    VERIFY_NOT_NULL(TAG, ((Data_t *)ctx)->ctx, ERROR);

    OTMContext_t *motCtx = (OTMContext_t *)(((Data_t *)ctx)->ctx); //ctx;

    if (OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        if (motCtx && motCtx->selectedDeviceInfo)
        {
            //Close the temporal secure session to verify the owner credential
            CAEndpoint_t *endpoint = (CAEndpoint_t *)&motCtx->selectedDeviceInfo->endpoint;
            endpoint->port = motCtx->selectedDeviceInfo->securePort;
            CAResult_t caResult = CAcloseSslSession(endpoint);
            if (CA_STATUS_OK != caResult)
            {
                OIC_LOG(ERROR, TAG, "Failed to close DTLS session");
                SetMOTResult(motCtx, OC_STACK_ERROR);
                return OC_STACK_DELETE_TRANSACTION;
            }

            caResult = CASelectCipherSuite(MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, endpoint->adapter);
            if (CA_STATUS_OK != caResult)
            {
                OIC_LOG(ERROR, TAG, "Failed to select TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256");
                SetMOTResult(motCtx, OC_STACK_ERROR);
                return OC_STACK_DELETE_TRANSACTION;
            }

            SetMOTResult(motCtx, OC_STACK_OK);
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "SubOwnerCredentialHandler : Unexpected result %d",
                  clientResponse->result);
        SetMOTResult(motCtx, clientResponse->result);
    }

exit:
    OIC_LOG(DEBUG, TAG, "OUT SubOwnerCredentialHandler");

    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Restores pstat after change credential
 */
static OCStackApplicationResult RestoreCredCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OC_UNUSED(handle);
    OC_UNUSED(clientResponse);

    VERIFY_NOT_NULL_RETURN(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);

    if (OC_STACK_OK != SetDOS(ctx, DOS_RFNOP, SubOwnerCredentialHandler))
    {
        OIC_LOG_V(ERROR, TAG, "OUT %s", __func__);
        return OC_STACK_DELETE_TRANSACTION;
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult PostSubOwnerCredentialCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "IN PostSubOwnerCredentialCB");

    OC_UNUSED(handle);
    OC_UNUSED(clientResponse);

    Data_t *data = (Data_t *) ctx;
    if (MOT_TYPE != data->type)
    {
        OIC_LOG(ERROR, TAG, "Invalid type");
        return OC_STACK_INVALID_PARAM;
    }

    if (!(data->ctx))
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OTMContext_t *motCtx = (OTMContext_t *) (data->ctx);
    if (!motCtx || !motCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t *deviceInfo = motCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};

    if (!PMGenerateQuery(true,
                         deviceInfo->endpoint.addr, deviceInfo->securePort,
                         deviceInfo->connType,
                         query, sizeof(query), OIC_RSRC_CRED_URI))
    {
        OIC_LOG(ERROR, TAG, "PostSubOwnerCredential : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);
    OCSecurityPayload *secPayload = (OCSecurityPayload *)OICCalloc(1, sizeof(OCSecurityPayload));
    if (!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    //Generate sub-owner credential for new device
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    const OicSecCred_t *ownerCredential = GetCredResourceData(&(deviceInfo->doxm->deviceID));
    if (!ownerCredential)
    {
        OIC_LOG(ERROR, TAG, "Can not find SubOwnerPSK.");
        return OC_STACK_NO_RESOURCE;
    }

    OicUuid_t ownerId = {.id = {0}};
    if (OC_STACK_OK == GetDoxmDeviceID(&ownerId))
    {
        OicSecCred_t newCredential;
        memcpy(&newCredential, ownerCredential, sizeof(OicSecCred_t));
        newCredential.next = NULL;

        //Set subject ID as SubOwner's ID
        memcpy(&(newCredential.subject), &ownerId, sizeof(OicUuid_t));

        //Set eowner ID as SubOwner's ID
        if (NULL == newCredential.eownerID)
        {
            newCredential.eownerID = OICCalloc(1, sizeof(OicUuid_t));
            if (NULL == newCredential.eownerID)
            {
                return OC_STACK_NO_MEMORY;
            }
        }
        memcpy(newCredential.eownerID->id, ownerId.id, sizeof(ownerId.id));

        //Fill private data as empty string
        newCredential.privateData.data = (uint8_t *)"";
        newCredential.privateData.len = 0;
        newCredential.privateData.encoding = ownerCredential->privateData.encoding;

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        newCredential.publicData.data = NULL;
        newCredential.publicData.len = 0;
        newCredential.publicData.encoding = ownerCredential->publicData.encoding;
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

        OCCallbackData cbData;
        cbData.cb = &RestoreCredCB;
        cbData.context = ctx;
        cbData.cd = NULL;
        OCStackResult res = OCDoResource(NULL, OC_REST_POST, query,
                                         &deviceInfo->endpoint, (OCPayload *)secPayload,
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

    OIC_LOG(DEBUG, TAG, "OUT PostSubOwnerCredentialCB");

    return OC_STACK_OK;
}

static OCStackResult PostSubOwnerCredential(OTMContext_t *motCtx)
{
    OIC_LOG(DEBUG, TAG, "IN PostSubOwnerCredential");

    if (!motCtx || !motCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }

    Data_t *data = NULL;
    data = (Data_t *) OICCalloc(1, sizeof(Data_t));
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    data->type = MOT_TYPE;
    data->ctx = motCtx;

    if (SetDOS(data, DOS_RFPRO, PostSubOwnerCredentialCB) != OC_STACK_OK)
    {
        FreeData(data);
        return OC_STACK_ERROR;
    }

    OIC_LOG(DEBUG, TAG, "OUT PostSubOwnerCredential");

    return OC_STACK_OK;
}

static void MOTSessionEstablished(const CAEndpoint_t *endpoint, OicSecDoxm_t *newDevDoxm,
                                  OTMContext_t *motCtx)
{
    OC_UNUSED(endpoint);
    OC_UNUSED(newDevDoxm);

    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    // Delete previous credential such as preconfigured-pin
    OCStackResult res = RemoveCredential(&(motCtx->selectedDeviceInfo->doxm->deviceID));
    if (OC_STACK_RESOURCE_DELETED != res)
    {
        OIC_LOG(WARNING, TAG, "Cannot find credential.");
    }

    res = SaveSubOwnerPSK(motCtx->selectedDeviceInfo);
    if (OC_STACK_OK == res)
    {
        // POST sub owner credential to new device.
        res = PostSubOwnerCredential(motCtx);
        if (OC_STACK_OK != res)
        {
            OIC_LOG(ERROR, TAG, "Failed to send POST request for SubOwner Credential");
            SetMOTResult(motCtx, res);
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed to save the SubOwner PSK.");
        SetMOTResult(motCtx, res);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
}

static void MOTSessionFailed(const CAEndpoint_t *endpoint, const CAErrorInfo_t *info,
                             OicSecDoxm_t *newDevDoxm, OTMContext_t *motCtx)
{
    OC_UNUSED(endpoint);

    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    if (CA_DTLS_AUTHENTICATION_FAILURE != info->result)
    {
        OIC_LOG_V(ERROR, TAG, "Ownership Transfer session establishment failed, error %u", info->result);
        goto exit;
    }

    // Reset for the next attempt
    RemoveOTMContext(motCtx->selectedDeviceInfo->endpoint.addr, motCtx->selectedDeviceInfo->securePort);

    OCStackResult res = PDMDeleteDevice(&(motCtx->selectedDeviceInfo->doxm->deviceID));
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "PDMDeleteDevice failed");
        SetMOTResult(motCtx, res);
        goto exit;
    }

    if (OIC_RANDOM_DEVICE_PIN == newDevDoxm->oxmSel)
    {
        OIC_LOG(DEBUG, TAG, "The PIN may be incorrect.");

        motCtx->attemptCnt++;

        if (WRONG_PIN_MAX_ATTEMP > motCtx->attemptCnt)
        {
            res = StartMultipleOwnershipTransfer(motCtx, motCtx->selectedDeviceInfo);
            if (OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Failed to Re-StartMultipleOwnershipTransfer");
                SetMOTResult(motCtx, res);
            }
        }
        else
        {
            OIC_LOG(ERROR, TAG, "User has exceeded the number of authentication attempts.");
            SetMOTResult(motCtx, OC_STACK_AUTHENTICATION_FAILURE);
        }
    }
    else if (OIC_PRECONFIG_PIN == newDevDoxm->oxmSel)
    {
        OIC_LOG(DEBUG, TAG, "The PIN may be incorrect.");

        // Delete previous credential
        res = RemoveCredential(&(motCtx->selectedDeviceInfo->doxm->deviceID));
        if (OC_STACK_RESOURCE_DELETED != res)
        {
            OIC_LOG(WARNING, TAG, "Cannot find credential.");
        }

        SetMOTResult(motCtx, OC_STACK_AUTHENTICATION_FAILURE);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed to establish secure session.");
        SetMOTResult(motCtx, OC_STACK_AUTHENTICATION_FAILURE);
    }

exit:
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
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
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    VERIFY_NOT_NULL(TAG, endpoint, ERROR);
    VERIFY_NOT_NULL(TAG, info, ERROR);

    OIC_LOG_V(INFO, TAG, "Received status from remote device(%s:%d) : %d",
              endpoint->addr, endpoint->port, info->result);

    OTMContext_t *motCtx = GetOTMContext(endpoint->addr, endpoint->port);
    VERIFY_NOT_NULL(TAG, motCtx, ERROR);

    OicSecDoxm_t *newDevDoxm = motCtx->selectedDeviceInfo->doxm;
    VERIFY_NOT_NULL(TAG, newDevDoxm, ERROR);

    // Make sure the address matches.
    if ((0 != strncmp(motCtx->selectedDeviceInfo->endpoint.addr, endpoint->addr,
                      sizeof(endpoint->addr))) ||
        (motCtx->selectedDeviceInfo->securePort != endpoint->port))
    {
        OIC_LOG_V(ERROR, TAG, "Mismatched: expected address %s:%u",
                  motCtx->selectedDeviceInfo->endpoint.addr, motCtx->selectedDeviceInfo->securePort);
        goto exit;
    }

    // If temporal secure session established successfully
    if (CA_STATUS_OK == info->result)
    {
        MOTSessionEstablished(endpoint, newDevDoxm, motCtx);
    }
    else
    {
        MOTSessionFailed(endpoint, info, newDevDoxm, motCtx);
    }

exit:
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
}

/**
 * Function to add a device to the provisioning database via the
 * Provisioning Database Manager (PDM).
 * @param  selectedDevice [IN] Device to add to the provisioning database.
 * @return OC_STACK_OK in case of success and other values otherwise.
 */
static OCStackResult SetupMOTPDM(OCProvisionDev_t *selectedDevice)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OCStackResult res = OC_STACK_INVALID_PARAM;
    PdmDeviceState_t pdmState = PDM_DEVICE_UNKNOWN;
    char deviceId[UUID_STRING_SIZE];
    bool isSubowner = false;
    bool isDuplicate = true;

    VERIFY_NOT_NULL(TAG, selectedDevice, ERROR);
    VERIFY_NOT_NULL(TAG, selectedDevice->doxm, ERROR);

    res = PMIsSubownerOfDevice(selectedDevice, &isSubowner);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    res = PDMGetDeviceState(&selectedDevice->doxm->deviceID, &pdmState);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    if (!OCConvertUuidToString(selectedDevice->doxm->deviceID.id, deviceId))
    {
        OIC_LOG(WARNING, TAG, "Failed to covert uuid to string");
        return OC_STACK_ERROR;
    }

    if ((PDM_DEVICE_ACTIVE == pdmState) && !isSubowner)
    {
        OIC_LOG_V(WARNING, TAG, "Not a subowner for device[%s] dectected in PDM.", deviceId);
        OIC_LOG_V(WARNING, TAG, "[%s] will be removed from PDM.", deviceId);

        res = PDMDeleteDevice(&selectedDevice->doxm->deviceID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);
    }

    // Checking duplication of Device ID.
    res = PDMIsDuplicateDevice(&selectedDevice->doxm->deviceID, &isDuplicate);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Internal error in PDMIsDuplicateDevice : %d", res);
        goto exit;
    }

    if (isDuplicate)
    {
        if (PDM_DEVICE_STALE == pdmState)
        {
            OIC_LOG(INFO, TAG, "Detected duplicated UUID in stale status, "
                    "device status will revert back to initial status.");
            res = PDMSetDeviceState(&selectedDevice->doxm->deviceID, PDM_DEVICE_INIT);
            if (OC_STACK_OK != res)
            {
                OIC_LOG_V(ERROR, TAG, "Internal error in PDMSetDeviceState : %d", res);
                goto exit;
            }
        }
        else if (PDM_DEVICE_INIT == pdmState)
        {
            OIC_LOG_V(ERROR, TAG, "[%s]'s ownership transfer process is already started.", deviceId);
            res = OC_STACK_DUPLICATE_REQUEST;
            goto exit;
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Unknown device status while OTM.");
            res = OC_STACK_ERROR;
            goto exit;
        }
    }
    else
    {
        res = PDMAddDevice(&selectedDevice->doxm->deviceID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);
    }

exit:
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
    return res;
}

static OCStackResult StartMultipleOwnershipTransfer(OTMContext_t *motCtx,
        OCProvisionDev_t *selectedDevice)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OCStackResult res = OC_STACK_INVALID_PARAM;
    OicUuid_t myUuid = {.id = {0}};

    VERIFY_NOT_NULL(TAG, motCtx, ERROR);
    VERIFY_NOT_NULL(TAG, selectedDevice, ERROR);
    VERIFY_NOT_NULL(TAG, selectedDevice->doxm, ERROR);
    motCtx->selectedDeviceInfo = selectedDevice;

    res = GetDoxmDeviceID(&myUuid);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to GetDoxmDeviceID");
        SetMOTResult(motCtx, res);
        return res;
    }

    if (memcmp(selectedDevice->doxm->owner.id, myUuid.id, sizeof(myUuid.id)) == 0)
    {
        res = OC_STACK_INVALID_DEVICE_INFO;
        OIC_LOG(ERROR, TAG, "Owner cannot be registered as sub-owner.");
        SetMOTResult(motCtx, res);
        return res;
    }

    if ((NULL == selectedDevice->doxm->mom) ||
        (selectedDevice->doxm->mom &&
         (OIC_MULTIPLE_OWNER_DISABLE == selectedDevice->doxm->mom->mode)))
    {
        res = OC_STACK_NOT_ACCEPTABLE;
        OIC_LOG(ERROR, TAG, "MOT is disabled for the selected device.");
        SetMOTResult(motCtx, res);
        return res;
    }
    // Setup PDM to perform the MOT, PDM will be cleanup if necessary.
    res = SetupMOTPDM(selectedDevice);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "SetupMOTPDM error : %d", res);
        SetMOTResult(motCtx, res);
        return res;
    }

    // Register DTLS event handler to catch the dtls event while handshake
    if (CA_STATUS_OK != CAregisterSslHandshakeCallback(MOTDtlsHandshakeCB))
    {
        OIC_LOG(WARNING, TAG, "StartOwnershipTransfer : Failed to register DTLS handshake callback.");
    }

    OicSecOxm_t oxmSel = selectedDevice->doxm->oxmSel;

    VERIFY_SUCCESS(TAG, 0 == MOTIsSupportedOnboardingType(selectedDevice->doxm), ERROR);

    res = OTMSetOTCallback(selectedDevice->doxm->oxmSel, &motCtx->otmCallback);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    // Only two functions required for MOT
    VERIFY_NOT_NULL(TAG, motCtx->otmCallback.loadSecretCB, ERROR);
    VERIFY_NOT_NULL(TAG, motCtx->otmCallback.createSecureSessionCB, ERROR);

    if (OIC_RANDOM_DEVICE_PIN == oxmSel)
    {
        if (CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskForRandomPinOxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to register DTLS credential handler for Random PIN OxM.");
        }
    }

    // Save the current context instance to use on the dtls handshake callback
    res = AddOTMContext(motCtx, selectedDevice->endpoint.addr, selectedDevice->securePort);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    res = motCtx->otmCallback.loadSecretCB(motCtx);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    res = motCtx->otmCallback.createSecureSessionCB(motCtx);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

exit:
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
    return res;
}

OCStackResult MOTDoOwnershipTransfer(void *ctx,
                                     OCProvisionDev_t *selectedDevicelist,
                                     OCProvisionResultCB resultCallback)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    OCStackResult res = OC_STACK_INVALID_PARAM;
    OTMContext_t *motCtx = NULL;
    OCProvisionDev_t *pCurDev = NULL;

    VERIFY_NOT_NULL(TAG, selectedDevicelist, ERROR);
    VERIFY_NOT_NULL(TAG, resultCallback, ERROR);

    res = OC_STACK_NO_MEMORY;
    motCtx = (OTMContext_t *)OICCalloc(1, sizeof(OTMContext_t));
    VERIFY_NOT_NULL(TAG, motCtx, ERROR);

    motCtx->ctxResultCallback = resultCallback;
    motCtx->ctxHasError = false;
    motCtx->userCtx = ctx;
    motCtx->ctxResultArraySize = 0;
    LL_FOREACH(selectedDevicelist, pCurDev)
    {
        motCtx->ctxResultArraySize++;
    }

    motCtx->ctxResultArray =
        (OCProvisionResult_t *)OICCalloc(motCtx->ctxResultArraySize, sizeof(OCProvisionResult_t));
    VERIFY_NOT_NULL(TAG, motCtx->ctxResultArray, ERROR);

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

exit:
    if (OC_STACK_OK != res)
    {
        if (motCtx)
        {
            OICFree(motCtx->ctxResultArray);
            OICFree(motCtx);
        }
    }
    else
    {
        OIC_LOG_V(DEBUG, TAG, "%s: otm ok", __func__);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return res;
}
