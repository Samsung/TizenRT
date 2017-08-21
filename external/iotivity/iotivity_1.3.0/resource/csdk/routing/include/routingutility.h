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

/**
 * @file
 * This file contains the utility functions used by Routing manager and RI
 */

#ifndef ROUTING_UTILITY_H_
#define ROUTING_UTILITY_H_

//TODO Endpoint will also include this file, remove unnecessary includes.

#include "cacommon.h"
#include "octypes.h"
#ifdef ROUTING_GATEWAY
#include "routingmanager.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Maximum source or destination address length added to Route Option.
 */
#define MAX_ADDR_LEN 40

/**
 * Gateway ID length.
 */
#define GATEWAY_ID_LENGTH sizeof(uint32_t)

/**
 * Endpoint ID length.
 */
#define ENDPOINT_ID_LENGTH sizeof(uint16_t)

/**
 * Routing option number.
 */
// TODO: We need to define proper Option number.
#define RM_OPTION_MESSAGE_SWITCHING 65524

/**
 * Macro to verify the validity of input argument with the return.
 */
#define RM_NULL_CHECK_WITH_RET(arg, log_tag, log_message) \
    if (NULL == arg ){ \
        OIC_LOG_V(ERROR, log_tag, "Invalid input:%s", log_message); \
        return OC_STACK_INVALID_PARAM; \
    } \

/**
 * Macro to verify the validity of input argument.
 */
#define RM_NULL_CHECK_VOID(arg, log_tag, log_message) \
    if (NULL == arg ){ \
        OIC_LOG_V(ERROR, log_tag, "Invalid input:%s", log_message); \
        return; \
    } \

/**
 * Macro to verify the return of an API.
 */
#define RM_VERIFY_SUCCESS(op, successCode) { if (op != successCode) \
            {OIC_LOG_V(ERROR, TAG, "%s failed!!", #op); goto exit;} }

/**
 * Message types in RouteOption to differentiate a normal response and Empty response.
 */
typedef enum
{
   NOR = 0,  /**< Normal Message. */
   ACK,      /**< Empty Acknowledgement message. */
   RST      /**< Empty Reset message. */
}MSGType;

/**
 * This structure is used to hold the source address, destination address, message type and
 * sequence number. This collectively forms the value of Route option in the message.
 */
typedef struct
{
    uint32_t srcGw;               /**< Source gateway for this packet. */
    uint32_t destGw;              /**< Destination gateway for this packet. */
    uint16_t mSeqNum;             /**< Multicast sequence Number. */
    uint16_t srcEp;               /**< Source endpoint for this packet. */
    uint16_t destEp;              /**< Destination endpoint for this packet. */
    uint8_t msgType;              /**< Type of Message: Empty or normal. */
} RMRouteOption_t;

/**
 * To set the stack mode in Routing manager.
 */
void RMSetStackMode(OCMode mode);

/**
 * Adds the destination address to the Route options.
 * If Route option is already present, it adds the destination address information to
 * Route option else creates a new Route option with the destination address info.
 * @param[in]       destination     Destination address.
 * @param[in,out]   message         Request/response message to add the route option
 * @param[in]       isRequest       True if message is request else false.
 * @param[out]      doPost          True if a POST message be sent for empty packet to
 *                                  Routing gateway.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 */
OCStackResult RMAddInfo(const char *destination, void *message, bool isRequest,
                        bool *doPost);

/**
 * Removes the Route Option from the header options.
 * @param[in,out]   options     Header options present in request/response message.
 * @param[in,out]   numOptions  Number of options present in request/response message.
 * @param[in,out]   endpoint    Remote address updated with the actual source of request/response.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 */
OCStackResult RMUpdateInfo(CAHeaderOption_t **options, uint8_t *numOptions,
                           CAEndpoint_t *endpoint);

/**
 * Gets the index of the routing option if present.
 * @param[in]    options     Header options present in request/response message.
 * @param[in]    numOptions  Number of options present in request/response message.
 * @param[out]   index       Index of the route option present in Header options.
 * @return  NONE.
 */
void RMGetRouteOptionIndex(const CAHeaderOption_t *options, uint8_t numOptions,
                           int8_t *index);

/**
 * To create a Routing option from the CARouteOption_t structure.
 * @param[in]    optValue    Routing information.
 * @param[out]   options     Routing information in the form of Header options.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 */
OCStackResult RMCreateRouteOption(const RMRouteOption_t *optValue, CAHeaderOption_t *options);

/**
 * To parse the routing option from the Headeroptions.
 * @param[in]    options    Routing information in the form of Header options.
 * @param[out]   optValue   Route information after parsing.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 */
OCStackResult RMParseRouteOption(const CAHeaderOption_t *options, RMRouteOption_t *optValue);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ROUTING_MANAGER_H_ */
