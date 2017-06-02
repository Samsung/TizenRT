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

/**
 * @file
 *
 * This file contains the util function for LE adapter. This maintains the
 * list of services an individual GATT Client connected to and operations on
 * that list, such as getting the service info with BD address or with
 * position etc. This is mainly useful for the multicast transmission of
 * data where client needs to have the info of all the services to which it
 * is connected.
 */

#ifndef TZ_BLE_UTIL_H_
#define TZ_BLE_UTIL_H_

#include <bluetooth.h>

#include "cacommon.h"

typedef struct
{
    bt_gatt_client_h clientHandle;
    bt_gatt_h serviceHandle;
    bt_gatt_h readChar;
    bt_gatt_h writeChar;
    char *remoteAddress;
} LEServerInfo;

typedef struct _LEServerInfoList
{
    LEServerInfo *serverInfo;
    struct _LEServerInfoList *next;
}LEServerInfoList;

typedef struct _LEClientInfoList
{
    char *remoteAddress;
    struct _LEClientInfoList *next;
} LEClientInfoList;

/**
 * Different characteristics types.
 *
 * This provides information of different characteristics
 * which will be added to OIC service.
 */
typedef enum
{
    BLE_GATT_WRITE_CHAR = 0, /**< write_char This will be used to get the unicast response. */
    BLE_GATT_READ_CHAR,      /**< read_char This will be used update value to OIC server. */
    BLE_GATT_NOTIFY_CHAR     /**< Reserved char for the time being. */
} CHAR_TYPE;

/**
 * Used to increment the registered service count.
 */
void CAIncrementRegisteredServiceCount();

/**
 * Used to decrement the registered service count.
 */
void CADecrementRegisteredServiceCount();

/**
 * Used to reset the registered service count.
 */
void CAResetRegisteredServiceCount();

/**
 * Used to get the total registered service count.
 * @return  Total registered service count.
 */
int32_t  CAGetRegisteredServiceCount();

/**
 * Used to add the serverListInfo structure to the Server List.
 *
 * @param[in] serverList     Pointer to the ble server list which holds the info of list of
 *                           servers registered by the client.
 * @param[in] leServerInfo   Pointer where serverInfo structure needs to be appended with
 *                           serverList.
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAAddLEServerInfoToList(LEServerInfoList **serverList,
                                   LEServerInfo *leServerInfo);

/**
 * Used to remove the serverListInfo structure from the Server List.
 *
 * @param[in,out] serverList     Pointer to the ble server list which holds the info of list of
 *                               servers registered by the client.
 * @param[in]     remoteAddress  Remote address to be removed from the client list.
 */
void CARemoveLEServerInfoFromList(LEServerInfoList **serverList,
                                        const char *remoteAddress);

/**
 * Used to get the serviceInfo from the list.
 *
 * @param[in]  serverList       Pointer to the ble service list which holds the info of list
 *                              of servers registered by the client.
 * @param[in]  leAddress        BD address of the device where GATTServer information is required.
 * @param[out] leServerInfo     Info of service and characteristic handle of the given BD address
 *                              registered by client.
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAGetLEServerInfo(LEServerInfoList *serverList, const char *leAddress,
                             LEServerInfo **leServerInfo);

/**
 * Used to get the clientInfo from the list by position.
 *
 * @param[in]  serverList      Pointer to the ble service list which holds the info of list
 *                             of servers registered by the client.
 * @param[in]  position        The service information of particular position in the list.
 * @param[out] leServerInfo    Info of service and characteristic handle of the given BD address
 *                              registered by client.
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAGetLEServerInfoByPosition(LEServerInfoList *serverList, int32_t position,
                                       LEServerInfo **leServerInfo);

/**
 * Used to clear BLE service list.
 *
 * @param[in]  serverList   Pointer to the ble service list which holds the info of list of
 *                          servers registered by the client.
 */
void CAFreeLEServerList(LEServerInfoList *serverList);

/**
 * @brief Used to get remove particular BLE service info from list
 * @param[in] bleServerInfo Pointer to the structure which needs to be cleared.
 */
void CAFreeLEServerInfo(LEServerInfo *bleServerInfo);

/**
 * Used to add the client address to the Client List.
 *
 * @param[in] clientList     Pointer to the ble client list which holds the info of list of
 *                           clients connected to the server.
 * @param[in] clientAddress  Client remote address.
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAAddLEClientInfoToList(LEClientInfoList **clientList,
                                   char *clientAddress);

/**
 * Used to remove the client address from the Client List.
 *
 * @param[in,out] clientList     Pointer to the ble client list which holds the info of list of
 *                               clients connected by the server.
 * @param[in]     clientAddress  Remote address to be removed from the client list.
 */
void CARemoveLEClientInfoFromList(LEClientInfoList **clientList,
                                        const char *clientAddress);

/**
 * Used to disconnect all the clients connected to the server.
 *
 * @param[in,out] clientList     Pointer to the ble client list which holds the info of list of
 *                               clients connected by the server.
 */
void CADisconnectAllClient(LEClientInfoList *clientList);

/**
 * Used to get the Error message.
 * @param[in] err   Error code(::bt_error_e).
 * @return  Error string corresponding to the BT error code.
 */
const char *CALEGetErrorMsg(bt_error_e err);

#endif /* TZ_BLE_UTIL_H_ */
