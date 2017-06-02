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

/** @file   JniEasySetup.h
 *
 *   @brief  This file contains native API for creating RemoteEnrollee Device
 */

#ifndef __JNI_ES_EASYSETUP_H
#define __JNI_ES_EASYSETUP_H

#include <jni.h>

#include "EasySetup.hpp"
#include "RemoteEnrollee.h"
#include "ESException.h"
#include "oic_string.h"

#include "JniJvm.h"
#include "JniEasySetup.h"
#include "JniRemoteEnrollee.h"
#include "JniEsUtils.h"

using namespace OIC::Service;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * API for creating and returning the RemoteEnrollee object to Java Layer
 *
 * @return jobject - RemoteEnrolleee
 */
JNIEXPORT jobject JNICALL
Java_org_iotivity_service_easysetup_mediator_EasySetup_nativeCreateRemoteEnrollee
(JNIEnv *env, jobject thiz, jobject jResource);


#ifdef __cplusplus
}
#endif
#endif // __JNI_ES_EASYSETUP_H
