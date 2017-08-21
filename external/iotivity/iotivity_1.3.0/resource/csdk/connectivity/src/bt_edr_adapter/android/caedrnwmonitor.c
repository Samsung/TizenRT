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

#include "caedrinterface.h"
#include "caedrnwmonitor.h"
#include "logger.h"
#include "oic_malloc.h"
#include "cathreadpool.h" /* for thread pool */
#include "octhread.h"
#include "uarraylist.h"
#include "caadapterutils.h"
#include "caedrserver.h"
#include "caedrutils.h"

#include "org_iotivity_ca_CaEdrInterface.h"

//#define DEBUG_MODE
#define TAG PCF("OIC_CA_EDR_MONITOR")

static JavaVM *g_jvm;
static jobject g_context;
static CAEDRNetworkStatusCallback g_networkChangeCb = NULL;

static const char CLASSPATH_BT_ADPATER[] = "android/bluetooth/BluetoothAdapter";

void CAEDRNetworkMonitorJNISetContext()
{
    OIC_LOG(DEBUG, TAG, "CAEDRNetworkMonitorJNISetContext");
    g_context = (jobject) CANativeJNIGetContext();
}

//getting jvm
void CAEDRNetworkMonitorJniInit()
{
    OIC_LOG(DEBUG, TAG, "CAEDRNetworkMonitorJniInit");
    g_jvm = (JavaVM*) CANativeJNIGetJavaVM();
}

CAResult_t CAEDRInitializeNetworkMonitor(const ca_thread_pool_t threadPool)
{
    if (!threadPool)
    {
        return CA_STATUS_FAILED;
    }
    else
    {
        CAEDRNetworkMonitorJniInit();
        CANativeJNIGetJavaVM();
    }

    return CA_STATUS_OK;
}

void CAEDRSetNetworkChangeCallback(CAEDRNetworkStatusCallback networkChangeCallback)
{
    OIC_LOG(DEBUG, TAG, "CAEDRSetNetworkChangeCallback");
    g_networkChangeCb = networkChangeCallback;
}

void CAEDRTerminateNetworkMonitor(void)
{
}

CAResult_t CAEDRStartNetworkMonitor()
{
    return CA_STATUS_OK;
}

CAResult_t CAEDRStopNetworkMonitor()
{
    return CA_STATUS_OK;
}

CAResult_t CAEDRClientSetCallbacks(void)
{
    return CA_STATUS_OK;
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaEdrInterface_caEdrStateChangedCallback(JNIEnv *env, jobject obj,
                                                              jint status)
{
    if (!env || !obj)
    {
        OIC_LOG(ERROR, TAG, "parameter is null");
        return;
    }

    // STATE_ON:12, STATE_OFF:10
    OIC_LOG(DEBUG, TAG, "CaEdrInterface - Network State Changed");

    if (NULL == g_networkChangeCb)
    {
        OIC_LOG(DEBUG, TAG, "g_networkChangeCb is null");
        return;
    }

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADPATER);
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
        CANetworkStatus_t newStatus = CA_INTERFACE_UP;

        CAResult_t res = CAEDRStartReceiveThread(false);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "Failed to CAEDRStartReceiveThread");
            return;
        }
        CAEDRServerStartAcceptThread();
        g_networkChangeCb(newStatus);
    }
    else if (state_off == status)
    {
        CANetworkStatus_t newStatus = CA_INTERFACE_DOWN;
        CAEDRServerStop();
        CAEDRNativeSocketCloseToAll(env);
        CAEDRNativeRemoveAllDeviceState();
        CAEDRNativeRemoveAllDeviceSocket(env);
        g_networkChangeCb(newStatus);
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaEdrInterface_caEdrBondStateChangedCallback(JNIEnv *env, jobject obj,
                                                                  jstring addr)
{
    if (!env || !obj)
    {
        OIC_LOG(ERROR, TAG, "parameter is null");
        return;
    }

    OIC_LOG(DEBUG, TAG, "CaEdrInterface - Bond State Changed");

    if (addr)
    {
        CAEDRNativeRemoveDeviceSocketBaseAddr(env, addr);
        CAEDRNativeRemoveDevice(addr);
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_ca_CaEdrInterface_caEdrConnectionStateChangedCallback(JNIEnv *env,
                                                                        jobject obj,
                                                                        jstring addr,
                                                                        jint isConnected)
{
    if (!env || !obj || !addr)
    {
        OIC_LOG(ERROR, TAG, "parameter is null");
        return;
    }

    OIC_LOG_V(DEBUG, TAG, "CaEdrInterface - Connection State Changed : %d", isConnected);

    if (!isConnected)
    {
        const char *address = (*env)->GetStringUTFChars(env, addr, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            return;
        }

        CAEDRNativeRemoveDeviceSocketBaseAddr(env, addr);
        CAEDRNativeRemoveDevice(address);
        (*env)->ReleaseStringUTFChars(env, addr, address);
    }
}
