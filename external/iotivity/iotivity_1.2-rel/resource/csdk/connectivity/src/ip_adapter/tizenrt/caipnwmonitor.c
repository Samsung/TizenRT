/******************************************************************
*
* Copyright 2014 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <errno.h>
#include <slsi_wifi/slsi_wifi_api.h>

#include "octhread.h"
#include "caipnwmonitor.h"
#include "caadapterutils.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include <coap/utlist.h>

#define TAG "OIC_CA_IP_MONITOR"

/**
 * Mutex for synchronizing access to cached interface and IP address information.
 */
static oc_mutex g_networkMonitorContextMutex = NULL;

/**
 * Used to storing network interface.
 */
static u_arraylist_t *g_netInterfaceList = NULL;

/**
 * Used to storing adapter changes callback interface.
 */
static struct CAIPCBData_t *g_adapterCallbackList = NULL;

/**
 * Initialize the network interface monitoring list.
 */
static CAResult_t CAIPInitializeNetworkMonitorList();

/**
 * Destroy the network interface monitoring list.
 */
static void CAIPDestroyNetworkMonitorList();

/**
 * Compare the interface with the already added interface in list.
 */
static bool CACmpNetworkList(uint32_t ifiindex);

/**
 * Add new network interface in list.
 */
static CAResult_t CAAddNetworkMonitorList(CAInterface_t *ifitem);

/**
 * Remove network interace from list.
 */
static void CARemoveNetworkMonitorList(int ifiindex);

/**
 * Pass the changed network status through the stored callback.
 */
static void CAIPPassNetworkChangesToAdapter(CANetworkStatus_t status);

/**
 * Create new interface item.
 */
static CAInterface_t *CANewInterfaceItem(int index, const char *name, int family,
                                         const char *addr, int flags);


static CAResult_t CAIPInitializeNetworkMonitorList()
{
    if (!g_networkMonitorContextMutex)
    {
        g_networkMonitorContextMutex = oc_mutex_new();
        if (!g_networkMonitorContextMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (!g_netInterfaceList)
    {
        g_netInterfaceList = u_arraylist_create();
        if (!g_netInterfaceList)
        {
            OIC_LOG(ERROR, TAG, "u_arraylist_create has failed");
            CAIPDestroyNetworkMonitorList();
            return CA_STATUS_FAILED;
        }
    }
    return CA_STATUS_OK;
}

static void CAIPDestroyNetworkMonitorList()
{
    if (g_netInterfaceList)
    {
        u_arraylist_destroy(g_netInterfaceList);
        g_netInterfaceList = NULL;
    }

    if (g_networkMonitorContextMutex)
    {
        oc_mutex_free(g_networkMonitorContextMutex);
        g_networkMonitorContextMutex = NULL;
    }
}

static bool CACmpNetworkList(uint32_t ifiindex)
{
    if (!g_netInterfaceList)
    {
        OIC_LOG(ERROR, TAG, "g_netInterfaceList is NULL");
        return false;
    }

    oc_mutex_lock(g_networkMonitorContextMutex);

    uint32_t list_length = u_arraylist_length(g_netInterfaceList);
    for (uint32_t list_index = 0; list_index < list_length; list_index++)
    {
        CAInterface_t *currItem = (CAInterface_t *) u_arraylist_get(g_netInterfaceList,
                                                                    list_index);
        if (currItem->index == ifiindex)
        {
            oc_mutex_unlock(g_networkMonitorContextMutex);
            return true;
        }
    }
    oc_mutex_unlock(g_networkMonitorContextMutex);
    return false;
}

static CAResult_t CAAddNetworkMonitorList(CAInterface_t *ifitem)
{
    VERIFY_NON_NULL(g_netInterfaceList, TAG, "g_netInterfaceList is NULL");
    VERIFY_NON_NULL(ifitem, TAG, "ifitem is NULL");

    oc_mutex_lock(g_networkMonitorContextMutex);
    bool result = u_arraylist_add(g_netInterfaceList, (void *) ifitem);
    if (!result)
    {
        OIC_LOG(ERROR, TAG, "u_arraylist_add failed.");
        oc_mutex_unlock(g_networkMonitorContextMutex);
        return CA_STATUS_FAILED;
    }
    oc_mutex_unlock(g_networkMonitorContextMutex);
    return CA_STATUS_OK;
}

static void CARemoveNetworkMonitorList(int ifiindex)
{ // pkbook
    VERIFY_NON_NULL_VOID(g_netInterfaceList, TAG, "g_netInterfaceList is NULL");

    oc_mutex_lock(g_networkMonitorContextMutex);

    uint32_t list_length = u_arraylist_length(g_netInterfaceList);
    for (uint32_t list_index = 0; list_index < list_length; list_index++)
    {
        CAInterface_t *removedifitem = (CAInterface_t *) u_arraylist_get(
                g_netInterfaceList, list_index);
        if (removedifitem && ((int)removedifitem->index) == ifiindex)
        {
            if (u_arraylist_remove(g_netInterfaceList, list_index))
            {
                OICFree(removedifitem);
                oc_mutex_unlock(g_networkMonitorContextMutex);
                return;
            }
            continue;
        }
    }
    oc_mutex_unlock(g_networkMonitorContextMutex);
    return;
}

CAResult_t CAIPStartNetworkMonitor(CAIPAdapterStateChangeCallback callback,
                                   CATransportAdapter_t adapter) // pkbook
{
    CAResult_t res = CAIPInitializeNetworkMonitorList();
    if (CA_STATUS_OK == res)
    {
        return CAIPSetNetworkMonitorCallback(callback, adapter);
    }
    return res;
}

CAResult_t CAIPStopNetworkMonitor(CATransportAdapter_t adapter)
{
    CAIPDestroyNetworkMonitorList();
    return CAIPUnSetNetworkMonitorCallback(adapter);
}

int CAGetPollingInterval(int interval)
{
    return interval;
}

static void CAIPPassNetworkChangesToAdapter(CANetworkStatus_t status)
{
    CAIPCBData_t *cbitem = NULL;

    LL_FOREACH(g_adapterCallbackList, cbitem)
    {
        if (cbitem && cbitem->adapter)
        {
            cbitem->callback(cbitem->adapter, status);
            CALogAdapterStateInfo(cbitem->adapter, status);
        }
    }
}

CAResult_t CAIPSetNetworkMonitorCallback(CAIPAdapterStateChangeCallback callback,
                                         CATransportAdapter_t adapter)
{ // pkbook
    if (!callback)
    {
        OIC_LOG(ERROR, TAG, "callback is null");
        return CA_STATUS_INVALID_PARAM;
    }

    CAIPCBData_t *cbitem = NULL;
    LL_FOREACH(g_adapterCallbackList, cbitem)
    {
        if (cbitem && adapter == cbitem->adapter && callback == cbitem->callback)
        {
            OIC_LOG(DEBUG, TAG, "this callback is already added");
            return CA_STATUS_OK;
        }
    }

    cbitem = (CAIPCBData_t *)OICCalloc(1, sizeof(*cbitem));
    if (!cbitem)
    {
        OIC_LOG(ERROR, TAG, "Malloc failed");
        return CA_STATUS_FAILED;
    }

    cbitem->adapter = adapter;
    cbitem->callback = callback;
    LL_APPEND(g_adapterCallbackList, cbitem);

    return CA_STATUS_OK;
}

CAResult_t CAIPUnSetNetworkMonitorCallback(CATransportAdapter_t adapter)
{
    CAIPCBData_t *cbitem = NULL;
    CAIPCBData_t *tmpCbitem = NULL;
    LL_FOREACH_SAFE(g_adapterCallbackList, cbitem, tmpCbitem)
    {
        if (cbitem && adapter == cbitem->adapter)
        {
            OIC_LOG(DEBUG, TAG, "remove specific callback");
            LL_DELETE(g_adapterCallbackList, cbitem);
            OICFree(cbitem);
            return CA_STATUS_OK;
        }
    }
    return CA_STATUS_OK;
}

static CAInterface_t *CANewInterfaceItem(int index, const char *name, int family,
                                         const char *addr, int flags)
{
    CAInterface_t *ifitem = (CAInterface_t *)OICCalloc(1, sizeof (CAInterface_t));
    if (!ifitem)
    {
        OIC_LOG(ERROR, TAG, "Malloc failed");
        return NULL;
    }

    OICStrcpy(ifitem->name, sizeof (ifitem->name), name);
    ifitem->index = index;
    ifitem->family = family;
    OICStrcpy(ifitem->addr, sizeof (ifitem->addr), addr);
    ifitem->flags = flags;

    return ifitem;
}

extern mqd_t g_nwevent_mqfd; // pkmsgq

u_arraylist_t *CAFindInterfaceChange()
{
    u_arraylist_t *iflist = NULL;

	int len = 0;
	char buf[4];

	len = mq_receive(g_nwevent_mqfd, buf, 4, NULL);
	if(len <= 0) return NULL;

	if(buf[0] == 'd' && buf[1] == 'e' && buf[2] == 'l') {
		printf("Receive the event(IF is down)\n");
	}
	else if(buf[0] == 'g' && buf[1] == 'e' && buf[2] == 'n') {
		printf("Receive the event(IF is UP)\n");
		iflist = CAIPGetInterfaceInformation(0);
        if (!iflist) {
            OIC_LOG_V(ERROR, TAG, "get interface info failed: %s", strerror(errno));
            return NULL;
        }
	}

	return iflist;
}

u_arraylist_t *CAIPGetInterfaceInformation(int desiredIndex)
{
    if (desiredIndex < 0)
    {
        OIC_LOG_V(ERROR, TAG, "invalid index : %d", desiredIndex);
        return NULL;
    }

    u_arraylist_t *iflist = u_arraylist_create();
    if (!iflist)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to create iflist: %s", strerror(errno));
        return NULL;
    }

    struct ifaddrs *ifp = NULL;
    if (-1 == getifaddrs(&ifp))
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get ifaddrs: %s", strerror(errno));
        u_arraylist_destroy(iflist);
        return NULL;
    }

    struct ifaddrs *ifa = NULL;
    for (ifa = ifp; ifa; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
        {
            continue;
        }
        int family = ifa->ifa_addr->sa_family;
        if ((ifa->ifa_flags & IFF_LOOPBACK) || (AF_INET != family && AF_INET6 != family))
        {
            continue;
        }

        int ifindex = if_nametoindex(ifa->ifa_name);
        if (desiredIndex && (ifindex != desiredIndex))
        {
            continue;
        }

        int length = u_arraylist_length(iflist);
        int already = false;
        for (int i = length-1; i >= 0; i--)
        {
            CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);

            if (ifitem
                && (int)ifitem->index == ifindex
                && ifitem->family == (uint16_t)family)
            {
                already = true;
                break;
            }
        }
        if (already)
        {
            continue;
        }

        CAInterface_t *ifitem = (CAInterface_t *)OICCalloc(1, sizeof(CAInterface_t));
        if (!ifitem)
        {
            OIC_LOG(ERROR, TAG, "Malloc failed");
            goto exit;
        }

        OICStrcpy(ifitem->name, INTERFACE_NAME_MAX, ifa->ifa_name);
        ifitem->index = ifindex;
        ifitem->family = family;
        ifitem->flags = ifa->ifa_flags;

        if (ifitem->family == AF_INET6)
        {
            struct sockaddr_in6 *in6 = (struct sockaddr_in6*) ifa->ifa_addr;
            inet_ntop(ifitem->family, (void *)&(in6->sin6_addr), ifitem->addr,
                      sizeof(ifitem->addr));
        }
        else if (ifitem->family == AF_INET)
        {
            struct sockaddr_in *in = (struct sockaddr_in*) ifa->ifa_addr;
            inet_ntop(ifitem->family, (void *)&(in->sin_addr), ifitem->addr,
                      sizeof(ifitem->addr));
        }

        bool result = u_arraylist_add(iflist, ifitem);
        if (!result)
        {
            OIC_LOG(ERROR, TAG, "u_arraylist_add failed.");
            goto exit;
        }

        bool isFound = CACmpNetworkList(ifitem->index);
        if (!isFound)
        {
            CAInterface_t *newifitem = CANewInterfaceItem(ifitem->index, ifitem->name, ifitem->family,
                                                          ifitem->addr, ifitem->flags);
            CAResult_t ret = CAAddNetworkMonitorList(newifitem);
            if (CA_STATUS_OK != ret)
            {
                OICFree(newifitem);
                goto exit;
            }
            CAIPPassNetworkChangesToAdapter(CA_INTERFACE_UP);
            OIC_LOG_V(DEBUG, TAG, "Added interface: %s (%d)", ifitem->name, ifitem->family);
        }
    }
#ifndef __TIZENRT__
    freeifaddrs(ifp);
#endif
    return iflist;

exit:
#ifndef __TIZENRT__
    freeifaddrs(ifp);
#endif
    u_arraylist_destroy(iflist);
    return NULL;
}
