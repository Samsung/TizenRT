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
#include "simulator_device_info_jni.h"
#include "simulator_platform_info_jni.h"
#include "simulator_exceptions_jni.h"
#include "simulator_resource_utils_jni.h"
#include "simulator_utils_jni.h"
#include "jni_sharedobject_holder.h"
#include "jni_listener_holder.h"
#include "jni_string.h"

#include "simulator_manager.h"

class JNILogger : public ILogger
{
    public:
        void setJavaLogger(JNIEnv *env, jobject logger)
        {
            m_logger = env->NewWeakGlobalRef(logger);
        }

        void write(std::string time, ILogger::Level level, std::string message)
        {
            JNIEnv *env = GetEnv();
            if (nullptr == env)
                return;

            jobject logger = env->NewLocalRef(m_logger);
            if (!logger)
            {
                ReleaseEnv();
                return;
            }

            jclass loggerCls = env->GetObjectClass(logger);
            if (!loggerCls)
            {
                ReleaseEnv();
                return;
            }

            jmethodID writeMId = env->GetMethodID(loggerCls, "write",
                                                  "(Ljava/lang/String;ILjava/lang/String;)V");
            if (!writeMId)
            {
                ReleaseEnv();
                return;
            }

            jstring msg = env->NewStringUTF(message.c_str());
            jstring timeStr = env->NewStringUTF(time.c_str());
            env->CallVoidMethod(logger, writeMId, timeStr, static_cast<jint>(level), msg);
            env->DeleteLocalRef(msg);
            env->DeleteLocalRef(timeStr);
            ReleaseEnv();
        }

    private:
        jweak m_logger;
};

static void onResourceFound(jobject listener, SimulatorRemoteResourceSP remoteResource)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID callbackMethod = env->GetMethodID(listenerCls, "onResourceFound",
                               "(Lorg/oic/simulator/client/SimulatorRemoteResource;)V");

    jobject resource = CreateSimulatorRemoteResource(env, remoteResource);
    if (!resource)
        return;

    env->CallVoidMethod(listener, callbackMethod, resource);
    ReleaseEnv();
}

static void onDeviceInfoReceived(jobject listener, const std::string &hostUri,
                                 DeviceInfo &deviceInfo)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = env->GetMethodID(listenerCls, "onDeviceFound",
                                 "(Ljava/lang/String;Lorg/oic/simulator/DeviceInfo;)V");


    jstring jHostUri = env->NewStringUTF(hostUri.c_str());
    jobject jDeviceInfo = JniDeviceInfo(env).toJava(deviceInfo);
    if (!jDeviceInfo)
    {
        ReleaseEnv();
        return;
    }

    env->CallVoidMethod(listener, listenerMethodId, jHostUri, jDeviceInfo);
    if (env->ExceptionCheck())
    {
        ReleaseEnv();
        return;
    }

    ReleaseEnv();
}

static void onPlatformInfoReceived(jobject listener, const std::string &hostUri,
                                   PlatformInfo &platformInfo)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = env->GetMethodID(listenerCls, "onPlatformFound",
                                 "(Ljava/lang/String;Lorg/oic/simulator/PlatformInfo;)V");

    jstring jHostUri = env->NewStringUTF(hostUri.c_str());
    jobject jPlatformInfo = JniPlatformInfo(env).toJava(platformInfo);
    if (!jPlatformInfo)
    {
        ReleaseEnv();
        return;
    }

    env->CallVoidMethod(listener, listenerMethodId, jHostUri, jPlatformInfo);
    if (env->ExceptionCheck())
    {
        ReleaseEnv();
        return;
    }

    ReleaseEnv();
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_SimulatorManager_nativeCreateResource
(JNIEnv *env, jobject /*object*/, jstring jConfigPath)
{
    VALIDATE_INPUT_RET(env, !jConfigPath, "Path is null!", nullptr)

    try
    {
        JniString jniPath(env, jConfigPath);
        SimulatorResourceSP resource = SimulatorManager::getInstance()->createResource(
                                           jniPath.get());
        return CreateSimulatorResource(env, resource);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return nullptr;
}

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_SimulatorManager_nativeCreateResources
(JNIEnv *env, jobject /*object*/, jstring jConfigPath, jint jCount)
{
    VALIDATE_INPUT_RET(env, !jConfigPath, "Path is null!", nullptr)
    VALIDATE_INPUT_RET(env, !jConfigPath || jCount < 0, "Invalid count value!", nullptr)

    try
    {
        JniString jniPath(env, jConfigPath);
        std::vector<SimulatorResourceSP> resources =
            SimulatorManager::getInstance()->createResource(jniPath.get(), jCount);
        return CreateSimulatorResourceVector(env, resources);
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return nullptr;
}

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_SimulatorManager_nativeCreateSingleResource
(JNIEnv *env, jobject /*object*/, jstring jName, jstring jUri, jstring jResourceType)
{
    VALIDATE_INPUT_RET(env, !jName, "Name is null!", nullptr)
    VALIDATE_INPUT_RET(env, !jUri, "URI is null!", nullptr)
    VALIDATE_INPUT_RET(env, !jResourceType, "Resource type is null!", nullptr)

    try
    {
        JniString jniName(env, jName);
        JniString jniUri(env, jUri);
        JniString jniResourceType(env, jResourceType);

        SimulatorSingleResourceSP resource = SimulatorManager::getInstance()->createSingleResource(
                jniName.get(), jniUri.get(), jniResourceType.get());
        return CreateSimulatorResource(env, std::dynamic_pointer_cast<SimulatorResource>(resource));
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return nullptr;
}

JNIEXPORT jobject JNICALL
Java_org_oic_simulator_SimulatorManager_nativeCreateCollectionResource
(JNIEnv *env, jobject /*object*/, jstring jName, jstring jUri, jstring jResourceType)
{
    VALIDATE_INPUT_RET(env, !jName, "Name is null!", nullptr)
    VALIDATE_INPUT_RET(env, !jUri, "URI is null!", nullptr)
    VALIDATE_INPUT_RET(env, !jResourceType, "Resource type is null!", nullptr)

    try
    {
        JniString jniName(env, jName);
        JniString jniUri(env, jUri);
        JniString jniResourceType(env, jResourceType);

        SimulatorCollectionResourceSP resource = SimulatorManager::getInstance()->createCollectionResource(
                    jniName.get(), jniUri.get(), jniResourceType.get());
        return CreateSimulatorResource(env, std::dynamic_pointer_cast<SimulatorResource>(resource));
    }
    catch (InvalidArgsException &e)
    {
        ThrowInvalidArgsException(env, e.code(), e.what());
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }

    return nullptr;
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_SimulatorManager_nativeSearchResource
(JNIEnv *env, jobject /*object*/, jstring jResourceType, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    ResourceFindCallback callback =  std::bind([](
                                         std::shared_ptr<SimulatorRemoteResource> resource,
                                         const std::shared_ptr<JniListenerHolder> &listenerRef)
    {
        onResourceFound(listenerRef->get(), resource);
    }, std::placeholders::_1, JniListenerHolder::create(env, jListener));

    try
    {
        if (!jResourceType)
        {
            SimulatorManager::getInstance()->findResource(callback);
        }
        else
        {
            JniString type(env, jResourceType);
            SimulatorManager::getInstance()->findResource(type.get(), callback);
        }

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
Java_org_oic_simulator_SimulatorManager_nativeSetDeviceInfo
(JNIEnv *env, jobject /*object*/, jstring jDeviceName)
{
    VALIDATE_INPUT(env, !jDeviceName, "Device name is null!")

    try
    {
        JniString jniDeviceName(env, jDeviceName);
        SimulatorManager::getInstance()->setDeviceInfo(jniDeviceName.get());
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
Java_org_oic_simulator_SimulatorManager_nativeFindDevices
(JNIEnv *env, jobject /*object*/, jstring jHostUri, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    DeviceInfoCallback callback =  std::bind([](const std::string & host, DeviceInfo & deviceInfo,
                                   const std::shared_ptr<JniListenerHolder> &listenerRef)
    {
        onDeviceInfoReceived(listenerRef->get(), host, deviceInfo);
    }, std::placeholders::_1, std::placeholders::_2, JniListenerHolder::create(env, jListener));

    try
    {
        JniString jniHostUri(env, jHostUri);
        SimulatorManager::getInstance()->getDeviceInfo(jniHostUri.get(), callback);
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
Java_org_oic_simulator_SimulatorManager_nativeSetPlatformInfo
(JNIEnv *env, jobject /*object*/, jobject jPlatformInfo)
{
    VALIDATE_INPUT(env, !jPlatformInfo, "Platform info is null!")

    try
    {
        PlatformInfo info = JniPlatformInfo(env).toCpp(jPlatformInfo);
        SimulatorManager::getInstance()->setPlatformInfo(info);
    }
    catch (SimulatorException &e)
    {
        ThrowSimulatorException(env, e.code(), e.what());
    }
}

JNIEXPORT void JNICALL
Java_org_oic_simulator_SimulatorManager_nativeGetPlatformInformation
(JNIEnv *env, jobject /*object*/, jstring jHostUri, jobject jListener)
{
    VALIDATE_CALLBACK(env, jListener)

    PlatformInfoCallback callback =  std::bind([](const std::string & host,
                                     PlatformInfo & platformInfo,
                                     const std::shared_ptr<JniListenerHolder> &listenerRef)
    {
        onPlatformInfoReceived(listenerRef->get(), host, platformInfo);
    }, std::placeholders::_1, std::placeholders::_2, JniListenerHolder::create(env, jListener));

    try
    {
        JniString jniHostUri(env, jHostUri);
        SimulatorManager::getInstance()->getPlatformInfo(jniHostUri.get(), callback);
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
Java_org_oic_simulator_SimulatorManager_nativeSetLogger
(JNIEnv *env, jobject /*object*/, jobject jLogger)
{
    static std::shared_ptr<JNILogger> target(new JNILogger());
    target->setJavaLogger(env, jLogger);
    SimulatorManager::getInstance()->setLogger(target);
}

#ifdef __cplusplus
}
#endif