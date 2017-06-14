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

#include "simulator_utils_jni.h"

jobject SimulatorResultToJava(JNIEnv *env, SimulatorResult errorCode)
{
    switch (errorCode)
    {
        case SIMULATOR_OK:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_OK", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_RESOURCE_CREATED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_RESOURCE_CREATED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_RESOURCE_DELETED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_RESOURCE_DELETED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_CONTINUE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_CONTINUE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_RESOURCE_CHANGED:
        {
            static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                      "SIMULATOR_RESOURCE_CHANGED", "Lorg/oic/simulator/SimulatorResult;");
            return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
        }
        case SIMULATOR_INVALID_URI:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_URI", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_QUERY:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_QUERY", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_IP:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_IP", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_PORT:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_PORT", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_CALLBACK:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_CALLBACK", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_METHOD:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_METHOD", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_PARAM:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_PARAM", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_OBSERVE_PARAM:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_OBSERVE_PARAM", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_NO_MEMORY:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_NO_MEMORY", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_COMM_ERROR:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_COMM_ERROR", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_TIMEOUT:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_TIMEOUT", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_ADAPTER_NOT_ENABLED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_ADAPTER_NOT_ENABLED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_NOTIMPL:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_NOTIMPL", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_NO_RESOURCE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_NO_RESOURCE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_RESOURCE_ERROR:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_RESOURCE_ERROR", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_SLOW_RESOURCE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_SLOW_RESOURCE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_DUPLICATE_REQUEST:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_DUPLICATE_REQUEST", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_NO_OBSERVERS:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_NO_OBSERVERS", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_OBSERVER_NOT_FOUND:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_OBSERVER_NOT_FOUND", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_VIRTUAL_DO_NOT_HANDLE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_VIRTUAL_DO_NOT_HANDLE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_OPTION:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_OPTION", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_MALFORMED_RESPONSE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_MALFORMED_RESPONSE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_PERSISTENT_BUFFER_REQUIRED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_PERSISTENT_BUFFER_REQUIRED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_REQUEST_HANDLE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_REQUEST_HANDLE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_DEVICE_INFO:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_DEVICE_INFO", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_JSON:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_JSON", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_UNAUTHORIZED_REQ:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_UNAUTHORIZED_REQ", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_TOO_LARGE_REQ:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_TOO_LARGE_REQ", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_PDM_IS_NOT_INITIALIZED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_PDM_IS_NOT_INITIALIZED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_DUPLICATE_UUID:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_DUPLICATE_UUID", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INCONSISTENT_DB:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INCONSISTENT_DB", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_AUTHENTICATION_FAILURE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_AUTHENTICATION_FAILURE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
#ifdef WITH_PRESENCE
        case SIMULATOR_PRESENCE_STOPPED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_PRESENCE_STOPPED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_PRESENCE_TIMEOUT:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_PRESENCE_TIMEOUT", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_PRESENCE_DO_NOT_HANDLE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_PRESENCE_DO_NOT_HANDLE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
#endif
        case SIMULATOR_INVALID_TYPE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_TYPE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_NOT_SUPPORTED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_NOT_SUPPORTED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_OPERATION_NOT_ALLOWED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_OPERATION_NOT_ALLOWED", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_OPERATION_IN_PROGRESS:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_OPERATION_IN_PROGRESS", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_INVALID_RESPONSE_CODE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_INVALID_RESPONSE_CODE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_UKNOWN_PROPERTY:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_UKNOWN_PROPERTY", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_TYPE_MISMATCH:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_TYPE_MISMATCH", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_BAD_VALUE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_BAD_VALUE", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_BAD_OBJECT:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_BAD_OBJECT", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        case SIMULATOR_BAD_SCHEMA:
            {
                static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                                          "SIMULATOR_BAD_SCHEMA", "Lorg/oic/simulator/SimulatorResult;");
                return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
            }
        default:
            break;
    }

    static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.simulatorResultCls,
                              "SIMULATOR_ERROR", "Lorg/oic/simulator/SimulatorResult;");
    return env->GetStaticObjectField(gSimulatorClassRefs.simulatorResultCls, fieldID);
}

