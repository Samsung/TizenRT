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

#include "simulator_resource_utils_jni.h"
#include "simulator_utils_jni.h"
#include "jni_sharedobject_holder.h"
#include "jni_vector.h"

jobject CreateSingleResource(JNIEnv *env, SimulatorResourceSP singleResource)
{
    if (!singleResource)
        return nullptr;

    auto *objectHolder = JniSharedObjectHolder<SimulatorResource>::create(singleResource);
    if (!objectHolder)
        return nullptr;

    static jmethodID simulatorSingleResourceCtor = env->GetMethodID(
                gSimulatorClassRefs.simulatorSingleResourceCls, "<init>", "(J)V");

    jobject resource = (jobject) env->NewObject(gSimulatorClassRefs.simulatorSingleResourceCls,
                       simulatorSingleResourceCtor, reinterpret_cast<jlong>(objectHolder));
    if (!resource)
    {
        delete objectHolder;
        return nullptr;
    }

    return resource;
}

jobject CreateCollectionResource(JNIEnv *env, SimulatorResourceSP collectionResource)
{
    if (!collectionResource)
        return nullptr;

    auto *objectHolder = JniSharedObjectHolder<SimulatorResource>::create(collectionResource);
    if (!objectHolder)
        return nullptr;

    static jmethodID simulatorCollectionResourceCtor = env->GetMethodID(
                gSimulatorClassRefs.simulatorCollectionResourceCls, "<init>", "(J)V");
    jobject resource = (jobject) env->NewObject(gSimulatorClassRefs.simulatorCollectionResourceCls,
                       simulatorCollectionResourceCtor, reinterpret_cast<jlong>(objectHolder));
    if (!resource)
    {
        delete objectHolder;
        return nullptr;
    }

    return resource;
}

jobject CreateSimulatorResource(JNIEnv *env, SimulatorResourceSP resource)
{
    if (!resource)
        return nullptr;

    if (SimulatorResource::Type::COLLECTION_RESOURCE == resource->getType())
        return CreateCollectionResource(env, resource);
    return CreateSingleResource(env, resource);
}

jobject CreateSimulatorResourceVector(JNIEnv *env, std::vector<SimulatorResourceSP> &resources)
{
    if (!resources.size())
        return nullptr;

    jobject vectorObject = JniVector(env).toJava();
    if (!vectorObject)
        return nullptr;

    static jmethodID addMethod = env->GetMethodID(gSimulatorClassRefs.vectorCls,
                                 "add", "(Ljava/lang/Object;)Z");

    for (auto &resource : resources)
        env->CallBooleanMethod(vectorObject, addMethod, CreateSimulatorResource(env, resource));

    return vectorObject;
}

jobject CreateSimulatorRemoteResource(JNIEnv *env, SimulatorRemoteResourceSP &remoteResource)
{
    if (!remoteResource)
        return nullptr;

    auto *objectHolder = JniSharedObjectHolder<SimulatorRemoteResource>::create(remoteResource);
    if (!objectHolder)
        return nullptr;

    static jmethodID simulatorRemoteResourceCtor = env->GetMethodID(
                gSimulatorClassRefs.simulatorRemoteResourceCls, "<init>", "(J)V");

    jobject resource = (jobject) env->NewObject(gSimulatorClassRefs.simulatorRemoteResourceCls,
                       simulatorRemoteResourceCtor, reinterpret_cast<jlong>(objectHolder));
    if (!resource)
    {
        delete objectHolder;
        return nullptr;
    }

    // Setting all required fields
    static jfieldID uriID = env->GetFieldID(gSimulatorClassRefs.simulatorRemoteResourceCls,
                                            "mUri", "Ljava/lang/String;");
    static jfieldID connTypeID = env->GetFieldID(gSimulatorClassRefs.simulatorRemoteResourceCls,
                                 "mConnType", "I");
    static jfieldID hostID = env->GetFieldID(gSimulatorClassRefs.simulatorRemoteResourceCls,
                             "mHost", "Ljava/lang/String;");
    static jfieldID uidID = env->GetFieldID(gSimulatorClassRefs.simulatorRemoteResourceCls,
                                            "mId", "Ljava/lang/String;");
    static jfieldID resTypesID = env->GetFieldID(gSimulatorClassRefs.simulatorRemoteResourceCls,
                                 "mResTypes", "Ljava/util/Vector;");
    static jfieldID interfacesID = env->GetFieldID(gSimulatorClassRefs.simulatorRemoteResourceCls,
                                   "mResInterfaces", "Ljava/util/Vector;");
    static jfieldID isObservableID = env->GetFieldID(gSimulatorClassRefs.simulatorRemoteResourceCls,
                                     "mIsObservable", "Z");

    jstring uri = env->NewStringUTF(remoteResource->getURI().c_str());
    env->SetObjectField(resource, uriID, uri);

    jint connType = remoteResource->getConnectivityType();
    env->SetIntField(resource, connTypeID, connType);

    jstring host = env->NewStringUTF(remoteResource->getHost().c_str());
    env->SetObjectField(resource, hostID, host);

    jstring uid = env->NewStringUTF(remoteResource->getID().c_str());
    env->SetObjectField(resource, uidID, uid);

    std::vector<std::string> resourceTypes = remoteResource->getResourceTypes();
    jobject ResTypes = JniVector(env).toJava(resourceTypes);
    env->SetObjectField(resource, resTypesID, ResTypes);

    std::vector<std::string> interfaceTypes = remoteResource->getInterface();
    jobject resInterfaces = JniVector(env).toJava(interfaceTypes);
    env->SetObjectField(resource, interfacesID, resInterfaces);

    env->SetBooleanField(resource, isObservableID, remoteResource->isObservable());

    return resource;
}