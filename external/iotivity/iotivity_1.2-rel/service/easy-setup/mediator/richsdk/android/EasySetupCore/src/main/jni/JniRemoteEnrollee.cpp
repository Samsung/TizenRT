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

#include "JniRemoteEnrollee.h"

#include "JniOcRepresentation.h"

using namespace OIC::Service;

JniRemoteEnrollee::JniRemoteEnrollee(std::shared_ptr<RemoteEnrollee> remoteEnrollee)
    : m_sharedResource(remoteEnrollee) {}

JniRemoteEnrollee::~JniRemoteEnrollee()
{
    ES_LOGD("JniRemoteEnrollee::~JniRemoteEnrollee()");
    m_sharedResource = NULL;

    jint envRet;
    JNIEnv *env = GetESJNIEnv(envRet);
    if (NULL == env) return;

    if (JNI_EDETACHED == envRet) g_jvm->DetachCurrentThread();
}

JniRemoteEnrollee *JniRemoteEnrollee::getJniRemoteEnrollee(JNIEnv *env, jobject thiz)
{
    JniRemoteEnrollee *remoteEnrollee = ESGetHandle<JniRemoteEnrollee>(env, thiz);
    if (env->ExceptionCheck())
    {
        ES_LOGE("getJniRemoteEnrollee :: Failed to get native handle from RemoteEnrollee object");
    }
    if (!remoteEnrollee)
    {
        ES_LOGE("getJniRemoteEnrollee :: no resource");
    }
    return remoteEnrollee;
}

void JniRemoteEnrollee::getStatus(JNIEnv *env, jobject jListener)
{
    JniGetEnrolleeStatusListener *onGetEnrolleeStatusReceived =
                    addStatusListener<JniGetEnrolleeStatusListener>(env, jListener);

    GetStatusCb getEnrolleeStatusCallback = [onGetEnrolleeStatusReceived]
            (std::shared_ptr<OIC::Service::GetEnrolleeStatus > getEnrolleeStatus)
    {
        onGetEnrolleeStatusReceived->getEnrolleeStatusCallback(getEnrolleeStatus);
    };

    try
    {
        m_sharedResource->getStatus(getEnrolleeStatusCallback);
    }
    catch (ESBadRequestException exception)
    {
        ES_LOGE("JNI getStatus :: Exception occured");
        //throw the exception to java
        throwESException( env,  exception.what());
    }
}

void JniRemoteEnrollee::getConfiguration(JNIEnv *env, jobject jListener)
{
    JniGetConfigurationStatusListener *onGetConfigurationStatusReceived =
                    addStatusListener<JniGetConfigurationStatusListener>(env, jListener);

    GetConfigurationStatusCb getConfigurationStatusCallback = [onGetConfigurationStatusReceived]
            (std::shared_ptr<OIC::Service::GetConfigurationStatus > getConfigurationStatus)
    {
        onGetConfigurationStatusReceived->getConfigurationStatusCallback(getConfigurationStatus);
    };

    try
    {
        m_sharedResource->getConfiguration(getConfigurationStatusCallback);
    }
    catch (ESBadRequestException exception)
    {
        ES_LOGE("JNI getConfiguration :: Exception occured");
        //throw the exception to java
        throwESException( env,  exception.what());
    }
}

void JniRemoteEnrollee::provisionSecurity(JNIEnv *env, jobject jListener)
{
    JniSecurityStatusListener *onSecurityProvStatusReceived =
                    addStatusListener<JniSecurityStatusListener>(env, jListener);

    SecurityProvStatusCb secProvStatusCallback = [onSecurityProvStatusReceived]
            (std::shared_ptr<OIC::Service::SecProvisioningStatus > SecProvisioningStatus)
    {
        onSecurityProvStatusReceived->secProvisionStatusCallback(SecProvisioningStatus);
    };

    try
    {
        m_sharedResource->provisionSecurity(secProvStatusCallback);
    }
    catch (ESBadRequestException exception)
    {
        ES_LOGE("JNI provisionSecurity :: Exception occured");
        //throw the exception to java
        throwESException( env,  exception.what());
    }
}

void JniRemoteEnrollee::provisionDeviceProperties(JNIEnv *env,
                                                    jobject jRepresentation,
                                                    jobject jListener)
{
    OCRepresentation *representation = JniOcRepresentation::getOCRepresentationPtr(env, jRepresentation);
    if (!representation)
    {
        return;
    }

    DeviceProp deviceProp(*representation);
    JniDevicePropProvisioningStatusListener *onDevicePropProvStatusReceived =
                    addStatusListener<JniDevicePropProvisioningStatusListener>(env, jListener);

    DevicePropProvStatusCb devicePropProvStatusCallback = [onDevicePropProvStatusReceived]
            (std::shared_ptr<OIC::Service::DevicePropProvisioningStatus > devicePropProvisioningStatus)
    {
        onDevicePropProvStatusReceived->onDevicePropProvisioningStatusCallback(devicePropProvisioningStatus);
    };

    try
    {
        m_sharedResource->provisionDeviceProperties(deviceProp, devicePropProvStatusCallback);
    }
    catch (ESBadRequestException exception)
    {
        ES_LOGE("JNI provisionDeviceProperties :: Exception occured");
        //throw the exception to java
        throwESException( env,  exception.what());
    }
}

void JniRemoteEnrollee::provisionCloudProperties(JNIEnv *env,
                                                jobject jRepresentation,
                                                jstring jCloudID,
                                                jint jCredID,
                                                jobject jListener)
{
    OCRepresentation *representation = JniOcRepresentation::getOCRepresentationPtr(env, jRepresentation);
    if (!representation)
    {
        return;
    }
    if (!jCloudID)
    {
        return;
    }

    CloudProp cloudProp(*representation);
    cloudProp.setCloudID(env->GetStringUTFChars(jCloudID, NULL));
    cloudProp.setCredID(jCredID);

    JniCloudPropProvisioningStatusListener *onCloudPropProvisioningStatusReceived =
                    addStatusListener<JniCloudPropProvisioningStatusListener>(env, jListener);

    CloudPropProvStatusCb cloudPropProvStatusCallback = [onCloudPropProvisioningStatusReceived]
            (std::shared_ptr< OIC::Service::CloudPropProvisioningStatus > cloudPropProvisioningStatus)

    {
        onCloudPropProvisioningStatusReceived->onCloudPropProvisioningStatus(cloudPropProvisioningStatus);
    };

    try
    {
        m_sharedResource->provisionCloudProperties(cloudProp, cloudPropProvStatusCallback);
    }
    catch (ESBadRequestException exception)
    {
        ES_LOGE("JNI startProvisioning :: Exception occured");
        //throw the exception to java
        throwESException(env, exception.what());
    }
}

//JNI
JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeGetStatus
(JNIEnv *env, jobject jClass, jobject jListener)
{
    ES_LOGD("nativeGetStatus Enter");

    JniRemoteEnrollee *remoteEnrollee = JniRemoteEnrollee::getJniRemoteEnrollee(env, jClass);
    remoteEnrollee->getStatus(env, jListener);

    ES_LOGD("nativeGetStatus Exit");
}

//JNI
JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeGetConfiguration
(JNIEnv *env, jobject jClass, jobject jListener)
{
    ES_LOGD("nativegetConfiguration Enter");

    JniRemoteEnrollee *remoteEnrollee = JniRemoteEnrollee::getJniRemoteEnrollee(env, jClass);
    remoteEnrollee->getConfiguration(env, jListener);

    ES_LOGD("nativegetConfiguration Exit");
}

JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeProvisionSecurity
(JNIEnv *env, jobject jClass, jobject jListener)
{
    ES_LOGD("nativeStartSecurityProvision Enter");

    JniRemoteEnrollee *remoteEnrollee = JniRemoteEnrollee::getJniRemoteEnrollee(env, jClass);
    remoteEnrollee->provisionSecurity(env, jListener);

    ES_LOGD("nativeStartSecurityProvision Exit");
}

JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeProvisionDeviceProperties
(JNIEnv *env, jobject jClass, jobject jRepresentation, jobject jListener)
{
    ES_LOGD("nativeProvisionDeviceProperties Enter");

    JniRemoteEnrollee *remoteEnrollee = JniRemoteEnrollee::getJniRemoteEnrollee(env, jClass);
    remoteEnrollee->provisionDeviceProperties(env, jRepresentation, jListener);

    ES_LOGD("nativeProvisionDeviceProperties Exit");
}

JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeProvisionCloudProperties
(JNIEnv *env, jobject jClass, jobject jRepresentation, jstring jCloudID, jint jCredID, jobject jListener)
{
    ES_LOGD("nativeprovisionCloudProperties Enter");

    JniRemoteEnrollee *remoteEnrollee = JniRemoteEnrollee::getJniRemoteEnrollee(env, jClass);
    remoteEnrollee->provisionCloudProperties(env, jRepresentation, jCloudID, jCredID, jListener);

    ES_LOGD("nativeprovisionCloudProperties Exit");
}
