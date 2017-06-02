/* ****************************************************************j
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include "canfcinterface.h"

#include "caadapterutils.h"
#include "octhread.h"
#include "oic_malloc.h"
#include "oic_string.h"

/**
 * TAG
 * Logging tag for module name
 */
#define TAG "NFC_SERVER"

static CANFCPacketReceivedCallback g_packetReceivedCallback;

static JavaVM *g_jvm = NULL;
static jobject g_context = NULL;
static jobject g_activity = NULL;
static jobject g_nfcInterface = NULL;
static jmethodID g_sendMethod = NULL;

static void CANfcJniInit();
static void CANfcJNISetContext();
static CAResult_t CANfcCreateJniInterfaceObject();
static CAResult_t CANfcSendDataImpl(const CAEndpoint_t * ep, const char* data, uint32_t dataLen);

static const char CLASS_NFCINTERFACE[] = "org/iotivity/ca/CaNfcInterface";

static void CANfcJniInit()
{
    OIC_LOG(DEBUG, TAG, "CANfcJniInit");
    g_jvm = (JavaVM*) CANativeJNIGetJavaVM();
}

static void CANfcJNISetContext()
{
    OIC_LOG(DEBUG, TAG, "CANfcJNISetContext");
    g_context = (jobject) CANativeJNIGetContext();
    g_activity = (jobject) CANativeGetActivity();
}

void CANFCSetPacketReceiveCallback(CANFCPacketReceivedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");

    g_packetReceivedCallback = callback;

    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t SetCreateNdefMessageCallbackfromNative(JNIEnv* env)
{
    OIC_LOG(DEBUG, TAG, "SetCreateNdefMessageCallbackfromNative IN");

    VERIFY_NON_NULL(env, TAG, "env");
    VERIFY_NON_NULL(g_context, TAG, "g_context");
    VERIFY_NON_NULL(g_activity, TAG, "g_activity");
    VERIFY_NON_NULL(g_nfcInterface, TAG, "g_nfcInterface");

    jclass cid_NfcAdapter = (*env)->FindClass(env, "android/nfc/NfcAdapter");
    if (!cid_NfcAdapter)
    {
        OIC_LOG(ERROR, TAG, "Could not get NfcAdapter class");
        return CA_STATUS_FAILED;
    }

    jmethodID mid_getAdapter = (*env)->GetStaticMethodID(env, cid_NfcAdapter,
                                       "getDefaultAdapter",
                                       "(Landroid/content/Context;)Landroid/nfc/NfcAdapter;");
    if (!mid_getAdapter)
    {
        OIC_LOG(ERROR, TAG, "Could not get methodId mid_getAdapter");
        return CA_STATUS_FAILED;
    }

    jobject adapter = (*env)->CallStaticObjectMethod(env, cid_NfcAdapter, mid_getAdapter,
                                                     g_context);
    if (!adapter)
    {
        OIC_LOG(ERROR, TAG, "Could not get NfcAdapter");
        return CA_STATUS_FAILED;
    }

    jmethodID mid_setCallback = (*env)->GetMethodID(
        env, cid_NfcAdapter, "setNdefPushMessageCallback",
        "(Landroid/nfc/NfcAdapter$CreateNdefMessageCallback;Landroid/app/Activity;"
        "[Landroid/app/Activity;)V");
    if (!mid_setCallback)
    {
        OIC_LOG(ERROR, TAG, "Could not get mid_setCallback");
        return CA_STATUS_FAILED;
    }

    jclass cid_Activity = (*env)->FindClass(env, "android/app/Activity");
    if (!cid_Activity)
    {
        OIC_LOG(ERROR, TAG, "Could not get Activity class");
        return CA_STATUS_FAILED;
    }

    jobjectArray tempArr = (jobjectArray) (*env)->NewObjectArray(env, 0, cid_Activity, NULL);
    (*env)->CallVoidMethod(env, adapter, mid_setCallback, g_nfcInterface, g_activity, tempArr);

    OIC_LOG(DEBUG, TAG, "SetCreateNdefMessageCallbackfromNative OUT");
    return CA_STATUS_OK;
}

CAResult_t CANfcCreateJniInterfaceObject()
{
    OIC_LOG(DEBUG, TAG, "CANfcCreateJniInterfaceObject IN");

    VERIFY_NON_NULL(g_activity, TAG, "g_activity");
    VERIFY_NON_NULL(g_context, TAG, "g_context");
    VERIFY_NON_NULL(g_jvm, TAG, "g_jvm");

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
        return CA_STATUS_FAILED;
    }

    jclass jni_NfcInterface = (*env)->FindClass(env, CLASS_NFCINTERFACE);
    if (!jni_NfcInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaNfcInterface class");
        goto error_exit;
    }

    jmethodID NfcInterfaceConstructorMethod = (*env)->GetMethodID(env, jni_NfcInterface, "<init>",
                                            "(Landroid/content/Context;Landroid/app/Activity;)V");
    if (!NfcInterfaceConstructorMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaNfcInterface constructor method");
        goto error_exit;
    }

    jobject jni_nfcInstance = (*env)->NewObject(env, jni_NfcInterface,
                                                NfcInterfaceConstructorMethod, jApplicationContext,
                                                g_activity);
    if (!jni_nfcInstance)
    {
        OIC_LOG(ERROR, TAG, "Create instance for CaNfcInterface failed");
        goto error_exit;
    }

    g_nfcInterface = (*env)->NewGlobalRef(env, jni_nfcInstance);
    if (!g_nfcInterface)
    {
        OIC_LOG(ERROR, TAG, "NewGlobalRef  for nfcInterface failed");
        goto error_exit;
    }

    OIC_LOG(DEBUG, TAG, "Create instance for CaNfcInterface");

    CAResult_t result = SetCreateNdefMessageCallbackfromNative(env);
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "SetCreateNdefMessageCallbackfromNative failed");
        goto error_exit;
    }

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    OIC_LOG(DEBUG, TAG, "CANfcCreateJniInterfaceObject OUT");
    return CA_STATUS_OK;

error_exit:
    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_FAILED;
}

CAResult_t CAInitializeNfcServer()
{
    CANfcJniInit();
    CANfcJNISetContext();

    CAResult_t result = CANfcCreateJniInterfaceObject();
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CANfcJniInit failed");
    }

    return result;
}

CAResult_t CANFCStartServer()
{
    bool isAttached = false;

    OIC_LOG(INFO, TAG, "CANFCStartServer : IN");

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

    jclass jni_NfcInterface = (*env)->FindClass(env, CLASS_NFCINTERFACE);
    if (!jni_NfcInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaNFCClientInterface class");
        goto error_exit;
    }

    jmethodID methodId = (*env)->GetMethodID(env, jni_NfcInterface, "caNfcInitialize", "()V");
    if (!methodId)
    {
        OIC_LOG(ERROR, TAG, "Could not get methodId");
        goto error_exit;
    }

    if (!g_nfcInterface)
    {
        OIC_LOG(ERROR, TAG, "g_nfcInterface NULL");
        goto error_exit;
    }

    (*env)->CallVoidMethod(env, g_nfcInterface, methodId);
    OIC_LOG(DEBUG, TAG, "caNfcInitialize");

    jmethodID sendDataMethodId = (*env)->GetMethodID(env, jni_NfcInterface, "processSendRquest",
                                                     "([B)V");
    if (!sendDataMethodId)
    {
        OIC_LOG(ERROR, TAG, "Could not get sendDataMethodId");
        goto error_exit;
    }

    g_sendMethod = sendDataMethodId;

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

void CANFCStopServer()
{
    // JNI Call to unregstier nfc adapter
}

/*
 * Class:     org_iotivity_ca_CaNfcInterface
 * Method:    caNativeNfcCreateNdefMessage
 * Signature: ([B)Landroid/nfc/NdefMessage;
 */
JNIEXPORT jobject JNICALL
Java_org_iotivity_ca_CaNfcInterface_caNativeNfcCreateNdefMessage(JNIEnv *env, jobject obj,
                                                                 jbyteArray sendData)
{
    OIC_LOG(DEBUG, TAG, "caNativeNfcCreateNdefMessage : IN");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(obj, TAG, "obj is null", NULL);

    const char *mime = "application/org.iotivity.ca.sample_service";
    jstring mimeString = (*env)->NewStringUTF(env, mime);
    if (!mimeString)
    {
        OIC_LOG(ERROR, TAG, "NewStringUTF failed for mimeString");
        return NULL;
    }

    const char *type = "US_ASCII";
    jstring charSetString = (*env)->NewStringUTF(env, type);
    if (!charSetString)
    {
        OIC_LOG(ERROR, TAG, "NewStringUTF failed for charSetString");
        return NULL;
    }

    jmethodID mid_getBytes = CAGetJNIMethodID(env, "java/lang/String", "getBytes",
                                              "(Ljava/lang/String;)[B");
    if (!mid_getBytes)
    {
        OIC_LOG(ERROR, TAG, "Could not get methodId for mid_getBytes");
        return NULL;
    }

    jbyteArray mimeTypeArr = (*env)->CallObjectMethod(env, mimeString, mid_getBytes,
                                                      charSetString);
    if (!mimeTypeArr)
    {
        OIC_LOG(ERROR, TAG, "getBytes failed for mimeTypeArr");
    }

    jclass cid_NdefRecord = (*env)->FindClass(env, "android/nfc/NdefRecord");
    if (!cid_NdefRecord)
    {
        OIC_LOG(ERROR, TAG, "Could not get NdefRecord class for cid_NdefRecord");
        return NULL;
    }

    jmethodID mid_createRecord = (*env)->GetMethodID(env, cid_NdefRecord, "<init>",
                                                     "(S[B[B[B)V");
    if (!mid_createRecord)
    {
        OIC_LOG(ERROR, TAG, "Could not get methodId for mid_createRecord");
        return NULL;
    }

    jfieldID fid_tnfType = (*env)->GetStaticFieldID(env, cid_NdefRecord, "TNF_MIME_MEDIA", "S");

    jint tnfType = (*env)->GetStaticShortField(env, cid_NdefRecord, fid_tnfType);
    OIC_LOG_V(ERROR, TAG, "tnfType : %d", tnfType);

    jbyteArray nullArr = (*env)->NewByteArray(env, 0);

    jobject ndefRecord = (*env)->NewObject(env, cid_NdefRecord, mid_createRecord, tnfType,
                                           mimeTypeArr, nullArr, sendData);
    if (!ndefRecord)
    {
        OIC_LOG(ERROR, TAG, "createNdefRecord failed for ndefRecord");
        return NULL;
    }

    jclass cid_NdefMsg = (*env)->FindClass(env, "android/nfc/NdefMessage");
    if (!cid_NdefMsg)
    {
        OIC_LOG(ERROR, TAG, "Could not get NdefMessage class for cid_NdefMsg");
        return NULL;
    }

    jmethodID mid_createMsg = (*env)->GetMethodID(env, cid_NdefMsg, "<init>",
                                                  "([Landroid/nfc/NdefRecord;)V");
    if (!mid_createMsg)
    {
        OIC_LOG(ERROR, TAG, "Could not get methodId for mid_createMsg");
        return NULL;
    }
    jobjectArray tempArr = (jobjectArray) (*env)->NewObjectArray(env, 1, cid_NdefRecord,
                                                                 ndefRecord);

    jobject ndefMsg = (*env)->NewObject(env, cid_NdefMsg, mid_createMsg, tempArr);
    if (!ndefMsg)
    {
        OIC_LOG(ERROR, TAG, "createNdefMessage failed for ndefMsg");
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "caNativeNfcCreateNdefMessage : OUT");
    return ndefMsg;
}

/*
 * Class:     org_iotivity_ca_CaNfcInterface
 * Method:    caNativeNfcInvokeBeam
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_org_iotivity_ca_CaNfcInterface_caNativeNfcInvokeBeam(JNIEnv *env, jobject obj)
{
    OIC_LOG(DEBUG, TAG, "cANativeNfcInvokeBeam : IN");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", false);
    VERIFY_NON_NULL_RET(obj, TAG, "obj is null", false);
    VERIFY_NON_NULL_RET(g_context, TAG, "g_context is null", false);
    VERIFY_NON_NULL_RET(g_activity, TAG, "g_activity is null", false);

    jclass cid_NfcAdapter = (*env)->FindClass(env, "android/nfc/NfcAdapter");
    if (!cid_NfcAdapter)
    {
        OIC_LOG(ERROR, TAG, "Could not get NfcAdapter cid_NfcAdapter ");
        return JNI_FALSE;
    }

    jmethodID mid_getAdapter = (*env)->GetStaticMethodID(env, cid_NfcAdapter,
                                       "getDefaultAdapter",
                                       "(Landroid/content/Context;)Landroid/nfc/NfcAdapter;");
    if (!mid_getAdapter)
    {
        OIC_LOG(ERROR, TAG, "Could not get methodId mid_getAdapter");
        return JNI_FALSE;
    }

    jobject adapter = (*env)->CallStaticObjectMethod(env, cid_NfcAdapter, mid_getAdapter,
                                                     g_context);
    if (!adapter)
    {
        OIC_LOG(ERROR, TAG, "getDefaultAdapter failed adapter");
        return JNI_FALSE;
    }

    jmethodID mid_invokeBeam = (*env)->GetMethodID(env, cid_NfcAdapter, "invokeBeam",
                                                   "(Landroid/app/Activity;)Z");
    if (!mid_invokeBeam)
    {
        OIC_LOG(ERROR, TAG, "Could not get methodId mid_invokeBeam");
        return JNI_FALSE;
    }


    jboolean isSuccess = (*env)->CallBooleanMethod(env, adapter, mid_invokeBeam, g_activity);
    if (!isSuccess)
    {
        OIC_LOG(ERROR, TAG, "invokeBeam has failed");
    }
    OIC_LOG(DEBUG, TAG, "cANativeNfcInvokeBeam : OUT");

    return isSuccess;
}

/*
 * Class:     org_iotivity_ca_CaNfcInterface
 * Method:    caNativeNfcPacketReceived
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaNfcInterface_caNativeNfcPacketReceived(JNIEnv *env, jobject obj,
                                                              jbyteArray data)
{
    OIC_LOG(DEBUG, TAG, "caNfcPacketReceived : IN");
    VERIFY_NON_NULL_VOID(env, TAG, "env is null");
    VERIFY_NON_NULL_VOID(obj, TAG, "obj is null");
    VERIFY_NON_NULL_VOID(data, TAG, "data is null");

    char recvBuffer[COAP_MAX_PDU_SIZE] = {0};

    // get Byte Array and covert to char*
    jint length = (*env)->GetArrayLength(env, data);

    if (length >= COAP_MAX_PDU_SIZE)
    {
        OIC_LOG_V(ERROR, TAG, "caNfcPacketReceived - Invalid CoAP length : %d",
                  length);
        return;
    }

    jboolean isCopy;
    jbyte *jni_received_data = (jbyte*) (*env)->GetByteArrayElements(env, data, &isCopy);

    OIC_LOG_V(DEBUG, TAG, "caNfcPacketReceived - raw data received : %s",
              jni_received_data);

    memcpy(recvBuffer, (const char*) jni_received_data, length);
    (*env)->ReleaseByteArrayElements(env, data, jni_received_data, JNI_ABORT);

    const char* address = "AA:BB:CC:DD:EE:FF";

    OIC_LOG_V(DEBUG, TAG, "caNfcPacketReceived - data. : %s, %d",
              recvBuffer, length);

    CASecureEndpoint_t sep =
            {.endpoint =
                {.adapter = CA_ADAPTER_NFC,
                 .flags = CA_DEFAULT_FLAGS
                }
             };
    OICStrcpy(sep.endpoint.addr, sizeof(sep.endpoint.addr), address);

    g_packetReceivedCallback(&sep, recvBuffer, length);
    OIC_LOG(DEBUG, TAG, "caNfcPacketReceived : OUT");
}

CAResult_t CANfcSendDataImpl(const CAEndpoint_t * ep, const char* data, uint32_t dataLen)
{
    VERIFY_NON_NULL(ep, TAG, "CANfcSendDataImpl : endpoint is null");
    VERIFY_NON_NULL(data, TAG, "CANfcSendDataImpl : data is null");
    VERIFY_NON_NULL(g_jvm, TAG, "CANfcSendDataImpl : g_jvm is null");

    OIC_LOG(INFO, TAG, "CANfcSendDataImpl moved env outside");
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

    OIC_LOG(INFO, TAG, "creating send buffer");
    jbyteArray sendData = (*env)->NewByteArray(env, dataLen);
    if (!sendData)
    {
        OIC_LOG(ERROR, TAG, "Failed to create ByteArray");
        if (isAttached)
        {
            (*g_jvm)->DetachCurrentThread(g_jvm);
        }

        return CA_SEND_FAILED;

    }

    (*env)->SetByteArrayRegion(env, sendData, 0, dataLen, (jbyte*) data);

    (*env)->CallVoidMethod(env, g_nfcInterface, g_sendMethod, sendData);
    OIC_LOG(DEBUG, TAG, "send data through NFC");

    (*env)->DeleteLocalRef(env, sendData);

    if (isAttached)
    {
        (*g_jvm)->DetachCurrentThread(g_jvm);
    }

    return CA_STATUS_OK;
}

void CANFCSendData(const CAEndpoint_t *endpoint, const void *data, uint32_t dataLength)
{
    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");
    VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

    // JNI to Send data
    CANfcSendDataImpl(endpoint, data, dataLength);
    return;
}

