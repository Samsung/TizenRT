/*****************************************************************j
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

#include <tinyara/config.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <errno.h>
#ifdef __linux__
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif

#ifdef __TINYARA__
#include <uio.h>
#endif

#include "pdu.h"
#include "caipinterface.h"
#include "caadapterutils.h"
#ifdef __WITH_DTLS__
#include "caadapternetdtls.h"
#endif
#include "camutex.h"
#include "oic_malloc.h"
#include "oic_string.h"

/*
 * Logging tag for module name
 */
#define TAG "OIC_CA_IP_SERVER"

#ifdef CONFIG_NET_LWIP
#define SOCK_CLOEXEC 0
#else
#define SOCK_CLOEXEC 1
#endif

#define SELECT_TIMEOUT 1     // select() seconds (and termination latency)


#define IPv4_MULTICAST     "224.0.1.187"
static struct in_addr IPv4MulticastAddress = { 0 };

#define IPv6_DOMAINS       16
#define IPv6_MULTICAST_INT "ff01::fd"
static struct in6_addr IPv6MulticastAddressInt;
#define IPv6_MULTICAST_LNK "ff02::fd"
static struct in6_addr IPv6MulticastAddressLnk;
#define IPv6_MULTICAST_RLM "ff03::fd"
static struct in6_addr IPv6MulticastAddressRlm;
#define IPv6_MULTICAST_ADM "ff04::fd"
static struct in6_addr IPv6MulticastAddressAdm;
#define IPv6_MULTICAST_SIT "ff05::fd"
static struct in6_addr IPv6MulticastAddressSit;
#define IPv6_MULTICAST_ORG "ff08::fd"
static struct in6_addr IPv6MulticastAddressOrg;
#define IPv6_MULTICAST_GLB "ff0e::fd"
static struct in6_addr IPv6MulticastAddressGlb;

static char *ipv6mcnames[IPv6_DOMAINS] =
{
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

struct in6_pktinfo
{
  struct in6_addr ipi6_addr;
  int             ipi6_ifindex;
};

struct in_pktinfo {
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


static CAIPExceptionCallback g_exceptionCallback;

static CAIPPacketReceivedCallback g_packetReceivedCallback;

static void CAHandleNetlink();
static void CAFindReadyMessage();
static void CASelectReturned(fd_set *readFds, int ret);
static void CAProcessNewInterface(CAInterface_t *ifchanged);
static CAResult_t CAReceiveMessage(int fd, CATransportFlags_t flags);

#define SET(TYPE, FDS) \
    if (caglobals.ip.TYPE.fd != -1) \
    { \
        FD_SET(caglobals.ip.TYPE.fd, FDS); \
    }

#define ISSET(TYPE, FDS, FLAGS) \
    if (caglobals.ip.TYPE.fd != -1 && FD_ISSET(caglobals.ip.TYPE.fd, FDS)) \
    { \
        fd = caglobals.ip.TYPE.fd; \
        flags = FLAGS; \
    }

void CADeInitializeIPGlobals()
{
  if (caglobals.ip.u6.fd != -1)
  {
    close(caglobals.ip.u6.fd);
    caglobals.ip.u6.fd = -1;
  }

  if (caglobals.ip.u6s.fd != -1)
  {
    close(caglobals.ip.u6s.fd);
    caglobals.ip.u6s.fd = -1;
  }

  if (caglobals.ip.u4.fd != -1)
  {
    close(caglobals.ip.u4.fd);
    caglobals.ip.u4.fd = -1;
  }

  if (caglobals.ip.u4s.fd != -1)
  {
    close(caglobals.ip.u4s.fd);
    caglobals.ip.u4s.fd = -1;
  }

  if (caglobals.ip.m6.fd != -1)
  {
    close(caglobals.ip.m6.fd);
    caglobals.ip.m6.fd = -1;
  }

  if (caglobals.ip.m6s.fd != -1)
  {
    close(caglobals.ip.m6s.fd);
    caglobals.ip.m6s.fd = -1;
  }

  if (caglobals.ip.m4.fd != -1)
  {
    close(caglobals.ip.m4.fd);
    caglobals.ip.m4.fd = -1;
  }

  if (caglobals.ip.m4s.fd != -1)
  {
    close(caglobals.ip.m4s.fd);
    caglobals.ip.m4s.fd = -1;
  }
}

static void CAReceiveHandler(void *data)
{
  (void)data;

  while (!caglobals.ip.terminate)
  {
    CAFindReadyMessage();
  }
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
  if (caglobals.ip.shutdownFds[0] != -1)
  {
    FD_SET(caglobals.ip.shutdownFds[0], &readFds);
  }
  if (caglobals.ip.netlinkFd != -1)
  {
    FD_SET(caglobals.ip.netlinkFd, &readFds);
  }

  int ret = select(caglobals.ip.maxfd + 1, &readFds, NULL, NULL, tv);

  if (caglobals.ip.terminate)
  {
    OIC_LOG_V(DEBUG, TAG, "Packet receiver Stop request received.");
    return;
  }
  if (ret <= 0)
  {
    if (ret < 0)
    {
      OIC_LOG_V(FATAL, TAG, "select error %s", strerror(errno));
    }
    return;
  }

  CASelectReturned(&readFds, ret);
}

static void CASelectReturned(fd_set *readFds, int ret)
{
  (void)ret;
  int fd = -1;
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
                  else if (FD_ISSET(caglobals.ip.netlinkFd, readFds))
                  {
                    CAHandleNetlink();
                    break;
                  }
                  else if (FD_ISSET(caglobals.ip.shutdownFds[0], readFds))
                  {
                    char buf[10] = {0};
                    ssize_t len = read(caglobals.ip.shutdownFds[0], buf, sizeof(buf));
                    if (-1 == len)
                    {
                      continue;
                    }

                    CAInterface_t *ifchanged = CAFindInterfaceChange();
                    if (ifchanged)
                    {
                      CAProcessNewInterface(ifchanged);
                      OICFree(ifchanged);
                    }
                    break;
                  }
                  else
                  {
                    break;
                  }

    (void)CAReceiveMessage(fd, flags);
    FD_CLR(fd, readFds);
  }
}

static CAResult_t CAReceiveMessage(int fd, CATransportFlags_t flags)
{
  char recvBuffer[COAP_MAX_PDU_SIZE];

  size_t len;
  int level, type, namelen;
  struct sockaddr_storage srcAddr;
  unsigned char *pktinfo = NULL;
  struct cmsghdr *cmp = NULL;
  struct iovec iov = { recvBuffer, sizeof(recvBuffer) };
  union control
  {
    struct cmsghdr cmsg;
    unsigned char data[CMSG_SPACE(sizeof(struct in6_pktinfo))];
  } cmsg;

  if (flags & CA_IPV6)
  {
    namelen = sizeof(struct sockaddr_in6);
    level = IPPROTO_IPV6;
    type = IPV6_PKTINFO;
    len = sizeof(struct in6_pktinfo);
  }
  else
  {
    namelen = sizeof(struct sockaddr_in);
    level = IPPROTO_IP;
    type = IP_PKTINFO;
    len = sizeof(struct in6_pktinfo);
  }

  struct msghdr msg = { .msg_name = &srcAddr,
    .msg_namelen = namelen,
     .msg_iov = &iov,
      .msg_iovlen = 1,
       .msg_control = &cmsg,
        .msg_controllen = CMSG_SPACE(len)
  };

  ssize_t recvLen = recvmsg(fd, &msg, flags);
  if (-1 == recvLen)
  {
    OIC_LOG_V(ERROR, TAG, "Recvfrom failed %s", strerror(errno));
    return CA_STATUS_FAILED;
  }

  if (flags & CA_MULTICAST)
  {
    for (cmp = CMSG_FIRSTHDR(&msg); cmp != NULL; cmp = CMSG_NXTHDR(&msg, cmp))
    {
      if (cmp->cmsg_level == level && cmp->cmsg_type == type)
      {
        pktinfo = CMSG_DATA(cmp);
      }
    }
  }

  CASecureEndpoint_t sep = {.endpoint = {.adapter = CA_ADAPTER_IP, .flags = flags}};
#if 0
  if (flags & CA_IPV6)
  {
    sep.endpoint.interface = ((struct sockaddr_in6 *)&srcAddr)->sin6_scope_id;
    ((struct sockaddr_in6 *)&srcAddr)->sin6_scope_id = 0;

    if ((flags & CA_MULTICAST) && pktinfo)
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
    if ((flags & CA_MULTICAST) && pktinfo)
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

  CAConvertAddrToName(&srcAddr, msg.msg_namelen, sep.endpoint.addr, &sep.endpoint.port);

  if (flags & CA_SECURE)
  {
#ifdef __WITH_DTLS__
    int ret = CAAdapterNetDtlsDecrypt(&sep, (uint8_t *)recvBuffer, recvLen);
    OIC_LOG_V(DEBUG, TAG, "CAAdapterNetDtlsDecrypt returns [%d]", ret);
#else
    OIC_LOG(ERROR, TAG, "Encrypted message but no DTLS");
#endif
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
  OIC_LOG(DEBUG, TAG, "IN");
  OIC_LOG(DEBUG, TAG, "OUT");
}

static int CACreateSocket(int family, uint16_t *port)
{
  int socktype = SOCK_DGRAM;
#ifdef SOCK_CLOEXEC
  socktype |= SOCK_CLOEXEC;
#endif
  int fd = socket(family, socktype, IPPROTO_UDP);
  if (-1 == fd)
  {
    OIC_LOG_V(ERROR, TAG, "create socket failed: %s", strerror(errno));
    return -1;
  }

#ifndef SOCK_CLOEXEC
  int fl = fcntl(fd, F_GETFD);
  if (-1 == fl || -1 == fcntl(fd, F_SETFD, fl | FD_CLOEXEC))
  {
    OIC_LOG_V(ERROR, TAG, "set FD_CLOEXEC failed: %s", strerror(errno));
    close(fd);
    return -1;
  }
#endif

  struct sockaddr_storage sa = { .ss_family = family };
  socklen_t socklen;

  if (family == AF_INET6)
  {
    int on = 1;

    if (-1 == setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)))
    {
      OIC_LOG_V(ERROR, TAG, "IPV6_V6ONLY failed: %s", strerror(errno));
    }

    if (*port)      // only do this for multicast ports
    {
      if (-1 == setsockopt(fd, IPPROTO_IPV6, IPV6_RECVPKTINFO, &on, sizeof(on)))
      {
        OIC_LOG_V(ERROR, TAG, "IPV6_RECVPKTINFO failed: %s", strerror(errno));
      }
    }

    ((struct sockaddr_in6 *)&sa)->sin6_port = htons(*port);
    socklen = sizeof(struct sockaddr_in6);
  }
  else
  {
    if (*port)      // only do this for multicast ports
    {
      int on = 1;
      if (-1 == setsockopt(fd, IPPROTO_IP, IP_PKTINFO, &on, sizeof(on)))
      {
        OIC_LOG_V(ERROR, TAG, "IP_PKTINFO failed: %s", strerror(errno));
      }
    }

    ((struct sockaddr_in *)&sa)->sin_port = htons(*port);
    socklen = sizeof(struct sockaddr_in);
  }

  if (*port)  // use the given port
  {
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)))
    {
      OIC_LOG_V(ERROR, TAG, "SO_REUSEADDR failed: %s", strerror(errno));
      close(fd);
      return -1;
    }
  }

  if (-1 == bind(fd, (struct sockaddr *)&sa, socklen))
  {
    OIC_LOG_V(ERROR, TAG, "bind socket failed: %s", strerror(errno));
    close(fd);
    return -1;
  }

  if (!*port)  // return the assigned port
  {
    if (-1 == getsockname(fd, (struct sockaddr *)&sa, &socklen))
    {
      OIC_LOG_V(ERROR, TAG, "getsockname failed: %s", strerror(errno));
      close(fd);
      return -1;
    }
    *port = ntohs(family == AF_INET6 ?
                  ((struct sockaddr_in6 *)&sa)->sin6_port :
                  ((struct sockaddr_in *)&sa)->sin_port);
  }

  return fd;
}

#define CHECKFD(FD) \
    if (FD > caglobals.ip.maxfd) \
        caglobals.ip.maxfd = FD;
#define NEWSOCKET(FAMILY, NAME) \
    caglobals.ip.NAME.fd = CACreateSocket(FAMILY, &caglobals.ip.NAME.port); \
    CHECKFD(caglobals.ip.NAME.fd)

static void CAInitializeNetlink()
{
#ifdef __linux__
  // create NETLINK fd for interface change notifications
  struct sockaddr_nl sa = { AF_NETLINK, 0, 0, RTMGRP_LINK };

  caglobals.ip.netlinkFd = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
  if (caglobals.ip.netlinkFd == -1)
  {
    OIC_LOG_V(ERROR, TAG, "netlink socket failed: %s", strerror(errno));
  }
  else
  {
    int r = bind(caglobals.ip.netlinkFd, (struct sockaddr *)&sa, sizeof(sa));
    if (r)
    {
      OIC_LOG_V(ERROR, TAG, "netlink bind failed: %s", strerror(errno));
      close(caglobals.ip.netlinkFd);
      caglobals.ip.netlinkFd = -1;
    }
    else
    {
      CHECKFD(caglobals.ip.netlinkFd);
    }
  }
#endif
}

static void CAInitializePipe()
{
  caglobals.ip.selectTimeout = -1;
#ifdef HAVE_PIPE2
  int ret = pipe2(caglobals.ip.shutdownFds, O_CLOEXEC);
#else
  int ret = pipe(caglobals.ip.shutdownFds);
  if (-1 != ret)
  {
    ret = fcntl(caglobals.ip.shutdownFds[0], F_GETFD);
    if (-1 != ret)
    {
      ret = fcntl(caglobals.ip.shutdownFds[0], F_SETFD, ret | FD_CLOEXEC);
    }
    if (-1 != ret)
    {
      ret = fcntl(caglobals.ip.shutdownFds[1], F_GETFD);
    }
    if (-1 != ret)
    {
      ret = fcntl(caglobals.ip.shutdownFds[1], F_SETFD, ret | FD_CLOEXEC);
    }
    if (-1 == ret)
    {
      close(caglobals.ip.shutdownFds[1]);
      close(caglobals.ip.shutdownFds[0]);
      caglobals.ip.shutdownFds[0] = -1;
      caglobals.ip.shutdownFds[1] = -1;
    }
  }
#endif
  if (-1 == ret)
  {
    OIC_LOG_V(ERROR, TAG, "pipe failed: %s", strerror(errno));
    caglobals.ip.selectTimeout = SELECT_TIMEOUT; //poll needed for shutdown
  }
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
    NEWSOCKET(AF_INET6, u6)
    NEWSOCKET(AF_INET6, u6s)
    NEWSOCKET(AF_INET6, m6)
    NEWSOCKET(AF_INET6, m6s)
    OIC_LOG_V(INFO, TAG, "IPv6 unicast port: %u", caglobals.ip.u6.port);
  }
  if (caglobals.ip.ipv4enabled)
  {
    NEWSOCKET(AF_INET, u4)
    NEWSOCKET(AF_INET, u4s)
    NEWSOCKET(AF_INET, m4)
    NEWSOCKET(AF_INET, m4s)
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
  // create pipe for fast shutdown
  CAInitializePipe();
  CHECKFD(caglobals.ip.shutdownFds[0]);
  CHECKFD(caglobals.ip.shutdownFds[1]);

  // create source of network interface change notifications
  CAInitializeNetlink();

  caglobals.ip.selectTimeout = CAGetPollingInterval(caglobals.ip.selectTimeout);

  res = CAIPStartListenServer();
  if (CA_STATUS_OK != res)
  {
    OIC_LOG_V(ERROR, TAG, "Failed to start listening server![%d]", res);
    return res;
  }

  caglobals.ip.terminate = false;
  res = ca_thread_pool_add_task(threadPool, CAReceiveHandler, NULL);
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
  caglobals.ip.started = false;
  caglobals.ip.terminate = true;

  if (caglobals.ip.shutdownFds[1] != -1)
  {
    close(caglobals.ip.shutdownFds[1]);
    // receive thread will stop immediately
  }
  else
  {
    // receive thread will stop in SELECT_TIMEOUT seconds.
  }
}

void CAWakeUpForChange()
{
  if (caglobals.ip.shutdownFds[1] != -1)
  {
    ssize_t len = 0;
    do
    {
      len = write(caglobals.ip.shutdownFds[1], "w", 1);
    }
    while ((len == -1) && (errno == EINTR));
    if ((len == -1) && (errno != EINTR) && (errno != EPIPE))
    {
      OIC_LOG_V(DEBUG, TAG, "write failed: %s", strerror(errno));
    }
  }
}

static void applyMulticastToInterface4(struct in_addr inaddr)
{
  if (!caglobals.ip.ipv4enabled)
  {
    return;
  }

  struct ip_mreqn mreq = { .imr_multiaddr = IPv4MulticastAddress,
    .imr_address = inaddr,
     .imr_ifindex = 0
  };
  if (setsockopt(caglobals.ip.m4.fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)))
  {
    if (EADDRINUSE != errno)
    {
      OIC_LOG_V(ERROR, TAG, "IPv4 IP_ADD_MEMBERSHIP failed: %s", strerror(errno));
    }
  }
  if (setsockopt(caglobals.ip.m4s.fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)))
  {
    if (EADDRINUSE != errno)
    {
      OIC_LOG_V(ERROR, TAG, "secure IPv4 IP_ADD_MEMBERSHIP failed: %s", strerror(errno));
    }
  }
}

static void applyMulticast6(int fd, struct in6_addr *addr, uint32_t interface)
{
#if 0
  struct ipv6_mreq mreq = {.ipv6mr_multiaddr = *addr, .ipv6mr_interface = interface};

  if (setsockopt(fd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq, sizeof(mreq)))
  {
    if (EADDRINUSE != errno)
    {
      OIC_LOG_V(ERROR, TAG, "IPv6 IP_ADD_MEMBERSHIP failed: %s", strerror(errno));
    }
  }
#endif
}

static void applyMulticastToInterface6(uint32_t interface)
{
  if (!caglobals.ip.ipv6enabled)
  {
    return;
  }
  //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressInt, interface);
  applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressLnk, interface);
  //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressRlm, interface);
  //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressAdm, interface);
  //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressSit, interface);
  //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressOrg, interface);
  //applyMulticast6(caglobals.ip.m6.fd, &IPv6MulticastAddressGlb, interface);

  //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressInt, interface);
  applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressLnk, interface);
  //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressRlm, interface);
  //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressAdm, interface);
  //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressSit, interface);
  //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressOrg, interface);
  //applyMulticast6(caglobals.ip.m6s.fd, &IPv6MulticastAddressGlb, interface);
}

CAResult_t CAIPStartListenServer()
{
  u_arraylist_t *iflist = CAIPGetInterfaceInformation(0);
  if (!iflist)
  {
    OIC_LOG_V(ERROR, TAG, "get interface info failed: %s", strerror(errno));
    return CA_STATUS_FAILED;
  }

  uint32_t len = u_arraylist_length(iflist);
  OIC_LOG_V(DEBUG, TAG, "IP network interfaces found: %d", len);

  for (uint32_t i = 0; i < len; i++)
  {
    CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);

    if (!ifitem)
    {
      continue;
    }
    if ((ifitem->flags & (IFF_UP | IFF_RUNNING)) != (IFF_UP | IFF_RUNNING))
    {
      continue;
    }
    if (ifitem->family == AF_INET)
    {
      struct in_addr inaddr;
      inaddr.s_addr = ifitem->ipv4addr;
      applyMulticastToInterface4(inaddr);
      OIC_LOG_V(DEBUG, TAG, "IPv4 network interface: %s", ifitem->name);
    }
    if (ifitem->family == AF_INET6)
    {
      applyMulticastToInterface6(ifitem->index);
      OIC_LOG_V(DEBUG, TAG, "IPv6 network interface: %s", ifitem->name);
    }
  }

  u_arraylist_destroy(iflist);
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

  uint32_t len = u_arraylist_length(iflist);
  OIC_LOG_V(DEBUG, TAG, "IP network interfaces found: %d", len);

  for (uint32_t i = 0; i < len; i++)
  {
    CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);

    if (!ifitem)
    {
      continue;
    }

    if ((ifitem->flags & (IFF_UP | IFF_RUNNING)) != (IFF_UP | IFF_RUNNING))
    {
      continue;
    }
    if (ifitem->family == AF_INET)
    {
      close(caglobals.ip.m4.fd);
      close(caglobals.ip.m4s.fd);
      caglobals.ip.m4.fd = -1;
      caglobals.ip.m4s.fd = -1;
      OIC_LOG_V(DEBUG, TAG, "IPv4 network interface: %s cloed", ifitem->name);
    }
    if (ifitem->family == AF_INET6)
    {
      close(caglobals.ip.m6.fd);
      close(caglobals.ip.m6s.fd);
      caglobals.ip.m6.fd = -1;
      caglobals.ip.m6s.fd = -1;
      OIC_LOG_V(DEBUG, TAG, "IPv6 network interface: %s", ifitem->name);
    }
  }
  u_arraylist_destroy(iflist);
  return CA_STATUS_OK;
}

static void CAProcessNewInterface(CAInterface_t *ifitem)
{
  if (!ifitem)
  {
    OIC_LOG(DEBUG, TAG, "ifitem is null");
    return;
  }

  applyMulticastToInterface6(ifitem->index);
  struct in_addr inaddr = { .s_addr = ifitem->ipv4addr };
  applyMulticastToInterface4(inaddr);
}
static void CAHandleNetlink()
{
#ifdef __linux__
  char buf[4096];
  struct nlmsghdr *nh;
  struct sockaddr_nl sa;
  struct iovec iov = { buf, sizeof(buf) };
  struct msghdr msg = { (void *) &sa, sizeof(sa), &iov, 1, NULL, 0, 0 };

  size_t len = recvmsg(caglobals.ip.netlinkFd, &msg, 0);

  for (nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, len); nh = NLMSG_NEXT(nh, len))
  {
    if (nh != NULL && nh->nlmsg_type != RTM_NEWLINK)
    {
      continue;
    }

    struct ifinfomsg *ifi = (struct ifinfomsg *)NLMSG_DATA(nh);
    if (!ifi || (ifi->ifi_flags & IFF_LOOPBACK) || !(ifi->ifi_flags & IFF_RUNNING))
    {
      continue;
    }

    int newIndex = ifi->ifi_index;

    u_arraylist_t *iflist = CAIPGetInterfaceInformation(newIndex);
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

      if ((int)ifitem->index != newIndex)
      {
        continue;
      }

      CAProcessNewInterface(ifitem);
      break; // we found the one we were looking for
    }
    u_arraylist_destroy(iflist);
  }
#endif // __linux__
}

void CAIPSetPacketReceiveCallback(CAIPPacketReceivedCallback callback)
{
  g_packetReceivedCallback = callback;
}

void CAIPSetExceptionCallback(CAIPExceptionCallback callback)
{
  g_exceptionCallback = callback;
}

static void sendData(int fd, const CAEndpoint_t *endpoint,
                     const void *data, uint32_t dlen,
                     const char *cast, const char *fam)
{
  OIC_LOG(DEBUG, TAG, "IN");

  if (!endpoint)
  {
    OIC_LOG(DEBUG, TAG, "endpoint is null");
    return;
  }

  char *secure = (endpoint->flags & CA_SECURE) ? "secure " : "";
  (void)secure;   // eliminates release warning
  struct sockaddr_storage sock;
  CAConvertNameToAddr(endpoint->addr, endpoint->port, &sock);

  socklen_t socklen;
  if (sock.ss_family == AF_INET6)
  {
    struct sockaddr_in6 *sock6 = (struct sockaddr_in6 *)&sock;
#if 0
    if (!sock6->sin6_scope_id)
    {
      sock6->sin6_scope_id = endpoint->interface;
    }
#endif
    socklen = sizeof(struct sockaddr_in6);
  }
  else
  {
    socklen = sizeof(struct sockaddr_in);
  }

  ssize_t len = sendto(fd, data, dlen, 0, (struct sockaddr *)&sock, socklen);
  if (-1 == len)
  {
    // If logging is not defined/enabled.
    (void)cast;
    (void)fam;
    OIC_LOG_V(ERROR, TAG, "%s%s %s sendTo failed: %s", secure, cast, fam, strerror(errno));
  }
  else
  {
    OIC_LOG_V(INFO, TAG, "%s%s %s sendTo is successful: %zd bytes", secure, cast, fam, len);
  }
}

static void sendMulticastData6(const u_arraylist_t *iflist,
                               CAEndpoint_t *endpoint,
                               const void *data, uint32_t datalen)
{
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
  int fd = caglobals.ip.u6.fd;

  uint32_t len = u_arraylist_length(iflist);
  for (uint32_t i = 0; i < len; i++)
  {
    CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
    if (!ifitem)
    {
      continue;
    }
    if ((ifitem->flags & (IFF_UP | IFF_RUNNING)) != (IFF_UP | IFF_RUNNING))
    {
      continue;
    }
    if (ifitem->family != AF_INET6)
    {
      continue;
    }

    int index = ifitem->index;
    if (setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &index, sizeof(index)))
    {
      OIC_LOG_V(ERROR, TAG, "setsockopt6 failed: %s", strerror(errno));
      return;
    }
    sendData(fd, endpoint, data, datalen, "multicast", "ipv6");
  }
}

static void sendMulticastData4(const u_arraylist_t *iflist,
                               CAEndpoint_t *endpoint,
                               const void *data, uint32_t datalen)
{
  VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");

  struct ip_mreqn mreq = { .imr_multiaddr = IPv4MulticastAddress,
    .imr_ifindex = 0
  };
  OICStrcpy(endpoint->addr, sizeof(endpoint->addr), IPv4_MULTICAST);
  int fd = caglobals.ip.u4.fd;

  uint32_t len = u_arraylist_length(iflist);
  for (uint32_t i = 0; i < len; i++)
  {
    CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
    if (!ifitem)
    {
      continue;
    }
    if ((ifitem->flags & (IFF_UP | IFF_RUNNING)) != (IFF_UP | IFF_RUNNING))
    {
      continue;
    }
    if (ifitem->family != AF_INET)
    {
      continue;
    }

    struct in_addr inaddr;
    inaddr.s_addr = ifitem->ipv4addr;
    mreq.imr_address = inaddr;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)))
    {
      OIC_LOG_V(ERROR, TAG, "send IP_MULTICAST_IF failed: %s (using defualt)",
                strerror(errno));
    }
    sendData(fd, endpoint, data, datalen, "multicast", "ipv4");
  }
}

void CAIPSendData(CAEndpoint_t *endpoint, const void *data, uint32_t datalen,
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

    int fd;
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

CAResult_t CAGetIPInterfaceInformation(CAEndpoint_t **info, uint32_t *size)
{
  VERIFY_NON_NULL(info, TAG, "info is NULL");
  VERIFY_NON_NULL(size, TAG, "size is NULL");

  u_arraylist_t *iflist = CAIPGetInterfaceInformation(0);
  if (!iflist)
  {
    OIC_LOG_V(ERROR, TAG, "get interface info failed: %s", strerror(errno));
    return CA_STATUS_FAILED;
  }

  uint32_t len = u_arraylist_length(iflist);
  uint32_t length = len;

#ifdef __WITH_DTLS__
  //If DTLS is supported, each interface can support secure port as well
  length = len * 2;
#endif

  CAEndpoint_t *eps = (CAEndpoint_t *)OICCalloc(length, sizeof(CAEndpoint_t));
  if (!eps)
  {
    OIC_LOG(ERROR, TAG, "Malloc Failed");
    u_arraylist_destroy(iflist);
    return CA_MEMORY_ALLOC_FAILED;
  }

  for (uint32_t i = 0, j = 0; i < len; i++)
  {
    CAInterface_t *ifitem = (CAInterface_t *)u_arraylist_get(iflist, i);
    if (!ifitem)
    {
      continue;
    }

    eps[j].adapter = CA_ADAPTER_IP;
    eps[j].interface = 0;

    if (ifitem->family == AF_INET6)
    {
      eps[j].flags = CA_IPV6;
      eps[j].port = caglobals.ip.u6.port;
    }
    else
    {
      eps[j].flags = CA_IPV4;
      eps[j].port = caglobals.ip.u4.port;

      inet_ntop(AF_INET, &(ifitem->ipv4addr), eps[j].addr, MAX_ADDR_STR_SIZE_CA);
    }

#ifdef __WITH_DTLS__
    j++;

    eps[j].adapter = CA_ADAPTER_IP;
    eps[j].interface = 0;

    if (ifitem->family == AF_INET6)
    {
      eps[j].flags = CA_IPV6 | CA_SECURE;
      eps[j].port = caglobals.ip.u6s.port;
    }
    else
    {
      eps[j].flags = CA_IPV4 | CA_SECURE;
      eps[j].port = caglobals.ip.u4s.port;
      inet_ntop(AF_INET, &(ifitem->ipv4addr), eps[j].addr, MAX_ADDR_STR_SIZE_CA);
    }
#endif
    j++;
  }

  *info = eps;
  *size = len;

  u_arraylist_destroy(iflist);

  return CA_STATUS_OK;
}
