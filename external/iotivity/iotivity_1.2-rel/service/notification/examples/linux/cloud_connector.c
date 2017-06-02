//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// This sample provides the way to create cloud sample

#include "ocstack.h"
#include "ocpayload.h"
#include "rd_client.h"
#include "cloud_connector.h"

static bool isCloudLoggedin = false;

OCStackResult OCCloudSignup(const char *host, const char *deviceId,
        const char *authprovider,
        const char *authcode, OCClientResponseHandler response)
{
        char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
        snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, DEFAULT_AUTH_SIGNUP);

        OCCallbackData cbData;
        memset(&cbData, 0, sizeof(OCCallbackData));
        cbData.cb = response;
        cbData.cd = NULL;
        cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

        printf("Host: %s\n", targetUri);
        printf("Dev id: %s\n", deviceId);
        printf("Auth Provider: %s\n", authprovider);
        printf("Auth Code: %s\n", authcode);

        OCRepPayload *registerPayload = OCRepPayloadCreate();

        OCRepPayloadSetPropString(registerPayload, "di", deviceId);
        OCRepPayloadSetPropString(registerPayload, "authprovider", authprovider);
        OCRepPayloadSetPropString(registerPayload, "authcode", authcode);

        return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)registerPayload,
                CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudSession(const char *host, const char *query, const char *uId,
        const char *deviceId,
        const char *accesstoken,
        bool isLogin, OCClientResponseHandler response)
{
        char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
        snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

        OCCallbackData cbData;
        memset(&cbData, 0, sizeof(OCCallbackData));
        cbData.cb = response;
        cbData.cd = NULL;
        cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

        OCRepPayload *loginoutPayload = OCRepPayloadCreate();

        if (uId != NULL)
        {
                OCRepPayloadSetPropString(loginoutPayload, "uid", uId);
        }

        if (deviceId != NULL)
        {
                OCRepPayloadSetPropString(loginoutPayload, "di", deviceId);
        }

        if (accesstoken != NULL)
        {
                OCRepPayloadSetPropString(loginoutPayload, "accesstoken", accesstoken);
        }
        OCRepPayloadSetPropBool(loginoutPayload, "login", isLogin);

        return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)loginoutPayload,
                CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

//Client should call refresh before expiresin or when receive 4.01 during sign-in
OCStackResult OCCloudRefresh(const char *host, const char *query, const char *uId,
        const char *deviceId, const char *refreshtoken, OCClientResponseHandler response)
{
        char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
        snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

        OCCallbackData cbData;
        memset(&cbData, 0, sizeof(OCCallbackData));
        cbData.cb = response;
        cbData.cd = NULL;
        cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

        OCRepPayload *refreshPayload = OCRepPayloadCreate();

        OCRepPayloadSetPropString(refreshPayload, "uid", uId);
        OCRepPayloadSetPropString(refreshPayload, "di", deviceId);
        OCRepPayloadSetPropString(refreshPayload, "granttype", "refresh_token");
        OCRepPayloadSetPropString(refreshPayload, "refreshtoken", refreshtoken);

        return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)refreshPayload,
                CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult OCCloudLogin(const char *host, const char *uId, const char *deviceId,
        const char *accesstoken, OCClientResponseHandler response)
{
        return OCCloudSession(host, DEFAULT_AUTH_SESSION, uId, deviceId, accesstoken, true, response);
}

OCStackResult OCCloudLogout(const char *host, OCClientResponseHandler response)
{
        return OCCloudSession(host, DEFAULT_AUTH_SESSION, NULL, NULL, NULL, false, response);
}

void printRepresentation(OCRepPayloadValue *value)
{
        while (value)
        {
                printf("Key: %s / ", value->name);
                switch (value->type)
                {
                case OCREP_PROP_NULL:
                        printf("Value: None\n");
                        break;
                case OCREP_PROP_INT:
                        printf("Value: %d\n", value->i);
                        break;
                case OCREP_PROP_DOUBLE:
                        printf("Value: %f\n", value->d);
                        break;
                case OCREP_PROP_BOOL:
                        printf("Value: %d\n", value->b);
                        break;
                case OCREP_PROP_STRING:
                        printf("Value: %s\n", value->str);
                        break;
                case OCREP_PROP_BYTE_STRING:
                        printf("Value: Byte String\n");
                        break;
                case OCREP_PROP_OBJECT:
                        printf("Value: Object\n");
                        break;
                case OCREP_PROP_ARRAY:
                        printf("Value: Array\n");
                        break;
                default:
                        break;
                }
                value = value->next;
        }
}

// callback for sign-up and sign-in
OCStackApplicationResult CloudSignupCallback(void *ctx,
        OCDoHandle handle, OCClientResponse *clientResponse)
{
    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        printf("Invalid Cloud Login/out callback received\n");
    }

    printf("Signup response received: %d\n", clientResponse->result);

    if (clientResponse->payload != NULL &&
            clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        printf("PAYLOAD_TYPE_REPRESENTATION received\n");
        printf("Sign-in using retrieved accesstoken when disconnected or reboot\n");

        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;

        printf("Get payload values\n");
        printRepresentation(val);
    }
    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult CloudLoginoutCallback(void *ctx,
        OCDoHandle handle, OCClientResponse *clientResponse)
{
    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        printf("Invalid Cloud Login/out callback received\n");
    }

    printf("Login/out response received: %d\n", clientResponse->result);

    if (clientResponse->payload != NULL &&
            clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        printf("PAYLOAD_TYPE_REPRESENTATION received\n");

        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;

        printf("Get payload values\n");
        printRepresentation(val);

        if(clientResponse->result < 5)
        {
            isCloudLoggedin = true;
        }
    }

    return OC_STACK_KEEP_TRANSACTION;
}

bool IsCloudLoggedin()
{
    return isCloudLoggedin;
}

