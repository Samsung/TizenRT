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

#ifndef _JNI_NOTIFICATION_COMMON_H_
#define _JNI_NOTIFICATION_COMMON_H_

#include <jni.h>
#include <android/log.h>
#include "JniSharedObjectHolder.h"


#define  NSTAG    "NotificationService_JNI"

#define JNI_CURRENT_VERSION JNI_VERSION_1_6

#define NS_LOGI(...) __android_log_print(ANDROID_LOG_INFO, NSTAG, __VA_ARGS__)
#define NS_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, NSTAG, __VA_ARGS__)
#define NS_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, NSTAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

#define JNI_EXCEPTION 1000
#define JNI_NO_NATIVE_POINTER 1001
#define JNI_INVALID_VALUE 1002
#define NATIVE_EXCEPTION 1003

#define JNI_NS_OK 100
#define JNI_NS_ERROR 200
#define JNI_NS_SUCCESS 300
#define JNI_NS_FAIL 400
#define JNI_NS_ALLOW 500
#define JNI_NS_DENY 600

jobject getNSException(JNIEnv *env, const char *file, const char *functionName, const int line,
                       const int code, const char *message);
void throwNSException(JNIEnv *env, jobject ex);

#define GetNSException(code, message) getNSException(env, __FILE__, __func__, __LINE__, code, message)
#define ThrowNSException(code, message) throwNSException(env, GetNSException(code, message))

static const char *NSResultToChar(const int nsresult);
int NSExceptionInit(JNIEnv *env);

#ifdef __cplusplus
}
#endif

#endif // _JNI_NOTIFICATION_COMMON_H_
