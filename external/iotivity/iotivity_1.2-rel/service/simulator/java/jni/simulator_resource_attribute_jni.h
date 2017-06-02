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

#ifndef SIMULATOR_RESOURCE_ATTRIBUTE_JNI_H_
#define SIMULATOR_RESOURCE_ATTRIBUTE_JNI_H_

#include <jni.h>
#include "simulator_resource_model.h"

jobject SimulatorResourceAttributeToJava(JNIEnv *env,
        const SimulatorResourceAttribute &attribute);

bool SimulatorResourceAttributeToCpp(JNIEnv *env, jobject jAttribute,
                                     SimulatorResourceAttribute &attribute);

jobject SimulatorResourceAttributesToJava(JNIEnv *env,
        const std::map<std::string, SimulatorResourceAttribute> &attributes);

#endif
