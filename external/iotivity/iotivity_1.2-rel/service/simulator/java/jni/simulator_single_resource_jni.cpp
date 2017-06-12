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

#include "simulator_resource_model_jni.h"
#include "simulator_resource_attribute_jni.h"
#include "simulator_exceptions_jni.h"
#include "simulator_utils_jni.h"
#include "jni_sharedobject_holder.h"
#include "jni_listener_holder.h"
#include "jni_string.h"
#include "jni_vector.h"

#include "simulator_single_resource.h"

#define VALIDATE_OBJECT(ENV, OBJECT) if (!OBJECT){ThrowBadObjectException(ENV, "No corresponsing native object!"); return;}
#define VALIDATE_OBJECT_RET(ENV, OBJECT, RET) if (!OBJECT){ThrowBadObjectException(ENV, "No corresponsing native object!"); return RET;}

static SimulatorSingleResourceSP simulatorSingleResourceToCpp(JNIEnv *env, jobject object)
{
    JniSharedObjectHolder<SimulatorSingleResource> *jniResource =
        getHandle<JniSharedObjectHolder<SimulatorSingleResource>>(env, object);
    if (jniResource)
        return jniResource->get();
    return nullptr;
}

static AutoUpdateType autoUpdateTypeToCpp(JNIEnv *env, jobject jType)
{
    static jmethodID ordinalMID = env->GetMethodID(
                                      gSimulatorClassRefs.autoUpdateTypeCls, "ordinal", "()I");

    int ordinal = env->CallIntMethod(jType, ordinalMID);
    return AutoUpdateType(ordinal);
}

static void onAutoUpdationComplete(jobject listener, const std::string &uri, const int id)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethod = env->GetMethodID(listenerCls, "onUpdateComplete",
                               "(Ljava/lang/String;I)V");

    jstring jUri = env->NewStringUTF(uri.c_str());
    env->CallVoidMethod(listener, listenerMethod, jUri, id);
    ReleaseEnv();
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeGetAttribute
(JNIEnv *env, jobject object, jstring jAttrName)
{
    VALIDATE_INPUT_RET(env, !jAttrName, "Attribute name is null!", nullptr)

    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, singleResource, nullptr)

    JniString jniAttrName(env, jAttrName);
    SimulatorResourceAttribute attribute;
    if (singleResource->getAttribute(jniAttrName.get(), attribute))
        return SimulatorResourceAttributeToJava(env, attribute);
    return nullptr;
}

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeGetAttributes
(JNIEnv *env, jobject object)
{
    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, singleResource, nullptr)

    return SimulatorResourceAttributesToJava(env, singleResource->getAttributes());
}

JNIEXPORT jboolean JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeAddAttribute
(JNIEnv *env, jobject object, jobject jResAttribute)
{
    VALIDATE_INPUT_RET(env, !jResAttribute, "Resource attribute is null!", false)

    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, singleResource, false)

    try
    {
        SimulatorResourceAttribute attribute;
        if (!SimulatorResourceAttributeToCpp(env, jResAttribute, attribute))
        {
            ThrowSimulatorException(env, SIMULATOR_ERROR,
                                    "Failed to covnert SimulatorResourceAttribute java object!");
            return false;
        }
        return singleResource->addAttribute(attribute);
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return false;
}

JNIEXPORT jboolean JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeUpdateAttribute
(JNIEnv *env, jobject object, jstring jAttrName, jobject jAttrValue)
{
    VALIDATE_INPUT_RET(env, !jAttrName, "Attribute name is null!", false)
    VALIDATE_INPUT_RET(env, !jAttrValue, "Attribute value is null!", false)

    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, singleResource, false)

    AttributeValueVariant value;
    if (!AttributeValueToCpp(env, jAttrValue, value))
    {
        ThrowSimulatorException(env, SIMULATOR_ERROR,
                                "Failed to covnert AttributeValue java object!");
        return false;
    }

    SimulatorResourceAttribute attribute(JniString(env, jAttrName).get());
    attribute.setValue(value);
    return singleResource->updateAttributeValue(attribute);
}

JNIEXPORT jboolean JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeRemoveAttribute
(JNIEnv *env, jobject object, jstring jAttrName)
{
    VALIDATE_INPUT_RET(env, !jAttrName, "Attribute name is null!", false)

    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, singleResource, false)

    try
    {
        JniString jniAttrName(env, jAttrName);
        return singleResource->removeAttribute(jniAttrName.get());
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }

    return false;
}

JNIEXPORT jint JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeStartResourceUpdation
(JNIEnv *env, jobject object, jobject jType, jint jInterval, jobject jListener)
{
    VALIDATE_CALLBACK_RET(env, jListener, -1)

    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, singleResource, -1)

    jobject listenerRef = env->NewGlobalRef(jListener);
    SimulatorSingleResource::AutoUpdateCompleteCallback callback =  [listenerRef](
                const std::string & uri, const int id)
    {
        onAutoUpdationComplete(listenerRef, uri, id);
    };

    try
    {
        AutoUpdateType autoUpdateType = autoUpdateTypeToCpp(env, jType);
        return singleResource->startResourceUpdation(autoUpdateType, jInterval, callback);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return -1;
}

JNIEXPORT jint JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeStartAttributeUpdation
(JNIEnv *env, jobject object, jstring jAttrName, jobject jType, jint jInterval, jobject jListener)
{
    VALIDATE_INPUT_RET(env, !jAttrName, "Attribute name is null!", -1)
    VALIDATE_CALLBACK_RET(env, jListener, -1)

    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, singleResource, -1)

    jobject listenerRef = env->NewGlobalRef(jListener);
    SimulatorSingleResource::AutoUpdateCompleteCallback callback =
        [listenerRef](const std::string & uri, const int id)
    {
        onAutoUpdationComplete(listenerRef, uri, id);
    };

    try
    {
        JniString jniAttrName(env, jAttrName);
        AutoUpdateType autoUpdateType = autoUpdateTypeToCpp(env, jType);
        return singleResource->startAttributeUpdation(jniAttrName.get(), autoUpdateType,
                jInterval, callback);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return -1;
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeStopUpdation
(JNIEnv *env, jobject object, jint id)
{
    SimulatorSingleResourceSP singleResource = simulatorSingleResourceToCpp(env, object);
    VALIDATE_OBJECT(env, singleResource)

    singleResource->stopUpdation(id);
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorSingleResource_nativeDispose
(JNIEnv *env, jobject object)
{
    JniSharedObjectHolder<SimulatorSingleResource> *resource =
        getHandle<JniSharedObjectHolder<SimulatorSingleResource>>(env, object);
    delete resource;
}

#ifdef __cplusplus
}
#endif
