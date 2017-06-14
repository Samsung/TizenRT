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
//#include "psinterface.h"
#include "securevirtualresourcetypes.h"
#include "doxmresource.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "srmutility.h"
#include "pmutility.h"
#include "credresource.h"
#include "payload_logging.h"

#include "utils.h"
#include "cloudAuth.h"

#define TAG "cloudAuth"

#define LOGIN_OK 4

#define MAX_URI_QUERY MAX_URI_LENGTH + MAX_QUERY_LENGTH

typedef struct
{
    char *accesstoken;
    char *refreshtoken;
    char *tokentype;
    int64_t expiresin;
    char *uid;
    char *redirecturi;
    char *certificate;
    char *sid;
} sessionObject_t;

static sessionObject_t sessionObject = {0,0,0,0,0,0,0,0};

extern void handleCB(void* ctx, OCStackResult result, void* data);

/**
 * Session free function
 *
 * @return  OCStackResult application result
 */
void SessionFree()
{
    if (sessionObject.accesstoken)  OICFree(sessionObject.accesstoken);
    if (sessionObject.refreshtoken) OICFree(sessionObject.refreshtoken);
    if (sessionObject.tokentype)    OICFree(sessionObject.tokentype);
    if (sessionObject.uid)          OICFree(sessionObject.uid);
    if (sessionObject.redirecturi)  OICFree(sessionObject.redirecturi);
    if (sessionObject.certificate)  OICFree(sessionObject.certificate);
    if (sessionObject.sid)          OICFree(sessionObject.sid);

    memset(&sessionObject, 0, sizeof(sessionObject_t));
}

/**
 * Session init function
 *
 * @param[in] sessionObject session data
 * @return  OCStackResult application result
 */
OCStackResult SessionInit()
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    SessionFree();

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);

    return OC_STACK_OK;
}

/**
 * Session parse payload
 *
 * @param[in] payload
 * @param[in] sessionObject session data
 * @return  OCStackResult application result
 */
OCStackResult SessionParsePayload(OCRepPayload *payload)
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    VERIFY_NON_NULL(TAG, payload, ERROR);

    SessionInit();

    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_ACCESS_TOKEN,
                                   &sessionObject.accesstoken))
    {
        OIC_LOG(ERROR, TAG, "Can't get: accesstoken");
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_REFRESH_TOKEN,
                                   &sessionObject.refreshtoken))
    {
        OIC_LOG(ERROR, TAG, "Can't get: refreshtoken");
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_TOKEN_TYPE,
                                   &sessionObject.tokentype))
    {
        OIC_LOG(ERROR, TAG, "Can't get: tokentype");
    }
    if (!OCRepPayloadGetPropInt(payload, OC_RSRVD_EXPIRES_IN,
                                &(sessionObject.expiresin)))
    {
        OIC_LOG(ERROR, TAG, "Can't get: expiresin");
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_USER_UUID,
                                   &sessionObject.uid))
    {
        OIC_LOG(ERROR, TAG, "Can't get: uid");
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_REDIRECT_URI,
                                   &sessionObject.redirecturi))
    {
        OIC_LOG(ERROR, TAG, "Can't get: redirecturi");
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_CERTIFICATE,
                                   &sessionObject.certificate))
    {
        OIC_LOG(ERROR, TAG, "Can't get: certificate");
    }
    if (!OCRepPayloadGetPropString(payload, OC_RSRVD_SUBJECT_ID,
                                   &sessionObject.sid))
    {
        OIC_LOG(ERROR, TAG, "Can't get: sid");
    }

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);
exit:
    return OC_STACK_OK;
}

/**
 * Cloud login handler
 *
 * @param[in] response  login response
 */
OCStackApplicationResult handleCloudSignUpResponse(void *ctx,
                                                          OCDoHandle handle,
                                                          OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    if (!response)
    {
        OIC_LOG(ERROR, TAG, "Received NULL response!");
        goto exit;
    }

    if (response->payload)
    {
        OIC_LOG(ERROR, TAG, "Payload received");
        OIC_LOG_PAYLOAD(DEBUG, response->payload);
    }

    if (response->result != LOGIN_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Login error: %d",response->result);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Login successful");
        SessionParsePayload((OCRepPayload*)response->payload);
    }
exit:
    handleCB(NULL, OC_STACK_OK, NULL);
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Cloud authentication
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] authProvider      auth provider
 * @param[in] authToken         token
 * @return  OCStackResult application result
 */
OCStackResult
CloudSignUp(OCDevAddr *endPoint,
           const char *authProvider,
           const char *authToken,
           OCClientResponseHandler response)
{
    char uri[MAX_URI_LENGTH] = { 0 };

    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    if (!endPoint || !authProvider || !authToken)
    {
        OIC_LOG(ERROR, TAG, "Some of the input params are NULL");
        return OC_STACK_INVALID_PARAM;
    }

    char *deviceId = getDeviceId();
    if (!deviceId)
    {
        OIC_LOG(ERROR, TAG, "Can't get the device id");
        return OC_STACK_ERROR;
    }

    snprintf(uri, MAX_URI_LENGTH, DEFAULT_QUERY,
             endPoint->addr, endPoint->port, OC_RSRVD_ACCOUNT_URI);

    OIC_LOG_V(DEBUG, TAG, "Query created: %s", uri);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.context = (void*)handleCloudSignUpResponse;

    OCRepPayload *payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_DEVICE_ID, deviceId);
    OCRepPayloadSetPropString(payload, OC_RSRVD_AUTHPROVIDER, OICStrdup(authProvider));
    OCRepPayloadSetPropString(payload, OC_RSRVD_AUTHCODE, OICStrdup(authToken));

    return OCDoResource(NULL, OC_REST_POST, uri, NULL, (OCPayload *)payload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

/**
 * Cloud Sign In handler
 *
 * @param[in] OCClientResponse  cloud response
 * @return  OCStackApplicationResult application result
 */
OCStackApplicationResult handleCloudSignInResponse(void *ctx,
                                                          OCDoHandle handle,
                                                          OCClientResponse *response)
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    if (!response)
    {
        OIC_LOG(ERROR, TAG, "Received NULL response!");
        goto exit;
    }

    if (response->payload)
    {
        OIC_LOG(ERROR, TAG, "Payload received");
        OIC_LOG_PAYLOAD(DEBUG, response->payload);
    }

    if (response->result < 4 && response->payload)
    {
        OIC_LOG_V(ERROR, TAG, "Sign In error: result: %d, payload exist: %s",
                  response->result, response->payload ? "yes" : "no");
        goto exit;
    }

    if (!OCRepPayloadGetPropString((OCRepPayload*)response->payload, OC_RSRVD_USER_UUID,
                                   &(sessionObject.uid)))
    {
        OIC_LOG(ERROR, TAG, "Can't get: uid");
    }

    if (!OCRepPayloadGetPropInt((OCRepPayload*)response->payload, OC_RSRVD_EXPIRES_IN,
                                &(sessionObject.expiresin)))
    {
        OIC_LOG(ERROR, TAG, "Cann't get: expiresin");
    }

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);

exit:
    handleCB(NULL, OC_STACK_OK, NULL);
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Sign In request function
 *
 * @param[in] endPoint          cloud host and port
 * @return  OCStackResult application result
 */
OCStackResult CloudSignIn(OCDevAddr  *endPoint, OCClientResponseHandler response)
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    if (!endPoint)
    {
        OIC_LOG_V(ERROR, TAG, "%s: endPoint is NULL",__func__);
        return OC_STACK_INVALID_PARAM;
    }

    if (!sessionObject.uid)
    {
        OIC_LOG_V(ERROR, TAG, "%s: UID is missing. Please run Sign Up first",__func__);
        return OC_STACK_ERROR;
    }

    if (!sessionObject.accesstoken)
    {
        OIC_LOG_V(ERROR, TAG, "%s: accesstoken is missing. Please run Sign Up first",__func__);
        return OC_STACK_ERROR;
    }

    char *deviceId = getDeviceId();
    if (!deviceId)
    {
        OIC_LOG(ERROR, TAG, "Can't get the device id");
        return OC_STACK_ERROR;
    }

    OCRepPayload* payload = OCRepPayloadCreate();
    if (NULL == payload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_USER_UUID, sessionObject.uid);
    OCRepPayloadSetPropString(payload, OC_RSRVD_DEVICE_ID, deviceId);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ACCESS_TOKEN,
                              sessionObject.accesstoken);
    OCRepPayloadSetPropBool(payload, OC_RSRVD_LOGIN, true);

    char uri[MAX_URI_QUERY] = { 0, };
    snprintf(uri, MAX_URI_QUERY, DEFAULT_QUERY,
             endPoint->addr, endPoint->port,
             OC_RSRVD_ACCOUNT_SESSION_URI);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.context = (void*)handleCloudSignInResponse;

    return OCDoResource(NULL, OC_REST_POST, uri, NULL,
                       (OCPayload *)payload,
                       CT_ADAPTER_TCP,
                       OC_LOW_QOS, &cbData, NULL, 0);
}
/**
 * Cloud Sign Out handler
 *
 * @param[in] OCClientResponse  cloud response
 * @return  OCStackApplicationResult application result
 */
OCStackApplicationResult handleCloudSignOutResponse(void *ctx,
                                                          OCDoHandle handle,
                                                          OCClientResponse *response)
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    if (!response)
    {
        OIC_LOG(ERROR, TAG, "Received NULL response!");
        goto exit;
    }

    if (response->payload)
    {
        OIC_LOG(ERROR, TAG, "Payload received");
        OIC_LOG_PAYLOAD(DEBUG, response->payload);
    }

    if (response->result < 4 && response->payload)
    {
        OIC_LOG_V(ERROR, TAG, "Sign Out error");
        return OC_STACK_DELETE_TRANSACTION;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Sign Out OK");
    }

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);

exit:
    handleCB(NULL, OC_STACK_OK, NULL);
    return OC_STACK_DELETE_TRANSACTION;
}
/**
 * Sign In request function
 *
 * @param[in] endPoint          cloud host and port
 * @return  OCStackResult application result
 */
OCStackResult CloudSignOut(OCDevAddr  *endPoint, OCClientResponseHandler response)
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    if (!endPoint)
    {
        OIC_LOG_V(ERROR, TAG, "%s: endPoint is NULL",__func__);
        return OC_STACK_INVALID_PARAM;
    }

    char *deviceId = getDeviceId();
    if (!deviceId)
    {
        OIC_LOG(ERROR, TAG, "Cann't get the device id");
        return OC_STACK_ERROR;
    }

    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_DEVICE_ID, deviceId);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ACCESS_TOKEN,
                              sessionObject.accesstoken);
    OCRepPayloadSetPropBool(payload, OC_RSRVD_LOGIN, false);

    char uri[MAX_URI_QUERY] = { 0, };
    snprintf(uri, MAX_URI_QUERY, DEFAULT_QUERY,
             endPoint->addr, endPoint->port,
             OC_RSRVD_ACCOUNT_SESSION_URI);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.context = (void*)handleCloudSignOutResponse;

    return OCDoResource(NULL, OC_REST_POST, uri, NULL,
                       (OCPayload *)payload,
                       CT_ADAPTER_TCP,
                       OC_LOW_QOS, &cbData, NULL, 0);
}

