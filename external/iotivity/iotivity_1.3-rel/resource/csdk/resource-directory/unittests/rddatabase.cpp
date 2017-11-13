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
    #include "platform_features.h"
    #include "rd_server.h"
    #include "rd_client.h"
    #include "rd_database.h"
    #include "ocstack.h"
    #include "logger.h"
    #include "oic_malloc.h"
    #include "oic_string.h"
    #include "ocpayload.h"
    #include "payload_logging.h"
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
#define TAG "RDDatabaseTests"

std::chrono::seconds const SHORT_TEST_TIMEOUT = std::chrono::seconds(5);

//-----------------------------------------------------------------------------
// Callback functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Tests
//-----------------------------------------------------------------------------
class RDDatabaseTests : public testing::Test {
    protected:
    virtual void SetUp()
    {
        remove("RD.db");
        OCInit("127.0.0.1", 5683, OC_CLIENT_SERVER);
        EXPECT_EQ(OC_STACK_OK, OCRDDatabaseInit());
    }

    virtual void TearDown()
    {
        EXPECT_EQ(OC_STACK_OK, OCRDDatabaseClose());
        OCStop();
    }
};

typedef struct Resource
{
    const char *uri;
    const char *rt;
    const char *itf;
    uint8_t bm;
} Resource;

static OCRepPayload *CreateRDPublishPayload(const char *deviceId,
                                            int64_t ttl,
                                            Resource *resources,
                                            size_t nresources)
{
    OCRepPayload *repPayload = OCRepPayloadCreate();
    EXPECT_TRUE(repPayload != NULL);
    EXPECT_TRUE(deviceId != NULL);
    EXPECT_TRUE(OCRepPayloadSetPropString(repPayload, OC_RSRVD_DEVICE_ID, deviceId));
    EXPECT_TRUE(OCRepPayloadSetPropInt(repPayload, OC_RSRVD_DEVICE_TTL, (ttl == 0 ? 86400: ttl)));

    const OCRepPayload *linkArr[nresources];
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {nresources, 0, 0};

    for (size_t i = 0; i < nresources; ++i)
    {
        OCRepPayload *link = OCRepPayloadCreate();
        OCRepPayloadSetPropString(link, OC_RSRVD_HREF, resources[i].uri);
        char anchor[MAX_URI_LENGTH];
        snprintf(anchor, MAX_URI_LENGTH, "ocf://%s", deviceId);
        OCRepPayloadSetPropString(link, OC_RSRVD_URI, anchor);
        size_t rtDim[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **rt = (char **)OICMalloc(sizeof(char *) * 1);
        EXPECT_TRUE(NULL != rt) << "Failed to OICMalloc rt!";
        if (NULL == rt)
        {
            return NULL;
        }
        rt[0] = OICStrdup(resources[i].rt);
        EXPECT_TRUE(NULL != rt[0]) << "Failed to OICStrdup rt!";
        if (NULL == rt[0])
        {
            OICFree(rt);
            return NULL;
        }
        bool result = false;
        result = OCRepPayloadSetStringArray(link, OC_RSRVD_RESOURCE_TYPE, (const char **)rt, rtDim);
        OICFree(rt[0]);
        OICFree(rt);
        EXPECT_TRUE(result) << "Failed to set resource type!";
        if (!result)
        {
            return NULL;
        }

        size_t itfDim[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **itf = (char **)OICMalloc(sizeof(char *) * 1);
        EXPECT_TRUE(NULL != itf) << "Failed to OICMalloc itf!";
        if (NULL == itf)
        {
            return NULL;
        }
        itf[0] = OICStrdup(resources[i].itf);
        EXPECT_TRUE(NULL != itf[0]) << "Failed to OICStrdup itf!";
        if (NULL == itf[0])
        {
            OICFree(itf);
            return NULL;
        }

        result = OCRepPayloadSetStringArray(link, OC_RSRVD_INTERFACE, (const char **)itf, itfDim);
        OICFree(itf[0]);
        OICFree(itf);
        EXPECT_TRUE(result) << "Failed to set interface!";
        if (!result)
        {
            return NULL;
        }

        OCRepPayload *policy = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP, resources[i].bm);
        OCRepPayloadSetPropObjectAsOwner(link, OC_RSRVD_POLICY, policy);
        size_t epsDim[MAX_REP_ARRAY_DEPTH] = {2, 0, 0};
        OCRepPayload *eps[2];
        eps[0] = OCRepPayloadCreate();
        OCRepPayloadSetPropString(eps[0], OC_RSRVD_ENDPOINT, "coap://127.0.0.1:1234");
        OCRepPayloadSetPropInt(eps[0], OC_RSRVD_PRIORITY, 1);
        eps[1] = OCRepPayloadCreate();
        OCRepPayloadSetPropString(eps[1], OC_RSRVD_ENDPOINT, "coaps://[::1]:5678");
        OCRepPayloadSetPropInt(eps[1], OC_RSRVD_PRIORITY, 1);
        OCRepPayloadSetPropObjectArray(link, OC_RSRVD_ENDPOINTS, (const OCRepPayload **)eps, epsDim);
        linkArr[i] = link;
    }

    OCRepPayloadSetPropObjectArray(repPayload, OC_RSRVD_LINKS, linkArr, dimensions);

    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *)repPayload);
    return repPayload;
}

static void EndpointsVerify(const OCEndpointPayload *eps)
{
    EXPECT_STREQ(eps->tps, "coap");
    EXPECT_STREQ(eps->addr, "127.0.0.1");
    EXPECT_EQ(eps->family, OC_IP_USE_V4);
    EXPECT_EQ(eps->port, 1234);
    EXPECT_EQ(eps->pri, 1);
    EXPECT_STREQ(eps->next->tps, "coaps");
    EXPECT_STREQ(eps->next->addr, "::1");
    EXPECT_EQ(eps->next->family, (OC_FLAG_SECURE | OC_IP_USE_V6));
    EXPECT_EQ(eps->next->port, 5678);
    EXPECT_EQ(eps->next->pri, 1);
    EXPECT_TRUE(eps->next->next == NULL);
}

static OCRepPayload *CreateResources(const char *deviceId)
{
    Resource resources[] = {
        { "/a/thermostat", "core.thermostat", OC_RSRVD_INTERFACE_DEFAULT, OC_DISCOVERABLE },
        { "/a/light", "core.light", OC_RSRVD_INTERFACE_DEFAULT, OC_DISCOVERABLE }
    };
    return CreateRDPublishPayload(deviceId, 0, resources, 2);
}

TEST_F(RDDatabaseTests, Create)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
}

TEST_F(RDDatabaseTests, StoreResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    const char *deviceId = "7a960f46-a52e-4837-bd83-460b1a6dd56b";
    OCRepPayload *repPayload = CreateResources(deviceId);
    ASSERT_TRUE(NULL != repPayload) << "CreateResources failed!";

    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));

    OCDiscoveryPayload *discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(NULL, "core.light", &discPayload));
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(OC_RSRVD_INTERFACE_DEFAULT, NULL, &discPayload));
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(NULL, "core.light", &discPayload));
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(OC_RSRVD_INTERFACE_DEFAULT, "core.light", &discPayload));
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;

    OCPayloadDestroy((OCPayload *)repPayload);
}

TEST_F(RDDatabaseTests, AddResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    const char *deviceId = "7a960f46-a52e-4837-bd83-460b1a6dd56b";

    OCRepPayload *repPayload = CreateResources(deviceId);
    ASSERT_TRUE(NULL != repPayload) << "CreateResources failed!";
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    Resource resources[] = {
        { "/a/light2", "core.light", OC_RSRVD_INTERFACE_DEFAULT, OC_DISCOVERABLE }
    };
    repPayload = CreateRDPublishPayload(deviceId, 0, resources, 1);
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    OCDiscoveryPayload *discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(OC_RSRVD_INTERFACE_LL, NULL, &discPayload));
    bool foundThermostat = false;
    bool foundLight = false;
    bool foundLight2 = false;
    for (OCResourcePayload *resource = discPayload->resources; resource; resource = resource->next)
    {
        if (!strcmp("/a/thermostat", resource->uri))
        {
            foundThermostat = true;
        }
        else if (!strcmp("/a/light", resource->uri))
        {
            foundLight = true;
        }
        else if (!strcmp("/a/light2", resource->uri))
        {
            foundLight2 = true;
        }
    }
    EXPECT_TRUE(foundThermostat);
    EXPECT_TRUE(foundLight);
    EXPECT_TRUE(foundLight2);
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;
}

TEST_F(RDDatabaseTests, UpdateResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    const char *anchor = "ocf://7a960f46-a52e-4837-bd83-460b1a6dd56b";
    const char *deviceId = &anchor[6];

    OCRepPayload *repPayload = CreateResources(deviceId);
    ASSERT_TRUE(NULL != repPayload) << "CreateResources failed!";

    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    Resource resources[] = {
        { "/a/thermostat", "x.core.r.thermostat", "x.core.if.thermostat", OC_DISCOVERABLE | OC_OBSERVABLE },
        { "/a/light", "x.core.r.light", "x.core.if.light", OC_DISCOVERABLE | OC_OBSERVABLE }
    };
    repPayload = CreateRDPublishPayload(deviceId, 0, resources, 2);
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    OCDiscoveryPayload *discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(OC_RSRVD_INTERFACE_LL, NULL, &discPayload));
    bool foundThermostat = false;
    bool foundLight = false;
    for (OCResourcePayload *resource = discPayload->resources; resource; resource = resource->next)
    {
        if (!strcmp("/a/thermostat", resource->uri))
        {
            foundThermostat = true;
            EXPECT_STREQ("x.core.r.thermostat", resource->types->value);
            EXPECT_TRUE(resource->types->next == NULL);
            EXPECT_STREQ("x.core.if.thermostat", resource->interfaces->value);
            EXPECT_TRUE(resource->interfaces->next == NULL);
            EXPECT_EQ(OC_DISCOVERABLE | OC_OBSERVABLE, resource->bitmap);
            EXPECT_STREQ(anchor, resource->anchor);
            EndpointsVerify(resource->eps);
        }
        else if (!strcmp("/a/light", resource->uri))
        {
            foundLight = true;
            EXPECT_STREQ("x.core.r.light", resource->types->value);
            EXPECT_TRUE(resource->types->next == NULL);
            EXPECT_STREQ("x.core.if.light", resource->interfaces->value);
            EXPECT_TRUE(resource->interfaces->next == NULL);
            EXPECT_EQ(OC_DISCOVERABLE | OC_OBSERVABLE, resource->bitmap);
            EXPECT_STREQ(anchor, resource->anchor);
            EndpointsVerify(resource->eps);
        }
    }
    EXPECT_TRUE(foundThermostat);
    EXPECT_TRUE(foundLight);
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;
}

TEST_F(RDDatabaseTests, AddAndUpdateResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    const char *anchor = "ocf://7a960f46-a52e-4837-bd83-460b1a6dd56b";
    const char *deviceId = &anchor[6];

    OCRepPayload *repPayload = CreateResources(deviceId);
    ASSERT_TRUE(NULL != repPayload) << "CreateResources failed!";
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    Resource resources[] = {
        { "/a/thermostat", "x.core.r.thermostat", "x.core.if.thermostat", OC_DISCOVERABLE | OC_OBSERVABLE },
        { "/a/light", "x.core.r.light", "x.core.if.light", OC_DISCOVERABLE | OC_OBSERVABLE },
        { "/a/light2", "core.light", OC_RSRVD_INTERFACE_DEFAULT, OC_DISCOVERABLE }
    };
    repPayload = CreateRDPublishPayload(deviceId, 0, resources, 3);
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    OCDiscoveryPayload *discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(OC_RSRVD_INTERFACE_LL, NULL, &discPayload));
    bool foundThermostat = false;
    bool foundLight = false;
    bool foundLight2 = false;
    for (OCResourcePayload *resource = discPayload->resources; resource; resource = resource->next)
    {
        if (!strcmp("/a/thermostat", resource->uri))
        {
            foundThermostat = true;
            EXPECT_STREQ("x.core.r.thermostat", resource->types->value);
            EXPECT_TRUE(resource->types->next == NULL);
            EXPECT_STREQ("x.core.if.thermostat", resource->interfaces->value);
            EXPECT_TRUE(resource->interfaces->next == NULL);
            EXPECT_EQ(OC_DISCOVERABLE | OC_OBSERVABLE, resource->bitmap);
            EXPECT_STREQ(anchor, resource->anchor);
            EndpointsVerify(resource->eps);
        }
        else if (!strcmp("/a/light", resource->uri))
        {
            foundLight = true;
            EXPECT_STREQ("x.core.r.light", resource->types->value);
            EXPECT_TRUE(resource->types->next == NULL);
            EXPECT_STREQ("x.core.if.light", resource->interfaces->value);
            EXPECT_TRUE(resource->interfaces->next == NULL);
            EXPECT_EQ(OC_DISCOVERABLE | OC_OBSERVABLE, resource->bitmap);
            EXPECT_STREQ(anchor, resource->anchor);
            EndpointsVerify(resource->eps);
        }
        else if (!strcmp("/a/light2", resource->uri))
        {
            foundLight2 = true;
            EXPECT_STREQ("core.light", resource->types->value);
            EXPECT_TRUE(resource->types->next == NULL);
            EXPECT_STREQ(OC_RSRVD_INTERFACE_DEFAULT, resource->interfaces->value);
            EXPECT_TRUE(resource->interfaces->next == NULL);
            EXPECT_EQ(OC_DISCOVERABLE, resource->bitmap);
            EXPECT_STREQ(anchor, resource->anchor);
            EndpointsVerify(resource->eps);
        }
    }
    EXPECT_TRUE(foundThermostat);
    EXPECT_TRUE(foundLight);
    EXPECT_TRUE(foundLight2);
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;
}

TEST_F(RDDatabaseTests, DeleteResourcesDevice)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    const char *deviceIds[2] =
    {
        "7a960f46-a52e-4837-bd83-460b1a6dd56b",
        "983656a7-c7e5-49c2-a201-edbeb7606fb5",
    };
    OCRepPayload *payloads[2];
    payloads[0] = CreateResources(deviceIds[0]);
    ASSERT_TRUE(NULL != payloads[0]) << "CreateResources failed!";
    payloads[1] = CreateResources(deviceIds[1]);
    ASSERT_TRUE(NULL != payloads[1]) << "CreateResources failed!";
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(payloads[0]));
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(payloads[1]));

    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDeleteResources(deviceIds[0], NULL, 0));

    OCDiscoveryPayload *discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(OC_RSRVD_INTERFACE_LL, NULL, &discPayload));
    bool found0 = false;
    bool found1 = false;
    for (OCDiscoveryPayload *payload = discPayload; payload; payload = payload->next)
    {
        if (!strcmp((const char *) deviceIds[0], payload->sid))
        {
            found0 = true;
        }
        if (!strcmp((const char *) deviceIds[1], payload->sid))
        {
            found1 = true;
        }
    }
    EXPECT_FALSE(found0);
    EXPECT_TRUE(found1);
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;

    OCPayloadDestroy((OCPayload *)payloads[0]);
    OCPayloadDestroy((OCPayload *)payloads[1]);
}

TEST_F(RDDatabaseTests, TTLLapsedDeleteDevice)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    const char *deviceIds[2] =
    {
        "7a960f46-a52e-4837-bd83-460b1a6dd56b",
        "983656a7-c7e5-49c2-a201-edbeb7606fb5",
    };
    OCRepPayload *repPayload = CreateResources(deviceIds[0]);
    ASSERT_TRUE(NULL != repPayload) << "CreateResources failed!";
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    Resource resources[] = {
        { "/a/light2", "core.light", OC_RSRVD_INTERFACE_DEFAULT, OC_DISCOVERABLE }
    };
    repPayload = CreateRDPublishPayload(deviceIds[0], 1, resources, 1);
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);
    repPayload = CreateRDPublishPayload(deviceIds[1], OIC_RD_PUBLISH_TTL, resources, 1);
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseStoreResources(repPayload));
    OCPayloadDestroy((OCPayload *)repPayload);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    OCDiscoveryPayload *discPayload = NULL;
    EXPECT_EQ(OC_STACK_OK, OCRDDatabaseDiscoveryPayloadCreate(OC_RSRVD_INTERFACE_LL, NULL, &discPayload));
    bool found = false;
    for (OCDiscoveryPayload *payload = discPayload; payload; payload = payload->next)
    {
        if (!strcmp(deviceIds[0], payload->sid))
        {
            found = true;
        }
    }
    EXPECT_FALSE(found);
    OCDiscoveryPayloadDestroy(discPayload);
    discPayload = NULL;
}
