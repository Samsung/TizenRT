//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
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
#include "ocstack.h"
#include "oic_malloc.h"
#include "cainterface.h"
#include "secureresourcemanager.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "svcresource.h"
#include "srmtestcommon.h"
#include "security_internals.h"

using namespace std;

#define NUM_SVC_IN_CBOR_DB (2)

TEST(SVCResourceTest, CBORConversionTests)
{
    OicSecSvc_t *svc1 = (OicSecSvc_t *) OICCalloc(1, sizeof(*svc1));
    ASSERT_TRUE(NULL != svc1);
    uint8_t svcdid[] = {0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,
                        0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35};
    memcpy(svc1->svcdid.id, svcdid, sizeof(svcdid));
    ASSERT_EQ(sizeof(svc1->svcdid.id), sizeof(svcdid));

    svc1->svct = (OicSecSvcType_t) 1;
    uint8_t owners[] = {0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39,
                        0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39};
    svc1->ownersLen = 1;
    svc1->owners = (OicUuid_t *)OICCalloc(svc1->ownersLen, sizeof(*svc1->owners));
    ASSERT_TRUE(NULL != svc1->owners);
    memcpy(svc1->owners[0].id, owners, sizeof(owners));
    ASSERT_EQ(sizeof(svc1->owners[0].id), sizeof(owners));

    svc1->next = (OicSecSvc_t *) OICCalloc(1, sizeof(*svc1->next));
    ASSERT_TRUE(svc1->next != NULL);
    memcpy(svc1->next->svcdid.id, svcdid, sizeof(svcdid));
    ASSERT_EQ(sizeof(svc1->next->svcdid.id), sizeof(svcdid));
    svc1->next->svct = (OicSecSvcType_t) 1;
    uint8_t owners1[] = {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
                        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36};
    svc1->next->ownersLen = 1;
    svc1->next->owners = (OicUuid_t *)OICCalloc(svc1->next->ownersLen,
                                                sizeof(*svc1->next->owners));
    ASSERT_TRUE(NULL != svc1->next->owners);
    memcpy(svc1->next->owners[0].id, owners1, sizeof(owners1));
    svc1->next->next = NULL;

    size_t size = 0;
    uint8_t *psStorage = NULL;
    EXPECT_EQ(OC_STACK_OK, SVCToCBORPayload(svc1, &psStorage, &size));
    ASSERT_TRUE(NULL != psStorage);

    OicSecSvc_t *svc = NULL;
    EXPECT_EQ(OC_STACK_OK, CBORPayloadToSVC(psStorage, size, &svc));
    ASSERT_TRUE(NULL != svc);

    int cnt = 0;
    OicSecSvc_t *tempSvc = svc;
    while (tempSvc)
    {
        EXPECT_EQ(ACCESS_MGMT_SERVICE, tempSvc->svct);
        cnt++;
        tempSvc = tempSvc->next;
    }
    EXPECT_EQ(NUM_SVC_IN_CBOR_DB, cnt);

    OICFree(psStorage);
    DeleteSVCList(svc);
    DeleteSVCList(svc1);
}
