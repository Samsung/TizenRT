/******************************************************************
*
* Copyright 2016 Samsung Electronics All Rights Reserved.
*
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************/
#ifdef __WITH_DTLS__

#include "gtest/gtest.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "resourcemanager.h"
#include "crlresource.h"
#include "securevirtualresourcetypes.h"
#include "srmtestcommon.h"
#include "srmutility.h"
#include "psinterface.h"
#include "security_internals.h"

#define TAG  "SRM-CRL-UT"

 //InitCRLResource Tests
TEST(CRLResourceTest, InitCRLResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, InitCRLResource());
}

//DeInitCredResource Tests
TEST(CRLResourceTest, DeInitCRLResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DeInitCRLResource());
}

//GetCRLResource Tests
TEST(CRLResourceTest, GetCRLResource)
{
    OicSecCrl_t *nullCrl = GetCRLResource();
    EXPECT_NE((OicSecCrl_t*)NULL, nullCrl);
    DeleteCrl(nullCrl);
}

//CrlToCBORPayload Tests
TEST(CRLResourceTest, CrlToCBORPayload)
{
    uint8_t *payload = NULL;
    size_t size;
    OicSecCrl_t *crl = GetCRLResource();
    ASSERT_TRUE(NULL != crl);
    size = 0;
    EXPECT_EQ(OC_STACK_OK, CrlToCBORPayload(crl, &payload, &size, NULL));
    DeleteCrl(crl);
    OICFree(payload);
}

//CBORPaylodToCrl Tests
TEST(CRLResourceTest, CBORPayloadToCrl)
{
    uint8_t *payload = NULL;
    size_t size;
    OicSecCrl_t *crl = GetCRLResource();
    ASSERT_TRUE(NULL != crl);
    size = 0;
    EXPECT_EQ(OC_STACK_OK, CrlToCBORPayload(crl, &payload, &size, NULL));
    DeleteCrl(crl);
    crl = NULL;
    EXPECT_EQ(OC_STACK_OK, CBORPayloadToCrl(payload, size, &crl));
    DeleteCrl(crl);
    OICFree(payload);
}

//GetDerCrl Tests
TEST(CRLResourceTest, GetDerCrl)
{
    ByteArray crlArray = {NULL, 0};
    GetDerCrl(&crlArray);
    EXPECT_NE(0, crlArray.len);
}

#endif
