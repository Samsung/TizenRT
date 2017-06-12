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

#include "octypes.h"
#include "logger.h"
#include "payload_logging.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "psinterface.h"
#include "securevirtualresourcetypes.h"
#include "doxmresource.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "pmutility.h"
#include "credresource.h"
#include "payload_logging.h"
#include "cacommonutil.h"

#include "utils.h"
#include "cloudAuth.h"
#include "cloudCommon.h"

#define TAG "cloudAuth"

#define LOGIN_OK 4

#define MAX_URI_QUERY MAX_URI_LENGTH + MAX_QUERY_LENGTH

typedef struct
{
    char *accesstoken;
    char *refreshtoken;
    char *tokentype;
    long  expiresin;
    char *uid;
    char *redirecturi;
    char *certificate;
    char *sid;
} sessionObject_t;

static sessionObject_t sessionObject = {NULL, NULL, NULL, 0, NULL, NULL, NULL, NULL};

/**
 * Session free function
 *
 * @return  OCStackResult application result
 */
static void SessionFree()
{
    OICFree(sessionObject.accesstoken);
    OICFree(sessionObject.refreshtoken);
    OICFree(sessionObject.tokentype);
    OICFree(sessionObject.uid);
    OICFree(sessionObject.redirecturi);
    OICFree(sessionObject.certificate);
    OICFree(sessionObject.sid);

    memset(&sessionObject, 0, sizeof(sessionObject_t));
}

/**
 * Session parse payload
 *
 * @param[in] payload
 * @param[in] sessionObject session data
 * @return  OCStackResult application result
 */
static OCStackResult SessionParsePayload(OCRepPayload *payload)
{
    VERIFY_NON_NULL_RET(payload, TAG, "NULL payload", OC_STACK_ERROR);

    SessionFree();

    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_ACCESS_TOKEN,
                                   &sessionObject.accesstoken))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_ACCESS_TOKEN);
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_REFRESH_TOKEN,
                                   &sessionObject.refreshtoken))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_REFRESH_TOKEN);
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_TOKEN_TYPE,
                                   &sessionObject.tokentype))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_TOKEN_TYPE);
    }
    int64_t tmp = 0;
    if (!OCRepPayloadGetPropInt(payload, OC_RSRVD_EXPIRES_IN, &tmp))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_EXPIRES_IN);
    }
    else
    {
        sessionObject.expiresin = tmp;
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_USER_UUID,
                                   &sessionObject.uid))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_USER_UUID);
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_REDIRECT_URI,
                                   &sessionObject.redirecturi))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_REDIRECT_URI);
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_CERTIFICATE,
                                   &sessionObject.certificate))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_CERTIFICATE);
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_SUBJECT_ID,
                                   &sessionObject.sid))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_SUBJECT_ID);
    }

    return OC_STACK_OK;
}

/**
 * Sends Sign Up request to cloud
 *
 * @param[in] ctx                    context
 * @param[in] handle                 handle
 * @param[in] response               response from peer
 * @return  OCStackApplicationResult application result
 */
static OCStackApplicationResult handleCloudSignUpResponse(void *ctx,
                                                          OCDoHandle handle,
                                                          OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    VERIFY_NON_NULL_RET(response, TAG, "Received NULL response", OC_STACK_DELETE_TRANSACTION);

    if (response->payload)
    {
        OIC_LOG(INFO, TAG, "Payload received");
        OIC_LOG_PAYLOAD(DEBUG, response->payload);
    }

    if (response->result != LOGIN_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Login error: %d",response->result);
    }
    else
    {
        SessionParsePayload((OCRepPayload*)response->payload);
        OIC_LOG(INFO, TAG, "Sign Up OK");
    }

    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult CloudSignUp(const OCDevAddr *endPoint,
                          const char *authProvider,
                          const char *authToken)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endPoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(authProvider, TAG, "NULL endPoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(authToken, TAG, "NULL endPoint", OC_STACK_INVALID_PARAM);

    char *deviceId = getDeviceId();
    VERIFY_NON_NULL_RET(deviceId, TAG, "Can't get the device id", OC_STACK_ERROR);

    snprintf(uri, MAX_URI_LENGTH, DEFAULT_QUERY,
             endPoint->addr, endPoint->port, OC_RSRVD_ACCOUNT_URI);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = handleCloudSignUpResponse;
    cbData.cd = unlockMenu;

    OCRepPayload *payload = OCRepPayloadCreate();
    VERIFY_NON_NULL_RET(payload, TAG, "Failed to allocate payload", OC_STACK_NO_MEMORY);

    OCRepPayloadSetPropString(payload, OC_RSRVD_DEVICE_ID, deviceId);
    OCRepPayloadSetPropString(payload, OC_RSRVD_AUTHPROVIDER, authProvider);
    OCRepPayloadSetPropString(payload, OC_RSRVD_AUTHCODE, authToken);

    OICFree(deviceId);

    return OCDoResource(NULL, OC_REST_POST, uri, NULL, (OCPayload *)payload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

/**
 * Sends Sign In request to cloud
 *
 * @param[in] ctx                    context
 * @param[in] handle                 handle
 * @param[in] response               response from peer
 * @return  OCStackApplicationResult application result
 */
static OCStackApplicationResult handleCloudSignInResponse(void *ctx,
                                                          OCDoHandle handle,
                                                          OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    VERIFY_NON_NULL_RET(response, TAG, "Received NULL response", OC_STACK_DELETE_TRANSACTION);

    if (response->payload)
    {
        OIC_LOG(INFO, TAG, "Payload received");
        OIC_LOG_PAYLOAD(DEBUG, response->payload);
    }

    if (response->result != LOGIN_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Sign In error: result: %d", response->result);
        return OC_STACK_DELETE_TRANSACTION;
    }

    sessionObject.expiresin = 0;
    int64_t tmp = 0;
    if (!OCRepPayloadGetPropInt((OCRepPayload*)response->payload, OC_RSRVD_EXPIRES_IN, &tmp))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_EXPIRES_IN);
    }
    else
    {
        sessionObject.expiresin = tmp;
    }

    OIC_LOG(INFO, TAG, "Sign In OK");

    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handleCloudSignOutResponse(void *ctx,
                                                           OCDoHandle handle,
                                                           OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    VERIFY_NON_NULL_RET(response, TAG, "Received NULL response", OC_STACK_DELETE_TRANSACTION);

    if (response->payload)
    {
        OIC_LOG(INFO, TAG, "Payload received");
        OIC_LOG_PAYLOAD(DEBUG, response->payload);
    }

    if (response->result != LOGIN_OK)
    {
        OIC_LOG(ERROR, TAG, "Sign Out error");
        return OC_STACK_DELETE_TRANSACTION;
    }

    OIC_LOG(INFO, TAG, "Sign Out OK");

    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Sends Sign In/Out request to cloud
 *
 * @param[in] endPoint               peer endPoint
 * @param[in] signIn                 is it Sign In or Sign Out request
 * @return  OCStackApplicationResult application result
 */
static OCStackResult CloudSign(const OCDevAddr *endPoint, bool signIn)
{
    VERIFY_NON_NULL_RET(endPoint, TAG, "NULL endPoint", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(sessionObject.uid, TAG,
                        "UID is missing. Please run Sign Up first", OC_STACK_ERROR);
    VERIFY_NON_NULL_RET(sessionObject.accesstoken, TAG,
                        "accesstoken is missing. Please run Sign Up first", OC_STACK_ERROR);

    char *deviceId = getDeviceId();
    VERIFY_NON_NULL_RET(deviceId, TAG, "Can't get the device id", OC_STACK_ERROR);

    OCRepPayload* payload = OCRepPayloadCreate();
    VERIFY_NON_NULL_RET(payload, TAG, "Failed to allocate payload", OC_STACK_NO_MEMORY);

    OCRepPayloadSetPropString(payload, OC_RSRVD_USER_UUID, sessionObject.uid);
    OCRepPayloadSetPropString(payload, OC_RSRVD_DEVICE_ID, deviceId);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ACCESS_TOKEN, sessionObject.accesstoken);
    OCRepPayloadSetPropBool(payload, OC_RSRVD_LOGIN, signIn);

    OICFree(deviceId);

    char uri[MAX_URI_QUERY] = { 0 };
    snprintf(uri, MAX_URI_QUERY, DEFAULT_QUERY,
             endPoint->addr, endPoint->port,
             OC_RSRVD_ACCOUNT_SESSION_URI);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = signIn? handleCloudSignInResponse : handleCloudSignOutResponse;
    cbData.cd = unlockMenu;

    return OCDoResource(NULL, OC_REST_POST, uri, NULL, (OCPayload *)payload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult CloudSignIn(const OCDevAddr *endPoint)
{
    return CloudSign(endPoint, true);
}

OCStackResult CloudSignOut(const OCDevAddr *endPoint)
{
    return CloudSign(endPoint, false);
}

