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
#include "JniJvm.h"

JavaVM *g_jvm = NULL;

jclass g_cls_RemoteEnrollee = NULL;
jclass g_cls_ESException = NULL;
jclass g_cls_EnrolleeStatus = NULL;
jclass g_cls_EnrolleeConf = NULL;
jclass g_cls_getEnrolleeStatus = NULL;
jclass g_cls_getConfigurationStatus = NULL;
jclass g_cls_SecurityProvisioningStatus = NULL;
jclass g_cls_DevicePropProvisioningStatus = NULL;
jclass g_cls_CloudPropProvisioningStatus = NULL;
jclass g_cls_Integer = NULL;
jclass g_cls_OcRepresentation = NULL;

jmethodID g_mid_RemoteEnrollee_ctor = NULL;
jmethodID g_mid_ESException_ctor = NULL;
jmethodID g_mid_EnrolleeStatus_ctor = NULL;
jmethodID g_mid_EnrolleeConf_ctor = NULL;
jmethodID g_mid_getEnrolleeStatus_ctor = NULL;
jmethodID g_mid_getConfigurationStatus_ctor = NULL;
jmethodID g_mid_SecurityProvisioningStatus_ctor = NULL;
jmethodID g_mid_DevicePropProvisioningStatus_ctor = NULL;
jmethodID g_mid_CloudPropProvisioningStatus_ctor = NULL;
jmethodID g_mid_Integer_ctor = NULL;
jmethodID g_mid_OcRepresentation_N_ctor_bool = NULL;

// JNI OnLoad
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    ES_LOGI("JNI_OnLoad");
    JNIEnv *env;
    g_jvm = vm;

    if (g_jvm->GetEnv((void **)&env, JNI_CURRENT_VERSION) != JNI_OK)
    {
        ES_LOGE("Failed to get the environment using GetEnv()");
        return JNI_ERR;
    }

    jclass clazz = nullptr;

    // Remote Enrollee
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/RemoteEnrollee");
    if (!clazz) return JNI_ERR;
    g_cls_RemoteEnrollee = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_RemoteEnrollee_ctor = env->GetMethodID(g_cls_RemoteEnrollee, "<init>",
                                "(J)V");
    if (!g_mid_RemoteEnrollee_ctor) return JNI_ERR;

    // ESException
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/ESException");
    if (!clazz) return JNI_ERR;
    g_cls_ESException = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_ESException_ctor = env->GetMethodID(g_cls_ESException, "<init>", "(Ljava/lang/String;)V");
    if (!g_mid_ESException_ctor) return JNI_ERR;

    // EnrolleeStatus
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/EnrolleeStatus");
    if (!clazz) return JNI_ERR;

    g_cls_EnrolleeStatus = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_EnrolleeStatus_ctor = env->GetMethodID(g_cls_EnrolleeStatus, "<init>",
                                "(Lorg/iotivity/base/OcRepresentation;)V");
    if (!g_mid_EnrolleeStatus_ctor) return JNI_ERR;

    // EnrolleeConf
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/EnrolleeConf");
    if (!clazz) return JNI_ERR;

    g_cls_EnrolleeConf = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_EnrolleeConf_ctor = env->GetMethodID(g_cls_EnrolleeConf, "<init>",
                                "(Lorg/iotivity/base/OcRepresentation;)V");
    if (!g_mid_EnrolleeConf_ctor) return JNI_ERR;

    // getEnrolleeStatus
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/GetEnrolleeStatus");
    if (!clazz) return JNI_ERR;

    g_cls_getEnrolleeStatus = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_getEnrolleeStatus_ctor = env->GetMethodID(g_cls_getEnrolleeStatus, "<init>",
                                "(ILorg/iotivity/service/easysetup/mediator/EnrolleeStatus;)V");
    if (!g_mid_getEnrolleeStatus_ctor) return JNI_ERR;

    // getConfigurationStatus
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/GetConfigurationStatus");
    if (!clazz) return JNI_ERR;

    g_cls_getConfigurationStatus = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_getConfigurationStatus_ctor = env->GetMethodID(g_cls_getConfigurationStatus, "<init>",
                                "(ILorg/iotivity/service/easysetup/mediator/EnrolleeConf;)V");
    if (!g_mid_getConfigurationStatus_ctor) return JNI_ERR;

    // SecurityProvisioningStatus
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/SecurityProvisioningStatus");
    if (!clazz) return JNI_ERR;

    g_cls_SecurityProvisioningStatus = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_SecurityProvisioningStatus_ctor = env->GetMethodID(g_cls_SecurityProvisioningStatus, "<init>",
                                "(ILjava/lang/String;)V");
    if (!g_mid_SecurityProvisioningStatus_ctor) return JNI_ERR;

    // DevicePropProvisioningStatus
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/DevicePropProvisioningStatus");
    if (!clazz) return JNI_ERR;

    g_cls_DevicePropProvisioningStatus = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_DevicePropProvisioningStatus_ctor = env->GetMethodID(g_cls_DevicePropProvisioningStatus, "<init>",
                                "(I)V");
    if (!g_mid_DevicePropProvisioningStatus_ctor) return JNI_ERR;

    // CloudPropProvisioningStatus
    clazz = env->FindClass("org/iotivity/service/easysetup/mediator/CloudPropProvisioningStatus");
    if (!clazz) return JNI_ERR;
    g_cls_CloudPropProvisioningStatus = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_CloudPropProvisioningStatus_ctor = env->GetMethodID(g_cls_CloudPropProvisioningStatus, "<init>",
                                "(I)V");
    if (!g_mid_CloudPropProvisioningStatus_ctor) return JNI_ERR;

    // Integer
    clazz = env->FindClass("java/lang/Integer");
    if (!clazz) return JNI_ERR;
    g_cls_Integer = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_Integer_ctor = env->GetMethodID(g_cls_Integer, "<init>",
                                "(I)V");
    if (!g_mid_Integer_ctor) return JNI_ERR;

    //OcRepresentation
    clazz = env->FindClass("org/iotivity/base/OcRepresentation");
    if (!clazz) return JNI_ERR;
    g_cls_OcRepresentation = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);

    g_mid_OcRepresentation_N_ctor_bool = env->GetMethodID(g_cls_OcRepresentation, "<init>", "(JZ)V");
    if (!g_mid_OcRepresentation_N_ctor_bool) return JNI_ERR;

    return JNI_CURRENT_VERSION;
}

//JNI OnUnload
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    ES_LOGI("JNI_OnUnload");
    JNIEnv *env;

    if (g_jvm->GetEnv((void **)&env, JNI_CURRENT_VERSION) != JNI_OK)
    {
        ES_LOGE("Failed to get the environment using GetEnv()");
        return;
    }
    env->DeleteGlobalRef(g_cls_RemoteEnrollee);
    env->DeleteGlobalRef(g_cls_ESException);
}
