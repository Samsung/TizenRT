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

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "payload_logging.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "cainterface.h"
#include "ocserverrequest.h"
#include "resourcemanager.h"
#include "verresource.h"
#include "doxmresource.h"
#include "psinterface.h"
#include "srmresourcestrings.h"
#include "securevirtualresourcetypes.h"
#include "srmutility.h"

#define TAG  "OIC_SEC_VER"

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint8_t CBOR_SIZE = 255;

/** Max cbor size payload. */
static const uint16_t CBOR_MAX_SIZE = 4400;

/** VER Map size - Number of mandatory items. */
static const uint8_t VER_MAP_SIZE = 2;

static OCResourceHandle    gVerHandle = NULL;

/** Security version is mapped with iotivity release version */
const char* SECURITY_VERSION = IOTIVITY_VERSION;

static OicSecVer_t gVer =
{
    {0},                  /* char *secv */
    {.id = {0}},         /* OicUuid_t deviceID */
};

void DeleteVerBinData(OicSecVer_t* ver)
{
    if (ver)
    {
        //Clean ver itself
        OICFree(ver);
    }
}

OCStackResult VerToCBORPayload(const OicSecVer_t *ver, uint8_t **payload, size_t *size)
{
    if (NULL == ver || NULL == payload || NULL != *payload || NULL == size)
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
    CborEncoder verMap;

    int64_t cborEncoderResult = CborNoError;
    uint8_t mapSize = VER_MAP_SIZE;
    char* strUuid = NULL;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    cborEncoderResult |= cbor_encoder_create_map(&encoder, &verMap, mapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Ver Map.");

    //SecV -- Mandatory
    cborEncoderResult |= cbor_encode_text_string(&verMap, OIC_JSON_SEC_V_NAME,
        strlen(OIC_JSON_SEC_V_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SecV Tag.");
    cborEncoderResult |= cbor_encode_text_string(&verMap, ver->secv, strlen(ver->secv));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SecV Value.");

    //DeviceId -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&verMap, OIC_JSON_DEVICE_ID_NAME,
        strlen(OIC_JSON_DEVICE_ID_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Id Tag.");
    ret = ConvertUuidToStr(&ver->deviceID, &strUuid);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
    cborEncoderResult = cbor_encode_text_string(&verMap, strUuid, strlen(strUuid));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Id Value.");
    OICFree(strUuid);
    strUuid = NULL;


    // Close ver(first) container
    cborEncoderResult |= cbor_encoder_close_container(&encoder, &verMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing VerMap.");

    if (CborNoError == cborEncoderResult)
    {
        *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
        *payload = outPayload;
        ret = OC_STACK_OK;
    }
exit:
    if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
    {
        OIC_LOG(DEBUG, TAG, "Memory getting reallocated.");
        // reallocate and try again!
        OICFree(outPayload);
        outPayload = NULL;
        // Since the allocated initial memory failed, double the memory.
        cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
        OIC_LOG_V(DEBUG, TAG, "Ver reallocation size : %zd.", cborLen);
        cborEncoderResult = CborNoError;
        ret = VerToCBORPayload(ver, payload, &cborLen);
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

OCStackResult CBORPayloadToVer(const uint8_t *cborPayload, size_t size,
                                OicSecVer_t **secVer)
{
    if (NULL == cborPayload || NULL == secVer || NULL != *secVer || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    *secVer = NULL;
    char *strUuid = NULL;

    CborParser parser = { .end = NULL};
    CborError cborFindResult = CborNoError;
    size_t len = 0;
    CborValue verCbor = { .parser = NULL };
    cbor_parser_init(cborPayload, size, 0, &parser, &verCbor);
    CborValue verMap = { .parser = NULL };
    OicSecVer_t *ver = (OicSecVer_t *)OICCalloc(1, sizeof(OicSecVer_t));
    VERIFY_NON_NULL(TAG, ver, ERROR);


    cborFindResult = cbor_value_map_find_value(&verCbor, OIC_JSON_SEC_V_NAME, &verMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&verMap))
    {
        char *version = NULL;
        cborFindResult = cbor_value_dup_text_string(&verMap, &version, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Security Version Value.");
        memcpy(ver->secv, version, len);
        OICFree(version);
    }

    cborFindResult = cbor_value_map_find_value(&verCbor, OIC_JSON_DEVICE_ID_NAME, &verMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&verMap))
    {
        cborFindResult = cbor_value_dup_text_string(&verMap, &strUuid , &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Device Id Value.");
        ret = ConvertStrToUuid(strUuid , &ver->deviceID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        OICFree(strUuid );
        strUuid  = NULL;
    }

    *secVer = ver;
    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        OIC_LOG (ERROR, TAG, "CBORPayloadToVer failed!!!");
        DeleteVerBinData(ver);
        ret = OC_STACK_ERROR;
    }
    return ret;
}

static OCEntityHandlerResult HandleVerGetRequest (const OCEntityHandlerRequest * ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_OK;

    OIC_LOG(DEBUG, TAG, "Ver EntityHandle processing GET request");

    /*
     * For GET request return ver resource CBOR payload.
     * For non-valid query return NULL json payload.
     * The version is static built-in information, so VerToCBORPayload will
     * return valid ver resource json.
     */
    uint8_t *payload = NULL;
    size_t size = 0;
    if (OC_STACK_OK != VerToCBORPayload(&gVer, &payload, &size))
    {
        payload = NULL;
    }

    // Send response payload to request originator
    if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, payload, size))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleVerGetRequest");
    }

    OICFree(payload);

    return ehRet;
}

OCEntityHandlerResult VerEntityHandler(OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest * ehRequest,
                                        void* callbackParam)
{
    (void)callbackParam;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    if(NULL == ehRequest)
    {
        return ehRet;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");

        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandleVerGetRequest(ehRequest);
                break;

            default:
                ehRet = OC_EH_ERROR;
                SendSRMResponse(ehRequest, ehRet, NULL, 0);
                break;
        }
    }

    return ehRet;
}

OCStackResult CreateVerResource()
{
    OCStackResult ret = OCCreateResource(&gVerHandle,
                                         OIC_RSRC_TYPE_SEC_VER,
                                         OC_RSRVD_INTERFACE_DEFAULT,
                                         OIC_RSRC_VER_URI,
                                         VerEntityHandler,
                                         NULL,
                                         OC_SECURE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG (FATAL, TAG, "Unable to instantiate Ver resource");
        DeInitVerResource();
    }
    return ret;
}

/**
 * Get the security version.
 *
 * @return the version string of security.
 */
const char* GetSecVersion()
{
    OIC_LOG(DEBUG, TAG, "GetSecVersion");
    return gVer.secv;
}

const OicSecVer_t* GetVerResourceData()
{
    return &gVer;
}

OCStackResult InitVerResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    OICStrcpy(gVer.secv, OIC_SEC_MAX_VER_LEN, SECURITY_VERSION);

    //Read device id from doxm
    OicUuid_t deviceID = {.id={0}};
    ret = GetDoxmDeviceID(&deviceID);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving doxm device ID");
        return ret;
    }
    memcpy(&gVer.deviceID, &deviceID, sizeof(OicUuid_t));

    //Instantiate 'oic.sec.ver'
    ret = CreateVerResource();
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "Error while creating VER resource");
    }

    return ret;
}

OCStackResult DeInitVerResource()
{
    OCStackResult ret = OCDeleteResource(gVerHandle);

    memset(&gVer, 0, sizeof(gVer));

    if (OC_STACK_OK == ret)
    {
        return OC_STACK_OK;
    }
    else
    {
        return OC_STACK_ERROR;
    }
}
