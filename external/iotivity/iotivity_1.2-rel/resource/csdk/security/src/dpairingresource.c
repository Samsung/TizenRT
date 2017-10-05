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

#include "iotivity_config.h"
#include <stdlib.h>
#include <string.h>
#include "ocstack.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif
#include "base64.h"
#include "resourcemanager.h"
#include "dpairingresource.h"
#include "psinterface.h"
#include "utlist.h"
#include "srmresourcestrings.h"
#include "cainterface.h"
#include "doxmresource.h"
#include "pconfresource.h"
#include "credresource.h"
#include "aclresource.h"
#include "srmutility.h"
#include "ocserverrequest.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "payload_logging.h"

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#define TAG  "OIC_SRM_DPAIRING"

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint16_t CBOR_SIZE = 1024;

/** Max cbor size payload. */
static const uint16_t CBOR_MAX_SIZE = 4400;

/** DOXM Map size - Number of mandatory items. */
static const uint8_t DPAIR_MAP_SIZE = 3;

static OicSecDpairing_t     *gDpair = NULL;
static OCResourceHandle   gDpairHandle = NULL;
static OicSecDpairing_t      gDefaultDpair =
{
    PRM_NOT_ALLOWED,       /* OicSecPrm_t spm */
    {.id = {0}},                   /* OicUuid_t pdeviceID */
    {.id = {0}},                   /* OicUuid_t rowner */
};

void DeleteDpairingBinData(OicSecDpairing_t* dpair)
{
    if (dpair)
    {
        //Clean dpairing itself
        OICFree(dpair);
    }
}

/**
 * Get the default value.
 * @retval  the gDefaultDpair pointer;
 */
static OicSecDpairing_t* GetDpairingDefault()
{
    OIC_LOG (DEBUG, TAG, "GetDpairingDefault");

    return &gDefaultDpair;
}

/**
 * This method is used by SRM to retrieve Dpairing resource data..
 */
void SetDpairingResourceOwner(OicUuid_t *rowner)
{
    OIC_LOG (DEBUG, TAG, "SetDpairingResourceOwner");
    if (gDpair)
    {
        memcpy(&gDpair->rownerID, rowner, sizeof(OicUuid_t));
    }
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * Function to save PairingPSK.
 *
 * @param[in] endpoint   current endpoint.
 * @param[in] peerDevID   peer device indentitiy.
 * @param[in] isPairingServer   indicate if it generates PairingPSK for server or client.
 *
 * @return  OC_STACK_OK on success
 */
OCStackResult SavePairingPSK(OCDevAddr *endpoint,
            OicUuid_t *peerDevID, OicUuid_t *owner, bool isPairingServer)
{
    OIC_LOG(DEBUG, TAG, "IN SavePairingPSK");

    if(NULL == endpoint || NULL == peerDevID || NULL == owner)
    {
        OIC_LOG_V(ERROR, TAG, "Invalid Input parameters in [%s]\n", __FUNCTION__);
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult res = OC_STACK_ERROR;

    OicUuid_t ptDeviceID = {.id={0}};
    if (OC_STACK_OK != GetDoxmDeviceID(&ptDeviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving provisioning tool's device ID");
        return res;
    }

    uint8_t pairingPSK[OWNER_PSK_LENGTH_128] = {0};
    OicSecKey_t pairingKey = {pairingPSK, OWNER_PSK_LENGTH_128, OIC_ENCODING_RAW};

    //Generating PairingPSK using OwnerPSK scheme
    CAResult_t pskRet = CAGenerateOwnerPSK((const CAEndpoint_t *)endpoint,
            (uint8_t *)OIC_RSRC_TYPE_SEC_DPAIRING,
            strlen(OIC_RSRC_TYPE_SEC_DPAIRING),
            (isPairingServer ? ptDeviceID.id : peerDevID->id), sizeof(OicUuid_t), // server
            (isPairingServer ? peerDevID->id : ptDeviceID.id), sizeof(OicUuid_t), // client
            pairingPSK, OWNER_PSK_LENGTH_128);

    if (CA_STATUS_OK == pskRet)
    {
        OIC_LOG(DEBUG, TAG, "pairingPSK dump:\n");
        OIC_LOG_BUFFER(DEBUG, TAG, pairingPSK, OWNER_PSK_LENGTH_128);
        //Generating new credential for direct-pairing client

        OicSecCred_t *cred = GenerateCredential(peerDevID,
                SYMMETRIC_PAIR_WISE_KEY, NULL,
                &pairingKey, owner, NULL);
        OICClearMemory(pairingPSK, sizeof(pairingPSK));
        VERIFY_NON_NULL(TAG, cred, ERROR);

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

    OIC_LOG(DEBUG, TAG, "OUT SavePairingPSK");
exit:
    return res;
}
#endif // __WITH_DTLS__ or __WITH_TLS__

OCStackResult DpairingToCBORPayload(const OicSecDpairing_t *dpair, uint8_t **payload, size_t *size)
{
    if (NULL == dpair || NULL == payload || NULL != *payload || NULL == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    size_t cborLen = *size;
    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }

    *payload = NULL;
    *size = 0;

    OCStackResult ret = OC_STACK_ERROR;

    CborEncoder encoder;
    CborEncoder dpairMap;

    int64_t cborEncoderResult = CborNoError;
    uint8_t mapSize = DPAIR_MAP_SIZE;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    cborEncoderResult = cbor_encoder_create_map(&encoder, &dpairMap, mapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating DPAIRING Map");

    //spm -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&dpairMap, OIC_JSON_SPM_NAME,
        strlen(OIC_JSON_SPM_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SPM name tag");
    cborEncoderResult = cbor_encode_int(&dpairMap, dpair->spm);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SPM value");

    //PDEVICEID -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&dpairMap, OIC_JSON_PDEVICE_ID_NAME,
        strlen(OIC_JSON_PDEVICE_ID_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PDeviceID tag");
    {
        char *deviceId = NULL;
        ret = ConvertUuidToStr(&dpair->pdeviceID, &deviceId);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&dpairMap, deviceId, strlen(deviceId));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode PDeviceID value");
        OICFree(deviceId);
    }

    //ROWNER -- Mandatory
    {
        char *rowner = NULL;
        cborEncoderResult = cbor_encode_text_string(&dpairMap, OIC_JSON_ROWNERID_NAME,
            strlen(OIC_JSON_ROWNERID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ROWNER tag");
        ret = ConvertUuidToStr(&dpair->rownerID, &rowner);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&dpairMap, rowner, strlen(rowner));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Rowner ID value");
        OICFree(rowner);
    }

    cborEncoderResult = cbor_encoder_close_container(&encoder, &dpairMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close dpairMap");

     if (CborNoError == cborEncoderResult)
    {
        *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
        *payload = outPayload;
        ret = OC_STACK_OK;
    }

exit:
    if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
    {
       // reallocate and try again!
       OICFree(outPayload);
       outPayload = NULL;
       // Since the allocated initial memory failed, double the memory.
       cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
       cborEncoderResult = CborNoError;
       ret = DpairingToCBORPayload(dpair, payload, &cborLen);
       *size = cborLen;
    }

    if ((CborNoError != cborEncoderResult) || (OC_STACK_OK != ret))
    {
       OICFree(outPayload);
       outPayload = NULL;
       *payload = NULL;
       *size = 0;
       ret = OC_STACK_ERROR;
    }

    return ret;
}

OCStackResult CBORPayloadToDpair(const uint8_t *cborPayload, size_t size,
                                OicSecDpairing_t **secDpair)
{
    if (NULL == cborPayload || NULL == secDpair || NULL != *secDpair || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    *secDpair = NULL;

    CborValue dpairCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;

    cbor_parser_init(cborPayload, size, 0, &parser, &dpairCbor);
    CborValue dpairMap = { .parser = NULL };
    OicSecDpairing_t *dpair = NULL;
    cborFindResult = cbor_value_enter_container(&dpairCbor, &dpairMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering DPairing Map");

    dpair = (OicSecDpairing_t *)OICCalloc(1, sizeof(*dpair));
    VERIFY_NON_NULL(TAG, dpair, ERROR);

    while (cbor_value_is_valid(&dpairMap) && cbor_value_is_text_string(&dpairMap))
    {
        char *name = NULL;
        size_t len = 0;
        CborType type = CborInvalidType;
        cborFindResult = cbor_value_dup_text_string(&dpairMap, &name, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding tag name");
        cborFindResult = cbor_value_advance(&dpairMap);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing a value in DPair map");

        type = cbor_value_get_type(&dpairMap);
        if (0 == strcmp(OIC_JSON_SPM_NAME, name) && cbor_value_is_integer(&dpairMap))
        {
            int spm;

            cborFindResult = cbor_value_get_int(&dpairMap, &spm);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding SPM Value");
            dpair->spm = (OicSecPrm_t)spm;
        }

        if (0 == strcmp(OIC_JSON_PDEVICE_ID_NAME, name))
        {
            char *id = NULL;
            cborFindResult = cbor_value_dup_text_string(&dpairMap, &id, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PDeviceID value");
            ret = ConvertStrToUuid(id, &dpair->pdeviceID);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            OICFree(id);
        }

        if (0 == strcmp(OIC_JSON_ROWNERID_NAME, name))
        {
            char *id = NULL;
            cborFindResult = cbor_value_dup_text_string(&dpairMap, &id, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RownerID value");
            ret = ConvertStrToUuid(id, &dpair->rownerID);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            OICFree(id);
        }

        if (CborMapType != type && cbor_value_is_valid(&dpairMap))
        {
            cborFindResult = cbor_value_advance(&dpairMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing the Dpair Map");
        }
        OICFree(name);
    }

    *secDpair = dpair;
    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        OIC_LOG (ERROR, TAG, "CBORPayloadToDoxm failed");
        DeleteDpairingBinData(dpair);
        dpair = NULL;
        *secDpair = NULL;
        ret = OC_STACK_ERROR;
    }
    return ret;
}
/**
 * Function to handle the handshake result in Direct-Pairing.
 * This function will be invoked after DTLS handshake
 * @param   endPoint  [IN] The remote endpoint.
 * @param   errorInfo [IN] Error information from the endpoint.
 * @return  NONE
 */
void DPairingDTLSHandshakeCB(const CAEndpoint_t *endpoint, const CAErrorInfo_t *info)
{
    OIC_LOG_V(INFO, TAG, "IN DPairingDTLSHandshakeCB");

    if(gDpair && endpoint && info)
    {
        OIC_LOG_V(INFO, TAG, "Received status from remote device(%s:%d) : %d",
                 endpoint->addr, endpoint->port, info->result);

        if(CA_STATUS_OK == info->result)
        {
            OIC_LOG(INFO, TAG, "DPairingDTLSHandshakeCB - Connection success.");
        }
        else if(CA_DTLS_AUTHENTICATION_FAILURE == info->result)
        {
            OIC_LOG(INFO, TAG, "DPairingDTLSHandshakeCB - Authentication failed");

        }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        CAregisterSslHandshakeCallback(NULL);
#endif // __WITH_DTLS__ or __WITH_TLS__

        // delete temporary key
        RemoveCredential(&gDpair->pdeviceID);
    }

    OIC_LOG_V(INFO, TAG, "OUT DPairingDTLSHandshakeCB");
}

static OCEntityHandlerResult HandleDpairingPostRequest (const OCEntityHandlerRequest * ehRequest)
{
    OIC_LOG (DEBUG, TAG, "Dpairing EntityHandle  processing POST request");
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OicSecDpairing_t* newDpair = NULL;
    OCStackResult res = OC_STACK_OK;

    const OicSecPconf_t *pconf = GetPconfResourceData();
    if (true == pconf->edp)
    {
        uint8_t *payload = ((OCSecurityPayload*)ehRequest->payload)->securityData;
        size_t size = ((OCSecurityPayload*)ehRequest->payload)->payloadSize;
        if (payload)
        {
            res = CBORPayloadToDpair(payload, size, &newDpair);
        }
    }
    else
    {
        OIC_LOG (DEBUG, TAG, "EDP == false : Direct-Pairing Disabled");
        ehRet = OC_EH_ERROR;
    }

    if (OC_STACK_OK == res && newDpair && false == IsPairedDevice(&newDpair->pdeviceID))
    {
        // Check if valid Post request
        bool prmMached = false;
        for (size_t i=0; i<pconf->prmLen; i++)
        {
            if (newDpair->spm == pconf->prm[i])
            {
                prmMached = true;
                break;
            }
        }
        OIC_LOG_V(DEBUG, TAG, "Parsed spm is %s", prmMached ? "valid" :
                "invalid, send error response");

        // Update local Dpairing with new Dpairing & prepare dtls session
        if (prmMached && '\0' != (char)newDpair->pdeviceID.id[0])
        {
            if(!gDpair)
            {
                gDpair = GetDpairingDefault();
            }
            gDpair->spm = newDpair->spm;
            memcpy(&gDpair->pdeviceID, &newDpair->pdeviceID, sizeof(OicUuid_t));
            memcpy(&gDpair->rownerID, &pconf->rownerID, sizeof(OicUuid_t));

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            // Add temporary psk
            OCStackResult res;
            OicUuid_t subjectId = {.id={0}};
            res = AddTmpPskWithPIN(&gDpair->pdeviceID,
                           SYMMETRIC_PAIR_WISE_KEY,
                           (char*)pconf->pin.val, DP_PIN_LENGTH,
                           &gDpair->rownerID, &subjectId);
            if(res != OC_STACK_OK ||
                    memcmp(&gDpair->pdeviceID, &subjectId, sizeof(OicUuid_t)))
            {
                OIC_LOG_V(ERROR, TAG, "Failed to save the temporal PSK : %d", res);
                goto exit;
            }

            // Prepare to establish a secure channel with Pin-based PSK cipher suite
            if (CA_STATUS_OK != CAEnableAnonECDHCipherSuite(false) ||
                CA_STATUS_OK != CASelectCipherSuite(MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, CA_ADAPTER_IP))
            {
                OIC_LOG_V(ERROR, TAG, "Failed to select TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA_256");
                goto exit;
            }

            if(CA_STATUS_OK != CAregisterSslHandshakeCallback(DPairingDTLSHandshakeCB))
            {
                OIC_LOG(WARNING, TAG, "DirectPairingHandler : Failed to register"
                        " DTLS handshake callback.");
                goto exit;
            }
#endif // __WITH_DTLS__ or __WITH_TLS__

            // should be lock /oic/sec/dpairing resource if Direct-Pairing starts normally ?
            OIC_LOG (DEBUG, TAG, "/oic/sec/dpairing resource created");

            ehRet = OC_EH_RESOURCE_CREATED;
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Error in request check");
        }
    }


#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
exit:
#endif // __WITH_DTLS__ or __WITH_TLS__

    // Send payload to request originator
    if(OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG (ERROR, TAG, "SendSRMResponse failed in HandleDpairingPostRequest");
    }

    if (OC_EH_ERROR == ehRet && gDpair)
    {
        RemoveCredential(&gDpair->pdeviceID);
        gDpair = NULL;
    }

    DeleteDpairingBinData(newDpair);
    OIC_LOG_V (DEBUG, TAG, "%s RetVal %d", __func__ , ehRet);
    return ehRet;
}

static OCEntityHandlerResult HandleDpairingPutRequest (const OCEntityHandlerRequest * ehRequest)
{
    OIC_LOG (DEBUG, TAG, "Dpairing EntityHandle  processing PUT request (Comfirmation)");

    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OicSecDpairing_t* newDpair = NULL;
    OCStackResult res = OC_STACK_OK;

    const OicSecPconf_t *pconf = GetPconfResourceData();
    if (true == pconf->edp)
    {
        uint8_t *payload = ((OCSecurityPayload*)ehRequest->payload)->securityData;
        size_t size = ((OCSecurityPayload*)ehRequest->payload)->payloadSize;
        if (payload)
        {
            res = CBORPayloadToDpair(payload, size, &newDpair);
        }

    }
    else
    {
        OIC_LOG (DEBUG, TAG, "EDP == false : Direct-Pairing Disabled");
        ehRet = OC_EH_ERROR;
    }


    if ((OC_STACK_OK == res) && gDpair && newDpair)
    {
        OIC_LOG(DEBUG, TAG, "Received direct-pairing finalization request");

        // Check if valid Put request
        VERIFY_SUCCESS(TAG, PRM_NOT_ALLOWED == newDpair->spm, ERROR);

        const OicSecPconf_t *pconf = GetPconfResourceData();
        VERIFY_NON_NULL(TAG, pconf, ERROR);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        OCServerRequest *request = GetServerRequestUsingHandle(ehRequest->requestHandle);
        VERIFY_NON_NULL(TAG, request, ERROR);
        VERIFY_SUCCESS(TAG, (request->devAddr.flags | OC_FLAG_SECURE), ERROR);

        //Generate new credential
        OIC_LOG_V(INFO, TAG, "SavePairingPSK for %s(%d)", request->devAddr.addr,
                request->devAddr.port);
        OCStackResult res = SavePairingPSK(&request->devAddr, &newDpair->pdeviceID,
                (OicUuid_t *)&pconf->rownerID, true);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);
#endif // __WITH_DTLS__ or __WITH_TLS__

        //Generate new acl
        OicSecPdAcl_t *pdAcl;
        LL_FOREACH(pconf->pdacls, pdAcl)
        {
            OicSecAcl_t* acl = (OicSecAcl_t*)OICCalloc(1, sizeof(OicSecAcl_t));
            VERIFY_NON_NULL(TAG, acl, ERROR);

            OicSecAce_t* ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
            VERIFY_NON_NULL(TAG, ace, ERROR);

            LL_APPEND(acl->aces, ace);

            memcpy(&ace->subjectuuid, &gDpair->pdeviceID, sizeof(OicUuid_t));

            for(size_t i = 0; i < pdAcl->resourcesLen; i++)
            {
                OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
                VERIFY_NON_NULL(TAG, rsrc, ERROR);
                LL_APPEND(ace->resources, rsrc);

                //href
                rsrc->href = OICStrdup(pdAcl->resources[i]);

                // TODO: Append 'if' and 'rt' as workaround
                // if
                rsrc->interfaceLen = 1;
                rsrc->interfaces = (char**)OICCalloc(rsrc->interfaceLen, sizeof(char));
                VERIFY_NON_NULL(TAG, (rsrc->interfaces), ERROR);
                rsrc->interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
                VERIFY_NON_NULL(TAG, (rsrc->interfaces[0]), ERROR);

                //rt
                rsrc->typeLen = 1;
                rsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char));
                VERIFY_NON_NULL(TAG, (rsrc->types), ERROR);
                rsrc->types[0] = OICStrdup("oic.core");
                VERIFY_NON_NULL(TAG, (rsrc->types[0]), ERROR);
            }

            ace->permission = pdAcl->permission;

            //Copy the validity
            if(pdAcl->periods || pdAcl->recurrences)
            {
                OicSecValidity_t* validity = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
                VERIFY_NON_NULL(TAG, validity, ERROR);

                if(pdAcl->periods && pdAcl->periods[0])
                {
                    size_t periodLen = strlen(pdAcl->periods[0]) + 1;
                    validity->period = (char*)OICMalloc(periodLen * sizeof(char));
                    VERIFY_NON_NULL(TAG, (validity->period), ERROR);
                    OICStrcpy(validity->period, periodLen, pdAcl->periods[0]);
                }

                if(pdAcl->recurrences && 0 < pdAcl->prdRecrLen)
                {
                    validity->recurrenceLen = pdAcl->prdRecrLen;
                    validity->recurrences = (char**)OICMalloc(sizeof(char*) * pdAcl->prdRecrLen);
                    VERIFY_NON_NULL(TAG, (validity->recurrences), ERROR);

                    for(size_t i = 0; i < pdAcl->prdRecrLen; i++)
                    {
                        size_t recurrenceLen = strlen(pdAcl->recurrences[i]) + 1;
                        validity->recurrences[i] = (char*)OICMalloc(recurrenceLen  * sizeof(char));
                        VERIFY_NON_NULL(TAG, (validity->recurrences[i]), ERROR);

                        OICStrcpy(validity->recurrences[i], recurrenceLen, pdAcl->recurrences[i]);
                    }
                }

                LL_APPEND(ace->validities, validity);
            }

            size_t size = 0;
            uint8_t *payload = NULL;
            if (OC_STACK_OK == AclToCBORPayload(acl, &payload, &size))
            {
                AppendACL(payload, size);
                OICFree(payload);
            }
            DeleteACLList(acl);
        }

        //update pconf device list
        AddPairedDevice(&newDpair->pdeviceID);

        //Initialize dpairing resource
        gDpair = NULL;

        OIC_LOG (DEBUG, TAG, "/oic/sec/dpairing resource updated,"
                "direct-pairing finalization success");
        ehRet = OC_EH_OK;
    }

exit:

    //Send payload to request originator
    if(OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG (ERROR, TAG, "SendSRMResponse failed in HandleDpairingPutRequest");
    }

    DeleteDpairingBinData(newDpair);
    OIC_LOG_V (DEBUG, TAG, "%s RetVal %d", __func__ , ehRet);
    return ehRet;
}
/*
 * This internal method is the entity handler for Dpairing resources and
 * will handle REST request (GET/POST) for them.
 */
OCEntityHandlerResult DpairingEntityHandler (OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest * ehRequest,
                                        void* callbackParameter)
{
    OIC_LOG(DEBUG, TAG, "Received request DpairingEntityHandler");
    (void)callbackParameter;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    if (!ehRequest)
    {
        return ehRet;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG (DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                break;

            case OC_REST_POST:
                ehRet = HandleDpairingPostRequest(ehRequest);
                break;

            case OC_REST_PUT:
                ehRet = HandleDpairingPutRequest(ehRequest);
                break;

            case OC_REST_DELETE:
                break;

            default:
                ehRet = OC_EH_ERROR;
                SendSRMResponse(ehRequest, ehRet, NULL, 0);
        }
    }

    return ehRet;
}

/*
 * This internal method is used to create '/oic/sec/dpairing' resource.
 */
OCStackResult CreateDpairingResource()
{
    OCStackResult ret;

    ret = OCCreateResource(&gDpairHandle,
                           OIC_RSRC_TYPE_SEC_DPAIRING,
                           OC_RSRVD_INTERFACE_DEFAULT,
                           OIC_RSRC_DPAIRING_URI,
                           DpairingEntityHandler,
                           NULL,
                           OC_SECURE | OC_EXPLICIT_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Unable to instantiate Dpairing resource");
        DeInitDpairingResource();
    }
    return ret;
}

/**
 * Initialize Dpairing resource by loading data from persistent storage.
 *
 * @retval  OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult InitDpairingResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    // Instantiate 'oic.sec.dpairing'
    ret = CreateDpairingResource();
    if (OC_STACK_OK != ret)
    {
        DeInitDpairingResource();
    }
    return ret;
}

/**
 * Perform cleanup for Dpairing resources.
 *
 * @return
 * OC_STACK_OK    - no error
 * OC_STACK_ERROR - stack process error
 *
 */
OCStackResult DeInitDpairingResource()
{
    OCStackResult ret = OCDeleteResource(gDpairHandle);
    gDpair = NULL;

    if(OC_STACK_OK == ret)
    {
        return OC_STACK_OK;
    }
    else
    {
        return OC_STACK_ERROR;
    }
}

OCStackResult SetDpairingRownerId(const OicUuid_t* newROwner)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t *cborPayload = NULL;
    size_t size = 0;
    OicUuid_t prevId = {.id={0}};

    if(NULL == newROwner)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    if(NULL == gDpair)
    {
        ret = OC_STACK_NO_RESOURCE;
    }

    if(newROwner && gDpair)
    {
        memcpy(prevId.id, gDpair->rownerID.id, sizeof(prevId.id));
        memcpy(gDpair->rownerID.id, newROwner->id, sizeof(newROwner->id));

        ret = DpairingToCBORPayload(gDpair, &cborPayload, &size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_DPAIRING_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }

    return ret;

exit:
    OICFree(cborPayload);
    memcpy(gDpair->rownerID.id, prevId.id, sizeof(prevId.id));
    return ret;
}

OCStackResult GetDpairingRownerId(OicUuid_t *rowneruuid)
{
    OCStackResult retVal = OC_STACK_ERROR;
    if (gDpair)
    {
        *rowneruuid = gDpair->rownerID;
        retVal = OC_STACK_OK;
    }
    return retVal;
}
