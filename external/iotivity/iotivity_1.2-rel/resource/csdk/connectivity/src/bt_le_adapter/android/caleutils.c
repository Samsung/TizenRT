/******************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
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

#include <jni.h>
#include <stdio.h>
#include <android/log.h>

#include "caleutils.h"
#include "logger.h"
#include "cathreadpool.h"
#include "uarraylist.h"
#include "caadapterutils.h"

#define TAG PCF("OIC_CA_LE_UTILS")

jobject CALEGetUuidFromString(JNIEnv *env, const char* uuid)
{
    VERIFY_NON_NULL_RET(uuid, TAG, "uuid is null", NULL);
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    jclass jni_cid_UUID = (*env)->FindClass(env, "java/util/UUID");
    if (!jni_cid_UUID)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_UUID is not available");
        goto error_exit;
    }

    jmethodID jni_mid_fromString = (*env)->GetStaticMethodID(env, jni_cid_UUID, "fromString",
                                                             "(Ljava/lang/String;)"
                                                             "Ljava/util/UUID;");
    if (!jni_mid_fromString)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_fromString is not available");
        goto error_exit;
    }

    jstring str_uuid = (*env)->NewStringUTF(env, uuid);
    if (!str_uuid)
    {
        OIC_LOG(ERROR, TAG, "str_uuid is not available");
        goto error_exit;
    }

    jobject jni_obj_uuid = (*env)->CallStaticObjectMethod(env, jni_cid_UUID, jni_mid_fromString,
                                                          str_uuid);
    if (!jni_obj_uuid)
    {
        OIC_LOG(ERROR, TAG, "Fail to get jni uuid object");
        goto error_exit;
    }

    return jni_obj_uuid;

error_exit:
    CACheckJNIException(env);
    return NULL;
}

jobject CALEGetParcelUuid(JNIEnv *env, jobject uuid)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(uuid, TAG, "uuid is null", NULL);

    jclass jni_cid_ParcelUuid = (*env)->FindClass(env, "android/os/ParcelUuid");
    if (!jni_cid_ParcelUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_ParcelUuid is not available");
        goto error_exit;
    }

    jmethodID jni_mid_ParcelUuid = (*env)->GetMethodID(env, jni_cid_ParcelUuid, "<init>",
                                                       "(Ljava/util/UUID;)V");
    if (!jni_mid_ParcelUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_ParcelUuid is not available");
        goto error_exit;
    }

    jobject jni_ParcelUuid = (*env)->NewObject(env, jni_cid_ParcelUuid, jni_mid_ParcelUuid, uuid);
    if (!jni_ParcelUuid)
    {
        OIC_LOG(ERROR, TAG, "Fail to get jni ParcelUuid");
        goto error_exit;
    }

    return jni_ParcelUuid;
error_exit:
    CACheckJNIException(env);
    return NULL;
}

jobject CALEGetParcelUuidFromString(JNIEnv *env, const char* uuid)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(uuid, TAG, "uuid is null", NULL);

    jclass jni_cid_ParcelUuid = (*env)->FindClass(env, "android/os/ParcelUuid");
    if (!jni_cid_ParcelUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_ParcelUuid is not available");
        goto error_exit;
    }

    jmethodID jni_mid_fromString = (*env)->GetStaticMethodID(env, jni_cid_ParcelUuid,
                                                             "fromString",
                                                             "(Ljava/lang/String;)"
                                                             "Landroid/os/ParcelUuid;");
    if (!jni_mid_fromString)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_fromString is not available");
        goto error_exit;
    }

    jstring str_uuid = (*env)->NewStringUTF(env, uuid);
    if (!str_uuid)
    {
        OIC_LOG(ERROR, TAG, "str_uuid is not available");
        goto error_exit;
    }

    jobject jni_obj_parcelUuid = (*env)->CallStaticObjectMethod(env, jni_cid_ParcelUuid,
                                                                jni_mid_fromString,
                                                                str_uuid);
    if (!jni_obj_parcelUuid)
    {
        OIC_LOG(ERROR, TAG, "Fail to get jni uuid object");
        goto error_exit;
    }

    return jni_obj_parcelUuid;
error_exit:
    CACheckJNIException(env);
    return NULL;
}

bool CALEIsBondedDevice(JNIEnv *env, jobject bluetoothDevice)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", false);
    VERIFY_NON_NULL_RET(bluetoothDevice, TAG, "bluetoothDevice is null", false);

    jmethodID jni_mid_getBondState = CAGetJNIMethodID(env, "android/bluetooth/BluetoothDevice",
                                                      "getBondState",
                                                      "()I");
    if (!jni_mid_getBondState)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBondState is null");
        return false;
    }

    jint jni_bondState = (jint)(*env)->CallIntMethod(env, bluetoothDevice, jni_mid_getBondState);
    CACheckJNIException(env);
    OIC_LOG_V(DEBUG, TAG, "bond state is %d", jni_bondState);

    if (BOND_BONDED == jni_bondState)
    {
        OIC_LOG(DEBUG, TAG, "remote device is bonded");
        return true;
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "remote device is not bonded");
        return false;
    }

    return false;
}

jobjectArray CALEGetBondedDevices(JNIEnv *env)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getBondedDevices: jni_cid_BTAdapter is null");
        goto error_exit;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    CACheckJNIException(env);
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getBondedDevices: bluetooth adapter is null");
        goto error_exit;
    }

    // Get a list of currently paired devices
    jmethodID jni_mid_getBondedDevices = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                             "getBondedDevices",
                                                             "()Ljava/util/Set;");
    if (!jni_mid_getBondedDevices)
    {
        OIC_LOG(ERROR, TAG, "getBondedDevices: jni_mid_getBondedDevicesr is null");
        goto error_exit;
    }

    jobject jni_obj_setPairedDevices = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                                jni_mid_getBondedDevices);
    if (!jni_obj_setPairedDevices)
    {
        OIC_LOG(ERROR, TAG, "getBondedDevices: jni_obj_setPairedDevices is null");
        goto error_exit;
    }

    jmethodID jni_mid_toArray = CAGetJNIMethodID(env, "java/util/Set", "toArray",
                                                 "()[Ljava/lang/Object;");
    if (!jni_mid_toArray)
    {
        OIC_LOG(ERROR, TAG, "getBondedDevices: jni_mid_toArray is null");
        return NULL;
    }

    jobjectArray jni_arrayPairedDevices = (jobjectArray)(
            (*env)->CallObjectMethod(env, jni_obj_setPairedDevices, jni_mid_toArray));
    if (!jni_arrayPairedDevices)
    {
        OIC_LOG(ERROR, TAG, "getBondedDevices: jni_arrayPairedDevices is null");
        goto error_exit;
    }

    return jni_arrayPairedDevices;

error_exit:
    CACheckJNIException(env);
    return NULL;
}

jint CALEGetBTStateOnInfo(JNIEnv *env)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", -1);

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getBTStateOnInfo: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        return -1;
    }

    jfieldID jni_fid_stateon = (*env)->GetStaticFieldID(env, jni_cid_BTAdapter, "STATE_ON", "I");
    if (!jni_fid_stateon)
    {
        OIC_LOG(ERROR, TAG, "get_field_state is not available");
        CACheckJNIException(env);
        return -1;
    }

    jint jni_int_val = (*env)->GetStaticIntField(env, jni_cid_BTAdapter, jni_fid_stateon);
    CACheckJNIException(env);
    OIC_LOG_V(DEBUG, TAG, "bluetooth.STATE_ON state integer value : %d", jni_int_val);

    return jni_int_val;
}

CAResult_t CALECheckPlatformVersion(JNIEnv *env, uint16_t level)
{
    jint jni_int_sdk = CALEGetBuildVersion(env);
    if (jni_int_sdk < level)
    {
        OIC_LOG(ERROR, TAG, "it is not supported");
        return CA_NOT_SUPPORTED;
    }

    return CA_STATUS_OK;
}

jint CALEGetBuildVersion(JNIEnv *env)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", -1);

    // VERSION is a nested class within android.os.Build (hence "$" rather than "/")
    jclass jni_cls_version = (*env)->FindClass(env, "android/os/Build$VERSION");
    if (!jni_cls_version)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_version is null");
        CACheckJNIException(env);
        return -1;
    }

    jfieldID jni_fid_sdk = (*env)->GetStaticFieldID(env, jni_cls_version, "SDK_INT", "I");
    if (!jni_fid_sdk)
    {
        OIC_LOG(ERROR, TAG, "jni_fid_sdk is null");
        CACheckJNIException(env);
        return -1;
    }

    jint jni_int_sdk = (*env)->GetStaticIntField(env, jni_cls_version, jni_fid_sdk);
    CACheckJNIException(env);
    OIC_LOG_V(DEBUG, TAG, "sdk version is %d", jni_int_sdk);

    return jni_int_sdk;
}

jint CALEGetBuildVersionCodeForName(JNIEnv *env, const char* versionName)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", -1);
    VERIFY_NON_NULL_RET(versionName, TAG, "versionName is null", -1);

    // VERSION is a nested class within android.os.Build (hence "$" rather than "/")
    jclass jni_cls_version = (*env)->FindClass(env, "android/os/Build$VERSION_CODES");
    if (!jni_cls_version)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_version is null");
        CACheckJNIException(env);
        return -1;
    }

    jfieldID jni_fid_version = (*env)->GetStaticFieldID(env, jni_cls_version, versionName, "I");
    if (!jni_fid_version)
    {
        OIC_LOG(ERROR, TAG, "jni_fid_version is null");
        CACheckJNIException(env);
        return -1;
    }

    jint jni_int_version = (*env)->GetStaticIntField(env, jni_cls_version, jni_fid_version);
    CACheckJNIException(env);
    OIC_LOG_V(DEBUG, TAG, "version [%s] is %d",versionName, jni_int_version);

    return jni_int_version;
}

jboolean CALEIsEnableBTAdapter(JNIEnv *env)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", JNI_FALSE);

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        return JNI_FALSE;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return JNI_FALSE;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return JNI_FALSE;
    }

    // isEnable()
    jmethodID jni_mid_isEnable = (*env)->GetMethodID(env, jni_cid_BTAdapter, "isEnabled", "()Z");
    if (!jni_mid_isEnable)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_isEnable is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        return JNI_FALSE;
    }

    jboolean jni_isEnable = (*env)->CallBooleanMethod(env, jni_obj_BTAdapter, jni_mid_isEnable);
    CACheckJNIException(env);
    OIC_LOG_V(DEBUG, TAG, "adapter state is %d", jni_isEnable);

    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    return jni_isEnable;
}

jstring CALEGetAddressFromBTDevice(JNIEnv *env, jobject bluetoothDevice)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(bluetoothDevice, TAG, "bluetoothDevice is null", NULL);

    jmethodID jni_mid_getAddress = CAGetJNIMethodID(env, "android/bluetooth/BluetoothDevice",
                                                    "getAddress",
                                                    "()Ljava/lang/String;");
    if (!jni_mid_getAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getAddress is null");
        return NULL;
    }

    jstring jni_address = (jstring)(*env)->CallObjectMethod(env, bluetoothDevice,
                                                            jni_mid_getAddress);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is null");
        CACheckJNIException(env);
        return NULL;
    }

    return jni_address;
}

jint CALEGetConstantsValue(JNIEnv *env, const char* classType, const char* name)
{
    VERIFY_NON_NULL_RET(env, TAG, "env", -1);
    VERIFY_NON_NULL_RET(classType, TAG, "classType", -1);
    VERIFY_NON_NULL_RET(name, TAG, "name", -1);

    jclass jni_cid = (*env)->FindClass(env, classType);
    if (!jni_cid)
    {
        OIC_LOG(ERROR, TAG, "jni_cid is null");
        CACheckJNIException(env);
        return -1;
    }

    jfieldID jni_fieldID = (*env)->GetStaticFieldID(env, jni_cid,
                                                    name, "I");
    if (!jni_fieldID)
    {
        OIC_LOG(ERROR, TAG, "jni_fieldID is null");
        CACheckJNIException(env);
        return -1;
    }

    jint jni_id = (*env)->GetStaticIntField(env, jni_cid, jni_fieldID);
    CACheckJNIException(env);
    return jni_id;
}

jobject CALEGetRemoteDevice(JNIEnv *env, jstring address)
{
    OIC_LOG(DEBUG, TAG, "CALEGetRemoteDevice");

    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(address, TAG, "address is null", NULL);

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        goto error_exit;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        goto error_exit;
    }

    // gat bt adapter object
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        goto error_exit;
    }

    jmethodID jni_mid_getRemoteDevice = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                            "getRemoteDevice",
                                                            METHODID_BT_REMOTE_DEVICE);
    if (!jni_mid_getRemoteDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getRemoteDevice is null");
        goto error_exit;
    }

    jobject jni_obj_device = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                      jni_mid_getRemoteDevice,
                                                      address);
    if (!jni_obj_device)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_device is null");
        goto error_exit;
    }
    return jni_obj_device;

error_exit:
    CACheckJNIException(env);
    return NULL;
}

jstring CALEGetAddressFromGatt(JNIEnv *env, jobject gatt)
{
    OIC_LOG(DEBUG, TAG, "CALEGetAddressFromGatt");

    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(gatt, TAG, "gatt is null", NULL);

    jmethodID jni_mid_getDevice = CAGetJNIMethodID(env, CLASSPATH_BT_GATT, "getDevice",
                                                   METHODID_BT_DEVICE);
    if (!jni_mid_getDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDevice is null");
        return NULL;
    }

    jobject jni_obj_device = (*env)->CallObjectMethod(env, gatt, jni_mid_getDevice);
    if (!jni_obj_device)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_device is null");
        CACheckJNIException(env);
        return NULL;
    }

    jstring jni_address = CALEGetAddressFromBTDevice(env, jni_obj_device);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is null");
        return NULL;
    }

    return jni_address;
}
