//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
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
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <getopt.h>

#include "ocstack.h"
#include "logger.h"
#include "occlientbasicops.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "common.h"

#define MAX_IP_ADDR_ST_SZ  16 //string size of "155.255.255.255" (15 + 1)
#define MAX_PORT_ST_SZ  6     //string size of "65535" (5 + 1)

static int UnicastDiscovery = 0;
static int TestCase = 0;
static int Connectivity = 0;

//The following variable determines the interface protocol (IP, etc)
//to be used for sending unicast messages. Default set to IP.
static OCConnectivityType ConnType = CT_ADAPTER_IP;
static const char *RESOURCE_DISCOVERY_QUERY = "%s/oic/res";

int gQuitFlag = 0;

struct ResourceNode *resourceList;
/*
 * SIGINT handler: set gQuitFlag to 1 for graceful termination
 * */
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

    if(!payload)
    {
        std::cout << "Failed to create put payload object"<<std::endl;
        std::exit(1);
    }

    OCRepPayloadSetPropInt(payload, "power", 15);
    OCRepPayloadSetPropBool(payload, "state", true);

    return (OCPayload*) payload;
}

static void PrintUsage()
{
    OIC_LOG(INFO, TAG, "Usage : occlient -u <0|1> -t <1|2|3> -c <0|1>");
    OIC_LOG(INFO, TAG, "-u <0|1> : Perform multicast/unicast discovery of resources");
    OIC_LOG(INFO, TAG, "-t 1 : Discover Resources");
    OIC_LOG(INFO, TAG, "-t 2 : Discover Resources and"
            " Initiate Nonconfirmable Get/Put/Post Requests");
    OIC_LOG(INFO, TAG, "-t 3 : Discover Resources and Initiate "
            "Confirmable Get/Put/Post Requests");
    OIC_LOG(INFO, TAG, "-c 0 : Default auto-selection");
    OIC_LOG(INFO, TAG, "-c 1 : IP Connectivity Type");
}

/*
 * Returns the first resource in the list
 */
const ResourceNode * getResource()
{
    return resourceList;
}

OCStackResult InvokeOCDoResource(std::ostringstream &query,
                                 OCMethod method,
                                 const OCDevAddr *dest,
                                 OCQualityOfService qos,
                                 OCClientResponseHandler cb,
                                 OCHeaderOption * options, uint8_t numOptions)
{
    OCStackResult ret;
    OCCallbackData cbData;

    cbData.cb = cb;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OCPayload* payload = (method == OC_REST_PUT || method == OC_REST_POST) ? putPayload() : NULL;

    ret = OCDoRequest(NULL, method, query.str().c_str(), dest,
                      payload, CT_DEFAULT, qos, &cbData, options, numOptions);

    OCPayloadDestroy(payload);

    if (ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoResource returns error %d with method %d",
                 ret, method);
    }

    return ret;
}

OCStackApplicationResult putReqCB(void* ctx, OCDoHandle /*handle*/,
                                  OCClientResponse * clientResponse)
{
    if(ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "<====Callback Context for PUT received successfully====>");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "<====Callback Context for PUT fail====>");
    }

    if(clientResponse)
    {
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, ("=============> Put Response"));
    }
    else
    {
        OIC_LOG(ERROR, TAG, "<====PUT Callback fail to receive clientResponse====>\n");
    }
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult postReqCB(void *ctx, OCDoHandle /*handle*/,
                                   OCClientResponse *clientResponse)
{
    if(ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "<====Callback Context for POST received successfully====>");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "<====Callback Context for POST fail====>");
    }

    if(clientResponse)
    {
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, ("=============> Post Response"));
    }
    else
    {
        OIC_LOG(ERROR, TAG, "<====POST Callback fail to receive clientResponse====>\n");
    }

    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult getReqCB(void* ctx, OCDoHandle /*handle*/,
                                  OCClientResponse * clientResponse)
{
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "<====Callback Context for GET received successfully====>");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "<====Callback Context for GET fail====>");
    }

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s",  getResult(clientResponse->result));
        OIC_LOG_V(INFO, TAG, "SEQUENCE NUMBER: %d", clientResponse->sequenceNumber);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OIC_LOG(INFO, TAG, ("=============> Get Response"));

        if (clientResponse->numRcvdVendorSpecificHeaderOptions > 0 )
        {
            OIC_LOG (INFO, TAG, "Received vendor specific options");
            uint8_t i = 0;
            OCHeaderOption * rcvdOptions = clientResponse->rcvdVendorSpecificHeaderOptions;
            for (i = 0; i < clientResponse->numRcvdVendorSpecificHeaderOptions; i++)
            {
                if (((OCHeaderOption) rcvdOptions[i]).protocolID == OC_COAP_ID)
                {
                    OIC_LOG_V(INFO, TAG, "Received option with OC_COAP_ID and ID %u with",
                            ((OCHeaderOption)rcvdOptions[i]).optionID );

                    OIC_LOG_BUFFER(INFO, TAG, ((OCHeaderOption)rcvdOptions[i]).optionData,
                        MAX_HEADER_OPTION_DATA_LENGTH);
                }
            }
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "<====GET Callback fail to receive clientResponse====>\n");
    }
    return OC_STACK_DELETE_TRANSACTION;
}

/*
 * This is a function called back when a device is discovered
 */
OCStackApplicationResult discoveryReqCB(void* ctx, OCDoHandle /*handle*/,
                                        OCClientResponse * clientResponse)
{
    if (ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG(INFO, TAG, "\n<====Callback Context for DISCOVERY query "
               "received successfully====>");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "\n<====Callback Context for DISCOVERY fail====>");
    }

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG,
                "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);

        collectUniqueResource(clientResponse);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "<====DISCOVERY Callback fail to receive clientResponse====>\n");
    }
    return (UnicastDiscovery) ?
           OC_STACK_DELETE_TRANSACTION : OC_STACK_KEEP_TRANSACTION ;
}

int InitPutRequest(OCQualityOfService qos)
{
    std::ostringstream query;
    //Get most recently inserted resource
    const ResourceNode * resource  = getResource();

    if(!resource)
    {
        OIC_LOG_V(ERROR, TAG, "Resource null, can't do PUT request\n");
        return -1;
    }
    query << resource->uri;
    OIC_LOG_V(INFO, TAG,"Executing InitPutRequest, Query: %s", query.str().c_str());

    return (InvokeOCDoResource(query, OC_REST_PUT, &resource->endpoint,
           ((qos == OC_HIGH_QOS) ? OC_HIGH_QOS: OC_LOW_QOS),
            putReqCB, NULL, 0));
}

int InitPostRequest(OCQualityOfService qos)
{
    OCStackResult result;
    std::ostringstream query;
    //Get most recently inserted resource
    const ResourceNode *resource  = getResource();

    if(!resource)
    {
        OIC_LOG_V(ERROR, TAG, "Resource null, can't do POST request\n");
        return -1;
    }

    query << resource->uri;
    OIC_LOG_V(INFO, TAG,"Executing InitPostRequest, Query: %s", query.str().c_str());

    // First POST operation (to create an LED instance)
    result = InvokeOCDoResource(query, OC_REST_POST, &resource->endpoint,
            ((qos == OC_HIGH_QOS) ? OC_HIGH_QOS: OC_LOW_QOS),
            postReqCB, NULL, 0);
    if (OC_STACK_OK != result)
    {
        // Error can happen if for example, network connectivity is down
        OIC_LOG(ERROR, TAG, "First POST call did not succeed");
    }

    // Second POST operation (to create an LED instance)
    result = InvokeOCDoResource(query, OC_REST_POST, &resource->endpoint,
            ((qos == OC_HIGH_QOS) ? OC_HIGH_QOS: OC_LOW_QOS),
            postReqCB, NULL, 0);
    if (OC_STACK_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Second POST call did not succeed");
    }

    // This POST operation will update the original resourced /a/led
    return (InvokeOCDoResource(query, OC_REST_POST, &resource->endpoint,
                ((qos == OC_HIGH_QOS) ? OC_HIGH_QOS: OC_LOW_QOS),
                postReqCB, NULL, 0));
}

int InitGetRequest(OCQualityOfService qos)
{
    std::ostringstream query;
    //Get most recently inserted resource
    const ResourceNode * resource  = getResource();

    if(!resource)
    {
        OIC_LOG_V(ERROR, TAG, "Resource null, can't do GET request\n");
        return -1;
    }
    query << resource->uri;
    OIC_LOG_V(INFO, TAG,"Executing InitGetRequest, Query: %s", query.str().c_str());

    return (InvokeOCDoResource(query, OC_REST_GET, &resource->endpoint,
            (qos == OC_HIGH_QOS)?OC_HIGH_QOS:OC_LOW_QOS, getReqCB, NULL, 0));
}

int InitDiscovery()
{
    OCStackResult ret;
    OCCallbackData cbData;
    char queryUri[200];
    char ipaddr[100] = { '\0' };

    if (UnicastDiscovery)
    {
        OIC_LOG(INFO, TAG, "Enter IP address (with optional port) of the Server hosting resource\n");
        OIC_LOG(INFO, TAG, "IPv4: 192.168.0.15:45454\n");
        OIC_LOG(INFO, TAG, "IPv6: [fe80::20c:29ff:fe1b:9c5]:45454\n");

        if (fgets(ipaddr, sizeof (ipaddr), stdin))
        {
            StripNewLineChar(ipaddr); //Strip newline char from ipaddr
        }
        else
        {
            OIC_LOG(ERROR, TAG, "!! Bad input for IP address. !!");
            return OC_STACK_INVALID_PARAM;
        }
    }

    snprintf(queryUri, sizeof (queryUri), RESOURCE_DISCOVERY_QUERY, ipaddr);

    cbData.cb = discoveryReqCB;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    ret = OCDoRequest(NULL, OC_REST_DISCOVER, queryUri, 0, 0, CT_DEFAULT,
                      OC_LOW_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }
    return ret;
}

void queryResource()
{
    switch(TestCase)
    {
        case TEST_DISCOVER_REQ:
            break;
        case TEST_NON_CON_OP:
            InitGetRequest(OC_LOW_QOS);
            InitPutRequest(OC_LOW_QOS);
            InitPostRequest(OC_LOW_QOS);
            break;
        case TEST_CON_OP:
            InitGetRequest(OC_HIGH_QOS);
            InitPutRequest(OC_HIGH_QOS);
            InitPostRequest(OC_HIGH_QOS);
            break;
        default:
            PrintUsage();
            break;
    }
}


void collectUniqueResource(const OCClientResponse * clientResponse)
{
    OCDiscoveryPayload* pay = (OCDiscoveryPayload*) clientResponse->payload;
    OCResourcePayload* res = pay->resources;

    // Including the NUL terminator, length of UUID string of the form:
    //   "a62389f7-afde-00b6-cd3e-12b97d2fcf09"
#   define UUID_LENGTH 37

    char sidStr[UUID_LENGTH];

    while(res) {

        int ret = snprintf(sidStr, UUID_LENGTH,
                "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                pay->sid[0], pay->sid[1], pay->sid[2], pay->sid[3],
                pay->sid[4], pay->sid[5], pay->sid[6], pay->sid[7],
                pay->sid[8], pay->sid[9], pay->sid[10], pay->sid[11],
                pay->sid[12], pay->sid[13], pay->sid[14], pay->sid[15]
                );

        if (ret == UUID_LENGTH - 1)
        {
            if(insertResource(sidStr, res->uri, clientResponse) == 1)
            {
                OIC_LOG_V(INFO,TAG,"%s%s%s%s\n",sidStr, ":", res->uri, " is new");
                printResourceList();
                queryResource();
            }
            else {
                OIC_LOG_V(INFO,TAG,"%s%s%s%s\n",sidStr, ":", res->uri, " is old");
            }
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Could Not Retrieve the Server ID");
        }

        res = res->next;
    }
}

/* This function searches for the resource(sid:uri) in the ResourceList.
 * If the Resource is found in the list then it returns 0 else insert
 * the resource to front of the list and returns 1.
 */
int insertResource(const char * sid, char const * uri,
            const OCClientResponse * clientResponse)
{
    ResourceNode * iter = resourceList;
    char * sid_cpy =  OICStrdup(sid);
    char * uri_cpy = OICStrdup(uri);

    //Checking if the resource(sid:uri) is new
    while(iter)
    {
        if((strcmp(iter->sid, sid) == 0) && (strcmp(iter->uri, uri) == 0))
        {
            OICFree(sid_cpy);
            OICFree(uri_cpy);
            return 0;
        }
        else
        {
            iter = iter->next;
        }
    }

    //Creating new ResourceNode
    if((iter = (ResourceNode *) OICMalloc(sizeof(ResourceNode))))
    {
        iter->sid = sid_cpy;
        iter->uri = uri_cpy;
        iter->endpoint = clientResponse->devAddr;
        iter->next = NULL;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Memory not allocated to ResourceNode");
        OICFree(sid_cpy);
        OICFree(uri_cpy);
        return -1;
    }

    //Adding new ResourceNode to front of the ResourceList
    if(!resourceList)
    {
        resourceList = iter;
    }
    else
    {
        iter->next = resourceList;
        resourceList = iter;
    }
    return 1;
}

void printResourceList()
{
    ResourceNode * iter;
    iter = resourceList;
    OIC_LOG(INFO, TAG, "Resource List: ");
    while(iter)
    {
        OIC_LOG(INFO, TAG, "*****************************************************");
        OIC_LOG_V(INFO, TAG, "sid = %s",iter->sid);
        OIC_LOG_V(INFO, TAG, "uri = %s", iter->uri);
        OIC_LOG_V(INFO, TAG, "ip = %s", iter->endpoint.addr);
        OIC_LOG_V(INFO, TAG, "port = %d", iter->endpoint.port);
        switch (iter->endpoint.adapter)
        {
            case OC_ADAPTER_IP:
                OIC_LOG(INFO, TAG, "connType = Default (IPv4)");
                break;
            case OC_ADAPTER_GATT_BTLE:
                OIC_LOG(INFO, TAG, "connType = BLE");
                break;
            case OC_ADAPTER_RFCOMM_BTEDR:
                OIC_LOG(INFO, TAG, "connType = BT");
                break;
            default:
                OIC_LOG(INFO, TAG, "connType = Invalid connType");
                break;
        }
        OIC_LOG(INFO, TAG, "*****************************************************");
        iter = iter->next;
    }
}

void freeResourceList()
{
    OIC_LOG(INFO, TAG, "Freeing ResourceNode List");
    ResourceNode * temp;
    while(resourceList)
    {

        temp = resourceList;
        resourceList = resourceList->next;
        OICFree((void *)temp->sid);
        OICFree((void *)temp->uri);
        OICFree(temp);
    }
    resourceList = NULL;
}

int main(int argc, char* argv[])
{
    int opt;
    resourceList = NULL;
    while ((opt = getopt(argc, argv, "u:t:c:")) != -1)
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
            default:
                PrintUsage();
                return -1;
        }
    }

    if ((UnicastDiscovery != 0 && UnicastDiscovery != 1) ||
        (TestCase < TEST_DISCOVER_REQ || TestCase >= MAX_TESTS) ||
        (Connectivity < CT_ADAPTER_DEFAULT || Connectivity >= MAX_CT))
    {
        PrintUsage();
        return -1;
    }

    /* Initialize OCStack*/
    if (OCInit1(OC_CLIENT, OC_DEFAULT_FLAGS, OC_DEFAULT_FLAGS) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack init error");
        return 0;
    }

    if(Connectivity == CT_ADAPTER_DEFAULT || Connectivity ==  CT_IP)
    {
        ConnType =  CT_ADAPTER_IP;//CT_DEFAULT;
    }
    else
    {
        OIC_LOG(INFO, TAG, "Default Connectivity type selected");
        PrintUsage();
    }

    InitDiscovery();

    // Break from loop with Ctrl+C
    signal(SIGINT, handleSigInt);

    while (!gQuitFlag)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return 0;
        }
        sleep(2);
    }

    freeResourceList();
    OIC_LOG(INFO, TAG, "Exiting occlient main loop...");
    if (OCStop() != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack stop error");
    }
    return 0;
}


