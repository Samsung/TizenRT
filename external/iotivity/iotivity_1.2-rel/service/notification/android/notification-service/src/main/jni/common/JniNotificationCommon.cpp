//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "JniNotificationCommon.h"
#include <cstddef>
#include "JniUtils.h"

static jclass g_cls_NSException = NULL;
static jmethodID g_mid_NSException_ctor = NULL;
static jmethodID g_mid_NSException_setNativeExceptionLocation = NULL;

static const char *NSResultToChar(const int nsresult)
{
    switch (nsresult)
    {
        case JNI_NS_OK:
            return "OK";
        case JNI_NS_ERROR:
            return "ERROR";
        case JNI_NS_SUCCESS:
            return "SUCCESS";
        case JNI_NS_FAIL:
            return "FAIL";
        case JNI_NS_ALLOW:
            return "ALLOW";
        case JNI_NS_DENY:
            return "DENY";
        case JNI_EXCEPTION:
            return "JNI_EXCEPTION";
        case JNI_NO_NATIVE_POINTER:
            return "JNI_NO_NATIVE_POINTER";
        case JNI_INVALID_VALUE:
            return "JNI_INVALID_VALUE";
        case NATIVE_EXCEPTION:
            return "NATIVE_EXCEPTION";
        default:
            return "";
    }
}


jobject getNSException(JNIEnv *env, const char *file, const char *functionName,
                       const int line, const int code, const char *message)
{
    NS_LOGE ("Failed : %s" , message );
    const char *codeChar = NSResultToChar(code);
    if (codeChar[0] == '\0')
    {
        codeChar = NSResultToChar(JNI_INVALID_VALUE);
    }
    jobject exception = env->NewObject(
                            g_cls_NSException,
                            g_mid_NSException_ctor,
                            env->NewStringUTF(codeChar),
                            env->NewStringUTF(message));
    if (!exception)
    {
        return NULL;
    }
    env->CallVoidMethod(
        exception,
        g_mid_NSException_setNativeExceptionLocation,
        env->NewStringUTF(file),
        env->NewStringUTF(functionName),
        line);
    if (env->ExceptionCheck())
    {
        return NULL;
    }
    return exception;
}

void throwNSException(JNIEnv *env, jobject exception)
{
    env->Throw((jthrowable)exception);
}

int NSExceptionInit(JNIEnv *env)
{
    if (!env)
    {
        NS_LOGE ("JNIEnv is null");
        return JNI_ERR;
    }

    //OcException
    jclass localNSException = env->FindClass(
                                  "org/iotivity/service/ns/common/NSException");
    if (!localNSException)
    {
        NS_LOGE ("Failed to get local NSException");
        return JNI_ERR;
    }
    g_cls_NSException = (jclass)env->NewGlobalRef(localNSException);


    g_mid_NSException_ctor = env->GetMethodID(g_cls_NSException,
                             "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_mid_NSException_ctor)
    {
        NS_LOGE ("Failed to Get MethodID");
        return JNI_ERR;
    }

    g_mid_NSException_setNativeExceptionLocation = env->GetMethodID(g_cls_NSException,
            "setNativeExceptionLocation",
            "(Ljava/lang/String;Ljava/lang/String;I)V");
    if (!g_mid_NSException_setNativeExceptionLocation)
    {
        NS_LOGE ("Failed to Get MethodID");
        return JNI_ERR;
    }

    env->DeleteLocalRef(localNSException);

    return JNI_CURRENT_VERSION;
}
