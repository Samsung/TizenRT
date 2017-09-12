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
#include "logger.h"
#include "calenwmonitor.h"
#include "caleclient.h"
#include "caleserver.h"
#include "caleutils.h"
#include "caleinterface.h"
#include "caadapterutils.h"

#include "octhread.h"

#include "org_iotivity_ca_CaLeClientInterface.h"
#include "org_iotivity_ca_CaLeServerInterface.h"

#define TAG PCF("OIC_CA_LE_MONITOR")

static const jint CONNECTION_FAILED_TO_BE_EASTABLISHED = 62;

static JavaVM *g_jvm;

/**
 * @var g_bleDeviceStateChangedCallback
 * @brief Maintains the callback to be notified on device state changed.
 */
static CALEDeviceStateChangedCallback g_bleDeviceStateChangedCallback = NULL;

/**
 * @var g_bleConnectionStateChangedCallback
 * @brief Maintains the callback to be notified on device state changed.
 */
static CALEConnectionStateChangedCallback g_bleConnectionStateChangedCallback = NULL;

/**
 * @var g_bleDeviceStateChangedCbMutex
 * @brief Mutex to synchronize access to the deviceStateChanged Callback when the state
 *           of the LE adapter gets change.
 */
static oc_mutex g_bleDeviceStateChangedCbMutex = NULL;

/**
 * @var g_bleConnectionStateChangedCbMutex
 * @brief Mutex to synchronize access to the LE ConnectionStateChanged Callback when the state
 *           of the LE adapter gets change.
 */
static oc_mutex g_bleConnectionStateChangedCbMutex = NULL;

//getting context
void CALENetworkMonitorJNISetContext()
{
    OIC_LOG(DEBUG, TAG, "CALENetworkMonitorJNISetContext - it is not supported");
}

//getting jvm
void CALENetworkMonitorJniInit()
{
    OIC_LOG(DEBUG, TAG, "CALENetworkMonitorJniInit");
    g_jvm = CANativeJNIGetJavaVM();
}

void CALESetAdapterStateCallback(CALEDeviceStateChangedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "CALESetAdapterStateCallback");
    g_bleDeviceStateChangedCallback = callback;
}

CAResult_t CAInitializeLEAdapter()
{
    // Nothing to do.

    return CA_STATUS_OK;
}

CAResult_t CAStartLEAdapter()
{
    // Nothing to do.

    return CA_STATUS_OK;
}

CAResult_t CAStopLEAdapter()
{
    // Nothing to do.

    return CA_STATUS_OK;
}

CAResult_t CAInitLENwkMonitorMutexVaraibles()
{
    if (NULL == g_bleDeviceStateChangedCbMutex)
    {
        g_bleDeviceStateChangedCbMutex = oc_mutex_new();
        if (NULL == g_bleDeviceStateChangedCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_bleConnectionStateChangedCbMutex)
    {
    	g_bleConnectionStateChangedCbMutex = oc_mutex_new();
        if (NULL == g_bleConnectionStateChangedCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            oc_mutex_free(g_bleDeviceStateChangedCbMutex);
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CATerminateLENwkMonitorMutexVaraibles()
{
    oc_mutex_free(g_bleDeviceStateChangedCbMutex);
    g_bleDeviceStateChangedCbMutex = NULL;

    oc_mutex_free(g_bleConnectionStateChangedCbMutex);
    g_bleConnectionStateChangedCbMutex = NULL;
}

CAResult_t CAGetLEAdapterState()
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
        OIC_LOG(DEBUG, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }
        return CA_ADAPTER_NOT_ENABLED;
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;
}

CAResult_t CAInitializeLENetworkMonitor()
{
    CAResult_t res = CAInitLENwkMonitorMutexVaraibles();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CAInitLENwkMonitorMutexVaraibles has failed");
        return CA_STATUS_FAILED;
    }

    CALENetworkMonitorJNISetContext();
    CALENetworkMonitorJniInit();

    return CA_STATUS_OK;
}

void CATerminateLENetworkMonitor()
{
    OIC_LOG(DEBUG, TAG, "CATerminateLENetworkMonitor");

    CATerminateLENwkMonitorMutexVaraibles();
}

CAResult_t CASetLEAdapterStateChangedCb(CALEDeviceStateChangedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "Setting CALEDeviceStateChangedCallback");

    oc_mutex_lock(g_bleDeviceStateChangedCbMutex);
    CALESetAdapterStateCallback(callback);
    oc_mutex_unlock(g_bleDeviceStateChangedCbMutex);

    return CA_STATUS_OK;
}

CAResult_t CAUnSetLEAdapterStateChangedCb()
{
    OIC_LOG(DEBUG, TAG, "it is not required in this platform");
    return CA_STATUS_OK;
}

CAResult_t CASetLENWConnectionStateChangedCb(CALEConnectionStateChangedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "CASetLENWConnectionStateChangedCb");
    oc_mutex_lock(g_bleConnectionStateChangedCbMutex);
    g_bleConnectionStateChangedCallback = callback;
    oc_mutex_unlock(g_bleConnectionStateChangedCbMutex);
    return CA_STATUS_OK;
}

CAResult_t CAUnsetLENWConnectionStateChangedCb()
{
    OIC_LOG(DEBUG, TAG, "CAUnsetLENWConnectionStateChangedCb");
    oc_mutex_lock(g_bleConnectionStateChangedCbMutex);
    g_bleConnectionStateChangedCallback = NULL;
    oc_mutex_unlock(g_bleConnectionStateChangedCbMutex);
    return CA_STATUS_OK;
}

static CAResult_t CALEStateConnectedCallback(JNIEnv *env, jstring jni_address,
                                             jboolean isDescriptorFound)
{
    VERIFY_NON_NULL(env, TAG, "env");
    VERIFY_NON_NULL(jni_address, TAG, "jni_address");

    if (CALEClientGetFlagFromState(env, jni_address, CA_LE_DESCRIPTOR_FOUND) == isDescriptorFound)
    {
        if (g_bleConnectionStateChangedCallback)
        {
            const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
            if (!address)
            {
                OIC_LOG(ERROR, TAG, "address is null");
                CACheckJNIException(env);
                return CA_STATUS_FAILED;
            }

            g_bleConnectionStateChangedCallback(CA_ADAPTER_GATT_BTLE, address, true);
            OIC_LOG(DEBUG, TAG, "BLE is connected");

            (*env)->ReleaseStringUTFChars(env, jni_address, address);
        }
    }

    return CA_STATUS_OK;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeStateChangedCallback(JNIEnv *env, jobject obj,
                                                                   jint status)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");

    OIC_LOG_V(INFO, TAG, "CaLeClientInterface - Network State Changed : status(%d)", status);

    if (!g_bleDeviceStateChangedCallback)
    {
        OIC_LOG(ERROR, TAG, "gNetworkChangeCb is null");
        return;
    }

    jint state_on = CALEGetConstantsValue(env, CLASSPATH_BT_ADAPTER, "STATE_ON");
    jint state_off = CALEGetConstantsValue(env, CLASSPATH_BT_ADAPTER, "STATE_OFF");
    jint state_turning_off = CALEGetConstantsValue(env, CLASSPATH_BT_ADAPTER, "STATE_TURNING_OFF");

    if (state_on == status) // STATE_ON:12
    {
        CANetworkStatus_t newStatus = CA_INTERFACE_UP;
        CALEClientCreateDeviceList();

        if (!(caglobals.bleFlags & CA_LE_SERVER_DISABLE))
        {
            CALEServerCreateCachedDeviceList();
        }

        g_bleDeviceStateChangedCallback(newStatus);
    }
    else if (state_turning_off == status) // BT_STATE_TURNING_OFF:13
    {
        // gatt Device list will be removed.
        // so it is need to create list again when adapter is enabled.
        CAStopLEGattClient();
    }
    else if (state_off == status) // STATE_OFF:10
    {
        // remove obj for client
        CAResult_t res = CALEClientRemoveAllGattObjs(env);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientRemoveAllGattObjs has failed");
        }

        res = CALEClientResetDeviceStateForAll();
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientResetDeviceStateForAll has failed");
        }

        // remove obj for server
        res = CALEServerRemoveAllDevices(env);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEServerRemoveAllDevices has failed");
        }

        CANetworkStatus_t newStatus = CA_INTERFACE_DOWN;
        g_bleDeviceStateChangedCallback(newStatus);
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeBondStateChangedCallback(JNIEnv *env, jobject obj,
                                                                      jstring jaddr)
{
    OIC_LOG(DEBUG, TAG, "CaLeClientInterface - Bond State Changed");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(jaddr, TAG, "jaddr is null");

    // geneally 'addr' parameter will be not ble address, if you didn't bond for BLE.
    // below logics will be needed when ble pairing is set.

    CAResult_t res = CALEClientDisconnectforAddress(env, jaddr);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientDisconnectforAddress has failed");
    }

    // remove obj for client
    res = CALEClientRemoveGattObjForAddr(env, jaddr);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CANativeRemoveGattObjForAddr has failed");
    }

    res = CALEClientRemoveDeviceInScanDeviceList(env, jaddr);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientRemoveDeviceInScanDeviceList has failed");
    }

    // remove obej for server
    res = CALEServerRemoveDevice(env, jaddr);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEServerRemoveDevice has failed");
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattNWConnectionStateChangeCallback(JNIEnv *env,
                                                                                 jobject obj,
                                                                                 jobject gatt,
                                                                                 jint status,
                                                                                 jint newState)
{
    OIC_LOG_V(INFO, TAG, "CALeGattNWConnectionStateChangeCallback - status %d, newstate %d",
              status, newState);
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt");

    jint state_disconnected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE,
                                                    "STATE_DISCONNECTED");
    if (state_disconnected == newState)
    {
        jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
        if (!jni_address)
        {
            OIC_LOG(ERROR, TAG, "jni_address is null");
            return;
        }

        const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            CACheckJNIException(env);
            return;
        }

        if (CONNECTION_FAILED_TO_BE_EASTABLISHED != status)
        {
            if (g_bleConnectionStateChangedCallback)
            {
                OIC_LOG_V(DEBUG, TAG, "LE Disconnected state is %s", address);
                g_bleConnectionStateChangedCallback(CA_ADAPTER_GATT_BTLE, address, false);
            }
        }

        (*env)->ReleaseStringUTFChars(env, jni_address, address);
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeServerInterface_caLeGattServerNWConnectionStateChangeCallback(
        JNIEnv *env, jobject obj, jobject device, jint status, jint newState)
{
    OIC_LOG_V(DEBUG, TAG, "caLeGattServerNWConnectionStateChangeCallback - status %d, newstate %d",
              status, newState);
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(device, TAG, "device");

    if (CONNECTION_FAILED_TO_BE_EASTABLISHED != status)
    {
        if (g_bleConnectionStateChangedCallback)
        {
            jstring jni_remoteAddress = CALEGetAddressFromBTDevice(env, device);
            if (!jni_remoteAddress)
            {
                OIC_LOG(ERROR, TAG, "jni_remoteAddress is null");
                return;
            }

            const char* address = (*env)->GetStringUTFChars(env, jni_remoteAddress, NULL);
            if (!address)
            {
                OIC_LOG(ERROR, TAG, "address is null");
                CACheckJNIException(env);
                return;
            }

            // STATE_DISCONNECTED
            jint state_disconnected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE,
                                                            "STATE_DISCONNECTED");

            // STATE_CONNECTED
            jint state_connected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE,
                                                         "STATE_CONNECTED");

            if (state_disconnected == newState)
            {
                OIC_LOG_V(DEBUG, TAG, "LE Disconnected state is %d, %s", newState, address);
                g_bleConnectionStateChangedCallback(CA_ADAPTER_GATT_BTLE, address, false);
            }
            else if (state_connected == newState)
            {
                OIC_LOG_V(DEBUG, TAG, "LE Connected state is %d, %s", newState, address);
                g_bleConnectionStateChangedCallback(CA_ADAPTER_GATT_BTLE, address, true);
            }
            else
            {
                OIC_LOG_V(DEBUG, TAG, "Unknown state : %d, %s", newState, address);
            }

            (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, address);
        }
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattNWServicesDiscoveredCallback(JNIEnv *env,
                                                                              jobject obj,
                                                                              jobject gatt,
                                                                              jint status)
{
    OIC_LOG_V(INFO, TAG, "caLeGattNWServicesDiscoveredCallback - status %d", status);
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt");

    if (GATT_SUCCESS == status)
    {
        jstring jni_address = CALEGetAddressFromGatt(env, gatt);
        if (!jni_address)
        {
            OIC_LOG(ERROR, TAG, "jni_address is null");
            return;
        }

        if (CA_STATUS_OK != CALEStateConnectedCallback(env, jni_address, JNI_FALSE))
        {
            OIC_LOG(ERROR, TAG, "CALEStateConnectedCallback has failed");
        }

        (*env)->DeleteLocalRef(env, jni_address);
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattNWDescriptorWriteCallback(JNIEnv *env,
                                                                           jobject obj,
                                                                           jobject gatt,
                                                                           jint status)
{
    OIC_LOG_V(INFO, TAG, "caLeGattNWDescriptorWriteCallback - status %d", status);
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt");

    if (GATT_SUCCESS == status)
    {
        jstring jni_address = CALEGetAddressFromGatt(env, gatt);
        if (!jni_address)
        {
            OIC_LOG(ERROR, TAG, "jni_address is null");
            return;
        }

        if (CA_STATUS_OK != CALEStateConnectedCallback(env, jni_address, JNI_TRUE))
        {
            OIC_LOG(ERROR, TAG, "CALEStateConnectedCallback has failed");
        }

        (*env)->DeleteLocalRef(env, jni_address);
    }
}
