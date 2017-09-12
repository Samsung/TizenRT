/******************************************************************
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
#include "catcpadapterutils_eth.h"

#include <Arduino.h>
#include <Ethernet.h>
#include <socket.h>
#include <w5100.h>
#include <EthernetUdp.h>
#include <IPAddress.h>

#include "logger.h"
#include "cacommon.h"
#include "caadapterinterface.h"
#include "caadapterutils.h"

#define TAG "IPU"

CAResult_t CAArduinoGetAvailableTCPSocket(int *sockID)
{
    VERIFY_NON_NULL(sockID, TAG, "sockID");
    //Is any socket available to work with ?
    *sockID = 0;
    for (int i = 1; i < MAX_SOCK_NUM; i++)
    {
        uint8_t state = W5100.readSnSR(i);
        if (state == SnSR::CLOSED || state == SnSR::FIN_WAIT)
        {
            *sockID = i;
            break;
        }
    }

    if (*sockID == 0)
    {
        OIC_LOG(ERROR, TAG, "sockID 0");
        return CA_SOCKET_OPERATION_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CAArduinoInitTCPSocket(uint16_t *port, int *socketID)
{
    OIC_LOG(DEBUG, TAG, "IN");
    VERIFY_NON_NULL(port, TAG, "port");
    VERIFY_NON_NULL(socketID, TAG, "socketID");

    CAResult_t ret = CAArduinoGetAvailableTCPSocket(socketID);
    if (ret != CA_STATUS_OK)
    {
        OIC_LOG(ERROR, TAG, "get sock fail");
        return ret;
    }

    //Create a tcp socket on which to recv/send.
    if (!socket(*socketID, SnMR::TCP, *port, 0))
    {
        OIC_LOG(ERROR, TAG, "sock fail");
        return CA_STATUS_FAILED;
    }
    OIC_LOG_V(DEBUG, TAG, "socketId:%d", *socketID);
    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}
