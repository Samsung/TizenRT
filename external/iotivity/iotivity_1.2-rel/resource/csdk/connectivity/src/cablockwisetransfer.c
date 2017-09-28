/* ****************************************************************
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
 ******************************************************************/

// Defining _BSD_SOURCE or _DEFAULT_SOURCE causes header files to expose
// definitions that may otherwise be skipped. Skipping can cause implicit
// declaration warnings and/or bugs and subtle problems in code execution.
// For glibc information on feature test macros,
// Refer http://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
//
// This file requires #define use due to random()
// For details on compatibility and glibc support,
// Refer http://www.gnu.org/software/libc/manual/html_node/BSD-Random.html
#define _DEFAULT_SOURCE
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caadapterutils.h"
#include "cainterface.h"
#include "camessagehandler.h"
#include "caremotehandler.h"
#include "cablockwisetransfer.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "octhread.h"
#include "logger.h"

#define TAG "OIC_CA_BWT"

#define BLOCKWISE_OPTION_BUFFER    (sizeof(unsigned int))
#define BLOCK_NUMBER_IDX           4
#define BLOCK_M_BIT_IDX            3
#define PORT_LENGTH                2

#define BLOCK_SIZE(arg) (1 << ((arg) + 4))

#define MAX_BLOCK_DATA_TIMEOUT_SECONDS   (60 * 1)  // 1 minutes.
#define MILLISECONDS_PER_SECOND   (1000)

// context for block-wise transfer
static CABlockWiseContext_t g_context = { .sendThreadFunc = NULL,
                                          .receivedThreadFunc = NULL,
                                          .dataList = NULL };

static uint32_t GetTicks(uint32_t milliSeconds);

static bool CACheckPayloadLength(const CAData_t *sendData)
{
    size_t payloadLen = 0;
    CAGetPayloadInfo(sendData, &payloadLen);

    // check if message has to be transfered to a block
    size_t maxBlockSize = BLOCK_SIZE(CA_DEFAULT_BLOCK_SIZE);
    OIC_LOG_V(DEBUG, TAG, "payloadLen=%zu, maxBlockSize=%zu", payloadLen, maxBlockSize);

    if (payloadLen <= maxBlockSize)
    {
        return false;
    }

    return true;
}

CAResult_t CAInitializeBlockWiseTransfer(CASendThreadFunc sendThreadFunc,
                                         CAReceiveThreadFunc receivedThreadFunc)
{
    OIC_LOG(DEBUG, TAG, "CAInitializeBlockWiseTransfer");

    // set block-wise transfer context
    if (!g_context.sendThreadFunc)
    {
        g_context.sendThreadFunc = sendThreadFunc;
    }

    if (!g_context.receivedThreadFunc)
    {
        g_context.receivedThreadFunc = receivedThreadFunc;
    }

    if (!g_context.dataList)
    {
        g_context.dataList = u_arraylist_create();
    }

    CAResult_t res = CAInitBlockWiseMutexVariables();
    if (CA_STATUS_OK != res)
    {
        u_arraylist_free(&g_context.dataList);
        g_context.dataList = NULL;
        OIC_LOG(ERROR, TAG, "init has failed");
    }

    return res;
}

CAResult_t CATerminateBlockWiseTransfer()
{
    OIC_LOG(DEBUG, TAG, "CATerminateBlockWiseTransfer");

    if (g_context.dataList)
    {
        CARemoveAllBlockDataFromList();
        u_arraylist_free(&g_context.dataList);
    }

    CATerminateBlockWiseMutexVariables();

    return CA_STATUS_OK;
}

CAResult_t CAInitBlockWiseMutexVariables()
{
    if (!g_context.blockDataListMutex)
    {
        g_context.blockDataListMutex = oc_mutex_new();
        if (!g_context.blockDataListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (!g_context.blockDataSenderMutex)
    {
        g_context.blockDataSenderMutex = oc_mutex_new();
        if (!g_context.blockDataSenderMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            CATerminateBlockWiseMutexVariables();
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CATerminateBlockWiseMutexVariables()
{
    if (g_context.blockDataListMutex)
    {
        oc_mutex_free(g_context.blockDataListMutex);
        g_context.blockDataListMutex = NULL;
    }

    if (g_context.blockDataSenderMutex)
    {
        oc_mutex_free(g_context.blockDataSenderMutex);
        g_context.blockDataSenderMutex = NULL;
    }
}

CAResult_t CASendBlockWiseData(const CAData_t *sendData)
{
    VERIFY_NON_NULL(sendData, TAG, "sendData");

    // check if message type is CA_MSG_RESET
    if (sendData->requestInfo)
    {
        if (CA_MSG_RESET == sendData->requestInfo->info.type)
        {
            OIC_LOG(DEBUG, TAG, "reset message can't be sent to the block");
            return CA_NOT_SUPPORTED;
        }

        /*
         * Other uses of the Block options in conjunction with multicast
         * messages are for further study.
         */
        if (sendData->requestInfo->isMulticast)
        {
            OIC_LOG(DEBUG, TAG, "multicast message can't be sent to the block");
            return CA_NOT_SUPPORTED;
        }
    }
    else if (sendData->responseInfo)
    {
        if (CA_MSG_RESET == sendData->responseInfo->info.type)
        {
            OIC_LOG(DEBUG, TAG, "reset message can't be sent to the block");
            return CA_NOT_SUPPORTED;
        }
        if (sendData->responseInfo->isMulticast)
        {
            OIC_LOG(DEBUG, TAG, "multicast message can't be sent to the block");
            return CA_NOT_SUPPORTED;
        }
    }

    // #1. check if it is already included in block data list
    CABlockData_t *currData = NULL;
    CAResult_t res = CACheckBlockDataValidation(sendData, &currData);
    if (CA_STATUS_OK != res)
    {
        // #2. if it is not included, add the data into list
        if (!currData)
        {
            OIC_LOG(DEBUG, TAG, "There is no block data");

            bool isBlock = CACheckPayloadLength(sendData);
            if (!isBlock)
            {
                if (sendData->requestInfo)
                {
                    currData = CACreateNewBlockData(sendData);
                    if (!currData)
                    {
                        OIC_LOG(ERROR, TAG, "failed to create block data");
                        return CA_MEMORY_ALLOC_FAILED;
                    }
                }
                return CA_NOT_SUPPORTED;
            }
            currData = CACreateNewBlockData(sendData);
            if (!currData)
            {
                OIC_LOG(ERROR, TAG, "failed to create block data");
                return CA_MEMORY_ALLOC_FAILED;
            }
        }
    }

    // #3. check request/response block option type and payload length
    res = CACheckBlockOptionType(currData);
    if (CA_STATUS_OK == res)
    {
        // #4. send block message
        OIC_LOG(DEBUG, TAG, "send first block msg");
        res = CAAddSendThreadQueue(currData->sentData,
                                   (const CABlockDataID_t *) &currData->blockDataId);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            return res;
        }
    }

    return res;
}

CAResult_t CAAddSendThreadQueue(const CAData_t *sendData, const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL(sendData, TAG, "sendData");
    VERIFY_NON_NULL(blockID, TAG, "blockID");

    CAData_t *cloneData = CACloneCAData(sendData);
    if (!cloneData)
    {
        OIC_LOG(ERROR, TAG, "clone has failed");
        CARemoveBlockDataFromList(blockID);
        return CA_STATUS_FAILED;
    }

    if (g_context.sendThreadFunc)
    {
        oc_mutex_lock(g_context.blockDataSenderMutex);
        g_context.sendThreadFunc(cloneData);
        oc_mutex_unlock(g_context.blockDataSenderMutex);
    }
    else
    {
        CADestroyDataSet(cloneData);
    }
    return CA_STATUS_OK;
}

CAResult_t CACheckBlockOptionType(CABlockData_t *currData)
{
    VERIFY_NON_NULL(currData, TAG, "currData");
    VERIFY_NON_NULL(currData->sentData, TAG, "currData->sentData");

    bool isBlock = CACheckPayloadLength(currData->sentData);
    if (!isBlock)
    {
        return CA_NOT_SUPPORTED;
    }

    // set block option (COAP_OPTION_BLOCK2 or COAP_OPTION_BLOCK1)
    if (currData->sentData->requestInfo) // request message
    {
        currData->type = COAP_OPTION_BLOCK1;
    }
    else // response message
    {
        currData->type = COAP_OPTION_BLOCK2;
    }

    return CA_STATUS_OK;
}

// TODO make pdu const after libcoap is updated to support that.
CAResult_t CAReceiveBlockWiseData(coap_pdu_t *pdu, const CAEndpoint_t *endpoint,
                                  const CAData_t *receivedData, size_t dataLen)
{
    OIC_LOG(DEBUG, TAG, "CAReceiveBlockWiseData");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->transport_hdr, TAG, "pdu->transport_hdr");
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(receivedData, TAG, "receivedData");

    // check if received message type is CA_MSG_RESET
    if (CA_EMPTY == pdu->transport_hdr->udp.code)
    {
        OIC_LOG(DEBUG, TAG, "code is CA_EMPTY..");

        if (!receivedData->responseInfo->info.token)
        {
            // get token from block-wise transfer list when CA_EMPTY(RST/ACK) is received
            CAResult_t res = CAGetTokenFromBlockDataList(pdu, endpoint,
                                                         receivedData->responseInfo);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "fail to get token");
                return res;
            }
        }

        CABlockDataID_t* blockDataID = CACreateBlockDatablockId(
                receivedData->responseInfo->info.token,
                receivedData->responseInfo->info.tokenLength,
                endpoint->port);
        if (NULL == blockDataID || blockDataID->idLength < 1)
        {
            // if retransmission is timeout, callback msg will be send without token.
            if (NULL == blockDataID && !receivedData->responseInfo->info.token)
            {
                OIC_LOG(INFO, TAG, "retransmission was stopped");
                return CA_REQUEST_TIMEOUT;
            }

            OIC_LOG(ERROR, TAG, "blockId is null");
            CADestroyBlockID(blockDataID);
            return CA_STATUS_FAILED;
        }

        // If we didn't send the last block message and received EMPTY message,
        // we have to remain the block data from list.
        CABlockData_t *data = CAGetBlockDataFromBlockDataList(blockDataID);
        if (data && (data->block1.m || data->block2.m))
        {
            OIC_LOG(DEBUG, TAG, "this is normal EMPTY message for blockwise-transfer.");
            CADestroyBlockID(blockDataID);
            return CA_STATUS_OK;
        }

        CARemoveBlockDataFromList(blockDataID);
        CADestroyBlockID(blockDataID);
        return CA_NOT_SUPPORTED;
    }

    // check if block option is set and get block data
    coap_block_t block = { 0, 0, 0 };

    // get block1 option
    int isBlock1 = coap_get_block(pdu, COAP_OPTION_BLOCK1, &block);
    if (isBlock1)
    {
        CAResult_t res = CASetNextBlockOption1(pdu, endpoint, receivedData, block, dataLen);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "setting has failed");
            return res;
        }
    }

    // get block2 option
    int isBlock2 = coap_get_block(pdu, COAP_OPTION_BLOCK2, &block);
    if (isBlock2)
    {
        CAResult_t res = CASetNextBlockOption2(pdu, endpoint, receivedData, block, dataLen);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "setting has failed");
            return res;
        }
    }

    // if there is no block option in pdu, check if there is error code.
    if (!isBlock1 && !isBlock2)
    {
        CABlockDataID_t* blockDataID = CACreateBlockDatablockId(
                (CAToken_t)pdu->transport_hdr->udp.token,
                pdu->transport_hdr->udp.token_length,
                endpoint->port);
        if (NULL == blockDataID || blockDataID->idLength < 1)
        {
            OIC_LOG(ERROR, TAG, "blockId is null");
            CADestroyBlockID(blockDataID);
            return CA_STATUS_FAILED;
        }

        uint32_t code = CA_RESPONSE_CODE(pdu->transport_hdr->udp.code);
        if (CA_REQUEST_ENTITY_INCOMPLETE == code)
        {
            CABlockData_t *data = CAGetBlockDataFromBlockDataList(blockDataID);
            if (!data)
            {
                OIC_LOG(ERROR, TAG, "getting has failed");
                CADestroyBlockID(blockDataID);
                return CA_STATUS_FAILED;
            }

            coap_block_t *block = CAGetBlockOption(blockDataID, data->type);
            if (!block)
            {
                OIC_LOG(ERROR, TAG, "block is null");
                CADestroyBlockID(blockDataID);
                return CA_STATUS_FAILED;
            }

            CAResult_t res = CA_STATUS_OK;
            if (COAP_OPTION_BLOCK2 == data->type)
            {
                res = CASetNextBlockOption2(pdu, endpoint, receivedData, *block, dataLen);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "setting has failed");
                    CADestroyBlockID(blockDataID);
                    return CA_STATUS_FAILED;
                }
            }
            else if (COAP_OPTION_BLOCK1 == data->type)
            {
                res = CASetNextBlockOption1(pdu, endpoint, receivedData, *block, dataLen);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "setting has failed");
                    CADestroyBlockID(blockDataID);
                    return res;
                }
            }
            CADestroyBlockID(blockDataID);
        }
        else
        {
            // normal pdu data
            OIC_LOG(DEBUG, TAG, "it's normal pdu");

            // if received data is response message
            // and sent data remain in block data list, remove block data
            if (receivedData->responseInfo)
            {
                CARemoveBlockDataFromList(blockDataID);
            }
            CADestroyBlockID(blockDataID);
            return CA_NOT_SUPPORTED;
        }
    }
    return CA_STATUS_OK;
}

CAResult_t CAProcessNextStep(const coap_pdu_t *pdu, const CAData_t *receivedData,
                             uint8_t blockWiseStatus, const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->hdr, TAG, "pdu->hdr");
    VERIFY_NON_NULL(blockID, TAG, "blockID");

    CAResult_t res = CA_STATUS_OK;
    CAData_t *data = NULL;

    // process blockWiseStatus
    switch (blockWiseStatus)
    {
        case CA_OPTION2_FIRST_BLOCK:
            res = CAAddSendThreadQueue(receivedData, blockID);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "add has failed");
                return res;
            }
            break;

        case CA_OPTION2_REQUEST:
            // add data to send thread
            data = CAGetDataSetFromBlockDataList(blockID);
            if (!data)
            {
                OIC_LOG(ERROR, TAG, "it's unavailable");
                return CA_STATUS_FAILED;
            }

            if (data->responseInfo)
            {
                data->responseInfo->info.type =
                        (pdu->transport_hdr->udp.type == CA_MSG_CONFIRM) ?
                                CA_MSG_ACKNOWLEDGE : CA_MSG_NONCONFIRM;
                data->responseInfo->info.messageId = pdu->transport_hdr->udp.id;

                res = CAAddSendThreadQueue(data, blockID);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "add has failed");
                    return res;
                }
            }
            break;

        case CA_OPTION1_RESPONSE:
        case CA_OPTION2_RESPONSE:
        case CA_OPTION1_REQUEST_BLOCK:
            res = CASendBlockMessage(pdu, pdu->transport_hdr->udp.type, blockID);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "send has failed");
                return res;
            }
            break;

        case CA_OPTION2_LAST_BLOCK:
            // process last block and send upper layer
            res = CAReceiveLastBlock(blockID, receivedData);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "receive has failed");
                return res;
            }

            // remove data from list
            res = CARemoveBlockDataFromList(blockID);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "remove has failed");
                return res;
            }
            break;

        case CA_OPTION1_REQUEST_LAST_BLOCK:
            // process last block and send upper layer
            res = CAReceiveLastBlock(blockID, receivedData);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "receive has failed");
                return res;
            }
            break;

        case CA_BLOCK_INCOMPLETE:
            // add data to send thread
            res = CASendErrorMessage(pdu, blockWiseStatus, CA_REQUEST_ENTITY_INCOMPLETE, blockID);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "send has failed");
                return res;
            }
            break;

        case CA_BLOCK_TOO_LARGE:
            if (receivedData->requestInfo)
            {
                res = CASendErrorMessage(pdu, blockWiseStatus, CA_REQUEST_ENTITY_TOO_LARGE,
                                         blockID);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "send has failed");
                    return res;
                }
            }
            else if (receivedData->responseInfo)
            {
                res = CASendBlockMessage(pdu, pdu->transport_hdr->udp.type, blockID);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "send has failed");
                    return res;
                }
            }
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "no logic [%d]", blockWiseStatus);
    }
    return CA_STATUS_OK;
}

static CAResult_t CASendDirectEmptyResponse(const CAEndpoint_t *endpoint, uint16_t messageId)
{
    OIC_LOG(DEBUG, TAG, "Entering CASendDirectEmptyResponse");
    CAResponseInfo_t respInfo = {
        .result = CA_EMPTY
    };
    respInfo.info.type = CA_MSG_ACKNOWLEDGE;
    respInfo.info.messageId = messageId;
    respInfo.info.dataType = CA_RESPONSE_DATA;

    CAResult_t caResult = CASendResponse(endpoint, &respInfo);

    OIC_LOG(DEBUG, TAG, "Exit CASendDirectEmptyResponse");
    return caResult;
}

CAResult_t CASendBlockMessage(const coap_pdu_t *pdu, CAMessageType_t msgType,
                              const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->transport_hdr, TAG, "pdu->transport_hdr");
    VERIFY_NON_NULL(blockID, TAG, "blockID");

    CAData_t *data = CAGetDataSetFromBlockDataList(blockID);
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "CAData is unavailable");
        return CA_STATUS_FAILED;
    }

    CAMessageType_t sentMsgType = CA_MSG_NONCONFIRM;
    switch (msgType)
    {
        case CA_MSG_CONFIRM:
            sentMsgType = CA_MSG_ACKNOWLEDGE;
            break;
        case CA_MSG_ACKNOWLEDGE:
            sentMsgType = CA_MSG_CONFIRM;
            break;
        default:
            sentMsgType = CA_MSG_NONCONFIRM;
            break;
    }

    uint32_t code = pdu->transport_hdr->udp.code;
    if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
    {
        if (data->responseInfo)
        {
            OIC_LOG(DEBUG, TAG, "set response info");
            data->responseInfo->info.messageId = pdu->transport_hdr->udp.id;
            data->responseInfo->info.type = sentMsgType;
            data->responseInfo->result = CA_CONTINUE;
        }
    }
    else
    {
        if (data->requestInfo)
        {
            // if the received response message type is CON, send empty message.
            // and then, send next block request message with new messagId.
            if (msgType == CA_MSG_CONFIRM)
            {
                CASendDirectEmptyResponse(data->remoteEndpoint,
                                          data->requestInfo->info.messageId);
                sentMsgType = CA_MSG_CONFIRM;
            }

            OIC_LOG(DEBUG, TAG, "need new msgID");
            data->requestInfo->info.messageId = 0;
            data->requestInfo->info.type = sentMsgType;
        }
        else if (data->responseInfo)
        {
            data->responseInfo->info.messageId = pdu->transport_hdr->udp.id;
            data->responseInfo->info.type = sentMsgType;
        }
    }

    // add data to send thread
    CAResult_t res = CAAddSendThreadQueue(data, blockID);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "add has failed");
    }

    return res;
}

CAResult_t CASendErrorMessage(const coap_pdu_t *pdu, uint8_t status,
                              CAResponseResult_t responseResult, const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->transport_hdr, TAG, "pdu->transport_hdr");
    VERIFY_NON_NULL(blockID, TAG, "blockID");

    // create error responseInfo
    CABlockData_t *data = CAGetBlockDataFromBlockDataList(blockID);
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "data is unavailable");
        return CA_STATUS_FAILED;
    }

    if (!data->sentData)
    {
        OIC_LOG(ERROR, TAG, "data has no sent-data");
        return CA_STATUS_FAILED;
    }

    CAData_t *cloneData = CACloneCAData(data->sentData);
    if (!cloneData)
    {
        OIC_LOG(ERROR, TAG, "clone has failed");
        return CA_MEMORY_ALLOC_FAILED;
    }

    CAMessageType_t sentMsgType = CA_MSG_NONCONFIRM;
    switch (pdu->transport_hdr->udp.type)
    {
        case CA_MSG_CONFIRM:
            sentMsgType = CA_MSG_ACKNOWLEDGE;
            break;
        case CA_MSG_ACKNOWLEDGE:
            sentMsgType = CA_MSG_CONFIRM;
            break;
        default:
            sentMsgType = CA_MSG_NONCONFIRM;
    }

    if (cloneData->responseInfo)
    {
        cloneData->responseInfo->info.messageId = pdu->transport_hdr->udp.id;
        cloneData->responseInfo->info.type = sentMsgType;
        cloneData->responseInfo->result = responseResult;
    }
    else
    {
        CAInfo_t responseData = { .tokenLength = pdu->transport_hdr->udp.token_length };
        responseData.token = (CAToken_t) OICMalloc(responseData.tokenLength);
        if (!responseData.token)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
            CADestroyDataSet(cloneData);
            return CA_MEMORY_ALLOC_FAILED;
        }
        memcpy(responseData.token, pdu->transport_hdr->udp.token, responseData.tokenLength);

        cloneData->responseInfo = (CAResponseInfo_t*) OICCalloc(1, sizeof(CAResponseInfo_t));
        if (!cloneData->responseInfo)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
            CADestroyDataSet(cloneData);
            OICFree(responseData.token);
            return CA_MEMORY_ALLOC_FAILED;
        }

        cloneData->responseInfo->info = responseData;
        cloneData->responseInfo->info.type = sentMsgType;
        cloneData->responseInfo->result = responseResult;
    }

    OIC_LOG(DEBUG, TAG, "set response message to send error code");

    // if there is a requestInfo, remove it to send response message
    if (cloneData->requestInfo)
    {
        CADestroyRequestInfoInternal(cloneData->requestInfo);
        cloneData->requestInfo = NULL;
    }

    // add data to send thread
    if (g_context.sendThreadFunc)
    {
        oc_mutex_lock(g_context.blockDataSenderMutex);
        g_context.sendThreadFunc(cloneData);
        oc_mutex_unlock(g_context.blockDataSenderMutex);
    }
    else
    {
        CADestroyDataSet(cloneData);
    }

    // if error code is 4.08, remove the stored payload and initialize block number
    if (CA_BLOCK_INCOMPLETE == status)
    {
        OICFree(data->payload);
        data->payload = NULL;
        data->payloadLength = 0;
        data->receivedPayloadLen = 0;
        data->block1.num = 0;
        data->block2.num = 0;
    }

    return CA_STATUS_OK;
}

CAResult_t CAReceiveLastBlock(const CABlockDataID_t *blockID, const CAData_t *receivedData)
{
    VERIFY_NON_NULL(blockID, TAG, "blockID");
    VERIFY_NON_NULL(receivedData, TAG, "receivedData");

    // total block data have to notify to Application
    CAData_t *cloneData = CACloneCAData(receivedData);
    if (!cloneData)
    {
        OIC_LOG(ERROR, TAG, "clone has failed");
        return CA_MEMORY_ALLOC_FAILED;
    }

    // update payload
    size_t fullPayloadLen = 0;
    CAPayload_t fullPayload = CAGetPayloadFromBlockDataList(blockID, &fullPayloadLen);
    if (fullPayload)
    {
        CAResult_t res = CAUpdatePayloadToCAData(cloneData, fullPayload, fullPayloadLen);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "update has failed");
            CADestroyDataSet(cloneData);
            return res;
        }
    }

    if (g_context.receivedThreadFunc)
    {
        g_context.receivedThreadFunc(cloneData);
    }
    else
    {
        CADestroyDataSet(cloneData);
    }

    return CA_STATUS_OK;
}

static CABlockData_t* CACheckTheExistOfBlockData(const CABlockDataID_t* blockDataID,
                                                 coap_pdu_t *pdu, const CAEndpoint_t *endpoint,
                                                 uint8_t blockType)
{
    // Get BlockData data. If does not exist, create a new data
    CABlockData_t *data = CAGetBlockDataFromBlockDataList(blockDataID);
    if (!data)
    {
        OIC_LOG(DEBUG, TAG, "block data doesn't exist in list. create new one");

        CAData_t *cadata = CACreateNewDataSet(pdu, endpoint);
        if (!cadata)
        {
            OIC_LOG(ERROR, TAG, "data is null");
            return NULL;
        }

        data = CACreateNewBlockData(cadata);
        if (!data)
        {
            OIC_LOG(ERROR, TAG, "failed to create a new block data");
            CADestroyDataSet(cadata);
            return NULL;
        }
        CADestroyDataSet(cadata);
    }

    // update BLOCK OPTION type
    CAResult_t res = CAUpdateBlockOptionType(blockDataID, blockType);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "update has failed");
        return NULL;
    }

    return data;
}

// TODO make pdu const after libcoap is updated to support that.
CAResult_t CASetNextBlockOption1(coap_pdu_t *pdu, const CAEndpoint_t *endpoint,
                                 const CAData_t *receivedData, coap_block_t block,
                                 size_t dataLen)
{
    OIC_LOG(INFO, TAG, "CASetNextBlockOption1");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->transport_hdr, TAG, "pdu->transport_hdr");
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(receivedData, TAG, "receivedData");

    OIC_LOG_V(INFO, TAG, "num:%d, M:%d, sze:%d", block.num, block.m, block.szx);

    CABlockDataID_t* blockDataID = CACreateBlockDatablockId(
            (CAToken_t)pdu->transport_hdr->udp.token,
            pdu->transport_hdr->udp.token_length,
            endpoint->port);
    if (NULL == blockDataID || blockDataID->idLength < 1)
    {
        OIC_LOG(ERROR, TAG, "blockId is null");
        CADestroyBlockID(blockDataID);
        return CA_STATUS_FAILED;
    }

    CAResult_t res = CA_STATUS_OK;
    CABlockData_t *data = CACheckTheExistOfBlockData(blockDataID, pdu, endpoint,
                                                     COAP_OPTION_BLOCK1);
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "Failed to create or get block data");
        res = CA_STATUS_FAILED;
        goto exit;
    }

    uint8_t blockWiseStatus = CA_BLOCK_UNKNOWN;
    uint32_t code = pdu->transport_hdr->udp.code;
    if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
    {
        // received message type is request
        OIC_LOG_V(INFO, TAG, "num:%d, M:%d", block.num, block.m);

        // check the size option
        bool isSizeOption = CAIsPayloadLengthInPduWithBlockSizeOption(pdu, COAP_OPTION_SIZE1,
                                                                      &(data->payloadLength));

        blockWiseStatus = CACheckBlockErrorType(data, &block, receivedData,
                                                COAP_OPTION_BLOCK1, dataLen);

        if (CA_BLOCK_RECEIVED_ALREADY != blockWiseStatus)
        {
            // store the received payload and merge
            res = CAUpdatePayloadData(data, receivedData, blockWiseStatus,
                                      isSizeOption, COAP_OPTION_BLOCK1);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "update has failed");
                goto exit;
            }

            res = CAUpdateBlockOptionItems(data, pdu, &block, COAP_OPTION_BLOCK1,
                                           blockWiseStatus);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "update has failed");
                goto exit;
            }

            // update block data
            res = CAUpdateBlockData(data, block, COAP_OPTION_BLOCK1);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "update has failed");
                goto exit;
            }
        }

        // check the blcok-wise transfer status for next step
        if (CA_BLOCK_UNKNOWN == blockWiseStatus || CA_BLOCK_RECEIVED_ALREADY == blockWiseStatus)
        {
            OIC_LOG_V(DEBUG, TAG, "M bit is %d", block.m);

            blockWiseStatus = (0 == block.m) ?
                    CA_OPTION1_REQUEST_LAST_BLOCK : CA_OPTION1_REQUEST_BLOCK;
        }
    }
    else
    {
        // received message type is response
        uint32_t code = CA_RESPONSE_CODE(pdu->transport_hdr->udp.code);
        if (0 == block.m && (CA_REQUEST_ENTITY_INCOMPLETE != code
                && CA_REQUEST_ENTITY_TOO_LARGE != code))
        {
            int isBlock2 = coap_get_block(pdu, COAP_OPTION_BLOCK2, &block);
            if (isBlock2)
            {
                OIC_LOG(INFO, TAG, "received data is combining block1 and block2");
                // initialize block number for response message
                data->block1.num = 0;
                CADestroyBlockID(blockDataID);
                return CA_STATUS_OK;
            }
            else
            {
                OIC_LOG(INFO, TAG, "received data is not bulk data");
                CAReceiveLastBlock(blockDataID, receivedData);
                res = CA_STATUS_OK;
                goto exit;
            }
        }

        blockWiseStatus = CA_OPTION1_RESPONSE;
        res = CAUpdateBlockOptionItems(data, pdu, &block, COAP_OPTION_BLOCK1, blockWiseStatus);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "update has failed");
            CADestroyBlockID(blockDataID);
            return res;
        }

        res = CAUpdateBlockData(data, block, COAP_OPTION_BLOCK1);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "update has failed");
            goto exit;
        }
    }

    res = CAProcessNextStep(pdu, receivedData, blockWiseStatus, blockDataID);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "setting has failed");
        goto exit;
    }

    CADestroyBlockID(blockDataID);
    return res;

exit:
    CARemoveBlockDataFromList(blockDataID);
    CADestroyBlockID(blockDataID);
    return res;
}

// TODO make pdu const after libcoap is updated to support that.
CAResult_t CASetNextBlockOption2(coap_pdu_t *pdu, const CAEndpoint_t *endpoint,
                                 const CAData_t *receivedData, coap_block_t block,
                                 size_t dataLen)
{
    OIC_LOG(DEBUG, TAG, "CASetNextBlockOption2");
    OIC_LOG_V(INFO, TAG, "num:%d, M:%d, sze:%d", block.num, block.m, block.szx);

    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->transport_hdr, TAG, "pdu->transport_hdr");
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(receivedData, TAG, "receivedData");

    CABlockDataID_t* blockDataID = CACreateBlockDatablockId(
            (CAToken_t)pdu->transport_hdr->udp.token,
            pdu->transport_hdr->udp.token_length,
            endpoint->port);
    if (NULL == blockDataID || blockDataID->idLength < 1)
    {
        OIC_LOG(ERROR, TAG, "blockId is null");
        CADestroyBlockID(blockDataID);
        return CA_STATUS_FAILED;
    }

    CAResult_t res = CA_STATUS_OK;
    CABlockData_t *data = CACheckTheExistOfBlockData(blockDataID, pdu, endpoint,
                                                     COAP_OPTION_BLOCK2);
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "Failed to create or get block data");
        res = CA_STATUS_FAILED;
        goto exit;
    }

    uint8_t blockWiseStatus = CA_BLOCK_UNKNOWN;
    if (0 == block.num && CA_GET == pdu->transport_hdr->udp.code && 0 == block.m)
    {
        OIC_LOG(INFO, TAG, "first block number");

        res = CAUpdateBlockOptionItems(data, pdu, &block, COAP_OPTION_BLOCK2, blockWiseStatus);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "update has failed");
            goto exit;
        }

        // first block data have to notify to Application
        res = CAUpdateBlockData(data, block, COAP_OPTION_BLOCK2);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "update has failed");
            goto exit;
        }
        blockWiseStatus = CA_OPTION2_FIRST_BLOCK;
    }
    else
    {
        uint32_t code = pdu->transport_hdr->udp.code;
        if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
        {
            // received message type is request
            OIC_LOG_V(INFO, TAG, "num:%d, M:%d", block.num, block.m);

            blockWiseStatus = CA_OPTION2_REQUEST;

            res = CAUpdateBlockOptionItems(data, pdu, &block, COAP_OPTION_BLOCK2, blockWiseStatus);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "update has failed");
                goto exit;
            }

            res = CAUpdateBlockData(data, block, COAP_OPTION_BLOCK2);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "update has failed");
                goto exit;
            }
        }
        else
        {
            // received message type is response
            OIC_LOG(DEBUG, TAG, "received response message with block option2");

            // check the size option
            bool isSizeOption = CAIsPayloadLengthInPduWithBlockSizeOption(pdu,
                                                                          COAP_OPTION_SIZE2,
                                                                          &(data->payloadLength));

            uint32_t code = CA_RESPONSE_CODE(pdu->transport_hdr->udp.code);
            if (CA_REQUEST_ENTITY_INCOMPLETE != code && CA_REQUEST_ENTITY_TOO_LARGE != code)
            {
                // check if received payload is exact
                blockWiseStatus = CACheckBlockErrorType(data, &block, receivedData,
                                                        COAP_OPTION_BLOCK2, dataLen);
            }

            if (CA_BLOCK_RECEIVED_ALREADY != blockWiseStatus)
            {
                // store the received payload and merge
                res = CAUpdatePayloadData(data, receivedData, blockWiseStatus,
                                          isSizeOption, COAP_OPTION_BLOCK2);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "update has failed");
                    goto exit;
                }
            }

            if (0 == block.m && CA_BLOCK_UNKNOWN == blockWiseStatus) // Last block is received
            {
                OIC_LOG(DEBUG, TAG, "M bit is 0");
                blockWiseStatus = CA_OPTION2_LAST_BLOCK;
            }
            else
            {
                if (CA_BLOCK_UNKNOWN == blockWiseStatus ||
                        CA_BLOCK_RECEIVED_ALREADY == blockWiseStatus)
                {
                    OIC_LOG(DEBUG, TAG, "M bit is 1");
                    blockWiseStatus = CA_OPTION2_RESPONSE;
                }

                res = CAUpdateBlockOptionItems(data, pdu, &block, COAP_OPTION_BLOCK2,
                                               blockWiseStatus);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "update has failed");
                    goto exit;
                }

                res = CAUpdateBlockData(data, block, COAP_OPTION_BLOCK2);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "update has failed");
                    goto exit;
                }
            }
        }
    }

    res = CAProcessNextStep(pdu, receivedData, blockWiseStatus, blockDataID);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "setting has failed");
        goto exit;
    }

    CADestroyBlockID(blockDataID);
    return CA_STATUS_OK;

exit:
    CARemoveBlockDataFromList(blockDataID);
    CADestroyBlockID(blockDataID);
    return res;
}

CAResult_t CAUpdateBlockOptionItems(CABlockData_t *currData, const coap_pdu_t *pdu,
                                    coap_block_t *block, uint16_t blockType,
                                    uint32_t status)
{
    VERIFY_NON_NULL(currData, TAG, "currData");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(block, TAG, "block");

    // update block data
    CAResult_t res = CA_STATUS_OK;
    uint32_t code = CA_RESPONSE_CODE(pdu->transport_hdr->udp.code);

    if (CA_REQUEST_ENTITY_INCOMPLETE == code || CA_REQUEST_ENTITY_TOO_LARGE == code)
    {
        // response error code of the received block message
        res = CAHandleBlockErrorResponse(block, blockType, code);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "error handle has failed");
            return res;
        }
    }
    else
    {
        // update block option items
        switch (status)
        {
            case CA_OPTION1_RESPONSE:
                if (currData->block1.num > block->num)
                {
                    OIC_LOG(ERROR, TAG, "received incorrect block num");
                    return CA_STATUS_FAILED;
                }
                block->num++;
                break;
            case CA_OPTION2_REQUEST:
                block->m = 0;
                break;
            case CA_OPTION2_RESPONSE:
                if (currData->block2.num > block->num)
                {
                    OIC_LOG(ERROR, TAG, "received incorrect block num");
                    return CA_STATUS_FAILED;
                }
                block->num++;
                block->m = 0;
                break;
            case CA_BLOCK_TOO_LARGE:
                // if state of received block message is CA_BLOCK_TOO_LARGE or CA_BLOCK_INCOMPLETE
                // we set the response error code appropriately and send
                if (COAP_OPTION_BLOCK2 == blockType)
                {
                    block->num++;
                    block->m = 0;
                    block->szx = currData->block2.szx;
                }
                else
                {
                    block->szx = currData->block1.szx;
                }
                break;
            default:
                OIC_LOG_V(ERROR, TAG, "no logic [%d]", status);
        }

        if (CA_BLOCK_INCOMPLETE != status && CA_BLOCK_TOO_LARGE != status)
        {
            // negotiate block size
            res = CANegotiateBlockSize(currData, block, pdu, blockType);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "negotiation has failed");
                return res;
            }
        }
    }
    return res;
}

CAResult_t CASetMoreBitFromBlock(size_t payloadLen, coap_block_t *block)
{
    VERIFY_NON_NULL(block, TAG, "block");

    if ((size_t) ((block->num + 1) << (block->szx + BLOCK_NUMBER_IDX)) < payloadLen)
    {
        OIC_LOG(DEBUG, TAG, "Set the M-bit(1)");
        block->m = 1;
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Set the M-bit(0)");
        block->m = 0;
    }

    return CA_STATUS_OK;
}

CAResult_t CANegotiateBlockSize(CABlockData_t *currData, coap_block_t *block,
                                const coap_pdu_t *pdu, uint16_t blockType)
{
    OIC_LOG(DEBUG, TAG, "IN-NegotiateBlockSize");

    VERIFY_NON_NULL(currData, TAG, "currData");
    VERIFY_NON_NULL(block, TAG, "block");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->transport_hdr, TAG, "pdu->transport_hdr");

    bool isReqMsg = false;
    uint32_t code = pdu->transport_hdr->udp.code;
    if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
    {
        isReqMsg = true;
    }

    // #1. check the block option type
    if (COAP_OPTION_BLOCK2 == blockType)
    {
        // #2. check the message type
        if (!isReqMsg)
        {
            if (block->szx > currData->block2.szx)
            {
                OIC_LOG(DEBUG, TAG, "sze is big");

                // #3. calculate new block number from block size
                unsigned int blockNum = BLOCK_SIZE(block->szx) /
                                        BLOCK_SIZE(currData->block2.szx) - 1;
                OIC_LOG(DEBUG, TAG, "num is set as Negotiation");
                block->num += blockNum;
                block->szx = currData->block2.szx;
                OIC_LOG_V(DEBUG, TAG, "updated block num: %d", block->num);
            }
        }
        else
        {
            if (block->szx > currData->block2.szx)
            {
                OIC_LOG(DEBUG, TAG, "sze is big");
                block->szx = currData->block2.szx;
            }
        }
    }
    else if (COAP_OPTION_BLOCK1 == blockType)
    {
        if (!isReqMsg)
        {
            if (block->szx < currData->block1.szx)
            {
                OIC_LOG(DEBUG, TAG, "sze is small");

                unsigned int blockNum = BLOCK_SIZE(currData->block1.szx) /
                                        BLOCK_SIZE(block->szx) - 1;
                block->num += blockNum;
                OIC_LOG_V(DEBUG, TAG, "updated block num: %d", block->num);
            }
        }
        else
        {
            if (block->szx > currData->block1.szx)
            {
                OIC_LOG(DEBUG, TAG, "sze is big");
                block->szx = currData->block1.szx;
            }
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Invalid block option");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT-NegotiateBlockSize");

    return CA_STATUS_OK;
}

CAResult_t CAUpdateBlockData(CABlockData_t *currData, coap_block_t block,
                             uint16_t blockType)
{
    VERIFY_NON_NULL(currData, TAG, "currData");

    // check if block size is bigger than CABlockSize_t
    if (block.szx > CA_BLOCK_SIZE_1024_BYTE)
    {
        OIC_LOG(DEBUG, TAG, "invalid block szx");
        return CA_STATUS_FAILED;
    }

    // update block option
    if (COAP_OPTION_BLOCK2 == blockType)
    {
        currData->block2 = block;
    }
    else
    {
        currData->block1 = block;
    }

    OIC_LOG(DEBUG, TAG, "data has updated");
    return CA_STATUS_OK;
}

CAResult_t CAUpdateMessageId(coap_pdu_t *pdu, const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(pdu->transport_hdr, TAG, "pdu->transport_hdr");
    VERIFY_NON_NULL(blockID, TAG, "blockID");

    // if message is sent, update messageId in block-wise transfer list
    CAData_t * cadata = CAGetDataSetFromBlockDataList(blockID);
    if (!cadata)
    {
        OIC_LOG(ERROR, TAG, "CAData is unavailable");
        return CA_STATUS_FAILED;
    }

    if (cadata->requestInfo)
    {
        cadata->requestInfo->info.messageId = pdu->transport_hdr->udp.id;
    }

    return CA_STATUS_OK;
}

CAResult_t CAAddBlockOption(coap_pdu_t **pdu, const CAInfo_t *info,
                            const CAEndpoint_t *endpoint, coap_list_t **options)
{
    OIC_LOG(DEBUG, TAG, "IN-AddBlockOption");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL((*pdu), TAG, "(*pdu)");
    VERIFY_NON_NULL((*pdu)->transport_hdr, TAG, "(*pdu)->transport_hdr");
    VERIFY_NON_NULL(info, TAG, "info");
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(options, TAG, "options");

    CAResult_t res = CA_STATUS_OK;
    size_t dataLength = 0;
    if (info->payload)
    {
        dataLength = info->payloadSize;
        OIC_LOG_V(DEBUG, TAG, "dataLength - %zu", dataLength);
    }

    CABlockDataID_t* blockDataID = CACreateBlockDatablockId(
            (CAToken_t)(*pdu)->transport_hdr->udp.token,
            (*pdu)->transport_hdr->udp.token_length,
            endpoint->port);
    if (NULL == blockDataID || blockDataID->idLength < 1)
    {
        OIC_LOG(ERROR, TAG, "blockId is null");
        res = CA_STATUS_FAILED;
        goto exit;
    }

    uint32_t repCode = CA_RESPONSE_CODE((*pdu)->transport_hdr->udp.code);
    if (CA_REQUEST_ENTITY_INCOMPLETE == repCode)
    {
        OIC_LOG(INFO, TAG, "don't use option");
        res = CA_STATUS_OK;
        goto exit;
    }

    uint8_t blockType = CAGetBlockOptionType(blockDataID);
    if (COAP_OPTION_BLOCK2 == blockType)
    {
        res = CAAddBlockOption2(pdu, info, dataLength, blockDataID, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }
    }
    else if (COAP_OPTION_BLOCK1 == blockType)
    {
        res = CAAddBlockOption1(pdu, info, dataLength, blockDataID, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "no BLOCK option");

        // in case it is not large data, add option list to pdu.
        if (*options)
        {
            for (coap_list_t *opt = *options; opt; opt = opt->next)
            {
                OIC_LOG_V(DEBUG, TAG, "[%s] opt will be added.",
                          COAP_OPTION_DATA(*(coap_option *) opt->data));
                OIC_LOG_V(DEBUG, TAG, "[%d] pdu length", (*pdu)->length);

                if (0 == coap_add_option(*pdu, COAP_OPTION_KEY(*(coap_option *) opt->data),
                                         COAP_OPTION_LENGTH(*(coap_option *) opt->data),
                                         COAP_OPTION_DATA(*(coap_option *) opt->data)))
                {
                    OIC_LOG(ERROR, TAG, "coap_add_option has failed");
                    res = CA_STATUS_FAILED;
                    goto exit;
                }
            }
        }
        OIC_LOG_V(DEBUG, TAG, "[%d] pdu length after option", (*pdu)->length);

        // if response data is so large. it have to send as block transfer
        if (!coap_add_data(*pdu, dataLength, (const unsigned char *) info->payload))
        {
            OIC_LOG(INFO, TAG, "it has to use block");
            res = CA_STATUS_FAILED;
            goto exit;
        }
        else
        {
            OIC_LOG(INFO, TAG, "not Blockwise Transfer");
        }
    }

    uint32_t code = (*pdu)->transport_hdr->udp.code;
    if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
    {
        // if received message type is RESET from remote device,
        // we have to use the updated message id of request message to find token.
        CABlockData_t *blockData = CAGetBlockDataFromBlockDataList(blockDataID);
        if (blockData)
        {
            res = CAUpdateMessageId(*pdu, blockDataID);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "fail to update message id");
                goto exit;
            }
        }
    }

    CAResetBlockDataTTL(blockDataID);
exit:
    CADestroyBlockID(blockDataID);
    OIC_LOG(DEBUG, TAG, "OUT-AddBlockOption");
    return res;
}

CAResult_t CAAddBlockOption2(coap_pdu_t **pdu, const CAInfo_t *info, size_t dataLength,
                             const CABlockDataID_t *blockID, coap_list_t **options)
{
    OIC_LOG(DEBUG, TAG, "IN-AddBlockOption2");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL((*pdu), TAG, "(*pdu)");
    VERIFY_NON_NULL((*pdu)->transport_hdr, TAG, "(*pdu)->transport_hdr");
    VERIFY_NON_NULL(info, TAG, "info");
    VERIFY_NON_NULL(blockID, TAG, "blockID");
    VERIFY_NON_NULL(options, TAG, "options");

    // get set block data from CABlock list-set.
    coap_block_t *block1 = CAGetBlockOption(blockID, COAP_OPTION_BLOCK1);
    coap_block_t *block2 = CAGetBlockOption(blockID, COAP_OPTION_BLOCK2);
    if (!block1 || !block2)
    {
        OIC_LOG(ERROR, TAG, "getting has failed");
        return CA_STATUS_FAILED;
    }

    CAResult_t res = CA_STATUS_OK;
    uint32_t code = (*pdu)->transport_hdr->udp.code;
    if (CA_GET != code && CA_POST != code && CA_PUT != code && CA_DELETE != code)
    {
        CASetMoreBitFromBlock(dataLength, block2);

        // if block number is 0, add size2 option
        if (0 == block2->num)
        {
            res = CAAddBlockSizeOption(*pdu, COAP_OPTION_SIZE2, dataLength, options);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "add has failed");
                goto exit;
            }
        }

        res = CAAddBlockOptionImpl(block2, COAP_OPTION_BLOCK2, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }

        if (block1->num)
        {
            OIC_LOG(DEBUG, TAG, "combining block1 and block2");
            res = CAAddBlockOptionImpl(block1, COAP_OPTION_BLOCK1, options);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "add has failed");
                goto exit;
            }
            // initialize block number
            block1->num = 0;
        }

        res = CAAddOptionToPDU(*pdu, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }

        if (!coap_add_block(*pdu, dataLength, (const unsigned char *) info->payload,
                            block2->num, block2->szx))
        {
            OIC_LOG(ERROR, TAG, "Data length is smaller than the start index");
            return CA_STATUS_FAILED;
        }

        CALogBlockInfo(block2);

        if (!block2->m)
        {
            // if sent message is last response block message, remove data
            CARemoveBlockDataFromList(blockID);
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "option2, not response msg");
        res = CAAddBlockOptionImpl(block2, COAP_OPTION_BLOCK2, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }

        res = CAAddOptionToPDU(*pdu, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }
        CALogBlockInfo(block2);
    }

    return CA_STATUS_OK;

exit:
    CARemoveBlockDataFromList(blockID);
    return res;
}

CAResult_t CAAddBlockOption1(coap_pdu_t **pdu, const CAInfo_t *info, size_t dataLength,
                             const CABlockDataID_t *blockID, coap_list_t **options)
{
    OIC_LOG(DEBUG, TAG, "IN-AddBlockOption1");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL((*pdu), TAG, "(*pdu)");
    VERIFY_NON_NULL((*pdu)->transport_hdr, TAG, "(*pdu)->transport_hdr");
    VERIFY_NON_NULL(info, TAG, "info");
    VERIFY_NON_NULL(blockID, TAG, "blockID");
    VERIFY_NON_NULL(options, TAG, "options");

    // get set block data from CABlock list-set.
    coap_block_t *block1 = CAGetBlockOption(blockID, COAP_OPTION_BLOCK1);
    if (!block1)
    {
        OIC_LOG(ERROR, TAG, "getting has failed");
        return CA_STATUS_FAILED;
    }

    CAResult_t res = CA_STATUS_OK;
    uint32_t code = (*pdu)->transport_hdr->udp.code;
    if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
    {
        CASetMoreBitFromBlock(dataLength, block1);

        // if block number is 0, add size1 option
        if (0 == block1->num)
        {
            res = CAAddBlockSizeOption(*pdu, COAP_OPTION_SIZE1, dataLength, options);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "add has failed");
                goto exit;
            }
        }

        // add block option to option list.
        res = CAAddBlockOptionImpl(block1, COAP_OPTION_BLOCK1, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }

        // add option list to pdu.
        res = CAAddOptionToPDU(*pdu, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }

        // add the payload data as the block size.
        if (!coap_add_block(*pdu, dataLength, (const unsigned char *) info->payload, block1->num,
                            block1->szx))
        {
            OIC_LOG(ERROR, TAG, "Data length is smaller than the start index");
            return CA_STATUS_FAILED;
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "received response message with block option1");

        // add block option to option list.
        res = CAAddBlockOptionImpl(block1, COAP_OPTION_BLOCK1, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }

        // add option list to pdu.
        res = CAAddOptionToPDU(*pdu, options);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "add has failed");
            goto exit;
        }

        // add the payload data as the block size.
        if (!coap_add_data(*pdu, dataLength, (const unsigned char *) info->payload))
        {
            OIC_LOG(ERROR, TAG, "failed to add payload");
            return CA_STATUS_FAILED;
        }

        // if it is last block message, remove block data from list.
        if (0 == block1->m)
        {
            // remove data from list
            res = CARemoveBlockDataFromList(blockID);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "remove has failed");
                return res;
            }
        }
    }

    CALogBlockInfo(block1);

    OIC_LOG(DEBUG, TAG, "OUT-AddBlockOption1");

    return CA_STATUS_OK;

exit:
    CARemoveBlockDataFromList(blockID);
    return res;
}

CAResult_t CAAddBlockOptionImpl(coap_block_t *block, uint8_t blockType,
                                coap_list_t **options)
{
    OIC_LOG(DEBUG, TAG, "IN-AddBlockOptionImpl");
    VERIFY_NON_NULL(block, TAG, "block");
    VERIFY_NON_NULL(options, TAG, "options");

    unsigned char buf[BLOCKWISE_OPTION_BUFFER] = { 0 };
    unsigned int optionLength = coap_encode_var_bytes(buf,
                                                      ((block->num << BLOCK_NUMBER_IDX)
                                                       | (block->m << BLOCK_M_BIT_IDX)
                                                       | block->szx));

    int ret = coap_insert(options,
                          CACreateNewOptionNode(blockType, optionLength, (char *) buf),
                          CAOrderOpts);
    if (ret <= 0)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    OIC_LOG(DEBUG, TAG, "OUT-AddBlockOptionImpl");
    return CA_STATUS_OK;
}

CAResult_t CAAddOptionToPDU(coap_pdu_t *pdu, coap_list_t **options)
{
    // after adding the block option to option list, add option list to pdu.
    if (*options)
    {
        for (coap_list_t *opt = *options; opt; opt = opt->next)
        {
            OIC_LOG_V(DEBUG, TAG, "[%s] opt will be added.",
                      COAP_OPTION_DATA(*(coap_option *) opt->data));

            OIC_LOG_V(DEBUG, TAG, "[%d] pdu length", pdu->length);
            int ret = coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *) opt->data),
                                      COAP_OPTION_LENGTH(*(coap_option *) opt->data),
                                      COAP_OPTION_DATA(*(coap_option *) opt->data));
            if (!ret)
            {
                return CA_STATUS_FAILED;
            }
        }
    }

    OIC_LOG_V(DEBUG, TAG, "[%d] pdu length after option", pdu->length);

    return CA_STATUS_OK;
}

CAResult_t CAAddBlockSizeOption(coap_pdu_t *pdu, uint16_t sizeType, size_t dataLength,
                                coap_list_t **options)
{
    OIC_LOG(DEBUG, TAG, "IN-CAAddBlockSizeOption");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(options, TAG, "options");

    if (sizeType != COAP_OPTION_SIZE1 && sizeType != COAP_OPTION_SIZE2)
    {
        OIC_LOG(ERROR, TAG, "unknown option type");
        return CA_STATUS_FAILED;
    }

    unsigned char value[BLOCKWISE_OPTION_BUFFER] = { 0 };
    unsigned int optionLength = coap_encode_var_bytes(value, dataLength);

    int ret = coap_insert(options,
                          CACreateNewOptionNode(sizeType, optionLength, (char *) value),
                          CAOrderOpts);
    if (ret <= 0)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    OIC_LOG(DEBUG, TAG, "OUT-CAAddBlockSizeOption");

    return CA_STATUS_OK;
}

// TODO make pdu const after libcoap is updated to support that.
bool CAIsPayloadLengthInPduWithBlockSizeOption(coap_pdu_t *pdu,
                                               uint16_t sizeType,
                                               size_t *totalPayloadLen)
{
    OIC_LOG(DEBUG, TAG, "IN-CAIsPayloadLengthInPduWithBlockSizeOption");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(totalPayloadLen, TAG, "totalPayloadLen");

    if (sizeType != COAP_OPTION_SIZE1 && sizeType != COAP_OPTION_SIZE2)
    {
        OIC_LOG(ERROR, TAG, "unknown option type");
        return CA_STATUS_FAILED;
    }

    coap_opt_iterator_t opt_iter;
    coap_opt_t *option = coap_check_option(pdu, sizeType, &opt_iter);
    if (option)
    {
        OIC_LOG(DEBUG, TAG, "get size option from pdu");
        *totalPayloadLen = coap_decode_var_bytes(COAP_OPT_VALUE(option),
                                                 COAP_OPT_LENGTH(option));

        OIC_LOG_V(DEBUG, TAG, "the total payload length to be received is [%zu]bytes",
                  *totalPayloadLen);

        return true;
    }

    OIC_LOG(DEBUG, TAG, "OUT-CAIsPayloadLengthInPduWithBlockSizeOption");

    return false;
}

uint8_t CACheckBlockErrorType(CABlockData_t *currData, coap_block_t *receivedBlock,
                              const CAData_t *receivedData, uint16_t blockType,
                              size_t dataLen)
{
    OIC_LOG(DEBUG, TAG, "IN-CheckBlockError");

    VERIFY_NON_NULL(currData, TAG, "currData is NULL");
    VERIFY_NON_NULL(receivedBlock, TAG, "receivedBlock is NULL");
    VERIFY_NON_NULL(receivedData, TAG, "receivedData is NULL");

    // #1. check the received payload length
    size_t blockPayloadLen = 0;
    CAGetPayloadInfo(receivedData, &blockPayloadLen);

    // #2. check if the block sequence is right
    if (COAP_OPTION_BLOCK1 == blockType)
    {
        size_t prePayloadLen = currData->receivedPayloadLen;
        if (prePayloadLen != (size_t) BLOCK_SIZE(receivedBlock->szx) * receivedBlock->num)
        {
            if (receivedBlock->num > currData->block1.num + 1)
            {
                // 408 Error handling of block loss
                OIC_LOG(ERROR, TAG, "option1: error 4.08");
                OIC_LOG(ERROR, TAG, "it didn't order");
                return CA_BLOCK_INCOMPLETE;
            }
            return CA_BLOCK_RECEIVED_ALREADY;
        }
    }
    else if (COAP_OPTION_BLOCK2 == blockType)
    {
        if (receivedBlock->num != currData->block2.num)
        {
            if (receivedBlock->num > currData->block2.num)
            {
                // 408 Error handling of block loss
                OIC_LOG(ERROR, TAG, "option2: error 4.08");
                OIC_LOG(ERROR, TAG, "it didn't order");
                return CA_BLOCK_INCOMPLETE;
            }
            else
            {
                OIC_LOG(ERROR, TAG, "already received this block");
                return CA_BLOCK_RECEIVED_ALREADY;
            }
        }
    }

    // #3. check if error check logic is required
    size_t optionLen = dataLen - blockPayloadLen;
    if (receivedBlock->m && blockPayloadLen != (size_t) BLOCK_SIZE(receivedBlock->szx))
    {
        // 413 Error handling of too large entity
        if (COAP_MAX_PDU_SIZE < BLOCK_SIZE(receivedBlock->szx) + optionLen)
        {
            // buffer size is smaller than received block size
            OIC_LOG(ERROR, TAG, "error type 4.13");
            OIC_LOG(ERROR, TAG, "too large size");

            // set the block size to be smaller than COAP_MAX_PDU_SIZE
            for (int32_t size = CA_DEFAULT_BLOCK_SIZE; size >= 0; size--)
            {
                if (COAP_MAX_PDU_SIZE >= BLOCK_SIZE(size) + optionLen)
                {
                    OIC_LOG_V(ERROR, TAG, "replace sze with %d", size);
                    currData->block2.szx = size;
                    currData->block1.szx = size;
                    break;
                }
            }
            return CA_BLOCK_TOO_LARGE;
        }
        else
        {
            // 408 Error handling of payload loss
            OIC_LOG(ERROR, TAG, "error type 4.08");
            OIC_LOG(ERROR, TAG, "payload len != block sze");
            return CA_BLOCK_INCOMPLETE;
        }
    }
    else if (0 == receivedBlock->m && 0 != currData->payloadLength)
    {
        // if the received block is last block, check the total payload length
        size_t receivedPayloadLen = currData->receivedPayloadLen;
        receivedPayloadLen += blockPayloadLen;

        if (receivedPayloadLen != currData->payloadLength)
        {
            OIC_LOG(ERROR, TAG, "error type 4.08");
            OIC_LOG(ERROR, TAG, "total payload length is wrong");
            return CA_BLOCK_INCOMPLETE;
        }
    }

    OIC_LOG(DEBUG, TAG, "received all data normally");

    OIC_LOG(DEBUG, TAG, "OUT-CheckBlockError");

    return CA_BLOCK_UNKNOWN;
}

CAResult_t CAUpdatePayloadData(CABlockData_t *currData, const CAData_t *receivedData,
                               uint8_t status, bool isSizeOption, uint16_t blockType)
{
    OIC_LOG(DEBUG, TAG, "IN-UpdatePayloadData");

    VERIFY_NON_NULL(currData, TAG, "currData");
    VERIFY_NON_NULL(receivedData, TAG, "receivedData");

    // if error code is 4.08, do not update payload
    if (CA_BLOCK_INCOMPLETE == status)
    {
        OIC_LOG(ERROR, TAG, "no require to update");
        return CA_STATUS_OK;
    }

    size_t blockPayloadLen = 0;
    CAPayload_t blockPayload = CAGetPayloadInfo(receivedData, &blockPayloadLen);

    if (CA_BLOCK_TOO_LARGE == status)
    {
        blockPayloadLen = (COAP_OPTION_BLOCK2 == blockType) ?
                BLOCK_SIZE(currData->block2.szx) : BLOCK_SIZE(currData->block1.szx);
    }

    // memory allocation for the received block payload
    size_t prePayloadLen = currData->receivedPayloadLen;
    if (blockPayload)
    {
        if (currData->payloadLength)
        {
            // in case the block message has the size option
            // allocate the memory for the total payload
            if (isSizeOption)
            {
                CAPayload_t prePayload = currData->payload;

                OIC_LOG(DEBUG, TAG, "allocate memory for the total payload");
                currData->payload = (CAPayload_t) OICCalloc(1, currData->payloadLength);
                if (NULL == currData->payload)
                {
                    OIC_LOG(ERROR, TAG, "out of memory");
                    return CA_MEMORY_ALLOC_FAILED;
                }
                memcpy(currData->payload, prePayload, prePayloadLen);
                OICFree(prePayload);
            }

            // update the total payload
            memcpy(currData->payload + prePayloadLen, blockPayload, blockPayloadLen);
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "allocate memory for the received block payload");

            size_t totalPayloadLen = prePayloadLen + blockPayloadLen;
            CAPayload_t newPayload = OICRealloc(currData->payload, totalPayloadLen);
            if (NULL == newPayload)
            {
                OIC_LOG(ERROR, TAG, "out of memory");
                return CA_MEMORY_ALLOC_FAILED;
            }

            // update the total payload
            memset(newPayload + prePayloadLen, 0, blockPayloadLen);
            currData->payload = newPayload;
            memcpy(currData->payload + prePayloadLen, blockPayload, blockPayloadLen);
        }

        // update received payload length
        currData->receivedPayloadLen += blockPayloadLen;

        OIC_LOG_V(DEBUG, TAG, "updated payload: %s, len: %zu", currData->payload,
                  currData->receivedPayloadLen);
    }

    OIC_LOG(DEBUG, TAG, "OUT-UpdatePayloadData");
    return CA_STATUS_OK;
}

CAData_t* CACreateNewDataSet(const coap_pdu_t *pdu, const CAEndpoint_t *endpoint)
{
    VERIFY_NON_NULL_RET(pdu, TAG, "pdu", NULL);
    VERIFY_NON_NULL_RET(pdu->transport_hdr, TAG, "pdu->transport_hdr", NULL);
    VERIFY_NON_NULL_RET(endpoint, TAG, "endpoint", NULL);

    CARequestInfo_t* requestInfo = NULL;
    CAResponseInfo_t* responseInfo = NULL;

    uint32_t code = pdu->transport_hdr->udp.code;
    if (CA_GET == code || CA_POST == code || CA_PUT == code || CA_DELETE == code)
    {
        CAInfo_t responseData = { .tokenLength = pdu->transport_hdr->udp.token_length };
        responseData.token = (CAToken_t) OICMalloc(responseData.tokenLength);
        if (!responseData.token)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
            return NULL;
        }
        memcpy(responseData.token, pdu->transport_hdr->udp.token, responseData.tokenLength);

        responseInfo = (CAResponseInfo_t*) OICCalloc(1, sizeof(CAResponseInfo_t));
        if (!responseInfo)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
            OICFree(responseData.token);
            return NULL;
        }
        responseInfo->info = responseData;
    }
    else
    {
        CAInfo_t requestData = { .tokenLength = pdu->transport_hdr->udp.token_length };
        requestData.token = (CAToken_t) OICMalloc(requestData.tokenLength);
        if (!requestData.token)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
            return NULL;
        }
        memcpy(requestData.token, pdu->transport_hdr->udp.token, requestData.tokenLength);

        requestInfo = (CARequestInfo_t*) OICCalloc(1, sizeof(CARequestInfo_t));
        if (!requestInfo)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
            OICFree(requestData.token);
            return NULL;
        }
        requestInfo->info = requestData;

        // get resource uri information from received response message
        // to send next request message to remote device
        CAResponseInfo_t* resInfo = (CAResponseInfo_t*)OICCalloc(1, sizeof(*resInfo));
        if (!resInfo)
        {
            OIC_LOG(ERROR, TAG, "memory allocation failed");
            OICFree(requestData.token);
            OICFree(requestInfo);
            return NULL;
        }

        CAGetResponseInfoFromPDU(pdu, resInfo, endpoint);
        requestInfo->method = CA_GET;
        requestInfo->info.messageId = CAGetMessageIdFromPduBinaryData(pdu->transport_hdr,
                                                                      pdu->length);
        requestInfo->info.resourceUri = OICStrdup(resInfo->info.resourceUri);

        // after copying the resource uri, destroy response info.
        CADestroyResponseInfoInternal(resInfo);
    }

    CAData_t *data = (CAData_t *) OICCalloc(1, sizeof(CAData_t));
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "out of memory");
        if (NULL != requestInfo)
        {
            OICFree(requestInfo->info.resourceUri);
            OICFree(requestInfo->info.token);
            OICFree(requestInfo);
        }
        if (NULL != responseInfo)
        {
            OICFree(responseInfo->info.token);
            OICFree(responseInfo);
        }
        return NULL;
    }

    data->requestInfo = requestInfo;
    data->responseInfo = responseInfo;
    data->remoteEndpoint = CACloneEndpoint(endpoint);
    data->type = SEND_TYPE_UNICAST;

    return data;
}

CAData_t *CACloneCAData(const CAData_t *data)
{
    VERIFY_NON_NULL_RET(data, TAG, "data", NULL);

    CAData_t *clone = (CAData_t *) OICCalloc(1, sizeof(CAData_t));
    if (!clone)
    {
        OIC_LOG(ERROR, TAG, "out of memory");
        return NULL;
    }
    *clone = *data;

    if (data->requestInfo)
    {
        clone->requestInfo = CACloneRequestInfo(data->requestInfo);
    }
    else if (data->responseInfo)
    {
        clone->responseInfo = CACloneResponseInfo(data->responseInfo);
    }

    if (data->remoteEndpoint)
    {
        clone->remoteEndpoint = CACloneEndpoint(data->remoteEndpoint);
    }

    return clone;
}

CAResult_t CAUpdatePayloadToCAData(CAData_t *data, const CAPayload_t payload,
                                   size_t payloadLen)
{
    OIC_LOG(DEBUG, TAG, "IN-UpdatePayload");

    VERIFY_NON_NULL(data, TAG, "data is NULL");
    VERIFY_NON_NULL(payload, TAG, "payload is NULL");

    CAPayload_t newPayload = NULL;
    switch (data->dataType)
    {
        case CA_REQUEST_DATA:
            if (!data->requestInfo)
            {
                OIC_LOG(ERROR, TAG, "request info is null");
                return CA_STATUS_FAILED;
            }
            // allocate payload field
            newPayload = OICRealloc(data->requestInfo->info.payload, payloadLen);
            if (!newPayload)
            {
                OIC_LOG(ERROR, TAG, "out of memory");
                return CA_STATUS_FAILED;
            }
            data->requestInfo->info.payload = newPayload;
            memcpy(data->requestInfo->info.payload, payload, payloadLen);
            data->requestInfo->info.payloadSize = payloadLen;
            break;

        case CA_RESPONSE_DATA:
            if (!data->responseInfo)
            {
                OIC_LOG(ERROR, TAG, "response info is null");
                return CA_STATUS_FAILED;
            }
            // allocate payload field
            newPayload = OICRealloc(data->responseInfo->info.payload, payloadLen);
            if (!newPayload)
            {
                OIC_LOG(ERROR, TAG, "out of memory");
                return CA_STATUS_FAILED;
            }
            data->responseInfo->info.payload = newPayload;
            memcpy(data->responseInfo->info.payload, payload, payloadLen);
            data->responseInfo->info.payloadSize = payloadLen;
            break;

        default:
            // does not occur case
            OIC_LOG(ERROR, TAG, "not supported data type");
            return CA_NOT_SUPPORTED;
    }

    OIC_LOG(DEBUG, TAG, "OUT-UpdatePayload");

    return CA_STATUS_OK;
}

CAPayload_t CAGetPayloadInfo(const CAData_t *data, size_t *payloadLen)
{
    VERIFY_NON_NULL_RET(data, TAG, "data", NULL);
    VERIFY_NON_NULL_RET(payloadLen, TAG, "payloadLen", NULL);

    if (data->requestInfo)
    {
        if (data->requestInfo->info.payload)
        {
            *payloadLen = data->requestInfo->info.payloadSize;
            return data->requestInfo->info.payload;
        }
    }
    else if (data->responseInfo)
    {
        if (data->responseInfo->info.payload)
        {
            *payloadLen = data->responseInfo->info.payloadSize;
            return data->responseInfo->info.payload;
        }
    }

    return NULL;
}

CAResult_t CAHandleBlockErrorResponse(coap_block_t *block, uint16_t blockType,
                                      uint32_t responseResult)
{
    OIC_LOG(DEBUG, TAG, "IN-HandleBlockErrorRes");
    VERIFY_NON_NULL(block, TAG, "block is NULL");

    // update block data
    switch (responseResult)
    {
        case CA_REQUEST_ENTITY_INCOMPLETE:
            block->num = 0;
            break;
        case CA_REQUEST_ENTITY_TOO_LARGE:
            if (COAP_OPTION_BLOCK1 == blockType)
            {
                block->num++;
            }
            block->m = 0;
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "there is no Error Code of BWT[%d]", responseResult);
            return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT-HandleBlockErrorRes");
    return CA_STATUS_OK;
}

CAResult_t CAUpdateBlockOptionType(const CABlockDataID_t *blockID, uint8_t blockType)
{
    OIC_LOG(DEBUG, TAG, "IN-UpdateBlockOptionType");
    VERIFY_NON_NULL(blockID, TAG, "blockID");

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            currData->type = blockType;
            oc_mutex_unlock(g_context.blockDataListMutex);
            OIC_LOG(DEBUG, TAG, "OUT-UpdateBlockOptionType");
            return CA_STATUS_OK;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-UpdateBlockOptionType");
    return CA_STATUS_FAILED;
}

uint8_t CAGetBlockOptionType(const CABlockDataID_t *blockID)
{
    OIC_LOG(DEBUG, TAG, "IN-GetBlockOptionType");
    VERIFY_NON_NULL_RET(blockID, TAG, "blockID", 0);

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            oc_mutex_unlock(g_context.blockDataListMutex);
            OIC_LOG(DEBUG, TAG, "OUT-GetBlockOptionType");
            return currData->type;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-GetBlockOptionType");
    return 0;
}

CAData_t *CAGetDataSetFromBlockDataList(const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL_RET(blockID, TAG, "blockID", NULL);

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            oc_mutex_unlock(g_context.blockDataListMutex);
            return currData->sentData;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    return NULL;
}

CABlockData_t *CAUpdateDataSetFromBlockDataList(const CABlockDataID_t *blockID,
                                                const CAData_t *sendData)
{
    VERIFY_NON_NULL_RET(blockID, TAG, "blockID", NULL);
    VERIFY_NON_NULL_RET(sendData, TAG, "sendData", NULL);

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            CADestroyDataSet(currData->sentData);
            currData->sentData = CACloneCAData(sendData);
            oc_mutex_unlock(g_context.blockDataListMutex);
            return currData;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    return NULL;
}

CAResult_t CAGetTokenFromBlockDataList(const coap_pdu_t *pdu, const CAEndpoint_t *endpoint,
                                       CAResponseInfo_t *responseInfo)
{
    OIC_LOG(DEBUG, TAG, "IN-CAGetTokenFromBlockDataList");
    VERIFY_NON_NULL(pdu, TAG, "pdu");
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(responseInfo, TAG, "responseInfo");

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (NULL == currData)
        {
            continue;
        }

        if (NULL != currData->sentData && NULL != currData->sentData->requestInfo)
        {
            if (pdu->transport_hdr->udp.id == currData->sentData->requestInfo->info.messageId &&
                    endpoint->adapter == currData->sentData->remoteEndpoint->adapter)
            {
                if (NULL != currData->sentData->requestInfo->info.token)
                {
                    uint8_t length = currData->sentData->requestInfo->info.tokenLength;
                    responseInfo->info.tokenLength = length;
                    responseInfo->info.token = (char *) OICMalloc(length);
                    if (NULL == responseInfo->info.token)
                    {
                        OIC_LOG(ERROR, TAG, "out of memory");
                        oc_mutex_unlock(g_context.blockDataListMutex);
                        return CA_MEMORY_ALLOC_FAILED;
                    }
                    memcpy(responseInfo->info.token, currData->sentData->requestInfo->info.token,
                           responseInfo->info.tokenLength);

                    oc_mutex_unlock(g_context.blockDataListMutex);
                    OIC_LOG(DEBUG, TAG, "OUT-CAGetTokenFromBlockDataList");
                    return CA_STATUS_OK;
                }
            }
        }
    }

    oc_mutex_unlock(g_context.blockDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-CAGetTokenFromBlockDataList");
    return CA_STATUS_FAILED;
}

CAResult_t CACheckBlockDataValidation(const CAData_t *sendData, CABlockData_t **blockData)
{
    VERIFY_NON_NULL(sendData, TAG, "sendData");
    VERIFY_NON_NULL(blockData, TAG, "blockData");

    if (sendData->responseInfo)
    {
        CABlockDataID_t* blockDataID = CACreateBlockDatablockId(
                (CAToken_t)sendData->responseInfo->info.token,
                sendData->responseInfo->info.tokenLength,
                sendData->remoteEndpoint->port);
        if (NULL == blockDataID || blockDataID->idLength < 1)
        {
            OIC_LOG(ERROR, TAG, "blockId is null");
            CADestroyBlockID(blockDataID);
            return CA_STATUS_FAILED;
        }

        CABlockData_t *updatedData = CAUpdateDataSetFromBlockDataList(blockDataID, sendData);
        if (updatedData)
        {
            OIC_LOG(DEBUG, TAG, "Send response about the received block request.");
            *blockData = updatedData;
            CADestroyBlockID(blockDataID);
            return CA_STATUS_OK;
        }
        CADestroyBlockID(blockDataID);
    }

    return CA_STATUS_FAILED;
}

CABlockData_t *CAGetBlockDataFromBlockDataList(const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL_RET(blockID, TAG, "blockID", NULL);

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            oc_mutex_unlock(g_context.blockDataListMutex);
            return currData;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    return NULL;
}

coap_block_t *CAGetBlockOption(const CABlockDataID_t *blockID, uint16_t blockType)
{
    OIC_LOG(DEBUG, TAG, "IN-GetBlockOption");
    VERIFY_NON_NULL_RET(blockID, TAG, "blockID", NULL);

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            oc_mutex_unlock(g_context.blockDataListMutex);
            OIC_LOG(DEBUG, TAG, "OUT-GetBlockOption");
            if (COAP_OPTION_BLOCK2 == blockType)
            {
                return &currData->block2;
            }
            else if (COAP_OPTION_BLOCK1 == blockType)
            {
                return &currData->block1;
            }
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-GetBlockOption");
    return NULL;
}

CAPayload_t CAGetPayloadFromBlockDataList(const CABlockDataID_t *blockID,
                                          size_t *fullPayloadLen)
{
    OIC_LOG(DEBUG, TAG, "IN-GetFullPayload");
    VERIFY_NON_NULL_RET(blockID, TAG, "blockID", NULL);
    VERIFY_NON_NULL_RET(fullPayloadLen, TAG, "fullPayloadLen", NULL);

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            oc_mutex_unlock(g_context.blockDataListMutex);
            *fullPayloadLen = currData->receivedPayloadLen;
            OIC_LOG(DEBUG, TAG, "OUT-GetFullPayload");
            return currData->payload;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-GetFullPayload");
    return NULL;
}

CABlockData_t *CACreateNewBlockData(const CAData_t *sendData)
{
    OIC_LOG(DEBUG, TAG, "IN-CACreateNewBlockData");
    VERIFY_NON_NULL_RET(sendData, TAG, "sendData", NULL);

    // create block data
    CABlockData_t *data = (CABlockData_t *) OICCalloc(1, sizeof(CABlockData_t));
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "memory alloc has failed");
        return NULL;
    }

    data->block1.szx = CA_DEFAULT_BLOCK_SIZE;
    data->block2.szx = CA_DEFAULT_BLOCK_SIZE;
    data->sentData = CACloneCAData(sendData);
    if (!data->sentData)
    {
        OIC_LOG(ERROR, TAG, PCF("memory alloc has failed"));
        OICFree(data);
        return NULL;
    }

    CAToken_t token = NULL;
    uint8_t tokenLength = 0;
    if (data->sentData->requestInfo)
    {
        // update token info
        tokenLength = data->sentData->requestInfo->info.tokenLength;
        token = data->sentData->requestInfo->info.token;
    }
    else if (data->sentData->responseInfo)
    {
        tokenLength = data->sentData->responseInfo->info.tokenLength;
        token = data->sentData->responseInfo->info.token;
    }

    if (!data->sentData->remoteEndpoint)
    {
        OIC_LOG(ERROR, TAG, "remoteEndpoint is null");
        CADestroyDataSet(data->sentData);
        OICFree(data);
        return NULL;
    }

    CABlockDataID_t* blockDataID = CACreateBlockDatablockId(token, tokenLength,
                                                            data->sentData->remoteEndpoint->port);
    if (NULL == blockDataID || blockDataID->idLength < 1)
    {
        OIC_LOG(ERROR, TAG, "blockId is null");
        CADestroyBlockID(blockDataID);
        CADestroyDataSet(data->sentData);
        OICFree(data);
        return NULL;
    }
    data->blockDataId = blockDataID;
    data->ttl = GetTicks(MAX_BLOCK_DATA_TIMEOUT_SECONDS * MILLISECONDS_PER_SECOND);

    oc_mutex_lock(g_context.blockDataListMutex);

    bool res = u_arraylist_add(g_context.dataList, (void *) data);
    if (!res)
    {
        OIC_LOG(ERROR, TAG, "add has failed");
        CADestroyBlockID(data->blockDataId);
        CADestroyDataSet(data->sentData);
        OICFree(data);
        oc_mutex_unlock(g_context.blockDataListMutex);
        return NULL;
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-CreateBlockData");
    return data;
}

CAResult_t CARemoveBlockDataFromList(const CABlockDataID_t *blockID)
{
    OIC_LOG(DEBUG, TAG, "CARemoveBlockData");
    VERIFY_NON_NULL(blockID, TAG, "blockID");

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *currData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(currData, blockID))
        {
            CABlockData_t *removedData = u_arraylist_remove(g_context.dataList, i);
            if (!removedData)
            {
                OIC_LOG(ERROR, TAG, "data is NULL");
                oc_mutex_unlock(g_context.blockDataListMutex);
                return CA_STATUS_FAILED;
            }

            // destroy memory
            CADestroyDataSet(removedData->sentData);
            CADestroyBlockID(removedData->blockDataId);
            OICFree(removedData->payload);
            OICFree(removedData);
            oc_mutex_unlock(g_context.blockDataListMutex);
            return CA_STATUS_OK;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    return CA_STATUS_OK;
}

CAResult_t CARemoveAllBlockDataFromList()
{
    OIC_LOG(DEBUG, TAG, "CARemoveAllBlockDataFromList");

    oc_mutex_lock(g_context.blockDataListMutex);

    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = len; i > 0; i--)
    {
        CABlockData_t *removedData = u_arraylist_remove(g_context.dataList, i - 1);
        if (removedData)
        {
            // destroy memory
            if (removedData->sentData)
            {
                CADestroyDataSet(removedData->sentData);
            }
            CADestroyBlockID(removedData->blockDataId);
            OICFree(removedData->payload);
            OICFree(removedData);
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);

    return CA_STATUS_OK;
}

void CADestroyDataSet(CAData_t* data)
{
    VERIFY_NON_NULL_VOID(data, TAG, "data");

    if (data->remoteEndpoint)
    {
        CAFreeEndpoint(data->remoteEndpoint);
        data->remoteEndpoint = NULL;
    }
    if (data->requestInfo)
    {
        CADestroyRequestInfoInternal(data->requestInfo);
        data->requestInfo = NULL;
    }
    if (data->responseInfo)
    {
        CADestroyResponseInfoInternal(data->responseInfo);
        data->responseInfo = NULL;
    }
    OICFree(data);
}

CABlockDataID_t* CACreateBlockDatablockId(const CAToken_t token, uint8_t tokenLength,
                                          uint16_t portNumber)
{
    char port[PORT_LENGTH] = { 0, };
    port[0] = (char) ((portNumber >> 8) & 0xFF);
    port[1] = (char) (portNumber & 0xFF);

    CABlockDataID_t* blockDataID = (CABlockDataID_t *) OICMalloc(sizeof(CABlockDataID_t));
    if (!blockDataID)
    {
        OIC_LOG(ERROR, TAG, "memory alloc has failed");
        return NULL;
    }
    blockDataID->idLength = tokenLength + sizeof(port);
    blockDataID->id = (uint8_t *) OICMalloc(blockDataID->idLength);
    if (!blockDataID->id)
    {
        OIC_LOG(ERROR, TAG, "memory alloc has failed");
        OICFree(blockDataID);
        return NULL;
    }

    if (token)
    {
        memcpy(blockDataID->id, token, tokenLength);
    }

    memcpy(blockDataID->id + tokenLength, port, sizeof(port));

    OIC_LOG(DEBUG, TAG, "BlockID is ");
    OIC_LOG_BUFFER(DEBUG, TAG, (const uint8_t *)blockDataID->id, blockDataID->idLength);

    return blockDataID;
}

void CADestroyBlockID(CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL_VOID(blockID, TAG, "blockID");
    OICFree(blockID->id);
    OICFree(blockID);
    blockID = NULL;
}

bool CABlockidMatches(const CABlockData_t *currData, const CABlockDataID_t *blockID)
{
    VERIFY_NON_NULL_RET(currData, TAG, "currData", false);
    VERIFY_NON_NULL_RET(blockID, TAG, "blockID", false);
    VERIFY_NON_NULL_RET(blockID->id, TAG, "blockID->id", false);

    if ((currData->blockDataId)
        && (currData->blockDataId->id)
        && (currData->blockDataId->idLength == blockID->idLength)
        && !memcmp(currData->blockDataId->id, blockID->id, currData->blockDataId->idLength))
    {
        return true;
    }
    return false;
}

void CALogBlockInfo(coap_block_t *block)
{
    VERIFY_NON_NULL_VOID(block, TAG, "block");

    OIC_LOG(DEBUG, TAG, "block option info");

    OIC_LOG_V(DEBUG, TAG, "block option-num : %d", block->num);

    OIC_LOG_V(DEBUG, TAG, "block option-m   : %d", block->m);

    OIC_LOG_V(DEBUG, TAG, "block option-szx : %d", block->szx);
}

CAResult_t CARemoveBlockDataFromListWithSeed(const CAToken_t token, uint8_t tokenLength,
                                             uint16_t portNumber)
{
    CABlockDataID_t* blockDataID = CACreateBlockDatablockId(token, tokenLength, portNumber);
    if (NULL == blockDataID || blockDataID->idLength < 1)
    {
        OIC_LOG(ERROR, TAG, "blockId is null");
        CADestroyBlockID(blockDataID);
        return CA_STATUS_FAILED;
    }

    CAResult_t res = CARemoveBlockDataFromList(blockDataID);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CARemoveBlockDataFromList failed");
    }

    CADestroyBlockID(blockDataID);
    return res;
}

uint32_t GetTicks(uint32_t milliSeconds)
{
    coap_tick_t now;
    coap_ticks(&now);

    // Guard against overflow of uint32_t
    if (milliSeconds <= ((UINT32_MAX - (uint32_t)now) * MILLISECONDS_PER_SECOND) /
                             COAP_TICKS_PER_SECOND)
    {
        return now + (milliSeconds * COAP_TICKS_PER_SECOND)/MILLISECONDS_PER_SECOND;
    }
    else
    {
        return UINT32_MAX;
    }
}

void CAResetBlockDataTTL(const CABlockDataID_t *blockID)
{
    oc_mutex_lock(g_context.blockDataListMutex);
    size_t len = u_arraylist_length(g_context.dataList);
    for (size_t i = 0; i < len; i++)
    {
        CABlockData_t *blockData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (CABlockidMatches(blockData, blockID))
        {
            blockData->ttl = GetTicks(MAX_BLOCK_DATA_TIMEOUT_SECONDS * MILLISECONDS_PER_SECOND);
            oc_mutex_unlock(g_context.blockDataListMutex);
            return;
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);
}

void CACheckAndDeleteTimedOutBlockData()
{
    coap_tick_t now;
    coap_ticks(&now);

    oc_mutex_lock(g_context.blockDataListMutex);
    for (size_t i = 0; i < u_arraylist_length(g_context.dataList); i++)
    {
        CABlockData_t *blockData = (CABlockData_t *) u_arraylist_get(g_context.dataList, i);
        if (blockData && blockData->ttl < now)
        {
            OIC_LOG(INFO, TAG, "Deleting timed-out BlockData");
            OIC_LOG(DEBUG, TAG, "BlockID is ");
            OIC_LOG_BUFFER(DEBUG, TAG, (const uint8_t *) blockData->blockDataId->id,
                           blockData->blockDataId->idLength);

            blockData = (CABlockData_t *) u_arraylist_remove(g_context.dataList, i);
            if (blockData)
            {
                // destroy memory
                CADestroyDataSet(blockData->sentData);
                CADestroyBlockID(blockData->blockDataId);
                OICFree(blockData->payload);
                OICFree(blockData);
            }
        }
    }
    oc_mutex_unlock(g_context.blockDataListMutex);
}
