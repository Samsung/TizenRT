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

#include "simulator_exceptions_jni.h"
#include "simulator_utils_jni.h"

extern SimulatorClassRefs gSimulatorClassRefs;

void ThrowSimulatorException(JNIEnv *env, SimulatorResult code, const char *message)
{
    static jmethodID simulatorExceptionCtor = env->GetMethodID(
                gSimulatorClassRefs.simulatorExceptionCls, "<init>", "(ILjava/lang/String;)V");

    jobject exceptionObject = env->NewObject(gSimulatorClassRefs.simulatorExceptionCls,
                              simulatorExceptionCtor, code, env->NewStringUTF(message));
    if (exceptionObject)
    {
        env->Throw((jthrowable) exceptionObject);
    }
}

void ThrowInvalidArgsException(JNIEnv *env, SimulatorResult code, const char *message)
{
    static jmethodID invalidArgsExceptionCtor = env->GetMethodID(
                gSimulatorClassRefs.invalidArgsExceptionCls, "<init>", "(ILjava/lang/String;)V");

    jobject exceptionObject = env->NewObject(gSimulatorClassRefs.invalidArgsExceptionCls,
                              invalidArgsExceptionCtor, code, env->NewStringUTF(message));
    if (exceptionObject)
    {
        env->Throw((jthrowable) exceptionObject);
    }
}

void ThrowNoSupportException(JNIEnv *env, const char *message)
{
    static jmethodID noSupportExceptionCtor = env->GetMethodID(
                gSimulatorClassRefs.noSupportExceptionCls, "<init>", "(Ljava/lang/String;)V");

    jobject exceptionObject = env->NewObject(gSimulatorClassRefs.noSupportExceptionCls,
                              noSupportExceptionCtor, env->NewStringUTF(message));
    if (exceptionObject)
    {
        env->Throw((jthrowable) exceptionObject);
    }
}

void ThrowOperationInProgressException(JNIEnv *env, const char *message)
{
    static jmethodID operationInProgressExceptionCtor = env->GetMethodID(
                gSimulatorClassRefs.operationInProgressExceptionCls, "<init>", "(Ljava/lang/String;)V");

    jobject exceptionObject = env->NewObject(gSimulatorClassRefs.operationInProgressExceptionCls,
                              operationInProgressExceptionCtor, env->NewStringUTF(message));
    if (exceptionObject)
    {
        env->Throw((jthrowable) exceptionObject);
    }
}

void ThrowBadObjectException(JNIEnv *env, const char *message)
{
    static jmethodID simulatorExceptionCtor = env->GetMethodID(
                gSimulatorClassRefs.simulatorExceptionCls, "<init>", "(ILjava/lang/String;)V");

    jobject exceptionObject = env->NewObject(gSimulatorClassRefs.simulatorExceptionCls,
                              simulatorExceptionCtor, SIMULATOR_BAD_OBJECT, env->NewStringUTF(message));
    if (exceptionObject)
    {
        env->Throw((jthrowable) exceptionObject);
    }
}