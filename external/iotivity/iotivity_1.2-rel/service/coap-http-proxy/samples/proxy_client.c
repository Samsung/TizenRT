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

#include "iotivity_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <getopt.h>
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "payload_logging.h"

/**
 * List of methods that can be initiated from proxy client
 */
typedef enum {
    TEST_DISCOVER_REQ = 1,
    TEST_GET_REQ,
    TEST_POST_REQ,
    TEST_PUT_REQ,
    TEST_DELETE_REQ,
    MAX_TESTS
} clientTests_t;

#define TAG "PROXY_CLIENT"
#define MAX_HTTP_URI_LENGTH 1024
// Tracking user input
static clientTests_t testCase = MAX_TESTS;
static const char* discoveryQuery = "/oic/res";
static OCDevAddr serverAddr;
OCConnectivityType connType;

// Will be taken as user input
static char httpResource[MAX_HTTP_URI_LENGTH];

int gQuitFlag = 0;
/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        gQuitFlag = 1;
    }
}

OCPayload* putPayload()
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(INFO, TAG, "Failed to create payload object");
        return NULL;
    }

    /* Set properties for your http server here.
    */
    OCRepPayloadSetPropInt(payload, "power", 10);
    OCRepPayloadSetPropBool(payload, "state", true);

    return (OCPayload*)payload;
}

static void PrintUsage()
{
    OIC_LOG(INFO, TAG, "Usage : proxy_client -t <1..5>");
    OIC_LOG(INFO, TAG, "-t 1  :  Discover Proxy");
    OIC_LOG(INFO, TAG, "-t 2 -p \"http_uri\":  Discover Proxy and Initiate HTTP GET Request");
    OIC_LOG(INFO, TAG, "-t 3 -p \"http_uri\":  Discover Proxy and Initiate HTTP POST Request");
    OIC_LOG(INFO, TAG, "-t 4 -p \"http_uri\":  Discover Proxy and Initiate HTTP PUT Request");
    OIC_LOG(INFO, TAG, "-t 5 -p \"http_uri\":  Discover Proxy and Initiate HTTP DELETE Request");
}

OCStackApplicationResult putReqCB(void* ctx, OCDoHandle handle,
                                  OCClientResponse* clientResponse)
{
    UNUSED(ctx);
    UNUSED(handle);
    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %u", clientResponse->result);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, "=============> Put Response");
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "%s received Null clientResponse", __func__);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult postReqCB(void* ctx, OCDoHandle handle,
                                   OCClientResponse* clientResponse)
{
    UNUSED(ctx);
    UNUSED(handle);
    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %u", clientResponse->result);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, "=============> Post Response");
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "%s received Null clientResponse", __func__);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult deleteReqCB(void* ctx,
                                     OCDoHandle handle,
                                     OCClientResponse* clientResponse)
{
    UNUSED(ctx);
    UNUSED(handle);
    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %d",  clientResponse->result);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, "=============> Delete Response");
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "%s received Null clientResponse", __func__);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult getReqCB(void* ctx, OCDoHandle handle,
                                  OCClientResponse* clientResponse)
{
    UNUSED(ctx);
    UNUSED(handle);
    if (!clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "%s received NULL clientResponse", __func__);
        return OC_STACK_DELETE_TRANSACTION;
    }

    OIC_LOG_V(INFO, TAG, "StackResult: %d", clientResponse->result);
    OIC_LOG_V(INFO, TAG, "SEQUENCE NUMBER: %d", clientResponse->sequenceNumber);
    OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
    OIC_LOG(INFO, TAG, "=============> Get Response");

    if (clientResponse->numRcvdVendorSpecificHeaderOptions > 0)
    {
        OIC_LOG (INFO, TAG, "Received vendor specific options");
        uint8_t i = 0;
        OCHeaderOption* rcvdOptions = clientResponse->rcvdVendorSpecificHeaderOptions;
        for (i = 0; i < clientResponse->numRcvdVendorSpecificHeaderOptions; i++)
        {
            if (((OCHeaderOption)rcvdOptions[i]).protocolID == OC_COAP_ID)
            {
                OIC_LOG_V(INFO, TAG, "Received option with OC_COAP_ID and ID %u with",
                          ((OCHeaderOption)rcvdOptions[i]).optionID);

                OIC_LOG_BUFFER(INFO, TAG, ((OCHeaderOption)rcvdOptions[i]).optionData,
                                          ((OCHeaderOption)rcvdOptions[i]).optionLength);
            }
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

int InitProxyRequest()
{
    OIC_LOG(INFO, TAG, "InitProxyRequest");
    OCHeaderOption option;
    memset(&option, 0, sizeof(option));
    option.protocolID = OC_COAP_ID;
    option.optionID = OC_RSRVD_PROXY_OPTION_ID;
    strncpy((char*)option.optionData, httpResource, sizeof(option.optionData));
    size_t opLen = strlen(httpResource);
    option.optionLength = opLen < sizeof(option.optionData) ? opLen : sizeof(option.optionData);

    // A request with proxy uri shall not have resource uri
    OCDoHandle handle;
    OCCallbackData cbData;
    cbData.context = NULL;
    cbData.cd = NULL;
    OCPayload* payload = NULL;
    OCMethod method;

    switch(testCase)
    {
        case TEST_DISCOVER_REQ:
            return 1;
        case TEST_GET_REQ:
            method = OC_REST_GET;
            cbData.cb = getReqCB;
            break;
        case TEST_POST_REQ:
            method = OC_REST_POST;
            cbData.cb = postReqCB;
            payload = putPayload();
            break;
        case TEST_PUT_REQ:
            method = OC_REST_PUT;
            cbData.cb = putReqCB;
            payload = putPayload();
            break;
        case TEST_DELETE_REQ:
            method = OC_REST_DELETE;
            cbData.cb = deleteReqCB;
            break;
        default:
            return 1;
    }

    OCStackResult ret = OCDoResource(&handle, method, NULL, &serverAddr,
                                     payload, connType, OC_LOW_QOS, &cbData, &option, 1);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoResource returns error %d with method %d", ret, method);
        return 1;
    }

    return 0;
}

// This is a function called back when a device is discovered
OCStackApplicationResult discoveryReqCB(void* ctx, OCDoHandle handle,
                                        OCClientResponse* clientResponse)
{
    UNUSED(ctx);
    UNUSED(handle);
    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %d", clientResponse->result);
        OIC_LOG_V(INFO, TAG, "Discovered on %u", clientResponse->connType);
        OIC_LOG_V(INFO, TAG, "Device =============> Discovered @ %s:%u",
                              clientResponse->devAddr.addr,
                              clientResponse->devAddr.port);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        serverAddr = clientResponse->devAddr;
        connType = clientResponse->connType;
        OCDiscoveryPayload* payload = (OCDiscoveryPayload*) clientResponse->payload;
        if (!payload)
        {
            // Receive other discovery responses
            return OC_STACK_KEEP_TRANSACTION;
        }

        int found = 0;
        OCResourcePayload* resource = (OCResourcePayload*) payload->resources;
        while (resource)
        {
            if (resource->uri && strcmp(resource->uri, OC_RSRVD_PROXY_URI) == 0)
            {
                found = 1;
                break;
            }
            resource = resource->next;
        }

        if (!found)
        {
            OIC_LOG(INFO, TAG, "No proxy in payload");
            return OC_STACK_KEEP_TRANSACTION;
        }

        switch (testCase)
        {
            case TEST_DISCOVER_REQ:
                break;
            case TEST_GET_REQ:
            case TEST_POST_REQ:
            case TEST_PUT_REQ:
            case TEST_DELETE_REQ:
                InitProxyRequest();
                break;
            default:
                PrintUsage();
                break;
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "discoveryReqCB received Null clientResponse");
    }
    return OC_STACK_KEEP_TRANSACTION;
}

int InitDiscovery()
{
    OCStackResult ret;
    OCCallbackData cbData;
    cbData.cb = discoveryReqCB;
    cbData.context = NULL;
    cbData.cd = NULL;

    ret = OCDoResource(NULL, OC_REST_DISCOVER, discoveryQuery, NULL, 0, CT_DEFAULT,
                       OC_LOW_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoResource error [%d]", ret);
    }
    return ret;
}

int main(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "t:p:")) != -1)
    {
        switch (opt)
        {
            case 't':
                testCase = atoi(optarg);
                break;
            case 'p':
                if (optarg)
                {
                    /* Copy maximum upto "sizeof(httpResource) - 1" to safeguard against
                       buffer overrun. Last byte is already NULL. */
                    strncpy(httpResource, optarg, sizeof(httpResource) - 1);
                }
                break;
            default:
                PrintUsage();
                return -1;
        }
    }

    if ((testCase < TEST_DISCOVER_REQ || testCase >= MAX_TESTS) ||
        (testCase != TEST_DISCOVER_REQ && httpResource[0] == '\0'))
    {
        PrintUsage();
        return -1;
    }

    if (OCInit1(OC_CLIENT, OC_DEFAULT_FLAGS, OC_DEFAULT_FLAGS) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack initialization error");
        return -1;
    }

    if (InitDiscovery())
    {
        OIC_LOG(ERROR, TAG, "Failure initiating discovery");
        return -1;
    }

    // Break from loop with Ctrl+C
    OIC_LOG(INFO, TAG, "Entering proxy_client main loop.");
    signal(SIGINT, handleSigInt);
    while (!gQuitFlag)
    {

        if (OCProcess() != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return -1;
        }

        sleep(1);
    }

    OIC_LOG(INFO, TAG, "Exiting proxy_client main loop.");
    if (OCStop() != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack stop error");
        return -1;
    }

    return 0;
}

