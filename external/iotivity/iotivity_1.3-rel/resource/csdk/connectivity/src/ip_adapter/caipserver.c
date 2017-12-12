/* ****************************************************************
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

#ifndef __APPLE_USE_RFC_3542
#define __APPLE_USE_RFC_3542 // for PKTINFO
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE // for in6_pktinfo
#endif

#include "iotivity_config.h"
#include "iotivity_debug.h"

#include <sys/types.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif
#include <errno.h>
#ifdef __linux__
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif

#ifdef __TIZENRT__
#include <tinyara/config.h>
#include <mqueue.h>
#include <net/lwip/inet.h>
#include <net/lwip/sockets.h>
#endif
#include <coap/pdu.h>
#include <inttypes.h>
#include "caipinterface.h"
#include "caipnwmonitor.h"
#include "caadapterutils.h"
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "ca_adapter_net_ssl.h"
#endif
#include "octhread.h"
#include "oic_malloc.h"
#include "oic_string.h"

#define USE_IP_MREQN
#if defined(_WIN32) || defined(__TIZENRT__)
#undef USE_IP_MREQN
#endif

/*
 * Logging tag for module name
 */
#define TAG "OIC_CA_IP_SERVER"

#ifdef __TIZENRT__
mqd_t g_nwevent_mqfd;
#ifdef CONFIG_NET_LWIP
#define SOCK_CLOEXEC 0
#else
#define SOCK_CLOEXEC 1
#endif
#endif
#define SELECT_TIMEOUT 1     // select() seconds (and termination latency)

#define IPv4_MULTICAST     "224.0.1.187"
static struct in_addr IPv4MulticastAddress = { 0 };

#define IPv6_DOMAINS       16
#define IPv6_MULTICAST_INT "ff01::158"
static struct in6_addr IPv6MulticastAddressInt;
#define IPv6_MULTICAST_LNK "ff02::158"
static struct in6_addr IPv6MulticastAddressLnk;
#define IPv6_MULTICAST_RLM "ff03::158"
static struct in6_addr IPv6MulticastAddressRlm;
#define IPv6_MULTICAST_ADM "ff04::158"
static struct in6_addr IPv6MulticastAddressAdm;
#define IPv6_MULTICAST_SIT "ff05::158"
static struct in6_addr IPv6MulticastAddressSit;
#define IPv6_MULTICAST_ORG "ff08::158"
static struct in6_addr IPv6MulticastAddressOrg;
#define IPv6_MULTICAST_GLB "ff0e::158"
static struct in6_addr IPv6MulticastAddressGlb;

/*
 * Buffer size for the receive message buffer
 */
#define RECV_MSG_BUF_LEN 16384

static char *ipv6mcnames[IPv6_DOMAINS] = {
    NULL,
    IPv6_MULTICAST_INT,
    IPv6_MULTICAST_LNK,
    IPv6_MULTICAST_RLM,
    IPv6_MULTICAST_ADM,
    IPv6_MULTICAST_SIT,
    NULL,
    NULL,
    IPv6_MULTICAST_ORG,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    IPv6_MULTICAST_GLB,
    NULL
};

#ifdef __TIZENRT__ /* this code block may be modified */
struct in6_pktinfo {
        struct in6_addr ipi6_addr;
        int             ipi6_ifindex;
};

struct in_pktinfo
{
  unsigned int   ipi_ifindex;  /* Interface index */
  struct in_addr ipi_spec_dst; /* Local address */
  struct in_addr ipi_addr;     /* Header Destination
                                    address */
};

#define RTMGRP_LINK 1
#define IP_PKTINFO         8
#define IPV6_PKTINFO            50
#define IPV6_MULTICAST_IF 9
#define IPV6_V6ONLY 27
#define IPV6_RECVPKTINFO       50
#define IPV6_JOIN_GROUP 12
#endif

#if defined (_WIN32)
#define IFF_UP_RUNNING_FLAGS  (IFF_UP)

    char* caips_get_error(){
        static char buffer[32];
        snprintf(buffer, 32, "%i", WSAGetLastError());
        return buffer;
    }
#define CAIPS_GET_ERROR \
    caips_get_error()
#else
#define IFF_UP_RUNNING_FLAGS  (IFF_UP|IFF_RUNNING)

#define CAIPS_GET_ERROR \
    strerror(errno)
#endif
static CAIPErrorHandleCallback g_ipErrorHandler = NULL;

static CAIPPacketReceivedCallback g_packetReceivedCallback = NULL;

static void CAFindReadyMessage();
#if !defined(WSA_WAIT_EVENT_0)
static void CASelectReturned(fd_set *readFds, int ret);
#else
static void CAEventReturned(CASocketFd_t socket);
#endif

static CAResult_t CAReceiveMessage(CASocketFd_t fd, CATransportFlags_t flags);

static void CACloseFDs()
{
#ifndef __TIZENRT__
#if !defined(WSA_WAIT_EVENT_0)
    if (caglobals.ip.shutdownFds[0] != -1)
    {
        close(caglobals.ip.shutdownFds[0]);
        caglobals.ip.shutdownFds[0] = -1;
    }
#endif
#endif
    CADeInitializeIPGlobals();
}

static void CAReceiveHandler(void *data)
{
    (void)data;

    while (!caglobals.ip.terminate)
    {
        CAFindReadyMessage();
    }
    CACloseFDs();
}

#define CLOSE_SOCKET(TYPE) \
    if (caglobals.ip.TYPE.fd != OC_INVALID_SOCKET) \
    { \
        OC_CLOSE_SOCKET(caglobals.ip.TYPE.fd); \
        caglobals.ip.TYPE.fd = OC_INVALID_SOCKET; \
    }

#if !defined(WSA_WAIT_EVENT_0)

#define SET(TYPE, FDS) \
    if (caglobals.ip.TYPE.fd != OC_INVALID_SOCKET) \
    { \
        FD_SET(caglobals.ip.TYPE.fd, FDS); \
    }

#define ISSET(TYPE, FDS, FLAGS) \
    if (caglobals.ip.TYPE.fd != OC_INVALID_SOCKET && FD_ISSET(caglobals.ip.TYPE.fd, FDS)) \
    { \
        fd = caglobals.ip.TYPE.fd; \
        flags = FLAGS; \
    }


static void CAFindReadyMessage()
{
    fd_set readFds;
    struct timeval timeout;

    timeout.tv_sec = caglobals.ip.selectTimeout;
    timeout.tv_usec = 0;
    struct timeval *tv = caglobals.ip.selectTimeout == -1 ? NULL : &timeout;

    FD_ZERO(&readFds);
    SET(u6,  &readFds)
    SET(u6s, &readFds)
    SET(u4,  &readFds)
    SET(u4s, &readFds)
    SET(m6,  &readFds)
    SET(m6s, &readFds)
    SET(m4,  &readFds)
    SET(m4s, &readFds)

#ifndef __TIZENRT__
    if (caglobals.ip.shutdownFds[0] != -1)
    {
        FD_SET(caglobals.ip.shutdownFds[0], &readFds);
    }
#endif
    if (caglobals.ip.netlinkFd != OC_INVALID_SOCKET)
    {
        FD_SET(caglobals.ip.netlinkFd, &readFds);
    }

    int ret = select(caglobals.ip.maxfd + 1, &readFds, NULL, NULL, tv);

    if (caglobals.ip.terminate)
    {
        OIC_LOG_V(DEBUG, TAG, "Packet receiver Stop request received.");
        return;
    }

#ifdef __TIZENRT__
    u_arraylist_t *iflist = CAFindInterfaceChange();
    if (iflist)
    {
        uint32_t listLength = u_arraylist_length(iflist);
        for (uint32_t i = 0; i < listLength; i++)
        {
            CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
            if (ifitem)
            {
                CAProcessNewInterface(ifitem);
            }
        }
        u_arraylist_destroy(iflist);
    }
#endif
    if (0 == ret)
    {
        return;
    }
    else if (0 < ret)
    {
        CASelectReturned(&readFds, ret);
    }
    else // if (0 > ret)
    {
        OIC_LOG_V(FATAL, TAG, "select error %s", CAIPS_GET_ERROR);
        return;
    }
}

static void CASelectReturned(fd_set *readFds, int ret)
{
    (void)ret;
    CASocketFd_t fd = OC_INVALID_SOCKET;
    CATransportFlags_t flags = CA_DEFAULT_FLAGS;

    while (!caglobals.ip.terminate)
    {
        ISSET(u6,  readFds, CA_IPV6)
        else ISSET(u6s, readFds, CA_IPV6 | CA_SECURE)
        else ISSET(u4,  readFds, CA_IPV4)
        else ISSET(u4s, readFds, CA_IPV4 | CA_SECURE)
        else ISSET(m6,  readFds, CA_MULTICAST | CA_IPV6)
        else ISSET(m6s, readFds, CA_MULTICAST | CA_IPV6 | CA_SECURE)
        else ISSET(m4,  readFds, CA_MULTICAST | CA_IPV4)
        else ISSET(m4s, readFds, CA_MULTICAST | CA_IPV4 | CA_SECURE)
        else if ((caglobals.ip.netlinkFd != OC_INVALID_SOCKET) && FD_ISSET(caglobals.ip.netlinkFd, readFds))
        {
#ifndef __TIZENRT__
            OIC_LOG_V(DEBUG, TAG, "Netlink event detacted");
            u_arraylist_t *iflist = CAFindInterfaceChange();
            if (iflist)
            {
                size_t listLength = u_arraylist_length(iflist);
                for (size_t i = 0; i < listLength; i++)
                {
                    CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
                    if (ifitem)
                    {
                        CAProcessNewInterface(ifitem);
                    }
                }
                u_arraylist_destroy(iflist);
            }
            break;
#endif
        }
#ifndef __TIZENRT__
        else if (FD_ISSET(caglobals.ip.shutdownFds[0], readFds))
        {
            char buf[10] = {0};
            ssize_t len = read(caglobals.ip.shutdownFds[0], buf, sizeof (buf));
            if (-1 == len)
            {
                continue;
            }
            break;
        }
#endif
        else
        {
            break;
        }
        (void)CAReceiveMessage(fd, flags);
        FD_CLR(fd, readFds);
    }
}

#else // if defined(WSA_WAIT_EVENT_0)

#define PUSH_HANDLE(HANDLE, ARRAY, INDEX) \
{ \
    ARRAY[INDEX] = HANDLE; \
    INDEX++; \
}

// Create WSAEvent for SOCKET and push the new event into ARRAY
#define PUSH_SOCKET(SOCKET, ARRAY, INDEX) \
    if (SOCKET != OC_INVALID_SOCKET) \
    { \
        WSAEVENT NewEvent = WSACreateEvent(); \
        if (WSA_INVALID_EVENT != NewEvent) \
        { \
            if (0 != WSAEventSelect(SOCKET, NewEvent, FD_READ)) \
            { \
                OIC_LOG_V(ERROR, TAG, "WSAEventSelect failed %d", WSAGetLastError()); \
                BOOL closed = WSACloseEvent(NewEvent); \
                assert(closed); \
                if (!closed) \
                { \
                    OIC_LOG_V(ERROR, TAG, "WSACloseEvent(NewEvent) failed %d", WSAGetLastError()); \
                } \
            } \
            else \
            { \
                PUSH_HANDLE(NewEvent, ARRAY, INDEX); \
            } \
        } \
        else \
        { \
            OIC_LOG_V(ERROR, TAG, "WSACreateEvent failed %d", WSAGetLastError()); \
        }\
    }

#define INSERT_SOCKET(FD, ARRAY, INDEX) \
    { \
        if (OC_INVALID_SOCKET != FD) \
        { \
            ARRAY[INDEX] = FD; \
        } \
    }


// Inserts the socket into the SOCKET_ARRAY and pushes the socket event into EVENT_ARRAY
#define PUSH_IP_SOCKET(TYPE, EVENT_ARRAY, SOCKET_ARRAY, INDEX) \
    { \
        if (OC_INVALID_SOCKET != caglobals.ip.TYPE.fd) \
        { \
            INSERT_SOCKET(caglobals.ip.TYPE.fd, SOCKET_ARRAY, INDEX); \
            PUSH_SOCKET(caglobals.ip.TYPE.fd, EVENT_ARRAY, INDEX); \
        } \
    }

#define IS_MATCHING_IP_SOCKET(TYPE, SOCKET, FLAGS) \
    if ((caglobals.ip.TYPE.fd != OC_INVALID_SOCKET) && (caglobals.ip.TYPE.fd == SOCKET)) \
    { \
        fd = caglobals.ip.TYPE.fd; \
        flags = FLAGS; \
    }

#define EVENT_ARRAY_SIZE  10

static void CAFindReadyMessage()
{
    CASocketFd_t socketArray[EVENT_ARRAY_SIZE];
    HANDLE eventArray[EVENT_ARRAY_SIZE];
    DWORD arraySize = 0;
    DWORD eventIndex;

    // socketArray and eventArray should have same number of elements
    OC_STATIC_ASSERT(_countof(socketArray) == _countof(eventArray), "Arrays should have same number of elements");
    OC_STATIC_ASSERT(_countof(eventArray) <= WSA_MAXIMUM_WAIT_EVENTS, "Too many events for a single Wait");

    PUSH_IP_SOCKET(u6,  eventArray, socketArray, arraySize);
    PUSH_IP_SOCKET(u6s, eventArray, socketArray, arraySize);
    PUSH_IP_SOCKET(u4,  eventArray, socketArray, arraySize);
    PUSH_IP_SOCKET(u4s, eventArray, socketArray, arraySize);
    PUSH_IP_SOCKET(m6,  eventArray, socketArray, arraySize);
    PUSH_IP_SOCKET(m6s, eventArray, socketArray, arraySize);
    PUSH_IP_SOCKET(m4,  eventArray, socketArray, arraySize);
    PUSH_IP_SOCKET(m4s, eventArray, socketArray, arraySize);

    if (WSA_INVALID_EVENT != caglobals.ip.shutdownEvent)
    {
        INSERT_SOCKET(OC_INVALID_SOCKET, socketArray, arraySize);
        PUSH_HANDLE(caglobals.ip.shutdownEvent, eventArray, arraySize);
    }

    if (WSA_INVALID_EVENT != caglobals.ip.addressChangeEvent)
    {
        INSERT_SOCKET(OC_INVALID_SOCKET, socketArray, arraySize);
        PUSH_HANDLE(caglobals.ip.addressChangeEvent, eventArray, arraySize);
    }

    // Should not have overflowed buffer
    assert(arraySize <= (_countof(socketArray)));

    // Timeout is unnecessary on Windows
    assert(-1 == caglobals.ip.selectTimeout);

    while (!caglobals.ip.terminate)
    {
        DWORD ret = WSAWaitForMultipleEvents(arraySize, eventArray, FALSE, WSA_INFINITE, FALSE);
        assert(ret >= WSA_WAIT_EVENT_0);
        assert(ret < (WSA_WAIT_EVENT_0 + arraySize));

        switch (ret)
        {
            case WSA_WAIT_FAILED:
                OIC_LOG_V(ERROR, TAG, "WSAWaitForMultipleEvents returned WSA_WAIT_FAILED %d", WSAGetLastError());
                break;
            case WSA_WAIT_IO_COMPLETION:
                OIC_LOG_V(ERROR, TAG, "WSAWaitForMultipleEvents returned WSA_WAIT_IO_COMPLETION %d", WSAGetLastError());
                break;
            case WSA_WAIT_TIMEOUT:
                OIC_LOG_V(ERROR, TAG, "WSAWaitForMultipleEvents returned WSA_WAIT_TIMEOUT %d", WSAGetLastError());
                break;
            default:
                eventIndex = ret - WSA_WAIT_EVENT_0;
                if ((eventIndex >= 0) && (eventIndex < arraySize))
                {
                    if (false == WSAResetEvent(eventArray[eventIndex]))
                    {
                        OIC_LOG_V(ERROR, TAG, "WSAResetEvent failed %d", WSAGetLastError());
                    }

                    // Handle address changes if addressChangeEvent is triggered.
                    if ((caglobals.ip.addressChangeEvent != WSA_INVALID_EVENT) &&
                        (caglobals.ip.addressChangeEvent == eventArray[eventIndex]))
                    {
                        u_arraylist_t *iflist = CAFindInterfaceChange();
                        if (iflist)
                        {
                            size_t listLength = u_arraylist_length(iflist);
                            for (size_t i = 0; i < listLength; i++)
                            {
                                CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
                                if (ifitem)
                                {
                                    CAProcessNewInterface(ifitem);
                                }
                            }
                            u_arraylist_destroy(iflist);
                        }
                        break;
                    }

                    // Break out if shutdownEvent is triggered.
                    if ((caglobals.ip.shutdownEvent != WSA_INVALID_EVENT) &&
                        (caglobals.ip.shutdownEvent == eventArray[eventIndex]))
                    {
                        break;
                    }
                    CAEventReturned(socketArray[eventIndex]);
                }
                else
                {
                    OIC_LOG_V(ERROR, TAG, "WSAWaitForMultipleEvents failed %d", WSAGetLastError());
                }
                break;
        }

    }

    for (size_t i = 0; i < arraySize; i++)
    {
        HANDLE h = eventArray[i];
        if (h != caglobals.ip.addressChangeEvent)
        {
            BOOL closed = WSACloseEvent(h);
            assert(closed);
            if (!closed)
            {
                OIC_LOG_V(ERROR, TAG, "WSACloseEvent (Index %i) failed %d", i, WSAGetLastError());
            }
        }
    }

    if (caglobals.ip.terminate)
    {
        caglobals.ip.shutdownEvent = WSA_INVALID_EVENT;
    }
}

static void CAEventReturned(CASocketFd_t socket)
{
    CASocketFd_t fd = OC_INVALID_SOCKET;
    CATransportFlags_t flags = CA_DEFAULT_FLAGS;

    while (!caglobals.ip.terminate)
    {
        IS_MATCHING_IP_SOCKET(u6,  socket, CA_IPV6)
        else IS_MATCHING_IP_SOCKET(u6s, socket, CA_IPV6 | CA_SECURE)
        else IS_MATCHING_IP_SOCKET(u4,  socket, CA_IPV4)
        else IS_MATCHING_IP_SOCKET(u4s, socket, CA_IPV4 | CA_SECURE)
        else IS_MATCHING_IP_SOCKET(m6,  socket, CA_MULTICAST | CA_IPV6)
        else IS_MATCHING_IP_SOCKET(m6s, socket, CA_MULTICAST | CA_IPV6 | CA_SECURE)
        else IS_MATCHING_IP_SOCKET(m4,  socket, CA_MULTICAST | CA_IPV4)
        else IS_MATCHING_IP_SOCKET(m4s, socket, CA_MULTICAST | CA_IPV4 | CA_SECURE)
        else
        {
            break;
        }
        (void)CAReceiveMessage(socket, flags);
        // We will never get more than one match per socket, so always break.
        break;
    }
}

#endif

void CAUnregisterForAddressChanges()
{
#ifdef _WIN32
    if (caglobals.ip.addressChangeEvent != WSA_INVALID_EVENT)
    {
        OC_VERIFY(WSACloseEvent(caglobals.ip.addressChangeEvent));
        caglobals.ip.addressChangeEvent = WSA_INVALID_EVENT;
    }
#else
    if (caglobals.ip.netlinkFd != OC_INVALID_SOCKET)
    {
        close(caglobals.ip.netlinkFd);
        caglobals.ip.netlinkFd = OC_INVALID_SOCKET;
    }
#endif
}

void CADeInitializeIPGlobals()
{
    CLOSE_SOCKET(u6);
    CLOSE_SOCKET(u6s);
    CLOSE_SOCKET(u4);
    CLOSE_SOCKET(u4s);
    CLOSE_SOCKET(m6);
    CLOSE_SOCKET(m6s);
    CLOSE_SOCKET(m4);
    CLOSE_SOCKET(m4s);

    CAUnregisterForAddressChanges();
}

static CAResult_t CAReceiveMessage(CASocketFd_t fd, CATransportFlags_t flags)
{
    char recvBuffer[RECV_MSG_BUF_LEN] = {0};
    int level = 0;
    int type = 0;
    int namelen = 0;
    struct sockaddr_storage srcAddr = { .ss_family = 0 };
    unsigned char *pktinfo = NULL;
#if !defined(WSA_CMSG_DATA)
    size_t len = 0;
    struct cmsghdr *cmp = NULL;
    struct iovec iov = { .iov_base = recvBuffer, .iov_len = sizeof (recvBuffer) };
    union control
    {
        struct cmsghdr cmsg;
        unsigned char data[CMSG_SPACE(sizeof (struct in6_pktinfo))];
    } cmsg;

#ifndef __TIZENRT__ /* temporarilly disabled IPv6 */
    if (flags & CA_IPV6)
    {
        namelen = sizeof (struct sockaddr_in6);
        level = IPPROTO_IPV6;
        type = IPV6_PKTINFO;
        len = sizeof (struct in6_pktinfo);
    }
    else
#endif
    {
        namelen = sizeof (struct sockaddr_in);
        level = IPPROTO_IP;
        type = IP_PKTINFO;
        len = sizeof (struct in6_pktinfo);
    }

    struct msghdr msg = { .msg_name = &srcAddr,
                          .msg_namelen = namelen,
                          .msg_iov = &iov,
                          .msg_iovlen = 1,
                          .msg_control = &cmsg,
                          .msg_controllen = CMSG_SPACE(len) };

    ssize_t recvLen = recvmsg(fd, &msg, flags);
    if (OC_SOCKET_ERROR == recvLen)
    {
        OIC_LOG_V(ERROR, TAG, "Recvfrom failed %s", strerror(errno));
        return CA_STATUS_FAILED;
    }

#ifdef __TIZENRT__
    if (flags & CA_MULTICAST)
    {
#endif
        for (cmp = CMSG_FIRSTHDR(&msg); cmp != NULL; cmp = CMSG_NXTHDR(&msg, cmp))
        {
            if (cmp->cmsg_level == level && cmp->cmsg_type == type)
            {
                pktinfo = CMSG_DATA(cmp);
            }
        }
#ifdef __TIZENRT__
    }
#endif
#else // if defined(WSA_CMSG_DATA)
    union control
    {
        WSACMSGHDR cmsg;
        uint8_t data[WSA_CMSG_SPACE(sizeof (IN6_PKTINFO))];
    } cmsg;
    memset(&cmsg, 0, sizeof(cmsg));

    if (flags & CA_IPV6)
    {
        namelen  = sizeof (struct sockaddr_in6);
        level = IPPROTO_IPV6;
        type = IPV6_PKTINFO;
    }
    else
    {
        namelen = sizeof (struct sockaddr_in);
        level = IPPROTO_IP;
        type = IP_PKTINFO;
    }

    WSABUF iov = {.len = sizeof (recvBuffer), .buf = recvBuffer};
    WSAMSG msg = {.name = (PSOCKADDR)&srcAddr,
                  .namelen = namelen,
                  .lpBuffers = &iov,
                  .dwBufferCount = 1,
                  .Control = {.buf = (char*)cmsg.data, .len = sizeof (cmsg)}
                 };

    uint32_t recvLen = 0;
    uint32_t ret = caglobals.ip.wsaRecvMsg(fd, &msg, (LPDWORD)&recvLen, 0,0);
    if (OC_SOCKET_ERROR == ret)
    {
        OIC_LOG_V(ERROR, TAG, "WSARecvMsg failed %i", WSAGetLastError());
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, TAG, "WSARecvMsg recvd %u bytes", recvLen);

    for (WSACMSGHDR *cmp = WSA_CMSG_FIRSTHDR(&msg); cmp != NULL;
         cmp = WSA_CMSG_NXTHDR(&msg, cmp))
    {
        if (cmp->cmsg_level == level && cmp->cmsg_type == type)
        {
            pktinfo = WSA_CMSG_DATA(cmp);
        }
    }
#endif // !defined(WSA_CMSG_DATA)

#ifndef __TIZENRT__
    if (!pktinfo)
    {
        OIC_LOG(ERROR, TAG, "pktinfo is null");
        return CA_STATUS_FAILED;
    }
#endif

    CASecureEndpoint_t sep = {.endpoint = {.adapter = CA_ADAPTER_IP, .flags = flags}};

#ifndef __TIZENRT__ /* temporarilly disabled IPv6 */
    if (flags & CA_IPV6)
    {
        sep.endpoint.ifindex = ((struct in6_pktinfo *)pktinfo)->ipi6_ifindex;

        if (flags & CA_MULTICAST)
        {
            struct in6_addr *addr = &(((struct in6_pktinfo *)pktinfo)->ipi6_addr);
            unsigned char topbits = ((unsigned char *)addr)[0];
            if (topbits != 0xff)
            {
                sep.endpoint.flags &= ~CA_MULTICAST;
            }
        }
    }
    else
#endif
    {
#ifndef __TIZENRT__
        sep.endpoint.ifindex = ((struct in_pktinfo *)pktinfo)->ipi_ifindex;
#else
        if((struct in_pktinfo *)pktinfo){
            sep.endpoint.ifindex = ((struct in_pktinfo *)pktinfo)->ipi_ifindex;
	}
#endif

#ifndef __TIZENRT__
        if (flags & CA_MULTICAST)
#else
        if ((flags & CA_MULTICAST) && pktinfo)
#endif
        {
            struct in_addr *addr = &((struct in_pktinfo *)pktinfo)->ipi_addr;
            uint32_t host = ntohl(addr->s_addr);
            unsigned char topbits = ((unsigned char *)&host)[3];
            if (topbits < 224 || topbits > 239)
            {
                sep.endpoint.flags &= ~CA_MULTICAST;
            }
        }
    }

    CAConvertAddrToName(&srcAddr, namelen, sep.endpoint.addr, &sep.endpoint.port);

    if (flags & CA_SECURE)
    {
#ifdef __WITH_DTLS__
#ifdef TB_LOG
        int decryptResult =
#endif
        CAdecryptSsl(&sep, (uint8_t *)recvBuffer, recvLen);
        OIC_LOG_V(DEBUG, TAG, "CAdecryptSsl returns [%d]", decryptResult);
#else
        OIC_LOG(ERROR, TAG, "Encrypted message but no DTLS");
#endif // __WITH_DTLS__
    }
    else
    {
        if (g_packetReceivedCallback)
        {
            g_packetReceivedCallback(&sep, recvBuffer, recvLen);
        }
    }

    return CA_STATUS_OK;
}

void CAIPPullData()
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
}

static CASocketFd_t CACreateSocket(int family, uint16_t *port, bool isMulticast)
{
    int socktype = SOCK_DGRAM;
#ifdef SOCK_CLOEXEC
    socktype |= SOCK_CLOEXEC;
#endif
    CASocketFd_t fd = socket(family, socktype, IPPROTO_UDP);
    if (OC_INVALID_SOCKET == fd)
    {
        OIC_LOG_V(ERROR, TAG, "create socket failed: %s", CAIPS_GET_ERROR);
        return OC_INVALID_SOCKET;
    }

#if !defined(SOCK_CLOEXEC) && defined(FD_CLOEXEC)
    int fl = fcntl(fd, F_GETFD);
    if (-1 == fl || -1 == fcntl(fd, F_SETFD, fl|FD_CLOEXEC))
    {
        OIC_LOG_V(ERROR, TAG, "set FD_CLOEXEC failed: %s", strerror(errno));
        close(fd);
        return OC_INVALID_SOCKET;
    }
#endif
    struct sockaddr_storage sa = { .ss_family = (short)family };
    socklen_t socklen = 0;

#ifndef __TIZENRT__ /* temporarilly disabled IPv6 */
    if (family == AF_INET6)
    {
        int on = 1;

        if (OC_SOCKET_ERROR == setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, OPTVAL_T(&on), sizeof (on)))
        {
            OIC_LOG_V(ERROR, TAG, "IPV6_V6ONLY failed: %s", CAIPS_GET_ERROR);
        }

#if defined(IPV6_RECVPKTINFO)
        if (OC_SOCKET_ERROR == setsockopt(fd, IPPROTO_IPV6, IPV6_RECVPKTINFO, &on, sizeof (on)))
#else
        if (OC_SOCKET_ERROR == setsockopt(fd, IPPROTO_IPV6, IPV6_PKTINFO, OPTVAL_T(&on), sizeof (on)))
#endif
        {
            OIC_LOG_V(ERROR, TAG, "IPV6_RECVPKTINFO failed: %s",CAIPS_GET_ERROR);
        }

        ((struct sockaddr_in6 *)&sa)->sin6_port = htons(*port);
        socklen = sizeof (struct sockaddr_in6);
    }
    else
#endif
    {
        int on = 1;
        if (OC_SOCKET_ERROR == setsockopt(fd, IPPROTO_IP, IP_PKTINFO, OPTVAL_T(&on), sizeof (on)))
        {
            OIC_LOG_V(ERROR, TAG, "IP_PKTINFO failed: %s", CAIPS_GET_ERROR);
        }

        ((struct sockaddr_in *)&sa)->sin_port = htons(*port);
        socklen = sizeof (struct sockaddr_in);
    }

    if (isMulticast && *port) // use the given port
    {
        int on = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, OPTVAL_T(&on), sizeof (on)))
        {
            OIC_LOG_V(ERROR, TAG, "SO_REUSEADDR failed: %s", CAIPS_GET_ERROR);
            OC_CLOSE_SOCKET(fd);
            return OC_INVALID_SOCKET;
        }
    }

    if (OC_SOCKET_ERROR == bind(fd, (struct sockaddr *)&sa, socklen))
    {
        OIC_LOG_V(ERROR, TAG, "bind socket failed: %s", CAIPS_GET_ERROR);
        OC_CLOSE_SOCKET(fd);
        return OC_INVALID_SOCKET;
    }

    if (!*port) // return the assigned port
    {
        if (OC_SOCKET_ERROR == getsockname(fd, (struct sockaddr *)&sa, &socklen))
        {
            OIC_LOG_V(ERROR, TAG, "getsockname failed: %s", CAIPS_GET_ERROR);
            OC_CLOSE_SOCKET(fd);
            return OC_INVALID_SOCKET;
        }
        *port = ntohs(family == AF_INET6 ?
                      ((struct sockaddr_in6 *)&sa)->sin6_port :
                      ((struct sockaddr_in *)&sa)->sin_port);
    }

    return fd;
}

#ifdef _WIN32
#define CHECKFD(FD)
#else
#define CHECKFD(FD) \
do \
{ \
    if (FD > caglobals.ip.maxfd) \
    { \
        caglobals.ip.maxfd = FD; \
    } \
} while (0)
#endif
#define NEWSOCKET(FAMILY, NAME, MULTICAST) \
do \
{ \
    caglobals.ip.NAME.fd = CACreateSocket(FAMILY, &caglobals.ip.NAME.port, MULTICAST); \
    if (caglobals.ip.NAME.fd == OC_INVALID_SOCKET) \
    {   \
        caglobals.ip.NAME.port = 0; \
        caglobals.ip.NAME.fd = CACreateSocket(FAMILY, &caglobals.ip.NAME.port, MULTICAST); \
    }   \
    CHECKFD(caglobals.ip.NAME.fd); \
} while(0)

static void CARegisterForAddressChanges()
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);
#ifdef _WIN32
    caglobals.ip.addressChangeEvent = WSACreateEvent();
    if (WSA_INVALID_EVENT == caglobals.ip.addressChangeEvent)
    {
        OIC_LOG(ERROR, TAG, "WSACreateEvent failed");
    }
#else
    caglobals.ip.netlinkFd = OC_INVALID_SOCKET;
#ifdef __linux__
    // create NETLINK fd for interface change notifications
    struct sockaddr_nl sa = { AF_NETLINK, 0, 0,
                              RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR };

    caglobals.ip.netlinkFd = socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_ROUTE);
    if (caglobals.ip.netlinkFd == OC_INVALID_SOCKET)
    {
        OIC_LOG_V(ERROR, TAG, "netlink socket failed: %s", strerror(errno));
    }
    else
    {
        int r = bind(caglobals.ip.netlinkFd, (struct sockaddr *)&sa, sizeof (sa));
        if (r)
        {
            OIC_LOG_V(ERROR, TAG, "netlink bind failed: %s", strerror(errno));
            close(caglobals.ip.netlinkFd);
            caglobals.ip.netlinkFd = OC_INVALID_SOCKET;
        }
        else
        {
            CHECKFD(caglobals.ip.netlinkFd);
        }
    }
#elif defined (__TIZENRT__) // pkmsgq
    struct mq_attr lq_attr;
    lq_attr.mq_maxmsg = 10;
    lq_attr.mq_msgsize = 4;
    lq_attr.mq_flags = 0;
    g_nwevent_mqfd = mq_open("netlink_evtq", O_RDWR | O_NONBLOCK | O_CREAT, 0666, &lq_attr);
    if (g_nwevent_mqfd == (mqd_t) - 1)
    {
        OIC_LOG_V(ERROR, TAG,"RECV mq_open failed\n");
        return ;
    }
#endif
#endif
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
}

static void CAInitializeFastShutdownMechanism()
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);
    caglobals.ip.selectTimeout = -1; // don't poll for shutdown
    int ret = -1;
#if defined(WSA_WAIT_EVENT_0)
    caglobals.ip.shutdownEvent = WSACreateEvent();
    if (WSA_INVALID_EVENT != caglobals.ip.shutdownEvent)
    {
        ret = 0;
    }
#elif defined(HAVE_PIPE2)
#ifndef __TIZENRT__
    ret = pipe2(caglobals.ip.shutdownFds, O_CLOEXEC);
    CHECKFD(caglobals.ip.shutdownFds[0]);
    CHECKFD(caglobals.ip.shutdownFds[1]);
#endif
#else
#ifndef __TIZENRT__
    ret = pipe(caglobals.ip.shutdownFds);
    if (-1 != ret)
    {
        ret = fcntl(caglobals.ip.shutdownFds[0], F_GETFD);
        if (-1 != ret)
        {
            ret = fcntl(caglobals.ip.shutdownFds[0], F_SETFD, ret|FD_CLOEXEC);
        }
        if (-1 != ret)
        {
            ret = fcntl(caglobals.ip.shutdownFds[1], F_GETFD);
        }
        if (-1 != ret)
        {
            ret = fcntl(caglobals.ip.shutdownFds[1], F_SETFD, ret|FD_CLOEXEC);
        }
        if (-1 == ret)
        {
            close(caglobals.ip.shutdownFds[1]);
            close(caglobals.ip.shutdownFds[0]);
            caglobals.ip.shutdownFds[0] = -1;
            caglobals.ip.shutdownFds[1] = -1;
        }
    }
    CHECKFD(caglobals.ip.shutdownFds[0]);
    CHECKFD(caglobals.ip.shutdownFds[1]);
#endif
#endif
    if (-1 == ret)
    {
        OIC_LOG_V(ERROR, TAG, "fast shutdown mechanism init failed: %s", CAIPS_GET_ERROR);
        caglobals.ip.selectTimeout = SELECT_TIMEOUT; //poll needed for shutdown
    }
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
}

CAResult_t CAIPStartServer(const ca_thread_pool_t threadPool)
{
    CAResult_t res = CA_STATUS_OK;

    if (caglobals.ip.started)
    {
        return res;
    }
    if (!IPv4MulticastAddress.s_addr)
    {
        (void)inet_pton(AF_INET, IPv4_MULTICAST, &IPv4MulticastAddress);
        (void)inet_pton(AF_INET6, IPv6_MULTICAST_INT, &IPv6MulticastAddressInt);
        (void)inet_pton(AF_INET6, IPv6_MULTICAST_LNK, &IPv6MulticastAddressLnk);
        (void)inet_pton(AF_INET6, IPv6_MULTICAST_RLM, &IPv6MulticastAddressRlm);
        (void)inet_pton(AF_INET6, IPv6_MULTICAST_ADM, &IPv6MulticastAddressAdm);
        (void)inet_pton(AF_INET6, IPv6_MULTICAST_SIT, &IPv6MulticastAddressSit);
        (void)inet_pton(AF_INET6, IPv6_MULTICAST_ORG, &IPv6MulticastAddressOrg);
        (void)inet_pton(AF_INET6, IPv6_MULTICAST_GLB, &IPv6MulticastAddressGlb);
    }

    if (!caglobals.ip.ipv6enabled && !caglobals.ip.ipv4enabled)
    {
        caglobals.ip.ipv4enabled = true;  // only needed to run CA tests
    }

    if (caglobals.ip.ipv6enabled)
    {
        NEWSOCKET(AF_INET6, u6, false);
        NEWSOCKET(AF_INET6, u6s, false);
        NEWSOCKET(AF_INET6, m6, true);
        NEWSOCKET(AF_INET6, m6s, true);
        OIC_LOG_V(INFO, TAG, "IPv6 unicast port: %u", caglobals.ip.u6.port);
    }
    if (caglobals.ip.ipv4enabled)
    {
        NEWSOCKET(AF_INET, u4, false);
        NEWSOCKET(AF_INET, u4s, false);
        NEWSOCKET(AF_INET, m4, true);
        NEWSOCKET(AF_INET, m4s, true);
        OIC_LOG_V(INFO, TAG, "IPv4 unicast port: %u", caglobals.ip.u4.port);
    }

    OIC_LOG_V(DEBUG, TAG,
              "socket summary: u6=%d, u6s=%d, u4=%d, u4s=%d, m6=%d, m6s=%d, m4=%d, m4s=%d",
              caglobals.ip.u6.fd, caglobals.ip.u6s.fd, caglobals.ip.u4.fd, caglobals.ip.u4s.fd,
              caglobals.ip.m6.fd, caglobals.ip.m6s.fd, caglobals.ip.m4.fd, caglobals.ip.m4s.fd);

    OIC_LOG_V(DEBUG, TAG,
              "port summary: u6 port=%d, u6s port=%d, u4 port=%d, u4s port=%d, m6 port=%d,"
              "m6s port=%d, m4 port=%d, m4s port=%d",
              caglobals.ip.u6.port, caglobals.ip.u6s.port, caglobals.ip.u4.port,
              caglobals.ip.u4s.port, caglobals.ip.m6.port, caglobals.ip.m6s.port,
              caglobals.ip.m4.port, caglobals.ip.m4s.port);
#if defined (SIO_GET_EXTENSION_FUNCTION_POINTER)
    caglobals.ip.wsaRecvMsg = NULL;
    GUID GuidWSARecvMsg = WSAID_WSARECVMSG;
    DWORD copied = 0;
    int err = WSAIoctl(caglobals.ip.u4.fd, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidWSARecvMsg, sizeof(GuidWSARecvMsg), &(caglobals.ip.wsaRecvMsg), sizeof(caglobals.ip.wsaRecvMsg), &copied, 0, 0);
    if (0 != err)
    {
        OIC_LOG_V(ERROR, TAG, "WSAIoctl failed %i", WSAGetLastError());
        return CA_STATUS_FAILED;
    }
#endif
    // set up appropriate FD mechanism for fast shutdown
    CAInitializeFastShutdownMechanism();

    // create source of network address change notifications
    CARegisterForAddressChanges();

    caglobals.ip.selectTimeout = CAGetPollingInterval(caglobals.ip.selectTimeout);

    res = CAIPStartListenServer();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start listening server![%d]", res);
        return res;
    }

    caglobals.ip.terminate = false;
#ifndef __TIZENRT__
    res = ca_thread_pool_add_task(threadPool, CAReceiveHandler, NULL);
#else
    res = ca_thread_pool_add_task(threadPool, CAReceiveHandler, NULL, NULL, "IoT_ReceiveHandler",
                                  CONFIG_IOTIVITY_RECEIVEHANDLER_PTHREAD_STACKSIZE);
#endif
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "thread_pool_add_task failed");
        return res;
    }
    OIC_LOG(DEBUG, TAG, "CAReceiveHandler thread started successfully.");

    caglobals.ip.started = true;
    return CA_STATUS_OK;
}

void CAIPStopServer()
{
    caglobals.ip.terminate = true;
    int err;

#if !defined(WSA_WAIT_EVENT_0)
#ifndef __TIZENRT__
    if (caglobals.ip.shutdownFds[1] != -1)
    {
        close(caglobals.ip.shutdownFds[1]);
        caglobals.ip.shutdownFds[1] = -1;
        // receive thread will stop immediately
    }
    else
    {
        // receive thread will stop in SELECT_TIMEOUT seconds.
    }
#endif
#else
    // receive thread will stop immediately.
    if (!WSASetEvent(caglobals.ip.shutdownEvent))
    {
        OIC_LOG_V(DEBUG, TAG, "set shutdown event failed: %d", WSAGetLastError());
    }
#endif

    if (!caglobals.ip.started)
    { // Close fd's since receive handler was not started
        CACloseFDs();
    }
    caglobals.ip.started = false;
#ifdef __TIZENRT__
    if (g_nwevent_mqfd)
    {
        err = mq_close(g_nwevent_mqfd);
        if (err < 0)
        {
            OIC_LOG_V(ERROR, TAG, "Error to close message queue(%d)", err);
        }
        err = mq_unlink("netlink_evtq");
        if (err < 0)
        {
            OIC_LOG_V(ERROR, TAG, "Error to unlink message queue(%d)", err);
        }
    }
#endif
}

void CAWakeUpForChange()
{
#if !defined(WSA_WAIT_EVENT_0)
#ifndef __TIZENRT__
    if (caglobals.ip.shutdownFds[1] != -1)
    {
        ssize_t len = 0;
        do
        {
            len = write(caglobals.ip.shutdownFds[1], "w", 1);
        } while ((len == -1) && (errno == EINTR));
        if ((len == -1) && (errno != EINTR) && (errno != EPIPE))
        {
            OIC_LOG_V(DEBUG, TAG, "write failed: %s", strerror(errno));
        }
    }
#endif
#else
    if (!WSASetEvent(caglobals.ip.shutdownEvent))
    {
        OIC_LOG_V(DEBUG, TAG, "set shutdown event failed: %d", WSAGetLastError());
    }
#endif
}

static void applyMulticastToInterface4(uint32_t ifindex)
{
    if (!caglobals.ip.ipv4enabled)
    {
        return;
    }

#if defined(USE_IP_MREQN)
    struct ip_mreqn mreq = { .imr_multiaddr = IPv4MulticastAddress,
                             .imr_address.s_addr = htonl(INADDR_ANY),
                             .imr_ifindex = ifindex };
#else
    struct ip_mreq mreq  = { .imr_multiaddr.s_addr = IPv4MulticastAddress.s_addr,
                             .imr_interface.s_addr = htonl(ifindex) };
#endif

    int ret = setsockopt(caglobals.ip.m4.fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, OPTVAL_T(&mreq), sizeof (mreq));
    if (OC_SOCKET_ERROR == ret)
    {
#if !defined(WSAEINVAL)
        if (EADDRINUSE != errno)
#else
        if (WSAEINVAL != WSAGetLastError()) // Joining multicast group more than once (IPv4 Flavor)
#endif
        {
            OIC_LOG_V(ERROR, TAG, "       IPv4 IP_ADD_MEMBERSHIP failed: %s", CAIPS_GET_ERROR);
        }
    }
    ret = setsockopt(caglobals.ip.m4s.fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, OPTVAL_T(&mreq), sizeof (mreq));
    if (OC_SOCKET_ERROR == ret)
    {
#if !defined(WSAEINVAL)
        if (EADDRINUSE != errno)
#else
        if (WSAEINVAL == WSAGetLastError())
        {
            // We're trying to join the multicast group again.
            // If the interface has gone down and come back up, the socket might be in
            // an inconsistent state where it still thinks we're joined when the interface
            // doesn't think we're joined.  So try to leave and rejoin the group just in case.
            setsockopt(caglobals.ip.m4s.fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, OPTVAL_T(&mreq),
                             sizeof(mreq));
            ret = setsockopt(caglobals.ip.m4s.fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, OPTVAL_T(&mreq),
                             sizeof(mreq));
        }
        if (OC_SOCKET_ERROR == ret)
#endif
        {
            OIC_LOG_V(ERROR, TAG, "SECURE IPv4 IP_ADD_MEMBERSHIP failed: %s", CAIPS_GET_ERROR);
        }
    }
}

static void applyMulticast6(CASocketFd_t fd, struct in6_addr *addr, uint32_t ifindex)
{
#ifndef __TIZENRT__ /* temporarilly disabled IPv6 */
    struct ipv6_mreq mreq = { .ipv6mr_interface = ifindex };

    // VS2013 has problems with struct copies inside struct initializers, so copy separately.
    mreq.ipv6mr_multiaddr = *addr;

    int ret = setsockopt(fd, IPPROTO_IPV6, IPV6_JOIN_GROUP, OPTVAL_T(&mreq), sizeof (mreq));
    if (OC_SOCKET_ERROR == ret)
    {
#if !defined(_WIN32)
        if (EADDRINUSE != errno)
#else
        if (WSAEINVAL == WSAGetLastError())
        {
            // We're trying to join the multicast group again.
            // If the interface has gone down and come back up, the socket might be in
            // an inconsistent state where it still thinks we're joined when the interface
            // doesn't think we're joined.  So try to leave and rejoin the group just in case.
            setsockopt(fd, IPPROTO_IPV6, IPV6_LEAVE_GROUP, OPTVAL_T(&mreq), sizeof(mreq));
            ret = setsockopt(fd, IPPROTO_IPV6, IPV6_JOIN_GROUP, OPTVAL_T(&mreq), sizeof(mreq));
        }
        if (OC_SOCKET_ERROR == ret)
#endif
        {
            OIC_LOG_V(ERROR, TAG, "IPv6 IPV6_JOIN_GROUP failed: %s", CAIPS_GET_ERROR);
        }
    }
#endif
}

static void applyMulticastToInterface6(uint32_t ifindex)
{
    if (!caglobals.ip.ipv6enabled)
    {
        return;
    }
    //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressInt, ifindex);
    applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressLnk, ifindex);
    applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressRlm, ifindex);
    //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressAdm, ifindex);
    applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressSit, ifindex);
    //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressOrg, ifindex);
    //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressGlb, ifindex);

    //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressInt, ifindex);
    applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressLnk, ifindex);
    applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressRlm, ifindex);
    //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressAdm, ifindex);
    applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressSit, ifindex);
    //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressOrg, ifindex);
    //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressGlb, ifindex);
}

CAResult_t CAIPStartListenServer()
{
    if (caglobals.ip.started)
    {
        OIC_LOG(DEBUG, TAG, "Adapter is started already");
        return CA_STATUS_OK;
    }

    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);
    u_arraylist_t *iflist = CAIPGetInterfaceInformation(0);
    if (!iflist)
    {
        OIC_LOG_V(ERROR, TAG, "CAIPGetInterfaceInformation() failed: %s", strerror(errno));
        return CA_STATUS_FAILED;
    }

    size_t len = u_arraylist_length(iflist);
    OIC_LOG_V(DEBUG, TAG, "IP network interfaces found: %" PRIuPTR, len);

    for (size_t i = 0; i < len; i++)
    {
        CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);

        if (!ifitem)
        {
            continue;
        }
        if ((ifitem->flags & IFF_UP_RUNNING_FLAGS) != IFF_UP_RUNNING_FLAGS)
        {
            continue;
        }
        if (ifitem->family == AF_INET)
        {
            OIC_LOG_V(DEBUG, TAG, "Adding IPv4 interface(%i) to multicast group", ifitem->index);
            applyMulticastToInterface4(ifitem->index);
        }
        if (ifitem->family == AF_INET6)
        {
            OIC_LOG_V(DEBUG, TAG, "Adding IPv6 interface(%i) to multicast group", ifitem->index);
            applyMulticastToInterface6(ifitem->index);
        }
    }

    u_arraylist_destroy(iflist);
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
    return CA_STATUS_OK;
}

CAResult_t CAIPStopListenServer()
{
    u_arraylist_t *iflist = CAIPGetInterfaceInformation(0);
    if (!iflist)
    {
        OIC_LOG_V(ERROR, TAG, "Get interface info failed: %s", strerror(errno));
        return CA_STATUS_FAILED;
    }

    size_t len = u_arraylist_length(iflist);
    OIC_LOG_V(DEBUG, TAG, "IP network interfaces found: %" PRIuPTR, len);

    for (size_t i = 0; i < len; i++)
    {
        CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);

        if (!ifitem)
        {
            continue;
        }
        if ((ifitem->flags & IFF_UP_RUNNING_FLAGS) != IFF_UP_RUNNING_FLAGS)
        {
            continue;
        }
        if (ifitem->family == AF_INET)
        {
            CLOSE_SOCKET(m4);
            CLOSE_SOCKET(m4s);
            OIC_LOG_V(DEBUG, TAG, "IPv4 network interface: %s cloed", ifitem->name);
        }
        if (ifitem->family == AF_INET6)
        {
            CLOSE_SOCKET(m6);
            CLOSE_SOCKET(m6s);
            OIC_LOG_V(DEBUG, TAG, "IPv6 network interface: %s", ifitem->name);
        }
    }
    u_arraylist_destroy(iflist);
    return CA_STATUS_OK;
}

void CAProcessNewInterface(CAInterface_t *ifitem)
{
    if (!ifitem)
    {
        OIC_LOG(DEBUG, TAG, "ifitem is null");
        return;
    }

    if (ifitem->family == AF_INET6)
    {
        OIC_LOG_V(DEBUG, TAG, "Adding a new IPv6 interface(%i) to multicast group", ifitem->index);
        applyMulticastToInterface6(ifitem->index);
    }
    if (ifitem->family == AF_INET)
    {
        OIC_LOG_V(DEBUG, TAG, "Adding a new IPv4 interface(%i) to multicast group", ifitem->index);
        applyMulticastToInterface4(ifitem->index);
    }
}

void CAIPSetPacketReceiveCallback(CAIPPacketReceivedCallback callback)
{
    g_packetReceivedCallback = callback;
}

static void sendData(CASocketFd_t fd, const CAEndpoint_t *endpoint,
                     const void *data, size_t dlen,
                     const char *cast, const char *fam)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    if (!endpoint)
    {
        OIC_LOG(DEBUG, TAG, "endpoint is null");
        if (g_ipErrorHandler)
        {
            g_ipErrorHandler(endpoint, data, dlen, CA_STATUS_INVALID_PARAM);
        }
        return;
    }

    (void)cast;  // eliminates release warning
    (void)fam;

    struct sockaddr_storage sock = { .ss_family = 0 };
    CAConvertNameToAddr(endpoint->addr, endpoint->port, &sock);

    socklen_t socklen = 0;
    if (sock.ss_family == AF_INET6)
    {
        socklen = sizeof(struct sockaddr_in6);
    }
    else
    {
        socklen = sizeof(struct sockaddr_in);
    }

#ifdef TB_LOG
    const char *secure = (endpoint->flags & CA_SECURE) ? "secure " : "";
#endif
#if !defined(_WIN32)
    ssize_t len = sendto(fd, data, dlen, 0, (struct sockaddr *)&sock, socklen);
    if (OC_SOCKET_ERROR == len)
    {
         // If logging is not defined/enabled.
        if (g_ipErrorHandler)
        {
            g_ipErrorHandler(endpoint, data, dlen, CA_SEND_FAILED);
        }
        OIC_LOG_V(ERROR, TAG, "%s%s %s sendTo failed: %s", secure, cast, fam, strerror(errno));
        CALogSendStateInfo(endpoint->adapter, endpoint->addr, endpoint->port,
                           len, false, strerror(errno));
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "%s%s %s sendTo is successful: %zd bytes", secure, cast, fam, len);
        CALogSendStateInfo(endpoint->adapter, endpoint->addr, endpoint->port,
                           len, true, NULL);
    }
#else
    int err = 0;
    int len = 0;
    size_t sent = 0;
    do {
        int dataToSend = ((dlen - sent) > INT_MAX) ? INT_MAX : (int)(dlen - sent);
        len = sendto(fd, ((char*)data) + sent, dataToSend, 0, (struct sockaddr *)&sock, socklen);
        if (OC_SOCKET_ERROR == len)
        {
            err = WSAGetLastError();
            if ((WSAEWOULDBLOCK != err) && (WSAENOBUFS != err))
            {
                 // If logging is not defined/enabled.
                if (g_ipErrorHandler)
                {
                    g_ipErrorHandler(endpoint, data, dlen, CA_SEND_FAILED);
                }

                OIC_LOG_V(ERROR, TAG, "%s%s %s sendTo failed: %i", secure, cast, fam, err);
            }
        }
        else
        {
            sent += len;
            if (sent != (size_t)len)
            {
                OIC_LOG_V(DEBUG, TAG, "%s%s %s sendTo (Partial Send) is successful: "
                                      "currently sent: %ld bytes, "
                                      "total sent: %" PRIuPTR " bytes, "
                                      "remaining: %" PRIuPTR " bytes",
                                      secure, cast, fam, len, sent, (dlen - sent));
            }
            else
            {
                OIC_LOG_V(INFO, TAG, "%s%s %s sendTo is successful: %ld bytes",
                                     secure, cast, fam, len);
            }
        }
    } while ((OC_SOCKET_ERROR == len) && ((WSAEWOULDBLOCK == err) || (WSAENOBUFS == err)) || (sent < dlen));
#endif
}

static void sendMulticastData6(const u_arraylist_t *iflist,
                               CAEndpoint_t *endpoint,
                               const void *data, size_t datalen)
{
#ifndef __TIZENRT__ /* temporarilly disabled IPv6 */
    if (!endpoint)
    {
        OIC_LOG(DEBUG, TAG, "endpoint is null");
        return;
    }

    int scope = endpoint->flags & CA_SCOPE_MASK;
    char *ipv6mcname = ipv6mcnames[scope];
    if (!ipv6mcname)
    {
        OIC_LOG_V(INFO, TAG, "IPv6 multicast scope invalid: %d", scope);
        return;
    }
    OICStrcpy(endpoint->addr, sizeof(endpoint->addr), ipv6mcname);
    CASocketFd_t fd = caglobals.ip.u6.fd;

    size_t len = u_arraylist_length(iflist);
    for (size_t i = 0; i < len; i++)
    {
        CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
        if (!ifitem)
        {
            continue;
        }
        if ((ifitem->flags & IFF_UP_RUNNING_FLAGS) != IFF_UP_RUNNING_FLAGS)
        {
            continue;
        }
        if (ifitem->family != AF_INET6)
        {
            continue;
        }

        int index = ifitem->index;
        if (setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, OPTVAL_T(&index), sizeof (index)))
        {
            OIC_LOG_V(ERROR, TAG, "setsockopt6 failed: %s", CAIPS_GET_ERROR);
            return;
        }
        sendData(fd, endpoint, data, datalen, "multicast", "ipv6");
    }
#endif
}

static void sendMulticastData4(const u_arraylist_t *iflist,
                               CAEndpoint_t *endpoint,
                               const void *data, size_t datalen)
{
    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");

#if defined(USE_IP_MREQN)
    struct ip_mreqn mreq = { .imr_multiaddr = IPv4MulticastAddress,
                             .imr_address.s_addr = htonl(INADDR_ANY),
                             .imr_ifindex = 0};
#else
    struct ip_mreq mreq  = { .imr_multiaddr.s_addr = IPv4MulticastAddress.s_addr,
                             .imr_interface = {0}};
#endif

    OICStrcpy(endpoint->addr, sizeof(endpoint->addr), IPv4_MULTICAST);
    CASocketFd_t fd = caglobals.ip.u4.fd;

    size_t len = u_arraylist_length(iflist);
    for (size_t i = 0; i < len; i++)
    {
        CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
        if (!ifitem)
        {
            continue;
        }
        if ((ifitem->flags & IFF_UP_RUNNING_FLAGS) != IFF_UP_RUNNING_FLAGS)
        {
            continue;
        }
        if (ifitem->family != AF_INET)
        {
            continue;
        }
#if defined(USE_IP_MREQN)
        mreq.imr_ifindex = ifitem->index;
#else
        mreq.imr_interface.s_addr = htonl(ifitem->index);
#endif
        if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, OPTVAL_T(&mreq), sizeof (mreq)))
        {
            OIC_LOG_V(ERROR, TAG, "send IP_MULTICAST_IF failed: %s (using defualt)",
                    CAIPS_GET_ERROR);
        }
        sendData(fd, endpoint, data, datalen, "multicast", "ipv4");
    }
}

void CAIPSendData(CAEndpoint_t *endpoint, const void *data, size_t datalen,
                  bool isMulticast)
{
    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");
    VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

    bool isSecure = (endpoint->flags & CA_SECURE) != 0;

    if (isMulticast)
    {
        endpoint->port = isSecure ? CA_SECURE_COAP : CA_COAP;

        u_arraylist_t *iflist = CAIPGetInterfaceInformation(0);
        if (!iflist)
        {
            OIC_LOG_V(ERROR, TAG, "get interface info failed: %s", strerror(errno));
            return;
        }

        if ((endpoint->flags & CA_IPV6) && caglobals.ip.ipv6enabled)
        {
            sendMulticastData6(iflist, endpoint, data, datalen);
        }
        if ((endpoint->flags & CA_IPV4) && caglobals.ip.ipv4enabled)
        {
            sendMulticastData4(iflist, endpoint, data, datalen);
        }

        u_arraylist_destroy(iflist);
    }
    else
    {
        if (!endpoint->port)    // unicast discovery
        {
            endpoint->port = isSecure ? CA_SECURE_COAP : CA_COAP;
        }

        CASocketFd_t fd;
        if (caglobals.ip.ipv6enabled && (endpoint->flags & CA_IPV6))
        {
            fd = isSecure ? caglobals.ip.u6s.fd : caglobals.ip.u6.fd;
#ifndef __WITH_DTLS__
            fd = caglobals.ip.u6.fd;
#endif
            sendData(fd, endpoint, data, datalen, "unicast", "ipv6");
        }
        if (caglobals.ip.ipv4enabled && (endpoint->flags & CA_IPV4))
        {
            fd = isSecure ? caglobals.ip.u4s.fd : caglobals.ip.u4.fd;
#ifndef __WITH_DTLS__
            fd = caglobals.ip.u4.fd;
#endif
            sendData(fd, endpoint, data, datalen, "unicast", "ipv4");
        }
    }
}

CAResult_t CAGetIPInterfaceInformation(CAEndpoint_t **info, size_t *size)
{
    VERIFY_NON_NULL(info, TAG, "info is NULL");
    VERIFY_NON_NULL(size, TAG, "size is NULL");

    u_arraylist_t *iflist = CAIPGetInterfaceInformation(0);
    if (!iflist)
    {
        OIC_LOG_V(ERROR, TAG, "get interface info failed: %s", strerror(errno));
        return CA_STATUS_FAILED;
    }

#ifdef __WITH_DTLS__
    const size_t endpointsPerInterface = 2;
#else
    const size_t endpointsPerInterface = 1;
#endif

    size_t interfaces = u_arraylist_length(iflist);
    for (size_t i = 0; i < u_arraylist_length(iflist); i++)
    {
        CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
        if (!ifitem)
        {
            continue;
        }

        if ((ifitem->family == AF_INET6 && !caglobals.ip.ipv6enabled) ||
            (ifitem->family == AF_INET && !caglobals.ip.ipv4enabled))
        {
            interfaces--;
        }
    }

    if (!interfaces)
    {
        OIC_LOG(DEBUG, TAG, "network interface size is zero");
        return CA_STATUS_OK;
    }

    size_t totalEndpoints = interfaces * endpointsPerInterface;
    CAEndpoint_t *eps = (CAEndpoint_t *)OICCalloc(totalEndpoints, sizeof (CAEndpoint_t));
    if (!eps)
    {
        OIC_LOG(ERROR, TAG, "Malloc Failed");
        u_arraylist_destroy(iflist);
        return CA_MEMORY_ALLOC_FAILED;
    }

    for (size_t i = 0, j = 0; i < u_arraylist_length(iflist); i++)
    {
        CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
        if (!ifitem)
        {
            continue;
        }

        if ((ifitem->family == AF_INET6 && !caglobals.ip.ipv6enabled) ||
            (ifitem->family == AF_INET && !caglobals.ip.ipv4enabled))
        {
            continue;
        }

        eps[j].adapter = CA_ADAPTER_IP;
        eps[j].ifindex = ifitem->index;

        if (ifitem->family == AF_INET6)
        {
            eps[j].flags = CA_IPV6;
            eps[j].port = caglobals.ip.u6.port;
        }
        else
        {
            eps[j].flags = CA_IPV4;
            eps[j].port = caglobals.ip.u4.port;
        }
        OICStrcpy(eps[j].addr, sizeof(eps[j].addr), ifitem->addr);

#ifdef __WITH_DTLS__
        j++;

        eps[j].adapter = CA_ADAPTER_IP;
        eps[j].ifindex = ifitem->index;

        if (ifitem->family == AF_INET6)
        {
            eps[j].flags = CA_IPV6 | CA_SECURE;
            eps[j].port = caglobals.ip.u6s.port;
        }
        else
        {
            eps[j].flags = CA_IPV4 | CA_SECURE;
            eps[j].port = caglobals.ip.u4s.port;
        }
        OICStrcpy(eps[j].addr, sizeof(eps[j].addr), ifitem->addr);
#endif
        j++;
    }

    *info = eps;
    *size = totalEndpoints;

    u_arraylist_destroy(iflist);

    return CA_STATUS_OK;
}

void CAIPSetErrorHandler(CAIPErrorHandleCallback errorHandleCallback)
{
    g_ipErrorHandler = errorHandleCallback;
}

CAResult_t CAGetLinkLocalZoneId(uint32_t ifindex, char **zoneId)
{
    return CAGetLinkLocalZoneIdInternal(ifindex, zoneId);
}
