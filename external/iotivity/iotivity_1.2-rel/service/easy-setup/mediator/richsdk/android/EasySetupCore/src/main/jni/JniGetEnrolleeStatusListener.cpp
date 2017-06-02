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

#include "JniGetEnrolleeStatusListener.h"
#include "JniRemoteEnrollee.h"

using namespace OC;
using namespace OIC::Service;

JniGetEnrolleeStatusListener::JniGetEnrolleeStatusListener(JNIEnv *env, jobject jListener,
        JniRemoteEnrollee *owner)
    : m_ownerResource(owner)
{
    m_jwListener = env->NewWeakGlobalRef(jListener);
}

JniGetEnrolleeStatusListener::~JniGetEnrolleeStatusListener()
{
    ES_LOGI("~JniGetEnrolleeStatusListener()");
    if (m_jwListener)
    {
        jint ret;
        JNIEnv *env = GetESJNIEnv(ret);
        if (NULL == env) return;
        env->DeleteWeakGlobalRef(m_jwListener);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
    }
}

void JniGetEnrolleeStatusListener::getEnrolleeStatusCallback (
    std::shared_ptr<GetEnrolleeStatus> getEnrolleeStatusCb)
{
    ES_LOGI("JniGetEnrolleeStatusListener::getEnrolleeStatusCallback enter");

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
                                      "GetEnrolleeStatus;"
                                      ")V");

    if (!midL)
    {
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    EnrolleeStatus enrolleeStatus = getEnrolleeStatusCb->getEnrolleeStatus();
    OCRepresentation m_Rep = enrolleeStatus.getRepresentation();
    OCRepresentation* rep = new OCRepresentation(m_Rep);
    jlong handle = reinterpret_cast<jlong>(rep);
    jobject jRepresentation = env->NewObject(g_cls_OcRepresentation,
                                            g_mid_OcRepresentation_N_ctor_bool,
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

    jobject jEnrolleeStatus = NULL;
    jEnrolleeStatus = env->NewObject(g_cls_EnrolleeStatus,
                                        g_mid_EnrolleeStatus_ctor,
                                        (jobject)jRepresentation);
    if (!jEnrolleeStatus) {
        ES_LOGE("JniGetEnrolleeStatusListener::getEnrolleeStatusCallback Unable to create the jEnrolleeStatus");
        return ;
    }

    ESResult esResult = getEnrolleeStatusCb->getESResult();
    jobject jgetEnrolleeStatus = NULL;
    jgetEnrolleeStatus = env->NewObject(g_cls_getEnrolleeStatus,
                                            g_mid_getEnrolleeStatus_ctor,
                                            (jint)esResult,
                                            (jobject)jEnrolleeStatus);

    if (!jgetEnrolleeStatus)
    {
        ES_LOGE("JniGetEnrolleeStatusListener::getEnrolleeStatusCallback Unable to create the java object");
        return ;
    }

    env->CallVoidMethod(jListener, midL, jgetEnrolleeStatus);

    if (env->ExceptionCheck())
    {
        ES_LOGE("Java exception is thrown");
        checkExAndRemoveListener(env);
        if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
        return;
    }

    if (JNI_EDETACHED == ret) g_jvm->DetachCurrentThread();
}

void JniGetEnrolleeStatusListener::checkExAndRemoveListener(JNIEnv *env)
{
    if (env->ExceptionCheck())
    {
        jthrowable ex = env->ExceptionOccurred();
        env->ExceptionClear();
        m_ownerResource->removeStatusListener<JniGetEnrolleeStatusListener>
                                                            (env, m_jwListener);

        env->Throw((jthrowable)ex);
    }
    else
    {
        m_ownerResource->removeStatusListener<JniGetEnrolleeStatusListener>
                                                            (env, m_jwListener);
    }
}

