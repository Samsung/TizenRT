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
#include "simulator_resource_model_schema_jni.h"
#include "simulator_exceptions_jni.h"
#include "simulator_utils_jni.h"
#include "jni_sharedobject_holder.h"
#include "jni_listener_holder.h"
#include "jni_queryparam.h"
#include "jni_string.h"
#include "jni_vector.h"
#include "jni_map.h"

#include "simulator_remote_resource.h"

#define VALIDATE_OBJECT(ENV, OBJECT) if (!OBJECT){ThrowBadObjectException(ENV, "No corresponsing native object!"); return;}
#define VALIDATE_OBJECT_RET(ENV, OBJECT, RET) if (!OBJECT){ThrowBadObjectException(ENV, "No corresponsing native object!"); return RET;}

static SimulatorRemoteResourceSP simulatorRemoteResourceToCpp(
    JNIEnv *env, jobject object)
{
    JniSharedObjectHolder<SimulatorRemoteResource> *jniResource =
        getHandle<JniSharedObjectHolder<SimulatorRemoteResource>>(env, object);
    if (jniResource)
        return jniResource->get();
    return nullptr;
}

static void onObserveCallback(jobject listener, const std::string &uid, SimulatorResult /*result*/,
                              const SimulatorResourceModel &representation, const int seq)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = env->GetMethodID(listenerCls, "onObserveNotification",
                                 "(Ljava/lang/String;Lorg/oic/simulator/SimulatorResourceModel;I)V");

    jobject jResModel = SimulatorResourceModelToJava(env, representation);
    jstring jUid = env->NewStringUTF(uid.c_str());
    env->CallVoidMethod(listener, listenerMethodId, jUid, jResModel, seq);
    ReleaseEnv();
}

static void onGetCallback(jobject listener, const std::string &uid, SimulatorResult result,
                          const SimulatorResourceModel &representation)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = env->GetMethodID(listenerCls, "onGetResponse",
                                 "(Ljava/lang/String;Lorg/oic/simulator/SimulatorResult;Lorg/oic/simulator/SimulatorResourceModel;)V");

    jobject jResModel = SimulatorResourceModelToJava(env, representation);
    jstring jUid = env->NewStringUTF(uid.c_str());
    env->CallVoidMethod(listener, listenerMethodId, jUid,
                        SimulatorResultToJava(env, result), jResModel);

    ReleaseEnv();
}

static void onPutCallback(jobject listener, const std::string &uid, SimulatorResult result,
                          const SimulatorResourceModel &representation)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = env->GetMethodID(listenerCls, "onPutResponse",
                                 "(Ljava/lang/String;Lorg/oic/simulator/SimulatorResult;Lorg/oic/simulator/SimulatorResourceModel;)V");

    jobject jResModel = SimulatorResourceModelToJava(env, representation);
    jstring jUid = env->NewStringUTF(uid.c_str());
    env->CallVoidMethod(listener, listenerMethodId, jUid,
                        SimulatorResultToJava(env, result), jResModel);

    ReleaseEnv();
}

static void onPostCallback(jobject listener, const std::string &uid, SimulatorResult result,
                           const SimulatorResourceModel &representation)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = env->GetMethodID(listenerCls, "onPostResponse",
                                 "(Ljava/lang/String;Lorg/oic/simulator/SimulatorResult;Lorg/oic/simulator/SimulatorResourceModel;)V");

    jobject jResModel = SimulatorResourceModelToJava(env, representation);
    jstring jUid = env->NewStringUTF(uid.c_str());
    env->CallVoidMethod(listener, listenerMethodId, jUid,
                        SimulatorResultToJava(env, result), jResModel);

    ReleaseEnv();
}

static void onVerificationCallback(jobject listener, const std::string &uid, int id,
                                   OperationState opState)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = nullptr;
    if (OP_START == opState)
    {
        listenerMethodId = env->GetMethodID(listenerCls, "onVerificationStarted", "(Ljava/lang/String;I)V");
    }
    else if (OP_COMPLETE == opState)
    {
        listenerMethodId = env->GetMethodID(listenerCls, "onVerificationCompleted",
                                            "(Ljava/lang/String;I)V");
    }
    else if (OP_ABORT == opState)
    {
        listenerMethodId = env->GetMethodID(listenerCls, "onVerificationAborted", "(Ljava/lang/String;I)V");
    }

    // Invoke java callback method
    if (nullptr != listenerMethodId)
    {
        jstring jUid = env->NewStringUTF(uid.c_str());
        env->CallVoidMethod(listener, listenerMethodId, jUid, id);
    }

    if (OP_COMPLETE == opState || OP_ABORT == opState)
        env->DeleteGlobalRef(listener);
    ReleaseEnv();
}

class JniRequestType
{
    public:
        static jobject toJava(JNIEnv *env, RequestType requestType)
        {
            switch (requestType)
            {
                case RequestType::RQ_TYPE_GET:
                    {
                        static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.requestTypeCls,
                                                  "GET",
                                                  "Lorg/oic/simulator/client/SimulatorRemoteResource$RequestType;");
                        return env->GetStaticObjectField(gSimulatorClassRefs.requestTypeCls, fieldID);
                    }

                case RequestType::RQ_TYPE_PUT:
                    {
                        static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.requestTypeCls,
                                                  "PUT",
                                                  "Lorg/oic/simulator/client/SimulatorRemoteResource$RequestType;");
                        return env->GetStaticObjectField(gSimulatorClassRefs.requestTypeCls, fieldID);
                    }

                case RequestType::RQ_TYPE_POST:
                    {
                        static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.requestTypeCls,
                                                  "POST",
                                                  "Lorg/oic/simulator/client/SimulatorRemoteResource$RequestType;");
                        return env->GetStaticObjectField(gSimulatorClassRefs.requestTypeCls, fieldID);
                    }

                case RequestType::RQ_TYPE_DELETE:
                    {
                        static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.requestTypeCls,
                                                  "DELETE",
                                                  "Lorg/oic/simulator/client/SimulatorRemoteResource$RequestType;");
                        return env->GetStaticObjectField(gSimulatorClassRefs.requestTypeCls, fieldID);
                    }

                default:
                    break;
            }

            static jfieldID fieldID = env->GetStaticFieldID(gSimulatorClassRefs.requestTypeCls,
                                      "UKNOWN",
                                      "Lorg/oic/simulator/client/SimulatorRemoteResource$RequestType;");
            return env->GetStaticObjectField(gSimulatorClassRefs.requestTypeCls, fieldID);
        }

        static RequestType toCpp(JNIEnv *env, jobject jRequestType)
        {
            static jmethodID ordinalMID = env->GetMethodID(
                                              gSimulatorClassRefs.requestTypeCls, "ordinal", "()I");
            int ordinal = env->CallIntMethod(jRequestType, ordinalMID);
            return RequestType(ordinal);
        }
};

class JniSimulatorRequestModel
{
    public:
        static jobject toJava(JNIEnv *env, const SimulatorRequestModel &requestModel)
        {
            // Convert RequestType
            jobject jRequestType = JniRequestType::toJava(env, requestModel.getType());

            // Convert Query Parameters
            JniMap jQueryParams(env);
            for (auto &qpEntry : requestModel.getQueryParams())
            {
                jstring jQueryParamName = env->NewStringUTF((qpEntry.first).c_str());
                jobject jQueryParamValues = JniVector(env).toJava(qpEntry.second);

                jQueryParams.put(jQueryParamName, jQueryParamValues);
            }

            // Convert Request body model
            jobject jRequestBodyModel  =
                AttributePropertyToJava(env, requestModel.getRequestBodySchema());

            // Create Java SimulatorResourceModel object
            static jmethodID simulatorRequestModelCtor = env->GetMethodID(
                        gSimulatorClassRefs.simulatorRequestModelCls, "<init>",
                        "(Lorg/oic/simulator/client/SimulatorRemoteResource$RequestType;"
                        "Ljava/util/Map;Lorg/oic/simulator/ModelProperty;)V");

            return env->NewObject(gSimulatorClassRefs.simulatorRequestModelCls,
                                  simulatorRequestModelCtor, jRequestType,
                                  jQueryParams.get(), jRequestBodyModel);
        }
};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_org_oic_simulator_client_SimulatorRemoteResource_nativeGet
(JNIEnv *env, jobject object, jstring jResInterface, jobject jQueryParams, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniInterface(env, jResInterface);
        std::map<std::string, std::string> queryParams =
            JniQueryParameter(env).toCpp(jQueryParams);

        SimulatorRemoteResource::ResponseCallback callback =  std::bind([](
                    const std::string & uid, SimulatorResult result,
                    const SimulatorResourceModel & resModel,
                    const std::shared_ptr<JniListenerHolder> &listenerRef)
        {
            onGetCallback(listenerRef->get(), uid, result, resModel);
        }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
        JniListenerHolder::create(env, jListener));

        resource->get(jniInterface.get(), queryParams, callback);
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
Java_org_oic_simulator_client_SimulatorRemoteResource_nativePut
(JNIEnv *env, jobject object, jstring jResInterface, jobject jQueryParams,
 jobject jRepresentation, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniInterface(env, jResInterface);
        std::map<std::string, std::string> queryParams =
            JniQueryParameter(env).toCpp(jQueryParams);

        SimulatorResourceModel resModel;
        SimulatorResourceModelToCpp(env, jRepresentation, resModel);

        SimulatorRemoteResource::ResponseCallback callback =  std::bind([](
                    const std::string & uid, SimulatorResult result,
                    const SimulatorResourceModel & resModel,
                    const std::shared_ptr<JniListenerHolder> &listenerRef)
        {
            onPutCallback(listenerRef->get(), uid, result, resModel);
        }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
        JniListenerHolder::create(env, jListener));

        resource->put(jniInterface.get(), queryParams, resModel, callback);
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
Java_org_oic_simulator_client_SimulatorRemoteResource_nativePost
(JNIEnv *env, jobject object, jstring jResInterface, jobject jQueryParams,
 jobject jRepresentation, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        JniString jniInterface(env, jResInterface);
        std::map<std::string, std::string> queryParams =
            JniQueryParameter(env).toCpp(jQueryParams);

        SimulatorResourceModel resModel;
        SimulatorResourceModelToCpp(env, jRepresentation, resModel);

        SimulatorRemoteResource::ResponseCallback callback =  std::bind([](
                    const std::string & uid, SimulatorResult result,
                    const SimulatorResourceModel & resModel,
                    const std::shared_ptr<JniListenerHolder> &listenerRef)
        {
            onPostCallback(listenerRef->get(), uid, result, resModel);
        }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
        JniListenerHolder::create(env, jListener));

        resource->post(jniInterface.get(), queryParams, resModel, callback);
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

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_client_SimulatorRemoteResource_nativeSetConfigInfo
(JNIEnv *env, jobject object, jstring jConfigPath)
{
    VALIDATE_INPUT_RET(env, !jConfigPath, "Path is null!", nullptr)

    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, nullptr)

    try
    {
        JniString jniConfigPath(env, jConfigPath);
        std::map<RequestType, SimulatorRequestModel> requestModels =
            resource->configure(jniConfigPath.get());

        JniMap jRequestModelsMap(env);

        // Add entry to map
        for (auto &requestModelEntry : requestModels)
        {
            jobject jRequestType = JniRequestType::toJava(env, requestModelEntry.first);
            jobject jRequestModel = JniSimulatorRequestModel::toJava(env, requestModelEntry.second);

            jRequestModelsMap.put(jRequestType, jRequestModel);
        }

        return jRequestModelsMap.get();
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }

    return nullptr;
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_client_SimulatorRemoteResource_nativeStartObserve
(JNIEnv *env, jobject object, jobject jQueryParams, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        std::map<std::string, std::string> queryParams =
            JniQueryParameter(env).toCpp(jQueryParams);

        SimulatorRemoteResource::ObserveNotificationCallback callback =  std::bind([](
                    const std::string & uid, SimulatorResult result,
                    const SimulatorResourceModel & representation, const int seq,
                    const std::shared_ptr<JniListenerHolder> &listenerRef)
        {
            onObserveCallback(listenerRef->get(), uid, result, representation, seq);
        }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
        std::placeholders::_4, JniListenerHolder::create(env, jListener));

        resource->observe(ObserveType::OBSERVE, callback);
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
Java_org_oic_simulator_client_SimulatorRemoteResource_nativeStopObserve
(JNIEnv *env, jobject object)
{
    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->cancelObserve();
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT jint JNICALL
Java_org_oic_simulator_client_SimulatorRemoteResource_nativeStartAutoRequesting
(JNIEnv *env, jobject object, jobject jRequestType, jobject jListener)
{
    VALIDATE_CALLBACK_RET(env, jListener, -1)

    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT_RET(env, resource, -1)

    try
    {
        RequestType type = JniRequestType::toCpp(env, jRequestType);
        SimulatorRemoteResource::AutoRequestGenerationCallback callback =  std::bind([](
                    const std::string & uid, int id, OperationState opState,
                    const std::shared_ptr<JniListenerHolder> &listenerRef)
        {
            onVerificationCallback(listenerRef->get(), uid, id, opState);
        }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
        JniListenerHolder::create(env, jListener));

        return resource->startAutoRequesting(type, callback);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (NoSupportException &e)
    {
        ThrowNoSupportException(env, e.what());
    }
    catch (OperationInProgressException &e)
    {
        ThrowOperationInProgressException(env, e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return -1;
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_client_SimulatorRemoteResource_nativeStopAutoRequesting
(JNIEnv *env, jobject object, jint id)
{
    SimulatorRemoteResourceSP resource = simulatorRemoteResourceToCpp(env, object);
    VALIDATE_OBJECT(env, resource)

    try
    {
        resource->stopAutoRequesting(id);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (NoSupportException &e)
    {
        ThrowNoSupportException(env, e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_client_SimulatorRemoteResource_nativeDispose
(JNIEnv *env, jobject object)
{
    JniSharedObjectHolder<SimulatorRemoteResource> *resource =
        getHandle<JniSharedObjectHolder<SimulatorRemoteResource>>(env, object);
    delete resource;
}

#ifdef __cplusplus
}
#endif
