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
#include <mutex>

#define OBJECT_CLS "java/lang/Object"
#define INTEGER_CLS "java/lang/Integer"
#define DOUBLE_CLS "java/lang/Double"
#define BOOLEAN_CLS "java/lang/Boolean"
#define STRING_CLS "java/lang/String"
#define VECTOR_CLS "java/util/Vector"
#define HASHMAP_CLS "java/util/HashMap"
#define MAP_CLS "java/util/Map"
#define MAP_ENTRY_CLS "java/util/Map$Entry"
#define SET_CLS "java/util/Set"
#define ITERATOR_CLS "java/util/Iterator"

#define SIMULATOR_RESOURCE_CLS "org/oic/simulator/server/SimulatorResource"
#define SIMULATOR_RESOURCE_TYPE_CLS "org/oic/simulator/server/SimulatorResource$Type"
#define SIMULATOR_SINGLE_RESOURCE_CLS "org/oic/simulator/server/SimulatorSingleResource"
#define SIMULATOR_COLLECTION_RESOURCE_CLS "org/oic/simulator/server/SimulatorCollectionResource"
#define SIMULATOR_RESOURCE_MODEL_CLS "org/oic/simulator/SimulatorResourceModel"
#define SIMULATOR_RESOURCE_ATTRIBUTE_CLS "org/oic/simulator/SimulatorResourceAttribute"
#define ATTRIBUTE_VALUE_CLS "org/oic/simulator/AttributeValue"
#define ATTRIBUTE_VALUE_TYPE_CLS "org/oic/simulator/AttributeValue$ValueType"
#define ATTRIBUTE_TYPE_INFO_CLS "org/oic/simulator/AttributeValue$TypeInfo"
#define ATTRIBUTE_PROPERTY_CLS "org/oic/simulator/AttributeProperty"
#define ATTRIBUTE_PROPERTY_TYPE_CLS "org/oic/simulator/AttributeProperty$Type"
#define AUTO_UPDATE_TYPE_CLS "org/oic/simulator/server/SimulatorResource$AutoUpdateType"

#define SIMULATOR_REMOTE_RESOURCE_CLS "org/oic/simulator/client/SimulatorRemoteResource"
#define REQUEST_TYPE_CLS "org/oic/simulator/client/SimulatorRemoteResource$RequestType"
#define OBSERVER_CLS "org/oic/simulator/server/Observer"
#define DEVICE_INFO_CLS "org/oic/simulator/DeviceInfo"
#define PLATFORM_INFO_CLS "org/oic/simulator/PlatformInfo"
#define SIMULATOR_REQUEST_MODEL_CLS "org/oic/simulator/client/SimulatorRequestModel"

#define SIMULATOR_EXCEPTION_CLS "org/oic/simulator/SimulatorException"
#define INVALID_ARGS_EXCEPTION_CLS "org/oic/simulator/InvalidArgsException"
#define NO_SUPPORT_EXCEPTION_CLS "org/oic/simulator/NoSupportException"
#define OPERATION_IN_PROGRESS_EXCEPTION_CLS "org/oic/simulator/OperationInProgressException"
#define SIMULATOR_RESULT_CLS "org/oic/simulator/SimulatorResult"

#define INTEGER_PROPERTY "org/oic/simulator/IntegerProperty"
#define INTEGER_PROPERTY_BUILDER "org/oic/simulator/IntegerProperty$Builder"
#define DOUBLE_PROPERTY "org/oic/simulator/DoubleProperty"
#define DOUBLE_PROPERTY_BUILDER "org/oic/simulator/DoubleProperty$Builder"
#define BOOLEAN_PROPERTY "org/oic/simulator/BooleanProperty"
#define BOOLEAN_PROPERTY_BUILDER "org/oic/simulator/BooleanProperty$Builder"
#define STRING_PROPERTY "org/oic/simulator/StringProperty"
#define STRING_PROPERTY_BUILDER "org/oic/simulator/StringProperty$Builder"
#define ARRAY_PROPERTY "org/oic/simulator/ArrayProperty"
#define ARRAY_PROPERTY_BUILDER "org/oic/simulator/ArrayProperty$Builder"
#define MODEL_PROPERTY "org/oic/simulator/ModelProperty"

#define INTEGER_1D_ARRAY "[Ljava/lang/Integer;"
#define INTEGER_2D_ARRAY "[[Ljava/lang/Integer;"
#define INTEGER_3D_ARRAY "[[Ljava/lang/Integer;"
#define DOUBLE_1D_ARRAY "[Ljava/lang/Double;"
#define DOUBLE_2D_ARRAY "[[Ljava/lang/Double;"
#define DOUBLE_3D_ARRAY "[[Ljava/lang/Double;"
#define BOOLEAN_1D_ARRAY "[Ljava/lang/Boolean;"
#define BOOLEAN_2D_ARRAY "[[Ljava/lang/Boolean;"
#define BOOLEAN_3D_ARRAY "[[Ljava/lang/Boolean;"
#define STRING_1D_ARRAY "[Ljava/lang/String;"
#define STRING_2D_ARRAY "[[Ljava/lang/String;"
#define STRING_3D_ARRAY "[[Ljava/lang/String;"
#define SIMULATOR_RESOURCE_MODEL_1D_ARRAY "[Lorg/oic/simulator/SimulatorResourceModel;"
#define SIMULATOR_RESOURCE_MODEL_2D_ARRAY "[[Lorg/oic/simulator/SimulatorResourceModel;"
#define SIMULATOR_RESOURCE_MODEL_3D_ARRAY "[[Lorg/oic/simulator/SimulatorResourceModel;"

SimulatorClassRefs gSimulatorClassRefs;
static std::mutex gJvmMutex;
static JavaVM *gJavaVM;

JNIEnv *GetEnv()
{
    std::unique_lock<std::mutex> lock(gJvmMutex);
    if (!gJavaVM)
        return nullptr;

    JNIEnv *env = nullptr;
    jint ret = gJavaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
    switch (ret)
    {
        case JNI_OK:
            return env;
        case JNI_EDETACHED:
            if (!gJavaVM->AttachCurrentThread((void **)&env, NULL))
                return env;
    }

    return nullptr;
}

void ReleaseEnv()
{
    std::unique_lock<std::mutex> lock(gJvmMutex);
    if (!gJavaVM)
        return;
    gJavaVM->DetachCurrentThread();
}

static void getClassRef(JNIEnv *env, const char *className, jclass &classRef)
{
    jclass localRef = env->FindClass(className);
    if (localRef)
    {
        classRef = (jclass) env->NewGlobalRef(localRef);
        env->DeleteLocalRef(localRef);
    }
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * /*reserved*/)
{
    if (!vm)
        return JNI_ERR;

    JNIEnv *env = nullptr;
    if (JNI_OK != vm->GetEnv((void **) &env, JNI_VERSION_1_6))
        return JNI_ERR;

    // Get the class references
    getClassRef(env, OBJECT_CLS, gSimulatorClassRefs.objectCls);
    getClassRef(env, INTEGER_CLS, gSimulatorClassRefs.integerCls);
    getClassRef(env, DOUBLE_CLS, gSimulatorClassRefs.doubleCls);
    getClassRef(env, BOOLEAN_CLS, gSimulatorClassRefs.booleanCls);
    getClassRef(env, STRING_CLS, gSimulatorClassRefs.stringCls);
    getClassRef(env, VECTOR_CLS, gSimulatorClassRefs.vectorCls);
    getClassRef(env, HASHMAP_CLS, gSimulatorClassRefs.hashMapCls);
    getClassRef(env, MAP_CLS, gSimulatorClassRefs.mapCls);
    getClassRef(env, MAP_ENTRY_CLS, gSimulatorClassRefs.mapEntryCls);
    getClassRef(env, SET_CLS, gSimulatorClassRefs.setCls);
    getClassRef(env, ITERATOR_CLS, gSimulatorClassRefs.iteratorCls);

    getClassRef(env, SIMULATOR_RESOURCE_CLS, gSimulatorClassRefs.simulatorResourceCls);
    getClassRef(env, SIMULATOR_RESOURCE_TYPE_CLS, gSimulatorClassRefs.simulatorResourceTypeCls);
    getClassRef(env, SIMULATOR_SINGLE_RESOURCE_CLS, gSimulatorClassRefs.simulatorSingleResourceCls);
    getClassRef(env, SIMULATOR_COLLECTION_RESOURCE_CLS,
                gSimulatorClassRefs.simulatorCollectionResourceCls);
    getClassRef(env, SIMULATOR_RESOURCE_MODEL_CLS, gSimulatorClassRefs.simulatorResourceModelCls);
    getClassRef(env, SIMULATOR_RESOURCE_ATTRIBUTE_CLS,
                gSimulatorClassRefs.simulatorResourceAttributeCls);
    getClassRef(env, ATTRIBUTE_VALUE_CLS, gSimulatorClassRefs.attributeValueCls);
    getClassRef(env, ATTRIBUTE_VALUE_TYPE_CLS, gSimulatorClassRefs.attributeValueTypeCls);
    getClassRef(env, ATTRIBUTE_TYPE_INFO_CLS, gSimulatorClassRefs.attributeTypeInfoCls);
    getClassRef(env, ATTRIBUTE_PROPERTY_CLS, gSimulatorClassRefs.attributePropertyCls);
    getClassRef(env, ATTRIBUTE_PROPERTY_TYPE_CLS, gSimulatorClassRefs.attributePropertyTypeCls);
    getClassRef(env, INTEGER_PROPERTY, gSimulatorClassRefs.integerPropertyCls);
    getClassRef(env, INTEGER_PROPERTY_BUILDER, gSimulatorClassRefs.integerPropertyBuilderCls);
    getClassRef(env, DOUBLE_PROPERTY, gSimulatorClassRefs.doublePropertyCls);
    getClassRef(env, DOUBLE_PROPERTY_BUILDER, gSimulatorClassRefs.doublePropertyBuilderCls);
    getClassRef(env, BOOLEAN_PROPERTY, gSimulatorClassRefs.booleanPropertyCls);
    getClassRef(env, BOOLEAN_PROPERTY_BUILDER, gSimulatorClassRefs.booleanPropertyBuilderCls);
    getClassRef(env, STRING_PROPERTY, gSimulatorClassRefs.stringPropertyCls);
    getClassRef(env, STRING_PROPERTY_BUILDER, gSimulatorClassRefs.stringPropertyBuilderCls);
    getClassRef(env, ARRAY_PROPERTY, gSimulatorClassRefs.arrayPropertyCls);
    getClassRef(env, ARRAY_PROPERTY_BUILDER, gSimulatorClassRefs.arrayPropertyBuilderCls);
    getClassRef(env, MODEL_PROPERTY, gSimulatorClassRefs.modelPropertyCls);
    getClassRef(env, AUTO_UPDATE_TYPE_CLS, gSimulatorClassRefs.autoUpdateTypeCls);

    getClassRef(env, SIMULATOR_REMOTE_RESOURCE_CLS,
                gSimulatorClassRefs.simulatorRemoteResourceCls);
    getClassRef(env, REQUEST_TYPE_CLS, gSimulatorClassRefs.requestTypeCls);
    getClassRef(env, OBSERVER_CLS, gSimulatorClassRefs.observerCls);
    getClassRef(env, DEVICE_INFO_CLS, gSimulatorClassRefs.deviceInfoCls);
    getClassRef(env, PLATFORM_INFO_CLS, gSimulatorClassRefs.platformInfoCls);
    getClassRef(env, SIMULATOR_REQUEST_MODEL_CLS, gSimulatorClassRefs.simulatorRequestModelCls);

    getClassRef(env, SIMULATOR_EXCEPTION_CLS, gSimulatorClassRefs.simulatorExceptionCls);
    getClassRef(env, INVALID_ARGS_EXCEPTION_CLS, gSimulatorClassRefs.invalidArgsExceptionCls);
    getClassRef(env, NO_SUPPORT_EXCEPTION_CLS, gSimulatorClassRefs.noSupportExceptionCls);
    getClassRef(env, OPERATION_IN_PROGRESS_EXCEPTION_CLS,
                gSimulatorClassRefs.operationInProgressExceptionCls);
    getClassRef(env, SIMULATOR_RESULT_CLS, gSimulatorClassRefs.simulatorResultCls);

    getClassRef(env, INTEGER_1D_ARRAY, gSimulatorClassRefs.integer1DArrayCls);
    getClassRef(env, INTEGER_2D_ARRAY, gSimulatorClassRefs.integer2DArrayCls);
    getClassRef(env, INTEGER_3D_ARRAY, gSimulatorClassRefs.integer3DArrayCls);
    getClassRef(env, DOUBLE_1D_ARRAY, gSimulatorClassRefs.double1DArrayCls);
    getClassRef(env, DOUBLE_2D_ARRAY, gSimulatorClassRefs.double2DArrayCls);
    getClassRef(env, DOUBLE_3D_ARRAY, gSimulatorClassRefs.double3DArrayCls);
    getClassRef(env, BOOLEAN_1D_ARRAY, gSimulatorClassRefs.boolean1DArrayCls);
    getClassRef(env, BOOLEAN_2D_ARRAY, gSimulatorClassRefs.boolean2DArrayCls);
    getClassRef(env, BOOLEAN_3D_ARRAY, gSimulatorClassRefs.boolean3DArrayCls);
    getClassRef(env, STRING_1D_ARRAY, gSimulatorClassRefs.string1DArrayCls);
    getClassRef(env, STRING_2D_ARRAY, gSimulatorClassRefs.string2DArrayCls);
    getClassRef(env, STRING_3D_ARRAY, gSimulatorClassRefs.string3DArrayCls);
    getClassRef(env, SIMULATOR_RESOURCE_MODEL_1D_ARRAY,
                gSimulatorClassRefs.simulatorResModel1DArrayCls);
    getClassRef(env, SIMULATOR_RESOURCE_MODEL_2D_ARRAY,
                gSimulatorClassRefs.simulatorResModel2DArrayCls);
    getClassRef(env, SIMULATOR_RESOURCE_MODEL_3D_ARRAY,
                gSimulatorClassRefs.simulatorResModel3DArrayCls);

    gJavaVM = vm;
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void * /*reserved*/)
{
    if (!vm)
        return;

    JNIEnv *env = nullptr;
    if (JNI_OK != vm->GetEnv((void **) &env, JNI_VERSION_1_6))
        return;

    // Release the class global references
    env->DeleteGlobalRef(gSimulatorClassRefs.objectCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.integerCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.doubleCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.booleanCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.stringCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.hashMapCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.vectorCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.mapCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.mapEntryCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.setCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.iteratorCls);

    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorResourceCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorSingleResourceCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorCollectionResourceCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorResourceModelCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorResourceAttributeCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.attributeValueCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.attributeValueTypeCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.attributeTypeInfoCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.attributePropertyCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.attributePropertyTypeCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.integerPropertyCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.integerPropertyBuilderCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.doublePropertyCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.doublePropertyBuilderCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.booleanPropertyCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.booleanPropertyBuilderCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.stringPropertyCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.stringPropertyBuilderCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.arrayPropertyCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.arrayPropertyBuilderCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.modelPropertyCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.autoUpdateTypeCls);

    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorRemoteResourceCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.requestTypeCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.observerCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.deviceInfoCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.platformInfoCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorRequestModelCls);

    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorExceptionCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.invalidArgsExceptionCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.noSupportExceptionCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.operationInProgressExceptionCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorResultCls);

    env->DeleteGlobalRef(gSimulatorClassRefs.integer1DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.integer2DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.integer3DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.double1DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.double2DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.double3DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.boolean1DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.boolean2DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.boolean3DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.string1DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.string2DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.string3DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorResModel1DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorResModel2DArrayCls);
    env->DeleteGlobalRef(gSimulatorClassRefs.simulatorResModel3DArrayCls);
}

#ifdef __cplusplus
}
#endif
