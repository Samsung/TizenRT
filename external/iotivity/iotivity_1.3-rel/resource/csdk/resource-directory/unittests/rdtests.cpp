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
    #include "iotivity_config.h"
    #include "rd_client.h"
    #include "rd_server.h"
    #include "ocpayload.h"
    #include "ocstack.h"
    #include "logger.h"
    #include "oic_malloc.h"
    #include "oic_string.h"
    #include "payload_logging.h"
    #include "cacommon.h"
    #include "coap/pdu.h"
}

#include <gtest/gtest.h>
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
#define TAG "RDTests"

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
                                                OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "Successfully published resources.");

    EXPECT_EQ(OC_STACK_RESOURCE_CHANGED, clientResponse->result);

    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handleDeleteCB(__attribute__((unused))void *ctx,
                                               __attribute__((unused)) OCDoHandle handle,
                                               OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "Successfully delete resources.");

    EXPECT_EQ(OC_STACK_RESOURCE_DELETED, clientResponse->result);

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
    cbData.cb = &handleDiscoveryCB;
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
    EXPECT_EQ(OC_STACK_INVALID_IP, OCRDPublish(NULL, 0, CT_ADAPTER_IP, nullptr, 0, 0, 0, OC_LOW_QOS));
}

TEST_F(RDTests, RDPublishResourceNullCB)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, OCRDPublish(NULL, "127.0.0.1", CT_ADAPTER_IP, nullptr,
                                                     0, 0, 0, OC_LOW_QOS));
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
                                       1, OIC_RD_PUBLISH_TTL, &cbData, OC_LOW_QOS));
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
                                       2, OIC_RD_PUBLISH_TTL, &cbData, OC_LOW_QOS));
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

#ifdef RD_SERVER

static OCStackApplicationResult UpdateSelValueVerify(__attribute__((unused))void *ctx,
                                                     __attribute__((unused)) OCDoHandle handle,
                                                     OCClientResponse *clientResponse)
{
    EXPECT_GT(clientResponse->result, OC_STACK_RESOURCE_CHANGED);
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_F(RDTests, UpdateSelValue)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCRDStart());

    itst::Callback postCB(&UpdateSelValueVerify);
    OCRepPayload *payload = OCRepPayloadCreate();
    EXPECT_TRUE(payload != NULL);
    EXPECT_TRUE(OCRepPayloadSetPropInt(payload, "sel", 90));
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_POST, "127.0.0.1/oic/rd?if=oic.if.baseline", NULL,
            (OCPayload*) payload, CT_DEFAULT, OC_HIGH_QOS, postCB, NULL, 0));
    EXPECT_EQ(OC_STACK_OK, postCB.Wait(100));

    EXPECT_EQ(OC_STACK_OK, OCRDStop());
}

#endif

#if (defined(RD_SERVER) && defined(RD_CLIENT))
class RDDiscoverTests : public ::testing::TestWithParam<uint16_t>
{
    protected:
        virtual void SetUp()
        {
            remove("RD.db");
            numOptions = 0;
            uint16_t format = GetParam();
            OCSetHeaderOption(options, &numOptions, CA_OPTION_ACCEPT, &format, sizeof(format));
            EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT_SERVER));
            EXPECT_EQ(OC_STACK_OK, OCRDStart());
        }

        virtual void TearDown()
        {
            OCStop();
            remove("RD.db");
        }
    public:
        static const unsigned char *di[3];
        OCHeaderOption options[1];
        size_t numOptions;
};
const unsigned char *RDDiscoverTests::di[3] =
{
    (const unsigned char *) "7a960f46-a52e-4837-bd83-460b1a6dd56b",
    (const unsigned char *) "983656a7-c7e5-49c2-a201-edbeb7606fb5",
    (const unsigned char *) "9338c0b2-2373-4324-ba78-17c0ef79506d"
};

static bool EndsWith(const char *str, const char *suffix)
{
    size_t len = strlen(str);
    size_t suffixLen = strlen(suffix);
    if (len < suffixLen)
    {
        return false;
    }
    return !strcmp(&str[len - suffixLen], suffix);
}

static OCStackApplicationResult DiscoverAllResourcesVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        bool foundId = false;
        bool foundLight = false;
        bool foundLight2 = false;
        for (OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload; payload;
             payload = payload->next)
        {
            if (!strcmp((const char *) RDDiscoverTests::di[0], payload->sid))
            {
                foundId = true;
                for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
                {
                    if (EndsWith(resource->uri, "/a/light2"))
                    {
                        foundLight2 = true;
                    }
                    else if (EndsWith(resource->uri, "/a/light"))
                    {
                        foundLight = true;
                    }
                }
            }
        }
        EXPECT_TRUE(foundId);
        EXPECT_TRUE(foundLight);
        EXPECT_TRUE(foundLight2);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DiscoverAllResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[2];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles,
              2, OIC_RD_PUBLISH_TTL, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    itst::Callback discoverCB(&DiscoverAllResourcesVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res", NULL, 0, CT_DEFAULT,
                    OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult ResourceQueryMatchesLocalAndRemoteVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        // Verify that only resources with the queried type are present in the response
        for (OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload; payload;
             payload = payload->next)
        {
            EXPECT_TRUE(payload->resources != NULL);
            for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
            {
                EXPECT_TRUE(EndsWith(payload->resources->uri, "/a/light"));
                EXPECT_STREQ("core.light", payload->resources->types->value);
                EXPECT_TRUE(payload->resources->types->next == NULL);
            }
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, ResourceQueryMatchesLocalAndRemote)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[1];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles,
              1, OIC_RD_PUBLISH_TTL, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    itst::Callback discoverCB(&ResourceQueryMatchesLocalAndRemoteVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult ResourceQueryMatchesLocalOnlyVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_STREQ(OCGetServerInstanceIDString(), payload->sid);
        EXPECT_TRUE(payload->next == NULL);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, ResourceQueryMatchesLocalOnly)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[2];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light2",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              1, OIC_RD_PUBLISH_TTL, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    OIC_LOG(INFO, TAG, "Published");

    itst::Callback discoverCB(&ResourceQueryMatchesLocalOnlyVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult ResourceQueryMatchesRemoteOnlyVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_STREQ((const char *)RDDiscoverTests::di[0], payload->sid);
        EXPECT_TRUE(payload->next == NULL);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

static void SetStringArray(OCRepPayload *payload, const char *name, const char *value)
{
    size_t dim[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
    char **ss = (char **)OICMalloc(sizeof(char *) * 1);
    ss[0] = OICStrdup(value);
    OCRepPayloadSetStringArray(payload, name, (const char **)ss, dim);
}

TEST_P(RDDiscoverTests, ResourceQueryMatchesRemoteOnly)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[1];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCRepPayload *repPayload = OCRepPayloadCreate();
    ASSERT_TRUE(repPayload != NULL);
    EXPECT_TRUE(OCRepPayloadSetPropString(repPayload, OC_RSRVD_DEVICE_ID, (const char *)di[0]));
    EXPECT_TRUE(OCRepPayloadSetPropInt(repPayload, OC_RSRVD_DEVICE_TTL, 86400));
    OCDevAddr address;
    address.port = 54321;
    OICStrcpy(address.addr,MAX_ADDR_STR_SIZE, "192.168.1.1");

    std::string resourceURI_light = "/a/light";
    std::string resourceTypeName_light = "core.light2";

    const OCRepPayload *linkArr[1];
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};

    OCRepPayload *link = OCRepPayloadCreate();
    OCRepPayloadSetPropString(link, OC_RSRVD_HREF, resourceURI_light.c_str());
    SetStringArray(link, OC_RSRVD_RESOURCE_TYPE, resourceTypeName_light.c_str());
    SetStringArray(link, OC_RSRVD_INTERFACE, OC_RSRVD_INTERFACE_DEFAULT);
    OCRepPayloadSetPropInt(link, OC_RSRVD_INS, 0);
    SetStringArray(link, OC_RSRVD_MEDIA_TYPE, OC_MEDIA_TYPE_APPLICATION_JSON);
    OCRepPayload *policy = OCRepPayloadCreate();
    OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP, OC_DISCOVERABLE);
    OCRepPayloadSetPropObjectAsOwner(link, OC_RSRVD_POLICY, policy);
    linkArr[0] = link;

    OCRepPayloadSetPropObjectArray(repPayload, OC_RSRVD_LINKS, linkArr, dimensions);

    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *)repPayload);

    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_POST, "127.0.0.1/oic/rd", NULL,
                    (OCPayload *)repPayload, CT_DEFAULT, OC_HIGH_QOS, publishCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    itst::Callback discoverCB(&ResourceQueryMatchesRemoteOnlyVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light2", NULL, 0,
                    CT_DEFAULT,OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DatabaseHas0ResourceQueryMatchesVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_TRUE(payload->next == NULL);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DatabaseHas0ResourceQueryMatches)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[4];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light2",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light3",
                                            "oic.if.baseline", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[3], "core.light4",
                                            "oic.if.baseline", "/a/light4", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publish0CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              1, OIC_RD_PUBLISH_TTL, publish0CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish0CB.Wait(100));
    itst::Callback publish1CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[1], CT_ADAPTER_IP, &handles[2],
              1, OIC_RD_PUBLISH_TTL, publish1CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish1CB.Wait(100));
    itst::Callback publish2CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[2], CT_ADAPTER_IP, &handles[3],
              1, OIC_RD_PUBLISH_TTL, publish2CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish2CB.Wait(100));

    itst::Callback discoverCB(&DatabaseHas0ResourceQueryMatchesVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light", NULL, 0,
                    CT_DEFAULT,OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DatabaseHas1ResourceQueryMatchVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_TRUE(payload->next != NULL);
        if (payload->next)
        {
            payload = payload->next;
            EXPECT_TRUE(payload->resources != NULL);
            for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
            {
                EXPECT_TRUE(EndsWith(payload->resources->uri, "/a/light2"));
                EXPECT_STREQ("core.light2", payload->resources->types->value);
                EXPECT_TRUE(payload->resources->types->next == NULL);
            }
            payload = payload->next;
            EXPECT_TRUE(payload == NULL);
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DatabaseHas1ResourceQueryMatch)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[4];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light2",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light3",
                                            "oic.if.baseline", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[3], "core.light4",
                                            "oic.if.baseline", "/a/light4", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publish0CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              1, OIC_RD_PUBLISH_TTL, publish0CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish0CB.Wait(100));
    itst::Callback publish1CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[1], CT_ADAPTER_IP, &handles[2],
              1, OIC_RD_PUBLISH_TTL, publish1CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish1CB.Wait(100));
    itst::Callback publish2CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[2], CT_ADAPTER_IP, &handles[3],
              1, OIC_RD_PUBLISH_TTL, publish2CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish2CB.Wait(100));

    itst::Callback discoverCB(&DatabaseHas1ResourceQueryMatchVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light2", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DatabaseHasNResourceQueryMatchesVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_TRUE(payload->next != NULL);
        if (payload->next)
        {
            payload = payload->next;
            for (int i = 0; i < 3; ++i)
            {
                EXPECT_TRUE(payload->resources != NULL);
                for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
                {
                    EXPECT_STREQ("core.light", payload->resources->types->value);
                    EXPECT_TRUE(payload->resources->types->next == NULL);
                }
                payload = payload->next;
            }
            EXPECT_TRUE(payload == NULL);
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DatabaseHasNResourceQueryMatches)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[4];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light",
                                            "oic.if.baseline", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[3], "core.light",
                                            "oic.if.baseline", "/a/light4", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publish0CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              1, OIC_RD_PUBLISH_TTL, publish0CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish0CB.Wait(100));
    itst::Callback publish1CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[1], CT_ADAPTER_IP, &handles[2],
              1, OIC_RD_PUBLISH_TTL, publish1CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish1CB.Wait(100));
    itst::Callback publish2CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[2], CT_ADAPTER_IP, &handles[3],
              1, OIC_RD_PUBLISH_TTL, publish2CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish2CB.Wait(100));

    itst::Callback discoverCB(&DatabaseHasNResourceQueryMatchesVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DatabaseHas0InterfaceQueryMatchesVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_TRUE(payload->next == NULL);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DatabaseHas0InterfaceQueryMatches)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[4];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.one", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light2",
                                            "oic.if.two", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light3",
                                            "oic.if.three", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[3], "core.light4",
                                            "oic.if.four", "/a/light4", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publish0CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              1, OIC_RD_PUBLISH_TTL, publish0CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish0CB.Wait(100));
    itst::Callback publish1CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[1], CT_ADAPTER_IP, &handles[2],
              1, OIC_RD_PUBLISH_TTL, publish1CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish1CB.Wait(100));
    itst::Callback publish2CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[2], CT_ADAPTER_IP, &handles[3],
              1, OIC_RD_PUBLISH_TTL, publish2CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish2CB.Wait(100));

    itst::Callback discoverCB(&DatabaseHas0InterfaceQueryMatchesVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=oic.if.one", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DatabaseHas1InterfaceQueryMatchVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_TRUE(payload->next != NULL);
        if (payload->next)
        {
            payload = payload->next;
            for (int i = 0; i < 1; ++i)
            {
                EXPECT_TRUE(payload->resources != NULL);
                for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
                {
                    EXPECT_TRUE(EndsWith(payload->resources->uri, "/a/light2"));
                    bool foundInterface = false;
                    for (OCStringLL *iface = payload->resources->interfaces; iface; iface = iface->next)
                    {
                        if (!strcmp("oic.if.two", iface->value))
                        {
                            foundInterface = true;
                        }
                    }
                    EXPECT_TRUE(foundInterface);
                }
                payload = payload->next;
            }
            EXPECT_TRUE(payload == NULL);
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DatabaseHas1InterfaceQueryMatch)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[4];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.one", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light2",
                                            "oic.if.two", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light3",
                                            "oic.if.three", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[3], "core.light4",
                                            "oic.if.four", "/a/light4", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publish0CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              1, OIC_RD_PUBLISH_TTL, publish0CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish0CB.Wait(100));
    itst::Callback publish1CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[1], CT_ADAPTER_IP, &handles[2],
              1, OIC_RD_PUBLISH_TTL, publish1CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish1CB.Wait(100));
    itst::Callback publish2CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[2], CT_ADAPTER_IP, &handles[3],
              1, OIC_RD_PUBLISH_TTL, publish2CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish2CB.Wait(100));

    itst::Callback discoverCB(&DatabaseHas1InterfaceQueryMatchVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=oic.if.two", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DatabaseHasNInterfaceQueryMatchesVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_TRUE(payload->next != NULL);
        if (payload->next)
        {
            payload = payload->next;
            for (int i = 0; i < 3; ++i)
            {
                EXPECT_TRUE(payload->resources != NULL);
                for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
                {
                    bool foundInterface = false;
                    for (OCStringLL *iface = payload->resources->interfaces; iface; iface = iface->next)
                    {
                        if (!strcmp("oic.if.a", iface->value))
                        {
                            foundInterface = true;
                        }
                    }
                    EXPECT_TRUE(foundInterface);
                }
                payload = payload->next;
            }
            EXPECT_TRUE(payload == NULL);
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DatabaseHasNInterfaceQueryMatches)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[4];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.a", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light",
                                            "oic.if.a", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light",
                                            "oic.if.a", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[3], "core.light",
                                            "oic.if.a", "/a/light4", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publish0CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              1, OIC_RD_PUBLISH_TTL, publish0CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish0CB.Wait(100));
    itst::Callback publish1CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[1], CT_ADAPTER_IP, &handles[2],
              1, OIC_RD_PUBLISH_TTL, publish1CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish1CB.Wait(100));
    itst::Callback publish2CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[2], CT_ADAPTER_IP, &handles[3],
              1, OIC_RD_PUBLISH_TTL, publish2CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish2CB.Wait(100));

    itst::Callback discoverCB(&DatabaseHasNInterfaceQueryMatchesVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=oic.if.a", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult ResourceAndInterfaceQueryMatchVerify(void *ctx,
        OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
        EXPECT_TRUE(payload->next != NULL);
        if (payload->next)
        {
            payload = payload->next;
            for (int i = 0; i < 1; ++i)
            {
                EXPECT_TRUE(payload->resources != NULL);
                for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
                {
                    EXPECT_TRUE(EndsWith(payload->resources->uri, "/a/light2"));
                    EXPECT_STREQ("core.light2", payload->resources->types->value);
                    EXPECT_TRUE(payload->resources->types->next == NULL);
                    bool foundInterface = false;
                    for (OCStringLL *iface = payload->resources->interfaces; iface; iface = iface->next)
                    {
                        if (!strcmp("oic.if.two", iface->value))
                        {
                            foundInterface = true;
                        }
                    }
                    EXPECT_TRUE(foundInterface);
                }
                payload = payload->next;
            }
            EXPECT_TRUE(payload == NULL);
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, ResourceAndInterfaceQueryMatch)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[4];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.a", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light2",
                                            "oic.if.two", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light3",
                                            "oic.if.two", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[3], "core.light2",
                                            "oic.if.a", "/a/light4", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publish0CB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, &handles[1],
              3, OIC_RD_PUBLISH_TTL, publish0CB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publish0CB.Wait(100));

    itst::Callback discoverCB(&ResourceAndInterfaceQueryMatchVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light2&if=oic.if.two",
                    NULL, 0, CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult BaselineVerify(void *ctx, OCDoHandle handle,
        OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
    OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload;
    EXPECT_TRUE(payload->type != NULL);
    EXPECT_TRUE(payload->iface != NULL);
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, Baseline)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[1];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles,
              1, OIC_RD_PUBLISH_TTL, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    itst::Callback discoverCB(&BaselineVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=oic.if.baseline", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DeleteDeviceVerify(void *ctx, OCDoHandle handle,
        OCClientResponse *response)
{
    (void) ctx;
    (void) handle;
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        for (OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload; payload;
             payload = payload->next)
        {
            EXPECT_STRNE((const char *) RDDiscoverTests::di[0], payload->sid);
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DeleteDevice)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[1];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles,
              1, OIC_RD_PUBLISH_TTL, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    itst::Callback deleteCB(&handleDeleteCB);
    EXPECT_EQ(OC_STACK_OK, OCRDDeleteWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, nullptr, 0,
                                                  deleteCB, OC_HIGH_QOS));
    EXPECT_EQ(OC_STACK_OK, deleteCB.Wait(100));

    itst::Callback discoverCB(&DeleteDeviceVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult Delete1Verify(void *ctx, OCDoHandle handle,
        OCClientResponse *response)
{
    (void) ctx;
    (void) handle;
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        bool foundId = false;
        bool foundLight = false;
        bool foundLight2 = false;
        for (OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload; payload;
             payload = payload->next)
        {
            if (!strcmp((const char *) RDDiscoverTests::di[0], payload->sid))
            {
                foundId = true;
                for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
                {
                    if (EndsWith(resource->uri, "/a/light2"))
                    {
                        foundLight2 = true;
                    }
                    else if (EndsWith(resource->uri, "/a/light"))
                    {
                        foundLight = true;
                    }
                }
            }
        }
        EXPECT_TRUE(foundId);
        EXPECT_FALSE(foundLight);
        EXPECT_TRUE(foundLight2);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, Delete1)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[2];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles,
              2, OIC_RD_PUBLISH_TTL, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    itst::Callback deleteCB(&handleDeleteCB);
    EXPECT_EQ(OC_STACK_OK, OCRDDeleteWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles, 1,
                                                  deleteCB, OC_HIGH_QOS));
    EXPECT_EQ(OC_STACK_OK, deleteCB.Wait(100));

    itst::Callback discoverCB(&Delete1Verify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

static OCStackApplicationResult DeleteNVerify(void *ctx, OCDoHandle handle,
        OCClientResponse *response)
{
    (void) ctx;
    (void) handle;
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(response->payload != NULL);
    if (response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);
        bool foundId = false;
        bool foundLight = false;
        bool foundLight2 = false;
        bool foundLight3 = false;
        for (OCDiscoveryPayload *payload = (OCDiscoveryPayload *) response->payload; payload;
             payload = payload->next)
        {
            if (!strcmp((const char *) RDDiscoverTests::di[0], payload->sid))
            {
                foundId = true;
                for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
                {
                    if (EndsWith(resource->uri, "/a/light3"))
                    {
                        foundLight3 = true;
                    }
                    else if (EndsWith(resource->uri, "/a/light2"))
                    {
                        foundLight2 = true;
                    }
                    else if (EndsWith(resource->uri, "/a/light"))
                    {
                        foundLight = true;
                    }
                }
            }
        }
        EXPECT_TRUE(foundId);
        EXPECT_FALSE(foundLight);
        EXPECT_FALSE(foundLight2);
        EXPECT_TRUE(foundLight3);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

TEST_P(RDDiscoverTests, DeleteN)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[3];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[1], "core.light",
                                            "oic.if.baseline", "/a/light2", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[2], "core.light",
                                            "oic.if.baseline", "/a/light3", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles,
              3, OIC_RD_PUBLISH_TTL, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    itst::Callback deleteCB(&handleDeleteCB);
    EXPECT_EQ(OC_STACK_OK, OCRDDeleteWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles, 2,
                                                  deleteCB, OC_HIGH_QOS));
    EXPECT_EQ(OC_STACK_OK, deleteCB.Wait(100));

    itst::Callback discoverCB(&DeleteNVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

INSTANTIATE_TEST_CASE_P(ContentFormat, RDDiscoverTests,
        ::testing::Values(COAP_MEDIATYPE_APPLICATION_VND_OCF_CBOR, COAP_MEDIATYPE_APPLICATION_CBOR));

TEST_P(RDDiscoverTests, TTLLapsedDeleteDevice)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OCResourceHandle handles[1];
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles[0], "core.light",
                                            "oic.if.baseline", "/a/light", rdEntityHandler,
                                            NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    itst::Callback publishCB(&handlePublishCB);
    EXPECT_EQ(OC_STACK_OK, OCRDPublishWithDeviceId(NULL, "127.0.0.1", di[0], CT_ADAPTER_IP, handles,
              1, 1, publishCB, OC_LOW_QOS));
    EXPECT_EQ(OC_STACK_OK, publishCB.Wait(100));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    itst::Callback discoverCB(&DeleteDeviceVerify);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res", NULL, 0,
                    CT_DEFAULT, OC_HIGH_QOS, discoverCB, options, numOptions));
    EXPECT_EQ(OC_STACK_OK, discoverCB.Wait(100));
}

#endif

