/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include "JniCloudPropProvisioningStatusListener.h"
#include "JniRemoteEnrollee.h"

using namespace OIC::Service;

JniCloudPropProvisioningStatusListener::JniCloudPropProvisioningStatusListener(JNIEnv *env, jobject jListener,
        JniRemoteEnrollee *owner)
    : m_ownerResource(owner)
{
    m_jwListener = env->NewWeakGlobalRef(jListener);
}

JniCloudPropProvisioningStatusListener::~JniCloudPropProvisioningStatusListener()
{
    ES_LOGI("~JniCloudPropProvisioningStatusListener()");
    if (m_jwListener)
    {
        jint ret;
        JNIEnv *env = GetESJNIEnv(ret);
        if (NULL == env) return;
        env->DeleteWeakGlobalRef(m_jwListener);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
    }
}

void JniCloudPropProvisioningStatusListener::onCloudPropProvisioningStatus(std::shared_ptr<CloudPropProvisioningStatus>
        cloudPropProvisioningStatus)
{

    ES_LOGI("JniCloudPropProvisioningStatusListener::onCloudPropProvisioningStatus enter");

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
                                      "CloudPropProvisioningStatus;"
                                      ")V");
    if (!midL)
    {
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    ESResult esResult = cloudPropProvisioningStatus->getESResult();

    //create the java object
    jobject jCloudPropProvisioningStatus = NULL;
    jCloudPropProvisioningStatus = env->NewObject(g_cls_CloudPropProvisioningStatus,
                                                g_mid_CloudPropProvisioningStatus_ctor,
                                                (jint)esResult);

    ES_LOGI("JniCloudPropProvisioningStatus::onCloudPropProvisioningStatus - %d", esResult);
    if (!jCloudPropProvisioningStatus)
    {
        ES_LOGE("JniCloudPropProvisioningStatus::onCloudPropProvisioningStatus Unable to create the java object");
        return ;
    }

    env->CallVoidMethod(jListener, midL, jCloudPropProvisioningStatus);

    bool needRemoveListener = false;

    if(esResult == ES_OK)
    {
        needRemoveListener = true;
    }

    if (env->ExceptionCheck())
    {
        ES_LOGE("Java exception is thrown");
        if(needRemoveListener)
            checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
}

void JniCloudPropProvisioningStatusListener::checkExAndRemoveListener(JNIEnv *env)
{
    if (env->ExceptionCheck())
    {
        jthrowable ex = env->ExceptionOccurred();
        env->ExceptionClear();
        m_ownerResource->removeStatusListener<JniCloudPropProvisioningStatusListener>(env, m_jwListener);
        env->Throw((jthrowable)ex);
    }
    else
    {
        m_ownerResource->removeStatusListener<JniCloudPropProvisioningStatusListener>(env, m_jwListener);
    }
}
