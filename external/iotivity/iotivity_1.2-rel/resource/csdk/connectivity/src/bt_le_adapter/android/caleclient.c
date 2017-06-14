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

#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <unistd.h>

#include "calestate.h"
#include "caleclient.h"
#include "caleserver.h"
#include "caleutils.h"
#include "caleinterface.h"
#include "caadapterutils.h"

#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cathreadpool.h" /* for thread pool */
#include "octhread.h"
#include "uarraylist.h"
#include "org_iotivity_ca_CaLeClientInterface.h"

//#define TAG PCF("OIC_CA_LE_CLIENT")
#define TAG BLE_CLIENT_TAG

#define MICROSECS_PER_SEC 1000000
#define WAIT_TIME_WRITE_CHARACTERISTIC 10 * MICROSECS_PER_SEC
#define WAIT_TIME_SCAN_INTERVAL_DEFAULT 10
#define WAIT_TIME_SCANNED_CHECKING 30

#define GATT_CONNECTION_PRIORITY_BALANCED   0
#define GATT_FAILURE                        257
#define GATT_INSUFFICIENT_AUTHENTICATION    5
#define GATT_INSUFFICIENT_ENCRYPTION        15
#define GATT_INVALID_ATTRIBUTE_LENGTH       13
#define GATT_INVALID_OFFSET                 7
#define GATT_READ_NOT_PERMITTED             2
#define GATT_REQUEST_NOT_SUPPORTED          6
#define GATT_WRITE_NOT_PERMITTED            3

// samsung
#define BLE_SCAN_API_LEVEL (100) //(21)
#define BLE_MIN_API_LEVEL  (18)
#define HIDDEN_API

static ca_thread_pool_t g_threadPoolHandle = NULL;

JavaVM *g_jvm;
static u_arraylist_t *g_deviceList = NULL; // device list to have same UUID
static u_arraylist_t *g_gattObjectList = NULL;
static u_arraylist_t *g_deviceStateList = NULL;

static CAPacketReceiveCallback g_packetReceiveCallback = NULL;
static CABLEErrorHandleCallback g_clientErrorCallback;
static jobject g_leScanCallback = NULL;
static jobject g_leGattCallback = NULL;
static jobject g_context = NULL;
static jobjectArray g_uuidList = NULL;

// it will be prevent to start send logic when adapter has stopped.
static bool g_isStartedLEClient = false;

static jbyteArray g_sendBuffer = NULL;
static uint32_t g_targetCnt = 0;
static uint32_t g_currentSentCnt = 0;
static bool g_isFinishedSendData = false;
static oc_mutex g_SendFinishMutex = NULL;
static oc_mutex g_threadMutex = NULL;
static oc_cond g_threadCond = NULL;
static oc_cond g_deviceDescCond = NULL;

static oc_mutex g_threadSendMutex = NULL;
static oc_mutex g_threadWriteCharacteristicMutex = NULL;
static oc_cond g_threadWriteCharacteristicCond = NULL;
static bool g_isSignalSetFlag = false;

static oc_mutex g_bleReqRespClientCbMutex = NULL;
static oc_mutex g_bleServerBDAddressMutex = NULL;

static oc_mutex g_deviceListMutex = NULL;
static oc_mutex g_gattObjectMutex = NULL;
static oc_mutex g_deviceStateListMutex = NULL;

static oc_mutex g_deviceScanRetryDelayMutex = NULL;
static oc_cond g_deviceScanRetryDelayCond = NULL;

static oc_mutex g_threadScanIntervalMutex = NULL;
static oc_cond g_threadScanIntervalCond = NULL;

static oc_mutex g_threadSendStateMutex = NULL;
static oc_mutex g_setValueMutex = NULL;

static int32_t g_scanIntervalTime = WAIT_TIME_SCAN_INTERVAL_DEFAULT;
static int32_t g_scanIntervalTimePrev = WAIT_TIME_SCAN_INTERVAL_DEFAULT;
static int32_t g_intervalCount = 0;
static bool g_isWorkingScanThread = false;
static CALEScanState_t g_curScanningStep = BLE_SCAN_NONE;
static CALEScanState_t g_nextScanningStep = BLE_SCAN_ENABLE;

static CABLEDataReceivedCallback g_CABLEClientDataReceivedCallback = NULL;
static int32_t g_jniIntSdk = -1;

static bool g_setHighQoS = true;
static bool g_setFullScanFlag = true;
jclass g_LEInterface = NULL;
/**
 * check if retry logic for connection routine has to be stopped or not.
 * in case of error value including this method, connection routine has to be stopped.
 * since there is no retry logic for this error reason in this client.
 * @param state constant value of bluetoothgatt.
 * @return true - waiting for background connection in BT platform.
 *         false - connection routine has to be stopped.
 */
static bool CALECheckConnectionStateValue(jint state)
{
    switch(state)
    {
        case GATT_CONNECTION_PRIORITY_BALANCED:
        case GATT_FAILURE:
        case GATT_INSUFFICIENT_AUTHENTICATION:
        case GATT_INSUFFICIENT_ENCRYPTION:
        case GATT_INVALID_ATTRIBUTE_LENGTH:
        case GATT_INVALID_OFFSET:
        case GATT_READ_NOT_PERMITTED:
        case GATT_REQUEST_NOT_SUPPORTED:
        case GATT_WRITE_NOT_PERMITTED:
            return true;
        default:
            return false;
    }
}

/**
 * delete global reference for g_sendBuffer
 * @param[in]   env                   JNI interface pointer.
 */
static void CALEDeleteSendBuffer(JNIEnv *env)
{
    OIC_LOG(INFO, TAG, "CALEDeleteSendBuffer");
    oc_mutex_lock(g_setValueMutex);
    if (g_sendBuffer)
    {
        OIC_LOG(INFO, TAG, "delete send buffer");
        (*env)->DeleteGlobalRef(env, g_sendBuffer);
        g_sendBuffer = NULL;
    }
    oc_mutex_unlock(g_setValueMutex);
}

void CALEClientSetScanInterval(int32_t intervalTime, int32_t workingCount,
                               CALEScanState_t nextScanningStep)
{
    OIC_LOG_V(DEBUG, TAG, "CALEClientSetScanInterval : %d -> %d, next scan state will be %d",
              g_scanIntervalTime, intervalTime, nextScanningStep);

    // previous time should be stored.
    if (0 < workingCount)
    {
        g_scanIntervalTimePrev = g_scanIntervalTime;
    }
    g_scanIntervalTime = intervalTime;
    g_intervalCount = workingCount;
    g_nextScanningStep = nextScanningStep;
}

void CALERestartScanWithInterval(int32_t intervalTime, int32_t workingCount,
                                 CALEScanState_t nextScanningStep)
{
    if (intervalTime == g_scanIntervalTime
            && workingCount == g_intervalCount
            && nextScanningStep == g_nextScanningStep)
    {
        OIC_LOG(DEBUG, TAG, "setting duplicate interval time");
        return;
    }

    CALEClientSetScanInterval(intervalTime, workingCount, nextScanningStep);
    oc_cond_signal(g_threadScanIntervalCond);
}

static void CALEScanThread(void* object)
{
    (void)object;

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return;
        }
        isAttached = true;
    }

    oc_mutex_lock(g_threadScanIntervalMutex);
    while(g_isWorkingScanThread)
    {
        OIC_LOG(DEBUG, TAG, "scan waiting time out");
        if (BLE_SCAN_ENABLE == g_curScanningStep)
        {
            //stop scan
            CAResult_t ret = CALEClientStopScan();
            if (CA_STATUS_OK != ret)
            {
                OIC_LOG(INFO, TAG, "CALEClientStopScan has failed");
            }
        }
        else if (BLE_SCAN_DISABLE == g_curScanningStep)
        {
            //start scan
            CAResult_t ret = CALEClientStartScan();
            if (CA_STATUS_OK != ret)
            {
                OIC_LOG(INFO, TAG, "CALEClientStartScan has failed");
            }
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "scan thread is started");
            // standby scanning
            CALERestartScanWithInterval(0, 0, BLE_SCAN_DISABLE);
        }

        OIC_LOG_V(DEBUG, TAG, "wait for Scan Interval Time during %d sec", g_scanIntervalTime);
        if (OC_WAIT_SUCCESS == oc_cond_wait_for(g_threadScanIntervalCond,
                                                g_threadScanIntervalMutex,
                                                g_scanIntervalTime * MICROSECS_PER_SEC))
        {
            // called signal scan thread will be terminated
            OIC_LOG(DEBUG, TAG, "signal scanInterval waiting");
            if (BLE_SCAN_DISABLE == g_nextScanningStep)
            {
                g_curScanningStep = BLE_SCAN_ENABLE;
            }
            else
            {
                g_curScanningStep = BLE_SCAN_DISABLE;
            }
        }
        else
        {
           if (BLE_SCAN_ENABLE == g_curScanningStep)
           {
               if (g_intervalCount > 0)
               {
                   if (g_intervalCount == 1)
                   {
                       OIC_LOG(DEBUG, TAG, "reset default time");
                       CALEClientSetScanInterval(g_scanIntervalTimePrev, 0, BLE_SCAN_ENABLE);
                   }
                   g_intervalCount--;
                   OIC_LOG_V(DEBUG, TAG, "interval count : %d", g_intervalCount);
               }
               g_curScanningStep = BLE_SCAN_DISABLE;
           }
           else
           {
               g_curScanningStep = BLE_SCAN_ENABLE;
           }
        }
    }
    oc_mutex_unlock(g_threadScanIntervalMutex);

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }
}

CAResult_t CALEClientStartScanWithInterval()
{
    if (g_isWorkingScanThread)
    {
        OIC_LOG(DEBUG, TAG, "scan interval logic already running");
        return CA_STATUS_OK;
    }

    // initialize scan flags
    g_curScanningStep = BLE_SCAN_NONE;
    g_isWorkingScanThread = true;
    g_intervalCount = 0;
    g_scanIntervalTime = g_scanIntervalTimePrev;
    g_nextScanningStep = BLE_SCAN_ENABLE;

    if (CA_STATUS_OK != ca_thread_pool_add_task(g_threadPoolHandle, CALEScanThread, NULL, NULL))
    {
        OIC_LOG(ERROR, TAG, "Failed to create read thread!");
        g_isWorkingScanThread = false;
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

void CALEClientStopScanWithInterval()
{
    g_isWorkingScanThread = false;
    oc_cond_signal(g_threadScanIntervalCond);
}

//getting jvm
void CALEClientJniInit()
{
    OIC_LOG(DEBUG, TAG, "CALEClientJniInit");
    g_jvm = (JavaVM*) CANativeJNIGetJavaVM();
}

void CALEClientJNISetContext()
{
    OIC_LOG(DEBUG, TAG, "CALEClientJNISetContext");
    g_context = (jobject) CANativeJNIGetContext();
}

CAResult_t CALECreateJniInterfaceObject()
{
    OIC_LOG(DEBUG, TAG, "CALECreateJniInterfaceObject");

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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    jmethodID mid_getApplicationContext = CAGetJNIMethodID(env, "android/content/Context",
                                                           "getApplicationContext",
                                                           "()Landroid/content/Context;");

    if (!mid_getApplicationContext)
    {
        OIC_LOG(ERROR, TAG, "Could not get getApplicationContext method");
        return CA_STATUS_FAILED;
    }

    jobject jApplicationContext = (*env)->CallObjectMethod(env, g_context,
                                                           mid_getApplicationContext);
    if (!jApplicationContext)
    {
        OIC_LOG(ERROR, TAG, "Could not get application context");
        goto error_exit;
    }

    jclass jni_LEInterface = (*env)->FindClass(env, "org/iotivity/ca/CaLeClientInterface");
    if (!jni_LEInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaLeClientInterface class");
        goto error_exit;
    }
    g_LEInterface = (jclass)((*env)->NewGlobalRef(env, jni_LEInterface));

    jmethodID LeInterfaceConstructorMethod = (*env)->GetMethodID(env, jni_LEInterface, "<init>",
                                                                 "(Landroid/content/Context;)V");
    if (!LeInterfaceConstructorMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaLeClientInterface constructor method");
        goto error_exit;
    }

    (*env)->NewObject(env, jni_LEInterface, LeInterfaceConstructorMethod, jApplicationContext);
    OIC_LOG(DEBUG, TAG, "Create instance for CaLeClientInterface");

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;

error_exit:
    CACheckJNIException(env);
    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_FAILED;
}

CAResult_t CALEClientInitialize()
{
    OIC_LOG(DEBUG, TAG, "CALEClientInitialize");

    CALEClientJniInit();

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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    g_jniIntSdk = CALEGetBuildVersion(env);
    if (g_jniIntSdk < BLE_MIN_API_LEVEL)
    {
        OIC_LOG(ERROR, TAG, "it is not supported");

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }
        return CA_STATUS_FAILED;
    }

    CAResult_t ret = CALEClientInitGattMutexVaraibles();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientInitGattMutexVaraibles has failed!");
        CALEClientTerminateGattMutexVariables();

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }

        return ret;
    }

    g_deviceDescCond = oc_cond_new();

    // init mutex for send logic
    g_threadCond = oc_cond_new();
    g_threadWriteCharacteristicCond = oc_cond_new();
    g_deviceScanRetryDelayCond = oc_cond_new();
    g_threadScanIntervalCond =  oc_cond_new();

    CALEClientCreateDeviceList();
    CALEClientJNISetContext();

    ret = CALEClientCreateUUIDList();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientCreateUUIDList has failed");

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }

        return ret;
    }

    ret = CALECreateJniInterfaceObject(); /* create java caleinterface instance*/
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALECreateJniInterfaceObject has failed");

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }

        return ret;
    }
    g_isStartedLEClient = true;

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;
}

void CALEClientTerminate()
{
    OIC_LOG(DEBUG, TAG, "CALEClientTerminate");

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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return;
        }
        isAttached = true;
    }

    // stop scan
    CAResult_t ret = CALEClientStopScan();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(INFO, TAG, "CALEClientStopScan has failed");
    }

    if (g_leScanCallback)
    {
        (*env)->DeleteGlobalRef(env, g_leScanCallback);
        g_leScanCallback = NULL;
    }

    if (g_leGattCallback)
    {
        (*env)->DeleteGlobalRef(env, g_leGattCallback);
        g_leGattCallback = NULL;
    }

    if (g_LEInterface)
    {
        (*env)->DeleteGlobalRef(env, g_LEInterface);
        g_LEInterface = NULL;
    }

    CALEDeleteSendBuffer(env);

    if (g_uuidList)
    {
        (*env)->DeleteGlobalRef(env, g_uuidList);
        g_uuidList = NULL;
    }

    ret = CALERemoveAllDeviceState(g_deviceStateList,
                                   g_deviceStateListMutex);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALERemoveAllDeviceState has failed");
    }

    oc_mutex_lock(g_deviceStateListMutex);
    OICFree(g_deviceStateList);
    g_deviceStateList = NULL;
    oc_mutex_unlock(g_deviceStateListMutex);

    ret = CALEClientRemoveAllScanDevices(env);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientRemoveAllScanDevices has failed");
    }

    ret = CALEClientRemoveAllGattObjs(env);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientRemoveAllGattObjs has failed");
    }

    CALEClientSetSendFinishFlag(true);

    CALEClientTerminateGattMutexVariables();
    CALEClientDestroyJniInterface();

    oc_cond_free(g_deviceDescCond);
    oc_cond_free(g_threadCond);
    oc_cond_free(g_threadWriteCharacteristicCond);
    oc_cond_free(g_deviceScanRetryDelayCond);
    oc_cond_free(g_threadScanIntervalCond);

    g_deviceDescCond = NULL;
    g_threadCond = NULL;
    g_threadWriteCharacteristicCond = NULL;
    g_deviceScanRetryDelayCond = NULL;
    g_threadScanIntervalCond = NULL;

    g_isSignalSetFlag = false;

    // stop scanning
    CALEClientStopScanWithInterval();

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }
}

jobject CALEClientHiddenConnectGatt(jobject btDevice, const char* address)
{
    OIC_LOG(INFO, TAG, "IN - CALEClientHiddenConnectGatt");

    if (!g_jvm)
    {
        OIC_LOG(ERROR, TAG, "g_jvm is null");
        return NULL;
    }

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return NULL;
        }
        isAttached = true;
    }

    jstring jni_address = (*env)->NewStringUTF(env, address);
    jmethodID jni_connectGattHiddenMethod = (*env)->GetStaticMethodID(env, g_LEInterface,
                                                                     "connectGattforHidden",
                                                                     "(Landroid/bluetooth/BluetoothDevice;"
                                                                     "Ljava/lang/String;)Landroid/bluetooth/BluetoothGatt;");
    if (!jni_connectGattHiddenMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get jni_connectGatt Hidden Method");
        goto error_exit;
    }

    jobject gatt = (*env)->CallStaticObjectMethod(env, g_LEInterface, jni_connectGattHiddenMethod, btDevice, jni_address);

    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "connectGattforHidden has failed");
        goto detach_thread;
    }

    OIC_LOG(INFO, TAG, "OUT - CALEClientHiddenConnectGatt");

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return gatt;

error_exit:
    CACheckJNIException(env);

detach_thread:
    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return NULL;
}

CAResult_t CALEClientDestroyJniInterface()
{
    OIC_LOG(DEBUG, TAG, "CALEClientDestroyJniInterface");

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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    jclass jni_LeInterface = (*env)->FindClass(env, "org/iotivity/ca/CaLeClientInterface");
    if (!jni_LeInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaLeClientInterface class");
        goto error_exit;
    }

    jmethodID jni_InterfaceDestroyMethod = (*env)->GetStaticMethodID(env, jni_LeInterface,
                                                                     "destroyLeInterface",
                                                                     "()V");
    if (!jni_InterfaceDestroyMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaLeClientInterface destroy method");
        goto error_exit;
    }

    (*env)->CallStaticVoidMethod(env, jni_LeInterface, jni_InterfaceDestroyMethod);

    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "destroyLeInterface has failed");
        goto detach_thread;
    }

    OIC_LOG(DEBUG, TAG, "Destroy instance for CaLeClientInterface");

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;

error_exit:
    CACheckJNIException(env);

detach_thread:
    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_FAILED;
}

void CALEClientSendFinish(JNIEnv *env, jobject gatt)
{
    OIC_LOG(DEBUG, TAG, "CALEClientSendFinish");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");

    if (gatt)
    {
        CAResult_t res = CALEClientDisconnect(env, gatt);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientDisconnect has failed");
        }
    }
    CALEClientUpdateSendCnt(env);
}

CAResult_t CALEClientSendNegotiationMessage(const char* address)
{
    VERIFY_NON_NULL(address, TAG, "address is null");

    return CALEClientSendUnicastMessageImpl(address, NULL, 0);
}

CAResult_t CALEClientSendUnicastMessage(const char* address,
                                        const uint8_t* data,
                                        const uint32_t dataLen)
{
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    return CALEClientSendUnicastMessageImpl(address, data, dataLen);
}

CAResult_t CALEClientSendMulticastMessage(const uint8_t* data,
                                          const uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEClientSendMulticastMessage(%p)", data);
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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAResult_t ret = CALEClientSendMulticastMessageImpl(env, data, dataLen);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientSendMulticastMessageImpl has failed");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return ret;
}

CAResult_t CALEClientStartUnicastServer(const char* address)
{
#ifndef TB_LOG
    (void)address;
#endif
    OIC_LOG_V(DEBUG, TAG, "it is not needed in this platform (%s)", address);

    return CA_NOT_SUPPORTED;
}

CAResult_t CALEClientStartMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "it is not needed in this platform");

    return CA_NOT_SUPPORTED;
}

void CALEClientStopUnicastServer()
{
    OIC_LOG(DEBUG, TAG, "CALEClientStopUnicastServer");
}

void CALEClientStopMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "CALEClientStopMulticastServer");
}

void CALEClientSetCallback(CAPacketReceiveCallback callback)
{
    g_packetReceiveCallback = callback;
}

void CASetBLEClientErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    g_clientErrorCallback = callback;
}

CAResult_t CALEClientIsThereScannedDevices(JNIEnv *env, const char* address)
{
    VERIFY_NON_NULL(env, TAG, "env");

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceList is not available");
        return CA_STATUS_FAILED;
    }

    if (0 == u_arraylist_length(g_deviceList) // multicast
            || (address && !CALEClientIsDeviceInScanDeviceList(env, address))) // unicast
    {
        // Wait for LE peripherals to be discovered.

        // Number of times to wait for discovery to complete.
        static size_t const RETRIES = 5;

        static uint64_t const TIMEOUT =
            2 * MICROSECS_PER_SEC;  // Microseconds

        // set scan interval and start scan
        CALERestartScanWithInterval(WAIT_TIME_SCANNED_CHECKING, 1, BLE_SCAN_ENABLE);

        bool devicesDiscovered = false;
        for (size_t i = 0; i < RETRIES; ++i)
        {
            OIC_LOG(DEBUG, TAG, "waiting for target device");
            if (oc_cond_wait_for(g_deviceDescCond,
                                 g_threadSendMutex,
                                 TIMEOUT) == OC_WAIT_SUCCESS)
            {
                OIC_LOG(DEBUG, TAG, "time out");
                oc_mutex_lock(g_deviceListMutex);
                size_t scannedDeviceLen = u_arraylist_length(g_deviceList);
                oc_mutex_unlock(g_deviceListMutex);

                if (0 < scannedDeviceLen)
                {
                    if (!address  // multicast
                        || (address && CALEClientIsDeviceInScanDeviceList(env, address))) // unicast
                    {
                      devicesDiscovered = true;
                      break;
                    }
                    else
                    {
                        if (address)
                        {
                            OIC_LOG(INFO, TAG, "waiting..");

                            oc_mutex_lock(g_deviceScanRetryDelayMutex);
                            if (oc_cond_wait_for(g_deviceScanRetryDelayCond,
                                                 g_deviceScanRetryDelayMutex,
                                                 MICROSECS_PER_SEC) == OC_WAIT_SUCCESS)
                            {
                                OIC_LOG(INFO, TAG, "finish to waiting for target device");
                                oc_mutex_unlock(g_deviceScanRetryDelayMutex);
                                break;
                            }
                            oc_mutex_unlock(g_deviceScanRetryDelayMutex);
                            // time out

                            // checking whether a target device is found while waiting for time-out.
                            if (CALEClientIsDeviceInScanDeviceList(env, address))
                            {
                                devicesDiscovered = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        // reset scan interval time after checking scanned devices
        CALERestartScanWithInterval(0, 0, BLE_SCAN_DISABLE);

        // time out for scanning devices
        if (!devicesDiscovered)
        {
            return CA_STATUS_FAILED;
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "there is a target device in the scanned devices");
    }

    return CA_STATUS_OK;
}


CAResult_t CALEClientSendUnicastMessageImpl(const char* address, const uint8_t* data,
                                            const uint32_t dataLen)
{
    OIC_LOG_V(INFO, TAG, "CALEClientSendUnicastMessageImpl, address: %s, data: %p", address,
              data);
    VERIFY_NON_NULL(address, TAG, "address is null");

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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);
        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    oc_mutex_lock(g_threadSendMutex);

    CALEClientSetSendFinishFlag(false);

    CAResult_t ret = CALEClientIsThereScannedDevices(env, address);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(INFO, TAG, "there is no scanned device");
        goto error_exit;
    }

    if (g_context && g_deviceList)
    {
        uint32_t length = u_arraylist_length(g_deviceList);
        for (uint32_t index = 0; index < length; index++)
        {
            jobject jarrayObj = (jobject) u_arraylist_get(g_deviceList, index);
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
                CACheckJNIException(env);
                goto error_exit;
            }

            if (!strcasecmp(setAddress, address))
            {
                (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
                (*env)->DeleteLocalRef(env, jni_setAddress);

                CALEDeleteSendBuffer(env);

                if (data && dataLen > 0)
                {
                    jbyteArray jni_arr = (*env)->NewByteArray(env, dataLen);
                    CACheckJNIException(env);
                    (*env)->SetByteArrayRegion(env, jni_arr, 0, dataLen, (jbyte*) data);
                    CACheckJNIException(env);
                    g_sendBuffer = (jbyteArray)(*env)->NewGlobalRef(env, jni_arr);
                    CACheckJNIException(env);
                }

                // Target device to send message is just one.
                g_targetCnt = 1;

                ret = CALEClientSendData(env, jarrayObj);
                if (CA_STATUS_OK != ret)
                {
                    OIC_LOG(ERROR, TAG, "CALEClientSendData in unicast is failed");
                    goto error_exit;
                }

                OIC_LOG(INFO, TAG, "wake up");
                break;
            }
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->DeleteLocalRef(env, jni_setAddress);
        }
    }

    OIC_LOG(DEBUG, TAG, "connection routine is finished for unicast");

    // wait for finish to send data through "CALeGattServicesDiscoveredCallback"
    // if there is no connection state.
    oc_mutex_lock(g_threadMutex);
    if (!g_isFinishedSendData)
    {
        OIC_LOG(DEBUG, TAG, "waiting send finish signal");
        oc_cond_wait(g_threadCond, g_threadMutex);
        OIC_LOG(DEBUG, TAG, "connection / send is finished for unicast");
    }
    oc_mutex_unlock(g_threadMutex);

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    oc_mutex_unlock(g_threadSendMutex);
    OIC_LOG(INFO, TAG, "unicast - send logic has finished");
    if (CALEIsValidState(address, CA_LE_SEND_STATE,
                         STATE_SEND_SUCCESS,
                         g_deviceStateList,
                         g_deviceStateListMutex))
    {
        OIC_LOG(INFO, TAG, "send success");
        ret = CA_STATUS_OK;
    }
    else if (CALEIsValidState(address, CA_LE_SEND_STATE,
                              STATE_SEND_MTU_NEGO_SUCCESS,
                              g_deviceStateList,
                              g_deviceStateListMutex))
    {
        OIC_LOG(INFO, TAG, "mtu nego success");
        ret = CA_STATUS_OK;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "send failure");
        ret = CA_SEND_FAILED;
    }

    // reset send state
    CAResult_t resetRet = CALEUpdateDeviceState(address, CA_LE_SEND_STATE,
                                                STATE_SEND_NONE,
                                                g_deviceStateList,
                                                g_deviceStateListMutex);
    if (CA_STATUS_OK != resetRet)
    {
        OIC_LOG_V(ERROR, TAG, "CALEUpdateDeviceState has failed (%d)", resetRet);
        ret = CA_SEND_FAILED;
    }

    return ret;

    // error label.
error_exit:
    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    oc_mutex_unlock(g_threadSendMutex);
    return CA_SEND_FAILED;
}

CAResult_t CALEClientSendMulticastMessageImpl(JNIEnv *env, const uint8_t* data,
                                              const uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CASendMulticastMessageImpl, send to, data: %p, %u", data, dataLen);
    VERIFY_NON_NULL(data, TAG, "data is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceList is null");
        return CA_STATUS_FAILED;
    }

    oc_mutex_lock(g_threadSendMutex);

    CALEClientSetSendFinishFlag(false);

    OIC_LOG(DEBUG, TAG, "set byteArray for data");
    CALEDeleteSendBuffer(env);

    CAResult_t res = CALEClientIsThereScannedDevices(env, NULL);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(INFO, TAG, "there is no scanned device");
        goto error_exit;
    }

    uint32_t length = u_arraylist_length(g_deviceList);
    g_targetCnt = length;

    jbyteArray jni_arr = (*env)->NewByteArray(env, dataLen);
    (*env)->SetByteArrayRegion(env, jni_arr, 0, dataLen, (jbyte*) data);
    g_sendBuffer = (jbyteArray)(*env)->NewGlobalRef(env, jni_arr);

    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_deviceList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is not available");
            continue;
        }

        res = CALEClientSendData(env, jarrayObj);
        if (res != CA_STATUS_OK)
        {
            OIC_LOG(ERROR, TAG, "BT device - send has failed");
        }
    }

    OIC_LOG(DEBUG, TAG, "connection routine is finished for multicast");

    // wait for finish to send data through "CALeGattServicesDiscoveredCallback"
    oc_mutex_lock(g_threadMutex);
    if (!g_isFinishedSendData)
    {
        OIC_LOG(DEBUG, TAG, "waiting send finish signal");
        oc_cond_wait(g_threadCond, g_threadMutex);
        OIC_LOG(DEBUG, TAG, "connection / send is finished for multicast");
    }
    oc_mutex_unlock(g_threadMutex);
    oc_mutex_unlock(g_threadSendMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALEClientSendMulticastMessageImpl");
    return CA_STATUS_OK;

error_exit:
    oc_mutex_unlock(g_threadSendMutex);
    OIC_LOG(ERROR, TAG, "OUT - CALEClientSendMulticastMessageImpl");
    return CA_SEND_FAILED;
}

CAResult_t CALEClientSendData(JNIEnv *env, jobject device)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientSendData");
    VERIFY_NON_NULL(device, TAG, "device is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    // get BLE address from bluetooth device object.
    char* address = NULL;
    CALEState_t* state = NULL;
    jstring jni_address = CALEClientGetLEAddressFromBTDevice(env, device);
    if (jni_address)
    {
        OIC_LOG(INFO, TAG, "there is gatt object..it's not first connection");
        address = (char*)(*env)->GetStringUTFChars(env, jni_address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is not available");
            CACheckJNIException(env);
            return CA_STATUS_FAILED;
        }
        oc_mutex_lock(g_deviceStateListMutex);
        state = CALEGetStateInfo(address, g_deviceStateList);
        oc_mutex_unlock(g_deviceStateListMutex);
    }

    // Since disconnect event can be caused from BT stack while connection step is running.
    // DeviceState has to have current status for processing send failure.
    OIC_LOG(INFO, TAG, "set STATE_SEND_PREPARING");
    CAResult_t res = CALEClientUpdateDeviceStateWithBtDevice(env, device,
                                                             CA_LE_SEND_STATE,
                                                             STATE_SEND_PREPARING);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientUpdateDeviceStateWithBtDevice has failed");
        if (address)
        {
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
        }
        return CA_STATUS_FAILED;
    }

    if (!state)
    {
        OIC_LOG(DEBUG, TAG, "state is empty..start to connect LE");

        // cancel previous connection request before connection
        // if there is gatt object in g_gattObjectList.
        if (jni_address)
        {
            jobject gatt = CALEClientGetGattObjInList(env, address);
            if (gatt)
            {
                CAResult_t res = CALEClientDisconnect(env, gatt);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(INFO, TAG, "there is no gatt object");
                }
            }
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
        }

        // connection request
        jobject newGatt = CALEClientConnect(env, device,
                                            JNI_FALSE);
        if (NULL == newGatt)
        {
            OIC_LOG(ERROR, TAG, "CALEClientConnect has failed");
            return CA_STATUS_FAILED;
        }
    }
    else
    {
        if (CALEIsValidState(address, CA_LE_CONNECTION_STATE,
                             STATE_SERVICE_CONNECTED,
                             g_deviceStateList,
                             g_deviceStateListMutex))
        {
            OIC_LOG(INFO, TAG, "GATT has already connected");

            jobject gatt = CALEClientGetGattObjInList(env, address);
            if (!gatt)
            {
                OIC_LOG(ERROR, TAG, "CALEClientGetGattObjInList has failed");
                (*env)->ReleaseStringUTFChars(env, jni_address, address);
                return CA_STATUS_FAILED;
            }

            CAResult_t ret = CALESetValueAndWriteCharacteristic(env, gatt);
            if (CA_STATUS_OK != ret)
            {
                OIC_LOG(ERROR, TAG, "CALESetValueAndWriteCharacteristic has failed");
                (*env)->ReleaseStringUTFChars(env, jni_address, address);
                return ret;
            }
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
        }
        else if(CALEIsValidState(address, CA_LE_CONNECTION_STATE,
                                 STATE_CONNECTED,
                                 g_deviceStateList,
                                 g_deviceStateListMutex))
        {
            OIC_LOG(INFO, TAG, "service connecting...");
        }
        else if(CALEIsValidState(address, CA_LE_CONNECTION_STATE,
                                 STATE_DISCONNECTED,
                                 g_deviceStateList,
                                 g_deviceStateListMutex))
        {
            OIC_LOG(INFO, TAG, "STATE_DISCONNECTED - start to connect LE");

            // cancel previous connection request before connection
            // if there is gatt object in g_gattObjectList.
            if (jni_address)
            {
                jobject gatt = CALEClientGetGattObjInList(env, address);
                if (gatt)
                {
                    CAResult_t res = CALEClientDisconnect(env, gatt);
                    if (CA_STATUS_OK != res)
                    {
                        OIC_LOG(INFO, TAG, "there is no gatt object");
                    }
                }
                (*env)->ReleaseStringUTFChars(env, jni_address, address);
            }

            OIC_LOG(DEBUG, TAG, "start to connect LE");
            jobject gatt = CALEClientConnect(env, device,
                                             CALEGetFlagFromState(env, jni_address,
                                                                  CA_LE_AUTO_CONNECT_FLAG,
                                                                  g_deviceStateList,
                                                                  g_deviceStateListMutex));

            if (NULL == gatt)
            {
                OIC_LOG(ERROR, TAG, "CALEClientConnect has failed");
                return CA_STATUS_FAILED;
            }
        }
    }

    return CA_STATUS_OK;
}

jstring CALEClientGetAddressFromGattObj(JNIEnv *env, jobject gatt)
{
    VERIFY_NON_NULL_RET(gatt, TAG, "gatt is null", NULL);
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    jmethodID jni_mid_getDevice = CAGetJNIMethodID(env, CLASSPATH_BT_GATT, "getDevice",
                                                   "()Landroid/bluetooth/BluetoothDevice;");
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
        CACheckJNIException(env);
        return NULL;
    }

    return jni_address;
}

/**
 * BLE layer
 */
CAResult_t CALEClientGattClose(JNIEnv *env, jobject bluetoothGatt)
{
    // GATT CLOSE
    OIC_LOG(DEBUG, TAG, "Gatt Close");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    // get BluetoothGatt method
    OIC_LOG(DEBUG, TAG, "get BluetoothGatt method");
    jmethodID jni_mid_closeGatt = CAGetJNIMethodID(env, CLASSPATH_BT_GATT, "close", "()V");
    if (!jni_mid_closeGatt)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_closeGatt is null");
        return CA_STATUS_OK;
    }

    // call disconnect gatt method
    OIC_LOG(DEBUG, TAG, "request to close GATT");
    (*env)->CallVoidMethod(env, bluetoothGatt, jni_mid_closeGatt);

    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "closeGATT has failed");
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CALEClientStartScan()
{
    if (!g_isStartedLEClient)
    {
        OIC_LOG(ERROR, TAG, "LE client is not started");
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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);
        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    OIC_LOG(DEBUG, TAG, "CALEClientStartScan");

    CAResult_t ret = CA_STATUS_OK;
    // scan gatt server with UUID
    if (g_leScanCallback && g_uuidList)
    {
        if (g_jniIntSdk >= BLE_SCAN_API_LEVEL)
        {
            if (!g_setFullScanFlag)
            {
                //new uuid scan with callback
                ret = CALEClientStartScanWithUUIDImplForV21(env, g_uuidList, g_leScanCallback);
            }
            else
            {
                //new full scan with callback
                ret = CALEClientStartScanImplForV21(env, g_leScanCallback);
            }
        }
        else
        {
            if (!g_setFullScanFlag)
            {
                ret = CALEClientStartScanWithUUIDImpl(env, g_uuidList, g_leScanCallback);
            }
            else
            {
                ret = CALEClientStartScanImpl(env, g_leScanCallback);
            }
        }

        if (CA_STATUS_OK != ret)
        {
            if (CA_ADAPTER_NOT_ENABLED == ret)
            {
                OIC_LOG(DEBUG, TAG, "Adapter is disabled");
            }
            else
            {
                OIC_LOG(ERROR, TAG, "start scan has failed");
            }
        }
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return ret;
}

CAResult_t CALEClientStartScanImpl(JNIEnv *env, jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CALEClientStartScanImpl IN");
    VERIFY_NON_NULL(callback, TAG, "callback is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get default bt adapter class
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get start le scan method
    jmethodID jni_mid_startLeScan = (*env)->GetMethodID(env, jni_cid_BTAdapter, "startLeScan",
                                                        "(Landroid/bluetooth/BluetoothAdapter$"
                                                        "LeScanCallback;)Z");
    if (!jni_mid_startLeScan)
    {
        OIC_LOG(ERROR, TAG, "startLeScan: jni_mid_startLeScan is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // gat bt adapter object
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_obj_BTAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // call start le scan method
    OIC_LOG(INFO, TAG, "CALL API - startLeScan");
    jboolean jni_obj_startLeScan = (*env)->CallBooleanMethod(env, jni_obj_BTAdapter,
                                                             jni_mid_startLeScan, callback);
    if (!jni_obj_startLeScan)
    {
        OIC_LOG(INFO, TAG, "startLeScan has failed");
        CACheckJNIException(env);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "LeScan has started");
    }

    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    return CA_STATUS_OK;
}

CAResult_t CALEClientStartScanImplForV21(JNIEnv *env, jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CALEClientStartScanImplForV21 IN");
    VERIFY_NON_NULL(callback, TAG, "callback is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    CAResult_t res = CA_STATUS_FAILED;
    // get default bt adapter class
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    "()Landroid/bluetooth/"
                                                                    "BluetoothAdapter;");
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get bluetoothLeScanner class
    jclass jni_cid_leScanner = (*env)->FindClass(env, CLASSPATH_LE_SCANNER);
    if (!jni_cid_leScanner)
    {
        OIC_LOG(ERROR, TAG, "getState From leScanner: jni_cid_leScanner is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getBluetoothLeScanner = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                                  "getBluetoothLeScanner",
                                                                  "()Landroid/bluetooth/"
                                                                  "le/BluetoothLeScanner;");
    if (!jni_mid_getBluetoothLeScanner)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBluetoothLeScanner is null");
        CACheckJNIException(env);
        goto error_exit;
    }

    // get startScan(ScanCallback callback) method
    jmethodID jni_mid_startScan = (*env)->GetMethodID(env, jni_cid_leScanner, "startScan",
                                                      "(Landroid/bluetooth/le/ScanCallback;)V");
    if (!jni_mid_startScan)
    {
        OIC_LOG(ERROR, TAG, "startScan: jni_mid_startScan is null");
        CACheckJNIException(env);
        goto error_exit;
    }

    // gat le scanner object
    jobject jni_obj_leScanner = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                         jni_mid_getBluetoothLeScanner);
    if (!jni_obj_leScanner)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_obj_leScanner is null");
        CACheckJNIException(env);
        goto error_exit;
    }

    // call startScan method
    OIC_LOG(INFO, TAG, "CALL API - startScan(for level21)");
    (*env)->CallVoidMethod(env, jni_obj_leScanner, jni_mid_startScan, callback);
    if (CACheckJNIException(env))
    {
        OIC_LOG(INFO, TAG, "startScan has failed");
        (*env)->DeleteLocalRef(env, jni_obj_leScanner);
        goto error_exit;
    }
    res = CA_STATUS_OK;
    (*env)->DeleteLocalRef(env, jni_obj_leScanner);

error_exit:
    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_cid_leScanner);
    return res;
}

CAResult_t CALEClientStartScanWithUUIDImpl(JNIEnv *env, jobjectArray uuids, jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CALEClientStartScanWithUUIDImpl IN");
    VERIFY_NON_NULL(callback, TAG, "callback is null");
    VERIFY_NON_NULL(uuids, TAG, "uuids is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get start le scan method
    jmethodID jni_mid_startLeScan = (*env)->GetMethodID(env, jni_cid_BTAdapter, "startLeScan",
                                                        "([Ljava/util/UUID;Landroid/bluetooth/"
                                                        "BluetoothAdapter$LeScanCallback;)Z");
    if (!jni_mid_startLeScan)
    {
        OIC_LOG(ERROR, TAG, "startLeScan: jni_mid_startLeScan is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get bt adapter object
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_obj_BTAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // call start le scan method
    OIC_LOG(INFO, TAG, "CALL API - startLeScan (with UUID)");
    jboolean jni_obj_startLeScan = (*env)->CallBooleanMethod(env, jni_obj_BTAdapter,
                                                             jni_mid_startLeScan, uuids, callback);
    if (!jni_obj_startLeScan)
    {
        OIC_LOG(INFO, TAG, "startLeScan has failed");
        CACheckJNIException(env);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "LeScan has started");
    }

    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    return CA_STATUS_OK;
}

CAResult_t CALEClientStartScanWithUUIDImplForV21(JNIEnv *env, jobjectArray uuids, jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CALEClientStartScanWithUUIDImplForV21 IN");
    VERIFY_NON_NULL(callback, TAG, "callback is null");
    VERIFY_NON_NULL(uuids, TAG, "uuids is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get bluetoothLeScanner class
    jclass jni_cid_leScanner = (*env)->FindClass(env, CLASSPATH_LE_SCANNER);
    if (!jni_cid_leScanner)
    {
        OIC_LOG(ERROR, TAG, "getState From leScanner: jni_cid_leScanner is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    // get startScan(with UUID) method
    jmethodID jni_mid_startScan = (*env)->GetMethodID(env, jni_cid_leScanner,
                                                      "startScan",
                                                      "(Ljava/util/List;"
                                                      "Landroid/bluetooth/le/ScanSettings;"
                                                      "Landroid/bluetooth/le/ScanCallback;"
                                                      ")V");
    if (!jni_mid_startScan)
    {
        OIC_LOG(ERROR, TAG, "startScan: jni_mid_startScan is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_leScanner);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_cid_leScanner);

    // get scanfilter.Builder class id
    jclass jni_cid_scanfilterBuilder = (*env)->FindClass(env,
                                                         "android/bluetooth/le/"
                                                         "ScanFilter$Builder");
    if (!jni_cid_scanfilterBuilder)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_cid_scanfilterBuilder is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    // get scanfilter.Builder(ctor) method id
    jmethodID jni_mid_scanfilterBuilderCtor = (*env)->GetMethodID(env, jni_cid_scanfilterBuilder,
                                                                  "<init>", "()V");
    if (!jni_mid_scanfilterBuilderCtor)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_cid_scanfilterBuilderCtor is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_scanfilterBuilder);
        return CA_STATUS_FAILED;
    }

    // call scanfilter.Builder()
    jobject jni_obj_scanfilterBuilder = (*env)->NewObject(env, jni_cid_scanfilterBuilder,
                                                          jni_mid_scanfilterBuilderCtor);
    if (!jni_obj_scanfilterBuilder)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_obj_scanfilterBuilder is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_scanfilterBuilder);
        return CA_STATUS_FAILED;
    }

    // get scanfilter.Builder.setServiceUuid method id
    jmethodID jni_mid_setServiceUuid = (*env)->GetMethodID(env, jni_cid_scanfilterBuilder,
                                                           "setServiceUuid",
                                                           "(Landroid/os/ParcelUuid;)Landroid/"
                                                           "bluetooth/le/ScanFilter$Builder;");
    if (!jni_mid_setServiceUuid)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_mid_setServiceUuid is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_scanfilterBuilder);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilterBuilder);
        return CA_STATUS_FAILED;
    }

    // get scanfilter.Builder.build method id
    jmethodID jni_mid_build_scanfilterBuilder = (*env)->GetMethodID(env,
                                                                    jni_cid_scanfilterBuilder,
                                                                    "build",
                                                                    "()Landroid/bluetooth/le/"
                                                                    "ScanFilter;");
    if (!jni_mid_build_scanfilterBuilder)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_mid_build_scanfilterBuilder is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_scanfilterBuilder);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilterBuilder);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_cid_scanfilterBuilder);

    // call ParcelUuid.fromSting(uuid)
    jobject jni_obj_parcelUuid = CALEGetParcelUuidFromString(env, OIC_GATT_SERVICE_UUID);
    if (!jni_obj_parcelUuid)
    {
        OIC_LOG(ERROR, TAG, "scanSettings: jni_obj_parcelUuid is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilterBuilder);
        return CA_STATUS_FAILED;
    }

    // call setServiceUuid(uuid)
    jobject jni_obj_setServiceUuid = (*env)->CallObjectMethod(env,
                                                              jni_obj_scanfilterBuilder,
                                                              jni_mid_setServiceUuid,
                                                              jni_obj_parcelUuid);
    if (!jni_obj_setServiceUuid)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_obj_setServiceUuid is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilterBuilder);
        (*env)->DeleteLocalRef(env, jni_obj_parcelUuid);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_obj_parcelUuid);
    (*env)->DeleteLocalRef(env, jni_obj_setServiceUuid);

    // call build()
    jobject jni_obj_scanfilter = (*env)->CallObjectMethod(env,
                                                          jni_obj_scanfilterBuilder,
                                                          jni_mid_build_scanfilterBuilder);
    if (!jni_obj_scanfilter)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_obj_scanfilter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilterBuilder);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_obj_scanfilterBuilder);

    // get scanSettings.Builder class id
    jclass jni_cid_scanSettingsBuilder = (*env)->FindClass(env,
                                                          "android/bluetooth/le/"
                                                          "ScanSettings$Builder");
    if (!jni_cid_scanSettingsBuilder)
    {
        OIC_LOG(ERROR, TAG, "scanSettings: jni_cid_scanSettingsBuilder is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        return CA_STATUS_FAILED;
    }

    // get scanSettings.Builder(ctor) method id
    jmethodID jni_mid_scanSettingsBuilderCtor = (*env)->GetMethodID(env, jni_cid_scanSettingsBuilder,
                                                                    "<init>", "()V");
    if (!jni_mid_scanSettingsBuilderCtor)
    {
        OIC_LOG(ERROR, TAG, "scanSettings: jni_mid_scanSettingsBuilderCtor is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_cid_scanSettingsBuilder);
        return CA_STATUS_FAILED;
    }

    // get scanSettings.Builder.setScanMode method id
    jmethodID jni_mid_setScanMode = (*env)->GetMethodID(env, jni_cid_scanSettingsBuilder,
                                                        "setScanMode",
                                                         "(I)Landroid/"
                                                         "bluetooth/le/ScanSettings$Builder;");
    if (!jni_mid_setScanMode)
    {
        OIC_LOG(ERROR, TAG, "scanSettings: jni_mid_setScanMode is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_cid_scanSettingsBuilder);
        return CA_STATUS_FAILED;
    }

    // get scanSettings.Builder.build method id
    jmethodID jni_mid_build_scanSettings = (*env)->GetMethodID(env,
                                                               jni_cid_scanSettingsBuilder,
                                                               "build",
                                                               "()Landroid/bluetooth/le/"
                                                               "ScanSettings;");
    if (!jni_mid_build_scanSettings)
    {
        OIC_LOG(ERROR, TAG, "scanSettings: jni_mid_build_scanSettings is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_cid_scanSettingsBuilder);
        return CA_STATUS_FAILED;
    }

    // call scanSettings.Builder()
    jobject jni_obj_scanSettingBuilder = (*env)->NewObject(env, jni_cid_scanSettingsBuilder,
                                                           jni_mid_scanSettingsBuilderCtor);
    if (!jni_obj_scanSettingBuilder)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_obj_scanSettingBuilder is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_cid_scanSettingsBuilder);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_cid_scanSettingsBuilder);

    jclass jni_cid_arrayList = (*env)->FindClass(env, "java/util/ArrayList");
    if (!jni_cid_arrayList)
    {
        OIC_LOG(ERROR, TAG, "ArrayList: jni_cid_arrayList is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_arrayListCtor = (*env)->GetMethodID(env, jni_cid_arrayList, "<init>", "()V");
    if (!jni_mid_arrayListCtor)
    {
        OIC_LOG(ERROR, TAG, "ArrayList: jni_mid_arrayListCtor is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_cid_arrayList);
        (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_arrayListAdd = (*env)->GetMethodID(env, jni_cid_arrayList,
                                                         "add", "(Ljava/lang/Object;)Z");
    if (!jni_mid_arrayListAdd)
    {
        OIC_LOG(ERROR, TAG, "ArrayList: jni_mid_arrayListAdd is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_cid_arrayList);
        (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);
        return CA_STATUS_FAILED;
    }

    jobject jni_obj_filterList = (*env)->NewObject(env, jni_cid_arrayList, jni_mid_arrayListCtor);
    if (!jni_obj_filterList)
    {
        OIC_LOG(ERROR, TAG, "ArrayList: jni_obj_filterList is null");
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_cid_arrayList);
        (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_cid_arrayList);

    jboolean jni_bool_arrayListIsAdded = (*env)->CallBooleanMethod(env, jni_obj_filterList,
                                                                   jni_mid_arrayListAdd,
                                                                   jni_obj_scanfilter);
    if (!jni_bool_arrayListIsAdded)
    {
        OIC_LOG(ERROR, TAG, "ArrayList: jni_bool_arrayListIsAdded is null");
        (*env)->DeleteLocalRef(env, jni_obj_filterList);
        (*env)->DeleteLocalRef(env, jni_obj_scanfilter);
        (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_obj_scanfilter);

    // get ScanSettings.SCAN_MODE_BALANCED jint value
    jint jni_int_scanBalancedMode = CALEGetConstantsValue(env, CLASSPATH_LE_SCANSETTINGS,
                                                          "SCAN_MODE_BALANCED");
    CACheckJNIException(env);

    // call setScanMode(SCAN_MODE_BALANCED)
    jobject jni_obj_setScanMode = (*env)->CallObjectMethod(env, jni_obj_scanSettingBuilder,
                                                           jni_mid_setScanMode,
                                                           jni_int_scanBalancedMode);
    if (!jni_obj_setScanMode)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_obj_setScanMode is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);
        (*env)->DeleteLocalRef(env, jni_obj_filterList);
        return CA_STATUS_FAILED;
    }

    // call build
    jobject jni_obj_scanSettings = (*env)->CallObjectMethod(env, jni_obj_scanSettingBuilder,
                                                                 jni_mid_build_scanSettings);
    if (!jni_obj_scanSettings)
    {
        OIC_LOG(ERROR, TAG, "scanfilter: jni_obj_scanSettings is null");
        (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);
        (*env)->DeleteLocalRef(env, jni_obj_filterList);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_obj_scanSettingBuilder);

    CAResult_t res = CA_STATUS_FAILED;
    // get default bt adapter class
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        goto error_exit;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    "()Landroid/bluetooth/"
                                                                    "BluetoothAdapter;");
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        goto error_exit;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        goto error_exit;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getBluetoothLeScanner = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                                  "getBluetoothLeScanner",
                                                                  "()Landroid/bluetooth/"
                                                                  "le/BluetoothLeScanner;");
    if (!jni_mid_getBluetoothLeScanner)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBluetoothLeScanner is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        goto error_exit;
    }

    // get le scanner object
    jobject jni_obj_leScanner = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                         jni_mid_getBluetoothLeScanner);
    if (!jni_obj_leScanner)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_leScanner is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        goto error_exit;
    }
    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);

    // call startScan method
    OIC_LOG(INFO, TAG, "CALL API - startScanWithUUID(for level 21)");
    (*env)->CallVoidMethod(env, jni_obj_leScanner, jni_mid_startScan, jni_obj_filterList,
                           jni_obj_scanSettings, callback);
    if (CACheckJNIException(env))
    {
        OIC_LOG(INFO, TAG, "startScan has failed");
    }
    else
    {
        res = CA_STATUS_OK;
    }
    (*env)->DeleteLocalRef(env, jni_obj_leScanner);

error_exit:
    (*env)->DeleteLocalRef(env, jni_obj_scanSettings);
    (*env)->DeleteLocalRef(env, jni_obj_filterList);
    return res;
}

jobject CALEClientGetUUIDObject(JNIEnv *env, const char* uuid)
{
    VERIFY_NON_NULL_RET(uuid, TAG, "uuid is null", NULL);
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    // setting UUID
    jclass jni_cid_uuid = (*env)->FindClass(env, CLASSPATH_BT_UUID);
    if (!jni_cid_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_uuid is null");
        goto error_exit;
    }

    jmethodID jni_mid_fromString = (*env)->GetStaticMethodID(env, jni_cid_uuid, "fromString",
                                                             "(Ljava/lang/String;)"
                                                             "Ljava/util/UUID;");
    if (!jni_mid_fromString)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_fromString is null");
        goto error_exit;
    }

    jstring jni_uuid = (*env)->NewStringUTF(env, uuid);
    CACheckJNIException(env);
    jobject jni_obj_uuid = (*env)->CallStaticObjectMethod(env, jni_cid_uuid, jni_mid_fromString,
                                                          jni_uuid);
    if (!jni_obj_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_uuid is null");
        goto error_exit;
    }

    return jni_obj_uuid;

error_exit:
    CACheckJNIException(env);
    return NULL;
}

CAResult_t CALEClientStopScan()
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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);
        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAResult_t ret = CA_STATUS_FAILED;

    if (g_jniIntSdk >= BLE_SCAN_API_LEVEL)
    {
        ret = CALEClientStopScanImplForV21(env, g_leScanCallback);
    }
    else
    {
        ret = CALEClientStopScanImpl(env, g_leScanCallback);
    }

    if (CA_STATUS_OK != ret)
    {
        if (CA_ADAPTER_NOT_ENABLED == ret)
        {
            OIC_LOG(DEBUG, TAG, "Adapter is disabled");
        }
        else
        {
            OIC_LOG(ERROR, TAG, "CALEClientStopScanImpl has failed");
        }
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return ret;
}

CAResult_t CALEClientStopScanImpl(JNIEnv *env, jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CALEClientStopScanImpl IN");
    VERIFY_NON_NULL(callback, TAG, "callback is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get default bt adapter class
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get start le scan method
    jmethodID jni_mid_stopLeScan = (*env)->GetMethodID(env, jni_cid_BTAdapter, "stopLeScan",
                                                       "(Landroid/bluetooth/"
                                                       "BluetoothAdapter$LeScanCallback;)V");
    if (!jni_mid_stopLeScan)
    {
        OIC_LOG(ERROR, TAG, "stopLeScan: jni_mid_stopLeScan is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get bt adapter object
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(INFO, TAG, "CALL API - stopLeScan");
    // call start le scan method
    (*env)->CallVoidMethod(env, jni_obj_BTAdapter, jni_mid_stopLeScan, callback);
    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "stopLeScan has failed");
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        return CA_STATUS_FAILED;
    }

    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
    (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
    return CA_STATUS_OK;
}

CAResult_t CALEClientStopScanImplForV21(JNIEnv *env, jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CALEClientStopScanImplForV21 IN");
    VERIFY_NON_NULL(callback, TAG, "callback is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get default bt adapter class
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_cid_BTAdapter is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    "()Landroid/bluetooth/"
                                                                    "BluetoothAdapter;");
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get bluetoothLeScanner class
    jclass jni_cid_leScanner = (*env)->FindClass(env, CLASSPATH_LE_SCANNER);
    if (!jni_cid_leScanner)
    {
        OIC_LOG(ERROR, TAG, "getState From leScanner: jni_cid_leScanner is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        return CA_STATUS_FAILED;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getBluetoothLeScanner = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                                  "getBluetoothLeScanner",
                                                                  "()Landroid/bluetooth/"
                                                                  "le/BluetoothLeScanner;");
    if (!jni_mid_getBluetoothLeScanner)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBluetoothLeScanner is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_cid_leScanner);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_cid_BTAdapter);

    // get stopScan(ScanCallback callback) method
    jmethodID jni_mid_stopScan = (*env)->GetMethodID(env, jni_cid_leScanner, "stopScan",
                                                      "(Landroid/bluetooth/le/ScanCallback;)V");
    if (!jni_mid_stopScan)
    {
        OIC_LOG(ERROR, TAG, "stopScan: jni_mid_stopScan is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_obj_BTAdapter);
        (*env)->DeleteLocalRef(env, jni_cid_leScanner);
        return CA_STATUS_FAILED;
    }
    (*env)->DeleteLocalRef(env, jni_cid_leScanner);

    // gat le scanner object
    jobject jni_obj_leScanner = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                         jni_mid_getBluetoothLeScanner);
    if (!jni_obj_leScanner)
    {
        OIC_LOG(ERROR, TAG, "getState From BTAdapter: jni_obj_leScanner is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    // call stopScan method
    OIC_LOG(INFO, TAG, "CALL API - stopScan for level 21");
    (*env)->CallVoidMethod(env, jni_obj_leScanner, jni_mid_stopScan, callback);
    if (CACheckJNIException(env))
    {
        OIC_LOG(INFO, TAG, "stopScan for level 21 has failed");
        (*env)->DeleteLocalRef(env, jni_obj_leScanner);
        return CA_STATUS_FAILED;
    }

    (*env)->DeleteLocalRef(env, jni_obj_leScanner);
    return CA_STATUS_OK;
}

CAResult_t CALEClientDirectConnect(JNIEnv *env, jobject bluetoothDevice, jboolean autoconnect)
{
    OIC_LOG(DEBUG, TAG, "CALEClientDirectConnect");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothDevice, TAG, "bluetoothDevice is null");

    oc_mutex_lock(g_threadSendMutex);

    jstring jni_address = CALEGetAddressFromBTDevice(env, bluetoothDevice);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is not available");
        oc_mutex_unlock(g_threadSendMutex);
        return CA_STATUS_FAILED;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        CACheckJNIException(env);
        oc_mutex_unlock(g_threadSendMutex);
        return CA_STATUS_FAILED;
    }

    CAResult_t res = CA_STATUS_OK;
    if(CALEIsValidState(address, CA_LE_CONNECTION_STATE,
                        STATE_DISCONNECTED,
                        g_deviceStateList,
                        g_deviceStateListMutex))
    {
        jobject newGatt = CALEClientConnect(env, bluetoothDevice, autoconnect);
        if (NULL == newGatt)
        {
            OIC_LOG(INFO, TAG, "newGatt is not available");
            res = CA_STATUS_FAILED;
        }
    }
    oc_mutex_unlock(g_threadSendMutex);

    return res;
}

jobject CALEClientConnect(JNIEnv *env, jobject bluetoothDevice, jboolean autoconnect)
{
    OIC_LOG(DEBUG, TAG, "CALEClientConnect");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(bluetoothDevice, TAG, "bluetoothDevice is null", NULL);

    // reset scan interval time after checking scanned devices
    CALERestartScanWithInterval(0, 0, BLE_SCAN_DISABLE);

    // since there is no callback related stop success
    // and scanning should be stopped before connectGatt is called.
    // it should wait a few micro seconds.
    usleep(100000);

    // get gatt object from Bluetooth Device object for closeProfileProxy(..)
    jstring jni_address = CALEClientGetLEAddressFromBTDevice(env, bluetoothDevice);
    if (jni_address)
    {
        const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is not available");
            return NULL;
        }

        // close the gatt service
        jobject gatt = CALEClientGetGattObjInList(env, address);
        if (gatt)
        {
            CAResult_t res = CALEClientCloseProfileProxy(env, gatt);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientCloseProfileProxy has failed");
                (*env)->ReleaseStringUTFChars(env, jni_address, address);
                return NULL;
            }

            // clean previous gatt object after close profile service
            res = CALEClientRemoveGattObjForAddr(env, jni_address);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientRemoveGattObjForAddr has failed");
                (*env)->ReleaseStringUTFChars(env, jni_address, address);
                return NULL;
            }
        }
        (*env)->ReleaseStringUTFChars(env, jni_address, address);
    }

    jobject newGatt = CALEClientGattConnect(env, bluetoothDevice, autoconnect);
    if (!newGatt)
    {
        OIC_LOG(DEBUG, TAG, "re-connection will be started");
        return NULL;
    }

    // add new gatt object into g_gattObjectList
    CAResult_t res = CALEClientAddGattobjToList(env, newGatt);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientAddGattobjToList has failed");
        return NULL;
    }

    return newGatt;
}

jobject CALEClientGattConnect(JNIEnv *env, jobject bluetoothDevice, jboolean autoconnect)
{
    OIC_LOG(DEBUG, TAG, "GATT CONNECT");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(bluetoothDevice, TAG, "bluetoothDevice is null", NULL);

    if (!g_leGattCallback)
    {
        OIC_LOG(INFO, TAG, "g_leGattCallback is null");
        return NULL;
    }

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return NULL;
    }

    jstring jni_address = CALEGetAddressFromBTDevice(env, bluetoothDevice);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "bleConnect: CALEGetAddressFromBTDevice is null");
        return NULL;
    }

    jobject jni_obj_connectGatt = NULL;
    jint jni_int_sdk = CALEGetBuildVersion(env);
    OIC_LOG_V(INFO, TAG, "API level is %d", jni_int_sdk);
    if (jni_int_sdk >= 23) // upper than API level 23
    {
        jmethodID jni_mid_connectGatt = CAGetJNIMethodID(env, "android/bluetooth/BluetoothDevice",
                                                         "connectGatt",
                                                         "(Landroid/content/Context;ZLandroid/"
                                                         "bluetooth/BluetoothGattCallback;I)"
                                                         "Landroid/bluetooth/BluetoothGatt;");
        if (!jni_mid_connectGatt)
        {
            OIC_LOG(ERROR, TAG, "bleConnect: jni_mid_connectGatt is null");
            return NULL;
        }

        jint jni_transport_le = CALEGetConstantsValue(env, CLASSPATH_BT_DEVICE, "TRANSPORT_LE");
        OIC_LOG_V(INFO, TAG, "CALL API - connectGatt with transport LE(%d)", jni_transport_le);
        jni_obj_connectGatt = (*env)->CallObjectMethod(env, bluetoothDevice,
                                                       jni_mid_connectGatt, NULL,
                                                       autoconnect, g_leGattCallback,
                                                       jni_transport_le);
        if (!jni_obj_connectGatt)
        {
            OIC_LOG(ERROR, TAG, "connectGatt was failed..it will be removed");
            CACheckJNIException(env);
            CALEClientRemoveDeviceInScanDeviceList(env, jni_address);
            CALEClientUpdateSendCnt(env);
            return NULL;
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "le connecting..please wait..");
        }
    }
    else // lower than API level 23
    {
#ifdef HIDDEN_API
        const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "GetStringUTFChars has failed");
            return NULL;
        }
        OIC_LOG(INFO, TAG, "CALL API - connectGatt for hidden");
        jni_obj_connectGatt = CALEClientHiddenConnectGatt(bluetoothDevice, address);

#else

        jmethodID jni_mid_connectGatt = CAGetJNIMethodID(env, "android/bluetooth/BluetoothDevice",
                                                         "connectGatt",
                                                         "(Landroid/content/Context;ZLandroid/"
                                                         "bluetooth/BluetoothGattCallback;)"
                                                         "Landroid/bluetooth/BluetoothGatt;");
        if (!jni_mid_connectGatt)
        {
            OIC_LOG(ERROR, TAG, "bleConnect: jni_mid_connectGatt is null");
            return NULL;
        }

        OIC_LOG(INFO, TAG, "CALL API - connectGatt");
        jni_obj_connectGatt = (*env)->CallObjectMethod(env, bluetoothDevice,
                                                               jni_mid_connectGatt,
                                                               NULL,
                                                               autoconnect, g_leGattCallback);
#endif
        if (!jni_obj_connectGatt)
        {
            OIC_LOG(ERROR, TAG, "connectGatt was failed..it will be removed");
            CACheckJNIException(env);
            CALEClientRemoveDeviceInScanDeviceList(env, jni_address);
            CALEClientUpdateSendCnt(env);
            return NULL;
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "le connecting..please wait..");
        }

    }

    return jni_obj_connectGatt;
}

bool CALEClientIsConnected(const char* address)
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

CAResult_t CALEClientCloseProfileProxy(JNIEnv *env, jobject gatt)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientCloseProfileProxy");

    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(gatt, TAG, "gatt is null");

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

    // get closeProfileProxy method
    jmethodID jni_mid_closeProfileProxy = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                              "closeProfileProxy",
                                                              "(ILandroid/bluetooth/"
                                                              "BluetoothProfile;)V");
    if (!jni_mid_closeProfileProxy)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_closeProfileProxy is null");
        goto error_exit;
    }

    jclass jni_cid_BTProfile = (*env)->FindClass(env, CLASSPATH_BT_PROFILE);
    if (!jni_cid_BTProfile)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTProfile is null");
        goto error_exit;
    }

    // GATT - Constant value : 7 (0x00000007)
    jfieldID id_gatt = (*env)->GetStaticFieldID(env, jni_cid_BTProfile,
                                                "GATT", "I");
    if (!id_gatt)
    {
        OIC_LOG(ERROR, TAG, "id_gatt is null");
        goto error_exit;
    }

    jint jni_gatt = (*env)->GetStaticIntField(env, jni_cid_BTProfile, id_gatt);
    CACheckJNIException(env);

    OIC_LOG(DEBUG, TAG, "CALL API - close the connection of the profile proxy to the Service");
    (*env)->CallVoidMethod(env, jni_obj_BTAdapter, jni_mid_closeProfileProxy, jni_gatt, gatt);
    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "closeProfileProxy has failed");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEClientCloseProfileProxy");
    return CA_STATUS_OK;

error_exit:
    CACheckJNIException(env);
    return CA_STATUS_FAILED;
}


CAResult_t CALEClientDisconnect(JNIEnv *env, jobject bluetoothGatt)
{
    OIC_LOG(DEBUG, TAG, "GATT DISCONNECT");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");

    // get BluetoothGatt method
    OIC_LOG(DEBUG, TAG, "get gatt disconnect method");
    jmethodID jni_mid_disconnectGatt  = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                         "disconnect", "()V");
    if (!jni_mid_disconnectGatt)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_disconnectGatt is null");
        return CA_STATUS_FAILED;
    }

    // call disconnect gatt method
    OIC_LOG(INFO, TAG, "CALL API - disconnect");
    (*env)->CallVoidMethod(env, bluetoothGatt, jni_mid_disconnectGatt);
    if (CACheckJNIException(env))
    {
        OIC_LOG(ERROR, TAG, "disconnect has failed");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "disconnecting Gatt...");

    return CA_STATUS_OK;
}

CAResult_t CALEClientDisconnectAll(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CALEClientDisconnectAll");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    OIC_LOG_V(DEBUG, TAG, "list length : %d", length);
    for (uint32_t index = 0; index < length; index++)
    {
        OIC_LOG(DEBUG, TAG, "start CALEClientDisconnectAll");
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }
        CAResult_t res = CALEClientDisconnect(env, jarrayObj);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientDisconnect has failed");
            continue;
        }
    }

    return CA_STATUS_OK;
}

CAResult_t CALEClientDisconnectforAddress(JNIEnv *env, jstring remote_address)
{
    OIC_LOG(DEBUG, TAG, "IN-CALEClientDisconnectforAddress");
    VERIFY_NON_NULL(env, TAG, "env is null");

    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        return CA_STATUS_OK;
    }

    char* address = (char*)(*env)->GetStringUTFChars(env, remote_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }

        jstring jni_setAddress = CALEClientGetAddressFromGattObj(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            (*env)->ReleaseStringUTFChars(env, remote_address, address);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            CACheckJNIException(env);
            (*env)->ReleaseStringUTFChars(env, remote_address, address);
            return CA_STATUS_FAILED;
        }

        OIC_LOG_V(DEBUG, TAG, "target address : %s, set address : %s", address, setAddress);
        if (!strcasecmp(address, setAddress))
        {
            CAResult_t res = CALEClientDisconnect(env, jarrayObj);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientDisconnect has failed");
                (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
                (*env)->ReleaseStringUTFChars(env, remote_address, address);
                return CA_STATUS_FAILED;
            }
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->ReleaseStringUTFChars(env, remote_address, address);
            return CA_STATUS_OK;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
    }
    (*env)->ReleaseStringUTFChars(env, remote_address, address);

    OIC_LOG(DEBUG, TAG, "OUT-CALEClientDisconnectforAddress");
    return CA_STATUS_OK;
}

CAResult_t CALEClientRequestMTU(JNIEnv *env, jobject bluetoothGatt, jint size)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get BluetoothGatt.requestMtu method
    OIC_LOG(DEBUG, TAG, "get BluetoothGatt.requestMtu method");
    jmethodID jni_mid_requestMtu = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                    "requestMtu", "(I)Z");
    if (!jni_mid_requestMtu)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_requestMtu is null");
        return CA_STATUS_FAILED;
    }

    // call requestMtu
    OIC_LOG(INFO, TAG, "CALL API - requestMtu");
    jboolean ret = (*env)->CallBooleanMethod(env, bluetoothGatt, jni_mid_requestMtu, size);
    if (!ret)
    {
        OIC_LOG(ERROR, TAG, "requestMtu has failed");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CALEClientDiscoverServices(JNIEnv *env, jobject bluetoothGatt)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get BluetoothGatt.discoverServices method
    OIC_LOG(DEBUG, TAG, "get BluetoothGatt.discoverServices method");
    jmethodID jni_mid_discoverServices = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                          "discoverServices", "()Z");
    if (!jni_mid_discoverServices)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_discoverServices is null");
        return CA_STATUS_FAILED;
    }

    // call disconnect gatt method
    OIC_LOG(INFO, TAG, "CALL API - discoverServices");
    jboolean ret = (*env)->CallBooleanMethod(env, bluetoothGatt, jni_mid_discoverServices);
    if (!ret)
    {
        OIC_LOG(ERROR, TAG, "discoverServices has not been started");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

static void CALEWriteCharacteristicThread(void* object)
{
    VERIFY_NON_NULL_VOID(object, TAG, "object is null");

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return;
        }
        isAttached = true;
    }

    jobject gatt = (jobject)object;
    CAResult_t ret = CALESetValueAndWriteCharacteristic(env, gatt);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALESetValueAndWriteCharacteristic has failed");
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }
}

CAResult_t CALESetValueAndWriteCharacteristic(JNIEnv* env, jobject gatt)
{
    OIC_LOG(DEBUG, TAG, "CALESetValueAndWriteCharacteristic");

    VERIFY_NON_NULL(gatt, TAG, "gatt is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
    if (!jni_address)
    {
        CALEClientSendFinish(env, gatt);
        return CA_STATUS_FAILED;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        CACheckJNIException(env);
        CALEClientSendFinish(env, gatt);
        return CA_STATUS_FAILED;
    }

    oc_mutex_lock(g_threadSendStateMutex);

    if (CALEIsValidState(address, CA_LE_SEND_STATE, STATE_SENDING,
                         g_deviceStateList,
                         g_deviceStateListMutex))
    {
        OIC_LOG(INFO, TAG, "current state is SENDING");
        (*env)->ReleaseStringUTFChars(env, jni_address, address);
        oc_mutex_unlock(g_threadSendStateMutex);
        return CA_STATUS_OK;
    }

    if (CA_STATUS_OK != CALEUpdateDeviceState(address, CA_LE_SEND_STATE,
                                              STATE_SENDING,
                                              g_deviceStateList,
                                              g_deviceStateListMutex))
    {
        OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
        (*env)->ReleaseStringUTFChars(env, jni_address, address);
        CALEClientSendFinish(env, gatt);
        oc_mutex_unlock(g_threadSendStateMutex);
        return CA_STATUS_FAILED;
    }

    (*env)->ReleaseStringUTFChars(env, jni_address, address);

    oc_mutex_unlock(g_threadSendStateMutex);

    jbyteArray sendData = NULL;
    oc_mutex_lock(g_setValueMutex);
    if (g_sendBuffer)
    {
        OIC_LOG(INFO, TAG, "alloc local reference for data");
        sendData = (jbyteArray)(*env)->NewLocalRef(env, g_sendBuffer);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "send Buffer is empty");
        oc_mutex_unlock(g_setValueMutex);
        return CA_STATUS_FAILED;
    }
    oc_mutex_unlock(g_setValueMutex);

    // send data
    jobject jni_obj_character = CALEClientCreateGattCharacteristic(env, gatt, sendData);
    if (!jni_obj_character)
    {
        if (sendData)
        {
            (*env)->DeleteLocalRef(env, sendData);
        }
        CALEClientSendFinish(env, gatt);
        return CA_STATUS_FAILED;
    }

    if (sendData)
    {
        (*env)->DeleteLocalRef(env, sendData);
    }

    CAResult_t ret = CALEClientWriteCharacteristicImpl(env, gatt, jni_obj_character);
    if (CA_STATUS_OK != ret)
    {
        CALEClientSendFinish(env, gatt);
        return CA_STATUS_FAILED;
    }

    // wait for callback for write Characteristic with success to sent data
    OIC_LOG_V(DEBUG, TAG, "callback flag is %d", g_isSignalSetFlag);
    oc_mutex_lock(g_threadWriteCharacteristicMutex);
    if (!g_isSignalSetFlag)
    {
        OIC_LOG(DEBUG, TAG, "wait for callback to notify writeCharacteristic is success");
        if (OC_WAIT_SUCCESS != oc_cond_wait_for(g_threadWriteCharacteristicCond,
                                  g_threadWriteCharacteristicMutex,
                                  WAIT_TIME_WRITE_CHARACTERISTIC))
        {
            OIC_LOG(ERROR, TAG, "there is no response. write has failed");
            g_isSignalSetFlag = false;
            oc_mutex_unlock(g_threadWriteCharacteristicMutex);
            return CA_STATUS_FAILED;
        }
    }
    // reset flag set by writeCharacteristic Callback
    g_isSignalSetFlag = false;
    oc_mutex_unlock(g_threadWriteCharacteristicMutex);

    CALEClientUpdateSendCnt(env);

    OIC_LOG(INFO, TAG, "writeCharacteristic success!!");
    return CA_STATUS_OK;
}

CAResult_t CALEClientWriteCharacteristic(JNIEnv *env, jobject gatt)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientWriteCharacteristic");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(gatt, TAG, "gatt is null");

    jobject gattParam = (*env)->NewGlobalRef(env, gatt);
    CACheckJNIException(env);
    if (CA_STATUS_OK != ca_thread_pool_add_task(g_threadPoolHandle, CALEWriteCharacteristicThread,
                                                (void*)gattParam, NULL))
    {
        OIC_LOG(ERROR, TAG, "Failed to create read thread!");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEClientWriteCharacteristic");
    return CA_STATUS_OK;
}

CAResult_t CALEClientWriteCharacteristicImpl(JNIEnv *env, jobject bluetoothGatt,
                                             jobject gattCharacteristic)
{
    OIC_LOG(DEBUG, TAG, "WRITE GATT CHARACTERISTIC");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");
    VERIFY_NON_NULL(gattCharacteristic, TAG, "gattCharacteristic is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_STATUS_FAILED;
    }

    // get BluetoothGatt.write characteristic method
    OIC_LOG(DEBUG, TAG, "write characteristic method");
    jmethodID jni_mid_writeCharacteristic = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                             "writeCharacteristic",
                                                             "(Landroid/bluetooth/"
                                                             "BluetoothGattCharacteristic;)Z");
    if (!jni_mid_writeCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_writeCharacteristic is null");
        return CA_STATUS_FAILED;
    }

    // call disconnect gatt method
    OIC_LOG(INFO, TAG, "CALL API - writeCharacteristic");
    jboolean ret = (jboolean)(*env)->CallBooleanMethod(env, bluetoothGatt,
                                                       jni_mid_writeCharacteristic,
                                                       gattCharacteristic);
    if (ret)
    {
        OIC_LOG(DEBUG, TAG, "writeCharacteristic is called successfully");
    }
    else
    {
        CACheckJNIException(env);
        OIC_LOG(ERROR, TAG, "writeCharacteristic has failed");
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CALEClientReadCharacteristic(JNIEnv *env, jobject bluetoothGatt)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_STATUS_FAILED;
    }

    jstring jni_uuid = (*env)->NewStringUTF(env, OIC_GATT_CHARACTERISTIC_RESPONSE_UUID);
    if (!jni_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_uuid is null");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    jobject jni_obj_GattCharacteristic = CALEClientGetGattService(env, bluetoothGatt, jni_uuid);
    if (!jni_obj_GattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_GattCharacteristic is null");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "read characteristic method");
    jmethodID jni_mid_readCharacteristic = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                            "readCharacteristic",
                                                            "(Landroid/bluetooth/"
                                                            "BluetoothGattCharacteristic;)Z");
    if (!jni_mid_readCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_readCharacteristic is null");
        return CA_STATUS_FAILED;
    }

    // call disconnect gatt method
    OIC_LOG(INFO, TAG, "CALL API - readCharacteristic");
    jboolean ret = (*env)->CallBooleanMethod(env, bluetoothGatt, jni_mid_readCharacteristic,
                                             jni_obj_GattCharacteristic);
    if (ret)
    {
        OIC_LOG(DEBUG, TAG, "readCharacteristic success");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "readCharacteristic has failed");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CALEClientSetCharacteristicNotification(JNIEnv *env, jobject bluetoothGatt,
                                                   jobject characteristic)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");
    VERIFY_NON_NULL(characteristic, TAG, "characteristic is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get BluetoothGatt.setCharacteristicNotification method
    OIC_LOG(DEBUG, TAG, "CALEClientSetCharacteristicNotification");
    jmethodID jni_mid_setNotification = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                         "setCharacteristicNotification",
                                                         "(Landroid/bluetooth/"
                                                         "BluetoothGattCharacteristic;Z)Z");
    if (!jni_mid_setNotification)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getService is null");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(INFO, TAG, "CALL API - setCharacteristicNotification");
    jboolean ret = (*env)->CallBooleanMethod(env, bluetoothGatt, jni_mid_setNotification,
                                             characteristic, JNI_TRUE);
    if (JNI_TRUE == ret)
    {
        OIC_LOG(DEBUG, TAG, "setCharacteristicNotification success");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "setCharacteristicNotification has failed");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

jobject CALEClientGetGattService(JNIEnv *env, jobject bluetoothGatt, jstring characterUUID)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(bluetoothGatt, TAG, "bluetoothGatt is null", NULL);
    VERIFY_NON_NULL_RET(characterUUID, TAG, "characterUUID is null", NULL);

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return NULL;
    }

    // get BluetoothGatt.getService method
    OIC_LOG(DEBUG, TAG, "BluetoothGatt.getService");
    jmethodID jni_mid_getService = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                    "getService",
                                                    "(Ljava/util/UUID;)Landroid/bluetooth/"
                                                    "BluetoothGattService;");
    if (!jni_mid_getService)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getService is null");
        return NULL;
    }

    jobject jni_obj_service_uuid = CALEClientGetUUIDObject(env, OIC_GATT_SERVICE_UUID);
    if (!jni_obj_service_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_service_uuid is null");
        return NULL;
    }

    // get bluetooth gatt service
    OIC_LOG(DEBUG, TAG, "request to get service");
    jobject jni_obj_gattService = (*env)->CallObjectMethod(env, bluetoothGatt, jni_mid_getService,
                                                           jni_obj_service_uuid);
    if (!jni_obj_gattService)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_gattService is null");
        CACheckJNIException(env);
        return NULL;
    }

    // get bluetooth gatt service method
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

    const char* uuid = (*env)->GetStringUTFChars(env, characterUUID, NULL);
    if (!uuid)
    {
        OIC_LOG(ERROR, TAG, "uuid is null");
        CACheckJNIException(env);
        return NULL;
    }

    jobject jni_obj_tx_uuid = CALEClientGetUUIDObject(env, uuid);
    if (!jni_obj_tx_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_tx_uuid is null");
        (*env)->ReleaseStringUTFChars(env, characterUUID, uuid);
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "CALL API getCharacteristic");
    jobject jni_obj_GattCharacteristic = (*env)->CallObjectMethod(env, jni_obj_gattService,
                                                                  jni_mid_getCharacteristic,
                                                                  jni_obj_tx_uuid);
    if (!jni_obj_GattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "getCharacteristic has failed");
        CACheckJNIException(env);
        return NULL;
    }

    (*env)->ReleaseStringUTFChars(env, characterUUID, uuid);
    return jni_obj_GattCharacteristic;
}

jobject CALEClientCreateGattCharacteristic(JNIEnv *env, jobject bluetoothGatt, jbyteArray data)
{
    OIC_LOG(DEBUG, TAG, "CALEClientCreateGattCharacteristic");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(bluetoothGatt, TAG, "bluetoothGatt is null", NULL);
    VERIFY_NON_NULL_RET(data, TAG, "data is null", NULL);

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return NULL;
    }

    jstring jni_uuid = (*env)->NewStringUTF(env, OIC_GATT_CHARACTERISTIC_REQUEST_UUID);
    if (!jni_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_uuid is null");
        goto error_exit;
    }

    jobject jni_obj_GattCharacteristic = CALEClientGetGattService(env, bluetoothGatt, jni_uuid);
    if (!jni_obj_GattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_GattCharacteristic is null");
        return NULL;
    }

    jclass jni_cid_BTGattCharacteristic = (*env)->FindClass(env, "android/bluetooth"
                                                            "/BluetoothGattCharacteristic");
    if (!jni_cid_BTGattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTGattCharacteristic is null");
        goto error_exit;
    }

    OIC_LOG(DEBUG, TAG, "set value in Characteristic");
    jmethodID jni_mid_setValue = (*env)->GetMethodID(env, jni_cid_BTGattCharacteristic, "setValue",
                                                     "([B)Z");
    if (!jni_mid_setValue)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setValue is null");
        goto error_exit;
    }

    OIC_LOG(DEBUG, TAG, "CALL API - setValue");
    jboolean ret = (*env)->CallBooleanMethod(env, jni_obj_GattCharacteristic, jni_mid_setValue,
                                             data);
    if (JNI_TRUE == ret)
    {
        OIC_LOG(DEBUG, TAG, "the locally stored value has been set");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "the locally stored value hasn't been set");
        goto error_exit;
    }

    if (!g_setHighQoS)
    {
        OIC_LOG(DEBUG, TAG, "setWriteType with WRITE_TYPE_NO_RESPONSE");
        // set Write Type
        jmethodID jni_mid_setWriteType = (*env)->GetMethodID(env, jni_cid_BTGattCharacteristic,
                                                             "setWriteType", "(I)V");
        if (!jni_mid_setWriteType)
        {
            OIC_LOG(ERROR, TAG, "jni_mid_setWriteType is null");
            goto error_exit;
        }

        jfieldID jni_fid_no_response = (*env)->GetStaticFieldID(env, jni_cid_BTGattCharacteristic,
                                                                "WRITE_TYPE_NO_RESPONSE", "I");
        if (!jni_fid_no_response)
        {
            OIC_LOG(ERROR, TAG, "jni_fid_no_response is not available");
            goto error_exit;
        }

        jint jni_int_val = (*env)->GetStaticIntField(env, jni_cid_BTGattCharacteristic,
                                                     jni_fid_no_response);
        CACheckJNIException(env);

        (*env)->CallVoidMethod(env, jni_obj_GattCharacteristic, jni_mid_setWriteType, jni_int_val);
        if (CACheckJNIException(env))
        {
            OIC_LOG(ERROR, TAG, "setWriteType has failed");
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "It will run with response property");
    }

    return jni_obj_GattCharacteristic;

error_exit:
    CACheckJNIException(env);
    return NULL;
}

jbyteArray CALEClientGetValueFromCharacteristic(JNIEnv *env, jobject characteristic)
{
    VERIFY_NON_NULL_RET(characteristic, TAG, "characteristic is null", NULL);
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return NULL;
    }

    jmethodID jni_mid_getValue  = CAGetJNIMethodID(env, "android/bluetooth/"
                                                   "BluetoothGattCharacteristic",
                                                   "getValue", "()[B");
    if (!jni_mid_getValue)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getValue is null");
        return NULL;
    }

    jbyteArray jni_obj_data_array = (*env)->CallObjectMethod(env, characteristic,
                                                             jni_mid_getValue);
    CACheckJNIException(env);
    return jni_obj_data_array;
}

CAResult_t CALEClientCreateUUIDList()
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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    // create new object array
    jclass jni_cid_uuid_list = (*env)->FindClass(env, CLASSPATH_BT_UUID);
    if (!jni_cid_uuid_list)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_uuid_list is null");
        CACheckJNIException(env);
        goto error_exit;
    }

    jobjectArray jni_obj_uuid_list = (jobjectArray)(*env)->NewObjectArray(env, 1,
                                                                          jni_cid_uuid_list, NULL);
    if (!jni_obj_uuid_list)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_uuid_list is null");
        CACheckJNIException(env);
        goto error_exit;
    }

    // make uuid list
    jobject jni_obj_uuid = CALEClientGetUUIDObject(env, OIC_GATT_SERVICE_UUID);
    if (!jni_obj_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_uuid is null");
        goto error_exit;
    }
    (*env)->SetObjectArrayElement(env, jni_obj_uuid_list, 0, jni_obj_uuid);

    g_uuidList = (jobjectArray)(*env)->NewGlobalRef(env, jni_obj_uuid_list);
    CACheckJNIException(env);

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;

    // error label.
error_exit:

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }
    return CA_STATUS_FAILED;
}

CAResult_t CALEClientSetUUIDToDescriptor(JNIEnv *env, jobject bluetoothGatt,
                                         jobject characteristic)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");
    VERIFY_NON_NULL(characteristic, TAG, "characteristic is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    OIC_LOG(DEBUG, TAG, "CALEClientSetUUIDToDescriptor");
    jmethodID jni_mid_getDescriptor  = CAGetJNIMethodID(env, "android/bluetooth/"
                                                        "BluetoothGattCharacteristic",
                                                        "getDescriptor",
                                                        "(Ljava/util/UUID;)Landroid/bluetooth/"
                                                        "BluetoothGattDescriptor;");
    if (!jni_mid_getDescriptor)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDescriptor is null");
        return CA_STATUS_FAILED;
    }

    jobject jni_obj_cc_uuid = CALEClientGetUUIDObject(env, OIC_GATT_CHARACTERISTIC_CONFIG_UUID);
    if (!jni_obj_cc_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_cc_uuid is null");
    }

    OIC_LOG(DEBUG, TAG, "request to get descriptor");
    jobject jni_obj_descriptor = (*env)->CallObjectMethod(env, characteristic,
                                                          jni_mid_getDescriptor, jni_obj_cc_uuid);
    if (!jni_obj_descriptor)
    {
        OIC_LOG(INFO, TAG, "jni_obj_descriptor is null");
        goto error_exit;
    }

    OIC_LOG(DEBUG, TAG, "set value in descriptor");
    jclass jni_cid_descriptor = (*env)->FindClass(env,
                                                  "android/bluetooth/BluetoothGattDescriptor");
    if (!jni_cid_descriptor)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_descriptor is null");
        goto error_exit;
    }

    jmethodID jni_mid_setValue = (*env)->GetMethodID(env, jni_cid_descriptor, "setValue", "([B)Z");
    if (!jni_mid_setValue)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setValue is null");
        goto error_exit;
    }

    jfieldID jni_fid_NotiValue = NULL;
    if (g_setHighQoS)
    {
        OIC_LOG(DEBUG, TAG, "get ENABLE_INDICATION_VALUE");
        jni_fid_NotiValue = (*env)->GetStaticFieldID(env, jni_cid_descriptor,
                                                     "ENABLE_INDICATION_VALUE", "[B");
        if (!jni_fid_NotiValue)
        {
            OIC_LOG(ERROR, TAG, "jni_fid_NotiValue is null");
            goto error_exit;
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "get ENABLE_NOTIFICATION_VALUE");
        jni_fid_NotiValue = (*env)->GetStaticFieldID(env, jni_cid_descriptor,
                                                     "ENABLE_NOTIFICATION_VALUE", "[B");
        if (!jni_fid_NotiValue)
        {
            OIC_LOG(ERROR, TAG, "jni_fid_NotiValue is null");
            goto error_exit;
        }
    }

    jboolean jni_setvalue = (*env)->CallBooleanMethod(
            env, jni_obj_descriptor, jni_mid_setValue,
            (jbyteArray)(*env)->GetStaticObjectField(env, jni_cid_descriptor, jni_fid_NotiValue));
    if (jni_setvalue)
    {
        OIC_LOG(DEBUG, TAG, "setValue success");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "setValue has failed");
        goto error_exit;
    }

    jmethodID jni_mid_writeDescriptor  = CAGetJNIMethodID(env, "android/bluetooth/BluetoothGatt",
                                                          "writeDescriptor",
                                                          "(Landroid/bluetooth/"
                                                          "BluetoothGattDescriptor;)Z");
    if (!jni_mid_writeDescriptor)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_writeDescriptor is null");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(INFO, TAG, "CALL API - writeDescriptor");
    jboolean jni_ret = (*env)->CallBooleanMethod(env, bluetoothGatt, jni_mid_writeDescriptor,
                                                 jni_obj_descriptor);
    if (jni_ret)
    {
        OIC_LOG(DEBUG, TAG, "writeDescriptor success");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "writeDescriptor has failed");
        goto error_exit;
    }

    return CA_STATUS_OK;

error_exit:
    CACheckJNIException(env);
    return CA_STATUS_FAILED;
}

void CALEClientCreateScanDeviceList(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CALEClientCreateScanDeviceList");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");

    oc_mutex_lock(g_deviceListMutex);
    // create new object array
    if (g_deviceList == NULL)
    {
        OIC_LOG(DEBUG, TAG, "Create device list");

        g_deviceList = u_arraylist_create();
    }
    oc_mutex_unlock(g_deviceListMutex);
}

CAResult_t CALEClientAddScanDeviceToList(JNIEnv *env, jobject device)
{
    VERIFY_NON_NULL(device, TAG, "device is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_deviceListMutex);

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "gdevice_list is null");
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    jstring jni_remoteAddress = CALEGetAddressFromBTDevice(env, device);
    if (!jni_remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_remoteAddress is null");
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    const char* remoteAddress = (*env)->GetStringUTFChars(env, jni_remoteAddress, NULL);
    if (!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "remoteAddress is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_remoteAddress);
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    if (!CALEClientIsDeviceInScanDeviceList(env, remoteAddress))
    {
        jobject gdevice = (*env)->NewGlobalRef(env, device);
        CACheckJNIException(env);
        u_arraylist_add(g_deviceList, gdevice);
        oc_cond_signal(g_deviceDescCond);
        OIC_LOG_V(DEBUG, TAG, "Added a new BT Device in deviceList [%s]", remoteAddress);
    }
    (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, remoteAddress);
    (*env)->DeleteLocalRef(env, jni_remoteAddress);

    oc_mutex_unlock(g_deviceListMutex);

    return CA_STATUS_OK;
}

bool CALEClientIsDeviceInScanDeviceList(JNIEnv *env, const char* remoteAddress)
{
    VERIFY_NON_NULL_RET(env, TAG, "env is null", true);
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", true);

    if (!g_deviceList)
    {
        OIC_LOG(DEBUG, TAG, "g_deviceList is null");
        return true;
    }

    uint32_t length = u_arraylist_length(g_deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_deviceList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            return true;
        }

        jstring jni_setAddress = CALEGetAddressFromBTDevice(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            return true;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            CACheckJNIException(env);
            (*env)->DeleteLocalRef(env, jni_setAddress);
            return true;
        }

        if (!strcasecmp(remoteAddress, setAddress))
        {
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->DeleteLocalRef(env, jni_setAddress);
            return true;
        }

        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->DeleteLocalRef(env, jni_setAddress);
    }
    return false;
}

CAResult_t CALEClientRemoveAllScanDevices(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveAllScanDevices");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_deviceListMutex);

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceList is null");
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_deviceList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }
        (*env)->DeleteGlobalRef(env, jarrayObj);
        jarrayObj = NULL;
    }

    OICFree(g_deviceList);
    g_deviceList = NULL;

    oc_mutex_unlock(g_deviceListMutex);
    return CA_STATUS_OK;
}

CAResult_t CALEClientRemoveDeviceInScanDeviceList(JNIEnv *env, jstring address)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveDeviceInScanDeviceList");
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_deviceListMutex);

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceList is null");
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_deviceList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            oc_mutex_unlock(g_deviceListMutex);
            return CA_STATUS_FAILED;
        }

        jstring jni_setAddress = CALEGetAddressFromBTDevice(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            oc_mutex_unlock(g_deviceListMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            CACheckJNIException(env);
            oc_mutex_unlock(g_deviceListMutex);
            return CA_STATUS_FAILED;
        }

        const char* remoteAddress = (*env)->GetStringUTFChars(env, address, NULL);
        if (!remoteAddress)
        {
            OIC_LOG(ERROR, TAG, "remoteAddress is null");
            CACheckJNIException(env);
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            oc_mutex_unlock(g_deviceListMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcasecmp(setAddress, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "remove object : %s", remoteAddress);
            (*env)->DeleteGlobalRef(env, jarrayObj);
            jarrayObj = NULL;
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->ReleaseStringUTFChars(env, address, remoteAddress);

            if (NULL == u_arraylist_remove(g_deviceList, index))
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                oc_mutex_unlock(g_deviceListMutex);
                return CA_STATUS_FAILED;
            }
            oc_mutex_unlock(g_deviceListMutex);
            return CA_STATUS_OK;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->ReleaseStringUTFChars(env, address, remoteAddress);
    }

    oc_mutex_unlock(g_deviceListMutex);
    OIC_LOG(DEBUG, TAG, "There are no object in the device list");

    return CA_STATUS_OK;
}

/**
 * Gatt Object List
 */

CAResult_t CALEClientAddGattobjToList(JNIEnv *env, jobject gatt)
{
    OIC_LOG(DEBUG, TAG, "CALEClientAddGattobjToList");
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(gatt, TAG, "gatt is null");

    oc_mutex_lock(g_gattObjectMutex);

    if (!g_gattObjectList)
    {
        OIC_LOG(ERROR, TAG, "g_gattObjectList is not available");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_FAILED;
    }

    jstring jni_remoteAddress = CALEClientGetAddressFromGattObj(env, gatt);
    if (!jni_remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_remoteAddress is null");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_FAILED;
    }

    const char* remoteAddress = (*env)->GetStringUTFChars(env, jni_remoteAddress, NULL);
    if (!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "remoteAddress is null");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_remoteAddress);
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, TAG, "remote address : %s", remoteAddress);
    if (!CALEClientIsGattObjInList(env, remoteAddress))
    {
        jobject newGatt = (*env)->NewGlobalRef(env, gatt);
        u_arraylist_add(g_gattObjectList, newGatt);
        OIC_LOG(INFO, TAG, "added a newGatt object to gattObjectList");
    }

    (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, remoteAddress);
    (*env)->DeleteLocalRef(env, jni_remoteAddress);
    oc_mutex_unlock(g_gattObjectMutex);
    return CA_STATUS_OK;
}

bool CALEClientIsGattObjInList(JNIEnv *env, const char* remoteAddress)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", true);

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            return true;
        }

        jstring jni_setAddress = CALEClientGetAddressFromGattObj(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            return true;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            CACheckJNIException(env);
            (*env)->DeleteLocalRef(env, jni_setAddress);
            return true;
        }

        if (!strcasecmp(remoteAddress, setAddress))
        {
            OIC_LOG(DEBUG, TAG, "the device is already set");
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->DeleteLocalRef(env, jni_setAddress);
            return true;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->DeleteLocalRef(env, jni_setAddress);
    }

    OIC_LOG(DEBUG, TAG, "There are no GATT object in list. it can be added");
    return false;
}

jobject CALEClientGetGattObjInList(JNIEnv *env, const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CALEClientGetGattObjInList");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", NULL);

    oc_mutex_lock(g_gattObjectMutex);
    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return NULL;
        }

        jstring jni_setAddress = CALEClientGetAddressFromGattObj(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return NULL;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            CACheckJNIException(env);
            (*env)->DeleteLocalRef(env, jni_setAddress);
            oc_mutex_unlock(g_gattObjectMutex);
            return NULL;
        }

        if (!strcasecmp(remoteAddress, setAddress))
        {
            OIC_LOG(DEBUG, TAG, "the device is already set");
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            oc_mutex_unlock(g_gattObjectMutex);
            return jarrayObj;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->DeleteLocalRef(env, jni_setAddress);
    }

    oc_mutex_unlock(g_gattObjectMutex);
    OIC_LOG(DEBUG, TAG, "There are no the gatt object in list");
    return NULL;
}

CAResult_t CALEClientRemoveAllGattObjs(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveAllGattObjs");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_gattObjectMutex);
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }
        (*env)->DeleteGlobalRef(env, jarrayObj);
        jarrayObj = NULL;
    }

    OICFree(g_gattObjectList);
    g_gattObjectList = NULL;
    OIC_LOG(INFO, TAG, "g_gattObjectList is removed");
    oc_mutex_unlock(g_gattObjectMutex);
    return CA_STATUS_OK;
}

CAResult_t CALEClientRemoveGattObj(JNIEnv *env, jobject gatt)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveGattObj");
    VERIFY_NON_NULL(gatt, TAG, "gatt is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_gattObjectMutex);
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        jstring jni_setAddress = CALEClientGetAddressFromGattObj(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            CACheckJNIException(env);
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        jstring jni_remoteAddress = CALEClientGetAddressFromGattObj(env, gatt);
        if (!jni_remoteAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_remoteAddress is null");
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        const char* remoteAddress = (*env)->GetStringUTFChars(env, jni_remoteAddress, NULL);
        if (!remoteAddress)
        {
            OIC_LOG(ERROR, TAG, "remoteAddress is null");
            CACheckJNIException(env);
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcasecmp(setAddress, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "remove object : %s", remoteAddress);
            (*env)->DeleteGlobalRef(env, jarrayObj);
            jarrayObj = NULL;
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, remoteAddress);

            if (NULL == u_arraylist_remove(g_gattObjectList, index))
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                oc_mutex_unlock(g_gattObjectMutex);
                return CA_STATUS_FAILED;
            }
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_OK;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->ReleaseStringUTFChars(env, jni_remoteAddress, remoteAddress);
    }

    oc_mutex_unlock(g_gattObjectMutex);
    OIC_LOG(DEBUG, TAG, "there are no target object");
    return CA_STATUS_OK;
}

CAResult_t CALEClientRemoveGattObjForAddr(JNIEnv *env, jstring addr)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveGattObjForAddr");
    VERIFY_NON_NULL(addr, TAG, "addr is null");
    VERIFY_NON_NULL(env, TAG, "env is null");

    oc_mutex_lock(g_gattObjectMutex);
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        jstring jni_setAddress = CALEClientGetAddressFromGattObj(env, jarrayObj);
        if (!jni_setAddress)
        {
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, jni_setAddress, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            CACheckJNIException(env);
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        const char* remoteAddress = (*env)->GetStringUTFChars(env, addr, NULL);
        if (!remoteAddress)
        {
            OIC_LOG(ERROR, TAG, "remoteAddress is null");
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcasecmp(setAddress, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "remove object : %s", remoteAddress);
            (*env)->DeleteGlobalRef(env, jarrayObj);
            jarrayObj = NULL;
            (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
            (*env)->ReleaseStringUTFChars(env, addr, remoteAddress);
            if (NULL == u_arraylist_remove(g_gattObjectList, index))
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                oc_mutex_unlock(g_gattObjectMutex);
                return CA_STATUS_FAILED;
            }
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_OK;
        }
        (*env)->ReleaseStringUTFChars(env, jni_setAddress, setAddress);
        (*env)->ReleaseStringUTFChars(env, addr, remoteAddress);
    }

    oc_mutex_unlock(g_gattObjectMutex);
    OIC_LOG(DEBUG, TAG, "there are no target object");
    return CA_STATUS_FAILED;
}

jstring CALEClientGetLEAddressFromBTDevice(JNIEnv *env, jobject bluetoothDevice)
{
    VERIFY_NON_NULL_RET(env, TAG, "env", NULL);
    VERIFY_NON_NULL_RET(bluetoothDevice, TAG, "bluetoothDevice", NULL);

    // get Bluetooth Address
    jstring jni_btTargetAddress = CALEGetAddressFromBTDevice(env, bluetoothDevice);
    if (!jni_btTargetAddress)
    {
        OIC_LOG(ERROR, TAG, "CALEGetAddressFromBTDevice has failed");
        return NULL;
    }

    const char* targetAddress = (*env)->GetStringUTFChars(env, jni_btTargetAddress, NULL);
    if (!targetAddress)
    {
        OIC_LOG(ERROR, TAG, "targetAddress is not available");
        CACheckJNIException(env);
        return NULL;
    }

    // get method ID of getDevice()
    jmethodID jni_mid_getDevice = CAGetJNIMethodID(env, CLASSPATH_BT_GATT,
                                                   "getDevice", METHODID_BT_DEVICE);
    if (!jni_mid_getDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDevice is null");
        (*env)->ReleaseStringUTFChars(env, jni_btTargetAddress, targetAddress);
        return NULL;
    }

    oc_mutex_lock(g_gattObjectMutex);

    size_t length = u_arraylist_length(g_gattObjectList);
    OIC_LOG_V(DEBUG, TAG, "length of gattObjectList : %d", length);
    OIC_LOG_V(DEBUG, TAG, "targetAddress : %s", targetAddress);

    for (size_t index = 0; index < length; index++)
    {
        jobject jarrayObj = (jobject) u_arraylist_get(g_gattObjectList, index);
        if (!jarrayObj)
        {
            oc_mutex_unlock(g_gattObjectMutex);
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            (*env)->ReleaseStringUTFChars(env, jni_btTargetAddress, targetAddress);
            return NULL;
        }

        jobject jni_obj_device = (*env)->CallObjectMethod(env, jarrayObj, jni_mid_getDevice);
        if (!jni_obj_device)
        {
            CACheckJNIException(env);
            oc_mutex_unlock(g_gattObjectMutex);
            OIC_LOG(ERROR, TAG, "jni_obj_device is null");
            (*env)->ReleaseStringUTFChars(env, jni_btTargetAddress, targetAddress);
            return NULL;
        }

        jstring jni_btAddress = CALEGetAddressFromBTDevice(env, jni_obj_device);
        if (!jni_btAddress)
        {
            oc_mutex_unlock(g_gattObjectMutex);
            OIC_LOG(ERROR, TAG, "CALEGetAddressFromBTDevice has failed");
            (*env)->ReleaseStringUTFChars(env, jni_btTargetAddress, targetAddress);
            (*env)->DeleteLocalRef(env, jni_obj_device);
            return NULL;
        }

        const char* btAddress = (*env)->GetStringUTFChars(env, jni_btAddress, NULL);
        if (!btAddress)
        {
            CACheckJNIException(env);
            oc_mutex_unlock(g_gattObjectMutex);
            OIC_LOG(ERROR, TAG, "btAddress is not available");
            (*env)->ReleaseStringUTFChars(env, jni_btTargetAddress, targetAddress);
            (*env)->DeleteLocalRef(env, jni_btAddress);
            (*env)->DeleteLocalRef(env, jni_obj_device);
            return NULL;
        }

        OIC_LOG_V(DEBUG, TAG, "btAddress : %s (idx: %d)", btAddress, index);
        if (!strcasecmp(targetAddress, btAddress))
        {
            OIC_LOG(DEBUG, TAG, "Found Gatt object from BT device");

            // get LE address
            jstring jni_LEAddress = CALEClientGetAddressFromGattObj(env, jarrayObj);
            if (!jni_LEAddress)
            {
                OIC_LOG(ERROR, TAG, "jni_LEAddress is null");
            }
            oc_mutex_unlock(g_gattObjectMutex);
            (*env)->ReleaseStringUTFChars(env, jni_btTargetAddress, targetAddress);
            (*env)->ReleaseStringUTFChars(env, jni_btAddress, btAddress);
            (*env)->DeleteLocalRef(env, jni_btAddress);
            (*env)->DeleteLocalRef(env, jni_obj_device);
            return jni_LEAddress;
        }
        (*env)->ReleaseStringUTFChars(env, jni_btAddress, btAddress);
        (*env)->DeleteLocalRef(env, jni_btAddress);
        (*env)->DeleteLocalRef(env, jni_obj_device);
    }
    oc_mutex_unlock(g_gattObjectMutex);

    (*env)->ReleaseStringUTFChars(env, jni_btTargetAddress, targetAddress);
    OIC_LOG_V(DEBUG, TAG, "[%s] doesn't exist in gattObjectList", targetAddress);
    return NULL;
}

/**
 * BT State List
 */
CAResult_t CALEClientUpdateDeviceStateWithBtDevice(JNIEnv *env,
                                                   jobject device,
                                                   uint16_t state_type,
                                                   uint16_t target_state)
{
    VERIFY_NON_NULL(device, TAG, "device is null");

    // get Bluetooth Address
    jstring jni_Address = CALEGetAddressFromBTDevice(env, device);
    if (!jni_Address)
    {
        OIC_LOG(ERROR, TAG, "CALEGetAddressFromBTDevice has failed");
        return CA_STATUS_FAILED;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_Address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "targetAddress is not available");
        CACheckJNIException(env);
        (*env)->DeleteLocalRef(env, jni_Address);
        return CA_STATUS_FAILED;
    }

    if (CALEIsValidState(address, state_type, target_state,
                         g_deviceStateList,
                         g_deviceStateListMutex))
    {
        (*env)->DeleteLocalRef(env, jni_Address);
        return CA_STATUS_OK;
    }

    CAResult_t res = CALEUpdateDeviceState(address, state_type,
                                           target_state,
                                           g_deviceStateList,
                                           g_deviceStateListMutex);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
    }
    (*env)->ReleaseStringUTFChars(env, jni_Address, address);
    (*env)->DeleteLocalRef(env, jni_Address);

    return res;
}

CAResult_t CALEClientSetFlagToState(JNIEnv *env, jstring jni_address,
                                    jint state_idx, jboolean flag)
{
    return CALESetFlagToState(env, jni_address, state_idx, flag,
                              g_deviceStateList, g_deviceStateListMutex);
}

jboolean CALEClientGetFlagFromState(JNIEnv *env, jstring jni_address, jint state_idx)
{
    return CALEGetFlagFromState(env, jni_address, state_idx, g_deviceStateList,
                                g_deviceStateListMutex);
}

uint16_t CALEClientGetMtuSize(const char* address)
{
    return CALEGetMtuSize(address, g_deviceStateList, g_deviceStateListMutex);
}

void CALEClientCreateDeviceList()
{
    OIC_LOG(DEBUG, TAG, "CALEClientCreateDeviceList");

    // create new object array
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "Create g_gattObjectList");

        g_gattObjectList = u_arraylist_create();
    }

    if (!g_deviceStateList)
    {
        OIC_LOG(DEBUG, TAG, "Create g_deviceStateList");

        g_deviceStateList = u_arraylist_create();
    }

    if (!g_deviceList)
    {
        OIC_LOG(DEBUG, TAG, "Create g_deviceList");

        g_deviceList = u_arraylist_create();
    }
}

CAResult_t CALEClientResetDeviceStateForAll()
{
    return CALEResetDeviceStateForAll(g_deviceStateList, g_deviceStateListMutex);
}

/**
 * Check Sent Count for remove g_sendBuffer
 */
void CALEClientUpdateSendCnt(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CALEClientUpdateSendCnt");

    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    // mutex lock
    oc_mutex_lock(g_threadMutex);

    g_currentSentCnt++;

    if (g_targetCnt <= g_currentSentCnt)
    {
        g_targetCnt = 0;
        g_currentSentCnt = 0;

        CALEDeleteSendBuffer(env);

        // notity the thread
        oc_cond_signal(g_threadCond);
        oc_cond_signal(g_threadWriteCharacteristicCond);

        CALEClientSetSendFinishFlag(true);
        OIC_LOG(DEBUG, TAG, "set signal for send data");
    }

#ifdef SCAN_INTERVAL
    // reset interval scan logic
    CALERestartScanWithInterval(g_scanIntervalTimePrev, 0, BLE_SCAN_ENABLE);
#endif

    // mutex unlock
    oc_mutex_unlock(g_threadMutex);
}

CAResult_t CALEClientInitGattMutexVaraibles()
{
    if (NULL == g_bleReqRespClientCbMutex)
    {
        g_bleReqRespClientCbMutex = oc_mutex_new();
        if (NULL == g_bleReqRespClientCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_bleServerBDAddressMutex)
    {
        g_bleServerBDAddressMutex = oc_mutex_new();
        if (NULL == g_bleServerBDAddressMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadMutex)
    {
        g_threadMutex = oc_mutex_new();
        if (NULL == g_threadMutex)
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

    if (NULL == g_deviceListMutex)
    {
        g_deviceListMutex = oc_mutex_new();
        if (NULL == g_deviceListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_gattObjectMutex)
    {
        g_gattObjectMutex = oc_mutex_new();
        if (NULL == g_gattObjectMutex)
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

    if (NULL == g_SendFinishMutex)
    {
        g_SendFinishMutex = oc_mutex_new();
        if (NULL == g_SendFinishMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadWriteCharacteristicMutex)
    {
        g_threadWriteCharacteristicMutex = oc_mutex_new();
        if (NULL == g_threadWriteCharacteristicMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_deviceScanRetryDelayMutex)
    {
        g_deviceScanRetryDelayMutex = oc_mutex_new();
        if (NULL == g_deviceScanRetryDelayMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadSendStateMutex)
    {
        g_threadSendStateMutex = oc_mutex_new();
        if (NULL == g_threadSendStateMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadScanIntervalMutex)
    {
        g_threadScanIntervalMutex = oc_mutex_new();
        if (NULL == g_threadScanIntervalMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_setValueMutex)
    {
        g_setValueMutex = oc_mutex_new();
        if (NULL == g_setValueMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CALEClientTerminateGattMutexVariables()
{
    oc_mutex_free(g_bleReqRespClientCbMutex);
    g_bleReqRespClientCbMutex = NULL;

    oc_mutex_free(g_bleServerBDAddressMutex);
    g_bleServerBDAddressMutex = NULL;

    oc_mutex_free(g_threadMutex);
    g_threadMutex = NULL;

    oc_mutex_free(g_threadSendMutex);
    g_threadSendMutex = NULL;

    oc_mutex_free(g_deviceListMutex);
    g_deviceListMutex = NULL;

    oc_mutex_free(g_SendFinishMutex);
    g_SendFinishMutex = NULL;

    oc_mutex_free(g_threadWriteCharacteristicMutex);
    g_threadWriteCharacteristicMutex = NULL;

    oc_mutex_free(g_deviceScanRetryDelayMutex);
    g_deviceScanRetryDelayMutex = NULL;

    oc_mutex_free(g_threadSendStateMutex);
    g_threadSendStateMutex = NULL;

    oc_mutex_free(g_threadScanIntervalMutex);
    g_threadScanIntervalMutex = NULL;

    oc_mutex_free(g_gattObjectMutex);
    g_gattObjectMutex = NULL;

    oc_mutex_free(g_deviceStateListMutex);
    g_deviceStateListMutex = NULL;

    oc_mutex_free(g_setValueMutex);
    g_setValueMutex = NULL;
}

void CALEClientSetSendFinishFlag(bool flag)
{
    OIC_LOG_V(DEBUG, TAG, "g_isFinishedSendData is %d", flag);

    oc_mutex_lock(g_SendFinishMutex);
    g_isFinishedSendData = flag;
    oc_mutex_unlock(g_SendFinishMutex);
}

/**
 * adapter common
 */

CAResult_t CAStartLEGattClient()
{
    // init mutex for send logic
    if (!g_deviceDescCond)
    {
        g_deviceDescCond = oc_cond_new();
    }

    if (!g_threadCond)
    {
        g_threadCond = oc_cond_new();
    }

    if (!g_threadWriteCharacteristicCond)
    {
        g_threadWriteCharacteristicCond = oc_cond_new();
    }

    if (!g_threadScanIntervalCond)
    {
        g_threadScanIntervalCond = oc_cond_new();
    }

    CAResult_t ret = CALEClientStartScanWithInterval();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientStartScanWithInterval has failed");
        return ret;
    }

    g_isStartedLEClient = true;
    return CA_STATUS_OK;
}

void CAStopLEGattClient()
{
    OIC_LOG(DEBUG, TAG, "CAStopBLEGattClient");

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
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread has failed");
            return;
        }
        isAttached = true;
    }

    CAResult_t ret = CALEClientDisconnectAll(env);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientDisconnectAll has failed");
    }

    CALEClientStopScanWithInterval();

    oc_mutex_lock(g_threadWriteCharacteristicMutex);
    OIC_LOG(DEBUG, TAG, "signal - WriteCharacteristic cond");
    oc_cond_signal(g_threadWriteCharacteristicCond);
    oc_mutex_unlock(g_threadWriteCharacteristicMutex);

    CALEClientSetSendFinishFlag(true);
    oc_mutex_lock(g_threadMutex);
    OIC_LOG(DEBUG, TAG, "signal - g_threadCond cond");
    oc_cond_signal(g_threadCond);
    oc_mutex_unlock(g_threadMutex);

    oc_mutex_lock(g_deviceScanRetryDelayMutex);
    OIC_LOG(DEBUG, TAG, "signal - delay cond");
    oc_cond_signal(g_deviceScanRetryDelayCond);
    oc_mutex_unlock(g_deviceScanRetryDelayMutex);

    oc_mutex_lock(g_threadScanIntervalMutex);
    OIC_LOG(DEBUG, TAG, "signal - delay cond");
    oc_cond_signal(g_threadScanIntervalCond);
    oc_mutex_unlock(g_threadScanIntervalMutex);

    oc_mutex_lock(g_threadSendMutex);
    OIC_LOG(DEBUG, TAG, "signal - g_deviceDesc cond");
    oc_cond_signal(g_deviceDescCond);
    oc_mutex_unlock(g_threadSendMutex);

    oc_cond_free(g_deviceDescCond);
    oc_cond_free(g_threadCond);
    oc_cond_free(g_threadWriteCharacteristicCond);
    oc_cond_free(g_deviceScanRetryDelayCond);
    oc_cond_free(g_threadScanIntervalCond);

    g_deviceDescCond = NULL;
    g_threadCond = NULL;
    g_threadWriteCharacteristicCond = NULL;
    g_deviceScanRetryDelayCond = NULL;
    g_threadScanIntervalCond = NULL;

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

}

CAResult_t CAInitializeLEGattClient()
{
    OIC_LOG(DEBUG, TAG, "Initialize GATT Client");
    CALEClientInitialize();
    return CA_STATUS_OK;
}

void CATerminateLEGattClient()
{
    OIC_LOG(INFO, TAG, "IN - Terminate GATT Client");
    CAStopLEGattClient();
    CALEClientTerminate();
    OIC_LOG(INFO, TAG, "OUT - Terminate GATT Client");
}

CAResult_t CAUpdateCharacteristicsToGattServer(const char *remoteAddress, const uint8_t  *data,
                                               uint32_t dataLen, CALETransferType_t type,
                                               int32_t position)
{
    OIC_LOG(INFO, TAG, "call CALEClientSendUnicastMessage");
    VERIFY_NON_NULL(remoteAddress, TAG, "remoteAddress is null");

    if (LE_UNICAST != type || position < 0)
    {
        OIC_LOG(ERROR, TAG, "this request is not unicast");
        return CA_STATUS_INVALID_PARAM;
    }

    return CALEClientSendUnicastMessage(remoteAddress, data, dataLen);
}

CAResult_t CAUpdateCharacteristicsToAllGattServers(const uint8_t *data, uint32_t dataLen)
{
    OIC_LOG(DEBUG, TAG, "call CALEClientSendMulticastMessage");
    VERIFY_NON_NULL(data, TAG, "data is null");

    return CALEClientSendMulticastMessage(data, dataLen);
}

void CASetLEReqRespClientCallback(CABLEDataReceivedCallback callback)
{
    oc_mutex_lock(g_bleReqRespClientCbMutex);
    g_CABLEClientDataReceivedCallback = callback;
    oc_mutex_unlock(g_bleReqRespClientCbMutex);
}

void CASetLEClientThreadPoolHandle(ca_thread_pool_t handle)
{
    g_threadPoolHandle = handle;
}

CAResult_t CAGetLEAddress(char **local_address)
{
    VERIFY_NON_NULL(local_address, TAG, "local_address");
    return CA_NOT_SUPPORTED;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeRegisterLeScanCallback(JNIEnv *env, jobject obj,
                                                                    jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CaLeRegisterLeScanCallback");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(callback, TAG, "callback is null");

    g_leScanCallback = (*env)->NewGlobalRef(env, callback);
    CACheckJNIException(env);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeRegisterLeScanCallbackForV21(JNIEnv *env,
                                                                          jobject obj,
                                                                          jobject callback)
{
    OIC_LOG(DEBUG, TAG, "caLeRegisterLeScanCallbackForV21");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(callback, TAG, "callback is null");

    g_leScanCallback = (*env)->NewGlobalRef(env, callback);
    CACheckJNIException(env);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeRegisterGattCallback(JNIEnv *env, jobject obj,
                                                                  jobject callback)
{
    OIC_LOG(DEBUG, TAG, "CaLeRegisterGattCallback");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(callback, TAG, "callback is null");

    g_leGattCallback = (*env)->NewGlobalRef(env, callback);
    CACheckJNIException(env);
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeScanCallback(JNIEnv *env, jobject obj,
                                                          jobject device)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(device, TAG, "device is null");

    CAResult_t res = CALEClientAddScanDeviceToList(env, device);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "CALEClientAddScanDeviceToList has failed : %d", res);
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeScanFailedCallback(JNIEnv *env, jobject obj,
                                                                jint errorCode)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");

    switch (errorCode)
    {
        case 1:
            OIC_LOG(ERROR, TAG, "BLE scan has failed, error is SCAN_FAILED_ALREADY_STARTED");
            break;

        case 2:
            OIC_LOG(ERROR, TAG,
                    "BLE scan has failed, error is SCAN_FAILED_APPLICATION_REGISTRATION_FAILED");
            break;

        case 3:
            OIC_LOG(ERROR, TAG, "BLE scan has failed, error is SCAN_FAILED_INTERNAL_ERROR");
            break;

        case 4:
            OIC_LOG(ERROR, TAG, "BLE scan has failed, error is SCAN_FAILED_FEATURE_UNSUPPORTED");
            break;

        default:
            OIC_LOG(ERROR, TAG, "BLE scan has failed with unknown error");
            break;
    }
}

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    CALeGattConnectionStateChangeCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;II)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattConnectionStateChangeCallback(JNIEnv *env,
                                                                               jobject obj,
                                                                               jobject gatt,
                                                                               jint status,
                                                                               jint newstate)
{
    OIC_LOG_V(INFO, TAG, "CALeGattConnectionStateChangeCallback - status %d, newstate %d", status,
              newstate);
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt is null");

    jint state_connected = CALEGetConstantsValue(env, CLASSPATH_BT_PROFILE, "STATE_CONNECTED");

    jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "CALEClientGetAddressFromGattObj has failed");
        goto error_exit;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        CACheckJNIException(env);
        goto error_exit;
    }
    OIC_LOG_V(INFO, TAG, "CALeGattConnectionStateChangeCallback - address [%s]", address);

    if (state_connected == newstate)
    {
        OIC_LOG(DEBUG, TAG, "LE is connected");
        if (GATT_SUCCESS == status)
        {
            CAResult_t res = CALEUpdateDeviceState(address,
                                                   CA_LE_CONNECTION_STATE,
                                                   STATE_CONNECTED,
                                                   g_deviceStateList,
                                                   g_deviceStateListMutex);
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
                goto error_exit;
            }

            res = CALEClientAddGattobjToList(env, gatt);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientAddGattobjToList has failed");
                goto error_exit;
            }

            res = CALEClientDiscoverServices(env, gatt);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientDiscoverServices has failed");
                goto error_exit;
            }
        }
        else
        {
            OIC_LOG(INFO, TAG, "unknown status");
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
        }
    }
    else // STATE_DISCONNECTED == newstate
    {
        OIC_LOG(DEBUG, TAG, "LE is disconnected");

        if (CALEIsValidState(address, CA_LE_SEND_STATE, STATE_SEND_PREPARING,
                             g_deviceStateList, g_deviceStateListMutex))
        {
            OIC_LOG(INFO, TAG, "current state is STATE_SEND_PREPARING");
            CAResult_t res = CALEUpdateDeviceState(address, CA_LE_SEND_STATE,
                                                   STATE_SEND_FAIL,
                                                   g_deviceStateList,
                                                   g_deviceStateListMutex);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
            }
        }

        CAResult_t res = CALEUpdateDeviceState(address,
                                               CA_LE_CONNECTION_STATE,
                                               STATE_DISCONNECTED,
                                               g_deviceStateList,
                                               g_deviceStateListMutex);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
            goto error_exit;
        }
        (*env)->ReleaseStringUTFChars(env, jni_address, address);

        res = CALEClientGattClose(env, gatt);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientGattClose has failed");
        }

        if (CALECheckConnectionStateValue(status))
        {
            // this state is unexpected reason to disconnect
            // if the reason is suitable, connection logic of the device will be destroyed.
            OIC_LOG(INFO, TAG, "connection logic destroy");
            goto error_exit;
        }
        else
        {
            // other reason except for gatt_success is expected to running
            // background connection in BT platform.
            OIC_LOG(INFO, TAG, "unknown status or manual disconnected state");
        }
        CALEClientUpdateSendCnt(env);
    }
    return;

    // error label.
error_exit:
    CALEClientSendFinish(env, gatt);
    return;
}

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    CALeGattServicesDiscoveredCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattServicesDiscoveredCallback(JNIEnv *env,
                                                                            jobject obj,
                                                                            jobject gatt,
                                                                            jint status)
{
    OIC_LOG_V(INFO, TAG, "CALeGattServicesDiscoveredCallback - status %d", status);
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt is null");

    if (GATT_SUCCESS != status) // discovery error
    {
        CALEClientSendFinish(env, gatt);
        return;
    }

    jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
    if (!jni_address)
    {
        CALEClientSendFinish(env, gatt);
        return;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        CACheckJNIException(env);
        CALEClientSendFinish(env, gatt);
        return;
    }

    jstring jni_uuid = (*env)->NewStringUTF(env, OIC_GATT_CHARACTERISTIC_RESPONSE_UUID);
    if (!jni_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_uuid is null");
        goto error_exit;
    }

    jobject jni_obj_GattCharacteristic = CALEClientGetGattService(env, gatt, jni_uuid);
    if (!jni_obj_GattCharacteristic)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_GattCharacteristic is null");
        goto error_exit;
    }

    CAResult_t res = CALEClientSetCharacteristicNotification(env, gatt,
                                                             jni_obj_GattCharacteristic);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientSetCharacteristicNotification has failed");
        goto error_exit;
    }

    res = CALEClientSetUUIDToDescriptor(env, gatt, jni_obj_GattCharacteristic);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(INFO, TAG, "Descriptor is not found : %d", res);

        res = CALESetFlagToState(env, jni_address, CA_LE_DESCRIPTOR_FOUND, JNI_FALSE,
                                 g_deviceStateList, g_deviceStateListMutex);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALESetFlagToState has failed");
            goto error_exit;
        }

        res = CALEUpdateDeviceState(address, CA_LE_CONNECTION_STATE,
                                    STATE_SERVICE_CONNECTED,
                                    g_deviceStateList,
                                    g_deviceStateListMutex);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
            goto error_exit;
        }

        res = CALEClientRequestMTU(env, gatt, CA_SUPPORTED_BLE_MTU_SIZE);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientRequestMTU has failed");
            goto error_exit;
        }
    }
    else
    {
        res = CALESetFlagToState(env, jni_address, CA_LE_DESCRIPTOR_FOUND, JNI_TRUE,
                                 g_deviceStateList, g_deviceStateListMutex);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALESetFlagToState has failed");
            goto error_exit;
        }
    }

#ifdef SCAN_INTERVAL
    // reset interval scan logic
    CALERestartScanWithInterval(g_scanIntervalTimePrev, 0, BLE_SCAN_ENABLE);
#endif

    OIC_LOG(INFO, TAG, "ServicesDiscovery is successful");
    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    return;

    // error label.
error_exit:
    OIC_LOG(ERROR, TAG, "ServicesDiscovery has failed");
    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    CALEClientSendFinish(env, gatt);
    return;
}

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    CALeGattCharacteristicWritjclasseCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;Landroid/bluetooth/BluetoothGattCharacteristic;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattCharacteristicWriteCallback(
        JNIEnv *env, jobject obj, jobject gatt, jbyteArray data, jint status)
{
    OIC_LOG_V(INFO, TAG, "CALeGattCharacteristicWriteCallback - status : %d", status);
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt is null");

    // send success & signal
    jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
    if (!jni_address)
    {
        goto error_exit;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        CACheckJNIException(env);
        goto error_exit;
    }

    if (GATT_SUCCESS != status) // error case
    {
        OIC_LOG(ERROR, TAG, "send failure");

        // retry to write
        CAResult_t res = CALEClientWriteCharacteristic(env, gatt);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "WriteCharacteristic has failed");
            oc_mutex_lock(g_threadWriteCharacteristicMutex);
            g_isSignalSetFlag = true;
            oc_cond_signal(g_threadWriteCharacteristicCond);
            oc_mutex_unlock(g_threadWriteCharacteristicMutex);

            CAResult_t res = CALEUpdateDeviceState(address, CA_LE_SEND_STATE,
                                                   STATE_SEND_FAIL,
                                                   g_deviceStateList,
                                                   g_deviceStateListMutex);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
            }

            if (g_clientErrorCallback)
            {
                jint length = (*env)->GetArrayLength(env, data);
                CACheckJNIException(env);
                g_clientErrorCallback(address, data, length, CA_SEND_FAILED);
                CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, address, 0, length,
                                   false, "writeChar failure");
            }

            (*env)->ReleaseStringUTFChars(env, jni_address, address);
            goto error_exit;
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "send success");
        CAResult_t res = CALEUpdateDeviceState(address, CA_LE_SEND_STATE,
                                               STATE_SEND_SUCCESS,
                                               g_deviceStateList,
                                               g_deviceStateListMutex);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
        }

        oc_mutex_lock(g_threadWriteCharacteristicMutex);
        OIC_LOG(DEBUG, TAG, "g_isSignalSetFlag is set true and signal");
        g_isSignalSetFlag = true;
        oc_cond_signal(g_threadWriteCharacteristicCond);
        oc_mutex_unlock(g_threadWriteCharacteristicMutex);

        CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, address, 0,
                           (*env)->GetArrayLength(env, data),
                           true, "writeChar success");
    }

    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    return;

    // error label.
error_exit:

    CALEClientSendFinish(env, gatt);
    return;
}

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    CALeGattCharacteristicChangedCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;Landroid/bluetooth/BluetoothGattCharacteristic;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattCharacteristicChangedCallback(
        JNIEnv *env, jobject obj, jobject gatt, jbyteArray data)
{
    OIC_LOG(DEBUG, TAG, "CALeGattCharacteristicChangedCallback");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt is null");
    VERIFY_NON_NULL_VOID(data, TAG, "data is null");

    // get Byte Array and convert to uint8_t*
    jint length = (*env)->GetArrayLength(env, data);

    jboolean isCopy;
    jbyte *jni_byte_responseData = (jbyte*) (*env)->GetByteArrayElements(env, data, &isCopy);
    CACheckJNIException(env);

    uint8_t* receivedData = OICMalloc(length);
    if (!receivedData)
    {
        OIC_LOG(ERROR, TAG, "receivedData is null");
        return;
    }

    memcpy(receivedData, jni_byte_responseData, length);
    (*env)->ReleaseByteArrayElements(env, data, jni_byte_responseData, JNI_ABORT);

    jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
    if (!jni_address)
    {
        OIC_LOG(ERROR, TAG, "jni_address is null");
        OICFree(receivedData);
        return;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        CACheckJNIException(env);
        OICFree(receivedData);
        return;
    }

    OIC_LOG_V(DEBUG, TAG, "CALeGattCharacteristicChangedCallback - data. : %p, %d",
              receivedData, length);

    uint32_t sentLength = 0;
    oc_mutex_lock(g_bleServerBDAddressMutex);
    g_CABLEClientDataReceivedCallback(address, receivedData, length, &sentLength);
    oc_mutex_unlock(g_bleServerBDAddressMutex);

    (*env)->ReleaseStringUTFChars(env, jni_address, address);
}

/*
 * Class:     org_iotivity_ca_jar_caleinterface
 * Method:    CALeGattDescriptorWriteCallback
 * Signature: (Landroid/bluetooth/BluetoothGatt;Landroid/bluetooth/BluetoothGattDescriptor;I)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattDescriptorWriteCallback(JNIEnv *env, jobject obj,
                                                                         jobject gatt,
                                                                         jint status)
{
    OIC_LOG_V(INFO, TAG, "CALeGattDescriptorWriteCallback - status %d", status);
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(gatt, TAG, "gatt is null");

    if (GATT_SUCCESS != status) // error
    {
        goto error_exit;
    }

    jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
    if (!jni_address)
    {
        goto error_exit;
    }

    const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        CACheckJNIException(env);
        goto error_exit;
    }

    CAResult_t res = CALEUpdateDeviceState(address, CA_LE_CONNECTION_STATE,
                                           STATE_SERVICE_CONNECTED,
                                           g_deviceStateList,
                                           g_deviceStateListMutex);
    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEUpdateDeviceState has failed");
        goto error_exit;
    }

    res = CALEClientRequestMTU(env, gatt, CA_SUPPORTED_BLE_MTU_SIZE);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientRequestMTU has failed");
        goto error_exit;
    }
    return;

// error label.
error_exit:

    CALEClientSendFinish(env, gatt);
    return;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaLeClientInterface_caLeGattMtuChangedCallback(JNIEnv *env,
                                                                    jobject obj,
                                                                    jobject gatt,
                                                                    jint mtu,
                                                                    jint status)
{
    OIC_LOG_V(INFO, TAG, "caLeGattMtuChangedCallback - status %d, "
              "mtu[%d-including Header size 3 byte]", mtu, status);

    (void)obj;

    if (0 == status)
    {
        if (g_sendBuffer)
        {
            CAResult_t res = CALEClientWriteCharacteristic(env, gatt);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientWriteCharacteristic has failed");
            }
        }
        else
        {
            OIC_LOG(INFO, TAG, "mtu nego is done");
            jstring jni_address = CALEClientGetAddressFromGattObj(env, gatt);
            if (!jni_address)
            {
                CALEClientSendFinish(env, gatt);
                return;
            }

            const char* address = (*env)->GetStringUTFChars(env, jni_address, NULL);
            if (!address)
            {
                CACheckJNIException(env);
                (*env)->DeleteLocalRef(env, jni_address);
                CALEClientSendFinish(env, gatt);
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
            CALEClientUpdateSendCnt(env);
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
            (*env)->DeleteLocalRef(env, jni_address);
        }
    }
}
