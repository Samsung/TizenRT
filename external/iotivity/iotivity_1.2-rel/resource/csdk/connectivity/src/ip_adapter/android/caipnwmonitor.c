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

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <linux/if.h>
#include <coap/utlist.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>

#include "caadapterutils.h"
#include "caipnwmonitor.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "org_iotivity_ca_CaIpInterface.h"
#include "caifaddrs.h"

#define TAG "OIC_CA_IP_MONITOR"
#define NETLINK_MESSAGE_LENGTH  (4096)
#define IFC_LABEL_LOOP          "lo"
#define IFC_ADDR_LOOP_IPV4      "127.0.0.1"
#define IFC_ADDR_LOOP_IPV6      "::1"
/**
 * Used to storing adapter changes callback interface.
 */
static struct CAIPCBData_t *g_adapterCallbackList = NULL;

/**
 * Create new interface item to add in activated interface list.
 * @param[in]  index    Network interface index number.
 * @param[in]  name     Network interface name.
 * @param[in]  family   Network interface family type.
 * @param[in]  addr     New interface address.
 * @param[in]  flags    The active flag word of a device.
 * @return  CAInterface_t objects.
 */
static CAInterface_t *CANewInterfaceItem(int index, const char *name, int family,
                                         const char *addr, int flags);

/**
 * Add created new interface item activated interface list.
 * @param[in]  iflist   Network interface array list.
 * @param[in]  index    Network interface index number.
 * @param[in]  name     Network interface name.
 * @param[in]  family   Network interface family type.
 * @param[in]  addr     New interface address.
 * @param[in]  flags    The active flag word of a device.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
static CAResult_t CAAddInterfaceItem(u_arraylist_t *iflist, int index,
                                     const char *name, int family, const char *addr, int flags);

/**
 * Initialize JNI interface.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAIPJniInit();

/**
 * Destroy JNI interface.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
static CAResult_t CAIPDestroyJniInterface();

#define MAX_INTERFACE_INFO_LENGTH 1024 // allows 32 interfaces from SIOCGIFCONF

CAResult_t CAIPStartNetworkMonitor(CAIPAdapterStateChangeCallback callback,
                                   CATransportAdapter_t adapter)
{
    CAResult_t res = CAIPJniInit();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "failed to initialize ip jni interface");
        return res;
    }

    return CAIPSetNetworkMonitorCallback(callback, adapter);
}

CAResult_t CAIPStopNetworkMonitor(CATransportAdapter_t adapter)
{
    CAIPUnSetNetworkMonitorCallback(adapter);

    // if there is no callback to pass the changed status, stop monitoring.
    if (!g_adapterCallbackList)
    {
        return CAIPDestroyJniInterface();
    }

    return CA_STATUS_OK;
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
        }
    }
}

CAResult_t CAIPSetNetworkMonitorCallback(CAIPAdapterStateChangeCallback callback,
                                         CATransportAdapter_t adapter)
{
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

u_arraylist_t *CAFindInterfaceChange()
{
    // release netlink event
    char buf[NETLINK_MESSAGE_LENGTH] = { 0 };
    struct sockaddr_nl sa = { 0 };
    struct iovec iov = { .iov_base = buf,
                         .iov_len = sizeof (buf) };
    struct msghdr msg = { .msg_name = (void *)&sa,
                          .msg_namelen = sizeof (sa),
                          .msg_iov = &iov,
                          .msg_iovlen = 1 };

    ssize_t len = recvmsg(caglobals.ip.netlinkFd, &msg, 0);
    return NULL;
}

/**
 * Used to send netlink query to kernel and recv response from kernel.
 *
 * @param[in]   idx       desired network interface index, 0 means all interfaces.
 * @param[out]  iflist    linked list.
 *
 */
static bool CAParsingNetorkInfo(int idx, u_arraylist_t *iflist)
{
    if ((idx < 0) || (iflist == NULL))
    {
        return false;
    }

    struct ifaddrs *ifp = NULL;
    CAResult_t ret = CAGetIfaddrsUsingNetlink(&ifp);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get ifaddrs err code is: %d", ret);
        return false;
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
        if (idx && (ifindex != idx))
        {
            continue;
        }

        char ipaddr[MAX_ADDR_STR_SIZE_CA] = {0};
        if (family == AF_INET6)
        {
            struct sockaddr_in6 *in6 = (struct sockaddr_in6*) ifa->ifa_addr;
            inet_ntop(family, (void *)&(in6->sin6_addr), ipaddr, sizeof(ipaddr));
        }
        else if (family == AF_INET)
        {
            struct sockaddr_in *in = (struct sockaddr_in*) ifa->ifa_addr;
            inet_ntop(family, (void *)&(in->sin_addr), ipaddr, sizeof(ipaddr));
        }

        if ((strcmp(ipaddr, IFC_ADDR_LOOP_IPV4) == 0) ||
            (strcmp(ipaddr, IFC_ADDR_LOOP_IPV6) == 0) ||
            (strcmp(ifa->ifa_name, IFC_LABEL_LOOP) == 0))
        {
            OIC_LOG(DEBUG, TAG, "LOOPBACK continue!!!");
            continue;
        }

        CAResult_t result = CAAddInterfaceItem(iflist, ifindex,
                                               ifa->ifa_name, family,
                                               ipaddr, ifa->ifa_flags);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, TAG, "CAAddInterfaceItem fail");
            goto exit;
        }
    }
    CAFreeIfAddrs(ifp);
    return true;

exit:
    CAFreeIfAddrs(ifp);
    return false;
}

u_arraylist_t *CAIPGetInterfaceInformation(int desiredIndex)
{
    u_arraylist_t *iflist = u_arraylist_create();
    if (!iflist)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to create iflist: %s", strerror(errno));
        return NULL;
    }

    if (!CAParsingNetorkInfo(desiredIndex, iflist))
    {
        goto exit;
    }

    return iflist;

exit:
    u_arraylist_destroy(iflist);
    return NULL;
}

static CAResult_t CAAddInterfaceItem(u_arraylist_t *iflist, int index,
                                     const char *name, int family, const char *addr, int flags)
{
    CAInterface_t *ifitem = CANewInterfaceItem(index, name, family, addr, flags);
    if (!ifitem)
    {
        return CA_STATUS_FAILED;
    }
    bool result = u_arraylist_add(iflist, ifitem);
    if (!result)
    {
        OIC_LOG(ERROR, TAG, "u_arraylist_add failed.");
        OICFree(ifitem);
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

static CAInterface_t *CANewInterfaceItem(int index, const char *name, int family,
                                         const char *addr, int flags)
{
    CAInterface_t *ifitem = (CAInterface_t *)OICCalloc(1, sizeof (CAInterface_t));
    if (!ifitem)
    {
        OIC_LOG(ERROR, TAG, "Malloc failed in CANewInterfaceItem");
        return NULL;
    }

    OICStrcpy(ifitem->name, sizeof (ifitem->name), name);
    ifitem->index = index;
    ifitem->family = family;
    OICStrcpy(ifitem->addr, sizeof (ifitem->addr), addr);
    ifitem->flags = flags;

    return ifitem;
}

CAResult_t CAIPJniInit()
{
    OIC_LOG(DEBUG, TAG, "CAIPJniInit_IN");

    JavaVM *jvm = CANativeJNIGetJavaVM();
    if (!jvm)
    {
        OIC_LOG(ERROR, TAG, "Could not get JavaVM pointer");
        return CA_STATUS_FAILED;
    }

    jobject context = CANativeJNIGetContext();
    if (!context)
    {
        OIC_LOG(ERROR, TAG, "unable to get application context");
        return CA_STATUS_FAILED;
    }

    JNIEnv* env = NULL;
    if ((*jvm)->GetEnv(jvm, (void**) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        OIC_LOG(ERROR, TAG, "Could not get JNIEnv pointer");
        return CA_STATUS_FAILED;
    }

    jmethodID mid_getApplicationContext = CAGetJNIMethodID(env, "android/content/Context",
                                                           "getApplicationContext",
                                                           "()Landroid/content/Context;");

    if (!mid_getApplicationContext)
    {
        OIC_LOG(ERROR, TAG, "Could not get getApplicationContext method");
        return CA_STATUS_FAILED;
    }

    jobject jApplicationContext = (*env)->CallObjectMethod(env, context,
                                                           mid_getApplicationContext);
    if (!jApplicationContext)
    {
        OIC_LOG(ERROR, TAG, "Could not get application context");
        return CA_STATUS_FAILED;
    }

    jclass cls_CaIpInterface = (*env)->FindClass(env, "org/iotivity/ca/CaIpInterface");
    if (!cls_CaIpInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaIpInterface class");
        return CA_STATUS_FAILED;
    }

    jmethodID mid_CaIpInterface_ctor = (*env)->GetMethodID(env, cls_CaIpInterface, "<init>",
                                                                   "(Landroid/content/Context;)V");
    if (!mid_CaIpInterface_ctor)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaIpInterface constructor method");
        return CA_STATUS_FAILED;
    }

    (*env)->NewObject(env, cls_CaIpInterface, mid_CaIpInterface_ctor, jApplicationContext);
    OIC_LOG(DEBUG, TAG, "Create CaIpInterface instance, success");

    OIC_LOG(DEBUG, TAG, "CAIPJniInit_OUT");
    return CA_STATUS_OK;
}

static CAResult_t CAIPDestroyJniInterface()
{
    OIC_LOG(DEBUG, TAG, "CAIPDestroyJniInterface");

    JavaVM *jvm = CANativeJNIGetJavaVM();
    if (!jvm)
    {
        OIC_LOG(ERROR, TAG, "Could not get JavaVM pointer");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*jvm)->GetEnv(jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*jvm)->AttachCurrentThread(jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    jclass jni_IpInterface = (*env)->FindClass(env, "org/iotivity/ca/CaIpInterface");
    if (!jni_IpInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaIpInterface class");
        goto error_exit;
    }

    jmethodID jni_InterfaceDestroyMethod = (*env)->GetStaticMethodID(env, jni_IpInterface,
                                                                     "destroyIpInterface",
                                                                     "()V");
    if (!jni_InterfaceDestroyMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaIpInterface destroy method");
        goto error_exit;
    }

    (*env)->CallStaticVoidMethod(env, jni_IpInterface, jni_InterfaceDestroyMethod);

    if ((*env)->ExceptionCheck(env))
    {
        OIC_LOG(ERROR, TAG, "destroyIpInterface has failed");
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        goto error_exit;
    }

    OIC_LOG(DEBUG, TAG, "Destroy instance for CaIpInterface");

    if (isAttached)
    {
        (*jvm)->DetachCurrentThread(jvm);
    }

    return CA_STATUS_OK;

error_exit:

    if (isAttached)
    {
        (*jvm)->DetachCurrentThread(jvm);
    }

    return CA_STATUS_FAILED;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaIpInterface_caIpStateEnabled(JNIEnv *env, jclass class)
{
    (void)env;
    (void)class;

    OIC_LOG(DEBUG, TAG, "Wifi is in Activated State");
    CAIPPassNetworkChangesToAdapter(CA_INTERFACE_UP);

    // Apply network interface changes.
    u_arraylist_t *iflist = CAIPGetInterfaceInformation(0);
    if (!iflist)
    {
        OIC_LOG_V(ERROR, TAG, "get interface info failed: %s", strerror(errno));
        return;
    }

    uint32_t listLength = u_arraylist_length(iflist);
    for (uint32_t i = 0; i < listLength; i++)
    {
        CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
        if (!ifitem)
        {
            continue;
        }

        CAProcessNewInterface(ifitem);

    }
    u_arraylist_destroy(iflist);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaIpInterface_caIpStateDisabled(JNIEnv *env, jclass class)
{
    (void)env;
    (void)class;

    OIC_LOG(DEBUG, TAG, "Wifi is in Deactivated State");
    CAIPPassNetworkChangesToAdapter(CA_INTERFACE_DOWN);
}
