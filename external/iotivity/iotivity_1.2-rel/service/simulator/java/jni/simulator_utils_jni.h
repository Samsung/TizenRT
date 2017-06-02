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

#ifndef SIMULATOR_UTILS_JNI_H_
#define SIMULATOR_UTILS_JNI_H_

#include <jni.h>
#include "simulator_error_codes.h"

typedef struct
{
    jclass objectCls;
    jclass integerCls;
    jclass doubleCls;
    jclass booleanCls;
    jclass stringCls;
    jclass hashMapCls;
    jclass vectorCls;
    jclass mapCls;
    jclass mapEntryCls;
    jclass setCls;
    jclass iteratorCls;

    jclass simulatorResourceCls;
    jclass simulatorResourceTypeCls;
    jclass simulatorSingleResourceCls;
    jclass simulatorCollectionResourceCls;
    jclass simulatorResourceModelCls;
    jclass simulatorResourceAttributeCls;
    jclass attributeValueCls;
    jclass attributeValueTypeCls;
    jclass attributeTypeInfoCls;
    jclass attributePropertyCls;
    jclass attributePropertyTypeCls;
    jclass integerPropertyCls;
    jclass integerPropertyBuilderCls;
    jclass doublePropertyCls;
    jclass doublePropertyBuilderCls;
    jclass booleanPropertyCls;
    jclass booleanPropertyBuilderCls;
    jclass stringPropertyCls;
    jclass stringPropertyBuilderCls;
    jclass arrayPropertyCls;
    jclass arrayPropertyBuilderCls;
    jclass modelPropertyCls;
    jclass autoUpdateTypeCls;

    jclass simulatorRemoteResourceCls;
    jclass requestTypeCls;
    jclass observerCls;
    jclass deviceInfoCls;
    jclass platformInfoCls;
    jclass simulatorRequestModelCls;

    jclass simulatorExceptionCls;
    jclass invalidArgsExceptionCls;
    jclass noSupportExceptionCls;
    jclass operationInProgressExceptionCls;
    jclass simulatorResultCls;

    jclass integer1DArrayCls;
    jclass integer2DArrayCls;
    jclass integer3DArrayCls;
    jclass double1DArrayCls;
    jclass double2DArrayCls;
    jclass double3DArrayCls;
    jclass boolean1DArrayCls;
    jclass boolean2DArrayCls;
    jclass boolean3DArrayCls;
    jclass string1DArrayCls;
    jclass string2DArrayCls;
    jclass string3DArrayCls;
    jclass simulatorResModel1DArrayCls;
    jclass simulatorResModel2DArrayCls;
    jclass simulatorResModel3DArrayCls;
} SimulatorClassRefs;

#define VALIDATE_INPUT(ENV, CONDITION, MSG) if (CONDITION) {ThrowInvalidArgsException(ENV, SIMULATOR_INVALID_PARAM, MSG); return;}
#define VALIDATE_INPUT_RET(ENV, CONDITION, MSG, RET) if (CONDITION) {ThrowInvalidArgsException(ENV, SIMULATOR_INVALID_PARAM, MSG); return RET;}

#define VALIDATE_CALLBACK(ENV, CALLBACK) if (!CALLBACK){ThrowInvalidArgsException(env, SIMULATOR_INVALID_CALLBACK, "Invalid callback!"); return;}
#define VALIDATE_CALLBACK_RET(ENV, CALLBACK, RET) if (!CALLBACK){ThrowInvalidArgsException(env, SIMULATOR_INVALID_CALLBACK, "Invalid callback!"); return RET;}

static jfieldID getHandleField(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    return env->GetFieldID(cls, "mNativeHandle", "J");
}

template <typename T>
static T *getHandle(JNIEnv *env, jobject jobj)
{
    jlong handle = env->GetLongField(jobj, getHandleField(env, jobj));
    return reinterpret_cast<T *>(handle);
}

template <typename T>
static void setHandle(JNIEnv *env, jobject jobj, T *type)
{
    jlong handle = reinterpret_cast<jlong>(type);
    env->SetLongField(jobj, getHandleField(env, jobj), handle);
}

extern JNIEnv *GetEnv();
extern void ReleaseEnv();
extern SimulatorClassRefs gSimulatorClassRefs;

jobject SimulatorResultToJava(JNIEnv *env, SimulatorResult errorCode);

#endif
