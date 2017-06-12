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
 *****************************************************************/
#include "iotivity_config.h"

#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "platform_features.h"
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
#include "pmutility.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

// declaration(s) for provisioning client using C-level provisioning API
// user input definition for main loop on provisioning client
#define _10_DISCOV_MOT_ENABLED_DEV_         10
#define _11_DISCOV_MULTIPLE_OWNED_DEV_         11
#define _20_PERFORM_MOT_        20
#define _30_GET_LED_RESOURCE_  30
#define _31_PUT_LED_RESOURCE_  31
#define _40_PROVISION_ACL_  40
#define _41_PROVISION_CRED_  41
#define _99_EXIT_PRVN_CLT_          99

#define ACL_RESRC_MAX_NUM   16
#define ACL_RESRC_ARRAY_SIZE   3 //This value is used only for sample (not OCF spec)
#define ACL_RESRC_MAX_LEN   128
#define ACL_PEMISN_CNT      5
#define DISCOVERY_TIMEOUT   5  // 5 sec
#define CALLBACK_TIMEOUT    60  // 1 min
#define TAG "subownerclient"

static const char* SVR_DB_FILE_NAME = "oic_svr_db_subowner_client.dat";
        // '_' for separaing from the same constant variable in |srmresourcestrings.c|
static const char* PRVN_DB_FILE_NAME = "oic_pdm_subowner.db";
static const OicSecPrm_t  SUPPORTED_PRMS[1] =
{
    PRM_PRE_CONFIGURED,
};

// |g_ctx| means provision manager application context and
// the following, includes |un/own_list|, could be variables, which |g_ctx| has,
// for accessing all function(s) for these, they are declared on global domain
static const char* g_ctx = "SubOwner Client Application Context";
static char* g_svr_fname;
static char* g_prvn_fname;
static OCProvisionDev_t* g_own_list;
static OCProvisionDev_t* g_unown_list;
static OCProvisionDev_t* g_motdev_list;
static OCProvisionDev_t* g_mowned_list;
static int g_own_cnt;
static int g_unown_cnt;
static int g_motdev_cnt;
static int g_mowned_cnt;
static bool g_doneCB;

// function declaration(s) for calling them before implementing
static OCProvisionDev_t* getDevInst(const OCProvisionDev_t*, const int);
static int printDevList(const OCProvisionDev_t*);
static size_t printUuidList(const OCUuidList_t*);
static int printResultList(const OCProvisionResult_t*, const int);
static void printUuid(const OicUuid_t*);
static FILE* fopen_prvnMng(const char*, const char*);
static int waitCallbackRet(void);
static int selectTwoDiffNum(int*, int*, const int, const char*);

// callback function(s) for provisioning client using C-level provisioning API
static void multipleOwnershipTransferCB(void* ctx, int nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Multiple Ownership Transfer SUCCEEDED - ctx: %s", (char*) ctx);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Multiple Ownership Transfer FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
    }
    g_doneCB = true;
}

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

// response handler for LED requests.
static void LedCB(void *ctx, OCDoHandle UNUSED,
                                                    OCClientResponse *clientResponse)
{
    if(clientResponse)
    {
        if(OC_STACK_OK == clientResponse->result)
        {
            printf("Received OC_STACK_OK from server\n");
            if(clientResponse->payload)
            {
                printf("Response ===================> %s\n", clientResponse->payload);
            }
        }
        else if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            printf("Received OC_STACK_RESOURCE_CHANGED from server\n");
        }
        else
        {
            printf("Error in response : %d\n", clientResponse->result);
        }
    }
    else
    {
        printf("Hit the response callback but can not find response data\n");
    }

    g_doneCB = true;
}

static void inputPinCB(char* pin, size_t len)
{
    if(!pin || OXM_RANDOM_PIN_MAX_SIZE>=len)
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
    static OCPersistentStorage ps = {fopen_prvnMng, fread, fwrite, fclose, unlink};
    if(OC_STACK_OK != OCRegisterPersistentStorageHandler(&ps))
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

static int discoverMotSupportedDevices(void)
{
    // delete un/owned device lists before updating them
    if(g_motdev_list)
    {
        OCDeleteDiscoveredDevices(g_motdev_list);
        g_motdev_list = NULL;
    }

    // call |OCDiscoverMultipleOwnerEnabledDevices| API actually
    printf("   Discovering Multiple Ownership Transfer enabled Devices on Network..\n");
    if(OC_STACK_OK != OCDiscoverMultipleOwnerEnabledDevices(DISCOVERY_TIMEOUT, &g_motdev_list))
    {
        OIC_LOG(ERROR, TAG, "OCDiscoverMultipleOwnerEnalbedDevices API error");
        return -1;
    }

    // display the discovered device lists
    printf("   > Discovered Multiple Ownership Transfer Enabled Devices\n");
    g_motdev_cnt = printDevList(g_motdev_list);

    return 0;
}

static int discoverSubOwnerDevices()
{
    // delete un/owned device lists before updating them
    if(g_mowned_list)
    {
        OCDeleteDiscoveredDevices(g_mowned_list);
        g_mowned_list = NULL;
    }

    // call |OCDiscoverMultipleOwnedDevices| API actually
    printf("   Discovering Multiple Owned Devices on Network..\n");
    if(OC_STACK_OK != OCDiscoverMultipleOwnedDevices(DISCOVERY_TIMEOUT, &g_mowned_list))
    {
        OIC_LOG(ERROR, TAG, "OCDiscoverMultipleOwnerEnabledDevices API error");
        return -1;
    }

    // display the discovered device lists
    printf("   > Discovered Multiple Owned Devices\n");
    g_mowned_cnt = printDevList(g_mowned_list);

    return 0;
}

static int multipleOwnershipTransfer(void)
{
    // check |unown_list| for registering devices
    if(!g_motdev_list || 0 >=g_motdev_cnt)
    {
        printf("   > MultipleOwnershipTransfer Enabled Device List is Empty\n");
        printf("   > Please Discover Devices first, with [10] Menu\n");
        return 0;  // normal case
    }

    // call |getDevInst| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;

#ifdef MULTIPLE_OWNER
    OCProvisionDev_t* dev = NULL;
    LL_FOREACH(g_motdev_list, dev)
    {
        if(OIC_PRECONFIG_PIN == dev->doxm->oxmSel)
        {
            //Pre-Configured PIN initialization
            const char* testPreconfigPin = "12341234";
            if(OC_STACK_OK != OCAddPreconfigPin(dev, testPreconfigPin, strlen(testPreconfigPin)))
            {
                printf("\n\n\n*** %60s ***\n", "WARNNING : Failed to save the pre-configured PIN");
                printf("*** %60s ***\n\n\n", "WARNNING : You can't use the pre-configured PIN OxM for MOT");
                return -1;
            }
        }
    }
#endif //MULTIPLE_OWNER

    if(OC_STACK_OK != OCDoMultipleOwnershipTransfer(g_ctx, g_motdev_list, multipleOwnershipTransferCB))
    {
        OIC_LOG(ERROR, TAG, "_20_PERFORM_MOT_: error");
        return -1;
    }

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return -1;
    }

    // display the registered result
    printf("   > Registered Discovered Devices\n");

    return 0;
}

static int sendGetLed()
{
    int selDevNum;
    char query[256] = {0};
    OCCallbackData cbData;
    cbData.cb = &LedCB;
    cbData.context = NULL;
    cbData.cd = NULL;

    printDevList(g_mowned_list);

    // select device for provisioning access control list
    for( ; ; )
    {
        printf("   > Enter Device Number, for sending GET LED request: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &selDevNum);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<selDevNum && g_mowned_cnt>=selDevNum)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* selDev = getDevInst(g_mowned_list, selDevNum);
    if(NULL == selDev)
    {
        printf("Failed to getDevInst()\n");
        return -1;
    }

    if(PMGenerateQuery(true, selDev->endpoint.addr, selDev->securePort, selDev->connType,
                       query, sizeof(query), "/a/led"))
    {
        g_doneCB = false;
        printf("query=%s\n", query);
        if(OC_STACK_OK != OCDoResource(NULL, OC_REST_GET, query, NULL, NULL, selDev->connType,
                                       OC_HIGH_QOS, &cbData, NULL, 0))
        {
            printf("********************************\n");
            printf("Failed to send GET request to %s\n", query);
            printf("********************************\n");
            g_doneCB = true;
            return -1;
        }

        waitCallbackRet();
    }
    else
    {
        printf("Failed to generate GET request for /a/led\n");
        return -1;
    }

    return 0;
}

static int sendPutLed()
{
    int selDevNum;
    char query[256] = {0};
    OCCallbackData cbData;
    cbData.cb = &LedCB;
    cbData.context = NULL;
    cbData.cd = NULL;

    printDevList(g_mowned_list);
    // select device for provisioning access control list
    for( ; ; )
    {
        printf("   > Enter Device Number, for sending PUT LED request: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &selDevNum);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<selDevNum && g_mowned_cnt>=selDevNum)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* selDev = getDevInst(g_mowned_list, selDevNum);
    if(NULL == selDev)
    {
        printf("Failed to getDevInst()\n");
        return -1;
    }

    if(PMGenerateQuery(true, selDev->endpoint.addr, selDev->securePort, selDev->connType,
                       query, sizeof(query), "/a/led"))
    {
        g_doneCB = false;
        printf("query=%s\n", query);
        if(OC_STACK_OK != OCDoResource(NULL, OC_REST_PUT, query, NULL, NULL, selDev->connType,
                                       OC_LOW_QOS, &cbData, NULL, 0))
        {
            printf("********************************\n");
            printf("Failed to send PUT request to %s\n", query);
            printf("********************************\n");
            g_doneCB = true;
            return -1;
        }

        waitCallbackRet();
    }
    else
    {
        printf("Failed to generate PUT request for /a/led\n");
        return -1;
    }

    return 0;
}


static OicSecAcl_t* createAclForLEDAccess(const OicUuid_t* subject)
{
    if(NULL == subject)
    {
        OIC_LOG(ERROR, TAG, "createAcl: Invalid paramters");
        return NULL;
    }
    // allocate memory for |acl| struct
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
    memcpy(ace->subjectuuid.id, subject->id, sizeof(subject->id));

    // fill the href
    char* rsrc_in = "/a/led";  // '1' for null termination
    OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    if(!rsrc)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        goto CRACL_ERROR;
    }

    size_t len = strlen(rsrc_in)+1;  // '1' for null termination
    rsrc->href = (char*) OICCalloc(len, sizeof(char));
    if(!rsrc->href)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        goto CRACL_ERROR;
    }
    OICStrcpy(rsrc->href, len, rsrc_in);

    //fill the resource type (rt)
    rsrc->typeLen = 1;
    rsrc->types = (char**)OICCalloc(1, sizeof(char*));
    if(!rsrc->types)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        goto CRACL_ERROR;
    }
    rsrc->types[0] = OICStrdup("oic.r.core");
    if(!rsrc->types[0])
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICStrdup error return");
        goto CRACL_ERROR;
    }

    //fill the interface (if)
    rsrc->interfaceLen = 1;
    rsrc->interfaces = (char**)OICCalloc(1, sizeof(char*));
    if(!rsrc->interfaces)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        goto CRACL_ERROR;
    }
    rsrc->interfaces[0] = OICStrdup("oic.if.baseline");
    if(!rsrc->interfaces[0])
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICStrdup error return");
        goto CRACL_ERROR;
    }

    LL_APPEND(ace->resources, rsrc);

    // full permission for /a/led
    ace->permission = PERMISSION_FULL_CONTROL;

    ace->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
    if(NULL == ace->eownerID)
    {
        OIC_LOG(ERROR, TAG, "createAcl: OICCalloc error return");
        goto CRACL_ERROR;
    }

    memcpy(ace->eownerID->id, subject->id, sizeof(subject->id));

    return acl;

CRACL_ERROR:
    OCDeleteACLList(acl);  // after here |acl| points nothing
    return NULL;
}

static int provisionAclForLed()
{
    OicSecAcl_t* acl = NULL;

    // check |own_list| for provisioning access control list
    if(!g_mowned_list || 1> g_mowned_cnt)
    {
        printf("   > MOT Device List is Empty\n");
        printf("   > Please Perform MOT first, with [12|21] Menu\n");
        return 0;  // normal case
    }

    // display the MOT dev list
    printf("   > MOT Devices\n");
    g_mowned_cnt = printDevList(g_mowned_list);

    // select device for provisioning access control list
    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Device Number, for Provisioning LED's ACL: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_mowned_list>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    g_doneCB = false;
    printf("   Provisioning Selected ACL..\n");
    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*) g_mowned_list, dev_num);
    if(!dev)
    {
        OIC_LOG(ERROR, TAG, "provisionAcl: device instance empty");
        goto PVACL_ERROR;
    }

    acl = createAclForLEDAccess(&dev->doxm->subOwners->uuid);
    if(NULL == acl)
    {
        OIC_LOG(ERROR, TAG, "provisionAcl: Failed to create ACL for LED");
        return -1;
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
    // display the ACL-provisioned result
    printf("   > Provisioned Selected ACL\n");

    OCDeleteACLList(acl);  // after here |acl| points nothing
    return 0;

PVACL_ERROR:
    OCDeleteACLList(acl);
    return -1;
}

static int provisionCred()
{
    // check |unown_list| for registering devices
    if(!g_mowned_list|| 0 >=g_mowned_cnt)
    {
        printf("   > Multiple Owned Device List is Empty\n");
        printf("   > Please Discover Devices first, with [13] Menu\n");
        return 0;  // normal case
    }

    // display the MOT dev list
    printf("   > Multiple Owned Devices\n");
    g_mowned_cnt = printDevList(g_mowned_list);

    int dev_num = 0;
    for( ; ; )
    {
        printf("   > Enter Multiple Owned Device Number to link : ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &dev_num);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        if(0<dev_num && g_mowned_cnt>=dev_num)
        {
            break;
        }
        printf("     Entered Wrong Number. Please Enter Again\n");
    }

    OCProvisionDev_t* motDev = getDevInst(g_mowned_list, dev_num);
    if(NULL == motDev)
    {
        OIC_LOG(ERROR, TAG, "Failed to getDevInst()");
        return -1;
    }

    // display the MOT dev list
    printf("   > Owned Devices\n");
    g_own_cnt = printDevList(g_own_list);

    for( ; ; )
    {
        printf("   > Enter Owned Device Number to link : ");
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

    OCProvisionDev_t* ownDev = getDevInst(g_own_list, dev_num);
    if(NULL == ownDev)
    {
        OIC_LOG(ERROR, TAG, "Failed to getDevInst()");
        return -1;
    }

    // call |OCProvisionCredentials| API actually
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    printf("   Provisioning Selected Pairwise Devices..\n");
    OCStackResult rst = OCProvisionCredentials((void*) g_ctx,
                    SYMMETRIC_PAIR_WISE_KEY, OWNER_PSK_LENGTH_128,
                    ownDev, motDev, provisionCredCB);
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

    // display the pairwise-provisioned result
    printf("   > Provisioned Selected Pairwise Devices\n");

    return 0;

PVPWS_ERROR:
    return -1;
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

static void printMenu(void)
{
    printf("************************************************************\n");
    printf("****** OIC Provisioning Client with using C-level API ******\n");
    printf("************************************************************\n\n");

    printf("** [A] DISCOVER DEVICES ON NETWORK\n");
    printf("** 10. Discover Multiple Ownership Transfer Enabled Devices on Network\n");
    printf("** 11. Discover Multiple Owned Devices on Network\n\n");

    printf("** [B] PERFORM MULTIPLE OWNERSHIP TRANSFER\n");
    printf("** 20. Perform the Multiple Ownership Transfer for ALL discovered dievices\n\n");

    printf("** [C] Get/Put Request for APPLICATION RESOURCE\n");
    printf("** 30. Get LED resource\n");
    printf("** 31. Put LED resource\n\n");

    printf("** [D] LINK DEVICES\n");
    printf("** 40. Provision ACL for LED Resource\n");
    printf("** 41. Provison Credential\n\n");

    printf("** [F] EXIT PROVISIONING CLIENT\n");
    printf("** 99. Exit Provisionong Client\n\n");

    printf("************************************************************\n\n");
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

    // main loop for provisioning manager
    int mnNum = 0;
    int selDevNum = 0;
    for( ; ; )
    {
        printf("\n");
        printMenu();
        printf(">> Enter Menu Number: ");
        for(int ret=0; 1!=ret; )
        {
            ret = scanf("%d", &mnNum);
            for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                        // '0x20<=code' is character region
        }
        printf("\n");
        switch(mnNum)
        {
        case _10_DISCOV_MOT_ENABLED_DEV_:
            if(discoverMotSupportedDevices())
            {
                OIC_LOG(ERROR, TAG, "_12_MOT_DISCOV_DEV_: error");
            }
            break;
        case _11_DISCOV_MULTIPLE_OWNED_DEV_:
            if(discoverSubOwnerDevices())
            {
                OIC_LOG(ERROR, TAG, "_13_DISCOV_MULTIPLE_OWNED_DEV_: error");
            }
            break;
        case _20_PERFORM_MOT_:
            if(multipleOwnershipTransfer())
            {
                OIC_LOG(ERROR, TAG, "_21_PERFORM_MOT_: error");
            }
            break;
        case _30_GET_LED_RESOURCE_:
            if(sendGetLed())
            {
                OIC_LOG(ERROR, TAG, "_30_GET_LED_RESOURCE_: error");
            }
            break;
        case _31_PUT_LED_RESOURCE_:
            if(sendPutLed())
            {
                OIC_LOG(ERROR, TAG, "_31_PUT_LED_RESOURCE_: error");
            }
            break;
        case _40_PROVISION_ACL_:
            if(provisionAclForLed())
            {
                OIC_LOG(ERROR, TAG, "_40_PROVISION_ACL_: error");
            }
            break;
        case _41_PROVISION_CRED_:
            OIC_LOG(ERROR, TAG, "NOT SUPPORTED YET.");
            break;
            /*
            if(provisionCred())
            {
                OIC_LOG(ERROR, TAG, "_41_PROVISION_CRED_: error");
            }
            break;
            */
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
    OCDeleteDiscoveredDevices(g_motdev_list);  // after here |g_motdev_list| points nothing

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
