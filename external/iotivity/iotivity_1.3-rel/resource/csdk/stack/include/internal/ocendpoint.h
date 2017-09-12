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
 * This file contains APIs for Endpoint.
 */
#ifndef OC_ENDPOINT_H
#define OC_ENDPOINT_H

#include <stdio.h>
#include <stdint.h>
#include "octypes.h"
#include "cacommon.h"

#define OC_MAX_TPS_STR_SIZE          (12)
#define OC_MAX_ADDR_STR_SIZE         (46)
#define OC_MAX_PORT_STR_SIZE         (6)
#define OC_ENDPOINT_TPS_TOKEN        "://"
#define OC_ENDPOINT_ADDR_TOKEN       ':'
#define OC_ENDPOINT_BRACKET_START     '['
#define OC_ENDPOINT_BRACKET_END       ']'
#define OC_ENDPOINT_IPV4_NULL_TOKEN '\0'
#define OC_ENDPOINT_IPV4_SEGMENT_TOKEN '.'
#define OC_ENDPOINT_IPV4_MIN_VALUE '0'
#define OC_ENDPOINT_IPV4_MAX_VALUE '9'

#define DEFAULT_ENDPOINT_PRI         (1)

#define COAP_STR                     "coap"
#define COAPS_STR                    "coaps"
#ifdef TCP_ADAPTER
#define COAP_TCP_STR                 "coap+tcp"
#define COAPS_TCP_STR                "coaps+tcp"
#endif
#ifdef HTTP_ADAPTER
#define HTTP_STR                     "http"
#define HTTPS_STR                    "https"
#endif
#ifdef EDR_ADAPTER
#define COAP_RFCOMM_STR              "coap+rfcomm"
#endif

/*
 * This function gets Supported TPS Flags in Device from givenFlag.
 *
 * @param[in]  givenFlags Given Flags of Transport Protocol Suites.
 * @param[out] out        Result Flags of Transport Protocol Suites.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
 OCStackResult OCGetSupportedEndpointFlags(const OCTpsSchemeFlags givenFlags,
                                           OCTpsSchemeFlags* out);

/*
 * This function gets Matched TPS Flags from given flags.
 *
 * @param[in]  adapter Given Adapter of CATransportAdapter_t.
 * @param[in]  flags   Given Flags of CATransportFlags_t.
 * @param[out] out     Result Flags of Transport Protocol Suites.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
 OCStackResult OCGetMatchedTpsFlags(const CATransportAdapter_t adapter,
                                    const CATransportFlags_t flags, OCTpsSchemeFlags* out);
/*
 * This function returns pointer of endpoint string.
 *
 * @param[in]  endpoint Pointer of Endpoint Payload.
 *
 * @return     char pointer on success, NULL on failure.
 */
 char* OCCreateEndpointString(const OCEndpointPayload* endpoint);

/*
 * This function returns pointer of endpoint string.
 *
 * @param[in]  endpoint Pointer of Endpoint.
 *
 * @return     char pointer on success, NULL on failure.
 */
 char* OC_CALL OCCreateEndpointStringFromCA(const CAEndpoint_t* endpoint);

/*
 * This function parse endpoint string from payload.
 *
 * @param[in]  endpointStr     Pointer of Endpoint string.
 * @param[out] out             Pointer of OCEndpointPayload.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
 OCStackResult OCParseEndpointString(const char* endpointStr, OCEndpointPayload* out);

/*
 * This function convert tps flag to endpoint string.
 *
 * @param[in]  tps             Given tps flag.
 * @param[out] out             Pointer of char pointer for create string.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
 OCStackResult OCConvertTpsToString(const OCTpsSchemeFlags tps, char** out);

 /*
  * This function returns flags of supported TPS on stack.
  *
  * @return Bit combinations of supported OCTpsSchemeFlags.
  */
 OCTpsSchemeFlags OCGetSupportedTpsFlags();

 #endif //OC_ENDPOINT_H