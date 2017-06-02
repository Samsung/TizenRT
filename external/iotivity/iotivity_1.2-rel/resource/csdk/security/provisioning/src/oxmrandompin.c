/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include "securevirtualresourcetypes.h"
#include "doxmresource.h"
#include "credresource.h"
#include "cacommon.h"
#include "cainterface.h"
#include "ocrandom.h"
#include "oic_malloc.h"
#include "logger.h"
#include "pbkdf2.h"
#include "base64.h"
#include "oxmrandompin.h"
#include "ownershiptransfermanager.h"
#include "pinoxmcommon.h"

#define TAG "OIC_OXM_RandomPIN"

OCStackResult CreatePinBasedSelectOxmPayload(OTMContext_t* otmCtx, uint8_t **payload, size_t *size)
{
    if(!otmCtx || !otmCtx->selectedDeviceInfo || !payload || *payload || !size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    otmCtx->selectedDeviceInfo->doxm->oxmSel = OIC_RANDOM_DEVICE_PIN;

    return DoxmToCBORPayload(otmCtx->selectedDeviceInfo->doxm, payload, size, true);
}

OCStackResult CreatePinBasedOwnerTransferPayload(OTMContext_t* otmCtx, uint8_t **payload, size_t *size)
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

    return DoxmToCBORPayload(otmCtx->selectedDeviceInfo->doxm, payload, size, true);
}

OCStackResult InputPinCodeCallback(OTMContext_t *otmCtx)
{
    if (!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        return OC_STACK_INVALID_PARAM;
    }

    uint8_t pinData[OXM_RANDOM_PIN_MAX_SIZE + 1] = {0};

    OCStackResult res = InputPin((char*)pinData, sizeof(pinData));
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to input PIN");
        return res;
    }

    /**
     * Since PSK will be used directly while PIN based ownership transfer,
     * Credential should not be saved into SVR.
     * For this reason, We will use a temporary get_psk_info callback to random PIN OxM.
     */
    //in case of OTM
    if(!(otmCtx->selectedDeviceInfo->doxm->owned))
    {
        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskForRandomPinOxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to register DTLS credentials handler for random PIN OxM.");
            res = OC_STACK_ERROR;
        }
    }
#ifdef MULTIPLE_OWNER
    //in case of MOT
    else if(otmCtx->selectedDeviceInfo->doxm->owned &&
            otmCtx->selectedDeviceInfo->doxm->mom &&
            OIC_MULTIPLE_OWNER_DISABLE != otmCtx->selectedDeviceInfo->doxm->mom->mode)
    {
        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskForMotRandomPinOxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to register TLS credentials handler for random PIN OxM.");
            res = OC_STACK_ERROR;
        }
    }
#endif //MULTIPLE_OWNER

    //Set the device id to derive temporal PSK
    SetUuidForPinBasedOxm(&(otmCtx->selectedDeviceInfo->doxm->deviceID));

    return res;
}

OCStackResult CreateSecureSessionRandomPinCallback(OTMContext_t* otmCtx)
{
    OIC_LOG(INFO, TAG, "IN CreateSecureSessionRandomPinCallbak");

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

    OCProvisionDev_t* selDevInfo = otmCtx->selectedDeviceInfo;
    CAEndpoint_t endpoint;
    memcpy(&endpoint, &selDevInfo->endpoint, sizeof(CAEndpoint_t));

    if(CA_ADAPTER_IP == endpoint.adapter)
    {
        endpoint.port = selDevInfo->securePort;
        caresult = CAInitiateHandshake(&endpoint);
    }
    else if (CA_ADAPTER_GATT_BTLE == endpoint.adapter)
    {
        caresult = CAInitiateHandshake(&endpoint);
    }
#ifdef __WITH_TLS__
    else
    {
        endpoint.port = selDevInfo->tcpPort;
        caresult = CAinitiateSslHandshake(&endpoint);
    }
#endif
    if (CA_STATUS_OK != caresult)
    {
        OIC_LOG_V(ERROR, TAG, "DTLS handshake failure.");
        return OC_STACK_ERROR;
    }

    OIC_LOG(INFO, TAG, "OUT CreateSecureSessionRandomPinCallbak");

    return OC_STACK_OK;
}
