/* *****************************************************************
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
 * @file
 * This file contains common function for connection manager util
 */

#ifndef CA_MANAGER_UTIL_H_
#define CA_MANAGER_UTIL_H_

#include "cacommon.h"

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * Information related to endpoint.
 * Used for switching endpoint information.
 */
typedef struct
{
    CATransportAdapter_t adapter;
    CATransportFlags_t flags;
    char *addr;
    uint16_t udpPort;
    uint16_t tcpPort;
} CMIpInfo_t;

/**
 * D2S, D2D information about specific di.
 */
typedef struct
{
    char *deviceId;
    CMIpInfo_t d2dInfo;
    CMIpInfo_t d2sInfo;
} CMDeviceInfo_t;

/**
 * List structure for CMDeviceInfo_t.
 */
typedef struct CMDeviceInfoList
{
    CMDeviceInfo_t *deviceInfo;
    struct CMDeviceInfoList *next;
} CMDeviceInfoList_t;

/**
 * This function is used to add the DeviceInfo structure to IP connection info table.
 *
 * @param[in,out] devInfoList   Pointer to IP connection info table which contains
 *                              D2D,D2S ip related informaions.
 * @param[in]     endpoint      remote Endpoint to be added in IP connection info table.
 *
 * @return :: CA_STATUS_OK or Appropriate error code.
 * @retval :: CA_STATUS_OK  Successful.
 * @retval :: CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAMgrUtilAddDevInfoToList(CMDeviceInfoList_t **devInfoList,
                                     const CAEndpoint_t *endpoint);

/**
 * This function is used to get the DeviceInfo which have specific device id
 * from the table.
 *
 * @param[in]  devInfoList  Pointer to IP connection info table which contains
 *                          D2D,D2S ip related informaions.
 * @param[in]  deviceId     Unique remote device id to access IP connection info
 *                          table.
 * @param[out] devInfo      Table information obtained with a specific device id.
 *
 * @return :: CA_STATUS_OK or Appropriate error code.
 * @retval :: CA_STATUS_OK  Successful.
 * @retval :: CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAMgrUtilGetDevInfo(CMDeviceInfoList_t *devInfoList, const char *deviceId,
                               CMDeviceInfo_t **devInfo);

/**
 * This function is used to update IP connection table component which have specific device id.
 *
 * @param[in,out] devInfoList   Pointer to IP connection info table which contains
 *                              D2D,D2S ip related informaions.
 * @param[in]     endpoint      remote Endpoint to be updated in IP connection info table.
 *
 * @return :: CA_STATUS_OK or Appropriate error code.
 * @retval :: CA_STATUS_OK  Successful.
 * @retval :: CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAMgrUtilUpdateDevInfo(CMDeviceInfoList_t *devInfoList,
                                  const CAEndpoint_t *endpoint);

/**
 * This function is used to reset all D2D informations.
 *
 * @param[in,out] devInfoList   Pointer to IP connection info table which contains
 *                              D2D,D2S ip related informaions.
 *
 * @return :: CA_STATUS_OK or Appropriate error code.
 * @retval :: CA_STATUS_OK  Successful.
 * @retval :: CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAMgrUtilResetDevInfo(CMDeviceInfoList_t *devInfoList);

/**
 * This function is used to clear IP connection info table.
 *
 * @param[in,out] devInfoList   Pointer to IP connection info table which contains
 *                              D2D,D2S ip related informaions.
 */
void CAMgrUtilDestroyDevInfoList(CMDeviceInfoList_t *devInfoList);

/**
 *  This function is used to get a device id from uri.
 *
 *  @param[in] uri
 *
 *  @return :: device id
 */
char* CAMgrUtilGetDeviceId(char *uri);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // CA_MANAGER_UTIL_H_
