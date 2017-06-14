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
#include <string.h>
#include <android/log.h>
#include <unistd.h>
#include "caleserver.h"
#include "caleutils.h"
#include "caleinterface.h"
#include "caadapterutils.h"
#include "calestate.h"

#include "logger.h"
#include "oic_malloc.h"
#include "cathreadpool.h"
#include "octhread.h"
#include "uarraylist.h"
#include "org_iotivity_ca_CaLeServerInterface.h"

#define TAG PCF("OIC_CA_LE_SERVER")

#define WAIT_TIME_WRITE_CHARACTERISTIC 10000000
#define INVALID_STATE -1

static JavaVM *g_jvm = NULL;
static jobject g_context = NULL;
static jobject g_bluetoothGattServer = NULL;
static jobject g_bluetoothGattServerCallback = NULL;
static jobject g_leAdvertiseCallback = NULL;
static jobject g_bluetoothManager = NULL;

static CAPacketReceiveCallback g_packetReceiveCallback = NULL;
static CABLEErrorHandleCallback g_serverErrorCallback;

static u_arraylist_t *g_connectedDeviceList = NULL;
static u_arraylist_t *g_deviceStateList = NULL;

static bool g_isStartServer = false;
static bool g_isInitializedServer = false;

static jbyteArray g_sendBuffer = NULL;
static jobject g_obj_bluetoothDevice = NULL;

static CABLEDataReceivedCallback g_CABLEServerDataReceivedCallback = NULL;
static oc_mutex g_bleReqRespCbMutex = NULL;
static oc_mutex g_bleClientBDAddressMutex = NULL;
static oc_mutex g_connectedDeviceListMutex = NULL;

static oc_mutex g_threadSendMutex = NULL;
static oc_mutex g_threadSendNotifyMutex = NULL;
static oc_cond g_threadSendNotifyCond = NULL;
static bool g_isSignalSetFlag = false;

static oc_mutex g_deviceStateListMutex = NULL;

static jint g_state_connected = INVALID_STATE;
static jint g_state_disconnected = INVALID_STATE;

static const char CLASSPATH_BT_ADVERTISE_CB[] = "android/bluetooth/le/AdvertiseCallback";
static const char CLASSPATH_BT_GATTSERVER[] = "android/bluetooth/BluetoothGattServer";

static bool g_setHighQoS = true;

void CALEServerJNISetContext()
{
    OIC_LOG(DEBUG, TAG, "CALEServerJNISetContext");
    g_context = (jobject) CANativeJNIGetContext();
}

void CALeServerJniInit()
{
    OIC_LOG(DEBUG, TAG, "CALeServerJniInit");
    g_jvm = (JavaVM*) CANativeJNIGetJavaVM();
}

CAResult_t CALEServerCreateJniInterfaceObject()
{
    OIC_LOG(DEBUG, TAG, "CALEServerCreateJniInterfaceObject");

    if (!g_context)
    {
        OIC_LOG(ERROR, TAG, "g_context is null");
        return CA_STATUS_FAILED;
    }

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    jclass jni_LEInterface = (*env)->FindClass(env, "org/iotivity/ca/CaLeServerInterface");
    if (!jni_LEInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaLeServerInterface class");
        goto exit;
    }

    jmethodID LeInterfaceConstructorMethod = (*env)->GetMethodID(env, jni_LEInterface, "<init>",
                                                                 "()V");
    if (!LeInterfaceConstructorMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaLeServerInterface constructor method");
        goto exit;
    }

    (*env)->NewObject(env, jni_LEInterface, LeInterfaceConstructorMethod, g_context);
    OIC_LOG(DEBUG, TAG, "Create instance for CaLeServerInterface");

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;

    exit:

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_FAILED;
}

/**
 * get the current connection state of the gatt profile to the remote device.
 * @param[in]   env              JNI interface pointer.
 * @param[in]   device           bluetooth device object
 * @return  state of the profile connection.
 */
static jint CALEServerGetConnectionState(JNIEnv *env, jobject device)
{
    OIC_LOG(DEBUG, TAG, "CALEServerGetConnectionState");

    VERIFY_NON_NULL_RET(env, TAG, "env", -1);
    VERIFY_NON_NULL_RET(device, TAG, "device", -1);

    jmethodID jni_mid_getConnectionState = CAGetJNIMethodID(env, "android/bluetooth"
                                                            "/BluetoothManager",
                                                            "getConnectionState",
                                                            "(Landroid/bluetooth/BluetoothDevice"
                                                            ";I)I");
    if (!jni_mid_getConnectionState)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getConnectionState is null");
        return -1;
    }

    if (!g_bluetoothManager)
    {
        OIC_LOG(ERROR, TAG, "g_bluetoothManager is null");
        return -1;
    }

    jint jni_state = (jint)(*env)->CallIntMethod(env, g_bluetoothManager,
                                                 jni_mid_getConnectionState,
                                                 device, GATT_PROFILE);
    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "getConnectionState has failed");
        return -1;
    }

    OIC_LOG_V(INFO, TAG, "connection state is %d", jni_state);
    return jni_state;
}

jobject CALEServerSetResponseData(JNIEnv *env, jbyteArray responseData)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerSetResponseData");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(responseData, TAG, "responseData is null", NULL);

    if (!g_bluetoothGattServer)
    {
        OIC_LOG(ERROR, TAG, "Check BluetoothGattServer status");
        return NULL;
    }

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "CALEServerSetResponseData");
    jmethodID jni_mid_getService = CAGetJNIMethodID(env, CLASSPATH_BT_GATTSERVER,
                                                    "getService",
                                                    "(Ljava/util/UUID;)Landroid/bluetooth/"
                                                    "BluetoothGattService;");
    if (!jni_mid_getService)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getService is null");
        return NULL;
    }

    jobject jni_obj_serviceUUID = CALEGetUuidFromString(env, OIC_GATT_SERVICE_UUID);
    if (!jni_obj_serviceUUID)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_serviceUUID is null");
        return NULL;
    }

    jobject jni_obj_bluetoothGattService = (*env)->CallObjectMethod(env, g_bluetoothGattServer,
                                                                    jni_mid_getService,
                                                                    jni_obj_serviceUUID);
    if (!jni_obj_bluetoothGattService)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_bluetoothGattService is null");
        CACheckJNIException(env);
        return NULL;
    }

    jmethodID jni_mid_getCharacteristic = CAGetJNIMethodID(env, "android/bluetooth/"
                                                           "BluetoothGattService",
                                                           "getCharacteristic",
                                                           "(Ljava/util/UUID;)"
                                                           "Landroid/bluetooth/"
                                                           "BluetoothGattCharacteristic;");
    if (!jni_mid_getCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getCharacteristic is null");
        return NULL;
    }

    jobject jni_obj_responseUUID = CALEGetUuidFromString(env,
                                                         OIC_GATT_CHARACTERISTIC_RESPONSE_UUID);
    if (!jni_obj_responseUUID)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_responseUUID is null");
        return NULL;
    }

    jobject jni_obj_bluetoothGattCharacteristic = (*env)->CallObjectMethod(
            env, jni_obj_bluetoothGattService, jni_mid_getCharacteristic, jni_obj_responseUUID);
    if (!jni_obj_bluetoothGattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_bluetoothGattCharacteristic is null");
        CACheckJNIException(env);
        return NULL;
    }

    jmethodID jni_mid_setValue = CAGetJNIMethodID(env, "android/bluetooth/"
                                                  "BluetoothGattCharacteristic",
                                                  "setValue", "([B)Z");
    if (!jni_mid_setValue)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setValue is null");
        return NULL;
    }

    jboolean jni_boolean_setValue = (*env)->CallBooleanMethod(env,
                                                              jni_obj_bluetoothGattCharacteristic,
                                                              jni_mid_setValue, responseData);
    if (JNI_FALSE == jni_boolean_setValue)
    {
        OIC_LOG(ERROR, TAG, "Fail to set response data");
        CACheckJNIException(env);
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerSetResponseData");
    return jni_obj_bluetoothGattCharacteristic;
}

CAResult_t CALEServerSendResponseData(JNIEnv *env, jobject device, jobject responseData)
{
    OIC_LOG(DEBUG, TAG, "CALEServerSendResponseData");
    VERIFY_NON_NULL(responseData, TAG, "responseData is null");
    VERIFY_NON_NULL(device, TAG, "device is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    if (!g_bluetoothGattServer)
    {
        OIC_LOG(ERROR, TAG, "g_bluetoothGattServer is not available");
        return CA_STATUS_FAILED;
    }

    if (g_state_connected != CALEServerGetConnectionState(env, device))
    {
        OIC_LOG(ERROR, TAG, "it is not connected state");
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_notifyCharacteristicChanged = CAGetJNIMethodID(env,
                                                      CLASSPATH_BT_GATTSERVER,
                                                      "notifyCharacteristicChanged",
                                                      "(Landroid/bluetooth/BluetoothDevice;"
                                                      "Landroid/bluetooth/"
                                                      "BluetoothGattCharacteristic;Z)Z");
    if (!jni_mid_notifyCharacteristicChanged)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_notifyCharacteristicChanged is null");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "CALL API - notifyCharacteristicChanged");

    jboolean jni_boolean_notifyCharacteristicChanged = (*env)->CallBooleanMethod(
            env, g_bluetoothGattServer, jni_mid_notifyCharacteristicChanged, device, responseData,
            JNI_FALSE);
    if (JNI_FALSE == jni_boolean_notifyCharacteristicChanged)
    {
        OIC_LOG(ERROR, TAG, "Fail to notify characteristic");
        CACheckJNIException(env);
        return CA_SEND_FAILED;
    }

    OIC_LOG_V(DEBUG, TAG, "callback flag is %d", g_isSignalSetFlag);
    oc_mutex_lock(g_threadSendNotifyMutex);
    if (!g_isSignalSetFlag)
    {
        OIC_LOG(DEBUG, TAG, "wait for callback to notify notifyCharacteristic is success");
        if (0 != oc_cond_wait_for(g_threadSendNotifyCond, g_threadSendNotifyMutex,
                                  WAIT_TIME_WRITE_CHARACTERISTIC))
        {
            OIC_LOG(ERROR, TAG, "there is no response. notifyCharacteristic has failed");
            oc_mutex_unlock(g_threadSendNotifyMutex);
            return CA_STATUS_FAILED;
        }
    }
    // reset flag set by writeCharacteristic Callback
    g_isSignalSetFlag = false;
    oc_mutex_unlock(g_threadSendNotifyMutex);
    OIC_LOG(INFO, TAG, "notifyCharacteristic success");
    return CA_STATUS_OK;
}

CAResult_t CALEServerSendResponse(JNIEnv *env, jobject device, jint requestId, jint status,
                                        jint offset, jbyteArray value)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerSendResponse");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(device, TAG, "device is null");
    VERIFY_NON_NULL(value, TAG, "value is null");

    OIC_LOG(DEBUG, TAG, "CALEServerSendResponse");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    if (!g_bluetoothGattServer)
    {
        OIC_LOG(ERROR, TAG, "g_bluetoothGattServer is not available");
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_sendResponse = CAGetJNIMethodID(env, CLASSPATH_BT_GATTSERVER,
                                                      "sendResponse",
                                                      "(Landroid/bluetooth/BluetoothDevice;"
                                                      "III[B)Z");
    if (!jni_mid_sendResponse)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_sendResponse is null");
        return CA_STATUS_FAILED;
    }

    jboolean jni_boolean_sendResponse = (*env)->CallBooleanMethod(env, g_bluetoothGattServer,
                                                                  jni_mid_sendResponse, device,
                                                                  requestId, status, offset,
                                                                  value);
    if (JNI_FALSE == jni_boolean_sendResponse)
    {
        OIC_LOG(ERROR, TAG, "Fail to send response for gatt characteristic write request");
        CACheckJNIException(env);
        return CA_SEND_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerSendResponse");
    return CA_STATUS_OK;
}

CAResult_t CALEServerStartAdvertise()
{
    if ((caglobals.bleFlags & CA_LE_ADV_DISABLE) || CA_DEFAULT_BT_FLAGS == caglobals.bleFlags)
    {
        OIC_LOG_V(INFO, TAG, "the advertisement of the bleFlags is disable[%d]",
                  caglobals.bleFlags);
        return CA_STATUS_OK;
    }

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    // start advertise
    CAResult_t ret = CALEServerStartAdvertiseImpl(env, g_leAdvertiseCallback);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStartAdvertiseImpl has failed");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }
    return ret;
}

CAResult_t CALEServerStartAdvertiseImpl(JNIEnv *env, jobject advertiseCallback)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStartAdvertiseImpl");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(advertiseCallback, TAG, "advertiseCallback is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    jclass jni_cid_AdvertiseSettings = (*env)->FindClass(env,
                                                         "android/bluetooth/le/"
                                                         "AdvertiseSettings$Builder");
    if (!jni_cid_AdvertiseSettings)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_AdvertiseSettings is null");
        goto error_exit;
    }

    jmethodID jni_mid_AdvertiseSettings = (*env)->GetMethodID(env, jni_cid_AdvertiseSettings,
                                                              "<init>", "()V");
    if (!jni_mid_AdvertiseSettings)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_AdvertiseSettings is null");
        goto error_exit;
    }

    jobject jni_AdvertiseSettings = (*env)->NewObject(env, jni_cid_AdvertiseSettings,
                                                      jni_mid_AdvertiseSettings);
    if (!jni_AdvertiseSettings)
    {
        OIC_LOG(ERROR, TAG, "jni_AdvertiseSettings is null");
        goto error_exit;
    }

    jmethodID jni_mid_setAdvertiseMode = (*env)->GetMethodID(env, jni_cid_AdvertiseSettings,
                                                             "setAdvertiseMode",
                                                             "(I)Landroid/bluetooth/le/"
                                                             "AdvertiseSettings$Builder;");
    if (!jni_mid_setAdvertiseMode)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setAdvertiseMode is null");
        goto error_exit;
    }

    // 0: Low power, 1: Balanced
    jobject jni_obj_setAdvertiseMode = (*env)->CallObjectMethod(env, jni_AdvertiseSettings,
                                                                jni_mid_setAdvertiseMode, 0);
    if (!jni_obj_setAdvertiseMode)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_setAdvertiseMode is null");
        goto error_exit;
    }

    jmethodID jni_mid_setConnectable = (*env)->GetMethodID(env, jni_cid_AdvertiseSettings,
                                                           "setConnectable",
                                                           "(Z)Landroid/bluetooth/le/"
                                                           "AdvertiseSettings$Builder;");
    if (!jni_mid_setConnectable)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setConnectable is null");
        goto error_exit;
    }

    jobject jni_obj_setConnectable = (*env)->CallObjectMethod(env, jni_AdvertiseSettings,
                                                              jni_mid_setConnectable, JNI_TRUE);
    if (!jni_obj_setConnectable)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_setConnectable is null");
        goto error_exit;
    }

    jmethodID jni_mid_setTimeout = (*env)->GetMethodID(env, jni_cid_AdvertiseSettings, "setTimeout",
                                                       "(I)Landroid/bluetooth/le/"
                                                       "AdvertiseSettings$Builder;");
    if (!jni_mid_setTimeout)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setTimeout is null");
        goto error_exit;
    }

    //A value of 0 will disable the time limit
    jobject jni_obj_setTimeout = (*env)->CallObjectMethod(env, jni_AdvertiseSettings,
                                                          jni_mid_setTimeout, 0);
    if (!jni_obj_setTimeout)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_setTimeout is null");
        goto error_exit;
    }

    jclass jni_cid_AdvertiseDataBuilder = (*env)->FindClass(env,
                                                            "android/bluetooth/le/"
                                                            "AdvertiseData$Builder");
    if (!jni_cid_AdvertiseDataBuilder)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_AdvertiseDataBuilder is null");
        goto error_exit;
    }

    jmethodID jni_mid_AdvertiseDataBuilder = (*env)->GetMethodID(env, jni_cid_AdvertiseDataBuilder,
                                                                 "<init>", "()V");
    if (!jni_mid_AdvertiseDataBuilder)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_AdvertiseDataBuilder is null");
        goto error_exit;
    }

    jobject jni_AdvertiseDataBuilder = (*env)->NewObject(env, jni_cid_AdvertiseDataBuilder,
                                                         jni_mid_AdvertiseDataBuilder);
    if (!jni_AdvertiseDataBuilder)
    {
        OIC_LOG(ERROR, TAG, "jni_AdvertiseDataBuilder is null");
        goto error_exit;
    }

    jobject jni_AdvertiseDataBuilderForScanRsp = (*env)->NewObject(env,
                                                                   jni_cid_AdvertiseDataBuilder,
                                                                   jni_mid_AdvertiseDataBuilder);
    if (!jni_AdvertiseDataBuilderForScanRsp)
    {
        OIC_LOG(ERROR, TAG, "jni_AdvertiseDataBuilderForScanRsp is null");
        goto error_exit;
    }

    jobject jni_obj_serviceUUID = CALEGetUuidFromString(env, OIC_GATT_SERVICE_UUID);
    if (!jni_obj_serviceUUID)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_serviceUUID is null");
        return CA_STATUS_FAILED;
    }

    jobject jni_ParcelUuid = CALEGetParcelUuid(env, jni_obj_serviceUUID);
    if (!jni_ParcelUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_ParcelUuid is null");
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_addServiceUuid = (*env)->GetMethodID(env, jni_cid_AdvertiseDataBuilder,
                                                           "addServiceUuid",
                                                           "(Landroid/os/ParcelUuid;)Landroid/"
                                                           "bluetooth/le/AdvertiseData$Builder;");
    if (!jni_mid_addServiceUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_addServiceUuid is null");
        goto error_exit;
    }

    jobject jni_obj_addServiceUuid = (*env)->CallObjectMethod(env, jni_AdvertiseDataBuilder,
                                                              jni_mid_addServiceUuid,
                                                              jni_ParcelUuid);
    if (!jni_obj_addServiceUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_addServiceUuid is null");
        goto error_exit;
    }

    // Device name has to be included in advertise packet after Android API 23
    OIC_LOG(DEBUG, TAG, "device name will be added into advertise packet");
    jmethodID jni_mid_setIncludeDeviceName = (*env)->GetMethodID(env, jni_cid_AdvertiseDataBuilder,
                                                                 "setIncludeDeviceName",
                                                                 "(Z)Landroid/"
                                                                 "bluetooth/le/"
                                                                 "AdvertiseData$Builder;");
    if (!jni_mid_setIncludeDeviceName)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setIncludeDeviceName is null");
        goto error_exit;
    }

    jobject jni_obj_setIncludeDeviceName  = (*env)->CallObjectMethod(env,
                                                               jni_AdvertiseDataBuilderForScanRsp,
                                                               jni_mid_setIncludeDeviceName,
                                                               JNI_TRUE);
    if (!jni_obj_setIncludeDeviceName)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_setIncludeDeviceName is null");
        goto error_exit;
    }

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, "android/bluetooth/BluetoothAdapter");
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    "()Landroid/bluetooth/"
                                                                    "BluetoothAdapter;");
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        goto error_exit;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        goto error_exit;
    }

    jmethodID jni_mid_getBluetoothLeAdvertiser = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                                     "getBluetoothLeAdvertiser",
                                                                     "()Landroid/bluetooth/le/"
                                                                     "BluetoothLeAdvertiser;");
    if (!jni_mid_getBluetoothLeAdvertiser)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBluetoothLeAdvertiser is null");
        goto error_exit;
    }

    jobject jni_obj_getBluetoothLeAdvertiser = (*env)->CallObjectMethod(
            env, jni_obj_BTAdapter, jni_mid_getBluetoothLeAdvertiser);
    if (!jni_obj_getBluetoothLeAdvertiser)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_getBluetoothLeAdvertiser is null");
        goto error_exit;
    }

    jmethodID jni_mid_build_LeAdvertiseSettings = (*env)->GetMethodID(env,
                                                                      jni_cid_AdvertiseSettings,
                                                                      "build",
                                                                      "()Landroid/bluetooth/le/"
                                                                      "AdvertiseSettings;");
    if (!jni_mid_build_LeAdvertiseSettings)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_build_LeAdvertiseSettings is null");
        goto error_exit;
    }

    jobject jni_obj_build_LeAdvertiseSettings = (*env)->CallObjectMethod(
            env, jni_AdvertiseSettings, jni_mid_build_LeAdvertiseSettings);
    if (!jni_obj_build_LeAdvertiseSettings)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_build_LeAdvertiseSettings is null");
        goto error_exit;
    }

    jmethodID jni_mid_build_LeAdvertiseData = (*env)->GetMethodID(env, jni_cid_AdvertiseDataBuilder,
                                                                  "build",
                                                                  "()Landroid/bluetooth/le/"
                                                                  "AdvertiseData;");
    if (!jni_mid_build_LeAdvertiseData)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_build_LeAdvertiseData is null");
        goto error_exit;
    }

    jobject jni_obj_build_LeAdvertiseData = (*env)->CallObjectMethod(env, jni_AdvertiseDataBuilder,
                                                                     jni_mid_build_LeAdvertiseData);
    if (!jni_obj_build_LeAdvertiseData)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_build_LeAdvertiseData is null");
        goto error_exit;
    }

    jobject jni_obj_build_LeAdvertiseDataForScanRsp = (*env)->CallObjectMethod(env,
                                                                jni_AdvertiseDataBuilderForScanRsp,
                                                                jni_mid_build_LeAdvertiseData);
    if (!jni_obj_build_LeAdvertiseDataForScanRsp)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_build_LeAdvertiseDataForScanRsp is null");
        goto error_exit;
    }

    jmethodID jni_mid_startAdvertising = CAGetJNIMethodID(env, "android/bluetooth/le/"
                                                          "BluetoothLeAdvertiser",
                                                          "startAdvertising",
                                                          "(Landroid/bluetooth/le/"
                                                          "AdvertiseSettings;Landroid/bluetooth/"
                                                          "le/AdvertiseData;Landroid/bluetooth/"
                                                          "le/AdvertiseData;Landroid/bluetooth/"
                                                          "le/AdvertiseCallback;)V");
    if (!jni_mid_startAdvertising)
    {
       OIC_LOG(ERROR, TAG, "jni_mid_startAdvertising is null");
       return CA_STATUS_FAILED;
    }

    (*env)->CallVoidMethod(env, jni_obj_getBluetoothLeAdvertiser, jni_mid_startAdvertising,
                           jni_obj_build_LeAdvertiseSettings, jni_obj_build_LeAdvertiseData,
                           jni_obj_build_LeAdvertiseDataForScanRsp, advertiseCallback);
    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "StartAdvertising has failed");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "Advertising started!!");
    OIC_LOG(DEBUG, TAG, "OUT - CALEServerStartAdvertise");
    return CA_STATUS_OK;

error_exit:
    CACheckJNIException(env);
    return CA_STATUS_FAILED;
}

CAResult_t CALEServerStopAdvertise()
{
    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAResult_t ret = CALEServerStopAdvertiseImpl(env, g_leAdvertiseCallback);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStopAdvertise has failed");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return ret;
}

CAResult_t CALEServerStopAdvertiseImpl(JNIEnv *env, jobject advertiseCallback)
{
    OIC_LOG(DEBUG, TAG, "CALEServerStopAdvertiseImpl");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(advertiseCallback, TAG, "advertiseCallback is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, "android/bluetooth/BluetoothAdapter");
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        goto error_exit;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    "()Landroid/bluetooth/"
                                                                    "BluetoothAdapter;");
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        goto error_exit;
    }

    jmethodID jni_mid_getBTLeAdvertiser = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                                     "getBluetoothLeAdvertiser",
                                                                     "()Landroid/bluetooth/le/"
                                                                     "BluetoothLeAdvertiser;");
    if (!jni_mid_getBTLeAdvertiser)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBTLeAdvertiser is null");
        goto error_exit;
    }

    jmethodID jni_mid_stopAdvertising = CAGetJNIMethodID(env, "android/bluetooth/le/"
                                                         "BluetoothLeAdvertiser",
                                                         "stopAdvertising",
                                                         "(Landroid/bluetooth/le/"
                                                         "AdvertiseCallback;)V");
    if (!jni_mid_stopAdvertising)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_stopAdvertising is null");
        goto error_exit;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        goto error_exit;
    }

    jobject jni_obj_getBluetoothLeAdvertiser = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                                        jni_mid_getBTLeAdvertiser);
    if (!jni_obj_getBluetoothLeAdvertiser)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_getBluetoothLeAdvertiser is null");
        goto error_exit;
    }

    (*env)->CallVoidMethod(env, jni_obj_getBluetoothLeAdvertiser, jni_mid_stopAdvertising,
                           advertiseCallback);
    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "getBluetoothLeAdvertiser has failed");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "Advertising stopped!!");
    return CA_STATUS_OK;

error_exit:
    CACheckJNIException(env);
    return CA_STATUS_FAILED;
}

CAResult_t CALEServerStartGattServer(JNIEnv *env, jobject gattServerCallback)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStartGattServer");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(gattServerCallback, TAG, "gattServerCallback is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    if (g_isStartServer)
    {
        OIC_LOG(DEBUG, TAG, "Gatt server already started");
    }

    g_bluetoothGattServerCallback = (*env)->NewGlobalRef(env, gattServerCallback);

    // open gatt server
    jobject bluetoothGattServer = CALEServerOpenGattServer(env);
    if (!bluetoothGattServer)
    {
        OIC_LOG(ERROR, TAG, "bluetoothGattServer is null");
        return CA_STATUS_FAILED;
    }

    g_bluetoothGattServer = (*env)->NewGlobalRef(env, bluetoothGattServer);
    if (!g_bluetoothGattServer)
    {
        OIC_LOG(ERROR, TAG, "g_bluetoothGattServer is null");
        return CA_STATUS_FAILED;
    }

    // create gatt service
    jobject bluetoothGattService = CALEServerCreateGattService(env);
    if (!bluetoothGattService)
    {
        OIC_LOG(ERROR, TAG, "bluetoothGattService is null");
        return CA_STATUS_FAILED;
    }

    // add gatt service
    CAResult_t res = CALEServerAddGattService(env, g_bluetoothGattServer,
                                              bluetoothGattService);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEServerAddGattService has failed");
    }
    return res;
}

jobject CALEServerOpenGattServer(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerOpenGattServer");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        goto error_exit;
    }

    jclass jni_cid_context = (*env)->FindClass(env, "android/content/Context");
    if (!jni_cid_context)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_context is null");
        goto error_exit;
    }

    jfieldID jni_fid_bluetoothService = (*env)->GetStaticFieldID(env, jni_cid_context,
                                                                 "BLUETOOTH_SERVICE",
                                                                 "Ljava/lang/String;");
    if (!jni_fid_bluetoothService)
    {
        OIC_LOG(ERROR, TAG, "jni_fid_bluetoothService is null");
        goto error_exit;
    }

    jmethodID jni_mid_getSystemService = (*env)->GetMethodID(env, jni_cid_context,
                                                             "getSystemService",
                                                             "(Ljava/lang/String;)"
                                                             "Ljava/lang/Object;");
    if (!jni_mid_getSystemService)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getSystemService is null");
        goto error_exit;
    }

    jmethodID jni_mid_openGattServer = CAGetJNIMethodID(env, "android/bluetooth/"
                                                        "BluetoothManager",
                                                        "openGattServer",
                                                        "(Landroid/content/Context;"
                                                        "Landroid/bluetooth/"
                                                        "BluetoothGattServerCallback;)"
                                                        "Landroid/bluetooth/"
                                                        "BluetoothGattServer;");
    if (!jni_mid_openGattServer)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_openGattServer is null");
        return NULL;
    }

    jobject jni_obj_bluetoothService = (*env)->GetStaticObjectField(env, jni_cid_context,
                                                                    jni_fid_bluetoothService);
    if (!jni_obj_bluetoothService)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_bluetoothService is null");
        goto error_exit;
    }

    jobject jni_obj_bluetoothManager = (*env)->CallObjectMethod(env, g_context,
                                                                jni_mid_getSystemService,
                                                                jni_obj_bluetoothService);
    if (!jni_obj_bluetoothManager)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_bluetoothManager is null");
        goto error_exit;
    }

    if (g_bluetoothManager)
    {
        (*env)->DeleteGlobalRef(env, g_bluetoothManager);
    }
    g_bluetoothManager = (*env)->NewGlobalRef(env, jni_obj_bluetoothManager);

    jobject jni_obj_bluetoothGattServer = (*env)->CallObjectMethod(env, jni_obj_bluetoothManager,
                                                                   jni_mid_openGattServer,
                                                                   g_context,
                                                                   g_bluetoothGattServerCallback);
    if (!jni_obj_bluetoothGattServer)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_bluetoothGattServer is null");
        goto error_exit;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerOpenGattServer");
    return jni_obj_bluetoothGattServer;

error_exit:
    CACheckJNIException(env);
    return NULL;
}

jobject CALEServerCreateGattService(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerCreateGattService");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return NULL;
    }

    jclass jni_cid_bluetoothGattService = (*env)->FindClass(env, "android/bluetooth/"
                                                            "BluetoothGattService");
    if (!jni_cid_bluetoothGattService)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_bluetoothGattService is null");
        goto error_exit;
    }

    jclass jni_cid_bluetoothGattCharacteristic = (*env)->FindClass(env, "android/bluetooth/"
                                                                   "BluetoothGattCharacteristic");
    if (!jni_cid_bluetoothGattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_bluetoothGattCharacteristic is null");
        goto error_exit;
    }

    jfieldID jni_fid_serviceType = (*env)->GetStaticFieldID(env, jni_cid_bluetoothGattService,
                                                            "SERVICE_TYPE_PRIMARY", "I");
    if (!jni_fid_serviceType)
    {
        OIC_LOG(ERROR, TAG, "jni_fid_serviceType is null");
        goto error_exit;
    }

    jfieldID jni_fid_readProperties = NULL;
    jfieldID jni_fid_writeProperties = NULL;
    if (g_setHighQoS)
    {
        jni_fid_readProperties = (*env)->GetStaticFieldID(env,
                                                          jni_cid_bluetoothGattCharacteristic,
                                                          "PROPERTY_INDICATE", "I");
        if (!jni_fid_readProperties)
        {
            OIC_LOG(ERROR, TAG, "jni_fid_readProperties is null");
            goto error_exit;
        }

        jni_fid_writeProperties = (*env)->GetStaticFieldID(env,
                                                           jni_cid_bluetoothGattCharacteristic,
                                                           "PROPERTY_WRITE", "I");
        if (!jni_fid_writeProperties)
        {
            OIC_LOG(ERROR, TAG, "jni_fid_writeProperties is null");
            goto error_exit;
        }
    }
    else
    {
        jni_fid_readProperties = (*env)->GetStaticFieldID(env,
                                                          jni_cid_bluetoothGattCharacteristic,
                                                          "PROPERTY_NOTIFY", "I");
        if (!jni_fid_readProperties)
        {
            OIC_LOG(ERROR, TAG, "jni_fid_readProperties is null");
            goto error_exit;
        }

        jni_fid_writeProperties = (*env)->GetStaticFieldID(env,
                                                           jni_cid_bluetoothGattCharacteristic,
                                                           "PROPERTY_WRITE_NO_RESPONSE", "I");
        if (!jni_fid_writeProperties)
        {
            OIC_LOG(ERROR, TAG, "jni_fid_writeProperties is null");
            goto error_exit;
        }
    }

    jfieldID jni_fid_readPermissions = (*env)->GetStaticFieldID(env,
                                                                jni_cid_bluetoothGattCharacteristic,
                                                                "PERMISSION_READ", "I");
    if (!jni_fid_readPermissions)
    {
        OIC_LOG(ERROR, TAG, "jni_fid_readPermissions is null");
        goto error_exit;
    }

    jfieldID jni_fid_writePermissions = (*env)->GetStaticFieldID(
            env, jni_cid_bluetoothGattCharacteristic, "PERMISSION_WRITE", "I");
    if (!jni_fid_writePermissions)
    {
        OIC_LOG(ERROR, TAG, "jni_fid_writePermissions is null");
        goto error_exit;
    }

    jmethodID jni_mid_bluetoothGattService = (*env)->GetMethodID(env, jni_cid_bluetoothGattService,
                                                                 "<init>", "(Ljava/util/UUID;I)V");
    if (!jni_mid_bluetoothGattService)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_bluetoothGattService is null");
        goto error_exit;
    }

    jmethodID jni_mid_addCharacteristic = (*env)->GetMethodID(env, jni_cid_bluetoothGattService,
                                                              "addCharacteristic",
                                                              "(Landroid/bluetooth/"
                                                              "BluetoothGattCharacteristic;)Z");
    if (!jni_mid_addCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_addCharacteristic is null");
        goto error_exit;
    }

    jmethodID jni_mid_bluetoothGattCharacteristic = (*env)->GetMethodID(
            env, jni_cid_bluetoothGattCharacteristic, "<init>", "(Ljava/util/UUID;II)V");
    if (!jni_mid_bluetoothGattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_bluetoothGattCharacteristic is null");
        goto error_exit;
    }

    jobject jni_obj_serviceUUID = CALEGetUuidFromString(env, OIC_GATT_SERVICE_UUID);
    if (!jni_obj_serviceUUID)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_serviceUUID is null");
        return NULL;
    }

    jint jni_int_serviceType = (*env)->GetStaticIntField(env, jni_cid_bluetoothGattService,
                                                         jni_fid_serviceType);
    jobject jni_bluetoothGattService = (*env)->NewObject(env, jni_cid_bluetoothGattService,
                                                         jni_mid_bluetoothGattService,
                                                         jni_obj_serviceUUID, jni_int_serviceType);
    if (!jni_bluetoothGattService)
    {
        OIC_LOG(ERROR, TAG, "jni_bluetoothGattService is null");
        goto error_exit;
    }

    jobject jni_obj_readUuid = CALEGetUuidFromString(env, OIC_GATT_CHARACTERISTIC_RESPONSE_UUID);
    if (!jni_obj_readUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_readUuid is null");
        return NULL;
    }

    jint jni_int_readProperties = (*env)->GetStaticIntField(env,
                                                            jni_cid_bluetoothGattCharacteristic,
                                                            jni_fid_readProperties);
    CACheckJNIException(env);

    jint jni_int_readPermissions = (*env)->GetStaticIntField(env,
                                                             jni_cid_bluetoothGattCharacteristic,
                                                             jni_fid_readPermissions);
    CACheckJNIException(env);

    jint jni_int_writePermissions = (*env)->GetStaticIntField(env,
                                                              jni_cid_bluetoothGattCharacteristic,
                                                              jni_fid_writePermissions);
    CACheckJNIException(env);

    jobject jni_readCharacteristic = (*env)->NewObject(env, jni_cid_bluetoothGattCharacteristic,
                                                       jni_mid_bluetoothGattCharacteristic,
                                                       jni_obj_readUuid, jni_int_readProperties,
                                                       jni_int_readPermissions|
                                                       jni_int_writePermissions);
    if (!jni_readCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_readCharacteristic is null");
        goto error_exit;
    }

    jboolean jni_boolean_addReadCharacteristic = (*env)->CallBooleanMethod(
            env, jni_bluetoothGattService, jni_mid_addCharacteristic, jni_readCharacteristic);
    if (!jni_boolean_addReadCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_boolean_addReadCharacteristic is null");
        goto error_exit;
    }

    jobject jni_obj_writeUuid = CALEGetUuidFromString(env, OIC_GATT_CHARACTERISTIC_REQUEST_UUID);
    if (!jni_obj_writeUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_bluetoothGattServer is null");
        return NULL;
    }

    jint jni_int_writeProperties = (*env)->GetStaticIntField(env,
                                                             jni_cid_bluetoothGattCharacteristic,
                                                             jni_fid_writeProperties);
    CACheckJNIException(env);

    jobject jni_writeCharacteristic = (*env)->NewObject(env, jni_cid_bluetoothGattCharacteristic,
                                                        jni_mid_bluetoothGattCharacteristic,
                                                        jni_obj_writeUuid, jni_int_writeProperties,
                                                        jni_int_writePermissions);
    if (!jni_writeCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_writeCharacteristic is null");
        goto error_exit;
    }

    jboolean jni_boolean_addWriteCharacteristic = (*env)->CallBooleanMethod(
            env, jni_bluetoothGattService, jni_mid_addCharacteristic, jni_writeCharacteristic);
    if (JNI_FALSE == jni_boolean_addWriteCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "Fail to add jni_boolean_addReadCharacteristic");
        goto error_exit;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerCreateGattService");
    return jni_bluetoothGattService;

error_exit:
    CACheckJNIException(env);
    return NULL;
}

CAResult_t CALEServerAddDescriptor(JNIEnv *env, jobject characteristic)
{
    OIC_LOG(DEBUG, TAG, "CALEServerAddDescriptor");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(characteristic, TAG, "characteristic is null");

    jclass jni_cid_bluetoothGattDescriptor = (*env)->FindClass(env, "android/bluetooth/"
                                                               "BluetoothGattDescriptor");
    if (!jni_cid_bluetoothGattDescriptor)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_bluetoothGattDescriptor is null");
        goto error_exit;
    }

    jmethodID jni_mid_bluetoothGattDescriptor = (*env)->GetMethodID(env,
                                                                    jni_cid_bluetoothGattDescriptor,
                                                                    "<init>",
                                                                    "(Ljava/util/UUID;I)V");
    if (!jni_mid_bluetoothGattDescriptor)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_bluetoothGattDescriptor is null");
        goto error_exit;
    }

    jfieldID jni_fid_readPermissions = (*env)->GetStaticFieldID(env,
                                                                jni_cid_bluetoothGattDescriptor,
                                                                "PERMISSION_READ", "I");
    if (!jni_fid_readPermissions)
    {
        OIC_LOG(ERROR, TAG, "jni_fid_readPermissions is null");
        goto error_exit;
    }

    jobject jni_obj_readUuid = CALEGetUuidFromString(env, OIC_GATT_CHARACTERISTIC_CONFIG_UUID);
    if (!jni_obj_readUuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_readUuid is null");
        return CA_STATUS_FAILED;
    }

    jint jni_int_readPermissions = (*env)->GetStaticIntField(env, jni_cid_bluetoothGattDescriptor,
                                                             jni_fid_readPermissions);
    CACheckJNIException(env);

    OIC_LOG(DEBUG, TAG, "initialize new Descriptor");

    jobject jni_readDescriptor = (*env)->NewObject(env, jni_cid_bluetoothGattDescriptor,
                                                   jni_mid_bluetoothGattDescriptor,
                                                   jni_obj_readUuid, jni_int_readPermissions);
    if (!jni_readDescriptor)
    {
        OIC_LOG(ERROR, TAG, "jni_readDescriptor is null");
        goto error_exit;
    }

    jclass jni_cid_GattCharacteristic = (*env)->FindClass(env, "android/bluetooth/"
                                                          "BluetoothGattCharacteristic");
    if (!jni_cid_GattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_GattCharacteristic is null");
        goto error_exit;
    }

    jmethodID jni_mid_addDescriptor = (*env)->GetMethodID(env, jni_cid_GattCharacteristic,
                                                          "addDescriptor",
                                                          "(Landroid/bluetooth/"
                                                          "BluetoothGattDescriptor;)Z");
    if (!jni_mid_addDescriptor)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_addDescriptor is null");
        goto error_exit;
    }

    jboolean jni_boolean_addDescriptor = (*env)->CallBooleanMethod(env, characteristic,
                                                                   jni_mid_addDescriptor,
                                                                   jni_readDescriptor);

    if (JNI_FALSE == jni_boolean_addDescriptor)
    {
        OIC_LOG(ERROR, TAG, "addDescriptor has failed");
        goto error_exit;
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "addDescriptor success");
    }
    return CA_STATUS_OK;

error_exit:
    CACheckJNIException(env);
    return CA_STATUS_FAILED;
}

CAResult_t CALEServerAddGattService(JNIEnv *env, jobject bluetoothGattServer,
                                          jobject bluetoothGattService)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerAddGattService");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGattServer, TAG, "bluetoothGattServer is null");
    VERIFY_NON_NULL(bluetoothGattService, TAG, "bluetoothGattService is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    jmethodID jni_mid_addService = CAGetJNIMethodID(env, CLASSPATH_BT_GATTSERVER,
                                                    "addService",
                                                    "(Landroid/bluetooth/BluetoothGattService;)"
                                                    "Z");
     if (!jni_mid_addService)
     {
         OIC_LOG(ERROR, TAG, "jni_mid_addService is null");
         return CA_STATUS_FAILED;
     }

    jboolean jni_boolean_addService = (*env)->CallBooleanMethod(env, bluetoothGattServer,
                                                                jni_mid_addService,
                                                                bluetoothGattService);

    if (JNI_FALSE == jni_boolean_addService)
    {
        OIC_LOG(ERROR, TAG, "Fail to add GATT service");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerAddGattService");
    return CA_STATUS_OK;
}

CAResult_t CALEServerConnect(JNIEnv *env, jobject bluetoothDevice)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerConnect");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothDevice, TAG, "bluetoothDevice is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    jmethodID jni_mid_connect = CAGetJNIMethodID(env, CLASSPATH_BT_GATTSERVER,
                                                 "connect",
                                                 "(Landroid/bluetooth/BluetoothDevice;Z)Z");
    if (!jni_mid_connect)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_connect is null");
        return CA_STATUS_FAILED;
    }

    jboolean jni_boolean_connect = (*env)->CallBooleanMethod(env, g_bluetoothGattServer,
                                                             jni_mid_connect, bluetoothDevice,
                                                             JNI_FALSE);
    if (JNI_FALSE == jni_boolean_connect)
    {
        OIC_LOG(ERROR, TAG, "Fail to connect");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerConnect");
    return CA_STATUS_OK;
}

CAResult_t CALEServerDisconnectAllDevices(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerDisconnectAllDevices");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_connectedDeviceListMutex);
    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceList is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_connectedDeviceList, index);
        if (!jarrayObj)
        {
            OIC_LOG_V(ERROR, TAG, "object[%d] is null", index);
            continue;
        }

        // disconnect for device obj
        CAResult_t res = CALEServerDisconnect(env, jarrayObj);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Disconnect for this device[%d] has failed", index);
            continue;
        }
    }

    oc_mutex_unlock(g_connectedDeviceListMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALEServerDisconnectAllDevices");
    return CA_STATUS_OK;
}

CAResult_t CALEServerDisconnect(JNIEnv *env, jobject bluetoothDevice)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerDisconnect");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothDevice, TAG, "bluetoothDevice is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    jmethodID jni_mid_cancelConnection = CAGetJNIMethodID(env, CLASSPATH_BT_GATTSERVER,
                                                          "cancelConnection",
                                                          "(Landroid/bluetooth/BluetoothDevice;)"
                                                          "V");
    if (!jni_mid_cancelConnection)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_cancelConnection is null");
        return CA_STATUS_FAILED;
    }

    (*env)->CallVoidMethod(env, g_bluetoothGattServer, jni_mid_cancelConnection, bluetoothDevice);

    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "cancelConnection has failed");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerDisconnect");
    return CA_STATUS_OK;
}

CAResult_t CALEServerGattClose(JNIEnv *env, jobject bluetoothGattServer)
{
    // GATT CLOSE
    OIC_LOG(DEBUG, TAG, "GattServer Close");
    VERIFY_NON_NULL(bluetoothGattServer, TAG, "bluetoothGattServer is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    // get BluetoothGatt class
    OIC_LOG(DEBUG, TAG, "get BluetoothGatt class");
    jmethodID jni_mid_closeGatt = CAGetJNIMethodID(env, CLASSPATH_BT_GATTSERVER,
                                                   "close", "()V");
    if (!jni_mid_closeGatt)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_closeGatt is null");
        return CA_STATUS_OK;
    }

    // call disconnect gatt method
    OIC_LOG(DEBUG, TAG, "request to close GATT");
    (*env)->CallVoidMethod(env, bluetoothGattServer, jni_mid_closeGatt);

    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "closeGATT has failed");
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CALEServerSend(JNIEnv *env, jobject bluetoothDevice, jbyteArray responseData)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerSend");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothDevice, TAG, "bluetoothDevice is null");
    VERIFY_NON_NULL(responseData, TAG, "responseData is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    jobject responseChar = CALEServerSetResponseData(env, responseData);
    if (!responseChar)
    {
        OIC_LOG(ERROR, TAG, "responseChar is null");
        return CA_STATUS_FAILED;
    }

    CAResult_t result = CALEServerSendResponseData(env, bluetoothDevice, responseChar);
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Fail to send response data");
        return result;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerSend");
    return result;
}

CAResult_t CALEServerInitialize()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerInitialize");

    CALeServerJniInit();

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAResult_t ret = CALECheckPlatformVersion(env, 21);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "it is not supported");

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }
        return ret;
    }

    g_threadSendNotifyCond = oc_cond_new();

    ret = CALEServerInitMutexVaraibles();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerInitMutexVaraibles has failed");

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }
        return CA_STATUS_FAILED;
    }

    CALEServerJNISetContext();
    CALEServerCreateCachedDeviceList();

    ret = CALEServerCreateJniInterfaceObject();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerCreateJniInterfaceObject has failed");

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }
        return CA_STATUS_FAILED;
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    g_isInitializedServer = true;
    OIC_LOG(DEBUG, TAG, "OUT - CALEServerInitialize");
    return CA_STATUS_OK;
}

void CALEServerTerminate()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerTerminate");

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return;
        }
        isAttached = true;
    }

    if (g_sendBuffer)
    {
        (*env)->DeleteGlobalRef(env, g_sendBuffer);
        g_sendBuffer = NULL;
    }

    if (g_bluetoothManager)
    {
        (*env)->DeleteGlobalRef(env, g_bluetoothManager);
        g_bluetoothManager = NULL;
    }

    oc_cond_free(g_threadSendNotifyCond);
    g_threadSendNotifyCond = NULL;

    CALEServerTerminateMutexVaraibles();
    CALEServerTerminateConditionVaraibles();

    g_isInitializedServer = false;

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerTerminate");
}

CAResult_t CALEServerSendUnicastMessage(const char* address, const uint8_t* data, uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendUnicastMessage(%s, %p)", address, data);
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAResult_t ret = CALEServerSendUnicastMessageImpl(env, address, data, dataLen);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerSendUnicastMessageImpl has failed");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return ret;
}

CAResult_t CALEServerSendMulticastMessage(const uint8_t* data, uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendMulticastMessage(%p)", data);
    VERIFY_NON_NULL(data, TAG, "data is null");

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAResult_t ret = CALEServerSendMulticastMessageImpl(env, data, dataLen);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerSendMulticastMessageImpl has failed");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return ret;
}

CAResult_t CALEServerStartMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStartMulticastServer");

    if (!g_isInitializedServer)
    {
        OIC_LOG(INFO, TAG, "server is not initialized");
        return CA_STATUS_FAILED;
    }

    if (g_isStartServer)
    {
        OIC_LOG(INFO, TAG, "server is already started..it will be skipped");
        return CA_STATUS_FAILED;
    }

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    g_isStartServer = true;

    // start gatt server
    CAResult_t ret = CALEServerStartGattServer(env, g_bluetoothGattServerCallback);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "Fail to start gatt server");
        return ret;
    }

    // start advertise
    ret = CALEServerStartAdvertise();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStartAdvertise has failed");
    }

    // get Constants Value from Android Platform
    g_state_connected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE, "STATE_CONNECTED");
    g_state_disconnected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE, "STATE_DISCONNECTED");

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerStartMulticastServer");
    return ret;
}

CAResult_t CALEServerStopMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStopMulticastServer");

    if (false == g_isStartServer)
    {
        OIC_LOG(INFO, TAG, "server is already stopped..it will be skipped");
        return CA_STATUS_FAILED;
    }

    CAResult_t ret = CALEServerStopAdvertise();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStopAdvertise has failed");
    }

    g_isStartServer = false;

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerStopMulticastServer");
    return ret;
}

void CALEServerSetCallback(CAPacketReceiveCallback callback)
{
    OIC_LOG(DEBUG, TAG, "CALEServerSetCallback");
    g_packetReceiveCallback = callback;
}

CAResult_t CALEServerSendUnicastMessageImpl(JNIEnv *env, const char* address, const uint8_t* data,
                                            uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendUnicastMessageImpl, address: %s, data: %p",
            address, data);
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceList is null");
        return CA_STATUS_FAILED;
    }

    oc_mutex_lock(g_threadSendMutex);

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        OIC_LOG(DEBUG, TAG, "check device address");
        jobject jarrayObj = (jobject) u_arraylist_get(g_connectedDeviceList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            goto error_exit;
        }

        jstring jni_setAddress = CALEGetAddressFromBTDevice(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            goto error_exit;
        }
        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            goto error_exit;
        }

        OIC_LOG_V(DEBUG, TAG, "setAddress : %s", setAddress);
        OIC_LOG_V(DEBUG, TAG, "address : %s", address);

        if (!strcasecmp(setAddress, address))
        {
            OIC_LOG(DEBUG, TAG, "found the device");

            if (g_obj_bluetoothDevice)
            {
                (*env)->DeleteGlobalRef(env, g_obj_bluetoothDevice);
                g_obj_bluetoothDevice = NULL;
            }

            if (jarrayObj)
            {
                g_obj_bluetoothDevice = (*env)->NewGlobalRef(env, jarrayObj);
                CACheckJNIException(env);
            }
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            break;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
    }

    if (g_obj_bluetoothDevice)
    {
        jbyteArray jni_arr = (*env)->NewByteArray(env, dataLen);
        CACheckJNIException(env);
        (*env)->SetByteArrayRegion(env, jni_arr, 0, dataLen, (jbyte*) data);
        CACheckJNIException(env);
        g_sendBuffer = (jbyteArray)(*env)->NewGlobalRef(env, jni_arr);
        CACheckJNIException(env);

        CAResult_t res = CALEServerSend(env, g_obj_bluetoothDevice, g_sendBuffer);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "send has failed");
            goto error_exit;
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "There are no device to send in the list");
        goto error_exit;
    }

    if (g_sendBuffer)
    {
        (*env)->DeleteGlobalRef(env, g_sendBuffer);
        g_sendBuffer = NULL;
    }

    oc_mutex_unlock(g_threadSendMutex);
    OIC_LOG(INFO, TAG, "unicast - send request is successful");
    return CA_STATUS_OK;

error_exit:
    if (g_sendBuffer)
    {
        (*env)->DeleteGlobalRef(env, g_sendBuffer);
        g_sendBuffer = NULL;
    }

    if (g_obj_bluetoothDevice)
    {
        (*env)->DeleteGlobalRef(env, g_obj_bluetoothDevice);
        g_obj_bluetoothDevice = NULL;
    }

    oc_mutex_unlock(g_threadSendMutex);
    return CA_SEND_FAILED;
}

CAResult_t CALEServerSendMulticastMessageImpl(JNIEnv *env, const uint8_t *data, uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendMulticastMessageImpl, send to, data: %s", data);
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceList is null");
        return CA_STATUS_FAILED;
    }

    oc_mutex_lock(g_threadSendMutex);

    OIC_LOG(DEBUG, TAG, "set data into g_sendBuffer for notify");
    if (g_sendBuffer)
    {
        (*env)->DeleteGlobalRef(env, g_sendBuffer);
        g_sendBuffer = NULL;
    }
    jbyteArray jni_arr = (*env)->NewByteArray(env, dataLen);
    CACheckJNIException(env);
    (*env)->SetByteArrayRegion(env, jni_arr, 0, dataLen, (jbyte*) data);
    CACheckJNIException(env);
    g_sendBuffer = (jbyteArray)(*env)->NewGlobalRef(env, jni_arr);
    CACheckJNIException(env);

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_connectedDeviceList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }

        // send data for all device
        jbyteArray jni_bytearr_data = (*env)->NewByteArray(env, dataLen);
        CACheckJNIException(env);
        (*env)->SetByteArrayRegion(env, jni_bytearr_data, 0, dataLen, (jbyte*) data);
        CACheckJNIException(env);

        jstring jni_address = CALEGetAddressFromBTDevice(env, jarrayObj);
        if (!jni_address)
        {
            OIC_LOG(ERROR, TAG, "CALEGetAddressFromBTDevice has failed");
            continue;
        }

        const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is not available");
            continue;
        }

        if (g_obj_bluetoothDevice)
        {
            (*env)->DeleteGlobalRef(env, g_obj_bluetoothDevice);
            g_obj_bluetoothDevice = NULL;
        }

        if (jarrayObj)
        {
            g_obj_bluetoothDevice = (*env)->NewGlobalRef(env, jarrayObj);
            CACheckJNIException(env);
        }

        CAResult_t res = CALEServerSend(env, g_obj_bluetoothDevice, jni_bytearr_data);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "send has failed for the device[%s]", address);
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
            if (g_obj_bluetoothDevice)
            {
                (*env)->DeleteGlobalRef(env, g_obj_bluetoothDevice);
                g_obj_bluetoothDevice = NULL;
            }
            continue;
        }

        OIC_LOG_V(INFO, TAG, "unicast - send request is successful for a device[%s]", address);
        (*env)->ReleaseStringUTFChars(env, jni_address, address);
    }

    if (g_sendBuffer)
    {
        (*env)->DeleteGlobalRef(env, g_sendBuffer);
        g_sendBuffer = NULL;
    }

    oc_mutex_unlock(g_threadSendMutex);
    return CA_STATUS_OK;
}

void CALEServerCreateCachedDeviceList()
{
    oc_mutex_lock(g_connectedDeviceListMutex);
    // create new object array
    if (!g_connectedDeviceList)
    {
        OIC_LOG(DEBUG, TAG, "Create device list");
        g_connectedDeviceList = u_arraylist_create();
    }
    oc_mutex_unlock(g_connectedDeviceListMutex);

    oc_mutex_lock(g_deviceStateListMutex);
    // create new object array
    if (!g_deviceStateList)
    {
        OIC_LOG(DEBUG, TAG, "Create device list");
        g_deviceStateList = u_arraylist_create();
    }
    oc_mutex_unlock(g_deviceStateListMutex);
}

bool CALEServerIsDeviceInList(JNIEnv *env, const char* remoteAddress)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", false);
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", false);

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "list is null");
        return false;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_connectedDeviceList, index);

        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            return false;
        }

        jstring jni_setAddress = CALEGetAddressFromBTDevice(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            return false;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            return false;
        }

        if (!strcasecmp(remoteAddress, setAddress))
        {
            OIC_LOG(ERROR, TAG, "the device is already set");
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            return true;
        }
        else
        {
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            continue;
        }
    }

    OIC_LOG(DEBUG, TAG, "there are no device in the list");
    return false;
}

CAResult_t CALEServerAddDeviceToList(JNIEnv *env, jobject device)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerAddDeviceToList");
    VERIFY_NON_NULL(device, TAG, "device is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_connectedDeviceListMutex);

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "list is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    jstring jni_remoteAddress = CALEGetAddressFromBTDevice(env, device);
    if (!jni_remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_remoteAddress is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    const char* remoteAddress = (*env)->GetStringUTFChars(env, jni_remoteAddress, NULL);
    if (!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "remoteAddress is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    if (false == CALEServerIsDeviceInList(env, remoteAddress))
    {
        jobject jni_obj_device = (*env)->NewGlobalRef(env, device);
        u_arraylist_add(g_connectedDeviceList, jni_obj_device);
        OIC_LOG_V(DEBUG, TAG, "Set the object to ArrayList as Element : %s", remoteAddress);
    }

    (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, remoteAddress);
    oc_mutex_unlock(g_connectedDeviceListMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALEServerAddDeviceToList");
    return CA_STATUS_OK;
}

CAResult_t CALEServerRemoveAllDevices(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerRemoveAllDevices");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_connectedDeviceListMutex);
    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceList is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_connectedDeviceList, index);
        if (jarrayObj)
        {
            (*env)->DeleteGlobalRef(env, jarrayObj);
        }
    }

    OICFree(g_connectedDeviceList);
    g_connectedDeviceList = NULL;
    oc_mutex_unlock(g_connectedDeviceListMutex);

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerRemoveAllDevices");
    return CA_STATUS_OK;
}

CAResult_t CALEServerRemoveDevice(JNIEnv *env, jstring address)
{
    OIC_LOG(DEBUG, TAG, "IN CALEServerRemoveDevice");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(address, TAG, "address is null");

    oc_mutex_lock(g_connectedDeviceListMutex);
    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "no deviceList");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_connectedDeviceList, index);

        if (jarrayObj)
        {
            jstring jni_setAddress = CALEGetAddressFromBTDevice(env, jarrayObj);
            if (!jni_setAddress)
            {
                OIC_LOG(ERROR, TAG, "wrong device address");
                continue;
            }
            const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
            if (!setAddress)
            {
                OIC_LOG(ERROR, TAG, "setAddress is null");
                continue;
            }

            const char* remoteAddress = (*env)->GetStringUTFChars(env, address, NULL);
            if (!remoteAddress)
            {
                OIC_LOG(ERROR, TAG, "remoteAddress is null");
                (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
                continue;
            }

            if (!strcasecmp(setAddress, remoteAddress))
            {
                OIC_LOG_V(DEBUG, TAG, "device address : %s", remoteAddress);

                (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
                (*env)->ReleaseStringUTFChars(env, address, remoteAddress);
                (*env)->DeleteGlobalRef(env, jarrayObj);
                jarrayObj = NULL;

                if (NULL == u_arraylist_remove(g_connectedDeviceList, index))
                {
                    OIC_LOG(ERROR, TAG, "List removal failed.");
                    oc_mutex_unlock(g_connectedDeviceListMutex);
                    return CA_STATUS_FAILED;
                }
                oc_mutex_unlock(g_connectedDeviceListMutex);
                return CA_STATUS_OK;
            }
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->ReleaseStringUTFChars(env, address, remoteAddress);
        }
    }

    oc_mutex_unlock(g_connectedDeviceListMutex);

    OIC_LOG(DEBUG, TAG, "there are no device in the device list");

    OIC_LOG(DEBUG, TAG, "IN CALEServerRemoveDevice");
    return CA_STATUS_FAILED;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeRegisterGattServerCallback(JNIEnv *env, jobject obj,
                                                                        jobject callback)
{
    OIC_LOG(DEBUG, TAG, "Register Le Gatt Server Callback");
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(callback, TAG, "callback");

    g_bluetoothGattServerCallback = (*env)->NewGlobalRef(env, callback);
    CACheckJNIException(env);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeRegisterBluetoothLeAdvertiseCallback(JNIEnv *env,
                                                                                  jobject obj,
                                                                                  jobject callback)
{
    OIC_LOG(DEBUG, TAG, "Register Le Advertise Callback");
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(callback, TAG, "callback");

    g_leAdvertiseCallback = (*env)->NewGlobalRef(env, callback);
    CACheckJNIException(env);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeGattServerConnectionStateChangeCallback(
        JNIEnv *env, jobject obj, jobject device, jint status, jint newState)
{
    OIC_LOG(DEBUG, TAG, " Gatt Server ConnectionStateChange Callback");
    OIC_LOG_V(DEBUG, TAG, "New connection State: %d", newState);

    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(device, TAG, "device");

    jstring jni_remoteAddress = CALEGetAddressFromBTDevice(env, device);
    if (!jni_remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_remoteAddress is null");
        return;
    }

    const char* remoteAddress = (*env)->GetStringUTFChars(env, jni_remoteAddress, NULL);
    if (!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "remoteAddress is null");
        CACheckJNIException(env);
        return;
    }

    if (newState == g_state_connected)
    {
        OIC_LOG(DEBUG, TAG, "LE CONNECTED");

        if (false == CALEServerIsDeviceInList(env, remoteAddress))
        {
            OIC_LOG(DEBUG, TAG, "add connected device to cache");
            CALEServerAddDeviceToList(env, device);
        }

        CAResult_t res = CALEUpdateDeviceState(remoteAddress,
                                               CA_LE_CONNECTION_STATE,
                                               STATE_CONNECTED,
                                               g_deviceStateList,
                                               g_deviceStateListMutex);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
        }

        res = CALEServerStopAdvertise();
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(DEBUG, TAG, "CALEServerStopAdvertise has failed");
        }
    }
    else if (newState == g_state_disconnected)
    {
        OIC_LOG(DEBUG, TAG, "LE DISCONNECTED");

        jstring jni_remoteAddress = CALEGetAddressFromBTDevice(env, device);
        CAResult_t res = CALEServerRemoveDevice(env, jni_remoteAddress);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEServerRemoveDevice has failed");
        }

        res = CALEUpdateDeviceState(remoteAddress,
                                    CA_LE_CONNECTION_STATE,
                                    STATE_DISCONNECTED,
                                    g_deviceStateList,
                                    g_deviceStateListMutex);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
        }

        // start advertise
        res = CALEServerStartAdvertise();
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEServerStartAdvertise has failed");
        }
    }
    else
    {
#ifndef TB_LOG
        (void)status;
#endif
        OIC_LOG_V(DEBUG, TAG, "LE Connection state is [newState : %d, status %d]", newState,
                status);
    }
    (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, remoteAddress);
    (*env)->DeleteLocalRef(env, jni_remoteAddress);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeGattServerServiceAddedCallback(JNIEnv *env,
                                                                            jobject obj,
                                                                            jint status,
                                                                            jobject gattService)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(gattService, TAG, "gattService");
#ifndef TB_LOG
    (void)status;
#endif
    OIC_LOG_V(DEBUG, TAG, "Gatt Service Added Callback(%d)", status);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeGattServerCharacteristicReadRequestCallback(
        JNIEnv *env, jobject obj, jobject device, jbyteArray data)
{
    OIC_LOG(DEBUG, TAG, " Gatt Server Characteristic Read Request Callback");
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(device, TAG, "device");
    VERIFY_NON_NULL_VOID(data, TAG, "data");
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeGattServerCharacteristicWriteRequestCallback(
        JNIEnv *env, jobject obj, jobject device, jbyteArray data,
        jint requestId, jint offset, jbyteArray value)
{
    OIC_LOG_V(DEBUG, TAG, "Gatt Server Characteristic Write Request Callback");
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(device, TAG, "device");
    VERIFY_NON_NULL_VOID(data, TAG, "data");

    if (g_setHighQoS)
    {
        CALEServerSendResponse(env, device, requestId, 0, offset, value);
    }
    else
    {
        (void)requestId;
        (void)offset;
        (void)value;
    }

    // get Byte Array and covert to uint8_t*
    jint length = (*env)->GetArrayLength(env, data);
    CACheckJNIException(env);

    jboolean isCopy;
    jbyte *jni_byte_requestData = (jbyte *) (*env)->GetByteArrayElements(env, data, &isCopy);
    CACheckJNIException(env);

    uint8_t* requestData = NULL;
    requestData = OICMalloc(length);
    if (!requestData)
    {
        OIC_LOG(ERROR, TAG, "requestData is null");
        return;
    }

    memcpy(requestData, jni_byte_requestData, length);
    (*env)->ReleaseByteArrayElements(env, data, jni_byte_requestData, JNI_ABORT);

    jstring jni_address = CALEGetAddressFromBTDevice(env, device);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is null");
        OICFree(requestData);
        return;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        CACheckJNIException(env);
        OICFree(requestData);
        return;
    }

    OIC_LOG_V(DEBUG, TAG, "remote device address : %s, %p, %d", address, requestData, length);

    oc_mutex_lock(g_bleClientBDAddressMutex);
    uint32_t sentLength = 0;
    g_CABLEServerDataReceivedCallback(address, requestData, length,
                                      &sentLength);
    oc_mutex_unlock(g_bleClientBDAddressMutex);

    (*env)->ReleaseStringUTFChars(env, jni_address, address);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeGattServerNotificationSentCallback(JNIEnv *env,
                                                                                jobject obj,
                                                                                jobject device,
                                                                                jint status)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(device, TAG, "device");

    OIC_LOG_V(DEBUG, TAG, "Gatt Server Notification Sent Callback (status : %d)",
              status);

    jstring jni_address = CALEGetAddressFromBTDevice(env, device);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is null");
        return;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        (*env)->DeleteLocalRef(env, jni_address);
        return;
    }

    jint gatt_success = CALEGetConstantsValue(env, CLASSPATH_BT_GATT, "GATT_SUCCESS");
    if (gatt_success != status) // error case
    {
        OIC_LOG(ERROR, TAG, "it will be sent again.");

        CAResult_t res = CALEServerSend(env, device, g_sendBuffer);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "send has failed");

            if (g_obj_bluetoothDevice)
            {
                (*env)->DeleteGlobalRef(env, g_obj_bluetoothDevice);
                g_obj_bluetoothDevice = NULL;
            }

            oc_mutex_lock(g_threadSendNotifyMutex);
            g_isSignalSetFlag = true;
            oc_cond_signal(g_threadSendNotifyCond);
            oc_mutex_unlock(g_threadSendNotifyMutex);

            (*env)->ReleaseStringUTFChars(env, jni_address, address);
            (*env)->DeleteLocalRef(env, jni_address);
            return;
        }

        CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, address, 0, -1,
                           false, "notifyChar failure");
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "notify success");

        if (g_obj_bluetoothDevice)
        {
            (*env)->DeleteGlobalRef(env, g_obj_bluetoothDevice);
            g_obj_bluetoothDevice = NULL;
        }

        // next data can be sent
        oc_mutex_lock(g_threadSendNotifyMutex);
        OIC_LOG(DEBUG, TAG, "g_isSignalSetFlag is set true and signal");
        g_isSignalSetFlag = true;
        oc_cond_signal(g_threadSendNotifyCond);
        oc_mutex_unlock(g_threadSendNotifyMutex);

        CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, address, 0, -1,
                           true, "notifyChar success");
    }
    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    (*env)->DeleteLocalRef(env, jni_address);

}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeAdvertiseStartSuccessCallback(JNIEnv *env,
                                                                           jobject obj,
                                                                           jobject settingsInEffect)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(settingsInEffect, TAG, "settingsInEffect");

    OIC_LOG(DEBUG, TAG, "LE Advertise Start Success Callback");
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeAdvertiseStartFailureCallback(JNIEnv *env,
                                                                           jobject obj,
                                                                           jint errorCode)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");

    OIC_LOG_V(INFO, TAG, "LE Advertise Start Failure Callback(%d)", errorCode);

    jint data_too_large = CALEGetConstantsValue(env, CLASSPATH_BT_ADVERTISE_CB,
                                                "ADVERTISE_FAILED_DATA_TOO_LARGE");
    jint already_started = CALEGetConstantsValue(env, CLASSPATH_BT_ADVERTISE_CB,
                                                 "ADVERTISE_FAILED_ALREADY_STARTED");

    if (data_too_large == errorCode)
    {
        OIC_LOG_V(ERROR, TAG, "advertise data too large. please check length of device name");
    }
    else if (already_started == errorCode)
    {
        OIC_LOG_V(INFO, TAG, "advertising is already started");
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeGattServerMtuChangedCallback(JNIEnv * env,
                                                                          jobject obj,
                                                                          jobject device,
                                                                          jint mtu)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(device, TAG, "device");

    OIC_LOG_V(INFO, TAG, "gatt MTU size is changed (%d byte)", mtu);

    jstring jni_address = CALEGetAddressFromBTDevice(env, device);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is null");
        return;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        (*env)->DeleteLocalRef(env, jni_address);
        return;
    }

    // update mtu size
    CAResult_t res = CALESetMtuSize(address, mtu - CA_BLE_MTU_HEADER_SIZE,
                                    g_deviceStateList, g_deviceStateListMutex);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALESetMtuSize has failed");
    }

    res = CALEUpdateDeviceState(address, CA_LE_SEND_STATE,
                                STATE_SEND_MTU_NEGO_SUCCESS,
                                g_deviceStateList,
                                g_deviceStateListMutex);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
    }

    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    (*env)->DeleteLocalRef(env, jni_address);
}

/**
 * adapter common
 */

CAResult_t CAStartLEGattServer()
{
    // start gatt service
    CALEServerStartMulticastServer();

    return CA_STATUS_OK;
}

CAResult_t CAStopLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "CAStopLEGattServer");

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return CA_STATUS_FAILED;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(INFO, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAResult_t ret = CALEServerGattClose(env, g_bluetoothGattServer);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerGattClose has failed");
        return CA_STATUS_FAILED;
    }

    ret = CALEServerStopMulticastServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStopMulticastServer has failed");
        return CA_STATUS_FAILED;
    }

    ret = CALEServerDisconnectAllDevices(env);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerDisconnectAllDevices has failed");
        return CA_STATUS_FAILED;
    }

    ret = CALEServerRemoveAllDevices(env);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerRemoveAllDevices has failed");
        return CA_STATUS_FAILED;
    }

    if (g_leAdvertiseCallback)
    {
        (*env)->DeleteGlobalRef(env, g_leAdvertiseCallback);
    }

    if (g_bluetoothGattServer)
    {
        (*env)->DeleteGlobalRef(env, g_bluetoothGattServer);
    }

    if (g_bluetoothGattServerCallback)
    {
        (*env)->DeleteGlobalRef(env, g_bluetoothGattServerCallback);
    }

    if (g_obj_bluetoothDevice)
    {
        (*env)->DeleteGlobalRef(env, g_obj_bluetoothDevice);
        g_obj_bluetoothDevice = NULL;
    }

    oc_mutex_lock(g_threadSendNotifyMutex);
    oc_cond_signal(g_threadSendNotifyCond);
    oc_mutex_unlock(g_threadSendNotifyMutex);

    g_isStartServer = false;

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;
}

CAResult_t CAInitializeLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "Initialize Gatt Server");
    return CALEServerInitialize();
}

void CATerminateLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "Terminate Gatt Server");
    CALEServerTerminate();
}

void CASetLEReqRespServerCallback(CABLEDataReceivedCallback callback)
{
    oc_mutex_lock(g_bleReqRespCbMutex);
    g_CABLEServerDataReceivedCallback = callback;
    oc_mutex_unlock(g_bleReqRespCbMutex);
}

void CASetBLEServerErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    g_serverErrorCallback = callback;
}

CAResult_t CAUpdateCharacteristicsToGattClient(const char *address,
                                               const uint8_t *charValue,
                                               uint32_t charValueLen)
{
    CAResult_t result = CA_SEND_FAILED;
    VERIFY_NON_NULL(address, TAG, "env is null");
    VERIFY_NON_NULL(charValue, TAG, "device is null");

    if (address)
    {
        result = CALEServerSendUnicastMessage(address, charValue, charValueLen);
    }

    return result;
}

CAResult_t CAUpdateCharacteristicsToAllGattClients(const uint8_t *charValue,
                                                   uint32_t charValueLen)
{
    VERIFY_NON_NULL(charValue, TAG, "device is null");

    CAResult_t result = CALEServerSendMulticastMessage(charValue, charValueLen);

    return result;
}

void CASetLEServerThreadPoolHandle(ca_thread_pool_t handle)
{
    OIC_LOG(INFO, TAG, "CASetLEServerThreadPoolHandle is not support");
    (void)handle;
}

CAResult_t CALEServerInitMutexVaraibles()
{
    if (NULL == g_bleReqRespCbMutex)
    {
        g_bleReqRespCbMutex = oc_mutex_new();
        if (NULL == g_bleReqRespCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_bleClientBDAddressMutex)
    {
        g_bleClientBDAddressMutex = oc_mutex_new();
        if (NULL == g_bleClientBDAddressMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_connectedDeviceListMutex)
    {
        g_connectedDeviceListMutex = oc_mutex_new();
        if (NULL == g_connectedDeviceListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadSendMutex)
    {
        g_threadSendMutex = oc_mutex_new();
        if (NULL == g_threadSendMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadSendNotifyMutex)
    {
        g_threadSendNotifyMutex = oc_mutex_new();
        if (NULL == g_threadSendNotifyMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_deviceStateListMutex)
    {
        g_deviceStateListMutex = oc_mutex_new();
        if (NULL == g_deviceStateListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CALEServerTerminateMutexVaraibles()
{
    oc_mutex_free(g_bleReqRespCbMutex);
    g_bleReqRespCbMutex = NULL;

    oc_mutex_free(g_bleClientBDAddressMutex);
    g_bleClientBDAddressMutex = NULL;

    oc_mutex_free(g_connectedDeviceListMutex);
    g_connectedDeviceListMutex = NULL;

    oc_mutex_free(g_threadSendMutex);
    g_threadSendMutex = NULL;

    oc_mutex_free(g_threadSendNotifyMutex);
    g_threadSendNotifyMutex = NULL;

    oc_mutex_free(g_deviceStateListMutex);
    g_deviceStateListMutex = NULL;
}

void CALEServerTerminateConditionVaraibles()
{
    OIC_LOG(DEBUG, TAG, "this method is not supported");
}

bool CALEServerIsConnected(const char* address)
{
    if (CALEIsValidState(address, CA_LE_CONNECTION_STATE,
                         STATE_SERVICE_CONNECTED,
                         g_deviceStateList,
                         g_deviceStateListMutex))
    {
        OIC_LOG(DEBUG, TAG, "current state is connected");
        return true;
    }
    OIC_LOG(DEBUG, TAG, "current state is not connected");
    return false;
}

uint16_t CALEServerGetMtuSize(const char* address)
{
    return CALEGetMtuSize(address, g_deviceStateList, g_deviceStateListMutex);
}

