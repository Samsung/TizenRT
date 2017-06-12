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

extern "C"
{
    #include "rd_client.h"
    #include "rd_server.h"
    #include "ocstack.h"
    #include "logger.h"
    #include "oic_malloc.h"
}

#include "gtest/gtest.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <stdint.h>

#include "gtest_helper.h"

using namespace std;

namespace itst = iotivity::test;

#define DEFAULT_CONTEXT_VALUE 0x99

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------
static const char TAG[] = "RDTests";

std::chrono::seconds const SHORT_TEST_TIMEOUT = std::chrono::seconds(5);

//-----------------------------------------------------------------------------
// Callback functions
//-----------------------------------------------------------------------------
#if (defined(RD_SERVER) && defined(RD_CLIENT))
static OCStackApplicationResult handleDiscoveryCB(__attribute__((unused))void *ctx,
                                                  __attribute__((unused)) OCDoHandle handle,
                                                  __attribute__((unused))
                                                  OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "Successfully found RD.");

    return OC_STACK_DELETE_TRANSACTION;
}
#endif

#ifdef RD_CLIENT
static OCStackApplicationResult handlePublishCB(__attribute__((unused))void *ctx,
                                                __attribute__((unused)) OCDoHandle handle,
                                                __attribute__((unused))
                                                OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "Successfully published resources.");

    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handleDeleteCB(__attribute__((unused))void *ctx,
                                               __attribute__((unused)) OCDoHandle handle,
                                               __attribute__((unused))
                                               OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "Successfully delete resources.");

    return OC_STACK_DELETE_TRANSACTION;
}
#endif

//-----------------------------------------------------------------------------
// Entity handler
//-----------------------------------------------------------------------------
OCEntityHandlerResult rdEntityHandler(OCEntityHandlerFlag /*flag*/,
                                      OCEntityHandlerRequest * /*entityHandlerRequest*/,
                                      void* /*callbackParam*/)
{
    OIC_LOG(INFO, TAG, "Entering entityHandler");

    return OC_EH_OK;
}

//-----------------------------------------------------------------------------
//  Tests
//-----------------------------------------------------------------------------
class RDTests : public testing::Test {
    protected:
    virtual void SetUp()
    {
        OCInit("127.0.0.1", 5683, OC_CLIENT_SERVER);
    }

    virtual void TearDown()
    {
        OCStop();
    }
};

#if (defined(RD_SERVER) && defined(RD_CLIENT))
TEST_F(RDTests, CreateRDResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCRDStart());

    OCCallbackData cbData;
    cbData.cb = &handleDiscoveryCB;;
    cbData.cd = NULL;
    cbData.context = (void*) DEFAULT_CONTEXT_VALUE;
    EXPECT_EQ(OC_STACK_OK, OCRDDiscover(NULL, CT_ADAPTER_IP, &cbData, OC_LOW_QOS));

    EXPECT_EQ(OC_STACK_OK, OCRDStop());
}
#endif

#ifdef RD_CLIENT
// TEST_F(RDTests, CreateRDPayload)
// {
//     itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
//
//     OCResourceHandle handle;
//     EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle, "core.light",
//                                             "oic.if.baseline", "/a/light", rdEntityHandler,
//                                             NULL, OC_DISCOVERABLE | OC_OBSERVABLE));
//
//     // Create RD Publish Payload.
//     // OCRDPayload *rdPayload = (OCRDPayload *) OCRDPublishPayloadCreate(&handle, 1, 86400);
//     // EXPECT_TRUE(rdPayload != NULL);
//
//     // Cleanup.
//     // OCPayloadDestroy(rdPayload);
// }
//
// TEST_F(RDTests, ConvertParseRDPayload)
// {
//     itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
//
//     OCResourceHandle handle;
//     EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle, "core.light",
//                                             "oic.if.baseline", "/a/light", rdEntityHandler,
//                                             NULL, OC_DISCOVERABLE | OC_OBSERVABLE));
//
//     // Create RD Publish Payload.
//     OCRDPayload *rdPayload = (OCRDPayload *) OCRDPublishPayloadCreate(&handle, 1, 86400);
//     EXPECT_TRUE(rdPayload != NULL);
//
//     if (rdPayload)
//     {
//         // Convert RD Publish Payload to CBOR.
//         size_t curSize = 255;
//         uint8_t *outPayload = (uint8_t *)OICCalloc(1, curSize);
//         OCRDPayloadToCbor(rdPayload, outPayload, &curSize);
//
//         // Parse CBOR back to RD Publish Payload.
//         CborParser parser;
//         CborValue rootValue;
//         cbor_parser_init(outPayload, curSize, 0, &parser, &rootValue);
//
//         OCRDPayload *rdPayload = NULL;
//         EXPECT_EQ(OC_STACK_OK, OCRDCborToPayload(&rootValue, (OCPayload**) &rdPayload));
//
//         // Cleanup.
//         OICFree(outPayload);
//     }
//     OCRDPayloadDestroy(rdPayload);
// }

TEST_F(RDTests, RDPublishResourceNullAddr)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_INVALID_IP, OCRDPublish(NULL, 0, CT_ADAPTER_IP, nullptr, 0, 0, OC_LOW_QOS));
}

TEST_F(RDTests, RDPublishResourceNullCB)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, OCRDPublish(NULL, "127.0.0.1", CT_ADAPTER_IP, nullptr,
                                                     0, 0, OC_LOW_QOS));
}

TEST_F(RDTests, RDPublishResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCCallbackData cbData;
    cbData.cb = &handlePublishCB;
    cbData.cd = NULL;
    cbData.context = (void*) DEFAULT_CONTEXT_VALUE;

    OCResourceHandle handle;

    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle, "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));

    EXPECT_EQ(OC_STACK_OK, OCRDPublish(NULL, "127.0.0.1", CT_ADAPTER_IP, &handle,
                                       1, &cbData, OC_LOW_QOS));
}

TEST_F(RDTests, RDPublishMultipleResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCCallbackData cbData;
    cbData.cb = &handlePublishCB;
    cbData.cd = NULL;
    cbData.context = (void*) DEFAULT_CONTEXT_VALUE;

    OCResourceHandle handles[2];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));

    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));

    EXPECT_EQ(OC_STACK_OK, OCRDPublish(NULL, "127.0.0.1", CT_ADAPTER_IP, handles,
                                       2, &cbData, OC_LOW_QOS));
}

TEST_F(RDTests, RDDeleteResourceNullAddr)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_INVALID_IP, OCRDDelete(NULL, 0, CT_ADAPTER_IP, nullptr, 0, 0, OC_LOW_QOS));
}

TEST_F(RDTests, RDDeleteResourceNullCB)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, OCRDDelete(NULL, "127.0.0.1", CT_ADAPTER_IP, nullptr,
                                                    0, 0, OC_LOW_QOS));
}

TEST_F(RDTests, RDDeleteAllResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCCallbackData cbData;
    cbData.cb = &handleDeleteCB;
    cbData.cd = NULL;
    cbData.context = (void*) DEFAULT_CONTEXT_VALUE;

    EXPECT_EQ(OC_STACK_OK, OCRDDelete(NULL, "127.0.0.1", CT_ADAPTER_IP, nullptr, 0, &cbData,
                                      OC_LOW_QOS));
}

TEST_F(RDTests, RDDeleteSpecificResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCCallbackData cbData;
    cbData.cb = &handleDeleteCB;
    cbData.cd = NULL;
    cbData.context = (void*) DEFAULT_CONTEXT_VALUE;

    OCResourceHandle handle;

    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle, "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, OC_DISCOVERABLE | OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCRDDelete(NULL, "127.0.0.1", CT_ADAPTER_IP, &handle,
                                      1, &cbData, OC_LOW_QOS));
}
#endif
