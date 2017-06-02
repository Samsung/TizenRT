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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include "oic_malloc.h"
#include "logger.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "pmutility.h"
#include "cacommonutil.h"

#define TAG "OIC_CLOUD_ACL_GROUP"

/**
 * ACL create group request received data handler
 *
 * @param[in] ctx       context
 * @param[out] data     data required to external application
 * @param[in] response  peer response
 * @return  OCStackResult application result
 */
static OCStackResult handleAclCreateGroupResponse(void *ctx, void **data, OCClientResponse *response)
{
    OC_UNUSED(ctx);
    if (NULL == response->payload)
    {
        OIC_LOG(ERROR, TAG, "Receive NULL payload");
        return OC_STACK_INVALID_PARAM;
    }

    char *gid = NULL;

    if (!OCRepPayloadGetPropString((const OCRepPayload *)response->payload, OC_RSRVD_GROUP_ID, &gid))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_GROUP_ID);
        return OC_STACK_MALFORMED_RESPONSE;
    }

    *data = gid;
    return OC_STACK_OK;
}

/**
 * ACL find my group received data handler
 *
 * @param[in] ctx       context
 * @param[out] data     data required to external application
 * @param[in] response  peer response
 * @return  OCStackResult application result
 */
static OCStackResult handleAclFindMyGroupResponse(void *ctx, void **data, OCClientResponse *response)
{
    OC_UNUSED(ctx);
    if (NULL == response->payload)
    {
        OIC_LOG(ERROR, TAG, "Receive NULL payload");
        return OC_STACK_INVALID_PARAM;
    }

    const OCRepPayload *payload = (const OCRepPayload *)response->payload;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };

    stringArray_t *gidlist = OICCalloc(1, sizeof(stringArray_t));
    if (NULL == gidlist)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate gidlist");
        return OC_STACK_NO_MEMORY;
    }

    if (!OCRepPayloadGetStringArray(payload, OC_RSRVD_GROUP_ID_LIST, &gidlist->array, dimensions))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_GROUP_ID_LIST);
        OICFree(gidlist);
        return OC_STACK_MALFORMED_RESPONSE;
    }

    gidlist->length = calcDimTotal(dimensions);

    *data = gidlist;
    return OC_STACK_OK;
}

OCStackResult OCCloudAclCreateGroup(void* ctx,
                                    const char *groupType,
                                    const char *groupMasterId,
                                    const OCDevAddr *endPoint,
                                    OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupType, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, handleAclCreateGroupResponse, NULL);

    OCRepPayload *payload = OCRepPayloadCreate();
    if (!payload)
    {
        return OC_STACK_NO_MEMORY;
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_GROUP_TYPE, groupType);

    //add next fields if they were filled
    if (groupMasterId) OCRepPayloadSetPropString(payload, OC_RSRVD_GROUP_MASTER_ID, groupMasterId);

    return OCDoResource(NULL, OC_REST_POST, uri, NULL, (OCPayload *)payload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclFindMyGroup(void* ctx,
                                    const char *memberId,
                                    const OCDevAddr *endPoint,
                                    OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL);

    if (memberId)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, MAX_URI_LENGTH - len, "?%s=%s", OC_RSRVD_MEMBER_ID, memberId);
    }

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, handleAclFindMyGroupResponse, NULL);

    return OCDoResource(NULL, OC_REST_GET, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclDeleteGroup(void* ctx,
                                    const char *groupId,
                                    const char *groupMasterId,
                                    const OCDevAddr *endPoint,
                                    OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s?%s=%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL, OC_RSRVD_GROUP_ID, groupId);

    if (groupMasterId)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, MAX_URI_LENGTH - len, "&%s=%s", OC_RSRVD_GROUP_MASTER_ID, groupMasterId);
    }

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    return OCDoResource(NULL, OC_REST_DELETE, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclJoinToInvitedGroup(void* ctx,
                                           const char *groupId,
                                           const OCDevAddr *endPoint,
                                           OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s/%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL, groupId);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    return OCDoResource(NULL, OC_REST_POST, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclObserveGroup(void* ctx,
                                     const char *groupId,
                                     const OCDevAddr *endPoint,
                                     OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s/%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL, groupId);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    return OCDoResource(NULL, OC_REST_OBSERVE, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclShareDeviceIntoGroup(void* ctx,
                                             const char *groupId,
                                             const stringArray_t *memberIds,
                                             const stringArray_t *deviceIds,
                                             const OCDevAddr *endPoint,
                                             OCCloudResponseCB callback)
{
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(memberIds, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(deviceIds, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s/%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL, groupId);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    OCRepPayload *payload = OCRepPayloadCreate();
    if (!payload)
    {
        return OC_STACK_NO_MEMORY;
    }

    dimensions[0] = memberIds->length;
    OCRepPayloadSetStringArray(payload, OC_RSRVD_MEMBER_ID_LIST,
                               (const char **)memberIds->array, dimensions);
    dimensions[0] = deviceIds->length;
    OCRepPayloadSetStringArray(payload, OC_RSRVD_DEVICE_ID_LIST,
                               (const char **)deviceIds->array, dimensions);

    return OCDoResource(NULL, OC_REST_POST, uri, NULL, (OCPayload *)payload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclDeleteDeviceFromGroup(void* ctx,
                                              const char *groupId,
                                              const stringArray_t *memberIds,
                                              const stringArray_t *deviceIds,
                                              const OCDevAddr *endPoint,
                                              OCCloudResponseCB callback)

{
    char uri[MAX_URI_LENGTH * 4] = { 0 };
    int max_size = sizeof(uri);

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(memberIds, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(deviceIds, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, max_size, "%s%s:%d%s/%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL, groupId);

    for (size_t i = 0; i < memberIds->length; i++)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, max_size - len, "%c%s=%s", (0 == i)?'?':'&',
                OC_RSRVD_MEMBER_ID_LIST, memberIds->array[i]);
    }

    for (size_t i = 0; i < deviceIds->length; i++)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, max_size - len, "%c%s=%s",
                (0 == i && 0 == memberIds->length)?'?':'&',
                OC_RSRVD_DEVICE_ID_LIST, deviceIds->array[i]);
    }

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    return OCDoResource(NULL, OC_REST_DELETE, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclGroupGetInfo(void* ctx,
                                     const char *groupId,
                                     const char *memberId,
                                     const OCDevAddr *endPoint,
                                     OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s/%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_GROUP_URL, groupId);

    if (memberId)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, MAX_URI_LENGTH - len, "?%s=%s", OC_RSRVD_MEMBER_ID, memberId);
    }

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    return OCDoResource(NULL, OC_REST_GET, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

