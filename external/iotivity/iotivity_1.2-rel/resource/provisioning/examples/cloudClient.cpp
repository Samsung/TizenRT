//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ocstack.h"
#include "logger.h"
#include "octhread.h"
#include "cathreadpool.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "ocprovisioningmanager.h"

#include "utils.h"
#include "cloudAuth.h"
#include "cloudWrapper.h"
#include "OCApi.h"
#include "OCCloudProvisioning.hpp"

#ifdef __unix__
#include <unistd.h> //for unlink
#endif

#define TAG "cloudClient"
using namespace OC;

#define DEFAULT_HOST            "127.0.0.1"
#define DEFAULT_PORT            OC_MULTICAST_PORT
#define DEFAULT_DEVICEID        "6A757374-776F-726B-4465-765575696430"
#define DEFAULT_USERID          "6A757374-776F-726B-4465-765575696430"
#define DEFAULT_AUTH_PROVIDER   "github"
#define DEFAULT_DB_FILE         "./cloud.dat"
#define DEFAULT_RESPONSE_WAIT_TIME (10 * 1000000) //10s

#define GITHUB_AUTH_LINK        "https://github.com/login?return_to=%2Flogin%2Foauth%2Fauthorize%3Fclient_id%3Dea9c18f540323b0213d0%26redirect_uri%3Dhttp%253A%252F%252Fwww.example.com%252Foauth_callback%252F"

static bool fExit = false;

static OCDevAddr endPoint;
static char token[1024];
static char authProvider[1024];
static char *fname = DEFAULT_DB_FILE;
static uint64_t timeout;
static uint16_t g_credId = 0;

oc_cond cond;
oc_mutex mutex;
std::string ip(DEFAULT_HOST);
OCCloudProvisioning g_cloudProv(ip, (uint16_t)DEFAULT_PORT);

typedef enum {
    SIGN_UP       = 1,
    SIGN_IN       = 2,
    SIGN_OUT      = 3,

    HOST          = 4,
    PORT          = 5,
    DB_FILE       = 6,
    AUTH_PROVIDER = 7,
    USE_RSA = 8,
    SAVE_TRUST_CERT = 9,
    USE_SECURE_CONN = 10,

    CSR_SIGN      = 19,

    CRL_GET       = 20,
    CRL_POST      = 21,

    ACL_ID_CREATE = 30,
    ACL_ID_GET_BY_DEVICE = 31,
    ACL_ID_DELETE = 32,

    ACL_INDIVIDUAL_GET_INFO = 40,
    ACL_INDIVIDUAL_UPDATE_ACE = 41,
    ACL_INDIVIDUAL_DELETE = 42,

    ACL_GROUP_CREATE = 50,
    ACL_GROUP_FIND   = 51,
    ACL_GROUP_JOIN   = 52,
    ACL_GROUP_OBSERVE= 53,
    ACL_GROUP_DELETE = 54,

    ACL_GROUP_SHARE_DEVICE  = 60,
    ACL_GROUP_DELETE_DEVICE  = 61,
    ACL_GROUP_GET_INFO  = 62,

    ACL_POLICY_CHECK_REQUEST = 70,

    ACL_GROUP_INVITE_USER = 80,
    ACL_GROUP_GET_INVITE  = 81,
    ACL_GROUP_DELETE_INVITE  = 82,
    ACL_GROUP_CANCEL_INVITE  = 83,

    EXIT          = 99
}CloudRequest;

void printMenu()
{
    printf("************************************************************\n");
    printf("****************** Cloud Client Requests *******************\n");
    printf("************************************************************\n");
    printf("** AUTHORIZATION\n");
    printf("** %d - Sign Up request\n", SIGN_UP);
    printf("** %d - Sign In request\n", SIGN_IN);
    printf("** %d - Sign Out request\n", SIGN_OUT);

    printf("** SETTINGS \n");
    printf("** %d - Change default host\n", HOST);
    printf("** %d - Change default port\n", PORT);
    printf("** %d - Change default database filename\n", DB_FILE);
    printf("** %d - Change default auth provider\n", AUTH_PROVIDER);
    printf("** %d - Change TLS cipher suite to RSA\n", USE_RSA);
    printf("** %d - Save Trust Cert. Chain into Cred of SVR\n", SAVE_TRUST_CERT);
    printf("** %d - Change Protocol type (CoAP/CoAPs)\n", USE_SECURE_CONN);

    printf("** CERTIFICATE REQUEST\n");
    printf("** %d - Certificate Request\n", CSR_SIGN);

    printf("** CRL\n");
    printf("** %d - CRL GET Request\n", CRL_GET);
    printf("** %d - CRL POST Request\n", CRL_POST);

    printf("** ACL MANAGER\n");
    printf("** %d - ACL id create Request\n", ACL_ID_CREATE);
    printf("** %d - ACL id retrieve by device Request\n", ACL_ID_GET_BY_DEVICE);
    printf("** %d - ACL id delete Request\n", ACL_ID_DELETE);

    printf("** ACL INDIVIDUAL\n");
    printf("** %d - ACL individual get info Request\n", ACL_INDIVIDUAL_GET_INFO);
    printf("** %d - ACL individual update ACE Request\n", ACL_INDIVIDUAL_UPDATE_ACE);
    printf("** %d - ACL individual delete Request\n", ACL_INDIVIDUAL_DELETE);

    printf("** ACL GROUP MANAGER\n");
    printf("** %d - ACL Create Group Request\n", ACL_GROUP_CREATE);
    printf("** %d - ACL Find Group Request\n", ACL_GROUP_FIND);
    printf("** %d - ACL Join to invited Group Request\n", ACL_GROUP_JOIN);
    printf("** %d - ACL Observe Group Request\n", ACL_GROUP_OBSERVE);
    printf("** %d - ACL Delete Group Request\n", ACL_GROUP_DELETE);

    printf("** ACL INDIVIDUAL GROUP\n");
    printf("** %d - ACL Share device into Group Request\n", ACL_GROUP_SHARE_DEVICE);
    printf("** %d - ACL Delete device from Group Request\n", ACL_GROUP_DELETE_DEVICE);
    printf("** %d - ACL Get Group Info Request\n", ACL_GROUP_GET_INFO);

    printf("** ACL POLICY ENFORCEMENT\n");
    printf("** %d - ACL Check Permissions Request\n", ACL_POLICY_CHECK_REQUEST);

    printf("** ACL MEMBER INVITATION\n");
    printf("** %d - ACL Invite user to group Request\n", ACL_GROUP_INVITE_USER);
    printf("** %d - ACL Retrieve invitation Request\n", ACL_GROUP_GET_INVITE);
    printf("** %d - ACL Delete invitation Request\n", ACL_GROUP_DELETE_INVITE);
    printf("** %d - ACL Cancel invitation Request\n", ACL_GROUP_CANCEL_INVITE);

    printf("** EXIT\n");
    printf("** %d - Exit cloud client\n\n", EXIT);
    printf("************************************************************\n");

    printf(">> Enter Menu number:\n");
}

/**
 * This is default callback to all requests
 * Used to sync with main menu
 *
 * @param[in] ctx          context
 * @param[in] result       result
 * @param[in] data         data
 */
void handleCB(void* ctx, OCStackResult result, void* data)
{
    OC_UNUSED(ctx);
    OC_UNUSED(data);

    printf("Cloud request Result is == %d", result);
    oc_mutex_lock(mutex);
    oc_cond_signal(cond);
    oc_mutex_unlock(mutex);
}

void handleCB1(OCStackResult result, void *data)
{
    OC_UNUSED(data);

    printf("Cloud request Result is == %d", result);
    oc_mutex_lock(mutex);
    oc_cond_signal(cond);
    oc_mutex_unlock(mutex);
}

void handleCB2(OCStackResult result, std::string data)
{
    printf("Cloud request Result is == %d", result);
    printf("ACL ID for the device is == %s", data.c_str());

    oc_mutex_lock(mutex);
    oc_cond_signal(cond);
    oc_mutex_unlock(mutex);
}

static int saveTrustCert(void)
{
    OCStackResult res = OC_STACK_ERROR;
    OIC_LOG(INFO, TAG, "Save Trust Cert. Chain into Cred of SVR");

    ByteArray trustCertChainArray = {0, 0};

    FILE *fp = fopen("rootca.crt", "rb+");

    if (fp)
    {
        size_t fsize;
        if (fseeko(fp, 0, SEEK_END) == 0 && (fsize = ftello(fp)) > 0)
        {
            trustCertChainArray.data = (uint8_t*)OICCalloc(1, fsize);
            trustCertChainArray.len = fsize;
            if (NULL == trustCertChainArray.data)
            {
                OIC_LOG(ERROR,TAG,"Failed to allocate memory");
                fclose(fp);
                return res;
            }
            rewind(fp);
            if (fsize != fread(trustCertChainArray.data, 1, fsize, fp))
            {
                OIC_LOG(ERROR, TAG, "Certiface not read completely");
            }
            fclose(fp);
        }
    }
    OIC_LOG_BUFFER(DEBUG, TAG, trustCertChainArray.data, trustCertChainArray.len);

    res = OCSaveTrustCertChain(trustCertChainArray.data, trustCertChainArray.len, OIC_ENCODING_PEM,&g_credId);

    if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "OCSaveTrustCertChainBin API error");
        return res;
    }
    OIC_LOG_V(INFO, TAG, "CredId of Saved Trust Cert. Chain into Cred of SVR : %d.\n", g_credId);

    return res;
}

static void userRequests(void *data)
{
    (void) data;
    //defaults
    memset(token, 0, sizeof(token));
    memset(authProvider, 0, sizeof(authProvider));
    strncpy(authProvider, DEFAULT_AUTH_PROVIDER, sizeof(authProvider));
    memset(&endPoint, 0, sizeof(endPoint));
    strncpy(endPoint.addr, DEFAULT_HOST, sizeof(endPoint.addr));
    endPoint.port = DEFAULT_PORT;

    mutex = oc_mutex_new();
    cond = oc_cond_new();

    while (false == fExit)
    {
        OCStackResult res = OC_STACK_ERROR;
        timeout = DEFAULT_RESPONSE_WAIT_TIME;
        //startup report
        printf("-----------------------------------------------------------\n");
        printf("Connecting to: %s:%d\n", endPoint.addr, endPoint.port);
        printf("via auth provider: %s\n", authProvider);
        printf("srv file: %s\n", fname);
        printf("CoAP prefix: %s\n", DEFAULT_PREFIX);
        printf("-----------------------------------------------------------\n");

        printMenu();

        int request = 0;
        scanf("%d", &request);
        for( ; 0x20<=getchar(); );

        switch (request)
        {
        case SIGN_UP:
            if (0 == strncmp(authProvider, DEFAULT_AUTH_PROVIDER, sizeof(authProvider)))
            {
                printf("Paste to browser %s and get auth code\n", GITHUB_AUTH_LINK);
            }
            readString(token, sizeof(token), "auth token", "check link above");
            res = CloudSignUp(&endPoint, authProvider, token, handleCloudSignUpResponse);
            break;
        case SIGN_IN:
            res = CloudSignIn(&endPoint, handleCloudSignInResponse);
            break;
        case SIGN_OUT:
            res = CloudSignOut(&endPoint, handleCloudSignOutResponse);
            break;
        case HOST:
            {
                readString(endPoint.addr, sizeof(endPoint.addr), "host ip address", DEFAULT_HOST);
                std::string str(endPoint.addr);
                g_cloudProv.setIpAddr(str);
                break;
            }
        case PORT:
        {
            char example[8];
            snprintf(example, sizeof(example), "%d", DEFAULT_PORT);
            int tmp = 0;
            readInteger(&tmp, "port number", example);
            endPoint.port = tmp;
            g_cloudProv.setPort((uint16_t)tmp);
        }
        break;
        case CRL_GET:
            res = OCWrapperGetCRL( g_cloudProv, handleCB1);
            break;
        case CRL_POST:
            res = OCWrapperPostCRL( g_cloudProv, handleCB1);
            break;
        case ACL_GROUP_CREATE:
            res = OCWrapperAclCreateGroup(&endPoint, handleCB);
            break;
        case ACL_GROUP_FIND:
            res = OCWrapperAclFindMyGroup(&endPoint, handleCB);
            break;
        case ACL_GROUP_DELETE:
            res = OCWrapperAclDeleteGroup(&endPoint, handleCB);
            break;
        case ACL_GROUP_JOIN:
            res = OCWrapperAclJoinToInvitedGroup(&endPoint, handleCB);
            break;
        case ACL_GROUP_OBSERVE:
            res = OCWrapperAclObserveGroup(&endPoint, handleCB);
            break;
        case ACL_GROUP_SHARE_DEVICE:
            res = OCWrapperAclShareDeviceIntoGroup(&endPoint, handleCB);
            break;
        case ACL_GROUP_DELETE_DEVICE:
            res = OCWrapperAclDeleteDeviceFromGroup(&endPoint, handleCB);
            break;
        case ACL_GROUP_GET_INFO:
            res = OCWrapperAclGroupGetInfo(&endPoint, handleCB);
            break;
        case ACL_GROUP_INVITE_USER:
            res = OCWrapperAclInviteUser(&endPoint, handleCB);
            break;
        case ACL_GROUP_GET_INVITE:
            res = OCWrapperAclGetInvitation(&endPoint, handleCB);
            break;
        case ACL_GROUP_DELETE_INVITE:
            res = OCWrapperAclDeleteInvitation(&endPoint, handleCB);
            break;
        case ACL_GROUP_CANCEL_INVITE:
            res = OCWrapperAclCancelInvitation(&endPoint, handleCB);
            break;
        case ACL_POLICY_CHECK_REQUEST:
            res = OCWrapperAclPolicyCheck(&endPoint, handleCB);
            break;
        case ACL_ID_GET_BY_DEVICE:
            res = OCWrapperAclIdGetByDevice( g_cloudProv, handleCB2);
            break;
        case ACL_ID_CREATE:
            res = OCWrapperAclIdCreate(&endPoint, handleCB);
            break;
        case ACL_ID_DELETE:
            res = OCWrapperAclIdDelete(&endPoint, handleCB);
            break;
        case ACL_INDIVIDUAL_GET_INFO:
            res = OCWrapperAclIndividualGetInfo( g_cloudProv, handleCB1);
            break;
        case ACL_INDIVIDUAL_UPDATE_ACE:
            res = OCWrapperAclIndividualUpdateAce(&endPoint, handleCB);
            break;
        case ACL_INDIVIDUAL_DELETE:
            res = OCWrapperAclIndividualDelete(&endPoint, handleCB);
            break;
        case CSR_SIGN:
            res = OCWrapperCertificateIssueRequest( g_cloudProv, handleCB1);
            break;
        case USE_RSA:
            res = CAManager::setCipherSuite(0x35, OC_ADAPTER_TCP);
            break;
        case SAVE_TRUST_CERT:
            saveTrustCert();
            break;
        case USE_SECURE_CONN:
        {
            int tmp = 0;
            readInteger(&tmp, "CoAP protocol type", "0 - non-secure, 1 - secure");
            setCoapPrefix(0 == tmp ? false : true);
        }
            break;
        case EXIT:
            oc_mutex_free(mutex);
            oc_cond_free(cond);
            fExit = true;
            break;
        default:
            printf(">> Entered Wrong Menu Number. Please Enter Again\n\n");
            break;
        }

        //if requests were sent then wait response
        if (res == OC_STACK_OK)
        {
            oc_mutex_lock(mutex);
            oc_cond_wait_for(cond, mutex, timeout);
            oc_mutex_unlock(mutex);
        }
    }


}

FILE* server_fopen(const char *path, const char *mode)
{
    OC_UNUSED(path);
    return fopen(fname, mode);
}

/**
 * Check file accessibility
 *
 * @param[in] name        file path
 * @return true           if check was successful
 */
static bool checkConfig(const char *name)
{
    FILE* file = fopen(name, "rb");

    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

static void printUsage(char *name)
{
    printf("Wrong arguments count!\n");
    printf("Usage : %s <database_filename>\n", name);
    printf("Examples : 1) %s 2) %s cloud.dat\n", name, name);
}

//==============================================================
int main(int argc, char *argv[])
{
    OC_UNUSED(argc);
    OC_UNUSED(argv);

    if (2 == argc)
    {
        fname = argv[1];

        if (!checkConfig(fname))
        {
            OIC_LOG_V(ERROR, TAG, "Can't open database file %s, exit!", fname);
            return -1;
        }
    }
    else if (argc > 2)
    {
        printUsage(argv[0]);
    }

    //Initialize Persistent Storage for SVR database
    OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};

    OCRegisterPersistentStorageHandler(&ps);

    OCMode stackMode = OC_CLIENT_SERVER;
    if (OCInit(NULL, 0, stackMode) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack init error, exit\n");
        return 1;
    }

    ca_thread_pool_t g_threadPoolHandle = NULL;
    CAResult_t res = ca_thread_pool_init(1, &g_threadPoolHandle);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "thread pool initialize error.");
        return res;
    }
#ifndef __TIZENRT__
    res = ca_thread_pool_add_task(g_threadPoolHandle, userRequests, NULL);
#else
	res = ca_thread_pool_add_task(g_threadPoolHandle, userRequests, NULL, "IoT_userRequest", 4092);
#endif
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "thread pool add task error.");
        goto error;
    }

    while(false == fExit)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG,"OCProcess process error, exit\n");
            return 2;
        }
    }

    if (OCStop() != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStop process error\n");
    }

    error:
    if (g_threadPoolHandle)
    {
        ca_thread_pool_free(g_threadPoolHandle);
    }

    return 0;
}
