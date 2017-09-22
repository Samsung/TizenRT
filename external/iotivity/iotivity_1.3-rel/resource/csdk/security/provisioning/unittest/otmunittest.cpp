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
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "ocstack.h"
#include "utlist.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocprovisioningmanager.h"
#include "oxmjustworks.h"
#include "oxmrandompin.h"
#include "oxmmanufacturercert.h"
#include "provisioningdatabasemanager.h"
#ifdef MULTIPLE_OWNER
#include "multipleownershiptransfermanager.h"
#endif //MULTIPLE_OWNER
#include "srmutility.h"
#include "doxmresource.h"
#include "pmtypes.h"
#include "pmutility.h"
#include "ocrandom.h"

#define OTM_TIMEOUT 60
#define DISCOVERY_TIMEOUT 10

#define SVR_DB_FILE_NAME "oic_svr_db_client.dat"
#define PM_DB_FILE_NAME "test.db"

static OCPersistentStorage gPst;
static bool g_doneCB;
static bool g_callbackResult;
static const char* g_otmCtx = "Test User Context";
static OCProvisionDev_t* g_unownedDevices = NULL;
static OCProvisionDev_t* g_ownedDevices = NULL;
static int gNumOfUnownDevice = 0;
static int gNumOfOwnDevice = 0;

using namespace std;

#define TAG "OTM_UNITTEST"

TEST(JustWorksOxMTest, NullParam)
{
    OTMContext_t* otmCtx = NULL;
    OCStackResult res = OC_STACK_ERROR;
    uint8_t *payloadRes = NULL;
    size_t size = 0;

    //LoadSecretJustWorksCallback always returns OC_STACK_OK.
    res = LoadSecretJustWorksCallback(otmCtx);
    EXPECT_TRUE(OC_STACK_OK == res);

    res = CreateSecureSessionJustWorksCallback(otmCtx);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateJustWorksSelectOxmPayload(otmCtx, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateJustWorksOwnerTransferPayload(otmCtx, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    OTMContext_t otmCtx2;
    otmCtx2.selectedDeviceInfo = NULL;

    //LoadSecretJustWorksCallback always returns OC_STACK_OK.
    res = LoadSecretJustWorksCallback(&otmCtx2);
    EXPECT_TRUE(OC_STACK_OK == res);

    res = CreateSecureSessionJustWorksCallback(&otmCtx2);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateJustWorksSelectOxmPayload(&otmCtx2, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateJustWorksOwnerTransferPayload(&otmCtx2, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);
}

TEST(RandomPinOxMTest, NullParam)
{
    OTMContext_t* otmCtx = NULL;
    OCStackResult res = OC_STACK_ERROR;
    uint8_t *payloadRes = NULL;
    size_t size = 0;

    //LoadSecretJustWorksCallback always returns OC_STACK_OK.
    res = InputPinCodeCallback(otmCtx);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateSecureSessionRandomPinCallback(otmCtx);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreatePinBasedSelectOxmPayload(otmCtx, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreatePinBasedOwnerTransferPayload(otmCtx, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    OTMContext_t otmCtx2;
    otmCtx2.selectedDeviceInfo = NULL;

    //LoadSecretJustWorksCallback always returns OC_STACK_OK.
    res = InputPinCodeCallback(&otmCtx2);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateSecureSessionRandomPinCallback(&otmCtx2);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreatePinBasedSelectOxmPayload(&otmCtx2, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreatePinBasedOwnerTransferPayload(&otmCtx2, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);
}

TEST(ManufacturerCertOxMTest, NullParam)
{
    OTMContext_t* otmCtx = NULL;
    OCStackResult res = OC_STACK_ERROR;
    uint8_t *payloadRes = NULL;
    size_t size = 0;

    res = PrepareMCertificateCallback(otmCtx);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateSecureSessionMCertificateCallback(otmCtx);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateMCertificateBasedSelectOxmPayload(otmCtx, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateMCertificateBasedOwnerTransferPayload(otmCtx, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    OTMContext_t otmCtx2;
    otmCtx2.selectedDeviceInfo = NULL;

    res = InputPinCodeCallback(&otmCtx2);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateSecureSessionMCertificateCallback(&otmCtx2);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateMCertificateBasedSelectOxmPayload(&otmCtx2, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);

    res = CreateMCertificateBasedOwnerTransferPayload(&otmCtx2, &payloadRes, &size);
    EXPECT_TRUE(OC_STACK_INVALID_PARAM == res);
}

/****************************************
 * Test the OTM modules with sample server
 ****************************************/
static FILE* fopen_prvnMng(const char* path, const char* mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
      return fopen(SVR_DB_FILE_NAME, mode);
    }
    OIC_LOG_V(DEBUG, TAG, "use db: %s", path);
    return fopen(path, mode);
}

// callback function(s) for provisioning client using C-level provisioning API
static void ownershipTransferCB(void* ctx, size_t UNUSED1, OCProvisionResult_t* UNUSED2, bool hasError)
{
    OC_UNUSED(UNUSED1);
    OC_UNUSED(UNUSED2);
    OC_UNUSED(ctx);

    if(!hasError)
    {
        OIC_LOG_V(DEBUG, TAG, "Ownership Transfer SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(FATAL, TAG, "Ownership Transfer FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;

    OIC_LOG_V(DEBUG, TAG, "%s: done(has erro: %s)", __func__, hasError ? "yes" : "no");
}

// callback function(s) for provisioning client using C-level provisioning API
static void removeDeviceCB(void* ctx, size_t UNUSED1, OCProvisionResult_t* UNUSED2, bool hasError)
{
    OC_UNUSED(UNUSED1);
    OC_UNUSED(UNUSED2);
    OC_UNUSED(ctx);

    if(!hasError)
    {
        OIC_LOG_V(DEBUG, TAG, "Remove device request SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(FATAL, TAG, "Remove device request FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;
}

static int waitCallbackRet(void)
{
    for(int i = 0; !g_doneCB && OTM_TIMEOUT > i; ++i)
    {
        sleep(1);
        if(OC_STACK_OK != OCProcess())
        {
            OIC_LOG(FATAL, TAG, "OCStack process error");
            return -1;
        }
    }

    return 0;
}

#define UUID_TEMPLATE "11111111-1234-1234-1234-12345678901"
#define UUID_TEMPLATE_LEN 35

static void RemoveUnknownDeviceFromDevList(OCProvisionDev_t* devList)
{
    OCProvisionDev_t* unknowDev = NULL;
    OCProvisionDev_t* tempDev1 = NULL;
    OCProvisionDev_t* tempDev2 =NULL;

    LL_FOREACH_SAFE(devList, tempDev1, tempDev2)
    {
        char *uuidString = NULL;
        EXPECT_EQ(OC_STACK_OK, ConvertUuidToStr((const OicUuid_t*) &tempDev1->doxm->deviceID, &uuidString));

        if (strncmp(UUID_TEMPLATE, uuidString, UUID_TEMPLATE_LEN) != 0)
        {
            OIC_LOG_V(DEBUG, TAG, "%s: delete from list id:%s ip:%s:%d",__func__,
            uuidString ? uuidString :"unknow id",
            tempDev1->endpoint.addr,tempDev1->endpoint.port);

            LL_DELETE(devList, tempDev1);
            LL_APPEND(unknowDev, tempDev1);
        }
        OICFree(uuidString);
    }

    OCDeleteDiscoveredDevices(unknowDev);
}

TEST(InitForOTM, NullParam)
{
    OCStackResult result = OC_STACK_ERROR;

    OTMCallbackData_t otmcb;
    otmcb.loadSecretCB = LoadSecretJustWorksCallback;
    otmcb.createSecureSessionCB = CreateSecureSessionJustWorksCallback;
    otmcb.createSelectOxmPayloadCB = CreateJustWorksSelectOxmPayload;
    otmcb.createOwnerTransferPayloadCB = CreateJustWorksOwnerTransferPayload;
#ifdef HAVE_UNISTD_H
    EXPECT_EQ(0, access(SVR_DB_FILE_NAME, F_OK));
#endif // HAVE_UNISTD_H
    gPst.open = fopen_prvnMng;
    gPst.read = fread;
    gPst.write = fwrite;
    gPst.close = fclose;
    gPst.unlink = unlink;

    // register the persistent storage handler for SVR
    result = OCRegisterPersistentStorageHandler(&gPst);
    EXPECT_EQ(OC_STACK_OK, result);

    // initialize OC stack and provisioning manager
    result = OCInit(NULL, 0, OC_CLIENT_SERVER);
    EXPECT_EQ(OC_STACK_OK, result);

    //initialize Provisioning DB Manager
    result = OCInitPM(PM_DB_FILE_NAME);
    EXPECT_EQ(OC_STACK_OK, result);

    // register callback function(s) for Justworks OxM
    result = OCSetOwnerTransferCallbackData(OIC_JUST_WORKS, &otmcb);
    EXPECT_EQ(OC_STACK_OK, result);

    g_doneCB = false;
    g_callbackResult = false;

    gNumOfUnownDevice = 0;
    gNumOfOwnDevice = 0;
    // close Provisioning DB
    EXPECT_EQ(OC_STACK_OK, OCClosePM());
}
/*
TEST(OCDiscoverSingleDeviceInUnicast, Simple)
{
    OCProvisionDev_t* foundDevice = NULL;

    OicUuid_t uuid = {0};
    ConvertStrToUuid("11111111-1234-1234-1234-123456789011", &uuid);

    EXPECT_EQ(OC_STACK_OK, OCDiscoverSingleDeviceInUnicast(DISCOVERY_TIMEOUT, &uuid, "::1", CT_ADAPTER_IP, &foundDevice));

    int NumOfFoundDevice = 0;
    OCProvisionDev_t* tempDev = foundDevice;
    while(tempDev)
    {
        NumOfFoundDevice++;
        tempDev = tempDev->next;
    }

    PMDeleteDeviceList(foundDevice);
    OIC_LOG_V(DEBUG, TAG, "Discoveed: %d devices", NumOfFoundDevice);
    //TODO fix after:
    //54:22.622 INFO: OIC_SRM: Received error from remote device with result, 7 for request uri, /oic/sec/doxm
    //EXPECT_EQ(true, NumOfFoundDevice > 0);
}
*/
TEST(OCDiscoverUnownedDevices, Simple)
{
    //initialize Provisioning DB Manager
    EXPECT_EQ(OC_STACK_OK, OCInitPM(PM_DB_FILE_NAME));
    EXPECT_EQ(OC_STACK_OK, OCDiscoverUnownedDevices(DISCOVERY_TIMEOUT, &g_unownedDevices));

    RemoveUnknownDeviceFromDevList(g_unownedDevices);

    OCProvisionDev_t* tempDev = g_unownedDevices;
    while(tempDev)
    {
        gNumOfUnownDevice++;

        char *uuidString = NULL;
        EXPECT_EQ(OC_STACK_OK, ConvertUuidToStr((const OicUuid_t*) &tempDev->doxm->deviceID, &uuidString));

        OIC_LOG_V(DEBUG, TAG, "%d: id:%s ip:%s:%d", gNumOfUnownDevice,
        uuidString ? uuidString :"unknow id",
        tempDev->endpoint.addr,tempDev->endpoint.port);

        OICFree(uuidString);

        tempDev = tempDev->next;
    }

    EXPECT_EQ(true, gNumOfUnownDevice > 0);
    // close Provisioning DB
    EXPECT_EQ(OC_STACK_OK, OCClosePM());
}

TEST(OCDoOwnershipTransfer, Simple)
{
    //initialize Provisioning DB Manager
    EXPECT_EQ(OC_STACK_OK, OCInitPM(PM_DB_FILE_NAME));
    ASSERT_EQ(true, gNumOfUnownDevice > 0);

    g_doneCB = false;
    EXPECT_EQ(OC_STACK_OK, OCDoOwnershipTransfer((void*)g_otmCtx, g_unownedDevices, ownershipTransferCB));

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(FATAL, TAG, "OCProvisionCredentials callback error");
        return;
    }

    EXPECT_EQ(true, g_callbackResult);
    EXPECT_EQ(true, g_doneCB);
    // close Provisioning DB
    EXPECT_EQ(OC_STACK_OK, OCClosePM());
}

TEST(OCDiscoverOwnedDevices, Simple)
{
    //initialize Provisioning DB Manager
    EXPECT_EQ(OC_STACK_OK, OCInitPM(PM_DB_FILE_NAME));
    OCStackResult result = OC_STACK_ERROR;
    gNumOfOwnDevice = 0;

    EXPECT_EQ(OC_STACK_OK, OCDiscoverOwnedDevices(DISCOVERY_TIMEOUT, &g_ownedDevices));

    RemoveUnknownDeviceFromDevList(g_ownedDevices);

    OCProvisionDev_t* tempDev = g_ownedDevices;
    while(tempDev)
    {
        gNumOfOwnDevice++;
        tempDev = tempDev->next;
    }

    if (gNumOfUnownDevice > gNumOfOwnDevice)
    {
        OIC_LOG(DEBUG, TAG, "Discovering unowned devices on");

        if(g_unownedDevices)
        {
            OCDeleteDiscoveredDevices(g_unownedDevices);
            g_unownedDevices = NULL;
        }

        result = OCDiscoverUnownedDevices(DISCOVERY_TIMEOUT, &g_unownedDevices);
        EXPECT_EQ(OC_STACK_OK, result);

        RemoveUnknownDeviceFromDevList(g_unownedDevices);

        tempDev = g_unownedDevices;
        while(tempDev)
        {
            gNumOfUnownDevice++;
            tempDev = tempDev->next;
        }

        if (gNumOfUnownDevice)
        {
            g_doneCB = false;
            EXPECT_EQ(OC_STACK_OK, OCDoOwnershipTransfer((void*)g_otmCtx, g_unownedDevices, ownershipTransferCB));

            if(waitCallbackRet())  // input |g_doneCB| flag implicitly
            {
                OIC_LOG(FATAL, TAG, "OCProvisionCredentials callback error");
                return;
            }

            EXPECT_EQ(true, g_callbackResult);
            EXPECT_EQ(true, g_doneCB);
        }
    }

    EXPECT_EQ(true, gNumOfOwnDevice > 0);
    // close Provisioning DB
    EXPECT_EQ(OC_STACK_OK, OCClosePM());
}

TEST(PerformLinkDevices, NullParam)
{
    if (gNumOfOwnDevice < 2)
    {
        OIC_LOG(WARNING, TAG, "Link can only pair owned devices");
        OIC_LOG_V(DEBUG, TAG, "Number of owned devices: %d",gNumOfOwnDevice);
        return;
    }

    OicUuid_t myUuid;
    OCStackResult result = OC_STACK_ERROR;
    result = GetDoxmDeviceID(&myUuid);
    EXPECT_EQ(OC_STACK_OK, result);

    //Extract target device except PT to perform link devices.
    OCProvisionDev_t* dev1 = NULL;
    OCProvisionDev_t* dev2 = NULL;
    OCProvisionDev_t* tempDev = g_ownedDevices;

    while(tempDev)
    {
        if(memcmp(tempDev->doxm->deviceID.id, myUuid.id, UUID_LENGTH) != 0)
        {
            if(NULL == dev1)
            {
                dev1 = tempDev;
            }
            else if(NULL == dev2)
            {
                dev2 = tempDev;
                break;
            }
        }
        tempDev = tempDev->next;
    }
    EXPECT_TRUE(NULL != dev1);
    EXPECT_TRUE(NULL != dev2);

    // TODO: Pairwise provisioning (Cred & ACL)
    // TODO: This part will be updated after ACL and credential data-structure is updated.

    EXPECT_EQ(OC_STACK_OK, result);
}


TEST(PerformUnlinkDevices, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    // TODO: Unlink devices
    // TODO: This part will be updated after ACL and credential data-structure is updated.

    EXPECT_EQ(OC_STACK_OK, result);
}

//TODO will fix after IOT-2106
#if 0
//#ifdef MULTIPLE_OWNER
static OCProvisionDev_t* g_motEnabledDevices = NULL;
static OCProvisionDev_t* g_multiplOwnedDevices = NULL;

static void updateDoxmForMOTCB(void* ctx, size_t nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    OC_UNUSED(nOfRes);
    OC_UNUSED(arr);

    if(!hasError)
    {
        OIC_LOG_V(DEBUG, TAG, "POST 'doxm' SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(FATAL, TAG, "POST 'doxm'  FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;
}

static void provisionPreconfiguredPinCB(void* ctx, size_t nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    OC_UNUSED(nOfRes);
    OC_UNUSED(arr);

    if(!hasError)
    {
        OIC_LOG_V(DEBUG, TAG, "Provision Preconfigured-PIN SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(FATAL, TAG, "Provision Preconfigured-PIN FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;
}

TEST(EnableMOT, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    if(NULL == g_ownedDevices)
    {
        OIC_LOG(DEBUG, TAG, "Discovering Only Owned Devices on Network..");
        result = OCDiscoverOwnedDevices(DISCOVERY_TIMEOUT, &g_ownedDevices);
        EXPECT_EQ(OC_STACK_OK, result);
        RemoveUnknownDeviceFromDevList(g_ownedDevices);
    }

    EXPECT_NE((OCProvisionDev_t*)NULL, g_ownedDevices);

    g_doneCB = false;
    ASSERT_EQ(OC_STACK_OK, OCChangeMOTMode(NULL, g_ownedDevices, OIC_MULTIPLE_OWNER_ENABLE, updateDoxmForMOTCB));
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(FATAL, TAG, "OCChangeMOTMode callback error");
        return;
    }

    EXPECT_TRUE(g_callbackResult);
}

TEST(DiscoverMOTEnabledDevices, NullParam)
{
    if(g_motEnabledDevices)
    {
        PMDeleteDeviceList(g_motEnabledDevices);
    }

    ASSERT_EQ(OC_STACK_OK, OCDiscoverMultipleOwnerEnabledDevices(DISCOVERY_TIMEOUT, &g_motEnabledDevices));

    RemoveUnknownDeviceFromDevList(g_motEnabledDevices);

    EXPECT_EQ(true, g_motEnabledDevices != NULL);
}

TEST(RegisterPreconfiguredPIN, NullParam)
{
    OCStackResult result = SetPreconfigPin("12341234", strlen("12341234"));
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST(ProvisonPreconfiguredPIN, NullParam)
{
    ASSERT_EQ(true, g_motEnabledDevices != NULL);
    OCStackResult result = OC_STACK_OK;

    g_doneCB = false;
    result = OCProvisionPreconfigPin(NULL, g_motEnabledDevices, "12341234", strlen("12341234"), provisionPreconfiguredPinCB);
    ASSERT_EQ(OC_STACK_OK, result);
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(FATAL, TAG, "OCProvisionPreconfigPin callback error");
        return;
    }

    EXPECT_EQ(true, g_callbackResult);
}

TEST(SelectMOTMethod, NullParam)
{
    ASSERT_EQ(true, g_motEnabledDevices != NULL);
    OCStackResult result = OC_STACK_OK;

    g_doneCB = false;
    result = OCSelectMOTMethod(NULL, g_motEnabledDevices, OIC_PRECONFIG_PIN, updateDoxmForMOTCB);
    ASSERT_EQ(OC_STACK_OK, result);
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(FATAL, TAG, "OCSelectMOTMethod callback error");
        return;
    }

    EXPECT_EQ(true, g_callbackResult);
}

// TODO: Need to new server to perform MOT
TEST(PerformMOT, NullParam)
{
    ASSERT_EQ(true, g_motEnabledDevices != NULL);
    OCStackResult result = OC_STACK_OK;

    g_doneCB = false;
    result = OCDoMultipleOwnershipTransfer(NULL, g_motEnabledDevices, ownershipTransferCB);
    ASSERT_EQ(OC_STACK_OK, result);
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(FATAL, TAG, "OCDoMultipleOwnershipTransfer callback error");
        return;
    }
    EXPECT_EQ(true, g_callbackResult);
}

TEST(DiscoverMultipleOwnedDevices, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    if(g_multiplOwnedDevices)
    {
        PMDeleteDeviceList(g_multiplOwnedDevices);
    }

    OIC_LOG(DEBUG, TAG, "Discovering MOT Enabled Devices on Network..");
    result = OCDiscoverMultipleOwnedDevices(DISCOVERY_TIMEOUT, &g_multiplOwnedDevices);
    ASSERT_EQ(OC_STACK_OK, result);
    RemoveUnknownDeviceFromDevList(g_multiplOwnedDevices);
    EXPECT_TRUE(NULL != g_multiplOwnedDevices);
}
#endif //MULTIPLE_OWNER

TEST(OCRemoveDevice, Simple)
{
    //initialize Provisioning DB Manager
    EXPECT_EQ(OC_STACK_OK, OCInitPM(PM_DB_FILE_NAME));
    ASSERT_EQ(true, gNumOfUnownDevice > 0);

    OicUuid_t myUuid;
    OCStackResult result = OC_STACK_ERROR;
    result = GetDoxmDeviceID(&myUuid);
    EXPECT_EQ(OC_STACK_OK, result);

    //Extract target device except PT to perform remove device.
    OCProvisionDev_t* removeDev = g_ownedDevices;
    while(removeDev)
    {
        if(memcmp(removeDev->doxm->deviceID.id, myUuid.id, UUID_LENGTH) != 0)
        {
            break;
        }
        removeDev = removeDev->next;
    }
    EXPECT_TRUE(NULL != removeDev);

    g_doneCB = false;
    g_callbackResult = false;

    result = OCRemoveDevice((void*)g_otmCtx, DISCOVERY_TIMEOUT, removeDev, removeDeviceCB);
    EXPECT_EQ(OC_STACK_OK, result);
    EXPECT_EQ(true, g_callbackResult);
    EXPECT_EQ(true, g_doneCB);
    // close Provisioning DB
    EXPECT_EQ(OC_STACK_OK, OCClosePM());
}

TEST(FinalizeOTMTest, NullParam)
{
    OCStackResult result = OCStop();
    EXPECT_EQ(OC_STACK_OK, result);

    PMDeleteDeviceList(g_unownedDevices);
    PMDeleteDeviceList(g_ownedDevices);
    result = PDMClose();
    EXPECT_EQ(OC_STACK_OK, result);
}
