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
 * This file contains CoAP to HTTP mapping
 */

#ifndef COAP_HTTP_MAP_H_
#define COAP_HTTP_MAP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "CoapHttpParser.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif

/**
 * Function to get CoAP code for an HTTP code.
 * @param[in]   httpCode            HTTP Code.
 * @param[in]   method              Request Method type
 * @param[out]  ocfCode             Corresponding OCF code.
 * @return ::OC_STACK_OK or appropriate error code.
 */
OCStackResult CHPGetOCCode(const HttpResponseResult_t httpCode, const OCMethod method,
                           OCEntityHandlerResult *ocfCode);

/**
 * Function to get CoAP option for an HTTP option.
 * @param[in]   httpOption        HTTP Option.
 * @param[out]  ret               Corresponding CoAP option.
 * @return ::OC_STACK_OK or appropriate error code.
 */
OCStackResult CHPGetOCOption(const HttpHeaderOption_t *httpOption, OCHeaderOption *ret);

/**
 * Function to get CoAP payload format for HTTP payload format.
 * @param[in]   httpContentType   HTTP payload format.
 * @return CoAP payload format.
 */
OCPayloadFormat CHPGetOCContentType(const char *httpContentType);

/**
 * Function to get HTTP method for OCF method.
 * @param[in]   method            OCF method.
 * @param[out]  httpMethod        Corresponding HTTP method.
 * @return ::OC_STACK_OK or appropriate error code.
 */
OCStackResult CHPGetHttpMethod(const OCMethod method, HttpMethod_t *httpMethod);

/**
 * Function to get HTTP option for OCF option.
 * @param[in]   option            OCF option.
 * @param[out]  ret               Corresponding HTTP option.
 * @return ::OC_STACK_OK or appropriate error code.
 */
OCStackResult CHPGetHttpOption(const OCHeaderOption* option, HttpHeaderOption_t **ret);

/**
 * Function to convert Json payload to CBOR representational payload.
 * @param[in]   rootJSon          Json payload.
 * @param[out]  payload           CBor representational payload.
 */
void CHPJsonToRepPayload(cJSON* rootJSon, OCRepPayload* payload);

/**
 * Function to convert CBOR representational payload to Json payload.
 * @param[in]   repData           Cbor representational payload.
 * @return CJson payload.
 */
cJSON* CHPRepPayloadToJson(OCRepPayload* repData);
#ifdef __cplusplus
}
#endif
#endif
