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

#include <stdio.h>
#include <string.h>
#include "routingutility.h"
#include "routingmanager.h"
#include "oic_malloc.h"
#include "include/logger.h"

/**
 * Logging tag for module name.
 */
#define TAG "OIC_RM_UTIL"

/**
 * Tag for printing the logs of forwarding the packet.
 */
#define RM_TAG "OIC_RM_RAP"

/**
 * Default routing option data length is 1 byte with any of the following values.
 * 00 - source and destination address is not present.
 * 01 - source and destination address is present with message type as ACK.
 * 10 - source and destination address is present with message type as RESET.
 * 11 - source and destination address is present with message type as NORMAL.
 */
#define DEFAULT_ROUTE_OPTION_LEN 1

/**
 * Minimum routing option data length is
 * Msg Type(1 bytes) + length of src address(1byte) + length of destination address(1byte) +
 * Seq Num(2bytes)
 */
#define MIN_ROUTE_OPTION_LEN 5

/**
 * Acknowledge message type is denoted as 01000000
 */
#define ACK_MESSAGE_TYPE (1 << 6)

/**
 * Reset message type is denoted as 10000000
 */
#define RST_MESSAGE_TYPE (1 << 7)

/**
 * Normal message type is denoted as 11000000
 */
#define NORMAL_MESSAGE_TYPE (3 << 6)

/**
 * Stack mode.
 */
static OCMode g_rmStackMode = OC_CLIENT;

void RMSetStackMode(OCMode mode)
{
    g_rmStackMode = mode;
}

// destination and source are <GatewayId><ClientId> here, where ClientId is optional.
OCStackResult RMAddInfo(const char *destination, void *message, bool isRequest,
                        bool *doPost)
{
    RM_NULL_CHECK_WITH_RET(message, TAG, "options");

    CAHeaderOption_t **options = NULL;
    uint8_t *numOptions = NULL;

    if (isRequest)
    {
        CARequestInfo_t *requestMsg = message;
        options = &(requestMsg->info.options);
        RM_NULL_CHECK_WITH_RET(options, TAG, "options");
        numOptions = &(requestMsg->info.numOptions);
        RM_NULL_CHECK_WITH_RET(numOptions, TAG, "numOptions");
    }
    else
    {
        CAResponseInfo_t *respMsg = message;
        if ('\0' == destination[0] && (CA_EMPTY == respMsg->result))
        {
            OIC_LOG(DEBUG, TAG, "Response is for an Endpoint, No need to add the routing Option");
            return OC_STACK_OK;
        }
        options = &(respMsg->info.options);
        RM_NULL_CHECK_WITH_RET(options, TAG, "options");
        numOptions = &(respMsg->info.numOptions);
        RM_NULL_CHECK_WITH_RET(numOptions, TAG, "numOptions");
    }


    CAHeaderOption_t *optionPtr = NULL;
    int8_t index = -1;

    RMGetRouteOptionIndex(*options, *numOptions, &index);

    if (-1 < index)
    {
        OIC_LOG(INFO, TAG, "Route option is present");
        optionPtr = *options;
    }
    else
    {
        OIC_LOG(INFO, TAG, "Route option is not present");
        index = *numOptions;
        optionPtr = OICCalloc((*numOptions + 1), sizeof(CAHeaderOption_t));
        if (!optionPtr)
        {
            OIC_LOG(ERROR, TAG, "OICCalloc failed");
            return OC_STACK_NO_MEMORY;
        }

        memcpy(optionPtr, *options, sizeof(CAHeaderOption_t) * (*numOptions));
    }

    OCStackResult res = OC_STACK_OK;
    RMRouteOption_t routeOption = {.destGw = 0};
    if (*numOptions != index)
    {
        OIC_LOG(INFO, TAG, "Route option is already present");
        res = RMParseRouteOption(&optionPtr[index], &routeOption);
        if (OC_STACK_OK != res)
        {
            OIC_LOG(ERROR, TAG, "RMParseRouteOption failed");
            return OC_STACK_ERROR;
        }
    }

    if (!isRequest)
    {
        CAResponseInfo_t *respMsg = message;
        if (CA_EMPTY == respMsg->result && CA_MSG_ACKNOWLEDGE == respMsg->info.type)
        {
            OIC_LOG(DEBUG, TAG, "CA_EMPTY WITH ACKNOWLEDGEMENT");
            routeOption.msgType = ACK;
            if (OC_SERVER == g_rmStackMode)
            {
                OIC_LOG(DEBUG, TAG, "This is server mode");
                // Send the Empty message in the response with adding the MSGType in Route option.
                respMsg->info.type = CA_MSG_NONCONFIRM;
                respMsg->result = CA_CONTENT;
            }
            else
            {
                OIC_LOG(DEBUG, TAG, "Send a POST request");
                if (NULL != doPost)
                {
                    *doPost = true;
                }
            }
        }
        else if (CA_EMPTY == respMsg->result && CA_MSG_RESET == respMsg->info.type)
        {
            OIC_LOG(DEBUG, TAG, "CA_EMPTY WITH RESET");
            routeOption.msgType = RST;
            respMsg->info.type = CA_MSG_NONCONFIRM;
            respMsg->result = CA_CONTENT;
        }
    }

    if(destination)
    {
        memcpy(&(routeOption.destGw), destination, sizeof(routeOption.destGw));
        memcpy(&(routeOption.destEp), destination + sizeof(routeOption.destGw),
               sizeof(routeOption.destEp));
    }

#ifdef ROUTING_GATEWAY
    // A gateway is supposed to add its ID as source.
    uint32_t gatewayId = RMGetGatewayId();
    if (gatewayId)
    {
        memcpy(&(routeOption.srcGw), &gatewayId, sizeof(routeOption.srcGw));
    }

    if(!routeOption.destGw)
    {
        routeOption.mSeqNum = RMGetMcastSeqNumber();
    }
#endif

    res = RMCreateRouteOption(&routeOption, optionPtr + index);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Creation of routing option failed");
        OICFree(optionPtr);
        return res;
    }

    if ((*numOptions) == index )
    {
        (*numOptions) = (*numOptions) + 1;
        OICFree(*options);
        *options = optionPtr;
    }

    return OC_STACK_OK;
}

OCStackResult RMUpdateInfo(CAHeaderOption_t **options, uint8_t *numOptions,
                           CAEndpoint_t *endpoint)
{
    RM_NULL_CHECK_WITH_RET(options, TAG, "options");
    RM_NULL_CHECK_WITH_RET(*options, TAG, "invalid option");
    RM_NULL_CHECK_WITH_RET(numOptions, TAG, "numOptions");
    RM_NULL_CHECK_WITH_RET(endpoint, TAG, "endpoint");

    if (0 >= *numOptions)
    {
        OIC_LOG(ERROR, TAG, "Invalid arguement: numOptions");
        return OC_STACK_ERROR;
    }

    int8_t routeIndex = -1;
    RMGetRouteOptionIndex(*options, *numOptions, &routeIndex);

    if (-1 >= routeIndex)
    {
        OIC_LOG(DEBUG, TAG, "Nothing to remove.");
        return OC_STACK_OK;
    }

    // Update Endpoint with source address from RM header option.
    if (DEFAULT_ROUTE_OPTION_LEN < (*options + routeIndex)->optionLength )
    {
        uint8_t dLen = 0;
        uint16_t count = sizeof(dLen) + DEFAULT_ROUTE_OPTION_LEN;
        memcpy(&dLen, (*options + routeIndex)->optionData + DEFAULT_ROUTE_OPTION_LEN, sizeof(dLen));
        count += dLen;
        uint8_t sLen = 0;
        memcpy(&sLen, (*options + routeIndex)->optionData + count, sizeof(sLen));
        count += sizeof(sLen);
        if (0 < sLen)
        {
            memcpy(endpoint->routeData, (*options + routeIndex)->optionData + count,
                   GATEWAY_ID_LENGTH);
            OIC_LOG_V(DEBUG, TAG, "adding srcgid: %u in endpoint [%d]",
                     *((uint32_t *)endpoint->routeData), sLen);

            count += GATEWAY_ID_LENGTH;

            if (GATEWAY_ID_LENGTH < sLen)
            {
                memcpy(endpoint->routeData + GATEWAY_ID_LENGTH,
                       (*options + routeIndex)->optionData + count, ENDPOINT_ID_LENGTH);
                OIC_LOG_V(DEBUG, TAG, "adding srceid: %u in endpoint",
                         *((uint16_t *)(endpoint->routeData + GATEWAY_ID_LENGTH)));
            }
        }
    }

    // Remove route option from header.
    for (uint8_t i = routeIndex; i < (*numOptions)-1; i++)
    {
        memcpy((*options) + i, (*options)+i+1, sizeof(**options));
    }
    *numOptions = (*numOptions) - 1;

    if (0 == *numOptions)
    {
        // Remove route option.
        OICFree(*options);
        *options = NULL;
    }
    return OC_STACK_OK;
}

void RMGetRouteOptionIndex(const CAHeaderOption_t *options, uint8_t numOptions, int8_t *index)
{
    RM_NULL_CHECK_VOID(index, TAG, "index");

    if (NULL == options)
    {
        OIC_LOG(INFO, TAG, "No existing options");
        return;
    }

    for (uint32_t i = 0; i < numOptions; i++)
    {
        OIC_LOG_V(DEBUG, TAG, "Request- optionID: %u", options[i].optionID);
        if (RM_OPTION_MESSAGE_SWITCHING == options[i].optionID)
        {
            OIC_LOG_V(INFO, TAG, "Found Option at %d", i);
            *index = i;
            break;
        }
    }
}

OCStackResult RMCreateRouteOption(const RMRouteOption_t *optValue, CAHeaderOption_t *options)
{
    RM_NULL_CHECK_WITH_RET(optValue, RM_TAG, "optValue");
    RM_NULL_CHECK_WITH_RET(options, RM_TAG, "options");

    uint8_t dLen = (optValue->destGw ? GATEWAY_ID_LENGTH:0) +
                        (optValue->destEp ? ENDPOINT_ID_LENGTH:0);
    uint8_t sLen = (optValue->srcGw ? GATEWAY_ID_LENGTH:0) +
                        (optValue->srcEp ? ENDPOINT_ID_LENGTH:0);

    OIC_LOG_V(DEBUG, RM_TAG, "createoption dlen %u slen [%u]", dLen, sLen);

    unsigned int totalLength = 0;
    uint8_t *tempData = NULL;

    if (0 == dLen && 0 == sLen)
    {
        OIC_LOG(DEBUG, RM_TAG, "Source and destination is not present");
        totalLength = DEFAULT_ROUTE_OPTION_LEN;
        tempData = OICCalloc(totalLength, sizeof(char));
        if (NULL == tempData)
        {
            OIC_LOG(ERROR, RM_TAG, "Calloc failed");
            return OC_STACK_NO_MEMORY;
        }

        if (ACK == optValue->msgType)
        {
            OIC_LOG(DEBUG, RM_TAG, "OptValue ACK Message Type");
            memset(tempData, ACK_MESSAGE_TYPE, DEFAULT_ROUTE_OPTION_LEN);
        }
        else if (RST == optValue->msgType)
        {
            OIC_LOG(DEBUG, RM_TAG, "OptValue RST Message Type");
            memset(tempData, RST_MESSAGE_TYPE, DEFAULT_ROUTE_OPTION_LEN);
        }
        else
        {
            OIC_LOG(DEBUG, RM_TAG, "OptValue NOR Message Type");
            memset(tempData, NORMAL_MESSAGE_TYPE, DEFAULT_ROUTE_OPTION_LEN);
        }
    }
    else
    {
        totalLength = MIN_ROUTE_OPTION_LEN + dLen + sLen;
        tempData = OICCalloc(totalLength, sizeof(char));
        if (NULL == tempData)
        {
            OIC_LOG(ERROR, RM_TAG, "Calloc failed");
            return OC_STACK_NO_MEMORY;
        }

        if (ACK == optValue->msgType)
        {
            OIC_LOG(DEBUG, RM_TAG, "OptValue ACK Message Type");
            memset(tempData, ACK_MESSAGE_TYPE, DEFAULT_ROUTE_OPTION_LEN);
        }
        else if (RST == optValue->msgType)
        {
            OIC_LOG(DEBUG, RM_TAG, "OptValue RST Message Type");
            memset(tempData, RST_MESSAGE_TYPE, DEFAULT_ROUTE_OPTION_LEN);
        }
        else
        {
            OIC_LOG(DEBUG, RM_TAG, "OptValue NOR Message Type");
            memset(tempData, NORMAL_MESSAGE_TYPE, DEFAULT_ROUTE_OPTION_LEN);
        }

        memcpy(tempData + DEFAULT_ROUTE_OPTION_LEN, &dLen, sizeof(dLen));
        unsigned int count = sizeof(dLen) + DEFAULT_ROUTE_OPTION_LEN;
        if (0 < dLen)
        {
            if (optValue->destGw)
            {
                memcpy(tempData + count, &(optValue->destGw), GATEWAY_ID_LENGTH);
                count += GATEWAY_ID_LENGTH;
            }

            if (optValue->destEp)
            {
                memcpy(tempData + count, &(optValue->destEp), ENDPOINT_ID_LENGTH);
                count += ENDPOINT_ID_LENGTH;
            }
        }

        memcpy(tempData + count, &sLen, sizeof(sLen));
        count += sizeof(sLen);
        if (0 < sLen)
        {
            if (optValue->srcGw)
            {
                memcpy(tempData + count, &(optValue->srcGw), GATEWAY_ID_LENGTH);
                count += GATEWAY_ID_LENGTH;
            }

            if (optValue->srcEp)
            {
                memcpy(tempData + count, &(optValue->srcEp), ENDPOINT_ID_LENGTH);
                count += ENDPOINT_ID_LENGTH;
            }
        }

        memcpy(tempData + count, &optValue->mSeqNum, sizeof(optValue->mSeqNum));
    }

    memcpy(options->optionData, tempData, totalLength);

    options->optionID = RM_OPTION_MESSAGE_SWITCHING;
    options->optionLength = totalLength;

    OIC_LOG_V(INFO, RM_TAG, "Option Length is %d", options->optionLength);

    OICFree(tempData);
    return OC_STACK_OK;
}

OCStackResult RMParseRouteOption(const CAHeaderOption_t *options, RMRouteOption_t *optValue)
{
    RM_NULL_CHECK_WITH_RET(options, RM_TAG, "options");
    RM_NULL_CHECK_WITH_RET(optValue, RM_TAG, "optValue");
    if (0 == options->optionLength)
    {
        OIC_LOG(ERROR, RM_TAG, "Option data is not present");
        return OC_STACK_ERROR;
    }

    OIC_LOG_V(DEBUG, RM_TAG, "Option Length is %d", options->optionLength);
    uint8_t mType = 0;
    memcpy(&mType, options->optionData, sizeof(mType));

    if (ACK_MESSAGE_TYPE == mType)
    {
        OIC_LOG(INFO, RM_TAG, "ACK_MESSAGE_TYPE");
        optValue->msgType = ACK;
    }
    else if (RST_MESSAGE_TYPE == mType)
    {
        OIC_LOG(INFO, RM_TAG, "RST_MESSAGE_TYPE");
        optValue->msgType = RST;
    }
    else if (NORMAL_MESSAGE_TYPE == mType)
    {
        OIC_LOG(INFO, RM_TAG, "NOR_MESSAGE_TYPE");
        optValue->msgType = NOR;
    }

    if (DEFAULT_ROUTE_OPTION_LEN == options->optionLength)
    {
        OIC_LOG(INFO, RM_TAG, "No source and destination are present");
    }
    else
    {
        uint8_t dLen = 0 ;
        uint16_t count = DEFAULT_ROUTE_OPTION_LEN;
        memcpy(&dLen, options->optionData + count, sizeof(dLen));
        count += sizeof(dLen);
        if (0 < dLen)
        {
            memcpy(&(optValue->destGw), options->optionData + count, GATEWAY_ID_LENGTH);
            count += GATEWAY_ID_LENGTH;

            if (GATEWAY_ID_LENGTH < dLen)
            {
                memcpy(&(optValue->destEp), options->optionData + count, ENDPOINT_ID_LENGTH);
                count += ENDPOINT_ID_LENGTH;
            }
        }

        uint8_t sLen = 0;
        memcpy(&sLen, options->optionData + count, sizeof(sLen));
        count += sizeof(sLen);
        if (0 < sLen)
        {
            memcpy(&(optValue->srcGw), options->optionData + count, GATEWAY_ID_LENGTH);
            count += GATEWAY_ID_LENGTH;

            if (GATEWAY_ID_LENGTH < sLen)
            {
                memcpy(&(optValue->srcEp), options->optionData + count, ENDPOINT_ID_LENGTH);
                count += ENDPOINT_ID_LENGTH;
            }
        }
        memcpy(&optValue->mSeqNum, options->optionData + count, sizeof(optValue->mSeqNum));
    }

    OIC_LOG_V(INFO, RM_TAG, "Option hopcount is %d", optValue->mSeqNum);
    OIC_LOG_V(INFO, RM_TAG, "Option Sender Addr is [%u][%u]", optValue->srcGw, optValue->srcEp);
    OIC_LOG_V(INFO, RM_TAG, "Option Dest Addr is [%u][%u]", optValue->destGw, optValue->destEp);
    OIC_LOG_V(INFO, RM_TAG, "Message Type is [%u]", optValue->msgType);
    return OC_STACK_OK;
}
