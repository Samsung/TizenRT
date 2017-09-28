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
#include "iotivity_config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ocprovisioningmanager.h"
#include "secureresourceprovider.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "aclresource.h"
#include "pstatresource.h"
#include "srmresourcestrings.h"
#include "credresource.h"
#include "doxmresource.h"
#include "pconfresource.h"
#include "credentialgenerator.h"
#include "cainterface.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif
#include "pmtypes.h"
#include "pmutility.h"
#include "srmutility.h"
#include "provisioningdatabasemanager.h"
#include "base64.h"
#include "utlist.h"
#include "ocpayload.h"

#ifdef __WITH_DTLS__
#include "crlresource.h"
#endif // WITH_X509__

#define TAG "OIC_SRPAPI"

/**
 * Macro to verify argument is not equal to NULL.
 * eg: VERIFY_NON_NULL_RET(TAG, ptrData, ERROR,OC_STACK_ERROR);
 */
#define VERIFY_NON_NULL_RET(tag, arg, logLevel, retValue) { if (NULL == (arg)) \
            { OIC_LOG((logLevel), tag, #arg " is NULL"); return retValue; } }

/**
 * Macro to verify success of operation.
 * eg: VERIFY_SUCCESS_RET(TAG, OC_STACK_OK == foo(), ERROR, OC_STACK_ERROR);
 */
#define VERIFY_SUCCESS_RET(tag, op, logLevel, retValue) { if (!(op)) \
            {OIC_LOG((logLevel), tag, #op " failed!!"); return retValue;} }


trustCertChainContext_t g_trustCertChainNotifier;

/**
 * Structure to carry credential data to callback.
 */
typedef struct CredentialData CredentialData_t;
struct CredentialData
{
    void *ctx;                                  /**< Pointer to user context.**/
    const OCProvisionDev_t *deviceInfo1;        /**< Pointer to OCProvisionDev_t.**/
    const OCProvisionDev_t *deviceInfo2;        /**< Pointer to OCProvisionDev_t.**/
    OicSecCred_t *credInfo;                     /**< Pointer to OicSecCred_t.**/
    OicSecCred_t *credInfoFirst;                /**< Pointer to OicSecCred_t.**/
    OCProvisionResultCB resultCallback;         /**< Pointer to result callback.**/
    OCProvisionResult_t *resArr;                /**< Result array.**/
    int numOfResults;                           /**< Number of results in result array.**/
};

/**
 * Structure to carry ACL provision API data to callback.
 */
typedef struct ACLData ACLData_t;
struct ACLData
{
    void *ctx;                                  /**< Pointer to user context.**/
    const OCProvisionDev_t *deviceInfo;         /**< Pointer to PMDevInfo_t.**/
    OCProvisionResultCB resultCallback;         /**< Pointer to result callback.**/
    OCProvisionResult_t *resArr;                /**< Result array.**/
    int numOfResults;                           /**< Number of results in result array.**/
};

// Structure to carry get security resource APIs data to callback.
typedef struct GetSecData GetSecData_t;
struct GetSecData {
    void *ctx;
    const OCProvisionDev_t *deviceInfo;         /**< Pointer to PMDevInfo_t.**/
    OCProvisionResultCB resultCallback;         /**< Pointer to result callback.**/
    OCProvisionResult_t *resArr;                /**< Result array.**/
    int numOfResults;                        /**< Number of results in result array.**/
};

/**
 * Structure to carry PCONF provision API data to callback.
 */
typedef struct PconfData PconfData_t;
struct PconfData
{
    void *ctx;                                  /**< Pointer to user context.**/
    const OCProvisionDev_t *deviceInfo;         /**< Pointer to PMDevInfo_t.**/
    OCProvisionResultCB resultCallback;         /**< Pointer to result callback.**/
    OCProvisionResult_t *resArr;                /**< Result array.**/
    int numOfResults;                           /**< Number of results in result array.**/
};

// Enum type index for unlink callback.
typedef enum {
    IDX_FIRST_DEVICE_RES = 0, // index for resulf of the first device
    IDX_SECOND_DEVICE_RES,    // index for result of the second device
    IDX_DB_UPDATE_RES         // index for result of updating provisioning database.
} IdxUnlinkRes_t;

// Structure to carry unlink APIs data to callback.
typedef struct UnlinkData UnlinkData_t;
struct UnlinkData {
    void *ctx;
    OCProvisionDev_t* unlinkDev;             /**< Pointer to OCProvisionDev_t to be unlinked.**/
    OCProvisionResult_t* unlinkRes;          /**< Result array.**/
    OCProvisionResultCB resultCallback;      /**< Pointer to result callback.**/
    int numOfResults;                        /**< Number of results in result array.**/
};

//Example of DELETE cred request -> coaps://0.0.0.0:5684/oic/sec/cred?sub=(BASE64 ENCODED UUID)
const char * SRP_FORM_DELETE_CREDENTIAL = "coaps://[%s]:%d%s?%s=%s";
const char * SRP_FORM_DELETE_CREDENTIAL_TCP = "coaps+tcp://[%s]:%d%s?%s=%s";

// Structure to carry remove APIs data to callback.
typedef struct RemoveData RemoveData_t;
struct RemoveData {
    void *ctx;
    OCProvisionDev_t* revokeTargetDev;      /**< Device which is going to be revoked..**/
    OCProvisionDev_t* linkedDevList;        /**< A list of devices which have invalid credential.**/
    OCProvisionResult_t* removeRes;         /**< Result array.**/
    OCProvisionResultCB resultCallback;     /**< Pointer to result callback.**/
    size_t numOfResults;                    /**< Number of results in result array.**/
    size_t sizeOfResArray;
    bool hasError;
};

/**
 * Function prototype
 */
static OCStackResult provisionCredentials(const OicSecCred_t *cred,
        const OCProvisionDev_t *deviceInfo, CredentialData_t *credData,
        OCClientResponseHandler responseHandler);


/**
 * Internal function to update result in result array.
 */
static void registerResultForCredProvisioning(CredentialData_t *credData,
                                              OCStackResult stackresult, int cause)
{

   OIC_LOG_V(INFO,TAG,"value of credData->numOfResults is %d",credData->numOfResults);
   if(1 == cause)
   {
       memcpy(credData->resArr[(credData->numOfResults)].deviceId.id,
              credData->deviceInfo1->doxm->deviceID.id,UUID_LENGTH);
   }
   else
   {
       memcpy(credData->resArr[(credData->numOfResults)].deviceId.id,
              credData->deviceInfo2->doxm->deviceID.id,UUID_LENGTH);
   }
   credData->resArr[(credData->numOfResults)].res = stackresult;
   ++(credData->numOfResults);
}

/**
 * Callback handler for handling callback of provisioning device 2.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult provisionCredentialCB2(void *ctx, OCDoHandle UNUSED,
                                                       OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL_RET(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    CredentialData_t *credData = (CredentialData_t *) ctx;
    (void)UNUSED;

    OCProvisionResultCB resultCallback = credData->resultCallback;
    OIC_LOG(INFO, TAG, "provisionCredentialCB2 called");
    if (clientResponse)
    {
        if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            registerResultForCredProvisioning(credData, OC_STACK_RESOURCE_CHANGED, 2);
            OCStackResult res =  PDMLinkDevices(&credData->deviceInfo1->doxm->deviceID,
                    &credData->deviceInfo2->doxm->deviceID);
            if (OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Error occured on PDMLinkDevices");
                return OC_STACK_DELETE_TRANSACTION;
            }
            OIC_LOG(INFO, TAG, "Link created successfully");

            ((OCProvisionResultCB)(resultCallback))(credData->ctx, credData->numOfResults,
                                                    credData->resArr,
                                                    false);
             OICFree(credData->resArr);
             OICFree(credData);
             return OC_STACK_DELETE_TRANSACTION;
        }

    }
    OIC_LOG(INFO, TAG, "provisionCredentialCB2 received Null clientResponse");
    registerResultForCredProvisioning(credData, OC_STACK_ERROR, 2);
    ((OCProvisionResultCB)(resultCallback))(credData->ctx, credData->numOfResults,
                                            credData->resArr,
                                            true);
    OICFree(credData->resArr);
    OICFree(credData);
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Callback handler for handling callback of provisioning device 1.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult provisionCredentialCB1(void *ctx, OCDoHandle UNUSED,
                                                       OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL_RET(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    (void)UNUSED;
    CredentialData_t* credData = (CredentialData_t*) ctx;
    OICFree(credData->credInfoFirst);
    const OCProvisionDev_t *deviceInfo = credData->deviceInfo2;
    OicSecCred_t *credInfo = credData->credInfo;
    const OCProvisionResultCB resultCallback = credData->resultCallback;
    if (clientResponse)
    {
        if (OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            // send credentials to second device
            registerResultForCredProvisioning(credData, OC_STACK_RESOURCE_CHANGED,1);
            OCStackResult res = provisionCredentials(credInfo, deviceInfo, credData,
                    provisionCredentialCB2);
            DeleteCredList(credInfo);
            if (OC_STACK_OK != res)
            {
                registerResultForCredProvisioning(credData, res,2);
                ((OCProvisionResultCB)(resultCallback))(credData->ctx, credData->numOfResults,
                                                        credData->resArr,
                                                        true);
                OICFree(credData->resArr);
                OICFree(credData);
                credData = NULL;
            }
        }
        else
        {
            registerResultForCredProvisioning(credData, OC_STACK_ERROR,1);
            ((OCProvisionResultCB)(resultCallback))(credData->ctx, credData->numOfResults,
                                                    credData->resArr,
                                                    true);
            OICFree(credData->resArr);
            OICFree(credData);
            credData = NULL;
        }
    }
    else
    {
        OIC_LOG(INFO, TAG, "provisionCredentialCB received Null clientResponse for first device");
        registerResultForCredProvisioning(credData, OC_STACK_ERROR,1);
       ((OCProvisionResultCB)(resultCallback))(credData->ctx, credData->numOfResults,
                                                     credData->resArr,
                                                     true);
        DeleteCredList(credInfo);
        OICFree(credData->resArr);
        OICFree(credData);
        credData = NULL;
    }
    return OC_STACK_DELETE_TRANSACTION;
}



/**
 * Internal function for handling credential generation and sending credential to resource server.
 *
 * @param[in] cred Instance of cred resource.
 * @param[in] deviceInfo information about device to which credential is to be provisioned.
 * @param[in] responseHandler callbak called by OC stack when request API receives response.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
static OCStackResult provisionCredentials(const OicSecCred_t *cred,
        const OCProvisionDev_t *deviceInfo, CredentialData_t *credData,
        OCClientResponseHandler responseHandler)
{
    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if (!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    int secureFlag = 0;
    OCStackResult res = CredToCBORPayload(cred, &secPayload->securityData,
                                          &secPayload->payloadSize, secureFlag);
    if((OC_STACK_OK != res) && (NULL == secPayload->securityData))
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Failed to CredToCBORPayload");
        return OC_STACK_NO_MEMORY;
    }

    OIC_LOG(DEBUG, TAG, "Created payload for Cred:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        deviceInfo->endpoint.addr,
                        deviceInfo->securePort,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_CRED_URI))
    {
        OIC_LOG(ERROR, TAG, "DeviceDiscoveryHandler : Failed to generate query");
        OCPayloadDestroy((OCPayload *)secPayload);
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData = {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = responseHandler;
    cbData.context = (void *) credData;
    cbData.cd = NULL;

    OCDoHandle handle = NULL;
    OCMethod method = OC_REST_POST;
    OCStackResult ret = OCDoResource(&handle, method, query, 0, (OCPayload*)secPayload,
            deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    OIC_LOG_V(INFO, TAG, "OCDoResource::Credential provisioning returned : %d",ret);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
        return ret;
    }
    return OC_STACK_OK;
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * Structure to carry certificate data to callback.
 */
typedef struct CertificateData CertData_t;
struct CertificateData
{
    void *ctx;                                  /**< Pointer to user context.**/
    const OCProvisionDev_t *deviceInfo;        /**< Pointer to OCProvisionDev_t.**/
    OicSecCred_t *credInfo;                     /**< Pointer to OicSecCred_t.**/
    OCProvisionResultCB resultCallback;         /**< Pointer to result callback.**/
    OCProvisionResult_t *resArr;                /**< Result array.**/
    int numOfResults;                           /**< Number of results in result array.**/
};

/**
 * Internal function to update result in result array.
 */
static void registerResultForCertProvisioning(CertData_t *certData,
                                              OCStackResult stackresult)
{

   OIC_LOG_V(INFO,TAG,"value of credData->numOfResults is %d",certData->numOfResults);
   memcpy(certData->resArr[(certData->numOfResults)].deviceId.id,
          certData->deviceInfo->doxm->deviceID.id,UUID_LENGTH);
   certData->resArr[(certData->numOfResults)].res = stackresult;
   ++(certData->numOfResults);
}

OCStackResult SRPRegisterTrustCertChainNotifier(void *ctx, TrustCertChainChangeCB callback)
{
    if (g_trustCertChainNotifier.callback)
    {
        OIC_LOG(ERROR, TAG, "Can't register Notifier, Unregister previous one");
        return OC_STACK_ERROR;
    }

    g_trustCertChainNotifier.callback = callback;
    g_trustCertChainNotifier.context = ctx;
    return OC_STACK_OK;
}

void SRPRemoveTrustCertChainNotifier()
{
    g_trustCertChainNotifier.callback = NULL;
    g_trustCertChainNotifier.context = NULL;
    return;
}

/**
 * Callback handler for handling callback of certificate provisioning device.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult provisionCertCB(void *ctx, OCDoHandle UNUSED,
                                                       OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL_RET(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    CertData_t *certData = (CertData_t *) ctx;
    (void)UNUSED;

    OCProvisionResultCB resultCallback = certData->resultCallback;
    OIC_LOG(INFO, TAG, "provisionCertCB called");
    if (clientResponse)
    {
        if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            registerResultForCertProvisioning(certData, OC_STACK_RESOURCE_CHANGED);
            ((OCProvisionResultCB)(resultCallback))(certData->ctx, certData->numOfResults,
                                                    certData->resArr,
                                                    false);
             OICFree(certData->resArr);
             OICFree(certData);
             return OC_STACK_DELETE_TRANSACTION;
        }

    }
    OIC_LOG(INFO, TAG, "provisionCertCredCB received Null clientResponse");
    registerResultForCertProvisioning(certData, OC_STACK_ERROR);
    ((OCProvisionResultCB)(resultCallback))(certData->ctx, certData->numOfResults,
                                            certData->resArr,
                                            true);
    OICFree(certData->resArr);
    OICFree(certData);
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult SRPProvisionTrustCertChain(void *ctx, OicSecCredType_t type, uint16_t credId,
        const OCProvisionDev_t *selectedDeviceInfo, OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "In SRPProvisionTrustCertChain");
    VERIFY_NON_NULL_RET(TAG, selectedDeviceInfo, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, resultCallback, ERROR,  OC_STACK_INVALID_CALLBACK);
    if (SIGNED_ASYMMETRIC_KEY != type)
    {
        OIC_LOG(INFO, TAG, "Invalid key type");
        return OC_STACK_INVALID_PARAM;
    }

    OicSecCred_t *trustCertChainCred = GetCredEntryByCredId(credId);
    if(NULL == trustCertChainCred)
    {
        OIC_LOG(ERROR, TAG, "Can not find matched Trust Cert. Chain.");
        return OC_STACK_NO_RESOURCE;
    }

    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        DeleteCredList(trustCertChainCred);
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    int secureFlag = 0;
    if(OC_STACK_OK != CredToCBORPayload(trustCertChainCred, &secPayload->securityData, &secPayload->payloadSize, secureFlag))
    {
        DeleteCredList(trustCertChainCred);
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Failed to CredToCBORPayload");
        return OC_STACK_NO_MEMORY;
    }
    DeleteCredList(trustCertChainCred);
    OIC_LOG(DEBUG, TAG, "Created payload for Cred:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        selectedDeviceInfo->endpoint.addr,
                        selectedDeviceInfo->securePort,
                        selectedDeviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_CRED_URI))
    {
        OIC_LOG(ERROR, TAG, "SRPProvisionTrustCertChain : Failed to generate query");
        OCPayloadDestroy((OCPayload *)secPayload);
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &provisionCertCB;
    CertData_t *certData = (CertData_t *) OICCalloc(1, sizeof(CertData_t));
    if (NULL == certData)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation problem");
        OCPayloadDestroy((OCPayload *)secPayload);
        return OC_STACK_NO_MEMORY;
    }
    certData->deviceInfo = selectedDeviceInfo;
    certData->resultCallback = resultCallback;
    certData->credInfo = NULL; //credInfo not used in the response handler
    certData->numOfResults=0;
    certData->ctx = ctx;

    int noOfRiCalls = 1;
    certData->resArr = (OCProvisionResult_t*)OICCalloc(noOfRiCalls, sizeof(OCProvisionResult_t));
    if (certData->resArr == NULL)
    {
        OICFree(certData);
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    cbData.context = (void *)certData;
    cbData.cd = NULL;
    OCMethod method = OC_REST_POST;
    OCDoHandle handle = NULL;
    OIC_LOG(DEBUG, TAG, "Sending Cred info to resource server");
    OCStackResult ret = OCDoResource(&handle, method, query,
            &selectedDeviceInfo->endpoint, (OCPayload*)secPayload,
            selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OICFree(certData->resArr);
        OICFree(certData);
    }

    VERIFY_SUCCESS_RET(TAG, (OC_STACK_OK == ret), ERROR, OC_STACK_ERROR);
    return OC_STACK_OK;
}

OCStackResult SRPSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize,
                                            OicEncodingType_t encodingType, uint16_t *credId)
{
    OIC_LOG(DEBUG, TAG, "IN SRPSaveTrustCertChain");
    VERIFY_NON_NULL_RET(TAG, trustCertChain, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, credId, ERROR,  OC_STACK_INVALID_PARAM);

    OCStackResult res = OC_STACK_ERROR;

    OicSecCred_t *cred = (OicSecCred_t *)OICCalloc(1, sizeof(*cred));
    VERIFY_NON_NULL_RET(TAG, cred, ERROR, OC_STACK_NO_MEMORY);

    res = GetDoxmDeviceID(&cred->subject);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Cann't get the device id(GetDoxmDeviceID)");
        DeleteCredList(cred);
        return res;
    }

    cred->credUsage= (char *)OICCalloc(1, strlen(TRUST_CA)+1 );
    VERIFY_NON_NULL_RET(TAG, cred->credUsage, ERROR, OC_STACK_NO_MEMORY);
    OICStrcpy(cred->credUsage, strlen(TRUST_CA) + 1, TRUST_CA);

    cred->credType = SIGNED_ASYMMETRIC_KEY;

    if (encodingType == OIC_ENCODING_PEM)
    {
        cred->optionalData.data = (uint8_t *)OICCalloc(1, chainSize + 1);
        VERIFY_NON_NULL_RET(TAG, cred->optionalData.data, ERROR, OC_STACK_NO_MEMORY);
        cred->optionalData.len = chainSize + 1;
    }
    else
    {
        cred->optionalData.data = (uint8_t *)OICCalloc(1, chainSize);
        VERIFY_NON_NULL_RET(TAG, cred->optionalData.data, ERROR, OC_STACK_NO_MEMORY);
        cred->optionalData.len = chainSize;
    }
    memcpy(cred->optionalData.data, trustCertChain, chainSize);
    cred->optionalData.encoding = encodingType;
    cred->optionalData.revstat = false;

    res = AddCredential(cred);
    if(res != OC_STACK_OK)
    {
        DeleteCredList(cred);
        return res;
    }
    *credId = cred->credId;

    if (g_trustCertChainNotifier.callback)
    {
        uint8_t *certChain = (uint8_t*)OICCalloc(1, sizeof(uint8_t) * chainSize);
        VERIFY_NON_NULL_RET(TAG, certChain, ERROR, OC_STACK_NO_MEMORY);
        memcpy(certChain, trustCertChain, chainSize);
        g_trustCertChainNotifier.callback(g_trustCertChainNotifier.context, *credId,
                certChain, chainSize);
        OICFree(certChain);
    }

    OIC_LOG(DEBUG, TAG, "OUT SRPSaveTrustCertChain");

    return res;
}


OCStackResult SRPSaveOwnCertChain(OicSecKey_t * cert, OicSecKey_t * key, uint16_t *credId)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(TAG, cert, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, cert->data, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, key, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, key->data, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, credId, ERROR,  OC_STACK_INVALID_PARAM);

    OCStackResult res = OC_STACK_ERROR;

    OicSecCred_t *cred = (OicSecCred_t *)OICCalloc(1, sizeof(*cred));
    VERIFY_NON_NULL_RET(TAG, cred, ERROR, OC_STACK_NO_MEMORY);

    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    res = GetDoxmDeviceID(&cred->subject);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Cann't get the device id(GetDoxmDeviceID)");
        DeleteCredList(cred);
        return res;
    }

    cred->credUsage= (char *)OICCalloc(1, strlen(PRIMARY_CERT)+1 );
    VERIFY_NON_NULL_RET(TAG, cred->credUsage, ERROR, OC_STACK_NO_MEMORY);
    OICStrcpy(cred->credUsage, strlen(PRIMARY_CERT) + 1, PRIMARY_CERT) ;

    cred->credType = SIGNED_ASYMMETRIC_KEY;

    OicSecKey_t *publicData = &cred->publicData;
    publicData->data = (uint8_t *)OICCalloc(1, cert->len);
    VERIFY_NON_NULL_RET(TAG, publicData->data, ERROR, OC_STACK_NO_MEMORY);
    memcpy(publicData->data, cert->data, cert->len);
    publicData->len = cert->len;
    publicData->encoding = cert->encoding;

    OicSecKey_t *privateData = &cred->privateData;
    privateData->data = (uint8_t *)OICCalloc(1, key->len);
    VERIFY_NON_NULL_RET(TAG, privateData->data, ERROR, OC_STACK_NO_MEMORY);
    memcpy(privateData->data, key->data, key->len);
    privateData->len = key->len;
    privateData->encoding = key->encoding;

    res = AddCredential(cred);
    if(res != OC_STACK_OK)
    {
        DeleteCredList(cred);
        return res;
    }
    *credId = cred->credId;

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return res;
}
#endif // __WITH_DTLS__ || __WITH_TLS__

OCStackResult SRPProvisionCredentials(void *ctx, OicSecCredType_t type, size_t keySize,
                                      const OCProvisionDev_t *pDev1,
                                      const OCProvisionDev_t *pDev2,
                                      OCProvisionResultCB resultCallback)
{
    VERIFY_NON_NULL_RET(TAG, pDev1, ERROR,  OC_STACK_INVALID_PARAM);
    if (SYMMETRIC_PAIR_WISE_KEY == type)
    {
        VERIFY_NON_NULL_RET(TAG, pDev2, ERROR,  OC_STACK_INVALID_PARAM);
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "SRPUnlinkDevices : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }
    if (SYMMETRIC_PAIR_WISE_KEY == type &&
        0 == memcmp(&pDev1->doxm->deviceID, &pDev2->doxm->deviceID, sizeof(OicUuid_t)))
    {
        OIC_LOG(INFO, TAG, "SRPUnlinkDevices : Same device ID");
        return OC_STACK_INVALID_PARAM;
    }

    if (SYMMETRIC_PAIR_WISE_KEY == type &&
       !(OWNER_PSK_LENGTH_128 == keySize || OWNER_PSK_LENGTH_256 == keySize))
    {
        OIC_LOG(INFO, TAG, "Invalid key size");
        return OC_STACK_INVALID_PARAM;
    }

    OIC_LOG(INFO, TAG, "In SRPProvisionCredentials");

    if (SYMMETRIC_PAIR_WISE_KEY == type)
    {
        bool linkExisits = true;
        OCStackResult res = PDMIsLinkExists(&pDev1->doxm->deviceID, &pDev2->doxm->deviceID, &linkExisits);

        if (res != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Internal error occured");
            return res;
        }
        if (linkExisits)
        {
            OIC_LOG(ERROR, TAG, "Link already exists");
            return OC_STACK_INVALID_PARAM;
        }
    }

    OicUuid_t provTooldeviceID =   {{0,}};
    if (OC_STACK_OK != GetDoxmDeviceID(&provTooldeviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving provisioning tool's device ID");
        return OC_STACK_ERROR;
    }
    OIC_LOG(INFO, TAG, "retrieved deviceid");
    switch (type)
    {
        case SYMMETRIC_PAIR_WISE_KEY:
        {
            const OCProvisionDev_t *firstDevice = pDev1;
            const OCProvisionDev_t *secondDevice = pDev2;

            OicSecCred_t *firstCred = NULL;
            OicSecCred_t *secondCred = NULL;
            OCStackResult res = PMGeneratePairWiseCredentials(type, keySize, &provTooldeviceID,
                    &firstDevice->doxm->deviceID, &secondDevice->doxm->deviceID,
                    &firstCred, &secondCred);
            VERIFY_SUCCESS_RET(TAG, (res==OC_STACK_OK), ERROR, OC_STACK_ERROR);
            OIC_LOG(INFO, TAG, "Credentials generated successfully");
            CredentialData_t *credData =
                (CredentialData_t *) OICCalloc(1, sizeof(CredentialData_t));
            if (NULL == credData)
            {
                OICFree(firstCred);
                OICFree(secondCred);
                OIC_LOG(ERROR, TAG, "Memory allocation problem");
                return OC_STACK_NO_MEMORY;
            }
            credData->deviceInfo1 = firstDevice;
            credData->deviceInfo2 = secondDevice;
            credData->credInfo = secondCred;
            credData->ctx = ctx;
            credData->credInfoFirst = firstCred;
            credData->numOfResults = 0;
            credData->resultCallback = resultCallback;
            // first call to provision creds to device1.
            // second call to provision creds to device2.
            int noOfRiCalls = 2;
            credData->resArr =
                (OCProvisionResult_t*)OICCalloc(noOfRiCalls, sizeof(OCProvisionResult_t));
            if (NULL == credData->resArr)
            {
                OICFree(firstCred);
                OICFree(secondCred);
                OICFree(credData);
                OIC_LOG(ERROR, TAG, "Memory allocation problem");
                return OC_STACK_NO_MEMORY;
            }
            res = provisionCredentials(firstCred, firstDevice, credData, &provisionCredentialCB1);
            if (OC_STACK_OK != res)
            {
                DeleteCredList(firstCred);
                DeleteCredList(secondCred);
                OICFree(credData->resArr);
                OICFree(credData);
            }
            OIC_LOG_V(INFO, TAG, "provisionCredentials returned: %d",res);
            VERIFY_SUCCESS_RET(TAG, (res==OC_STACK_OK), ERROR, OC_STACK_ERROR);
            return res;
        }
        default:
        {
            OIC_LOG(ERROR, TAG, "Invalid option.");
            return OC_STACK_INVALID_PARAM;
        }
    }
    return OC_STACK_ERROR;
}

/**
 * Internal Function to store results in result array during ACL provisioning.
 */
static void registerResultForACLProvisioning(ACLData_t *aclData,
                                             OCStackResult stackresult)
{
   OIC_LOG_V(INFO, TAG, "Inside registerResultForACLProvisioning aclData->numOfResults is %d\n",
                       aclData->numOfResults);
   memcpy(aclData->resArr[(aclData->numOfResults)].deviceId.id,
          aclData->deviceInfo->doxm->deviceID.id, UUID_LENGTH);
   aclData->resArr[(aclData->numOfResults)].res = stackresult;
   ++(aclData->numOfResults);
}

/**
 * Callback handler of SRPProvisionACL.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPProvisionACLCB(void *ctx, OCDoHandle UNUSED,
                                                  OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "Inside SRPProvisionACLCB.");
    (void)UNUSED;
    VERIFY_NON_NULL_RET(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    ACLData_t *aclData = (ACLData_t*)ctx;
    OCProvisionResultCB resultCallback = aclData->resultCallback;

    if (clientResponse)
    {
        if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            registerResultForACLProvisioning(aclData, OC_STACK_RESOURCE_CHANGED);
            ((OCProvisionResultCB)(resultCallback))(aclData->ctx, aclData->numOfResults,
                                                    aclData->resArr,
                                                    false);
             OICFree(aclData->resArr);
             OICFree(aclData);
             return OC_STACK_DELETE_TRANSACTION;
        }
    }
    registerResultForACLProvisioning(aclData, OC_STACK_ERROR);
    ((OCProvisionResultCB)(resultCallback))(aclData->ctx, aclData->numOfResults,
                                            aclData->resArr,
                                            true);
    OIC_LOG_V(ERROR, TAG, "SRPProvisionACLCB received Null clientResponse");
    OICFree(aclData->resArr);
    OICFree(aclData);
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult SRPProvisionACL(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
        OicSecAcl_t *acl, OCProvisionResultCB resultCallback)
{
    VERIFY_NON_NULL_RET(TAG, selectedDeviceInfo, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, acl, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, resultCallback, ERROR,  OC_STACK_INVALID_CALLBACK);

    // if rowneruuid is empty, set it to device ID
    OicUuid_t emptyOwner = {.id = {0} };
    if (memcmp(&(acl->rownerID.id), &emptyOwner, UUID_IDENTITY_SIZE) == 0)
    {
        OIC_LOG(DEBUG, TAG, "Set Rowner to PT's deviceId, because Rowner of ACL is empty");
        OicUuid_t oicUuid;

        if (OC_STACK_OK == GetDoxmDeviceID(&oicUuid))
        {
            memcpy(&(acl->rownerID.id), &oicUuid, UUID_IDENTITY_SIZE);
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "Failed to set Rowner to PT's deviceID\
                becuase it failed to retrieve Doxm DeviceID");
            return OC_STACK_ERROR;
        }
    }

    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    if(OC_STACK_OK != AclToCBORPayload(acl, &secPayload->securityData, &secPayload->payloadSize))
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Failed to AclToCBORPayload");
        return OC_STACK_NO_MEMORY;
    }
    OIC_LOG(DEBUG, TAG, "Created payload for ACL:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        selectedDeviceInfo->endpoint.addr,
                        selectedDeviceInfo->securePort,
                        selectedDeviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_ACL_URI))
    {
        OIC_LOG(ERROR, TAG, "DeviceDiscoveryHandler : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &SRPProvisionACLCB;
    ACLData_t *aclData = (ACLData_t *) OICCalloc(1, sizeof(ACLData_t));
    if (aclData == NULL)
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    aclData->deviceInfo = selectedDeviceInfo;
    aclData->resultCallback = resultCallback;
    aclData->numOfResults=0;
    aclData->ctx = ctx;
    // call to provision ACL to device1.
    int noOfRiCalls = 1;
    aclData->resArr = (OCProvisionResult_t*)OICCalloc(noOfRiCalls, sizeof(OCProvisionResult_t));
    if (aclData->resArr == NULL)
    {
        OICFree(aclData);
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    cbData.context = (void *)aclData;
    cbData.cd = NULL;
    OCMethod method = OC_REST_POST;
    OCDoHandle handle = NULL;
    OIC_LOG(DEBUG, TAG, "Sending ACL info to resource server");
    OCStackResult ret = OCDoResource(&handle, method, query,
            &selectedDeviceInfo->endpoint, (OCPayload*)secPayload,
            selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OICFree(aclData->resArr);
        OICFree(aclData);
    }
    VERIFY_SUCCESS_RET(TAG, (OC_STACK_OK == ret), ERROR, OC_STACK_ERROR);
    return OC_STACK_OK;
}

OCStackResult SRPSaveACL(const OicSecAcl_t *acl)
{
    OIC_LOG(DEBUG, TAG, "IN SRPSaveACL");
    VERIFY_NON_NULL_RET(TAG, acl, ERROR,  OC_STACK_INVALID_PARAM);

    OCStackResult res =  InstallACL(acl);

    OIC_LOG(DEBUG, TAG, "OUT SRPSaveACL");
    return res;
}

/**
 * Internal Function to store results in result array during Direct-Pairing provisioning.
 */
static void registerResultForDirectPairingProvisioning(PconfData_t *pconfData,
                                             OCStackResult stackresult)
{
   OIC_LOG_V(INFO, TAG, "Inside registerResultForDirectPairingProvisioning "
           "pconfData->numOfResults is %d\n", pconfData->numOfResults);
   memcpy(pconfData->resArr[(pconfData->numOfResults)].deviceId.id,
          pconfData->deviceInfo->doxm->deviceID.id, UUID_LENGTH);
   pconfData->resArr[(pconfData->numOfResults)].res = stackresult;
   ++(pconfData->numOfResults);
}

/**
 * Callback handler of SRPProvisionDirectPairing.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPProvisionDirectPairingCB(void *ctx, OCDoHandle UNUSED,
                                                  OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "Inside SRPProvisionDirectPairingCB.");
    (void)UNUSED;
    VERIFY_NON_NULL_RET(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    PconfData_t *pconfData = (PconfData_t*)ctx;
    OCProvisionResultCB resultCallback = pconfData->resultCallback;

    if (clientResponse)
    {
        if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            registerResultForDirectPairingProvisioning(pconfData, OC_STACK_OK);
            ((OCProvisionResultCB)(resultCallback))(pconfData->ctx, pconfData->numOfResults,
                                                    pconfData->resArr,
                                                    false);
             OICFree(pconfData->resArr);
             OICFree(pconfData);
             return OC_STACK_DELETE_TRANSACTION;
        }
    }
    registerResultForDirectPairingProvisioning(pconfData, OC_STACK_ERROR);
    ((OCProvisionResultCB)(resultCallback))(pconfData->ctx, pconfData->numOfResults,
                                            pconfData->resArr,
                                            true);
    OIC_LOG_V(ERROR, TAG, "SRPProvisionDirectPairingCB received Null clientResponse");
    OICFree(pconfData->resArr);
    OICFree(pconfData);
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult SRPProvisionDirectPairing(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
        OicSecPconf_t *pconf, OCProvisionResultCB resultCallback)
{
    VERIFY_NON_NULL_RET(TAG, selectedDeviceInfo, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, pconf, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, resultCallback, ERROR,  OC_STACK_INVALID_CALLBACK);

    // check direct-pairing capability
    if (true != selectedDeviceInfo->doxm->dpc)
    {
        OIC_LOG(ERROR, TAG, "Resouce server does not have Direct-Pairing Capability ");
        return OC_STACK_UNAUTHORIZED_REQ;
    }

    OicUuid_t provTooldeviceID =   {.id={0}};
    if (OC_STACK_OK != GetDoxmDeviceID(&provTooldeviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving provisioning tool's device ID");
        return OC_STACK_ERROR;
    }
    memcpy(&pconf->rownerID, &provTooldeviceID, sizeof(OicUuid_t));

    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    if (OC_STACK_OK != PconfToCBORPayload(pconf, &(secPayload->securityData),
                &(secPayload->payloadSize)))
    {
        OCPayloadDestroy((OCPayload*)secPayload);
        OIC_LOG(ERROR, TAG, "Failed to PconfToCborPayload");
        return OC_STACK_NO_MEMORY;
    }
    OIC_LOG(DEBUG, TAG, "Created payload for pconf set");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);


    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                selectedDeviceInfo->endpoint.addr,
                selectedDeviceInfo->securePort,
                selectedDeviceInfo->connType,
                query, sizeof(query), OIC_RSRC_PCONF_URI))
    {
        OIC_LOG(ERROR, TAG, "SRPProvisionDirectPairing : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &SRPProvisionDirectPairingCB;
    PconfData_t *pconfData = (PconfData_t *) OICCalloc(1, sizeof(PconfData_t));
    if (NULL == pconfData)
    {
        OCPayloadDestroy((OCPayload*)secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    pconfData->deviceInfo = selectedDeviceInfo;
    pconfData->resultCallback = resultCallback;
    pconfData->numOfResults=0;
    pconfData->ctx = ctx;
    // call to provision PCONF to device1.
    int noOfRiCalls = 1;
    pconfData->resArr = (OCProvisionResult_t*)OICCalloc(noOfRiCalls, sizeof(OCProvisionResult_t));
    if (NULL == pconfData->resArr)
    {
        OICFree(pconfData);
        OCPayloadDestroy((OCPayload*)secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    cbData.context = (void *)pconfData;
    cbData.cd = NULL;
    OCMethod method = OC_REST_POST;
    OCDoHandle handle = NULL;
    OIC_LOG(DEBUG, TAG, "Sending PCONF info to resource server");
    OCStackResult ret = OCDoResource(&handle, method, query,
            &selectedDeviceInfo->endpoint, (OCPayload*)secPayload,
            selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (OC_STACK_OK != ret)
    {
        OICFree(pconfData->resArr);
        OICFree(pconfData);
    }
    VERIFY_SUCCESS_RET(TAG, (OC_STACK_OK == ret), ERROR, OC_STACK_ERROR);
    return OC_STACK_OK;
}

static void DeleteUnlinkData_t(UnlinkData_t *unlinkData)
{
    if (unlinkData)
    {
        OICFree(unlinkData->unlinkDev);
        OICFree(unlinkData->unlinkRes);
        OICFree(unlinkData);
    }
}

static void registerResultForUnlinkDevices(UnlinkData_t *unlinkData, OCStackResult stackresult,
                                           IdxUnlinkRes_t idx)
{
    if (NULL != unlinkData)
    {
        OIC_LOG_V(INFO, TAG, "Inside registerResultForUnlinkDevices unlinkData->numOfResults is %d\n",
                            unlinkData->numOfResults);
        OIC_LOG_V(INFO, TAG, "Stack result :: %d", stackresult);

        OicUuid_t *pUuid = &unlinkData->unlinkRes[(unlinkData->numOfResults)].deviceId;

        // Set result in the result array according to the position (devNum).
        if (idx != IDX_DB_UPDATE_RES)
        {
            memcpy(pUuid->id, unlinkData->unlinkDev[idx].doxm->deviceID.id, sizeof(pUuid->id));
        }
        else
        {   // When deivce ID is 000... this means it's the result of database update.
            memset(pUuid->id, 0, sizeof(pUuid->id));
        }
        unlinkData->unlinkRes[(unlinkData->numOfResults)].res = stackresult;
        ++(unlinkData->numOfResults);
        OIC_LOG (INFO, TAG, "Out registerResultForUnlinkDevices");
    }
}

static OCStackResult SendDeleteCredentialRequest(void* ctx,
                                                 OCClientResponseHandler respHandler,
                                                 const OCProvisionDev_t* revokedDev,
                                                 const OCProvisionDev_t* destDev)
{
    OIC_LOG(DEBUG, TAG, "IN SendDeleteCredentialRequest");

    if (NULL == ctx || NULL == respHandler || NULL == revokedDev || NULL == destDev)
    {
        return OC_STACK_INVALID_PARAM;
    }

    char *subID = NULL;
    OCStackResult ret = ConvertUuidToStr(&revokedDev->doxm->deviceID, &subID);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "SendDeleteCredentialRequest : Failed to canonical UUID encoding");
        return OC_STACK_ERROR;
    }

    char addressEncoded[CA_MAX_URI_LENGTH] = {0};
    OCStackResult result = OCEncodeAddressForRFC6874(addressEncoded,
                                                     sizeof(addressEncoded),
                                                     destDev->endpoint.addr);
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteCredentialRequest : encoding error %d\n", result);
        return OC_STACK_ERROR;
    }

    char reqBuf[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    int snRet = 0;
                    //coaps://0.0.0.0:5684/oic/sec/cred?subjectid=(Canonical ENCODED UUID)
    const char *srpUri = SRP_FORM_DELETE_CREDENTIAL;
#ifdef __WITH_TLS__
    if(CA_ADAPTER_TCP == destDev->endpoint.adapter)
    {
        srpUri = SRP_FORM_DELETE_CREDENTIAL_TCP;
    }
#endif

    snRet = snprintf(reqBuf, sizeof(reqBuf), srpUri, addressEncoded,
                     destDev->securePort, OIC_RSRC_CRED_URI, OIC_JSON_SUBJECTID_NAME, subID);
    OICFree(subID);
    if (snRet < 0)
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteCredentialRequest : Error (snprintf) %d\n", snRet);
        return OC_STACK_ERROR;
    }
    else if ((size_t)snRet >= sizeof(reqBuf))
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteCredentialRequest : Truncated (snprintf) %d\n", snRet);
        return OC_STACK_ERROR;
    }

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(cbData));
    cbData.context = ctx;
    cbData.cb = respHandler;
    cbData.cd = NULL;
    OIC_LOG_V(INFO, TAG, "URI: %s",reqBuf);

    OIC_LOG(DEBUG, TAG, "Sending remove credential request to resource server");

    ret = OCDoResource(NULL, OC_REST_DELETE, reqBuf,
                                     &destDev->endpoint, NULL,
                                     CT_ADAPTER_IP, OC_HIGH_QOS, &cbData, NULL, 0);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteCredentialRequest : Error in OCDoResource %d", ret);
    }
    OIC_LOG(DEBUG, TAG, "OUT SendDeleteCredentialRequest");

    return ret;
}

static OCStackResult SendDeleteACLRequest(void* ctx,
                                                 OCClientResponseHandler respHandler,
                                                 const OCProvisionDev_t* revokedDev,
                                                 const OCProvisionDev_t* destDev)
{
    OIC_LOG(DEBUG, TAG, "IN SendDeleteACLRequest");

    if (NULL == ctx || NULL == respHandler || NULL == revokedDev || NULL == destDev)
    {
        return OC_STACK_INVALID_PARAM;
    }

    char *subID = NULL;
    OCStackResult ret = ConvertUuidToStr(&revokedDev->doxm->deviceID, &subID);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "SendDeleteACLRequest : Failed to canonical UUID encoding");
        return OC_STACK_ERROR;
    }

    char addressEncoded[CA_MAX_URI_LENGTH] = {0};
    OCStackResult result = OCEncodeAddressForRFC6874(addressEncoded,
                                                     sizeof(addressEncoded),
                                                     destDev->endpoint.addr);
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteCredentialRequest : encoding error %d\n", result);
        return OC_STACK_ERROR;
    }


    char reqBuf[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    int snRet = 0;
                    //coaps://0.0.0.0:5684/oic/sec/acl?subjectuuid=(Canonical ENCODED UUID)
    snRet = snprintf(reqBuf, sizeof(reqBuf), SRP_FORM_DELETE_CREDENTIAL, addressEncoded,
                     destDev->securePort, OIC_RSRC_ACL_URI, OIC_JSON_SUBJECTID_NAME, subID);
    OICFree(subID);
    if (snRet < 0)
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteACLRequest : Error (snprintf) %d\n", snRet);
        return OC_STACK_ERROR;
    }
    else if ((size_t)snRet >= sizeof(reqBuf))
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteACLRequest : Truncated (snprintf) %d\n", snRet);
        return OC_STACK_ERROR;
    }

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(cbData));
    cbData.context = ctx;
    cbData.cb = respHandler;
    cbData.cd = NULL;
    OIC_LOG_V(INFO, TAG, "URI: %s",reqBuf);

    OIC_LOG(DEBUG, TAG, "Sending remove ACL request to resource server");

    ret = OCDoResource(NULL, OC_REST_DELETE, reqBuf,
                                     &destDev->endpoint, NULL,
                                     CT_ADAPTER_IP, OC_HIGH_QOS, &cbData, NULL, 0);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "SendDeleteACLRequest : Error in OCDoResource %d", ret);
    }
    OIC_LOG(DEBUG, TAG, "OUT SendDeleteACLRequest");

    return ret;
}

/**
 * Callback handler of unlink second device.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] handle          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction and
 *          OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPUnlinkDevice2CB(void *unlinkCtx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    (void) handle;
    OIC_LOG(DEBUG, TAG, "IN SRPUnlinkDevice2CB");
    VERIFY_NON_NULL_RET(TAG, unlinkCtx, ERROR, OC_STACK_DELETE_TRANSACTION);
    UnlinkData_t* unlinkData = (UnlinkData_t*)unlinkCtx;

    if (clientResponse)
    {
        OIC_LOG(DEBUG, TAG, "Valid client response for device 2");
        registerResultForUnlinkDevices(unlinkData, clientResponse->result, IDX_SECOND_DEVICE_RES);

        if (OC_STACK_RESOURCE_DELETED == clientResponse->result)
        {
            OIC_LOG(DEBUG, TAG, "Credential of device2 revoked");
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Unable to delete credential information from device 2");
            unlinkData->resultCallback(unlinkData->ctx,
                                       unlinkData->numOfResults, unlinkData->unlinkRes, true);
            goto error;
        }
    }
    else
    {
        registerResultForUnlinkDevices(unlinkData, OC_STACK_INVALID_REQUEST_HANDLE,
                                       IDX_SECOND_DEVICE_RES);
        unlinkData->resultCallback(unlinkData->ctx,
                                   unlinkData->numOfResults, unlinkData->unlinkRes, true);
        OIC_LOG(ERROR, TAG, "SRPUnlinkDevice2CB received Null clientResponse");
        goto error;
    }

    //Update provisioning DB when succes case.
    if (OC_STACK_OK != PDMUnlinkDevices(&unlinkData->unlinkDev[0].doxm->deviceID,
                                       &unlinkData->unlinkDev[1].doxm->deviceID))
    {
        OIC_LOG(FATAL, TAG, "All requests are successfully done but update provisioning DB FAILED.");
        registerResultForUnlinkDevices(unlinkData, OC_STACK_INCONSISTENT_DB, IDX_DB_UPDATE_RES);
        unlinkData->resultCallback(unlinkData->ctx,
                                   unlinkData->numOfResults, unlinkData->unlinkRes, true);
        goto error;
    }
    unlinkData->resultCallback(unlinkData->ctx, unlinkData->numOfResults, unlinkData->unlinkRes,
                               false);

error:
    DeleteUnlinkData_t(unlinkData);
    OIC_LOG(DEBUG, TAG, "OUT SRPUnlinkDevice2CB");
    return OC_STACK_DELETE_TRANSACTION;

}

/**
 * Callback handler of unlink first device.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] handle          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction and
 *          OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPUnlinkDevice1CB(void *unlinkCtx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "Inside SRPUnlinkDevice1CB ");
    VERIFY_NON_NULL_RET(TAG, unlinkCtx, ERROR, OC_STACK_DELETE_TRANSACTION);
    UnlinkData_t* unlinkData = (UnlinkData_t*)unlinkCtx;
    (void) handle;

    if (clientResponse)
    {
        OIC_LOG(DEBUG, TAG, "Valid client response for device 1");
        registerResultForUnlinkDevices(unlinkData, clientResponse->result, IDX_FIRST_DEVICE_RES);

        if (OC_STACK_RESOURCE_DELETED == clientResponse->result)
        {
            OIC_LOG(DEBUG, TAG, "Credential of device 1 is revoked");

            // Second revocation request to second device.
            OCStackResult res = SendDeleteCredentialRequest((void*)unlinkData, &SRPUnlinkDevice2CB,
                                                    &unlinkData->unlinkDev[0],
                                                    &unlinkData->unlinkDev[1] /*Dest*/);
            OIC_LOG_V(DEBUG, TAG, "Credential revocation request device 2, result :: %d",res);
            if (OC_STACK_OK != res)
            {
                 OIC_LOG(ERROR, TAG, "Error while sending revocation request for device 2");
                 registerResultForUnlinkDevices(unlinkData, OC_STACK_INVALID_REQUEST_HANDLE,
                                                IDX_SECOND_DEVICE_RES);
                 unlinkData->resultCallback(unlinkData->ctx,
                                            unlinkData->numOfResults, unlinkData->unlinkRes, true);
                 goto error;
            }
            else
            {
                OIC_LOG(DEBUG, TAG, "Request for credential revocation successfully sent");
                return OC_STACK_DELETE_TRANSACTION;
            }
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Unable to delete credential information from device 1");

            unlinkData->resultCallback(unlinkData->ctx, unlinkData->numOfResults,
                                            unlinkData->unlinkRes, true);
            goto error;
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Invalid response from server");
        registerResultForUnlinkDevices(unlinkData, OC_STACK_INVALID_REQUEST_HANDLE,
                                       IDX_FIRST_DEVICE_RES );
        unlinkData->resultCallback(unlinkData->ctx,
                                   unlinkData->numOfResults, unlinkData->unlinkRes,
                                   true);
        OIC_LOG(ERROR, TAG, "SRPUnlinkDevice1CB received Null clientResponse");
    }

error:
    OIC_LOG_V(INFO, TAG, "Out SRPUnlinkDevice1CB");
    DeleteUnlinkData_t(unlinkData);
    return OC_STACK_DELETE_TRANSACTION;
}

/*
* Function to unlink devices.
* This function will remove the credential & relationship between the two devices.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] pTargetDev1 first device information to be unlinked.
* @param[in] pTargetDev2 second device information to be unlinked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            device unlink is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
*/
OCStackResult SRPUnlinkDevices(void* ctx,
                               const OCProvisionDev_t* pTargetDev1,
                               const OCProvisionDev_t* pTargetDev2,
                               OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN SRPUnlinkDevices");

    if (!pTargetDev1 || !pTargetDev2 || !pTargetDev1->doxm || !pTargetDev2->doxm)
    {
        OIC_LOG(INFO, TAG, "SRPUnlinkDevices : NULL parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "SRPUnlinkDevices : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }
    if (0 == memcmp(&pTargetDev1->doxm->deviceID, &pTargetDev2->doxm->deviceID, sizeof(OicUuid_t)))
    {
        OIC_LOG(INFO, TAG, "SRPUnlinkDevices : Same device ID");
        return OC_STACK_INVALID_PARAM;
    }

    OIC_LOG(INFO, TAG, "Unlinking following devices: ");
    PMPrintOCProvisionDev(pTargetDev1);
    PMPrintOCProvisionDev(pTargetDev2);

    // Mark the link status stale
    OCStackResult res = PDMSetLinkStale(&pTargetDev1->doxm->deviceID, &pTargetDev2->doxm->deviceID);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(FATAL, TAG, "unable to update DB. Try again.");
        return res;
    }

    UnlinkData_t* unlinkData = (UnlinkData_t*)OICCalloc(1, sizeof(UnlinkData_t));
    VERIFY_NON_NULL_RET(TAG, unlinkData, ERROR, OC_STACK_NO_MEMORY);

    //Initialize unlink data
    unlinkData->ctx = ctx;
    unlinkData->unlinkDev = (OCProvisionDev_t*)OICCalloc(2, sizeof(OCProvisionDev_t));
    if (NULL == unlinkData->unlinkDev)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    unlinkData->unlinkRes = (OCProvisionResult_t*)OICCalloc(3, sizeof(OCProvisionResult_t));
    if (NULL == unlinkData->unlinkRes)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    memcpy(&unlinkData->unlinkDev[0], pTargetDev1, sizeof(OCProvisionDev_t));
    memcpy(&unlinkData->unlinkDev[1], pTargetDev2, sizeof(OCProvisionDev_t));

    unlinkData->numOfResults = 0;
    unlinkData->resultCallback = resultCallback;

    res = SendDeleteCredentialRequest((void*)unlinkData, &SRPUnlinkDevice1CB,
                                       &unlinkData->unlinkDev[1], &unlinkData->unlinkDev[0]);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPUnlinkDevices : SendDeleteCredentialRequest failed");
        goto error;
    }

    return res;

error:
    OIC_LOG(INFO, TAG, "OUT SRPUnlinkDevices");
    DeleteUnlinkData_t(unlinkData);
    return res;
}

static void DeleteRemoveData_t(RemoveData_t* pRemoveData)
{
    if (pRemoveData)
    {
        OICFree(pRemoveData->revokeTargetDev);
        OCDeleteDiscoveredDevices(pRemoveData->linkedDevList);
        OICFree(pRemoveData->removeRes);
        OICFree(pRemoveData);
    }
}

static void registerResultForRemoveDevice(RemoveData_t *removeData, OicUuid_t *pLinkedDevId,
                                          OCStackResult stackresult, bool hasError)
{
    OIC_LOG_V(INFO, TAG, "Inside registerResultForRemoveDevice removeData->numOfResults is %zu\n",
                         removeData->numOfResults + 1);
    if (pLinkedDevId)
    {
        memcpy(removeData->removeRes[(removeData->numOfResults)].deviceId.id,
               &pLinkedDevId->id, sizeof(pLinkedDevId->id));
    }
    else
    {
        memset(removeData->removeRes[(removeData->numOfResults)].deviceId.id,
               0, sizeof(pLinkedDevId->id) );
    }
    removeData->removeRes[(removeData->numOfResults)].res = stackresult;
    removeData->hasError = hasError;
    ++(removeData->numOfResults);

    // If we get suffcient result from linked devices, we have to call user callback and do free
    if (removeData->sizeOfResArray == removeData->numOfResults)
    {
        if(!removeData->hasError)
        {
            // Remove device info from prvisioning database
            if (OC_STACK_OK != PDMDeleteDevice(&removeData->revokeTargetDev->doxm->deviceID))
            {
                OIC_LOG(ERROR, TAG, "ResultForRemoveDevice : Failed to remove device in PDM.");
                removeData->hasError = true;
            }
        }
        removeData->resultCallback(removeData->ctx, removeData->numOfResults, removeData->removeRes,
                                   removeData->hasError);
        DeleteRemoveData_t(removeData);
    }
 }

static void registerResultForResetDevice(RemoveData_t *removeData, OicUuid_t *pLinkedDevId,
                                          OCStackResult stackresult, bool hasError)
{
    OIC_LOG_V(INFO, TAG, "Inside registerResultForResetDevice removeData->numOfResults is %zu\n",
                         removeData->numOfResults + 1);
    if (pLinkedDevId)
    {
        memcpy(removeData->removeRes[(removeData->numOfResults)].deviceId.id,
               &pLinkedDevId->id, sizeof(pLinkedDevId->id));
    }
    else
    {
        memset(removeData->removeRes[(removeData->numOfResults)].deviceId.id,
               0, sizeof(pLinkedDevId->id) );
    }
    removeData->removeRes[(removeData->numOfResults)].res = stackresult;
    removeData->hasError = hasError;
    ++(removeData->numOfResults);

    // If we get suffcient result from linked devices, we have to call user callback and do free
    if (removeData->sizeOfResArray == removeData->numOfResults)
    {
        removeData->resultCallback(removeData->ctx, removeData->numOfResults, removeData->removeRes,
                                   removeData->hasError);
        DeleteRemoveData_t(removeData);
    }
}

/**
 * Callback handler of unlink first device.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] handle          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPRemoveDeviceCB(void *delDevCtx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    //Update the delete credential into delete device context
    //Save the deleted status in delDevCtx
    (void)handle;
    OIC_LOG_V(INFO, TAG, "Inside SRPRemoveDeviceCB.");
    VERIFY_NON_NULL_RET(TAG, delDevCtx, ERROR, OC_STACK_DELETE_TRANSACTION);
    OCStackResult res = OC_STACK_ERROR;

    RemoveData_t* removeData = (RemoveData_t*)delDevCtx;

    if (clientResponse)
    {
        OicUuid_t revDevUuid = {.id={0}};
        if(UUID_LENGTH == clientResponse->identity.id_length)
        {
            memcpy(revDevUuid.id, clientResponse->identity.id, sizeof(revDevUuid.id));
            if (OC_STACK_RESOURCE_DELETED == clientResponse->result)
            {
                res = PDMUnlinkDevices(&removeData->revokeTargetDev->doxm->deviceID, &revDevUuid);
                if (OC_STACK_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "PDMSetLinkStale() FAIL: PDB is an obsolete one.");
                           registerResultForRemoveDevice(removeData, &revDevUuid,
                           OC_STACK_INCONSISTENT_DB, true);

                    return OC_STACK_DELETE_TRANSACTION;
                }

                registerResultForRemoveDevice(removeData, &revDevUuid,
                                              OC_STACK_RESOURCE_DELETED, false);
            }
            else
            {
                registerResultForRemoveDevice(removeData, &revDevUuid,
                                              clientResponse->result, true);
                OIC_LOG(ERROR, TAG, "Unexpected result from DELETE credential request!");
            }
        }
        else
        {
            OIC_LOG_V(WARNING, TAG, "Incorrect length of device UUID was sent from %s:%d",
                     clientResponse->devAddr.addr, clientResponse->devAddr.port);

            if (OC_STACK_RESOURCE_DELETED == clientResponse->result)
            {
                /**
                  * Since server's credential was deleted,
                  * register result as OC_STACK_INCONSISTENT_DB with NULL UUID.
                  */
                OIC_LOG_V(ERROR, TAG, "But server's credential was deleted.");
                registerResultForRemoveDevice(removeData, NULL, OC_STACK_INCONSISTENT_DB, true);
            }
            else
            {
                registerResultForRemoveDevice(removeData, NULL, clientResponse->result, true);
            }
        }
    }
    else
    {
        registerResultForRemoveDevice(removeData, NULL, OC_STACK_ERROR, true);
        OIC_LOG(ERROR, TAG, "SRPRemoveDevices received Null clientResponse");
    }


    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Callback handler of reset device.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] handle          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPSyncDeviceCredCB(void *delDevCtx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    //Update the delete credential into delete device context
    //Save the deleted status in delDevCtx
    (void)handle;
    OIC_LOG_V(INFO, TAG, "Inside SRPSyncDeviceCredCB.");
    VERIFY_NON_NULL_RET(TAG, delDevCtx, ERROR, OC_STACK_DELETE_TRANSACTION);
    OCStackResult res = OC_STACK_ERROR;

    RemoveData_t* removeData = (RemoveData_t*)delDevCtx;
    OCProvisionDev_t * pTargetDev = PMCloneOCProvisionDev(removeData->revokeTargetDev);
    OCProvisionResultCB resultCallback = removeData->resultCallback;
    if (clientResponse)
    {
        OicUuid_t revDevUuid = {.id={0}};
        if(UUID_LENGTH == clientResponse->identity.id_length)
        {
            memcpy(revDevUuid.id, clientResponse->identity.id, sizeof(revDevUuid.id));
            if (OC_STACK_RESOURCE_DELETED == clientResponse->result)
            {
                res = PDMUnlinkDevices(&removeData->revokeTargetDev->doxm->deviceID, &revDevUuid);
                if (OC_STACK_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "PDMSetLinkStale() FAIL: PDB is an obsolete one.");
                           registerResultForResetDevice(removeData, &revDevUuid,
                           OC_STACK_INCONSISTENT_DB, true);

                    return OC_STACK_DELETE_TRANSACTION;
                }

                registerResultForResetDevice(removeData, &revDevUuid,
                                              OC_STACK_RESOURCE_DELETED, false);
            }
            else
            {
                registerResultForResetDevice(removeData, &revDevUuid,
                                              clientResponse->result, false);
                OIC_LOG(ERROR, TAG, "Unexpected result from DELETE credential request!");
            }
        }
        else
        {
            OIC_LOG_V(WARNING, TAG, "Incorrect length of device UUID was sent from %s:%d",
                     clientResponse->devAddr.addr, clientResponse->devAddr.port);

            if (OC_STACK_RESOURCE_DELETED == clientResponse->result)
            {
                /**
                  * Since server's credential was deleted,
                  * register result as OC_STACK_INCONSISTENT_DB with NULL UUID.
                  */
                OIC_LOG_V(ERROR, TAG, "But server's credential was deleted.");
                registerResultForResetDevice(removeData, NULL, OC_STACK_INCONSISTENT_DB, true);
            }
            else
            {
                registerResultForResetDevice(removeData, NULL, clientResponse->result, true);
            }
        }
    }
    else
    {
        registerResultForResetDevice(removeData, NULL, OC_STACK_ERROR, true);
        OIC_LOG(ERROR, TAG, "SRPSyncDevice received Null clientResponse");
    }

    SRPResetDevice(pTargetDev, resultCallback);

    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Callback handler of reset device sync-up
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] handle          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPSyncDeviceACLCB(void *ctx, OCDoHandle UNUSED,
        OCClientResponse *clientResponse)
{
    (void)ctx;
    (void)UNUSED;
    (void)clientResponse;
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Callback handler of device remote reset.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPResetDeviceCB(void *ctx, OCDoHandle UNUSED,
        OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "IN SRPResetDeviceCB");
    (void)UNUSED;
    if(OC_STACK_OK == clientResponse->result)
    {
        OIC_LOG(DEBUG, TAG, "Change Target Device Pstat Cm SUCCEEDED");
    }

    // Delete Cred and ACL related to the target device.
    const OicSecCred_t *cred = NULL;
    OCProvisionDev_t * pTargetDev = (OCProvisionDev_t *)ctx;
    cred = GetCredResourceData(&pTargetDev->doxm->deviceID);
    if (cred == NULL)
    {
        OIC_LOG(ERROR, TAG, "OCResetDevice : Failed to get credential of target device.");
        goto error;
    }

    OCStackResult res = RemoveCredential(&cred->subject);
    if (res != OC_STACK_RESOURCE_DELETED && res != OC_STACK_NO_RESOURCE)
    {
        OIC_LOG(ERROR, TAG, "OCResetDevice : Failed to remove credential.");
        goto error;
    }

    res = RemoveACE(&cred->subject, NULL);
    if (res != OC_STACK_RESOURCE_DELETED && res != OC_STACK_NO_RESOURCE)
    {
        OIC_LOG(ERROR, TAG, "OCResetDevice : Failed to remove ACL.");
        goto error;
    }
    if (OC_STACK_OK != PDMDeleteDevice(&pTargetDev->doxm->deviceID))
    {
        OIC_LOG(ERROR, TAG, "OCResetDevice : Failed to delete device from PDM");
    }

    //Close the DTLS session of the reset device.
    CAEndpoint_t* endpoint = (CAEndpoint_t *)&clientResponse->devAddr;
    CAResult_t caResult = CAcloseSslSession(endpoint);
    if(CA_STATUS_OK != caResult)
    {
        OIC_LOG_V(WARNING, TAG, "OCResetDevice : Failed to close DTLS session : %d", caResult);
    }

    /**
     * If there is no linked device, PM does not send any request.
     * So we should directly invoke the result callback to inform the result of OCResetDevice.
     */
    if(OC_STACK_NO_RESOURCE == res)
    {
        res = OC_STACK_OK;
    }

error:
    OICFree(pTargetDev);
    return OC_STACK_DELETE_TRANSACTION;

}

static OCStackResult GetListofDevToReqDeleteCred(const OCProvisionDev_t* pRevokeTargetDev,
                                                 const OCProvisionDev_t* pOwnedDevList,
                                                 OCUuidList_t* pLinkedUuidList,
                                                 OCProvisionDev_t** ppLinkedDevList,
                                                 size_t *numOfLinkedDev)
{
    // pOwnedDevList could be NULL. It means no alived and owned device now.
    if (pRevokeTargetDev == NULL || pLinkedUuidList == NULL ||\
        ppLinkedDevList == NULL || numOfLinkedDev == NULL)
    {
        return OC_STACK_INVALID_PARAM;
    }

    size_t cnt = 0;
    OCUuidList_t *curUuid = NULL, *tmpUuid = NULL;
    LL_FOREACH_SAFE(pLinkedUuidList, curUuid, tmpUuid)
    {
        // Mark the link status stale.
        OCStackResult res = PDMSetLinkStale(&curUuid->dev, &pRevokeTargetDev->doxm->deviceID);
        if (OC_STACK_OK != res)
        {
            OIC_LOG(FATAL, TAG, "PDMSetLinkStale() FAIL: PDB is an obsolete one.");
            return OC_STACK_SVR_DB_NOT_EXIST;
        }

        if (pOwnedDevList)
        {
            // If this linked device is alive (power-on), add the deivce to the list.
            const OCProvisionDev_t *curDev = NULL;
            const OCProvisionDev_t *tmpDev = NULL;
            LL_FOREACH_SAFE(pOwnedDevList, curDev, tmpDev)
            {
                if (memcmp(curDev->doxm->deviceID.id, curUuid->dev.id, sizeof(curUuid->dev.id)) == 0)
                {
                    OCProvisionDev_t* targetDev = PMCloneOCProvisionDev(curDev);
                    if (NULL == targetDev)
                    {
                        OIC_LOG(ERROR, TAG, "SRPRemoveDevice : Cloning OCProvisionDev_t Failed.");
                        return OC_STACK_NO_MEMORY;
                    }

                    LL_PREPEND(*ppLinkedDevList, targetDev);
                    cnt++;
                    break;
                }
            }
        }
    }
    *numOfLinkedDev = cnt;
    return OC_STACK_OK;
}

/*
* Function to device revocation
* This function will remove credential of target device from all devices in subnet.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
* @param[in] pTargetDev Device information to be revoked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            credential revocation is finished.
* @return  OC_STACK_OK in case of success and other value otherwise.
*          If OC_STACK_OK is returned, the caller of this API should wait for callback.
*          OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
*/
OCStackResult SRPRemoveDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                             const OCProvisionDev_t* pTargetDev, OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN SRPRemoveDevice");

    if (!pTargetDev  || 0 == waitTimeForOwnedDeviceDiscovery)
    {
        OIC_LOG(INFO, TAG, "SRPRemoveDevice : NULL parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "SRPRemoveDevice : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    // Declare variables in here to handle error cases with goto statement.
    OCProvisionDev_t* pOwnedDevList = NULL;
    OCProvisionDev_t* pLinkedDevList = NULL;
    RemoveData_t* removeData = NULL;

    //1. Find all devices that has a credential of the revoked device
    OCUuidList_t* pLinkedUuidList = NULL;
    size_t numOfDevices = 0;
    OCStackResult res = OC_STACK_ERROR;
    res = PDMGetLinkedDevices(&pTargetDev->doxm->deviceID, &pLinkedUuidList, &numOfDevices);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDevice : Failed to get linked devices information");
        return res;
    }
    // if there is no related device, we can skip further process.
    if (0 == numOfDevices)
    {
        OIC_LOG(DEBUG, TAG, "SRPRemoveDevice : No linked device found.");
        res = OC_STACK_CONTINUE;
        goto error;
    }

    //2. Find owned device from the network
    res = PMDeviceDiscovery(waitTimeForOwnedDeviceDiscovery, true, &pOwnedDevList);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDevice : Failed to PMDeviceDiscovery");
        goto error;
    }

    //3. Make a list of devices to send DELETE credential request
    //   by comparing owned devices from provisioning database with mutlicast discovery result.
    size_t numOfLinkedDev = 0;
    res = GetListofDevToReqDeleteCred(pTargetDev, pOwnedDevList, pLinkedUuidList,
                                      &pLinkedDevList, &numOfLinkedDev);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDevice : GetListofDevToReqDeleteCred() failed");
        goto error;
    }
    if (0 == numOfLinkedDev) // This case means, there is linked device but it's not alive now.
    {                       // So we don't have to send request message.
        OIC_LOG(DEBUG, TAG, "SRPRemoveDevice : No alived & linked device found.");
        res = OC_STACK_CONTINUE;
        goto error;
    }

    // 4. Prepare RemoveData Context data.
    removeData = (RemoveData_t*)OICCalloc(1, sizeof(RemoveData_t));
    if (!removeData)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDevices : Failed to memory allocation");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->revokeTargetDev = PMCloneOCProvisionDev(pTargetDev);
    if (!removeData->revokeTargetDev)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDevices : PMCloneOCProvisionDev Failed");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->removeRes =
        (OCProvisionResult_t*)OICCalloc(numOfLinkedDev, sizeof(OCProvisionResult_t));
    if (!removeData->removeRes)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDevices : Failed to memory allocation");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->ctx = ctx;
    removeData->linkedDevList = pLinkedDevList;
    removeData->resultCallback = resultCallback;
    removeData->numOfResults = 0;
    removeData->sizeOfResArray = numOfLinkedDev;
    removeData->hasError = false;

    // 5. Send DELETE credential request to linked devices.
    OCProvisionDev_t *curDev = NULL, *tmpDev = NULL;
    OCStackResult totalRes = OC_STACK_ERROR;  /* variable for checking request is sent or not */
    LL_FOREACH_SAFE(pLinkedDevList, curDev, tmpDev)
    {
        res = SendDeleteCredentialRequest((void*)removeData, &SRPRemoveDeviceCB,
                                           removeData->revokeTargetDev, curDev);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "SRPRemoveDevice : Fail to send the DELETE credential request to\
                     %s:%u", curDev->endpoint.addr, curDev->endpoint.port);
        }
        else
        {
            totalRes = OC_STACK_OK; // This means at least one request is successfully sent.
        }
    }

    PDMDestoryOicUuidLinkList(pLinkedUuidList); //TODO: Modify API name to have unified convention.
    PMDeleteDeviceList(pOwnedDevList);
    OIC_LOG(INFO, TAG, "OUT SRPRemoveDevice");

    return totalRes; // Caller of this API should wait callback if totalRes == OC_STACK_OK.

error:
    PDMDestoryOicUuidLinkList(pLinkedUuidList);
    PMDeleteDeviceList(pOwnedDevList);
    PMDeleteDeviceList(pLinkedDevList);
    if (removeData)
    {
        OICFree(removeData->revokeTargetDev);
        OICFree(removeData->removeRes);
        OICFree(removeData);
    }
    OIC_LOG(INFO, TAG, "OUT ERROR case SRPRemoveDevice");
    return res;
}

/*
* Function to device revocation
* This function will remove credential of target device from all devices in subnet.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] pOwnedDevList List of owned devices
* @param[in] pTargetDev Device information to be revoked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            credential revocation is finished.
* @return  OC_STACK_OK in case of success and other value otherwise.
*          If OC_STACK_OK is returned, the caller of this API should wait for callback.
*          OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
*/
OCStackResult SRPRemoveDeviceWithoutDiscovery(void* ctx, const OCProvisionDev_t* pOwnedDevList,
                             const OCProvisionDev_t* pTargetDev, OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN SRPRemoveDeviceWithoutDiscovery");

    if (!pOwnedDevList)
    {
        OIC_LOG(WARNING, TAG, "SRPRemoveDeviceWithoutDiscovery : Owned Device List is empty");
        return OC_STACK_CONTINUE;
    }
    if (!pTargetDev)
    {
        OIC_LOG(INFO, TAG, "SRPRemoveDeviceWithoutDiscovery : NULL parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "SRPRemoveDeviceWithoutDiscovery : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    // Declare variables in here to handle error cases with goto statement.
    OCProvisionDev_t* pLinkedDevList = NULL;
    RemoveData_t* removeData = NULL;

    //1. Find all devices that has a credential of the revoked device
    OCUuidList_t* pLinkedUuidList = NULL;
    size_t numOfDevices = 0;
    OCStackResult res = OC_STACK_ERROR;
    res = PDMGetLinkedDevices(&pTargetDev->doxm->deviceID, &pLinkedUuidList, &numOfDevices);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(WARNING, TAG, "SRPRemoveDeviceWithoutDiscovery : Failed to get linked devices information");
        return OC_STACK_CONTINUE;
    }
    // if there is no related device, we can skip further process.
    if (0 == numOfDevices)
    {
        OIC_LOG(WARNING, TAG, "SRPRemoveDeviceWithoutDiscovery : No linked device found.");
        return OC_STACK_CONTINUE;
    }

    //2. Make a list of devices to send DELETE credential request
    //   by comparing owned devices from provisioning database with mutlicast discovery result.
    size_t numOfLinkedDev = 0;
    res = GetListofDevToReqDeleteCred(pTargetDev, pOwnedDevList, pLinkedUuidList,
                                      &pLinkedDevList, &numOfLinkedDev);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDeviceWithoutDiscovery : GetListofDevToReqDeleteCred() failed");
        goto error;
    }
    if (0 == numOfLinkedDev) // This case means, there is linked device but it's not alive now.
    {                       // So we don't have to send request message.
        OIC_LOG(DEBUG, TAG, "SRPRemoveDeviceWithoutDiscovery : No alived & linked device found.");
        return OC_STACK_CONTINUE;
    }

    // 3. Prepare RemoveData Context data.
    removeData = (RemoveData_t*)OICCalloc(1, sizeof(RemoveData_t));
    if (!removeData)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDeviceWithoutDiscovery : Failed to memory allocation");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->revokeTargetDev = PMCloneOCProvisionDev(pTargetDev);
    if (!removeData->revokeTargetDev)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDeviceWithoutDiscovery : PMCloneOCProvisionDev Failed");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->removeRes =
        (OCProvisionResult_t*)OICCalloc(numOfLinkedDev, sizeof(OCProvisionResult_t));
    if (!removeData->removeRes)
    {
        OIC_LOG(ERROR, TAG, "SRPRemoveDeviceWithoutDiscovery : Failed to memory allocation");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->ctx = ctx;
    removeData->linkedDevList = pLinkedDevList;
    removeData->resultCallback = resultCallback;
    removeData->numOfResults = 0;
    removeData->sizeOfResArray = numOfLinkedDev;
    removeData->hasError = false;

    // 5. Send DELETE credential request to linked devices.
    OCProvisionDev_t *curDev = NULL, *tmpDev = NULL;
    OCStackResult totalRes = OC_STACK_ERROR;  /* variable for checking request is sent or not */
    LL_FOREACH_SAFE(pLinkedDevList, curDev, tmpDev)
    {
        res = SendDeleteCredentialRequest((void*)removeData, &SRPRemoveDeviceCB,
                                           removeData->revokeTargetDev, curDev);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "SRPRemoveDeviceWithoutDiscovery : Fail to send the DELETE credential request to\
                     %s:%u", curDev->endpoint.addr, curDev->endpoint.port);
        }
        else
        {
            totalRes = OC_STACK_OK; // This means at least one request is successfully sent.
        }
    }

    PDMDestoryOicUuidLinkList(pLinkedUuidList); //TODO: Modify API name to have unified convention.
    OIC_LOG(INFO, TAG, "OUT SRPRemoveDeviceWithoutDiscovery");

    return totalRes; // Caller of this API should wait callback if totalRes == OC_STACK_OK.

error:
    PDMDestoryOicUuidLinkList(pLinkedUuidList);
    PMDeleteDeviceList(pLinkedDevList);
    if (removeData)
    {
        OICFree(removeData->revokeTargetDev);
        OICFree(removeData->removeRes);
        OICFree(removeData);
    }
    OIC_LOG(INFO, TAG, "OUT ERROR case SRPRemoveDeviceWithoutDiscovery");
    return res;
}

/*
 * Function to sync-up credential and ACL of the target device.
 * This function will remove credential and ACL of target device from all devices in subnet.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 *            when there is an error, this user callback is called immediately.
 * @return OC_STACK_OK in case of success and other value otherwise.
 *         If OC_STACK_OK is returned, the caller of this API should wait for callback.
 *         OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
 */
OCStackResult SRPSyncDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                         const OCProvisionDev_t* pTargetDev, OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN SRPSyncDevice");
    if (!pTargetDev  || 0 == waitTimeForOwnedDeviceDiscovery)
    {
        OIC_LOG(INFO, TAG, "SRPSyncDevice : NULL parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "SRPSyncDevice : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    // Declare variables in here to handle error cases with goto statement.
    OCProvisionDev_t* pOwnedDevList = NULL;
    OCProvisionDev_t* pLinkedDevList = NULL;
    RemoveData_t* removeData = NULL;

    //1. Find all devices that has a credential of the revoked device
    OCUuidList_t* pLinkedUuidList = NULL;
    size_t numOfDevices = 0;
    OCStackResult res = OC_STACK_ERROR;
    res = PDMGetLinkedDevices(&pTargetDev->doxm->deviceID, &pLinkedUuidList, &numOfDevices);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPSyncDevice : Failed to get linked devices information");
        return res;
    }
    // if there is no related device, we can skip further process.
    if (0 == numOfDevices)
    {
        OIC_LOG(DEBUG, TAG, "SRPSyncDevice : No linked device found.");
        res = OC_STACK_CONTINUE;
        goto error;
    }

    //2. Find owned device from the network
    res = PMDeviceDiscovery(waitTimeForOwnedDeviceDiscovery, true, &pOwnedDevList);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPSyncDevice : Failed to PMDeviceDiscovery");
        goto error;
    }

    //3. Make a list of devices to send DELETE credential request
    //   by comparing owned devices from provisioning database with mutlicast discovery result.
    size_t numOfLinkedDev = 0;
    res = GetListofDevToReqDeleteCred(pTargetDev, pOwnedDevList, pLinkedUuidList,
                                      &pLinkedDevList, &numOfLinkedDev);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "SRPSyncDevice : GetListofDevToReqDeleteCred() failed");
        goto error;
    }
    if (0 == numOfLinkedDev) // This case means, there is linked device but it's not alive now.
    {                       // So we don't have to send request message.
        OIC_LOG(DEBUG, TAG, "SRPSyncDevice : No alived & linked device found.");
        res = OC_STACK_CONTINUE;
        goto error;
    }

    // 4. Prepare RemoveData Context data.
    removeData = (RemoveData_t*)OICCalloc(1, sizeof(RemoveData_t));
    if (!removeData)
    {
        OIC_LOG(ERROR, TAG, "SRPSyncDevice : Failed to memory allocation");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->revokeTargetDev = PMCloneOCProvisionDev(pTargetDev);
    if (!removeData->revokeTargetDev)
    {
        OIC_LOG(ERROR, TAG, "SRPSyncDevice : PMCloneOCProvisionDev Failed");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->removeRes =
        (OCProvisionResult_t*)OICCalloc(numOfLinkedDev, sizeof(OCProvisionResult_t));
    if (!removeData->removeRes)
    {
        OIC_LOG(ERROR, TAG, "SRPSyncDevice : Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    removeData->ctx = ctx;
    removeData->linkedDevList = pLinkedDevList;
    removeData->resultCallback = resultCallback;
    removeData->numOfResults = 0;
    removeData->sizeOfResArray = numOfLinkedDev;
    removeData->hasError = false;

    // 5. Send DELETE credential request to linked devices.
    OCProvisionDev_t *curDev = NULL, *tmpDev = NULL;
    OCStackResult totalRes = OC_STACK_ERROR;  /* variable for checking request is sent or not */
    LL_FOREACH_SAFE(pLinkedDevList, curDev, tmpDev)
    {
        res = SendDeleteACLRequest((void*)removeData, &SRPSyncDeviceACLCB,
                                           removeData->revokeTargetDev, curDev);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "SRPSyncDevice : Fail to send the DELETE ACL request to\
                     %s:%u", curDev->endpoint.addr, curDev->endpoint.port);
            goto error;
        }
        res = SendDeleteCredentialRequest((void*)removeData, &SRPSyncDeviceCredCB,
                                           removeData->revokeTargetDev, curDev);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "SRPSyncDevice : Fail to send the DELETE credential request to\
                     %s:%u", curDev->endpoint.addr, curDev->endpoint.port);
            totalRes = OC_STACK_ERROR;
        }
        else
        {
            totalRes = OC_STACK_OK; // This means at least one request is successfully sent.
        }
    }

    PDMDestoryOicUuidLinkList(pLinkedUuidList); //TODO: Modify API name to have unified convention.
    PMDeleteDeviceList(pOwnedDevList);
    OIC_LOG(INFO, TAG, "OUT SRPSyncDevice");

    return totalRes; // Caller of this API should wait callback if totalRes == OC_STACK_OK.

error:
    PDMDestoryOicUuidLinkList(pLinkedUuidList);
    PMDeleteDeviceList(pOwnedDevList);
    PMDeleteDeviceList(pLinkedDevList);
    if (removeData)
    {
        OICFree(removeData->revokeTargetDev);
        OICFree(removeData->removeRes);
        OICFree(removeData);
    }
    OIC_LOG(INFO, TAG, "OUT ERROR case SRPSyncDevice");
    return res;
}

/*
 * Function for remote reset
 * This function will send pstat PUT message to the target device to initiate remote reset.
 *
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 *            when there is an error, this user callback is called immediately.
 * @return OC_STACK_OK in case of success and other value otherwise.
 *         If OC_STACK_OK is returned, the caller of this API should wait for callback.
 *         OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
 */
OCStackResult SRPResetDevice(const OCProvisionDev_t* pTargetDev,
        OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN SRPResetDevice");
    if (!pTargetDev)
    {
        OIC_LOG(INFO, TAG, "SRPResetDevice : NULL parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "SRPResetDevice : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    OCStackResult res = OC_STACK_ERROR;
    OicSecPstat_t * pstat = (OicSecPstat_t *) OICCalloc(1, sizeof(OicSecPstat_t));
    if (!pstat)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory");
        return OC_STACK_NO_MEMORY;
    }

    pstat->cm = RESET;
    pstat->isOp = false;
    memcpy(pstat->deviceID.id, pTargetDev->doxm->deviceID.id, sizeof(OicUuid_t));
    pstat->tm = TAKE_OWNER;
    pstat->om = (OicSecDpom_t)(SINGLE_SERVICE_CLIENT_DRIVEN); // the only mode IoTivity supports currently
    pstat->smLen = 1;
    pstat->sm = (OicSecDpom_t *) OICCalloc(pstat->smLen, sizeof(OicSecDpom_t));
    if (NULL == pstat->sm)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory");
        OICFree(pstat);
        return OC_STACK_NO_MEMORY;
    }
    pstat->sm[0] = (OicSecDpom_t)(SINGLE_SERVICE_CLIENT_DRIVEN); // the only mode IoTivity supports currently

    OCSecurityPayload * secPayload = (OCSecurityPayload *) OICCalloc(1, sizeof(OCSecurityPayload));
    if (!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    if (OC_STACK_OK != PstatToCBORPayload(pstat, &(secPayload->securityData),
                &(secPayload->payloadSize), true))
    {
        OCPayloadDestroy((OCPayload *) secPayload);
        OIC_LOG(ERROR, TAG, "Failed to PstatToCBORPayload");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    OIC_LOG(DEBUG, TAG, "Created payload for pstat set");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if (!PMGenerateQuery(true,
                pTargetDev->endpoint.addr,
                pTargetDev->securePort,
                pTargetDev->connType,
                query, sizeof(query), OIC_RSRC_PSTAT_URI))
    {
        OIC_LOG(ERROR, TAG, "SRPResetDevice : Failed to generate query");
        OCPayloadDestroy((OCPayload *) secPayload);
        res = OC_STACK_ERROR;
        goto error;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData = { .context = NULL, .cb = NULL, .cd = NULL };
    OCMethod method = OC_REST_POST;
    OCDoHandle handle = NULL;
    OCProvisionDev_t * targetDev = PMCloneOCProvisionDev(pTargetDev);
    if (NULL == targetDev)
    {
        OIC_LOG(ERROR, TAG, "target dev is null");
        res = OC_STACK_ERROR;
        goto error;
    }
    cbData.cb = &SRPResetDeviceCB;
    cbData.context = (void *) targetDev;
    cbData.cd = NULL;
    OIC_LOG(DEBUG, TAG, "Sending PSTAT info to resource server");
    res = OCDoResource(&handle, method, query,
            &targetDev->endpoint, (OCPayload *)secPayload,
            targetDev->connType, OC_LOW_QOS, &cbData, NULL, 0);\
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

error:
    OICFree(pstat->sm);
    OICFree(pstat);
    OIC_LOG(INFO, TAG, "OUT SRPResetDevice");
    return res;
}

/**
 * Internal Function to store results in result array during GetCredResourceCB.
 */
static void registerResultForGetCredResourceCB(GetSecData_t *GetSecData,
                                             OCStackResult stackresult)
{
   OIC_LOG_V(INFO, TAG, "Inside registerResultForGetCredResourceCB "
           "GetSecData->numOfResults is %d\n", GetSecData->numOfResults);
   memcpy(GetSecData->resArr[(GetSecData->numOfResults)].deviceId.id,
          GetSecData->deviceInfo->doxm->deviceID.id, UUID_LENGTH);
   GetSecData->resArr[(GetSecData->numOfResults)].res = stackresult;
   ++(GetSecData->numOfResults);
}

/**
 * Callback handler of SRPGetCredResource.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPGetCredResourceCB(void *ctx, OCDoHandle UNUSED,
                                                  OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "Inside SRPGetCredResourceCB.");
    (void)UNUSED;
    VERIFY_NON_NULL_RET(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    GetSecData_t *GetSecData = (GetSecData_t*)ctx;
    OCProvisionResultCB resultCallback = GetSecData->resultCallback;

    if (clientResponse)
    {
        if(OC_STACK_OK == clientResponse->result)
        {
            uint8_t *payload = ((OCSecurityPayload*)clientResponse->payload)->securityData;
            size_t size = ((OCSecurityPayload*)clientResponse->payload)->payloadSize;

            OIC_LOG_BUFFER(DEBUG, TAG, payload, size);
            (void)size;
            (void)payload;

            registerResultForGetCredResourceCB(GetSecData, OC_STACK_OK);
            ((OCProvisionResultCB)(resultCallback))(GetSecData->ctx, GetSecData->numOfResults,
                                                    GetSecData->resArr,
                                                    false);
             OICFree(GetSecData->resArr);
             OICFree(GetSecData);

            return OC_STACK_DELETE_TRANSACTION;
        }
    }
    registerResultForGetCredResourceCB(GetSecData, OC_STACK_OK);
    ((OCProvisionResultCB)(resultCallback))(GetSecData->ctx, GetSecData->numOfResults,
                                            GetSecData->resArr,
                                            false);
    OIC_LOG_V(ERROR, TAG, "SRPGetCredResourceCB received Null clientResponse");
    OICFree(GetSecData->resArr);
    OICFree(GetSecData);

    return OC_STACK_DELETE_TRANSACTION;
}


OCStackResult SRPGetCredResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
        OCProvisionResultCB resultCallback)
{
    VERIFY_NON_NULL_RET(TAG, selectedDeviceInfo, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, resultCallback, ERROR,  OC_STACK_INVALID_CALLBACK);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        selectedDeviceInfo->endpoint.addr,
                        selectedDeviceInfo->securePort,
                        selectedDeviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_CRED_URI))
    {
        OIC_LOG(ERROR, TAG, "SRPGetCredResource : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &SRPGetCredResourceCB;
    GetSecData_t* GetSecData = (GetSecData_t*)OICCalloc(1, sizeof(GetSecData_t));
    if (NULL == GetSecData)
    {
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    GetSecData->deviceInfo = selectedDeviceInfo;
    GetSecData->resultCallback = resultCallback;
    GetSecData->numOfResults=0;
    GetSecData->ctx = ctx;

    int noOfRiCalls = 1;
    GetSecData->resArr = (OCProvisionResult_t*)OICCalloc(noOfRiCalls, sizeof(OCProvisionResult_t));
    if (NULL == GetSecData->resArr)
    {
        OICFree(GetSecData);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    cbData.context = (void *)GetSecData;
    cbData.cd = NULL;
    OCMethod method = OC_REST_GET;
    OCDoHandle handle = NULL;
    OIC_LOG(DEBUG, TAG, "Sending Get Cred to  resource server");
    OCStackResult ret = OCDoResource(&handle, method, query, NULL, NULL,
            selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
        OICFree(GetSecData->resArr);
        OICFree(GetSecData);
    }
    VERIFY_SUCCESS_RET(TAG, (OC_STACK_OK == ret), ERROR, OC_STACK_ERROR);
    OIC_LOG(DEBUG, TAG, "OUT SRPGetCredResource");

    return OC_STACK_OK;
}

/**
 * Internal Function to store results in result array during GetACLResourceCB.
 */
static void registerResultForGetACLResourceCB(GetSecData_t *GetSecData,
                                             OCStackResult stackresult)
{
   OIC_LOG_V(INFO, TAG, "Inside registerResultForGetACLResourceCB "
           "GetSecData->numOfResults is %d\n", GetSecData->numOfResults);
   memcpy(GetSecData->resArr[(GetSecData->numOfResults)].deviceId.id,
          GetSecData->deviceInfo->doxm->deviceID.id, UUID_LENGTH);
   GetSecData->resArr[(GetSecData->numOfResults)].res = stackresult;
   ++(GetSecData->numOfResults);
}

/**
 * Callback handler of SRPGetACLResource.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult SRPGetACLResourceCB(void *ctx, OCDoHandle UNUSED,
                                                  OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "Inside SRPGetACLResourceCB.");
    (void)UNUSED;
    VERIFY_NON_NULL_RET(TAG, ctx, ERROR, OC_STACK_DELETE_TRANSACTION);
    GetSecData_t *GetSecData = (GetSecData_t*)ctx;
    OCProvisionResultCB resultCallback = GetSecData->resultCallback;

    if (clientResponse)
    {
        if(OC_STACK_OK == clientResponse->result)
        {
            uint8_t *payload = ((OCSecurityPayload*)clientResponse->payload)->securityData;
            size_t size = ((OCSecurityPayload*)clientResponse->payload)->payloadSize;

            OIC_LOG_BUFFER(DEBUG, TAG, payload, size);
            (void)payload;
            (void)size;

            registerResultForGetACLResourceCB(GetSecData, OC_STACK_OK);
            ((OCProvisionResultCB)(resultCallback))(GetSecData->ctx, GetSecData->numOfResults,
                                                    GetSecData->resArr,
                                                    false);
             OICFree(GetSecData->resArr);
             OICFree(GetSecData);

            return OC_STACK_DELETE_TRANSACTION;
        }
    }
    registerResultForGetACLResourceCB(GetSecData, OC_STACK_OK);
    ((OCProvisionResultCB)(resultCallback))(GetSecData->ctx, GetSecData->numOfResults,
                                            GetSecData->resArr,
                                            false);
    OIC_LOG_V(ERROR, TAG, "SRPGetACLResourceCB received Null clientResponse");
    OICFree(GetSecData->resArr);
    OICFree(GetSecData);

    return OC_STACK_DELETE_TRANSACTION;
}


OCStackResult SRPGetACLResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
        OCProvisionResultCB resultCallback)
{
    VERIFY_NON_NULL_RET(TAG, selectedDeviceInfo, ERROR,  OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(TAG, resultCallback, ERROR,  OC_STACK_INVALID_CALLBACK);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        selectedDeviceInfo->endpoint.addr,
                        selectedDeviceInfo->securePort,
                        selectedDeviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_ACL_URI))
    {
        OIC_LOG(ERROR, TAG, "SRPGetACLResource : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &SRPGetACLResourceCB;
    GetSecData_t* GetSecData = (GetSecData_t*)OICCalloc(1, sizeof(GetSecData_t));
    if (NULL == GetSecData)
    {
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    GetSecData->deviceInfo = selectedDeviceInfo;
    GetSecData->resultCallback = resultCallback;
    GetSecData->numOfResults=0;
    GetSecData->ctx = ctx;

    int noOfRiCalls = 1;
    GetSecData->resArr = (OCProvisionResult_t*)OICCalloc(noOfRiCalls, sizeof(OCProvisionResult_t));
    if (NULL == GetSecData->resArr)
    {
        OICFree(GetSecData);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    cbData.context = (void *)GetSecData;
    cbData.cd = NULL;
    OCMethod method = OC_REST_GET;
    OCDoHandle handle = NULL;
    OIC_LOG(DEBUG, TAG, "Sending Get ACL to resource server");
    OCStackResult ret = OCDoResource(&handle, method, query, NULL, NULL,
            selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
        OICFree(GetSecData->resArr);
        OICFree(GetSecData);
    }
    VERIFY_SUCCESS_RET(TAG, (OC_STACK_OK == ret), ERROR, OC_STACK_ERROR);
    OIC_LOG(DEBUG, TAG, "OUT SRPGetACLResource");

    return OC_STACK_OK;
}

OCStackResult SRPReadTrustCertChain(uint16_t credId, uint8_t **trustCertChain,
                                     size_t *chainSize)
{
    OIC_LOG(DEBUG, TAG, "IN SRPReadTrustCertChain");

    OCStackResult res = OC_STACK_ERROR;
    int secureFlag = 0;
    OicSecCred_t* credData = GetCredEntryByCredId(credId);
    if(credData)
    {
        res = CredToCBORPayload((const OicSecCred_t*) credData, trustCertChain,
                                chainSize, secureFlag);
        if(OC_STACK_OK != res)
        {
            OIC_LOG(INFO, TAG, "CredToCBORPayload failed");
        }
    }
    DeleteCredList(credData);
    return res;
}
