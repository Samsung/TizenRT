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
#include <gtest/gtest.h>
#include "secureresourceprovider.h"

static OicSecAcl_t acl;
static OCProvisionDev_t pDev1;
static OCProvisionDev_t pDev2;
static OicSecCredType_t credType = SYMMETRIC_PAIR_WISE_KEY;
static OicSecOxm_t oicSecDoxmJustWorks = OIC_JUST_WORKS;
static OicSecOxm_t oicSecDoxmRandomPin = OIC_RANDOM_DEVICE_PIN;
static unsigned short timeout = 60;
static OicSecDoxm_t defaultDoxm1 =
{
    &oicSecDoxmJustWorks,   /* uint16_t *oxm */
    1,                      /* size_t oxmLen */
    OIC_JUST_WORKS,         /* uint16_t oxmSel */
    SYMMETRIC_PAIR_WISE_KEY,/* OicSecCredType_t sct */
    false,                  /* bool owned */
    {{0}},                  /* OicUuid_t deviceID */
    false,                  /* bool dpc */
    {{0}},                  /* OicUuid_t owner */
#ifdef MULTIPLE_OWNER
    NULL,                   /* OicSecSubOwner_t* subOwners */
    NULL,                   /* OicSecMom_t *mom */
#endif //MULTIPLE_OWNER
    {{0}}                   /* rownerID */
};

static OicSecDoxm_t defaultDoxm2 =
{
    &oicSecDoxmRandomPin,   /* uint16_t *oxm */
    1,                      /* size_t oxmLen */
    OIC_RANDOM_DEVICE_PIN,  /* uint16_t oxmSel */
    SYMMETRIC_PAIR_WISE_KEY,/* OicSecCredType_t sct */
    false,                  /* bool owned */
    {{0}},                  /* OicUuid_t deviceID */
    false,                  /* bool dpc */
    {{0}},                  /* OicUuid_t owner */
#ifdef MULTIPLE_OWNER
    NULL,                   /* OicSecSubOwner_t* subOwners */
    NULL,                   /* OicSecMom_t *mom */
#endif //MULTIPLE_OWNER
    {{0}}                   /* rownerID */
};

static void provisioningCB (void* UNUSED1, size_t UNUSED2, OCProvisionResult_t *UNUSED3, bool UNUSED4)
{
    //dummy callback
    (void) UNUSED1;
    (void) UNUSED2;
    (void) UNUSED3;
    (void) UNUSED4;
}

TEST(SRPProvisionACLTest, NullDeviceInfo)
{
    pDev1.doxm = &defaultDoxm1;
    uint8_t deviceId1[] = {0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x49, 0x64};
    memcpy(pDev1.doxm->deviceID.id, deviceId1, sizeof(deviceId1));

    pDev2.doxm = &defaultDoxm2;
    uint8_t deviceId2[] = {0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x49, 0x63};
    memcpy(pDev2.doxm->deviceID.id, deviceId2, sizeof(deviceId2));

    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPProvisionACL(NULL, NULL, &acl, OIC_SEC_ACL_V2, &provisioningCB));
}

TEST(SRPProvisionACLTest, NullCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPProvisionACL(NULL, &pDev1, &acl, OIC_SEC_ACL_V2, NULL));
}

TEST(SRPProvisionACLTest, NullACL)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPProvisionACL(NULL, &pDev1, NULL, OIC_SEC_ACL_V2, &provisioningCB));
}

TEST(SRPProvisionACLTest, InvalidVersion)
{
    EXPECT_EQ(OC_STACK_ERROR, SRPProvisionACL(NULL, &pDev1, &acl, OIC_SEC_ACL_UNKNOWN, &provisioningCB));
}

TEST(SRPProvisionCredentialsTest, NullDevice1)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPProvisionCredentials(NULL, credType,
                                                              OWNER_PSK_LENGTH_128, NULL,
                                                              &pDev2, NULL, NULL, NULL, &provisioningCB));
}

TEST(SRPProvisionCredentialsTest, SamelDeviceId)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPProvisionCredentials(NULL, credType,
                                                              OWNER_PSK_LENGTH_128, &pDev1,
                                                              &pDev1, NULL, NULL, NULL, &provisioningCB));
}

TEST(SRPProvisionCredentialsTest, NullCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPProvisionCredentials(NULL, credType,
                                                                 OWNER_PSK_LENGTH_128,
                                                                 &pDev1, &pDev2, NULL, NULL, NULL, NULL));
}

TEST(SRPProvisionCredentialsTest, InvalidKeySize)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPProvisionCredentials(NULL, credType,
                                                                0, &pDev1, &pDev2, NULL, NULL, NULL, 
                                                                &provisioningCB));
}

TEST(SRPUnlinkDevicesTest, NullDevice1)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPUnlinkDevices(NULL, NULL, &pDev2, provisioningCB));
}

TEST(SRPUnlinkDevicesTest, SamelDeviceId)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPUnlinkDevices(NULL, &pDev1, &pDev1, provisioningCB));
}

TEST(SRPUnlinkDevicesTest, NullCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPUnlinkDevices(NULL, &pDev1, &pDev2, NULL));
}

TEST(SRPRemoveDeviceTest, NullTargetDevice)
{
    unsigned short waitTime = 10 ;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPRemoveDevice(NULL, waitTime, NULL, provisioningCB));
}

TEST(SRPRemoveDeviceTest, NullResultCallback)
{
    unsigned short waitTime = 10;
    OCProvisionDev_t dev1;
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPRemoveDevice(NULL, waitTime, &dev1, NULL));
}

TEST(SRPRemoveDeviceTest, ZeroWaitTime)
{
    OCProvisionDev_t dev1;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPRemoveDevice(NULL, 0, &dev1, NULL));
}

const char *SECURE_RESOURCE_PROVIDER_TEST_FILE_NAME = "secureresourceprovider.dat";
OCPersistentStorage ps = { NULL, NULL, NULL, NULL, NULL};

static FILE* TestFopen(const char *path, const char *mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen(SECURE_RESOURCE_PROVIDER_TEST_FILE_NAME, mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

void SetPersistentHandler(OCPersistentStorage *persistentStorage)
{
    if(persistentStorage)
    {
        persistentStorage->open = TestFopen;
        persistentStorage->read = fread;
        persistentStorage->write = fwrite;
        persistentStorage->close = fclose;
        persistentStorage->unlink = remove;
    }
}

class SRPTest : public ::testing::Test
{
public:
    static void SetUpTestCase()
    {
        SetPersistentHandler(&ps);
        OCStackResult res = OCRegisterPersistentStorageHandler(&ps);
        ASSERT_TRUE(res == OC_STACK_OK);
        res = OCInit(NULL, 0, OC_SERVER);
        ASSERT_TRUE(res == OC_STACK_OK);
    }

    static void TearDownTestCase()
    {
        OCStackResult res = OCStop();
        ASSERT_TRUE(res == OC_STACK_OK);
    }

    static const ByteArray g_caPublicKey;

    static const ByteArray g_derCode ;

    static ByteArray g_serNum;
};

static uint8_t certData[] = {
        0x30, 0x82, 0x02, 0x39, 0x30, 0x82, 0x01, 0xdf, 0x02, 0x01, 0x01, 0x30, 0x0a, 0x06, 0x08, 0x2a,
        0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x7c, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
        0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
        0x09, 0x53, 0x6f, 0x6d, 0x65, 0x73, 0x74, 0x61, 0x74, 0x65, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03,
        0x55, 0x04, 0x07, 0x0c, 0x08, 0x53, 0x6f, 0x6d, 0x65, 0x63, 0x69, 0x74, 0x79, 0x31, 0x0b, 0x30,
        0x09, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x02, 0x42, 0x42, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03,
        0x55, 0x04, 0x0b, 0x0c, 0x0d, 0x53, 0x65, 0x71, 0x75, 0x72, 0x69, 0x74, 0x79, 0x20, 0x50, 0x61,
        0x72, 0x74, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x02, 0x6f, 0x62, 0x31,
        0x14, 0x30, 0x12, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x05,
        0x6f, 0x62, 0x40, 0x62, 0x62, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x36, 0x30, 0x38, 0x31, 0x35, 0x31,
        0x33, 0x31, 0x31, 0x31, 0x37, 0x5a, 0x17, 0x0d, 0x31, 0x39, 0x30, 0x35, 0x31, 0x32, 0x31, 0x33,
        0x31, 0x31, 0x31, 0x37, 0x5a, 0x30, 0x81, 0xd4, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
        0x06, 0x13, 0x02, 0x55, 0x41, 0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x03,
        0x41, 0x73, 0x64, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x06, 0x47, 0x6f,
        0x74, 0x68, 0x61, 0x6d, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x02, 0x5a,
        0x5a, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x08, 0x42, 0x65, 0x61, 0x6d,
        0x54, 0x65, 0x61, 0x6d, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
        0x01, 0x09, 0x01, 0x16, 0x0d, 0x72, 0x61, 0x69, 0x6c, 0x40, 0x6d, 0x61, 0x69, 0x6c, 0x2e, 0x63,
        0x6f, 0x6d, 0x31, 0x32, 0x30, 0x30, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x29, 0x75, 0x75, 0x69,
        0x64, 0x3a, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 0x2d, 0x33, 0x32, 0x33, 0x32, 0x2d,
        0x33, 0x32, 0x33, 0x32, 0x2d, 0x33, 0x32, 0x33, 0x32, 0x2d, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32,
        0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 0x31, 0x34, 0x30, 0x32, 0x06, 0x03, 0x55, 0x1d, 0x11, 0x0c,
        0x2b, 0x75, 0x73, 0x65, 0x72, 0x69, 0x64, 0x3a, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37,
        0x2d, 0x36, 0x37, 0x36, 0x37, 0x2d, 0x36, 0x37, 0x36, 0x37, 0x2d, 0x36, 0x37, 0x36, 0x37, 0x2d,
        0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x36, 0x37, 0x30, 0x59, 0x30, 0x13,
        0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
        0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xf7, 0x13, 0x5c, 0x73, 0x72, 0xce, 0x10, 0xe5, 0x09,
        0x97, 0x9a, 0xf8, 0xf2, 0x70, 0xa6, 0x3d, 0x89, 0xf5, 0xc5, 0xe4, 0x44, 0xe2, 0x4a, 0xb6, 0x61,
        0xa8, 0x12, 0x8d, 0xb4, 0xdc, 0x2b, 0x47, 0x84, 0x60, 0x0c, 0x25, 0x66, 0xe9, 0xe0, 0xe5, 0xac,
        0x22, 0xbf, 0x15, 0xdc, 0x71, 0xb1, 0x88, 0x4f, 0x16, 0xbf, 0xc2, 0x77, 0x37, 0x76, 0x3f, 0xe0,
        0x67, 0xc6, 0x1d, 0x23, 0xfe, 0x7c, 0x8b, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
        0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x20, 0x47, 0xcc, 0x41, 0x8a, 0x27, 0xc7,
        0xd0, 0xaa, 0xb4, 0xab, 0x85, 0xbf, 0x09, 0x4d, 0x06, 0xd7, 0x7e, 0x0d, 0x39, 0xf9, 0x36, 0xa1,
        0x3d, 0x96, 0x23, 0xe2, 0x24, 0x64, 0x98, 0x63, 0x21, 0xba, 0x02, 0x21, 0x00, 0xe5, 0x8f, 0x7f,
        0xf1, 0xa6, 0x82, 0x03, 0x6a, 0x18, 0x7a, 0x54, 0xe7, 0x0e, 0x25, 0x77, 0xd8, 0x46, 0xfa, 0x96,
        0x8a, 0x7e, 0x14, 0xc4, 0xcb, 0x21, 0x32, 0x3e, 0x89, 0xd9, 0xba, 0x8c, 0x3f
    };

static uint8_t keyData[] = {
	    0x67, 0xc6, 0x1d, 0x23, 0xfe, 0x7c, 0x8b, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
        0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x20, 0x47, 0xcc, 0x41, 0x8a, 0x27, 0xc7,
        0xd0, 0xaa, 0xb4, 0xab, 0x85, 0xbf, 0x09, 0x4d, 0x06, 0xd7, 0x7e, 0x0d, 0x39, 0xf9, 0x36, 0xa1,
        0x3d, 0x96, 0x23, 0xe2, 0x24, 0x64, 0x98, 0x63, 0x21, 0xba, 0x02, 0x21
    };

TEST_F(SRPTest, SRPSaveTrustCertChainDER)
{
    int result;
    uint16_t credId;

    result = SRPSaveTrustCertChain(certData, sizeof(certData), OIC_ENCODING_DER, &credId);
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST_F(SRPTest, SRPSaveTrustCertChainPEM)
{
    int result;
    uint16_t credId;

    result = SRPSaveTrustCertChain(certData, sizeof(certData), OIC_ENCODING_PEM, &credId);
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST_F(SRPTest, SRPSaveTrustCertChainNullCertData)
{
    int result;
    uint16_t credId;

    result = SRPSaveTrustCertChain(NULL, sizeof(certData), OIC_ENCODING_DER, &credId);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST_F(SRPTest, SRPSaveTrustCertChainNullCredId)
{
    int result;

    result = SRPSaveTrustCertChain(certData, sizeof(certData), OIC_ENCODING_DER, NULL);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST_F(SRPTest, SRPSaveOwnCertChainTest)
{
    int result;
    uint16_t credId;
    OicSecKey_t cert;
    OicSecKey_t key;

    cert.data = certData;
    cert.len = sizeof(certData);
    cert.encoding = OIC_ENCODING_DER;

    key.data = keyData;
    key.len = sizeof(keyData);
    key.encoding = OIC_ENCODING_DER;

    result = SRPSaveOwnCertChain(&cert, &key, &credId);

    EXPECT_EQ(OC_STACK_OK, result);
}

TEST_F(SRPTest, SRPSaveOwnCertChainTestNullCert)
{
    int result;
    uint16_t credId;
    OicSecKey_t key;

    key.data = keyData;
    key.len = sizeof(keyData);

    result = SRPSaveOwnCertChain(NULL, &key, &credId);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST_F(SRPTest, SRPSaveOwnCertChainTestNullCertData)
{
    int result;
    uint16_t credId;
    OicSecKey_t cert;
    OicSecKey_t key;

    cert.data = NULL;
    cert.len = sizeof(certData);
    key.data = keyData;
    key.len = sizeof(keyData);

    result = SRPSaveOwnCertChain(&cert, &key, &credId);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST_F(SRPTest, SRPSaveOwnCertChainTestNullKey)
{
    int result;
    uint16_t credId;
    OicSecKey_t cert;

    cert.data = certData;
    cert.len = sizeof(certData);

    result = SRPSaveOwnCertChain(&cert, NULL, &credId);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST_F(SRPTest, SRPSaveOwnCertChainTestNullKeyData)
{
    int result;
    uint16_t credId;
    OicSecKey_t cert;
    OicSecKey_t key;

    cert.data = certData;
    cert.len = sizeof(certData);
    key.data = NULL;
    key.len = sizeof(keyData);

    result = SRPSaveOwnCertChain(&cert, &key, &credId);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST_F(SRPTest, SRPSaveOwnCertChainTestNullCredId)
{
    int result;
    OicSecKey_t cert;
    OicSecKey_t key;

    cert.data = certData;
    cert.len = sizeof(certData);
    key.data = keyData;
    key.len = sizeof(keyData);

    result = SRPSaveOwnCertChain(&cert, &key, NULL);

    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}


TEST(SRPProvisionTrustCertChainTest, SRPProvisionTrustCertChainNullSelectedDeviceInfo)
{
    int result;
    int ctx;
    OicSecCredType_t type = SIGNED_ASYMMETRIC_KEY;
    uint16_t credId = 0;

    result = SRPProvisionTrustCertChain(&ctx, type, credId, NULL, provisioningCB);
    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST(SRPProvisionTrustCertChainTest, SRPProvisionTrustCertChainNullResultCallback)
{
    int result;
    int ctx;
    OicSecCredType_t type = SIGNED_ASYMMETRIC_KEY;
    uint16_t credId = 0;
    OCProvisionDev_t deviceInfo;

    result = SRPProvisionTrustCertChain(&ctx, type, credId, &deviceInfo, NULL);
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, result);
}

TEST(SRPProvisionTrustCertChainTest, SRPProvisionTrustCertChainInvalidOicSecCredType)
{
    int result;
    int ctx;
    OicSecCredType_t type = PIN_PASSWORD;
    uint16_t credId = 0;
    OCProvisionDev_t deviceInfo;

    result = SRPProvisionTrustCertChain(&ctx, type, credId, &deviceInfo, provisioningCB);
    EXPECT_EQ(OC_STACK_INVALID_PARAM, result);
}

TEST_F(SRPTest, SRPProvisionTrustCertChainNoResource)
{
    int result;
    int ctx;
    OicSecCredType_t type = SIGNED_ASYMMETRIC_KEY;
    uint16_t credId = 0;
    OCProvisionDev_t deviceInfo;

    result = SRPProvisionTrustCertChain(&ctx, type, credId, &deviceInfo, provisioningCB);
    EXPECT_EQ(OC_STACK_ERROR, result);
}

TEST(SRPProvisionTrustCertChainTest, SRPGetACLResourceNoCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPGetACLResource(NULL, &pDev1, OIC_SEC_ACL_V2, NULL));
}

TEST(SRPProvisionTrustCertChainTest, SRPGetACLResourceNoDevice)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPGetACLResource(NULL, NULL, OIC_SEC_ACL_V2, provisioningCB));
}

TEST(SRPProvisionTrustCertChainTest, SRPGetCredResourceNoCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPGetCredResource(NULL, &pDev1, NULL));
}

TEST(SRPProvisionTrustCertChainTest, SRPGetCredResourceNoDevice)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPGetCredResource(NULL, NULL, provisioningCB));
}

TEST(SRPProvisionTrustCertChainTest, SRPResetDeviceNoCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPResetDevice(&pDev1, NULL));
}

TEST(SRPProvisionTrustCertChainTest, SRPResetDeviceNoDevice)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPResetDevice(NULL, provisioningCB));
}

TEST(SRPProvisionTrustCertChainTest, SRPSyncDeviceNoCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK, SRPSyncDevice(NULL, timeout, &pDev1, NULL));
}

TEST(SRPProvisionTrustCertChainTest, SRPSyncDeviceNoDevice)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPSyncDevice(NULL, timeout, NULL, provisioningCB));
}

TEST(SRPProvisionTrustCertChainTest, SRPSyncDeviceZeroWaitTime)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SRPSyncDevice(NULL, 0, &pDev1, provisioningCB));
}

TEST(SRPProvisionTrustCertChainTest, SRPRemoveDeviceWithoutDiscoveryNoCallback)
{
    EXPECT_EQ(OC_STACK_INVALID_CALLBACK,
              SRPRemoveDeviceWithoutDiscovery(NULL, &pDev1, &pDev2, NULL));
}

TEST(SRPProvisionTrustCertChainTest, SRPRemoveDeviceWithoutDiscoveryNoDevice)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM,
              SRPRemoveDeviceWithoutDiscovery(NULL, &pDev1, NULL, provisioningCB));
}

TEST(SRPProvisionTrustCertChainTest, SRPRemoveDeviceWithoutDiscoveryNoDeviceList)
{
    EXPECT_EQ(OC_STACK_CONTINUE,
              SRPRemoveDeviceWithoutDiscovery(NULL, NULL, &pDev2, provisioningCB));
}

