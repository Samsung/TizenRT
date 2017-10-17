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
#include <string.h>
#include <inttypes.h>

#include "logger.h"
#include "oic_malloc.h"
#include "caadapterutils.h"
#include "canetworkconfigurator.h"
#include "cainterfacecontroller.h"
#include "caedradapter.h"
#include "caleadapter.h"
#include "canfcadapter.h"
#include "caremotehandler.h"
#include "cathreadpool.h"
#include "caipadapter.h"
#include "cainterface.h"
#include <coap/utlist.h>

#ifdef RA_ADAPTER
#include "caraadapter.h"
#endif

#ifdef TCP_ADAPTER
#include "catcpadapter.h"
#endif

#define TAG "OIC_CA_INF_CTR"

#define CA_MEMORY_ALLOC_CHECK(arg) {if (arg == NULL) \
    {OIC_LOG(ERROR, TAG, "memory error");goto memory_error_exit;} }

static CAConnectivityHandler_t *g_adapterHandler = NULL;

static size_t g_numberOfAdapters = 0;

static CANetworkPacketReceivedCallback g_networkPacketReceivedCallback = NULL;

static CAErrorHandleCallback g_errorHandleCallback = NULL;

static struct CANetworkCallback_t *g_networkChangeCallbackList = NULL;

/**
 * network callback structure is handling
 * for adapter state changed and connection state changed event.
 */
typedef struct CANetworkCallback_t
{

    /** Linked list; for multiple callback list.*/
    struct CANetworkCallback_t *next;

    /** Adapter state changed event callback. */
    CAAdapterStateChangedCB adapter;

    /** Connection state changed event callback. */
    CAConnectionStateChangedCB conn;

} CANetworkCallback_t;

static CAResult_t CAGetAdapterIndex(CATransportAdapter_t cType, size_t *adapterIndex)
{
    for (size_t index = 0 ; index < g_numberOfAdapters ; index++)
    {
        if (cType == g_adapterHandler[index].cType )
        {
            *adapterIndex = index;
            return CA_STATUS_OK;
        }
    }
    return CA_STATUS_FAILED;
}

static void CARegisterCallback(CAConnectivityHandler_t handler)
{
    if (handler.startAdapter == NULL ||
        handler.startListenServer == NULL ||
        handler.stopListenServer == NULL ||
        handler.startDiscoveryServer == NULL ||
        handler.sendData == NULL ||
        handler.sendDataToAll == NULL ||
        handler.GetnetInfo == NULL ||
        handler.readData == NULL ||
        handler.stopAdapter == NULL ||
        handler.terminate == NULL)
    {
        OIC_LOG(ERROR, TAG, "connectivity handler is not enough to be used!");
        return;
    }
    size_t numberofAdapters = g_numberOfAdapters + 1;
    CAConnectivityHandler_t *adapterHandler = OICRealloc(g_adapterHandler,
            (numberofAdapters) * sizeof(*adapterHandler));
    if (NULL == adapterHandler)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed during registration");
        return;
    }
    g_adapterHandler = adapterHandler;
    g_numberOfAdapters = numberofAdapters;
    g_adapterHandler[g_numberOfAdapters - 1] = handler;

    OIC_LOG_V(DEBUG, TAG, "%d type adapter, register complete!", handler.cType);
}

/**
 * Add a network callback from caller to the network callback list
 *
 * @param adapterCB  adapter state changed callback
 * @param connCB     connection state changed callback
 *
 * @return
 *     CAResult_t
 */
static CAResult_t AddNetworkStateChangedCallback(CAAdapterStateChangedCB adapterCB,
        CAConnectionStateChangedCB connCB)
{
    OIC_LOG(DEBUG, TAG, "Add NetworkStateChanged Callback");

    if (!adapterCB)
    {
        OIC_LOG(ERROR, TAG, "adapterCB is null");
        return CA_STATUS_INVALID_PARAM;
    }

#if defined(TCP_ADAPTER) || defined(EDR_ADAPTER) || defined(LE_ADAPTER)
    if (!connCB)
    {
        OIC_LOG(ERROR, TAG, "connCB is null");
        return CA_STATUS_INVALID_PARAM;
    }
#endif

    CANetworkCallback_t *callback = NULL;
    LL_FOREACH(g_networkChangeCallbackList, callback)
    {
        if (callback && adapterCB == callback->adapter && connCB == callback->conn)
        {
            OIC_LOG(DEBUG, TAG, "this callback is already added");
            return CA_STATUS_OK;
        }
    }

    callback = (CANetworkCallback_t *) OICCalloc(1, sizeof(CANetworkCallback_t));
    if (NULL == callback)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed during registration");
        return CA_MEMORY_ALLOC_FAILED;
    }

    callback->adapter = adapterCB;
#if defined(TCP_ADAPTER) || defined(EDR_ADAPTER) || defined(LE_ADAPTER)
    // Since IP adapter(UDP) is the Connectionless Protocol, it doesn't need.
    callback->conn = connCB;
#endif
    LL_APPEND(g_networkChangeCallbackList, callback);
    return CA_STATUS_OK;
}

/**
 * Remove a network callback from the network callback list
 *
 * @param adapterCB  adapter state changed callback
 * @param connCB     connection state changed callback
 *
 * @return
 *     CAResult_t
 */
static CAResult_t RemoveNetworkStateChangedCallback(CAAdapterStateChangedCB adapterCB,
        CAConnectionStateChangedCB connCB)
{
    OIC_LOG(DEBUG, TAG, "Remove NetworkStateChanged Callback");

    CANetworkCallback_t *callback = NULL;
    LL_FOREACH(g_networkChangeCallbackList, callback)
    {
        if (callback && adapterCB == callback->adapter && connCB == callback->conn)
        {
            OIC_LOG(DEBUG, TAG, "remove specific callback");
            LL_DELETE(g_networkChangeCallbackList, callback);
            OICFree(callback);
            callback = NULL;
            return CA_STATUS_OK;
        }
    }
    return CA_STATUS_OK;
}

/**
 * Remove all network callback from the network callback list
 */
static void RemoveAllNetworkStateChangedCallback()
{
    OIC_LOG(DEBUG, TAG, "Remove All NetworkStateChanged Callback");

    CANetworkCallback_t *callback = NULL;
    CANetworkCallback_t *tmp = NULL;
    LL_FOREACH_SAFE(g_networkChangeCallbackList, callback, tmp)
    {
        LL_DELETE(g_networkChangeCallbackList, callback);
        OICFree(callback);
        callback = NULL;
    }
    g_networkChangeCallbackList = NULL;
}

#ifdef RA_ADAPTER
CAResult_t CASetAdapterRAInfo(const CARAInfo_t *caraInfo)
{
    return CASetRAInfo(caraInfo);
}
#endif

static void CAReceivedPacketCallback(const CASecureEndpoint_t *sep,
                                     const void *data, size_t dataLen)
{
    if (g_networkPacketReceivedCallback != NULL)
    {
        g_networkPacketReceivedCallback(sep, data, dataLen);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "network packet received callback is NULL!");
    }
}

static void CAAdapterChangedCallback(CATransportAdapter_t adapter, CANetworkStatus_t status)
{
    // Call the callback.
    CANetworkCallback_t *callback  = NULL;
    LL_FOREACH(g_networkChangeCallbackList, callback)
    {
        if (callback && callback->adapter)
        {
            if (CA_INTERFACE_UP == status)
            {
                callback->adapter(adapter, true);
            }
            else if (CA_INTERFACE_DOWN == status)
            {
                callback->adapter(adapter, false);
            }
        }
    }
    OIC_LOG_V(DEBUG, TAG, "[%d] adapter status is changed to [%d]", adapter, status);
}

#if defined(TCP_ADAPTER) || defined(EDR_ADAPTER) || defined(LE_ADAPTER)
static void CAConnectionChangedCallback(const CAEndpoint_t *info, bool isConnected)
{
    // Call the callback.
    CANetworkCallback_t *callback = NULL;
    LL_FOREACH(g_networkChangeCallbackList, callback)
    {
        if (callback && callback->conn)
        {
            callback->conn(info, isConnected);
        }
    }
    OIC_LOG_V(DEBUG, TAG, "[%s] connection status is changed to [%d]", info->addr, isConnected);
}
#endif

static void CAAdapterErrorHandleCallback(const CAEndpoint_t *endpoint,
        const void *data, size_t dataLen,
        CAResult_t result)
{
    OIC_LOG(DEBUG, TAG, "received error from adapter in interfacecontroller");

    // Call the callback.
    if (g_errorHandleCallback != NULL)
    {
        g_errorHandleCallback(endpoint, data, dataLen, result);
    }
}

void CAInitializeAdapters(ca_thread_pool_t handle, CATransportAdapter_t transportType)
{
    OIC_LOG_V(DEBUG, TAG, "initialize adapters %d", transportType);

    // Initialize adapters and register callback.
#ifdef IP_ADAPTER
    if ((transportType & CA_ADAPTER_IP) || (CA_DEFAULT_ADAPTER == transportType)
        || (transportType == CA_ALL_ADAPTERS))
    {
        CAInitializeIP(CARegisterCallback, CAReceivedPacketCallback, CAAdapterChangedCallback,
                       CAAdapterErrorHandleCallback, handle);
    }
#endif /* IP_ADAPTER */

#ifdef EDR_ADAPTER
    if ((transportType & CA_ADAPTER_RFCOMM_BTEDR) || (CA_DEFAULT_ADAPTER == transportType)
        || (transportType == CA_ALL_ADAPTERS))
    {
        CAInitializeEDR(CARegisterCallback, CAReceivedPacketCallback, CAAdapterChangedCallback,
                        CAConnectionChangedCallback, CAAdapterErrorHandleCallback, handle);
    }
#endif /* EDR_ADAPTER */

#ifdef LE_ADAPTER
    if ((transportType & CA_ADAPTER_GATT_BTLE) || (CA_DEFAULT_ADAPTER == transportType)
        || (transportType == CA_ALL_ADAPTERS))
    {
        CAInitializeLE(CARegisterCallback, CAReceivedPacketCallback, CAAdapterChangedCallback,
                       CAConnectionChangedCallback, CAAdapterErrorHandleCallback, handle);
    }
#endif /* LE_ADAPTER */

#ifdef RA_ADAPTER
    if ((transportType & CA_ADAPTER_REMOTE_ACCESS) || (CA_DEFAULT_ADAPTER == transportType)
        || (transportType == CA_ALL_ADAPTERS))
    {
        CAInitializeRA(CARegisterCallback, CAReceivedPacketCallback, CAAdapterChangedCallback,
                       handle);
    }
#endif /* RA_ADAPTER */

#ifdef TCP_ADAPTER
    if ((transportType & CA_ADAPTER_TCP) || (CA_DEFAULT_ADAPTER == transportType)
        || (transportType == CA_ALL_ADAPTERS))
    {
        CAInitializeTCP(CARegisterCallback, CAReceivedPacketCallback, CAAdapterChangedCallback,
                        CAConnectionChangedCallback, CAAdapterErrorHandleCallback, handle);
    }
#endif /* TCP_ADAPTER */

#ifdef NFC_ADAPTER
    if ((transportType & CA_ADAPTER_NFC) || (CA_DEFAULT_ADAPTER == transportType)
        || (transportType == CA_ALL_ADAPTERS))
    {
        CAInitializeNFC(CARegisterCallback, CAReceivedPacketCallback, CAAdapterChangedCallback,
                        CAAdapterErrorHandleCallback, handle);
    }
#endif /* NFC_ADAPTER */
}

void CASetPacketReceivedCallback(CANetworkPacketReceivedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "Set Receiver handle callback");

    g_networkPacketReceivedCallback = callback;
}

CAResult_t CASetNetworkMonitorCallbacks(CAAdapterStateChangedCB adapterCB,
                                        CAConnectionStateChangedCB connCB)
{
    OIC_LOG(DEBUG, TAG, "Set network monitoring callback");
    CAResult_t res = AddNetworkStateChangedCallback(adapterCB, connCB);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "AddNetworkStateChangedCallback has failed");
        return CA_STATUS_FAILED;
    }
    return CA_STATUS_OK;
}

CAResult_t CAUnsetNetworkMonitorCallbacks(CAAdapterStateChangedCB adapterCB,
        CAConnectionStateChangedCB connCB)
{
    OIC_LOG(DEBUG, TAG, "Unset network monitoring callback");
    CAResult_t res = RemoveNetworkStateChangedCallback(adapterCB, connCB);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "RemoveNetworkStateChangedCallback has failed");
        return CA_STATUS_FAILED;
    }
    return CA_STATUS_OK;
}

void CASetErrorHandleCallback(CAErrorHandleCallback errorCallback)
{
    OIC_LOG(DEBUG, TAG, "Set error handle callback");
    g_errorHandleCallback = errorCallback;
}

CAResult_t CAStartAdapter(CATransportAdapter_t transportType)
{
    size_t index = 0;
    CAResult_t res = CA_STATUS_FAILED;

    OIC_LOG_V(DEBUG, TAG, "Start the adapter of CAConnectivityType[%d]", transportType);

    res = CAGetAdapterIndex(transportType, &index);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "unknown connectivity type!");
        return CA_STATUS_FAILED;
    }

    if (g_adapterHandler[index].startAdapter != NULL)
    {
        res = g_adapterHandler[index].startAdapter();
    }

    return res;
}

void CAStopAdapter(CATransportAdapter_t transportType)
{
    size_t index = 0;
    CAResult_t res = CA_STATUS_FAILED;

    OIC_LOG_V(DEBUG, TAG, "Stop the adapter of CATransportType[%d]", transportType);

    res = CAGetAdapterIndex(transportType, &index);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "unknown transport type!");
        return;
    }

    if (g_adapterHandler[index].stopAdapter != NULL)
    {
        g_adapterHandler[index].stopAdapter();
    }
}

CAResult_t CAGetNetworkInfo(CAEndpoint_t **info, size_t *size)
{
    VERIFY_NON_NULL(info, TAG, "info is null");
    VERIFY_NON_NULL(size, TAG, "size is null");

    CAEndpoint_t **tempInfo = (CAEndpoint_t **) OICCalloc(g_numberOfAdapters, sizeof(*tempInfo));
    if (!tempInfo)
    {
        OIC_LOG(ERROR, TAG, "Out of memory!");
        return CA_MEMORY_ALLOC_FAILED;
    }
    size_t *tempSize = (size_t *)OICCalloc(g_numberOfAdapters, sizeof(*tempSize));
    if (!tempSize)
    {
        OIC_LOG(ERROR, TAG, "Out of memory!");
        OICFree(tempInfo);
        return CA_MEMORY_ALLOC_FAILED;
    }

    CAResult_t res = CA_STATUS_FAILED;
    size_t resSize = 0;
    for (size_t index = 0; index < g_numberOfAdapters; index++)
    {
        if (g_adapterHandler[index].GetnetInfo != NULL)
        {
            // #1. get information for each adapter
            res = g_adapterHandler[index].GetnetInfo(&tempInfo[index],
                    &tempSize[index]);

            // #2. total size
            if (res == CA_STATUS_OK)
            {
                resSize += tempSize[index];
            }

            OIC_LOG_V(DEBUG,
                      TAG,
                      "%" PRIuPTR " adapter network info size is %" PRIuPTR " res:%u",
                      index,
                      tempSize[index],
                      res);
        }
    }

    OIC_LOG_V(DEBUG, TAG, "network info total size is %" PRIuPTR "!", resSize);

    if (resSize == 0)
    {
        OICFree(tempInfo);
        OICFree(tempSize);
        return res;
    }

    // #3. add data into result
    // memory allocation
    CAEndpoint_t *resInfo = (CAEndpoint_t *) OICCalloc(resSize, sizeof (*resInfo));
    CA_MEMORY_ALLOC_CHECK(resInfo);

    // #4. save data
    *info = resInfo;
    *size = resSize;

    for (size_t index = 0; index < g_numberOfAdapters; index++)
    {
        // check information
        if (tempSize[index] == 0)
        {
            continue;
        }

        memcpy(resInfo,
               tempInfo[index],
               sizeof(*resInfo) * tempSize[index]);

        resInfo += tempSize[index];

        // free adapter data
        OICFree(tempInfo[index]);
        tempInfo[index] = NULL;
    }
    OICFree(tempInfo);
    OICFree(tempSize);

    OIC_LOG(DEBUG, TAG, "each network info save success!");
    return CA_STATUS_OK;

    // memory error label.
memory_error_exit:

    for (size_t index = 0; index < g_numberOfAdapters; index++)
    {
        OICFree(tempInfo[index]);
        tempInfo[index] = NULL;
    }
    OICFree(tempInfo);
    OICFree(tempSize);

    return CA_MEMORY_ALLOC_FAILED;
}

CAResult_t CASendUnicastData(const CAEndpoint_t *endpoint, const void *data, uint32_t length,
                             CADataType_t dataType)
{
    size_t index = 0;
    CAResult_t res = CA_STATUS_FAILED;

    VERIFY_NON_NULL(endpoint, TAG, "endpoint is null");

    u_arraylist_t *list = CAGetSelectedNetworkList();
    if (!list)
    {
        OIC_LOG(ERROR, TAG, "No selected network");
        return CA_SEND_FAILED;
    }
    CATransportAdapter_t requestedAdapter = endpoint->adapter ? endpoint->adapter : CA_ALL_ADAPTERS;

    for (size_t i = 0; i < u_arraylist_length(list); i++)
    {
        void *ptrType = u_arraylist_get(list, i);

        if (NULL == ptrType)
        {
            continue;
        }

        CATransportAdapter_t connType = *(CATransportAdapter_t *)ptrType;
        if (0 == (connType & requestedAdapter))
        {
            continue;
        }

        res = CAGetAdapterIndex(connType, &index);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "unknown transport type!");
            return CA_STATUS_INVALID_PARAM;
        }

        int32_t sentDataLen = 0;

        if (NULL != g_adapterHandler[index].sendData)
        {
            OIC_LOG(DEBUG, TAG, "unicast message to adapter");
            sentDataLen = g_adapterHandler[index].sendData(endpoint, data, length, dataType);
        }

        if ((0 > sentDataLen) || ((uint32_t)sentDataLen != length))
        {
            OIC_LOG(ERROR, TAG, "Error sending data. The error will be reported in adapter.");
#ifdef SINGLE_THREAD
            //in case of single thread, no error handler. Report error immediately
            return CA_SEND_FAILED;
#endif
        }

    }

    return CA_STATUS_OK;
}

CAResult_t CASendMulticastData(const CAEndpoint_t *endpoint, const void *data, uint32_t length,
                               CADataType_t dataType)
{
    size_t index = 0;
    CAResult_t res = CA_STATUS_FAILED;

    VERIFY_NON_NULL(endpoint, TAG, "endpoint is null");

    u_arraylist_t *list = CAGetSelectedNetworkList();
    if (!list)
    {
        OIC_LOG(DEBUG, TAG, "No selected network");
        return CA_SEND_FAILED;
    }

    CATransportAdapter_t requestedAdapter = endpoint->adapter ? endpoint->adapter : CA_ALL_ADAPTERS;
    size_t selectedLength = u_arraylist_length(list);
    for (size_t i = 0; i < selectedLength; i++)
    {
        void *ptrType = u_arraylist_get(list, i);

        if (NULL == ptrType)
        {
            continue;
        }

        CATransportAdapter_t connType = *(CATransportAdapter_t *)ptrType;
        if (0 == (connType & requestedAdapter))
        {
            continue;
        }

        res = CAGetAdapterIndex(connType, &index);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "unknown connectivity type!");
            continue;
        }

        uint32_t sentDataLen = 0;

        if (NULL != g_adapterHandler[index].sendDataToAll)
        {
            void *payload = (void *) OICMalloc(length);
            if (!payload)
            {
                OIC_LOG(ERROR, TAG, "Out of memory!");
                return CA_MEMORY_ALLOC_FAILED;
            }
            memcpy(payload, data, length);
            sentDataLen = g_adapterHandler[index].sendDataToAll(endpoint, payload, length, dataType);
            OICFree(payload);
        }

        if (sentDataLen != length)
        {
            OIC_LOG(ERROR, TAG, "sendDataToAll failed! Error will be reported from adapter");
#ifdef SINGLE_THREAD
            //in case of single thread, no error handler. Report error immediately
            return CA_SEND_FAILED;
#endif
        }
    }

    return CA_STATUS_OK;
}

CAResult_t CAStartListeningServerAdapters()
{
    size_t index = 0;
    CAResult_t result = CA_STATUS_FAILED;

    u_arraylist_t *list = CAGetSelectedNetworkList();
    if (!list)
    {
        OIC_LOG(ERROR, TAG, "No selected network");
        return result;
    }

    size_t length = u_arraylist_length(list);
    for (size_t i = 0; i < length; i++)
    {
        void *ptrType = u_arraylist_get(list, i);

        if (ptrType == NULL)
        {
            continue;
        }

        CATransportAdapter_t connType = *(CATransportAdapter_t *)ptrType;

        if (CA_STATUS_OK != CAGetAdapterIndex(connType, &index))
        {
            OIC_LOG(ERROR, TAG, "unknown connectivity type!");
            continue;
        }

        if (g_adapterHandler[index].startListenServer != NULL)
        {
            const CAResult_t tmp =
                g_adapterHandler[index].startListenServer();

            // Successful listen if at least one adapter started.
            if (CA_STATUS_OK == tmp)
            {
                result = tmp;
            }
        }
    }

    return result;
}

CAResult_t CAStopListeningServerAdapters()
{
    size_t index = 0;
    CAResult_t res = CA_STATUS_FAILED;
    u_arraylist_t *list = CAGetSelectedNetworkList();
    if (!list)
    {
        OIC_LOG(ERROR, TAG, "No selected network");
        return CA_STATUS_FAILED;
    }

    size_t length = u_arraylist_length(list);
    for (size_t i = 0; i < length; i++)
    {
        void *ptrType = u_arraylist_get(list, i);
        if (ptrType == NULL)
        {
            continue;
        }

        CATransportAdapter_t connType = *(CATransportAdapter_t *)ptrType;

        res = CAGetAdapterIndex(connType, &index);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "unknown connectivity type!");
            continue;
        }

        if (g_adapterHandler[index].stopListenServer != NULL)
        {
            g_adapterHandler[index].stopListenServer();
        }
    }

    return CA_STATUS_OK;
}

CAResult_t CAStartDiscoveryServerAdapters()
{
    size_t index = 0;
    CAResult_t result = CA_STATUS_FAILED;

    u_arraylist_t *list = CAGetSelectedNetworkList();

    if (!list)
    {
        OIC_LOG(ERROR, TAG, "No selected network");
        return result;
    }

    size_t length = u_arraylist_length(list);
    for (size_t i = 0; i < length; i++)
    {
        void *ptrType = u_arraylist_get(list, i);

        if (ptrType == NULL)
        {
            continue;
        }

        CATransportAdapter_t connType = *(CATransportAdapter_t *)ptrType;

        if (CA_STATUS_OK != CAGetAdapterIndex(connType, &index))
        {
            OIC_LOG(DEBUG, TAG, "unknown connectivity type!");
            continue;
        }

        if (g_adapterHandler[index].startDiscoveryServer != NULL)
        {
            const CAResult_t tmp =
                g_adapterHandler[index].startDiscoveryServer();

            // Successful discovery if at least one adapter started.
            if (CA_STATUS_OK == tmp)
            {
                result = tmp;
            }
        }
    }

    return result;
}

bool CAIsLocalEndpoint(const CAEndpoint_t *ep)
{
    VERIFY_NON_NULL_RET(ep, TAG, "ep is null", false);

#ifdef IP_ADAPTER
    if (ep->adapter & CA_ADAPTER_IP)
    {
        return CAIPIsLocalEndpoint(ep);
    }
#endif /* IP_ADAPTER */

    //TODO: implement for the other adapters(EDR/LE/NFC)
    return false;
}

void CATerminateAdapters()
{
    for (size_t index = 0; index < g_numberOfAdapters; index++)
    {
        if (g_adapterHandler[index].terminate != NULL)
        {
            g_adapterHandler[index].terminate();
        }
    }

    OICFree(g_adapterHandler);
    g_adapterHandler = NULL;
    g_numberOfAdapters = 0;

    RemoveAllNetworkStateChangedCallback();
}

#ifdef SINGLE_THREAD
CAResult_t CAReadData()
{
    size_t index = 0;
    CAResult_t res = CA_STATUS_FAILED;
    u_arraylist_t *list = CAGetSelectedNetworkList();

    if (!list)
    {
        return CA_STATUS_FAILED;
    }

    size_t i = 0;
    for (i = 0; i < u_arraylist_length(list); i++)
    {
        void *ptrType = u_arraylist_get(list, i);
        if (NULL == ptrType)
        {
            OIC_LOG(ERROR, TAG, "get list fail");
            return CA_STATUS_FAILED;
        }

        CATransportAdapter_t connType = *(CATransportAdapter_t *) ptrType;

        res = CAGetAdapterIndex(connType, &index);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(DEBUG, TAG, "unknown connectivity type!");
            continue;
        }

        if (g_adapterHandler[index].readData != NULL)
        {
            g_adapterHandler[index].readData();
        }
    }

    return CA_STATUS_OK;
}
#endif
