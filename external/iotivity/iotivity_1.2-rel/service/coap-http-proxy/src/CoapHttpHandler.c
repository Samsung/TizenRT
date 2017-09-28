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

#include "CoapHttpHandler.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include <coap/pdu.h>
#include "ocpayload.h"
#include "uarraylist.h"
#include "CoapHttpParser.h"
#include "CoapHttpMap.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif

#define TAG "CHPHandler"

#define CHP_RESOURCE_TYPE_NAME "core.chp"
#define CHP_RESOURCE_INTF_NAME "oc.mi.def"

/**
 * CoAP request tracking.
 * This structure is used to hold CoAP request information
 */
typedef struct
{
    OCMethod method;
    OCRequestHandle requestHandle;
} CHPRequest_t;

/**
 * Pointer to handle of the newly created proxy resource.
 */
static OCResourceHandle g_proxyHandle = NULL;
static int g_isCHProxyInitialized = false;

/**
 * Function to hand over CoAP request handling to Proxy.
 */
OCStackResult CHPHandleOCFRequest(const OCEntityHandlerRequest* requestInfo,
                                   const char* proxyUri);

/**
 * Entity handler to receive requests from csdk.
 */
OCEntityHandlerResult CHPEntityHandler(OCEntityHandlerFlag flag,
                                       OCEntityHandlerRequest* entityHandlerRequest,
                                       void* callbackParam);
bool CHPIsInitialized()
{
    return g_isCHProxyInitialized;
}

OCStackResult CHPInitialize(bool secure)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    if (g_isCHProxyInitialized)
    {
        OIC_LOG(DEBUG, TAG, "CH Proxy already initialized");
        return OC_STACK_OK;
    }

    OCStackResult result = CHPParserInitialize();
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "Parser initialization failed[%d]", result);
        return result;
    }

    result = OCSetProxyURI(OC_RSRVD_PROXY_URI);
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "Setting proxy uri failed[%d]", result);
        CHPParserTerminate();
        return result;
    }

    uint8_t prop = OC_ACTIVE | OC_DISCOVERABLE | OC_SLOW;
    if(secure)
    {
        prop |= OC_SECURE;
    }
    result = OCCreateResource(&g_proxyHandle,
                               CHP_RESOURCE_TYPE_NAME,
                               CHP_RESOURCE_INTF_NAME,
                               OC_RSRVD_PROXY_URI,
                               CHPEntityHandler,
                               NULL,
                               prop);

    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "Create resource for proxy failed[%d]", result);
        CHPParserTerminate();
        return result;
    }

    g_isCHProxyInitialized = true;
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

OCStackResult CHPTerminate()
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    OCStackResult result = CHPParserTerminate();
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "Parser termination failed[%d]", result);
    }

    result = OCDeleteResource(g_proxyHandle);
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "Delete resource for proxy failed[%d]", result);
    }

    g_proxyHandle = NULL;
    g_isCHProxyInitialized = false;
    return result;
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
}

static void CHPGetProxyURI(OCHeaderOption* options, uint8_t *numOptions, char* uri,
                           size_t uriLength)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    if (!uri || uriLength <= 0)
    {
        OIC_LOG (INFO, TAG, "Invalid uri buffer");
        return;
    }

    if (!options || !numOptions || 0 == *numOptions)
    {
        OIC_LOG (INFO, TAG, "No options present");
        return;
    }

    for (int count = 0; count < *numOptions; count++)
    {
        if (options[count].protocolID == OC_COAP_ID &&
                options[count].optionID == COAP_OPTION_PROXY_URI)
        {
            OIC_LOG(DEBUG, TAG, "Proxy URI is present");
            // Extract proxy-uri and delete it from headeroptions.
            OICStrcpy(uri, uriLength, (char *)options[count].optionData);
            for (int fwd = count; fwd < *numOptions-1; fwd++)
            {
                options[count] = options[count+1];
            }
            *numOptions -= 1;
            return;
        }
    }

    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return;
}

// TODO: Will be moved to OCProxyPayload
static OCRepPayload* CHPGetDiscoveryPayload()
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, TAG, PCF("Failed to create Payload"));
        return NULL;
    }

    OCRepPayloadSetUri(payload, OC_RSRVD_PROXY_URI);
    OCRepPayloadSetPropString(payload, OC_RSRVD_DEVICE_ID, OCGetServerInstanceIDString());
    return payload;
}

OCEntityHandlerResult CHPEntityHandler(OCEntityHandlerFlag flag,
                                       OCEntityHandlerRequest* entityHandlerRequest,
                                       void* callbackParam)
{
    OIC_LOG(INFO, TAG, "Proxy request received");
    UNUSED(callbackParam);

    if (!g_isCHProxyInitialized)
    {
        OIC_LOG (ERROR, TAG, "Proxy not initialized");
        return OC_EH_INTERNAL_SERVER_ERROR;
    }

    if (!entityHandlerRequest)
    {
        OIC_LOG (ERROR, TAG, "Invalid request pointer");
        return OC_EH_ERROR;
    }

    if (flag & OC_OBSERVE_FLAG)
    {
        OIC_LOG_V (ERROR, TAG, "Proxy is not observable");
        return OC_EH_BAD_REQ;
    }
    else if (flag & OC_REQUEST_FLAG)
    {
        /*
         *  A proxy can handle two type of requests:
         *  1. Discovery request in which case proxy-uri option will not be present.
         *  2. Request for HTTP resource with proxy-uri option.
         */
        char proxyUri[MAX_HEADER_OPTION_DATA_LENGTH] = {'\0'};
        CHPGetProxyURI(entityHandlerRequest->rcvdVendorSpecificHeaderOptions,
                       &(entityHandlerRequest->numRcvdVendorSpecificHeaderOptions),
                       proxyUri, sizeof(proxyUri));

        if (proxyUri[0] != '\0')
        {
            // A request for HTTP resource. Response will be sent asynchronously
            if (OC_STACK_OK == CHPHandleOCFRequest(entityHandlerRequest,
                                                   proxyUri) )
            {
                return OC_EH_SLOW;
            }
        }
        else
        {
            OCEntityHandlerResult ehResult = OC_EH_ERROR;
            switch (entityHandlerRequest->method)
            {
                case OC_REST_GET:
                case OC_REST_DISCOVER:
                {
                    // Generate discovery payload
                    OIC_LOG (INFO, TAG, "Discovery request from client");
                    ehResult = OC_EH_OK;
                    OCEntityHandlerResponse response =
                                { .requestHandle = entityHandlerRequest->requestHandle,
                                  .resourceHandle = entityHandlerRequest->resource,
                                  .ehResult = ehResult};

                    response.payload = (OCPayload *)CHPGetDiscoveryPayload();
                    // Indicate that response is NOT in a persistent buffer
                    response.persistentBufferFlag = 0;

                    // Send the response
                    if (OCDoResponse(&response) != OC_STACK_OK)
                    {
                        OIC_LOG(ERROR, TAG, "Error sending response");
                        ehResult = OC_EH_ERROR;
                    }

                    OCPayloadDestroy(response.payload);
                    break;
                }
                default:
                    // Other methods are not supported
                    OIC_LOG (INFO, TAG, "Invalid method from client");
                    ehResult = OC_EH_METHOD_NOT_ALLOWED;
                    break;
            }
            return ehResult;
        }
    }

    return OC_EH_ERROR;
}

void CHPHandleHttpResponse(const HttpResponse_t *httpResponse, void *context)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    if (!httpResponse || !context)
    {
        OIC_LOG(ERROR, TAG, "Invalid arguements");
        return;
    }

    CHPRequest_t *ctxt = (CHPRequest_t *)context;
    OCEntityHandlerResponse response = { .requestHandle = ctxt->requestHandle,
                                         .resourceHandle = g_proxyHandle};
    response.persistentBufferFlag = 0;

    OCStackResult result = CHPGetOCCode(httpResponse->status, ctxt->method,
                                        &response.ehResult);
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "%s failed[%d]", __func__, result);
        response.ehResult = OC_EH_INTERNAL_SERVER_ERROR;
        if (OCDoResponse(&response) != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Error sending response");
        }
        OICFree(ctxt);
        return;
    }

    // ctxt not required now.
    OICFree(ctxt);

    if (httpResponse->dataFormat[0] != '\0')
    {
        OCPayloadFormat format = CHPGetOCContentType(httpResponse->dataFormat);
        switch (format)
        {
            case OC_FORMAT_CBOR:
                OIC_LOG(DEBUG, TAG, "Payload format is CBOR");
                result = OCParsePayload(&response.payload, PAYLOAD_TYPE_REPRESENTATION,
                                        httpResponse->payload, httpResponse->payloadLength);
                if (result != OC_STACK_OK)
                {
                    OIC_LOG(ERROR, TAG, "Error parsing payload");
                    response.ehResult = OC_EH_INTERNAL_SERVER_ERROR;
                    if (OCDoResponse(&response) != OC_STACK_OK)
                    {
                        OIC_LOG(ERROR, TAG, "Error sending response");
                    }
                    return;
                }
                break;
            case OC_FORMAT_JSON:
                OIC_LOG(DEBUG, TAG, "Payload format is JSON");
                cJSON *payloadJson = cJSON_Parse((char *)httpResponse->payload);
                if (!payloadJson)
                {
                    OIC_LOG(ERROR, TAG, "Unable to parse json response");
                    response.ehResult = OC_EH_INTERNAL_SERVER_ERROR;
                    if (OCDoResponse(&response) != OC_STACK_OK)
                    {
                        OIC_LOG(ERROR, TAG, "Error sending response");
                    }
                    return;
                }
                OCRepPayload* payloadCbor = OCRepPayloadCreate();
                if (!payloadCbor)
                {
                    response.ehResult = OC_EH_INTERNAL_SERVER_ERROR;
                    if (OCDoResponse(&response) != OC_STACK_OK)
                    {
                        OIC_LOG(ERROR, TAG, "Error sending response");
                    }
                    cJSON_Delete(payloadJson);
                    return;
                }

                CHPJsonToRepPayload(payloadJson, payloadCbor);
                response.payload = (OCPayload *)payloadCbor;
                cJSON_Delete(payloadJson);
                break;
            default:
                OIC_LOG(ERROR, TAG, "Payload format is not supported");
                response.ehResult = OC_EH_INTERNAL_SERVER_ERROR;
                if (OCDoResponse(&response) != OC_STACK_OK)
                {
                    OIC_LOG(ERROR, TAG, "Error sending response");
                }
                return;
        }
    }
    // Header Options parsing
    response.numSendVendorSpecificHeaderOptions = 0;
    OCHeaderOption *optionsPointer = response.sendVendorSpecificHeaderOptions;

    uint8_t tempOptionNumber = u_arraylist_length(httpResponse->headerOptions);
    for (int numOptions = 0; numOptions < tempOptionNumber &&
                             response.numSendVendorSpecificHeaderOptions < MAX_HEADER_OPTIONS;
                             numOptions++)
    {
        HttpHeaderOption_t *httpOption = u_arraylist_get(httpResponse->headerOptions, numOptions);
        result = CHPGetOCOption(httpOption, optionsPointer);
        if (OC_STACK_OK != result)
        {
            OIC_LOG_V(ERROR, TAG, "CHPGetCoAPOption failed[%d][%d]", result,
                                                response.numSendVendorSpecificHeaderOptions);
            continue;
        }

        response.numSendVendorSpecificHeaderOptions++;
        optionsPointer += 1;
    }

    if (OCDoResponse(&response) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Error sending response");
    }

    //OICFree(coapResponseInfo.info.payload);
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
}

OCStackResult CHPHandleOCFRequest(const OCEntityHandlerRequest* requestInfo,
                                   const char* proxyUri)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);

    HttpRequest_t httpRequest = { .httpMajor = 1,
                                  .httpMinor = 1};

    OCEntityHandlerResponse response = { .requestHandle = requestInfo->requestHandle,
                                         .resourceHandle = requestInfo->resource};
    OCStackResult result = CHPGetHttpMethod(requestInfo->method, &httpRequest.method);
    if (OC_STACK_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Method not found in HTTP");
        response.ehResult = OC_EH_BAD_REQ;
        if (OCDoResponse(&response) != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Error sending response");
        }

        return OC_STACK_ERROR;
    }

    uint8_t vendorOptions = requestInfo->numRcvdVendorSpecificHeaderOptions;
    if (vendorOptions)
    {
        httpRequest.headerOptions = u_arraylist_create();
        for (int option = 0; option < vendorOptions; option++)
        {
            HttpHeaderOption_t *httpOption = NULL;
            result = CHPGetHttpOption(requestInfo->rcvdVendorSpecificHeaderOptions + option,
                                      &httpOption);
            if (OC_STACK_OK != result || NULL == httpOption )
            {
                OIC_LOG_V(ERROR, TAG, "CHPGetHttpOption failed [%d]", result);
                continue;
            }
            u_arraylist_add(httpRequest.headerOptions, (void *)httpOption);
        }
    }

    OICStrcpy(httpRequest.resourceUri, sizeof(httpRequest.resourceUri), proxyUri);

    if (requestInfo->payload && requestInfo->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        // Conversion from cbor to json.
        cJSON *payloadJson = CHPRepPayloadToJson((OCRepPayload *)requestInfo->payload);
        if (!payloadJson)
        {
            response.ehResult = OC_EH_BAD_REQ;
            if (OCDoResponse(&response) != OC_STACK_OK)
            {
                OIC_LOG(ERROR, TAG, "Error sending response");
            }

            return OC_STACK_ERROR;

        }
        httpRequest.payload = (void *)cJSON_Print(payloadJson);
        httpRequest.payloadLength = strlen(httpRequest.payload);
        OICStrcpy(httpRequest.payloadFormat, sizeof(httpRequest.payloadFormat),
                  CBOR_CONTENT_TYPE);
        cJSON_Delete(payloadJson);
    }

    OICStrcpy(httpRequest.acceptFormat, sizeof(httpRequest.acceptFormat),
              ACCEPT_MEDIA_TYPE);
    CHPRequest_t *chpRequest = (CHPRequest_t *)OICCalloc(1, sizeof(CHPRequest_t));
    if (!chpRequest)
    {
        OIC_LOG(ERROR, TAG, "Calloc failed");
        response.ehResult = OC_EH_INTERNAL_SERVER_ERROR;
        if (OCDoResponse(&response) != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Error sending response");
        }

        OICFree(httpRequest.payload);
        u_arraylist_destroy(httpRequest.headerOptions);
        return OC_STACK_NO_MEMORY;
    }

    chpRequest->requestHandle = requestInfo->requestHandle;
    chpRequest->method = requestInfo->method;

    result = CHPPostHttpRequest(&httpRequest, CHPHandleHttpResponse,
                                (void *)chpRequest);
    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "CHPPostHttpRequest failed[%d]", result);
        switch (result)
        {
            case OC_STACK_INVALID_URI:
                response.ehResult = OC_EH_BAD_REQ;
                break;
            default:
                response.ehResult = OC_EH_INTERNAL_SERVER_ERROR;
        }

        if (OCDoResponse(&response) != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Error sending response");
        }

        OICFree(httpRequest.payload);
        OICFree(chpRequest);
        u_arraylist_destroy(httpRequest.headerOptions);
        return OC_STACK_ERROR;
    }

    if (!httpRequest.payloadCached)
    {
        // Free only if parser has not cached it.
        OICFree(httpRequest.payload);
    }
    u_arraylist_destroy(httpRequest.headerOptions);
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}
