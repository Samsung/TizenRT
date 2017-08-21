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

/**
 * @file
 * This file contains the APIs for BT LE communications.
 */
#ifndef CA_LE_LIST_H_
#define CA_LE_LIST_H_

#include "cacommon.h"
#include "cathreadpool.h" /* for thread pool */
#include "octhread.h"
#include "uarraylist.h"
#include "jni.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct le_state_info
{
    char address[CA_MACADDR_SIZE];
    uint16_t connectedState;
    uint16_t sendState;
    jboolean autoConnectFlag;
    jboolean isDescriptorFound;
    uint16_t mtuSize;
} CALEState_t;

/**
 * update new state information.
 * @param[in]   address               remote address.
 * @param[in]   state_type            state type.
 * @param[in]   target_state          state index to update.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEUpdateDeviceState(const char* address,
                                 uint16_t state_type,
                                 uint16_t target_state,
                                 u_arraylist_t *deviceList,
                                 oc_mutex deviceListMutex);

/**
 * check whether the remote address is existed or not.
 * @param[in]   address               remote address.
 * @param[in]   deviceList            target device list.
 * @return  true or false.
 */
bool CALEIsDeviceInList(const char* remoteAddress,
                        u_arraylist_t *deviceList);

/**
 * remove all device states.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALERemoveAllDeviceState(u_arraylist_t *deviceList,
                                    oc_mutex deviceListMutex);

/**
 * Reset values of device state for all of devices.
 * this method has to be invoked when BT adapter is disabled.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEResetDeviceStateForAll(u_arraylist_t *deviceList,
                                      oc_mutex deviceListMutex);

/**
 * remove the device state for a remote device.
 * @param[in]   remoteAddress         remote address.
 * @param[in]   deviceList            target device list.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALERemoveDeviceState(const char* remoteAddress,
                                 u_arraylist_t *deviceList);

/**
 * get state information for a remote device.
 * @param[in]   remoteAddress         remote address.
 * @param[in]   deviceList            target device list.
 * @return  CALEState_t.
 */
CALEState_t* CALEGetStateInfo(const char* remoteAddressm,
                              u_arraylist_t *deviceList);

/**
 * check whether the remote address has same state with target state.
 * @param[in]   remoteAddress         remote address.
 * @param[in]   state_type            state_type.
 * @param[in]   target_state          state index to check.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 * @return  true or false.
 */
bool CALEIsValidState(const char* remoteAddress,
                      uint16_t state_type,
                      uint16_t target_state,
                      u_arraylist_t *deviceList,
                      oc_mutex deviceListMutex);


/**
 * set flag into State List.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jni_address           remote address.
 * @param[in]   state_idx             state index.
 * @param[in]   flag                  auto connect flag.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 */
CAResult_t CALESetFlagToState(JNIEnv *env, jstring jni_address,
                              jint state_idx, jboolean flag,
                              u_arraylist_t *deviceList,
                              oc_mutex deviceListMutex);

/**
 * get flag from State List.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jni_address           remote address.
 * @param[in]   state_idx             state index.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 * @return  current flag;
 */
jboolean CALEGetFlagFromState(JNIEnv *env, jstring jni_address, jint state_idx,
                              u_arraylist_t *deviceList, oc_mutex deviceListMutex);

/**
 * This function is used to set MTU size
 * which negotiated between client and server device.
 * @param[in]   address               remote address.
 * @param[in]   mtuSize               MTU size.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALESetMtuSize(const char* address, uint16_t mtuSize,
                          u_arraylist_t *deviceList, oc_mutex deviceListMutex);

/**
 * get MTU size.
 * @param[in] address      the address of the remote device.
 * @param[in]   deviceList            target device list.
 * @param[in]   deviceListMutex       target device list mutex.
 * @return  mtu size negotiated from remote device.
 */
uint16_t CALEGetMtuSize(const char* address,
                        u_arraylist_t *deviceList,
                        oc_mutex deviceListMutex);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_LESERVER_H_ */
