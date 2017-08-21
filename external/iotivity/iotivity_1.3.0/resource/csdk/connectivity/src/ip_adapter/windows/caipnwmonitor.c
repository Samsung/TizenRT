/* *****************************************************************
*
* Copyright 2016 Microsoft
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
#include "iotivity_debug.h"
#include "caipinterface.h"

#include <assert.h>
#include <sys/types.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <mstcpip.h>
#include <iptypes.h>
#include <stdbool.h>
#include "octhread.h"
#include "caadapterutils.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "caipnwmonitor.h"
#include <coap/utlist.h>

#define TAG "IP_MONITOR"

// When this is defined, a socket will be used to get address change events.  The
// SIO_ADDRESS_LIST_CHANGE socket option is accessible on all versions of Windows
// and is available to both desktop and store apps, but it is more complex and hence
// more appropriate for use by applications using SIO_ADDRESS_LIST_QUERY to get addresses
// from Winsock rather than GetAdaptersAddresses() which we need to use to get additional
// information.  We expect NotifyUnicastIpAddressChange will be available to store apps
// at some point in the future and this workaround can go away at that point.
#define USE_SOCKET_ADDRESS_CHANGE_EVENT

/**
* Buffer size for PIP_ADAPTER_ADDRESSES
*/
#define WORKING_BUFFER_SIZE 15000

/**
 * Mutex for synchronizing access to cached address information.
 */
static oc_mutex g_CAIPNetworkMonitorMutex = NULL;

static bool g_CAIPNetworkMonitorSomeAddressWentAway = false;

typedef struct CANewAddress_t {
    struct CANewAddress_t *next;
    struct CANewAddress_t *prev;
    CAInterface_t *ipAddressInfo; 
} CANewAddress_t;

/**
 * List of network addresses that we've already reported.
 */
static u_arraylist_t *g_CAIPNetworkMonitorAddressList = NULL;

/**
 * Queue of new addresses that haven't yet been returned in CAFindInterfaceChange().
 */
static CANewAddress_t *g_CAIPNetworkMonitorNewAddressQueue = NULL;

/**
 * Transport adapter change callback list.
 */
static struct CAIPCBData_t *g_CAIPNetworkMonitorAdapterCallbackList = NULL;

static CAInterface_t *AllocateCAInterface(int index, const char *name, uint16_t family,
                                          const char *addr, int flags);

static u_arraylist_t *GetInterfaceInformation(int desiredIndex);

static void CAIPDestroyNetworkMonitorList();

static CAResult_t CAIPInitializeNetworkMonitorList()
{
    assert(!g_CAIPNetworkMonitorMutex);
    assert(!g_CAIPNetworkMonitorAddressList);

    g_CAIPNetworkMonitorMutex = oc_mutex_new();
    if (!g_CAIPNetworkMonitorMutex)
    {
        OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
        return CA_STATUS_FAILED;
    }

    g_CAIPNetworkMonitorAddressList = u_arraylist_create();
    if (!g_CAIPNetworkMonitorAddressList)
    {
        OIC_LOG(ERROR, TAG, "u_arraylist_create has failed");
        CAIPDestroyNetworkMonitorList();
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

/**
 * Destroy the network monitoring list.
 */
static void CAIPDestroyNetworkMonitorList()
{
    // Free any new addresses waiting to be indicated up.
    while (g_CAIPNetworkMonitorNewAddressQueue)
    {
        CANewAddress_t *change = g_CAIPNetworkMonitorNewAddressQueue;
        DL_DELETE(g_CAIPNetworkMonitorNewAddressQueue, change);
        OICFree(change->ipAddressInfo);
        OICFree(change);
    }

    // Free our cache of operational addresses.
    if (g_CAIPNetworkMonitorAddressList)
    {
        u_arraylist_destroy(g_CAIPNetworkMonitorAddressList);
        g_CAIPNetworkMonitorAddressList = NULL;
    }

    if (g_CAIPNetworkMonitorMutex)
    {
        oc_mutex_free(g_CAIPNetworkMonitorMutex);
        g_CAIPNetworkMonitorMutex = NULL;
    }
}

/**
 * See if a CAInterface_t with a given index and address already exists in a list.
 *
 * @param[in] ifIndex  Interface index to look for.
 * @param[in] family   Family of address to look for.
 * @param[in] addr     Address to look for.
 * @return true if already in the list, false if not.
 */
static bool CACmpNetworkList(uint32_t ifIndex, int family, const char *addr, u_arraylist_t *iflist)
{
    size_t list_length = u_arraylist_length(iflist);
    for (size_t list_index = 0; list_index < list_length; list_index++)
    {
        CAInterface_t *currItem = (CAInterface_t *) u_arraylist_get(iflist, list_index);
        if ((currItem->index == ifIndex) && (currItem->family == family) &&
            (strcmp(currItem->addr, addr) == 0))
        {
            return true;
        }
    }
    return false;
}

static HANDLE g_CAIPNetworkMonitorNotificationHandle = NULL;

/**
 * Handle a notification that the IP address info changed.
 *
 * @param[in]  context           Context passed to NotifyUnicastIpChange.
 * @param[in]  row               Interface that changed, or NULL on the initial callback.
 * @param[in]  notificationType  Type of change that occurred.
 */
static void CALLBACK IpAddressChangeCallback(void *context,
                                             MIB_UNICASTIPADDRESS_ROW *row,
                                             MIB_NOTIFICATION_TYPE notificationType)
{
    OC_UNUSED(notificationType);
    OC_UNUSED(row);
    OC_UNUSED(context);

    oc_mutex_lock(g_CAIPNetworkMonitorMutex);

    // Fetch new network address info.
    u_arraylist_t *newList = GetInterfaceInformation(0);
    size_t newLen = u_arraylist_length(newList);

    u_arraylist_t *oldList = g_CAIPNetworkMonitorAddressList;
    size_t oldLen = u_arraylist_length(oldList);

    if (caglobals.ip.addressChangeEvent)
    {
        // Check whether any addresses went away.
        for (size_t i = 0; i < oldLen; i++)
        {
            CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(oldList, i);
            if (!CACmpNetworkList(ifitem->index, ifitem->family, ifitem->addr, newList))
            {
                g_CAIPNetworkMonitorSomeAddressWentAway = true;
                break;
            }
        }

        // Check whether any new addresses are available.
        for (size_t i = 0; i < newLen; i++)
        {
            CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(newList, i);
            if (!CACmpNetworkList(ifitem->index, ifitem->family, ifitem->addr, oldList))
            {
                // Create a new CAInterface_t to add to the queue to indicate to the higher
                // layer.  We cannot simply invoke the callback here currently, since the
                // higher layer is not thread-safe.
                CAInterface_t *dup = AllocateCAInterface(ifitem->index, ifitem->name,
                                                         ifitem->family, ifitem->addr,
                                                         ifitem->flags);
                CANewAddress_t *change = (CANewAddress_t *)OICCalloc(1, sizeof(*change));
                if (change)
                {
                    change->ipAddressInfo = dup;
                    DL_APPEND(g_CAIPNetworkMonitorNewAddressQueue, change);
                }
                else
                {
                    OIC_LOG(WARNING, TAG, "Couldn't allocate memory for CANewAddress_t");
                }
            }
        }

        // If the new address queue is not empty, signal the transport server that it needs
        // to call CAFindInterfaceChange().  We don't need to set the event if an address
        // went away, since the higher layer just uses the event to ask for new addresses
        // in order to join the multicast group on the associated interface and address family.
        if (g_CAIPNetworkMonitorNewAddressQueue)
        {
            // Setting the event should always succeed, since the handle should always be
            // valid when this code is reached.
            OC_VERIFY(WSASetEvent(caglobals.ip.addressChangeEvent));
        }
    }

    // Replace old cached info.
    g_CAIPNetworkMonitorAddressList = newList;
    u_arraylist_destroy(oldList);

    oc_mutex_unlock(g_CAIPNetworkMonitorMutex);
}

#ifdef USE_SOCKET_ADDRESS_CHANGE_EVENT
static HANDLE g_CAIPNetworkMonitorShutdownEvent = NULL;

void UnregisterForIpAddressChange()
{
    if (g_CAIPNetworkMonitorShutdownEvent != NULL)
    {
        // Cancel the worker thread.
        OC_VERIFY(SetEvent(g_CAIPNetworkMonitorShutdownEvent));
        OC_VERIFY(WaitForSingleObject(g_CAIPNetworkMonitorNotificationHandle,
                                      INFINITE) == WAIT_OBJECT_0);
        OC_VERIFY(CloseHandle(g_CAIPNetworkMonitorShutdownEvent));
        g_CAIPNetworkMonitorShutdownEvent = NULL;
    }
    if (g_CAIPNetworkMonitorNotificationHandle != NULL)
    {
        OC_VERIFY(CloseHandle(g_CAIPNetworkMonitorNotificationHandle));
        g_CAIPNetworkMonitorNotificationHandle = NULL;
    }
}

DWORD WINAPI IpNetworkMonitorWorker(PVOID context)
{
    OVERLAPPED overlapped = {0};

    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData = {.wVersion = 0};
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != NO_ERROR)
    {
        return err;
    }

    SOCKET nwmSocket = WSASocketW(
        AF_INET6,
        SOCK_DGRAM,
        0, // Default proto.
        NULL, // No other protocol info.
        0, // Ignore group.
        WSA_FLAG_OVERLAPPED);
    if (INVALID_SOCKET == nwmSocket)
    {
        err = WSAGetLastError();
        goto done;
    }

    // Put socket into dual IPv4/IPv6 mode.
    BOOL ipv6Only = FALSE;
    if (SOCKET_ERROR == setsockopt(nwmSocket,
                                   IPPROTO_IPV6,
                                   IPV6_V6ONLY,
                                   (char*)&ipv6Only,
                                   sizeof(ipv6Only)))
    {
        err = WSAGetLastError();
        goto done;
    }

    overlapped.hEvent = CreateEvent(
        NULL, // No security descriptor.
        TRUE, // Manual reset event.
        FALSE, // Start not signaled.
        NULL); // No name.
    if (NULL == overlapped.hEvent)
    {
        err = GetLastError();
        goto done;
    }

    WSAEVENT eventList[2] = { overlapped.hEvent,
                              g_CAIPNetworkMonitorShutdownEvent,
                            };
    DWORD bytesReturned = 0;
    for (;;)
    {
        if (SOCKET_ERROR == WSAIoctl(nwmSocket,
                                     SIO_ADDRESS_LIST_CHANGE,
                                     NULL, // No input buffer.
                                     0,
                                     NULL, // No outupt buffer.
                                     0,
                                     &bytesReturned,
                                     &overlapped,
                                     NULL)) // No completion routine.
        {
            err = WSAGetLastError();
            if (err != ERROR_IO_PENDING)
            {
                break;
            }
            else
            {
                // Wait for an address change or a request to cancel the thread.
                DWORD waitStatus = WSAWaitForMultipleEvents(_countof(eventList),
                                                            eventList,
                                                            FALSE, // Wait for any one to fire.
                                                            WSA_INFINITE,
                                                            FALSE); // No I/O completion routines.

                if (waitStatus != WSA_WAIT_EVENT_0)
                {
                    // The cancel event was signaled.  There is no need to call CancelIo
                    // here, because we will close the socket handle below, causing any
                    // pending I/O to be canceled then.
                    assert(waitStatus == WSA_WAIT_EVENT_0 + 1);
                    break;
                }

                OC_VERIFY(WSAResetEvent(overlapped.hEvent));
            }
        }

        // We have a change to process.  The address change callback ignores
        // the parameters, so we just pass default values.
        IpAddressChangeCallback(context, NULL, MibInitialNotification);
    }

done:
    if (nwmSocket != INVALID_SOCKET)
    {
        closesocket(nwmSocket);
        nwmSocket = INVALID_SOCKET;
    }
    if (overlapped.hEvent != NULL)
    {
        OC_VERIFY(CloseHandle(overlapped.hEvent));
        overlapped.hEvent = NULL;
    }
    WSACleanup();
    return err;
}

BOOL RegisterForIpAddressChange()
{
    assert(g_CAIPNetworkMonitorNotificationHandle == NULL);

    g_CAIPNetworkMonitorShutdownEvent = CreateEvent(
        NULL, // No security descriptor.
        TRUE, // Manual reset event.
        FALSE, // Start not signaled.
        NULL); // No name.
    if (g_CAIPNetworkMonitorShutdownEvent == NULL)
    {
        return false;
    }

    g_CAIPNetworkMonitorNotificationHandle = CreateThread(
        NULL, // Default security attributes.
        0, // Default stack size.
        IpNetworkMonitorWorker,
        NULL, // No context.
        0, // Run immediately.
        NULL); // We don't need the thread id.

    if (g_CAIPNetworkMonitorNotificationHandle == NULL)
    {
        OC_VERIFY(CloseHandle(g_CAIPNetworkMonitorShutdownEvent));
        g_CAIPNetworkMonitorShutdownEvent = NULL;
        return false;
    }

    // Signal the callback to query the initial state.  The address change callback ignores
    // the parameters, so we just pass default values.
    IpAddressChangeCallback(NULL, NULL, MibInitialNotification);

    return true;
}
#endif

/**
 * Start network monitor.
 *
 * @param[in]  callback     Callback to be notified when IP/TCP adapter connection state changes.
 * @param[in]  adapter      Transport adapter.
 * @return ::CA_STATUS_OK or an appropriate error code.
 */
CAResult_t CAIPStartNetworkMonitor(CAIPAdapterStateChangeCallback callback,
                                   CATransportAdapter_t adapter)
{
    if (g_CAIPNetworkMonitorNotificationHandle != NULL)
    {
        // The monitor has already been started. This can happen when using both the
        // IP and TCP transport adapters.
        return CA_STATUS_OK;
    }

    CAResult_t res = CAIPInitializeNetworkMonitorList();
    if (res != CA_STATUS_OK)
    {
        return res;
    }

    res = CAIPSetNetworkMonitorCallback(callback, adapter);
    if (res != CA_STATUS_OK)
    {
        CAIPDestroyNetworkMonitorList();
        return res;
    }

#ifdef USE_SOCKET_ADDRESS_CHANGE_EVENT
    if (!RegisterForIpAddressChange())
    {
        CAIPDestroyNetworkMonitorList();
        CAIPUnSetNetworkMonitorCallback(adapter);
        return CA_STATUS_FAILED;
    }
#else
    int err = NotifyUnicastIpAddressChange(AF_UNSPEC, IpAddressChangeCallback, NULL,
                                           true,
                                           &g_CAIPNetworkMonitorNotificationHandle);
    if (err != NO_ERROR)
    {
        CAIPDestroyNetworkMonitorList();
        CAIPUnSetNetworkMonitorCallback(adapter);
        return CA_STATUS_FAILED;
    }
#endif
    return CA_STATUS_OK;
}

/**
 * Stops network monitor.
 *
 * @param[in]  adapter      Transport adapter.
 * @return ::CA_STATUS_OK or an appropriate error code.
 */
CAResult_t CAIPStopNetworkMonitor(CATransportAdapter_t adapter)
{
    if (g_CAIPNetworkMonitorNotificationHandle != NULL)
    {
#ifdef USE_SOCKET_ADDRESS_CHANGE_EVENT
        UnregisterForIpAddressChange();
#else
        int err = CancelMibChangeNotify2(g_CAIPNetworkMonitorNotificationHandle);
        assert(err == NO_ERROR);
        g_CAIPNetworkMonitorNotificationHandle = NULL;
#endif
    }

    CAIPDestroyNetworkMonitorList();
    return CAIPUnSetNetworkMonitorCallback(adapter);
}

/**
 * Let the network monitor update the polling interval.
 * @param[in] interval Current polling interval, in seconds
 *
 * @return  desired polling interval
 */
int CAGetPollingInterval(int interval)
{
    // Don't change the polling interval.
    return interval;
}

/**
 * Pass the changed network status through the stored callback.
 * Note that the current API doesn't allow us to specify which address changed,
 * the caller has to look at the return from CAFindInterfaceChange() to learn about
 * each new address, and look through CAIPGetInterfaceInformation() to see what's
 * missing to detect any removed addresses.
 *
 * @param[in] status Network status to pass to the callback.
 */
static void CAIPPassNetworkChangesToTransportAdapter(CANetworkStatus_t status)
{
    CAIPCBData_t *cbitem = NULL;
    LL_FOREACH(g_CAIPNetworkMonitorAdapterCallbackList, cbitem)
    {
        if (cbitem && cbitem->adapter)
        {
            cbitem->callback(cbitem->adapter, status);
        }
    }
}

/**
 * Set callback for receiving local IP/TCP adapter connection status.
 *
 * @param[in]  callback     Callback to be notified when IP/TCP adapter connection state changes.
 * @param[in]  adapter      Transport adapter.
 * @return ::CA_STATUS_OK or an appropriate error code.
 */
CAResult_t CAIPSetNetworkMonitorCallback(CAIPAdapterStateChangeCallback callback,
                                         CATransportAdapter_t adapter)
{
    if (!callback)
    {
        OIC_LOG(ERROR, TAG, "callback is null");
        return CA_STATUS_INVALID_PARAM;
    }

    CAIPCBData_t *cbitem = NULL;
    LL_FOREACH(g_CAIPNetworkMonitorAdapterCallbackList, cbitem)
    {
        if ((adapter == cbitem->adapter) && (callback == cbitem->callback))
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
    LL_APPEND(g_CAIPNetworkMonitorAdapterCallbackList, cbitem);

    return CA_STATUS_OK;
}

/**
 * Unset callback for receiving local IP/TCP adapter connection status.
 *
 * @param[in]  adapter      Transport adapter.
 * @return CA_STATUS_OK.
 */
CAResult_t CAIPUnSetNetworkMonitorCallback(CATransportAdapter_t adapter)
{
    CAIPCBData_t *cbitem = NULL;
    CAIPCBData_t *tmpCbitem = NULL;
    LL_FOREACH_SAFE(g_CAIPNetworkMonitorAdapterCallbackList, cbitem, tmpCbitem)
    {
        if (cbitem && adapter == cbitem->adapter)
        {
            OIC_LOG(DEBUG, TAG, "remove specific callback");
            LL_DELETE(g_CAIPNetworkMonitorAdapterCallbackList, cbitem);
            OICFree(cbitem);
            return CA_STATUS_OK;
        }
    }
    return CA_STATUS_OK;
}

/**
 * Allocate a new CAInterface_t entry for a given IP address.
 */
static CAInterface_t *AllocateCAInterface(int index, const char *name, uint16_t family,
                                          const char *addr, int flags)
{
    CAInterface_t *ifitem = (CAInterface_t *)OICCalloc(1, sizeof(*ifitem));
    if (!ifitem)
    {
        OIC_LOG(ERROR, TAG, "Allocating memory for a CAInterface_t failed");
        return NULL;
    }

    OICStrcpy(ifitem->name, sizeof(ifitem->name), name);
    ifitem->index = index;
    ifitem->family = family;
    OICStrcpy(ifitem->addr, sizeof(ifitem->addr), addr);
    ifitem->flags = flags;

    return ifitem;
}

/**
 * Find a new IP address.
 * The caller is responsible for freeing the pointer returned via u_arraylist_destroy().
 *
 * @return  Dynamically allocated IP address list, or NULL if no change.
 */
u_arraylist_t  *CAFindInterfaceChange()
{
    u_arraylist_t *iflist = u_arraylist_create();
    if (!iflist)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to create iflist: %s", strerror(errno));
        return NULL;
    }

    oc_mutex_lock(g_CAIPNetworkMonitorMutex);

    bool someAddressWentAway = g_CAIPNetworkMonitorSomeAddressWentAway;
    g_CAIPNetworkMonitorSomeAddressWentAway = false;

    bool newAddress = false;

    // Pop whole new address in list.
    while (g_CAIPNetworkMonitorNewAddressQueue)
    {
        CANewAddress_t *change = g_CAIPNetworkMonitorNewAddressQueue;

        bool result = u_arraylist_add(iflist, change->ipAddressInfo);
        if (!result)
        {
            OIC_LOG(ERROR, TAG, "u_arraylist_add failed.");
            break;
        }
        else
        {
            DL_DELETE(g_CAIPNetworkMonitorNewAddressQueue, change);
            OICFree(change);
            newAddress = true;
        }
    }

    oc_mutex_unlock(g_CAIPNetworkMonitorMutex);

    if (someAddressWentAway)
    {
        CAIPPassNetworkChangesToTransportAdapter(CA_INTERFACE_DOWN);
    }
    if (newAddress)
    {
        CAIPPassNetworkChangesToTransportAdapter(CA_INTERFACE_UP);
    }

    return iflist;
}

static bool IsValidNetworkAdapter(PIP_ADAPTER_ADDRESSES pAdapterAddr, int desiredIndex)
{
    bool valid = true;

    // If desiredIndex is non-zero, then only retrieve adapter corresponding to desiredIndex.
    // If desiredIndex is zero, then retrieve all adapters.
    if (desiredIndex && ((int)pAdapterAddr->IfIndex != desiredIndex))
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i not interesting.", pAdapterAddr->IfIndex);
        valid = false;
    }

    if (pAdapterAddr->IfType & IF_TYPE_SOFTWARE_LOOPBACK)
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i is loopback.", pAdapterAddr->IfIndex);
        valid = false;
    }

    if ((pAdapterAddr->OperStatus & IfOperStatusUp) == 0)
    {
        OIC_LOG_V(DEBUG, TAG, "\t\tInterface %i is not operational.", pAdapterAddr->IfIndex);
        valid = false;
    }
    return valid;
}

/*
 * Allocate a new CAInterface_t structure and add it to a given list.  A new entry is added
 * for each address.
 *
 * @param[in/out] iflist  List to add to.
 * @param[in]     name    Interface name.
 * @param[in]     index   Interface index.
 * @param[in]     family  Address family.
 * @param[in]     addr    Address.
 * @return Pointer to entry added, or NULL on failure.
 */
CAInterface_t *AddCAInterface(u_arraylist_t *iflist, const char *name, uint32_t index,
                              uint16_t family, const char *addr)
{
    CAInterface_t *ifitem = AllocateCAInterface(index, name, family, addr, IFF_UP);
    if (ifitem == NULL)
    {
        return NULL;
    }

    if (!u_arraylist_add(iflist, ifitem))
    {
        OIC_LOG(ERROR, TAG, "u_arraylist_add failed");
        OICFree(ifitem);
        return NULL;
    }

    return ifitem;
}

bool IsValidAddress(PIP_ADAPTER_UNICAST_ADDRESS pAddress)
{
    if (pAddress->Address.lpSockaddr->sa_family != AF_INET6)
    {
        // All IPv4 addresses are valid.
        return true;
    }

    PSOCKADDR_IN6 sockAddr = (PSOCKADDR_IN6)pAddress->Address.lpSockaddr;
    if (Ipv6UnicastAddressScope(sockAddr->sin6_addr.s6_addr) == ScopeLevelLink)
    {
        // IPv6 link local addresses are valid.
        return true;
    }

    // Other IPv6 addresses are valid if they are DNS eligible.
    // That is, ignore temporary addresses.
    return ((pAddress->Flags & IP_ADAPTER_ADDRESS_DNS_ELIGIBLE) != 0);
}

bool AddAddresses(PIP_ADAPTER_ADDRESSES pAdapterAddr, u_arraylist_t *iflist, int desiredIndex)
{
    bool bSucceeded = true;
    for (PIP_ADAPTER_ADDRESSES pCurAdapterAddr = pAdapterAddr;
         pCurAdapterAddr != NULL; pCurAdapterAddr = pCurAdapterAddr->Next)
    {
        OIC_LOG_V(DEBUG, TAG, "\tInterface Index: %u", pCurAdapterAddr->IfIndex);
        OIC_LOG_V(DEBUG, TAG, "\tInterface  name: %s", pCurAdapterAddr->AdapterName);

        if (!IsValidNetworkAdapter(pCurAdapterAddr, desiredIndex))
        {
            continue;
        }

        for (PIP_ADAPTER_UNICAST_ADDRESS pAddress = pCurAdapterAddr->FirstUnicastAddress;
             pAddress != NULL;
             pAddress = pAddress->Next)
        {
            if (!IsValidAddress(pAddress))
            {
                continue;
            }

            char addr[INET6_ADDRSTRLEN];
            if (!inet_ntop(pAddress->Address.lpSockaddr->sa_family,
                           INETADDR_ADDRESS(pAddress->Address.lpSockaddr),
                           addr,
                           sizeof(addr)))
            {
                continue;
            }

            CAInterface_t *ipAddressInfo = AddCAInterface(iflist, pCurAdapterAddr->AdapterName,
                                                          pCurAdapterAddr->IfIndex,
                                                          pAddress->Address.lpSockaddr->sa_family,
                                                          addr);
            if (!ipAddressInfo)
            {
                OIC_LOG_V(ERROR, TAG, "\tAdding address on interface %i failed",
                          pCurAdapterAddr->IfIndex);
                bSucceeded = false;
                break;
            }

            OIC_LOG_V(DEBUG, TAG, "\t\tAdded address %s", ipAddressInfo->addr);
        }
    }
    return bSucceeded;
}

/*
 * Get the set of IP_ADAPTER_ADDRESSES structures.  The caller is responsible for
 * freeng the set using OICFree on the pointer returned.
 *
 * @return List of network adapters.
 */
PIP_ADAPTER_ADDRESSES GetAdapters()
{
    ULONG ulOutBufLen = 0;
    PIP_ADAPTER_ADDRESSES pAdapterAddr = NULL;

    // We don't need most of the default information, so optimize this call by not
    // asking for them.
    ULONG flags = GAA_FLAG_SKIP_ANYCAST |
                  GAA_FLAG_SKIP_MULTICAST |
                  GAA_FLAG_SKIP_DNS_SERVER |
                  GAA_FLAG_SKIP_FRIENDLY_NAME;

    // Call up to 3 times: once to get the size, once to get the data, and once more
    // just in case there was an increase in length in between the first two. If the
    // length is still increasing due to more addresses being added, even this may fail
    // and we'll have to wait for the next IP address change notification.
    for (int i = 0; i < 3; i++)
    {
        ULONG ret = GetAdaptersAddresses(AF_UNSPEC, flags, NULL, pAdapterAddr, &ulOutBufLen);
        if (ERROR_BUFFER_OVERFLOW == ret)
        {
            // Redo with updated length.
            if (pAdapterAddr != NULL)
            {
                OICFree(pAdapterAddr);
            }
            pAdapterAddr = (PIP_ADAPTER_ADDRESSES) OICMalloc(ulOutBufLen);
            if (pAdapterAddr == NULL) {
                OIC_LOG(ERROR, TAG, "Allocating memory for GetAdaptersAddresses() failed");
                break;
            }
            continue;
        }
        if (NO_ERROR != ret)
        {
            OIC_LOG(ERROR, TAG, "GetAdaptersAddresses() failed");
            break;
        }

        // Succeeded getting adapters
        return pAdapterAddr;
    }

    if (pAdapterAddr != NULL)
    {
        OICFree(pAdapterAddr);
    }
    return NULL;
}

/**
 * Get the list of CAInterface_t items.  Currently only 0 is passed as the desiredIndex by any
 * caller.
 *
 * @param[in]  desiredIndex      Network interface index, or 0 for all.
 * @return  List of CAInterface_t items.
 */
static u_arraylist_t *GetInterfaceInformation(int desiredIndex)
{
    if (desiredIndex < 0)
    {
        OIC_LOG_V(ERROR, TAG, "invalid index : %d", desiredIndex);
        return NULL;
    }

    u_arraylist_t *iflist = u_arraylist_create();
    if (!iflist)
    {
        OIC_LOG(ERROR, TAG, "Failed to create iflist");
        return NULL;
    }

    PIP_ADAPTER_ADDRESSES pAdapterAddr = GetAdapters();
    if (!pAdapterAddr)
    {
        OIC_LOG(ERROR, TAG, "Enumerating Adapters failed");
        u_arraylist_destroy(iflist);
        return NULL;
    }

    // Cycle through network adapters.
    // Add valid network addresses to the address list.
    bool ret = AddAddresses(pAdapterAddr, iflist, desiredIndex);
    if (false == ret)
    {
        OIC_LOG(ERROR, TAG, "AddAddresses() failed");
        u_arraylist_destroy(iflist);
        iflist = NULL;
    }

    // Finished with network adapter list.
    OICFree(pAdapterAddr);

    return iflist;
}

u_arraylist_t *CAIPGetInterfaceInformation(int desiredIndex)
{
    OC_UNUSED(desiredIndex);

    u_arraylist_t *iflist = u_arraylist_create();
    if (!iflist)
    {
        OIC_LOG(ERROR, TAG, "Failed to create iflist");
        return NULL;
    }

    // Avoid extra kernel calls by just duplicating what's in our cache.
    oc_mutex_lock(g_CAIPNetworkMonitorMutex);

    size_t list_length = u_arraylist_length(g_CAIPNetworkMonitorAddressList);
    for (size_t list_index = 0; list_index < list_length; list_index++)
    {
        CAInterface_t *currItem = (CAInterface_t *)u_arraylist_get(g_CAIPNetworkMonitorAddressList,
                                                                   list_index);
        if (!AddCAInterface(iflist, currItem->name, currItem->index, currItem->family,
                            currItem->addr))
        {
            OIC_LOG(ERROR, TAG, "AddCAInterface() failed");
            u_arraylist_destroy(iflist);
            iflist = NULL;
            break;
        }
    }

    oc_mutex_unlock(g_CAIPNetworkMonitorMutex);

    return iflist;
}

CAResult_t CAGetLinkLocalZoneIdInternal(uint32_t ifindex, char **zoneId)
{
    if (!zoneId || (*zoneId != NULL))
    {
        return CA_STATUS_INVALID_PARAM;
    }

    PIP_ADAPTER_ADDRESSES pAdapters = GetAdapters();

    if (!pAdapters)
    {
        OICFree(pAdapters);
        return CA_STATUS_FAILED;
    }

    PIP_ADAPTER_ADDRESSES pCurrAdapter = NULL;
    pCurrAdapter = pAdapters;

    while (pCurrAdapter)
    {
        if (ifindex == pCurrAdapter->IfIndex)
        {
            OIC_LOG_V(DEBUG, TAG, "Given ifindex is %d parsed zoneId is %d",
                      ifindex, pCurrAdapter->ZoneIndices[ScopeLevelLink]);
            *zoneId = (char *)OICCalloc(IF_NAMESIZE, sizeof(char));
            _ultoa(pCurrAdapter->ZoneIndices[ScopeLevelLink], *zoneId, 10);
            break;
        }
        pCurrAdapter = pCurrAdapter->Next;
    }

    OICFree(pAdapters);

    if (!*zoneId)
    {
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}
