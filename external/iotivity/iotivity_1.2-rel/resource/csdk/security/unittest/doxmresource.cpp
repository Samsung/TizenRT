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
// http://www.apache.org/licenses/LICENSE-2.0
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
#include "ocserverrequest.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "resourcemanager.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "doxmresource.h"
#include "security_internals.h"

#define TAG  "SRM-DOXM"

OicSecDoxm_t * getBinDoxm()
{
    OicSecDoxm_t *doxm = (OicSecDoxm_t *)OICCalloc(1, sizeof(*doxm));
    if (!doxm)
    {
        return NULL;
    }
    doxm->oxmTypeLen =  1;
    doxm->oxmType = (OicUrn_t *)OICCalloc(doxm->oxmTypeLen, sizeof(*doxm->oxmType));
    if (!doxm->oxmType)
    {
        OICFree(doxm);
        return NULL;
    }
    doxm->oxmType[0] = (char *) OICMalloc(strlen(OXM_JUST_WORKS) + 1);
    if (!doxm->oxmType[0])
    {
        OICFree(doxm->oxmType);
        OICFree(doxm);
        return NULL;
    }

    OICStrcpy(doxm->oxmType[0], strlen(OXM_JUST_WORKS) + 1, OXM_JUST_WORKS);
    doxm->oxmLen     = 1;
    doxm->oxm        = (OicSecOxm_t *)OICCalloc(doxm->oxmLen, sizeof(OicSecOxm_t));
    if(!doxm->oxm)
    {
        OICFree(doxm->oxmType[0]);
        OICFree(doxm->oxmType);
        OICFree(doxm);
        return NULL;
    }

    doxm->oxm[0]     = OIC_JUST_WORKS;
    doxm->oxmSel     = OIC_JUST_WORKS;
    doxm->sct        = SYMMETRIC_PAIR_WISE_KEY;
    doxm->owned      = true;
    //TODO: Need more clarification on deviceIDFormat field type.
    //doxm.deviceIDFormat = URN;

    uint8_t deviceId[] = {0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x49, 0x64};
    memcpy(doxm->deviceID.id, deviceId, sizeof(deviceId));
    uint8_t ownerId[] = {0x6f, 0x77, 0x6e, 0x65, 0x72, 0x73, 0x49, 0x64};
    memcpy(doxm->owner.id, ownerId, sizeof(ownerId));
    return doxm;
}

 //InitDoxmResource Tests
TEST(DoxmResourceTest, InitDoxmResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, InitDoxmResource());
}

//DeInitDoxmResource Tests
TEST(DoxmResourceTest, DeInitDoxmResource)
{
    EXPECT_EQ(OC_STACK_ERROR, DeInitDoxmResource());
}

//CreateDoxmResource Tests
TEST(DoxmResourceTest, CreateDoxmResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, CreateDoxmResource());
}

 //DoxmEntityHandler Tests
TEST(DoxmResourceTest, DoxmEntityHandlerWithDummyRequest)
{
    OCEntityHandlerRequest req = OCEntityHandlerRequest();
    EXPECT_EQ(OC_EH_ERROR, DoxmEntityHandler(OCEntityHandlerFlag::OC_REQUEST_FLAG, &req, NULL));
}

TEST(DoxmResourceTest, DoxmEntityHandlerWithNULLRequest)
{
    EXPECT_EQ(OC_EH_ERROR, DoxmEntityHandler(OCEntityHandlerFlag::OC_REQUEST_FLAG, NULL, NULL));
}

TEST(DoxmResourceTest, DoxmEntityHandlerInvalidFlag)
{
    OCEntityHandlerRequest req = OCEntityHandlerRequest();
    EXPECT_EQ(OC_EH_ERROR, DoxmEntityHandler(OCEntityHandlerFlag::OC_OBSERVE_FLAG, &req, NULL));
}

TEST(DoxmResourceTest, DoxmEntityHandlerValidRequest)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, InitDoxmResource());
    char query[] = "oxm=0;owned=false;owner=owner1";
    OCEntityHandlerRequest req = OCEntityHandlerRequest();
    req.method = OC_REST_GET;
    req.query = OICStrdup(query);
    EXPECT_EQ(OC_EH_ERROR, DoxmEntityHandler(OCEntityHandlerFlag::OC_REQUEST_FLAG, &req, NULL));

    OICFree(req.query);
}

TEST(DoxmResourceTest, DoxmEntityHandlerDeviceIdQuery)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, InitDoxmResource());
    char query[] = "deviceuuid=2222222222222222";
    OCEntityHandlerRequest req = OCEntityHandlerRequest();
    req.method = OC_REST_GET;
    req.query = OICStrdup(query);
    EXPECT_EQ(OC_EH_ERROR, DoxmEntityHandler(OCEntityHandlerFlag::OC_REQUEST_FLAG, &req, NULL));

    OICFree(req.query);
}

TEST(DoxmResourceTest, DoxmToCBORPayloadNULL)
{
    OicSecDoxm_t *doxm =  getBinDoxm();
    size_t size = 10;
    uint8_t *payload = NULL;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(NULL, NULL, 0, false));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(doxm, NULL, &size, false));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(doxm, &payload, 0, false));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(NULL, &payload, &size, false));
    DeleteDoxmBinData(doxm);
}

TEST(DoxmResourceTest, DoxmToCBORPayloadVALID)
{
    OicSecDoxm_t *doxm =  getBinDoxm();

    uint8_t *payload = NULL;
    size_t size = 0;
    EXPECT_EQ(OC_STACK_OK, DoxmToCBORPayload(doxm, &payload, &size, false));
    EXPECT_TRUE(payload != NULL);

    DeleteDoxmBinData(doxm);
    OICFree(payload);
}

//CBORPayloadToDoxm Tests
TEST(DoxmResourceTest, CBORPayloadToDoxmNULL)
{
    OicSecDoxm_t *doxm = NULL;
    uint8_t *cborPayload = (uint8_t *)OICCalloc(1, sizeof(uint8_t));
    size_t size = 10;
    ASSERT_TRUE(NULL != cborPayload);
    EXPECT_EQ(OC_STACK_INVALID_PARAM, CBORPayloadToDoxm(NULL, 0, NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, CBORPayloadToDoxm(NULL, size, &doxm));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, CBORPayloadToDoxm(cborPayload, size, NULL));
    OICFree(cborPayload);
}

TEST(DoxmResourceTest, CBORPayloadToDoxmVALID)
{
    OicSecDoxm_t *doxm =  getBinDoxm();
    uint8_t *payload = NULL;
    size_t size = 0;
    EXPECT_EQ(OC_STACK_OK, DoxmToCBORPayload(doxm, &payload, &size, false));
    EXPECT_TRUE(payload != NULL);

    OicSecDoxm_t *doxmSec = NULL;
    EXPECT_EQ(OC_STACK_OK, CBORPayloadToDoxm(payload, size, &doxmSec));
    ASSERT_TRUE(doxmSec != NULL);
    EXPECT_EQ(doxmSec->oxmTypeLen, doxm->oxmTypeLen);
    EXPECT_STREQ(doxmSec->oxmType[0], doxm->oxmType[0]);
    EXPECT_EQ(doxmSec->oxmLen, doxm->oxmLen);
    EXPECT_EQ(doxmSec->oxm[0], doxm->oxm[0]);
    EXPECT_EQ(doxmSec->oxmSel, doxm->oxmSel);
    EXPECT_EQ(doxmSec->sct, doxm->sct);
    EXPECT_EQ(doxmSec->owned, doxm->owned);

    DeleteDoxmBinData(doxmSec);
    DeleteDoxmBinData(doxm);
    OICFree(payload);
}

#if 0
//HandleDoxmPostRequest Test
TEST(HandleDoxmPostRequestTest, HandleDoxmPostRequestValidInput)
{
    OCEntityHandlerRequest ehRequest = {};
    OCServerRequest svRequest = {};

    OicSecDoxm_t * doxm =  getBinDoxm();

    strcpy(svRequest.addressInfo.IP.ipAddress, "10.10.10.10");
    svRequest.addressInfo.IP.port = 2345;
    svRequest.connectivityType = CA_ETHERNET;

    ehRequest.reqJSONPayload = (unsigned char *) BinToDoxmJSON(doxm);
    ehRequest.requestHandle = (OCRequestHandle) &svRequest;

    EXPECT_EQ(OC_EH_ERROR, HandleDoxmPostRequest(&ehRequest));
    DeleteDoxmBinData(doxm);
    OICFree(ehRequest.reqJSONPayload);
}
#endif
