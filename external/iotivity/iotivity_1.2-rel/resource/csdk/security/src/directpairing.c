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
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif
#include "iotivity_config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif
#include "utlist.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "cainterface.h"

#include "directpairing.h"
#include "srmresourcestrings.h" //@note: SRM's internal header
#include "doxmresource.h"       //@note: SRM's internal header
#include "pconfresource.h"       //@note: SRM's internal header
#include "dpairingresource.h"       //@note: SRM's internal header
#include "credresource.h"

#include "pmtypes.h"
#include "pmutility.h"

#include "srmutility.h"

#define TAG ("OIC_DP")
static const uint16_t CBOR_SIZE = 1024;

/**
 * Structure to carry direct-pairing API data to callback.
 */
typedef struct DPairData
{
    OCDirectPairingDev_t        *peer;                         /**< Pointer to pairing target info.**/
    char                                  pin[DP_PIN_LENGTH];  /**< PIN **/
    OCDirectPairingResultCB    resultCallback;           /**< Pointer to result callback.**/
    void *userCtx;                                      /** < user context to pass in callback **/
} DPairData_t;

static OCDirectPairingDev_t *g_dp_paired = NULL;
static OCDirectPairingDev_t *g_dp_discover = NULL;
static DPairData_t *g_dp_proceed_ctx = NULL;


/**
 * Function to search node in linked list that matches given IP and port.
 *
 * @param[in] pList         List of OCProvisionDev_t.
 * @param[in] addr          address of target device.
 * @param[in] port          port of remote server.
 *
 * @return pointer of OCProvisionDev_t if exist, otherwise NULL
 */
OCDirectPairingDev_t* getDev(OCDirectPairingDev_t **ppList, const char* addr, const uint16_t port)
{
    if(NULL == addr)
    {
        OIC_LOG_V(ERROR, TAG, "Invalid Input parameters in [%s]\n", __FUNCTION__);
        return NULL;
    }

    OCDirectPairingDev_t *ptr = NULL;
    LL_FOREACH(*ppList, ptr)
    {
        if( strcmp(ptr->endpoint.addr, addr) == 0 && port == ptr->endpoint.port)
        {
            return ptr;
        }
    }

    return NULL;
}



/**
 * Add device information to list.
 *
 * @param[in] pList         List of OCProvisionDev_t.
 * @param[in] addr          address of target device.
 * @param[in] port          port of remote server.
 * @param[in] adapter       adapter type of endpoint.
 * @param[in] doxm          pointer to doxm instance.
 * @param[in] connType  connectivity type of endpoint
 *
 * @return OC_STACK_OK for success and errorcode otherwise.
 */
OCStackResult addDev(OCDirectPairingDev_t **ppList, OCDevAddr *endpoint,
                                      OCConnectivityType conn, OicSecPconf_t *pconf)
{
    if(NULL == endpoint || NULL == pconf)
    {
        OIC_LOG_V(ERROR, TAG, "Invalid Input parameters in [%s]\n", __FUNCTION__);
        return OC_STACK_INVALID_PARAM;
    }

    OCDirectPairingDev_t *ptr = getDev(ppList, endpoint->addr, endpoint->port);
    if(!ptr)
    {
        ptr = (OCDirectPairingDev_t *)OICCalloc(1, sizeof (OCDirectPairingDev_t));
        if (NULL == ptr)
        {
            OIC_LOG(ERROR, TAG, "Error while allocating memory for linkedlist node !!");
            return OC_STACK_NO_MEMORY;
        }

        memcpy(&ptr->endpoint, endpoint, sizeof(OCDevAddr));
        ptr->connType = conn;
        ptr->securePort = DEFAULT_SECURE_PORT;
        ptr->edp = pconf->edp;
        ptr->prm = pconf->prm;
        pconf->prm = NULL;  // to prevent free
        ptr->prmLen = pconf->prmLen;
        memcpy(&ptr->deviceID, &pconf->deviceID, sizeof(OicUuid_t));
        memcpy(&ptr->rowner, &pconf->rownerID, sizeof(OicUuid_t));
        ptr->next = NULL;

        LL_PREPEND(*ppList, ptr);
        OIC_LOG(INFO, TAG, "device added !");
    }

    return OC_STACK_OK;
}


/**
 * Add device information to list.
 *
 * @param[in] ppList         List of OCProvisionDev_t.
 * @param[in] pDev          target device.
 *
 * @return OC_STACK_OK for success and errorcode otherwise.
 */
OCStackResult addDev2(OCDirectPairingDev_t **ppList, OCDirectPairingDev_t *pDev)
{
    if(NULL == pDev)
    {
        OIC_LOG_V(ERROR, TAG, "Invalid Input parameters in [%s]\n", __FUNCTION__);
        return OC_STACK_INVALID_PARAM;
    }

    OCDirectPairingDev_t *ptr = getDev(ppList, pDev->endpoint.addr, pDev->endpoint.port);
    if(!ptr)
    {
        ptr = (OCDirectPairingDev_t *)OICCalloc(1, sizeof (OCDirectPairingDev_t));
        if (NULL == ptr)
        {
            OIC_LOG(ERROR, TAG, "Error while allocating memory for linkedlist node !!");
            return OC_STACK_NO_MEMORY;
        }

        memcpy(&ptr->endpoint, &pDev->endpoint, sizeof(OCDevAddr));
        ptr->connType = pDev->connType;
        ptr->securePort = pDev->securePort;
        ptr->edp = pDev->edp;
        ptr->prmLen = pDev->prmLen;
        ptr->prm = (OicSecPrm_t*)OICCalloc(ptr->prmLen, sizeof (OicSecPrm_t));
        if (NULL == ptr->prm)
        {
            OIC_LOG(ERROR, TAG, "Error while allocating memory for prm !!");
            return OC_STACK_NO_MEMORY;
        }
        memcpy(ptr->prm, pDev->prm, sizeof(OicSecPrm_t)*ptr->prmLen);
        memcpy(&ptr->deviceID, &pDev->deviceID, sizeof(OicUuid_t));
        memcpy(&ptr->rowner, &pDev->rowner, sizeof(OicUuid_t));
        ptr->next = NULL;

        LL_PREPEND(*ppList, ptr);
        OIC_LOG(INFO, TAG, "device added !");
    }

    return OC_STACK_OK;
}



/**
 * This function deletes list of provision target devices
 *
 * @param[in] pDevicesList         List of OCProvisionDev_t.
 */
void delList(OCDirectPairingDev_t *pList)
{
    if(pList)
    {
        OCDirectPairingDev_t *del = NULL, *tmp = NULL;
        LL_FOREACH_SAFE(pList, del, tmp)
        {
            LL_DELETE(pList, del);
            if (del && del->prm)
            {
                OICFree(del->prm);
            }
        }
    }
}

bool DPGenerateQuery(bool isSecure,
                     const char* address, const uint16_t port,
                     const OCConnectivityType connType,
                     char* buffer, size_t bufferSize, const char* uri)
{
    if(!address || !buffer || !uri)
    {
        OIC_LOG(ERROR, TAG, "DPGenerateQuery : Invalid parameters.");
        return false;
    }

    static char QPREFIX_COAP[] =  "coap://";
    static char QPREFIX_COAPS[] = "coaps://";
    static char QPREFIX_COAP_TCP[] =  "coap+tcp://";
    static char QPREFIX_COAPS_TCP[] = "coaps+tcp://";

    int snRet = 0;
    char* prefix = (isSecure == true) ? QPREFIX_COAPS : QPREFIX_COAP;

    switch(connType & CT_MASK_ADAPTER)
    {
// @todo: Remove this ifdef. On Arduino, CT_ADAPTER_TCP resolves to the same value
// as CT_ADAPTER_IP, resulting in a compiler error.
#ifdef WITH_TCP
#ifndef WITH_ARDUINO
        case CT_ADAPTER_TCP:
            prefix = (isSecure == true) ? QPREFIX_COAPS_TCP : QPREFIX_COAP_TCP;
#endif
#endif
        case CT_ADAPTER_IP:
            switch(connType & CT_MASK_FLAGS & ~CT_FLAG_SECURE)
            {
                case CT_IP_USE_V4:
                    snRet = snprintf(buffer, bufferSize, "%s%s:%d%s",
                                     prefix, address, port, uri);
                    break;
                case CT_IP_USE_V6:
                {
                    char addressEncoded[CA_MAX_URI_LENGTH] = {0};

                    OCStackResult result = OCEncodeAddressForRFC6874(addressEncoded,
                                                                     sizeof(addressEncoded),
                                                                     address);
                    if (OC_STACK_OK != result)
                    {
                        OIC_LOG_V(ERROR, TAG, "DPGenerateQuery : encoding error %d\n", result);
                        return false;
                    }

                    snRet = snprintf(buffer, bufferSize, "%s[%s]:%d%s",
                                     prefix, addressEncoded, port, uri);
                    break;
                }
                default:
                    OIC_LOG(ERROR, TAG, "Unknown address format.");
                    return false;
            }
            // snprintf return value check
            if (snRet < 0)
            {
                OIC_LOG_V(ERROR, TAG, "DPGenerateQuery : Error (snprintf) %d\n", snRet);
                return false;
            }
            else if ((size_t)snRet >= bufferSize)
            {
                OIC_LOG_V(ERROR, TAG, "DPGenerateQuery : Truncated (snprintf) %d\n", snRet);
                return false;
            }

            break;
#ifndef WITH_ARDUINO
        // TODO: We need to verify tinyDTLS in below cases
        case CT_ADAPTER_GATT_BTLE:
        case CT_ADAPTER_RFCOMM_BTEDR:
            OIC_LOG(ERROR, TAG, "Not supported connectivity adapter.");
            return false;
            break;
#endif
        default:
            OIC_LOG(ERROR, TAG, "Unknown connectivity adapter.");
            return false;
    }

    return true;
}

const OCDirectPairingDev_t* DPGetDiscoveredDevices()
{
    return g_dp_discover;
}

const OCDirectPairingDev_t* DPGetPairedDevices()
{
    return g_dp_paired;
}

void DPDeleteLists()
{
    delList(g_dp_discover);
    delList(g_dp_paired);
}

/**
 * Callback handler of FinalizeDirectPairing.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult DirectPairingFinalizeHandler(void *ctx, OCDoHandle UNUSED,
                                                  OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "IN DirectPairingFinalizeHandler()");
    (void)UNUSED;
    if(NULL == ctx)
    {
        OIC_LOG(ERROR, TAG, "Context is Null");
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCStackResult res;
    DPairData_t *dpairData = (DPairData_t*)ctx;
    OCDirectPairingDev_t *peer = dpairData->peer;
    OCDirectPairingResultCB resultCallback = dpairData->resultCallback;

    if (clientResponse)
    {
        if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            // result
            OIC_LOG(INFO, TAG, "DirectPairingFinalizeHandler : success PUT"
                    " request to /oic/sec/dpairing");

            CAEndpoint_t endpoint;
            memset(&endpoint, 0x00, sizeof(CAEndpoint_t));
            OICStrcpy(endpoint.addr, MAX_ADDR_STR_SIZE_CA, peer->endpoint.addr);
            endpoint.addr[MAX_ADDR_STR_SIZE_CA - 1] = '\0';
            endpoint.port = peer->securePort;

            OicUuid_t ptDeviceID = {.id={0}};
            if (OC_STACK_OK != GetDoxmDeviceID(&ptDeviceID))
            {
                OIC_LOG(ERROR, TAG, "Error while retrieving provisioning tool's device ID");
                resultCallback(dpairData->userCtx, peer, OC_STACK_ERROR);
                return OC_STACK_DELETE_TRANSACTION;
            }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            res = SavePairingPSK((OCDevAddr*)&endpoint, &peer->deviceID, &ptDeviceID, false);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Failed to PairingPSK generation");
                resultCallback(dpairData->userCtx, peer, res);
                return OC_STACK_DELETE_TRANSACTION;
            }

            //  close temporary sesion
            CAResult_t caResult = CAcloseSslSession((const CAEndpoint_t*)&endpoint);
            if(CA_STATUS_OK != caResult)
            {
                OIC_LOG(INFO, TAG, "Fail to close temporary dtls session");
            }
#endif // __WITH_DTLS__ or __WITH_TLS__

            OIC_LOG(INFO, TAG, "Direct-Papring was successfully completed.");

            // update paired list
            OCDirectPairingDev_t *dev = getDev(&g_dp_discover, peer->endpoint.addr,
                    peer->endpoint.port);
            res = addDev2(&g_dp_paired, dev);
            if (OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Error while adding a device to paired list.");
            }

            resultCallback(dpairData->userCtx, peer, OC_STACK_OK);

            return OC_STACK_DELETE_TRANSACTION;
        }
        else
        {
            OIC_LOG(INFO, TAG, "Direct-Papring received error response.");
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "DirectPairingFinalizeHandler received Null clientResponse");
    }

    resultCallback(dpairData->userCtx, peer, OC_STACK_ERROR);
    OICFree(dpairData);
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Finalize direct-pairing .
 *
 * @param[in] peer  target device to establish direct-pairing.
 * @param[in] resultCallback  result event callback.
 *
 * @return OC_STACK_OK on success otherwise error.
 */
OCStackResult FinalizeDirectPairing(void *ctx, OCDirectPairingDev_t* peer,
                                                     OCDirectPairingResultCB resultCallback)
{
    if(NULL == peer)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OicUuid_t deviceID =   {.id={0}};
    if (OC_STACK_OK != GetDoxmDeviceID(&deviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving device ID");
        return OC_STACK_ERROR;
    }

    OicSecDpairing_t dpair;
    memset(&dpair, 0, sizeof(OicSecDpairing_t));
    dpair.spm = (OicSecPrm_t)PRM_NOT_ALLOWED;
    memcpy(&dpair.pdeviceID, &deviceID, sizeof(OicUuid_t));

    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    OCStackResult ret = DpairingToCBORPayload(&dpair, &(secPayload->securityData),
            &(secPayload->payloadSize));

    if(OC_STACK_OK != ret)
    {
        OICFree(secPayload);
        OIC_LOG(ERROR, TAG, "Failed to DpairingToCBORPayload");
        return OC_STACK_NO_MEMORY;
    }
    OIC_LOG(DEBUG, TAG, "DPARING CBOR data:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!DPGenerateQuery(true,
                        peer->endpoint.addr,
                        peer->securePort,
                        peer->connType,
                        query, sizeof(query), OIC_RSRC_DPAIRING_URI))
    {
        OIC_LOG(ERROR, TAG, "DPDirectPairing : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    DPairData_t *dpairData = (DPairData_t *) OICCalloc(1, sizeof(DPairData_t));
    if (dpairData == NULL)
    {
        OICFree(secPayload->securityData);
        OICFree(secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    dpairData->peer = peer;
    dpairData->resultCallback = resultCallback;
    dpairData->userCtx = ctx;

    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = DirectPairingFinalizeHandler;
    cbData.context = (void*)dpairData;
    cbData.cd = NULL;

    OCMethod method = OC_REST_PUT;
    OCDoHandle handle = NULL;
    OIC_LOG(DEBUG, TAG, "Sending DPAIRNG setting to resource server");
    ret = OCDoResource(&handle, method, query,
            &peer->endpoint, (OCPayload*)secPayload,
            peer->connType, OC_LOW_QOS, &cbData, NULL, 0);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "error in OCDoResource");
        return OC_STACK_ERROR;
    }

    return OC_STACK_OK;
 }

/**
 * Function to handle the handshake result in Direct-Pairing.
 * This function will be invoked after DTLS handshake
 * @param   endPoint  [IN] The remote endpoint.
 * @param   errorInfo [IN] Error information from the endpoint.
 * @return  NONE
 */
void DirectPairingDTLSHandshakeCB(const CAEndpoint_t *endpoint, const CAErrorInfo_t *info)
{
    OIC_LOG_V(INFO, TAG, "IN DirectPairingDTLSHandshakeCB");


    if(g_dp_proceed_ctx && g_dp_proceed_ctx->peer && endpoint && info)
    {
        OIC_LOG_V(INFO, TAG, "Received status from remote device(%s:%d) : %d",
                 endpoint->addr, endpoint->port, info->result);

        OCDirectPairingDev_t *peer = g_dp_proceed_ctx->peer;
        OCDirectPairingResultCB resultCallback = g_dp_proceed_ctx->resultCallback;
        OCStackResult res;

        //Make sure the address matches.
        if(strncmp(peer->endpoint.addr, endpoint->addr, sizeof(endpoint->addr)) == 0 &&
                         peer->securePort == endpoint->port)
        {
            //In case of success, send next coaps request.
            if(CA_STATUS_OK == info->result)
            {
                OIC_LOG(INFO, TAG, "Now, finalize Direct-Pairing procedure.");

                res = FinalizeDirectPairing(g_dp_proceed_ctx->userCtx, peer, resultCallback);
                if(OC_STACK_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "Failed to finalize direct-pairing");
                    resultCallback(g_dp_proceed_ctx->userCtx, peer, res);
                }
            }
            else if(CA_DTLS_AUTHENTICATION_FAILURE == info->result)
            {
                OIC_LOG(INFO, TAG, "DirectPairingDTLSHandshakeCB - Authentication failed");
                resultCallback(g_dp_proceed_ctx->userCtx, peer, OC_STACK_AUTHENTICATION_FAILURE);
            }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            CAregisterSslHandshakeCallback(NULL);
#endif // __WITH_DTLS__ or __WITH_TLS__
            res = RemoveCredential(&peer->deviceID);
            if(OC_STACK_RESOURCE_DELETED != res)
            {
                OIC_LOG_V(ERROR, TAG, "Failed to remove temporal PSK : %d", res);
            }

            OICFree(g_dp_proceed_ctx);
            g_dp_proceed_ctx = NULL;
        }
        else
        {
            OIC_LOG_V(INFO, TAG, "DirectPairingDTLSHandshakeCB - Not matched to peer address");
        }
    }

    OIC_LOG_V(INFO, TAG, "OUT DirectPairingDTLSHandshakeCB");
}

/**
 * Callback handler of DPDirectPairing.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult DirectPairingHandler(void *ctx, OCDoHandle UNUSED,
                                                  OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "IN DirectPairingHandler.");
    (void)UNUSED;
    if(NULL == ctx)
    {
        OIC_LOG(ERROR, TAG, "Context is Null");
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCStackResult res = OC_STACK_ERROR;
    DPairData_t *dpairData = (DPairData_t*)ctx;
    OCDirectPairingResultCB resultCallback = (OCDirectPairingResultCB)dpairData->resultCallback;
    OicUuid_t subjectId = {.id={0}};

    if (clientResponse)
    {
        if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
        {
            // result
            OIC_LOG(INFO, TAG, "DirectPairingHandler : success POST request to /oic/sec/dpairing");

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            // Add temporary psk
            res = AddTmpPskWithPIN(&dpairData->peer->deviceID,
                           SYMMETRIC_PAIR_WISE_KEY,
                           (char*)dpairData->pin, DP_PIN_LENGTH,
                           &dpairData->peer->rowner, &subjectId);
            VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);


            // Start to establish a secure channel with Pin-based PSK cipher suite
            CAResult_t caresult;

            caresult = CAEnableAnonECDHCipherSuite(false);
            VERIFY_SUCCESS(TAG, CA_STATUS_OK == caresult, ERROR);

            caresult = CASelectCipherSuite(MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, CA_ADAPTER_IP);
            VERIFY_SUCCESS(TAG, CA_STATUS_OK == caresult, ERROR);

            //Register proceeding peer info. & DTLS event handler to catch the dtls event while handshake
            g_dp_proceed_ctx = dpairData;
            res = CAregisterSslHandshakeCallback(DirectPairingDTLSHandshakeCB);
            VERIFY_SUCCESS(TAG, CA_STATUS_OK == caresult, ERROR);

            // initiate dtls
            CAEndpoint_t *endpoint = (CAEndpoint_t *)OICCalloc(1, sizeof (CAEndpoint_t));
            VERIFY_NON_NULL(TAG, endpoint, FATAL);
            memcpy(endpoint,&dpairData->peer->endpoint,sizeof(CAEndpoint_t));
            endpoint->port = dpairData->peer->securePort;
            OIC_LOG_V(INFO, TAG, "Initiate DTLS handshake to %s(%d)", endpoint->addr,
                    endpoint->port);

            caresult = CAInitiateHandshake(endpoint);
            OICFree(endpoint);
            VERIFY_SUCCESS(TAG, CA_STATUS_OK == caresult, ERROR);
#endif // __WITH_DTLS__ or __WITH_TLS__

            res = OC_STACK_OK;
        }
        else
        {
            // result
            OIC_LOG(INFO, TAG, "DirectPairingHandler : fail POST request to /oic/sec/dpairing");
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "DirectPairingHandler received Null clientResponse");
    }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
exit:
#endif // __WITH_DTLS__ or __WITH_TLS__

    if (OC_STACK_OK != res)
    {
        if (0 < strlen((const char*)subjectId.id))
        {
            RemoveCredential(&dpairData->peer->deviceID);
            OICFree(dpairData);
            g_dp_proceed_ctx = NULL;
            OIC_LOG_V(INFO, TAG, "OUT DirectPairingHandler.");
            return OC_STACK_DELETE_TRANSACTION;
        }

        resultCallback(dpairData->userCtx, dpairData->peer, res);
    }
    OIC_LOG_V(INFO, TAG, "OUT DirectPairingHandler.");
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Start direct-pairing .
 *
 * @param[in] peer  target device to establish direct-pairing.
 * @param[in] pmSel  selected pairing method.
 * @param[in] pinNumber  secret value for dtls connection.
 *
 * @return OC_STACK_OK on success otherwise error.
 */
OCStackResult DPDirectPairing(void *ctx, OCDirectPairingDev_t* peer, OicSecPrm_t pmSel,
                                char *pinNumber, OCDirectPairingResultCB resultCallback)
{
    if(NULL == peer || NULL == pinNumber)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OicUuid_t deviceID =   {.id={0}};
    if (OC_STACK_OK != GetDoxmDeviceID(&deviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving device ID");
        return OC_STACK_ERROR;
    }

    OicSecDpairing_t dpair;
    memset(&dpair, 0, sizeof(OicSecDpairing_t));
    dpair.spm = pmSel;
    memcpy(&dpair.pdeviceID, &deviceID, sizeof(OicUuid_t));

    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    OCStackResult ret = DpairingToCBORPayload(&dpair, &(secPayload->securityData),
            &(secPayload->payloadSize));

    if(OC_STACK_OK != ret)
    {
        OICFree(secPayload);
        OIC_LOG(ERROR, TAG, "Failed to DpairingToCBORPayload");
        return OC_STACK_NO_MEMORY;
    }
    OIC_LOG(DEBUG, TAG, "DPARING CBOR data:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!DPGenerateQuery(false,
                        peer->endpoint.addr,
                        peer->endpoint.port,
                        //peer->securePort,
                        peer->connType,
                        query, sizeof(query), OIC_RSRC_DPAIRING_URI))
    {
        OIC_LOG(ERROR, TAG, "DPDirectPairing : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    DPairData_t *dpairData = (DPairData_t *) OICCalloc(1, sizeof(DPairData_t));
    if (dpairData == NULL)
    {
        OICFree(secPayload->securityData);
        OICFree(secPayload);
        OIC_LOG(ERROR, TAG, "Unable to allocate memory");
        return OC_STACK_NO_MEMORY;
    }
    dpairData->peer = peer;
    memcpy(dpairData->pin, pinNumber, DP_PIN_LENGTH);
    dpairData->resultCallback = resultCallback;
    dpairData->userCtx = ctx;

    OCCallbackData cbData =  {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = DirectPairingHandler;
    cbData.context = (void*)dpairData;
    cbData.cd = NULL;

    OCMethod method = OC_REST_POST;
    OCDoHandle handle = NULL;
    OIC_LOG(DEBUG, TAG, "Sending DPAIRNG setting to resource server");
    ret = OCDoResource(&handle, method, query,
            &peer->endpoint, (OCPayload*)secPayload,
            peer->connType, OC_LOW_QOS, &cbData, NULL, 0);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "error in OCDoResource");
        return OC_STACK_ERROR;
    }

    return OC_STACK_OK;

 }

/**
 * Callback handler for getting secure port information using /oic/res discovery.
 *
 * @param[in] ctx             user context
 * @param[in] handle          Handle for response
 * @param[in] clientResponse  Response information(It will contain payload)
 *
 * @return OC_STACK_KEEP_TRANSACTION to keep transaction and
 *         OC_STACK_DELETE_TRANSACTION to delete it.
 */
static OCStackApplicationResult DirectPairingPortDiscoveryHandler(void *ctx, OCDoHandle UNUSED,
                                 OCClientResponse *clientResponse)
{
    OIC_LOG(INFO, TAG, "Callback Context for Direct-Pairing Secure Port DISCOVER "
            "query recvd successfully");

    (void)ctx;
    (void)UNUSED;
    if (clientResponse)
    {
        if  (NULL == clientResponse->payload)
        {
            OIC_LOG(INFO, TAG, "Skiping Null payload");
        }
        else
        {
            if (PAYLOAD_TYPE_DISCOVERY != clientResponse->payload->type)
            {
                OIC_LOG(INFO, TAG, "Wrong payload type");
                return OC_STACK_DELETE_TRANSACTION;
            }

            uint16_t securePort = 0;
            OCResourcePayload* resPayload = ((OCDiscoveryPayload*)clientResponse->payload)->resources;
            OIC_LOG_PAYLOAD(INFO, clientResponse->payload);

            if (resPayload && resPayload->secure)
            {
                securePort = resPayload->port;
            }
            else
            {
                OIC_LOG(INFO, TAG, "Can not find secure port information.");
                return OC_STACK_DELETE_TRANSACTION;
            }

            OCDirectPairingDev_t *ptr = getDev(&g_dp_discover,
                    clientResponse->devAddr.addr, clientResponse->devAddr.port);
            if(!ptr)
            {
                OIC_LOG(ERROR, TAG, "Can not find device information in the discovery device list");
                return OC_STACK_DELETE_TRANSACTION;
            }
            ptr->securePort = securePort;

            OIC_LOG(INFO, TAG, "Exiting DirectPairingPortDiscoveryHandler.");
        }

        return  OC_STACK_DELETE_TRANSACTION;
    }
    else
    {
        OIC_LOG(INFO, TAG, "Skiping Null response");
    }
    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Callback handler for DPDeviceDiscovery API.
 *
 * @param[in] ctx             User context
 * @param[in] handle          Handler for response
 * @param[in] clientResponse  Response information (It will contain payload)
 * @return OC_STACK_KEEP_TRANSACTION to keep transaction and
 *         OC_STACK_DELETE_TRANSACTION to delete it.
 */
static OCStackApplicationResult DirectPairingDiscoveryHandler(void* ctx, OCDoHandle UNUSED,
        OCClientResponse * clientResponse)
{
    OIC_LOG(INFO, TAG, "Callback Context for Direct-Pairing DISCOVER query recvd successfully");

    (void)ctx;
    (void)UNUSED;
    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %d", clientResponse->result);
        OIC_LOG_V(INFO, TAG,
                "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);

        if  (NULL == clientResponse->payload)
        {
            OIC_LOG(INFO, TAG, "Skiping Null payload");
            return OC_STACK_KEEP_TRANSACTION;
        }
        if (OC_STACK_OK != clientResponse->result)
        {
            OIC_LOG(INFO, TAG, "Error in response");
            return OC_STACK_KEEP_TRANSACTION;
        }

        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        OicSecPconf_t *pconf = NULL;

        OCStackResult res = CBORPayloadToPconf(
                ((OCSecurityPayload*)clientResponse->payload)->securityData,
                CBOR_SIZE,&pconf);
        if (OC_STACK_OK != res )
        {
            OIC_LOG(INFO, TAG, "Ignoring malformed CBOR");
            return OC_STACK_KEEP_TRANSACTION;
        }
        else
        {
            if(pconf->edp)
            {
                OCDevAddr endpoint;
                memcpy(&endpoint, &clientResponse->devAddr, sizeof(OCDevAddr));

                OCStackResult res = addDev(&g_dp_discover, &endpoint,
                            clientResponse->connType, pconf);
                DeletePconfBinData(pconf);
                if (OC_STACK_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "Error while adding data to linkedlist.");
                    return OC_STACK_KEEP_TRANSACTION;
                }


                char rsrc_uri[MAX_URI_LENGTH+1] = {0};
                int wr_len = snprintf(rsrc_uri, sizeof(rsrc_uri), "%s?%s=%s",
                          OC_RSRVD_WELL_KNOWN_URI, OC_RSRVD_RESOURCE_TYPE, OIC_RSRC_TYPE_SEC_DPAIRING);
                if(wr_len <= 0 || (size_t)wr_len >= sizeof(rsrc_uri))
                {
                    OIC_LOG(ERROR, TAG, "rsrc_uri_string_print failed");
                    return OC_STACK_KEEP_TRANSACTION;
                }

                //Try to the unicast discovery to getting secure port
                char query[MAX_URI_LENGTH+MAX_QUERY_LENGTH+1] = {0};
                if(!DPGenerateQuery(false,
                                    clientResponse->devAddr.addr, clientResponse->devAddr.port,
                                    clientResponse->connType,
                                    query, sizeof(query), rsrc_uri))
                {
                    OIC_LOG(ERROR, TAG, "DirectPairingDiscoveryHandler : Failed to generate query");
                    return OC_STACK_KEEP_TRANSACTION;
                }
                OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

                OCCallbackData cbData;
                cbData.cb = &DirectPairingPortDiscoveryHandler;
                cbData.context = NULL;
                cbData.cd = NULL;
                OCStackResult ret = OCDoResource(NULL, OC_REST_DISCOVER, query, 0, 0,
                        clientResponse->connType, OC_LOW_QOS, &cbData, NULL, 0);
                if(OC_STACK_OK != ret)
                {
                    OIC_LOG(ERROR, TAG, "Failed to Secure Port Discovery");
                    return OC_STACK_KEEP_TRANSACTION;
                }
                else
                {
                    OIC_LOG_V(INFO, TAG, "OCDoResource with [%s] Success", query);
                }
            }
            return  OC_STACK_KEEP_TRANSACTION;
        }
    }
    else
    {
        OIC_LOG(INFO, TAG, "Skiping Null response");
    }

    return OC_STACK_DELETE_TRANSACTION;
}
#ifndef WITH_ARDUINO
/**
 * Discover direct-pairing devices in the same IP subnet. .
 *
 * @param[in] waittime  Timeout in seconds.
 *
 * @return OC_STACK_OK on success otherwise error.
 */
OCStackResult DPDeviceDiscovery(unsigned short waittime)
{
    OIC_LOG(DEBUG, TAG, "IN DPDeviceDiscovery");

    if (g_dp_discover)
    {
        delList(g_dp_discover);
        g_dp_discover = NULL;
    }

    OCStackResult ret;

    const char DP_DISCOVERY_QUERY[] = "/oic/sec/pconf";

    OCCallbackData cbData;
    cbData.cb = DirectPairingDiscoveryHandler;
    cbData.context = NULL;
    cbData.cd = NULL;

    /* Start a DP discovery query*/
    OIC_LOG_V(INFO, TAG, "Initiating Direct-Pairing Discovery : %s\n", DP_DISCOVERY_QUERY);
    OCDoHandle handle = NULL;
    ret = OCDoResource(&handle, OC_REST_DISCOVER, DP_DISCOVERY_QUERY, 0, 0, CT_DEFAULT,
                       OC_LOW_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
        return ret;
    }

    // wait..

    int clock_res = -1;
#if defined(_MSC_VER)
    time_t startTime = NULL;
    clock_res = (time(&startTime) == -1);
#else
    struct timespec startTime = {.tv_sec=0, .tv_nsec=0};
#if defined(__ANDROID__) || _POSIX_TIMERS > 0
    clock_res = clock_gettime(CLOCK_MONOTONIC, &startTime);
#endif
#endif
    if (0 != clock_res)
    {
        OIC_LOG(ERROR, TAG, "clock error");
        if(OC_STACK_OK !=  OCCancel(handle, OC_LOW_QOS, NULL, 0))
        {
            OIC_LOG(ERROR, TAG, "Failed to remove registered callback");
        }
        return OC_STACK_ERROR;
    }

    while (1)
    {
#if defined(_MSC_VER)
        time_t currTime = NULL;
        clock_res = (time(&currTime) == -1);
#else
        struct timespec currTime  = {.tv_sec=0, .tv_nsec=0};
#if defined(__ANDROID__) || _POSIX_TIMERS > 0
        clock_res = clock_gettime(CLOCK_MONOTONIC, &currTime);
#endif
#endif
        if (0 != clock_res)
        {
            OIC_LOG(ERROR, TAG, "clock error");
            ret = OC_STACK_ERROR;
            break;
        }
#if defined(_MSC_VER)
        long elapsed = currTime - startTime;
#else
        long elapsed = (currTime.tv_sec - startTime.tv_sec);
#endif
        if (elapsed > waittime)
        {
            break;
        }
        else
        {
            struct timespec timeout = {.tv_sec=0, .tv_nsec=100000000L};
            OCProcess();
            nanosleep(&timeout, NULL);
        }
    }

    // Waiting for each response.
    ret = OCCancel(handle, OC_LOW_QOS, NULL, 0);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "Failed to remove registered callback");
    }
    OIC_LOG(DEBUG, TAG, "OUT DPDeviceDiscovery");
    return ret;
}
#endif
