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

#include "JniSecurityStatusListener.h"
#include "JniRemoteEnrollee.h"
#include "oic_malloc.h"
#include "oic_string.h"

using namespace OIC::Service;

JniSecurityStatusListener::JniSecurityStatusListener(JNIEnv *env, jobject jListener,
        JniRemoteEnrollee *owner)
    : m_ownerResource(owner)
{
    m_jwListener = env->NewWeakGlobalRef(jListener);
}

JniSecurityStatusListener::~JniSecurityStatusListener()
{
    ES_LOGI("~JniSecurityStatusListener()");
    if (m_jwListener)
    {
        jint ret;
        JNIEnv *env = GetESJNIEnv(ret);
        if (NULL == env) return;
        env->DeleteWeakGlobalRef(m_jwListener);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
    }
}
void JniSecurityStatusListener::secProvisionStatusCallback(
                std::shared_ptr<SecProvisioningStatus> secProvisioningStatus)
{

    ES_LOGI("JniSecurityStatusListener::secProvisionStatusCallback enter");

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

    jmethodID midL = env->GetMethodID(clsL, "onProgress",
                                      "(Lorg/iotivity/service/easysetup/mediator/"
                                      "SecurityProvisioningStatus;"
                                      ")V");

    if (!midL)
    {
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    ESResult esResult = secProvisioningStatus->getESResult();

    //create the java object
    jobject jSecurityProvisioningStatus = NULL;
    jSecurityProvisioningStatus = env->NewObject(g_cls_SecurityProvisioningStatus,
                                        g_mid_SecurityProvisioningStatus_ctor,
                                        (jint)esResult,
                                        env->NewStringUTF(secProvisioningStatus->getDeviceUUID().c_str()));

    ES_LOGE("JniSecurityStatusListener::onSecurityProvisioningStatus UUID : %s",
                                                     secProvisioningStatus->getDeviceUUID().c_str());

    if (!jSecurityProvisioningStatus)
    {
        ES_LOGE("JniSecurityStatusListener::onSecurityProvisioningStatus Unable to create the java object");
        return ;
    }

    env->CallVoidMethod(jListener, midL, jSecurityProvisioningStatus);

    if (env->ExceptionCheck())
    {
        ES_LOGE("Java exception is thrown");
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
}

void JniSecurityStatusListener::checkExAndRemoveListener(JNIEnv *env)
{
    if (env->ExceptionCheck())
    {
        jthrowable ex = env->ExceptionOccurred();
        env->ExceptionClear();
        m_ownerResource->removeStatusListener<JniSecurityStatusListener>(env, m_jwListener);
        env->Throw((jthrowable)ex);
    }
    else
    {
        m_ownerResource->removeStatusListener<JniSecurityStatusListener>(env, m_jwListener);
    }
}
