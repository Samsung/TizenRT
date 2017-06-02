/******************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "octypes.h"
#include "ocrandom.h"
#include "cainterface.h"
#include "caremotehandler.h"
#include "camessagehandler.h"
#include "caprotocolmessage.h"
#include "canetworkconfigurator.h"
#include "cainterfacecontroller.h"
#include "logger.h"
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "ca_adapter_net_ssl.h"
#endif // __WITH_DTLS__ or __WITH_TLS__

#ifdef TCP_ADAPTER
#include "catcpadapter.h"
#endif

CAGlobals_t caglobals = { .clientFlags = 0,
                          .serverFlags = 0, };

#define TAG "OIC_CA_CONN_MGR"

static bool g_isInitialized = false;

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
// Taking callback all the way through adapters not the right approach, hence calling here.
extern void CAsetPkixInfoCallback(CAgetPkixInfoHandler infCallback);
extern void CAsetPskCredentialsCallback(CAgetPskCredentialsHandler credCallback);
extern void CAsetCredentialTypesCallback(CAgetCredentialTypesHandler credCallback);
extern void CAsetSetupPkContextCallback(CAsetupPkContextHandler setupPkCtxCallback);
#endif // __WITH_DTLS__ or __WITH_TLS__


CAResult_t CAInitialize(CATransportAdapter_t transportType)
{
    OIC_LOG_V(DEBUG, TAG, "IoTivity version is v%s", IOTIVITY_VERSION);
    OIC_LOG_V(DEBUG, TAG, "CAInitialize type : %d", transportType);

    if (!g_isInitialized)
    {
        if (0 != OCSeedRandom())
        {
            OIC_LOG(ERROR, TAG, "Seed Random Failed");
        }

        CAResult_t res = CAInitializeMessageHandler(transportType);
        if (res != CA_STATUS_OK)
        {
            OIC_LOG(ERROR, TAG, "CAInitialize has failed");
            CATerminateMessageHandler();
            return res;
        }
        g_isInitialized = true;
    }

    return CA_STATUS_OK;
}

void CATerminate()
{
    OIC_LOG(DEBUG, TAG, "CATerminate");

    if (g_isInitialized)
    {
        CATerminateMessageHandler();
        CATerminateNetworkType();

        g_isInitialized = false;
    }
}

CAResult_t CAStartListeningServer()
{
    OIC_LOG(DEBUG, TAG, "CAStartListeningServer");

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    return CAStartListeningServerAdapters();
}

CAResult_t CAStopListeningServer()
{
    OIC_LOG(DEBUG, TAG, "CAStopListeningServer");

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    return CAStopListeningServerAdapters();
}

CAResult_t CAStartDiscoveryServer()
{
    OIC_LOG(DEBUG, TAG, "CAStartDiscoveryServer");

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    return CAStartDiscoveryServerAdapters();
}

void CARegisterHandler(CARequestCallback ReqHandler, CAResponseCallback RespHandler,
                       CAErrorCallback ErrorHandler)
{
    OIC_LOG(DEBUG, TAG, "CARegisterHandler");

    if (!g_isInitialized)
    {
        OIC_LOG(DEBUG, TAG, "CA is not initialized");
        return;
    }

    CASetInterfaceCallbacks(ReqHandler, RespHandler, ErrorHandler);
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#ifdef MULTIPLE_OWNER
const CASecureEndpoint_t *CAGetSecureEndpointData(const CAEndpoint_t *peer)
{
    OIC_LOG(DEBUG, TAG, "IN CAGetSecurePeerInfo");

    if (!g_isInitialized)
    {
        OIC_LOG(DEBUG, TAG, "CA is not initialized");
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "OUT CAGetSecurePeerInfo");
    return GetCASecureEndpointData(peer);
}
#endif //MULTIPLE_OWNER

CAResult_t CAregisterSslHandshakeCallback(CAErrorCallback tlsHandshakeCallback)
{
    OIC_LOG(DEBUG, TAG, "CAregisterSslHandshakeCallback");
    if(!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    CAsetSslHandshakeCallback(tlsHandshakeCallback);
    return CA_STATUS_OK;
}

CAResult_t CAregisterPskCredentialsHandler(CAgetPskCredentialsHandler getTlsCredentialsHandler)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }
    CAsetPskCredentialsCallback(getTlsCredentialsHandler);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

CAResult_t CAregisterPkixInfoHandler(CAgetPkixInfoHandler getPkixInfoHandler)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }
    CAsetPkixInfoCallback(getPkixInfoHandler);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

CAResult_t CAregisterGetCredentialTypesHandler(CAgetCredentialTypesHandler getCredTypesHandler)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }
    CAsetCredentialTypesCallback(getCredTypesHandler);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}

CAResult_t CAregisterSetupPkContextHandler(CAsetupPkContextHandler setupPkContextCallback)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }
    CAsetSetupPkContextCallback(setupPkContextCallback);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return CA_STATUS_OK;
}
#endif // __WITH_DTLS__ or __WITH_TLS__

CAResult_t CACreateEndpoint(CATransportFlags_t flags,
                            CATransportAdapter_t adapter,
                            const char *addr,
                            uint16_t port,
                            CAEndpoint_t **object)
{
    if (!object)
    {
        OIC_LOG(ERROR, TAG, "Invalid Parameter");
        return CA_STATUS_INVALID_PARAM;
    }

    CAEndpoint_t *endpoint = CACreateEndpointObject(flags, adapter, addr, port);
    if (!endpoint)
    {
        return CA_STATUS_FAILED;
    }
    *object = endpoint;
    return CA_STATUS_OK;
}

void CADestroyEndpoint(CAEndpoint_t *rep)
{
    OIC_LOG(DEBUG, TAG, "CADestroyEndpoint");

    CAFreeEndpoint(rep);
}

CAResult_t CAGenerateToken(CAToken_t *token, uint8_t tokenLength)
{
    OIC_LOG(DEBUG, TAG, "CAGenerateToken");

    return CAGenerateTokenInternal(token, tokenLength);
}

void CADestroyToken(CAToken_t token)
{
    OIC_LOG(DEBUG, TAG, "CADestroyToken");

    CADestroyTokenInternal(token);

    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CAGetNetworkInformation(CAEndpoint_t **info, uint32_t *size)
{
    OIC_LOG(DEBUG, TAG, "CAGetNetworkInformation");

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    return CAGetNetworkInformationInternal(info, size);
}

static CAResult_t CASendMessageMultiAdapter(const CAEndpoint_t *object, const void *sendMsg,
                                            CADataType_t dataType)
{
    OIC_LOG(DEBUG, TAG, "CASendMessageMultipleAdapter");

    CATransportAdapter_t connTypes[] = {
            CA_ADAPTER_IP
#ifdef LE_ADAPTER
            ,CA_ADAPTER_GATT_BTLE
#endif
#ifdef EDR_ADAPTER
            ,CA_ADAPTER_RFCOMM_BTEDR
#endif
#ifdef NFC_ADAPTER
            ,CA_ADAPTER_NFC
#endif
#ifdef RA_ADAPTER
            ,CA_ADAPTER_REMOTE_ACCESS
#endif
#ifdef TCP_ADAPTER
            ,CA_ADAPTER_TCP
#endif
        };

    CAEndpoint_t *cloneEp = CACloneEndpoint(object);
    if (!cloneEp)
    {
        OIC_LOG(ERROR, TAG, "Failed to clone CAEndpoint");
        return CA_MEMORY_ALLOC_FAILED;
    }

    CAResult_t ret = CA_STATUS_OK;
    size_t numConnTypes = sizeof(connTypes) / sizeof(connTypes[0]);

    for (size_t i = 0; i < numConnTypes && ret == CA_STATUS_OK; i++)
    {
        cloneEp->adapter = connTypes[i];
        ret = CADetachSendMessage(cloneEp, sendMsg, dataType);
    }
    CAFreeEndpoint(cloneEp);
    return ret;
}

CAResult_t CASendRequest(const CAEndpoint_t *object, const CARequestInfo_t *requestInfo)
{
    OIC_LOG(DEBUG, TAG, "CASendRequest");

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    if (requestInfo && requestInfo->isMulticast &&
            (object->adapter == CA_DEFAULT_ADAPTER || object->adapter == CA_ALL_ADAPTERS))
    {
        return CASendMessageMultiAdapter(object, requestInfo, CA_REQUEST_DATA);
    }
    else if (requestInfo && requestInfo->info.event == CA_REQ_DISCONNECT &&
            (object->adapter == CA_ADAPTER_TCP || object->adapter == CA_ALL_ADAPTERS))
    {
        return CADetachSendNetworkReqMessage(object, requestInfo->info.event, CA_NETWORK_COMMAND);
    }
    else
    {
        return CADetachSendMessage(object, requestInfo, CA_REQUEST_DATA);
    }
}

CAResult_t CASendResponse(const CAEndpoint_t *object, const CAResponseInfo_t *responseInfo)
{
    OIC_LOG(DEBUG, TAG, "CASendResponse");

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    if (!responseInfo || !object)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    if (responseInfo->isMulticast &&
            (object->adapter == CA_DEFAULT_ADAPTER || object->adapter == CA_ALL_ADAPTERS))
    {
        return CASendMessageMultiAdapter(object, responseInfo, responseInfo->info.dataType);
    }
    else
    {
        return CADetachSendMessage(object, responseInfo, responseInfo->info.dataType);
    }
}

CAResult_t CASelectNetwork(CATransportAdapter_t interestedNetwork)
{
    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    CAResult_t res = CA_STATUS_OK;

    if (interestedNetwork & CA_ADAPTER_IP)
    {
        res = CAAddNetworkType(CA_ADAPTER_IP);
        OIC_LOG_V(DEBUG, TAG, "CAAddNetworkType(CA_IP_ADAPTER) function returns result: %d", res);
    }
    else if (interestedNetwork & CA_ADAPTER_RFCOMM_BTEDR)
    {
        res = CAAddNetworkType(CA_ADAPTER_RFCOMM_BTEDR);
        OIC_LOG_V(DEBUG, TAG, "CAAddNetworkType(CA_RFCOMM_ADAPTER) function returns result : %d", res);
    }
    else if (interestedNetwork & CA_ADAPTER_GATT_BTLE)
    {
        res = CAAddNetworkType(CA_ADAPTER_GATT_BTLE);
        OIC_LOG_V(DEBUG, TAG, "CAAddNetworkType(CA_GATT_ADAPTER) function returns result : %d", res);
    }

#ifdef RA_ADAPTER
    else if (interestedNetwork & CA_ADAPTER_REMOTE_ACCESS)
    {
        res = CAAddNetworkType(CA_ADAPTER_REMOTE_ACCESS);
        OIC_LOG_V(DEBUG, TAG,
                  "CAAddNetworkType(CA_ADAPTER_REMOTE_ACCESS) function returns result : %d", res);
    }
#endif

#ifdef TCP_ADAPTER
    else if (interestedNetwork & CA_ADAPTER_TCP)
    {
        res = CAAddNetworkType(CA_ADAPTER_TCP);
        OIC_LOG_V(DEBUG, TAG,
                  "CAAddNetworkType(CA_ADAPTER_TCP) function returns result : %d", res);
    }
#endif
    else if (interestedNetwork & CA_ADAPTER_NFC)
    {
        res = CAAddNetworkType(CA_ADAPTER_NFC);
        OIC_LOG_V(DEBUG, TAG, "CAAddNetworkType(CA_ADAPTER_NFC) function returns result : %d", res);
    }
    else
    {
        res = CA_NOT_SUPPORTED;
    }
    return res;
}

CAResult_t CAUnSelectNetwork(CATransportAdapter_t nonInterestedNetwork)
{
    OIC_LOG_V(DEBUG, TAG, "unselected network : %d", nonInterestedNetwork);

    if (!g_isInitialized)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    CAResult_t res = CA_STATUS_OK;

    if (nonInterestedNetwork & CA_ADAPTER_IP)
    {
        res = CARemoveNetworkType(CA_ADAPTER_IP);
        OIC_LOG_V(DEBUG, TAG, "CARemoveNetworkType(CA_IP_ADAPTER) function returns result : %d", res);
    }
    else if (nonInterestedNetwork & CA_ADAPTER_RFCOMM_BTEDR)
    {
        res = CARemoveNetworkType(CA_ADAPTER_RFCOMM_BTEDR);
        OIC_LOG_V(DEBUG, TAG, "CARemoveNetworkType(CA_RFCOMM_ADAPTER) function returns result : %d", res);
    }
    else if (nonInterestedNetwork & CA_ADAPTER_GATT_BTLE)
    {
        res = CARemoveNetworkType(CA_ADAPTER_GATT_BTLE);
        OIC_LOG_V(DEBUG, TAG, "CARemoveNetworkType(CA_GATT_ADAPTER) function returns result : %d", res);
    }
#ifdef RA_ADAPTER
    else if (nonInterestedNetwork & CA_ADAPTER_REMOTE_ACCESS)
    {
        res = CARemoveNetworkType(CA_ADAPTER_REMOTE_ACCESS);
        OIC_LOG_V(DEBUG, TAG, "CARemoveNetworkType(CA_ADAPTER_REMOTE_ACCESS) function returns result : %d",
                  res);
    }
#endif


#ifdef TCP_ADAPTER
    else if (nonInterestedNetwork & CA_ADAPTER_TCP)
    {
        res = CARemoveNetworkType(CA_ADAPTER_TCP);
        OIC_LOG_V(DEBUG, TAG, "CARemoveNetworkType(CA_ADAPTER_TCP) function returns result : %d",
                  res);
    }
#endif

    else
    {
        res = CA_STATUS_FAILED;
    }
    return res;
}

CAResult_t CAHandleRequestResponse()
{
    if (!g_isInitialized)
    {
        OIC_LOG(ERROR, TAG, "not initialized");
        return CA_STATUS_NOT_INITIALIZED;
    }

    CAHandleRequestResponseCallbacks();

    return CA_STATUS_OK;
}

CAResult_t CASelectCipherSuite(const uint16_t cipher, CATransportAdapter_t adapter)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);
    OIC_LOG_V(DEBUG, TAG, "cipher : %d , CATransportAdapter : %d", cipher, adapter);
    CAResult_t res = CA_STATUS_FAILED;
#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    res = CAsetTlsCipherSuite(cipher);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to CAsetTlsCipherSuite : %d", res);
    }
#else
    OIC_LOG(ERROR, TAG, "Method not supported");
#endif
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return res;
}

CAResult_t CAEnableAnonECDHCipherSuite(const bool enable)
{
    OIC_LOG_V(DEBUG, TAG, "CAEnableAnonECDHCipherSuite");
    CAResult_t res = CA_STATUS_FAILED;
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    // TLS_ECDH_ANON_WITH_AES_128_CBC_SHA256    0xFF00 replaces 0xC018
    res = CAsetTlsCipherSuite(enable ? 0xFF00 : 0x00);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to CAsetTlsCipherSuite : %d", res);
    }
#else
    OIC_LOG(ERROR, TAG, "Method not supported");
#endif
    OIC_LOG_V(ERROR, TAG, "Out %s", __func__);
    return res;
}

CAResult_t CAGenerateOwnerPSK(const CAEndpoint_t* endpoint,
                    const uint8_t* label, const size_t labelLen,
                    const uint8_t* rsrcServerDeviceID, const size_t rsrcServerDeviceIDLen,
                    const uint8_t* provServerDeviceID, const size_t provServerDeviceIDLen,
                    uint8_t* ownerPSK, const size_t ownerPskSize)
{
    OIC_LOG_V(DEBUG, TAG, "IN : CAGenerateOwnerPSK");
    CAResult_t res = CA_STATUS_FAILED;
#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    //newOwnerLabel and prevOwnerLabe can be NULL
    if (!endpoint || !label || 0 == labelLen || !ownerPSK || 0 == ownerPskSize)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    res = CAsslGenerateOwnerPsk(endpoint, label, labelLen,
                                      rsrcServerDeviceID, rsrcServerDeviceIDLen,
                                      provServerDeviceID, provServerDeviceIDLen,
                                      ownerPSK, ownerPskSize);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to CAGenerateOwnerPSK : %d", res);
    }
#else
    OIC_LOG(ERROR, TAG, "Method not supported");
#endif
    OIC_LOG_V(DEBUG, TAG, "OUT : CAGenerateOwnerPSK");
    return res;
}

CAResult_t CAInitiateHandshake(const CAEndpoint_t *endpoint)
{
    OIC_LOG_V(DEBUG, TAG, "IN : CAInitiateHandshake");
    CAResult_t res = CA_STATUS_FAILED;
#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    if (!endpoint)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    res = CAinitiateSslHandshake(endpoint);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to CAinitiateSslHandshake : %d", res);
    }
#else
        OIC_LOG(ERROR, TAG, "Method not supported");
#endif
    OIC_LOG_V(DEBUG, TAG, "OUT : CAInitiateHandshake");
    return res;
}

CAResult_t CAcloseSslSession(const CAEndpoint_t *endpoint)
{
    OIC_LOG_V(DEBUG, TAG, "IN : CAcloseSslSession");
    CAResult_t res = CA_STATUS_FAILED;
#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    if (!endpoint)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    res = CAcloseSslConnection(endpoint);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to CAsslClose : %d", res);
    }
#else
    OIC_LOG(ERROR, TAG, "Method not supported");
#endif
    OIC_LOG_V(DEBUG, TAG, "OUT : CAcloseSslSession");
    return res;
}

#ifdef TCP_ADAPTER
void CARegisterKeepAliveHandler(CAKeepAliveConnectionCallback ConnHandler)
{
    CATCPSetKeepAliveCallbacks(ConnHandler);
}
#endif
