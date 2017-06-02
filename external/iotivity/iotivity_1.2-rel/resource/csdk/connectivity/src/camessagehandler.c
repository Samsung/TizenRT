/* *****************************************************************
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
#include <stdint.h>

#include "cainterface.h"
#include "camessagehandler.h"
#include "caremotehandler.h"
#include "caprotocolmessage.h"
#include "logger.h"
#include "trace.h"
#ifndef WITH_UPSTREAM_LIBCOAP
#include "coap/config.h"
#endif
#include "oic_malloc.h"
#include "canetworkconfigurator.h"
#include "caadapterutils.h"
#include "cainterfacecontroller.h"
#include "caretransmission.h"
#include "oic_string.h"

#ifdef WITH_BWT
#include "cablockwisetransfer.h"
#endif

#ifndef  SINGLE_THREAD
#include "uqueue.h"
#include "cathreadpool.h" /* for thread pool */
#include "caqueueingthread.h"

#define SINGLE_HANDLE
#define MAX_THREAD_POOL_SIZE    20

// thread pool handle
static ca_thread_pool_t g_threadPoolHandle = NULL;

// message handler main thread
static CAQueueingThread_t g_sendThread;
static CAQueueingThread_t g_receiveThread;

#else
#define CA_MAX_RT_ARRAY_SIZE    3
#endif  // SINGLE_THREAD

#define TAG "OIC_CA_MSG_HANDLE"

static CARetransmission_t g_retransmissionContext;

// handler field
static CARequestCallback g_requestHandler = NULL;
static CAResponseCallback g_responseHandler = NULL;
static CAErrorCallback g_errorHandler = NULL;
static CANetworkMonitorCallback g_nwMonitorHandler = NULL;

static void CAErrorHandler(const CAEndpoint_t *endpoint,
                           const void *data, uint32_t dataLen,
                           CAResult_t result);

static CAData_t* CAGenerateHandlerData(const CAEndpoint_t *endpoint,
                                       const CARemoteId_t *identity,
                                       const void *data, CADataType_t dataType);

static void CASendErrorInfo(const CAEndpoint_t *endpoint, const CAInfo_t *info,
                            CAResult_t result);

#ifdef SINGLE_THREAD
static void CAProcessReceivedData(CAData_t *data);
#endif
static void CADestroyData(void *data, uint32_t size);
static void CALogPayloadInfo(CAInfo_t *info);
static bool CADropSecondMessage(CAHistory_t *history, const CAEndpoint_t *endpoint, uint16_t id,
                                CAToken_t token, uint8_t tokenLength);

/**
 * print send / receive message of CoAP.
 * @param[in] data      CA information which has send/receive message and endpoint.
 * @param[in] pdu       CoAP pdu low data.
 */
static void CALogPDUInfo(const CAData_t *data, const coap_pdu_t *pdu);

#ifndef ARDUINO
static char g_headerBuffer[MAX_LOG_BUFFER_SIZE] = {0};
static size_t g_headerIndex = 0;
static void CASamsungLogMessage(const CAData_t *data, const coap_pdu_t *pdu);
#endif

#ifdef WITH_BWT
void CAAddDataToSendThread(CAData_t *data)
{
    VERIFY_NON_NULL_VOID(data, TAG, "data");

    // add thread
    CAQueueingThreadAddData(&g_sendThread, data, sizeof(CAData_t));
}

void CAAddDataToReceiveThread(CAData_t *data)
{
    VERIFY_NON_NULL_VOID(data, TAG, "data");

    // add thread
    CAQueueingThreadAddData(&g_receiveThread, data, sizeof(CAData_t));
}
#endif

static bool CAIsSelectedNetworkAvailable()
{
    u_arraylist_t *list = CAGetSelectedNetworkList();
    if (!list || u_arraylist_length(list) == 0)
    {
        OIC_LOG(ERROR, TAG, "No selected network");
        return false;
    }

    return true;
}

static CAData_t* CAGenerateHandlerData(const CAEndpoint_t *endpoint,
                                       const CARemoteId_t *identity,
                                       const void *data, CADataType_t dataType)
{
    OIC_LOG(DEBUG, TAG, "CAGenerateHandlerData IN");
    CAInfo_t *info = NULL;
    CAData_t *cadata = (CAData_t *) OICCalloc(1, sizeof(CAData_t));
    if (!cadata)
    {
        OIC_LOG(ERROR, TAG, "memory allocation failed");
        return NULL;
    }
#ifdef SINGLE_THREAD
    CAEndpoint_t* ep = endpoint;
#else
    CAEndpoint_t* ep = CACloneEndpoint(endpoint);
    if (!ep)
    {
        OIC_LOG(ERROR, TAG, "endpoint clone failed");
        goto exit;
    }
#endif

    OIC_LOG_V(DEBUG, TAG, "address : %s", ep->addr);

    if (CA_RESPONSE_DATA == dataType)
    {
        CAResponseInfo_t* resInfo = (CAResponseInfo_t*)OICCalloc(1, sizeof(CAResponseInfo_t));
        if (!resInfo)
        {
            OIC_LOG(ERROR, TAG, "memory allocation failed");
            goto exit;
        }

        CAResult_t result = CAGetResponseInfoFromPDU(data, resInfo, endpoint);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, TAG, "CAGetResponseInfoFromPDU Failed");
            CADestroyResponseInfoInternal(resInfo);
            goto exit;
        }
        cadata->responseInfo = resInfo;
        info = &resInfo->info;
        if (identity)
        {
            info->identity = *identity;
        }
        OIC_LOG(DEBUG, TAG, "Response Info :");
        CALogPayloadInfo(info);
    }
    else if (CA_REQUEST_DATA == dataType)
    {
        CARequestInfo_t* reqInfo = (CARequestInfo_t*)OICCalloc(1, sizeof(CARequestInfo_t));
        if (!reqInfo)
        {
            OIC_LOG(ERROR, TAG, "memory allocation failed");
            goto exit;
        }

        CAResult_t result = CAGetRequestInfoFromPDU(data, endpoint, reqInfo);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, TAG, "CAGetRequestInfoFromPDU failed");
            CADestroyRequestInfoInternal(reqInfo);
            goto exit;
        }

        if (CADropSecondMessage(&caglobals.ca.requestHistory, endpoint, reqInfo->info.messageId,
                                reqInfo->info.token, reqInfo->info.tokenLength))
        {
            OIC_LOG(INFO, TAG, "Second Request with same Token, Drop it");
            CADestroyRequestInfoInternal(reqInfo);
            goto exit;
        }

        cadata->requestInfo = reqInfo;
        info = &reqInfo->info;
        if (identity)
        {
            info->identity = *identity;
        }
        OIC_LOG(DEBUG, TAG, "Request Info :");
        CALogPayloadInfo(info);
   }
    else if (CA_ERROR_DATA == dataType)
    {
        CAErrorInfo_t *errorInfo = (CAErrorInfo_t *)OICCalloc(1, sizeof (CAErrorInfo_t));
        if (!errorInfo)
        {
            OIC_LOG(ERROR, TAG, "Memory allocation failed!");
            goto exit;
        }

        CAResult_t result = CAGetErrorInfoFromPDU(data, endpoint, errorInfo);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, TAG, "CAGetErrorInfoFromPDU failed");
            OICFree(errorInfo);
            goto exit;
        }

        cadata->errorInfo = errorInfo;
        info = &errorInfo->info;
        if (identity)
        {
            info->identity = *identity;
        }
        OIC_LOG(DEBUG, TAG, "error Info :");
        CALogPayloadInfo(info);
    }

    cadata->remoteEndpoint = ep;
    cadata->dataType = dataType;

    OIC_LOG(DEBUG, TAG, "CAGenerateHandlerData OUT");
    return cadata;

exit:
    OICFree(cadata);
#ifndef SINGLE_THREAD
    CAFreeEndpoint(ep);
#endif
    return NULL;
}

static void CATimeoutCallback(const CAEndpoint_t *endpoint, const void *pdu, uint32_t size)
{
    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL_VOID(pdu, TAG, "pdu");
#ifdef SINGLE_THREAD
    CAEndpoint_t* ep = endpoint;
#else
    CAEndpoint_t* ep = CACloneEndpoint(endpoint);
    if (!ep)
    {
        OIC_LOG(ERROR, TAG, "clone failed");
        return;
    }
#endif

    CAResponseInfo_t* resInfo = (CAResponseInfo_t*)OICCalloc(1, sizeof(CAResponseInfo_t));

    if (!resInfo)
    {
        OIC_LOG(ERROR, TAG, "calloc failed");
#ifndef SINGLE_THREAD
        CAFreeEndpoint(ep);
#endif
        return;
    }

    resInfo->result = CA_RETRANSMIT_TIMEOUT;
    resInfo->info.type = CAGetMessageTypeFromPduBinaryData(pdu, size);
    resInfo->info.messageId = CAGetMessageIdFromPduBinaryData(pdu, size);

    CAResult_t res = CAGetTokenFromPDU((const coap_hdr_transport_t *) pdu, &(resInfo->info),
                                       endpoint);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "fail to get Token from retransmission list");
        CADestroyResponseInfoInternal(resInfo);
#ifndef SINGLE_THREAD
        CAFreeEndpoint(ep);
#endif
        return;
    }

    CAData_t *cadata = (CAData_t *) OICCalloc(1, sizeof(CAData_t));
    if (NULL == cadata)
    {
        OIC_LOG(ERROR, TAG, "memory allocation failed !");
#ifndef SINGLE_THREAD
        CAFreeEndpoint(ep);
#endif
        CADestroyResponseInfoInternal(resInfo);
        return;
    }

    cadata->type = SEND_TYPE_UNICAST;
    cadata->remoteEndpoint = ep;
    cadata->requestInfo = NULL;
    cadata->responseInfo = resInfo;

#ifdef WITH_BWT
    if (CAIsSupportedBlockwiseTransfer(endpoint->adapter))
    {
        res = CARemoveBlockDataFromListWithSeed(resInfo->info.token, resInfo->info.tokenLength,
                                                endpoint->port);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CARemoveBlockDataFromListWithSeed failed");
        }
    }
#endif // WITH_BWT

#ifdef SINGLE_THREAD
    CAProcessReceivedData(cadata);
#else
    CAQueueingThreadAddData(&g_receiveThread, cadata, sizeof(CAData_t));
#endif
}

static void CADestroyData(void *data, uint32_t size)
{
    OIC_LOG(DEBUG, TAG, "CADestroyData IN");
    if ((size_t)size < sizeof(CAData_t))
    {
        OIC_LOG_V(ERROR, TAG, "Destroy data too small %p %d", data, size);
    }
    CAData_t *cadata = (CAData_t *) data;

    if (NULL == cadata)
    {
        OIC_LOG(ERROR, TAG, "cadata is NULL");
        return;
    }
#ifndef SINGLE_THREAD
    if (NULL != cadata->remoteEndpoint)
    {
        CAFreeEndpoint(cadata->remoteEndpoint);
    }
#endif

    if (NULL != cadata->requestInfo)
    {
        CADestroyRequestInfoInternal((CARequestInfo_t *) cadata->requestInfo);
    }

    if (NULL != cadata->responseInfo)
    {
        CADestroyResponseInfoInternal((CAResponseInfo_t *) cadata->responseInfo);
    }

    if (NULL != cadata->errorInfo)
    {
        CADestroyErrorInfoInternal(cadata->errorInfo);
    }

    OICFree(cadata);
    OIC_LOG(DEBUG, TAG, "CADestroyData OUT");
}

#ifdef SINGLE_THREAD
static void CAProcessReceivedData(CAData_t *data)
{
    OIC_LOG(DEBUG, TAG, "CAProcessReceivedData IN");
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "thread data error!!");
        return;
    }

    // parse the data and call the callbacks.
    // #1 parse the data
    // #2 get endpoint
    CAEndpoint_t *rep = (CAEndpoint_t *)(data->remoteEndpoint);
    if (!rep)
    {
        OIC_LOG(ERROR, TAG, "remoteEndpoint error!!");
        return;
    }

    if (data->requestInfo && g_requestHandler)
    {
        g_requestHandler(rep, data->requestInfo);
    }
    else if (data->responseInfo && g_responseHandler)
    {
        g_responseHandler(rep, data->responseInfo);
    }
    else if (data->errorInfo && g_errorHandler)
    {
        g_errorHandler(rep, data->errorInfo);
    }

    CADestroyData(data, sizeof(CAData_t));

    OIC_LOG(DEBUG, TAG, "CAProcessReceivedData OUT");
}
#endif

#ifndef SINGLE_THREAD
static void CAReceiveThreadProcess(void *threadData)
{
#ifndef SINGLE_HANDLE
    CAData_t *data = (CAData_t *) threadData;
    OIC_TRACE_BEGIN(%s:CAProcessReceivedData, TAG);
    CAProcessReceivedData(data);
    OIC_TRACE_END();
#else
    (void)threadData;
#endif
}
#endif // SINGLE_THREAD

static CAResult_t CAProcessMulticastData(const CAData_t *data)
{
    VERIFY_NON_NULL(data, TAG, "data");
    VERIFY_NON_NULL(data->remoteEndpoint, TAG, "remoteEndpoint");

    coap_pdu_t *pdu = NULL;
    CAInfo_t *info = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;
    CAResult_t res = CA_SEND_FAILED;

    if (!data->requestInfo && !data->responseInfo)
    {
        OIC_LOG(ERROR, TAG, "request or response info is empty");
        return res;
    }

    if (data->requestInfo)
    {
        OIC_LOG(DEBUG, TAG, "requestInfo is available..");

        info = &data->requestInfo->info;
        pdu = CAGeneratePDU(CA_GET, info, data->remoteEndpoint, &options, &transport);
    }
    else if (data->responseInfo)
    {
        OIC_LOG(DEBUG, TAG, "responseInfo is available..");

        info = &data->responseInfo->info;
        pdu = CAGeneratePDU(data->responseInfo->result, info, data->remoteEndpoint,
                            &options, &transport);
    }

    if (!pdu)
    {
        OIC_LOG(ERROR,TAG,"Failed to generate multicast PDU");
        CASendErrorInfo(data->remoteEndpoint, info, CA_SEND_FAILED);
        coap_delete_list(options);
        return res;
    }

#ifdef WITH_BWT
    if (CAIsSupportedBlockwiseTransfer(data->remoteEndpoint->adapter))
    {
        // Blockwise transfer
        res = CAAddBlockOption(&pdu, info, data->remoteEndpoint, &options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(DEBUG, TAG, "CAAddBlockOption has failed");
            goto exit;
        }
    }
#endif // WITH_BWT

    CALogPDUInfo(data, pdu);

    res = CASendMulticastData(data->remoteEndpoint, pdu->transport_hdr, pdu->length, data->dataType);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "send failed:%d", res);
        goto exit;
    }

    coap_delete_list(options);
    coap_delete_pdu(pdu);
    return res;

exit:
    CAErrorHandler(data->remoteEndpoint, pdu->transport_hdr, pdu->length, res);
    coap_delete_list(options);
    coap_delete_pdu(pdu);
    return res;
}

static CAResult_t CAProcessSendData(const CAData_t *data)
{
    VERIFY_NON_NULL(data, TAG, "data");
    VERIFY_NON_NULL(data->remoteEndpoint, TAG, "remoteEndpoint");

    CAResult_t res = CA_STATUS_FAILED;

    if (CA_NETWORK_COMMAND == data->dataType)
    {
        if (CA_REQ_DISCONNECT == data->eventInfo)
        {
#ifdef TCP_ADAPTER
            // request TCP disconnect
            if (CA_ADAPTER_TCP == data->remoteEndpoint->adapter)
            {
                OIC_LOG(INFO, TAG, "request TCP disconnect");
                return CADisconnectSession(data->remoteEndpoint);
            }
#endif
        }
    }

    CASendDataType_t type = data->type;

    coap_pdu_t *pdu = NULL;
    CAInfo_t *info = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    if (SEND_TYPE_UNICAST == type)
    {
        OIC_LOG(DEBUG,TAG,"Unicast message");

#ifdef ROUTING_GATEWAY
        /*
         * When forwarding a packet, do not attempt retransmission as its the responsibility of
         * packet originator node
         */
        bool skipRetransmission = false;
#endif

        if (NULL != data->requestInfo)
        {
            OIC_LOG(DEBUG, TAG, "requestInfo is available..");

            info = &data->requestInfo->info;
#ifdef ROUTING_GATEWAY
            skipRetransmission = data->requestInfo->info.skipRetransmission;
#endif
            pdu = CAGeneratePDU(data->requestInfo->method, info, data->remoteEndpoint,
                                &options, &transport);
        }
        else if (NULL != data->responseInfo)
        {
            OIC_LOG(DEBUG, TAG, "responseInfo is available..");

            info = &data->responseInfo->info;
#ifdef ROUTING_GATEWAY
            skipRetransmission = data->responseInfo->info.skipRetransmission;
#endif
            pdu = CAGeneratePDU(data->responseInfo->result, info, data->remoteEndpoint,
                                &options, &transport);
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "request info, response info is empty");
            return CA_STATUS_INVALID_PARAM;
        }

        // interface controller function call.
        if (NULL != pdu)
        {
#ifdef WITH_BWT
            if (CAIsSupportedBlockwiseTransfer(data->remoteEndpoint->adapter))
            {
                // Blockwise transfer
                if (NULL != info)
                {
                    CAResult_t res = CAAddBlockOption(&pdu, info,
                                                      data->remoteEndpoint,
                                                      &options);
                    if (CA_STATUS_OK != res)
                    {
                        OIC_LOG(INFO, TAG, "to write block option has failed");
                        CAErrorHandler(data->remoteEndpoint, pdu->transport_hdr, pdu->length, res);
                        coap_delete_list(options);
                        coap_delete_pdu(pdu);
                        return res;
                    }
                }
            }
#endif // WITH_BWT
            CALogPDUInfo(data, pdu);

            OIC_LOG_V(INFO, TAG, "CASendUnicastData type : %d", data->dataType);
            res = CASendUnicastData(data->remoteEndpoint, pdu->transport_hdr, pdu->length, data->dataType);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG_V(ERROR, TAG, "send failed:%d", res);
                CAErrorHandler(data->remoteEndpoint, pdu->transport_hdr, pdu->length, res);
                coap_delete_list(options);
                coap_delete_pdu(pdu);
                return res;
            }

#ifdef WITH_TCP
            if (CAIsSupportedCoAPOverTCP(data->remoteEndpoint->adapter))
            {
                OIC_LOG(INFO, TAG, "retransmission will be not worked");
            }
            else
#endif
#ifdef ROUTING_GATEWAY
            if (!skipRetransmission)
#endif
            {
                // for retransmission
                res = CARetransmissionSentData(&g_retransmissionContext,
                                               data->remoteEndpoint,
                                               data->dataType,
                                               pdu->transport_hdr, pdu->length);
                if ((CA_STATUS_OK != res) && (CA_NOT_SUPPORTED != res))
                {
                    //when retransmission not supported this will return CA_NOT_SUPPORTED, ignore
                    OIC_LOG_V(INFO, TAG, "retransmission is not enabled due to error, res : %d", res);
                    coap_delete_list(options);
                    coap_delete_pdu(pdu);
                    return res;
                }
            }

            coap_delete_list(options);
            coap_delete_pdu(pdu);
        }
        else
        {
            OIC_LOG(ERROR,TAG,"Failed to generate unicast PDU");
            CASendErrorInfo(data->remoteEndpoint, info, CA_SEND_FAILED);
            return CA_SEND_FAILED;
        }
    }
    else if (SEND_TYPE_MULTICAST == type)
    {
        OIC_LOG(DEBUG,TAG,"Multicast message");
#ifdef WITH_TCP
        /*
         * If CoAP over TCP is enabled, the CoAP pdu wont be same for IP and other adapters.
         * That's why we need to generate two pdu's, one for IP and second for other transports.
         * Two possible cases we might have to split: a) when adapter is CA_DEFAULT_ADAPTER
         * b) when one of the adapter is IP adapter(ex: CA_ADAPTER_IP | CA_ADAPTER_GATT_BTLE)
         */
        if (data->remoteEndpoint->adapter == CA_DEFAULT_ADAPTER ||
                (CA_ADAPTER_IP & data->remoteEndpoint->adapter &&
                    CA_ADAPTER_IP != data->remoteEndpoint->adapter))
        {
            if (data->remoteEndpoint->adapter == CA_DEFAULT_ADAPTER)
            {
                data->remoteEndpoint->adapter = CA_ALL_ADAPTERS ^ CA_ADAPTER_IP;
            }
            else
            {
                data->remoteEndpoint->adapter = data->remoteEndpoint->adapter ^ CA_ADAPTER_IP;
            }
            CAProcessMulticastData(data);
            data->remoteEndpoint->adapter = CA_ADAPTER_IP;
            CAProcessMulticastData(data);
        }
        else
        {
            CAProcessMulticastData(data);
        }
#else
        CAProcessMulticastData(data);
#endif
    }
    return CA_STATUS_OK;
}

#ifndef SINGLE_THREAD
static void CASendThreadProcess(void *threadData)
{
    CAData_t *data = (CAData_t *) threadData;
    OIC_TRACE_BEGIN(%s:CAProcessSendData, TAG);
    CAProcessSendData(data);
    OIC_TRACE_END();
}
#endif

/*
 * If a second message arrives with the same message ID, token and the other address
 * family, drop it.  Typically, IPv6 beats IPv4, so the IPv4 message is dropped.
 */
static bool CADropSecondMessage(CAHistory_t *history, const CAEndpoint_t *ep, uint16_t id,
                                CAToken_t token, uint8_t tokenLength)
{
    if (!ep)
    {
        return true;
    }
    if (ep->adapter != CA_ADAPTER_IP)
    {
        return false;
    }
    if (!caglobals.ip.dualstack)
    {
        return false;
    }

    if (tokenLength > CA_MAX_TOKEN_LEN)
    {
        /*
         * If token length is more than CA_MAX_TOKEN_LEN,
         * we compare the first CA_MAX_TOKEN_LEN bytes only.
         */
        tokenLength = CA_MAX_TOKEN_LEN;
    }

    bool ret = false;
    CATransportFlags_t familyFlags = ep->flags & CA_IPFAMILY_MASK;

    for (size_t i = 0; i < sizeof(history->items) / sizeof(history->items[0]); i++)
    {
        CAHistoryItem_t *item = &(history->items[i]);
        if (id == item->messageId && tokenLength == item->tokenLength
            && memcmp(item->token, token, tokenLength) == 0)
        {
            if ((familyFlags ^ item->flags) == CA_IPFAMILY_MASK)
            {
                OIC_LOG_V(INFO, TAG, "IPv%c duplicate message ignored",
                          familyFlags & CA_IPV6 ? '6' : '4');
                ret = true;
                break;
            }
        }
    }

    history->items[history->nextIndex].flags = familyFlags;
    history->items[history->nextIndex].messageId = id;
    if (token && tokenLength)
    {
        memcpy(history->items[history->nextIndex].token, token, tokenLength);
        history->items[history->nextIndex].tokenLength = tokenLength;
    }

    if (++history->nextIndex >= HISTORYSIZE)
    {
        history->nextIndex = 0;
    }

    return ret;
}

static CAResult_t CAReceivedPacketCallback(const CASecureEndpoint_t *sep,
                                           const void *data, uint32_t dataLen)
{
    VERIFY_NON_NULL(sep, TAG, "remoteEndpoint");
    VERIFY_NON_NULL(data, TAG, "data");
    OIC_TRACE_BEGIN(%s:CAReceivedPacketCallback, TAG);

    if (0 == dataLen)
    {
        OIC_LOG(ERROR, TAG, "dataLen is zero");
        OIC_TRACE_END();

        return CA_STATUS_FAILED;
    }

    // samsung log
    OIC_LOG(DEBUG, TAG, "received pdu data :");
    if (dataLen < 32)
    {
        OIC_LOG_BUFFER(DEBUG, TAG,  data, dataLen);
    }
    else
    {
        OIC_LOG_BUFFER(DEBUG, TAG,  data, 32);
    }

    CAResult_t res = CA_STATUS_OK;
    uint32_t code = CA_NOT_FOUND;
    CAData_t *cadata = NULL;

    coap_pdu_t *pdu = (coap_pdu_t *) CAParsePDU((const char *) data, dataLen, &code,
                                                &(sep->endpoint));
    if (NULL == pdu)
    {
        OIC_LOG(ERROR, TAG, "Parse PDU failed");
        res = CA_STATUS_FAILED;
        goto exit;
    }

    OIC_LOG_V(DEBUG, TAG, "code = %d", code);
    if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
    {
        cadata = CAGenerateHandlerData(&(sep->endpoint), &(sep->identity), pdu, CA_REQUEST_DATA);
        if (!cadata)
        {
            OIC_LOG(ERROR, TAG, "CAReceivedPacketCallback, CAGenerateHandlerData failed!");
            coap_delete_pdu(pdu);
            goto exit;
        }
    }
    else
    {
        cadata = CAGenerateHandlerData(&(sep->endpoint), &(sep->identity), pdu, CA_RESPONSE_DATA);
        if (!cadata)
        {
            OIC_LOG(ERROR, TAG, "CAReceivedPacketCallback, CAGenerateHandlerData failed!");
            coap_delete_pdu(pdu);
            goto exit;
        }

#ifdef WITH_TCP
        if (CAIsSupportedCoAPOverTCP(sep->endpoint.adapter))
        {
            OIC_LOG(INFO, TAG, "retransmission is not supported");
        }
        else
#endif
        {
            // for retransmission
            void *retransmissionPdu = NULL;
            CARetransmissionReceivedData(&g_retransmissionContext, cadata->remoteEndpoint, pdu->transport_hdr,
                                         pdu->length, &retransmissionPdu);

            // get token from saved data in retransmission list
            if (retransmissionPdu && CA_EMPTY == code)
            {
                if (cadata->responseInfo)
                {
                    CAInfo_t *info = &cadata->responseInfo->info;
                    CAResult_t res = CAGetTokenFromPDU((const coap_hdr_transport_t *)retransmissionPdu,
                                                       info, &(sep->endpoint));
                    if (CA_STATUS_OK != res)
                    {
                        OIC_LOG(ERROR, TAG, "fail to get Token from retransmission list");
                        OICFree(info->token);
                        info->tokenLength = 0;
                    }
                }
            }
            OICFree(retransmissionPdu);
        }
    }

    cadata->type = SEND_TYPE_UNICAST;

    CALogPDUInfo(cadata, pdu);

#ifdef SINGLE_THREAD
    CAProcessReceivedData(cadata);
#else
#ifdef WITH_BWT
    if (CAIsSupportedBlockwiseTransfer(sep->endpoint.adapter))
    {
        CAResult_t res = CAReceiveBlockWiseData(pdu, &(sep->endpoint), cadata, dataLen);
        if (CA_NOT_SUPPORTED == res || CA_REQUEST_TIMEOUT == res)
        {
            OIC_LOG(DEBUG, TAG, "this message does not have block option");
            CAQueueingThreadAddData(&g_receiveThread, cadata, sizeof(CAData_t));
        }
        else
        {
            CADestroyData(cadata, sizeof(CAData_t));
        }
    }
    else
#endif
    {
        CAQueueingThreadAddData(&g_receiveThread, cadata, sizeof(CAData_t));
    }
#endif // SINGLE_THREAD

    coap_delete_pdu(pdu);

exit:
    OIC_LOG(DEBUG, TAG, "OUT - Recv Thread");
    OIC_TRACE_END();
    return res;
}

void CAHandleRequestResponseCallbacks()
{
#ifdef SINGLE_THREAD
    CAReadData();
    CARetransmissionBaseRoutine((void *)&g_retransmissionContext);
#else
#ifdef SINGLE_HANDLE
    // parse the data and call the callbacks.
    // #1 parse the data
    // #2 get endpoint

    oc_mutex_lock(g_receiveThread.threadMutex);

    u_queue_message_t *item = u_queue_get_element(g_receiveThread.dataQueue);

    oc_mutex_unlock(g_receiveThread.threadMutex);

    if (NULL == item || NULL == item->msg)
    {
        return;
    }

    // get endpoint
    CAData_t *td = (CAData_t *) item->msg;

    if (td->requestInfo && g_requestHandler)
    {
        OIC_LOG_V(DEBUG, TAG, "request callback : %d", td->requestInfo->info.numOptions);
        g_requestHandler(td->remoteEndpoint, td->requestInfo);
    }
    else if (td->responseInfo && g_responseHandler)
    {
        OIC_LOG_V(DEBUG, TAG, "response callback : %d", td->responseInfo->info.numOptions);
        g_responseHandler(td->remoteEndpoint, td->responseInfo);
    }
    else if (td->errorInfo && g_errorHandler)
    {
        OIC_LOG_V(DEBUG, TAG, "error callback error: %d", td->errorInfo->result);
        g_errorHandler(td->remoteEndpoint, td->errorInfo);
    }

    CADestroyData(item->msg, sizeof(CAData_t));
    OICFree(item);

#endif // SINGLE_HANDLE
#endif // SINGLE_THREAD
}

static CAData_t* CAPrepareSendData(const CAEndpoint_t *endpoint, const void *sendData,
                                   CADataType_t dataType)
{
    OIC_LOG(DEBUG, TAG, "CAPrepareSendData IN");

    CAData_t *cadata = (CAData_t *) OICCalloc(1, sizeof(CAData_t));
    if (!cadata)
    {
        OIC_LOG(ERROR, TAG, "memory allocation failed");
        return NULL;
    }

    if (CA_REQUEST_DATA == dataType)
    {
#ifdef SINGLE_THREAD
        CARequestInfo_t *request = (CARequestInfo_t *)sendData;
#else
        // clone request info
        CARequestInfo_t *request = CACloneRequestInfo((CARequestInfo_t *)sendData);
        if (!request)
        {
            OIC_LOG(ERROR, TAG, "CACloneRequestInfo failed");
            goto exit;
        }
#endif
        cadata->type = request->isMulticast ? SEND_TYPE_MULTICAST : SEND_TYPE_UNICAST;
        cadata->requestInfo =  request;
    }
    else if (CA_RESPONSE_DATA == dataType || CA_RESPONSE_FOR_RES == dataType)
    {
#ifdef SINGLE_THREAD
        CAResponseInfo_t *response = (CAResponseInfo_t *)sendData;
#else
        // clone response info
        CAResponseInfo_t *response = CACloneResponseInfo((CAResponseInfo_t *)sendData);
        if (!response)
        {
            OIC_LOG(ERROR, TAG, "CACloneResponseInfo failed");
            goto exit;
        }
#endif
        cadata->type = response->isMulticast ? SEND_TYPE_MULTICAST : SEND_TYPE_UNICAST;
        cadata->responseInfo = response;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "CAPrepareSendData unknown data type");
        goto exit;
    }

#ifdef SINGLE_THREAD
    CAEndpoint_t* ep = endpoint;
#else
    CAEndpoint_t* ep = CACloneEndpoint(endpoint);
    if (!ep)
    {
        OIC_LOG(ERROR, TAG, "endpoint clone failed");
        goto exit;
    }
#endif
    cadata->remoteEndpoint = ep;
    cadata->dataType = dataType;
    return cadata;

exit:
#ifndef SINGLE_THREAD
    CADestroyData(cadata, sizeof(CAData_t));
#else
    OICFree(cadata);
#endif
    return NULL;
}

CAResult_t CADetachSendNetworkReqMessage(const CAEndpoint_t *endpoint,
                                         CAConnectEvent_t event,
                                         CADataType_t dataType)
{
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");

    if (false == CAIsSelectedNetworkAvailable())
    {
        return CA_STATUS_FAILED;
    }

#ifndef SINGLE_THREAD
    CAData_t *cadata = (CAData_t *) OICCalloc(1, sizeof(CAData_t));
    if (!cadata)
    {
        OIC_LOG(ERROR, TAG, "cadata memory allocation failed");
        return CA_STATUS_FAILED;
    }

    CAEndpoint_t* ep = CACloneEndpoint(endpoint);
    if (!ep)
    {
        OIC_LOG(ERROR, TAG, "endpoint clone failed");
        OICFree(cadata);
        return CA_STATUS_FAILED;
    }

    cadata->remoteEndpoint = ep;
    cadata->eventInfo = event;
    cadata->dataType = dataType;

    CAQueueingThreadAddData(&g_sendThread, cadata, sizeof(CAData_t));
#endif

    return CA_STATUS_OK;
}

CAResult_t CADetachSendMessage(const CAEndpoint_t *endpoint, const void *sendMsg,
                               CADataType_t dataType)
{
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(sendMsg, TAG, "sendMsg");

    if (false == CAIsSelectedNetworkAvailable())
    {
        return CA_STATUS_FAILED;
    }

#ifdef ARDUINO
    // If max retransmission queue is reached, then don't handle new request
    if (CA_MAX_RT_ARRAY_SIZE == u_arraylist_length(g_retransmissionContext.dataList))
    {
        OIC_LOG(ERROR, TAG, "max RT queue size reached!");
        return CA_SEND_FAILED;
    }
#endif // ARDUINO

    CAData_t *data = CAPrepareSendData(endpoint, sendMsg, dataType);
    if(!data)
    {
        OIC_LOG(ERROR, TAG, "CAPrepareSendData failed");
        return CA_MEMORY_ALLOC_FAILED;
    }

    OIC_LOG_V(INFO_PRIVATE, TAG, "DID of endpoint of this message is %s", endpoint->remoteId);

#ifdef SINGLE_THREAD
    CAResult_t result = CAProcessSendData(data);
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CAProcessSendData failed");
        OICFree(data);
        return result;
    }

    OICFree(data);

#else
#ifdef WITH_BWT
    if (CAIsSupportedBlockwiseTransfer(endpoint->adapter))
    {
        // send block data
        CAResult_t res = CASendBlockWiseData(data);
        if (CA_NOT_SUPPORTED == res)
        {
            OIC_LOG(DEBUG, TAG, "normal msg will be sent");
            CAQueueingThreadAddData(&g_sendThread, data, sizeof(CAData_t));
            return CA_STATUS_OK;
        }
        else
        {
            CADestroyData(data, sizeof(CAData_t));
        }
        CACheckAndDeleteTimedOutBlockData();
        return res;
    }
    else
#endif // WITH_BWT
    {
        CAQueueingThreadAddData(&g_sendThread, data, sizeof(CAData_t));
    }
#endif // SINGLE_THREAD

    return CA_STATUS_OK;
}

void CASetInterfaceCallbacks(CARequestCallback ReqHandler, CAResponseCallback RespHandler,
                             CAErrorCallback errorHandler)
{
    g_requestHandler = ReqHandler;
    g_responseHandler = RespHandler;
    g_errorHandler = errorHandler;
}

void CASetNetworkMonitorCallback(CANetworkMonitorCallback nwMonitorHandler)
{
    g_nwMonitorHandler = nwMonitorHandler;
}

CAResult_t CAInitializeMessageHandler(CATransportAdapter_t transportType)
{
    CASetPacketReceivedCallback(CAReceivedPacketCallback);
    CASetErrorHandleCallback(CAErrorHandler);

#ifndef SINGLE_THREAD
    // create thread pool
    CAResult_t res = ca_thread_pool_init(MAX_THREAD_POOL_SIZE, &g_threadPoolHandle);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "thread pool initialize error.");
        return res;
    }

    // send thread initialize
    res = CAQueueingThreadInitialize(&g_sendThread, g_threadPoolHandle,
                                     CASendThreadProcess, CADestroyData);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize send queue thread");
        return res;
    }

    // start send thread
#ifndef __TIZENRT__
    res = CAQueueingThreadStart(&g_sendThread);
#else
    res = CAQueueingThreadStart(&g_sendThread, "IoT_MessageHandlerQueue");
#endif
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "thread start error(send thread).");
        return res;
    }

    // receive thread initialize
    res = CAQueueingThreadInitialize(&g_receiveThread, g_threadPoolHandle,
                                     CAReceiveThreadProcess, CADestroyData);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize receive queue thread");
        return res;
    }

#ifndef SINGLE_HANDLE // This will be enabled when RI supports multi threading
    // start receive thread
    res = CAQueueingThreadStart(&g_receiveThread);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "thread start error(receive thread).");
        return res;
    }
#endif // SINGLE_HANDLE

    // retransmission initialize
    res = CARetransmissionInitialize(&g_retransmissionContext, g_threadPoolHandle,
                                     CASendUnicastData, CATimeoutCallback, NULL);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize Retransmission.");
        return res;
    }

#ifdef WITH_BWT
    // block-wise transfer initialize
    res = CAInitializeBlockWiseTransfer(CAAddDataToSendThread, CAAddDataToReceiveThread);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize BlockWiseTransfer.");
        return res;
    }
#endif

    // start retransmission
    res = CARetransmissionStart(&g_retransmissionContext);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "thread start error(retransmission thread).");
        return res;
    }

    // initialize interface adapters by controller
    CAInitializeAdapters(g_threadPoolHandle, transportType);
#else
    // retransmission initialize
    CAResult_t res = CARetransmissionInitialize(&g_retransmissionContext, NULL, CASendUnicastData,
                                                CATimeoutCallback, NULL);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize Retransmission.");
        return res;
    }

    CAInitializeAdapters();
#endif // SINGLE_THREAD

    return CA_STATUS_OK;
}

void CATerminateMessageHandler()
{
#ifndef SINGLE_THREAD
    CATransportAdapter_t connType;
    u_arraylist_t *list = CAGetSelectedNetworkList();
    uint32_t length = u_arraylist_length(list);

    uint32_t i = 0;
    for (i = 0; i < length; i++)
    {
        void* ptrType = u_arraylist_get(list, i);

        if (NULL == ptrType)
        {
            continue;
        }

        connType = *(CATransportAdapter_t *)ptrType;
        CAStopAdapter(connType);
    }

    // stop retransmission
    if (NULL != g_retransmissionContext.threadMutex)
    {
        CARetransmissionStop(&g_retransmissionContext);
    }

    // stop thread
    // delete thread data
    if (NULL != g_sendThread.threadMutex)
    {
        CAQueueingThreadStop(&g_sendThread);
    }

    // stop thread
    // delete thread data
    if (NULL != g_receiveThread.threadMutex)
    {
#ifndef SINGLE_HANDLE // This will be enabled when RI supports multi threading
        CAQueueingThreadStop(&g_receiveThread);
#endif
    }

    // destroy thread pool
    if (NULL != g_threadPoolHandle)
    {
        ca_thread_pool_free(g_threadPoolHandle);
        g_threadPoolHandle = NULL;
    }

#ifdef WITH_BWT
    CATerminateBlockWiseTransfer();
#endif
    CARetransmissionDestroy(&g_retransmissionContext);
    CAQueueingThreadDestroy(&g_sendThread);
    CAQueueingThreadDestroy(&g_receiveThread);

    // terminate interface adapters by controller
    CATerminateAdapters();
#else
    // terminate interface adapters by controller
    CATerminateAdapters();

    // stop retransmission
    CARetransmissionStop(&g_retransmissionContext);
    CARetransmissionDestroy(&g_retransmissionContext);
#endif // SINGLE_THREAD
}

static void CALogPayloadInfo(CAInfo_t *info)
{
    if (info)
    {
        if (info->options)
        {
            for (uint32_t i = 0; i < info->numOptions; i++)
            {
                OIC_LOG_V(DEBUG, TAG, "optionID: %u", info->options[i].optionID);

                OIC_LOG_V(DEBUG, TAG, "list: %s", info->options[i].optionData);
            }
        }

        if (info->payload)
        {
            OIC_LOG_V(DEBUG, TAG, "payload: %p(%zu)", info->payload,
                      info->payloadSize);
        }

        if (info->token)
        {
            OIC_LOG(DEBUG, TAG, "token:");
            OIC_LOG_BUFFER(DEBUG, TAG, (const uint8_t *) info->token,
                           info->tokenLength);
        }
        OIC_LOG_V(DEBUG, TAG, "msgID: %u", info->messageId);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "info is NULL, cannot output log data");
    }
}

void CAErrorHandler(const CAEndpoint_t *endpoint,
                    const void *data, uint32_t dataLen,
                    CAResult_t result)
{
    OIC_LOG(DEBUG, TAG, "CAErrorHandler IN");
    VERIFY_NON_NULL_VOID(endpoint, TAG, "remoteEndpoint");
    VERIFY_NON_NULL_VOID(data, TAG, "data");

    if (0 == dataLen)
    {
        OIC_LOG(ERROR, TAG, "dataLen is zero");
        return;
    }

#ifndef SINGLE_THREAD
    uint32_t code = CA_NOT_FOUND;
    //Do not free remoteEndpoint and data. Currently they will be freed in data thread
    //Get PDU data
    coap_pdu_t *pdu = (coap_pdu_t *)CAParsePDU((const char *)data, dataLen, &code, endpoint);
    if (NULL == pdu)
    {
        OIC_LOG(ERROR, TAG, "Parse PDU failed");
        return;
    }

    CAData_t *cadata = CAGenerateHandlerData(endpoint, NULL, pdu, CA_ERROR_DATA);
    if (!cadata)
    {
        OIC_LOG(ERROR, TAG, "CAErrorHandler, CAGenerateHandlerData failed!");
        coap_delete_pdu(pdu);
        return;
    }

    cadata->errorInfo->result = result;

    CAQueueingThreadAddData(&g_receiveThread, cadata, sizeof(CAData_t));
    coap_delete_pdu(pdu);
#else
    (void)result;
#endif

    OIC_LOG(DEBUG, TAG, "CAErrorHandler OUT");
    return;
}

static void CASendErrorInfo(const CAEndpoint_t *endpoint, const CAInfo_t *info, CAResult_t result)
{
    OIC_LOG(DEBUG, TAG, "CASendErrorInfo IN");
#ifndef SINGLE_THREAD
    CAData_t *cadata = (CAData_t *) OICCalloc(1, sizeof(CAData_t));
    if (!cadata)
    {
        OIC_LOG(ERROR, TAG, "cadata memory allocation failed");
        return;
    }

    CAEndpoint_t* ep = CACloneEndpoint(endpoint);
    if (!ep)
    {
        OIC_LOG(ERROR, TAG, "endpoint clone failed");
        OICFree(cadata);
        return;
    }

    CAErrorInfo_t *errorInfo = (CAErrorInfo_t *)OICCalloc(1, sizeof (CAErrorInfo_t));
    if (!errorInfo)
    {
        OIC_LOG(ERROR, TAG, "errorInfo memory allocation failed");
        OICFree(cadata);
        CAFreeEndpoint(ep);
        return;
    }

    CAResult_t res = CACloneInfo(info, &errorInfo->info);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "info clone failed");
        OICFree(cadata);
        OICFree(errorInfo);
        CAFreeEndpoint(ep);
        return;
    }

    errorInfo->result = result;
    cadata->remoteEndpoint = ep;
    cadata->errorInfo = errorInfo;
    cadata->dataType = CA_ERROR_DATA;

    CAQueueingThreadAddData(&g_receiveThread, cadata, sizeof(CAData_t));
#endif
    OIC_LOG(DEBUG, TAG, "CASendErrorInfo OUT");
}

#ifndef ARDUINO
static void CALogPDUInfo(const CAData_t *data, const coap_pdu_t *pdu)
{
    OIC_LOG(DEBUG, TAG, "CALogPDUInfo");

    VERIFY_NON_NULL_VOID(data, TAG, "data");
    VERIFY_NON_NULL_VOID(pdu, TAG, "pdu");
    OIC_TRACE_BEGIN(%s:CALogPDUInfo, TAG);

    OIC_LOG(INFO, ANALYZER_TAG, "=================================================");
    if(SEND_TYPE_MULTICAST == data->type)
    {
        OIC_LOG(INFO, ANALYZER_TAG, "Is Multicast = true");
    }
    else
    {
        OIC_LOG(INFO, ANALYZER_TAG, "Is Multicast = false");
    }

    if (NULL != data->remoteEndpoint)
    {
        CALogAdapterTypeInfo(data->remoteEndpoint->adapter);
        OIC_LOG_V(INFO, ANALYZER_TAG, "Address = [%s]:[%d]", data->remoteEndpoint->addr,
                  data->remoteEndpoint->port);
    }

    switch(data->dataType)
    {
        case CA_REQUEST_DATA:
            OIC_LOG(INFO, ANALYZER_TAG, "Data Type = [CA_REQUEST_DATA]");
            break;
        case CA_RESPONSE_DATA:
            OIC_LOG(INFO, ANALYZER_TAG, "Data Type = [CA_RESPONSE_DATA]");
            break;
        case CA_ERROR_DATA:
            OIC_LOG(INFO, ANALYZER_TAG, "Data Type = [CA_ERROR_DATA]");
            break;
        case CA_RESPONSE_FOR_RES:
            OIC_LOG(INFO, ANALYZER_TAG, "Data Type = [CA_RESPONSE_FOR_RES]");
            break;
        default:
            OIC_LOG_V(INFO, ANALYZER_TAG, "Data Type = [%d]", data->dataType);
            break;
    }

    const CAInfo_t *info = NULL;
    if (NULL != data->requestInfo)
    {
        switch(data->requestInfo->method)
        {
            case CA_GET:
                OIC_LOG(INFO, ANALYZER_TAG, "Method = [GET]");
                break;
            case CA_POST:
                OIC_LOG(INFO, ANALYZER_TAG, "Method = [POST]");
                break;
            case CA_PUT:
                OIC_LOG(INFO, ANALYZER_TAG, "Method = [PUT]");
                break;
            case CA_DELETE:
                OIC_LOG(INFO, ANALYZER_TAG, "Method = [DELETE]");
                break;
            default:
                OIC_LOG_V(INFO, ANALYZER_TAG, "Method = [%d]", data->requestInfo->method);
                break;
        }
        info = &data->requestInfo->info;
    }

    if (NULL != data->responseInfo)
    {
        OIC_LOG_V(INFO, ANALYZER_TAG, "result code = [%d]", data->responseInfo->result);
        info = &data->responseInfo->info;
    }

    if (pdu->transport_hdr)
    {
        OIC_LOG_V(INFO, ANALYZER_TAG, "Msg ID = [%d]", pdu->transport_hdr->udp.id);
    }

    if (info)
    {
        OIC_LOG(INFO, ANALYZER_TAG, "Coap Token");
        OIC_LOG_BUFFER(INFO, ANALYZER_TAG, (const uint8_t *) info->token, info->tokenLength);
        OIC_TRACE_BUFFER("OIC_CA_MSG_HANDLE:CALogPDUInfo:token",
                         (const uint8_t *) info->token, info->tokenLength);
        OIC_LOG_V(INFO, ANALYZER_TAG, "Res URI = [%s]", info->resourceUri);
        OIC_TRACE_MARK(%s:CALogPDUInfo:uri:%s, TAG, info->resourceUri);

        if (CA_FORMAT_APPLICATION_CBOR == info->payloadFormat)
        {
            OIC_LOG(INFO, ANALYZER_TAG, "Payload Format = [CA_FORMAT_APPLICATION_CBOR]");
        }
        else
        {
            OIC_LOG_V(INFO, ANALYZER_TAG, "Payload Format = [%d]", info->payloadFormat);
        }
    }

    size_t payloadLen = (pdu->data) ? (unsigned char *) pdu->hdr + pdu->length - pdu->data : 0;
    OIC_LOG_V(INFO, ANALYZER_TAG, "CoAP Message Full Size = [%lu]", pdu->length);
    OIC_LOG(INFO, ANALYZER_TAG, "CoAP Header (+ 0xFF)");
    OIC_LOG_BUFFER(INFO, ANALYZER_TAG,  (const uint8_t *) pdu->transport_hdr,
                   pdu->length - payloadLen);
    OIC_LOG_V(INFO, ANALYZER_TAG, "CoAP Header size = [%lu]", pdu->length - payloadLen);

    OIC_LOG_V(INFO, ANALYZER_TAG, "CoAP Payload");
    OIC_LOG_BUFFER(INFO_PRIVATE, ANALYZER_TAG, pdu->data, payloadLen);
    OIC_LOG_V(INFO, ANALYZER_TAG, "CoAP Payload Size = [%lu]", payloadLen);
    OIC_LOG(INFO, ANALYZER_TAG, "=================================================");

    // samsung log
    CASamsungLogMessage(data, pdu);
    OIC_TRACE_END();
}

static void CASamsungLogMessage(const CAData_t *data, const coap_pdu_t *pdu)
{
    OIC_LOG(INFO, TAG, "CASamsungLogMessage");
    VERIFY_NON_NULL_VOID(data, TAG, "data");
    VERIFY_NON_NULL_VOID(pdu, TAG, "pdu");
    VERIFY_NON_NULL_VOID(data->remoteEndpoint, TAG, "data->remoteEndpoint");

    const CAInfo_t *info = NULL;
    if (NULL != data->requestInfo)
    {
        info = &data->requestInfo->info;
    }

    if (NULL != data->responseInfo)
    {
        info = &data->responseInfo->info;
    }

    VERIFY_NON_NULL_VOID(info, TAG, "info");

    memset(g_headerBuffer, 0, MAX_LOG_BUFFER_SIZE);
    g_headerIndex = 0;

    g_headerBuffer[g_headerIndex++] = data->dataType;
    g_headerBuffer[g_headerIndex++] = '|';
    g_headerBuffer[g_headerIndex++] = data->remoteEndpoint->adapter;
    g_headerBuffer[g_headerIndex++] = '|';
    g_headerBuffer[g_headerIndex++] = data->type;
    g_headerBuffer[g_headerIndex++] = '|';

    if (NULL != data->remoteEndpoint)
    {
        int i = 0;
        while (NULL != data->remoteEndpoint->addr[i])
        {
            g_headerBuffer[g_headerIndex++] = data->remoteEndpoint->addr[i];
            i++;
        }
        g_headerBuffer[g_headerIndex++] = ':';
        g_headerBuffer[g_headerIndex++] = (data->remoteEndpoint->port >> 8) & 0x0000ff;
        g_headerBuffer[g_headerIndex++] = data->remoteEndpoint->port & 0x000000ff;
    }

    g_headerBuffer[g_headerIndex++] = '|';
    if (data->requestInfo)
    {
        g_headerBuffer[g_headerIndex++] = data->requestInfo->method;
    }
    else
    {
        g_headerBuffer[g_headerIndex++] = 0;
    }

    g_headerBuffer[g_headerIndex++] = '|';
    if (data->responseInfo)
    {
        g_headerBuffer[g_headerIndex++] = data->responseInfo->result;
    }
    else
    {
        g_headerBuffer[g_headerIndex++] = 0;
    }
    g_headerBuffer[g_headerIndex++] = '|';

    if (pdu->transport_hdr)
    {
        g_headerBuffer[g_headerIndex++] = (pdu->transport_hdr->udp.id >> 8) & 0x0000ff;
        g_headerBuffer[g_headerIndex++] = pdu->transport_hdr->udp.id & 0x000000ff;
    }
    else
    {
        g_headerBuffer[g_headerIndex++] = 0;
        g_headerBuffer[g_headerIndex++] = 0;
    }
    g_headerBuffer[g_headerIndex++] = '|';

    if (info->token && info->tokenLength > 0)
    {
        for (size_t i = 0; i < info->tokenLength; i++)
        {
            g_headerBuffer[g_headerIndex++] = info->token[i];
        }
        g_headerBuffer[g_headerIndex++] = '|';
    }

    if (info->resourceUri)
    {
        size_t i = 0;
        while (NULL != info->resourceUri[i])
        {
            g_headerBuffer[g_headerIndex++] = info->resourceUri[i];
            i++;
        }
        g_headerBuffer[g_headerIndex++] = '|';
    }

    OIC_LOG_CA_BUFFER(INFO, TAG, g_headerBuffer, g_headerIndex, 1);
    size_t payloadLen = (unsigned char *) pdu->hdr + pdu->length - pdu->data;
    OIC_LOG_CA_BUFFER(INFO_PRIVATE, TAG, pdu->data, payloadLen, 0);
}

#else
static void CALogPDUInfo(const CAData_t *data, const coap_pdu_t *pdu)
{
    VERIFY_NON_NULL_VOID(pdu, TAG, "pdu");
    (void)data;

    OIC_LOG_V(DEBUG, TAG, "PDU Maker - payload : %s", pdu->data);
    OIC_LOG_V(DEBUG, TAG, "PDU Maker - type : %d", pdu->transport_hdr->udp.type);
    OIC_LOG_V(DEBUG, TAG, "PDU Maker - code : %d", pdu->transport_hdr->udp.code);
    OIC_LOG(DEBUG, TAG, "PDU Maker - token :");
    OIC_LOG_BUFFER(DEBUG, TAG, pdu->transport_hdr->udp.token,
                   pdu->transport_hdr->udp.token_length);
}
#endif
