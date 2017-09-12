/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include <ocstack.h>
#include <oic_malloc.h>
#include <OCApi.h>
#include <OCPlatform_impl.h>
#include <oxmjustworks.h>
#include <oxmrandompin.h>
#include <OCProvisioningManager.hpp>
#include <OCCloudProvisioning.hpp>
#include <gtest/gtest.h>

#define TIMEOUT 5
static std::string IP("127.0.0.1");
static uint16_t PORT = 5683;

namespace OCCloudProvisioningTest
{
    using namespace OC;

    void resultCallback(OCStackResult result, void *data)
    {
        (void)result;
        (void)data;
    }

    TEST(RequestCertTest, TestNullCallback)
    {
        OCCloudProvisioning cloud(IP, PORT);
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, cloud.requestCertificate(nullptr));
    }

    TEST(AclIdByDeviceTest, AclIdNullCallback)
    {
        OCCloudProvisioning cloud(IP, PORT);
        std::string devId("");
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, cloud.getAclIdByDevice(devId, nullptr));
    }

    TEST(AclInfoTest, AclInfoNullCallback)
    {
        OCCloudProvisioning cloud(IP, PORT);
        std::string aclId("");
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, cloud.getIndividualAclInfo(aclId, nullptr));
    }

    TEST(GetCRLTest, GetCrlNullCallback)
    {
        OCCloudProvisioning cloud(IP, PORT);
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, cloud.getCRL(nullptr));
    }

    TEST(PostCRLTest, PostCrlNullCallback)
    {
        std::string thisUpdate("");
        std::string nextUpdate("");
        OCByteString crl;
        stringArray_t serials;
        OCCloudProvisioning cloud(IP, PORT);
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, cloud.postCRL(thisUpdate,
                                                    nextUpdate,
                                                    &crl, &serials, nullptr));
    }
}
