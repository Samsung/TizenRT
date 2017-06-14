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

#include "cabtpairinginterface.h"
#include "cabtpairing.h"
#include "cacommon.h"
#include "logger.h"
#include <jni.h>

#define  TAG   "OIC_CA_BT_PAIRING"

/**
 * pointer to store JavaVM
 */
static JavaVM *g_jvm = NULL;

/**
 * pointer to store context for android callback interface
 */
static jobject g_context = NULL;
static jobject g_listener = NULL;

static const char CLASSPATH_CONTEXT[] = "android/content/Context";
static const char METHODID_CONTEXTNONPARAM[] = "()Landroid/content/Context;";
static const char CLASSPATH_BT_PAIRING_INTERFACE[] = "org/iotivity/ca/CaBtPairingInterface";
static const char CLASSPATH_BT_ADAPTER[] = "android/bluetooth/BluetoothAdapter";
static const char METHODID_OBJECTNONPARAM[] = "()Landroid/bluetooth/BluetoothAdapter;";
static const char CLASSPATH_BT_DEVICE[] = "android/bluetooth/BluetoothDevice";

static void CABTPaitingCreateJNIInterfaceObject(jobject context)
{
    JNIEnv* env = NULL;
    OIC_LOG(INFO, TAG, "OICEDRCreateJNIInterfaceObject");

    if ((*g_jvm)->GetEnv(g_jvm, (void**) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        OIC_LOG(ERROR, TAG, "Could not get JNIEnv pointer");
        return;
    }

    //getApplicationContext
    jclass contextClass = (*env)->FindClass(env, CLASSPATH_CONTEXT);
    if (!contextClass)
    {
        OIC_LOG(ERROR, TAG, "Could not get context object class");
        return;
    }

    jmethodID getApplicationContextMethod = (*env)->GetMethodID(env, contextClass,
                                                                "getApplicationContext",
                                                                METHODID_CONTEXTNONPARAM);
    if (!getApplicationContextMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get getApplicationContext method");
        return;
    }

    //Create EDRJniInterface instance
    jclass EDRJniInterface = (*env)->FindClass(env, CLASSPATH_BT_PAIRING_INTERFACE);
    if (!EDRJniInterface)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaEdrInterface class");
        return;
    }

    jmethodID EDRInterfaceConstructorMethod = (*env)->GetMethodID(env, EDRJniInterface, "<init>",
                                                                  "(Landroid/content/Context;)V");
    if (!EDRInterfaceConstructorMethod)
    {
        OIC_LOG(ERROR, TAG, "Could not get CaEdrInterface constructor method");
        return;
    }

    (*env)->NewObject(env, EDRJniInterface, EDRInterfaceConstructorMethod, context);
    OIC_LOG(INFO, TAG, "New Object has been created successfully");
}

CAResult_t CABTPairingInitialize(JNIEnv *env, JavaVM *jvm, jobject context)
{
    OIC_LOG(INFO, TAG, "CABTPairingInitialize");

    if (!context)
    {
        OIC_LOG(ERROR, TAG, "context is null");
        return CA_STATUS_FAILED;
    }

    g_jvm = jvm;
    g_context = (*env)->NewGlobalRef(env, context);

    if (g_context)
    {
        /* create java interface instance*/
        CABTPaitingCreateJNIInterfaceObject(g_context);
    }
    return CA_STATUS_OK;
}

void CABTPairingSetFoundDeviceListener(jobject listener)
{
    g_listener = listener;
}

CAResult_t CABTPairingStartScan(JNIEnv *env)
{
    OIC_LOG(INFO, TAG, "CABTPairingStartScan");
    if (!env)
    {
        OIC_LOG(ERROR, TAG, "parameter is null");
        return CA_STATUS_FAILED;
    }

    CABTPairingStopScan(env);

    // get default bt adapter class
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return CA_STATUS_FAILED;
    }

    // get remote bt adapter method ID
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        return CA_STATUS_FAILED;
    }

    // get startDiscovery() method ID
    jmethodID jni_mid_startDiscovery = (*env)->GetMethodID(env, jni_cid_BTAdapter, "startDiscovery",
                                                           "()Z");
    if (!jni_mid_startDiscovery)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_startDiscovery is null");
        return CA_STATUS_FAILED;
    }

    // get bluetooth adapter object
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        return CA_STATUS_FAILED;
    }

    // call startDiscovery() method
    jboolean jni_obj_startDiscovery = (*env)->CallBooleanMethod(env, jni_obj_BTAdapter,
                                                                jni_mid_startDiscovery);
    if (!jni_obj_startDiscovery)
    {
        OIC_LOG(ERROR, TAG, "startDiscovery has failed");
        return CA_STATUS_FAILED;
    }
    else
    {
        OIC_LOG(INFO, TAG, "startDiscovery has started");
        return CA_STATUS_OK;
    }
}

CAResult_t CABTPairingStopScan(JNIEnv *env)
{
    OIC_LOG(INFO, TAG, "CABTPairingStopScan");
    if (!env)
    {
        OIC_LOG(ERROR, TAG, "parameter is null");
        return CA_STATUS_FAILED;
    }

    // get default bt adapter class
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return CA_STATUS_FAILED;
    }

    // get remote bt adapter method ID
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        return CA_STATUS_FAILED;
    }

    // get cancelDiscovery() method ID
    jmethodID jni_mid_cancelDiscovery = (*env)->GetMethodID(env, jni_cid_BTAdapter, "cancelDiscovery",
                                                           "()Z");
    if (!jni_mid_cancelDiscovery)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_cancelDiscovery is null");
        return CA_STATUS_FAILED;
    }

    // gat bt adapter object
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        return CA_STATUS_FAILED;
    }

    // call cancelDiscovery() method
    jboolean jni_obj_cancelDiscovery = (*env)->CallBooleanMethod(env, jni_obj_BTAdapter,
                                                                 jni_mid_cancelDiscovery);
    if (!jni_obj_cancelDiscovery)
    {
        OIC_LOG(ERROR, TAG, "cancelDiscovery has failed");
        return CA_STATUS_FAILED;
    }
    else
    {
        OIC_LOG(INFO, TAG, "cancelDiscovery has started");
        return CA_STATUS_OK;
    }
}

CAResult_t CABTPairingCreateBond(JNIEnv *env, jobject device)
{
    OIC_LOG(INFO, TAG, "CABTPairingCreateBond");
    if (!env || !device)
    {
        OIC_LOG(ERROR, TAG, "parameter is null");
        return CA_STATUS_FAILED;
    }

    // get default bt adapter class
    jclass jni_cid_BTDevice = (*env)->FindClass(env, CLASSPATH_BT_DEVICE);
    if (!jni_cid_BTDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTDevice is null");
        return CA_STATUS_FAILED;
    }

    // get createBond() method ID
    jmethodID jni_mid_createBond = (*env)->GetMethodID(env, jni_cid_BTDevice, "createBond",
                                                       "()Z");
    if (!jni_mid_createBond)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_createBond is null");
        return CA_STATUS_FAILED;
    }

    // call createBond() method
    jboolean jni_obj_createBond = (*env)->CallBooleanMethod(env, device, jni_mid_createBond);
    if (!jni_obj_createBond)
    {
        OIC_LOG(ERROR, TAG, "createBond has failed");
        return CA_STATUS_FAILED;
    }
    else
    {
        OIC_LOG(INFO, TAG, "createBond has started");
        return CA_STATUS_OK;
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaBtPairingInterface_oicEdrStateChangedCallback(JNIEnv *env, jobject obj,
                                                                     jint status)
{
    OIC_LOG(INFO, TAG, "oicEdrStateChangedCallback");
    if (!env || !obj)
    {
        OIC_LOG(ERROR, TAG, "parameter is null");
        return;
    }

    // STATE_ON:12, STATE_OFF:10
    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return;
    }

    jfieldID id_state_on = (*env)->GetStaticFieldID(env, jni_cid_BTAdapter, "STATE_ON", "I");
    if (!id_state_on)
    {
        OIC_LOG(ERROR, TAG, "id_state_on is null");
        return;
    }

    jfieldID id_state_off = (*env)->GetStaticFieldID(env, jni_cid_BTAdapter, "STATE_OFF", "I");
    if (!id_state_off)
    {
        OIC_LOG(ERROR, TAG, "id_state_off is null");
        return;
    }

    jint state_on = (*env)->GetStaticIntField(env, jni_cid_BTAdapter, id_state_on);
    jint state_off = (*env)->GetStaticIntField(env, jni_cid_BTAdapter, id_state_off);

    if (state_on == status)
    {
        OIC_LOG(INFO, TAG, "oicEdrStateChangedCallback : state_on");
    }
    else if (state_off == status)
    {
        OIC_LOG(INFO, TAG, "oicEdrStateChangedCaloicEdrFoundDeviceCallbacklback : state_off");
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaBtPairingInterface_oicEdrBondStateChangedCallback(JNIEnv *env, jobject obj,
                                                                         jstring addr)
{
    OIC_LOG(INFO, TAG, "oicEdrBondStateChangedCallback");
    (void)env;
    (void)obj;
    (void)addr;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaBtPairingInterface_oicEdrFoundDeviceCallback(JNIEnv *env, jobject obj,
                                                                    jobject device)
{
    if (!env || !obj || !device)
    {
        return;
    }

    if (!g_listener)
    {
        return;
    }

    jclass jni_cls_listener = (*env)->GetObjectClass(env, g_listener);
    if (!jni_cls_listener)
    {
        OIC_LOG(ERROR, TAG, "could not get jni_cls_listener");
        return;
    }

    jmethodID jni_mid_listener = (*env)->GetMethodID(env, jni_cls_listener, "onBtDeviceFound",
                                        "(Landroid/bluetooth/BluetoothDevice;)V");
    if (!jni_mid_listener)
    {
        OIC_LOG(ERROR, TAG, "could not get Method ID");
        return;
    }

    (*env)->CallVoidMethod(env, g_listener, jni_mid_listener, device);
}

