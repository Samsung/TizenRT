/******************************************************************
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

#include "JniDevicePropProvisioningStatusListener.h"
#include "JniRemoteEnrollee.h"

using namespace OIC::Service;

JniDevicePropProvisioningStatusListener::JniDevicePropProvisioningStatusListener(JNIEnv *env, jobject jListener,
        JniRemoteEnrollee *owner)
    : m_ownerResource(owner)
{
    m_jwListener = env->NewWeakGlobalRef(jListener);
}

JniDevicePropProvisioningStatusListener::~JniDevicePropProvisioningStatusListener()
{
    ES_LOGI("~JniDevicePropProvisioningStatusListener()");
    if (m_jwListener)
    {
        jint ret;
        JNIEnv *env = GetESJNIEnv(ret);
        if (NULL == env) return;
        env->DeleteWeakGlobalRef(m_jwListener);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
    }
}

void JniDevicePropProvisioningStatusListener::onDevicePropProvisioningStatusCallback (std::shared_ptr<DevicePropProvisioningStatus>
        devicePropProvStatusCb)
{

    ES_LOGI("JniDevicePropProvisioningStatusListener::onDevicePropProvisioningStatusCallback enter");

    jint ret;
    JNIEnv *env = GetESJNIEnv(ret);
    if (NULL == env) return;
    jobject jListener = env->NewLocalRef(m_jwListener);
    if (!jListener)
    {
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    jclass clsL = env->GetObjectClass(jListener);
    if (!clsL)
    {
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }
    //TODO:
    jmethodID midL = env->GetMethodID(clsL, "onProgress",
                                      "(Lorg/iotivity/service/easysetup/mediator/"
                                      "DevicePropProvisioningStatus;"
                                      ")V");

    if (!midL)
    {
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    ESResult esResult = devicePropProvStatusCb->getESResult();

    jobject jDevicePropProvisioningStatus = NULL;
    jDevicePropProvisioningStatus = env->NewObject(g_cls_DevicePropProvisioningStatus,
                                                g_mid_DevicePropProvisioningStatus_ctor,
                                                (jint)esResult);

    ES_LOGI("JniDevicePropProvisioningStatus::onDevicePropProvisioningStatus - %d", esResult);
    if (!jDevicePropProvisioningStatus)
    {
        ES_LOGE("JniDevicePropProvisioningStatus::onDevicePropProvisioningStatus Unable to create the java object");
        return ;
    }

    env->CallVoidMethod(jListener, midL, jDevicePropProvisioningStatus);

    if (env->ExceptionCheck())
    {
        ES_LOGE("Java exception is thrown");
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
}

void JniDevicePropProvisioningStatusListener::checkExAndRemoveListener(JNIEnv *env)
{
    if (env->ExceptionCheck())
    {
        jthrowable ex = env->ExceptionOccurred();
        env->ExceptionClear();
        m_ownerResource->removeStatusListener<JniDevicePropProvisioningStatusListener>(env, m_jwListener);
        env->Throw((jthrowable)ex);
    }
    else
    {
        m_ownerResource->removeStatusListener<JniDevicePropProvisioningStatusListener>(env, m_jwListener);
    }
}
