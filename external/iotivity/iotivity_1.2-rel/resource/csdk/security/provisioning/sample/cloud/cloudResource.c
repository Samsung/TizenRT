/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/
#include <string.h>
#include "cloudCommon.h"
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

#include "cloudResource.h"

#define TAG "cloudResource"

LEDResource LED;
char *gResourceUri = (char *)"/a/led";

// This variable determines instance number of the LED resource.
// Used by POST method to create a new instance of LED resource.
static int gCurrLedInstance = 0;
#define SAMPLE_MAX_NUM_POST_INSTANCE  2
static LEDResource gLedInstance[SAMPLE_MAX_NUM_POST_INSTANCE];

static OCRepPayload* getPayload(const char* uri, int64_t power, bool state)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if(!payload)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate Payload");
        return NULL;
    }

    OCRepPayloadSetUri(payload, uri);
    OCRepPayloadSetPropBool(payload, "state", state);
    OCRepPayloadSetPropInt(payload, "power", power);

    return payload;
}

static OCEntityHandlerResult ProcessRequest (OCEntityHandlerRequest *request,
        OCRepPayload **payload)
{
    OIC_LOG_V(DEBUG, TAG, "%s: IN",  __func__);

    if(!request)
    {
        OIC_LOG_V(ERROR, TAG, "%s: OUT: request is NULL",__func__);
        return OC_EH_ERROR;
    }

    OIC_LOG_V(DEBUG, TAG, "%s: method: %d",  __func__, request->method);

    LEDResource *currLEDResource = &LED;

    if (request->resource == gLedInstance[0].handle)
    {
        currLEDResource = &gLedInstance[0];
        gResourceUri = (char *) "/a/led/0";
    }
    else if (request->resource == gLedInstance[1].handle)
    {
        currLEDResource = &gLedInstance[1];
        gResourceUri = (char *) "/a/led/1";
    }

    if(OC_REST_PUT == request->method)
    {
        if (!request->payload || request->payload->type != PAYLOAD_TYPE_REPRESENTATION)
        {
            OIC_LOG_V(ERROR, TAG, "%s: OUT: Incoming payload type(%d) is not a representation",
                      __func__, request->payload ? request->payload->type : 0);
            return OC_EH_ERROR;
        }
        OCRepPayload* input = (OCRepPayload*)request->payload;
        // Get pointer to query
        int64_t pow;
        if(OCRepPayloadGetPropInt(input, "power", &pow))
        {
            currLEDResource->power =pow;
        }

        bool state;
        if(OCRepPayloadGetPropBool(input, "state", &state))
        {
            currLEDResource->state = state;
        }
    }

    *payload = getPayload(gResourceUri, currLEDResource->power, currLEDResource->state);

    OIC_LOG_V(DEBUG, TAG, "%s: OUT",  __func__);

    return (*payload) ? OC_EH_OK : OC_EH_ERROR;
}

static OCEntityHandlerResult ProcessPostRequest (OCEntityHandlerRequest *request,
        char *resourceUri, OCRepPayload **payload)
{
    OCRepPayload *answer = NULL;
    OCEntityHandlerResult result = OC_EH_ERROR;

    /*
     * The entity handler determines how to process a POST request.
     * Per the REST paradigm, POST can also be used to update representation of existing
     * resource or create a new resource.
     * In the sample below, if the POST is for /a/led then a new instance of the LED
     * resource is created with default representation (if representation is included in
     * POST payload it can be used as initial values) as long as the instance is
     * lesser than max new instance count. Once max instance count is reached, POST on
     * /a/led updated the representation of /a/led (just like PUT)
     */

    if (request->resource == LED.handle)
    {
        if (gCurrLedInstance < SAMPLE_MAX_NUM_POST_INSTANCE)
        {
            // Create new LED instance
            char uri[15] = "/a/led/";
            int length = strlen(uri);
            snprintf (uri + length, sizeof(uri) - length, "%d", gCurrLedInstance);

            answer = OCRepPayloadCreate();
            if (!answer)
            {
                OIC_LOG(ERROR, TAG, "Failed to allocate Payload");
                return OC_EH_ERROR;
            }

            OCRepPayloadSetUri(answer, gResourceUri);
            OCRepPayloadSetPropString(answer, "createduri", uri);

            gLedInstance[gCurrLedInstance].power = 0;
            gLedInstance[gCurrLedInstance].state = false;

            if (OC_STACK_OK == createLEDResource(uri, &gLedInstance[gCurrLedInstance]))
            {
                OIC_LOG (INFO, TAG, "Created new LED instance");
                gLedInstance[gCurrLedInstance].state = false;
                gLedInstance[gCurrLedInstance].power = 0;
                gCurrLedInstance++;
                strncpy(resourceUri, uri, MAX_URI_LENGTH);
                result = OC_EH_OK;
            }
        }
        else
        {
            result = ProcessRequest(request, &answer);
        }
    }
    else
    {
        for (int i = 0; i < SAMPLE_MAX_NUM_POST_INSTANCE; i++)
        {
            if (request->resource == gLedInstance[i].handle)
            {
                result = ProcessRequest(request, &answer);
                break;
            }
        }
    }

    if (OC_EH_OK == result)
    {
        *payload = answer;
    }

    return result;
}

static OCEntityHandlerResult
OCLedHandler (OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *request,
        void *params)
{
    OC_UNUSED(params);
    OIC_LOG_V (INFO, TAG, "%s IN - flags: 0x%x", __func__, flag);

    OCEntityHandlerResult result = OC_EH_ERROR;
    char resourceUri[MAX_URI_LENGTH] = "";

    if (!request)
    {
        OIC_LOG (ERROR, TAG, "Invalid request pointer");
        return result;
    }

    OCRepPayload* payload = NULL;

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG_V(INFO, TAG, "Received %d request method from client", request->method);

        switch (request->method)
        {
            case OC_REST_GET:
                result = ProcessRequest(request, &payload);
                break;
            case OC_REST_PUT:
                result = ProcessRequest(request, &payload);
                break;
            case OC_REST_POST:
                result = ProcessPostRequest(request, resourceUri, &payload);
                break;
            default:
                OIC_LOG_V (ERROR, TAG, "Method %d doesn't supported!", request->method);
                break;
        }

        if (result == OC_EH_OK)
        {
            // Format the response.  Note this requires some info about the request
            OCEntityHandlerResponse response;
            memset(&response, 0, sizeof(OCEntityHandlerResponse));

            response.requestHandle = request->requestHandle;
            response.resourceHandle = request->resource;
            response.ehResult = result;
            response.payload = (OCPayload*)payload;
            response.numSendVendorSpecificHeaderOptions = 0;
            // Indicate that response is NOT in a persistent buffer
            response.persistentBufferFlag = 0;
            if (resourceUri[0])
            {
                memcpy(response.resourceUri, resourceUri, sizeof(resourceUri));
            }

            // Send the response
            if (OCDoResponse(&response) != OC_STACK_OK)
            {
                OIC_LOG(ERROR, TAG, "Error sending response");
                result = OC_EH_ERROR;
            }
        }
    }
    OCRepPayloadDestroy(payload);
    return result;
}

OCStackResult createLEDResource (char *uri, LEDResource *resource)
{
    if (!uri || !resource)
    {
        OIC_LOG(ERROR, TAG, "Resource URI and LED cannot be NULL");
        return OC_STACK_INVALID_PARAM;
    }

    return OCCreateResource(&resource->handle,
            "core.led",
            OC_RSRVD_INTERFACE_DEFAULT,
            uri,
            OCLedHandler,
            NULL,
            OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
}
