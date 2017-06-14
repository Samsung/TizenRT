//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include <iostream>
#include "octypes.h"
#include "ocstack.h"
#include "payload_logging.h"

bool g_foundResource = true;
static int count = 0;
#define DEFAULT_CONTEXT_VALUE       (0x99)

OCStackApplicationResult foundResource(void* ctx,
                OCDoHandle handle,
                OCClientResponse *clientResponse)
{
    (void)handle;
    (void) ctx;
    if (clientResponse == NULL)
    {
        std::cout << "foundResource received NULL clientResponse" << std::endl;
        return   OC_STACK_DELETE_TRANSACTION;
    }

    std::cout << "Found resource response." << std::endl;
    OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
    count++;

    if (count == 3)
    {
        g_foundResource = false;
        exit(0);
    }
    return OC_STACK_KEEP_TRANSACTION;
}

OCStackResult findResource(std::string str, OCClientResponseHandler cb)
{
    OCCallbackData cbData;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
    cbData.cb = cb;

    return OCDoResource(nullptr, OC_REST_DISCOVER,
             str.c_str(), nullptr, nullptr, CT_DEFAULT,
             static_cast<OCQualityOfService>(OC_LOW_QOS),
             &cbData, nullptr, 0);
}

int main()
{
    bool sendRequest = true;
    std::cout << "Initializing IoTivity Platform" << std::endl;
    OCStackResult result = OCInit(NULL, 0, OC_CLIENT_SERVER);
    if (result != OC_STACK_OK)
    {
        std::cout << "OCInit Failed" << result << std::endl;
        return -1;
    }

    std::cout << "Created Platform..." << std::endl;

    while (g_foundResource)
    {
        if (sendRequest)
        {
            sendRequest = false;
            std::cout << "Finding Resource light" << std::endl;
            if (OC_STACK_OK != findResource("/oic/res?rt=core.light", foundResource))
            {
                std::cout << "Exception finding resources : " << std::endl;
                sendRequest = true;
            }
            if (OC_STACK_OK != findResource("/oic/res?if=oic.if.baseline", foundResource))
            {
                std::cout << "Exception finding resources : " << std::endl;
                sendRequest = true;
            }
            if (OC_STACK_OK != findResource("/oic/res", foundResource))
            {
                std::cout << "Exception finding resources : " << std::endl;
                sendRequest = true;
            }
        }
    }
}
