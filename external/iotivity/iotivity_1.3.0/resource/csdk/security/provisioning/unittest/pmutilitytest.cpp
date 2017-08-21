/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/
#include "gtest/gtest.h"
#include "pmutility.h"
#include "pmutilityinternal.h"
#include "doxmresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "utlist.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

extern OCStackResult AddDevice(OCProvisionDev_t **ppDevicesList, const char* addr,
                               const uint16_t port, OCTransportAdapter adapter,
                               OCConnectivityType connType, OicSecDoxm_t *doxm);
#ifdef __cplusplus
}
#endif

OCProvisionDev_t* gList = NULL;

static bool UuidsIdentical(const OicUuid_t* first, const OicUuid_t* second)
{
    return (0 == memcmp(first, second, sizeof(OicUuid_t)));
}

#ifdef MULTIPLE_OWNER
class CloneOicSecMomTest : public testing::Test
{
public:
    CloneOicSecMomTest() :
        m_originalStruct({OIC_MULTIPLE_OWNER_DISABLE}),
        m_clonedStruct(NULL)
    {}

    void TearDown()
    {
        OICFree(m_clonedStruct);
    }
protected:

    OicSecMom_t m_originalStruct;
    OicSecMom_t* m_clonedStruct;
};

TEST(CloneOicSecMomSimpleTest, shouldReturnNullForNullInput)
{
    EXPECT_TRUE(NULL == CloneOicSecMom(NULL));
}

TEST_F(CloneOicSecMomTest, shouldReturnValidPointerForValidInput)
{
    m_clonedStruct = CloneOicSecMom(&m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct);
}

TEST_F(CloneOicSecMomTest, copyShouldHaveTheSameValueAsOriginal)
{
    m_clonedStruct = CloneOicSecMom(&m_originalStruct);
    EXPECT_EQ(m_originalStruct.mode, m_clonedStruct->mode);
}

TEST_F(CloneOicSecMomTest, shouldReturnDifferentAddressThanOriginalForValidInput)
{
    m_clonedStruct = CloneOicSecMom(&m_originalStruct);
    EXPECT_NE(&m_originalStruct, m_clonedStruct);
}

static OicSecSubOwner_t* BuildSampleOicSecSubOwner(MotStatus_t status,
                                                   const OicUuid_t* uuid)
{
    OicSecSubOwner_t* result =
        (OicSecSubOwner_t*)OICCalloc(1, sizeof(OicSecSubOwner_t));
    if (NULL == result)
    {
        return NULL;
    }

    result->status = status;
    memcpy(&result->uuid, uuid, sizeof(OicUuid_t));
    result->next = NULL;

    return result;
}

static void FreeOicSecSubOwner(OicSecSubOwner_t* input)
{
    OicSecSubOwner_t* current = input;
    OicSecSubOwner_t* temp = input;
    while (NULL != current)
    {
        temp = current;
        current = temp->next;
        OICFree(temp);
    }
}

class CloneOicSecSubOwnerTest : public testing::Test
{
public:
    CloneOicSecSubOwnerTest() :
        m_originalStruct(NULL),
        m_clonedStruct(NULL)
    {}

    void SetUp()
    {
        m_originalStruct =
            BuildSampleOicSecSubOwner(MOT_STATUS_READY, &s_sampleUuid);
        ASSERT_FALSE(NULL == m_originalStruct);

        m_originalStruct->next =
            BuildSampleOicSecSubOwner(MOT_STATUS_IN_PROGRESS, &s_sampleNextUuid);
        ASSERT_FALSE(NULL == m_originalStruct->next);
    }

    void TearDown()
    {
        FreeOicSecSubOwner(m_originalStruct);
        FreeOicSecSubOwner(m_clonedStruct);
    }
protected:

    static const OicUuid_t s_sampleUuid;
    static const OicUuid_t s_sampleNextUuid;

    OicSecSubOwner_t* m_originalStruct;
    OicSecSubOwner_t* m_clonedStruct;
};

const OicUuid_t CloneOicSecSubOwnerTest::s_sampleUuid = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

const OicUuid_t CloneOicSecSubOwnerTest::s_sampleNextUuid = {
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF }
};

TEST(CloneOicSecSubOwnerSimpleTest, shouldReturnNullForNullInput)
{
    EXPECT_TRUE(NULL == CloneOicSecSubOwner(NULL));
}

TEST_F(CloneOicSecSubOwnerTest, shouldReturnValidPointerForValidInput)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct);
}

TEST_F(CloneOicSecSubOwnerTest, copyShouldHaveTheSameStatusAsOriginal)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_EQ(m_originalStruct->status, m_clonedStruct->status);
}

TEST_F(CloneOicSecSubOwnerTest, copyShouldHaveTheSameUuidAsOriginal)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_TRUE(UuidsIdentical(&m_originalStruct->uuid, &m_clonedStruct->uuid));
}

TEST_F(CloneOicSecSubOwnerTest, copyShouldHaveNext)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct->next);
}

TEST_F(CloneOicSecSubOwnerTest, copyNextShouldHaveTheSameStatusAsOriginal)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_EQ(m_originalStruct->next->status, m_clonedStruct->next->status);
}

TEST_F(CloneOicSecSubOwnerTest, copyNextShouldHaveTheSameUuidAsOriginal)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_TRUE(UuidsIdentical(&m_originalStruct->next->uuid,
                               &m_clonedStruct->next->uuid));
}

TEST_F(CloneOicSecSubOwnerTest, copyShouldHaveDifferentAddressThanOriginal)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_NE(m_originalStruct, m_clonedStruct);
}

TEST_F(CloneOicSecSubOwnerTest, copyNextShouldHaveDifferentAddressThanOriginal)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_NE(m_originalStruct->next, m_clonedStruct->next);
}

TEST_F(CloneOicSecSubOwnerTest, copyShouldHaveNullLastNext)
{
    m_clonedStruct = CloneOicSecSubOwner(m_originalStruct);
    EXPECT_TRUE(NULL == m_clonedStruct->next->next);
}
#endif

class CloneOicSecDoxmTest : public testing::Test
{
public:
    CloneOicSecDoxmTest() :
        m_originalStruct(NULL),
        m_clonedStruct(NULL)
    {}

    void SetUp()
    {
        m_originalStruct = BuildSampleOicSecDoxm();
        ASSERT_FALSE(NULL == m_originalStruct);
    }

    void TearDown()
    {
        DeleteDoxmBinData(m_originalStruct);
        DeleteDoxmBinData(m_clonedStruct);
    }

protected:

    static const OicUuid_t s_sampleDeviceId;
    static const OicUuid_t s_sampleOwner;
    static const OicUuid_t s_sampleOwnerId;
    static const OicUuid_t s_sampleSubOwner;

    OicSecDoxm_t* m_originalStruct;
    OicSecDoxm_t* m_clonedStruct;

private:

    OicSecDoxm_t* BuildSampleOicSecDoxm()
    {
        OicSecDoxm_t* result =
            (OicSecDoxm_t*)OICCalloc(1, sizeof(OicSecDoxm_t));
        if (NULL == result)
        {
            return NULL;
        }

        memcpy(&result->deviceID, &s_sampleDeviceId, sizeof(result->deviceID));
        result->dpc = true;

#ifdef MULTIPLE_OWNER
        result->mom = (OicSecMom_t*)OICCalloc(1, sizeof(OicSecMom_t));
        if (NULL == result->mom)
        {
            return NULL;
        }
        result->mom->mode = OIC_MULTIPLE_OWNER_DISABLE;
        result->subOwners =
            BuildSampleOicSecSubOwner(MOT_STATUS_IN_PROGRESS, &s_sampleSubOwner);
        if (NULL == result->subOwners)
        {
            return NULL;
        }
#endif

        result->owned = true;
        memcpy(&result->owner, &s_sampleOwner, sizeof(result->owner));
        result->oxm = (OicSecOxm_t*)OICCalloc(1, sizeof(OicSecOxm_t));
        if (NULL == result->oxm)
        {
            return NULL;
        }
        result->oxm[0] = OIC_JUST_WORKS;
        result->oxmLen = 1U;
        result->oxmSel = OIC_JUST_WORKS;
        memcpy(&result->rownerID, &s_sampleOwnerId, sizeof(result->rownerID));
        result->sct = NO_SECURITY_MODE;

        return result;
    }
};

const OicUuid_t CloneOicSecDoxmTest::s_sampleDeviceId = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

const OicUuid_t CloneOicSecDoxmTest::s_sampleOwner = {
    { 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA }
};

const OicUuid_t CloneOicSecDoxmTest::s_sampleOwnerId = {
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF }
};

const OicUuid_t CloneOicSecDoxmTest::s_sampleSubOwner = {
    { 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC }
};

TEST(CloneOicSecDoxmSimpleTest, shouldReturnNullForNullInput)
{
    EXPECT_TRUE(NULL == CloneOicSecDoxm(NULL));
}

TEST_F(CloneOicSecDoxmTest, shouldReturnValidPointerForValidInput)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveNulledOxm)
{
    ASSERT_FALSE(NULL == m_originalStruct->oxm);

    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_TRUE(NULL == m_clonedStruct->oxm);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveZeroedOxmLen)
{
    ASSERT_NE(0U, m_originalStruct->oxmLen);

    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_EQ(0U, m_clonedStruct->oxmLen);
}

#ifdef MULTIPLE_OWNER
TEST_F(CloneOicSecDoxmTest, shouldReturnValidPointerForInputWithNullMom)
{
    OICFree(m_originalStruct->mom);
    m_originalStruct->mom = NULL;

    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveNonNullMom)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct->mom);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveDifferentMomAddress)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_NE(m_originalStruct->mom, m_clonedStruct->mom);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveNullMomForNullOriginal)
{
    OICFree(m_originalStruct->mom);
    m_originalStruct->mom = NULL;

    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_TRUE(NULL == m_clonedStruct->mom);
}

TEST_F(CloneOicSecDoxmTest, shouldReturnValidPointerForInputWithNullSubOwners)
{
    FreeOicSecSubOwner(m_originalStruct->subOwners);
    m_originalStruct->subOwners = NULL;

    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveNonNullSubOwners)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_FALSE(NULL == m_clonedStruct->subOwners);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveDifferentSubOwnersAddress)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_NE(m_originalStruct->subOwners, m_clonedStruct->subOwners);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveNullSubOwnersForNullOriginal)
{
    FreeOicSecSubOwner(m_originalStruct->subOwners);
    m_originalStruct->subOwners = NULL;

    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_TRUE(NULL == m_clonedStruct->subOwners);
}
#endif

TEST_F(CloneOicSecDoxmTest, copyShouldHaveTheSameDeviceIdAsOriginal)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_TRUE(UuidsIdentical(&m_originalStruct->deviceID,
                               &m_clonedStruct->deviceID));
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveTheSameOwnerIdAsOriginal)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_TRUE(UuidsIdentical(&m_originalStruct->owner,
                               &m_clonedStruct->owner));
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveTheSameRownerIdAsOriginal)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_TRUE(UuidsIdentical(&m_originalStruct->rownerID,
                               &m_clonedStruct->rownerID));
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveTheSameSctAsOriginal)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_EQ(m_originalStruct->sct, m_clonedStruct->sct);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveTheSameOwnedAsOriginal)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_EQ(m_originalStruct->owned, m_clonedStruct->owned);
}

TEST_F(CloneOicSecDoxmTest, copyShouldHaveTheSameDpcAsOriginal)
{
    m_clonedStruct = CloneOicSecDoxm(m_originalStruct);
    EXPECT_EQ(m_originalStruct->dpc, m_clonedStruct->dpc);
}

class CloneOCProvisionDevListTest : public ::testing::Test
{
public:
    CloneOCProvisionDevListTest() :
        m_originalStruct(NULL),
        m_clonedStruct(NULL)
    {}

    void SetUp()
    {
        m_originalStruct = BuildSampleOCProvisionDev();
        ASSERT_FALSE(NULL == m_originalStruct);

        m_originalStruct->next = BuildSampleOCProvisionDev();
        ASSERT_FALSE(NULL == m_originalStruct->next);
    }

    void TearDown()
    {
        PMDeleteDeviceList(m_originalStruct);
        PMDeleteDeviceList(m_clonedStruct);
    }
protected:

    OCProvisionDev_t* m_originalStruct;
    OCProvisionDev_t* m_clonedStruct;

private:

    OCProvisionDev_t* BuildSampleOCProvisionDev()
    {
        return (OCProvisionDev_t*)OICCalloc(1, sizeof(OCProvisionDev_t));
    }
};

TEST(CloneOCProvisionDevListSimpleTest, shouldReturnNullForNullInput)
{
    EXPECT_TRUE(NULL == PMCloneOCProvisionDevList(NULL));
}

TEST_F(CloneOCProvisionDevListTest, shouldReturnValidPointerForNonNullInput)
{
    m_clonedStruct = PMCloneOCProvisionDevList(m_originalStruct);

    EXPECT_FALSE(NULL == m_clonedStruct);
}

TEST_F(CloneOCProvisionDevListTest, shouldReturnValidPointerForNonNext)
{
    m_clonedStruct = PMCloneOCProvisionDevList(m_originalStruct);

    EXPECT_FALSE(NULL == m_clonedStruct->next);
}

TEST_F(CloneOCProvisionDevListTest, copyShouldHaveDifferentAddress)
{
    m_clonedStruct = PMCloneOCProvisionDevList(m_originalStruct);

    EXPECT_NE(m_originalStruct, m_clonedStruct);
}

TEST_F(CloneOCProvisionDevListTest, copyShouldHaveDifferentAddressForNext)
{
    m_clonedStruct = PMCloneOCProvisionDevList(m_originalStruct);

    EXPECT_NE(m_originalStruct->next, m_clonedStruct->next);
}


// List add Tests
TEST(ProvisionListTest, Addition)
{
    OCProvisionDev_t* el = NULL;
    OCStackResult res = OC_STACK_ERROR;
    OicSecDoxm_t* pDoxm = NULL;
    int cnt =0;

    // The first addition
    res = AddDevice(&gList, "10.20.30.40", 5684, OC_DEFAULT_ADAPTER, CT_IP_USE_V4, pDoxm);
    EXPECT_TRUE(OC_STACK_OK == res);
    EXPECT_TRUE(NULL != gList);

    LL_FOREACH(gList,el){ ++cnt; };
    EXPECT_TRUE(1 == cnt);

    // Same node must not be inserted
    res = AddDevice(&gList, "10.20.30.40", 5684, OC_ADAPTER_IP, CT_IP_USE_V4, pDoxm);
    EXPECT_TRUE(OC_STACK_OK == res);
    EXPECT_TRUE(NULL != gList);

    cnt = 0;
    LL_FOREACH(gList,el){ ++cnt; };
    EXPECT_TRUE(1 == cnt);

    // Differnet node must be inserted
    res = AddDevice(&gList, "110.120.130.140", 6789, OC_DEFAULT_ADAPTER, CT_IP_USE_V4, pDoxm);
    EXPECT_TRUE(OC_STACK_OK == res);
    EXPECT_TRUE(NULL != gList);

    cnt = 0;
    LL_FOREACH(gList,el){ ++cnt; };
    EXPECT_TRUE(2 == cnt);
}

// List Delete Tests
TEST(ProvisionListTest, Deletion)
{
    OCProvisionDev_t* el = NULL;
    int cnt =0;

    // Delete whole
    PMDeleteDeviceList(gList);
    gList = NULL;

    LL_FOREACH(gList,el){ ++cnt; };
    EXPECT_TRUE(0 == cnt);
}
