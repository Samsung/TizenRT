//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

// Warning disabled globally but VS2013 ignores the /wd4200 option in C++ files.
#if defined(_MSC_VER) && _MSC_VER < 1900
#pragma warning(disable : 4200)
#endif

#include "iotivity_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "ocstack.h"
#include "ocpayload.h"
#include "pinoxmcommon.h"
#include "cacommon.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <coap/pdu.h>
#include "logger.h"
#include "occlient.h"
#include "payload_logging.h"
#include "common.h"

#define VERIFY_SUCCESS(op)                          \
do                                                  \
{                                                   \
    if (op != OC_STACK_OK)                          \
    {                                               \
        OIC_LOG_V(FATAL, TAG, "%s failed!!", #op);  \
        goto exit;                                  \
    }                                               \
} while(0)

#ifdef ROUTING_GATEWAY
/**
 * Maximum number of gateway requests to form the routing table.
 */
#define MAX_NUM_GATEWAY_REQUEST 20

/**
 * Sleep duration after every OCProcess().
 */
#define SLEEP_DURATION 100000
#endif
// Tracking user input
static int UnicastDiscovery = 0;
static int TestCase = 0;
static int Connectivity = 0;
static int Introspection = 0;
static int OCFSpecVersion = 1;

static const char *DEVICE_DISCOVERY_QUERY = "%s/oic/d";
static const char *PLATFORM_DISCOVERY_QUERY = "%s/oic/p";
static const char *RESOURCE_DISCOVERY_QUERY = "%s/oic/res";

// Device information
static const char* gDeviceName = "OCClient";
static const char* gSpecVersion = "ocf.1.1.0";
static const char* gDataModelVersions = "ocf.res.1.1.0,ocf.sh.1.1.0";
static const char* gProtocolIndependentID = "31B59DA2-E68F-4A47-81C5-93E5CBF37D0B";

// Platform information
static const char* gDateOfManufacture = "2016-01-15";
static const char* gFirmwareVersion = "myFirmwareVersion";
static const char* gManufacturerName = "myName";
static const char* gOperatingSystemVersion = "myOS";
static const char* gHardwareVersion = "myHardwareVersion";
static const char* gPlatformID = "6A47983C-CA70-4397-8280-5C34EF23B63B";
static const char* gManufacturerUrl = "https://www.iotivity.org";
static const char* gModelNumber = "myModelNumber";
static const char* gPlatformVersion = "myPlatformVersion";
static const char* gSupportUrl = "https://www.iotivity.org";
static const char* gSystemTime = "2015-05-15T11.04";

//The following variable determines the interface protocol (IPv4, IPv6, etc)
//to be used for sending unicast messages. Default set to IP dual stack.
static OCConnectivityType ConnType = CT_ADAPTER_IP;
static OCDevAddr serverAddr;
static char discoveryAddr[100];
static std::string coapServerResource = "/a/light";
static std::string introspectionResType = "oic.wk.introspection";

int InitIntrospectionPayload(OCClientResponse * clientResponse);

#ifdef WITH_PRESENCE
// The handle for observe registration
OCDoHandle gPresenceHandle;
#endif
// After this crosses a threshold client deregisters for further notifications
int gNumObserveNotifies = 0;

#ifdef WITH_PRESENCE
int gNumPresenceNotifies = 0;
#endif

int gQuitFlag = 0;
/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        gQuitFlag = 1;
    }
}

OCPayload* createPayload()
{
    OCRepPayload* payload = OCRepPayloadCreate();

    if (!payload)
    {
        std::cout << "Failed to create payload object"<<std::endl;
        std::exit(1);
    }

    OCRepPayloadSetPropInt(payload, "power", 15);
    OCRepPayloadSetPropBool(payload, "state", true);

    return (OCPayload*) payload;
}

static void PrintUsage()
{
    OIC_LOG(INFO, TAG, "Usage : occlient -u <0|1> -t <1..24> -c <0|1> -i <0|1> -s <0|1>");
    OIC_LOG(INFO, TAG, "-u <0|1> : Perform multicast/unicast discovery of resources");
    OIC_LOG(INFO, TAG, "-c 0 : Use Default connectivity(IP)");
    OIC_LOG(INFO, TAG, "-c 1 : IP Connectivity Type");
    OIC_LOG(INFO, TAG, "-t 1  :  Discover Resources");
    OIC_LOG(INFO, TAG, "-t 2  :  Discover Resources and Initiate Nonconfirmable Get Request");
    OIC_LOG(INFO, TAG, "-t 3  :  Discover Resources and Initiate Nonconfirmable Get Request"
            " with query filter.");
    OIC_LOG(INFO, TAG, "-t 4  :  Discover Resources and Initiate Nonconfirmable Put Requests");
    OIC_LOG(INFO, TAG, "-t 5  :  Discover Resources and Initiate Nonconfirmable Post Requests");
    OIC_LOG(INFO, TAG, "-t 6  :  Discover Resources and Initiate Nonconfirmable Delete Requests");
    OIC_LOG(INFO, TAG, "-t 7  :  Discover Resources and Initiate Nonconfirmable Observe Requests");
    OIC_LOG(INFO, TAG, "-t 8  :  Discover Resources and Initiate Nonconfirmable Get Request "\
            "for a resource which is unavailable");
    OIC_LOG(INFO, TAG, "-t 9  :  Discover Resources and Initiate Confirmable Get Request");
    OIC_LOG(INFO, TAG, "-t 10 :  Discover Resources and Initiate Confirmable Post Request");
    OIC_LOG(INFO, TAG, "-t 11 :  Discover Resources and Initiate Confirmable Delete Requests");
    OIC_LOG(INFO, TAG, "-t 12 :  Discover Resources and Initiate Confirmable Observe Requests"\
            " and cancel with Low QoS");

#ifdef WITH_PRESENCE
    OIC_LOG(INFO, TAG, "-t 13 :  Discover Resources and Initiate Nonconfirmable presence");
    OIC_LOG(INFO, TAG, "-t 14 :  Discover Resources and Initiate Nonconfirmable presence with "\
            "filter");
    OIC_LOG(INFO, TAG, "-t 15 :  Discover Resources and Initiate Nonconfirmable presence with "\
            "2 filters");
    OIC_LOG(INFO, TAG, "-t 16 :  Discover Resources and Initiate Nonconfirmable multicast presence.");
#endif

    OIC_LOG(INFO, TAG, "-t 17 :  Discover Resources and Initiate Nonconfirmable Observe Requests "\
            "then cancel immediately with High QOS");
    OIC_LOG(INFO, TAG, "-t 18 :  Discover Resources and Initiate Nonconfirmable Get Request and "\
            "add  vendor specific header options");
    OIC_LOG(INFO, TAG, "-t 19 :  Discover Platform");
    OIC_LOG(INFO, TAG, "-t 20 :  Discover Devices");
    OIC_LOG(INFO, TAG, "-t 21 :  Discover Resources and Display endpoints of the server information");
    OIC_LOG(INFO, TAG, "-t 22 :  Discover Resources and Perform Get Requests by IPv4 + COAP + UDP "\
            "using server's endpoints information");
    OIC_LOG(INFO, TAG, "-t 23 :  Discover Resources and Perform Get Requests by IPv4 + COAP + TCP "\
            "using server's endpoints information");
    OIC_LOG(INFO, TAG, "-t 24 :  Discover Introspection Resources and Perform Get Request");
    OIC_LOG(INFO, TAG, "-s 0 :  Specify the device spec version as core.0.0.0");
    OIC_LOG(INFO, TAG, "-s 1 :  Specify the device spec version as ocf.1.1.0");
}

OCStackResult InvokeOCDoResource(std::ostringstream &query,
                                 OCDevAddr *remoteAddr,
                                 OCMethod method,
                                 OCQualityOfService qos,
                                 OCClientResponseHandler cb,
                                 OCHeaderOption * options,
                                 uint8_t numOptions)
{
    OCStackResult ret;
    OCCallbackData cbData;
    OCDoHandle handle;

    cbData.cb = cb;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OCPayload* payload = (method == OC_REST_PUT || method == OC_REST_POST) ? createPayload() : NULL;

    ret = OCDoRequest(&handle, method, query.str().c_str(), remoteAddr,
                      payload, (ConnType), qos, &cbData, options, numOptions);

    OCPayloadDestroy(payload);

    if (ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoResource returns error %d with method %d", ret, method);
    }
#ifdef WITH_PRESENCE
    else if (method == OC_REST_PRESENCE)
    {
        gPresenceHandle = handle;
    }
#endif

    return ret;
}

OCStackApplicationResult putReqCB(void* ctx, OCDoHandle /*handle*/,
                                  OCClientResponse * clientResponse)
{
    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for PUT recvd successfully");
    }

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s",  getResult(clientResponse->result));
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, ("=============> Put Response"));
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "putReqCB received Null clientResponse");
    }
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult postReqCB(void *ctx, OCDoHandle /*handle*/,
                                   OCClientResponse *clientResponse)
{
    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for POST recvd successfully");
    }

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s",  getResult(clientResponse->result));
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, ("=============> Post Response"));
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "postReqCB received Null clientResponse");
    }
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult deleteReqCB(void *ctx,
                                     OCDoHandle /*handle*/,
                                     OCClientResponse *clientResponse)
{
    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for DELETE recvd successfully");
    }

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s",  getResult(clientResponse->result));
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, ("=============> Delete Response"));
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "deleteReqCB received Null clientResponse");
    }
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult getReqCB(void* ctx, OCDoHandle /*handle*/,
                                  OCClientResponse * clientResponse)
{
    if (clientResponse == NULL)
    {
        OIC_LOG(INFO, TAG, "getReqCB received NULL clientResponse");
        return   OC_STACK_DELETE_TRANSACTION;
    }

    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for GET query recvd successfully");
    }

    OIC_LOG_V(INFO, TAG, "StackResult: %s",  getResult(clientResponse->result));
    OIC_LOG_V(INFO, TAG, "SEQUENCE NUMBER: %d", clientResponse->sequenceNumber);
    OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
    OIC_LOG(INFO, TAG, ("=============> Get Response"));

    if (clientResponse->numRcvdVendorSpecificHeaderOptions > 0)
    {
        OIC_LOG (INFO, TAG, "Received vendor specific options");
        uint8_t i = 0;
        OCHeaderOption * rcvdOptions = clientResponse->rcvdVendorSpecificHeaderOptions;
        for( i = 0; i < clientResponse->numRcvdVendorSpecificHeaderOptions; i++)
        {
            if (((OCHeaderOption)rcvdOptions[i]).protocolID == OC_COAP_ID)
            {
                OIC_LOG_V(INFO, TAG, "Received option with OC_COAP_ID and ID %u with",
                        ((OCHeaderOption)rcvdOptions[i]).optionID );
               if (COAP_OPTION_CONTENT_VERSION == ((OCHeaderOption)rcvdOptions[i]).optionID)
               {
                    uint16_t versionValue = rcvdOptions[i].optionData[0] * 256
                            + rcvdOptions[i].optionData[1];
                    OIC_LOG_V(INFO, TAG, "Received version value of %u", versionValue);
               }
               if (COAP_OPTION_CONTENT_FORMAT == ((OCHeaderOption)rcvdOptions[i]).optionID)
               {
                    uint16_t formatValue = rcvdOptions[i].optionData[0] * 256
                            + rcvdOptions[i].optionData[1];
                   OIC_LOG_V(INFO, TAG, "Received format value of %u", formatValue);
               }
                OIC_LOG_BUFFER(INFO, TAG, ((OCHeaderOption)rcvdOptions[i]).optionData,
                    MAX_HEADER_OPTION_DATA_LENGTH);
            }
        }
    }

    switch (TestCase)
    {
    case TEST_INTROSPECTION:
        InitIntrospectionPayload(clientResponse);
        break;
    default:
        break;
    }

    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult obsReqCB(void* ctx, OCDoHandle handle,
                                  OCClientResponse * clientResponse)
{
    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for OBS query recvd successfully");
    }

    if (clientResponse)
    {
        if (clientResponse->sequenceNumber <= MAX_SEQUENCE_NUMBER)
        {
            if (clientResponse->sequenceNumber == OC_OBSERVE_REGISTER)
            {
                OIC_LOG(INFO, TAG, "This also serves as a registration confirmation.");
            }

            OIC_LOG_V(INFO, TAG, "StackResult: %s",  getResult(clientResponse->result));
            OIC_LOG_V(INFO, TAG, "SEQUENCE NUMBER: %d", clientResponse->sequenceNumber);

            if (clientResponse->result == OC_STACK_OK)
            {
                OIC_LOG_V(INFO, TAG, "Callback Context for OBSERVE notification recvd successfully %d",
                        gNumObserveNotifies);
                OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
                OIC_LOG(INFO, TAG, ("=============> Obs Response"));
                gNumObserveNotifies++;

                if (gNumObserveNotifies > 15) //large number to test observing in DELETE case.
                {
                    if (TestCase == TEST_OBS_REQ_NON || TestCase == TEST_OBS_REQ_CON)
                    {
                        OIC_LOG(ERROR, TAG, "Cancelling with LOW QOS");
                        if (OCCancel (handle, OC_LOW_QOS, NULL, 0) != OC_STACK_OK)
                        {
                            OIC_LOG(ERROR, TAG, "Observe cancel error");
                        }
                        return OC_STACK_DELETE_TRANSACTION;
                    }
                    else if (TestCase == TEST_OBS_REQ_NON_CANCEL_IMM)
                    {
                        OIC_LOG(ERROR, TAG, "Cancelling with HIGH QOS");
                        if (OCCancel (handle, OC_HIGH_QOS, NULL, 0) != OC_STACK_OK)
                        {
                            OIC_LOG(ERROR, TAG, "Observe cancel error");
                        }
                    }
                }
            }
        }
        else
        {
            OIC_LOG(INFO, TAG, "No observe option header is returned in the response.");
            OIC_LOG(INFO, TAG, "For a registration request, it means the registration failed");
            return OC_STACK_DELETE_TRANSACTION;
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "obsReqCB received Null clientResponse");
    }
    return OC_STACK_KEEP_TRANSACTION;
}
#ifdef WITH_PRESENCE
OCStackApplicationResult presenceCB(void* ctx, OCDoHandle /*handle*/,
                                    OCClientResponse * clientResponse)
{
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for Presence recvd successfully");
    }

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s", getResult(clientResponse->result));
        OIC_LOG_V(INFO, TAG, "Callback Context for Presence notification recvd successfully %d",
                gNumPresenceNotifies);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, ("=============> Presence Response"));
        gNumPresenceNotifies++;
        if (gNumPresenceNotifies == 20)
        {
            if (OCCancel(gPresenceHandle, OC_LOW_QOS, NULL, 0) != OC_STACK_OK)
            {
                OIC_LOG(ERROR, TAG, "Presence cancel error");
            }
            return OC_STACK_DELETE_TRANSACTION;
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "presenceCB received Null clientResponse");
    }
    return OC_STACK_KEEP_TRANSACTION;
}
#endif

// This is a function called back when a device is discovered
OCStackApplicationResult discoveryReqCB(void* ctx, OCDoHandle /*handle*/,
                                        OCClientResponse * clientResponse)
{
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for DISCOVER query recvd successfully");
    }

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s", getResult(clientResponse->result));

        std::string connectionType = getConnectivityType (clientResponse->connType);
        OIC_LOG_V(INFO, TAG, "Discovered on %s", connectionType.c_str());
        OIC_LOG_V(INFO, TAG,
                "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);

        ConnType = clientResponse->connType;
        serverAddr = clientResponse->devAddr;

        OCDiscoveryPayload *payload = (OCDiscoveryPayload*) clientResponse->payload;
        if (!payload)
        {
            return OC_STACK_DELETE_TRANSACTION;
        }

        OCResourcePayload *resource = (OCResourcePayload*) payload->resources;
        int found = 0;
        while (resource)
        {
            if(resource->uri && strcmp(resource->uri, coapServerResource.c_str()) == 0)
            {
                found = 1;
                break;
            }
            resource = resource->next;
        }

        if(!found)
        {
            OIC_LOG_V (INFO, TAG, "No %s in payload", coapServerResource.c_str());
            return OC_STACK_KEEP_TRANSACTION;
        }

        switch(TestCase)
        {
            case TEST_GET_REQ_NON:
                InitGetRequest(OC_LOW_QOS, 0, 0);
                break;
            case TEST_GET_REQ_NON_WITH_FILTERS:
                InitGetRequest(OC_LOW_QOS, 0, 1);
                break;
            case TEST_PUT_REQ_NON:
                InitPutRequest(OC_LOW_QOS);
                break;
            case TEST_POST_REQ_NON:
                InitPostRequest(OC_LOW_QOS);
                break;
            case TEST_DELETE_REQ_NON:
                InitDeleteRequest(OC_LOW_QOS);
                break;
            case TEST_OBS_REQ_NON:
            case TEST_OBS_REQ_NON_CANCEL_IMM:
                InitObserveRequest(OC_LOW_QOS);
                break;
            case TEST_GET_UNAVAILABLE_RES_REQ_NON:
                InitGetRequestToUnavailableResource(OC_LOW_QOS);
                break;
            case TEST_GET_REQ_CON:
                InitGetRequest(OC_HIGH_QOS, 0, 0);
                break;
            case TEST_POST_REQ_CON:
                InitPostRequest(OC_HIGH_QOS);
                break;
            case TEST_DELETE_REQ_CON:
                InitDeleteRequest(OC_HIGH_QOS);
                break;
            case TEST_OBS_REQ_CON:
                InitObserveRequest(OC_HIGH_QOS);
                break;
#ifdef WITH_PRESENCE
            case TEST_OBS_PRESENCE:
            case TEST_OBS_PRESENCE_WITH_FILTER:
            case TEST_OBS_PRESENCE_WITH_FILTERS:
            case TEST_OBS_MULTICAST_PRESENCE:
                InitPresence();
                break;
#endif
            case TEST_GET_REQ_NON_WITH_VENDOR_HEADER_OPTIONS:
                InitGetRequest(OC_LOW_QOS, 1, 0);
                break;
            case TEST_DISCOVER_PLATFORM_REQ:
                InitPlatformDiscovery(OC_LOW_QOS);
                break;
            case TEST_DISCOVER_DEV_REQ:
                InitDeviceDiscovery(OC_LOW_QOS);
                break;
            case TEST_DISCOVER_REQ_SHOW_EPS:
                showEndpointsInfo(resource);
                break;
            case TEST_GET_REQ_UDP:
                InitGetRequestWithCoap(payload, true);
                break;
            case TEST_GET_REQ_TCP:
                InitGetRequestWithCoap(payload, false);
                break;
            case TEST_INTROSPECTION:
                InitIntrospection(payload);
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

OCStackApplicationResult PlatformDiscoveryReqCB(void* ctx,
                                                OCDoHandle /*handle*/,
                                                OCClientResponse * clientResponse)
{
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for Platform DISCOVER query recvd successfully");
    }

    if (clientResponse)
    {
        OIC_LOG(INFO, TAG, ("Discovery Response:"));
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "PlatformDiscoveryReqCB received Null clientResponse");
    }

    return (UnicastDiscovery) ? OC_STACK_DELETE_TRANSACTION : OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult DeviceDiscoveryReqCB(void* ctx, OCDoHandle /*handle*/,
                                              OCClientResponse * clientResponse)
{
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "Callback Context for Device DISCOVER query recvd successfully");
    }

    if (clientResponse)
    {
        OIC_LOG(INFO, TAG, ("Discovery Response:"));
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "PlatformDiscoveryReqCB received Null clientResponse");
    }

    return (UnicastDiscovery) ? OC_STACK_DELETE_TRANSACTION : OC_STACK_KEEP_TRANSACTION;
}

#ifdef WITH_PRESENCE
int InitPresence()
{
    OCStackResult result = OC_STACK_OK;
    OIC_LOG_V(INFO, TAG, "\n\nExecuting %s", __func__);
    std::ostringstream query;
    std::ostringstream querySuffix;
    query << OC_RSRVD_PRESENCE_URI;
    if (TestCase == TEST_OBS_PRESENCE)
    {
        result = InvokeOCDoResource(query, &serverAddr, OC_REST_PRESENCE,
                OC_LOW_QOS, presenceCB, NULL, 0);
    }
    if (TestCase == TEST_OBS_PRESENCE_WITH_FILTER || TestCase == TEST_OBS_PRESENCE_WITH_FILTERS)
    {
        querySuffix.str("");
        querySuffix << query.str() << "?rt=core.led";
        result = InvokeOCDoResource(querySuffix, &serverAddr, OC_REST_PRESENCE,
                OC_LOW_QOS, presenceCB, NULL, 0);
    }
    if (TestCase == TEST_OBS_PRESENCE_WITH_FILTERS)
    {
        if (result == OC_STACK_OK)
        {
            querySuffix.str("");
            querySuffix << query.str() << "?rt=core.fan";
            result = InvokeOCDoResource(querySuffix, &serverAddr, OC_REST_PRESENCE, OC_LOW_QOS,
                    presenceCB, NULL, 0);
        }
    }
    if (TestCase == TEST_OBS_MULTICAST_PRESENCE)
    {
        if (result == OC_STACK_OK)
        {
            result = InvokeOCDoResource(query, NULL, OC_REST_PRESENCE, OC_LOW_QOS,
                    presenceCB, NULL, 0);
        }
    }
    return result;
}
#endif

int InitGetRequestToUnavailableResource(OCQualityOfService qos)
{
    std::ostringstream query;
    query << "/SomeUnknownResource";
    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());
    return (InvokeOCDoResource(query, &serverAddr, OC_REST_GET, (qos == OC_HIGH_QOS)? OC_HIGH_QOS:OC_LOW_QOS,
            getReqCB, NULL, 0));
}

int InitIntrospectionPayload(OCClientResponse * clientResponse)
{
    std::ostringstream query;
    std::string introspectionPayloadUrl;
    OCRepPayload *introspectionInfo = (OCRepPayload*)clientResponse->payload;
    if (NULL == introspectionInfo)
    {
        OIC_LOG(ERROR, TAG, "\nFailed to get introspection info from NULL payload");
        return OC_STACK_ERROR;
    }

    OCRepPayloadValue *value = introspectionInfo->values;

    while (value)
    {
        if (strcmp(value->name, "urlInfo") == 0)
        {
            break;
        }
        value = value->next;
    }

    if (value && (value->arr.dimensions[0] > 0))
    {
        OCRepPayloadValue *prop = value->arr.objArray[0]->values;
        while (prop)
        {
            if (strcmp(prop->name, "url") == 0)
            {
                introspectionPayloadUrl = prop->str;
                break;
            }
            prop = prop->next;
        }
    }

    if (introspectionPayloadUrl.length() <= 0)
    {
        OIC_LOG(ERROR, TAG, "\nFailed to get introspection URL from payload");
        return OC_STACK_ERROR;
    }
    else
    {
        query << introspectionPayloadUrl;
        OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());
        return (InvokeOCDoResource(query, &serverAddr, OC_REST_GET, OC_LOW_QOS,
            getReqCB, NULL, 0));
    }
}

int InitIntrospection(OCDiscoveryPayload* dis)
{
    OCResourcePayload* resource = (OCResourcePayload*)dis->resources;
    bool found = false;
    std::string introspectionUri;
    while (resource && !found)
    {
        OCStringLL* resTypes = resource->types;
        while (resTypes && !found)
        {
            if (strcmp(resTypes->value, introspectionResType.c_str()) == 0)
            {
                introspectionUri = resource->uri;
                found = true;
            }
            resTypes = resTypes->next;
        }
        resource = resource->next;
    }

    if (introspectionUri.length() == 0)
    {
        OIC_LOG(ERROR, TAG, "Could not find URI for introspection");
    }

    std::ostringstream query;
    query << introspectionUri;
    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());
    return (InvokeOCDoResource(query, &serverAddr, OC_REST_GET, OC_LOW_QOS,
            getReqCB, NULL, 0));
}

int InitObserveRequest(OCQualityOfService qos)
{
    std::ostringstream query;
    query << coapServerResource;
    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());
    return (InvokeOCDoResource(query, &serverAddr, OC_REST_OBSERVE,
            (qos == OC_HIGH_QOS)? OC_HIGH_QOS:OC_LOW_QOS, obsReqCB, NULL, 0));
}

int InitPutRequest(OCQualityOfService qos)
{
    std::ostringstream query;
    query << coapServerResource;
    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());
    return (InvokeOCDoResource(query, &serverAddr, OC_REST_PUT, (qos == OC_HIGH_QOS)? OC_HIGH_QOS:OC_LOW_QOS,
            putReqCB, NULL, 0));
}

int InitPostRequest(OCQualityOfService qos)
{
    OCStackResult result;

    std::ostringstream query;
    query << coapServerResource;

    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());
    // First POST operation (to create an Light instance)
    result = InvokeOCDoResource(query, &serverAddr, OC_REST_POST,
                               ((qos == OC_HIGH_QOS) ? OC_HIGH_QOS: OC_LOW_QOS),
                               postReqCB, NULL, 0);
    if (OC_STACK_OK != result)
    {
        // Error can happen if for example, network connectivity is down
        OIC_LOG(INFO, TAG, "First POST call did not succeed");
    }

    // Second POST operation (to create an Light instance)
    result = InvokeOCDoResource(query, &serverAddr, OC_REST_POST,
                               ((qos == OC_HIGH_QOS) ? OC_HIGH_QOS: OC_LOW_QOS),
                               postReqCB, NULL, 0);
    if (OC_STACK_OK != result)
    {
        OIC_LOG(INFO, TAG, "Second POST call did not succeed");
    }

    // This POST operation will update the original resourced /a/light
    return (InvokeOCDoResource(query, &serverAddr, OC_REST_POST,
                               ((qos == OC_HIGH_QOS) ? OC_HIGH_QOS: OC_LOW_QOS),
                               postReqCB, NULL, 0));
}

void* RequestDeleteDeathResourceTask(void* myqos)
{
    sleep (30);//long enough to give the server time to finish deleting the resource.
    std::ostringstream query;
    query << coapServerResource;

    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());

    // Second DELETE operation to delete the resource that might have been removed already.
    OCQualityOfService qos;
    if (myqos == NULL)
    {
        qos = OC_LOW_QOS;
    }
    else
    {
        qos = OC_HIGH_QOS;
    }

    OCStackResult result = InvokeOCDoResource(query, &serverAddr, OC_REST_DELETE,
                               qos,
                               deleteReqCB, NULL, 0);

    if (OC_STACK_OK != result)
    {
        OIC_LOG(INFO, TAG, "Second DELETE Request also failed");
    }
    else
    {
        OIC_LOG(INFO, TAG, "Second DELETE Request sent successfully; Waiting for Callback");
    }

    return NULL;
}

int InitDeleteRequest(OCQualityOfService qos)
{
    OCStackResult result;
    std::ostringstream query;
    query << coapServerResource;

    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());

    // First DELETE operation
    result = InvokeOCDoResource(query, &serverAddr, OC_REST_DELETE,
                               qos,
                               deleteReqCB, NULL, 0);
    if (OC_STACK_OK != result)
    {
        // Error can happen if for example, network connectivity is down
        OIC_LOG(INFO, TAG, "DELETE Request did not succeed; Will try again.");
        //Create a thread to delete this resource again
        pthread_t threadId;
        pthread_create (&threadId, NULL, RequestDeleteDeathResourceTask, (void*)qos);
    }
    else
    {
        OIC_LOG(INFO, TAG, "DELETE Request sent successfully; Waiting for Callback");
    }
    return result;
}

int InitGetRequest(OCQualityOfService qos, uint8_t withVendorSpecificHeaderOptions,
                   bool getWithQuery)
{

    OCHeaderOption options[MAX_HEADER_OPTIONS];

    std::ostringstream query;
    query << coapServerResource;

    // ocserver is written to only process "power<X" query.
    if (getWithQuery)
    {
        OIC_LOG(INFO, TAG, "Using query power<50");
        query << "?power<50";
    }
    OIC_LOG_V(INFO, TAG, "\nExecuting %s with query %s", __func__, query.str().c_str());

    if (withVendorSpecificHeaderOptions)
    {
        memset(options, 0, sizeof(OCHeaderOption)* MAX_HEADER_OPTIONS);
        size_t numOptions = 0;
        uint8_t option0[] = { 16, 39 };
        uint16_t optionID = COAP_OPTION_ACCEPT;
        size_t optionDataSize = sizeof(option0);
        OCSetHeaderOption(options,
                          &numOptions,
                          optionID,
                          option0,
                          optionDataSize);

        uint8_t option1[] = { 0, 8 };
        optionID = COAP_OPTION_ACCEPT_VERSION;
        optionDataSize = sizeof(option1);
        OCSetHeaderOption(options,
                          &numOptions,
                          optionID,
                          option1,
                          optionDataSize);
    }
    if (withVendorSpecificHeaderOptions)
    {
        return (InvokeOCDoResource(query, &serverAddr, OC_REST_GET,
                (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS, getReqCB, options, 2));
    }
    else
    {
        return (InvokeOCDoResource(query, &serverAddr, OC_REST_GET,
                (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS, getReqCB, NULL, 0));
    }
}

int InitPlatformDiscovery(OCQualityOfService qos)
{
    OIC_LOG_V(INFO, TAG, "\n\nExecuting %s", __func__);

    OCStackResult ret;
    OCCallbackData cbData;
    char szQueryUri[MAX_QUERY_LENGTH] = { 0 };

    snprintf(szQueryUri, sizeof (szQueryUri) - 1, PLATFORM_DISCOVERY_QUERY, discoveryAddr);

    cbData.cb = PlatformDiscoveryReqCB;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    ret = OCDoRequest(NULL, OC_REST_DISCOVER, szQueryUri, NULL, 0, CT_DEFAULT,
                      (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS,
                      &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack device error");
    }

    return ret;
}

int InitDeviceDiscovery(OCQualityOfService qos)
{
    OIC_LOG_V(INFO, TAG, "\n\nExecuting %s", __func__);

    OCStackResult ret;
    OCCallbackData cbData;
    char szQueryUri[MAX_QUERY_LENGTH] = { 0 };

    snprintf(szQueryUri, sizeof (szQueryUri) - 1, DEVICE_DISCOVERY_QUERY, discoveryAddr);

    cbData.cb = DeviceDiscoveryReqCB;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    ret = OCDoRequest(NULL, OC_REST_DISCOVER, szQueryUri, NULL, 0, CT_DEFAULT,
                      (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS,
                      &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack device error");
    }

    return ret;
}

int InitDiscovery(OCQualityOfService qos, uint8_t withVendorSpecificHeaderOptions)
{
    OCStackResult ret;
    OCCallbackData cbData;
    char szQueryUri[MAX_QUERY_LENGTH] = { 0 };

    snprintf(szQueryUri, sizeof (szQueryUri) - 1, RESOURCE_DISCOVERY_QUERY, discoveryAddr);

    cbData.cb = discoveryReqCB;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    if (withVendorSpecificHeaderOptions)
    {
        OCHeaderOption options[MAX_HEADER_OPTIONS];
        memset(options, 0, sizeof(OCHeaderOption) * MAX_HEADER_OPTIONS);
        size_t numOptions = 0;

        uint8_t format = COAP_MEDIATYPE_APPLICATION_CBOR;
        uint16_t optionID = CA_OPTION_ACCEPT;
        OCSetHeaderOption(options, &numOptions, optionID, &format, sizeof(format));

        ret = OCDoRequest(NULL, OC_REST_DISCOVER, szQueryUri, NULL, 0, CT_DEFAULT,
                              (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS,
                              &cbData, options, 2);
    }
    else
    {
        ret = OCDoRequest(NULL, OC_REST_DISCOVER, szQueryUri, NULL, 0, CT_DEFAULT,
                           (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS,
                           &cbData, NULL, 0);
    }
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }
    return ret;
}

int InitGetRequestWithCoap(OCDiscoveryPayload* dis, bool isUdp)
{
    if (!dis)
    {
        OIC_LOG(INFO, TAG, "Given payload is NULL!!!");
        return -1;
    }

    // copy query
    std::ostringstream query;
    query << coapServerResource;

    // server addr
    OCDevAddr dev;
    memset(&dev, 0, sizeof(dev));
    dev.adapter = OC_DEFAULT_ADAPTER;

    // find endpoint with ipv4, UDP or TCP
    OCResourcePayload* res = dis->resources;
    while (res)
    {
        OCEndpointPayload* eps = res->eps;
        while (eps)
        {
            if (strcmp(eps->tps, (isUdp ? COAP_UDP : COAP_TCP)) == 0 &&
                strlen(eps->addr) < MAX_LENGTH_IPv4_ADDR)
            {
                OIC_LOG_V(INFO, TAG, "%s found!!!", (isUdp ? COAP_UDP : COAP_TCP));
                dev.adapter = (isUdp ? OC_ADAPTER_IP : OC_ADAPTER_TCP);
                dev.flags = OC_IP_USE_V4;
                dev.port = eps->port;
                memcpy(dev.addr, eps->addr, sizeof(dev.addr));
            }
            eps = eps->next;
        }
        res = res->next;
    }

    if (dev.adapter == (isUdp ? OC_ADAPTER_IP : OC_ADAPTER_TCP) && dev.flags == OC_IP_USE_V4)
    {
        OIC_LOG_V(INFO, TAG, "dev addr is %s", dev.addr);
        OIC_LOG_V(INFO, TAG, "dev port is %d", dev.port);
        OIC_LOG_V(INFO, TAG, "dev flags is %d", dev.flags);
        OIC_LOG_V(INFO, TAG, "dev adapter is %d", dev.adapter);

        // send ocdoresource
        return (InvokeOCDoResource(query, &dev, OC_REST_GET,
                OC_LOW_QOS, getReqCB, NULL, 0));
    }
    else
    {
        OIC_LOG(INFO, TAG, "Endpoints infomation not found on given payload!!!");
        return -1;
    }
}

void showEndpointsInfo(OCResourcePayload* res)
{
    if (!res)
    {
        OIC_LOG(INFO, TAG, "No endpoints information in given payload");
        return;
    }

    if (!res->eps)
    {
        OIC_LOG(INFO, TAG, "No endpoints information in given payload");
        return;
    }

    OCEndpointPayload* eps = res->eps;

    while (eps)
    {
        if (eps->family == OC_IP_USE_V6)
        {
            OIC_LOG_V(INFO, TAG, "Resource [%s] has endpoint [%s://[%s]:%d]",
                      res->uri, eps->tps, eps->addr, eps->port);
        }
        else if (eps->family == OC_IP_USE_V4)
        {
            OIC_LOG_V(INFO, TAG, "Resource [%s] has endpoint [%s://%s:%d]",
                      res->uri, eps->tps, eps->addr, eps->port);
        }
        else
        {
            OIC_LOG_V(INFO, TAG, "Resource [%s] has endpoint [%s://%s]",
                      res->uri, eps->tps, eps->addr);
        }

        eps = eps->next;
    }
}

OCStackResult SetDeviceInfo()
{
    OCResourceHandle resourceHandle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
    if (resourceHandle == NULL)
    {
        OIC_LOG(ERROR, TAG, "Device Resource does not exist.");
        goto exit;
    }

    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, gDeviceName));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION, gDataModelVersions));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID, gProtocolIndependentID));

    if (1 == OCFSpecVersion)
    {
        VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, gSpecVersion));
    }

    OIC_LOG(INFO, TAG, "Device information initialized successfully.");
    return OC_STACK_OK;

exit:
    return OC_STACK_ERROR;
}

OCStackResult SetPlatformInfo()
{
    OCResourceHandle resourceHandle = OCGetResourceHandleAtUri(OC_RSRVD_PLATFORM_URI);
    if (resourceHandle == NULL)
    {
        OIC_LOG(ERROR, TAG, "Platform Resource does not exist.");
        goto exit;
    }

    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_ID, gPlatformID));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_NAME, gManufacturerName));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_URL, gManufacturerUrl));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MODEL_NUM, gModelNumber));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_DATE, gDateOfManufacture));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_VERSION, gPlatformVersion));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_OS_VERSION, gOperatingSystemVersion));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_HARDWARE_VERSION, gHardwareVersion));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_FIRMWARE_VERSION, gFirmwareVersion));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SUPPORT_URL, gSupportUrl));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SYSTEM_TIME, gSystemTime));

    OIC_LOG(INFO, TAG, "Platform information initialized successfully.");
    return OC_STACK_OK;

exit:
    return OC_STACK_ERROR;
}

static FILE* server_fopen(const char* path, const char* mode)
{
    return fopen(path, mode);
}

#ifdef SECURED
void OC_CALL DisplayPinCB(char *pin, size_t pinSize, void *context)
{
    OC_UNUSED(context);

    if ((nullptr == pin) || (0 == pinSize))
    {
        OIC_LOG(INFO, TAG, "Invalid PIN");
        return;
    }

    OIC_LOG(INFO, TAG, "============================");
    OIC_LOG_V(INFO, TAG, "    PIN CODE : %s", pin);
    OIC_LOG(INFO, TAG, "============================");
}

void OC_CALL ClosePinDisplayCB(void)
{
    OIC_LOG(INFO, TAG, "============================");
    OIC_LOG(INFO, TAG, "    PIN DISPLAY CLOSED.");
    OIC_LOG(INFO, TAG, "============================");
}
#endif

int main(int argc, char* argv[])
{
    int opt;
    OCPersistentStorage ps{ server_fopen, fread, fwrite, fclose, unlink };

    while ((opt = getopt(argc, argv, "u:t:c:i:s:")) != -1)
    {
        switch(opt)
        {
            case 'u':
                UnicastDiscovery = atoi(optarg);
                break;
            case 't':
                TestCase = atoi(optarg);
                break;
            case 'c':
                Connectivity = atoi(optarg);
                break;
            case 'i':
                Introspection = atoi(optarg);
                break;
            case 's':
                OCFSpecVersion = atoi(optarg);
                break;
            default:
                PrintUsage();
                return -1;
        }
    }

    if (((UnicastDiscovery != 0) && (UnicastDiscovery != 1)) ||
            ((TestCase < TEST_DISCOVER_REQ) || (TestCase >= MAX_TESTS)) ||
            ((Connectivity < CT_ADAPTER_DEFAULT) || (Connectivity >= MAX_CT)) ||
            ((OCFSpecVersion != 0) && (OCFSpecVersion != 1)))
    {
        PrintUsage();
        return -1;
    }

    if (OC_STACK_OK != OCRegisterPersistentStorageHandler(&ps))
    {
        OIC_LOG(ERROR, TAG, "OCRegisterPersistentStorageHandler error");
        return 0;
    }

    if (OCInit1(OC_CLIENT_SERVER, OC_DEFAULT_FLAGS, OC_DEFAULT_FLAGS) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack init error");
        return 0;
    }

#ifdef SECURED
    // Set callbacks for handling pin display
    if (OC_STACK_OK != SetDisplayPinWithContextCB(DisplayPinCB, NULL))
    {
        OIC_LOG(ERROR, TAG, "Failed to set display pin callback");
        return 0;
    }

    SetClosePinDisplayCB(ClosePinDisplayCB);

    // Specify the type and length of the pin that will be generated upon request
    if (OC_STACK_OK != SetRandomPinPolicy(8, NUM_PIN))
    {
        OIC_LOG(ERROR, TAG, "Failed to set PIN policy");
        return 0;
    }
#endif

    if (OC_STACK_OK != SetDeviceInfo())
    {
        OIC_LOG(ERROR, TAG, "SetDeviceInfo failed");
        return 0;
    }

    if (OC_STACK_OK != SetPlatformInfo())
    {
        OIC_LOG(ERROR, TAG, "SetPlatformInfo failed");
        return 0;
    }

#ifdef ROUTING_GATEWAY
    /*
     * Before invoking Discover resource, we process the gateway requests
     * and form the routing table.
     */
    for (int index = 0; index < MAX_NUM_GATEWAY_REQUEST; index++)
    {
        if (OC_STACK_OK != OCProcess())
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return 0;
        }
        usleep(SLEEP_DURATION);
    }
#endif
    if (Connectivity == CT_ADAPTER_DEFAULT || Connectivity == CT_IP)
    {
        ConnType = CT_ADAPTER_IP;
    }
    else
    {
        OIC_LOG(INFO, TAG, "Default Connectivity type selected...");
        PrintUsage();
    }

    discoveryAddr[0] = '\0';

    if (UnicastDiscovery)
    {
        OIC_LOG(INFO, TAG, "Enter IP address of server with optional port number");
        OIC_LOG(INFO, TAG, "IPv4: 192.168.0.15:45454\n");
        OIC_LOG(INFO, TAG, "IPv6: [fe80::20c:29ff:fe1b:9c5]:45454\n");

        if (fgets(discoveryAddr, sizeof(discoveryAddr), stdin))
        {
            //Strip newline char from ipv4addr
            StripNewLineChar(discoveryAddr);
        }
        else
        {
            OIC_LOG(ERROR, TAG, "!! Bad input for IP address. !!");
            return OC_STACK_INVALID_PARAM;
        }
    }

    if (UnicastDiscovery == 0 && TestCase == TEST_DISCOVER_DEV_REQ)
    {
        InitDeviceDiscovery(OC_LOW_QOS);
    }
    else if (UnicastDiscovery == 0 && TestCase == TEST_DISCOVER_PLATFORM_REQ)
    {
        InitPlatformDiscovery(OC_LOW_QOS);
    }
    else
    {
        InitDiscovery(OC_LOW_QOS, 0);
    }

    // Break from loop with Ctrl+C
    OIC_LOG(INFO, TAG, "Entering occlient main loop...");
    signal(SIGINT, handleSigInt);
    while (!gQuitFlag)
    {

        if (OCProcess() != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return 0;
        }
#ifndef ROUTING_GATEAWAY
        sleep(1);
#endif
    }
    OIC_LOG(INFO, TAG, "Exiting occlient main loop...");

    if (OCStop() != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack stop error");
    }

    return 0;
}

std::string getConnectivityType (OCConnectivityType connType)
{
    switch (connType & CT_MASK_ADAPTER)
    {
        case CT_ADAPTER_IP:
            return "IP";

        case CT_IP_USE_V4:
            return "IPv4";

        case CT_IP_USE_V6:
            return "IPv6";

        case CT_ADAPTER_GATT_BTLE:
            return "GATT";

        case CT_ADAPTER_RFCOMM_BTEDR:
            return "RFCOMM";

        default:
            return "Incorrect connectivity";
    }
}
