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

#include "jni_vector.h"
#include "jni_string.h"
#include "simulator_utils_jni.h"

JniVector::JniVector(JNIEnv *env) : m_env(env) {}

jobject JniVector::toJava()
{
    static jmethodID vectorCtor = m_env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                  "<init>", "()V");
    return m_env->NewObject(gSimulatorClassRefs.vectorCls, vectorCtor);
}

jobject JniVector::toJava(std::vector<std::string> &cppVector)
{
    static jmethodID addMethod = m_env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                 "add", "(Ljava/lang/Object;)Z");

    jobject vectorObject = toJava();
    if (!vectorObject)
        return nullptr;

    for (size_t index = 0; index < cppVector.size(); index++)
    {
        jstring element = m_env->NewStringUTF(cppVector[index].c_str());
        m_env->CallBooleanMethod(vectorObject, addMethod, element);
        m_env->DeleteLocalRef(element);
    }

    return vectorObject;
}

int JniVector::getSize(jobject jVector)
{
    static jmethodID sizeMethodID = m_env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                    "size", "()I");
    return m_env->CallIntMethod(jVector, sizeMethodID);
}

void JniVector::addElementsCpp(jobject vector, int size, std::vector<int> &result)
{
    static jmethodID getMethod = m_env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                 "get", "(I)Ljava/lang/Object;");
    static jmethodID intValueMethod = m_env->GetMethodID(gSimulatorClassRefs.integerCls,
                                      "intValue", "()I");

    for (int index = 0; index < size; index++)
    {
        jobject intObject = m_env->CallObjectMethod(vector, getMethod, index);
        int value = m_env->CallIntMethod(intObject, intValueMethod);
        result.push_back(value);
    }
}

void JniVector::addElementsCpp(jobject vector, int size, std::vector<double> &result)
{
    static jmethodID getMethod = m_env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                 "get", "(I)Ljava/lang/Object;");
    static jmethodID doubleValueMethod = m_env->GetMethodID(gSimulatorClassRefs.doubleCls,
                                         "doubleValue", "()D");

    for (int index = 0; index < size; index++)
    {
        jobject doubleObject = m_env->CallObjectMethod(vector, getMethod, index);
        double value = m_env->CallDoubleMethod(doubleObject, doubleValueMethod);
        result.push_back(value);
    }
}

void JniVector::addElementsCpp(jobject vector, int size, std::vector<std::string> &result)
{
    static jmethodID getMethodID = m_env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                   "get", "(I)Ljava/lang/Object;");

    for (int index = 0; index < size; index++)
    {
        jstring stringObject = (jstring) m_env->CallObjectMethod(vector, getMethodID, index);
        JniString value(m_env, stringObject);
        result.push_back(value.get());
    }
}