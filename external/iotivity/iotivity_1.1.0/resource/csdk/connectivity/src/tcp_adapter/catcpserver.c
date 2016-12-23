/* ****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <errno.h>
//#include <sys/poll.h>

#ifndef WITH_ARDUINO
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include "catcpinterface.h"
#include "pdu.h"
#include "caadapterutils.h"
#include "camutex.h"
#include "oic_malloc.h"
#include "oic_string.h"

/**
 * Logging tag for module name.
 */
#define TAG "OIC_CA_TCP_SERVER"

/**
 * Server port number for local test.
 */
#define SERVER_PORT 8000

/**
 * Maximum CoAP over TCP header length
 * to know the total data length.
 */
#define TCP_MAX_HEADER_LEN  6

/**
 * Accept server file descriptor.
 */
static int g_acceptServerFD = -1;

/**
 * Mutex to synchronize device object list.
 */
static ca_mutex g_mutexObjectList = NULL;

/**
 * Conditional mutex to synchronize.
 */
static ca_cond g_condObjectList = NULL;

/**
 * Maintains the callback to be notified when data received from remote device.
 */
static CATCPPacketReceivedCallback g_packetReceivedCallback;

/**
 * Error callback to update error in TCP.
 */
static CATCPErrorHandleCallback g_TCPErrorHandler = NULL;

/**
 * Connected Callback to pass the connection information to RI.
 */
static CATCPKeepAliveHandleCallback g_keepaliveCallback = NULL;

static CAResult_t CATCPCreateMutex();
static void CATCPDestroyMutex();
static CAResult_t CATCPCreateCond();
static void CATCPDestroyCond();
static CAResult_t CACreateAcceptSocket();
static void CAAcceptConnection();
static void CAFindReadyMessage();
static void CASelectReturned(fd_set *readFds, int ret);
static void CAReceiveMessage(int fd);
static void CAReceiveHandler(void *data);
static int CATCPCreateSocket(int family, CATCPSessionInfo_t *tcpServerInfo);

#define CHECKFD(FD) \
    if (FD > caglobals.tcp.maxfd) \
        caglobals.tcp.maxfd = FD;

static void CATCPDestroyMutex()
{
  if (g_mutexObjectList)
  {
    ca_mutex_free(g_mutexObjectList);
    g_mutexObjectList = NULL;
  }
}

static CAResult_t CATCPCreateMutex()
{
  if (!g_mutexObjectList)
  {
    g_mutexObjectList = ca_mutex_new();
    if (!g_mutexObjectList)
    {
      OIC_LOG(ERROR, TAG, "Failed to created mutex!");
      return CA_STATUS_FAILED;
    }
  }

  return CA_STATUS_OK;
}

static void CATCPDestroyCond()
{
  if (g_condObjectList)
  {
    ca_cond_free(g_condObjectList);
    g_condObjectList = NULL;
  }
}

static CAResult_t CATCPCreateCond()
{
  if (!g_condObjectList)
  {
    g_condObjectList = ca_cond_new();
    if (!g_condObjectList)
    {
      OIC_LOG(ERROR, TAG, "Failed to created cond!");
      return CA_STATUS_FAILED;
    }
  }
  return CA_STATUS_OK;
}

static void CAReceiveHandler(void *data)
{
  (void)data;
  OIC_LOG(DEBUG, TAG, "IN - CAReceiveHandler");

  while (!caglobals.tcp.terminate)
  {
    CAFindReadyMessage();
  }

  ca_mutex_lock(g_mutexObjectList);
  ca_cond_signal(g_condObjectList);
  ca_mutex_unlock(g_mutexObjectList);

  OIC_LOG(DEBUG, TAG, "OUT - CAReceiveHandler");
}

static void CAFindReadyMessage()
{
  fd_set readFds;
  struct timeval timeout = { .tv_sec = caglobals.tcp.selectTimeout };

  FD_ZERO(&readFds);

  if (-1 != g_acceptServerFD)
  {
    FD_SET(g_acceptServerFD, &readFds);
  }
  if (-1 != caglobals.tcp.shutdownFds[0])
  {
    FD_SET(caglobals.tcp.shutdownFds[0], &readFds);
  }
  if (-1 != caglobals.tcp.connectionFds[0])
  {
    FD_SET(caglobals.tcp.connectionFds[0], &readFds);
  }

  uint32_t length = u_arraylist_length(caglobals.tcp.svrlist);
  for (size_t i = 0; i < length; i++)
  {
    CATCPSessionInfo_t *svritem =
      (CATCPSessionInfo_t *) u_arraylist_get(caglobals.tcp.svrlist, i);
    if (svritem && 0 <= svritem->fd)
    {
      FD_SET(svritem->fd, &readFds);
    }
  }

  int ret = select(caglobals.tcp.maxfd + 1, &readFds, NULL, NULL, &timeout);

  if (caglobals.tcp.terminate)
  {
    OIC_LOG_V(DEBUG, TAG, "Packet receiver Stop request received.");
    return;
  }
  if (0 >= ret)
  {
    if (0 > ret)
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

  if (g_acceptServerFD != -1 && FD_ISSET(g_acceptServerFD, readFds))
  {
    CAAcceptConnection();
    return;
  }
  else if (-1 != caglobals.tcp.connectionFds[0] &&
           FD_ISSET(caglobals.tcp.connectionFds[0], readFds))
  {
    // new connection was created from remote device.
    // exit the function to update read file descriptor.
    char buf[MAX_ADDR_STR_SIZE_CA] = {0};
    ssize_t len = read(caglobals.tcp.connectionFds[0], buf, sizeof(buf));
    if (-1 == len)
    {
      return;
    }
    OIC_LOG_V(DEBUG, TAG, "Received new connection event with [%s]", buf);
    FD_CLR(caglobals.tcp.connectionFds[0], readFds);
    return;
  }
  else
  {
    uint32_t length = u_arraylist_length(caglobals.tcp.svrlist);
    for (size_t i = 0; i < length; i++)
    {
      CATCPSessionInfo_t *svritem =
        (CATCPSessionInfo_t *) u_arraylist_get(caglobals.tcp.svrlist, i);
      if (svritem && svritem->fd >= 0)
      {
        if (FD_ISSET(svritem->fd, readFds))
        {
          CAReceiveMessage(svritem->fd);
          FD_CLR(svritem->fd, readFds);
        }
      }
    }
  }
}

static void CAAcceptConnection()
{
  struct sockaddr_storage clientaddr;
  socklen_t clientlen = sizeof(struct sockaddr_in);

  int sockfd = accept(g_acceptServerFD, (struct sockaddr *)&clientaddr,
                      &clientlen);
  if (-1 != sockfd)
  {
    CATCPSessionInfo_t *svritem =
      (CATCPSessionInfo_t *) OICCalloc(1, sizeof(*svritem));
    if (!svritem)
    {
      OIC_LOG(ERROR, TAG, "Out of memory");
      close(sockfd);
      return;
    }

    svritem->fd = sockfd;
    CAConvertAddrToName((struct sockaddr_storage *)&clientaddr, clientlen,
                        (char *) &svritem->sep.endpoint.addr, &svritem->sep.endpoint.port);

    ca_mutex_lock(g_mutexObjectList);
    bool result = u_arraylist_add(caglobals.tcp.svrlist, svritem);
    if (!result)
    {
      OIC_LOG(ERROR, TAG, "u_arraylist_add failed.");
      close(sockfd);
      OICFree(svritem);
      ca_mutex_unlock(g_mutexObjectList);
      return;
    }
    ca_mutex_unlock(g_mutexObjectList);

    CHECKFD(sockfd);
  }
}

static void CAReceiveMessage(int fd)
{
  // #1. get remote device information from file descriptor.
  size_t index = 0;
  CATCPSessionInfo_t *svritem = CAGetSessionInfoFromFD(fd, &index);
  if (!svritem)
  {
    OIC_LOG(ERROR, TAG, "there is no connection information in list");
    return;
  }

  // #2. get already allocated memory size.
  size_t bufSize = (svritem->totalDataLen == 0) ? TCP_MAX_HEADER_LEN : svritem->totalDataLen;
  if (!svritem->recvData)
  {
    svritem->recvData = (unsigned char *) OICCalloc(1, bufSize);
    if (!svritem->recvData)
    {
      OIC_LOG(ERROR, TAG, "out of memory");
      CADisconnectTCPSession(svritem, index);
      return;
    }
  }

  // #3. receive data from remote device.
  ssize_t recvLen = recv(fd, svritem->recvData + svritem->recvDataLen,
                         bufSize - svritem->recvDataLen, 0);
  if (recvLen <= 0)
  {
    if (EWOULDBLOCK != errno)
    {
      OIC_LOG_V(ERROR, TAG, "Recvfrom failed %s", strerror(errno));
      CADisconnectTCPSession(svritem, index);
    }
    return;
  }
  svritem->recvDataLen += recvLen;

  // #4. get actual data length from coap over tcp header.
  if (!svritem->totalDataLen)
  {
    coap_transport_type transport = coap_get_tcp_header_type_from_initbyte(
                                      ((unsigned char *) svritem->recvData)[0] >> 4);

    size_t headerLen = coap_get_tcp_header_length_for_transport(transport);
    if (svritem->recvDataLen >= headerLen)
    {
      svritem->totalDataLen = CAGetTotalLengthFromHeader(
                                (unsigned char *) svritem->recvData);
      bufSize = svritem->totalDataLen;
      unsigned char *newBuf = OICRealloc(svritem->recvData, bufSize);
      if (!newBuf)
      {
        OIC_LOG(ERROR, TAG, "out of memory");
        CADisconnectTCPSession(svritem, index);
        return;
      }
      svritem->recvData = newBuf;
    }
  }

  // #5. pass the received data information to upper layer.
  if ((svritem->totalDataLen == svritem->recvDataLen) && g_packetReceivedCallback)
  {
    svritem->sep.endpoint.adapter = CA_ADAPTER_TCP;
    g_packetReceivedCallback(&svritem->sep, svritem->recvData, svritem->recvDataLen);
    OIC_LOG_V(DEBUG, TAG, "total received data len:%d", svritem->recvDataLen);

    // initialize data info to receive next message.
    OICFree(svritem->recvData);
    svritem->recvData = NULL;
    svritem->recvDataLen = 0;
    svritem->totalDataLen = 0;
  }

  return;
}

static void CAWakeUpForReadFdsUpdate(const char *host)
{
  if (caglobals.tcp.connectionFds[1] != -1)
  {
    ssize_t len = 0;
    do
    {
      len = write(caglobals.tcp.connectionFds[1], host, strlen(host));
    }
    while ((len == -1) && (errno == EINTR));

    if ((len == -1) && (errno != EINTR) && (errno != EPIPE))
    {
      OIC_LOG_V(DEBUG, TAG, "write failed: %s", strerror(errno));
    }
  }
}

static int CATCPCreateSocket(int family, CATCPSessionInfo_t *svritem)
{
  // create tcp socket
  int fd = socket(family, SOCK_STREAM, IPPROTO_TCP);
  if (-1 == fd)
  {
    OIC_LOG_V(ERROR, TAG, "create socket failed: %s", strerror(errno));
    goto exit;
  }

  struct sockaddr_storage sa = { .ss_family = family };
  CAConvertNameToAddr(svritem->sep.endpoint.addr, svritem->sep.endpoint.port, &sa);
  socklen_t socklen = sizeof(struct sockaddr_in);

  // connect to TCP server
  int ret = connect(fd, (struct sockaddr *)&sa, socklen);
  if (0 == ret)
  {
    OIC_LOG(DEBUG, TAG, "connect socket success");
    CAWakeUpForReadFdsUpdate(svritem->sep.endpoint.addr);
  }
  else
  {
    OIC_LOG(ERROR, TAG, "failed to connect socket");
    goto exit;
  }

  return fd;

exit:
  if (fd >= 0)
  {
    close(fd);
  }
  return -1;
}

static CAResult_t CACreateAcceptSocket()
{
  if (g_acceptServerFD != -1)
  {
    OIC_LOG(DEBUG, TAG, "accept socket created already");
    return CA_STATUS_OK;
  }

  int reuse = 1;
  struct sockaddr_in server = { .sin_addr.s_addr = INADDR_ANY,
    .sin_family = AF_INET,
     .sin_port = htons(SERVER_PORT),
      .sin_zero = { 0 }
  };

  g_acceptServerFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (g_acceptServerFD < 0)
  {
    OIC_LOG(ERROR, TAG, "Failed to create socket");
    goto exit;
  }

  if (-1 == setsockopt(g_acceptServerFD, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
  {
    OIC_LOG(ERROR, TAG, "setsockopt SO_REUSEADDR");
    goto exit;
  }

  int serverlen = sizeof(server);
  if (-1 == bind(g_acceptServerFD, (struct sockaddr *)&server, serverlen))
  {
    OIC_LOG(ERROR, TAG, "bind() error");
    goto exit;
  }

  if (listen(g_acceptServerFD, caglobals.tcp.listenBacklog) != 0)
  {
    OIC_LOG(ERROR, TAG, "listen() error");
    goto exit;
  }

  CHECKFD(g_acceptServerFD);

  return CA_STATUS_OK;

exit:
  if (g_acceptServerFD >= 0)
  {
    close(g_acceptServerFD);
    g_acceptServerFD = -1;
  }
  return CA_STATUS_FAILED;
}

static void CAInitializePipe(int *fds)
{
  int ret = pipe(fds);
  if (-1 != ret)
  {
    ret = fcntl(fds[0], F_GETFD);
    if (-1 != ret)
    {
      ret = fcntl(fds[0], F_SETFD, ret | FD_CLOEXEC);
    }
    if (-1 != ret)
    {
      ret = fcntl(fds[1], F_GETFD);
    }
    if (-1 != ret)
    {
      ret = fcntl(fds[1], F_SETFD, ret | FD_CLOEXEC);
    }
    if (-1 == ret)
    {
      close(fds[1]);
      close(fds[0]);

      fds[0] = -1;
      fds[1] = -1;

      OIC_LOG_V(ERROR, TAG, "pipe failed: %s", strerror(errno));
    }
  }
}

CAResult_t CATCPStartServer(const ca_thread_pool_t threadPool)
{
  if (caglobals.tcp.started)
  {
    return CA_STATUS_OK;
  }

  if (!caglobals.tcp.ipv4tcpenabled)
  {
    caglobals.tcp.ipv4tcpenabled = true;    // only needed to run CA tests
  }

  CAResult_t res = CATCPCreateMutex();
  if (CA_STATUS_OK == res)
  {
    res = CATCPCreateCond();
  }
  if (CA_STATUS_OK != res)
  {
    OIC_LOG(ERROR, TAG, "failed to create mutex/cond");
    return res;
  }

  ca_mutex_lock(g_mutexObjectList);
  if (!caglobals.tcp.svrlist)
  {
    caglobals.tcp.svrlist = u_arraylist_create();
  }
  ca_mutex_unlock(g_mutexObjectList);

  if (caglobals.server)
  {
    res = CACreateAcceptSocket();
    if (CA_STATUS_OK != res)
    {
      OIC_LOG(ERROR, TAG, "failed to create accept socket");
      return res;
    }
  }

  // create pipe for fast shutdown
  CAInitializePipe(caglobals.tcp.shutdownFds);
  CHECKFD(caglobals.tcp.shutdownFds[0]);
  CHECKFD(caglobals.tcp.shutdownFds[1]);

  // create pipe for connection event
  CAInitializePipe(caglobals.tcp.connectionFds);
  CHECKFD(caglobals.tcp.connectionFds[0]);
  CHECKFD(caglobals.tcp.connectionFds[1]);

  caglobals.tcp.terminate = false;
  res = ca_thread_pool_add_task(threadPool, CAReceiveHandler, NULL);
  if (CA_STATUS_OK != res)
  {
    OIC_LOG(ERROR, TAG, "thread_pool_add_task failed");
    return res;
  }
  OIC_LOG(DEBUG, TAG, "CAReceiveHandler thread started successfully.");

  caglobals.tcp.started = true;
  return CA_STATUS_OK;
}

void CATCPStopServer()
{
  // mutex lock
  ca_mutex_lock(g_mutexObjectList);

  // set terminate flag
  caglobals.tcp.terminate = true;

  if (caglobals.tcp.shutdownFds[1] != -1)
  {
    close(caglobals.tcp.shutdownFds[1]);
    // receive thread will stop immediately
  }

  if (caglobals.tcp.connectionFds[1] != -1)
  {
    close(caglobals.tcp.connectionFds[1]);
  }

  if (caglobals.tcp.started)
  {
    ca_cond_wait(g_condObjectList, g_mutexObjectList);
  }
  caglobals.tcp.started = false;

  // mutex unlock
  ca_mutex_unlock(g_mutexObjectList);

  if (-1 != g_acceptServerFD)
  {
    close(g_acceptServerFD);
    g_acceptServerFD = -1;
  }

  CATCPDisconnectAll();
  CATCPDestroyMutex();
  CATCPDestroyCond();
}

void CATCPSetPacketReceiveCallback(CATCPPacketReceivedCallback callback)
{
  g_packetReceivedCallback = callback;
}

void CATCPSetKeepAliveCallback(CATCPKeepAliveHandleCallback keepaliveHandler)
{
  g_keepaliveCallback = keepaliveHandler;
}

static size_t CACheckPayloadLength(const void *data, size_t dlen)
{
  VERIFY_NON_NULL_RET(data, TAG, "data", -1);

  coap_transport_type transport = coap_get_tcp_header_type_from_initbyte(
                                    ((unsigned char *)data)[0] >> 4);

  coap_pdu_t *pdu = coap_new_pdu(transport, dlen);
  if (!pdu)
  {
    OIC_LOG(ERROR, TAG, "outpdu is null");
    return 0;
  }

  int ret = coap_pdu_parse((unsigned char *) data, dlen, pdu, transport);
  if (0 >= ret)
  {
    OIC_LOG(ERROR, TAG, "pdu parse failed");
    coap_delete_pdu(pdu);
    return 0;
  }

  size_t payloadLen = 0;
  size_t headerSize = coap_get_tcp_header_length_for_transport(transport);
  OIC_LOG_V(DEBUG, TAG, "headerSize : %d, pdu length : %d",
            headerSize, pdu->length);
  if (pdu->length > headerSize)
  {
    payloadLen = (unsigned char *) pdu->hdr + pdu->length - pdu->data;
  }

  OICFree(pdu);

  return payloadLen;
}

static void sendData(const CAEndpoint_t *endpoint,
                     const void *data, size_t dlen)
{
  // #1. get TCP Server object from list
  size_t index = 0;
  CATCPSessionInfo_t *svritem = CAGetTCPSessionInfoFromEndpoint(endpoint, &index);
  if (!svritem)
  {
    // if there is no connection info, connect to TCP Server
    svritem = CAConnectTCPSession(endpoint);
    if (!svritem)
    {
      OIC_LOG(ERROR, TAG, "Failed to create TCP server object");
      g_TCPErrorHandler(endpoint, data, dlen, CA_SEND_FAILED);
      return;
    }
  }

  // #2. check payload length
  size_t payloadLen = CACheckPayloadLength(data, dlen);
  // if payload length is zero, disconnect from TCP server
  if (!payloadLen)
  {
    OIC_LOG(DEBUG, TAG, "payload length is zero, disconnect from remote device");
    CADisconnectTCPSession(svritem, index);
    return;
  }

  // #3. check connection state
  if (svritem->fd < 0)
  {
    // if file descriptor value is wrong, remove TCP Server info from list
    OIC_LOG(ERROR, TAG, "Failed to connect to TCP server");
    CADisconnectTCPSession(svritem, index);
    g_TCPErrorHandler(endpoint, data, dlen, CA_SEND_FAILED);
    return;
  }

  // #4. send data to TCP Server
  ssize_t remainLen = dlen;
  do
  {
    ssize_t len = send(svritem->fd, data, remainLen, 0);
    if (-1 == len)
    {
      if (EWOULDBLOCK != errno)
      {
        OIC_LOG_V(ERROR, TAG, "unicast ipv4tcp sendTo failed: %s", strerror(errno));
        g_TCPErrorHandler(endpoint, data, dlen, CA_SEND_FAILED);
        return;
      }
      continue;
    }
    data += len;
    remainLen -= len;
  }
  while (remainLen > 0);

  OIC_LOG_V(INFO, TAG, "unicast ipv4tcp sendTo is successful: %zu bytes", dlen);
}

void CATCPSendData(CAEndpoint_t *endpoint, const void *data, uint32_t datalen,
                   bool isMulticast)
{
  VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");
  VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

  if (!isMulticast)
  {
    if (caglobals.tcp.ipv4tcpenabled && (endpoint->adapter & CA_ADAPTER_TCP))
    {
      sendData(endpoint, data, datalen);
    }
  }
}

CAResult_t CAGetTCPInterfaceInformation(CAEndpoint_t **info, uint32_t *size)
{
  VERIFY_NON_NULL(info, TAG, "info is NULL");
  VERIFY_NON_NULL(size, TAG, "size is NULL");

  return CA_NOT_SUPPORTED;
}

CATCPSessionInfo_t *CAConnectTCPSession(const CAEndpoint_t *endpoint)
{
  VERIFY_NON_NULL_RET(endpoint, TAG, "endpoint is NULL", NULL);

  // #1. create TCP server object
  CATCPSessionInfo_t *svritem = (CATCPSessionInfo_t *) OICCalloc(1, sizeof(*svritem));
  if (!svritem)
  {
    OIC_LOG(ERROR, TAG, "Out of memory");
    return NULL;
  }
  memcpy(svritem->sep.endpoint.addr, endpoint->addr, sizeof(svritem->sep.endpoint.addr));
  svritem->sep.endpoint.port = endpoint->port;

  // #2. create the socket and connect to TCP server
  if (caglobals.tcp.ipv4tcpenabled)
  {
    int fd = CATCPCreateSocket(AF_INET, svritem);
    if (-1 == fd)
    {
      OICFree(svritem);
      return NULL;
    }

    // #3. add TCP connection info to list
    svritem->fd = fd;
    ca_mutex_lock(g_mutexObjectList);
    if (caglobals.tcp.svrlist)
    {
      bool res = u_arraylist_add(caglobals.tcp.svrlist, svritem);
      if (!res)
      {
        OIC_LOG(ERROR, TAG, "u_arraylist_add failed.");
        close(svritem->fd);
        OICFree(svritem);
        ca_mutex_unlock(g_mutexObjectList);
        return NULL;
      }
    }
    ca_mutex_unlock(g_mutexObjectList);

    CHECKFD(fd);

    // pass the connection information to RI for keepalive.
    if (g_keepaliveCallback)
    {
      g_keepaliveCallback(svritem->sep.endpoint.addr, svritem->sep.endpoint.port, true);
    }
  }

  return svritem;
}

CAResult_t CADisconnectTCPSession(CATCPSessionInfo_t *svritem, size_t index)
{
  VERIFY_NON_NULL(svritem, TAG, "svritem is NULL");

  ca_mutex_lock(g_mutexObjectList);

  // close the socket and remove TCP connection info in list
  if (svritem->fd >= 0)
  {
    close(svritem->fd);
  }
  u_arraylist_remove(caglobals.tcp.svrlist, index);
  OICFree(svritem->recvData);
  OICFree(svritem);
  ca_mutex_unlock(g_mutexObjectList);

  // pass the connection information to RI for keepalive.
  if (g_keepaliveCallback)
  {
    g_keepaliveCallback(svritem->sep.endpoint.addr, svritem->sep.endpoint.port, false);
  }

  return CA_STATUS_OK;
}

void CATCPDisconnectAll()
{
  ca_mutex_lock(g_mutexObjectList);
  uint32_t length = u_arraylist_length(caglobals.tcp.svrlist);

  CATCPSessionInfo_t *svritem = NULL;
  for (size_t i = 0; i < length; i++)
  {
    svritem = (CATCPSessionInfo_t *) u_arraylist_get(caglobals.tcp.svrlist, i);
    if (svritem && svritem->fd >= 0)
    {
      shutdown(svritem->fd, SHUT_RDWR);
      close(svritem->fd);
      OICFree(svritem->recvData);
    }
  }
  u_arraylist_destroy(caglobals.tcp.svrlist);
  ca_mutex_unlock(g_mutexObjectList);
}

CATCPSessionInfo_t *CAGetTCPSessionInfoFromEndpoint(const CAEndpoint_t *endpoint,
    size_t *index)
{
  VERIFY_NON_NULL_RET(endpoint, TAG, "endpoint is NULL", NULL);
  VERIFY_NON_NULL_RET(index, TAG, "index is NULL", NULL);

  // get connection info from list
  uint32_t length = u_arraylist_length(caglobals.tcp.svrlist);
  for (size_t i = 0; i < length; i++)
  {
    CATCPSessionInfo_t *svritem = (CATCPSessionInfo_t *) u_arraylist_get(
                                    caglobals.tcp.svrlist, i);
    if (!svritem)
    {
      continue;
    }

    if (!strncmp(svritem->sep.endpoint.addr, endpoint->addr,
                 sizeof(svritem->sep.endpoint.addr))
        && (svritem->sep.endpoint.port == endpoint->port))
    {
      *index = i;
      return svritem;
    }
  }

  return NULL;
}

CATCPSessionInfo_t *CAGetSessionInfoFromFD(int fd, size_t *index)
{
  ca_mutex_lock(g_mutexObjectList);

  // check from the last item.
  CATCPSessionInfo_t *svritem = NULL;
  uint32_t length = u_arraylist_length(caglobals.tcp.svrlist);
  for (size_t i = 0; i < length; i++)
  {
    svritem = (CATCPSessionInfo_t *) u_arraylist_get(caglobals.tcp.svrlist, i);

    if (svritem && svritem->fd == fd)
    {
      *index = i;
      ca_mutex_unlock(g_mutexObjectList);
      return svritem;
    }
  }

  ca_mutex_unlock(g_mutexObjectList);

  return NULL;
}

size_t CAGetTotalLengthFromHeader(const unsigned char *recvBuffer)
{
  OIC_LOG(DEBUG, TAG, "IN - CAGetTotalLengthFromHeader");

  coap_transport_type transport = coap_get_tcp_header_type_from_initbyte(
                                    ((unsigned char *)recvBuffer)[0] >> 4);
  size_t optPaylaodLen = coap_get_length_from_header((unsigned char *)recvBuffer,
                         transport);
  size_t headerLen = coap_get_tcp_header_length((unsigned char *)recvBuffer);

  OIC_LOG_V(DEBUG, TAG, "option/paylaod length [%d]", optPaylaodLen);
  OIC_LOG_V(DEBUG, TAG, "header length [%d]", headerLen);
  OIC_LOG_V(DEBUG, TAG, "total data length [%d]", headerLen + optPaylaodLen);

  OIC_LOG(DEBUG, TAG, "OUT - CAGetTotalLengthFromHeader");
  return headerLen + optPaylaodLen;
}

void CATCPSetErrorHandler(CATCPErrorHandleCallback errorHandleCallback)
{
  g_TCPErrorHandler = errorHandleCallback;
}
