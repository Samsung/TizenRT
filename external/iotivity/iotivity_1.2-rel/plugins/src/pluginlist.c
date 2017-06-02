//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "pluginlist.h"

#include <stdlib.h>
#include <string.h>

#include "zigbee_wrapper.h"
#include <coap/utlist.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocstack.h"
#include "logger.h"

#define TAG "pluginlist"

static PIPluginBase * pluginList = NULL;

OCStackResult AddPlugin (PIPluginBase * plugin)
{
    if (!plugin)
    {
        return OC_STACK_INVALID_PARAM;
    }

    LL_APPEND(pluginList, plugin);
    return OC_STACK_OK;
}

OCStackResult DeletePlugin(PIPluginBase * plugin)
{
    OCStackResult result = OC_STACK_ERROR;
    if (!plugin)
    {
        return OC_STACK_INVALID_PARAM;
    }
    DeleteResourceList(plugin);
    LL_DELETE(pluginList, plugin);
    if (plugin->type == PLUGIN_ZIGBEE)
    {
        result = ZigbeeStop((PIPlugin_Zigbee *) plugin);
    }
    return result;
}


OCStackResult DeletePluginList()
{
    OCStackResult result = OC_STACK_OK;
    PIPluginBase * out = NULL;
    PIPluginBase * tmp = NULL;
    LL_FOREACH_SAFE(pluginList, out, tmp)
    {
        result = DeletePlugin(out);
        if (result != OC_STACK_OK)
        {
            break;
        }
    }
    if (result == OC_STACK_OK)
    {
        pluginList = NULL;
    }
    return result;
}

OCStackResult GetResourceFromHandle(PIPluginBase * plugin, PIResource ** piResource,
                                    OCResourceHandle * resourceHandle)
{
    if (!plugin || !resourceHandle || !piResource)
    {
        return OC_STACK_INVALID_PARAM;
    }
    PIResourceBase * out = NULL;
    PIResourceBase * tmp = NULL;
    LL_FOREACH_SAFE(plugin->resourceList, out, tmp)
    {
        if (out->piResource.resourceHandle == resourceHandle)
        {
            *piResource = (PIResource *) out;
            return OC_STACK_OK;
        }
    }
    return OC_STACK_NO_RESOURCE;
}

static bool ZigbeeStrEquals(const char * s1, const char * s2, size_t s1_length, size_t s2_length)
{
    if (!s1 || !s2 || s1_length == 0 || s2_length == 0)
    {
        return false;
    }
    if (s1_length == s2_length && memcmp(s1, s2, (s1_length + 1)) == 0)
    {
        return true;
    }
    return false;
}

OCStackResult GetResourceFromZigBeeNodeId(PIPluginBase * plugin,
                                          PIResource_Zigbee ** piResource,
                                          const char * nodeId,
                                          const char * endpointId,
                                          const char * clusterId)
{
    OIC_LOG(INFO, TAG, "Entered GetResourceFromZigBeeNodeId().");
    if(!plugin || !piResource || !nodeId || !clusterId || !endpointId)
    {
        OIC_LOG(ERROR, TAG, "Invalid param.");
        return OC_STACK_INVALID_PARAM;
    }
    if(plugin->type != PLUGIN_ZIGBEE)
    {
        OIC_LOG(ERROR, TAG, "Plugin Type is not Zigbee.");
        return OC_STACK_INVALID_PARAM;
    }

    PIResourceBase * out = NULL;
    PIResourceBase * tmp = NULL;
    size_t checkNodeIdLength = strlen(nodeId);
    size_t checkEndpointIdLength = strlen(endpointId);
    size_t checkClusterIdLength = strlen(clusterId);
    size_t indexLength = 0;
    LL_FOREACH_SAFE(plugin->resourceList, out, tmp)
    {
        indexLength = strlen(((PIResource_Zigbee *)out)->nodeId);
        if(ZigbeeStrEquals(nodeId,
                           ((PIResource_Zigbee *)out)->nodeId,
                           checkNodeIdLength,
                           indexLength) == false)
        {
            continue;
        }
        indexLength = strlen(((PIResource_Zigbee *)out)->endpointId);
        if(ZigbeeStrEquals(endpointId,
                           ((PIResource_Zigbee *)out)->endpointId,
                           checkEndpointIdLength,
                           indexLength) == false)
        {
            continue;
        }
        indexLength = strlen(((PIResource_Zigbee *)out)->clusterId);
        if(ZigbeeStrEquals(clusterId,
                           ((PIResource_Zigbee *)out)->clusterId,
                           checkClusterIdLength,
                           indexLength) == false)
        {
            continue;
        }
        OIC_LOG_V(INFO, TAG, "Found a match! URI = %s", out->piResource.uri);
        *piResource = (PIResource_Zigbee *) out;
        return OC_STACK_OK;
    }
    *piResource = NULL;
    return OC_STACK_NO_RESOURCE;
}

OCStackResult AddResourceToPlugin (PIPluginBase * plugin, PIResourceBase * resource)
{
    if (!plugin || !resource)
    {
        return OC_STACK_INVALID_PARAM;
    }

    LL_APPEND(plugin->resourceList, resource);

    return OC_STACK_NO_MEMORY;
}

OCStackResult UpdateZigbeeResourceNodeId(PIPluginBase * plugin,
                                         const char * eui,
                                         const char * nodeId)
{
    if (!plugin || !eui || !nodeId)
    {
        return OC_STACK_INVALID_PARAM;
    }
    if(plugin->type != PLUGIN_ZIGBEE)
    {
        return OC_STACK_INVALID_PARAM;
    }
    PIResourceBase * out = NULL;
    PIResourceBase * tmp = NULL;
    size_t checkLength = strlen(eui);
    LL_FOREACH_SAFE(plugin->resourceList, out, tmp)
    {
        size_t indexLength = strlen(((PIResource_Zigbee *)out)->eui);
        if(ZigbeeStrEquals(eui, ((PIResource_Zigbee *)out)->eui, checkLength, indexLength) != true)
        {
            continue;
        }
        OICStrcpy(((PIResource_Zigbee *)out)->nodeId, (strlen(nodeId)+1)*sizeof(char), nodeId);
    }
    return OC_STACK_OK;
}

OCStackResult DeleteResource(PIPluginBase * plugin, PIResourceBase * resource)
{
    if (!plugin || !resource)
    {
        return OC_STACK_INVALID_PARAM;
    }

    //Todo: Free all of resource allocations.
    PIResourceBase * resourceList = ((PIResourceBase *) plugin->resourceList);

    LL_DELETE(resourceList, resource);

    OCStackResult result = OCDeleteResource(resource->piResource.resourceHandle);
    if(result != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to delete resource with error: %d", result);
        return result;
    }

    OICFree (resource->piResource.uri);
    if (plugin->type == PLUGIN_ZIGBEE)
    {
        OICFree (((PIResource_Zigbee *)resource)->eui);
        OICFree (((PIResource_Zigbee *)resource)->nodeId);
        OICFree (((PIResource_Zigbee *)resource)->endpointId);
        OICFree (((PIResource_Zigbee *)resource)->clusterId);
    }
    OICFree (resource);
    return OC_STACK_OK;
}

OCStackResult DeleteResourceList(PIPluginBase * plugin)
{
    if (!plugin)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult result = OC_STACK_OK;
    PIResourceBase * out = NULL;
    PIResourceBase * tmp = NULL;

    LL_FOREACH_SAFE(plugin->resourceList, out, tmp)
    {
        result = DeleteResource(plugin, out);
        if (result != OC_STACK_OK)
        {
            break;
        }
    }
    if (result == OC_STACK_OK)
    {
        plugin->resourceList = NULL;
    }
    return result;
}
