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

#include <stdio.h>
#include <errno.h>

#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>
#include <w5100.h>
#include <IPAddress.h>

#include "catcpadapterutils_eth.h"
#include "catcpinterface.h"
#include <coap/pdu.h>
#include "caadapterutils.h"
#include "octhread.h"
#include "oic_malloc.h"
#include "oic_string.h"

/**
 * Logging tag for module name.
 */
#define TAG "TCP_SERVER"

/**
 * Maximum CoAP over TCP header length
 * to know the total data length.
 */
#define TCP_MAX_HEADER_LEN  6

#define TCP_UNICAST_PORT  55556

uint16_t tcpClientPort = 0;
uint8_t tcpClientIP[4] = {0};

/**
 * Maintains the callback to be notified when data received from remote device.
 */
static CATCPPacketReceivedCallback g_packetReceivedCallback = NULL;

/**
 * Error callback to update error in TCP.
 */
static CATCPErrorHandleCallback g_TCPErrorHandler = NULL;

static uint16_t g_unicastPort = 0;

static int g_unicastSocket = -1;

static EthernetServer server(TCP_UNICAST_PORT);

/**
 * @var g_receivedDataLen
 * @brief Actual length of data received.
 */
static uint32_t g_receivedDataLen = 0;

static void CATCPReadDataInternal();

uint16_t CAGetTcpServerPortNum(const char *ipAddress, bool isSecured)
{
    return g_unicastPort;
}

void CATCPPullData()
{
    CATCPReadDataInternal();
}

CAResult_t CATCPStartServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    if (caglobals.tcp.started)
    {
        return CA_STATUS_OK;
    }

    if (!caglobals.tcp.ipv4tcpenabled)
    {
        caglobals.tcp.ipv4tcpenabled = true;    // only needed to run CA tests
    }

    uint8_t rawIPAddr[4] = {0};
    char address[16] = {0};
    W5100.getIPAddress(rawIPAddr);
    int ret = snprintf(address, sizeof(address), "%d.%d.%d.%d", rawIPAddr[0], rawIPAddr[1], rawIPAddr[2],
                       rawIPAddr[3]);
    if(0 > ret)
    {
        OIC_LOG(ERROR, TAG, "Error getting IP");
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, TAG, "address:%s", address);
    int serverFD = 1;
    g_unicastPort = TCP_UNICAST_PORT;

    if (CAArduinoInitTCPSocket(&g_unicastPort, &serverFD) != CA_STATUS_OK)
    {
        OIC_LOG(DEBUG, TAG, "Init TCP Socket failed");
        return CA_STATUS_FAILED;
    }

    listen(serverFD);
    EthernetClass::_server_port[serverFD] = g_unicastPort;
    g_unicastSocket = serverFD;
    caglobals.ip.u4.port =  TCP_UNICAST_PORT;

    caglobals.tcp.terminate = false;
    caglobals.tcp.started = true;

    OIC_LOG_V(DEBUG, TAG, "g_unicastPort: %u", g_unicastPort);
    OIC_LOG_V(DEBUG, TAG, "g_unicastSocket: %d", g_unicastSocket);
    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CATCPStopServer()
{
    OIC_LOG(DEBUG, TAG, "IN");
    caglobals.tcp.terminate = true;
    caglobals.tcp.started = false;

    if (-1 != g_unicastSocket)
    {
        close(g_unicastSocket);
        g_unicastSocket = -1;
    }

    caglobals.ip.u4.port = 0;
    OIC_LOG(DEBUG, TAG, "OUT");
}

void CATCPSetPacketReceiveCallback(CATCPPacketReceivedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");

    g_packetReceivedCallback = callback;

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CATCPSetConnectionChangedCallback(CATCPConnectionHandleCallback connHandler)
{
    return;
}

void CATCPSetErrorHandler(CATCPErrorHandleCallback errorHandleCallback)
{
    return;
}

void CATCPReadDataInternal()
{
    OIC_LOG(DEBUG, TAG, "CATCPReadDataInternal IN");
    if (false == caglobals.tcp.started)
    {
        OIC_LOG(ERROR, TAG, "Server is not running");
        return;
    }

    EthernetClient client = server.available();

    if (client && client.connected())
    {
        client.getTCPClientIP(tcpClientIP);
        client.getTCPClientPort(&tcpClientPort);
        OIC_LOG_V(DEBUG, TAG, "Client ip is %d.%d.%d.%d , Port is : %d", tcpClientIP[0],
                tcpClientIP[1], tcpClientIP[2], tcpClientIP[3], tcpClientPort);

        OIC_LOG(DEBUG, TAG, "DATA Available");
        unsigned char *recvBuffer = NULL;
        size_t bufSize = TCP_MAX_HEADER_LEN;

        recvBuffer = (unsigned char *) OICCalloc(1, bufSize);
        if (!recvBuffer)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
        }

        bool isHeaderChecked = false;
        size_t totalLen = 0;
        size_t totalReceivedLen = 0;

        do
        {
            size_t bufferLen = 0;
            if (client.available())
            {
                bufferLen = client.read(recvBuffer, bufSize);
                totalReceivedLen += bufferLen;
            }

            if (!isHeaderChecked && totalReceivedLen)
            {
                coap_transport_t transport;
                size_t headerLen;
                CAGetTCPHeaderDetails(recvBuffer, &transport, &headerLen);
                if (totalReceivedLen >= headerLen)
                {
                    // get actual data length from coap over tcp header
                    totalLen = CAGetTotalLengthFromPacketHeader((const unsigned char *) recvBuffer,
                                                                 bufferLen);
                    bufSize = totalLen;
                    unsigned char *newBuf = (unsigned char *) OICRealloc(recvBuffer, bufSize);
                    if (NULL == newBuf)
                    {
                        OIC_LOG(ERROR, TAG, "out of memory");
                        OICFree(recvBuffer);
                        return;
                    }

                    recvBuffer = newBuf;
                    isHeaderChecked = true;
                }
            }

            if (totalLen == totalReceivedLen)
            {
                CASecureEndpoint_t ep =
                    {.endpoint = {.adapter = CA_ADAPTER_TCP,
                                  .flags = CA_DEFAULT_FLAGS,
                                  .port = tcpClientPort}};

                IPAddress clientIP = tcpClientIP;
                int ret = snprintf(ep.endpoint.addr, sizeof(ep.endpoint.addr), "%d.%d.%d.%d",
                                   clientIP[0], clientIP[1], clientIP[2], clientIP[3]);
                if(0 > ret)
                {
                    OIC_LOG(ERROR, TAG, "Error parsing client IP");
                    OICFree(recvBuffer);
                    return;
                }

                OIC_LOG_V(DEBUG, TAG, "Client ip is %d.%d.%d.%d", clientIP[0],
                          clientIP[1], clientIP[2], clientIP[3]);
                if (g_packetReceivedCallback)
                {
                    g_packetReceivedCallback(&ep, recvBuffer, totalLen);
                }

                OIC_LOG_V(DEBUG, TAG, "received data len:%d", totalLen);
                break;
            }
        } while (!totalLen || totalLen > totalReceivedLen);

        OICFree(recvBuffer);

        OIC_LOG(DEBUG, TAG, "OUT");
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "NoData");
    }
    return;
}

CAResult_t CAGetTCPInterfaceInformation(CAEndpoint_t **info, uint32_t *size)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(info, TAG, "info is NULL");
    VERIFY_NON_NULL(size, TAG, "size is NULL");

    return CA_NOT_SUPPORTED;
}

static ssize_t sendData(const CAEndpoint_t *endpoint,
                        const void *data, size_t dlen)
{
    uint16_t port = endpoint->port;
    uint8_t ipAddr[4] = { 0 };
    uint16_t parsedPort = 0;
    if (CAParseIPv4AddressInternal(endpoint->addr, ipAddr, sizeof(ipAddr),
                                   &parsedPort) != CA_STATUS_OK)
    {
        OIC_LOG(ERROR, TAG, "parse fail");
        return -1;
    }

    if (dlen > 65535) // Max value for uint16_t
    {
        // This will never happen as max buffer size we are dealing with is COAP_MAX_PDU_SIZE
        OIC_LOG(ERROR, TAG, "Size exceeded");
        return -1;
    }

    uint32_t ret = send(g_unicastSocket, (const uint8_t *)data, (uint16_t)dlen);
    if (ret <= 0)
    {
        OIC_LOG_V(ERROR, TAG, "SendData failed: %d", ret);
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return ret;
}

ssize_t CATCPSendData(CAEndpoint_t *endpoint, const void *data, size_t datalen)
{
    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");
    VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

    if (caglobals.tcp.ipv4tcpenabled && (endpoint->adapter & CA_ADAPTER_TCP))
    {
        return sendData(endpoint, data, datalen);
    }
    return -1;
}
