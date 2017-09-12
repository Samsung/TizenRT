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

#ifndef CA_MANAGER_LE_INF_H_
#define CA_MANAGER_LE_INF_H_

#include "cacommon.h"
#include "cautilinterface.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Register network monitor callbacks.
 * Adapter state changes and network state changes are delivered these callbacks.
 * @param[in]   adapterStateCB  Adapter state changed callback.
 * @param[in]   connStateCB     Connection state changed callback.
 */
void CASetLENetworkMonitorCallbacks(CAAdapterStateChangedCB adapterStateCB,
                                    CAConnectionStateChangedCB connStateCB);

/**
 * Set device information for auto connection.
 */
CAResult_t CASetLEClientAutoConnectionDeviceInfo();

/**
 * Unset device information to stop auto connection.
 */
CAResult_t CAUnsetLEClientAutoConnectionDeviceInfo();

#if defined(__ANDROID__) && defined(LE_ADAPTER)
/**
 * initialize client connection manager
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jvm                   invocation inferface for JAVA virtual machine.
 * @param[in]   context               application context.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED or ::CA_MEMORY_ALLOC_FAILED
 */
CAResult_t CAManagerLEClientInitialize(JNIEnv *env, JavaVM *jvm, jobject context);

/**
 * terminate client connection manager
 * @param[in]   env                   JNI interface pointer.
 *
 * @return  ::CA_STATUS_OK or ::CA_STATUS_FAILED or ::CA_MEMORY_ALLOC_FAILED
 */
CAResult_t CAManagerLEClientTerminate(JNIEnv *env);

/**
 * set BLE scan interval time and working count.
 * @param[in]  intervalTime         interval time(Seconds).
 * @param[in]  workingCount         working count for selected interval time.
 */
void CAManagerLESetScanInterval(jint intervalTime, jint workingCount);

/**
 * stop BLE scan.
 */
void CAManagerLEStopScan();
#endif

/**
 * start BLE advertising.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerLEStartAdvertising();

/**
 * stop BLE advertising.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerLEStopAdvertising();

/**
 * set CAUtilConfig_t configure.
 * @param[in]  config       ::CAUtilConfig_t value
 */
void CAManagerSetConfigure(CAUtilConfig_t config);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_MANAGER_LE_INF_H_ */

