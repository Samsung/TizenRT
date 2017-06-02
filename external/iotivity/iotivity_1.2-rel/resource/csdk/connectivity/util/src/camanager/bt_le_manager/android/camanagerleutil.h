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

#ifndef CA_MANAGER_LE_UTILS_H_
#define CA_MANAGER_LE_UTILS_H_

#include <jni.h>

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
 * get bluetooth device object from bluetooth adapter.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remote_address        remote address.
 * @return  bluetooth device object.
 */
jobject CAManagerGetRemoteDevice(JNIEnv *env, jstring address);

/**
 * check whether the devie is bonded or not.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   btDevice              bluetooth device object.
 * @return  true(bonded) or false(bond none)
 */
bool CAManagerIsDeviceBonded(JNIEnv *env, jobject btDevice);

/**
 * change bluetooth adapter state.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   controlFlag           true : enable(), false : disable()
 * @return  true or false
 */
bool CAManagerControlAdapter(JNIEnv *env, bool controlFlag);

/**
 * request to get rssi value from bluetooth
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         connected gatt object.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAManagerReadRemoteRssi(JNIEnv *env, jobject bluetoothGatt);

/**
 * get SharedPreference from context of application.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @param[in]   prefName              preferences name.
 * @return  SharedPreferences object.
 */
jobject CAManagerGetSharedPreference(JNIEnv *env, jobject context, jstring prefName);

/**
 * check whether the device is already put or not.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @param[in]   address               LE address.
 * @param[in]   set                   set<String> object related to connected list.
 * @return  true or false
 */
bool CAManagerIsConnectedDeviceAddress(JNIEnv *env, jobject context,
                                       jstring address, jobject set);

/**
 * get SharedPreferences.Editor object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @return  SharedPreferences.Editor object.
 */
jobject CAManagerGetPrefStringSet(JNIEnv *env, jobject context);

/**
 * update SharedPreferences for set<String> object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @param[in]   set                   set<String> object related to connected list.
 * @return  true or false
 */
bool CAManagerUpdatePrefStringSet(JNIEnv *env, jobject context, jobject set);

/**
 * get SharedPreferences for set<String> object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @return  set<String> object from SharedPreferences
 */
jobject CAManagerGetPrefStringSet(JNIEnv *env, jobject context);

/**
 * checks whether the preferences contains a preference.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @return  true or false
 */
bool CAManagerContainsPrefStringSet(JNIEnv *env, jobject context);

/**
 * add connected device into SharedPreference.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @param[in]   address               ble address.
 * @param[in]   set                   set<String> object related to connected list.
 * @return  true or false
 */
bool CAManagerAddConnectedDeviceAddress(JNIEnv *env, jobject context,
                                        jstring address, jobject set);

/**
 * get connected device from SharedPreference.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @return  LE address of connected device.
 */
jobject CAManagerGetConnectedDeviceAddress(JNIEnv *env, jobject context);

/**
 * remove connected device to SharedPreference.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   context               Application context interface.
 * @param[in]   address               LE address.
 * @param[in]   set                   set<String> object related to connected list.
 * @return  true or false
 */
bool CAManagerRemoveConnectedDeviceAddress(JNIEnv *env, jobject context,
                                           jstring address, jobject set);

/**
 * Vaildate a String bluetooth address.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   address               LE address.
 * @return  JNI_TRUE or JNI_FALSE
 */
jboolean CAManagerCheckBTAddress(JNIEnv *env, jstring address);

/**
 * create new set<String> object.
 * @param[in]   env                   JNI interface pointer.
 * @return  set<String> object
 */
jobject CAManagerCreateSetString(JNIEnv *env);

/**
 * remove connected device to SharedPreference.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   address               LE address.
 * @param[in]   set                   set<String> object related to connected list.
 * @param[in]   method_type           method name to get its ID.
 * @return  true or false
 */
bool CAManagerCallFuncSetString(JNIEnv *env, jstring address,
                                jobject set, CASetMethod_t method_type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_MANAGER_LE_UTILS_H_ */
