/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef CA_MANAGER_DEVICE_H_
#define CA_MANAGER_DEVICE_H_

#include "uarraylist.h"
#include "octhread.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Auto Connection Target Device Identity
 */
typedef struct
{
    char address[18];      /**< remote address */
    bool isAutoConnect;   /**< auto connection flag */
} CAManagerACData_t;

/**
 * get address from auto connection list.
 * @param[in]   idx                   index of auto connection data list.
 * @return  address
 */
char* CAManagerGetLEAddressFromACData(size_t idx);

/**
 * create auto connection list.
 */
void CAManagerCreateACDataList();

/**
 * initialize mutex.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerInitMutexVaraibles();

/**
 * terminate mutex.
 */
void CAManagerTerminateMutexVariables();

/**
 * check whether target address is already contained in list or not.
 * @param[in]   address              ble address.
 * @return  true or false
 */
bool CAManagerIsMatchedACData(const char* address);

/**
 * add auto connection data into list.
 * @param[in]   address              ble address.
 */
void CAManagerAddACData(const char* address);

/**
 * remove auto connection data for selected ble address.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   address               ble address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerRemoveData(const char* address);

/**
 * remove auto connection data for all devices.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerRemoveAllData();

/**
 * get current auto connection flag.
 * @param[in]   address              ble address.
 * @return  true or false
 */
bool CAManagerGetAutoConnectionFlag(const char* address);

/**
 * set auto connection flag.
 * @param[in]   address              ble address.
 * @param[in]   flag                  auto connection flag.
 */
void CAManagerSetAutoConnectionFlag(const char* address, bool flag);

/**
 * get length of auto connection list.
 * @return  list size
 */
size_t CAManagerGetACDataLength();

/**
 * set BT adapter recovery flag.
 * @param[in]   flag                  recovery flag.
 */
void CAManagerSetBTRecovery(bool flag);

/**
 * get BT adapter recovery flag.
 * @return  recovery flag.
 */
bool CAManagerIsRecoveryFlagSet();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_MANAGER_DEVICE_H_ */

