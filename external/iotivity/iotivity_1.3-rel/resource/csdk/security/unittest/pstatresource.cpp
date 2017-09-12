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

#include "iotivity_config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <gtest/gtest.h>

#include "ocpayload.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "cainterface.h"
#include "resourcemanager.h"
#include "secureresourcemanager.h"
#include "pstatresource.h"
#include "security_internals.h"

// InitPstatResource Tests
TEST(PstatResourceTest, InitPstatResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, InitPstatResource());
}

// DeInitPstatResource Tests
TEST(PstatResourceTest, DeInitPstatResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DeInitPstatResource());
}

//CreatePstatResource Tests
TEST(PstatResourceTest, CreatePstatResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, CreatePstatResource());
}

//PstatEntityHandler Tests
TEST(PstatResourceTest, PstatEntityHandlerWithDummyRequest)
{
    OCEntityHandlerRequest req = OCEntityHandlerRequest();
    EXPECT_EQ(OC_EH_ERROR, PstatEntityHandler(OCEntityHandlerFlag::OC_REQUEST_FLAG, &req));
}

TEST(PstatResourceTest, PstatEntityHandlerWithPostRequest)
{
    OicSecPstat_t *defaultPstat = (OicSecPstat_t *) OICCalloc(1, sizeof(*defaultPstat));
    ASSERT_TRUE(defaultPstat != NULL);
    defaultPstat->dos.state = DOS_RFOTM;
    defaultPstat->dos.pending = false;
    defaultPstat->isOp = false;
    defaultPstat->commitHash = 1234;
    defaultPstat->cm = 63;
    defaultPstat->tm = 48;
    defaultPstat->om = 0;
    defaultPstat->smLen = 1;
    defaultPstat->sm = (OicSecDpom_t *)OICCalloc(defaultPstat->smLen, sizeof(*defaultPstat->sm));
    ASSERT_TRUE(defaultPstat->sm != NULL);
    defaultPstat->sm[0] = 3;
    size_t size = 0;
    uint8_t *cbor = NULL;
    bool propertiesToInclude[PSTAT_PROPERTY_COUNT] = {
                                                      true,     // dos
                                                      false,    // isop
                                                      false,    // cm
                                                      true,     // tm
                                                      false,    // om
                                                      true,    // sm
                                                      true      // rowneruuid
                                                    };
    EXPECT_EQ(OC_STACK_OK, PstatToCBORPayloadPartial(defaultPstat, &cbor, &size, propertiesToInclude));
    DeletePstatBinData(defaultPstat);
    ASSERT_TRUE(cbor != NULL);

    OCEntityHandlerRequest req = OCEntityHandlerRequest();
    req.method = OC_REST_POST;
    req.payload = (OCPayload *) OCSecurityPayloadCreate(cbor, size);
    EXPECT_EQ(OC_EH_ERROR, PstatEntityHandler(OCEntityHandlerFlag::OC_REQUEST_FLAG, &req));
    OICFree(cbor);
    OCPayloadDestroy(req.payload);
}

TEST(PstatResourceTest, PstatEntityHandlerInvalidRequest)
{
    EXPECT_EQ(OC_EH_ERROR, PstatEntityHandler(OCEntityHandlerFlag::OC_OBSERVE_FLAG, NULL));
}

TEST(PstatResourceTest, PstatToCBORPayloadNULL)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PstatToCBORPayload(NULL, NULL, 0));
    // Case when cbor payload is NULL
    OicSecPstat_t pstat;
    size_t size = 10;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PstatToCBORPayload(&pstat, NULL, &size));
    uint8_t *cborPayload = (uint8_t *) OICCalloc(1, size);
    ASSERT_TRUE(NULL != cborPayload);
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PstatToCBORPayload(&pstat, &cborPayload, &size));
    OICFree(cborPayload);
    cborPayload = NULL;
    // Case when pstat is zero.
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PstatToCBORPayload(NULL, &cborPayload, &size));
    // Case when size is 0.
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PstatToCBORPayload(&pstat, &cborPayload, 0));
    OICFree(cborPayload);
}

TEST(PstatResourceTest, CBORPayloadToPstat)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, CBORPayloadToPstat(NULL, 0, NULL));
}

TEST(PstatResourceTest, PstatToCBORPayloadAndCBORPayloadToPstat)
{
    OicSecPstat_t pstat;
    pstat.dos.state = DOS_RFNOP;
    pstat.dos.pending = false;
    pstat.cm = NORMAL;
    pstat.commitHash = 0;
    pstat.isOp = true;
    pstat.tm = NORMAL;
    pstat.om = SINGLE_SERVICE_CLIENT_DRIVEN;
    pstat.smLen = 1;
    pstat.sm = (OicSecDpom_t*)OICCalloc(pstat.smLen, sizeof(*pstat.sm));
    ASSERT_TRUE(NULL != pstat.sm);
    pstat.sm[0] = SINGLE_SERVICE_CLIENT_DRIVEN;

    size_t size = 0;
    uint8_t *cbor = NULL;
    EXPECT_EQ(OC_STACK_OK, PstatToCBORPayload(&pstat, &cbor, &size));
    if (!cbor)
    {
        OICFree(pstat.sm);
        FAIL() << "Failed to convert PstatToCBORPayload";
        return;
    }
    ASSERT_TRUE(NULL != cbor);
    OicSecPstat_t *pstat1 = NULL;
    EXPECT_EQ(OC_STACK_OK, CBORPayloadToPstat(cbor, size, &pstat1));
    ASSERT_TRUE(NULL != pstat1);
    EXPECT_EQ(pstat.dos.state, pstat1->dos.state);
    EXPECT_EQ(pstat.dos.pending, pstat1->dos.pending);
    EXPECT_EQ(pstat.commitHash, pstat1->commitHash);
    EXPECT_EQ(pstat.isOp, pstat1->isOp);
    EXPECT_EQ(pstat.tm, pstat1->tm);
    EXPECT_EQ(pstat.om, pstat1->om);
    EXPECT_EQ(pstat.smLen, pstat1->smLen);
    EXPECT_EQ(pstat.sm[0], pstat1->sm[0]);

    DeletePstatBinData(pstat1);
    OICFree(cbor);
    OICFree(pstat.sm);
}
