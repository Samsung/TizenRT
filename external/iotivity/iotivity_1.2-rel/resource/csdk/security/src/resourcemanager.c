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
#include "resourcemanager.h"
#include "securevirtualresourcetypes.h"
#include "aclresource.h"
#include "pstatresource.h"
#include "doxmresource.h"
#include "credresource.h"
#include "svcresource.h"
#include "amaclresource.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "utlist.h"

//#ifdef DIRECT_PAIRING
#include "pconfresource.h"
#include "dpairingresource.h"
//#endif // DIRECT_PAIRING
#include "verresource.h"
#include "psinterface.h"

#define TAG "OIC_SRM_RM"

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "crlresource.h"
#endif // __WITH_DTLS__ || __WITH_TLS__

OCStackResult SendSRMResponse(const OCEntityHandlerRequest *ehRequest,
        OCEntityHandlerResult ehRet, uint8_t *cborPayload, size_t size)
{
    OIC_LOG(DEBUG, TAG, "SRM sending SRM response");
    OCStackResult ret = OC_STACK_ERROR;

    if (ehRequest)
    {
        OCEntityHandlerResponse response = {.requestHandle = 0};
        OCSecurityPayload ocPayload = {.base = {.type = PAYLOAD_TYPE_INVALID}};

        response.requestHandle = ehRequest->requestHandle;
        response.resourceHandle = ehRequest->resource;
        response.ehResult = ehRet;
        response.payload = (OCPayload *)(&ocPayload);
        response.payload->type = PAYLOAD_TYPE_SECURITY;
        ((OCSecurityPayload *)response.payload)->securityData = cborPayload;
        ((OCSecurityPayload *)response.payload)->payloadSize = size;
        response.persistentBufferFlag = 0;

        ret = OCDoResponse(&response);
    }
    return ret;
}

OCStackResult InitSecureResources( )
{
    OCStackResult ret;

    ret = InitPersistentStorageInterface();

    /*
     * doxm resource should be initialized first as it contains the DeviceID
     * which MAY be used during initialization of other resources.
     */

    if(OC_STACK_OK == ret)
    {
        ret = InitDoxmResource();
    }
    if(OC_STACK_OK == ret)
    {
        ret = InitPstatResource();
    }
    if(OC_STACK_OK == ret)
    {
        ret = InitACLResource();
    }
    if(OC_STACK_OK == ret)
    {
        ret = InitCredResource();
    }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    if(OC_STACK_OK == ret)
    {
        ret = InitCRLResource();
    }
#endif // __WITH_DTLS__ || __WITH_TLS__
    if(OC_STACK_OK == ret)
    {
        ret = InitSVCResource();
    }
    if(OC_STACK_OK == ret)
    {
        ret = InitAmaclResource();
    }
//#ifdef DIRECT_PAIRING
    if(OC_STACK_OK == ret)
    {
        ret = InitPconfResource();
    }
    if(OC_STACK_OK == ret)
    {
        ret = InitDpairingResource();
    }
//#endif // DIRECT_PAIRING
    if(OC_STACK_OK == ret)
    {
        ret = InitVerResource();
    }
    if(OC_STACK_OK != ret)
    {
        //TODO: Update the default behavior if one of the SVR fails
        DestroySecureResources();
    }
    return ret;
}

OCStackResult DestroySecureResources( )
{
    DeInitACLResource();
    DeInitCredResource();
    DeInitDoxmResource();
    DeInitPstatResource();
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    DeInitCRLResource();
#endif // __WITH_DTLS__ || __WITH_TLS__
    DeInitSVCResource();
    DeInitAmaclResource();
//#ifdef DIRECT_PAIRING
    DeInitPconfResource();
    DeInitDpairingResource();
//#endif // DIRECT_PAIRING
    DeInitVerResource();
    DeinitPersistentStorageInterface();

    return OC_STACK_OK;
}
