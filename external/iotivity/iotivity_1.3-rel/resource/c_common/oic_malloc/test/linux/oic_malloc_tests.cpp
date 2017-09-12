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

#include "iotivity_config.h"

extern "C" {
    #include "oic_malloc.h"
}

#include <gtest/gtest.h>
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
using namespace std;

//-----------------------------------------------------------------------------
//  Tests
//-----------------------------------------------------------------------------

struct OICCallocParams
{
    OICCallocParams(size_t elementsCount, size_t allocatedSize) :
        m_elementsCount(elementsCount),
        m_allocatedSize(allocatedSize)
    {}

    size_t m_elementsCount;
    size_t m_allocatedSize;
};

class OICAllocTestsBase : public testing::Test
{
public:
    OICAllocTestsBase() :
        m_allocatedSize(0),
        m_pBuffer(NULL)
    {}

    virtual void TearDown()
    {
        OICFree(m_pBuffer);
    }

protected:
    size_t m_allocatedSize;
    uint8_t* m_pBuffer;
};

class OICMallocPassTests : public OICAllocTestsBase,
                           public testing::WithParamInterface<size_t>
{
public:
    virtual void SetUp()
    {
        m_allocatedSize = GetParam();
    }
};

class OICMallocFailTests : public OICMallocPassTests
{};

class OICCallocPassTests : public OICAllocTestsBase,
                           public testing::WithParamInterface<OICCallocParams>
{
public:
    virtual void SetUp()
    {
        m_allocatedSize = GetParam().m_allocatedSize;
        m_elementsCount = GetParam().m_elementsCount;
    }

protected:
    size_t m_elementsCount;
};

class OICCallocFailTests : public OICCallocPassTests
{};

INSTANTIATE_TEST_CASE_P(OICMallocPassTests,
                        OICMallocPassTests,
                        ::testing::Values(1,
                                          128));
TEST_P(OICMallocPassTests, shouldPassOICMalloc)
{
    m_pBuffer = (uint8_t*)OICMalloc(m_allocatedSize);
    EXPECT_NE((decltype(m_pBuffer))NULL, m_pBuffer);
}

INSTANTIATE_TEST_CASE_P(OICMallocFailTests,
                        OICMallocFailTests,
                        ::testing::Values(0,
                                          SIZE_MAX,
                                          SIZE_MAX - 1,
                                          SIZE_MAX - 2,
                                          SIZE_MAX - 8));
TEST_P(OICMallocFailTests, shouldFailOICMalloc)
{
    m_pBuffer = (uint8_t *)OICMalloc(0);
    EXPECT_EQ((decltype(m_pBuffer))NULL, m_pBuffer);
}

INSTANTIATE_TEST_CASE_P(OICCallocPassTests,
                        OICCallocPassTests,
                        ::testing::Values(OICCallocParams(1, 1),
                                          OICCallocParams(1, 128),
                                          OICCallocParams(5, 128)));
TEST_P(OICCallocPassTests, shouldPassOICCalloc)
{
    m_pBuffer = (uint8_t *)OICCalloc(m_elementsCount, m_allocatedSize);
    EXPECT_NE((decltype(m_pBuffer))NULL, m_pBuffer);
}

INSTANTIATE_TEST_CASE_P(OICCallocFailTests,
                        OICCallocFailTests,
                        ::testing::Values(OICCallocParams(1, 0),
                                          OICCallocParams(0, 5),
                                          OICCallocParams(0, 0),
                                          OICCallocParams(1, SIZE_MAX),
                                          OICCallocParams(1, SIZE_MAX - 1),
                                          OICCallocParams(1, SIZE_MAX - 2),
                                          OICCallocParams(1, SIZE_MAX - 8),
                                          OICCallocParams(SIZE_MAX, 1),
                                          OICCallocParams(SIZE_MAX - 1, 1),
                                          OICCallocParams(SIZE_MAX - 2, 1),
                                          OICCallocParams(SIZE_MAX - 8, 1)));
TEST_P(OICCallocFailTests, shouldFailOICCalloc)
{
    m_pBuffer = (uint8_t *)OICCalloc(m_elementsCount, m_allocatedSize);
    EXPECT_EQ((decltype(m_pBuffer))NULL, m_pBuffer);
}

TEST(OICFreeAndSetToNull, FreeAndSetToNullPass1)
{
    // Try to deallocate a block of memory
    uint8_t* pBuffer = (uint8_t *)OICCalloc(1, 1);
    OICFreeAndSetToNull((void**)&pBuffer);
    EXPECT_TRUE(NULL == pBuffer);
}

TEST(OICFreeAndSetToNull, FreeAndSetToNullPass2)
{
    // Try to deallocate a block of NULL
    uint8_t* pBuffer = NULL;
    OICFreeAndSetToNull((void**)&pBuffer);
    EXPECT_TRUE(NULL == pBuffer);
}
