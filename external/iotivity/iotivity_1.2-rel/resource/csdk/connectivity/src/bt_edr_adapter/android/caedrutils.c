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
#include "caedrutils.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cathreadpool.h"
#include "uarraylist.h"
#include "caadapterutils.h"

#define ERROR_CODE (-1)
#define TAG PCF("OIC_CA_EDR_UTILS")

static const char METHODID_OBJECTNONPARAM[] = "()Landroid/bluetooth/BluetoothAdapter;";
static const char METHODID_STRINGNONPARAM[] = "()Ljava/lang/String;";
static const char CLASSPATH_BT_ADPATER[] = "android/bluetooth/BluetoothAdapter";
static const char CLASSPATH_BT_DEVICE[] = "android/bluetooth/BluetoothDevice";
static const char CLASSPATH_BT_SOCKET[] = "android/bluetooth/BluetoothSocket";

static u_arraylist_t *g_deviceStateList = NULL;
static u_arraylist_t *g_deviceObjectList = NULL;

// get address from bluetooth socket
jstring CAEDRNativeGetAddressFromDeviceSocket(JNIEnv *env, jobject bluetoothSocketObj)
{
    VERIFY_NON_NULL_RET(bluetoothSocketObj, TAG, "bluetoothSocketObj", NULL);

    jmethodID jni_mid_getRemoteDevice = CAGetJNIMethodID(
            env, CLASSPATH_BT_SOCKET, "getRemoteDevice", "()Landroid/bluetooth/BluetoothDevice;");
    if (!jni_mid_getRemoteDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getRemoteDevice is null");
        return NULL;
    }

    jobject jni_obj_remoteBTDevice = (*env)->CallObjectMethod(env, bluetoothSocketObj,
                                                              jni_mid_getRemoteDevice);
    if (!jni_obj_remoteBTDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_remoteBTDevice is null");
        return NULL;
    }

    jmethodID j_mid_getAddress = CAGetJNIMethodID(env, CLASSPATH_BT_DEVICE,
                                                  "getAddress",
                                                  METHODID_STRINGNONPARAM);
    if (!j_mid_getAddress)
    {
        OIC_LOG(ERROR, TAG, "j_mid_getAddress is null");
        (*env)->DeleteLocalRef(env, jni_obj_remoteBTDevice);
        return NULL;
    }

    jstring j_str_address = (*env)->CallObjectMethod(env, jni_obj_remoteBTDevice,
                                                     j_mid_getAddress);
    if (!j_str_address)
    {
        OIC_LOG(ERROR, TAG, "j_str_address is null");
        (*env)->DeleteLocalRef(env, jni_obj_remoteBTDevice);
        return NULL;
    }

    (*env)->DeleteLocalRef(env, jni_obj_remoteBTDevice);

    return j_str_address;
}

jstring CAEDRNativeGetLocalDeviceAddress(JNIEnv* env)
{
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADPATER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return NULL;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    jmethodID jni_mid_getAddress = (*env)->GetMethodID(env, jni_cid_BTAdapter, "getAddress",
                                                       METHODID_STRINGNONPARAM);
    if (!jni_mid_getAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getAddress is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    jstring jni_str_address = (jstring)(*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                                jni_mid_getAddress);
    if (!jni_str_address)
    {
        OIC_LOG(ERROR, TAG, "jni_str_address is null");
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);

    return jni_str_address;
}

jobjectArray CAEDRNativeGetBondedDevices(JNIEnv *env)
{
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADPATER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return NULL;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "default adapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "bluetooth adapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    // Get a list of currently paired devices
    jmethodID jni_mid_getBondedDevices = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                             "getBondedDevices",
                                                             "()Ljava/util/Set;");
    if (!jni_mid_getBondedDevices)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBondedDevicesr is null");
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    jobject jni_obj_setPairedDevices = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                                jni_mid_getBondedDevices);
    if (!jni_obj_setPairedDevices)
    {
        OIC_LOG(ERROR, TAG, "ni_obj_setPairedDevices is null");
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return NULL;
    }

    // Convert the set to an object array
    // object[] array = Set<BluetoothDevice>.toArray();
    jmethodID jni_mid_toArray = CAGetJNIMethodID(env, "java/util/Set",
                                                 "toArray", "()[Ljava/lang/Object;");
    if (!jni_mid_toArray)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_toArray is null");
        goto exit;
    }

    jobjectArray jni_arrayPairedDevices = (jobjectArray)(
            (*env)->CallObjectMethod(env, jni_obj_setPairedDevices, jni_mid_toArray));
    if (!jni_arrayPairedDevices)
    {
        OIC_LOG(ERROR, TAG, "jni_arrayPairedDevices is null");
        goto exit;
    }

    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_setPairedDevices);

    return jni_arrayPairedDevices;

exit:
    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_setPairedDevices);
    return NULL;
}

jint CAEDRNativeGetBTStateOnInfo(JNIEnv *env)
{
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADPATER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return ERROR_CODE;
    }

    jfieldID jni_fid_stateon = (*env)->GetStaticFieldID(env, jni_cid_BTAdapter, "STATE_ON", "I");
    if (jni_fid_stateon == 0)
    {
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);

        OIC_LOG(ERROR, TAG, "get_field_state is 0");
        return ERROR_CODE;
    }
    jint jni_int_val = (*env)->GetStaticIntField(env, jni_cid_BTAdapter, jni_fid_stateon);

    OIC_LOG_V(DEBUG, TAG, "bluetooth state integer value : %d", jni_int_val);

    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);

    return jni_int_val;
}

jboolean CAEDRNativeIsEnableBTAdapter(JNIEnv *env)
{
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADPATER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter: jni_cid_BTAdapter is null");
        return JNI_FALSE;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return JNI_FALSE;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return JNI_FALSE;
    }

    // isEnable()
    jmethodID jni_mid_isEnable = (*env)->GetMethodID(env, jni_cid_BTAdapter, "isEnabled", "()Z");
    if (!jni_mid_isEnable)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_isEnable is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        return JNI_FALSE;
    }

    jboolean jni_isEnable = (*env)->CallBooleanMethod(env, jni_obj_BTAdapter, jni_mid_isEnable);

    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);

    return jni_isEnable;
}

jstring CAEDRNativeGetAddressFromBTDevice(JNIEnv *env, jobject bluetoothDevice)
{
    VERIFY_NON_NULL_RET(bluetoothDevice, TAG, "bluetoothDevice", NULL);

    jmethodID jni_mid_getAddress = CAGetJNIMethodID(env,
                                                    CLASSPATH_BT_DEVICE,
                                                    "getAddress",
                                                    METHODID_STRINGNONPARAM);
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
        return NULL;
    }
    return jni_address;
}

/**
 * BT State List
 */
void CAEDRNativeCreateDeviceStateList()
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeCreateDeviceStateList");

    // create new object array
    if (NULL == g_deviceStateList)
    {
        OIC_LOG(DEBUG, TAG, "Create device list");

        g_deviceStateList = u_arraylist_create();
    }
}

void CAEDRUpdateDeviceState(CAConnectedState_t state, const char *address)
{
    VERIFY_NON_NULL_VOID(address, TAG, "address");

    CAConnectedDeviceInfo_t *deviceInfo =
            (CAConnectedDeviceInfo_t *) OICCalloc(1, sizeof(CAConnectedDeviceInfo_t));
    if (!deviceInfo)
    {
        OIC_LOG(ERROR, TAG, "deviceInfo is null");
        return;
    }
    OICStrcpy((char*) deviceInfo->address, sizeof(deviceInfo->address), address);
    deviceInfo->state = state;

    CAEDRNativeAddDeviceStateToList(deviceInfo);
}

void CAEDRNativeAddDeviceStateToList(CAConnectedDeviceInfo_t *deviceInfo)
{
    VERIFY_NON_NULL_VOID(deviceInfo, TAG, "deviceInfo");

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "gdevice_list is null");
        return;
    }

    if (CAEDRNativeIsDeviceInList((const char*) deviceInfo->address))
    {
        // delete previous state for update new state
        CAEDRNativeRemoveDevice((const char*) deviceInfo->address);
    }
    u_arraylist_add(g_deviceStateList, deviceInfo); // update new state
    OIC_LOG_V(DEBUG, TAG, "add new device state[%d] to list", deviceInfo->state);
}

bool CAEDRNativeIsDeviceInList(const char* remoteAddress)
{
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress", false);

    jint length = u_arraylist_length(g_deviceStateList);
    for (jint index = 0; index < length; index++)
    {
        CAConnectedDeviceInfo_t* deviceInfo =
                (CAConnectedDeviceInfo_t*) u_arraylist_get(g_deviceStateList, index);
        if (!deviceInfo)
        {
            OIC_LOG(ERROR, TAG, "deviceInfo object is null");
            return false;
        }

        if (!strcmp(remoteAddress, (const char*) deviceInfo->address))
        {
            OIC_LOG(DEBUG, TAG, "the device is already set");
            return true;
        }
    }

    OIC_LOG(DEBUG, TAG, "there are no the device in list.");
    return false;
}

void CAEDRNativeRemoveAllDeviceState()
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeRemoveAllDevices");

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceStateList is null");
        return;
    }

    jint index;
    jint length = u_arraylist_length(g_deviceStateList);
    for (index = 0; index < length; index++)
    {
        CAConnectedDeviceInfo_t* deviceInfo =
                (CAConnectedDeviceInfo_t*) u_arraylist_get(g_deviceStateList, index);
        if (!deviceInfo)
        {
            OIC_LOG(DEBUG, TAG, "jarrayObj is null");
            continue;
        }
        OICFree(deviceInfo);
    }

    OICFree(g_deviceStateList);
    g_deviceStateList = NULL;
    return;
}

void CAEDRNativeRemoveDevice(const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeRemoveDeviceforStateList");
    VERIFY_NON_NULL_VOID(remoteAddress, TAG, "remoteAddress");

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceStateList is null");
        return;
    }

    jint length = u_arraylist_length(g_deviceStateList);
    for (jint index = 0; index < length; index++)
    {
        CAConnectedDeviceInfo_t* deviceInfo =
                (CAConnectedDeviceInfo_t*) u_arraylist_get(g_deviceStateList, index);
        if (!deviceInfo)
        {
            OIC_LOG(DEBUG, TAG, "deviceInfo object is null");
            continue;
        }

        if (!strcmp((const char*) deviceInfo->address, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "remove [%s] info from list", remoteAddress);
            OICFree(deviceInfo);

            u_arraylist_remove(g_deviceStateList, index);
            break;
        }
    }
    return;
}

CAConnectedState_t CAEDRIsConnectedDevice(const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CAEDRIsConnectedDevice");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress", STATE_DISCONNECTED);

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceStateList is null");
        return STATE_DISCONNECTED;
    }

    jint length = u_arraylist_length(g_deviceStateList);
    for (jint index = 0; index < length; index++)
    {
        CAConnectedDeviceInfo_t* deviceInfo =
                (CAConnectedDeviceInfo_t*) u_arraylist_get(g_deviceStateList, index);
        if (!deviceInfo)
        {
            OIC_LOG(DEBUG, TAG, "deviceInfo object is null");
            continue;
        }

        if (!strcmp((const char*) deviceInfo->address, remoteAddress))
        {
            return deviceInfo->state;
        }
    }
    return STATE_DISCONNECTED;
}

/**
 * Device Socket Object List
 */
void CAEDRNativeCreateDeviceSocketList()
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeCreateDeviceSocketList");

    // create new object array
    if (NULL == g_deviceObjectList)
    {
        OIC_LOG(DEBUG, TAG, "Create Device object list");

        g_deviceObjectList = u_arraylist_create();
    }
}

void CAEDRNativeAddDeviceSocketToList(JNIEnv *env, jobject deviceSocket)
{
    OIC_LOG(DEBUG, TAG, "CANativeAddDeviceobjToList");
    VERIFY_NON_NULL_VOID(deviceSocket, TAG, "deviceSocket");

    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return;
    }

    jstring jni_remoteAddress = CAEDRNativeGetAddressFromDeviceSocket(env, deviceSocket);
    if (!jni_remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_remoteAddress is null");
        return;
    }

    const char* remoteAddress = (*env)->GetStringUTFChars(env, jni_remoteAddress, NULL);

    if (CAEDRNativeIsDeviceSocketInList(env, remoteAddress))
    {
        OIC_LOG(DEBUG, TAG, "the address exists in deviceObjectList. remove it to add new");
        CAEDRNativeRemoveDeviceSocketBaseAddr(env, jni_remoteAddress);
    }

    (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, remoteAddress);
    (*env)->DeleteLocalRef(env, jni_remoteAddress);

    CAEDRSocketInfo_t *socketInfo = (CAEDRSocketInfo_t *) OICCalloc(1, sizeof(*socketInfo));
    if (!socketInfo)
    {
        OIC_LOG(ERROR, TAG, "Out of memory");
        return;
    }

    jmethodID jni_mid_getInputStream = CAGetJNIMethodID(env, "android/bluetooth/BluetoothSocket",
                                                             "getInputStream",
                                                             "()Ljava/io/InputStream;");
    if (!jni_mid_getInputStream)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getInputStream is null");
        OICFree(socketInfo);
        return;
    }

    jobject jni_obj_inputStream = (*env)->CallObjectMethod(env, deviceSocket,
                                                           jni_mid_getInputStream);
    if (!jni_obj_inputStream)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_inputStream is null");
        OICFree(socketInfo);
        return;
    }

    socketInfo->deviceSocket = (*env)->NewGlobalRef(env, deviceSocket);
    socketInfo->inputStream = (*env)->NewGlobalRef(env, jni_obj_inputStream);
    (*env)->DeleteLocalRef(env, jni_obj_inputStream);

    bool result = u_arraylist_add(g_deviceObjectList, (void *) socketInfo);
    if (!result)
    {
        OIC_LOG(ERROR, TAG, "u_arraylist_add failed.");
        OICFree(socketInfo);
        return;
    }

    OIC_LOG(DEBUG, TAG, "add new device socket object to list");
}

bool CAEDRNativeIsDeviceSocketInList(JNIEnv *env, const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CANativeIsDeviceObjInList");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress", false);

    jint length = u_arraylist_length(g_deviceObjectList);
    for (jint index = 0; index < length; index++)
    {
        CAEDRSocketInfo_t *socketInfo = (CAEDRSocketInfo_t *) u_arraylist_get(g_deviceObjectList,
                                                                              index);
        if (!socketInfo)
        {
            OIC_LOG(DEBUG, TAG, "socketInfo is null");
            return false;
        }

        jobject jarrayObj = socketInfo->deviceSocket;
        if (!jarrayObj)
        {
            OIC_LOG(DEBUG, TAG, "jarrayObj is null");
            return false;
        }

        jstring jni_setAddress = CAEDRNativeGetAddressFromDeviceSocket(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(DEBUG, TAG, "jni_setAddress is null");
            return false;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(DEBUG, TAG, "setAddress is null");
            return false;
        }

        if (!strcmp(remoteAddress, setAddress))
        {
            OIC_LOG(DEBUG, TAG, "the device is already set");
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            return true;
        }
        else
        {
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            continue;
        }
    }

    OIC_LOG(DEBUG, TAG, "there are no the Device obejct in list");
    return false;
}

void CAEDRNativeSocketCloseToAll(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeSocketCloseToAll");

    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return;
    }

    jmethodID jni_mid_close = CAGetJNIMethodID(env, CLASSPATH_BT_SOCKET,
                                               "close", "()V");
    if (!jni_mid_close)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_close is null");
        return;
    }

    jint length = u_arraylist_length(g_deviceObjectList);
    for (jint index = 0; index < length; index++)
    {
        jobject jni_obj_socket = (jobject) u_arraylist_get(g_deviceObjectList, index);
        if (!jni_obj_socket)
        {
            OIC_LOG(ERROR, TAG, "socket obj is null");
            return;
        }

        (*env)->CallVoidMethod(env, jni_obj_socket, jni_mid_close);

        if ((*env)->ExceptionCheck(env))
        {
            OIC_LOG(ERROR, TAG, "close is Failed!!!");
            (*env)->ExceptionDescribe(env);
            (*env)->ExceptionClear(env);
            return;
        }
    }
}

void CAEDRNativeRemoveAllDeviceSocket(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CANativeRemoveAllDeviceObjsList");

    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return;
    }

    jint length = u_arraylist_length(g_deviceObjectList);
    for (jint index = 0; index < length; index++)
    {
        CAEDRSocketInfo_t *socketInfo = (CAEDRSocketInfo_t *) u_arraylist_get(g_deviceObjectList,
                                                                              index);
        if (!socketInfo)
        {
            OIC_LOG(ERROR, TAG, "socketInfo is null");
            continue;
        }

        jobject jdeviceSocket = socketInfo->deviceSocket;
        if (jdeviceSocket)
        {
            (*env)->DeleteGlobalRef(env, jdeviceSocket);
        }

        jobject jinputStream = socketInfo->inputStream;
        if (jinputStream)
        {
            (*env)->DeleteGlobalRef(env, jinputStream);
        }
    }

    OICFree(g_deviceObjectList);
    g_deviceObjectList = NULL;
    return;
}

void CAEDRNativeRemoveDeviceSocket(JNIEnv *env, jobject deviceSocket)
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeRemoveDeviceSocket");
    VERIFY_NON_NULL_VOID(deviceSocket, TAG, "deviceSocket");

    jstring jni_address = CAEDRNativeGetAddressFromDeviceSocket(env, deviceSocket);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is null");
        return;
    }

    CAEDRNativeRemoveDeviceSocketBaseAddr(env, jni_address);

    return;
}

void CAEDRNativeRemoveDeviceSocketBaseAddr(JNIEnv *env, jstring address)
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeRemoveDeviceSocketBaseAddr");
    VERIFY_NON_NULL_VOID(address, TAG, "address");

    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return;
    }

    const char* targetAddress = (*env)->GetStringUTFChars(env, address, NULL);

    jint length = u_arraylist_length(g_deviceObjectList);
    for (jint index = 0; index < length; index++)
    {
        CAEDRSocketInfo_t *socketInfo = (CAEDRSocketInfo_t *) u_arraylist_get(g_deviceObjectList,
                                                                              index);
        if (!socketInfo)
        {
            OIC_LOG(ERROR, TAG, "socketInfo is null");
            continue;
        }

        jobject jarrayObj = socketInfo->deviceSocket;
        if (!jarrayObj)
        {
            OIC_LOG(DEBUG, TAG, "jarrayObj is null");
            continue;
        }

        jstring jni_setAddress = CAEDRNativeGetAddressFromDeviceSocket(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            continue;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);

        if (!strcmp(setAddress, targetAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "remove object : %s", targetAddress);
            (*env)->DeleteGlobalRef(env, jarrayObj);
            jobject jinputStream = socketInfo->inputStream;
            if (jinputStream)
            {
                (*env)->DeleteGlobalRef(env, jinputStream);
            }
            (*env)->ReleaseStringUTFChars(env, address, targetAddress);
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->DeleteLocalRef(env, jni_setAddress);

            u_arraylist_remove(g_deviceObjectList, index);
            return;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->DeleteLocalRef(env, jni_setAddress);
    }

    OIC_LOG_V(DEBUG, TAG, "the target object doesn't exist in deviceObjectList (addr: %s)",
              targetAddress);
    (*env)->ReleaseStringUTFChars(env, address, targetAddress);

    return;
}

jobject CAEDRNativeGetDeviceSocket(uint32_t index)
{
    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return NULL;
    }

    CAEDRSocketInfo_t *socketInfo = (CAEDRSocketInfo_t *) u_arraylist_get(g_deviceObjectList,
                                                                          index);
    if (!socketInfo)
    {
        OIC_LOG(ERROR, TAG, "socketInfo is null");
        return NULL;
    }

    jobject jarrayObj = socketInfo->deviceSocket;
    if (!jarrayObj)
    {
        OIC_LOG(ERROR, TAG, "jarrayObj is not available");
        return NULL;
    }
    return jarrayObj;
}

jobject CAEDRNativeGetDeviceSocketBaseAddr(JNIEnv *env, const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeGetDeviceSocketBaseAddr");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress", NULL);

    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return NULL;
    }

    jint length = u_arraylist_length(g_deviceObjectList);
    for (jint index = 0; index < length; index++)
    {
        CAEDRSocketInfo_t *socketInfo = (CAEDRSocketInfo_t *) u_arraylist_get(g_deviceObjectList,
                                                                              index);
        if (!socketInfo)
        {
            OIC_LOG(ERROR, TAG, "socketInfo is null");
            continue;
        }

        jobject jarrayObj = socketInfo->deviceSocket;
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }

        jstring jni_setAddress = CAEDRNativeGetAddressFromDeviceSocket(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            continue;
        }
        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);

        if (!strcmp(setAddress, remoteAddress))
        {
            OIC_LOG_V(ERROR, TAG, "remove object : %s", remoteAddress);
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->DeleteLocalRef(env, jni_setAddress);
            return jarrayObj;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->DeleteLocalRef(env, jni_setAddress);
    }

    return NULL;
}

jobject CAEDRNativeGetInputStream(uint32_t index)
{
    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return NULL;
    }

    CAEDRSocketInfo_t *socketInfo = (CAEDRSocketInfo_t *) u_arraylist_get(g_deviceObjectList,
                                                                          index);
    if (!socketInfo)
    {
        OIC_LOG(ERROR, TAG, "socketInfo is null");
        return NULL;
    }

    jobject jarrayObj = socketInfo->inputStream;
    if (!jarrayObj)
    {
        OIC_LOG(ERROR, TAG, "jarrayObj is not available");
        return NULL;
    }
    return jarrayObj;
}

uint32_t CAEDRGetSocketListLength()
{
    if (!g_deviceObjectList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceObjectList is null");
        return 0;
    }

    return u_arraylist_length(g_deviceObjectList);
}

CAConnectedDeviceInfo_t *CAEDRGetDeviceInfoFromAddress(const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CAEDRGetDeviceInfoFromAddress");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress", NULL);

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "gdeviceStateList is null");
        return NULL;
    }

    jint length = u_arraylist_length(g_deviceStateList);
    for (jint index = 0; index < length; index++)
    {
        CAConnectedDeviceInfo_t* deviceInfo =
                (CAConnectedDeviceInfo_t*) u_arraylist_get(g_deviceStateList, index);
        if (!deviceInfo)
        {
            OIC_LOG(DEBUG, TAG, "deviceInfo object is null");
            continue;
        }

        if (!strcmp((const char*) deviceInfo->address, remoteAddress))
        {
            return deviceInfo;
        }
    }
    return NULL;
}
