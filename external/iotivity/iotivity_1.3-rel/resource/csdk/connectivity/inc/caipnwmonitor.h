/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/**
 * @file caipnwmonitor.h
 * This file provides APIs IP network monitor modules.
 */

#ifndef CA_IP_NW_INTERFACE_H_
#define CA_IP_NW_INTERFACE_H_

#include "cacommon.h"
#include "cathreadpool.h"
#include "uarraylist.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define INTERFACE_NAME_MAX 16

/**
 * Callback to be notified when IP adapter network state changes.
 *
 * @param[in]  adapter      Transport adapter.
 * @param[in]  status       Connection status either ::CA_INTERFACE_UP or ::CA_INTERFACE_DOWN.
 * @see CAIPSetConnectionStateChangeCallback() for registration.
 */
typedef void (*CAIPAdapterStateChangeCallback)(CATransportAdapter_t adapter,
                                               CANetworkStatus_t status);

/*
 * Structure for IP address information, to be used to construct a CAEndpoint_t.  The
 * structure name is a misnomer, as there is one entry per address not one per interface.
 * An interface with 4 addresses should result in 4 instances of CAInterface_t.
 */
typedef struct
{
    char name[INTERFACE_NAME_MAX];
    uint32_t index;
    uint32_t flags;
    uint16_t family;
    char addr[MAX_ADDR_STR_SIZE_CA];
} CAInterface_t;

typedef struct CAIPCBData_t
{
    struct CAIPCBData_t *next;
    CATransportAdapter_t adapter;
    CAIPAdapterStateChangeCallback callback;
} CAIPCBData_t;

/**
 * Set callback for receiving local IP/TCP adapter connection status.
 *
 * @param[in]  callback     Callback to be notified when IP/TCP adapter connection state changes.
 * @param[in]  adapter      Transport adapter.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAIPSetNetworkMonitorCallback(CAIPAdapterStateChangeCallback callback,
                                         CATransportAdapter_t adapter);

/**
 * Unset callback for receiving local IP/TCP adapter connection status.
 *
 * @param[in]  adapter      Transport adapter.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAIPUnSetNetworkMonitorCallback(CATransportAdapter_t adapter);

/**
 * Get a list of CAInterface_t items.
 *
 * @param[in]  desiredIndex      Network interface index.
 * @return  List of CAInterface_t items.
 */
u_arraylist_t *CAIPGetInterfaceInformation(int desiredIndex);

/**
 * Find network interface changes.
 *
 * @return  List of CAInterface_t items.
 */
u_arraylist_t *CAFindInterfaceChange();

/**
 * Start network monitor.
 *
 * @param[in]  callback     Callback to be notified when IP/TCP adapter connection state changes.
 * @param[in]  adapter      Transport adapter.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAIPStartNetworkMonitor(CAIPAdapterStateChangeCallback callback,
                                   CATransportAdapter_t adapter);

/**
 * Stops network monitor.
 *
 * @param[in]  adapter      Transport adapter.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAIPStopNetworkMonitor(CATransportAdapter_t adapter);

/**
 * Apply network interface changes.
 *
 * @param[in]  ifchanged      Changed interface.
 */
void CAProcessNewInterface(CAInterface_t *ifchanged);

/**
 * This function return link local zone id related from ifindex.
 *
 * @param ifindex[in]     interface index.
 * @param zoneId[out]     pointer of zoneId string, caller should free
 *                        zoneId using OICFree() when it returned CA_STATUS_OK.
 * @return  ::CA_STATUS_OK or ::CA_STATUS_INVALID_PARAM
 */
CAResult_t CAGetLinkLocalZoneIdInternal(uint32_t ifindex, char **zoneId);

#ifdef __cplusplus
}
#endif

#endif /* CA_IP_NW_INTERFACE_H_ */
