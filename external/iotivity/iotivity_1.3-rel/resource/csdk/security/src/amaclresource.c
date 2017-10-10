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

#include "iotivity_config.h"
#include <stdlib.h>
#include <string.h>
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "payload_logging.h"
#include "psinterface.h"
#include "resourcemanager.h"
#include "utlist.h"
#include "srmresourcestrings.h"
#include "srmutility.h"
#include "amaclresource.h"

#define TAG  "OIC_SRM_AMACL"

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint16_t CBOR_SIZE = 1024;

/* Max cbor size payload. */
static const uint16_t CBOR_MAX_SIZE = 4400;

/** AMACL Map size - Number of mandatory items. */
static const uint8_t AMACL_MAP_SIZE = 3;
static const uint8_t AMACL_RSRC_MAP_SIZE = 1;
static const uint8_t AMACL_RLIST_MAP_SIZE = 3;

static OicSecAmacl_t *gAmacl = NULL;
static OCResourceHandle gAmaclHandle = NULL;

void DeleteAmaclList(OicSecAmacl_t* amacl)
{
    if (amacl)
    {
        OicSecAmacl_t *amaclTmp1 = NULL, *amaclTmp2 = NULL;
        LL_FOREACH_SAFE(amacl, amaclTmp1, amaclTmp2)
        {
            LL_DELETE(amacl, amaclTmp1);

            // Clean Resources
            for (size_t i = 0; i < amaclTmp1->resourcesLen; i++)
            {
                OICFree(amaclTmp1->resources[i]);
            }
            OICFree(amaclTmp1->resources);

            // Clean Amacl node itself
            OICFree(amaclTmp1);
        }
    }
}

OCStackResult AmaclToCBORPayload(const OicSecAmacl_t *amaclS, uint8_t **cborPayload, size_t *cborSize)
{
    if (NULL == amaclS || NULL == cborPayload || NULL != *cborPayload || NULL == cborSize)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    size_t cborLen = *cborSize;
    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }

    *cborSize = 0;
    *cborPayload = NULL;

    CborEncoder encoder;
    CborEncoder amaclMap;
    int64_t cborEncoderResult = CborNoError;
    CborEncoder rsrcMap;
    CborEncoder rlistArray;

    const OicSecAmacl_t *amacl = amaclS;
    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NOT_NULL_RETURN(TAG, outPayload, ERROR, OC_STACK_ERROR);

    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Create AMACL Map
    cborEncoderResult = cbor_encoder_create_map(&encoder, &amaclMap, AMACL_MAP_SIZE);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding AMACL Map.");

    // resources -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&amaclMap, OIC_JSON_RESOURCES_NAME,
                strlen(OIC_JSON_RESOURCES_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding Resource Name Tag.");

    cborEncoderResult = cbor_encoder_create_map(&amaclMap, &rsrcMap, AMACL_RSRC_MAP_SIZE);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding Resource Map.");


    cborEncoderResult = cbor_encode_text_string(&rsrcMap, OIC_JSON_RLIST_NAME,
                strlen(OIC_JSON_RLIST_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RLIST Name Tag.");

    // TODO : Need to input array length by OicSecAmacl_t->resources->rlistLen based on spec.
    cborEncoderResult = cbor_encoder_create_array(&rsrcMap, &rlistArray, amacl->resourcesLen);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RLIST Array.");

    // TODO : Need to add OicSecAmacl_t->rlist as array rMap based on RAML spec.
    for (size_t i = 0; i < amacl->resourcesLen; i++)
    {
        // TODO : Need to create rMap structure based on RAML spec.
        CborEncoder rMap;
        cborEncoderResult = cbor_encoder_create_map(&rlistArray, &rMap, AMACL_RLIST_MAP_SIZE);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RLIST Map.");

        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_HREF_NAME,
                strlen(OIC_JSON_HREF_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding HREF Name Tag.");
        cborEncoderResult = cbor_encode_text_string(&rMap, amacl->resources[i],
                strlen(amacl->resources[i]));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding HREF Value in Map.");

        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_RT_NAME,
                strlen(OIC_JSON_RT_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");

        // TODO : Need to assign real value of RT
        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                strlen(OIC_JSON_EMPTY_STRING));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Value.");

        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_IF_NAME,
                strlen(OIC_JSON_IF_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");

        // TODO : Need to assign real value of IF
        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                strlen(OIC_JSON_EMPTY_STRING));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Value.");

        cborEncoderResult = cbor_encoder_close_container(&rlistArray, &rMap);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing RLIST Array.");
    }

    cborEncoderResult = cbor_encoder_close_container(&rsrcMap, &rlistArray);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing RLIST Array.");


    cborEncoderResult = cbor_encoder_close_container(&amaclMap, &rsrcMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing Resource Map.");

    cborEncoderResult = cbor_encoder_close_container(&encoder, &amaclMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing Amacl Map.");

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
       ret = AmaclToCBORPayload(amaclS, cborPayload, &cborLen);
       if (OC_STACK_OK == ret)
       {
           *cborSize = cborLen;
           ret = OC_STACK_OK;
       }
    }

    if (CborNoError != cborEncoderResult || ret != OC_STACK_OK)
    {
       OICFree(outPayload);
       outPayload = NULL;
       *cborSize = 0;
       *cborPayload = NULL;
       ret = OC_STACK_ERROR;
    }

    return ret;
}

OCStackResult CBORPayloadToAmacl(const uint8_t *cborPayload, size_t size,
                                 OicSecAmacl_t **secAmacl)
{
    if (NULL == cborPayload || NULL == secAmacl || NULL != *secAmacl || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    *secAmacl = NULL;

    OCStackResult ret = OC_STACK_ERROR;

    CborValue amaclCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;

    cbor_parser_init(cborPayload, size, 0, &parser, &amaclCbor);
    OicSecAmacl_t *headAmacl = (OicSecAmacl_t *)OICCalloc(1, sizeof(OicSecAmacl_t));
    VERIFY_NOT_NULL_RETURN(TAG, headAmacl, ERROR, OC_STACK_NO_MEMORY);

    CborValue amaclMap = { .parser = NULL };
    cborFindResult = cbor_value_enter_container(&amaclCbor, &amaclMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Entering Amacl Map.");

    while(cbor_value_is_valid(&amaclMap) && cbor_value_is_text_string(&amaclMap))
    {
        char *name = NULL;
        size_t len = 0;
        cborFindResult = cbor_value_dup_text_string(&amaclMap, &name, &len, NULL);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Amacl Data Name Tag.");
        cborFindResult = cbor_value_advance(&amaclMap);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Amacl Data Value.");

        //CborType type = cbor_value_get_type(&amaclMap);

        // Resources -- Mandatory
        if (0 == strcmp(OIC_JSON_RESOURCES_NAME, name))
        {
            // resource map
            CborValue rsrcMap = { .parser = NULL  };
            cborFindResult = cbor_value_enter_container(&amaclMap, &rsrcMap);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Entering Resource Map");

            while(cbor_value_is_valid(&rsrcMap) && cbor_value_is_text_string(&rsrcMap))
            {
                // resource name
                char *rsrcName = NULL;
                size_t rsrcNameLen = 0;
                cborFindResult = cbor_value_dup_text_string(&rsrcMap, &rsrcName, &rsrcNameLen, NULL);
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Resource Data Name Tag.");
                cborFindResult = cbor_value_advance(&rsrcMap);
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Resource Data Value.");

                // rlist
                if (0 == strcmp(OIC_JSON_RLIST_NAME, rsrcName))
                {
                    int i = 0;
                    // TODO : Need to assign array length to OicSecAmacl_t->resources->rlistLen based of RAML spec.
                    cborFindResult = cbor_value_get_array_length(&rsrcMap, &headAmacl->resourcesLen);
                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Rlist Array Len.");

                    CborValue rsrcArray = { .parser = NULL  };

                    // rlist array
                    cborFindResult = cbor_value_enter_container(&rsrcMap, &rsrcArray);
                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Entering Rlist Array");

                    // TODO : Need to check data structure of OicSecAmacl_t based on RAML spec.
                    headAmacl->resources = (char **) OICCalloc(headAmacl->resourcesLen, sizeof(*headAmacl->resources));
                    VERIFY_NOT_NULL(TAG, headAmacl->resources, ERROR);

                    while (cbor_value_is_valid(&rsrcArray))
                    {
                        // rMap
                        CborValue rMap = { .parser = NULL  };
                        cborFindResult = cbor_value_enter_container(&rsrcArray, &rMap);
                        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Entering Rlist Map");

                        while(cbor_value_is_valid(&rMap) && cbor_value_is_text_string(&rMap))
                        {
                            char *rMapName = NULL;
                            size_t rMapNameLen = 0;
                            cborFindResult = cbor_value_dup_text_string(&rMap, &rMapName, &rMapNameLen, NULL);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding RMap Data Name Tag.");
                            cborFindResult = cbor_value_advance(&rMap);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding RMap Data Value.");

                            // "href"
                            if (0 == strcmp(OIC_JSON_HREF_NAME, rMapName))
                            {
                                // TODO : Need to check data structure of OicSecAmacl_t based on RAML spec.
                                cborFindResult = cbor_value_dup_text_string(&rMap, &headAmacl->resources[i++], &len, NULL);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Href Value.");
                            }

                            // "rt"
                            if (0 == strcmp(OIC_JSON_RT_NAME, rMapName))
                            {
                                // TODO : Need to check data structure of OicSecAmacl_t and assign based on RAML spec.
                                char *rtData = NULL;
                                cborFindResult = cbor_value_dup_text_string(&rMap, &rtData, &len, NULL);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding RT Value.");
                                OICFree(rtData);
                            }

                            // "if"
                            if (0 == strcmp(OIC_JSON_IF_NAME, rMapName))
                            {
                                // TODO : Need to check data structure of OicSecAmacl_t and assign based on RAML spec.
                                char *ifData = NULL;
                                cborFindResult = cbor_value_dup_text_string(&rMap, &ifData, &len, NULL);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding IF Value.");
                                OICFree(ifData);
                            }

                            if (cbor_value_is_valid(&rMap))
                            {
                                cborFindResult = cbor_value_advance(&rMap);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Rlist Map.");
                            }
                            OICFree(rMapName);
                        }

                        if (cbor_value_is_valid(&rsrcArray))
                        {
                            cborFindResult = cbor_value_advance(&rsrcArray);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Resource Array.");
                        }
                    }
                }

                if (cbor_value_is_valid(&rsrcMap))
                {
                    cborFindResult = cbor_value_advance(&rsrcMap);
                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Resource Map.");
                }
                OICFree(rsrcName);
            }
        }

        OICFree(name);
    }

    *secAmacl = headAmacl;
    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        DeleteAmaclList(headAmacl);
        headAmacl = NULL;
        *secAmacl = NULL;
        ret = OC_STACK_ERROR;
    }
    return ret;
}

static OCEntityHandlerResult HandleAmaclGetRequest (const OCEntityHandlerRequest * ehRequest)
{
    // Convert Amacl data into JSON for transmission
    size_t size = 0;
    uint8_t *cborPayload = NULL;
    OCStackResult res = AmaclToCBORPayload(gAmacl, &cborPayload, &size);

    OCEntityHandlerResult ehRet = (res == OC_STACK_OK) ? OC_EH_OK : OC_EH_ERROR;

    // Send response payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, cborPayload, size)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    OICFree(cborPayload);

    OIC_LOG_V (DEBUG, TAG, "%s RetVal %d", __func__ , ehRet);
    return ehRet;
}

static OCEntityHandlerResult HandleAmaclPostRequest (const OCEntityHandlerRequest * ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    // Convert CBOR Amacl data into binary. This will also validate the Amacl data received.
    uint8_t *payload = ((OCSecurityPayload *) ehRequest->payload)->securityData;
    size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;
    if (payload)
    {
        OicSecAmacl_t *newAmacl = NULL;
        OCStackResult res = CBORPayloadToAmacl(payload, size, &newAmacl);
        if (newAmacl && OC_STACK_OK == res)
        {
            // Append the new Amacl to existing Amacl
            LL_APPEND(gAmacl, newAmacl);
            size_t cborSize = 0;
            // Convert Amacl data into JSON for update to persistent storage.
            uint8_t *cborPayload = NULL;
            res = AmaclToCBORPayload(gAmacl, &cborPayload, &cborSize);
            if (cborPayload && (OC_STACK_OK == res) &&
                (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_AMACL_NAME, cborPayload, cborSize)))
            {
                ehRet = OC_EH_RESOURCE_CREATED;
            }
            OICFree(cborPayload);
        }
        OICFree(payload);
    }

    // Send payload to request originator
    if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleAmaclPostRequest");
    }

    OIC_LOG_V(DEBUG, TAG, "%s RetVal %d", __func__ , ehRet);
    return ehRet;
}

/**
 * This internal method is the entity handler for Amacl resources and
 * will handle REST request (GET/PUT/POST/DEL) for them.
 */
static OCEntityHandlerResult AmaclEntityHandler (OCEntityHandlerFlag flag,
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
        OIC_LOG (DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandleAmaclGetRequest(ehRequest);
                break;

            case OC_REST_POST:
                ehRet = HandleAmaclPostRequest(ehRequest);
                break;

            default:
                ehRet = OC_EH_ERROR;
                SendSRMResponse(ehRequest, ehRet, NULL, 0);
        }
    }

    return ehRet;
}

/**
 * This internal method is used to create '/oic/sec/amacl' resource.
 */
static OCStackResult CreateAmaclResource()
{
    OCStackResult ret = OCCreateResource(&gAmaclHandle,
                                         OIC_RSRC_TYPE_SEC_AMACL,
                                         OC_RSRVD_INTERFACE_DEFAULT,
                                         OIC_RSRC_AMACL_URI,
                                         AmaclEntityHandler,
                                         NULL,
                                         OC_OBSERVABLE |
                                         OC_SECURE |
                                         OC_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG (FATAL, TAG, "Unable to instantiate Amacl resource");
        DeInitAmaclResource();
    }
    return ret;
}

OCStackResult InitAmaclResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    uint8_t *data = NULL;
    size_t size = 0;
    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_AMACL_NAME, &data, &size);

    // If database read failed
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(DEBUG, TAG, "ReadSVDataFromPS failed");
    }
    if (data)
    {
        // Read AMACL resource from PS
        ret = CBORPayloadToAmacl(data, size, &gAmacl);
        if (OC_STACK_OK != ret)
        {
            OIC_LOG(DEBUG, TAG, "ReadAMACLresourcefromPS failed");
        }
        OICFree(data);
    }

    // Instantiate 'oic/sec/amacl' resource
    ret = CreateAmaclResource();

    if (OC_STACK_OK != ret)
    {
        DeInitAmaclResource();
    }
    return ret;
}

void DeInitAmaclResource()
{
    OCDeleteResource(gAmaclHandle);
    gAmaclHandle = NULL;

    DeleteAmaclList(gAmacl);
    gAmacl = NULL;
}
