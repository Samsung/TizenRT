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

#include <jni.h>
#include "logger.h"
#include "cautilinterface.h"
#include "camanagerleinterface.h"
#include "camanagerleutil.h"
#include "caleautoconnector.h"
#include "cacommon.h"
#include "cacommonutil.h"
#include "camanagerdevice.h"
#include "caleclient.h"
#include "caleutils.h"

#define TAG "OIC_CA_MANAGER_LE"

static const jint SUPPORT_ADNROID_API_LEVEL = 18;
static const jint AUTH_FAIL = 5;
static const jint LINK_LOSS = 8;
static const jint ACCEPT_TIMEOUT_EXCEPTION = 16;
static const jint REMOTE_DISCONNECT = 19;
static const jint LOCAL_DISCONNECT = 22;
static const jint USER_REMOVED_BOND = 68;
static JavaVM *g_jvm = NULL;
static jobject g_context = NULL;
static jobject g_connectedDeviceSet = NULL;


CAResult_t CASetLEClientAutoConnectionDeviceInfo(const char* address)
{
    OIC_LOG(DEBUG, TAG, "CASetClientAutoConnectionDeviceInfo");
    VERIFY_NON_NULL(address, TAG, "address");

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(DEBUG, TAG, "AttachCurrentThread will be called for JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }
    OIC_LOG_V(DEBUG, TAG, "set [%s] for Auto Connection", address);

    jstring jni_leAddress = (*env)->NewStringUTF(env, address);
    if (!jni_leAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_leAddress is null");
        goto error_exit;
    }

    if (!CAManagerCheckBTAddress(env, jni_leAddress))
    {
        OIC_LOG(ERROR, TAG, "this address is not BT address string format");
        goto error_exit;
    }

    // if there is target address in SharedPreference, it will be reset.
    if (CAManagerIsConnectedDeviceAddress(env, g_context, jni_leAddress, g_connectedDeviceSet))
    {
        if (!CAManagerRemoveConnectedDeviceAddress(env, g_context, jni_leAddress,
                                                   g_connectedDeviceSet))
        {
            OIC_LOG(ERROR, TAG, "Preference - remove has failed");
        }
        else
        {
            OIC_LOG(INFO, TAG, "Preference - remove success");
        }
    }

    // it will be added new target address.
    if (!CAManagerAddConnectedDeviceAddress(env, g_context, jni_leAddress, g_connectedDeviceSet))
    {
        OIC_LOG(ERROR, TAG, "Preference - putting has failed");
    }
    else
    {
        OIC_LOG(INFO, TAG, "Preference - putting success");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;

error_exit:

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_FAILED;
}

CAResult_t CAUnsetLEClientAutoConnectionDeviceInfo(const char* address)
{
    OIC_LOG(DEBUG, TAG, "CAUnsetClientAutoConnectionDeviceInfo");
    VERIFY_NON_NULL(address, TAG, "address");

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(DEBUG, TAG, "AttachCurrentThread will be called for JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }
    OIC_LOG_V(DEBUG, TAG, "unset [%s] for Auto Connection", address);

    jstring jni_leAddress = (*env)->NewStringUTF(env, address);
    if (!jni_leAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_leAddress is null");
        goto error_exit;
    }

    if (!CAManagerCheckBTAddress(env, jni_leAddress))
    {
        OIC_LOG(ERROR, TAG, "this address is not BT address string format");
        goto error_exit;
    }

    // if there is target address in SharedPreference, it will be removed
    if (CAManagerIsConnectedDeviceAddress(env, g_context, jni_leAddress, g_connectedDeviceSet))
    {
        if (!CAManagerRemoveConnectedDeviceAddress(env, g_context, jni_leAddress,
                                                   g_connectedDeviceSet))
        {
            OIC_LOG(ERROR, TAG, "Preference - remove has failed");
        }
        else
        {
            OIC_LOG(INFO, TAG, "Preference - remove success");
        }
    }

    // remove target device for auto connection
    CAResult_t ret = CAManagerRemoveACData(env, jni_leAddress);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAManagerRemoveACData has failed");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return ret;

error_exit:

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_FAILED;
}

CAResult_t CAManagerLEClientInitialize(JNIEnv *env, JavaVM *jvm, jobject context)
{
    OIC_LOG(DEBUG, TAG, "CAManagerClientInitialize");
    VERIFY_NON_NULL(env, TAG, "env");
    VERIFY_NON_NULL(jvm, TAG, "jvm");
    VERIFY_NON_NULL(context, TAG, "context");

    jint jni_int_sdk = CALEGetBuildVersion(env);
    if (jni_int_sdk < SUPPORT_ADNROID_API_LEVEL)
    {
        OIC_LOG_V(ERROR, TAG, "it is not supported (%d)", jni_int_sdk);
        return CA_STATUS_FAILED;
    }

    g_jvm = jvm;
    g_context = (*env)->NewGlobalRef(env, context);;

    CAManagerInitMutexVaraibles();
    CAManagerInitLEAutoConnection();
    CAManagerCreateACDataList();

    // get last connected device list
    jobject set = NULL;
    set = CAManagerGetConnectedDeviceAddress(env, g_context);
    if (!set)
    {
        // create new set<String> object
        set = CAManagerCreateSetString(env);
        if (!set)
        {
            OIC_LOG(ERROR, TAG, "CAManagerCreateSetString has failed");
            return CA_STATUS_FAILED;
        }
        OIC_LOG(DEBUG, TAG, "created new SetString");
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "get previous Set<String> object");
    }

    g_connectedDeviceSet = (jobject)(*env)->NewGlobalRef(env, set);
    if (!g_connectedDeviceSet)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceSet is null");
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CAManagerLEClientTerminate(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CAManagerClientTerminate");
    VERIFY_NON_NULL(env, TAG, "env");

    // stop gatt connection
    CAResult_t res = CALEClientDisconnectAll(env);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientDisconnectAll has failed");
    }

    res = CAManagerRemoveAllACData(env);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CAManagerRemoveAllACData has failed");
    }

    CAManagerDestroyACDataList();
    CAManagerTerminateLEAutoConnection();
    CAManagerTerminateMutexVariables();

    if (g_context)
    {
        (*env)->DeleteGlobalRef(env, g_context);
        g_context = NULL;
    }

    if (g_connectedDeviceSet)
    {
        (*env)->DeleteGlobalRef(env, g_connectedDeviceSet);
        g_connectedDeviceSet = NULL;
    }

    return res;
}

void CAManagerLESetScanInterval(jint interval, jint count)
{
    OIC_LOG(DEBUG, TAG, "CAManagerLESetScanInterval");
    CALERestartScanWithInterval(interval, count, BLE_SCAN_ENABLE);
}

void CAManagerLEStopScan()
{
    OIC_LOG(DEBUG, TAG, "CAManagerLEStopScan");
    CALERestartScanWithInterval(0, 0, BLE_SCAN_DISABLE);
}

void CAManagerSetConfigure(CAUtilConfig_t config)
{
    OIC_LOG_V(INFO, TAG, "set configure for bleFlags : %d", config.bleFlags);
    caglobals.bleFlags = config.bleFlags;
}

CAResult_t CAManagerLEStartAdvertising()
{
    CAResult_t ret = CALEServerStartAdvertise();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStartAdvertise has failed");
    }
    return ret;
}

CAResult_t CAManagerLEStopAdvertising()
{
    CAResult_t ret = CALEServerStopAdvertise();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAManagerLEStopAdvertising has failed");
    }
    return ret;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerAdapterStateChangedCallback(
        JNIEnv *env, jobject obj, jint state)
{
    OIC_LOG_V(INFO, TAG, "caManagerAdapterStateChangedCallback - state %d", state);
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");

    jint state_on = CALEGetConstantsValue(env, CLASSPATH_BT_ADAPTER, "STATE_ON");
    jint state_off = CALEGetConstantsValue(env, CLASSPATH_BT_ADAPTER, "STATE_OFF");
    jint state_turning_off = CALEGetConstantsValue(env, CLASSPATH_BT_ADAPTER, "STATE_TURNING_OFF");

    if (state_on == state)
    {
        OIC_LOG(DEBUG, TAG, "AdapterStateChangedCallback : state_on");

        // when BT state is on. recovery flag has to be reset.
        CAManagerSetBTRecovery(false);

        // find target device for autoconnect
        size_t length = CAManagerGetACDataLength();
        OIC_LOG_V(DEBUG, TAG, "length of ACDataList : %d", length);
        for (size_t idx = 0; idx < length; idx++)
        {
            jstring leAddress = CAManagerGetLEAddressFromACData(env, idx);
            if (leAddress)
            {
                CAResult_t res = CAManagerStartAutoConnection(env, leAddress);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "CAManagerStartAutoConnection has failed");
                    return;
                }
            }
        }
    }
    else if (state_off == state)
    {
        OIC_LOG(DEBUG, TAG, "AdapterStateChangedCallback : state_off");

        // reset isAutoConnecting flag for all target devices
        size_t length = CAManagerGetACDataLength();
        OIC_LOG_V(DEBUG, TAG, "length of ACDataList : %d", length);
        for (size_t idx = 0; idx < length; idx++)
        {
            jstring address = CAManagerGetLEAddressFromACData(env, idx);
            if (address)
            {
                CAManagerSetAutoConnectingFlag(env, address, false);
            }
        }

        // check whether BT recovery is needed or not
        if (CAManagerIsRecoveryFlagSet())
        {
            CAManagerProcessRecovery(env, STATE_OFF);
        }
    }
    else if (state_turning_off == state)
    {
        OIC_LOG(DEBUG, TAG, "AdapterStateChangedCallback : state_turning_off");
    }
    else
    {
        OIC_LOG(INFO, TAG, "AdapterStateChangedCallback state is not available");
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerBondStateChangedCallback(
        JNIEnv *env, jobject obj, jobject device)
{
    OIC_LOG(INFO, TAG, "caManagerBondStateChangedCallback");
    // this callback is called by CaLeClientInterface
    // only when bond state is changed from BOND_BONDED to BOND_NONE
    OIC_LOG(DEBUG, TAG, "bond state is changed from BOND_BONDED to BOND_NONE");
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(device, TAG, "device");

    // get ble address from Bluetooth Device object
    jstring jni_leAddress = CALEClientGetLEAddressFromBTDevice(env, device);
    if (!jni_leAddress)
    {
        OIC_LOG(INFO, TAG, "unbonded : it isn't same device type");
        return;
    }

    char* leAddress = (char*)(*env)->GetStringUTFChars(env, jni_leAddress, NULL);
    if (!leAddress)
    {
        OIC_LOG(ERROR, TAG, "leAddress is null");
        return;
    }

    // if there is no data, CAData will be created.
    OIC_LOG_V(DEBUG, TAG, "bond none device : %s", leAddress);

    CAResult_t res = CAManagerRemoveACData(env, jni_leAddress);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CAManagerRemoveACData has failed");
    }

    (*env)->ReleaseStringUTFChars(env, jni_leAddress, leAddress);

    if (!CAManagerRemoveConnectedDeviceAddress(env, g_context, jni_leAddress,
                                               g_connectedDeviceSet))
    {
        OIC_LOG(ERROR, TAG, "CAManagerRemoveConnectedDeviceAddress has failed");
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerLeGattConnectionStateChangeCB(
        JNIEnv *env, jobject obj, jobject gatt, jint status, jint newState)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt");

    jint state_connected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE, "STATE_CONNECTED");
    jint state_disconnected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE, "STATE_DISCONNECTED");

    jstring jni_address = CALEGetAddressFromGatt(env, gatt);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "CALEGetAddressFromGatt is null");
        return;
    }

    char* address = (char*)(*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        (*env)->DeleteLocalRef(env, jni_address);
        return;
    }

    if (GATT_SUCCESS == status && state_connected == newState) // le connected
    {
        CAResult_t res = CAManagerReadRemoteRssi(env, gatt);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CAManagerReadRemoteRssi has failed");
            goto exit;
        }
    }
    else if (state_disconnected == newState)// le disconnected
    {
        if (LINK_LOSS == status || REMOTE_DISCONNECT == status)
        {
            if (!CAManagerIsInACDataList(env, jni_address))
            {
                OIC_LOG_V(DEBUG, TAG, "this[%s] is not target address for Auto Connection",
                          address);
                goto exit;
            }

            CAManagerSetAutoConnectingFlag(env, jni_address, false);

            CAResult_t res = CAManagerStartAutoConnection(env, jni_address);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CAManagerStartAutoConnection has failed");
                goto exit;
            }
        }
        else if (ACCEPT_TIMEOUT_EXCEPTION == status)
        {
            CAManagerProcessRecovery(env, START_RECOVERY);
        }
    }

exit:
    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    (*env)->DeleteLocalRef(env, jni_address);
}

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    caManagerLeServicesDiscoveredCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerLeServicesDiscoveredCallback(JNIEnv *env,
                                                                               jobject obj,
                                                                               jobject gatt,
                                                                               jint status)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt");

    if (GATT_SUCCESS == status)
    {
        if (!g_connectedDeviceSet)
        {
            OIC_LOG(INFO, TAG, "g_connectedDeviceSet is needed for CM. "
                    "please call CAManagerLEClientInitialize");
            return;
        }

        jstring jni_address = CALEGetAddressFromGatt(env, gatt);
        if (!jni_address)
        {
            OIC_LOG(ERROR, TAG, "CALEGetAddressFromGatt is null");
            return;
        }

        char* address = (char*)(*env)->GetStringUTFChars(env, jni_address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            (*env)->DeleteLocalRef(env, jni_address);
            return;
        }

        if (CAManagerIsConnectedDeviceAddress(env, g_context, jni_address, g_connectedDeviceSet))
        {
            OIC_LOG(INFO, TAG, "AC list - the address will be added to ACData list");
            CAManagerAddACData(env, jni_address);
            CAManagerSetAutoConnectingFlag(env, jni_address, false);

            // next connection will be requested with JNI_TRUE on autoConnect flag
            // after first connection
            CALEClientSetFlagToState(env, jni_address, CA_LE_AUTO_CONNECT_FLAG, JNI_TRUE);
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "AC list - the address is not set to AutoConnect");
        }

        (*env)->ReleaseStringUTFChars(env, jni_address, address);
        (*env)->DeleteLocalRef(env, jni_address);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "ServicesDiscovery has failed");
    }
}

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    caManagerLeRemoteRssiCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caManagerLeRemoteRssiCallback(JNIEnv *env,
                                                                       jobject obj,
                                                                       jobject gatt,
                                                                       jint rssi,
                                                                       jint status)
{
#ifndef TB_LOG
    (void)rssi;
    (void)status;
#endif
    OIC_LOG_V(DEBUG, TAG, "caManagerLeRemoteRssiCallback - rssi : %d: ", rssi);
    OIC_LOG_V(DEBUG, TAG, "caManagerLeRemoteRssiCallback - status : %d: ", status);
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt");
}
