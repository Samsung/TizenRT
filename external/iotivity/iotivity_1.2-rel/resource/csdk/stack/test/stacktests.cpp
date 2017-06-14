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


extern "C"
{
    #include "ocpayload.h"
    #include "ocstack.h"
    #include "ocstackinternal.h"
    #include "logger.h"
    #include "oic_malloc.h"
    #include "oic_string.h"
    #include "oic_time.h"
}

#include "gtest/gtest.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
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
static const char TAG[] = "TestHarness";

char gDeviceUUID[] = "myDeviceUUID";
char gManufacturerName[] = "myName";
static OCPrm_t pmSel;
static char pinNumber;
static OCDPDev_t peer;

std::chrono::seconds const SHORT_TEST_TIMEOUT = std::chrono::seconds(5);
std::chrono::seconds const LONG_TEST_TIMEOUT = std::chrono::seconds(300);

//-----------------------------------------------------------------------------
// Callback functions
//-----------------------------------------------------------------------------
extern "C"  OCStackApplicationResult asyncDoResourcesCallback(void* ctx,
        OCDoHandle /*handle*/, OCClientResponse * clientResponse)
{
    OIC_LOG(INFO, TAG, "Entering asyncDoResourcesCallback");

    EXPECT_EQ(OC_STACK_OK, clientResponse->result);

    if(ctx == (void*)DEFAULT_CONTEXT_VALUE) {
        OIC_LOG_V(INFO, TAG, "Callback Context recvd successfully");
    }
    OIC_LOG_V(INFO, TAG, "result = %d", clientResponse->result);

    return OC_STACK_KEEP_TRANSACTION;
}

static void resultCallback(void *UNUSED1, OCDPDev_t *UNUSED2, OCStackResult UNUSED3)
{
    (void) (UNUSED1);
    (void) (UNUSED2);
    (void) (UNUSED3);
}

extern "C" OCStackApplicationResult discoveryCallback(void* ctx,
        OCDoHandle /*handle*/, OCClientResponse * clientResponse)
{
    OIC_LOG(INFO, TAG, "Entering asyncDoResourcesCallback");

    EXPECT_EQ(OC_STACK_OK, clientResponse->result);

    if(ctx == (void*)DEFAULT_CONTEXT_VALUE)
    {
        OIC_LOG_V(INFO, TAG, "Callback Context recvd successfully");
    }

    OIC_LOG_V(INFO, TAG, "result = %d", clientResponse->result);

    OCDiscoveryPayload *discoveryPayload = ((OCDiscoveryPayload *) clientResponse->payload);
    EXPECT_TRUE(discoveryPayload != NULL);
    OCResourcePayload *res = discoveryPayload->resources;
    int count = 0;
    for (OCResourcePayload *res1 = discoveryPayload->resources; res1; res1 = res1->next)
    {
        count++;
    }
    EXPECT_EQ(3, count);
    EXPECT_EQ("/a/led1", res->uri);
    res = res->next;
    EXPECT_EQ("/a/led2", res->uri);
    res = res->next;
    EXPECT_EQ("/a/led3", res->uri);
    res = res->next;
    EXPECT_TRUE(res == NULL);

    return OC_STACK_KEEP_TRANSACTION;
}

//-----------------------------------------------------------------------------
// Entity handler
//-----------------------------------------------------------------------------
OCEntityHandlerResult entityHandler(OCEntityHandlerFlag /*flag*/,
        OCEntityHandlerRequest * /*entityHandlerRequest*/,
        void* /*callbackParam*/)
{
    OIC_LOG(INFO, TAG, "Entering entityHandler");

    return OC_EH_OK;
}

//-----------------------------------------------------------------------------
//  Local functions
//-----------------------------------------------------------------------------
void InitStack(OCMode mode)
{
    OIC_LOG(INFO, TAG, "Entering InitStack");

    EXPECT_EQ(OC_STACK_OK, OCInit(NULL, 0, mode));
    OIC_LOG(INFO, TAG, "Leaving InitStack");
}

uint8_t InitNumExpectedResources()
{
#ifdef WITH_PRESENCE
    // When presence is enabled, it is a resource and so is (currently) included
    // in the returned resource count returned by the OCGetNumberOfResources API.
    return 1;
#else
    return 0;
#endif
}

uint8_t InitResourceIndex()
{
#ifdef WITH_PRESENCE
    // When presence is enabled, it is a resource and so is (currently) included
    // in the returned resource count returned by the OCGetNumberOfResources API.
    // The index of the presence resource is 0, so the first user app resource index
    // is 1.
    return 1;
#else
    return 0;
#endif
}

class Callback
{
    public:
        Callback(OCClientResponseHandler cb) : m_cb(cb), m_called(false)
        {
            m_cbData.cb = &Callback::handler;
            m_cbData.cd = NULL;
            m_cbData.context = this;
        }
        void Wait(long waitTime)
        {
            uint64_t startTime = OICGetCurrentTime(TIME_IN_MS);
            while (!m_called)
            {
                uint64_t currTime = OICGetCurrentTime(TIME_IN_MS);
                long elapsed = (long)((currTime - startTime) / MS_PER_SEC);
                if (elapsed > waitTime)
                {
                    m_called = true;
                }
                OCProcess();
            }
        }
        operator OCCallbackData *()
        {
            return &m_cbData;
        }
    private:
        OCCallbackData m_cbData;
        OCClientResponseHandler m_cb;
        bool m_called;
        static OCStackApplicationResult handler(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
        {
            Callback *callback = (Callback *) ctx;
            OCStackApplicationResult result = callback->m_cb(NULL, handle, clientResponse);
            callback->m_called = true;
            return result;
        }
};

class OCDiscoverTests : public testing::Test
{
    protected:
        virtual void SetUp()
        {
            EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT_SERVER));
        }

        virtual void TearDown()
        {
            OCStop();
        }
};
//-----------------------------------------------------------------------------
//  Tests
//-----------------------------------------------------------------------------

TEST(StackInit, StackInitNullAddr)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit(0, 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackInit, StackInitNullPort)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 0, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackInit, StackInitNullAddrAndPort)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit(0, 0, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackInit, StackInitInvalidMode)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_ERROR, OCInit(0, 0, (OCMode)10));
    EXPECT_EQ(OC_STACK_ERROR, OCStop());
}

TEST(StackStart, StackStartSuccessClient)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, StackStartSuccessServer)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, StackStartSuccessClientServer)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, StackStartSuccessServerThenClient)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, StackStartSuccessClientThenServer)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT));
    EXPECT_EQ(OC_STACK_OK, OCStop());
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, StackStartSuccessiveInits)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.2", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoValid)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));

    OCPlatformInfo info =
    {
        gDeviceUUID,
        gManufacturerName,
        0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    EXPECT_EQ(OC_STACK_OK, OCSetPlatformInfo(info));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoWithClientMode)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT));

    OCPlatformInfo info =
    {
        gDeviceUUID,
        gManufacturerName,
        0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPlatformInfo(info));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoWithNoPlatformID)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));

    OCPlatformInfo info =
     {
         0,
         gDeviceUUID,
         0, 0, 0, 0, 0, 0, 0, 0, 0
     };

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPlatformInfo(info));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoWithNoManufacturerName)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));

    OCPlatformInfo info =
    {
        gDeviceUUID,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPlatformInfo(info));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoWithZeroLengthManufacturerName)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));

    OCPlatformInfo info =
    {
        gDeviceUUID,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    info.manufacturerName = (char *) "";

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPlatformInfo(info));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoWithTooLongManufacName)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    char gTooLongManufacturerName[MAX_PLATFORM_NAME_LENGTH+2];
    for (int i = 0; i <= MAX_PLATFORM_NAME_LENGTH; i++ )
    {
        gTooLongManufacturerName[i] = 'a';
    }
    gTooLongManufacturerName[MAX_PLATFORM_NAME_LENGTH+1] = '\0';
    OCPlatformInfo info =
    {
        gDeviceUUID,
        gTooLongManufacturerName,
        0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPlatformInfo(info));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoWithTooLongManufacURL)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    char gManufacturerUrl[MAX_PLATFORM_URL_LENGTH+2];
    for (int i = 0; i <= MAX_PLATFORM_URL_LENGTH; i++ )
    {
        gManufacturerUrl[i] = 'a';
    }
    gManufacturerUrl[MAX_PLATFORM_URL_LENGTH+1] = '\0';
    OCPlatformInfo info =
    {
        gDeviceUUID,
        gManufacturerName,
        gManufacturerUrl,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPlatformInfo(info));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetPlatformInfoWithOCSetPropertyValueAPI)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_ID, gDeviceUUID));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_NAME, gManufacturerName));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_URL, "http://www.iotivity.org"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MODEL_NUM, "S777"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_DATE, "15 Nov, 2016"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_OS_VERSION, "1.1"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_VERSION, "14"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_HARDWARE_VERSION, "0.1"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_FIRMWARE_VERSION, "0.1"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SUPPORT_URL, "http://www.iotivity.org"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SYSTEM_TIME, ""));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, "x.org.iotivity.AAAA", "value"));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, NULL, ""));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPropertyValue(PAYLOAD_TYPE_INVALID, NULL, NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPropertyValue(PAYLOAD_TYPE_INVALID, NULL, NULL));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, GetPlatformInfoWithOCGetPropertyValueAPI)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_ID, gDeviceUUID));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_NAME, gManufacturerName));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_URL, "http://www.iotivity.org"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MODEL_NUM, "S777"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_DATE, "15 Nov, 2016"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_OS_VERSION, "1.1"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_VERSION, "14"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_HARDWARE_VERSION, "0.1"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_FIRMWARE_VERSION, "0.1"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SUPPORT_URL, "http://www.iotivity.org"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SYSTEM_TIME, ""));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, "x.org.iotivity.AAAA", "value"));

    void *value = NULL;
    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_ID, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ(gDeviceUUID, (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_NAME, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ(gManufacturerName, (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_URL, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("http://www.iotivity.org", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MODEL_NUM, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("S777", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_DATE, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("15 Nov, 2016", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_OS_VERSION, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("1.1", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_VERSION, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("14", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_HARDWARE_VERSION, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("0.1", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_FIRMWARE_VERSION, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("0.1", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SUPPORT_URL, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("http://www.iotivity.org", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_SYSTEM_TIME, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK,OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, "x.org.iotivity.AAAA", &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("value", (char *)value);
    OICFree(value);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, SetDeviceInfoAPI)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, "Sample"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, "specVersion"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "x.org.iotivity.newproperty", "value"));
    OCStringLL *dataModelVersions = OCCreateOCStringLL("Data Model Version");
    EXPECT_TRUE(dataModelVersions != NULL);
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION, dataModelVersions));
    OCFreeOCStringLL(dataModelVersions);
    OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
    EXPECT_TRUE(handle != NULL);
    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "oic.wk.tv"));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPropertyValue(PAYLOAD_TYPE_INVALID, NULL, NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSetPropertyValue(PAYLOAD_TYPE_INVALID, "", NULL));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStart, GetDeviceInfoAPI)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_SERVER));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, "Sample"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, "specVersion"));
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "x.org.iotivity.newproperty", "value"));
    OCStringLL *dataModelVersions = OCCreateOCStringLL("Data Model Version");
    EXPECT_TRUE(dataModelVersions != NULL);
    EXPECT_EQ(OC_STACK_OK, OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION, dataModelVersions));
    OCFreeOCStringLL(dataModelVersions);
    OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
    EXPECT_TRUE(handle != NULL);
    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "oic.wk.tv"));

    void *value = NULL;
    EXPECT_EQ(OC_STACK_OK, OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("Sample", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK, OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("specVersion", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK, OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, "x.org.iotivity.newproperty", &value));
    ASSERT_TRUE(value != NULL);
    EXPECT_STREQ("value", (char *)value);
    OICFree(value);
    value = NULL;

    EXPECT_EQ(OC_STACK_OK, OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION, &value));
    ASSERT_TRUE(value != NULL);
    ASSERT_TRUE(((OCStringLL *)value)->value);
    EXPECT_STREQ("Data Model Version", ((OCStringLL *)value)->value);
    OCFreeOCStringLL((OCStringLL *) value);
    value = NULL;

    EXPECT_STREQ("oic.wk.d", OCGetResourceTypeName(handle, 0));
    EXPECT_STREQ("oic.wk.tv", OCGetResourceTypeName(handle, 1));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackDiscovery, DISABLED_DoResourceDeviceDiscovery)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OCCallbackData cbData;
    OCDoHandle handle;

    OIC_LOG(INFO, TAG, "Starting DoResourceDeviceDiscovery test ");
    InitStack(OC_CLIENT);

    /* Start a discovery query*/
    char szQueryUri[MAX_QUERY_LENGTH] = { 0 };
    strcpy(szQueryUri, OC_RSRVD_WELL_KNOWN_URI);
    cbData.cb = asyncDoResourcesCallback;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;
    EXPECT_EQ(OC_STACK_OK, OCDoResource(&handle,
                                        OC_REST_GET,
                                        szQueryUri,
                                        0,
                                        0,
                                        CT_ADAPTER_IP,
                                        OC_LOW_QOS,
                                        &cbData,
                                        NULL,
                                        0));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackStop, StackStopWithoutInit)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_ERROR, OCStop());
}

TEST(StackStop, StackStopRepeated)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    EXPECT_EQ(OC_STACK_OK, OCInit("127.0.0.1", 5683, OC_CLIENT));
    EXPECT_EQ(OC_STACK_OK, OCStop());
    EXPECT_EQ(OC_STACK_ERROR, OCStop());
}

TEST(StackResource, DISABLED_UpdateResourceNullURI)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OCCallbackData cbData;
    OCDoHandle handle;

    OIC_LOG(INFO, TAG, "Starting UpdateResourceNullURI test");
    InitStack(OC_CLIENT);

    /* Start a discovery query*/
    char szQueryUri[MAX_QUERY_LENGTH] = { 0 };
    strcpy(szQueryUri, OC_RSRVD_WELL_KNOWN_URI);
    cbData.cb = asyncDoResourcesCallback;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;
    EXPECT_EQ(OC_STACK_OK, OCDoResource(&handle,
                                        OC_REST_GET,
                                        szQueryUri,
                                        0,
                                        0,
                                        CT_ADAPTER_IP,
                                        OC_LOW_QOS,
                                        &cbData,
                                        NULL,
                                        0));
    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceBadParams)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceBadParams test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCCreateResource(NULL, //&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCCreateResource(&handle,
                                            NULL, //"core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    // Property bitmask out of range
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            255));// invalid bitmask for OCResourceProperty

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceBadUri)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceBadUri test");
    InitStack(OC_SERVER);

    const char *uri257 = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW";

    OCResourceHandle handle;

    EXPECT_EQ(OC_STACK_INVALID_URI, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            NULL, //"/a/led",
                                            0,
                                            0,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_INVALID_URI, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "", //"/a/led",
                                            0,
                                            0,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_INVALID_URI, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            uri257, //"/a/led",
                                            0,
                                            0,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceSuccess)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceSuccess test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    const char *url = OCGetResourceUri(handle);
    EXPECT_STREQ("/a/led", url);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceSuccessWithResourcePolicyPropNone)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceSuccessWithResourcePolicyPropNone test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    // the resource is non-discoverable & non-observable by the client.
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_RES_PROP_NONE));// the resource is non-discoverable &
                                                // non-observable by the client.
    const char* url = OCGetResourceUri(handle);
    EXPECT_STREQ("/a/led", url);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceWithClientStackMode)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceSuccess test");
    InitStack(OC_CLIENT);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceFailDuplicateUri)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceFailDuplicateUri test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    const char *url = OCGetResourceUri(handle);
    EXPECT_STREQ("/a/led", url);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceMultipleResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceMultipleResources test");
    InitStack(OC_SERVER);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle2,
                                            "core.led",
                                            "core.rw",
                                            "/a/led2",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    OCResourceHandle handle3;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle3,
                                            "core.led",
                                            "core.rw",
                                            "/a/led3",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    const char *url = OCGetResourceUri(handle1);
    EXPECT_STREQ("/a/led1", url);

    url = OCGetResourceUri(handle2);
    EXPECT_STREQ("/a/led2", url);

    url = OCGetResourceUri(handle3);
    EXPECT_STREQ("/a/led3", url);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceBadResoureType)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceBadResoureType test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCCreateResource(&handle,
                                            NULL, //"core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCCreateResource(&handle2,
                                            "",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, CreateResourceGoodResourceType)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting CreateResourceGoodResourceType test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, ResourceTypeName)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting ResourceTypeName test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceTypes;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(1, numResourceTypes);
    const char *resourceTypeName = OCGetResourceTypeName(handle, 0);
    EXPECT_STREQ("core.led", resourceTypeName);

    // try getting resource type names with an invalid index
    resourceTypeName = OCGetResourceTypeName(handle, 1);
    EXPECT_STREQ(NULL, resourceTypeName);
    // try getting resource type names with an invalid index
    resourceTypeName = OCGetResourceTypeName(handle, 10);
    EXPECT_STREQ(NULL, resourceTypeName);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, ResourceTypeAttrRepresentation)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting ResourceTypeAttrRepresentation test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceTypes;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(1, numResourceTypes);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, ResourceTypeInterface)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting ResourceTypeInterface test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);
    const char *resourceInterfaceName1 = OCGetResourceInterfaceName(handle, 0);
    EXPECT_STREQ(OC_RSRVD_INTERFACE_DEFAULT, resourceInterfaceName1);
    const char *resourceInterfaceName = OCGetResourceInterfaceName(handle, 1);
    EXPECT_STREQ("core.rw", resourceInterfaceName);

    // try getting resource interface names with an invalid index
    resourceInterfaceName = OCGetResourceInterfaceName(handle, 2);
    EXPECT_STREQ(NULL, resourceInterfaceName);
    // try getting resource interface names with an invalid index
    resourceInterfaceName = OCGetResourceInterfaceName(handle, 10);
    EXPECT_STREQ(NULL, resourceInterfaceName);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, ResourceDefaultInterfaceAlwaysFirst)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OIC_LOG(INFO, TAG, "Starting ResourceDefaultInterfaceAlwaysFirst test");

    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle,
                                        OC_RSRVD_INTERFACE_DEFAULT));
    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);

    const char *interfaceName_1 = OCGetResourceInterfaceName(handle, 0);
    EXPECT_STREQ(OC_RSRVD_INTERFACE_DEFAULT, interfaceName_1);

    const char*interfaceName_2 = OCGetResourceInterfaceName(handle, 1);
    EXPECT_STREQ("core.rw", interfaceName_2);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, ResourceDuplicateDefaultInterfaces)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OIC_LOG(INFO, TAG, "Starting ResourceDuplicateDefaultInterfaces test");

    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle,
                                        OC_RSRVD_INTERFACE_DEFAULT));
    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle,
                                        OC_RSRVD_INTERFACE_DEFAULT));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);

    const char *interfaceName_1 = OCGetResourceInterfaceName(handle, 0);
    EXPECT_STREQ(OC_RSRVD_INTERFACE_DEFAULT, interfaceName_1);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, ResourceDuplicateNonDefaultInterfaces)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);

    OIC_LOG(INFO, TAG, "Starting ResourceDuplicateInterfaces test");

    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle,
                                        "core.rw"));
    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle,
                                        "core.rw"));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, ResourceTypeInterfaceMethods)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting ResourceTypeInterfaceMethods test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, GetResourceProperties)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting GetResourceProperties test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

#ifdef MQ_PUBLISHER
    EXPECT_EQ(OC_ACTIVE|OC_DISCOVERABLE|OC_OBSERVABLE|OC_MQ_PUBLISHER, OCGetResourceProperties(handle));
#else
    EXPECT_EQ(OC_ACTIVE|OC_DISCOVERABLE|OC_OBSERVABLE, OCGetResourceProperties(handle));
#endif
    EXPECT_EQ(OC_STACK_OK, OCDeleteResource(handle));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, StackTestResourceDiscoverOneResourceBad)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting StackTestResourceDiscoverOneResourceBad test");
    InitStack(OC_SERVER);
    uint8_t numResources = 0;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a1/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    const char *url = OCGetResourceUri(handle);
    EXPECT_STREQ("/a1/led", url);

    //EXPECT_EQ(OC_STACK_INVALID_URI, OCHandleServerRequest(&res, uri, query, req, rsp));
    EXPECT_EQ(OC_STACK_OK, OCDeleteResource(handle));
    uint8_t numExpectedResources = 0;

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numExpectedResources));
    EXPECT_EQ(numExpectedResources, numResources);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, StackTestResourceDiscoverOneResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting StackTestResourceDiscoverOneResource test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    const char *url = OCGetResourceUri(handle);
    EXPECT_STREQ("/a/led", url);

    //EXPECT_EQ(OC_STACK_OK, OCHandleServerRequest(&res, uri, query, req, rsp));
    EXPECT_EQ(OC_STACK_OK, OCDeleteResource(handle));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResource, StackTestResourceDiscoverManyResources)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting StackTestResourceDiscoverManyResources test");
    InitStack(OC_SERVER);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE));
    const char *url = OCGetResourceUri(handle1);
    EXPECT_STREQ("/a/led1", url);

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle2,
                                            "core.led",
                                            "core.rw",
                                            "/a/led2",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    url = OCGetResourceUri(handle2);
    EXPECT_STREQ("/a/led2", url);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle2, "core.brightled"));
    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle2, "core.colorled"));

    OCResourceHandle handle3;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle3,
                                            "core.led",
                                            "core.rw",
                                            "/a/led3",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    url = OCGetResourceUri(handle3);
    EXPECT_STREQ("/a/led3", url);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle3, OC_RSRVD_INTERFACE_LL));
    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle3, OC_RSRVD_INTERFACE_BATCH));

    OCResourceHandle handle4;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle4,
                                            "core.led",
                                            "core.rw",
                                            "/a/led4",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE));
    url = OCGetResourceUri(handle4);
    EXPECT_STREQ("/a/led4", url);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle4, "core.brightled"));
    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle4, OC_RSRVD_INTERFACE_LL));
    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle4, OC_RSRVD_INTERFACE_BATCH));

    //EXPECT_EQ(OC_STACK_OK, OCHandleServerRequest(&res, uri, query, req, rsp));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindResourceTypeNameBad)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindResourceTypeNameBad test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceTypes;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(1, numResourceTypes);
    const char *resourceTypeName = OCGetResourceTypeName(handle, 0);
    EXPECT_STREQ("core.led", resourceTypeName);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCBindResourceTypeToResource(handle, NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCBindResourceTypeToResource(handle, "core.nameBad"));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCBindResourceTypeToResource(handle, "core.name bad"));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindResourceTypeNameGood)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindResourceTypeNameGood test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceTypes;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(1, numResourceTypes);
    const char *resourceTypeName = OCGetResourceTypeName(handle, 0);
    EXPECT_STREQ("core.led", resourceTypeName);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "core.brightled"));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(2, numResourceTypes);
    resourceTypeName = OCGetResourceTypeName(handle, 1);
    EXPECT_STREQ("core.brightled", resourceTypeName);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "core.reallybrightled"));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(3, numResourceTypes);
    resourceTypeName = OCGetResourceTypeName(handle, 2);
    EXPECT_STREQ("core.reallybrightled", resourceTypeName);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "x.ReallyReallyBrightLED"));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(4, numResourceTypes);
    resourceTypeName = OCGetResourceTypeName(handle, 3);
    EXPECT_STREQ("x.ReallyReallyBrightLED", resourceTypeName);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindResourceTypeAttribRepGood)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindResourceTypeAttribRepGood test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceTypes;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(1, numResourceTypes);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "core.brightled"));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(2, numResourceTypes);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "core.reallybrightled"));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceTypes(handle, &numResourceTypes));
    EXPECT_EQ(3, numResourceTypes);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}


TEST(StackBind, BindResourceInterfaceNameBad)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindResourceInterfaceNameBad test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);
    const char *resourceInterfaceName = OCGetResourceInterfaceName(handle, 1);
    EXPECT_STREQ("core.rw", resourceInterfaceName);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCBindResourceInterfaceToResource(handle, NULL));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindResourceInterfaceNameGood)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindResourceInterfaceNameGood test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);
    const char *resourceInterfaceName = OCGetResourceInterfaceName(handle, 1);
    EXPECT_STREQ("core.rw", resourceInterfaceName);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle, "core.r"));

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(3, numResourceInterfaces);
    resourceInterfaceName = OCGetResourceInterfaceName(handle, 2);
    EXPECT_STREQ("core.r", resourceInterfaceName);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindResourceInterfaceMethodsBad)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindResourceInterfaceMethodsBad test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCBindResourceInterfaceToResource(handle, 0));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindResourceInterfaceMethodsGood)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindResourceInterfaceMethodsGood test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);

    EXPECT_EQ(OC_STACK_OK, OCBindResourceInterfaceToResource(handle, "core.r"));

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle, &numResourceInterfaces));
    EXPECT_EQ(3, numResourceInterfaces);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindContainedResourceBad)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindContainedResourceBad test");
    InitStack(OC_SERVER);

    OCResourceHandle containerHandle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&containerHandle,
                                            "core.led",
                                            "core.rw",
                                            "/a/kitchen",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    OCResourceHandle handle0;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle0,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCBindResource(containerHandle, containerHandle));

    EXPECT_EQ(OC_STACK_ERROR, OCBindResource((OCResourceHandle) 0, handle0));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindContainedResourceGood)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindContainedResourceGood test");
    InitStack(OC_SERVER);

    uint8_t numResources = 0;
    uint8_t numExpectedResources = 0;

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numExpectedResources));

    OCResourceHandle containerHandle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&containerHandle,
                                            "core.led",
                                            "core.rw",
                                            "/a/kitchen",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle0;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle0,
                                            "core.led",
                                            "core.rw",
                                            "/a/led0",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle2,
                                            "core.led",
                                            "core.rw",
                                            "/a/led2",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle3;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle3,
                                            "core.led",
                                            "core.rw",
                                            "/a/led3",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle4;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle4,
                                            "core.led",
                                            "core.rw",
                                            "/a/led4",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle5;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle5,
                                            "core.led",
                                            "core.rw",
                                            "/a/led5",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);


    EXPECT_EQ(OC_STACK_OK, OCBindResource(containerHandle, handle0));
    EXPECT_EQ(OC_STACK_OK, OCBindResource(containerHandle, handle1));
    EXPECT_EQ(OC_STACK_OK, OCBindResource(containerHandle, handle2));
    EXPECT_EQ(OC_STACK_OK, OCBindResource(containerHandle, handle3));
    EXPECT_EQ(OC_STACK_OK, OCBindResource(containerHandle, handle4));
    EXPECT_EQ(OC_STACK_OK, OCBindResource(containerHandle, handle5));

    EXPECT_EQ(handle0, OCGetResourceHandleFromCollection(containerHandle, 0));
    EXPECT_EQ(handle1, OCGetResourceHandleFromCollection(containerHandle, 1));
    EXPECT_EQ(handle2, OCGetResourceHandleFromCollection(containerHandle, 2));
    EXPECT_EQ(handle3, OCGetResourceHandleFromCollection(containerHandle, 3));
    EXPECT_EQ(handle4, OCGetResourceHandleFromCollection(containerHandle, 4));
    EXPECT_EQ(handle5, OCGetResourceHandleFromCollection(containerHandle, 5));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}


TEST(StackBind, BindEntityHandlerBad)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindEntityHandlerBad test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    EXPECT_EQ(OC_STACK_INVALID_PARAM, OCBindResourceHandler(NULL, NULL, NULL));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackBind, BindEntityHandlerGood)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting BindEntityHandlerGood test");
    InitStack(OC_SERVER);

    OCResourceHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle,
                                            "core.led",
                                            "core.rw",
                                            "/a/led",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    OCEntityHandler myHandler = entityHandler;

    EXPECT_EQ(OC_STACK_OK, OCBindResourceHandler(handle, myHandler, NULL));

    EXPECT_EQ(myHandler, OCGetResourceHandler(handle));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResourceAccess, GetResourceByIndex)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting GetResourceByIndex test");
    InitStack(OC_SERVER);

    uint8_t numResources = 0;
    uint8_t numExpectedResources = 0;
    uint8_t resourceIndex = 0;
    uint8_t prevResources = 0;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numExpectedResources));
    prevResources = numExpectedResources;
    OCResourceHandle containerHandle;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&containerHandle,
                                            "core.led",
                                            "core.rw",
                                            "/a/kitchen",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle0;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle0,
                                            "core.led",
                                            "core.rw",
                                            "/a/led0",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle2,
                                            "core.led",
                                            "core.rw",
                                            "/a/led2",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle3;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle3,
                                            "core.led",
                                            "core.rw",
                                            "/a/led3",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle4;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle4,
                                            "core.led",
                                            "core.rw",
                                            "/a/led4",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle5;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle5,
                                            "core.led",
                                            "core.rw",
                                            "/a/led5",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);
    resourceIndex += prevResources;
    EXPECT_EQ(containerHandle, OCGetResourceHandle(resourceIndex));
    EXPECT_EQ(handle0, OCGetResourceHandle(++resourceIndex));
    EXPECT_EQ(handle1, OCGetResourceHandle(++resourceIndex));
    EXPECT_EQ(handle2, OCGetResourceHandle(++resourceIndex));
    EXPECT_EQ(handle3, OCGetResourceHandle(++resourceIndex));
    EXPECT_EQ(handle4, OCGetResourceHandle(++resourceIndex));
    EXPECT_EQ(handle5, OCGetResourceHandle(++resourceIndex));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResourceAccess, DeleteHeadResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting DeleteHeadResource test");
    InitStack(OC_SERVER);

    uint8_t numResources = 0;
    uint8_t numExpectedResources = 0;

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numExpectedResources));

    OCResourceHandle handle0;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle0,
                                            "core.led",
                                            "core.rw",
                                            "/a/led0",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    EXPECT_EQ(OC_STACK_OK, OCDeleteResource(handle0));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(--numExpectedResources, numResources);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResourceAccess, DeleteHeadResource2)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting DeleteHeadResource2 test");
    InitStack(OC_SERVER);

    uint8_t numResources = 0;
    uint8_t numExpectedResources = 0;

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numExpectedResources));
    OCResourceHandle handle0;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle0,
                                            "core.led",
                                            "core.rw",
                                            "/a/led0",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    EXPECT_EQ(OC_STACK_OK, OCDeleteResource(handle0));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(--numExpectedResources, numResources);

    EXPECT_EQ(handle1, OCGetResourceHandle(numResources - 1));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}


TEST(StackResourceAccess, DeleteLastResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting DeleteLastResource test");
    InitStack(OC_SERVER);

    uint8_t numResources = 0;
    uint8_t numExpectedResources = 0;

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numExpectedResources));

    OCResourceHandle handle0;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle0,
                                            "core.led",
                                            "core.rw",
                                            "/a/led0",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    EXPECT_EQ(OC_STACK_OK, OCDeleteResource(handle1));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(--numExpectedResources, numResources);

    EXPECT_EQ(handle0, OCGetResourceHandle(numResources - 1));

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle2,
                                            "core.led",
                                            "core.rw",
                                            "/a/led2",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackResourceAccess, DeleteMiddleResource)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting DeleteMiddleResource test");
    InitStack(OC_SERVER);

    uint8_t numResources = 0;
    uint8_t numExpectedResources = 0;
    uint8_t resourceIndex = InitResourceIndex();

    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numExpectedResources));
    resourceIndex = numExpectedResources;
    OCResourceHandle handle0;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle0,
                                            "core.led",
                                            "core.rw",
                                            "/a/led0",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle2,
                                            "core.led",
                                            "core.rw",
                                            "/a/led2",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(++numExpectedResources, numResources);

    EXPECT_EQ(OC_STACK_OK, OCDeleteResource(handle1));
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResources(&numResources));
    EXPECT_EQ(--numExpectedResources, numResources);

    EXPECT_EQ(handle0, OCGetResourceHandle(resourceIndex));
    EXPECT_EQ(handle2, OCGetResourceHandle(++resourceIndex));

    // Make sure the resource elements are still correct
    uint8_t numResourceInterfaces;
    EXPECT_EQ(OC_STACK_OK, OCGetNumberOfResourceInterfaces(handle2, &numResourceInterfaces));
    EXPECT_EQ(2, numResourceInterfaces);
    const char *resourceInterfaceName = OCGetResourceInterfaceName(handle2, 1);
    EXPECT_STREQ("core.rw", resourceInterfaceName);

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

// Visual Studio versions earlier than 2015 have bugs in is_pod and report the wrong answer.
#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
TEST(PODTests, OCHeaderOption)
{
    EXPECT_TRUE(std::is_pod<OCHeaderOption>::value);
}

TEST(PODTests, OCCallbackData)
{
    EXPECT_TRUE(std::is_pod<OCCallbackData>::value);
}
#endif

TEST(OCDoDirectPairingTests, Nullpeer)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM,OCDoDirectPairing(NULL, NULL, pmSel, &pinNumber, &resultCallback));
}

TEST(OCDoDirectPairingTests, NullCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK,OCDoDirectPairing(NULL, &peer, pmSel, &pinNumber, NULL));
}

TEST(OCDoDirectPairingTests, NullpinNumber)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM,OCDoDirectPairing(NULL, &peer, pmSel, NULL, &resultCallback));
}

TEST(StackResource, MultipleResourcesDiscovery)
{
    itst::DeadmanTimer killSwitch(SHORT_TEST_TIMEOUT);
    OIC_LOG(INFO, TAG, "Starting MultipleResourcesDiscovery test");
    InitStack(OC_SERVER);

    OCResourceHandle handle1;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle1,
                                            "core.led",
                                            "core.rw",
                                            "/a/led1",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));

    OCResourceHandle handle2;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle2,
                                            "core.led",
                                            "core.rw",
                                            "/a/led2",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    OCResourceHandle handle3;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handle3,
                                            "core.led",
                                            "core.rw",
                                            "/a/led3",
                                            0,
                                            NULL,
                                            OC_DISCOVERABLE|OC_OBSERVABLE));
    /* Start a discovery query*/
    char szQueryUri[MAX_QUERY_LENGTH] = "/oic/res?if=oic.if.ll";
    OCCallbackData cbData;
    cbData.cb = discoveryCallback;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OCDoHandle handle;
    EXPECT_EQ(OC_STACK_OK, OCDoResource(&handle,
                                        OC_REST_DISCOVER,
                                        szQueryUri,
                                        0,
                                        0,
                                        CT_ADAPTER_IP,
                                        OC_LOW_QOS,
                                        &cbData,
                                        NULL,
                                        0));

    EXPECT_EQ(OC_STACK_OK, OCStop());
}

TEST(StackPayload, CloneByteString)
{
    uint8_t bytes[] = { 0, 1, 2, 3 };
    OCByteString byteString;
    byteString.bytes = bytes;
    byteString.len = sizeof(bytes);

    OCRepPayload *original = OCRepPayloadCreate();
    ASSERT_TRUE(original != NULL);
    EXPECT_TRUE(OCRepPayloadSetPropByteString(original, "name", byteString));

    OCRepPayload *clone = OCRepPayloadClone(original);
    ASSERT_TRUE(clone != NULL);

    OCRepPayloadDestroy(original);

    OCByteString cloneByteString;
    EXPECT_TRUE(OCRepPayloadGetPropByteString(clone, "name", &cloneByteString));
    ASSERT_TRUE(cloneByteString.bytes != NULL);
    EXPECT_EQ(sizeof(bytes), cloneByteString.len);
    EXPECT_TRUE(0 == memcmp(bytes, cloneByteString.bytes, sizeof(bytes)));
    OICFree(cloneByteString.bytes);

    OCRepPayloadDestroy(clone);
}

TEST(StackUri, Rfc6874_Noop_1)
{
    char validIPv6Address[] = "FF01:0:0:0:0:0:0:FB";
    char bytes[100] = {0};
    strncpy(bytes, validIPv6Address, sizeof(bytes));

    OCStackResult result = OCEncodeAddressForRFC6874(bytes, sizeof(bytes), validIPv6Address);

    // No % sign, should do nothing
    EXPECT_STREQ(bytes, validIPv6Address);
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST(StackUri, Rfc6874_Noop_2)
{
    char validIPv6Address[] = "3812:a61::4:1";
    char bytes[100] = {0};

    OCStackResult result = OCEncodeAddressForRFC6874(bytes, sizeof(bytes), validIPv6Address);

    // No % sign, should do nothing
    EXPECT_STREQ(bytes, validIPv6Address);
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST(StackUri, Rfc6874_WithEncoding)
{
    char validIPv6Address[] =        "fe80::dafe:e3ff:fe00:ebfa%wlan0";
    char validIPv6AddressEncoded[] = "fe80::dafe:e3ff:fe00:ebfa%25wlan0";
    char bytes[100] = "";
    strncpy(bytes, validIPv6Address, sizeof(bytes));

    OCStackResult result = OCEncodeAddressForRFC6874(bytes, sizeof(bytes), validIPv6Address);

    // Encoding should have occured
    EXPECT_STREQ(bytes, validIPv6AddressEncoded);
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST(StackUri, Rfc6874_WithEncoding_ExtraPercent)
{
    char validIPv6Address[] = "fe80::dafe:e3ff:fe00:ebfa%%wlan0";
    char bytes[100] = {0};

    OCStackResult result = OCEncodeAddressForRFC6874(bytes, sizeof(bytes), validIPv6Address);

    // Encoding should have failed due to extra '%' character
    EXPECT_STREQ(bytes, "");
    EXPECT_EQ(OC_STACK_ERROR, result);
}

TEST(StackUri, Rfc6874_AlreadyEncoded)
{
    char validIPv6AddressEncoded[] = "fe80::dafe:e3ff:fe00:ebfa%25wlan0";
    char bytes[100] = {0};

    OCStackResult result = OCEncodeAddressForRFC6874(bytes, sizeof(bytes), validIPv6AddressEncoded);

    // Encoding should have failed due to extra '%' character
    EXPECT_STREQ(bytes, "");
    EXPECT_EQ(OC_STACK_ERROR, result);
}

TEST(StackUri, Rfc6874_NoOverflow)
{
    char validIPv6Address[] = "fe80::dafe:e3ff:fe00:ebfa%wlan0";
    char addrBuffer[100];
    char bytes[100] = {0};
    memset(addrBuffer, sizeof(addrBuffer), '_');

    // Just enough room to encode
    addrBuffer[sizeof(addrBuffer) - sizeof(validIPv6Address) - 3] = '\0';
    strcat(addrBuffer, validIPv6Address);

    OCStackResult result = OCEncodeAddressForRFC6874(bytes, sizeof(bytes), addrBuffer);

    // Encoding should have succeeded
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST(StackUri, Rfc6874_NoOverflow_2)
{
    char validIPv6Address[] = "fe80::dafe:e3ff:fe00:ebfa%wlan0";
    char addrBuffer[100];
    char bytes[100] = {0};
    memset(addrBuffer, sizeof(addrBuffer), '_');

    // Not enough room to encode
    addrBuffer[sizeof(addrBuffer) - sizeof(validIPv6Address) - 1] = '\0';
    strcat(addrBuffer, validIPv6Address);

    OCStackResult result = OCEncodeAddressForRFC6874(bytes, sizeof(bytes), addrBuffer);

    // Encoding should have failed due to output size limitations
    EXPECT_STREQ(bytes, "");
    EXPECT_EQ(OC_STACK_ERROR, result);
}

TEST(StackHeaderOption, setHeaderOption)
{
    uint8_t optionValue1[MAX_HEADER_OPTION_DATA_LENGTH] =
    { 1 };
    OCHeaderOption options[MAX_HEADER_OPTIONS] =
    {
    { OC_COAP_ID, 6, 8, optionValue1 }, };
    uint8_t optionData = 255;
    size_t optionDataSize = sizeof(optionData);
    size_t numOptions = 1;
    uint16_t optionID = 2048;
    EXPECT_EQ(OC_STACK_OK, OCSetHeaderOption(options,
                                             &numOptions,
                                             optionID,
                                             &optionData,
                                             optionDataSize));
    EXPECT_EQ(options[1].optionID, optionID);
    EXPECT_EQ(options[1].optionData[0], 255);
}

TEST(StackHeaderOption, getHeaderOption)
{
    uint8_t optionValue1[MAX_HEADER_OPTION_DATA_LENGTH] =
    { 1 };
    uint8_t optionValue2[MAX_HEADER_OPTION_DATA_LENGTH] =
    { 255 };
    OCHeaderOption options[MAX_HEADER_OPTIONS] =
    {
    { OC_COAP_ID, 6, 8, optionValue1 },
    { OC_COAP_ID, 2048, 16, optionValue2 }, };
    uint8_t optionData[MAX_HEADER_OPTION_DATA_LENGTH];
    size_t optionDataSize = sizeof(optionData);
    size_t numOptions = 2;
    uint16_t optionID = 6;
    uint16_t actualDataSize = 0;
    EXPECT_EQ(OC_STACK_OK, OCGetHeaderOption(options,
                                             numOptions,
                                             optionID,
                                             optionData,
                                             optionDataSize,
                                             &actualDataSize));
    EXPECT_EQ(optionData[0], 1);
    EXPECT_EQ(actualDataSize, 8);
}

static OCStackApplicationResult DiscoverBaslineResource(void *ctx, OCDoHandle handle,
    OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(NULL != response->payload);
    if (NULL != response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);

        OCDiscoveryPayload *payload = (OCDiscoveryPayload *)response->payload;
        EXPECT_TRUE(NULL != payload->sid);
        if (payload->name)
        {
            EXPECT_STREQ("StackTest", payload->name);
        }
        EXPECT_STREQ(OC_RSRVD_RESOURCE_TYPE_RES, payload->type->value);
        EXPECT_STREQ(OC_RSRVD_INTERFACE_LL, payload->iface->value);
        EXPECT_STREQ(OC_RSRVD_INTERFACE_DEFAULT, payload->iface->next->value);

        for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
        {
            if (0 == strcmp("/a/light", resource->uri))
            {
                EXPECT_STREQ("/a/light", resource->uri);
                EXPECT_STREQ("core.light", resource->types->value);
                EXPECT_EQ(NULL, resource->types->next);
                EXPECT_STREQ("oic.if.baseline", resource->interfaces->value);
                EXPECT_EQ(NULL, resource->interfaces->next);
                EXPECT_TRUE(resource->bitmap & OC_DISCOVERABLE);
                EXPECT_TRUE(resource->bitmap & OC_OBSERVABLE);
                EXPECT_FALSE(resource->secure);
                EXPECT_EQ(0, resource->port);
                EXPECT_EQ(NULL, resource->next);
            }
        }
    }

    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult DiscoverLinkedListResource(void *ctx, OCDoHandle handle,
    OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(NULL != response->payload);
    if (NULL != response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);

        OCDiscoveryPayload *payload = (OCDiscoveryPayload *)response->payload;
        EXPECT_NE((char *)NULL, payload->sid);
        EXPECT_EQ(NULL, payload->name);
        EXPECT_EQ(NULL, payload->type);
        EXPECT_EQ(NULL, payload->iface);

        for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
        {
            if (0 == strcmp("/a/light", resource->uri))
            {
                EXPECT_STREQ("/a/light", resource->uri);
                EXPECT_STREQ("core.light", resource->types->value);
                EXPECT_EQ(NULL, resource->types->next);
                EXPECT_STREQ("oic.if.baseline", resource->interfaces->value);
                EXPECT_EQ(NULL, resource->interfaces->next);
                EXPECT_TRUE(resource->bitmap & OC_DISCOVERABLE);
                EXPECT_TRUE(resource->bitmap & OC_OBSERVABLE);
                EXPECT_FALSE(resource->secure);
                EXPECT_EQ(0, resource->port);
                EXPECT_EQ(NULL, resource->next);
            }
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}


static OCStackApplicationResult DiscoverResourceTypeResponse(void *ctx, OCDoHandle handle,
    OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(NULL != response->payload);
    if (NULL != response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);

        OCDiscoveryPayload *payload = (OCDiscoveryPayload *)response->payload;
        EXPECT_NE((char *)NULL, payload->sid);
        EXPECT_EQ(NULL, payload->name);
        EXPECT_EQ(NULL, payload->type);
        EXPECT_EQ(NULL, payload->iface);
        EXPECT_TRUE(NULL != payload->resources);

        OCResourcePayload *resource = payload->resources;

        if (0 == strcmp("/a/light", resource->uri))
        {
            EXPECT_STREQ("/a/light", resource->uri);
            EXPECT_STREQ("core.light", resource->types->value);
            EXPECT_EQ(NULL, resource->types->next);
            EXPECT_STREQ("oic.if.baseline", resource->interfaces->value);
            EXPECT_EQ(NULL, resource->interfaces->next);
            EXPECT_TRUE(resource->bitmap & OC_DISCOVERABLE);
            EXPECT_TRUE(resource->bitmap & OC_OBSERVABLE);
            EXPECT_FALSE(resource->secure);
            EXPECT_EQ(0, resource->port);
            EXPECT_EQ(NULL, resource->next);
        }
    }

    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult DiscoverNoResource(void *ctx, OCDoHandle handle,
    OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_EQ(OC_STACK_OK, response->result);
    EXPECT_TRUE(NULL != response->payload);
    if (NULL != response->payload)
    {
        EXPECT_EQ(PAYLOAD_TYPE_DISCOVERY, response->payload->type);

        OCDiscoveryPayload *payload = (OCDiscoveryPayload *)response->payload;
        EXPECT_EQ(NULL, payload->sid);
        EXPECT_EQ(NULL, payload->name);
        EXPECT_EQ(NULL, payload->type);
        EXPECT_EQ(NULL, payload->iface);

        bool resourceFound = false;
        for (OCResourcePayload *resource = payload->resources; resource; resource = resource->next)
        {
            if (0 == strcmp("/a/light", resource->uri))
            {
                resourceFound = true;
            }
        }

        EXPECT_FALSE(resourceFound);
    }
    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult DiscoverUnicastErrorResponse(void *ctx, OCDoHandle handle,
    OCClientResponse *response)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    EXPECT_NE(OC_STACK_OK, response->result);
    EXPECT_TRUE(NULL == response->payload);

    return OC_STACK_DELETE_TRANSACTION;
}

TEST_F(OCDiscoverTests, DiscoverResourceWithInterfaceBaselineQuery)
{
   itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, "StackTest");
    Callback discoverCB(&DiscoverBaslineResource);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=oic.if.baseline", NULL,
        0, CT_DEFAULT, OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(100);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithoutAnyQuery)
{
   itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverLinkedListResource);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res", NULL, 0, CT_DEFAULT,
        OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(100);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithInterfaceLinkedListQuery)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverLinkedListResource);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=oic.if.ll", NULL, 0,
        CT_DEFAULT, OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(100);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithResourceTypeQuery)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverResourceTypeResponse);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=core.light", NULL, 0,
        CT_DEFAULT, OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(100);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithInvalidInterfaceQuery_Unicast)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverUnicastErrorResponse);
    char targetUri[MAX_URI_LENGTH * 2] ={ 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "127.0.0.1/oic/res?if=invalid");
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, targetUri, NULL, 0,
    CT_DEFAULT, OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(100);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithoutInterfaceQuery_Unicast)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverUnicastErrorResponse);
    char targetUri[MAX_URI_LENGTH * 2] ={ 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "127.0.0.1/oic/res?if=");
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, targetUri, NULL, 0, CT_DEFAULT,
    OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(10);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithInvalidResourceTypeQuery_Unicast)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverUnicastErrorResponse);
    char targetUri[MAX_URI_LENGTH * 2] ={ 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "127.0.0.1/oic/res?rt=invalid");
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, targetUri, NULL, 0,
    CT_DEFAULT, OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(10);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithoutResourceTypeQuery_Unicast)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverUnicastErrorResponse);
    char targetUri[MAX_URI_LENGTH * 2] ={ 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "127.0.0.1/oic/res?rt=");
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, targetUri, NULL, 0, CT_DEFAULT,
    OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(10);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithInvalidResourceTypeQuery)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverUnicastErrorResponse);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=invalid", NULL, 0,
    CT_DEFAULT, OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(5);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithoutResourceTypeQuery)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverUnicastErrorResponse);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?rt=", NULL, 0, CT_DEFAULT,
    OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(5);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithInvalidInterfaceQuery)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverNoResource);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=invalid", NULL, 0,
        CT_DEFAULT, OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(5);
}

TEST_F(OCDiscoverTests, DiscoverResourceWithoutInterfaceQuery)
{
    itst::DeadmanTimer killSwitch(LONG_TEST_TIMEOUT);

    OCResourceHandle handles;
    EXPECT_EQ(OC_STACK_OK, OCCreateResource(&handles, "core.light", "oic.if.baseline", "/a/light",
        entityHandler, NULL, (OC_DISCOVERABLE | OC_OBSERVABLE)));
    OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, "deviceName", "StackTest");
    Callback discoverCB(&DiscoverNoResource);
    EXPECT_EQ(OC_STACK_OK, OCDoResource(NULL, OC_REST_DISCOVER, "/oic/res?if=", NULL, 0, CT_DEFAULT,
        OC_HIGH_QOS, discoverCB, NULL, 0));
    discoverCB.Wait(5);
}
