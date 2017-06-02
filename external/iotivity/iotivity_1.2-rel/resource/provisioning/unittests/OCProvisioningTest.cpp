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

#include <ocstack.h>
#include <oic_malloc.h>
#include <OCApi.h>
#include <OCPlatform_impl.h>
#include <oxmjustworks.h>
#include <oxmrandompin.h>
#include <srmutility.h>
#include <OCProvisioningManager.hpp>
#include <gtest/gtest.h>

#define TIMEOUT 5

namespace OCProvisioningTest
{
    using namespace OC;

    void resultCallback(PMResultList_t *result, int hasError)
    {
        (void)result;
        (void)hasError;
    }

    TEST(ProvisionInitTest, TestWithEmptyPath)
    {
        std::string dbPath("");
        EXPECT_EQ(OC_STACK_OK, OCSecure::provisionInit(dbPath));
    }

    TEST(ProvisionInitTest, TestValidPath)
    {
        std::string dbPath("./dbPath");
        EXPECT_EQ(OC_STACK_OK, OCSecure::provisionInit(dbPath));
    }

    TEST(DiscoveryTest, SecureResource)
    {
        std::shared_ptr< OC::OCSecureResource > secureResource;
        OicUuid_t uuid;
        ConvertStrToUuid("11111111-1111-1111-1111-111111111111", &uuid);

        EXPECT_EQ(OC_STACK_TIMEOUT, OCSecure::discoverSingleDevice(TIMEOUT, &uuid, secureResource));
    }

    TEST(DiscoveryTest, SecureResourceZeroTimeout)
    {
        std::shared_ptr< OC::OCSecureResource > secureResource;
        OicUuid_t uuid;
        ConvertStrToUuid("11111111-1111-1111-1111-111111111111", &uuid);

        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::discoverSingleDevice(0, &uuid, secureResource));
    }

    TEST(DiscoveryTest, UnownedDevices)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_OK, OCSecure::discoverUnownedDevices(TIMEOUT, list));
    }

    TEST(DiscoveryTest, UnownedDevicesZeroTimeout)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::discoverUnownedDevices(0, list));
    }

#ifdef MULTIPLE_OWNER
    TEST(MOTDiscoveryTest, MultipleOwnerEnabledDevices)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_OK, OCSecure::discoverMultipleOwnerEnabledDevices(TIMEOUT, list));
    }

    TEST(MOTDiscoveryTest, MultipleOwnerEnabledDevicesZeroTimeOut)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::discoverMultipleOwnerEnabledDevices(0, list));
    }

    TEST(MOTDiscoveryTest, MultipleOwnedDevices)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_OK, OCSecure::discoverMultipleOwnedDevices(TIMEOUT, list));
    }

    TEST(MOTDiscoveryTest, MultipleOwnedDevicesZeroTimeOut)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::discoverMultipleOwnedDevices(0, list));
    }
#endif

    TEST(DiscoveryTest, OwnedDevices)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_OK, OCSecure::discoverOwnedDevices(TIMEOUT, list));
    }

    TEST(DiscoveryTest, OwnedDevicesZeroTimeout)
    {
        DeviceList_t list;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::discoverOwnedDevices(0, list));
    }

    TEST(OwnershipTest, OwnershipTransferNullCallback)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.doOwnershipTransfer(nullptr));
    }

#ifdef MULTIPLE_OWNER
    TEST(MOTOwnershipTest, MOTOwnershipTransferNullCallback)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.doMultipleOwnershipTransfer(nullptr));
    }

    TEST(selectMOTMethodTest, selectMOTMethodNullCallback)
    {
        OCSecureResource device;
        const OicSecOxm_t stsecOxm = OIC_PRECONFIG_PIN;
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.selectMOTMethod(stsecOxm, NULL));
    }

    TEST(changeMOTModeTest, changeMOTModeNullCallback)
    {
        OCSecureResource device;
        const OicSecMomType_t momType = OIC_MULTIPLE_OWNER_ENABLE;
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.changeMOTMode(momType, NULL));
    }

    TEST(addPreconfigPINTest, addPreconfigPINNullPin)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, device.addPreconfigPIN(NULL, 0));
    }

    TEST(provisionPreconfPinTest, provisionPreconfPinNullCallback)
    {
        OCSecureResource device;
        const char *pin = "test";
        size_t PinLength = 4;
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.provisionPreconfPin(pin, PinLength, NULL));
    }

    TEST(isMOTEnabledTest, isMOTEnabledWithoutDeviceInst)
    {
        OCSecureResource device;
        EXPECT_EQ(false, device.isMOTEnabled());
    }

    TEST(isMOTSupportedTest, isMOTSupportedWithoutDeviceInst)
    {
        OCSecureResource device;
        EXPECT_EQ(false, device.isMOTSupported());
    }

    TEST(getMOTMethodTest, getMOTMethodNullOxM)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, device.getMOTMethod(NULL));
    }
#endif

    TEST(DeviceInfoTest, DevInfoFromNetwork)
    {
        DeviceList_t owned, unowned;
        EXPECT_EQ(OC_STACK_OK, OCSecure::getDevInfoFromNetwork(TIMEOUT,
                    owned, unowned));
    }

    TEST(SetDisplayPinCBTest, SetDisplayPinCBTestNullCB)
    {
        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::setDisplayPinCB(nullptr));
    }

    TEST(ProvisionAclTest, ProvisionAclTestNullAcl)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, device.provisionACL(nullptr, resultCallback));
    }

    TEST(ProvisionAclTest, ProvisionAclTestNullCallback)
    {
        OCSecureResource device;
        OicSecAcl_t *acl = (OicSecAcl_t *)OICCalloc(1,sizeof(OicSecAcl_t));
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.provisionACL(acl, nullptr));
        OICFree(acl);
    }

    TEST(ProvisionAclTest, ProvisionAclTestNullCallbackNUllAcl)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, device.provisionACL(nullptr, nullptr));
    }

    TEST(ProvisionCredTest, ProvisionCredTestNullCallback)
    {
        OCSecureResource device, dev2;
        Credential cred;
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.provisionCredentials(cred, dev2, nullptr));
    }

    TEST(ProvisionPairwiseTest, ProvisionPairwiseTestNullCallback)
    {
        OCSecureResource device, dev2;
        Credential cred;
        OicSecAcl_t *acl1 = (OicSecAcl_t *)OICCalloc(1,sizeof(OicSecAcl_t));
        OicSecAcl_t *acl2 = (OicSecAcl_t *)OICCalloc(1,sizeof(OicSecAcl_t));
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.provisionPairwiseDevices(cred, acl1,
                    dev2, acl2, nullptr));
        OICFree(acl1);
        OICFree(acl2);
    }

    TEST(ProvisionDirectPairingTest, ProvisionDirectPairingTestNullPconf)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, device.provisionDirectPairing(nullptr, resultCallback));
    }

    TEST(ProvisionDirectPairingTest, ProvisionDirectPairingTestNullCallback)
    {
        OCSecureResource device;
        OicSecPconf_t *pconf = (OicSecPconf_t *)OICCalloc(1,sizeof(OicSecPconf_t));
        EXPECT_EQ(OC_STACK_INVALID_CALLBACK, device.provisionDirectPairing(pconf, nullptr));
        OICFree(pconf);
    }

    TEST(ProvisionDirectPairingTest, ProvisionDirectPairingTestNullCallbackNUllPconf)
    {
        OCSecureResource device;
        EXPECT_EQ(OC_STACK_INVALID_PARAM, device.provisionDirectPairing(nullptr, nullptr));
    }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    TEST(setDeviceIdSeed, NullParam)
    {
        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::setDeviceIdSeed(NULL, 0));
    }

    TEST(setDeviceIdSeed, InvalidParam)
    {
        uint8_t seed[1024] = {0};

        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::setDeviceIdSeed(seed, 0));
        EXPECT_EQ(OC_STACK_INVALID_PARAM, OCSecure::setDeviceIdSeed(seed, sizeof(seed)));
    }

    TEST(setDeviceIdSeed, ValidValue)
    {
        uint8_t seed[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                            0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
        EXPECT_EQ(OC_STACK_OK, OCSecure::setDeviceIdSeed(seed, sizeof(seed)));
    }
#endif // __WITH_DTLS__ || __WITH_TLS__
}
