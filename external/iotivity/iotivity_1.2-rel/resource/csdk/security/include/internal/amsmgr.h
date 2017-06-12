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

#ifndef IOTVT_SRM_AMSMGR_H
#define IOTVT_SRM_AMSMGR_H

#include <stdlib.h>
#include <stdint.h>

#include "ocstack.h"
#include "logger.h"
#include "policyengine.h"
#include "securevirtualresourcetypes.h"
#include "cainterface.h"

typedef struct PEContext PEContext_t;
/**
 * The AMS context.
 */
typedef struct AmsMgrContext
{
    OicUuid_t           amsDeviceId;  /**< DeviceID of the oic.sec.ams service. */
    CAEndpoint_t        *endpoint;
    CARequestInfo_t     *requestInfo;
} AmsMgrContext_t;

/**
 * This method updates AmsMgr context's endpoint & requestInfo.
 *
 * @param context is the policy engine context.
 * @param endpoint is the CA Endpoint info of the requester.
 * @param requestInfo is the CA RequestInfo of the requester.
 *
 * @return ::OC_STACK_OK if successful, else other value in case of error.
 */
OCStackResult UpdateAmsMgrContext(PEContext_t *context,
                                  const CAEndpoint_t *endpoint,
                                  const CARequestInfo_t *requestInfo);

/**
 * This method is called by PolicyEngine to Discover AMS service.
 * It sends muticast discovery request such as
 * /oic/sec/doxm?deviceid="AMSSrvcDeviceID" to discover AMS service
 * with deviceId="AMSSrvcDeviceID".
 *
 * @param context is the policy engine context.
 *
 * @return ::OC_STACK_OK,If able to successfully send multicast discovery request.
 * else ::OC_STACK_ERROR, If unable to successfully send multicast discovery request
 * due to error.
 */
OCStackResult DiscoverAmsService(PEContext_t *context);

/**
 * This method sends unicast request to retrieve the secured port info of the
 * discovered AMS service. It sends unicast discovery request such as
 * /oic/res?rt="oic.sec.doxm" to the discovered AMS service.
 *
 * @param context is the policy engine context.
 *
 * @return ::OC_STACK_OK,If able to successfully send unicast discovery request.
 * else ::OC_STACK_ERROR, If unable to successfully send unicast discovery request
 * due to error.
 */
OCStackResult SendUnicastSecurePortDiscovery(PEContext_t *context,
                                             OCDevAddr *devAddr,
                                             OCConnectivityType connType);

/**
 * This method sends unicast request to AMS service to get ACL for
 * the Subject and/or Resource. It sends unicast request such as
 * /oic/sec/acl?sub="subjectId";rsrc="/a/led" to get the ACL for
 * the subject & resource.
 *
 * @param context is the policy engine context.
 *
 * @return ::OC_STACK_OK, If able to successfully send unicast ACL request.
 * ::OC_STACK_ERROR, If unable to successfully send unicast ACL request due to error.
 *
 */
OCStackResult SendAclReq(PEContext_t *context,
                         OCDevAddr *devAddr,
                         OCConnectivityType connType,
                         uint16_t securedPort);

/*
 * This method is used by Policy engine to checks Amacl resource.
 * If Amacl is found then it fills up context->amsMgrContext->amsDeviceId
 * with amsID of the Amacl else leaves it empty.
 *
 * @param context is the policy engine context.
 *
 * @return true, if Amacl for the resource is found. false, if Amacl for the
 * resource is not found
 */
bool FoundAmaclForRequest(PEContext_t *context);

/*
 * This method is used by Policy engine to process AMS request.
 *
 * @param context is the policy engine context.
 */
void ProcessAMSRequest(PEContext_t *context);

/*
 * This method is used by Policy engine to free AMS context requestInfo/
 *
 * @param requestInfo is the pointer to @ref CARequestInfo_t.
 */
void FreeCARequestInfo(CARequestInfo_t *requestInfo);

#endif //IOTVT_SRM_AMSMGR_H
