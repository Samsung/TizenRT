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
#include "simulator_exceptions_jni.h"
#include "simulator_utils_jni.h"
#include "jni_sharedobject_holder.h"
#include "jni_listener_holder.h"
#include "jni_string.h"
#include "jni_vector.h"

#include "simulator_resource.h"

#define VALIDATE_OBJECT(ENV, OBJECT) if (!OBJECT){ThrowBadObjectException(ENV, "No corresponsing native object!"); return;}
#define VALIDATE_OBJECT_RET(ENV, OBJECT, RET) if (!OBJECT){ThrowBadObjectException(ENV, "No corresponsing native object!"); return RET;}

SimulatorResourceSP SimulatorResourceToCpp(JNIEnv *env, jobject object)
{
    JniSharedObjectHolder<SimulatorResource> *jniResource =
        getHandle<JniSharedObjectHolder<SimulatorResource>>(env, object);
    if (jniResource)
        return jniResource->get();
    return nullptr;
}

static jobject createObserverInfo(JNIEnv *env, const ObserverInfo &observerInfo)
{
    static jmethodID observerInfoCtor = env->GetMethodID(gSimulatorClassRefs.observerCls,
                                        "<init>", "(ILjava/lang/String;I)V");

    jstring address = env->NewStringUTF(observerInfo.address.c_str());
    jobject observer = (jobject) env->NewObject(gSimulatorClassRefs.observerCls,
                       observerInfoCtor, observerInfo.id, address, observerInfo.port);
    env->DeleteLocalRef(address);

    return observer;
}

static jobject createObserverInfoVector(JNIEnv *env,
                                        const std::vector<ObserverInfo> observersList)
{
    jobject vectorObject = JniVector(env).toJava();
    if (!vectorObject)
        return nullptr;

    static jmethodID addMethod = env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                 "add", "(Ljava/lang/Object;)Z");

    for (auto &observer : observersList)
        env->CallBooleanMethod(vectorObject, addMethod, createObserverInfo(env, observer));

    return vectorObject;
}

static void onObserverChange(jobject listener, const std::string &uri,
                             ObservationStatus state, const ObserverInfo &observerInfo)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethod = nullptr;
    if (ObservationStatus::REGISTER == state)
    {
        listenerMethod = env->GetMethodID(listenerCls, "onObserverAdded",
                                          "(Ljava/lang/String;Lorg/oic/simulator/server/Observer;)V");
    }
    else
    {
        listenerMethod = env->GetMethodID(listenerCls, "onObserverRemoved",
                                          "(Ljava/lang/String;Lorg/oic/simulator/server/Observer;)V");
    }

    jstring jUri = env->NewStringUTF(uri.c_str());
    jobject jobserver = createObserverInfo(env, observerInfo);

    env->CallVoidMethod(listener, listenerMethod, jUri, jobserver);
    ReleaseEnv();
}

static void onResourceModelChange(jobject listener, const std::string &uri,
                                  const SimulatorResourceModel &resModel)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethod = env->GetMethodID(listenerCls, "onResourceModelChanged",
                               "(Ljava/lang/String;Lorg/oic/simulator/SimulatorResourceModel;)V");

    jobject jResModel = SimulatorResourceModelToJava(env, resModel);
    jstring jUri = env->NewStringUTF(uri.c_str());
    env->CallVoidMethod(listener, listenerMethod, jUri, jResModel);
    ReleaseEnv();
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeGetName
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, nullptr)

    return env->NewStringUTF(resource->getName().c_str());
}

JNIEXPORT jstring JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeGetURI
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, nullptr)

    return env->NewStringUTF(resource->getURI().c_str());
}

JNIEXPORT jstring JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeGetResourceType
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, nullptr)

    return env->NewStringUTF(resource->getResourceType().c_str());
}

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeGetInterface
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, nullptr)

    std::vector<std::string> interfaces = resource->getInterface();
    return JniVector(env).toJava(interfaces);
}

JNIEXPORT jboolean JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeIsObservable
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, false)

    return resource->isObservable();
}

JNIEXPORT jboolean JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeIsDiscoverable
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, false)

    return resource->isDiscoverable();
}

JNIEXPORT jboolean JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeIsStarted
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, false)

    return resource->isStarted();
}

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeGetResourceModel
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, nullptr)

    SimulatorResourceModel resModel = resource->getResourceModel();
    return SimulatorResourceModelToJava(env, resModel);
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetName
(JNIEnv *env, jobject object, jstring jName)
{
    VALIDATE_INPUT(env, !jName, "Name is null!")

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniName(env, jName);
        resource->setName(jniName.get());
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetURI
(JNIEnv *env, jobject object, jstring jUri)
{
    VALIDATE_INPUT(env, !jUri, "Uri is null!")

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniUri(env, jUri);
        resource->setURI(jniUri.get());
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetResourceType
(JNIEnv *env, jobject object, jstring jResourceType)
{
    VALIDATE_INPUT(env, !jResourceType, "Resource type is null!")

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniType(env, jResourceType);
        resource->setResourceType(jniType.get());
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetInterface
(JNIEnv *env, jobject object, jstring jInterfaceType)
{
    VALIDATE_INPUT(env, !jInterfaceType, "Interface type is null!")

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniInterfaceType(env, jInterfaceType);
        resource->setInterface(jniInterfaceType.get());
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetInterfaces
(JNIEnv *env, jobject object, jobject jInterfaceTypes)
{
    VALIDATE_INPUT(env, !jInterfaceTypes, "Interface type is null!")

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    std::vector<std::string> interfaceList = JniVector(env).toCpp<std::string>(jInterfaceTypes);

    try
    {
        resource->setInterface(interfaceList);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeAddInterface
(JNIEnv *env, jobject object, jstring jInterfaceType)
{
    VALIDATE_INPUT(env, !jInterfaceType, "Interface type is null!")

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniInterfaceType(env, jInterfaceType);
        resource->addInterface(jniInterfaceType.get());
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (NoSupportException &e)
    {
        ThrowNoSupportException(env, e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetObservable
(JNIEnv *env, jobject object, jboolean jState)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->setObservable(static_cast<bool>(jState));
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetDiscoverable
(JNIEnv *env, jobject object, jboolean jState)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->setDiscoverable(static_cast<bool>(jState));
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetObserverListener
(JNIEnv *env, jobject object, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    SimulatorResource::ObserverCallback callback =  std::bind([](const std::string & uri,
            ObservationStatus state, const ObserverInfo & observerInfo,
            const std::shared_ptr<JniListenerHolder> &listenerRef)
    {
        onObserverChange(listenerRef->get(), uri, state, observerInfo);
    }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
    JniListenerHolder::create(env, jListener));

    try
    {
        resource->setObserverCallback(callback);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeSetResourceModelChangeListener
(JNIEnv *env, jobject object, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    SimulatorResource::ResourceModelUpdateCallback callback =  std::bind(
                [](const std::string & uri, const SimulatorResourceModel & resModel,
                   const std::shared_ptr<JniListenerHolder> &listenerRef)
    {
        onResourceModelChange(listenerRef->get(), uri, resModel);
    }, std::placeholders::_1, std::placeholders::_2, JniListenerHolder::create(env, jListener));

    try
    {
        resource->setModelChangeCallback(callback);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeStart
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->start();
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeStop
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->stop();
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeGetObservers
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, nullptr)

    return createObserverInfoVector(env, resource->getObservers());
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeNotify
(JNIEnv *env, jobject object, jint id)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->notify(id);
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_server_SimulatorResource_nativeNotifyAll
(JNIEnv *env, jobject object)
{
    SimulatorResourceSP resource = SimulatorResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->notifyAll();
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

#ifdef __cplusplus
}
#endif
