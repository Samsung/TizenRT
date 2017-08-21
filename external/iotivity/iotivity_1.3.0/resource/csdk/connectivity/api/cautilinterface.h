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

/**
 * @file
 *
 * This file contains APIs to handle adapters of connectivity abstraction layer
 * for network operations.
 * Application can use functions including network callback, BLE scan/advertisements,
 * set configurations of the adapters,  set port number Etc.
 */

#ifndef CA_UTILS_INTERFACE_H_
#define CA_UTILS_INTERFACE_H_

#include "cacommon.h"
#ifdef __JAVA__
#include "jni.h"
#endif
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * User Preference of connectivity channel for connection manager
 */
typedef enum
{
    /** Cloud TCP (Default) */
    CA_USER_PREF_CLOUD = 0,
    /** local UDP */
    CA_USER_PREF_LOCAL_UDP = 1,
    /** local TCP */
    CA_USER_PREF_LOCAL_TCP = 2
} CAConnectUserPref_t;

/*
 * CAUtilConfig_t structure.
 */
typedef struct
{
    CATransportBTFlags_t bleFlags;
    CAConnectUserPref_t connUserPref;
} CAUtilConfig_t;

/**
 * Callback function type for connection status changes delivery.
 * @param[out]   info           Remote endpoint information.
 * @param[out]   isConnected    Current connection status info.
 */
typedef void (*CAConnectionStateChangedCB)(const CAEndpoint_t *info, bool isConnected);

/**
 * Callback function type for adapter status changes delivery.
 * @param[out]   adapter    Transport type information.
 * @param[out]   enabled    Current adapter status info.
 */
typedef void (*CAAdapterStateChangedCB)(CATransportAdapter_t adapter, bool enabled);

/**
 * Register network monitoring callback.
 * Network status changes are delivered these callback.
 * @param[in]   adapterStateCB  Adapter state monitoring callback.
 * @param[in]   connStateCB     Connection state monitoring callback.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED or ::CA_MEMORY_ALLOC_FAILED
 */
CAResult_t CARegisterNetworkMonitorHandler(CAAdapterStateChangedCB adapterStateCB,
                                           CAConnectionStateChangedCB connStateCB);

/**
 * Unregister network monitoring callback.
 * Network status changes are delivered these callback.
 * @param[in]   adapterStateCB  Adapter state monitoring callback.
 * @param[in]   connStateCB     Connection state monitoring callback.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED or ::CA_MEMORY_ALLOC_FAILED
 */
CAResult_t CAUnregisterNetworkMonitorHandler(CAAdapterStateChangedCB adapterStateCB,
                                             CAConnectionStateChangedCB connStateCB);

/**
 * Set device to handle for auto connection.
 * @param[in]   address         LE address to set.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED.
 */
CAResult_t CASetAutoConnectionDeviceInfo(const char* address);

/**
 * Unset device to handle for auto connection.
 * @param[in]   address         LE address to unset.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED.
 */
CAResult_t CAUnsetAutoConnectionDeviceInfo(const char* address);

/**
 * Set the port number to assign .
 * @param[in]   adapter     Transport adapter information.
 * @param[in]   flag        Transport flag information.
 * @param[in]   port        The port number to use.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED.
 */
CAResult_t CASetPortNumberToAssign(CATransportAdapter_t adapter,
                                   CATransportFlags_t flag, uint16_t port);

/**
 * Get the assigned port number currently.
 * @param[in]   adapter     Transport adapter information.
 * @param[in]   flag        Transport flag information.
 *
 * @return  assigned port number information.
 */
uint16_t CAGetAssignedPortNumber(CATransportAdapter_t adapter, CATransportFlags_t flag);

#if defined(TCP_ADAPTER) && defined(WITH_CLOUD)
/**
 * Initializes the Connection Manager
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUtilCMInitailize();

/**
 * Terminate the Connection Manager
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUtilCMTerminate();

/**
 * Update RemoteDevice Information for Connection Manager
 * @param[in]  endpoint   Remote device information with specific device ID.
 * @param[in]  isCloud    with cloud or not.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAUtilCMUpdateRemoteDeviceInfo(const CAEndpoint_t *endpoint, bool isCloud);

/**
 * Reset RemoteDevice Info. for Connection Manager
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAUtilCMResetRemoteDeviceInfo();

/**
 * Set Connection Manager configuration
 * @param[in]  connPrefer  enum type of CAConnectUserPref_t.(default:CA_USER_PREF_CLOUD)
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAUtilCMSetConnectionUserConfig(CAConnectUserPref_t connPrefer);

/**
 * Get Connection Manager configuration
 * @param[out]  connPrefer  enum type of CAConnectUserPref_t.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAUtilCMGetConnectionUserConfig(CAConnectUserPref_t *connPrefer);
#endif //TCP_ADAPTER & WITH_CLOUD

#ifdef __JAVA__
#ifdef __ANDROID__
/**
 * initialize util client for android
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jvm                   invocation inferface for JAVA virtual machine.
 * @param[in]   context               application context.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED or ::CA_MEMORY_ALLOC_FAILED
 */
CAResult_t CAUtilClientInitialize(JNIEnv *env, JavaVM *jvm, jobject context);
#else
/**
 * initialize util client for android
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jvm                   invocation inferface for JAVA virtual machine.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED or ::CA_MEMORY_ALLOC_FAILED
 */
CAResult_t CAUtilClientInitialize(JNIEnv *env, JavaVM *jvm);
#endif //_ANDROID__

/**
 * terminate util client for android
 * @param[in]   env                   JNI interface pointer.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED or ::CA_MEMORY_ALLOC_FAILED
 */
CAResult_t CAUtilClientTerminate(JNIEnv *env);

// BT pairing
/**
 * start discovery for BT device which has iotivity UUID.
 * @param[in]  env              JNI interface pointer.
 */
CAResult_t CAUtilStartScan(JNIEnv *env);

/**
 * stop discovery
 * @param[in]  env              JNI interface pointer.
 */
CAResult_t CAUtilStopScan(JNIEnv *env);

/**
 * bonding between devices.
 * @param[in]  env              JNI interface pointer.
 * @param[in]  device           bluetooth device object.
 */
CAResult_t CAUtilCreateBond(JNIEnv *env, jobject device);

/**
 * set callback listener of found device.
 * @param[in]  listener         callback listener
 */
void CAUtilSetFoundDeviceListener(jobject listener);

/**
 * set interval time and working count for LE scan.
 * @param[in]  intervalTime         interval time(Seconds).
 * @param[in]  workingCount         working cycle for selected interval time.
 *
 * @return  ::CA_STATUS_OK or ::CA_NOT_SUPPORTED
 */
CAResult_t CAUtilSetLEScanInterval(jint intervalTime, jint workingCount);

/**
 * stop LE scan.
 * @return  ::CA_STATUS_OK or ::CA_NOT_SUPPORTED
 */
CAResult_t CAUtilStopLEScan();
#endif //__JAVA__

// BLE util
/**
 * start BLE advertising.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUtilStartLEAdvertising();

/**
 * stop BLE advertising.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUtilStopLEAdvertising();

/**
 * set CAUtil BT configure.
 * @param[in]  config       ::CAUtilConfig_t value
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUtilSetBTConfigure(CAUtilConfig_t config);

/**
 * return scope level of given ip address.
 * @param[in] addr         remote address.
 * @param[out] scopeLevel  scope level of given ip address.
 * @return      ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAGetIpv6AddrScope(const char *addr, CATransportFlags_t *scopeLevel);

/**
 * set CAUtil log preference.
 * @param[in]  level                     ::CAUtilLogLevel_t value
 * @param[in]  hidePrivateLogEntries     Private Log Entries.
 *                                       Example:
 *                                       true : hide private log.
 *                                       false : show private log.
 *                                       (privacy : uid, did, access token, etc)
 */
void CAUtilSetLogLevel(CAUtilLogLevel_t level, bool hidePrivateLogEntries);

#ifdef __cplusplus
} /* extern "C" */
#endif //__cplusplus

#endif /* CA_UTILS_INTERFACE_H_ */

