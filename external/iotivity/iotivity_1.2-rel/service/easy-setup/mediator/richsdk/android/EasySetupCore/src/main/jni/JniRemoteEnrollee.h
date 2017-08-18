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

/** @file   JniRemoteEnrollee.h
 *
 *   @brief  This file contains the JniRemoteEnrollee class
 *               & declaration of RemoteEnrollee APIs for JNI implementation
 */

#ifndef __JNI_ES_REMOTEENROLLEE_H
#define __JNI_ES_REMOTEENROLLEE_H

#include "ESRichCommon.h"
#include "ESException.h"
#include "RemoteEnrollee.h"

#include "JniJvm.h"
#include "JniEsUtils.h"
#include "JniGetEnrolleeStatusListener.h"
#include "JniGetConfigurationStatusListener.h"
#include "JniSecurityStatusListener.h"
#include "JniDevicePropProvisioningStatusListener.h"
#include "JniCloudPropProvisioningStatusListener.h"
#include "JniEsListenerManager.h"

using namespace OIC::Service;

/**
 * @class   JniRemoteEnrollee
 * @brief   This class contains all the APIs for RemoteEnrollee
 *
 * NOTE: JNI APIs internally call the APIs of this class.
 */
class JniRemoteEnrollee
{
    public:

        /**
        *@brief constructor
        */
        JniRemoteEnrollee(std::shared_ptr< RemoteEnrollee> remoteEnrollee);

        /**
         *@brief destructor
        */
        ~JniRemoteEnrollee();

        // ***** JNI APIs internally call the APIs of this class ***** //
        void getStatus(JNIEnv *env, jobject jListener);
        void getConfiguration(JNIEnv *env, jobject jListener);
        void provisionSecurity(JNIEnv *env, jobject jListener);
        void provisionDeviceProperties(JNIEnv *env, jobject jRepresentation, jobject jListener);
        void provisionCloudProperties(JNIEnv *env, jobject jRepresentation,
                                                            jstring jCloudID,
                                                            jint jCredID,
                                                            jobject jListener);

        static JniRemoteEnrollee *getJniRemoteEnrollee(JNIEnv *env, jobject thiz);

        template <class T>
        T *addStatusListener(JNIEnv *env, jobject jListener)
        {
            return JniEsListenerManager<T>().addListener(env, jListener, this);
        };

        template <class T>
        void removeStatusListener(JNIEnv *env, jobject jListener)
        {
            JniEsListenerManager<T>().removeListener(env, jListener);
        };

    private:
        std::shared_ptr<RemoteEnrollee> m_sharedResource;

};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * API for starting the Request Enrollee status process.
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeGetStatus
(JNIEnv *env, jobject jClass, jobject jListener);

/**
 * API for starting the Request EnrolleeConf process.
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeGetConfiguration
(JNIEnv *env, jobject jClass, jobject jListener);

/**
 * API for starting the Sequrity provisioning process.
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeProvisionSecurity
(JNIEnv *env, jobject jClass, jobject jListener);

/**
 * API for starting the Data provisioning process.
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeProvisionDeviceProperties
(JNIEnv *env, jobject jClass, jobject jRepresentation, jobject jListener);

/**
 * API for starting the cloud provisioning process.
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_easysetup_mediator_RemoteEnrollee_nativeProvisionCloudProperties
(JNIEnv *env, jobject jClass, jobject jRepresentation, jstring jCloudID, jint jCredID, jobject jListener);

#ifdef __cplusplus
}
#endif
#endif // __JNI_ES_REMOTEENROLLEE_H
