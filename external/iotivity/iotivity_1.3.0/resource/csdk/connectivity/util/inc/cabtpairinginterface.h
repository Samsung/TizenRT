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

#ifndef CA_BT_PAIRING_INTERFACE_H_
#define CA_BT_PAIRING_INTERFACE_H_

#include "cacommon.h"
#include "cautilinterface.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __ANDROID__
#ifdef EDR_ADAPTER
/**
 * initialize BT Pairing manager
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jvm                   invocation inferface for JAVA virtual machine.
 * @param[in]   context               application context.
 *
 * @return  CA_STATUS_OK or ERROR CODES (CAResult_t error codes in cacommon.h).
 */
CAResult_t CABTPairingInitialize(JNIEnv *env, JavaVM *jvm, jobject context);

/**
 * start discovery for BT device which has iotivity UUID.
 * @param[in]  env              JNI interface pointer.
 *
 * @return  CA_STATUS_OK or ERROR CODES (CAResult_t error codes in cacommon.h).
 */
CAResult_t CABTPairingStartScan(JNIEnv *env);

/**
 * stop discovery
 * @param[in]  env              JNI interface pointer.
 *
 * @return  CA_STATUS_OK or ERROR CODES (CAResult_t error codes in cacommon.h).
 */
CAResult_t CABTPairingStopScan(JNIEnv *env);

/**
 * bonding between devices.
 * @param[in]  env              JNI interface pointer.
 * @param[in]  device           bluetooth device object.
 *
 * @return  CA_STATUS_OK or ERROR CODES (CAResult_t error codes in cacommon.h).
 */
CAResult_t CABTPairingCreateBond(JNIEnv *env, jobject device);

/**
 * set callback listener of found device.
 * @param[in]  listener         callback listener
 */
void CABTPairingSetFoundDeviceListener(jobject listener);
#endif
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_LE_MANAGER_H_ */

