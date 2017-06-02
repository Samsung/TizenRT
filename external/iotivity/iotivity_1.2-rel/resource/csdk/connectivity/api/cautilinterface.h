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

#ifndef CA_UTILS_INTERFACE_H_
#define CA_UTILS_INTERFACE_H_

#include "cacommon.h"
#ifdef __ANDROID__
#include "jni.h"
#endif
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * this level depends on transmission time.
 * unicast based UDP will be checked by caretransmission.
 */
typedef enum
{
    HIGH_SPEED = 0,
    NORMAL_SPEED
} CMSpeedLevel_t;

typedef struct
{
    /** address for all **/
    char addr[MAX_ADDR_STR_SIZE_CA];

    /** adapter priority of all transmissions. **/
    CATransportAdapter_t adapter;

    /** level about speed of response. **/
    CMSpeedLevel_t level;
} CMConfigureInfo_t;

/*
 * CAUtilConfig_t structure.
 */
typedef struct
{
    CATransportBTFlags_t bleFlags;
    CMConfigureInfo_t cmInfo;
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

//custom advertisement data setting
#if defined(__TIZEN__) && defined(LE_ADAPTER) && defined(BLE_CUSTOM_ADVERTISE)
CAResult_t CASetAdvertisementData(const char* data, int length);
CAResult_t CASetScanResponseData(const char* data, int length);
#endif

#ifdef __APPLE__
/**
 * initialize client connection manager
 */
CAResult_t CAUtilClientInitialize();

/**
 * terminate client connection manager
 */
CAResult_t CAUtilClientTerminate();

/**
 * stop LE scan.
 * @return  ::CA_STATUS_OK or ::CA_NOT_SUPPORTED
 */
CAResult_t CAUtilStopLEScan();

/**
 * start LE scan.
 * @return  ::CA_STATUS_OK or ::CA_NOT_SUPPORTED
 */
CAResult_t CAUtilStartLEScan();
#endif

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
 *
 * @return  ::CA_STATUS_OK or ::CA_NOT_SUPPORTED
 */
CAResult_t CAUtilStopLEScan();
#endif

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
 * set CAUtil log preference.
 * @param[in]  level                     ::CAUtilLogLevel_t value
 * @param[in]  hidePrivateLogEntries     Private Log Entries.
 *                                       Example:
 *                                       true : hide private log.
 *                                       false : show private log.
 *                                       (privacy : uid, did, access token, etc)
 */
void CAUtilSetLogLevel(CAUtilLogLevel_t level, bool hidePrivateLogEntries);

/**
 * Set multicast time to live value to control the scope of the multicasts.
 * @param[in]  ttl         To be set to any value from 0 to 255.
 *                         Example:
 *                         0: Are restricted to the same host.
 *                         1: Are restricted to the same subnet.
 *                         32: Are restricted to the same site.
 *                         64: Are restricted to the same region.
 *                         128: Are restricted to the same continent.
 *                         255: Are unrestricted in scope.
 *                         We cannot support region, continent and unrestricted in scope.
 *
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUtilSetMulticastTTL(size_t ttl);

/**
 * Get multicast time to live value.
 * @param[out]  ttl        TTL pointer to get the stored multicast time to live.
 *
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUtilGetMulticastTTL(size_t *ttl);

/**
 * Disconnect TCP session.
 * When there is no transmission for a long time.
 * Some carrier Vendor is blocking data.
 * Thur, TCP Session is cleaned through this function.
 * @param[in]   address        Address.
 * @param[in]   port           Port.
 * @param[in]   flags          Transport flag.
 */
CAResult_t CAUtilTCPDisconnectSession(const char *address,
                                      uint16_t port,
                                      CATransportFlags_t flags);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_UTILS_INTERFACE_H_ */

