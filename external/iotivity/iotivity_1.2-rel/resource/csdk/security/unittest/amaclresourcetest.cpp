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

#include "gtest/gtest.h"
#include "cainterface.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "payload_logging.h"
#include "psinterface.h"
#include "secureresourcemanager.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "srmutility.h"
#include "amaclresource.h"
#include "security_internals.h"

using namespace std;

#define TAG  "SRM-AMACL-UT"

TEST(AMACLResourceTest, CBORAMACLConversion)
{
    OicSecAmacl_t *secAmacl = (OicSecAmacl_t *) OICCalloc(1, sizeof(*secAmacl));
    ASSERT_TRUE(NULL != secAmacl);
    uint8_t amss[] = {0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,
        0x35, 0x35, 0x35, 0x35, 0x35, 0x35};
    uint8_t amss1[] = {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36};
    secAmacl->amssLen = 2;
    secAmacl->amss = (OicUuid_t *)OICCalloc(secAmacl->amssLen, sizeof(*secAmacl->amss));
    if (!secAmacl->amss)
    {
        DeleteAmaclList(secAmacl);
    }
    ASSERT_TRUE(NULL != secAmacl->amss);
    memcpy(secAmacl->amss[0].id, amss, sizeof(amss));
    memcpy(secAmacl->amss[1].id, amss1, sizeof(amss1));

    const char *rsrc[] = { "/a/led", "/a/fan"};
    secAmacl->resourcesLen = 2;
    secAmacl->resources = (char **)OICCalloc(secAmacl->resourcesLen,
                          sizeof(*secAmacl->resources));
    if (!secAmacl->resources)
    {
        DeleteAmaclList(secAmacl);
    }
    ASSERT_TRUE(NULL != secAmacl->resources);
    for (size_t i = 0 ; i < secAmacl->resourcesLen; i++)
    {
        secAmacl->resources[i] = OICStrdup(rsrc[i]);
        ASSERT_TRUE(NULL != secAmacl->resources[i]);
    }
    uint8_t ownrs[] = {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32};
    memcpy(secAmacl->rownerID.id, ownrs, sizeof(ownrs));

    OicSecAmacl_t *secAmacl1 = (OicSecAmacl_t *) OICCalloc(1, sizeof(*secAmacl1));
    if (!secAmacl1)
    {
        DeleteAmaclList(secAmacl);
    }
    ASSERT_TRUE(NULL != secAmacl1);
    secAmacl1->amssLen = 2;
    secAmacl1->amss = (OicUuid_t *)OICCalloc(2, sizeof(*secAmacl1->amss));
    if (!secAmacl1->amss)
    {
        DeleteAmaclList(secAmacl);
        DeleteAmaclList(secAmacl1);
    }
    ASSERT_TRUE(NULL != secAmacl1->amss);
    memcpy(secAmacl1->amss[0].id, amss, sizeof(amss));
    memcpy(secAmacl1->amss[1].id, amss1, sizeof(amss1));

    const char *rsrc1[] = { "/b/led", "/b/fan"};
    secAmacl1->resourcesLen = 2;
    secAmacl1->resources = (char **)OICCalloc(secAmacl1->resourcesLen,
                            sizeof(*secAmacl1->resources));
    if (!secAmacl1->resources)
    {
        DeleteAmaclList(secAmacl);
        DeleteAmaclList(secAmacl1);
    }
    ASSERT_TRUE(NULL != secAmacl1->resources);
    for (size_t i = 0 ; i < secAmacl1->resourcesLen; i++)
    {
        secAmacl1->resources[i] = OICStrdup(rsrc1[i]);
        ASSERT_TRUE(NULL != secAmacl1->resources[i]);
    }
    memcpy(secAmacl1->rownerID.id, ownrs, sizeof(ownrs));
    secAmacl1->next = NULL;
    secAmacl->next = secAmacl1;

    size_t size = 0;
    uint8_t *psStorage = NULL;
    EXPECT_EQ(OC_STACK_OK, AmaclToCBORPayload(secAmacl, &psStorage, &size));
    if (!psStorage)
    {
        DeleteAmaclList(secAmacl);
    }
    ASSERT_TRUE(NULL != psStorage);

    OicSecAmacl_t *amacl = NULL;
    EXPECT_EQ(OC_STACK_OK, CBORPayloadToAmacl(psStorage, size, &amacl));
    if (!amacl)
    {
        DeleteAmaclList(secAmacl);
        OICFree(psStorage);
    }
    ASSERT_TRUE(NULL != amacl);

    EXPECT_EQ(secAmacl->amssLen, amacl->amssLen);
    EXPECT_EQ(sizeof(secAmacl->amss[0].id), sizeof(amacl->amss[0].id));
    EXPECT_EQ(sizeof(secAmacl->amss[1].id), sizeof(amacl->amss[1].id));
    EXPECT_STREQ(secAmacl->resources[0], amacl->resources[0]);
    EXPECT_STREQ(secAmacl->resources[1], amacl->resources[1]);
    EXPECT_EQ(secAmacl->resourcesLen, amacl->resourcesLen);
    EXPECT_EQ(memcmp(secAmacl->rownerID.id, amacl->rownerID.id, sizeof(amacl->rownerID.id)), 0);

    DeleteAmaclList(secAmacl);
    DeleteAmaclList(amacl);
    OICFree(psStorage);
}
