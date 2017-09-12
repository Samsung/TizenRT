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
#include "aclresource.h"
#include "pstatresource.h"
#include "doxmresource.h"
#include "credresource.h"
#include "amaclresource.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "utlist.h"
#include "psinterface.h"

//#ifdef DIRECT_PAIRING
#include "pconfresource.h"
#include "dpairingresource.h"
//#endif // DIRECT_PAIRING

#define TAG "OIC_SRM_RM"

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "crlresource.h"
#include "csrresource.h"
#include "rolesresource.h"
#endif // __WITH_DTLS__ || __WITH_TLS__

OCStackResult SendSRMResponse(const OCEntityHandlerRequest *ehRequest,
        OCEntityHandlerResult ehRet, uint8_t *cborPayload, size_t size)
{
    OIC_LOG(DEBUG, TAG, "SRM sending SRM response");
    OCEntityHandlerResponse response = {.requestHandle = NULL};
    OCStackResult ret = OC_STACK_ERROR;

    if (ehRequest)
    {
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

    /*
     * doxm resource should be initialized first as it contains the DeviceID
     * which MAY be used during initialization of other resources.
     */

    ret = InitDoxmResource();

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
    if(OC_STACK_OK == ret)
    {
        ret = InitCSRResource();
    }
    if(OC_STACK_OK == ret)
    {
        ret = InitRolesResource();
    }
#endif // __WITH_DTLS__ || __WITH_TLS__
#ifndef AMACL_RESOURCE_IMPLEMENTATION_COMPLETE
    OIC_LOG_V(WARNING, TAG, "%s: /amacl Resource implementation incomplete; not initializing.", __func__);
#endif // AMACL_RESOURCE_IMPLEMENTATION_COMPLETE
#ifdef AMACL_RESOURCE_IMPLEMENTATION_COMPLETE
    if(OC_STACK_OK == ret)
    {
        ret = InitAmaclResource();
    }
#endif // AMACL_RESOURCE_IMPLEMENTATION_COMPLETE
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
    if(OC_STACK_OK != ret)
    {
        //TODO: Update the default behavior if one of the SVR fails
        DestroySecureResources();
    }
    return ret;
}

OCStackResult DestroySecureResources( )
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    DeInitACLResource();
    DeInitCredResource();
    DeInitDoxmResource();
    DeInitPstatResource();
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    DeInitCRLResource();
    DeInitCSRResource();
    DeInitRolesResource();
#endif // __WITH_DTLS__ || __WITH_TLS__
    DeInitAmaclResource();
//#ifdef DIRECT_PAIRING
    DeInitPconfResource();
    DeInitDpairingResource();
//#endif // DIRECT_PAIRING

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return OC_STACK_OK;
}

OCStackResult ResetSecureResources()
{
    OCStackResult ret = OC_STACK_ERROR;

    ret = DestroySecureResources();

    if (OC_STACK_OK == ret)
    {
        ret = InitSecureResources();
    }

    if (OC_STACK_OK == ret)
    {
        ret = ResetSecureResourceInPS();
    }

    if (OC_STACK_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "%s: resetting device to mfr defaults failed!",
            __func__);
        // TODO: vendor may wish to fall back to hard-coded defaults
        // if the persistent storage backup values cannot be restored.
        // However in this case, vendor should ensure hard-coded
        // defaults match intended settings, especially the supported
        // OTMs default in /doxm resource.  IoTivity for example disables
        // JustWorks OTM in hard-coded defaults, which may not be suitable
        // to some devices.
    }

    return ret;
}
