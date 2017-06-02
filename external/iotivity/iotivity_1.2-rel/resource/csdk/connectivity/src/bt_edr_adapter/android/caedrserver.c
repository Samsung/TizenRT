/* ****************************************************************
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

#include "caedrinterface.h"
#include "caedrutils.h"
#include "caedrserver.h"
#include "logger.h"
#include "oic_malloc.h"
#include "cathreadpool.h" /* for thread pool */
#include "octhread.h"
#include "uarraylist.h"
#include "caadapterutils.h"
#include "org_iotivity_ca_CaEdrInterface.h"
#include "oic_string.h"

#define TAG PCF("OIC_CA_EDR_SERVER")

static const char METHODID_OBJECTNONPARAM[] = "()Landroid/bluetooth/BluetoothAdapter;";
static const char CLASSPATH_BT_ADPATER[] = "android/bluetooth/BluetoothAdapter";
static const char CLASSPATH_BT_UUID[] = "java/util/UUID";
static const char CLASSPATH_BT_SERVER_SOCKET[] = "android/bluetooth/BluetoothServerSocket";

static ca_thread_pool_t g_threadPoolHandle = NULL;

static JavaVM *g_jvm;

/**
 * when Both Mode(server and client) in App is set,
 * startDiscoveryServer and startListenningServer is calling.
 * and then both accept thread and receive thread is running redundantly.
 */
static bool g_isStartServer = false;

/**
 * Maximum CoAP over TCP header length
 * to know the total data length.
 */
#define EDR_MAX_HEADER_LEN  6

/**
 * Mutex to synchronize receive server.
 */
static oc_mutex g_mutexReceiveServer = NULL;

/**
 * Flag to control the Receive Unicast Data Thread.
 */
static bool g_stopUnicast = false;

/**
 * Mutex to synchronize accept server.
 */
static oc_mutex g_mutexAcceptServer = NULL;

/**
 * Flag to control the Accept Thread.
 */
static bool g_stopAccept = false;

/**
 * Mutex to synchronize server socket.
 */
static oc_mutex g_mutexServerSocket = NULL;

/**
 * Flag to control the server socket.
 */
static jobject g_serverSocket = NULL;

/**
 * Mutex to synchronize device state list.
 */
static oc_mutex g_mutexStateList = NULL;

/**
 * Mutex to synchronize device object list.
 */
static oc_mutex g_mutexObjectList = NULL;

/**
 * Mutex to synchronize start server state.
 */
static oc_mutex g_mutexStartServerState = NULL;

/**
 * Thread context information for unicast, multicast and secured unicast server.
 */
typedef struct
{
    bool *stopFlag;
    CAAdapterServerType_t type;
} CAAdapterReceiveThreadContext_t;

typedef struct
{
    bool *stopFlag;
} CAAdapterAcceptThreadContext_t;

/**
 * Maintains the callback to be notified when data received from remote
 * Bluetooth device.
 */
static CAEDRDataReceivedCallback g_edrPacketReceivedCallback = NULL;

static void CAReceiveHandler(void *data)
{
    OIC_LOG(DEBUG, TAG, "IN - CAReceiveHandler..");
    // Input validation
    VERIFY_NON_NULL_VOID(data, TAG, "Invalid thread context");

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(DEBUG, TAG, "CAReceiveHandler - Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread failed");
            return;
        }
        isAttached = true;
    }

    CAAdapterReceiveThreadContext_t *ctx = (CAAdapterReceiveThreadContext_t *) data;

    while (true != *(ctx->stopFlag))
    {
        if (!CAEDRNativeIsEnableBTAdapter(env))
        {
            OIC_LOG(INFO, TAG, "BT adapter is not enabled");
            break;
        }

        // if new socket object is added in socket list after below logic is ran.
        // new socket will be started to read after next while loop
        uint32_t length = CAEDRGetSocketListLength();
        if (0 != length)
        {
            for (uint32_t idx = 0; idx < length; idx++)
            {
                CAEDRNativeReadData(env, idx);
            }
        }
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    OICFree(ctx);

    OIC_LOG(DEBUG, TAG, "OUT - CAReceiveHandler");
}

static void CAAcceptHandler(void *data)
{
    OIC_LOG(DEBUG, TAG, "AcceptThread start");

    VERIFY_NON_NULL_VOID(data, TAG, "data is null");

    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(DEBUG, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread failed");
            return;
        }
        isAttached = true;
    }

    jobject jni_obj_BTServerSocket = CAEDRNativeListen(env);
    if (!jni_obj_BTServerSocket)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTServerSocket is null");

        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }

        return;
    }

    oc_mutex_lock(g_mutexServerSocket);
    g_serverSocket = (*env)->NewGlobalRef(env, jni_obj_BTServerSocket);
    oc_mutex_unlock(g_mutexServerSocket);

    CAAdapterAcceptThreadContext_t *ctx = (CAAdapterAcceptThreadContext_t *) data;

    // it should be initialized for restart accept thread
    oc_mutex_lock(g_mutexAcceptServer);
    g_stopAccept = false;
    oc_mutex_unlock(g_mutexAcceptServer);

    while (true != *(ctx->stopFlag))
    {
        // when BT state is changed with Off. its thread will be stopped
        if (!CAEDRNativeIsEnableBTAdapter(env))
        {
            OIC_LOG(INFO, TAG, "BT adapter is not enabled");
            oc_mutex_lock(g_mutexAcceptServer);
            g_stopAccept = true;
            oc_mutex_unlock(g_mutexAcceptServer);
            oc_mutex_lock(g_mutexServerSocket);
            g_serverSocket = NULL;
            oc_mutex_unlock(g_mutexServerSocket);
        }
        else
        {
            CAEDRNativeAccept(env, g_serverSocket);
        }
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    OICFree(ctx);

    OIC_LOG(DEBUG, TAG, "AcceptThread finish");

    return;
}

/**
 * implement for adapter common method.
 */
CAResult_t CAEDRServerStart()
{
    if (!g_threadPoolHandle)
    {
        return CA_STATUS_NOT_INITIALIZED;
    }

    oc_mutex_lock(g_mutexStartServerState);
    if (g_isStartServer)
    {
        OIC_LOG(DEBUG, TAG, "server already started");
        oc_mutex_unlock(g_mutexStartServerState);
        return CA_STATUS_OK;
    }
    oc_mutex_unlock(g_mutexStartServerState);

    CAResult_t res = CAEDRServerStartAcceptThread();
    if (CA_STATUS_OK == res)
    {
        res = CAEDRStartReceiveThread(false);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "failed to start receive thread");
            CAEDRServerStop();
            return CA_STATUS_FAILED;
        }
        oc_mutex_lock(g_mutexStartServerState);
        g_isStartServer = true;
        oc_mutex_unlock(g_mutexStartServerState);
    }

    return res;
}

CAResult_t CAEDRServerStop()
{
    CAEDRStopReceiveThread();

    oc_mutex_lock(g_mutexAcceptServer);
    g_stopAccept = true;
    oc_mutex_unlock(g_mutexAcceptServer);

    if (!g_jvm)
    {
        OIC_LOG(DEBUG, TAG, "g_jvm is null");
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
            OIC_LOG(ERROR, TAG, "AttachCurrentThread failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    CAEDRNatvieCloseServerTask(env);
    oc_mutex_lock(g_mutexStartServerState);
    g_isStartServer = false;
    oc_mutex_unlock(g_mutexStartServerState);

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;
}

void CAEDRSetPacketReceivedCallback(CAEDRDataReceivedCallback packetReceivedCallback)
{
    g_edrPacketReceivedCallback = packetReceivedCallback;
}

static void CAEDRServerDestroyMutex()
{
    if (g_mutexReceiveServer)
    {
        oc_mutex_free(g_mutexReceiveServer);
        g_mutexReceiveServer = NULL;
    }

    if (g_mutexAcceptServer)
    {
        oc_mutex_free(g_mutexAcceptServer);
        g_mutexAcceptServer = NULL;
    }

    if (g_mutexServerSocket)
    {
        oc_mutex_free(g_mutexServerSocket);
        g_mutexServerSocket = NULL;
    }

    if (g_mutexStateList)
    {
        oc_mutex_free(g_mutexStateList);
        g_mutexStateList = NULL;
    }

    if (g_mutexObjectList)
    {
        oc_mutex_free(g_mutexObjectList);
        g_mutexObjectList = NULL;
    }

    if (g_mutexStartServerState)
    {
        oc_mutex_free(g_mutexStartServerState);
        g_mutexStartServerState = NULL;
    }
}

static CAResult_t CAEDRServerCreateMutex()
{
    g_mutexReceiveServer = oc_mutex_new();
    if (!g_mutexReceiveServer)
    {
        OIC_LOG(ERROR, TAG, "Failed to created mutex!");
        return CA_STATUS_FAILED;
    }

    g_mutexAcceptServer = oc_mutex_new();
    if (!g_mutexAcceptServer)
    {
        OIC_LOG(ERROR, TAG, "Failed to created mutex!");

        CAEDRServerDestroyMutex();
        return CA_STATUS_FAILED;
    }

    g_mutexServerSocket = oc_mutex_new();
    if (!g_mutexServerSocket)
    {
        OIC_LOG(ERROR, TAG, "Failed to created mutex!");

        CAEDRServerDestroyMutex();
        return CA_STATUS_FAILED;
    }

    g_mutexStateList = oc_mutex_new();
    if (!g_mutexStateList)
    {
        OIC_LOG(ERROR, TAG, "Failed to created mutex!");

        CAEDRServerDestroyMutex();
        return CA_STATUS_FAILED;
    }

    g_mutexObjectList = oc_mutex_new();
    if (!g_mutexObjectList)
    {
        OIC_LOG(ERROR, TAG, "Failed to created mutex!");

        CAEDRServerDestroyMutex();
        return CA_STATUS_FAILED;
    }

    g_mutexStartServerState = oc_mutex_new();
    if (!g_mutexStartServerState)
    {
        OIC_LOG(ERROR, TAG, "Failed to created mutex!");

        CAEDRServerDestroyMutex();
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

void CAEDRServerJniInit()
{
    OIC_LOG(DEBUG, TAG, "CAEDRServerJniInit");
    g_jvm = CANativeJNIGetJavaVM();
}

CAResult_t CAEDRServerInitialize(ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, TAG, "CAEDRServerInitialize");
    VERIFY_NON_NULL(handle, TAG, "handle is NULL");

    g_threadPoolHandle = handle;
    CAEDRServerJniInit();

    return CAEDRServerCreateMutex();
}

CAResult_t CAEDRServerStartAcceptThread()
{
    bool isAttached = false;
    JNIEnv* env = NULL;
    jint res = (*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6);
    if (JNI_OK != res)
    {
        OIC_LOG(DEBUG, TAG, "Could not get JNIEnv pointer");
        res = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);

        if (JNI_OK != res)
        {
            OIC_LOG(ERROR, TAG, "AttachCurrentThread failed");
            return CA_STATUS_FAILED;
        }
        isAttached = true;
    }

    jstring jni_address = CAEDRNativeGetLocalDeviceAddress(env);
    if (jni_address)
    {
        const char* localAddress = (*env)->GetStringUTFChars(env, jni_address, NULL);
        OIC_LOG_V(DEBUG, TAG, "My BT Address is %s", localAddress);
        (*env)->ReleaseStringUTFChars(env, jni_address, localAddress);
    }

    oc_mutex_lock(g_mutexStateList);
    CAEDRNativeCreateDeviceStateList();
    oc_mutex_unlock(g_mutexStateList);

    oc_mutex_lock(g_mutexObjectList);
    CAEDRNativeCreateDeviceSocketList();
    oc_mutex_unlock(g_mutexObjectList);

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    CAAdapterReceiveThreadContext_t *ctx = (CAAdapterReceiveThreadContext_t *) OICMalloc(
            sizeof(CAAdapterReceiveThreadContext_t));
    if (!ctx)
    {
        OIC_LOG(ERROR, TAG, "Out of memory!");
        return CA_MEMORY_ALLOC_FAILED;
    }

    ctx->stopFlag = &g_stopAccept;
    if (CA_STATUS_OK != ca_thread_pool_add_task(g_threadPoolHandle, CAAcceptHandler,
                                                (void *) ctx, NULL))
    {
        OIC_LOG(ERROR, TAG, "Failed to create read thread!");
        OICFree((void *) ctx);
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

void CAEDRServerTerminate()
{
    OIC_LOG(DEBUG, TAG, "CAEDRServerTerminate");

    if (!g_jvm)
    {
        return;
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
            OIC_LOG(ERROR, TAG, "AttachCurrentThread failed");
            return;
        }
        isAttached = true;
    }

    CAEDRNativeSocketCloseToAll(env);

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    CAEDRNativeRemoveAllDeviceState();
    CAEDRNativeRemoveAllDeviceSocket(env);

    // delete mutex
    CAEDRServerDestroyMutex();
}

CAResult_t CAEDRStartReceiveThread(bool isSecured)
{
    OIC_LOG(DEBUG, TAG, "CAEDRStartReceiveThread");

    oc_mutex_lock(g_mutexReceiveServer);

    /**
     * The task to listen for data from unicast is added to the thread pool.
     * This is a blocking call is made where we try to receive some data..
     * We will keep waiting until some data is received.
     * This task will be terminated when thread pool is freed on stopping the adapters.
     * Thread context will be freed by thread on exit.
     */
    CAAdapterReceiveThreadContext_t *ctx = (CAAdapterReceiveThreadContext_t *) OICMalloc(
            sizeof(CAAdapterReceiveThreadContext_t));
    if (!ctx)
    {
        OIC_LOG(ERROR, TAG, "Out of memory!");
        oc_mutex_unlock(g_mutexReceiveServer);
        return CA_MEMORY_ALLOC_FAILED;
    }

    g_stopUnicast = false;
    ctx->stopFlag = &g_stopUnicast;
    ctx->type = isSecured ? CA_SECURED_UNICAST_SERVER : CA_UNICAST_SERVER;
    if (CA_STATUS_OK != ca_thread_pool_add_task(g_threadPoolHandle, CAReceiveHandler,
                                                (void *) ctx, NULL))
    {
        OIC_LOG(ERROR, TAG, "Failed to create read thread!");
        oc_mutex_unlock(g_mutexReceiveServer);
        OICFree((void *) ctx);
        return CA_STATUS_FAILED;
    }
    oc_mutex_unlock(g_mutexReceiveServer);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRStopReceiveThread()
{
    OIC_LOG(DEBUG, TAG, "CAEDRStopReceiveThread");

    oc_mutex_lock(g_mutexReceiveServer);
    g_stopUnicast = true;
    oc_mutex_unlock(g_mutexReceiveServer);

    return CA_STATUS_OK;
}

CAResult_t CAEDRNativeReadData(JNIEnv *env, uint32_t idx)
{
    if ((*env)->ExceptionCheck(env))
    {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        OIC_LOG(ERROR, TAG, "env error!!");
        return CA_STATUS_FAILED;
    }

    jobject jni_obj_inputStream = CAEDRNativeGetInputStream(idx);
    if (!jni_obj_inputStream)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_inputStream is null");
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_available = CAGetJNIMethodID(env, "java/io/InputStream", "available", "()I");
    if (!jni_mid_available)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_available is null");
        return CA_STATUS_FAILED;
    }

    jint available = (*env)->CallIntMethod(env, jni_obj_inputStream, jni_mid_available);

    if (0 < available)
    {
        jobject jni_obj_socket = CAEDRNativeGetDeviceSocket(idx);
        if (!jni_obj_socket)
        {
            OIC_LOG(ERROR, TAG, "jni_obj_socket is null");
            return CA_STATUS_FAILED;
        }

        jstring jni_str_address = CAEDRNativeGetAddressFromDeviceSocket(env, jni_obj_socket);
        if (!jni_str_address)
        {
            OIC_LOG(ERROR, TAG, "jni_str_address is null");
            return CA_STATUS_FAILED;
        }

        const char* address = (*env)->GetStringUTFChars(env, jni_str_address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            (*env)->DeleteLocalRef(env, jni_str_address);
            return CA_STATUS_FAILED;
        }

        CAConnectedDeviceInfo_t *deviceInfo =
                (CAConnectedDeviceInfo_t *) CAEDRGetDeviceInfoFromAddress(address);
        if (!deviceInfo)
        {
            OIC_LOG(ERROR, TAG, "failed to get device info from list");
            (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
            (*env)->DeleteLocalRef(env, jni_str_address);
            return CA_STATUS_FAILED;
        }

        jint bufSize = (deviceInfo->totalDataLen == 0) ? EDR_MAX_HEADER_LEN
                                                       : deviceInfo->totalDataLen;
        if (!deviceInfo->recvData)
        {
            deviceInfo->recvData = OICCalloc(1, bufSize);
            if (!deviceInfo->recvData)
            {
                OIC_LOG(ERROR, TAG, "out of memory");
                (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
                (*env)->DeleteLocalRef(env, jni_str_address);
                return CA_STATUS_FAILED;
            }
        }

        jint remainSize = (jint) bufSize - deviceInfo->recvDataLen;
        if (0 >= remainSize)
        {
            OIC_LOG(ERROR, TAG, "remainSize value is invalid.");
            (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
            (*env)->DeleteLocalRef(env, jni_str_address);
            return CA_STATUS_FAILED;
        }

        jbyteArray jbuf = (*env)->NewByteArray(env, remainSize);
        if (!jbuf)
        {
            OIC_LOG(ERROR, TAG, "jbuf is null");
            (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
            (*env)->DeleteLocalRef(env, jni_str_address);
            return CA_STATUS_FAILED;
        }

        jmethodID jni_mid_read = CAGetJNIMethodID(env, "java/io/InputStream", "read", "([BII)I");
        if (!jni_mid_read)
        {
            OIC_LOG(ERROR, TAG, "jni_mid_read is null");
            (*env)->DeleteLocalRef(env, jbuf);
            (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
            (*env)->DeleteLocalRef(env, jni_str_address);
            return CA_STATUS_FAILED;
        }

        OIC_LOG_V(DEBUG, TAG, "read InputStream (idx:%d, addr:%s)", idx, address);
        jint recvLen = (*env)->CallIntMethod(env, jni_obj_inputStream, jni_mid_read,
                                             jbuf, (jint) 0, remainSize);
        if (-1 == recvLen)
        {
            OIC_LOG(ERROR, TAG, "recvLen is -1");
            (*env)->DeleteLocalRef(env, jbuf);
            (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
            (*env)->DeleteLocalRef(env, jni_str_address);
            return CA_STATUS_FAILED;
        }
        OIC_LOG_V(DEBUG, TAG, "read success (length: %d bytes)", recvLen);

        jbyte* buf = (*env)->GetByteArrayElements(env, jbuf, NULL);
        if (!buf)
        {
            OIC_LOG(ERROR, TAG, "buf is null");
            (*env)->DeleteLocalRef(env, jbuf);
            (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
            (*env)->DeleteLocalRef(env, jni_str_address);
            return CA_STATUS_FAILED;
        }
        memcpy(deviceInfo->recvData + deviceInfo->recvDataLen, (const char*) buf, recvLen);
        deviceInfo->recvDataLen += recvLen;

        (*env)->ReleaseByteArrayElements(env, jbuf, buf, 0);
        (*env)->DeleteLocalRef(env, jbuf);

        if (!deviceInfo->totalDataLen && deviceInfo->recvData)
        {
            coap_transport_t transport = coap_get_tcp_header_type_from_initbyte(
                    ((unsigned char *) deviceInfo->recvData)[0] >> 4);
            size_t headerLen = coap_get_tcp_header_length_for_transport(transport);
            if (deviceInfo->recvDataLen >= headerLen)
            {
                deviceInfo->totalDataLen = coap_get_total_message_length(deviceInfo->recvData,
                                                                         deviceInfo->recvDataLen);
                OIC_LOG_V(DEBUG, TAG, "total data length [%d] bytes", deviceInfo->totalDataLen);

                uint8_t *newBuf = OICRealloc(deviceInfo->recvData, deviceInfo->totalDataLen);
                if (!newBuf)
                {
                    OIC_LOG(ERROR, TAG, "out of memory");
                    (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
                    (*env)->DeleteLocalRef(env, jni_str_address);
                    return CA_STATUS_FAILED;
                }
                deviceInfo->recvData = newBuf;
            }
        }

        if (deviceInfo->totalDataLen == deviceInfo->recvDataLen)
        {
            if ((*env)->ExceptionCheck(env))
            {
                OIC_LOG(ERROR, TAG, "read Error!!!");
                (*env)->ExceptionDescribe(env);
                (*env)->ExceptionClear(env);

                // update state to disconnect
                // the socket will be close next read thread routine
                oc_mutex_lock(g_mutexStateList);
                CAEDRUpdateDeviceState(STATE_DISCONNECTED, address);
                oc_mutex_unlock(g_mutexStateList);

                (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
                (*env)->DeleteLocalRef(env, jni_str_address);
                return CA_STATUS_FAILED;
            }

            if (g_edrPacketReceivedCallback)
            {
                OIC_LOG_V(DEBUG, TAG, "data will be sent to callback routine: %s, %d",
                          deviceInfo->recvData, deviceInfo->recvDataLen);

                uint32_t sentLength = 0;
                g_edrPacketReceivedCallback(address, (void*) deviceInfo->recvData,
                                            deviceInfo->recvDataLen, &sentLength);

                OICFree(deviceInfo->recvData);
                deviceInfo->recvData = NULL;
                deviceInfo->recvDataLen = 0;
                deviceInfo->totalDataLen = 0;
            }
        }

        (*env)->ReleaseStringUTFChars(env, jni_str_address, address);
        (*env)->DeleteLocalRef(env, jni_str_address);
    }

    return CA_STATUS_OK;
}

void CANativeStartListenTask(JNIEnv *env)
{
    jobject jni_obj_BTServerSocket = CAEDRNativeListen(env);
    if (!jni_obj_BTServerSocket)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTServerSocket is null");
        return;
    }

    oc_mutex_lock(g_mutexServerSocket);
    g_serverSocket = (*env)->NewGlobalRef(env, jni_obj_BTServerSocket);
    oc_mutex_unlock(g_mutexServerSocket);
}

jobject CAEDRNativeListen(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeListen - IN");

    if (!CAEDRNativeIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return NULL;
    }

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADPATER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return NULL;
    }

    // get BTadpater
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        return NULL;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        return NULL;
    }

    // get listen method ID
    jmethodID jni_mid_listen = (*env)->GetMethodID(
            env, jni_cid_BTAdapter, "listenUsingInsecureRfcommWithServiceRecord",
            "(Ljava/lang/String;Ljava/util/UUID;)Landroid/bluetooth/BluetoothServerSocket;");
    if (!jni_mid_listen)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_listen is null");
        return NULL;
    }
    // listenUsingInsecureRfcommWithServiceRecord  / listenUsingRfcommWithServiceRecord
    // setting UUID
    jclass jni_cid_uuid = (*env)->FindClass(env, CLASSPATH_BT_UUID);
    if (!jni_cid_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_listen is null");
        return NULL;
    }

    jmethodID jni_mid_fromString = (*env)->GetStaticMethodID(
            env, jni_cid_uuid, "fromString", "(Ljava/lang/String;)Ljava/util/UUID;");
    if (!jni_mid_fromString)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_fromString is null");
        return NULL;
    }

    jstring jni_uuid = (*env)->NewStringUTF(env, OIC_EDR_SERVICE_ID);
    if (!jni_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_uuid is null");
        return NULL;
    }
    jobject jni_obj_uuid = (*env)->CallStaticObjectMethod(env, jni_cid_uuid, jni_mid_fromString,
                                                          jni_uuid);
    if (!jni_obj_uuid)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_uuid is null");
        return NULL;
    }

    // create socket
    jstring jni_name = (*env)->NewStringUTF(env, "BluetoothTestSecure");
    if (!jni_name)
    {
        OIC_LOG(ERROR, TAG, "jni_name is null");
        return NULL;
    }
    jobject jni_obj_BTServerSocket = (*env)->CallObjectMethod(env, jni_obj_BTAdapter, jni_mid_listen,
                                                             jni_name, jni_obj_uuid);
    if (!jni_obj_BTServerSocket)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTServerSocket is null");
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "CAEDRNativeListen - OUT");

    return jni_obj_BTServerSocket;
}

void CAEDRNativeAccept(JNIEnv *env, jobject serverSocketObject)
{
    OIC_LOG(DEBUG, TAG, "CAEDRNativeAccept - IN");

    if (NULL != serverSocketObject)
    {
        jmethodID jni_mid_accept = CAGetJNIMethodID(env, CLASSPATH_BT_SERVER_SOCKET,
                                                    "accept",
                                                    "()Landroid/bluetooth/BluetoothSocket;");
        if (!jni_mid_accept)
        {
            OIC_LOG(ERROR, TAG, "jni_mid_accept is null");
            return;
        }

        OIC_LOG(DEBUG, TAG, "waiting for the new connection request...");

        jobject jni_obj_BTSocket = (*env)->CallObjectMethod(env, serverSocketObject,
                                                            jni_mid_accept);
        if (!jni_obj_BTSocket)
        {
            OIC_LOG(ERROR, TAG, "jni_obj_BTSocket is null");

            if ((*env)->ExceptionCheck(env))
            {
                OIC_LOG(ERROR, TAG, "socket might closed or timeout!!!");
                (*env)->ExceptionDescribe(env);
                (*env)->ExceptionClear(env);
                return;
            }
        }

        // get remote address
        jstring j_str_address = CAEDRNativeGetAddressFromDeviceSocket(env, jni_obj_BTSocket);
        if (!j_str_address)
        {
            OIC_LOG(ERROR, TAG, "j_str_address is null");
            return;
        }

        const char* address = (*env)->GetStringUTFChars(env, j_str_address, NULL);

        OIC_LOG_V(INFO, TAG, "accept a new connection from [%s]", address);

        // update state
        oc_mutex_lock(g_mutexStateList);
        CAEDRUpdateDeviceState(STATE_CONNECTED, address);
        oc_mutex_unlock(g_mutexStateList);

        (*env)->ReleaseStringUTFChars(env, j_str_address, address);
        (*env)->DeleteLocalRef(env, j_str_address);

        // set socket to list
        jobject jni_socket = (*env)->NewGlobalRef(env, jni_obj_BTSocket);
        if (!jni_socket)
        {
            OIC_LOG(ERROR, TAG, "jni_socket is null");
            (*env)->DeleteLocalRef(env, jni_obj_BTSocket);
            return;
        }

        oc_mutex_lock(g_mutexObjectList);
        CAEDRNativeAddDeviceSocketToList(env, jni_socket);
        oc_mutex_unlock(g_mutexObjectList);

        (*env)->DeleteGlobalRef(env, jni_socket);
        (*env)->DeleteLocalRef(env, jni_obj_BTSocket);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "serverSocket is close previously");
    }

    OIC_LOG(DEBUG, TAG, "CAEDRNativeAccept - OUT");
}

/**
 * InputStream & BluetoothServerSocket will be close for Terminating.
 */
void CAEDRNatvieCloseServerTask(JNIEnv* env)
{
    if (g_serverSocket)
    {
        OIC_LOG(DEBUG, TAG, "Accept Resource will be close");

        jmethodID jni_mid_close = CAGetJNIMethodID(env, CLASSPATH_BT_SERVER_SOCKET,
                                                   "close", "()V");
        if (!jni_mid_close)
        {
            OIC_LOG(ERROR, TAG, "jni_mid_close is null");
            return;
        }

        (*env)->CallVoidMethod(env, g_serverSocket, jni_mid_close);
        (*env)->DeleteGlobalRef(env, g_serverSocket);
        g_serverSocket = NULL;

        OIC_LOG(DEBUG, TAG, "close accept obj");
    }
}
