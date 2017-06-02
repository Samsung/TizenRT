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

/**
 * @file   simulator_jni_util.h
 *
 * @brief  This file contains the utility functions for conversions from java to CPP
 * and viceversa
 */

#ifndef SIMULATOR_EXCEPTIONS_JNI_H_
#define SIMULATOR_EXCEPTIONS_JNI_H_

#include <jni.h>

#include "simulator_error_codes.h"

void ThrowSimulatorException(JNIEnv *env, SimulatorResult code, const char *message);

void ThrowInvalidArgsException(JNIEnv *env, SimulatorResult code, const char *message);

void ThrowNoSupportException(JNIEnv *env, const char *message);

void ThrowOperationInProgressException(JNIEnv *env, const char *message);

void ThrowBadObjectException(JNIEnv *env, const char *message);

#endif
