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
#include "ocrandom.h"

#define TAG  "SRM-DOXM"

OicSecDoxm_t * getBinDoxm()
{
    OicSecDoxm_t *doxm = (OicSecDoxm_t *)OICCalloc(1, sizeof(*doxm));
    if (!doxm)
    {
        return NULL;
    }

    doxm->oxmLen     = 1;
    doxm->oxm        = (OicSecOxm_t *)OICCalloc(doxm->oxmLen, sizeof(OicSecOxm_t));
    if(!doxm->oxm)
    {
        OICFree(doxm);
        return NULL;
    }

    doxm->oxm[0]     = OIC_JUST_WORKS;
    doxm->oxmSel     = OIC_JUST_WORKS;
    doxm->sct        = SYMMETRIC_PAIR_WISE_KEY;
    doxm->owned      = true;

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
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(NULL, NULL, 0));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(doxm, NULL, &size));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(doxm, &payload, 0));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DoxmToCBORPayload(NULL, &payload, &size));
    DeleteDoxmBinData(doxm);
}

TEST(DoxmResourceTest, DoxmToCBORPayloadVALID)
{
    OicSecDoxm_t *doxm = getBinDoxm();

    uint8_t *payload = NULL;
    size_t size = 0;
    bool propertiesToInclude[DOXM_PROPERTY_COUNT];
    memset(propertiesToInclude, 0, sizeof(propertiesToInclude));
    propertiesToInclude[DOXM_OXMS] = true;
    propertiesToInclude[DOXM_OXMSEL] = true;
    propertiesToInclude[DOXM_SCT] = true;
    propertiesToInclude[DOXM_OWNED] = true;
    propertiesToInclude[DOXM_DEVICEUUID] = true;
    propertiesToInclude[DOXM_DEVOWNERUUID] = true;
    EXPECT_EQ(OC_STACK_OK, DoxmToCBORPayloadPartial(doxm, &payload, &size, propertiesToInclude));
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
    EXPECT_EQ(OC_STACK_OK, DoxmToCBORPayload(doxm, &payload, &size));
    EXPECT_TRUE(payload != NULL);

    OicSecDoxm_t *doxmSec = NULL;
    EXPECT_EQ(OC_STACK_OK, CBORPayloadToDoxm(payload, size, &doxmSec));
    ASSERT_TRUE(doxmSec != NULL);
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

#ifdef MULTIPLE_OWNER
typedef struct _DoxmMotParameters
{
    OicUuid_t m_uuid;
    MotStatus_t m_status;
} DoxmMotParameters;
#endif

typedef struct _DoxmTestParameter
{
    std::vector<OicSecOxm_t>        m_oxmArray;
    OicSecOxm_t                     m_oxmSel;
    OicSecCredType_t                m_sct;
    bool                            m_owned;
    OicUuid_t                       m_deviceID;
    bool                            m_dpc;
    OicUuid_t                       m_owner;

#ifdef MULTIPLE_OWNER
    std::vector<DoxmMotParameters>  m_subOwnersList;
    OicSecMomType_t                 m_mom;
#endif

    OicUuid_t                       m_rownerID;
} DoxmTestParameter;

class DoxmComparisonTests : public testing::Test
{
public:
    DoxmComparisonTests() : m_doxm1(nullptr), m_doxm2(nullptr)
    {};

protected:
    virtual void TearDown()
    {
        DeleteDoxmBinData(m_doxm1);
        DeleteDoxmBinData(m_doxm2);
    }

    void GenerateDoxmTestParameterUUIDs(DoxmTestParameter *param)
    {
        ASSERT_TRUE(OCGenerateUuid(param->m_deviceID.id));
        ASSERT_TRUE(OCGenerateUuid(param->m_owner.id));
        ASSERT_TRUE(OCGenerateUuid(param->m_rownerID.id));
    }

    void DoxmBinFromParameters(const DoxmTestParameter &param1, const DoxmTestParameter &param2)
    {
        DoxmBinFromParameter(param1, &m_doxm1);
        DoxmBinFromParameter(param2, &m_doxm2);
    }

    OicSecDoxm_t *m_doxm1;
    OicSecDoxm_t *m_doxm2;

private:
    void DoxmBinFromParameter(const DoxmTestParameter &param, OicSecDoxm_t **doxmBin);
};

void DoxmComparisonTests::DoxmBinFromParameter(const DoxmTestParameter &param, OicSecDoxm_t **doxmBin)
{
    OicSecDoxm_t *newDoxmBin = (OicSecDoxm_t *)OICCalloc(1, sizeof(OicSecDoxm_t));
    ASSERT_NE(nullptr, newDoxmBin);

    // oxm and oxmLen.
    size_t arrayLength = param.m_oxmArray.size();
    if (arrayLength == 0)
    {
        newDoxmBin->oxm = nullptr;
    }
    else
    {
        newDoxmBin->oxm = (OicSecOxm_t *)OICCalloc(sizeof(OicSecOxm_t), arrayLength);
        ASSERT_NE(nullptr, newDoxmBin->oxm);

        for (size_t i = 0; i < arrayLength; i++)
        {
            newDoxmBin->oxm[i] = param.m_oxmArray[i];
        }
    }
    newDoxmBin->oxmLen = arrayLength;

    // Other doxm fields.
    newDoxmBin->oxmSel = param.m_oxmSel;
    newDoxmBin->sct = param.m_sct;
    newDoxmBin->owned = param.m_owned;
    newDoxmBin->deviceID = param.m_deviceID;
    newDoxmBin->dpc = param.m_dpc;
    newDoxmBin->owner = param.m_owner;
    newDoxmBin->rownerID = param.m_rownerID;

    // doxm MOT fields.
#ifdef MULTIPLE_OWNER
    newDoxmBin->subOwners = nullptr;
    arrayLength = param.m_subOwnersList.size();
    for (size_t i = 0; i < arrayLength; i++)
    {
        OicSecSubOwner_t *newSubOwner = (OicSecSubOwner_t *)OICCalloc(1, sizeof(OicSecSubOwner_t));
        ASSERT_NE(nullptr, newSubOwner);

        newSubOwner->uuid = param.m_subOwnersList[i].m_uuid;
        newSubOwner->status = param.m_subOwnersList[i].m_status;
        newSubOwner->next = newDoxmBin->subOwners;
        newDoxmBin->subOwners = newSubOwner;
    }

    newDoxmBin->mom = (OicSecMom_t *)OICCalloc(1, sizeof(OicSecMom_t));
    ASSERT_NE(nullptr, newDoxmBin->mom);
    newDoxmBin->mom->mode = param.m_mom;
#endif

    *doxmBin = newDoxmBin;
}

TEST_F(DoxmComparisonTests, Equal1)
{
#ifdef MULTIPLE_OWNER
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_IN_PROGRESS };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));
#endif

    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {mot1}, // m_rownerID
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
#endif
        {0}
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    DoxmBinFromParameters(param1, param2);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, Equal2)
{
#ifdef MULTIPLE_OWNER
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_IN_PROGRESS };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));
    DoxmMotParameters mot2 = { {0}, MOT_STATUS_READY };
    ASSERT_TRUE(OCGenerateUuid(mot2.m_uuid.id));
    DoxmMotParameters mot3 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot3.m_uuid.id));
    DoxmMotParameters mot4 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot4.m_uuid.id));
#endif

    DoxmTestParameter param1 =
    {
        {OIC_RANDOM_DEVICE_PIN, OIC_JUST_WORKS},
        OIC_RANDOM_DEVICE_PIN,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {mot1, mot2, mot3, mot4},
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    DoxmBinFromParameters(param1, param2);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, Equal3)
{
#ifdef MULTIPLE_OWNER
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_IN_PROGRESS };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));
#endif

    DoxmTestParameter param1 =
    {
        {OIC_RANDOM_DEVICE_PIN, OIC_JUST_WORKS},
        OIC_RANDOM_DEVICE_PIN,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {mot1},
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    DoxmBinFromParameters(param1, param2);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, Equal4)
{
    DoxmTestParameter param1 =
    {
        {OIC_RANDOM_DEVICE_PIN, OIC_JUST_WORKS},
        OIC_RANDOM_DEVICE_PIN,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    DoxmBinFromParameters(param1, param2);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, Equal5)
{
    DoxmTestParameter param1 =
    {
        {OIC_RANDOM_DEVICE_PIN, OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_GROUP_KEY,
        true,
        {0},    // m_deviceID
        false,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_DISABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    DoxmBinFromParameters(param1, param2);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, Equal6)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE},
        OIC_JUST_WORKS,
        SYMMETRIC_GROUP_KEY,
        false,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    DoxmBinFromParameters(param1, param2);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, oxmMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE},
        OIC_JUST_WORKS,
        SYMMETRIC_GROUP_KEY,
        false,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param2.m_oxmArray[0] = OIC_CON_MFG_CERT;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, oxmLenMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE},
        OIC_JUST_WORKS,
        SYMMETRIC_GROUP_KEY,
        false,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param1.m_oxmArray.push_back(OIC_JUST_WORKS);

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, oxmSelMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE, OIC_JUST_WORKS, OIC_DECENTRALIZED_PUBLIC_KEY, OIC_RANDOM_DEVICE_PIN},
        OIC_RANDOM_DEVICE_PIN,
        SYMMETRIC_GROUP_KEY,
        false,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param2.m_oxmSel = OIC_JUST_WORKS;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, sctMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE, OIC_JUST_WORKS, OIC_DECENTRALIZED_PUBLIC_KEY, OIC_RANDOM_DEVICE_PIN},
        OIC_RANDOM_DEVICE_PIN,
        ASYMMETRIC_KEY,
        false,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param1.m_sct = SYMMETRIC_PAIR_WISE_KEY;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, ownedMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE, OIC_JUST_WORKS, OIC_DECENTRALIZED_PUBLIC_KEY, OIC_RANDOM_DEVICE_PIN},
        OIC_RANDOM_DEVICE_PIN,
        ASYMMETRIC_KEY,
        false,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param2.m_owned = !param2.m_owned;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, deviceIDMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE, OIC_DECENTRALIZED_PUBLIC_KEY, OIC_RANDOM_DEVICE_PIN},
        OIC_MANUFACTURER_CERTIFICATE,
        PIN_PASSWORD,
        false,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_DISABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    ASSERT_TRUE(OCGenerateUuid(param2.m_deviceID.id));

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, dpcMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_MANUFACTURER_CERTIFICATE, OIC_DECENTRALIZED_PUBLIC_KEY, OIC_RANDOM_DEVICE_PIN},
        OIC_MANUFACTURER_CERTIFICATE,
        PIN_PASSWORD,
        true,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_DISABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param1.m_dpc = !param1.m_dpc;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, ownerMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_RANDOM_DEVICE_PIN},
        OIC_RANDOM_DEVICE_PIN,
        PIN_PASSWORD,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_DISABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    ASSERT_TRUE(OCGenerateUuid(param2.m_owner.id));

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, rownerIDMismatch)
{
    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS, OIC_RANDOM_DEVICE_PIN},
        OIC_JUST_WORKS,
        ASYMMETRIC_KEY,
        true,
        {0},    // m_deviceID
        true,
        {0},    // m_owner

#ifdef MULTIPLE_OWNER
        {},
        OIC_MULTIPLE_OWNER_DISABLE,
#endif
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    ASSERT_TRUE(OCGenerateUuid(param1.m_rownerID.id));

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

#ifdef MULTIPLE_OWNER
TEST_F(DoxmComparisonTests, subOwnersLengthMismatch1)
{
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_READY };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));

    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner
        {mot1},
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param1.m_subOwnersList.erase(param1.m_subOwnersList.begin());

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, subOwnersLengthMismatch2)
{
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_IN_PROGRESS };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));
    DoxmMotParameters mot2 = { {0}, MOT_STATUS_READY };
    ASSERT_TRUE(OCGenerateUuid(mot2.m_uuid.id));
    DoxmMotParameters mot3 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot3.m_uuid.id));
    DoxmMotParameters mot4 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot4.m_uuid.id));

    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner
        {mot1, mot2, mot3},
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param2.m_subOwnersList.insert(param2.m_subOwnersList.begin(), mot4);

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, subOwnerIDMismatch)
{
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));
    DoxmMotParameters mot2 = { {0}, MOT_STATUS_READY };
    ASSERT_TRUE(OCGenerateUuid(mot2.m_uuid.id));
    DoxmMotParameters mot3 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot3.m_uuid.id));
    DoxmMotParameters mot4 = { {0}, MOT_STATUS_READY };
    ASSERT_TRUE(OCGenerateUuid(mot4.m_uuid.id));

    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner
        {mot1, mot2, mot3, mot4},
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;

    DoxmMotParameters mot5 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot5.m_uuid.id));
    param1.m_subOwnersList[2] = mot5;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, subOwnerStateMismatch)
{
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_DONE };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));

    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner
        {mot1},
        OIC_MULTIPLE_OWNER_TIMELY_ENABLE,
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;

    DoxmMotParameters mot2 = mot1;
    mot2.m_status = MOT_STATUS_READY;
    param2.m_subOwnersList[0] = mot2;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, subOwnerMomMismatch)
{
    DoxmMotParameters mot1 = { {0}, MOT_STATUS_READY };
    ASSERT_TRUE(OCGenerateUuid(mot1.m_uuid.id));
    DoxmMotParameters mot2 = { {0}, MOT_STATUS_IN_PROGRESS };
    ASSERT_TRUE(OCGenerateUuid(mot2.m_uuid.id));

    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        {0},    // m_deviceID
        false,
        {0},    // m_owner
        {mot1, mot2},
        OIC_MULTIPLE_OWNER_DISABLE,
        {0}     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;
    param1.m_mom = OIC_MULTIPLE_OWNER_ENABLE;

    DoxmBinFromParameters(param1, param2);

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, createMom)
{
    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        { 0 },    // m_deviceID
        false,
        { 0 },    // m_owner
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
        { 0 }     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;

    DoxmBinFromParameters(param1, param2);

    OICFreeAndSetToNull(reinterpret_cast<void**>(&m_doxm2->mom));

    OCStackResult result = DoxmUpdateWriteableProperty(m_doxm1, m_doxm2);
    EXPECT_TRUE(OC_STACK_OK == result);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, updateMomEnable)
{
    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        { 0 },    // m_deviceID
        false,
        { 0 },    // m_owner
        {},
        OIC_MULTIPLE_OWNER_DISABLE,
        { 0 }     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;

    DoxmBinFromParameters(param1, param2);

    m_doxm1->mom->mode = OIC_MULTIPLE_OWNER_ENABLE;

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));

    OCStackResult result = DoxmUpdateWriteableProperty(m_doxm1, m_doxm2);
    EXPECT_TRUE(OC_STACK_OK == result);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, updateMomDisable)
{
    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        { 0 },    // m_deviceID
        false,
        { 0 },    // m_owner
        {},
        OIC_MULTIPLE_OWNER_ENABLE,
        { 0 }     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;

    DoxmBinFromParameters(param1, param2);

    m_doxm1->mom->mode = OIC_MULTIPLE_OWNER_DISABLE;

    EXPECT_FALSE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));

    OCStackResult result = DoxmUpdateWriteableProperty(m_doxm1, m_doxm2);
    EXPECT_TRUE(OC_STACK_OK == result);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}

TEST_F(DoxmComparisonTests, updateMomEnableDisable)
{
    DoxmTestParameter param1 =
    {
        {OIC_JUST_WORKS},
        OIC_JUST_WORKS,
        SYMMETRIC_PAIR_WISE_KEY,
        false,
        { 0 },    // m_deviceID
        false,
        { 0 },    // m_owner
        {},
        OIC_MULTIPLE_OWNER_DISABLE,
        { 0 }     // m_rownerID
    };
    GenerateDoxmTestParameterUUIDs(&param1);

    DoxmTestParameter param2 = param1;

    DoxmBinFromParameters(param1, param2);

    m_doxm1->mom->mode = OIC_MULTIPLE_OWNER_ENABLE;

    OCStackResult result = DoxmUpdateWriteableProperty(m_doxm1, m_doxm2);
    EXPECT_TRUE(OC_STACK_OK == result);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));

    m_doxm1->mom->mode = OIC_MULTIPLE_OWNER_DISABLE;

    result = DoxmUpdateWriteableProperty(m_doxm1, m_doxm2);
    EXPECT_TRUE(OC_STACK_OK == result);

    EXPECT_TRUE(AreDoxmBinPropertyValuesEqual(m_doxm1, m_doxm2));
}
#endif // #ifdef MULTIPLE_OWNER
