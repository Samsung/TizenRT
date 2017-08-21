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
#include <unistd.h>
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
#include "securevirtualresourcetypes.h"
#include "provisioningdatabasemanager.h"
#ifdef MULTIPLE_OWNER
#include "multipleownershiptransfermanager.h"
#endif //MULTIPLE_OWNER
#include "srmutility.h"
#include "doxmresource.h"
#include "pmtypes.h"
#include "pmutility.h"

using namespace std;

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
#define TAG "JUSTWORKS_UNITTEST"
#define OTM_TIMEOUT 60
#define DISCOVERY_TIMEOUT 10

#define SVR_DB_FILE_NAME "oic_svr_db_client.dat"

static bool g_doneCB;
static bool g_callbackResult;
static pid_t g_myPID1;
static pid_t g_myPID2;

static const char* g_otmCtx = "Test User Context";
static OCProvisionDev_t* g_unownedDevices = NULL;
static OCProvisionDev_t* g_ownedDevices = NULL;
#ifdef MULTIPLE_OWNER
static OCProvisionDev_t* g_motEnabledDevices = NULL;
static OCProvisionDev_t* g_multiplOwnedDevices = NULL;
#endif //MULTIPLE_OWNER

static void GetCurrentWorkingDirectory(char* buf, size_t bufsize)
{
    char cwd[1024] = {0};
    const char* unittest_path = "resource/csdk/security/provisioning/unittest";
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if(strstr(cwd, unittest_path) == NULL)
        {
#if defined __linux__
#if __x86_64__
        snprintf(buf, bufsize, "%s/out/linux/x86_64/release/%s/", cwd, unittest_path);
        snprintf(buf, bufsize, "%s/out/linux/x86_64/release/%s/", cwd, unittest_path);
#else
        snprintf(buf, bufsize, "%s/out/linux/x86/release/%s/", cwd, unittest_path);
        snprintf(buf, bufsize, "%s/out/linux/x86/release/%s/", cwd, unittest_path);
#endif //__x86_64__
#endif //defined __linux__
        }
        else
        {
            snprintf(buf, bufsize, "%s/", cwd);
        }
    }
}

static FILE* fopen_prvnMng(const char* path, const char* mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        // input |g_svr_db_fname| internally by force, not using |path| parameter
        // because |OCPersistentStorage::open| is called |OCPersistentStorage| internally
        // with its own |SVR_DB_FILE_NAME|
        char cwd[1024] = { 0 };
        char svr_db_path[1024] = { 0 };
        GetCurrentWorkingDirectory(cwd, sizeof(cwd));
        snprintf(svr_db_path, sizeof(svr_db_path), "%s%s", cwd, SVR_DB_FILE_NAME);
        return fopen(svr_db_path, mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

// callback function(s) for provisioning client using C-level provisioning API
static void ownershipTransferCB(void* ctx, size_t UNUSED1, OCProvisionResult_t* UNUSED2, bool hasError)
{
    OC_UNUSED(UNUSED1);
    OC_UNUSED(UNUSED2);
    OC_UNUSED(ctx);

    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Ownership Transfer SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Ownership Transfer FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;
}

#ifdef MULTIPLE_OWNER
static void updateDoxmForMOTCB(void* ctx, size_t nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    OC_UNUSED(nOfRes);

    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "POST 'doxm' SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "POST 'doxm'  FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;
}

static void provisionPreconfiguredPinCB(void* ctx, size_t nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    OC_UNUSED(nOfRes);

    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision Preconfigured-PIN SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision Preconfigured-PIN FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;
}
#endif //MULTIPLE_OWNER

// callback function(s) for provisioning client using C-level provisioning API
static void removeDeviceCB(void* ctx, size_t UNUSED1, OCProvisionResult_t* UNUSED2, bool hasError)
{
    OC_UNUSED(UNUSED1);
    OC_UNUSED(UNUSED2);
    OC_UNUSED(ctx);

    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Remove device request SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Remove device request FAILED - ctx: %s", (char*) ctx);
    }
    g_callbackResult = !hasError;
    g_doneCB = true;
}


static int waitCallbackRet(void)
{
    struct timespec timeout;
    timeout.tv_sec  = 0;
    timeout.tv_nsec = 100000000L;

    for(long long i=0; !g_doneCB && OTM_TIMEOUT * 100000000L * 1000L > i; ++i)
    {
        nanosleep(&timeout, NULL);
        if(OC_STACK_OK != OCProcess())
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return -1;
        }
    }

    return 0;
}

static void RemoveUnknownDeviceFromDevList(OCProvisionDev_t* devList)
{
    OicUuid_t uuid1;
    OicUuid_t uuid2;
    ConvertStrToUuid("12345678-1234-1234-1234-123456789012", &uuid1);
    ConvertStrToUuid("11111111-1234-1234-1234-123456789012", &uuid2);

    OCProvisionDev_t* unknowDev = NULL;
    OCProvisionDev_t* tempDev1 = NULL;
    OCProvisionDev_t* tempDev2 =NULL;
    LL_FOREACH_SAFE(devList, tempDev1, tempDev2)
    {

        if(!(memcmp(tempDev1->doxm->deviceID.id, uuid1.id, sizeof(uuid1.id)) == 0 ||
             memcmp(tempDev1->doxm->deviceID.id, uuid2.id, sizeof(uuid2.id)) == 0))
        {
            LL_DELETE(devList, tempDev1);
            LL_APPEND(unknowDev, tempDev1);
        }
    }
    OCDeleteDiscoveredDevices(unknowDev);
}

/**
 * Generate default SVR DB path
 *
 * Exclude "out/<OS>/<platform>/<release>/" from current working directory path
 * Replace "unittest" by "sample" at the end of current working directory path
 * Add proper db file name to the end of path
 *
 * @param[in]  cwd  - current working directory
 * @param[out] path - generated default database path
 * @param[in] path_len - allocated length for variable path
 * @return OC_STACK_OK for success.
 */
static OCStackResult GenerateDefaultDbPath(const char *cwd, char *path, size_t path_len)
{
    const char FOLDER_OUT[]      = "out";
    const char FOLDER_UNITTEST[] = "unittest";
    const char FOLDER_SAMPLE[]   = "sample";

#ifdef _WIN32
    const char slash = '\\';
#else
    const char slash = '/';
#endif

    const char slash_str[2] = {slash, 0};

    path[0] = 0;

    char out[5] = {0};
    snprintf(out, sizeof(out), "%c%s%c", slash, FOLDER_OUT, slash);

    char *start = strstr((char*)cwd, out);
    if (NULL == start)
    {
        OIC_LOG_V(ERROR, TAG, "Can't find %s folder while parsing current working directory\n", FOLDER_OUT);
        return OC_STACK_ERROR;
    }
    start++; //Go to next symbol after slash

    char *end = start;
    for (int i = 0; i < 4; i++)
    {
        end = strchr(end, slash);
        if (NULL == end)
        {
            OIC_LOG_V(ERROR, TAG, "Can't find slash number %d while parsing current working directory\n", i);
            return OC_STACK_ERROR;
        }
        end++; //Go to next symbol after slash
    }

    //Cut "unittest" string at the end
    char *last = strstr(end, FOLDER_UNITTEST);
    if (NULL == last)
    {
        OIC_LOG_V(ERROR, TAG, "Can't find %s folder while parsing current working directory\n", FOLDER_UNITTEST);
        return OC_STACK_ERROR;
    }

    //Generate default svr db path
    OICStrcatPartial(path, path_len, cwd, start - cwd); //copy iotivity root path
    OICStrcatPartial(path, path_len, end, last - end); //copy 'resource/.../provisioning' path
    OICStrcatPartial(path, path_len, FOLDER_SAMPLE, sizeof(FOLDER_SAMPLE));
    OICStrcatPartial(path, path_len, slash_str, sizeof(slash_str));
    OICStrcatPartial(path, path_len, SVR_DB_FILE_NAME, sizeof(SVR_DB_FILE_NAME));

    return OC_STACK_OK;
}

TEST(InitForOTM, NullParam)
{
    OCStackResult result = OC_STACK_ERROR;

    OTMCallbackData_t otmcb;
    otmcb.loadSecretCB = LoadSecretJustWorksCallback;
    otmcb.createSecureSessionCB = CreateSecureSessionJustWorksCallback;
    otmcb.createSelectOxmPayloadCB = CreateJustWorksSelectOxmPayload;
    otmcb.createOwnerTransferPayloadCB = CreateJustWorksOwnerTransferPayload;

    static OCPersistentStorage pstStr;
    pstStr.open = fopen_prvnMng;
    pstStr.read = fread;
    pstStr.write = fwrite;
    pstStr.close = fclose;
    pstStr.unlink = unlink;

    //Get current path to execute the sample server.
    char cwd[1024] = {0};
    char server1_path[1024] = {0};
    char server2_path[1024] = {0};
    char pdb_path[1024] = {0};
    char cmd[1024] = {0};

    GetCurrentWorkingDirectory(cwd, sizeof(cwd));
    EXPECT_TRUE(0 < strlen(cwd));

    //Delete previous PDB, if exist.
    GetCurrentWorkingDirectory(cwd, sizeof(cwd));
    snprintf(cmd, sizeof(cmd), "rm -rf %stest.db", cwd);
    system(cmd);

    //Delete previous SVR DB, if exist.
    snprintf(cmd, sizeof(cmd), "rm -rf %s%s", cwd, SVR_DB_FILE_NAME);
    system(cmd);

    char default_svrdb_path[1024] = {0};
    result = GenerateDefaultDbPath(cwd, default_svrdb_path, sizeof(default_svrdb_path));
    EXPECT_EQ(OC_STACK_OK, result);

    //Copy default SVR DB to current folder
    snprintf(cmd, sizeof(cmd), "cp %s .", default_svrdb_path);
    system(cmd);

    //Execute sample server to perform ownership transfer
    int status1 = 0;
    int status2 = 0;
    if(0 == (g_myPID1 = fork()))
    {
        snprintf(server1_path, sizeof(server1_path), "%ssample_server1", cwd);
        status1 = system(server1_path);
        OC_UNUSED(status1);
    }
    if(0 == (g_myPID2 = fork()))
    {
        snprintf(server2_path, sizeof(server2_path), "%ssample_server2", cwd);
        status2 = system(server2_path);
        OC_UNUSED(status2);
    }

    //Waiting for sample server initialization.
    sleep(5);

    // register the persistent storage handler for SVR
    result = OCRegisterPersistentStorageHandler(&pstStr);
    EXPECT_EQ(OC_STACK_OK, result);

    // initialize OC stack and provisioning manager
    result = OCInit(NULL, 0, OC_CLIENT_SERVER);
    EXPECT_EQ(OC_STACK_OK, result);

    //initialize Provisioning DB Manager

    snprintf(pdb_path, sizeof(pdb_path), "%stest.db", cwd);
    result = OCInitPM(pdb_path);
    EXPECT_EQ(OC_STACK_OK, result);

    // register callback function(s) for Justworks OxM
    result = OCSetOwnerTransferCallbackData(OIC_JUST_WORKS, &otmcb);
    EXPECT_EQ(OC_STACK_OK, result);

    g_doneCB = false;
    g_callbackResult = false;
}

TEST(PerformSecureResourceDiscovery, NullParam)
{
    OCStackResult result = OC_STACK_ERROR;
    OCProvisionDev_t* foundDevice = NULL;

    OicUuid_t uuid;
    ConvertStrToUuid("12345678-1234-1234-1234-123456789012", &uuid);

    OIC_LOG(INFO, TAG, "Discovering Owned/Unowned Device using multicast\n");
    result = OCDiscoverSingleDevice(DISCOVERY_TIMEOUT, &uuid, &foundDevice);
    EXPECT_EQ(OC_STACK_OK, result);

    int NumOfFoundDevice = 0;
    OCProvisionDev_t* tempDev = foundDevice;
    while(tempDev)
    {
        NumOfFoundDevice++;
        tempDev = tempDev->next;
    }
    PMDeleteDeviceList(foundDevice);

    EXPECT_EQ(true, NumOfFoundDevice > 0);
}

TEST(PerformUnownedDeviceDiscovery, NullParam)
{
    OCStackResult result = OC_STACK_ERROR;

    OIC_LOG(INFO, TAG, "Discovering Only Unowned Devices on Network..\n");
    result = OCDiscoverUnownedDevices(DISCOVERY_TIMEOUT, &g_unownedDevices);
    EXPECT_EQ(OC_STACK_OK, result);

    RemoveUnknownDeviceFromDevList(g_unownedDevices);

    int NumOfUnownDevice = 0;
    OCProvisionDev_t* tempDev = g_unownedDevices;
    while(tempDev)
    {
        NumOfUnownDevice++;
        tempDev = tempDev->next;
    }

    //if two more devices are running on the subnet,
    //stop the unittest process in order to prevent unittest failure.
    if(2 < NumOfUnownDevice)
    {
        printf("%c[1;33;40m ******************************************************************\n", 0x1B);
        printf("%c[1;33;40m   [%d] devices are running on the your sub-network.\n", 0x1B, NumOfUnownDevice);
        printf("%c[1;33;40m   Please turn off the other devices, and then try once again.\n", 0x1B);
        printf("%c[1;33;40m ******************************************************************", 0x1B);
        printf("%c[1;0;40m \n", 0x1B);

        OCStackResult result = OCStop();
        EXPECT_EQ(OC_STACK_OK, result);

        PMDeleteDeviceList(g_unownedDevices);
        PMDeleteDeviceList(g_ownedDevices);
        result = PDMClose();
        EXPECT_EQ(OC_STACK_OK, result);

        kill(g_myPID2, SIGKILL);
        kill(g_myPID1, SIGKILL);

        int interpreter_res1 = system("pkill -f \"sample_server1\"");
        EXPECT_TRUE(0 <= interpreter_res1);
        int interpreter_res2 = system("pkill -f \"sample_server2\"");
        EXPECT_TRUE(0 <= interpreter_res2);

        exit(0);
    }

    EXPECT_EQ(2, NumOfUnownDevice);
}

TEST(PerformJustWorksOxM, NullParam)
{
    OCStackResult result = OC_STACK_ERROR;

    OIC_LOG(INFO, TAG, "Try Ownership Transfer for Unowned Devices...\n");
    result = OCDoOwnershipTransfer((void*)g_otmCtx, g_unownedDevices, ownershipTransferCB);
    EXPECT_EQ(OC_STACK_OK, result);

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return;
    }
    OIC_LOG(INFO, TAG, "Registered Discovered Unowned Device...\n");

    EXPECT_EQ(true, g_callbackResult);
    EXPECT_EQ(true, g_doneCB);
}


TEST(PerformOwnedDeviceDiscovery, NullParam)
{
    OCStackResult result = OC_STACK_ERROR;

    OIC_LOG(INFO, TAG, "Discovering Only Owned Devices on Network..\n");
    result = OCDiscoverOwnedDevices(DISCOVERY_TIMEOUT, &g_ownedDevices);
    EXPECT_EQ(OC_STACK_OK, result);

    RemoveUnknownDeviceFromDevList(g_ownedDevices);

    int NumOfOwnDevice = 0;
    OCProvisionDev_t* tempDev = g_ownedDevices;
    while(tempDev)
    {
        NumOfOwnDevice++;
        tempDev = tempDev->next;
    }

    EXPECT_EQ(2 , NumOfOwnDevice);
}

TEST(PerformLinkDevices, NullParam)
{
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

#ifdef MULTIPLE_OWNER
TEST(RegisterPreconfiguredPIN, NullParam)
{
    OCStackResult result = SetPreconfigPin("12341234", strlen("12341234"));
    EXPECT_EQ(OC_STACK_OK, result);
}

TEST(EnableMOT, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    if(NULL == g_ownedDevices)
    {
        OIC_LOG(INFO, TAG, "Discovering Only Owned Devices on Network..\n");
        result = OCDiscoverOwnedDevices(DISCOVERY_TIMEOUT, &g_ownedDevices);
        EXPECT_EQ(OC_STACK_OK, result);
        RemoveUnknownDeviceFromDevList(g_ownedDevices);
    }
    EXPECT_NE((OCProvisionDev_t*)NULL, g_ownedDevices);

    g_doneCB = false;
    result = OCChangeMOTMode(NULL, g_ownedDevices, OIC_MULTIPLE_OWNER_ENABLE, updateDoxmForMOTCB);
    EXPECT_EQ(OC_STACK_OK, result);
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCChangeMOTMode callback error");
        return;
    }

    EXPECT_TRUE(g_callbackResult);
}

TEST(DiscoverMOTEnabledDevices, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    if(g_motEnabledDevices)
    {
        PMDeleteDeviceList(g_motEnabledDevices);
    }

    OIC_LOG(INFO, TAG, "Discovering MOT Enabled Devices on Network..\n");
    result = OCDiscoverMultipleOwnerEnabledDevices(DISCOVERY_TIMEOUT, &g_motEnabledDevices);
    EXPECT_EQ(OC_STACK_OK, result);
    RemoveUnknownDeviceFromDevList(g_motEnabledDevices);
    EXPECT_NE((OCProvisionDev_t*)NULL, g_motEnabledDevices);
}

TEST(ProvisonPreconfiguredPIN, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    g_doneCB = false;
    result = OCProvisionPreconfigPin(NULL, g_motEnabledDevices, "12341234", strlen("12341234"), provisionPreconfiguredPinCB);
    EXPECT_EQ(OC_STACK_OK, result);
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionPreconfigPin callback error");
        return;
    }

    EXPECT_EQ(true, g_callbackResult);
}

TEST(SelectMOTMethod, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    g_doneCB = false;
    result = OCSelectMOTMethod(NULL, g_motEnabledDevices, OIC_PRECONFIG_PIN, updateDoxmForMOTCB);
    EXPECT_EQ(OC_STACK_OK, result);
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCSelectMOTMethod callback error");
        return;
    }

    EXPECT_EQ(true, g_callbackResult);
}

// TODO: Need to new server to perform MOT
/*
TEST(PerformMOT, NullParam)
{
    OCStackResult result = OC_STACK_OK;

    g_doneCB = false;
    result = OCDoMultipleOwnershipTransfer(NULL, g_motEnabledDevices, ownershipTransferCB);
    EXPECT_EQ(OC_STACK_OK, result);
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCDoMultipleOwnershipTransfer callback error");
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

    OIC_LOG(INFO, TAG, "Discovering MOT Enabled Devices on Network..\n");
    result = OCDiscoverMultipleOwnedDevices(DISCOVERY_TIMEOUT, &g_multiplOwnedDevices);
    EXPECT_EQ(OC_STACK_OK, result);
    RemoveUnknownDeviceFromDevList(g_multiplOwnedDevices);
    EXPECT_TRUE(NULL != g_multiplOwnedDevices);
}*/

#endif //MULTIPLE_OWNER

TEST(PerformRemoveDevice, NullParam)
{
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
}

TEST(FinalizeOTMTest, NullParam)
{
    OCStackResult result = OCStop();
    EXPECT_EQ(OC_STACK_OK, result);

    PMDeleteDeviceList(g_unownedDevices);
    PMDeleteDeviceList(g_ownedDevices);
    result = PDMClose();
    EXPECT_EQ(OC_STACK_OK, result);

    kill(g_myPID2, SIGKILL);
    kill(g_myPID1, SIGKILL);

    int interpreter_res1 = system("pkill -f \"sample_server1\"");
    EXPECT_TRUE(0 <= interpreter_res1);
    int interpreter_res2 = system("pkill -f \"sample_server2\"");
    EXPECT_TRUE(0 <= interpreter_res2);
}
