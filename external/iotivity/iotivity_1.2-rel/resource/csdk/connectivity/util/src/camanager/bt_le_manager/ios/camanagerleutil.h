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

#ifndef CA_MANAGER_LE_UTILS_H_
#define CA_MANAGER_LE_UTILS_H_

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    CM_CONTAINS = 1,
    CM_ADD,
    CM_REMOVE
} CASetMethod_t;

/**
 * check whether the device is already put or not.
 * @param[in]   address               LE address.
 * @return  true or false
 */
bool CAManagerIsConnectedDeviceAddress(const char* address);

/**
 * add connected device into SharedPreference.
 * @param[in]   address               ble address.
 * @return  CBPeripheral object or nil
 */
CBPeripheral* CAManagerGetValueConnectedDeviceAddress(const char* address);

/**
 * add connected device into SharedPreference.
 * @param[in]   peripheral            peripheral object.
 * @param[in]   address               ble address.
 * @return  true or false
 */
bool CAManagerSetValueConnectedDeviceAddress(CBPeripheral *peripheral, const char* address);

/**
 * add connected device into SharedPreference.
 * @param[in]   address               ble address.
 * @return  true or false
 */
bool CAManagerAddConnectedDeviceAddress(const char* address);

/**
 * get connected device from SharedPreference.
 * @return  true or false
 */
bool CAManagerGetConnectedDeviceAddress();

/**
 * remove connected device to SharedPreference.
 * @param[in]   address               LE address.
 * @return  true or false
 */
bool CAManagerRemoveConnectedDeviceAddress(const char* address);

/**
 * Vaildate a String bluetooth address.
 * @param[in]   address               LE address.
 * @return  true or false
 */
bool CAManagerCheckBTAddress(const char *address);

/**
 * create new set<String> object.
 * @return  true or false
 */
bool CAManagerCreateSetString();

/**
 * terminate new set<String> object.
 * @return  CA_STATUS_OK or CA_STATUS_FAILED
 */
CAResult_t CAManagerTerminateSetString();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_MANAGER_LE_UTILS_H_ */
