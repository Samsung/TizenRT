/******************************************************************
 *
 * Copyright 2017 Microsoft. All Rights Reserved.
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

/*
 * This file is derived from iotivity\resource\csdk\security\provisioning\sample\provisioningclient.c
 * That program is interactive, and requires user input to perform provisioning
 * steps, while this version takes input on the command line and executes a
 * sequence of provisioning operations automatically, then exists, reporting
 * whether the sequence succeeded.  This is so that we can script provisioning
 * for testing.
 */

#include "iotivity_config.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "payload_logging.h"
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
#include "occertutility.h"
#include "ocsecurity.h"
#include "ocstackinternal.h"
#include "pmutility.h"

#ifdef _MSC_VER
#include <io.h>

#define F_OK 0
#define access _access_s
#endif

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#define DISCOVERY_TIMEOUT   3  // 3 sec
#define CALLBACK_TIMEOUT    60  // 1 min
#define TAG "autoprovisioningclient"

static const char* SVR_DB_FILE_NAME = "oic_svr_db_client.dat";
        // '_' for separaing from the same constant variable in |srmresourcestrings.c|
static const char* PRVN_DB_FILE_NAME = "oic_autoprvn_mng.db";

static const char* TEST_CERT_NOT_BEFORE = "20170101000000"; // Not before field for certificates, in format YYYYMMDDhhmmss
static const char* TEST_CERT_NOT_AFTER = "20270101000000";  // + ten years
static const char* TEST_CERT_ROLE1 = "IoTivity-test-role1";
static const char* TEST_CERT_ROLE2 = "IoTivity-test-role2";
static const char* TEST_CERT_AUTHORITY = "IoTivity-test-OBT-authority-name";

// |g_ctx| means provision manager application context and
// the following, includes |un/own_list|, could be variables, which |g_ctx| has,
// for accessing all function(s) for these, they are declared on global domain
static char* g_ctx = "Provision Manager Client Application Context";
static char* g_svr_fname;
static char* g_prvn_fname;
static OCProvisionDev_t* g_own_list;
static OCProvisionDev_t* g_unown_list;
static int g_own_cnt;
static int g_unown_cnt;
static char* g_csr;    /* Certificate signing request from device */
static OicUuid_t g_uuidDev1;
static char* g_idPublicKey = NULL;

static volatile bool g_doneCB;    /* Set to true by the callback to indicate it completed. */
static bool g_successCB; /* Set to true by the callback to indicate success. */

// function declaration(s) for calling them before implementing
static OCProvisionDev_t* getDevInst(const OCProvisionDev_t*, const int);
static int printDevList(const OCProvisionDev_t*);
static size_t printResultList(const OCProvisionResult_t*, const size_t);
static void printUuid(const OicUuid_t*);
static int saveUuid(const OCProvisionResult_t* rslt_lst, const size_t rslt_cnt);
static FILE* fopen_prvnMng(const char*, const char*);
static int waitCallbackRet(void);

/*
 * Test CA key and certificate created with
 * iotivity/resource/csdk/security/scripts/test_cert_generation.sh
 */
static const char* g_caCertPem = "-----BEGIN CERTIFICATE-----\n"
"MIIBfjCCASSgAwIBAgIJAPQXoGTceaW5MAoGCCqGSM49BAMCMBkxFzAVBgNVBAoM\n"
"DklvVGl2aXR5VGVzdENBMB4XDTE3MDMxNTAwNTExOVoXDTMwMTEyMjAwNTExOVow\n"
"GTEXMBUGA1UECgwOSW9UaXZpdHlUZXN0Q0EwWTATBgcqhkjOPQIBBggqhkjOPQMB\n"
"BwNCAARvYPdt+LjqASlHoc2zrjo3hHGjZsI31c+bg9AwINW5TuRZsE03w/Ejotza\n"
"y4VDLImMlDhGP+K/f6OmKD3FNHhKo1UwUzAhBgNVHSUEGjAYBgorBgEEAYLefAEG\n"
"BgorBgEEAYLefAEHMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFNw+hm69Rxb5\n"
"UpclERf5r85g1nwmMAoGCCqGSM49BAMCA0gAMEUCIQDbvNLA3ZkwEzuoH6XUR6JS\n"
"UzZTVgsDgnJcOqtqOg0qEAIgUJR2g8XlMxqiuXP7JdwALdtnvCQTlJQbuD1gu+Jy\n"
"AdQ=\n"
"-----END CERTIFICATE-----\n";

static const char* g_caKeyPem = "-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEILx9VOHDrMYuan6SXN4CQIHHXNq6SjzanaDFDgIaOaXloAoGCCqGSM49\n"
"AwEHoUQDQgAEb2D3bfi46gEpR6HNs646N4Rxo2bCN9XPm4PQMCDVuU7kWbBNN8Px\n"
"I6Lc2suFQyyJjJQ4Rj/iv3+jpig9xTR4Sg==\n"
"-----END EC PRIVATE KEY-----\n";


/* At a few places in this file, warning 4028 is incorrectly produced, disable it for the whole file. */
#ifdef _MSC_VER
#pragma warning( disable : 4028)
#endif

// callback function(s) for provisioning client using C-level provisioning API
static void ownershipTransferCB(void* ctx, size_t nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    g_successCB = false;
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Ownership Transfer SUCCEEDED - ctx: %s", (char*) ctx);
        if (saveUuid((const OCProvisionResult_t*)arr, nOfRes) == 0)
        {
            g_successCB = true;
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Ownership Transfer FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
        g_successCB = false;
    }
    g_doneCB = true;
}

static void provisionCredCB(void* ctx, size_t nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision Credential SUCCEEDED - ctx: %s", (char*) ctx);
        g_successCB = true;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision Credential FAILED - ctx: %s", (char*) ctx);
        printResultList((const OCProvisionResult_t*) arr, nOfRes);
        g_successCB = false;
    }
    g_doneCB = true;
}

/* Function of type OCProvisionResultCB from \resource\csdk\security\provisioning\include\pmtypes.h */
void provisionTrustChainCB(void* ctx, int nOfRes, OCProvisionResult_t *arr, bool hasError)
{
    if(!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision Credential SUCCEEDED - ctx: %s", (char*)ctx);
        g_successCB = true;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision Credential FAILED - ctx: %s", (char*)ctx);
        printResultList((const OCProvisionResult_t*)arr, nOfRes);
        g_successCB = false;
    }
    g_doneCB = true;
}

static void getCsrForCertProvCB(void* ctx, size_t nOfRes, OCPMGetCsrResult_t* arr, bool hasError)
{
    g_successCB = false;

    if (!hasError)
    {
        if (nOfRes != 1)
        {
            OIC_LOG_V(ERROR, TAG, "getCsrForCertProvCB FAILED - ctx: %s", (char*)ctx);
            goto exit;
        }

        if (arr[0].encoding == OIC_ENCODING_DER)
        {
            OCStackResult res = OCConvertDerCSRToPem((char*)arr[0].csr, arr[0].csrLen, &g_csr);
            if (res != OC_STACK_OK)
            {
                OIC_LOG_V(ERROR, TAG, "getCsrForCertProvCB FAILED (CSR re-encoding failed) - error: %d, ctx: %s", res, (char*)ctx);
                goto exit;
            }
            g_successCB = true;
        }
        else if (arr[0].encoding == OIC_ENCODING_PEM)
        {
            g_csr = (char*)OICCalloc(1, arr[0].csrLen);
            if (g_csr == NULL)
            {
                OIC_LOG_V(ERROR, TAG, "getCsrForCertProvCB FAILED (memory allocation) - ctx: %s", (char*)ctx);
                goto exit;
            }

            memcpy(g_csr, arr[0].csr, arr[0].csrLen);

            OIC_LOG(INFO, TAG, "getCsrForCertProvCB success");
            g_successCB = true;
        }
        else
        {
            OIC_LOG_V(ERROR, TAG, "getCsrForCertProvCB FAILED (unknown encoding) - ctx: %s", (char*)ctx);
            goto exit;
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "getCsrForCertProvCB FAILED - ctx: %s", (char*)ctx);
    }

exit:
    g_doneCB = true;
}

OCStackApplicationResult getReqCB(void* ctx, OCDoHandle handle,
    OCClientResponse* clientResponse)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    g_doneCB = true;
    g_successCB = false;

    if (clientResponse == NULL)
    {
        OIC_LOG(ERROR, TAG, "getReqCB received NULL clientResponse");
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (clientResponse->result != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "getReqCB response %d", clientResponse->result);
        return OC_STACK_DELETE_TRANSACTION;
    }

    g_successCB = true;

    return OC_STACK_DELETE_TRANSACTION;
}

static void provisionAclCB(void* ctx, size_t nOfRes, OCProvisionResult_t* arr, bool hasError)
{
    if (!hasError)
    {
        OIC_LOG_V(INFO, TAG, "Provision ACL SUCCEEDED - ctx: %s", (char*)ctx);
        g_successCB = true;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Provision ACL FAILED - ctx: %s", (char*)ctx);
        printResultList((const OCProvisionResult_t*)arr, nOfRes);
        g_successCB = false;
    }
    g_doneCB = true;
}

static void assertRolesCB(void* ctx, bool hasError)
{
    OC_UNUSED(ctx); // unused in release builds

    if (!hasError)
    {
        OIC_LOG_V(INFO, TAG, "%s: Asserting roles SUCCEEDED - ctx: %s", __func__, (char*)ctx);
        g_successCB = true;
    }
    else
    {
        OIC_LOG_V(DEBUG, TAG, "%s: Asserting roles FAILED - ctx: %s", __func__, (char*)ctx);
        g_successCB = false;
    }
    g_doneCB = true;
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
        .unlink = remove
    };
    if (OC_STACK_OK != OCRegisterPersistentStorageHandler(&pstStr))
    {
        OIC_LOG(ERROR, TAG, "OCRegisterPersistentStorageHandler error");
        return -1;
    }

    // initialize OC stack and provisioning manager
    if (OC_STACK_OK != OCInit(NULL, 0, OC_CLIENT_SERVER))
    {
        OIC_LOG(ERROR, TAG, "OCStack init error");
        return -1;
    }

    if (access(PRVN_DB_FILE_NAME, F_OK) == 0)
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

    if (OC_STACK_OK != OCInitPM(PRVN_DB_FILE_NAME))
    {
        OIC_LOG(ERROR, TAG, "OC_PM init error");
        return -1;
    }

    return 0;
}

/* In some tests we need to close existing sessions after making a change
 * in order for the effect of the change to take effect. This function shuts
 * down the OC stack and restarts it.
 */
static int closeAllSessions()
{
    if (OC_STACK_OK != OCStop())
    {
        OIC_LOG_V(ERROR, TAG, "%s: OCStack stop error", __func__);
        return -1;
    }

    if (OC_STACK_OK != OCInit(NULL, 0, OC_CLIENT_SERVER))
    {
        OIC_LOG_V(ERROR, TAG, "%s: OCStack init error", __func__);
        return -1;
    }

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

    // call |OCGetDevInfoFromNetwork| API
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

    return 0;
}

static int registerDevices(void)
{
    // check |unown_list| for registering devices
    if(!g_unown_list || 0>=g_unown_cnt)
    {
        OIC_LOG(ERROR, TAG, "Unowned device list empty, must discover unowned devices first");
        return -1;  // Error, we should have registered unowned devices already
    }

    // call |OCDoOwnershipTransfer| API
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
        OIC_LOG(ERROR, TAG, "OCDoOwnershipTransfer callback error");
        return -1;
    }

    if(!g_successCB)
    {
        OIC_LOG(ERROR, TAG, "OCDoOwnershipTransfer callback failed");
        return -1;
    }

    // display the registered result
    printf("   > Registered Discovered Unowned Devices\n");

    return 0;
}

static int provisionTrustAnchor(int dev_num)
{
    OCProvisionDev_t* targetDevice = getDevInst((const OCProvisionDev_t*)g_own_list, dev_num);
    if(targetDevice == NULL)
    {
        OIC_LOG(ERROR, TAG, "Error, invalid device %d");
        return -1;
    }

    // Install the CA trust anchor locally
    uint16_t caCredId = 0;
    OCStackResult rst = OCSaveTrustCertChain((const uint8_t*)g_caCertPem, strlen(g_caCertPem) + 1,
        OIC_ENCODING_PEM, &caCredId);
    if (OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCSaveTrustCertChain error: %d", rst);
        return -1;
    }

    // Provision the CA root cert to the target device
    printf("   > Saving root certificate (trust anchor) to selected device..\n");
    g_doneCB = false;
    OicSecCredType_t type = SIGNED_ASYMMETRIC_KEY;

    rst = OCProvisionTrustCertChain((void*)g_ctx, type, caCredId, targetDevice, (OCProvisionResultCB)&provisionTrustChainCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionTrustCertChain returned error: %d", rst);
        return -1;
    }

    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionTrustCertChain callback error");
        return -1;
    }
    if(!g_successCB)
    {
        return -1;
    }

    printf("   > Provisioned certificate trust anchor\n");

    return 0;

}

/* If csr is not null, a copy will be made, and the caller must free it with OICFree. */
static int getCsr(int dev_num, char** csr)
{
    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*) g_own_list, dev_num);
    if(!dev)
    {
        OIC_LOG(ERROR, TAG, "getDevInst: device instance empty");
        goto GETCSR_ERROR;
    }

    g_doneCB = false;
    OCStackResult rst = OCGetCSRResource((void*) g_ctx, dev, getCsrForCertProvCB);
    if(OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCGetCSRResource API error: %d", rst);

        goto GETCSR_ERROR;
    }
    if(waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCGetCSRResource callback error");
        goto GETCSR_ERROR;
    }
    if(!g_successCB)
    {
        return -1;
    }

    rst = OCVerifyCSRSignature(g_csr);
    if(rst != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Failed to validate CSR signature");
        OICFreeAndSetToNull((void**)&g_csr);
        return -1;
    }

    if (csr != NULL)
    {
        *csr = OICStrdup(g_csr);
        if (*csr == NULL)
        {
            OIC_LOG(ERROR, TAG, "OICStrdup failed");
            OICFreeAndSetToNull((void**)&g_csr);
            return -1;
        }
    }

    OICFreeAndSetToNull((void**)&g_csr);

    printf("   > Get CSR SUCCEEDED\n");

    return 0;

GETCSR_ERROR:
    printf("   > Get CSR FAILED\n");
    return -1;
}

static int doGetRequest(const char* uri, int dev_num)
{
    OCStackResult res;
    OCCallbackData cbData;
    OCDoHandle handle;
    OCProvisionDev_t *device = NULL;

    device = getDevInst(g_own_list, dev_num);
    if (!device)
    {
        OIC_LOG(ERROR, TAG, "Selected device does not exist");
        return -1;
    }
    cbData.context = NULL;
    cbData.cd = NULL;
    cbData.cb = &getReqCB;
    OIC_LOG_V(INFO, TAG, "Performing GET on %s", uri);
    g_doneCB = false;

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = { 0 };
    if (!PMGenerateQuery(true,
        device->endpoint.addr,
        device->securePort,
        device->connType,
        query, sizeof(query), uri))
    {
        OIC_LOG_V(ERROR, TAG, "%s : Failed to generate query", __func__);
        return -1;
    }

    res = OCDoRequest(&handle, OC_REST_GET, query, NULL, NULL,
        device->connType, OC_HIGH_QOS, &cbData, NULL, 0);

    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoRequest returned error %d with method", res);
    }
    if (waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG_V(ERROR, TAG, "%s callback error", __func__);
    }
    if (g_successCB)
    {
        return 0;
    }

    return -1;
}

static int provisionCert(int dev_num, char* cert)
{
    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*)g_own_list, dev_num);
    if (!dev)
    {
        OIC_LOG(ERROR, TAG, "getDevInst: device instance empty");
        return -1;
    }

    printf("   > Provisioning certificate credential to selected device..\n");
    g_doneCB = false;
    OCStackResult rst = OCProvisionCertificate((void *)g_ctx, dev, cert, provisionCredCB);
    if (OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionCertificate returned error: %d", rst);
        return -1;
    }
    if (waitCallbackRet())
    {
        OIC_LOG_V(ERROR, TAG, "%s failed waiting for callback", __func__);
        return -1;
    }
    if (!g_successCB)
    {
        OIC_LOG_V(ERROR, TAG, "%s callback completed, but failed", __func__);
        return -1;
    }

    printf("   > Provision cert SUCCEEDED\n");

    return 0;
}

/*
 * Create a role or identity certificate for a device, based on the information in its CSR.
 * Assumes the CSR has already been validated wtih OCVerifyCSRSignature.
 * If role is not NULL, a role certificate is created, otherwise an identity certificate
 * is created.
 */
static int createCertFromCSR(const char* caKeyPem, const char* caCertPem, char* csr,
    const char* role, const char* authority, char** deviceCert)
{
    char* publicKey = NULL;
    char* serial = NULL;
    size_t serialLen;
    OicUuid_t uuid = { 0 };
    OCStackResult res = OC_STACK_ERROR;

    res = OCGetUuidFromCSR(csr, &uuid);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get UUID from CSR, error: %d", res);
        goto exit;
    }
    /* Note: a real OBT must make sure the UUID isn't already in use on the network. */

    res = OCGetPublicKeyFromCSR(csr, &publicKey);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get public key from CSR, error: %d", res);
        goto exit;
    }

    res = OCGenerateRandomSerialNumber(&serial, &serialLen);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCGenerateRandomSerialNumber failed, error: %d", res);
        goto exit;
    }

    size_t deviceCertLen;
    if (role != NULL)
    {
        res = OCGenerateRoleCertificate(
            &uuid,
            publicKey,
            caCertPem,
            caKeyPem,
            serial,
            TEST_CERT_NOT_BEFORE,
            TEST_CERT_NOT_AFTER,
            role,
            authority,
            deviceCert,
            &deviceCertLen);
    }
    else
    {
        res = OCGenerateIdentityCertificate(
            &uuid,
            publicKey,
            caCertPem,
            caKeyPem,
            serial,
            TEST_CERT_NOT_BEFORE,
            TEST_CERT_NOT_AFTER,
            deviceCert,
            &deviceCertLen);
    }
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed generating certificate, error: %d", res);
        goto exit;
    }

exit:
    OICFree(publicKey);
    OICFree(serial);

    return (res == OC_STACK_OK) ? 0 : -1;
}

static int setupOwnCert(OicUuid_t* inputUuid)
{
    OCUUIdentity deviceId = { 0 };
    uint16_t caCredId;
    char* serial = NULL;
    size_t serialLen = 0;
    size_t idPublicKeyLen = 0;
    char* idKey = NULL;
    size_t idKeyLen = 0;
    char* idCert = NULL;
    size_t idCertLen = 0;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    /* Set our own trust anchor so that we trust certs we've issued. */
    OCStackResult res = OCSaveTrustCertChain((const uint8_t*)g_caCertPem, strlen(g_caCertPem)+1, OIC_ENCODING_PEM, &caCredId);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "OCSaveTrustCertChain error: %d", res);
        goto exit;
    }

    /* Create identity certificate for use by the CA. */
    res = OCGenerateKeyPair(&g_idPublicKey, &idPublicKeyLen, &idKey, &idKeyLen);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCGenerateKeyPair failed, error: %d", res);
        goto exit;
    }

    res = OCGenerateRandomSerialNumber(&serial, &serialLen);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCGenerateRandomSerialNumber failed, error: %d", res);
        goto exit;
    }

    OicUuid_t* uuidForCert = inputUuid;
    OicUuid_t uuid = { 0 };
    if (inputUuid == NULL)
    {
        res = OCGetDeviceId(&deviceId);
        if (res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to get own UUID, error: %d", res);
            goto exit;
        }
        memcpy(uuid.id, deviceId.id, sizeof(uuid.id));
        uuidForCert = &uuid;
    }

    OIC_LOG(DEBUG, TAG, "Creating own cert with UUID:");
    printUuid(uuidForCert);

    res = OCGenerateIdentityCertificate(
        uuidForCert,
        g_idPublicKey,
        g_caCertPem,
        g_caKeyPem,
        serial,
        TEST_CERT_NOT_BEFORE,
        TEST_CERT_NOT_AFTER,
        &idCert,
        &idCertLen);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to create identity cert for CA, error: %d", res);
        goto exit;
    }

    uint16_t idCertCredId = 0;
    res = OCSaveOwnCertChain(idCert, idKey, &idCertCredId);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to save CA's identity cert & key, error: %d", res);
        goto exit;
    }

exit:
    OICFree(serial);
    if (idKey != NULL)
    {
        OICClearMemory(idKey, idKeyLen);
        OICFree(idKey);
    }
    OICFree(idCert);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return (res == OC_STACK_OK) ? 0 : -1;
}

static int setupOwnRoleCert(OicUuid_t* inputUuid, const char* role, const char* authority)
{
    OCUUIdentity deviceId = { 0 };
    char* serial = NULL;
    size_t serialLen = 0;
    char* roleCert = NULL;
    size_t roleCertLen = 0;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (g_idPublicKey == NULL)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed because own public key is NULL, has setupOwnCert been called?", __func__);
        return -1;
    }

    /* Create role certificate. */
    OCStackResult res = OCGenerateRandomSerialNumber(&serial, &serialLen);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCGenerateRandomSerialNumber failed, error: %d", res);
        goto exit;
    }

    OicUuid_t* uuidForCert = inputUuid;
    OicUuid_t uuid = { 0 };
    if (inputUuid == NULL)
    {
        res = OCGetDeviceId(&deviceId);
        if (res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to get own UUID, error: %d", res);
            goto exit;
        }
        memcpy(uuid.id, deviceId.id, sizeof(uuid.id));
        uuidForCert = &uuid;
    }

    OIC_LOG(DEBUG, TAG, "Creating own role cert with UUID:");
    printUuid(uuidForCert);

    res = OCGenerateRoleCertificate(
        uuidForCert,
        g_idPublicKey,
        g_caCertPem,
        g_caKeyPem,
        serial,
        TEST_CERT_NOT_BEFORE,
        TEST_CERT_NOT_AFTER,
        role,
        authority,
        &roleCert,
        &roleCertLen);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to create role cert, error: %d", res);
        goto exit;
    }

    uint16_t roleCertCredId = 0;
    res = OCSaveOwnRoleCert(roleCert, &roleCertCredId);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to save role cert, error: %d", res);
        goto exit;
    }

exit:
    OICFree(serial);
    OICFree(roleCert);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return (res == OC_STACK_OK) ? 0 : -1;
}

/*
 * Create an ACL for the /a/led resource.
 * Caller must call OCDeleteACLList(newAcl).
 * The role and authority parameters are optional.  If role is not NULL, a
 * role-based ACL will be created. Otherwise a subject-based ACL is created.
 */
static int createLedAcl(OicSecAcl_t** newAcl, const char* role, const char* authority)
{
    int ret = -1;
    OCUUIdentity ownUuid = { 0 };
    OicSecAcl_t* acl = NULL;
    OicSecAce_t* ace = NULL;
    OicSecRsrc_t* rsrc = NULL;
    const char* resource = "/a/led";
    const char* resource_type = "core.led";
    const char* resource_interface = "oic.if.baseline";
    uint16_t perms = PERMISSION_FULL_CONTROL;

    /* Create an ACL with one ACE */
    acl = (OicSecAcl_t*)OICCalloc(1, sizeof(OicSecAcl_t));
    if (!acl)
    {
        OIC_LOG_V(ERROR, TAG, "%s: OICCalloc failed (acl)", __func__);
        goto exit;
    }
    ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    if (!ace)
    {
        OIC_LOG_V(ERROR, TAG, "%s: OICCalloc failed (ace)", __func__);
        goto exit;
    }
    LL_APPEND(acl->aces, ace);

    if (role != NULL)    /* Create a role-based ACL */
    {
        ace->subjectType = OicSecAceRoleSubject;
        assert(strlen(role) + 1 < ROLEID_LENGTH);
        memcpy(ace->subjectRole.id, role, strlen(role) + 1);

        if (authority != NULL)
        {
            assert(strlen(authority) + 1 < ROLEAUTHORITY_LENGTH);
            memcpy(ace->subjectRole.authority, role, strlen(authority) + 1);
        }
        OIC_LOG_V(DEBUG, TAG, "Creating ACE with role id = %s, authority = %s:", ace->subjectRole.id, ace->subjectRole.authority);
    }
    else /* Create a subject based ACL */
    {
        /* Set uuid to our own */
        OCStackResult res = OCGetDeviceId(&ownUuid);
        if (res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to get own UUID, error: %d", res);
            goto exit;
        }
        ace->subjectType = OicSecAceUuidSubject;
        memcpy(ace->subjectuuid.id, ownUuid.id, sizeof(ace->subjectuuid.id));

        OicUuid_t uuid = { 0 };
        memcpy(uuid.id, ownUuid.id, sizeof(uuid.id));
        OIC_LOG(DEBUG, TAG, "Creating ACE with UUID:");
        printUuid(&uuid);
    }

    /* Add a resource (e.g. '/a/led') to the ACE */
    rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    if (!rsrc)
    {
        OIC_LOG_V(ERROR, TAG, "%s: OICCalloc failed (rsrc)", __func__);
        goto exit;
    }
    LL_APPEND(ace->resources, rsrc);
    rsrc->href = OICStrdup(resource);

    /* Set resource type, e.g., 'core.led' */
    rsrc->typeLen = 1;
    rsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char*));
    if (!rsrc->types)
    {
        OIC_LOG_V(ERROR, TAG, "%s: OICCalloc failed (rsrc->types)", __func__);
        goto exit;
    }
    rsrc->types[0] = OICStrdup(resource_type);

    /* Set interface, e.g., 'oic.if.baseline' */
    rsrc->interfaceLen = 1;
    rsrc->interfaces = (char**)OICCalloc(rsrc->interfaceLen, sizeof(char*));
    if (!rsrc->interfaces)
    {
        OIC_LOG_V(ERROR, TAG, "%s: OICCalloc failed (rsrc->interfaces)", __func__);
        goto exit;
    }
    rsrc->interfaces[0] = OICStrdup(resource_interface);

    if (!rsrc->href || !rsrc->types[0] || !rsrc->interfaces[0])
    {
        OIC_LOG_V(ERROR, TAG, "%s: OICStrdup failed", __func__);
        goto exit;
    }

    /* Set permission for the ACE */
    ace->permission = perms;

    ret = 0; /* success */
    *newAcl = acl;

exit:

    if (ret != 0)
    {
        *newAcl = NULL;
        OCDeleteACLList(acl);
    }

    return ret;
}

static int provisionAcl(int dev_num, OicSecAcl_t* acl)
{
    OCStackResult res = OC_STACK_ERROR;

    OCProvisionDev_t* dev = getDevInst((const OCProvisionDev_t*)g_own_list, dev_num);
    if (!dev)
    {
        OIC_LOG(ERROR, TAG, "getDevInst: device instance empty");
        goto exit;
    }

    g_doneCB = false;
    res = OCProvisionACL((void*)g_ctx, dev, acl, provisionAclCB);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionACL API error: %d", res);
        goto exit;
    }
    if (waitCallbackRet())
    {
        OIC_LOG(ERROR, TAG, "OCProvisionACL callback error");
        goto exit;
    }
    if (!g_successCB)
    {
        OIC_LOG_V(ERROR, TAG, "%s callback completed, but failed", __func__);
        goto exit;
    }

exit:

    return (res == OC_STACK_OK) ? 0 : -1;
}

/* Function to work around IOT-1927.  The ocrandom.h include is only required for the workaround.
 * @todo: when IOT-1927 is resolved remove this function
 */
#include "ocrandom.h"
int workAroundBug()
{
    /* Remove credential for 31313131-3131-3131-3131-313131313131 */
    const char* uuidStr = "31313131-3131-3131-3131-313131313131";
    OicUuid_t uuid = { 0 };
    if (!OCConvertStringToUuid(uuidStr, uuid.id))
    {
        OIC_LOG(ERROR, TAG, "UUID conversion failed - caller bug, or the .dat file not longer contains a cred for this UUID. ");
        return -1;
    }

    OCStackResult res = OCRemoveCredential(&uuid);
    if (res != OC_STACK_RESOURCE_DELETED)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to remove credential for subject UUID: ", __func__);
        OIC_LOG_BUFFER(DEBUG, TAG, uuid.id, UUID_LENGTH);
        return -1;
    }

    return 0;
}

static int testCertUse(int dev_num)
{
    char* csr = NULL;
    char* deviceCert = NULL;
    const char* uri = "/a/led";
    OicSecAcl_t* acl = NULL;

    // Make sure we own at least one device to provision
    if (!g_own_list || (g_own_cnt == 0))
    {
        OIC_LOG(ERROR, TAG, "Owned device list empty, must discover unowned devices first");
        return -1;  // Error, we should have registered unowned devices already
    }

    /* Provision the device with the CA root, and issue it a cert. */
    if (provisionTrustAnchor(dev_num) != 0)
    {
        return -1;
    }

    if (getCsr(dev_num, &csr) != 0)
    {
        return -1;
    }

    int ret = createCertFromCSR(g_caKeyPem, g_caCertPem, csr, NULL, NULL, &deviceCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to create identity certificate", __func__);
        goto exit;
    }

    ret = provisionCert(dev_num, deviceCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to provision certificate", __func__);
        goto exit;
    }

    /* Try a GET request, expecting success because the owner credential is used.*/
    ret = doGetRequest(uri, dev_num);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed when requesting %s", __func__, uri);
        goto exit;
    }

    ret = createLedAcl(&acl, NULL, NULL);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to create ACL", __func__);
        goto exit;
    }

    /* Provision an ACL on the server, allowing us to access '/a/led' with our cert. */
    ret = provisionAcl(dev_num, acl);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to provision ACL", __func__);
        goto exit;
    }

    /* Remove the owner credential */
    OCStackResult res = OCRemoveCredential(&g_uuidDev1);
    if (res != OC_STACK_RESOURCE_DELETED)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to remove owner credential for subject UUID: ", __func__);
        OIC_LOG_BUFFER(DEBUG, TAG, g_uuidDev1.id, UUID_LENGTH);
        ret = -1;
        goto exit;
    }

    /*
     * Work around bug IOT-1927
     * @todo: When that bug is resolved, remove this call and the function workAroundBug
     */
    if (workAroundBug() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s bug workaround failed: ", __func__);
        ret = -1;
        goto exit;
    }

    /* Close all secure sessions, so that we don't re-use a cached session */
    if (closeAllSessions() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to close sessions", __func__);
        ret = -1;
        goto exit;
    }

    /* Try a GET request, expect failure, we don't share a credential. */
    ret = doGetRequest(uri, dev_num);
    if (ret == 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Get request to %s succeeded, but should have failed", __func__, uri);
        goto exit;
    }

    /* Provision ourself a valid certificate credential */
    ret = setupOwnCert(NULL);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to self-provision a key/certificate", __func__);
        goto exit;
    }

    /* Close all secure sessions again */
    if (closeAllSessions() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to close sessions", __func__);
        ret = -1;
        goto exit;
    }

    /* Try a get request, expect success */
    ret = doGetRequest(uri, dev_num);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Get request to %s failed, but should have succeeded", __func__, uri);
        goto exit;
    }

exit:

    OICFree(csr);
    OICFree(deviceCert);
    OCDeleteACLList(acl);

    return ret;
}

static int testRoleProvisioning(int dev_num)
{
    char* csr = NULL;
    char* idCert = NULL;
    char* roleCert = NULL;

    // Make sure we own at least one device to provision
    if (!g_own_list || (g_own_cnt == 0))
    {
        OIC_LOG(ERROR, TAG, "Owned device list empty, must discover unowned devices first");
        return -1;  // Error, we should have registered unowned devices already
    }

    /* Provision the device with the CA root, and issue it a role and identity cert. */
    if (provisionTrustAnchor(dev_num) != 0)
    {
        return -1;
    }

    if (getCsr(dev_num, &csr) != 0)
    {
        return -1;
    }

    int ret = createCertFromCSR(g_caKeyPem, g_caCertPem, csr, NULL, NULL, &idCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to create identity certificate", __func__);
        goto exit;
    }

    ret = provisionCert(dev_num, idCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to provision id certificate", __func__);
        goto exit;
    }

    /* The first role cert will have no authority field (it's optional) */
    ret = createCertFromCSR(g_caKeyPem, g_caCertPem, csr, TEST_CERT_ROLE1, NULL, &roleCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to create role certificate", __func__);
        goto exit;
    }

    ret = provisionCert(dev_num, roleCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to provision role certificate", __func__);
        goto exit;
    }
    OICFreeAndSetToNull((void**)&roleCert);

    /* The second will have the authority field set */
    ret = createCertFromCSR(g_caKeyPem, g_caCertPem, csr, TEST_CERT_ROLE2, TEST_CERT_AUTHORITY, &roleCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to create role certificate", __func__);
        goto exit;
    }

    ret = provisionCert(dev_num, roleCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to provision role certificate", __func__);
        goto exit;
    }

exit:

    OICFree(csr);
    OICFree(idCert);
    OICFree(roleCert);

    return ret;
}

static int testRoleAssertion(int dev_num)
{
    char* csr = NULL;
    char* idCert = NULL;
    char* roleCert = NULL;
    OicSecAcl_t* acl = NULL;

    /* Make sure we own at least one device to provision */
    if (!g_own_list || (g_own_cnt == 0))
    {
        OIC_LOG(ERROR, TAG, "Owned device list empty, must discover unowned devices first");
        return -1;  // Error, we should have registered unowned devices already
    }

    /* Provision the device with the CA root, and issue it an identity cert. */
    if (provisionTrustAnchor(dev_num) != 0)
    {
        return -1;
    }

    if (getCsr(dev_num, &csr) != 0)
    {
        return -1;
    }

    int ret = createCertFromCSR(g_caKeyPem, g_caCertPem, csr, NULL, NULL, &idCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to create identity certificate", __func__);
        goto exit;
    }

    ret = provisionCert(dev_num, idCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to provision id certificate", __func__);
        goto exit;
    }

    /* Create and provision a role-based ACL allowing ROLE1 to access '/a/led'. */
    ret = createLedAcl(&acl, TEST_CERT_ROLE1, NULL);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to create led ACL", __func__);
        goto exit;
    }

    ret = provisionAcl(dev_num, acl);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to provision led ACL", __func__);
        goto exit;
    }

    /* Provision ourselves an identity and role cert.
     * For the identity cert we use a random UUID, since the server has an ACE granting our UUID
     * access to everything (as owner). We don't want to remove this ACE because it would lock
     * us out. Using another UUID makes us appear as another device on the network.
     */
    OicUuid_t notOurUuid;
    (void)OCGenerateUuid(notOurUuid.id);
    ret = setupOwnCert(&notOurUuid);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to self-provision a key/certificate", __func__);
        goto exit;
    }

    ret = setupOwnRoleCert(NULL, TEST_CERT_ROLE1, NULL);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to self-provision a key/certificate", __func__);
        goto exit;
    }

    /* Remove the owner credential so that we don't use it when asserting role certs. */
    OCStackResult res = OCRemoveCredential(&g_uuidDev1);
    if (res != OC_STACK_RESOURCE_DELETED)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to remove owner credential for subject UUID: ", __func__);
        OIC_LOG_BUFFER(DEBUG, TAG, g_uuidDev1.id, UUID_LENGTH);
        ret = -1;
        goto exit;
    }

    /*
     * Work around bug IOT-1927
     * @todo: When that bug is resolved, remove this call and the function workAroundBug
     */
    if (workAroundBug() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s bug workaround failed: ", __func__);
        ret = -1;
        goto exit;
    }

    /* Close all secure sessions */
    if (closeAllSessions() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to close sessions", __func__);
        ret = -1;
        goto exit;
    }

    /* Assert role certificate */
    OCProvisionDev_t *device = NULL;
    device = getDevInst(g_own_list, dev_num);
    if (!device)
    {
        OIC_LOG(ERROR, TAG, "Selected device does not exist");
        ret = -1;
        goto exit;
    }

    OCDevAddr devAddr = device->endpoint;
    devAddr.port = device->securePort;

    g_doneCB = false;
    res = OCAssertRoles(g_ctx, &devAddr, assertRolesCB);
    if (res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCAssertRoles returned error %d with method", res);
        ret = -1;
        goto exit;
    }
    if (waitCallbackRet())
    {
        OIC_LOG_V(ERROR, TAG, "%s callback error", __func__);
    }
    if (!g_successCB)
    {
        ret = -1;
        goto exit;
    }

exit:
    OICFree(csr);
    OICFree(idCert);
    OICFree(roleCert);
    OCDeleteACLList(acl);

    return ret;
}

static int testRoleAssertionAndUse(int dev_num)
{
    char* csr = NULL;
    char* idCert = NULL;
    char* roleCert = NULL;
    const char* uri = "/a/led";
    OicSecAcl_t* acl = NULL;

    // Make sure we own at least one device to provision
    if (!g_own_list || (g_own_cnt == 0))
    {
        OIC_LOG(ERROR, TAG, "Owned device list empty, must discover unowned devices first");
        return -1;  // Error, we should have registered unowned devices already
    }

    /* Provision the device with the CA root, and issue it an identity cert. */
    if (provisionTrustAnchor(dev_num) != 0)
    {
        return -1;
    }

    if (getCsr(dev_num, &csr) != 0)
    {
        return -1;
    }

    int ret = createCertFromCSR(g_caKeyPem, g_caCertPem, csr, NULL, NULL, &idCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to create identity certificate", __func__);
        goto exit;
    }

    ret = provisionCert(dev_num, idCert);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to provision id certificate", __func__);
        goto exit;
    }

    /* Create and provision a role-based ACL allowing ROLE1 to access '/a/led'. */
    ret = createLedAcl(&acl, TEST_CERT_ROLE1, NULL);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to create led ACL", __func__);
        goto exit;
    }

    ret = provisionAcl(dev_num, acl);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to provision led ACL", __func__);
        goto exit;
    }

    /* Provision ourselves an identity and role cert.
     * For the identity cert we use a random UUID, since the server has an ACE granting our UUID
     * access to everything (as owner). We don't want to remove this ACE because it would lock
     * us out. Using another UUID makes us appear as another device on the network.
     */
    OicUuid_t notOurUuid;
    (void) OCGenerateUuid(notOurUuid.id);
    ret = setupOwnCert(&notOurUuid);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to self-provision a key/certificate", __func__);
        goto exit;
    }

    ret = setupOwnRoleCert(NULL, TEST_CERT_ROLE1, NULL);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to self-provision a key/certificate", __func__);
        goto exit;
    }

    /* Remove the owner credential so that we don't use it when asserting role certs. */
    OCStackResult res = OCRemoveCredential(&g_uuidDev1);
    if (res != OC_STACK_RESOURCE_DELETED)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to remove owner credential for subject UUID: ", __func__);
        OIC_LOG_BUFFER(DEBUG, TAG, g_uuidDev1.id, UUID_LENGTH);
        ret = -1;
        goto exit;
    }

    /*
    * Work around bug IOT-1927
    * @todo: When that bug is resolved, remove this call and the function workAroundBug
    */
    if (workAroundBug() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s bug workaround failed: ", __func__);
        ret = -1;
        goto exit;
    }

    /* Close all secure sessions */
    if (closeAllSessions() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to close sessions", __func__);
        ret = -1;
        goto exit;
    }

    /* Try a get request, expect success, role certificate will be automatically asserted */
    ret = doGetRequest(uri, dev_num);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Get request to %s failed, but should have succeeded", __func__, uri);
        goto exit;
    }

exit:
    OICFree(csr);
    OICFree(idCert);
    OICFree(roleCert);
    OCDeleteACLList(acl);

    return ret;
}

static int provisionSymmetricRoleCred(int dev_num)
{
    OicSecRole_t role;
    memset(&role, 0, sizeof(role));
    OICStrcpy(role.id, sizeof(role.id), TEST_CERT_ROLE1);

    // call |OCProvisionCredentials| API
    // calling this API with callback actually acts like blocking
    // for error checking, the return value saved and printed
    g_doneCB = false;
    OCStackResult rst =
        OCProvisionSymmetricRoleCredentials((void*) g_ctx,
                    SYMMETRIC_PAIR_WISE_KEY, OWNER_PSK_LENGTH_128,
                    getDevInst((const OCProvisionDev_t*) g_own_list, dev_num),
                    NULL, NULL, &role,
                    provisionCredCB);
    if (OC_STACK_OK != rst)
    {
        OIC_LOG_V(ERROR, TAG, "OCProvisionCredentials API error: %d", rst);
        return -1;
    }
    if (waitCallbackRet())  // input |g_doneCB| flag implicitly
    {
        OIC_LOG(ERROR, TAG, "OCProvisionCredentials callback error");
        return -1;
    }

    return 0;
}

static int testSymmetricRoleUse(int dev_num)
{
    const char* uri = "/a/led";
    OicSecAcl_t* acl = NULL;

    // Make sure we own at least one device to provision
    if (!g_own_list || (g_own_cnt == 0))
    {
        OIC_LOG(ERROR, TAG, "Owned device list empty, must discover unowned devices first");
        return -1;  // Error, we should have registered unowned devices already
    }

    /* Create and provision a role-based ACL allowing ROLE1 to access '/a/led'. */
    int ret = createLedAcl(&acl, TEST_CERT_ROLE1, NULL);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to create led ACL", __func__);
        return ret;
    }

    ret = provisionAcl(dev_num, acl);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to provision led ACL", __func__);
        goto exit;
    }

    /* Remove the owner credential so that we don't use it when asserting role certs. */
    OCStackResult res = OCRemoveCredential(&g_uuidDev1);
    if (res != OC_STACK_RESOURCE_DELETED)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to remove owner credential for subject UUID: ", __func__);
        OIC_LOG_BUFFER(DEBUG, TAG, g_uuidDev1.id, UUID_LENGTH);
        ret = -1;
        goto exit;
    }

    /* The server has an owner PSK associated with our GUID. Change our GUID to something else,
     * which will both be used to generate the role credential and for the later connection.
     */
    const OCUUIdentity newIdentity = { .id = { 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46 } };
    if (OC_STACK_OK != OCSetDeviceId(&newIdentity))
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to set device ID", __func__);
        ret = -1;
        goto exit;
    }

    /* Create a new symmetric credential with the role. */
    ret = provisionSymmetricRoleCred(dev_num);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed to provision symmetric role pair-wise keys", __func__);
        goto exit;
    }

    /* Close all secure sessions */
    if (closeAllSessions() != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to close sessions", __func__);
        ret = -1;
        goto exit;
    }

    /* Try a get request, expect success */
    ret = doGetRequest(uri, dev_num);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s Get request to %s failed, but should have succeeded", __func__, uri);
        goto exit;
    }

exit:

    OCDeleteACLList(acl);

    return ret;
}

/* Get a specific device from the provided device list. The devices in the list
 * are numbered starting from 1.
 */
static OCProvisionDev_t* getDevInst(const OCProvisionDev_t* dev_lst, const int dev_num)
{
    if (!dev_lst || 0 >= dev_num)
    {
        printf("     Device List is Empty..\n");
        return NULL;
    }

    OCProvisionDev_t* lst = (OCProvisionDev_t*)dev_lst;
    for (int i = 0; lst; )
    {
        if (dev_num == ++i)
        {
            return lst;
        }
        lst = lst->next;
    }

    OIC_LOG_V(ERROR, TAG, "%s failed, requested device not found in list (does the device need to be discovered or owned first?)", __func__);
    return NULL;  // in here |lst| is always |NULL|
}

static int printDevList(const OCProvisionDev_t* dev_lst)
{
    if (!dev_lst)
    {
        printf("     Device List is Empty..\n\n");
        return 0;
    }

    OCProvisionDev_t* lst = (OCProvisionDev_t*)dev_lst;
    int lst_cnt = 0;
    for (; lst; )
    {
        printf("     [%d] ", ++lst_cnt);
        printUuid((const OicUuid_t*)&lst->doxm->deviceID);
        printf("\n");
        lst = lst->next;
    }
    printf("\n");

    return lst_cnt;
}

static size_t printResultList(const OCProvisionResult_t* rslt_lst, const size_t rslt_cnt)
{
    if (!rslt_lst || (0 == rslt_cnt))
    {
        printf("     Device List is Empty..\n\n");
        return 0;
    }

    size_t lst_cnt = 0;
    for (; rslt_cnt > lst_cnt; ++lst_cnt)
    {
        printf("     [%" PRIuPTR "] ", lst_cnt + 1);
        printUuid((const OicUuid_t*)&rslt_lst[lst_cnt].deviceId);
        printf(" - result: %d\n", rslt_lst[lst_cnt].res);
    }
    printf("\n");

    return lst_cnt;
}

static int saveUuid(const OCProvisionResult_t* rslt_lst, const size_t rslt_cnt)
{
    if (!rslt_lst || (rslt_cnt != 1))
    {
        OIC_LOG_V(ERROR, TAG, "%s: expected only one device", __func__);
        return -1;
    }

    memcpy(&g_uuidDev1, &rslt_lst[0].deviceId, sizeof(OicUuid_t));

    return 0;
}

static void printUuid(const OicUuid_t* uid)
{
    for (int i = 0; i<UUID_LENGTH; )
    {
        printf("%02X", (*uid).id[i++]);
        if (i == 4 || i == 6 || i == 8 || i == 10)  // canonical format for UUID has '8-4-4-4-12'
        {
            printf("-");
        }
    }
    printf("\n");
}

static FILE* fopen_prvnMng(const char* path, const char* mode)
{
    if (0 == strncmp(path, OC_SECURITY_DB_DAT_FILE_NAME, strlen(OC_SECURITY_DB_DAT_FILE_NAME)))
    {
        // input |g_svr_db_fname| internally by force, not using |path| parameter
        // because |OCPersistentStorage::open| is called |OCPersistentStorage| internally
        // with its own |SVR_DB_FILE_NAME|
        return fopen(SVR_DB_FILE_NAME, mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

static int waitCallbackRet(void)
{
    for(int i = 0; !g_doneCB; ++i)
    {
        if(OC_STACK_OK != OCProcess())
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return -1;
        }

        if (i == CALLBACK_TIMEOUT)
        {
            return -1;
        }

        sleep(1);
    }

    return 0;
}

void shutdownProvisionClient()
{
    if(OC_STACK_OK != OCStop())
    {
        OIC_LOG(ERROR, TAG, "OCStack stop error");
    }
    OCDeleteDiscoveredDevices(g_own_list);  // after here |g_own_list| points to nothing
    OCDeleteDiscoveredDevices(g_unown_list);  // after here |g_unown_list| points to nothing

    OICFreeAndSetToNull((void**)&g_svr_fname);
    OICFreeAndSetToNull((void**)&g_prvn_fname);
}

static int initDiscoverRegisterAllDevices()
{
    if(initProvisionClient())
    {
        OIC_LOG_V(ERROR, TAG, "%s: ProvisionClient init error", __func__);
        return -1;
    }

    if(discoverAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Discovery failed", __func__);
        return -1;
    }

    if(registerDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to onboard devices", __func__);
        return -1;
    }

    if(discoverAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Re-discovery failed after registerDevices", __func__);
        return -1;
    }

    return 0;

}

int TestTrustAnchorProvisioning()
{
    int ret = -1;

    OIC_LOG_V(ERROR, TAG, "Running %s", __func__);

    if(initDiscoverRegisterAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to discover and provision devices", __func__);
        goto exit;
    }

    /* There should be one owned device with number 1. */
    if(provisionTrustAnchor(1))
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to provision trust anchor to device", __func__);
        goto exit;
    }

    ret = 0;

exit:

    shutdownProvisionClient();

    return ret;
}

int TestCSRResource()
{
    int ret = -1;

    OIC_LOG_V(ERROR, TAG, "Running %s", __func__);

    if(initDiscoverRegisterAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to discover and provision devices", __func__);
        goto exit;
    }

    /* There should be one owned device with number 1. */
    if(getCsr(1, NULL))
    {
        OIC_LOG(ERROR, TAG, "Failed to get CSR from device");
        goto exit;
    }

    ret = 0;

exit:

    shutdownProvisionClient();

    return ret;
}

int TestCertUse()
{
    int ret = -1;

    OIC_LOG_V(ERROR, TAG, "Running %s", __func__);

    if (initDiscoverRegisterAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to discover and provision devices", __func__);
        goto exit;
    }

    /* There should be one owned device with number 1. */
    if (testCertUse(1))
    {
        OIC_LOG(ERROR, TAG, "Failed to authenticate to device with certificate");
        goto exit;
    }

    ret = 0;

exit:

    shutdownProvisionClient();

    return ret;
}

int TestRoleProvisioning()
{
    int ret = -1;

    OIC_LOG_V(ERROR, TAG, "Running %s", __func__);

    if (initDiscoverRegisterAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to discover and provision devices", __func__);
        goto exit;
    }

    /* There should be one owned device with number 1. */
    if (testRoleProvisioning(1))
    {
        OIC_LOG(ERROR, TAG, "Failed to provision roles to device");
        goto exit;
    }

    ret = 0;

exit:

    shutdownProvisionClient();

    return ret;
}

int TestRoleAssertion()
{
    int ret = -1;

    OIC_LOG_V(ERROR, TAG, "Running %s", __func__);

    if (initDiscoverRegisterAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to discover and provision devices", __func__);
        goto exit;
    }

    /* There should be one owned device with number 1. */
    if (testRoleAssertion(1))
    {
        OIC_LOG(ERROR, TAG, "Failed to assert roles");
        goto exit;
    }

    ret = 0;

exit:

    shutdownProvisionClient();

    return ret;
}

int TestRoleAssertionAndUse()
{
    int ret = -1;

    OIC_LOG_V(ERROR, TAG, "Running %s", __func__);

    if (initDiscoverRegisterAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to discover and provision devices", __func__);
        goto exit;
    }

    /* There should be one owned device with number 1. */
    if (testRoleAssertionAndUse(1))
    {
        OIC_LOG(ERROR, TAG, "Failed to assert and use roles");
        goto exit;
    }

    ret = 0;

exit:

    shutdownProvisionClient();

    return ret;
}

int TestSymmetricRoleUse()
{
    int ret = -1;

    OIC_LOG_V(ERROR, TAG, "Running %s", __func__);

    if (initDiscoverRegisterAllDevices())
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to discover and provision devices", __func__);
        goto exit;
    }

    /* There should be one owned device with number 1. */
    if (testSymmetricRoleUse(1))
    {
        OIC_LOG(ERROR, TAG, "Failed to use symmetric key role");
        goto exit;
    }

    ret = 0;

exit:

    shutdownProvisionClient();

    return ret;
}

// main function for provisioning client using C-level provisioning API
int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("%s: No test number provided (argc = %d)\n", argv[0], argc);
        return 1;
    }

    unsigned int testNumber = strtoul(argv[1], NULL, 10);
    switch (testNumber)
    {
        case 1:
            return TestTrustAnchorProvisioning();
        case 2:
            return TestCSRResource();
        case 3:
            return TestCertUse();
        case 4:
            return TestRoleProvisioning();
        case 5:
            return TestRoleAssertion();
        case 6:
            return TestRoleAssertionAndUse();
        case 7:
            return TestSymmetricRoleUse();
        default:
            printf("%s: Invalid test number\n", argv[0]);
            return 1;

        /* Note: when adding a new test, update NUM_TESTS in provisioningTest.py */
    }

}

#ifdef __cplusplus
}
#endif //__cplusplus
