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

#ifndef __JNI_ES_GET_ENROLLEE_STATUS_LISTENER_H_
#define __JNI_ES_GET_ENROLLEE_STATUS_LISTENER_H_

#include <jni.h>

#include "OCPlatform.h"
#include "RemoteEnrollee.h"
#include "ESRichCommon.h"

#include "JniJvm.h"

class JniRemoteEnrollee;

using namespace OIC::Service;

class JniGetEnrolleeStatusListener
{
    public:
        /**
         * @brief constructor
         */
        JniGetEnrolleeStatusListener(JNIEnv *env, jobject jListener, JniRemoteEnrollee *resource);

        /**
         * @brief destructor
        */
        ~JniGetEnrolleeStatusListener();

        /**
         * @brief callback function that will be passed to Native layer
        */
        void getEnrolleeStatusCallback (std::shared_ptr<GetEnrolleeStatus> getEnrolleeStatus);

    private:
        jweak m_jwListener;
        JniRemoteEnrollee *m_ownerResource;
        void checkExAndRemoveListener(JNIEnv *env);
};

#endif //__JNI_ES_GET_CONFIGURATION_STATUS_LISTENER_H_

