/* *****************************************************************
*
* Copyright 2016 Intel Corporation
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

#include "caipinterface.h"

#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include "platform_features.h"
#include <iptypes.h>
#include <stdbool.h>
#include "caadapterutils.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "caipnwmonitor.h"

#define TAG "IP_MONITOR"

/**
 * @todo Implement network interface monitoring in case the IP changes.
 * Not critical for win32 bring-up.
 */
CAResult_t CAIPStartNetworkMonitor(CAIPAdapterStateChangeCallback callback,
                                   CATransportAdapter_t adapter)
{
    return CA_STATUS_OK;
}

/**
 * @todo Implement network interface monitoring in case the IP changes.
 * Not critical for win32 bring-up.
 */
CAResult_t CAIPStopNetworkMonitor(CATransportAdapter_t adapter)

{
    return CA_STATUS_OK;
}

/**
 * @todo Implement network interface monitoring.
 * Not used in win32, but caipserver currently requires this function
 * be defined. not critical.
 */
int CAGetPollingInterval(int interval)
{
    return interval;
}

/**
 * @todo Implement network interface monitoring.
 * Not used in win32, but caipserver currently requires this function
 * be defined. not critical.
 */
u_arraylist_t *CAFindInterfaceChange()
{
    u_arraylist_t *iflist = NULL;
    return iflist;
}

/**
 * @todo Implement network interface monitoring.
 * Not critical for win32 bring-up.
 */
CAResult_t CAIPSetNetworkMonitorCallback(CAIPAdapterStateChangeCallback callback,
                                         CATransportAdapter_t adapter)
{
    return CA_NOT_SUPPORTED;
}

bool IsValidAdapter(PIP_ADAPTER_ADDRESSES pAdapterAddr, int desiredIndex, uint16_t family)
{
    bool valid = true;

    // If desiredIndex is non-zero, then only retrieve adapter corresponding to desiredIndex.
    // If desiredIndex is zero, then retrieve all adapters.
    if (desiredIndex && (pAdapterAddr->IfIndex != desiredIndex))
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i not interesting.", pAdapterAddr->IfIndex);
        valid = false;
    }

    if (pAdapterAddr->IfType & IF_TYPE_SOFTWARE_LOOPBACK)
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i is loopback.", pAdapterAddr->IfIndex);
        valid = false;
    }

    // If the adapter must support the requested family
    if ((family == AF_INET6) && ((pAdapterAddr->Flags & IP_ADAPTER_IPV6_ENABLED) == 0))
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i does not support IPv6", pAdapterAddr->IfIndex);
        valid = false;
    }
    else if ((family == AF_INET) && ((pAdapterAddr->Flags & IP_ADAPTER_IPV4_ENABLED) == 0))
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i does not support IPv4", pAdapterAddr->IfIndex);
        valid = false;
    }

    if ((pAdapterAddr->OperStatus & IfOperStatusUp) == 0)
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i is not operational.", pAdapterAddr->IfIndex);
        valid = false;
    }
    return valid;

}


bool AddCAInterface(u_arraylist_t *iflist, const char * name, uint32_t index, uint16_t family)
{
    bool bSucceeded = false;
    CAInterface_t *ifitem = (CAInterface_t *)OICCalloc(1, sizeof(*ifitem));
    if (ifitem)
    {
        OICStrcpy(ifitem->name, INTERFACE_NAME_MAX, name);
        ifitem->index = index;
        ifitem->family = family;
        ifitem->flags |= IFF_UP;// IsValidAddress() will have filtered out non-operational addresses already.

        if (u_arraylist_add(iflist, ifitem))
        {
            bSucceeded = true;
        }
        else
        {
            OIC_LOG(ERROR, TAG, "u_arraylist_add failed");
            OICFree(ifitem);
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Allocating memory for a CAInterface_t failed");
    }
    return bSucceeded;
}

bool AddInterfaces(PIP_ADAPTER_ADDRESSES pAdapterAddr, u_arraylist_t *iflist, int desiredIndex)
{
    bool bSucceeded = false;
    for (PIP_ADAPTER_ADDRESSES pCurAdapterAddr = pAdapterAddr;
         pCurAdapterAddr != NULL; pCurAdapterAddr = pCurAdapterAddr->Next)
    {
        OIC_LOG_V(DEBUG, TAG, "\tInterface Index: %u", pCurAdapterAddr->IfIndex);
        OIC_LOG_V(DEBUG, TAG, "\tInterface  name: %s", pCurAdapterAddr->AdapterName);

        // Prefer IPv6 over IPv4.
        if (pCurAdapterAddr->Flags & IP_ADAPTER_IPV6_ENABLED)
        {
            // Do not add loopback, duplicate, or non-operational adapters
            if (IsValidAdapter(pCurAdapterAddr, desiredIndex, AF_INET6))
            {
                if (AddCAInterface(iflist, pCurAdapterAddr->AdapterName, pCurAdapterAddr->IfIndex, AF_INET6))
                {
                    OIC_LOG_V(DEBUG, TAG, "\t\tAdded IPv6 interface %i", pCurAdapterAddr->IfIndex);
                    bSucceeded = true;
                }
                else
                {
                    OIC_LOG_V(ERROR, TAG, "\tAdding IPv6 interface %i failed", pCurAdapterAddr->IfIndex);
                    break;
                }
            }
            else
            {
                OIC_LOG_V(DEBUG, TAG, "\t\tIPv6 interface %i not valid, skipping...", pCurAdapterAddr->IfIndex);
            }
        }

        if (pCurAdapterAddr->Flags & IP_ADAPTER_IPV4_ENABLED)
        {
            // Do not add loopback, duplicate, or non-operational adapters
            if (IsValidAdapter(pCurAdapterAddr, desiredIndex, AF_INET))
            {
                if (AddCAInterface(iflist, pCurAdapterAddr->AdapterName, pCurAdapterAddr->IfIndex, AF_INET))
                {
                    OIC_LOG_V(DEBUG, TAG, "\t\tAdded IPv4 interface %i", pCurAdapterAddr->IfIndex);
                    bSucceeded = true;
                }
                else
                {
                    OIC_LOG_V(ERROR, TAG, "\tAdding IPv4 interface %i failed", pCurAdapterAddr->IfIndex);
                    break;
                }
            }
            else
            {
                OIC_LOG_V(DEBUG, TAG, "\t\tIPv6 interface %i not valid, skipping...", pCurAdapterAddr->IfIndex);
            }

        }
    }
    return bSucceeded;
}

PIP_ADAPTER_ADDRESSES GetAdapters()
{
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_ADDRESSES);
    PIP_ADAPTER_ADDRESSES pAdapterAddr = (IP_ADAPTER_ADDRESSES *) OICMalloc(ulOutBufLen);
    if (pAdapterAddr != NULL)
    {
        ULONG flags = 0;
        ULONG ret = GetAdaptersAddresses(AF_UNSPEC, flags, NULL, pAdapterAddr, &ulOutBufLen);
        if (ERROR_BUFFER_OVERFLOW == ret)
        {
            // Redo with updated length
            OICFree(pAdapterAddr);
            pAdapterAddr = (PIP_ADAPTER_ADDRESSES) OICMalloc(ulOutBufLen);
            if (pAdapterAddr != NULL) {
                ret = GetAdaptersAddresses(AF_UNSPEC, flags, NULL, pAdapterAddr, &ulOutBufLen);
                if (NO_ERROR != ret)
                {
                    OIC_LOG(ERROR, TAG, "GetAdaptersAddresses() failed");
                    OICFree(pAdapterAddr);
                    pAdapterAddr = NULL;
                }
                else
                {
                    // Succeeded getting adapters
                }
            }
            else
            {
                OIC_LOG(ERROR, TAG, "Second time allocating memory for GetAdaptersAddresses() failed");
            }
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Expected GetAdaptersAddresses() to fail on first try, but it didn't.");
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "First time allocating memory for GetAdaptersAddresses() failed");
    }
    return pAdapterAddr;
}

u_arraylist_t *CAIPGetInterfaceInformation(int desiredIndex)
{
    u_arraylist_t *iflist = u_arraylist_create();
    if (iflist)
    {
        PIP_ADAPTER_ADDRESSES pAdapterAddr = NULL;
        pAdapterAddr = GetAdapters();
        if (pAdapterAddr)
        {
            // Cycle through adapters
            // Add valid adapters to the interface list.
            bool ret = AddInterfaces(pAdapterAddr, iflist, desiredIndex);
            if (false == ret)
            {
                OIC_LOG(ERROR, TAG, "AddInterfaces() failed");
                u_arraylist_destroy(iflist);
                iflist = NULL;
            }

            // Finished with Adapter List
            OICFree(pAdapterAddr);
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Enumerating Adapters failed");
            u_arraylist_destroy(iflist);
            iflist = NULL;
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed to create iflist");
    }
    return iflist;
}
