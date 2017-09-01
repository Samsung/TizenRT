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
#include "casecurityinterface.h"
#include "ocrandom.h"
#include "oic_malloc.h"
#include "logger.h"
#include "pbkdf2.h"
#include "base64.h"
#include "oxmmanufacturercert.h"
#include "ownershiptransfermanager.h"
#include "srmresourcestrings.h"
#include "pkix_interface.h"
#include "mbedtls/ssl_ciphersuites.h"
#include "ocstackinternal.h"

#define TAG "OXM_MCertificate"

OCStackResult CreateMCertificateBasedSelectOxmPayload(OTMContext_t* otmCtx, uint8_t **payload, size_t *size)
{
    if (!otmCtx || !otmCtx->selectedDeviceInfo || !payload || *payload || !size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    otmCtx->selectedDeviceInfo->doxm->oxmSel = OIC_MANUFACTURER_CERTIFICATE;

    bool propertiesToInclude[DOXM_PROPERTY_COUNT];
    memset(propertiesToInclude, 0, sizeof(propertiesToInclude));
    propertiesToInclude[DOXM_OXMSEL] = true;

    return DoxmToCBORPayloadPartial(otmCtx->selectedDeviceInfo->doxm, payload,
        size, propertiesToInclude);
}

OCStackResult CreateConMCertificateBasedSelectOxmPayload(OTMContext_t* otmCtx, uint8_t **payload, size_t *size)
{
    if (!otmCtx || !otmCtx->selectedDeviceInfo || !payload || *payload || !size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    otmCtx->selectedDeviceInfo->doxm->oxmSel = OIC_CON_MFG_CERT;

    bool propertiesToInclude[DOXM_PROPERTY_COUNT];
    memset(propertiesToInclude, 0, sizeof(propertiesToInclude));
    propertiesToInclude[DOXM_OXMSEL] = true;

    return DoxmToCBORPayloadPartial(otmCtx->selectedDeviceInfo->doxm, payload,
        size, propertiesToInclude);
}

OCStackResult CreateMCertificateBasedOwnerTransferPayload(OTMContext_t* otmCtx, uint8_t **payload, size_t *size)
{
    if (!otmCtx || !otmCtx->selectedDeviceInfo || !payload || *payload || !size)
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

    return DoxmToCBORPayloadPartial(otmCtx->selectedDeviceInfo->doxm, payload,
        size, propertiesToInclude);
}

OCStackResult PrepareMCertificateCallback(OTMContext_t *otmCtx)
{
    OIC_LOG(INFO, TAG, "IN PrepareMCertificateCallback");

    if (!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        return OC_STACK_INVALID_PARAM;
    }

    if (CA_STATUS_OK != CAregisterPkixInfoHandler(GetManufacturerPkixInfo))
    {
        OIC_LOG(ERROR, TAG, "Failed to register PkixInfohandler");
        return OC_STACK_ERROR;
    }

    if (CA_STATUS_OK != CAregisterGetCredentialTypesHandler(InitManufacturerCipherSuiteList))
    {
        OIC_LOG(ERROR, TAG, "Failed to register CredentialTypesHandler");
        return OC_STACK_ERROR;
    }

    OIC_LOG(INFO, TAG, "OUT PrepareMCertificateCallback");

    return OC_STACK_OK;
}

OCStackResult CreateSecureSessionMCertificateCallback(OTMContext_t* otmCtx)
{
    OIC_LOG(INFO, TAG, "IN CreateSecureSessionMCertificateCallback");

    if (!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        return OC_STACK_INVALID_PARAM;
    }

    CAResult_t caresult = CAEnableAnonECDHCipherSuite(false);
    if (CA_STATUS_OK != caresult)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to disable anon cipher suite");
        return OC_STACK_ERROR;
    }
    OIC_LOG(INFO, TAG, "Anonymous cipher suite disabled.");

    caresult  = CASelectCipherSuite(MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8,
                                    otmCtx->selectedDeviceInfo->endpoint.adapter);
    if (CA_STATUS_OK != caresult)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to select MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8");
        return OC_STACK_ERROR;
    }
    OIC_LOG(INFO, TAG, "MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8 cipher suite selected.");

    OIC_LOG(INFO, TAG, "OUT CreateSecureSessionMCertificateCallback");

    return OC_STACK_OK;
}
