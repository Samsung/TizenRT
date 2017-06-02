/* ****************************************************************
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
 ******************************************************************/

/**
 * @file
 *
 * This file contains the implementation of classes and its members related
 * to CAManager. Network changes status will be passed from CA to application.
 */

#include "OCApi.h"
#include "CAManager.h"
#include "cautilinterface.h"
#include "casecurityinterface.h"
#include "logger.h"

#define TAG "OIC_CAMANAGER"

using namespace OC;
namespace
{
        CAManager::AdapterChangedCallback g_adapterHandler = nullptr;
        CAManager::ConnectionChangedCallback g_connectionHandler = nullptr;
}

OCStackResult convertCAResultToOCResult(CAResult_t caResult)
{
    switch (caResult)
    {
        case CA_STATUS_OK:
            return OC_STACK_OK;
        case CA_STATUS_INVALID_PARAM:
            return OC_STACK_INVALID_PARAM;
        case CA_STATUS_FAILED:
            return OC_STACK_ERROR;
        case CA_NOT_SUPPORTED:
            return OC_STACK_NOTIMPL;
        default:
            return OC_STACK_ERROR;
    }
}

void DefaultAdapterStateChangedHandler(CATransportAdapter_t adapter, bool enabled)
{
    if (g_adapterHandler)
    {
        g_adapterHandler((OCTransportAdapter) adapter, enabled);
    }
}

void DefaultConnectionStateChangedHandler(const CAEndpoint_t *info, bool isConnected)
{
    if (g_connectionHandler)
    {
        std::ostringstream ss;

        if (info->flags & CA_IPV6)
        {
            ss << '[' << info->addr << ']';
        }
        else
        {
            ss << info->addr;
        }
        if (info->port)
        {
            ss << ':' << info->port;
        }

        OCTransportAdapter adapter = (OCTransportAdapter)info->adapter;
        OCTransportFlags flags = (OCTransportFlags)info->flags;
        OCConnectivityType connType = (OCConnectivityType)
                ((adapter << CT_ADAPTER_SHIFT) | (flags & CT_MASK_FLAGS));

        g_connectionHandler(ss.str(), connType, isConnected);
    }
}

OCStackResult CAManager::setNetworkMonitorHandler(AdapterChangedCallback adapterHandler,
                                                  ConnectionChangedCallback connectionHandler)
{
    g_adapterHandler = adapterHandler;
    g_connectionHandler = connectionHandler;

    CAResult_t ret = CARegisterNetworkMonitorHandler(DefaultAdapterStateChangedHandler,
                                                     DefaultConnectionStateChangedHandler);

    return convertCAResultToOCResult(ret);
}

OCStackResult CAManager::setPortNumberToAssign(OCTransportAdapter adapter,
                                               OCTransportFlags flag, uint16_t port)
{
    OIC_LOG(INFO, TAG, "setPortNumberToAssign");
    CAResult_t ret = CASetPortNumberToAssign((CATransportAdapter_t) adapter,
                                             (CATransportFlags_t) flag, port);

    return convertCAResultToOCResult(ret);
}

uint16_t CAManager::getAssignedPortNumber(OCTransportAdapter adapter, OCTransportFlags flag)
{
    OIC_LOG(INFO, TAG, "getAssignedPortNumber");
    return CAGetAssignedPortNumber((CATransportAdapter_t) adapter, (CATransportFlags_t) flag);
}

OCStackResult CAManager::setMulticastTTL(size_t ttl)
{
    OIC_LOG(INFO, TAG, "setMulticastTTL");
    CAResult_t ret = CAUtilSetMulticastTTL(ttl);
    return convertCAResultToOCResult(ret);
}

OCStackResult CAManager::getMulticastTTL(size_t *ttl)
{
    OIC_LOG(INFO, TAG, "getMulticastTTL");
    CAResult_t ret = CAUtilGetMulticastTTL(ttl);
    return convertCAResultToOCResult(ret);
}

OCStackResult CAManager::setBTConfigure(const CAUtilConfig& config)
{
    OIC_LOG(INFO, TAG, "setBTConfigure");
    CAUtilConfig_t configs = {(CATransportBTFlags_t)config.bleFlag};
    CAResult_t ret = CAUtilSetBTConfigure(configs);
    return convertCAResultToOCResult(ret);
}

void CAManager::setLogLevel(OCLogLevel level, bool hidePrivateLogEntries)
{
    OIC_LOG(INFO, TAG, "setLogLevel");
    CAUtilSetLogLevel((CAUtilLogLevel_t) level, hidePrivateLogEntries);
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
OCStackResult CAManager::setCipherSuite(const uint16_t cipher, OCTransportAdapter adapter)
{
    CAResult_t ret = CASelectCipherSuite(cipher, (CATransportAdapter_t) adapter);
    return convertCAResultToOCResult(ret);
}
#endif // defined(__WITH_DTLS__) || defined(__WITH_TLS__)

#if defined(__TIZEN__) && defined(LE_ADAPTER) && defined(BLE_CUSTOM_ADVERTISE)
OCStackResult CAManager::setAdvertisementData(const char* data, int length)
{
    CAResult_t ret = CASetAdvertisementData(data, length);

    return convertCAResultToOCResult(ret);
}

OCStackResult CAManager::setScanResponseData(const char* data, int length)
{
    CAResult_t ret = CASetScanResponseData(data, length);

    return convertCAResultToOCResult(ret);
}
#endif

OCStackResult CAManager::startLEAdvertising()
{
    OIC_LOG(INFO, TAG, "startLEAdvertising");
    CAResult_t ret = CAUtilStartLEAdvertising();

    return convertCAResultToOCResult(ret);
}

OCStackResult CAManager::stopLEAdvertising()
{
    OIC_LOG(INFO, TAG, "stopLEAdvertising");
    CAResult_t ret = CAUtilStopLEAdvertising();

    return convertCAResultToOCResult(ret);
}

OCStackResult CAManager::disconnectTCPSession(const char *address,
                                              uint16_t port,
                                              OCTransportFlags flags)
{
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is invalid");
        return OCStackResult::OC_STACK_INVALID_PARAM;
    }

    OIC_LOG_V(INFO, TAG, "disconnectTCPSession [addr:%s][port:%d][flags:%d]",
              address, port, flags);
    CAResult_t ret = CAUtilTCPDisconnectSession(address, port, (CATransportFlags_t)flags);

    return convertCAResultToOCResult(ret);
}
