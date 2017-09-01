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

#include <memory.h>

#include "ocstack.h"
#include "experimental/securevirtualresourcetypes.h"
#include "doxmresource.h"
#include "credresource.h"
#include "cacommon.h"
#include "cainterface.h"
#include "ocrandom.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "pbkdf2.h"
#include "base64.h"
#include "oxmpreconfpin.h"
#include "ownershiptransfermanager.h"
#include "pinoxmcommon.h"
#include "srmresourcestrings.h"
#include "ocstackinternal.h"
#include "mbedtls/ssl_ciphersuites.h"

#define TAG "OIC_OXM_PreconfigPIN"

OCStackResult CreatePreconfigPinBasedSelectOxmPayload(OTMContext_t *otmCtx, uint8_t **payload, size_t *size)
{
    if(!otmCtx || !otmCtx->selectedDeviceInfo || !payload || *payload || !size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    otmCtx->selectedDeviceInfo->doxm->oxmSel = OIC_PRECONFIG_PIN;

    bool propertiesToInclude[DOXM_PROPERTY_COUNT];
    memset(propertiesToInclude, 0, sizeof(propertiesToInclude));
    propertiesToInclude[DOXM_OXMSEL] = true;

    return DoxmToCBORPayloadPartial(otmCtx->selectedDeviceInfo->doxm, payload, size, propertiesToInclude);
}

OCStackResult CreatePreconfigPinBasedOwnerTransferPayload(OTMContext_t *otmCtx, uint8_t **payload, size_t *size)
{
    if(!otmCtx || !otmCtx->selectedDeviceInfo || !payload || *payload || !size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OicUuid_t uuidPT = {.id={0}};
    *payload = NULL;
    *size = 0;

    if (OC_STACK_OK != GetDoxmDeviceID(&uuidPT))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving provisioning tool's device ID");
        return OC_STACK_ERROR;
    }
    memcpy(otmCtx->selectedDeviceInfo->doxm->owner.id, uuidPT.id , UUID_LENGTH);

    bool propertiesToInclude[DOXM_PROPERTY_COUNT];
    memset(propertiesToInclude, 0, sizeof(propertiesToInclude));
    propertiesToInclude[DOXM_DEVOWNERUUID] = true;

    return DoxmToCBORPayloadPartial(otmCtx->selectedDeviceInfo->doxm, payload, size, propertiesToInclude);
}

OCStackResult LoadPreconfigPinCodeCallback(OTMContext_t *otmCtx)
{
    OIC_LOG(INFO, TAG, "IN LoadPreconfigPinCodeCallback");
    OCStackResult res = OC_STACK_ERROR;
    OicSecCred_t* cred = GetCredResourceData(&otmCtx->selectedDeviceInfo->doxm->deviceID);
    if(NULL == cred)
    {
        OicUuid_t uuid = {.id={0}};
        memcpy(&uuid, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t));
        cred = GetCredResourceData(&uuid);
        if(NULL == cred)
        {
            OIC_LOG(ERROR, TAG, "Can not find the Credential for MOT");
            OIC_LOG(ERROR, TAG, "Please make sure the preconfigured PIN");
            return OC_STACK_ERROR;
        }
    }

    uint8_t* pinBuffer = NULL;
    size_t pinBufLen = 0;
    if(OIC_ENCODING_BASE64 == cred->privateData.encoding)
    {
        //In case of 'preconfig PIN', secret data(PIN) already exist.
        pinBufLen = B64DECODE_OUT_SAFESIZE(cred->privateData.len + 1);
        pinBuffer = (uint8_t*)OICCalloc(1, pinBufLen);
        if(NULL == pinBuffer)
        {
            OIC_LOG(ERROR, TAG, "Failed to memory allocation.");
            return OC_STACK_NO_MEMORY;
        }
        size_t pinLen = 0;
        if(B64_OK != b64Decode((char*)cred->privateData.data, cred->privateData.len, pinBuffer, pinBufLen, &pinLen))
        {
            OIC_LOG(ERROR, TAG, "Failed to base64 deconding for preconfig PIN");
            OICFree(pinBuffer);
            return OC_STACK_ERROR;
        }
        pinBufLen = pinLen;
    }
    else if(OIC_ENCODING_RAW == cred->privateData.encoding)
    {
        pinBuffer = (uint8_t*)OICMalloc(cred->privateData.len + 1);
        if(NULL == pinBuffer)
        {
            OIC_LOG(ERROR, TAG, "Failed to memory allocation.");
            return OC_STACK_NO_MEMORY;
        }
        memcpy(pinBuffer, cred->privateData.data, cred->privateData.len);
        pinBuffer[cred->privateData.len] = '\0';
        pinBufLen = cred->privateData.len;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Unknown encoding type for PreConfigured PIN credential");
        return OC_STACK_ERROR;
    }

    res = SetPreconfigPin((char*)pinBuffer, pinBufLen);
    OICFree(pinBuffer);
    if(OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to save the preconfig PIN : %d", res);
        return res;
    }

    //in case of OTM
    if(false == otmCtx->selectedDeviceInfo->doxm->owned)
    {
        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskForPreconfPinOxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to register DTLS credentials handler for random PIN OxM.");
            res = OC_STACK_ERROR;
        }
    }
#ifdef MULTIPLE_OWNER
    //in case of MOT
    else if(true == otmCtx->selectedDeviceInfo->doxm->owned &&
            otmCtx->selectedDeviceInfo->doxm->mom &&
            OIC_MULTIPLE_OWNER_DISABLE != otmCtx->selectedDeviceInfo->doxm->mom->mode)
    {
        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskForMotPreconfPinOxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to register DTLS credentials handler for random PIN OxM.");
            res = OC_STACK_ERROR;
        }
    }
#endif //MULTIPLE_OWNER

    //Set the device id to derive temporal PSK
    SetUuidForPinBasedOxm(&(otmCtx->selectedDeviceInfo->doxm->deviceID));

    OIC_LOG(INFO, TAG, "OUT LoadPreconfigPinCodeCallback");

    return res;
}

OCStackResult CreateSecureSessionPreconfigPinCallback(OTMContext_t* otmCtx)
{
    OIC_LOG(INFO, TAG, "IN CreateSecureSessionPreconfigPinCallback");

    if (!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        return OC_STACK_INVALID_PARAM;
    }

    CAResult_t caresult = CAEnableAnonECDHCipherSuite(false);
    if (CA_STATUS_OK != caresult)
    {
        OIC_LOG_V(ERROR, TAG, "Unable to disable anon cipher suite");
        return OC_STACK_ERROR;
    }
    OIC_LOG(INFO, TAG, "Anonymous cipher suite disabled.");

    caresult  = CASelectCipherSuite(MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, otmCtx->selectedDeviceInfo->endpoint.adapter);
    if (CA_STATUS_OK != caresult)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to select TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256");
        return OC_STACK_ERROR;
    }
    OIC_LOG(INFO, TAG, "TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256 cipher suite selected.");

    OIC_LOG(INFO, TAG, "OUT CreateSecureSessionPreconfigPinCallback");

    return OC_STACK_OK;
}
