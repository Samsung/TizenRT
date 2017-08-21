/* ****************************************************************
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

#ifndef CA_MANAGER_DEVICE_H_
#define CA_MANAGER_DEVICE_H_

#include <jni.h>
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
    jstring address;         /**< remote address */
    bool isAutoConnecting;   /**< whether GATT connection has been in progress */
} CAManagerACData_t;

/**
 * get address from auto connection list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   idx                   index of auto connection data list.
 * @return  address
 */
jstring CAManagerGetLEAddressFromACData(JNIEnv *env, size_t idx);

/**
 * create auto connection list.
 */
void CAManagerCreateACDataList();

/**
 * Destroy auto connection list.
 */
void CAManagerDestroyACDataList();

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
 * check whether target address is already contained in ACData list or not.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jaddress              ble address.
 */
bool CAManagerIsInACDataList(JNIEnv *env, jstring jaddress);

/**
 * add auto connection data into list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jaddress              ble address.
 */
void CAManagerAddACData(JNIEnv *env, jstring jaddress);

/**
 * remove auto connection data from ACData list for selected ble address.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jaddress              ble address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerRemoveACData(JNIEnv *env, jstring jaddress);

/**
 * remove auto connection data from ACData list for all devices.
 * @param[in]   env                   JNI interface pointer.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerRemoveAllACData(JNIEnv *env);

/**
 * get isAutoConnecting flag for the address.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jaddress              ble address.
 * @param[out]  flag                  isAutoConnecting flag
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerGetAutoConnectingFlag(JNIEnv *env, jstring jaddress, bool *flag);

/**
 * set isAutoConnecting flag for the address.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jaddress              ble address.
 * @param[in]   flag                  isAutoConnecting flag.
 * @return  true or false
 */
bool CAManagerSetAutoConnectingFlag(JNIEnv *env, jstring jaddress, bool flag);

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

