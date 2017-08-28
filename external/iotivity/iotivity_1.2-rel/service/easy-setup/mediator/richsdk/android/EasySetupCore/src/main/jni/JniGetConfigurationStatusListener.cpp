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

#include "JniGetConfigurationStatusListener.h"
#include "JniRemoteEnrollee.h"

using namespace OC;
using namespace OIC::Service;

JniGetConfigurationStatusListener::JniGetConfigurationStatusListener(JNIEnv *env, jobject jListener,
        JniRemoteEnrollee *owner)
    : m_ownerResource(owner)
{
    m_jwListener = env->NewWeakGlobalRef(jListener);
}

JniGetConfigurationStatusListener::~JniGetConfigurationStatusListener()
{
    ES_LOGI("~JniGetConfigurationStatusListener()");
    if (m_jwListener)
    {
        jint ret;
        JNIEnv *env = GetESJNIEnv(ret);
        if (NULL == env) return;
        env->DeleteWeakGlobalRef(m_jwListener);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
    }
}

void JniGetConfigurationStatusListener::getConfigurationStatusCallback (
    std::shared_ptr<GetConfigurationStatus> getConfigurationStatusCb)
{
    ES_LOGI("JniGetConfigurationStatusListener::provisioiningStatusCallback enter");

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
                                      "GetConfigurationStatus;"
                                      ")V");

    if (!midL)
    {
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    EnrolleeConf enrolleeConf = getConfigurationStatusCb->getEnrolleeConf();
    OCRepresentation m_ProvRep = enrolleeConf.getEasySetupRep();

    OCRepresentation* rep = new OCRepresentation(m_ProvRep);
    jlong handle = reinterpret_cast<jlong>(rep);
    jobject jRepresentation = env->NewObject(g_cls_OcRepresentation, g_mid_OcRepresentation_N_ctor_bool,
                                            handle, true);
    if (!jRepresentation)
    {
        delete rep;
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret)
        {
            g_jvm->DetachCurrentThread();
        }
        return;
    }

    jobject jEnrolleeConf = NULL;
    jEnrolleeConf = env->NewObject(g_cls_EnrolleeConf, g_mid_EnrolleeConf_ctor, (jobject)jRepresentation);
    if (!jEnrolleeConf) {
        ES_LOGE("JniGetConfigurationStatusListener::getConfigurationStatusCallback Unable to create the jEnrolleeConf");
        return ;
    }

    ESResult esResult = getConfigurationStatusCb->getESResult();
    jobject jgetConfigurationStatus = NULL;
    jgetConfigurationStatus = env->NewObject(g_cls_getConfigurationStatus,
                                                g_mid_getConfigurationStatus_ctor,
                                                (jint)esResult,
                                                (jobject)jEnrolleeConf);

    if (!jgetConfigurationStatus)
    {
        ES_LOGE("JniGetConfigurationStatusListener::getConfigurationStatusCallback Unable to create the java object");
        return ;
    }

    env->CallVoidMethod(jListener, midL, jgetConfigurationStatus);

    if (env->ExceptionCheck())
    {
        ES_LOGE("Java exception is thrown");
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
}

void JniGetConfigurationStatusListener::checkExAndRemoveListener(JNIEnv *env)
{
    if (env->ExceptionCheck())
    {
        jthrowable ex = env->ExceptionOccurred();
        env->ExceptionClear();
        m_ownerResource->removeStatusListener<JniGetConfigurationStatusListener>(env, m_jwListener);

        env->Throw((jthrowable)ex);
    }
    else
    {
        m_ownerResource->removeStatusListener<JniGetConfigurationStatusListener>(env, m_jwListener);
    }
}
