/* *****************************************************************
*
* Copyright 2017 Microsoft. All Rights Reserved.
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

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)

#include "iotivity_config.h"

#include "logger.h"
#include <stddef.h>
#include <string.h>
#include "oic_malloc.h"
#include "cacommon.h"

#include "ocsecurity.h"

#include "rolesresource.h"
#include "credresource.h"
#include "srmresourcestrings.h"
#include "ocstackinternal.h"
#include "pmutility.h"
#include "srmutility.h"
#include "ocpayload.h"

#define TAG "OIC_SECURITY"

typedef struct AssertRolesData AssertRolesData_t;
struct AssertRolesData {
    void *ctx;                                  /**< User-provided context **/
    OCAssertRolesCB resultCallback;             /**< Pointer to result callback **/
};

static OCConnectivityType endpointToConnType(const CAEndpoint_t* endpoint)
{
    /*
     * OCConnectivityType is CATransportAdapter_t | CATransportFlags_t (as 16-bit ints)
     * and CATransportFlags_t has some scope bits in the low byte that must be cleared.
     */
    return (endpoint->adapter << CT_ADAPTER_SHIFT) | ((uint16_t)endpoint->flags & (~CA_SCOPE_MASK));
}

static bool GenerateQuery(bool isSecure,
    const char* address, uint16_t port,
    OCConnectivityType connType,
    char* buffer, size_t bufferSize, const char* uriPath)
{
    if (!address || !buffer || !uriPath)
    {
        OIC_LOG(ERROR, TAG, "GenerateQuery : Invalid parameters.");
        return false;
    }

    int snRet = 0;
    const char* prefix = (isSecure == true) ? COAPS_PREFIX : COAP_PREFIX;

    switch (connType & CT_MASK_ADAPTER)
    {
    case CT_ADAPTER_TCP:
        prefix = (isSecure == true) ? COAPS_TCP_PREFIX : COAP_TCP_PREFIX;
        /* Fall-through to the IP adapter case instead of break here */
    case CT_ADAPTER_IP:
        switch (connType & CT_MASK_FLAGS & ~CT_FLAG_SECURE)
        {
        case CT_IP_USE_V4:
            snRet = snprintf(buffer, bufferSize, "%s%s:%d%s",
                prefix, address, port, uriPath);
            break;
        case CT_IP_USE_V6:
        {
            char addressEncoded[128] = { 0 };

            OCStackResult result = OCEncodeAddressForRFC6874(addressEncoded,
                sizeof(addressEncoded),
                address);
            if (OC_STACK_OK != result)
            {
                OIC_LOG_V(ERROR, TAG, "GenerateQuery : encoding error %d\n", result);
                return false;
            }

            snRet = snprintf(buffer, bufferSize, "%s[%s]:%d%s",
                prefix, addressEncoded, port, uriPath);
            break;
        }
        default:
            OIC_LOG_V(ERROR, TAG, "Unknown address format. (conntype = %d)", connType);
            return false;
        }
        break;
    case CT_ADAPTER_GATT_BTLE:
        snRet = snprintf(buffer, bufferSize, "%s%s%s",
            prefix, address, uriPath);
        break;
    case CT_ADAPTER_RFCOMM_BTEDR:
        OIC_LOG(ERROR, TAG, "Not supported connectivity adapter.");
        return false;
    default:
        OIC_LOG_V(ERROR, TAG, "Unknown connectivity adapter %d, (conntype = %d).", (connType & CT_MASK_ADAPTER), connType);
        return false;
    }

    // snprintf return value check
    if (snRet < 0)
    {
        OIC_LOG_V(ERROR, TAG, "GenerateQuery : Error (snprintf) %d\n", snRet);
        return false;
    }
    else if ((size_t)snRet >= bufferSize)
    {
        OIC_LOG_V(ERROR, TAG, "GenerateQuery : Truncated (snprintf) %d\n", snRet);
        return false;
    }

    return true;
}

/**
* Callback handler of OCAssertRoles.
*
* @param[in] ctx             ctx value passed to callback from calling function.
* @param[in] UNUSED          handle to an invocation
* @param[in] clientResponse  Response from queries to remote servers.
* @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
*          and  OC_STACK_KEEP_TRANSACTION to keep it.
*/
static OCStackApplicationResult OCAssertRolesCBHandler(void *ctx, OCDoHandle UNUSED,
    OCClientResponse *clientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    OC_UNUSED(UNUSED);
    VERIFY_NOT_NULL_RETURN(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    AssertRolesData_t *assertRolesData = (AssertRolesData_t*)ctx;
    OCAssertRolesCB resultCallback = assertRolesData->resultCallback;
    bool hasError = true;

    if (clientResponse && (OC_STACK_RESOURCE_CHANGED == clientResponse->result))
    {
        hasError = false;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s : NULL client response, or response indicating failure (%d)", __func__, clientResponse->result);
    }

    ((OCAssertRolesCB)(resultCallback))(assertRolesData->ctx, hasError);

    OICFree(assertRolesData);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult OC_CALL OCAssertRoles(void *ctx, const OCDevAddr *devAddr, OCAssertRolesCB resultCallback)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    VERIFY_NOT_NULL_RETURN(TAG, devAddr, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, resultCallback, ERROR, OC_STACK_INVALID_CALLBACK);

    CAEndpoint_t endpoint;
    CopyDevAddrToEndpoint(devAddr, &endpoint);
    OIC_LOG_V(DEBUG, TAG, "%s: Endpoint info: ", __func__);
    OIC_LOG_V(DEBUG, TAG, "%s:     adapter: %d", __func__, endpoint.adapter);
    OIC_LOG_V(DEBUG, TAG, "%s:     flags: %d", __func__, endpoint.flags);
    OIC_LOG_V(DEBUG, TAG, "%s:     port: %d", __func__, endpoint.port);
    OIC_LOG_V(DEBUG, TAG, "%s:     addr: %s", __func__, endpoint.addr);
    OIC_LOG_V(DEBUG, TAG, "%s:     index: %d", __func__, endpoint.ifindex);
    OIC_LOG_V(DEBUG, TAG, "%s:     remoteid: %s", __func__, endpoint.remoteId);

    /* 1. Retrieve all our stored role certificates */
    RoleCertChain_t *roles = NULL;
    OCStackResult res = GetAllRoleCerts(&roles);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Failed to retrieve role certificates");
        return OC_STACK_ERROR;
    }
    if (roles == NULL)
    {
        OIC_LOG(ERROR, TAG, "Can't assert roles, no role certificates installed");
        return OC_STACK_INCONSISTENT_DB;
    }

    /* 2. Encode them for transfer */
    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if (!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory");
        FreeRoleCertChainList(roles);
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    if (OC_STACK_OK != RolesToCBORPayload(roles, &secPayload->securityData, &secPayload->payloadSize))
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "RolesToCBORPayload failed");
        FreeRoleCertChainList(roles);
        return OC_STACK_NO_MEMORY;
    }
    OIC_LOG(DEBUG, TAG, "Created payload for asserting roles:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);
    FreeRoleCertChainList(roles);

    /* 3. Post roles to the roles resource on the server */
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = { 0 };
    OCConnectivityType connType = endpointToConnType(&endpoint);
    if (!GenerateQuery(true,
        endpoint.addr,
        endpoint.port,
        connType,
        query, sizeof(query), OIC_RSRC_ROLES_URI))
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG_V(ERROR, TAG, "%s: Failed to generate query", __func__);
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData = { .context = NULL,.cb = NULL,.cd = NULL };
    cbData.cb = &OCAssertRolesCBHandler;
    AssertRolesData_t *assertRolesData = (AssertRolesData_t *)OICCalloc(1, sizeof(AssertRolesData_t));
    if (assertRolesData == NULL)
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    assertRolesData->resultCallback = resultCallback;
    assertRolesData->ctx = ctx;

    cbData.context = (void *)assertRolesData;
    cbData.cd = NULL;
    OCMethod method = OC_REST_POST;
    OCDoHandle handle = NULL;

    OIC_LOG(DEBUG, TAG, "Sending roles to server");
    OCStackResult ret = OCDoRequest(&handle, method, query,
        devAddr, (OCPayload*)secPayload,
        endpoint.adapter, OC_HIGH_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OICFree(assertRolesData);
        OIC_LOG_V(ERROR, TAG, "%s OCDoRequest failed", __func__);
    }
    OCPayloadDestroy((OCPayload *)secPayload);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    VERIFY_SUCCESS_RETURN(TAG, (OC_STACK_OK == ret), ERROR, OC_STACK_ERROR);

    return ret;
}

#endif /* defined(__WITH_TLS__) || defined(__WITH_DTLS__) */