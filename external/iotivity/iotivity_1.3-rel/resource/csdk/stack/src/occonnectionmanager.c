/******************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include "iotivity_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <getopt.h>
#include "octypes.h"
#include "ocstack.h"
#include "ocstackinternal.h"
#include "ocrandom.h"
#include "logger.h"
#include "occonnectionmanager.h"
#include "ocpayload.h"
#include "oic_string.h"
#include "oic_malloc.h"

#include "cacommon.h"
#include "cainterface.h"
#include "cautilinterface.h"

#define DEFAULT_CONTEXT_VALUE (0x99)
#define VERIFY_NON_NULL(arg, logLevel, retVal) { if (!(arg)) { OIC_LOG((logLevel), \
             TAG, #arg " is NULL"); return (retVal); } }
#define VERIFY_NON_NULL_NR(arg, logLevel) { if (!(arg)) { OIC_LOG((logLevel), \
             TAG, #arg " is NULL"); return; } }
#define CLOUD_PREFIX "/oic/route\0"
#define CLOUD_PREFIX_LENGTH 10

#define TAG "OIC_RI_CM"

/**
 * Flag for first response message.
 * Afer first discovery response, D2D endpoint info need to be updated.
 */
static bool isFirstResponse = true;

/**
 * Adapter state change callback method.
 *
 * @param[in] adapter   CA network adapter type.
 * @param[in] enabled   current adapter state.
 */
static void OCAdapterStateChangedHandler(CATransportAdapter_t adapter, bool enabled);

/**
 *  Connection State change callback method.
 *
 *  @param[in] info         CAEndpoint which has address, port and etc.
 *  @param[in] isConnected  current connection state.
 */
static void OCConnectionStateChangedHandler(const CAEndpoint_t *info, bool isConnected);

/**
 * Check uri has device id.
 *
 * @param[in]  uri  uri of resource.
 * @return  true if uri has device id, or false.
 */
static bool OCCMHasDeviceId(const char *uri);

/**
 * This function is used to find resources when connection status is changed
 * or first discovery response.
 *
 * @return ::OC_STACK_OK or ERROR CODES (::OCStackResult_t error codes in octypes.h).
 */
static OCStackResult OCCMFindResource();

/**
 * Callback function for OCCMFindResource.
 *
 * @return ::OC_STACK_KEEP_TRANSACTION
 */
static OCStackApplicationResult OCCMFoundResource(void *ctx,
                                OCDoHandle handle, OCClientResponse *clientResponse);

OCStackResult OCCMInitialize()
{
    OIC_LOG_V(INFO, TAG, "%s", __func__);

    //Initialize the Connection Manager
    CAResult_t ret = CAUtilCMInitailize();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAInitializeConnectionManager failed!");
        return CAResultToOCResult(ret);
    }

    // Register NetworkMonitor
    ret = CARegisterNetworkMonitorHandler(OCAdapterStateChangedHandler,
                                          OCConnectionStateChangedHandler);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CARegisterNetworkMonitorHandler failed!");
        return CAResultToOCResult(ret);
    }

    return CAResultToOCResult(ret);
}

OCStackResult OCCMTerminate()
{
    OIC_LOG_V(INFO, TAG, "%s", __func__);

    //Unregister IP NetworkMonitor
    CAResult_t ret = CAUnregisterNetworkMonitorHandler(OCAdapterStateChangedHandler,
                                                       OCConnectionStateChangedHandler);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAUnregisterNetworkMonitorHandler has failed");
    }

    //Terminate the Connection Manager
    ret = CAUtilCMTerminate();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAUtilCMTerminate has failed");
    }

    return CAResultToOCResult(ret);
}

OCStackResult OCCMDiscoveryResource(OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "%s", __func__);

    VERIFY_NON_NULL(clientResponse, ERROR, OC_STACK_INVALID_PARAM);

    OCDiscoveryPayload *payload = (OCDiscoveryPayload*) clientResponse->payload;
    if (!payload)
    {
        OIC_LOG(ERROR, TAG, "payload is empty!");
        return OC_STACK_ERROR;
    }

    if (NULL == payload->sid)
    {
        OIC_LOG(ERROR, TAG, "Device ID is null");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(INFO, TAG, "Device ID : %s", payload->sid);

    OCResourcePayload *resource = (OCResourcePayload*) payload->resources;

    if (NULL == resource)
    {
        OIC_LOG(ERROR, TAG, "payload resource is NULL");
        return OC_STACK_ERROR;
    }

    if (NULL == resource->uri)
    {
        OIC_LOG(ERROR, TAG, "resource->uri is NULL");
        return OC_STACK_ERROR;
    }

    // update di in CM managed table
    bool isCloud = OCCMHasDeviceId(resource->uri);
    CAEndpoint_t endpoint = {.adapter = CA_DEFAULT_ADAPTER};
    CopyDevAddrToEndpoint(&(clientResponse->devAddr), &endpoint);

    CAResult_t ret = CAUtilCMUpdateRemoteDeviceInfo(&endpoint, isCloud);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAUtilCMUpdateRemoteDeviceInfo is failed.");
        return CAResultToOCResult(ret);
    }

#ifdef TCP_ADAPTER
    if (!isCloud && resource->tcpPort != 0)
    {
        endpoint.adapter = CA_ADAPTER_TCP;
        endpoint.port = resource->tcpPort;

        ret = CAUtilCMUpdateRemoteDeviceInfo(&endpoint, isCloud);
        if (CA_STATUS_OK != ret)
        {
            OIC_LOG(ERROR, TAG, "CAUtilCMUpdateRemoteDeviceInfo is failed!");
            return CAResultToOCResult(ret);
        }
    }
#endif

    //CM tries to find local resource when ocstack found a cloud resource.
    if (isFirstResponse)
    {
        OCStackResult ret = OCCMFindResource();
        if (OC_STACK_OK != ret)
        {
            OIC_LOG(ERROR, TAG, "OCCMFindResource is failed!");
            return ret;
        }
        isFirstResponse = false;
    }

    return OC_STACK_OK;
}

static void OCAdapterStateChangedHandler(CATransportAdapter_t adapter, bool enabled)
{
    OC_UNUSED(adapter);
    // check user configuration
    CAConnectUserPref_t connPrefer = CA_USER_PREF_CLOUD;
    CAResult_t ret = CAUtilCMGetConnectionUserConfig(&connPrefer);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "CAUtilCMGetConnectionUserConfig failed with error %u", ret);
    }

    if (CA_USER_PREF_CLOUD != connPrefer)
    {
        //set connection callback
        if (true == enabled)
        {
            OIC_LOG(DEBUG, TAG, "CM ConnectionStatusChangedHandler ENABLED");
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "CM ConnectionStatusChangedHandler DISABLED");
        }
    }
}

static void OCConnectionStateChangedHandler(const CAEndpoint_t *info, bool isConnected)
{
    OIC_LOG(DEBUG, TAG, "OCConnectionStateChangedHandler");
    OC_UNUSED(info);

    CAConnectUserPref_t connPrefer = CA_USER_PREF_CLOUD;
    CAResult_t ret = CAUtilCMGetConnectionUserConfig(&connPrefer);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "CAUtilCMGetConnectionUserConfig failed with error %u", ret);
    }
    else
    {
        if (CA_USER_PREF_CLOUD != connPrefer)
        {
            if (!isConnected)
            {
                OIC_LOG(DEBUG, TAG, "is disconnedted");

                //clean local ip up in CM managed table
                CAUtilCMResetRemoteDeviceInfo();
            }
            else
            {
                OIC_LOG(DEBUG, TAG, "is connected");

                // Find Resource
                OCCMFindResource();
            }
        }
    }
}

static OCStackResult OCCMFindResource()
{
    OIC_LOG_V(INFO, TAG, "%s", __func__);

    char szQueryUri[MAX_QUERY_LENGTH] = { 0 };
    snprintf(szQueryUri, sizeof(szQueryUri) - 1, "%s%c", OC_RSRVD_WELL_KNOWN_URI, '\0');

    OCCallbackData cbData = {0,};
    cbData.cb = OCCMFoundResource;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OCStackResult ret = OCDoResource(NULL, OC_REST_DISCOVER, szQueryUri,
                                     NULL, 0, CT_DEFAULT | CT_IP_USE_V4,
                                     OC_LOW_QOS, &cbData, NULL, 0);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
        return ret;
    }

    return OC_STACK_OK;
}

static OCStackApplicationResult OCCMFoundResource(void* ctx,
                                           OCDoHandle handle,
                                           OCClientResponse* clientResponse)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);
    OC_UNUSED(clientResponse);
    return OC_STACK_KEEP_TRANSACTION;
}

static bool OCCMHasDeviceId(const char* uri)
{
    bool ret = false;
    char *copyUri = OICStrdup(uri);
    bool hasCloudPrefix = strncmp(CLOUD_PREFIX, copyUri, CLOUD_PREFIX_LENGTH) == 0 ? true : false;

    int tokSize = 0;
    char *str = copyUri;

    char *savePtr = NULL;
    char *tok = strtok_r(str, "/", &savePtr);
    while (hasCloudPrefix && tok != NULL && ret != true)
    {
        tokSize = strlen(tok);
        if (tokSize == MAX_IDENTITY_SIZE - 1)
        {
            if (OCIsUUID(tok))
            {
                OIC_LOG_V(DEBUG, TAG, "[%s] has uuid", uri);
                ret = true;
            }
        }
        tok = strtok_r(NULL, "/", &savePtr);
    }
    OICFree(copyUri);
    copyUri = NULL;

    return ret;
}
