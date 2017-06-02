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

#include "occloudprovisioning.h"
#include "utils.h"

#include "oic_malloc.h"
#include "logger.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "pmutility.h"
#include "cacommonutil.h"

#define TAG "OIC_CLOUD_ACL_INVITE"

/**
 * This helper function parses "name" : { "gid":[], "mid":[] } payload
 *
 * @param[in] payload     received payload
 * @param[in] name        property name
 * @param[out] out        string array pair to fill
 * @return  OCStackResult application result
 */
static OCStackResult parseInvitePayload(const OCRepPayload *payload, const char *name, stringArrayPair_t *out)
{
    OCStackResult result = OC_STACK_NO_MEMORY;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
    OCRepPayload **heplerPayload  = NULL;
    size_t i = 0;

    if (!OCRepPayloadGetPropObjectArray(payload, name, &heplerPayload, dimensions))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", name);
        return OC_STACK_MALFORMED_RESPONSE;
    }

    size_t count = calcDimTotal(dimensions);

    stringArray_t *gidlist = &out->gidlist;
    stringArray_t *midlist = &out->midlist;

    gidlist->length = count;
    midlist->length = count;

    gidlist->array = OICCalloc(gidlist->length, sizeof(char *));
    if (NULL == gidlist->array)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate gidlist->array");
        goto exit;
    }

    midlist->array = OICCalloc(midlist->length, sizeof(char *));
    if (NULL == midlist->array)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate midlist->array");
        goto exit;
    }

    for (i = 0; i < gidlist->length; i++)
    {
        const OCRepPayload *payload = heplerPayload[i];

        if (!OCRepPayloadGetPropString(payload, OC_RSRVD_GROUP_ID, &gidlist->array[i]))
        {
            OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_GROUP_ID);
            result = OC_STACK_MALFORMED_RESPONSE;
            goto exit;
        }

        if (!OCRepPayloadGetPropString(payload, OC_RSRVD_MEMBER_ID, &midlist->array[i]))
        {
            OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_MEMBER_ID);
            result = OC_STACK_MALFORMED_RESPONSE;
            goto exit;
        }
        OCRepPayloadDestroy(heplerPayload[i]);
    }

    result = OC_STACK_OK;

exit:
    if (result != OC_STACK_OK)
    {
        clearStringArray(gidlist);
        clearStringArray(midlist);

        for (size_t k = i; k < gidlist->length; k++)
        {
            OCRepPayloadDestroy(heplerPayload[i]);
        }
    }
    OICFree(heplerPayload);
    return result;
}

/**
 * ACL get invitation request received data handler
 *
 * @param[in] ctx       context
 * @param[out] data     data required to external application
 * @param[in] response  peer response
 * @return  OCStackResult application result
 */
static OCStackResult handleAclGetInvitationResponse(void *ctx, void **data, OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OCStackResult result = OC_STACK_OK;

    if (NULL == response->payload)
    {
        OIC_LOG(ERROR, TAG, "Receive NULL payload");
        return OC_STACK_INVALID_PARAM;
    }

    inviteResponse_t *answer = OICCalloc(1, sizeof(inviteResponse_t));
    if (NULL == answer)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate answer");
        return OC_STACK_NO_MEMORY;
    }

    const OCRepPayload *payload = (const OCRepPayload *)response->payload;

    result = parseInvitePayload(payload, OC_RSRVD_INVITE, &answer->invite);
    if (result != OC_STACK_OK)
    {
        goto exit;
    }

    result = parseInvitePayload(payload, OC_RSRVD_INVITED, &answer->invited);
    if (result != OC_STACK_OK)
    {
        goto exit;
    }

    *data = answer;
exit:
    return result;
}

/**
 * ACL policy check request received data handler
 *
 * @param[in] ctx       context
 * @param[out] data     data required to external application
 * @param[in] response  peer response
 * @return  OCStackResult application result
 */
static OCStackResult handleAclPolicyCheckResponse(void *ctx, void **data, OCClientResponse *response)
{
    OC_UNUSED(ctx);

    if (NULL == response->payload)
    {
        OIC_LOG(ERROR, TAG, "Receive NULL payload");
        return OC_STACK_INVALID_PARAM;
    }

    char *gp = NULL;

    if (!OCRepPayloadGetPropString((const OCRepPayload *)response->payload, OC_RSRVD_GROUP_PERMISSION, &gp))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_GROUP_PERMISSION);
        return OC_STACK_MALFORMED_RESPONSE;
    }

    *data = gp;
    return OC_STACK_OK;
}

OCStackResult OCCloudAclInviteUser(void* ctx,
                                   const char *userId,
                                   const stringArray_t *groupIds,
                                   const stringArray_t *memberIds,
                                   const OCDevAddr *endPoint,
                                   OCCloudResponseCB callback)
{
    OCStackResult result = OC_STACK_ERROR;
    char uri[MAX_URI_LENGTH] = { 0 };
    size_t i = 0;

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupIds, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(memberIds, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    if (groupIds->length != memberIds->length)
    {
        OIC_LOG(ERROR, TAG, "members and groups lists should have the same length!!!");
        return OC_STACK_INVALID_PARAM;
    }

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_INVITE_URL);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    OCRepPayload *payload = OCRepPayloadCreate();
    if (!payload)
    {
        return OC_STACK_NO_MEMORY;
    }

    OCRepPayload **heplerPayload = OICCalloc(groupIds->length, sizeof(OCRepPayload *));

    for (i = 0; i < groupIds->length; i++)
    {
        heplerPayload[i] = OCRepPayloadCreate();
        if (!heplerPayload[i])
        {
            goto no_memory;
        }
        OCRepPayloadSetPropString(heplerPayload[i], OC_RSRVD_GROUP_ID, groupIds->array[i]);
        OCRepPayloadSetPropString(heplerPayload[i], OC_RSRVD_MEMBER_ID, memberIds->array[i]);
    }

    //add next fields if they were filled
    if (userId) OCRepPayloadSetPropString(payload, OC_RSRVD_USER_UUID, userId);

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {groupIds->length, 0, 0};
    OCRepPayloadSetPropObjectArray(payload, OC_RSRVD_INVITE,
            (const struct OCRepPayload **)heplerPayload, dimensions);

    return OCDoResource(NULL, OC_REST_POST, uri, NULL, (OCPayload *)payload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
no_memory:
    OCRepPayloadDestroy(payload);
    for (size_t k = 0; k < i; k++)
    {
        OCRepPayloadDestroy(heplerPayload[k]);
    }
    OCRepPayloadDestroy(*heplerPayload);
    return result;
}

OCStackResult OCCloudAclGetInvitation(void* ctx,
                                      const char *userId,
                                      const OCDevAddr *endPoint,
                                      OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_INVITE_URL);

    if (userId)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, MAX_URI_LENGTH -len, "?%s=%s", OC_RSRVD_USER_UUID, userId);
    }

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, handleAclGetInvitationResponse, NULL);

    return OCDoResource(NULL, OC_REST_GET, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclDeleteInvitation(void* ctx,
                                         const char *userId,
                                         const char *groupId,
                                         const OCDevAddr *endPoint,
                                         OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_INVITE_URL);

    if (userId)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, MAX_URI_LENGTH - len, "?%s=%s", OC_RSRVD_USER_UUID, userId);
    }

    size_t len = strlen(uri);
    snprintf(uri + len, MAX_URI_LENGTH - len, "%c%s=%s", userId?'&':'?', OC_RSRVD_GROUP_ID, groupId);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    return OCDoResource(NULL, OC_REST_DELETE, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclCancelInvitation(void* ctx,
                                         const char *userId,
                                         const char *groupId,
                                         const char *memberId,
                                         const OCDevAddr *endPoint,
                                         OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };
    size_t len = 0 ;

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(groupId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(memberId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_INVITE_URL);

    if (userId)
    {
        size_t len = strlen(uri);
        snprintf(uri + len, MAX_URI_LENGTH - len, "?%s=%s", OC_RSRVD_USER_UUID, userId);
    }

    len = strlen(uri);
    snprintf(uri + len, MAX_URI_LENGTH - len, "%c%s=%s", userId?'&':'?', OC_RSRVD_GROUP_ID, groupId);
    len = strlen(uri);
    snprintf(uri + len, MAX_URI_LENGTH - len, "&%s=%s", OC_RSRVD_MEMBER_ID, memberId);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, NULL, NULL);

    return OCDoResource(NULL, OC_REST_DELETE, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudAclPolicyCheck(void* ctx,
                                    const char *subjectId,
                                    const char *deviceId,
                                    const char *method,
                                    const char *user_uri,
                                    const OCDevAddr *endPoint,
                                    OCCloudResponseCB callback)
{
    char uri[MAX_URI_LENGTH] = { 0 };
    size_t len = 0;

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endpoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(subjectId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(deviceId, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(method, TAG, "NULL input param", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(user_uri, TAG, "NULL input param", OC_STACK_INVALID_PARAM);

    snprintf(uri, MAX_URI_LENGTH, "%s%s:%d%s", DEFAULT_PREFIX,
            endPoint->addr, endPoint->port, OC_RSRVD_ACL_VERIFY_URL);

    len = strlen(uri);
    snprintf(uri + len, MAX_URI_LENGTH - len, "?%s=%s", OC_RSRVD_SUBJECT_ID, subjectId);
    len = strlen(uri);
    snprintf(uri + len, MAX_URI_LENGTH - len, "&%s=%s", OC_RSRVD_DEVICE_ID, deviceId);
    len = strlen(uri);
    snprintf(uri + len, MAX_URI_LENGTH - len, "&%s=%s", OC_RSRVD_REQUEST_METHOD, method);
    len = strlen(uri);
    snprintf(uri + len, MAX_URI_LENGTH - len, "&%s=%s", OC_RSRVD_REQUEST_URI, user_uri);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, handleAclPolicyCheckResponse, NULL);

    return OCDoResource(NULL, OC_REST_GET, uri, NULL, NULL,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}
