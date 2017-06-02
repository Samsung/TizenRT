/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
 *****************************************************************/

#include "iotivity_config.h"

#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "utlist.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocprovisioningmanager.h"
#include "oxmjustworks.h"
#include "oxmrandompin.h"
#include "securevirtualresourcetypes.h"
#include "srmutility.h"
#include "pmtypes.h"
#include "oxmverifycommon.h"
#include "pkix_interface.h"
#include "hw_emul/hw_interface.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

// declaration(s) for provisioning client using C-level provisioning API
// user input definition for main loop on provisioning client
#define _10_DISCOV_ALL_DEVS_            10
#define _11_DISCOV_UNOWN_DEVS_          11
#define _12_DISCOV_OWN_DEVS_            12
#ifdef MULTIPLE_OWNER
#define _13_MOT_DISCOV_DEV_             13
#endif //MULTIPLE_OWNER
#define _20_REGIST_DEVS_                20
#define _30_PROVIS_PAIR_DEVS_           30
#define _31_PROVIS_CRED_                31
#define _32_PROVIS_ACL_                 32
#define _33_PROVIS_DP_                  33
#define _34_CHECK_LINK_STATUS_          34
#define _35_SAVE_ACL_                   35
#define _40_UNLINK_PAIR_DEVS_           40
#define _50_REMOVE_SELEC_DEV_           50
#define _51_REMOVE_DEV_WITH_UUID_       51
#define _52_RESET_SELEC_DEV_            52
#define _53_RESET_SVR_DB_               53
#define _60_GET_CRED_                   60
#define _61_GET_ACL_                    61
#ifdef MULTIPLE_OWNER
#define _70_MOT_CHANGE_MOM_             70
#define _71_MOT_PROV_PRECONF_PIN_       71
#define _72_MOT_OXM_SEL_                72
#define _73_MOT_REMOVE_SUBOWNER_        73
#define _74_MOT_REMOVE_ALL_SUBOWNER_        74
#endif //MULTIPLE_OWNER
#define _80_SELECT_PROTOCOL_            80
#define _81_SELECT_VERIF_METHOD_        81
#define _82_SECURE_STORAGE_HW_EMULATION_    82
#define _99_EXIT_PRVN_CLT_              99

#define ACL_RESRC_MAX_NUM   16
#define ACL_RESRC_ARRAY_SIZE   3 //This value is used only for sample (not OCF spec)
#define ACL_RESRC_MAX_LEN   128
#define ACL_PEMISN_CNT      5
#define DISCOVERY_TIMEOUT   10  // 10 sec
#define CALLBACK_TIMEOUT    60  // 1 min
#define TAG "provisioningclient"

static const char* ACL_PEMISN[5] = {"CREATE", "READ", "WRITE", "DELETE", "NOTIFY"};
static const char* SVR_DB_FILE_NAME = "oic_svr_db_client.dat";
        // '_' for separaing from the same constant variable in |srmresourcestrings.c|
static const char* PRVN_DB_FILE_NAME = "oic_prvn_mng.db";
static const OicSecPrm_t  SUPPORTED_PRMS[1] =
{
    PRM_PRE_CONFIGURED,
};

// |g_ctx| means provision manager application context and
// the following, includes |un/own_list|, could be variables, which |g_ctx| has,
// for accessing all function(s) for these, they are declared on global domain
static const char* g_ctx = "Provision Manager Client Application Context";
static char* g_svr_fname;
static char* g_prvn_fname;
static OCProvisionDev_t* g_own_list;
static OCProvisionDev_t* g_unown_list;
static int g_own_cnt;
static int g_unown_cnt;
#ifdef MULTIPLE_OWNER
static OCProvisionDev_t* g_mot_enable_list;
static int g_mot_enable_cnt;
#endif //MULTIPLE_OWNER

static bool g_doneCB;
#ifdef __WITH_TLS__
static int secure_protocol = 1;
static void setDevProtocol(OCProvisionDev_t* dev_lst);
#endif
// function declaration(s) for calling them before implementing
static OicSecAcl_t* createAcl(const int);
static OicSecAcl_t* createSimpleAcl(const OicUuid_t uuid);
static OicSecPdAcl_t* createPdAcl(const int);
static OCProvisionDev_t* getDevInst(const OCProvisionDev_t*, const int);
static int printDevList(const OCProvisionDev_t*);
static size_t printUuidList(const OCUuidList_t*);
static int printResultList(const OCProvisionResult_t*, const int);
static void printUuid(const OicUuid_t*);
static FILE* fopen_prvnMng(const char*, const char*);
static int waitCallbackRet(void);
static int selectTwoDiffNum(int*, int*, const int, const char*);

// callback function(s) for provisioning client using C-level provisioning API
static void ownershipTransferCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Ownership Transfer SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Ownership Transfer FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void provisionPairwiseCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision Pairwise SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision Pairwise FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void provisionCredCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision Credential SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision Credential FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void provisionAclCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision ACL SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision ACL FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void getCredCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "getCredCB SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "getCredCB FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void getAclCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "getAclCB SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "getAclCB FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void provisionDPCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision Direct-Pairing SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision Direct-Pairing FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void unlinkDevicesCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Unlink Devices SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Unlink Devices FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void removeDeviceCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Remove Device SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Remove Device FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void syncDeviceCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Sync Device SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Sync Device FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

#ifdef MULTIPLE_OWNER
static void updateDoxmForMOTCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "POST 'doxm' SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "POST 'doxm'  FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

static void deleteDoxmForMOTCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "DELETE 'doxm' SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "DELETE 'doxm'  FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

#endif //MULTIPLE_OWNER

static void inputPinCB(char* pin, size_t len)
{
    if(!pin || OXM_RANDOM_PIN_MIN_SIZE > len)
    {
        OIC_LOG(ERROR, TAG, "inputPinCB invalid parameters");
        return;
    }

    printf("   > INPUT PIN: ");
    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%32s", pin);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }
}

// function(s) for provisioning client using C-level provisioning API
static int initProvisionClient(void)
{
    // initialize persistent storage for SVR DB
    static OCPersistentStorage pstStr =
    {
        .open = fopen_prvnMng,
        .read = fread,
        .write = fwrite,
        .close = fclose,
        .unlink = unlink
    };
    if(OC_STACK_OK != OCRegisterPersistentStorageHandler(&pstStr))
    {
        OIC_LOG(ERROR, TAG, "OCRegisterPersistentStorageHandler error");
        return -1;
    }

    // initialize OC stack and provisioning manager
    if(OC_STACK_OK != OCInit(NULL, 0, OC_CLIENT_SERVER))
    {
        OIC_LOG(ERROR, TAG, "OCStack init error");
        return -1;
    }

    if (access(PRVN_DB_FILE_NAME, F_OK) != -1)
    {
        printf("************************************************************\n");
        printf("************Provisioning DB file already exists.************\n");
        printf("************************************************************\n");
    }
    else
    {
        printf("*************************************************************\n");
        printf("************No provisioning DB file, creating new************\n");
        printf("*************************************************************\n");
    }

    if(OC_STACK_OK != OCInitPM(PRVN_DB_FILE_NAME))
    {
        OIC_LOG(ERROR, TAG, "OC_PM init error");
        return -1;
    }

    SetInputPinCB(inputPinCB);

    return 0;
}

static int discoverAllDevices(void)
{
    // delete un/owned device lists before updating them
    if(g_own_list)
    {
        OCDeleteDiscoveredDevices(g_own_list);
        g_own_list = NULL;
    }
    if(g_unown_list)
    {
        OCDeleteDiscoveredDevices(g_unown_list);
        g_unown_list = NULL;
    }

    // call |OCGetDevInfoFromNetwork| API actually
    printf("   Discovering All Un/Owned Devices on Network..\n");
    if(OC_STACK_OK != OCGetDevInfoFromNetwork(DISCOVERY_TIMEOUT, &g_own_list, &g_unown_list))
    {
        OIC_LOG(ERROR, TAG, "OCGetDevInfoFromNetwork API error");
        return -1;
    }

    // display the discovered un/owned lists
    printf("   > Discovered Owned Devices\n");
    g_own_cnt = printDevList(g_own_list);
    printf("   > Discovered Unowned Devices\n");
    g_unown_cnt = printDevList(g_unown_list);
#ifdef __WITH_TLS__
    setDevProtocol(g_own_list);
    setDevProtocol(g_unown_list);
#endif
    return 0;
}


static int discoverUnownedDevices(void)
{
    // delete unowned device list before updating it
    if(g_unown_list)
    {
        OCDeleteDiscoveredDevices(g_unown_list);
        g_unown_list = NULL;
    }

    // call |OCDiscoverUnownedDevices| API actually
    printf("   Discovering Only Unowned Devices on Network..\n");
    if(OC_STACK_OK != OCDiscoverUnownedDevices(DISCOVERY_TIMEOUT, &g_unown_list))
    {
        OIC_LOG(ERROR, TAG, "OCDiscoverUnownedDevices API error");
        return -1;
    }

    // display the discovered unowned list
    printf("   > Discovered Unowned Devices\n");
    g_unown_cnt = printDevList(g_unown_list);
#ifdef __WITH_TLS__
    setDevProtocol(g_unown_list);
#endif
    return 0;
}

static int discoverOwnedDevices(void)
{
    // delete owned device list before updating it
    if(g_own_list)
    {
        OCDeleteDiscoveredDevices(g_own_list);
        g_own_list = NULL;
    }

    // call |OCDiscoverOwnedDevices| API actually
    printf("   Discovering Only Owned Devices on Network..\n");
    if(OC_STACK_OK != OCDiscoverOwnedDevices(DISCOVERY_TIMEOUT, &g_own_list))
    {
        OIC_LOG(ERROR, TAG, "OCDiscoverOwnedDevices API error");
        return -1;
    }

    // display the discovered owned list
    printf("   > Discovered Owned Devices\n");
    g_own_cnt = printDevList(g_own_list);
#ifdef __WITH_TLS__
    setDevProtocol(g_own_list);
#endif
    return 0;
}

#ifdef MULTIPLE_OWNER
static int discoverMOTEnabledDevices(void)
{
    // delete owned device list before updating it
    if(g_mot_enable_list)
    {
        OCDeleteDiscoveredDevices(g_mot_enable_list);
        g_mot_enable_list = NULL;
    }

    // call |OCDiscoverOwnedDevices| API actually
    printf("   Discovering Multiple Ownership Transfer Enabled Devices on Network..\n");
    if(OC_STACK_OK != OCDiscoverMultipleOwnerEnabledDevices(DISCOVERY_TIMEOUT, &g_mot_enable_list))
    {
        OIC_LOG(ERROR, TAG, "OCDiscoverMultipleOwnerEnalbedDevices API error");
        return -1;
    }

    // display the discovered owned list
    printf("   > Discovered Multiple Ownership Transfer Enabled Devices\n");
    g_mot_enable_cnt = printDevList(g_mot_enable_list);

    return 0;
}
#endif //MULTIPLE_OWNER

static int registerDevices(void)
{
    // check |unown_list| for registering devices
    if(!g_unown_list || 0>=g_unown_cnt)
    {
        printf("   > Unowned Device List, to Register Devices, is Empty\n");
        printf("   > Please Discover Unowned Devices first, with [10|11] Menu\n");
        return 0;  // normal case
    }

    // call |OCDoOwnershipTransfer| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Registering All Discovered Unowned Devices..\n");
    OCStackResult rst = OCDoOwnershipTransfer((void*) g_ctx, g_unown_list, ownershipTransferCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoOwnershipTransfer API error: %d", rst);
        return -1;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return -1;
    }

    // display the registered result
    printf("   > Registered Discovered Unowned Devices\n");
    printf("   > Please Discover Owned Devices for the Registered Result, with [10|12] Menu\n");

    return 0;
}

static int provisionPairwise(void)
{
    // check |own_list| for provisioning pairwise devices
    if(!g_own_list || 2>g_own_cnt)
    {
        printf("   > Owned Device List, to Provision the Pairwise, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select two devices for provisioning pairwise devices
    int dev_num[2] = {0};
    if(selectTwoDiffNum(&(dev_num[0]), &(dev_num[1]), g_own_cnt, "for Linking Devices"))
    {
        OIC_LOG(ERROR, TAG, "selectTwoDiffNum error return");
        return -1;  // not need to 'goto' |ERROR| before allocating |acl|
    }

    // create ACL(s) for each selected device
    OicSecAcl_t* acl[2] = {0};
    for(int i=0; 2>i; ++i)
    {
        acl[i] = createAcl(dev_num[i]);
        if(!acl[i])
        {
            OIC_LOG(ERROR, TAG, "createAcl error return");
            goto PVPWS_ERROR;
        }
    }

    // call |OCProvisionPairwiseDevices| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Provisioning Selected Pairwise Devices..\n");
    OCStackResult rst =
            OCProvisionPairwiseDevices((void*) g_ctx,
                    SYMMETRIC_PAIR_WISE_KEY, OWNER_PSK_LENGTH_128,
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num[0]), acl[0],
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num[1]), acl[1],
                    provisionPairwiseCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionPairwiseDevices API error: %d", rst);
        goto PVPWS_ERROR;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        goto PVPWS_ERROR;
    }
    OCDeleteACLList(acl[0]);
    OCDeleteACLList(acl[1]);

    // display the pairwise-provisioned result
    printf("   > Provisioned Selected Pairwise Devices\n");
    printf("   > Please Check Device's Status for the Linked Result, with [33] Menu\n");

    return 0;

PVPWS_ERROR:
    OCDeleteACLList(acl[0]);
    OCDeleteACLList(acl[1]);
    return -1;
}

static int provisionCred(void)
{
    // check |own_list| for provisioning pairwise credentials
    if(!g_own_list || 2>g_own_cnt)
    {
        printf("   > Owned Device List, to Provision Credentials, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select two devices for provisioning pairwise credentials
    int dev_num[2] = {0};
    if(selectTwoDiffNum(&(dev_num[0]), &(dev_num[1]), g_own_cnt, "for Linking CRED(s)"))
    {
        OIC_LOG(ERROR, TAG, "selectTwoDiffNum error return");
        return -1;
    }

    printf("   Select PSK length..\n");
    printf("   1 - 128bit(Default)\n");
    printf("   2 - 256bit\n");
    int sizeOption = 0;

    for(int ret=0; 1!=ret; )
    {
         ret = scanf("%d",&sizeOption);
         for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }
    size_t size = 0;

    switch(sizeOption)
    {
        case 1:
        {
            size = OWNER_PSK_LENGTH_128;
            break;
        }
        case 2:
        {
            size = OWNER_PSK_LENGTH_256;
            break;
        }
        default:
        {
            size = OWNER_PSK_LENGTH_128;
            break;
        }
    }


    // call |OCProvisionCredentials| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Provisioning Selected Pairwise Credentials..\n");
    OCStackResult rst =
            OCProvisionCredentials((void*) g_ctx,
                    SYMMETRIC_PAIR_WISE_KEY, size,
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num[0]),
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num[1]),
                    provisionCredCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionCredentials API error: %d", rst);
        return -1;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return -1;
    }

    // display the CRED-provisioned result
    printf("   > Provisioned Selected Pairwise Crendentials\n");
    printf("   > Please Check Device's Status for the Linked Result, with [34] Menu\n");

    return 0;
}

static int provisionAcl(void)
{
    // check |own_list| for provisioning access control list
    if(!g_own_list || 1>g_own_cnt)
    {
        printf("   > Owned Device List, to Provision ACL, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select device for provisioning access control list
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for Provisioning ACL: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_own_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    // create ACL for selected device
    OicSecAcl_t* acl = NULL;
    acl = createAcl(dev_num);
    if(!acl)
    {
        OIC_LOG(ERROR, TAG, "createAcl error return");
        goto PVACL_ERROR;
    }

    // call |OCProvisionACL| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Provisioning Selected ACL..\n");
    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*) g_own_list, dev_num);
    if(!dev)
    {
        OIC_LOG(ERROR, TAG, "provisionAcl: device instance empty");
        goto PVACL_ERROR;
    }
    OCStackResult rst = OCProvisionACL((void*) g_ctx, dev, acl, provisionAclCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionACL API error: %d", rst);
        goto PVACL_ERROR;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        goto PVACL_ERROR;
    }
    OCDeleteACLList(acl);  // after here |acl| points nothing

    // display the ACL-provisioned result
    printf("   > Provisioned Selected ACL\n");

    return 0;

PVACL_ERROR:
    OCDeleteACLList(acl);  // after here |acl| points nothing
    return -1;
}

static int provisionDirectPairing(void)
{
    // check |own_list| for provisioning direct-pairing
    if(!g_own_list || 1>g_own_cnt)
    {
        printf("   > Owned Device List, to Provision ACL, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select device for provisioning direct-pairing
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for Provisioning Direct-Pairing: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_own_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    // create Direct-Pairing Configuration(PIN, PDACL) for selected device
    // TODO: default acl -> input from user !
    OicSecPconf_t pconf;
    memset(&pconf, 0, sizeof(OicSecPconf_t));

    // set enable dp
    pconf.edp = true;

    // set default supported PRM types
    pconf.prmLen = sizeof(SUPPORTED_PRMS)/sizeof(OicSecPrm_t);
    pconf.prm = (OicSecPrm_t *)OICCalloc(pconf.prmLen, sizeof(OicSecPrm_t));
    if(pconf.prm)
    {
        for (size_t i=0; i<pconf.prmLen; i++)
        {
            pconf.prm[i] = SUPPORTED_PRMS[i];
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "create prm error return");
        goto PVDP_ERROR;
    }

    // set default pin
    const char DP_DEFAULT_PIN[] = "00000000";
    memcpy(pconf.pin.val, DP_DEFAULT_PIN, DP_PIN_LENGTH);

    // set default pdacl
    pconf.pdacls = createPdAcl(dev_num);
    if(!pconf.pdacls)
    {
        OIC_LOG(ERROR, TAG, "createPdAcl error return");
        goto PVDP_ERROR;
    }

    // call |OCProvisionDirectPairing| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Atempt Direct-Pairing Provisioning (PIN : [%s])..\n", (char*)pconf.pin.val);
    OCStackResult rst = OCProvisionDirectPairing((void*) g_ctx,
                                       getDevInst((const OCProvisionDev_t*) g_own_list, dev_num),
                                       &pconf, provisionDPCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionDirectPairing API error: %d", rst);
        if (OC_STACK_UNAUTHORIZED_REQ == rst)
        {
            OIC_LOG(ERROR, TAG, "Target Server NOT Support Direct-Pairing !!! (DPC == false)");
        }
        goto PVDP_ERROR;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        goto PVDP_ERROR;
    }
    OCDeletePdAclList(pconf.pdacls);

    // display the PCONF-provisioned result
    printf("   > SUCCESS to provision Direct-Pairing !!\n");

    return 0;

PVDP_ERROR:
    OCDeletePdAclList(pconf.pdacls);  // after here |acl| points nothing
    return -1;
}

static int checkLinkedStatus(void)
{
    // check |own_list| for checking selected link status on PRVN DB
    if(!g_own_list || 1>g_own_cnt)
    {
        printf("   > Owned Device List, to Check Linked Status on PRVN DB, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select device for checking selected link status on PRVN DB
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for Checking Linked Status on PRVN DB: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_own_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    // call |OCGetLinkedStatus| API actually
    printf("   Checking Selected Link Status on PRVN DB..\n");
    OCUuidList_t* dvid_lst = NULL;
    size_t dvid_cnt = 0;
    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*)g_own_list, dev_num);
    if(!dev || !dev->doxm)
    {
        OIC_LOG(ERROR, TAG, "checkLinkedStatus: device instance empty");
        goto CKLST_ERROR;
    }

    if(OC_STACK_OK !=
            OCGetLinkedStatus(
                    &dev->doxm->deviceID,
                    &dvid_lst, &dvid_cnt))  // allow empty list
    {
        OIC_LOG(ERROR, TAG, "OCGetLinkedStatus API error");
        goto CKLST_ERROR;
    }

    // display the linked status result
    printf("   > Checked Selected Link Status on PRVN DB\n");
    if(!dvid_lst || !dvid_cnt)  // |size_t| is unsigned
    {
        printf("     Linked Device List is Empty..\n");
        return 0;  // normal case
    }
    if(dvid_cnt != printUuidList((const OCUuidList_t*) dvid_lst))
    {
        OIC_LOG(ERROR, TAG, "printUuidList error return");
        goto CKLST_ERROR;
    }
    OCDeleteUuidList(dvid_lst);

    return 0;

CKLST_ERROR:
    OCDeleteUuidList(dvid_lst);
    return -1;
}

static int saveAcl(void)
{
    // create ACL to save into local SVR DB
    OicSecAcl_t* acl = NULL;
    OicUuid_t uuid =   {.id={0}};
    char strUuid[64] = {0};

    printf("[1] Use a test UUID [11111111-2222-3333-4444-555555555555]\n");
    printf("[2] Use a user input\n");
    int sel_num = 0;
    for( ; ; )
    {
        printf("   > Select Number, for Subject UUID of new ACE: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &sel_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(1 == sel_num)
        {
            OICStrcpy(strUuid, sizeof(strUuid), "11111111-2222-3333-4444-555555555555");
            break;
        }
        else if(2 == sel_num)
        {
            printf("   > Input the UUID : ");
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%64s", strUuid);
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
            }
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }


    printf("Selected Subject UUID : %s\n", strUuid);
    OCStackResult rst = ConvertStrToUuid(strUuid, &uuid);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "ConvertStrToUuid API error: %d", rst);
        goto SVACL_ERROR;
    }

    acl = createSimpleAcl(uuid);
    if(!acl)
    {
        OIC_LOG(ERROR, TAG, "createAcl error return");
        goto SVACL_ERROR;
    }

    // call |OCSaveACL| API actually
    rst = OCSaveACL(acl);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCSaveACL API error: %d", rst);
        goto SVACL_ERROR;
    }
    OCDeleteACLList(acl);  // after here |acl| points nothing

    // display the ACL-provisioned result
    printf("   > Saved Selected ACL\n");

    return 0;

SVACL_ERROR:
    OCDeleteACLList(acl);  // after here |acl| points nothing
    return -1;
}

static int getCred(void)
{
    // check |own_list| for checking selected link status on PRVN DB
    if(!g_own_list || 1>g_own_cnt)
    {
        printf("   > Owned Device List, to Check Linked Status on PRVN DB, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select device for checking selected link status on PRVN DB
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for Checking Linked Status on PRVN DB: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_own_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    // call |getDevInst| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*) g_own_list, dev_num);
    if(!dev)
    {
        OIC_LOG(ERROR, TAG, "getDevInst: device instance empty");
        goto PVACL_ERROR;
    }
    OCStackResult rst = OCGetCredResource((void*) g_ctx, dev, getCredCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCGetCred API error: %d", rst);
        goto PVACL_ERROR;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCGetCredResource callback error");
        goto PVACL_ERROR;
    }

    // display the result of get credential
    printf("   > Get Cred SUCCEEDED\n");

    return 0;

PVACL_ERROR:
    return -1;
}

static int getAcl(void)
{
    // check |own_list| for checking selected link status on PRVN DB
    if(!g_own_list || 1>g_own_cnt)
    {
        printf("   > Owned Device List, to Check Linked Status on PRVN DB, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select device for checking selected link status on PRVN DB
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for Checking Linked Status on PRVN DB: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_own_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    // call |getDevInst| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*) g_own_list, dev_num);
    if(!dev)
    {
        OIC_LOG(ERROR, TAG, "getDevInst: device instance empty");
        goto PVACL_ERROR;
    }
    OCStackResult rst = OCGetACLResource((void*) g_ctx, dev, getAclCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCGetACLResource API error: %d", rst);

        goto PVACL_ERROR;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCGetACLResource callback error");
        goto PVACL_ERROR;
    }

    // display the result of get credential
    printf("   > Get ACL SUCCEEDED\n");

    return 0;

PVACL_ERROR:
    return -1;
}

static int unlinkPairwise(void)
{
    // check |own_list| for unlinking pairwise devices
    if(!g_own_list || 2>g_own_cnt)
    {
        printf("   > Owned Device List, to Unlink the Pairwise, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select two devices for unlinking pairwise devices
    int dev_num[2] = {0};
    if(selectTwoDiffNum(&(dev_num[0]), &(dev_num[1]), g_own_cnt, "for Unlinking Devices"))
    {
        OIC_LOG(ERROR, TAG, "selectTwoDiffNum error return");
        return -1;
    }

    // call |OCUnlinkDevices| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Unlinking Selected Pairwise Devices..\n");
    OCStackResult rst =
            OCUnlinkDevices((void*) g_ctx,
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num[0]),
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num[1]),
                    unlinkDevicesCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCUnlinkDevices API error: %d", rst);
        return -1;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return -1;
    }

    // display the pairwise-unlinked result
    printf("   > Unlinked Selected Pairwise Devices\n");
    printf("   > Please Check Device's Status for the Unlinked Result, with [33] Menu\n");

    return 0;
}

static int removeDevice(void)
{
    // check |own_list| for removing device
    if(!g_own_list || 1>g_own_cnt)
    {
        printf("   > Owned Device List, to Remove Device, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;  // normal case
    }

    // select device for removing it
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for Removing Device: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_own_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    // call |OCRemoveDevice| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Removing Selected Owned Device..\n");
    OCStackResult rst =
            OCRemoveDevice((void*) g_ctx, DISCOVERY_TIMEOUT,
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num), removeDeviceCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCRemoveDevice API error: %d", rst);
        return -1;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return -1;
    }

    // display the removed result
    printf("   > Removed Selected Owned Device\n");
    printf("   > Please Discover Owned Devices for the Registered Result, with [10|12] Menu\n");

    return 0;
}

static int removeDeviceWithUuid(void)
{
    char strUuid[64] = {0};
    OicUuid_t revUuid;
    printf("Input the UUID : ");
    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%63s", strUuid);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }
    OCStackResult rst = ConvertStrToUuid(strUuid, &revUuid);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "ConvertStrToUuid API error: %d", rst);
        return -1;
    }

    g_doneCB = false;
    rst = OCRemoveDeviceWithUuid("RemoveDeviceWithUUID", DISCOVERY_TIMEOUT, &revUuid, removeDeviceCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCRemoveDeviceWithUuid API error: %d", rst);
        return -1;
    }

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCRemoveDeviceWithUuid callback error");
        return -1;
    }

    // display the removed result
    printf("   > Removed %s Device\n", strUuid);
    printf("   > Please Discover Owned Devices for the Registered Result, with [10|12] Menu\n");

    return 0;
}

OCStackResult displayNumCB(void * ctx, uint8_t mutualVerifNum[MUTUAL_VERIF_NUM_LEN])
{
    OIC_LOG(INFO, TAG, "IN displayMutualVerifNumCB");
    if (NULL != mutualVerifNum)
    {
        OIC_LOG(INFO, TAG, "############ mutualVerifNum ############");
        OIC_LOG_BUFFER(INFO, TAG, mutualVerifNum, MUTUAL_VERIF_NUM_LEN);
        OIC_LOG(INFO, TAG, "############ mutualVerifNum ############");
        OIC_LOG(INFO, TAG, "OUT displayMutualVerifNumCB");
    }
    else
    {
        OIC_LOG(INFO, TAG, "############ Confirm on the Server side ############");
    }
    return OC_STACK_OK;
}

OCStackResult confirmNumCB(void * ctx)
{
    for (;;)
    {
        int userConfirm;

        printf("   > Press 1 if the mutual verification numbers are the same\n");
        printf("   > Press 0 if the mutual verification numbers are not the same\n");

        for (int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &userConfirm);
            for (; 0x20<=getchar(); );  // for removing overflow garbage
                                        // '0x20<=code' is character region
        }
        if (1 == userConfirm)
        {
            break;
        }
        else if (0 == userConfirm)
        {
            return OC_STACK_USER_DENIED_REQ;
        }
        printf("   Entered Wrong Number. Please Enter Again\n");
    }
    return OC_STACK_OK;
}

#ifdef MULTIPLE_OWNER
static int changeMultipleOwnershipTrnasferMode(void)
{
    // check |own_list| for removing device
    if(!g_own_list || 1>g_own_cnt)
    {
        printf("   > Owned Device List is Empty\n");
        printf("   > Please Discover the Owned Devices, with [12] Menu\n");
        return 0;  // normal case
    }

    // select device for removing it
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for MOT Device: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_own_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    int mom = 0;
    for( ; ; )
    {
        printf("   0. Disable Multuple Ownership Transfer\n");
        printf("   1. Enable Multuple Ownership Transfer\n");
        printf("   2. (Not Supported yet) Timely Enable Multuple Ownership Transfer\n");
        printf("   > Enter Mode of Multuple Ownership Transfer : ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &mom);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0 <= dev_num && OIC_NUMBER_OF_MOM_TYPE > dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* motDev = getDevInst(g_own_list, dev_num);
    if(OC_STACK_OK == OCChangeMOTMode(NULL, motDev, (OicSecMomType_t)dev_num, updateDoxmForMOTCB))
    {
        g_doneCB = false;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "OCChangeMOTMode API error");
        return -1;
    }

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "waitCallbackRet callback error");
        return -1;
    }

    return 0;
}

static int selectMultipleOwnershipTrnasferMethod(void)
{
    // check |own_list| for removing device
    if(!g_mot_enable_list || 1>g_mot_enable_cnt)
    {
        printf("   > Multiple Ownership Transfer Enabled Device List is Empty\n");
        printf("   > Please Discover the Multiple Ownership Transfer Enabled Devices, with [13] Menu\n");
        return 0;  // normal case
    }

    // select device for removing it
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for MOT Device: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_mot_enable_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    const int preconfOxm = 4;
    int oxm = 0;
    for( ; ; )
    {
        printf("   %d. (Not Supported)\n", OIC_JUST_WORKS);
        printf("   %d. Random PIN OxM\n", OIC_RANDOM_DEVICE_PIN);
        printf("   %d. (Not Supported)\n", OIC_MANUFACTURER_CERTIFICATE);
        printf("   %d. (Not Supported)\n", OIC_DECENTRALIZED_PUBLIC_KEY);
        printf("   %d. Pre-Configured PIN OxM\n", OIC_PRECONFIG_PIN);
        printf("   > Enter Number of  OxM for Multiple Ownership Transfer : ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &oxm);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(OIC_PRECONFIG_PIN == oxm || OIC_RANDOM_DEVICE_PIN == oxm)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* motDev = getDevInst(g_mot_enable_list, dev_num);
    if(OC_STACK_OK ==  OCSelectMOTMethod(NULL, motDev, (OicSecOxm_t)oxm, updateDoxmForMOTCB))
    {
        g_doneCB = false;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "OCSelectMOTMethod API error");
        return -1;
    }

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "waitCallbackRet callback error");
        return -1;
    }

    return 0;
}

static int provisionPreconfigPIN()
{
    // check |own_list| for removing device
    if(!g_mot_enable_list || 1>g_mot_enable_cnt)
    {
        printf("   > Multiple Ownership Transfer Enabled Device List is Empty\n");
        printf("   > Please Discover the Multiple Ownership Transfer Enabled Devices, with [13] Menu\n");
        return 0;  // normal case
    }

    // select device for removing it
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for MOT Device: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_mot_enable_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    char preconfigPin[9] = {0};
    printf("   > Input the PreconfigPin (e.g. 12341234) : ");
    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%8s", preconfigPin);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    OCProvisionDev_t* motDev = getDevInst(g_mot_enable_list, dev_num);
    if(OC_STACK_OK == OCProvisionPreconfigPin(NULL, motDev, preconfigPin, strlen(preconfigPin), provisionCredCB))
    {
        g_doneCB = false;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "OCProvisionPreconfigPin API error");
        return -1;
    }

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "waitCallbackRet callback error");
        return -1;
    }

    return 0;
}

static int removeSubOwner(void)
{
    // check |g_mot_enable_list| for removing sub-owner
    if (!g_mot_enable_list || 1 > g_mot_enable_cnt)
    {
        printf("   > Multiple Ownership Transfer Enabled Device List is Empty\n");
        printf("   > Please Discover the Multiple Ownership Transfer Enabled Devices, with [13] Menu\n");
        return 0;  // normal case
    }

    // select resource server for removing sub-owner
    int dev_num = 0;
    for ( ; ; )
    {
        printf("   > Enter Device Number to remove sub-owner: ");
        for (int ret = 0; 1 != ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20 <= getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if (0 < dev_num && g_mot_enable_cnt >= dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* motDev = getDevInst(g_mot_enable_list, dev_num);
    if (motDev && motDev->doxm && motDev->doxm->subOwners)
    {
        OicSecSubOwner_t* subOwner = motDev->doxm->subOwners;
        int so_cnt = 0;
        while(subOwner)
        {
            printf("     [%zu] ", ++so_cnt);
            printUuid(&subOwner->uuid);
            printf("\n");
            subOwner = subOwner->next;
        }

        int so_num = 0;
        for ( ; ; )
        {
            printf("   > Enter SubOwner Number to be removed : ");
            for (int ret = 0; 1 != ret; )
            {
                ret = scanf("%d", &so_num);
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            if (0 < so_num && so_cnt >= so_num)
            {
                int target_num = 0;
                subOwner = motDev->doxm->subOwners;
                while (subOwner)
                {
                    if(so_num == ++target_num)
                    {
                        if (OC_STACK_OK != OCRemoveSubOwner(NULL, motDev, &subOwner->uuid, deleteDoxmForMOTCB))
                        {
                            return -1;
                        }

                        g_doneCB = false;

                        if(waitCallbackRet())  // input |g_doneCB| flag implicitly
                        {
                            OIC_LOG(ERROR, TAG, "waitCallbackRet callback error");
                            return -1;
                        }
                        return 0;
                    }
                    subOwner = subOwner->next;
                }
                break;
            }
            printf("     Entered Wrong Number. Please Enter Again\n");
        }
    }
    else
    {
        printf("     SubOwner list is empty.\n");
    }

    return 0;
}

static int removeAllSubOwner(void)
{
    // check |g_mot_enable_list| for removing sub-owner
    if (!g_mot_enable_list || 1 > g_mot_enable_cnt)
    {
        printf("   > Multiple Ownership Transfer Enabled Device List is Empty\n");
        printf("   > Please Discover the Multiple Ownership Transfer Enabled Devices, with [13] Menu\n");
        return 0;  // normal case
    }

    // select resource server for removing sub-owner
    int dev_num = 0;
    for ( ; ; )
    {
        printf("   > Enter Device Number to remove sub-owner: ");
        for (int ret = 0; 1 != ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20 <= getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if (0 < dev_num && g_mot_enable_cnt >= dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* motDev = getDevInst(g_mot_enable_list, dev_num);
    if (motDev && motDev->doxm && motDev->doxm->subOwners)
    {
        if (OC_STACK_OK != OCRemoveAllSubOwner(NULL, motDev, deleteDoxmForMOTCB))
        {
            return -1;
        }

        g_doneCB = false;

        if(waitCallbackRet())  // input |g_doneCB| flag implicitly
        {
            OIC_LOG(ERROR, TAG, "waitCallbackRet callback error");
            return -1;
        }
        return 0;
    }
    else
    {
        printf("     SubOwner list is empty.\n");
    }

    return 0;
}

#endif //MULTIPLE_OWNER

static int resetDevice(void)
{
    // check |own_list| for removing device
    if (!g_own_list || 1 > g_own_cnt)
    {
        printf("   > Owned Device List, to Reset Device, is Empty\n");
        printf("   > Please Register Unowned Devices first, with [20] Menu\n");
        return 0;
    }

    // select device for removing it
    int dev_num = 0;
    for ( ; ; )
    {
        printf("   > Enter Device Number, for Resetting Device: ");
        for (int ret = 0; 1 != ret; )
        {
            ret = scanf("%d", &dev_num);
            for ( ; 0x20 <= getchar() ; );  // for removing overflow garbages
                                            // '0x20 <= code' is character region
        }
        if (0 < dev_num && g_own_cnt >= dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    g_doneCB = false;
    printf("   Resetting Selected Owned Device..\n");

    OCStackResult rst = OCResetDevice((void *) g_ctx, DISCOVERY_TIMEOUT,
                    getDevInst((const OCProvisionDev_t *) g_own_list, dev_num), syncDeviceCB);
    if (OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCResetDevice API error: %d", rst);
        return -1;
    }

    if (waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return -1;
    }

    // display the removed result
    printf("   > Reset Selected Owned Device SUCCEEDED\n");
    printf("   > Please Discover Owned Devices for the Registered Result, with [10|12] Menu\n");

    return 0;
}

static int resetSVRDB(void)
{
    printf("   Resetting SVR DB..\n");
    OCStackResult rst = OCResetSVRDB();
    if (OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCResetSVRDB API error: %d", rst);
        return -1;
    }
    return 0;
}

static OicSecAcl_t* createAcl(const int dev_num)
{
    if(0>=dev_num || g_own_cnt<dev_num)
    {
        OIC_LOG(ERROR, TAG, "createAcl invalid parameters");
        return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
    }

    // allocate memory for |acl| struct
    printf("   **** Create ACL for the Selected Device[%d]\n", dev_num);
    OicSecAcl_t* acl = (OicSecAcl_t*) OICCalloc(1, sizeof(OicSecAcl_t));
    if(!acl)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
    }
    OicSecAce_t* ace = (OicSecAce_t*) OICCalloc(1, sizeof(OicSecAce_t));
    if(!ace)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
    }
    LL_APPEND(acl->aces, ace);

    // enter |subject| device number
    int num = 0;
    for( ; ; )
    {
        printf("   > [A] Enter Subject Device Number: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<num && g_own_cnt>=num && dev_num!=num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*)g_own_list, num);
    if(!dev || !dev->doxm)
    {
        OIC_LOG(ERROR, TAG, "createAcl: device instance empty");
        goto CRACL_ERROR;
    }
    memcpy(&ace->subjectuuid, &dev->doxm->deviceID, UUID_LENGTH);

    // enter number of |resources| in 'accessed' device
    for( ; ; )
    {
        printf("   > [B] Enter Number of Accessed Resources (under 16): ");
                // '16' is |ACL_RESRC_MAX_NUM|
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<num && ACL_RESRC_MAX_NUM>=num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter under 16 Again\n");
                // '16' is |ACL_RESRC_MAX_NUM|
    }

    // enter actually each 'accessed' |resources| name
    printf("         Enter Each Accessed Resource Name (each under 128 char)\n");
            // '128' is ACL_RESRC_MAX_LEN

    char rsrc_in[ACL_RESRC_MAX_LEN+1] = {0};  // '1' for null termination
    for(int i = 0; num > i; ++i)
    {
        OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
        if(!rsrc)
        {
            OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
            goto CRACL_ERROR;
        }

        printf("         Enter Accessed Resource[%d] Name: (e.g. /a/led)", i+1);
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%128s", rsrc_in);  // '128' is ACL_RESRC_MAX_LEN
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        size_t len = strlen(rsrc_in)+1;  // '1' for null termination
        rsrc->href = (char*) OICCalloc(len, sizeof(char));
        if(!rsrc->href)
        {
            OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
            goto CRACL_ERROR;
        }
        OICStrcpy(rsrc->href, len, rsrc_in);

        size_t arrLen = 0;
        while(1)
        {
            printf("         Enter Number of resource type for [%s] : ", rsrc->href);
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%zu", &arrLen);
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            if(ACL_RESRC_ARRAY_SIZE >= arrLen)
            {
                break;
            }
            printf("     Entered Wrong Number. Please Enter under %d Again\n", ACL_RESRC_ARRAY_SIZE);
        }

        rsrc->typeLen = arrLen;
        rsrc->types = (char**)OICCalloc(arrLen, sizeof(char*));
        if(!rsrc->types)
        {
            OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
            goto CRACL_ERROR;
        }

        for(size_t i = 0; i < arrLen; i++)
        {
            printf("         Enter ResourceType[%zu] Name (e.g. core.led): ", i+1);
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%128s", rsrc_in);  // '128' is ACL_RESRC_MAX_LEN
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            rsrc->types[i] = OICStrdup(rsrc_in);
            if(!rsrc->types[i])
            {
                OIC_LOG(ERROR, TAG, "createAcl: OICStrdup error return");
                goto CRACL_ERROR;
            }
        }

        while(1)
        {
            printf("         Enter Number of interface for [%s]: ", rsrc->href);
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%zu", &arrLen);
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            if(ACL_RESRC_ARRAY_SIZE >= arrLen)
            {
                break;
            }
            printf("     Entered Wrong Number. Please Enter under %d Again\n", ACL_RESRC_ARRAY_SIZE);
        }

        rsrc->interfaceLen = arrLen;
        rsrc->interfaces = (char**)OICCalloc(arrLen, sizeof(char*));
        if(!rsrc->interfaces)
        {
            OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
            goto CRACL_ERROR;
        }

        for(size_t i = 0; i < arrLen; i++)
        {
            printf("         Enter Interface[%zu] Name (e.g. oic.if.baseline): ", i+1);
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%128s", rsrc_in);  // '128' is ACL_RESRC_MAX_LEN
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            rsrc->interfaces[i] = OICStrdup(rsrc_in);
            if(!rsrc->interfaces[i])
            {
                OIC_LOG(ERROR, TAG, "createAcl: OICStrdup error return");
                goto CRACL_ERROR;
            }
        }

        LL_APPEND(ace->resources, rsrc);
    }

    // enter |permission| for this access
    printf("   > [C] Enter Permission for This Access\n");
    uint16_t pmsn = PERMISSION_FULL_CONTROL;  // default full permission
    uint16_t pmsn_msk = PERMISSION_CREATE;  // default permission mask
    for(int i=0; ACL_PEMISN_CNT>i; ++i)
    {
        char ans = 0;
        for( ; ; )
        {
            printf("         Enter %s Permission (y/n): ", ACL_PEMISN[i]);
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%c", &ans);
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            if('y'==ans || 'Y'==ans || 'n'==ans|| 'N'==ans)
            {
                ans &= ~0x20;  // for masking lower case, 'y/n'
                break;
            }
            printf("         Entered Wrong Answer. Please Enter 'y/n' Again\n");
        }
        if('N' == ans)  // masked lower case, 'n'
        {
            pmsn -= pmsn_msk;
        }
        pmsn_msk <<= 1;
    }
    ace->permission = pmsn;

    return acl;

CRACL_ERROR:
    OCDeleteACLList(acl);  // after here |acl| points nothing
    return NULL;
}

static OicSecAcl_t* createSimpleAcl(const OicUuid_t uuid)
{
    OIC_LOG(DEBUG, TAG, "createSimpleAcl IN");

    // allocate memory for |acl| struct
    OicSecAcl_t* acl = (OicSecAcl_t*) OICCalloc(1, sizeof(OicSecAcl_t));
    if(!acl)
    {
        OIC_LOG(DEBUG, TAG, "OICCalloc error return");
        return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
    }
    OicSecAce_t* ace = (OicSecAce_t*) OICCalloc(1, sizeof(OicSecAce_t));
    if(!ace)
    {
        OIC_LOG(DEBUG, TAG,  "OICCalloc error return");
        return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
    }
    LL_APPEND(acl->aces, ace);

    memcpy(&ace->subjectuuid, &uuid, UUID_LENGTH);

    OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    if(!rsrc)
    {
        OIC_LOG(DEBUG, TAG, "OICCalloc error return");
        OCDeleteACLList(acl);
        return NULL;
    }

    char href[] = "*";
    size_t len = strlen(href)+1;  // '1' for null termination
    rsrc->href = (char*) OICCalloc(len, sizeof(char));
    if(!rsrc->href)
    {
        OIC_LOG(DEBUG, TAG,  "OICCalloc error return");
        OCDeleteACLList(acl);
        return NULL;
    }
    OICStrcpy(rsrc->href, len, href);

    size_t arrLen = 1;
    rsrc->typeLen = arrLen;
    rsrc->types = (char**)OICCalloc(arrLen, sizeof(char*));
    if(!rsrc->types)
    {
        OIC_LOG(DEBUG, TAG,  "OICCalloc error return");
        OCDeleteACLList(acl);
        return NULL;
    }
    rsrc->types[0] = OICStrdup("");   // ignore

    rsrc->interfaceLen = 1;
    rsrc->interfaces = (char**)OICCalloc(arrLen, sizeof(char*));
    if(!rsrc->interfaces)
    {
        OIC_LOG(DEBUG, TAG,  "OICCalloc error return");
        OCDeleteACLList(acl);
        return NULL;
    }
    rsrc->interfaces[0] = OICStrdup("oic.if.baseline");  // ignore

    LL_APPEND(ace->resources, rsrc);

    ace->permission = 31;   // R/W/U/D

    OIC_LOG(DEBUG, TAG, "createSimpleAcl OUT");

    return acl;
}

static OicSecPdAcl_t* createPdAcl(const int dev_num)
{
    if(0>=dev_num || g_own_cnt<dev_num)
    {
        OIC_LOG(ERROR, TAG, "createAcl invalid parameters");
        return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
    }

    // allocate memory for |pdacl| struct
    printf("   **** Create PDACL for the Selected Device[%d]\n", dev_num);
    OicSecPdAcl_t* pdAcl = (OicSecPdAcl_t*) OICCalloc(1, sizeof(OicSecPdAcl_t));
    if(!pdAcl)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
    }


    // number of resources
    char rsrc_in[][ACL_RESRC_MAX_LEN+1] = {"*", "/rsrc/*"};
    pdAcl->resourcesLen = 1;

    // resource
    int num = pdAcl->resourcesLen;
    pdAcl->resources = (char**) OICCalloc(num, sizeof(char*));
    if(!pdAcl->resources)
    {
        OIC_LOG(ERROR, TAG, "createPdAcl: OICCalloc error return");
        goto CRPDACL_ERROR;
    }
    for(int i=0; num>i; ++i)
    {
        size_t len = strlen(rsrc_in[i])+1;  // '1' for null termination
        char* rsrc = (char*) OICCalloc(len, sizeof(char));
        if(!rsrc)
        {
            OIC_LOG(ERROR, TAG, "createPdAcl: OICCalloc error return");
            goto CRPDACL_ERROR;
        }
        OICStrcpy(rsrc, len, rsrc_in[i]);
        pdAcl->resources[i] = rsrc;  // after here, |rsrc| points nothing
    }

    // permission
    pdAcl->permission = PERMISSION_FULL_CONTROL;

    return pdAcl;

CRPDACL_ERROR:
    OCDeletePdAclList(pdAcl);
    return NULL;
}

static OCProvisionDev_t* getDevInst(const OCProvisionDev_t* dev_lst, const int dev_num)
{
    if(!dev_lst || 0>=dev_num)
    {
        printf("     Device List is Empty..\n");
        return NULL;
    }

    OCProvisionDev_t* lst = (OCProvisionDev_t*) dev_lst;
    for(int i=0; lst; )
    {
        if(dev_num == ++i)
        {
            return lst;
        }
        lst = lst->next;
    }

    return NULL;  // in here |lst| is always |NULL|
}

static int printDevList(const OCProvisionDev_t* dev_lst)
{
    if(!dev_lst)
    {
        printf("     Device List is Empty..\n\n");
        return 0;
    }

    OCProvisionDev_t* lst = (OCProvisionDev_t*) dev_lst;
    int lst_cnt = 0;
    for( ; lst; )
    {
        printf("     [%d] ", ++lst_cnt);
        printUuid((const OicUuid_t*) &lst->doxm->deviceID);
        printf("\n");
        lst = lst->next;
    }
    printf("\n");

    return lst_cnt;
}

static size_t printUuidList(const OCUuidList_t* uid_lst)
{
    if(!uid_lst)
    {
        printf("     Device List is Empty..\n\n");
        return 0;
    }

    OCUuidList_t* lst = (OCUuidList_t*) uid_lst;
    size_t lst_cnt = 0;
    for( ; lst; )
    {
        printf("     [%zu] ", ++lst_cnt);
        printUuid((const OicUuid_t*) &lst->dev);
        printf("\n");
        lst = lst->next;
    }
    printf("\n");

    return lst_cnt;
}

static int printResultList(const OCProvisionResult_t* rslt_lst, const int rslt_cnt)
{
    if(!rslt_lst || 0>=rslt_cnt)
    {
        printf("     Device List is Empty..\n\n");
        return 0;
    }

    int lst_cnt = 0;
    for( ; rslt_cnt>lst_cnt; ++lst_cnt)
    {
        printf("     [%d] ", lst_cnt+1);
        printUuid((const OicUuid_t*) &rslt_lst[lst_cnt].deviceId);
        printf(" - result: %d\n", rslt_lst[lst_cnt].res);
    }
    printf("\n");

    return lst_cnt;
}

static void printUuid(const OicUuid_t* uid)
{
    for(int i=0; i<UUID_LENGTH; )
    {
        printf("%02X", (*uid).id[i++]);
        if(i==4 || i==6 || i==8 || i==10)  // canonical format for UUID has '8-4-4-4-12'
        {
            printf("-");
        }
    }
}

static FILE* fopen_prvnMng(const char* path, const char* mode)
{
    (void)path;  // unused |path| parameter

    // input |g_svr_db_fname| internally by force, not using |path| parameter
    // because |OCPersistentStorage::open| is called |OCPersistentStorage| internally
    // with its own |SVR_DB_FILE_NAME|
    return fopen(SVR_DB_FILE_NAME, mode);
}

static int waitCallbackRet(void)
{
    for(int i=0; !g_doneCB && CALLBACK_TIMEOUT>i; ++i)
    {
        sleep(1);
        if(OC_STACK_OK != OCProcess())
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return -1;
        }
    }

    if(!g_doneCB)
    {
        OCPDMCleanupForTimeout();
    }

    return 0;
}

static int selectTwoDiffNum(int* a, int* b, const int max, const char* str)
{
    if(!a || !b || 2>max || !str)
    {
        return -1;
    }

    for( ; ; )
    {
        for(int i=0; 2>i; ++i)
        {
            int* num = 0==i?a:b;
            for( ; ; )
            {
                printf("   > Enter Device[%d] Number, %s: ", i+1, str);
                for(int ret=0; 1!=ret; )
                {
                    ret = scanf("%d", num);
                    for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                                // '0x20<=code' is character region
                }
                if(0<*num && max>=*num)
                {
                    break;
                }
                printf("     Entered Wrong Number. Please Enter Again\n");
            }
        }
        if(*a != *b)
        {
            printf("\n");
            return 0;
        }
    }

    return -1;
}

#ifdef __WITH_TLS__

static void setDevProtocol(OCProvisionDev_t* lst)
{
    if(!lst)
    {
        printf("     Device List is Empty..\n\n");
        return;
    }

    for( ; lst; )
    {
        if(2 == secure_protocol)
        {
            lst->connType &= ~CT_ADAPTER_IP; //reset IP flag
            lst->connType |= CT_ADAPTER_TCP; //set TCP flag
            lst->endpoint.adapter = OC_ADAPTER_TCP;
            lst->endpoint.port = lst->tcpPort;
            lst->securePort = lst->tcpPort;
        }
        lst = lst->next;
    }
}

static void selectSecureProtocol()
{
    printf("   Select protocol\n");
    printf("   1 - DTLS(Default)\n");
    printf("   2 - TLS\n");

    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%d",&secure_protocol);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
        // '0x20<=code' is character region
    }

    if(0 >= secure_protocol || 2 < secure_protocol)
    {
        secure_protocol = 1;
    }

    setDevProtocol(g_own_list);
    setDevProtocol(g_unown_list);
}
#endif

static void secureStorageHwEmulation()
{
    printf("   Enable Secure Storage HW Emulation\n");

    printf("         Enter Own Certificate File Path[~4095]: ");
    char cert_filepath[4096] = {0,};
    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%255s", cert_filepath);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    printf("         Enter Private Key File Path[~4095]: ");
    char key_filepath[4096] = {0,};
    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%255s", key_filepath);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    printf("         Enter Password for Key Password[~31][Press (Enter) to not set]: ");
    char pwd[32] = {0,};
    for(int i=0; i < 31; i++)
    {
        pwd[i] = (char)getchar();
        if (0x20 <= pwd[i])
        {
            pwd[i--] = '\0';
            continue;
        }
        if (0x0A == pwd[i])
        {
            pwd[i] = '\0';
            break;
        }
    }

    if (0 != SSemulSetCertkeyFilepath(cert_filepath, key_filepath, pwd))
    {
        printf("    Fail to set cert/key file path");
        return;
    }

    if (0 != SetHwPkixCallbacks(HWGetKeyContext,
                                                  HWFreeKeyContext,
                                                  HWGetOwnCertificateChain,
                                                  HWSetupPkContext))
    {
        printf("    Fail to regist HW Pkix Callbacks");
        return;
    }
    printf("    Success to regist HW Pkix Callbacks");
}

static void selectVerifMethod()
{
    int option;
    printf("   Select verification method for ownership transfer\n");
    printf("   0 - No verification\n");
    printf("   1 - Display only\n");
    printf("   2 - Confirm only\n");
    printf("   3 - Both Display and Confirm\n");

    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%d",&option);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
        // '0x20<=code' is character region
    }

    if(0 > option || 3 < option)
    {
        printf("Invalid option!");
    }
    SetVerifyOption((VerifyOptionBitmask_t) option);
    printf("Option %d chosen!", option);
}

static void printMenu(void)
{
    printf("************************************************************\n");
    printf("****** OIC Provisioning Client with using C-level API ******\n");
    printf("************************************************************\n\n");

    printf("** [A] DISCOVER DEVICES ON NETWORK\n");
    printf("** 10. Discover All Un/Owned Devices on Network\n");
    printf("** 11. Discover Only Unowned Devices on Network\n");
#ifdef MULTIPLE_OWNER
    printf("** 12. Discover Only Owned Devices on Network\n");
    printf("** 13. Discover Multiple Ownership Transfer Enabled Devices on Network\n\n");
#else
    printf("** 12. Discover Only Owned Devices on Network\n\n");
#endif //MULTIPLE_OWNER

    printf("** [B] REGISTER/OWN ALL DISCOVERED UNOWNED DEVICES\n");
    printf("** 20. Register/Own All Discovered Unowned Devices\n\n");

    printf("** [C] PROVISION/LINK PAIRWISE THINGS\n");
    printf("** 30. Provision/Link Pairwise Things\n");
    printf("** 31. Provision Credentials for Pairwise Things\n");
    printf("** 32. Provision the Selected Access Control List(ACL)\n");
    printf("** 33. Provision Direct-Pairing Configuration\n");
    printf("** 34. Check Linked Status of the Selected Device on PRVN DB\n");
    printf("** 35. Save the Selected Access Control List(ACL) into local SVR DB\n\n");

    printf("** [D] UNLINK PAIRWISE THINGS\n");
    printf("** 40. Unlink Pairwise Things\n\n");

    printf("** [E] REMOVE THE SELECTED DEVICE\n");
    printf("** 50. Remove the Selected Device\n");
    printf("** 51. Remove Device with UUID (UUID input is required)\n");
    printf("** 52. Reset the Selected Device\n");
    printf("** 53. Reset SVR DB\n\n");

    printf("** [F] GET SECURITY RESOURCE FOR DEBUGGING ONLY\n");
    printf("** 60. Get the Credential resources of the Selected Device\n");
    printf("** 61. Get the ACL resources of the Selected Device\n\n");

#ifdef MULTIPLE_OWNER
    printf("** [G] UPDATE THE MULTIPLE OWNERSHIP TRANSFER RELATED VALUE\n");
    printf("** 70. Change the Multiple Ownership transfer MODE(update mom)\n");
    printf("** 71. Provision Preconfigured PIN\n");
    printf("** 72. Change the Multiple Ownership transfer METHOD(update oxmsel)\n");
    printf("** 73. Remove Sub-Owner from Resource Server\n");
    printf("** 74. Remove All Sub-Owner from Resource Server\n\n");
#endif //MULTIPLE_OWNER

#ifdef __WITH_TLS__
    printf("** [H] SELECT SECURE PROTOCOL DTLS/TLS AND OTHERS\n");
    printf("** 80. Select secure protocol(default DTLS)\n");
    printf("** 81. Select verification method\n");
    printf("** 82. Enable secure storage hw emulation\n\n");
#else
    printf("** [H] SELECT VERIFICATION OPTION\n");
    printf("** 81. Select verification method\n\n");
#endif
    printf("** [I] EXIT PROVISIONING CLIENT\n");

    printf("** 99. Exit Provisionong Client\n\n");

    printf("************************************************************\n\n");
}

#if 0 // Code for enabling path configuration for PDB and SVR DBf
static void printUsage(void)
{
    printf("\n");
    printf("OIC Provisioning Client with using C-level API\n");
    printf("Usage: provisioningclient [option]...\n");
    printf("\n");
    printf("  -h                           print help for this provisioning client\n");
    printf("  -p=[prvn_db_file_path/name]  input PRVN DB file path and name\n");
    printf("                               if not exists, will load default DB file\n");
    printf("                               (default: |oic_prvn_mng.db| on working dir)\n");
    printf("                               (ex. -p=oic_prvn_mng.db)\n");
    printf("  -s=[svr_db_file_path/name]   input SVR DB file path and name\n");
    printf("                               if not exists, will load default DB file\n");
    printf("                               (default: |oic_svr_db_client.json| on working dir)\n");
    printf("                               (ex. -s=oic_svr_db_client.json)\n");
    printf("\n");
}
#endif

/**
 * Sample implementation of Export key block and master secret
 *
 * @param[in] p_expkey  Context for the callback
 * @aram[in] ms        Pointer to master secret (fixed length: 48 bytes)
 * @param[in] kb        Pointer to key block, see RFC 5246 section 6.3
 *                  (variable length: 2 * maclen + 2 * keylen + 2 * ivlen).
 * @param[in] maclen    MAC length
 * @param[in] keylen    Key length
 * @param[in] ivlen     IV length
 */
static void SslExportKeysCallback(const unsigned char* masterSecret,
                                  const unsigned char* keyBlock,
                                  size_t macLen, size_t keyLen, size_t ivLen)
{
    OIC_LOG_V(INFO, TAG, "In %s ", __func__);

    OIC_LOG(INFO, TAG, "[MASTER SECRET] : ");
    OIC_LOG_BUFFER(INFO, TAG, masterSecret, 48);

    OIC_LOG(INFO, TAG, "[KEY BLOCK] : ");
    OIC_LOG_BUFFER(INFO, TAG, keyBlock, (2 * macLen) + (2 * keyLen) + (2 * ivLen));

    OIC_LOG_V(INFO, TAG, "Mac Length = %d, Key Length = %d, IV Length = %d", macLen, keyLen, ivLen);

    OIC_LOG_V(INFO, TAG, "Out %s ", __func__);
}

// main function for provisioning client using C-level provisioning API
int main()
{
    // initialize provisioning client
    if(initProvisionClient())
    {
        OIC_LOG(ERROR, TAG, "ProvisionClient init error");
        goto PMCLT_ERROR;
    }

    if (CA_STATUS_OK !=
        CASetSslExportKeysCallback(SslExportKeysCallback, CA_SSL_EKCB_DTLS, CA_SSL_EKCB_CLIENT))
    {
        OIC_LOG(ERROR, TAG, "Failed to register the (D)TLS export Key Callback!");
        goto PMCLT_ERROR;
    }

    // Client can choose a allowed/not-allowed OxM method.
    if(OC_STACK_OK != OCSetOxmAllowStatus(OIC_DECENTRALIZED_PUBLIC_KEY, false))
    {
        OIC_LOG(WARNING, TAG, "Failed to disable OIC_DECENTRALIZED_PUBLIC_KEY OxM");
    }

    // set callbacks for verification options
    SetDisplayNumCB(NULL, displayNumCB);
    SetUserConfirmCB(NULL, confirmNumCB);

#ifdef MULTIPLE_OWNER
    SetPreconfigPin("12341234", 8);
#endif //MULTIPLE_OWNER

    // main loop for provisioning manager
    int mn_num = 0;
    for( ; ; )
    {
        printf("\n");
        printMenu();
        printf(">> Enter Menu Number: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &mn_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        printf("\n");
        switch(mn_num)
        {
        case _10_DISCOV_ALL_DEVS_:
            if(discoverAllDevices())
            {
                OIC_LOG(ERROR, TAG, "_10_DISCOV_ALL_DEVS_: error");
            }
            break;
        case _11_DISCOV_UNOWN_DEVS_:
            if(discoverUnownedDevices())
            {
                OIC_LOG(ERROR, TAG, "_11_DISCOV_UNOWN_DEVS_: error");
            }
            break;
        case _12_DISCOV_OWN_DEVS_:
            if(discoverOwnedDevices())
            {
                OIC_LOG(ERROR, TAG, "_12_DISCOV_OWN_DEVS_: error");
            }
            break;
#ifdef MULTIPLE_OWNER
        case _13_MOT_DISCOV_DEV_:
            if(discoverMOTEnabledDevices())
            {
                OIC_LOG(ERROR, TAG, "_13_MOT_DISCOV_DEV_: error");
            }
            break;
#endif //MULTIPLE_OWNER
        case _20_REGIST_DEVS_:
            if(registerDevices())
            {
                OIC_LOG(ERROR, TAG, "_20_REGIST_DEVS_: error");
            }
            break;
        case _30_PROVIS_PAIR_DEVS_:
            if(provisionPairwise())
            {
                OIC_LOG(ERROR, TAG, "_30_PROVIS_PAIR_DEVS_: error");
            }
            break;
        case _31_PROVIS_CRED_:
            if(provisionCred())
            {
                OIC_LOG(ERROR, TAG, "_31_PROVIS_CRED_: error");
            }
            break;
        case _32_PROVIS_ACL_:
            if(provisionAcl())
            {
                OIC_LOG(ERROR, TAG, "_32_PROVIS_ACL_: error");
            }
            break;
        case _33_PROVIS_DP_:
            if(provisionDirectPairing())
            {
                OIC_LOG(ERROR, TAG, "_33_PROVIS_DP_: error");
            }
            break;
        case _34_CHECK_LINK_STATUS_:
            if(checkLinkedStatus())
            {
                OIC_LOG(ERROR, TAG, "_34_CHECK_LINK_STATUS_: error");
            }
            break;
        case _35_SAVE_ACL_:
            if(saveAcl())
            {
                OIC_LOG(ERROR, TAG, "_35_SAVE_ACL_: error");
            }
            break;
        case _40_UNLINK_PAIR_DEVS_:
            if(unlinkPairwise())
            {
                OIC_LOG(ERROR, TAG, "_40_UNLINK_PAIR_DEVS_: error");
            }
            break;
        case _50_REMOVE_SELEC_DEV_:
            if(removeDevice())
            {
                OIC_LOG(ERROR, TAG, "_50_REMOVE_SELEC_DEV_: error");
            }
            break;
        case _51_REMOVE_DEV_WITH_UUID_:
            if(removeDeviceWithUuid())
            {
                OIC_LOG(ERROR, TAG, "_51_REMOVE_DEV_WITH_UUID_: error");
            }
            break;
        case _52_RESET_SELEC_DEV_:
            if(resetDevice())
            {
                OIC_LOG(ERROR, TAG, "_52_RESET_SELEC_DEV_: error");
            }
            break;
        case _53_RESET_SVR_DB_:
            if(resetSVRDB())
            {
                OIC_LOG(ERROR, TAG, "_53_RESET_SVR_DB_: error");
            }
            break;
        case _60_GET_CRED_:
            if(getCred())
            {
                OIC_LOG(ERROR, TAG, "_60_GET_CRED_: error");
            }
            break;
        case _61_GET_ACL_:
            if(getAcl())
            {
                OIC_LOG(ERROR, TAG, "_61_GET_ACL_: error");
            }
            break;
#ifdef MULTIPLE_OWNER
        case _70_MOT_CHANGE_MOM_:
            if(changeMultipleOwnershipTrnasferMode())
            {
                OIC_LOG(ERROR, TAG, "_70_MOT_CHANGE_MOM_: error");
            }
            break;
        case _71_MOT_PROV_PRECONF_PIN_:
            if(provisionPreconfigPIN())
            {
                OIC_LOG(ERROR, TAG, "_71_MOT_PROV_PRECONF_PIN_: error");
            }
            break;
        case _72_MOT_OXM_SEL_:
            if(selectMultipleOwnershipTrnasferMethod())
            {
                OIC_LOG(ERROR, TAG, "_72_MOT_OXM_SEL_: error");
            }
            break;
        case _73_MOT_REMOVE_SUBOWNER_:
            if(removeSubOwner())
            {
                OIC_LOG(ERROR, TAG, "_73_MOT_REMOVE_SUBOWNER_ : error");
            }
            break;
        case _74_MOT_REMOVE_ALL_SUBOWNER_:
            if(removeAllSubOwner())
            {
                OIC_LOG(ERROR, TAG, "_74_MOT_REMOVE_ALL_SUBOWNER_ : error");
            }
            break;
#endif //MULTIPLE_OWNER
#ifdef __WITH_TLS__
        case  _80_SELECT_PROTOCOL_:
            selectSecureProtocol();
            break;
#endif
        case _81_SELECT_VERIF_METHOD_:
            selectVerifMethod();
            break;
        case _82_SECURE_STORAGE_HW_EMULATION_:
            secureStorageHwEmulation();
            break;
        case _99_EXIT_PRVN_CLT_:
            goto PMCLT_ERROR;
        default:
            printf(">> Entered Wrong Number. Please Enter Again\n\n");
            break;
        }
    }

PMCLT_ERROR:
    if(OC_STACK_OK != OCStop())
    {
        OIC_LOG(ERROR, TAG, "OCStack stop error");
    }
    OCDeleteDiscoveredDevices(g_own_list);  // after here |g_own_list| points nothing
    OCDeleteDiscoveredDevices(g_unown_list);  // after here |g_unown_list| points nothing
#ifdef MULTIPLE_OWNER
    OCDeleteDiscoveredDevices(g_mot_enable_list);  // after here |g_motdev_list| points nothing
#endif //MULTIPLE_OWNER

    if(g_svr_fname)
    {
        OICFree(g_svr_fname);  // after here |g_svr_fname| points nothing
    }
    if(g_prvn_fname)
    {
        OICFree(g_prvn_fname);  // after here |g_prvn_fname| points nothing
    }
    return 0;  // always return normal case
}

#ifdef __cplusplus
}
#endif //__cplusplus
