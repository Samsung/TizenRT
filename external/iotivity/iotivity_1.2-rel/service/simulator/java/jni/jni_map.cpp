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

#include "jni_map.h"
#include "simulator_utils_jni.h"

JniMap::JniMap(JNIEnv *env)
{
    m_env = env;

    static jmethodID mapCtor = env->GetMethodID(
                                   gSimulatorClassRefs.hashMapCls, "<init>", "()V");
    m_hashMap = env->NewObject(gSimulatorClassRefs.hashMapCls, mapCtor);
}

void JniMap::put(jobject jKey, jobject jValue)
{
    static jmethodID putMethod = m_env->GetMethodID(gSimulatorClassRefs.hashMapCls,
                                 "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    m_env->CallObjectMethod(m_hashMap, putMethod, jKey, jValue);
}

jobject JniMap::get()
{
    return m_hashMap;
}
