/******************************************************************
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

#include "jni_queryparam.h"
#include "jni_string.h"
#include "simulator_utils_jni.h"

JniQueryParameter::JniQueryParameter(JNIEnv *env) : m_env(env) {}

std::map<std::string, std::string> JniQueryParameter::toCpp(jobject jQueryParams)
{
    std::map<std::string, std::string> result;
    if (!jQueryParams)
        return result;

    static jmethodID entrySetMethodID = m_env->GetMethodID(gSimulatorClassRefs.mapCls, "entrySet",
                                        "()Ljava/util/Set;");
    static jmethodID iteratorMethodID = m_env->GetMethodID(gSimulatorClassRefs.setCls, "iterator",
                                        "()Ljava/util/Iterator;");
    static jmethodID hasNextMethodID = m_env->GetMethodID(gSimulatorClassRefs.iteratorCls, "hasNext",
                                       "()Z");
    static jmethodID nextMethodID = m_env->GetMethodID(gSimulatorClassRefs.iteratorCls, "next",
                                    "()Ljava/lang/Object;");
    static jmethodID getKeyMethodID = m_env->GetMethodID(gSimulatorClassRefs.mapEntryCls, "getKey",
                                      "()Ljava/lang/Object;");
    static jmethodID getValueMethodID = m_env->GetMethodID(gSimulatorClassRefs.mapEntryCls, "getValue",
                                        "()Ljava/lang/Object;");

    jobject entrySet = m_env->CallObjectMethod(jQueryParams, entrySetMethodID);
    jobject iterator = m_env->CallObjectMethod(entrySet, iteratorMethodID);
    if (!entrySet || !iterator || m_env->ExceptionCheck())
        return result;

    while (m_env->CallBooleanMethod(iterator, hasNextMethodID))
    {
        jobject entry = m_env->CallObjectMethod(iterator, nextMethodID);
        jstring key = (jstring) m_env->CallObjectMethod(entry, getKeyMethodID);
        jstring value = (jstring) m_env->CallObjectMethod(entry, getValueMethodID);

        JniString jniKey(m_env, key);
        JniString jniValue(m_env, value);

        result[jniKey.get()] = jniValue.get();

        m_env->DeleteLocalRef(entry);
        m_env->DeleteLocalRef(key);
        m_env->DeleteLocalRef(value);
    }

    return result;
}
