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
#include <stdlib.h>
#include <string.h>

#include "ocstack.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "oic_malloc.h"
#include "utlist.h"
#include "payload_logging.h"
#include "resourcemanager.h"
#include "psinterface.h"
#include "svcresource.h"
#include "srmresourcestrings.h"
#include "srmutility.h"

#include "security_internals.h"

#define TAG  "OIC_SRM_SVC"

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint16_t CBOR_SIZE = 512;

/** Max cbor size payload. */
static const uint16_t CBOR_MAX_SIZE = 4400;

/** SVC Map size - Number of mandatory items. */
static const uint8_t SVC_MAP_SIZE = 3;

static OicSecSvc_t        *gSvc = NULL;
static OCResourceHandle    gSvcHandle = NULL;

void DeleteSVCList(OicSecSvc_t* svc)
{
    if (svc)
    {
        OicSecSvc_t *svcTmp1 = NULL, *svcTmp2 = NULL;
        LL_FOREACH_SAFE(svc, svcTmp1, svcTmp2)
        {
            LL_DELETE(svc, svcTmp1);

            // Clean Owners
            OICFree(svcTmp1->owners);

            // Clean SVC node itself
            OICFree(svcTmp1);
        }
    }
}

static size_t svcElementsCount(const OicSecSvc_t *secSvc)
{
    size_t size = 0;
    for (const OicSecSvc_t *svc = secSvc; svc; svc = svc->next)
    {
        size++;
    }
    return size;
}

OCStackResult SVCToCBORPayload(const OicSecSvc_t *svc, uint8_t **cborPayload,
                               size_t *cborSize)
{
    if (NULL == svc || NULL == cborPayload || NULL != *cborPayload || NULL == cborSize)
    {
       return OC_STACK_INVALID_PARAM;
    }

    size_t cborLen = *cborSize;
    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }
    *cborPayload = NULL;
    *cborSize = 0;

    int64_t cborEncoderResult = CborNoError;
    OCStackResult ret = OC_STACK_ERROR;
    CborEncoder encoder;
    CborEncoder svcArray;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);

    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Create SVC Array
    cborEncoderResult = cbor_encoder_create_array(&encoder, &svcArray, svcElementsCount(svc));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Create SVC Array.");

    while (svc)
    {
        CborEncoder svcMap;
        CborEncoder owners;

        cborEncoderResult = cbor_encoder_create_map(&svcArray, &svcMap, SVC_MAP_SIZE);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Create SVC Map.");

        // Service Device Identity
        cborEncoderResult = cbor_encode_text_string(&svcMap, OIC_JSON_SERVICE_DEVICE_ID,
            strlen(OIC_JSON_SERVICE_DEVICE_ID));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Add SVC Device Id.");
        cborEncoderResult = cbor_encode_byte_string(&svcMap, (uint8_t *)svc->svcdid.id,
            sizeof(svc->svcdid.id));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to ");

        // Service Type
        cborEncoderResult = cbor_encode_text_string(&svcMap, OIC_JSON_SERVICE_TYPE,
            strlen(OIC_JSON_SERVICE_TYPE));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Add SVC Serv Type Tag.");
        cborEncoderResult = cbor_encode_int(&svcMap, svc->svct);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Add SVC Serv Type Value.");

        // Owners
        // TODO: Need to modification to single ROwner, (Currently SINGLE_SERVICE_CLIENT_DRIVEN only)
        cborEncoderResult = cbor_encode_text_string(&svcMap, OIC_JSON_OWNERS_NAME,
            strlen(OIC_JSON_OWNERS_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Add SVC Owners Tag.");
        cborEncoderResult = cbor_encoder_create_array(&svcMap, &owners, svc->ownersLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Add SVC Array.");
        for (size_t i = 0; i < svc->ownersLen; i++)
        {
            cborEncoderResult = cbor_encode_byte_string(&owners, (uint8_t *)svc->owners[i].id,
                sizeof(svc->owners[i].id));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Add SVC Owners Value.");
        }
        cborEncoderResult = cbor_encoder_close_container(&svcMap, &owners);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Close SVC Array.");

        cborEncoderResult = cbor_encoder_close_container(&svcArray, &svcMap);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Close SVC Map.");

        svc = svc->next;
    }

    cborEncoderResult = cbor_encoder_close_container(&encoder, &svcArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to Close SVC Array.");

    if (CborNoError == cborEncoderResult)
    {
        *cborPayload = outPayload;
        *cborSize = cbor_encoder_get_buffer_size(&encoder, outPayload);
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
        ret = SVCToCBORPayload(svc, cborPayload, &cborLen);
        *cborSize = cborLen;
    }

    if (CborNoError != cborEncoderResult)
    {
        OICFree(outPayload);
        outPayload = NULL;
        *cborSize = 0;
        *cborPayload = NULL;
        ret = OC_STACK_ERROR;
    }

    return ret;
}

OCStackResult CBORPayloadToSVC(const uint8_t *cborPayload, size_t size,
                               OicSecSvc_t **secSvc)
{
    if (NULL == cborPayload || NULL == secSvc || NULL != *secSvc || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    *secSvc = NULL;

    OCStackResult ret = OC_STACK_ERROR;

    CborValue svcCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;

    cbor_parser_init(cborPayload, size, 0, &parser, &svcCbor);
    OicSecSvc_t *headSvc = NULL;

    CborValue svcArray = { .parser = NULL };
    cborFindResult = cbor_value_enter_container(&svcCbor, &svcArray);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Enter SVC Array.");

    while (cbor_value_is_valid(&svcArray))
    {
        CborValue svcMap = { .parser = NULL };
        OicSecSvc_t *svc = (OicSecSvc_t *) OICCalloc(1, sizeof(OicSecSvc_t));
        VERIFY_NON_NULL(TAG, svc, ERROR);
        cborFindResult = cbor_value_enter_container(&svcArray, &svcMap);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Enter SVC Map.");

        while (cbor_value_is_valid(&svcMap))
        {
            char* name = NULL;
            size_t len = 0;
            CborType type = CborInvalidType;

            cborFindResult = cbor_value_dup_text_string(&svcMap, &name, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Find Name.");
            cborFindResult = cbor_value_advance(&svcMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Advance.");

            type = cbor_value_get_type(&svcMap);
            // Service Device Identity
            if (0 == strcmp(OIC_JSON_SERVICE_DEVICE_ID, name) && cbor_value_is_byte_string(&svcMap))
            {
                uint8_t *subjectId = NULL;
                cborFindResult = cbor_value_dup_byte_string(&svcMap, &subjectId, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Find SubjectId.");
                memcpy(svc->svcdid.id, subjectId, len);
                OICFree(subjectId);
            }
            // Service Type
            if (0 == strcmp(OIC_JSON_SERVICE_TYPE, name) && cbor_value_is_integer(&svcMap))
            {
                int svct;

                cborFindResult = cbor_value_get_int(&svcMap, &svct);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Find SVCT.");
                svc->svct = (OicSecSvcType_t)svct;
            }

            // Owners -- Mandatory
            if (0 == strcmp(OIC_JSON_OWNERS_NAME, name) && cbor_value_is_array(&svcMap))
            {
                int i = 0;
                CborValue owners = { .parser = NULL };

                cborFindResult = cbor_value_get_array_length(&svcMap, &svc->ownersLen);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Find Owner Len.");
                cborFindResult = cbor_value_enter_container(&svcMap, &owners);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Enter Owner Array.");
                svc->owners = (OicUuid_t *)OICCalloc(svc->ownersLen, sizeof(*svc->owners));
                VERIFY_NON_NULL(TAG, svc->owners, ERROR);

                while (cbor_value_is_valid(&owners) && cbor_value_is_byte_string(&owners))
                {
                    uint8_t *owner = NULL;
                    cborFindResult = cbor_value_dup_byte_string(&owners, &owner, &len, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Find Owner Array Value.");
                    cborFindResult = cbor_value_advance(&owners);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Advance Owner Array.");
                    memcpy(svc->owners[i++].id, owner, len);
                    OICFree(owner);
                }
            }
            if (CborMapType != type  && cbor_value_is_valid(&svcMap))
            {
                cborFindResult = cbor_value_advance(&svcMap);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Advance SVC.");
            }
            OICFree(name);
        }

        svc->next = NULL;
        if (NULL == headSvc)
        {
            headSvc = svc;
        }
        else
        {
            OicSecSvc_t *temp = headSvc;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = svc;
        }
        if (cbor_value_is_valid(&svcArray))
        {
            cborFindResult = cbor_value_advance(&svcArray);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to Find Name.");
        }
    }
    *secSvc = headSvc;
    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        DeleteSVCList(headSvc);
        headSvc = NULL;
        *secSvc = NULL;
        ret = OC_STACK_ERROR;
    }
    return ret;
}

static OCEntityHandlerResult HandleSVCGetRequest(const OCEntityHandlerRequest * ehRequest)
{
    // Convert SVC data into JSON for transmission
    size_t size = 0;
    uint8_t *cborSvc = NULL;
    OCStackResult res =  SVCToCBORPayload(gSvc, &cborSvc, &size);
    OCEntityHandlerResult ehRet = (res == OC_STACK_OK) ? OC_EH_OK : OC_EH_ERROR;

    // Send response payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, cborSvc, size)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    OICFree(cborSvc);
    OIC_LOG_V (DEBUG, TAG, "%s RetVal %d", __func__ , ehRet);
    return ehRet;
}

static OCEntityHandlerResult HandleSVCPostRequest(const OCEntityHandlerRequest * ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    uint8_t *payload = ((OCSecurityPayload *) ehRequest->payload)->securityData;
    size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;
    if (payload)
    {
        // Convert CBOR SVC data into SVC. This will also validate the SVC data received.
        OicSecSvc_t *newSvc = NULL;
        OCStackResult res =  CBORPayloadToSVC(payload, size, &newSvc);
        if (newSvc && res == OC_STACK_OK)
        {
            // Append the new SVC to existing SVC
            LL_APPEND(gSvc, newSvc);

            // Convert SVC data into JSON for update to persistent storage
            size_t size = 0;
            uint8_t *cborPayload = NULL;
            res = SVCToCBORPayload(gSvc, &cborPayload, &size);
            if (cborPayload && OC_STACK_OK == res &&
                UpdateSecureResourceInPS(OIC_JSON_SVC_NAME, cborPayload, size) == OC_STACK_OK)
            {
                ehRet = OC_EH_CHANGED;
            }
            OICFree(cborPayload);
        }
    }

    // Send payload to request originator
    if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleSVCPostRequest");
    }

    OIC_LOG_V (DEBUG, TAG, "%s RetVal %d", __func__ , ehRet);
    return ehRet;
}

/**
 * This internal method is the entity handler for SVC resources and
 * will handle REST request (GET/PUT/POST/DEL) for them.
 */
static OCEntityHandlerResult SVCEntityHandler(OCEntityHandlerFlag flag,
                                              OCEntityHandlerRequest * ehRequest,
                                              void* callbackParameter)
{
    (void) callbackParameter;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    if (!ehRequest)
    {
        return ehRet;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandleSVCGetRequest(ehRequest);
                break;

            case OC_REST_POST:
                ehRet = HandleSVCPostRequest(ehRequest);
                break;

            default:
                ehRet = OC_EH_ERROR;
                SendSRMResponse(ehRequest, ehRet, NULL, 0);
        }
    }

    return ehRet;
}

/**
 * This internal method is used to create '/oic/sec/svc' resource.
 */
static OCStackResult CreateSVCResource()
{
    OCStackResult ret = OCCreateResource(&gSvcHandle,
                                         OIC_RSRC_TYPE_SEC_SVC,
                                         OC_RSRVD_INTERFACE_DEFAULT,
                                         OIC_RSRC_SVC_URI,
                                         SVCEntityHandler,
                                         NULL,
                                         OC_OBSERVABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG(FATAL, TAG, "Unable to instantiate SVC resource");
        DeInitSVCResource();
    }
    return ret;
}

OCStackResult InitSVCResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    OIC_LOG_V(DEBUG, TAG, "Begin %s ", __func__ );

    uint8_t *data = NULL;
    size_t size = 0;
    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_SVC_NAME, &data, &size);
    // If database read failed
    if (ret != OC_STACK_OK)
    {
        OIC_LOG (DEBUG, TAG, "ReadSVDataFromPS failed");
    }

    if (data)
    {
        // Convert CBOR SVC into binary format
        ret = CBORPayloadToSVC(data, size, &gSvc);
        if (ret != OC_STACK_OK)
        {
            OIC_LOG (DEBUG, TAG, " ConvertCBOR SVC into binary format failed");
        }
        OICFree(data);
    }

    // Instantiate 'oic.sec.svc'
    ret = CreateSVCResource();

    if (OC_STACK_OK != ret)
    {
        DeInitSVCResource();
    }

    OIC_LOG_V(DEBUG, TAG, "%s RetVal %d", __func__ , ret);
    return ret;
}

void DeInitSVCResource()
{
    OCDeleteResource(gSvcHandle);
    gSvcHandle = NULL;

    DeleteSVCList(gSvc);
    gSvc = NULL;
}
