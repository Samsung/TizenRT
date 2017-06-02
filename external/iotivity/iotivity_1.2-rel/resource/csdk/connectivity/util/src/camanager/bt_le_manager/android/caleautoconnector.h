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

#ifndef CA_AUTO_CONNECTOR_H_
#define CA_AUTO_CONNECTOR_H_

#include <jni.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define STATE_OFF 10
#define START_RECOVERY 1

/**
 * start auto connection.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remote_le_address     remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerStartAutoConnection(JNIEnv *env, jstring remote_le_address);

/**
 * request connect gatt on client in adapter
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remote_le_address     remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerConnectGatt(JNIEnv *env, jstring remote_le_address);

/**
 * initialize LE AutoConnection.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerInitLEAutoConnection();

/**
 * terminate LE AutoConnection
 */
void CAManagerTerminateLEAutoConnection();

/**
 * process BT recovery.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   adapter_state         recovery state to process.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAManagerProcessRecovery(JNIEnv *env, uint16_t adapter_state);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_AUTO_CONNECTOR_H_ */

