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


//logger.h included first to avoid conflict with RBL library PROGMEM attribute
#include "logger.h"

#include "cableserver.h"

#include <Arduino.h>
#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>

#include <coap/pdu.h>
#include "caleinterface.h"
#include "oic_malloc.h"
#include "caadapterutils.h"

#include "cafragmentation.h"

#define TAG "LES"
/**
 * Maximum TCP header length.
 */
#define TCP_MAX_HEADER_LENGTH 6

/**
 * Maintains the callback to be notified on receival of network packets from other
 * BLE devices
 */
static CABLEDataReceivedCallback g_bleServerDataReceivedCallback = NULL;

/**
 * Maximum number of tries to get the event on BLE Shield address.
 */
#define MAX_EVENT_COUNT 20

static bool g_serverRunning = false;
static uint8_t *g_coapBuffer = NULL;

/**
 * Actual length of data received.
 */
static size_t g_receivedDataLen = 0;

/**
 * Total Length of data that is being fragmented.
 */
static size_t g_packetDataLen = 0;

void CAGetTCPHeaderDetails(unsigned char* recvBuffer, size_t *headerlen)
{
    coap_transport_t transport = coap_get_tcp_header_type_from_initbyte(
        ((unsigned char *)recvBuffer)[0] >> 4);
    *headerlen = coap_get_tcp_header_length_for_transport(transport);
}

void CACheckLEDataInternal()
{
    CALEDoEvents();

    if (CAIsLEDataAvailable())
    {
        if (NULL == g_coapBuffer)
        {
            OIC_LOG(DEBUG, TAG, "IN");
            size_t bufSize = CA_SUPPORTED_BLE_MTU_SIZE;
            g_coapBuffer = (uint8_t *)OICCalloc(bufSize, 1);
            if (NULL == g_coapBuffer)
            {
                OIC_LOG(ERROR, TAG, "malloc");
                return;
            }

            while (CAIsLEDataAvailable() && g_receivedDataLen <= bufSize)
            {
                g_coapBuffer[g_receivedDataLen++] = CALEReadData();
            }

            OIC_LOG(DEBUG, TAG, "Read Comp BLE Pckt");

            if (g_receivedDataLen > 0)
            {
                OIC_LOG_V(DEBUG, TAG, "recv dataLen=%u", g_receivedDataLen);
                uint32_t sentLength = 0;
                g_bleServerDataReceivedCallback("", g_coapBuffer,
                                                g_receivedDataLen, &sentLength);
            }

            g_receivedDataLen = 0;
            OICFree(g_coapBuffer);
            g_coapBuffer = NULL;
        }
        OIC_LOG(DEBUG, TAG, "OUT");
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "NoData");
    }
    return;
}

CAResult_t CALEInitialize()
{
    OIC_LOG(DEBUG, TAG, "IN");

    // Set your BLE Shield name here, max. length 10
    ble_set_name(__OIC_DEVICE_NAME__);

    OIC_LOG(DEBUG, TAG, "LEName Set");

    ble_begin();

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CASetLEServerThreadPoolHandle(ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, TAG, "IN");
    OIC_LOG(DEBUG, TAG, "OUT");
}

void CASetBLEServerErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");
    OIC_LOG(DEBUG, TAG, "OUT");
}

unsigned char CAIsLEDataAvailable()
{
    return ble_available();
}

unsigned char CAIsLEConnected()
{
    return ble_connected();
}

uint8_t CALEReadData()
{
    return (uint8_t)ble_read();
}

CAResult_t CALEDoEvents()
{
    ble_do_events();
    return CA_STATUS_OK;
}

CAResult_t CAUpdateCharacteristicsToAllGattClients(const uint8_t *char_value,
                                                   uint32_t valueLength)
{
    // ble_write_bytes() api can send only max of 255 bytes at a time
    // This function shall never be called to send more than 255 bytes by the fragmentation logic.
    // Currently ble_write_bytes api returns void.
    ble_write_bytes((unsigned char *)char_value, (unsigned char)valueLength);
    return CA_STATUS_OK;
}

void CASetLEReqRespServerCallback(CABLEDataReceivedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");
    g_bleServerDataReceivedCallback = callback;
    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CAStartLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");
    CAResult_t result = CALEInitialize();
    if (CA_STATUS_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "ble init fail: %d", result);
        return CA_STATUS_FAILED;
    }
    /**
     * Below for loop is to process the BLE Events received from BLE Shield.
     * BLE Events includes BLE Shield Address Added as a patch to RBL Library.
     */
    for (int iter = 0; iter < MAX_EVENT_COUNT; iter++)
    {
        CACheckLEDataInternal();
    }

    g_serverRunning = true;
    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAStopLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");
    CATerminateLEGattServer();
    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAInitializeLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "Initialize GATT Server");
    return CA_STATUS_OK;
}

void CATerminateLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");
    if (true == g_serverRunning)
    {
        ble_radio_reset();
        g_serverRunning = false;
    }
    OIC_LOG(DEBUG, TAG, "OUT");
    return;
}

void CACheckLEData()
{
    if (false == g_serverRunning)
    {
        OIC_LOG(ERROR, TAG, "Server is not running");
        return;
    }
    CACheckLEDataInternal();
}
