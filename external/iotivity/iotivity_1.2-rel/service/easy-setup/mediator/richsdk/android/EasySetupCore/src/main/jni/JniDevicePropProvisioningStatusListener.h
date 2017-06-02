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

#ifndef __JNI_ES_DEVICE_PROP_PROVISIONING_STATUS_LISTENER_H_
#define __JNI_ES_DEVICE_PROP_PROVISIONING_STATUS_LISTENER_H_

#include <jni.h>

#include "RemoteEnrollee.h"
#include "ESRichCommon.h"

#include "JniJvm.h"

class JniRemoteEnrollee;

using namespace OIC::Service;

class JniDevicePropProvisioningStatusListener
{
    public:
        /**
         * @brief constructor
         */
        JniDevicePropProvisioningStatusListener(JNIEnv *env, jobject jListener, JniRemoteEnrollee *resource);

        /**
         * @brief destructor
        */
        ~JniDevicePropProvisioningStatusListener();

        /**
         * @brief callback function that will be passed to Native layer
        */
        void onDevicePropProvisioningStatusCallback (std::shared_ptr<DevicePropProvisioningStatus> devicePropProvisioningStatus);

    private:
        jweak m_jwListener;
        JniRemoteEnrollee *m_ownerResource;
        void checkExAndRemoveListener(JNIEnv *env);
};

#endif //__JNI_ES_DEVICE_PROP_PROVISIONING_STATUS_LISTENER_H_
