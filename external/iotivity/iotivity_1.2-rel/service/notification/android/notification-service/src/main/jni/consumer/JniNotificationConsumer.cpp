//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "JniNotificationConsumer.h"
#include "NSConsumerService.h"
#include "NSException.h"
#include "JniOcRepresentation.h"

static JavaVM *g_jvm_consumer = NULL;

static jobject g_obj_postListener = NULL;
static jobject g_obj_syncListener = NULL;
static jobject g_obj_discoverListener = NULL;
static jobject g_obj_acceptListener = NULL;

jclass g_cls_Message;
jclass g_cls_Provider;
jclass g_cls_SyncInfo;
jclass g_cls_SyncType;
jclass g_cls_MediaContents;
jclass g_cls_TopicState;
jclass g_cls_Message_Type;
jclass g_cls_ProviderState;
jclass g_cls_Topic;
jclass g_cls_TopicsList;
jclass g_cls_OcRepresentation;
jmethodID g_mid_OcRepresentation_N_ctor_bool = NULL;

static JNIEnv *GetJNIEnv(jint *ret)
{
    JNIEnv *env = NULL;

    *ret = g_jvm_consumer->GetEnv((void **) &env, JNI_CURRENT_VERSION);
    switch (*ret)
    {
        case JNI_OK:
            return env;
        case JNI_EDETACHED:
            if (g_jvm_consumer->AttachCurrentThread(&env, NULL) != JNI_OK)
            {
                NS_LOGE ("Failed to get the environment");
                return NULL;
            }
            else
            {
                return env;
            }
        case JNI_EVERSION:
            NS_LOGE ("JNI version is not supported");
            return NULL;
        default:
            NS_LOGE ("Failed to get the environment");
            return NULL;
    }
}

jobject getJavaProviderState(JNIEnv *env, OIC::Service::NSProviderState state)
{
    NS_LOGD ("ConsumerService_getJavaProviderState - IN");
    jobject providerState = NULL;
    switch (state)
    {
        case OIC::Service::NSProviderState::ALLOW:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_ProviderState,
                                          "ALLOW", "Lorg/iotivity/service/ns/consumer/Provider$ProviderState;");
                providerState = env->GetStaticObjectField(g_cls_ProviderState, fieldID);
                break;
            }
        case OIC::Service::NSProviderState::DENY:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_ProviderState,
                                          "DENY", "Lorg/iotivity/service/ns/consumer/Provider$ProviderState;");
                providerState = env->GetStaticObjectField(g_cls_ProviderState, fieldID);
                break;
            }
        case OIC::Service::NSProviderState::TOPIC:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_ProviderState,
                                          "TOPIC", "Lorg/iotivity/service/ns/consumer/Provider$ProviderState;");
                providerState = env->GetStaticObjectField(g_cls_ProviderState, fieldID);
                break;
            }
        case OIC::Service::NSProviderState::STOPPED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_ProviderState,
                                          "STOPPED", "Lorg/iotivity/service/ns/consumer/Provider$ProviderState;");
                providerState = env->GetStaticObjectField(g_cls_ProviderState, fieldID);
                break;
            }
        default:
            {
                providerState = NULL;
                break;
            }
    }
    NS_LOGD ("ConsumerService_getJavaProviderState - OUT");
    return providerState;
}

jobject getJavaSyncType(JNIEnv *env, OIC::Service::NSSyncInfo::NSSyncType nsType)
{
    NS_LOGD ("ConsumerService_getJavaSyncType - IN");

    // SyncType
    jclass cls_SyncType = (jclass) (env->NewLocalRef(g_cls_SyncType));
    if (!cls_SyncType)
    {
        NS_LOGE ("Failed to Get ObjectClass for SyncType");
        return NULL;
    }
    jobject syncType = NULL;
    switch (nsType)
    {
        case OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_UNREAD:
            {
                static jfieldID fieldID = env->GetStaticFieldID(cls_SyncType,
                                          "UNREAD", "Lorg/iotivity/service/ns/common/SyncInfo$SyncType;");
                syncType = env->GetStaticObjectField(cls_SyncType, fieldID);
                break;
            }
        case OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ :
            {
                static jfieldID fieldID = env->GetStaticFieldID(cls_SyncType,
                                          "READ", "Lorg/iotivity/service/ns/common/SyncInfo$SyncType;");
                syncType = env->GetStaticObjectField(cls_SyncType, fieldID);
                break;
            }
        case OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED :
            {
                static jfieldID fieldID = env->GetStaticFieldID(cls_SyncType,
                                          "DELETED", "Lorg/iotivity/service/ns/common/SyncInfo$SyncType;");
                syncType = env->GetStaticObjectField(cls_SyncType, fieldID);
                break;
            }
        default:
            {
                syncType = NULL;
                break;
            }
    }

    NS_LOGD ("ConsumerService_getJavaSyncType - OUT");
    return syncType;
}

jobject getJavaTopicState(JNIEnv *env, OIC::Service::NSTopic::NSTopicState nsState)
{
    NS_LOGD ("ConsumerService_getJavaTopicState - IN");

    // TopicState
    jclass cls_topicState = (jclass) (env->NewLocalRef(g_cls_TopicState));
    if (!cls_topicState)
    {
        NS_LOGE ("Failed to Get ObjectClass for TopicState Type");
        return NULL;
    }
    jobject topicState = NULL;

    switch (nsState)
    {
        case OIC::Service::NSTopic::NSTopicState::UNSUBSCRIBED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(cls_topicState,
                                          "UNSUBSCRIBED", "Lorg/iotivity/service/ns/common/Topic$TopicState;");
                topicState = env->GetStaticObjectField(cls_topicState, fieldID);
                break;
            }
        case OIC::Service::NSTopic::NSTopicState::SUBSCRIBED:
            {
                static jfieldID fieldID = env->GetStaticFieldID(cls_topicState,
                                          "SUBSCRIBED", "Lorg/iotivity/service/ns/common/Topic$TopicState;");
                topicState = env->GetStaticObjectField(cls_topicState, fieldID);
                break;
            }
        default:
            {
                topicState = NULL;
                break;
            }
    }

    NS_LOGD ("ConsumerService_getJavaTopicState - OUT");
    return topicState;
}

jobject getJavaTopicsList(JNIEnv *env, std::shared_ptr<OIC::Service::NSTopicsList> topicList)
{
    NS_LOGD ("ConsumerService_getJavaTopicsList - IN");
    jclass cls_topicList = (jclass) (env->NewLocalRef(g_cls_TopicsList));
    if (!cls_topicList)
    {
        NS_LOGE ("Failed to Get ObjectClass for TopicsList");
        return NULL;
    }
    jmethodID mid_topicList = env->GetMethodID(cls_topicList, "<init>", "()V");
    if (!mid_topicList)
    {
        NS_LOGE ("Failed to Get MethodID for TopicsList<init>");
        return NULL;
    }
    jobject obj_topicList = env->NewObject(cls_topicList, mid_topicList);
    if (!obj_topicList)
    {
        NS_LOGE ("Failed to Get object for TopicsList");
        return NULL;
    }
    jmethodID mid_addTopic = env->GetMethodID(cls_topicList, "addTopic",
                             "(Ljava/lang/String;Lorg/iotivity/service/ns/common/Topic$TopicState;)V");
    if (!mid_addTopic)
    {
        NS_LOGE ("Failed to Get MethodID for addTopic");
        return NULL;
    }
    for (auto it : topicList->getTopicsList())
    {
        jobject jState = getJavaTopicState(env, it.getState());
        std::string topicName = it.getTopicName();
        jstring jTopicName = env->NewStringUTF(topicName.c_str());
        env->CallVoidMethod(obj_topicList, mid_addTopic, jTopicName, jState);
    }
    env->DeleteLocalRef(cls_topicList);
    NS_LOGD ("ConsumerService_getJavaTopicsList - OUT");
    return obj_topicList;
}

bool getNativeTopicState(JNIEnv *env,  jobject jTopic , OIC::Service::NSTopic::NSTopicState &state )
{
    NS_LOGD ("ConsumerService_getNativeTopicState - IN");

    jclass cls_topic = env->GetObjectClass( jTopic);
    // TopicState
    jclass cls_TopicState = (jclass) (env->NewLocalRef(g_cls_TopicState));
    if (!cls_TopicState)
    {
        NS_LOGE ("Failed to Get ObjectClass for cls_TopicState Type");
        return false;
    }
    jmethodID mid = env->GetMethodID(cls_TopicState, "ordinal", "()I");
    jfieldID fid_state = env->GetFieldID( cls_topic, "mState",
                                          "Lorg/iotivity/service/ns/common/Topic$TopicState;");
    if (fid_state == NULL)
    {
        NS_LOGE ("Error: jfieldID for state type  is null");
        return false;
    }
    jobject jobj = env->GetObjectField( jTopic, fid_state);
    if (jobj == NULL)
    {
        NS_LOGE ("Error: object of field  state Type is null");
        return false;
    }
    jint jState = env->CallIntMethod(jobj, mid);
    state = (OIC::Service::NSTopic::NSTopicState) jState;
    NS_LOGD ("ConsumerService_getNativeTopicState - OUT");
    return true;

}

const char *getNativeTopicName(JNIEnv *env,  jobject jTopic)
{
    NS_LOGD ("ConsumerService_getNativeTopicName - IN");
    jclass cls_topic = env->GetObjectClass( jTopic);
    if (cls_topic == NULL)
    {
        NS_LOGE ("Error: Class for Topic is null");
        return nullptr;
    }
    jfieldID fid_name = env->GetFieldID( cls_topic, "mTopicName", "Ljava/lang/String;");
    if (fid_name == NULL)
    {
        NS_LOGE ("Error: jfieldID for Topic Name  is null");
        return nullptr;
    }
    jstring jTopicName = (jstring) env->GetObjectField( jTopic, fid_name);
    const char *topicName = NULL;
    if (jTopicName)
    {
        topicName = env->GetStringUTFChars( jTopicName, NULL);
    }
    else
    {
        NS_LOGI (TAG, "Info: topicName is null");
    }
    NS_LOGD ("ConsumerService_getNativeTopicName - OUT");
    return topicName;

}

std::shared_ptr<OIC::Service::NSTopicsList> getNativeTopicsList(JNIEnv *env, jobject jTopicList)
{
    NS_LOGD ("ConsumerService_getNativeTopicsList - IN");

    jclass cls_topicList = env->GetObjectClass( jTopicList);
    if (cls_topicList == NULL)
    {
        NS_LOGE ("Error: Class for Topic List  is null");
        return nullptr;
    }
    jfieldID fid_list = env->GetFieldID( cls_topicList, "mTopicsList", "Ljava/util/Vector;");
    if (fid_list == NULL)
    {
        NS_LOGE ("Error: jfieldID for Topic List  is null");
        return nullptr;
    }
    jobject jobj = env->GetObjectField( jTopicList, fid_list);
    if (jobj == NULL)
    {
        NS_LOGE ("Error: object of field  Topic List is null");
        return nullptr;
    }
    jclass cls_vec = env->FindClass("java/util/Vector");
    if (cls_vec == NULL)
    {
        NS_LOGE ("Error: Class for Vector not found");
        return nullptr;
    }
    jmethodID sizeMethod = env->GetMethodID(cls_vec,  "size", "()I");
    if (sizeMethod == NULL)
    {
        NS_LOGE ("Error: MethodId for Vector Size  not found");
        return nullptr;
    }
    int size = env->CallIntMethod(jobj, sizeMethod);
    jmethodID getMethod = env->GetMethodID(cls_vec,  "get", "(I)Ljava/lang/Object;");
    if (getMethod == NULL)
    {
        NS_LOGE ("Error: MethodId for Vector get  not found");
        return nullptr;
    }
    std::shared_ptr<OIC::Service::NSTopicsList> nsTopicList =
        std::make_shared<OIC::Service::NSTopicsList>();
    for (int index = 0; index < size; index++)
    {
        jobject topicObj = env->CallObjectMethod(jobj, getMethod, index);
        if (topicObj == NULL)
        {
            NS_LOGE ("Error: object of field  Topic  is null");
            return nullptr;
        }
        const char *name =  getNativeTopicName(env, topicObj);
        if (name == nullptr)
        {
            NS_LOGE ("Error: Couldn't find topic Name");
            return nullptr;
        }
        std::string topicName(name);
        OIC::Service::NSTopic::NSTopicState state = OIC::Service::NSTopic::NSTopicState::UNSUBSCRIBED;
        if (!getNativeTopicState(env, topicObj, state))
        {
            return nullptr;
        }
        nsTopicList->addTopic(topicName, state);
    }

    env->DeleteLocalRef(cls_vec);
    env->DeleteLocalRef(cls_topicList);
    NS_LOGD ("ConsumerService_getNativeTopicsList - OUT");
    return nsTopicList;
}

jobject getJavaProvider(JNIEnv *env, std::shared_ptr<OIC::Service::NSProvider> provider)
{
    NS_LOGD ("ConsumerService_getJavaProvider - IN");
    NS_LOGD ("ProviderId : %s\n", provider->getProviderId().c_str());

    jstring jProviderId = env->NewStringUTF(provider->getProviderId().c_str());
    auto *objectHolder = JniSharedObjectHolder<OIC::Service::NSProvider>::create(provider);
    if (!objectHolder)
    {
        NS_LOGE ("Failed to create new Object Holder for Provider");
        return NULL;
    }
    jlong pProvider = (long)objectHolder;

    jclass cls_provider = (jclass) (env->NewLocalRef(g_cls_Provider));
    if (!cls_provider)
    {
        NS_LOGE ("Failed to Get ObjectClass for Provider");
        return NULL;
    }
    jmethodID mid_provider = env->GetMethodID(
                                 cls_provider, "<init>", "(Ljava/lang/String;)V");
    if (!mid_provider)
    {
        NS_LOGE ("Failed to Get MethodID for Provider<init>");
        return NULL;
    }
    jobject obj_provider = env->NewObject(cls_provider, mid_provider, jProviderId);
    if (!obj_provider)
    {
        NS_LOGE ("Failed to create new Object for Provider");
        return NULL;
    }

    jfieldID nativeHandle = env->GetFieldID(cls_provider, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        NS_LOGE ("Failed to get nativeHandle for Provider");
        return NULL;
    }
    env->SetLongField(obj_provider, nativeHandle, pProvider);

    env->DeleteLocalRef(cls_provider);
    NS_LOGD ("ConsumerService_getJavaProvider - OUT");
    return obj_provider;
}

jobject getJavaMessageType(JNIEnv *env, OIC::Service::NSMessage::NSMessageType type)
{
    NS_LOGD ("ConsumerService_getJavaMessageType - IN");
    jobject messageType = NULL;
    switch (type)
    {
        case OIC::Service::NSMessage::NSMessageType::NS_MESSAGE_ALERT:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_Message_Type,
                                          "ALERT", "Lorg/iotivity/service/ns/common/Message$MessageType;");
                messageType = env->GetStaticObjectField(g_cls_Message_Type, fieldID);
                break;
            }
        case OIC::Service::NSMessage::NSMessageType::NS_MESSAGE_NOTICE:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_Message_Type,
                                          "NOTICE", "Lorg/iotivity/service/ns/common/Message$MessageType;");
                messageType = env->GetStaticObjectField(g_cls_Message_Type, fieldID);
                break;
            }
        case OIC::Service::NSMessage::NSMessageType::NS_MESSAGE_EVENT:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_Message_Type,
                                          "EVENT", "Lorg/iotivity/service/ns/common/Message$MessageType;");
                messageType = env->GetStaticObjectField(g_cls_Message_Type, fieldID);
                break;
            }
        case OIC::Service::NSMessage::NSMessageType::NS_MESSAGE_INFO:
            {
                static jfieldID fieldID = env->GetStaticFieldID(g_cls_Message_Type,
                                          "INFO", "Lorg/iotivity/service/ns/common/Message$MessageType;");
                messageType = env->GetStaticObjectField(g_cls_Message_Type, fieldID);
                break;
            }
        default:
            {
                messageType = NULL;
                break;
            }
    }
    NS_LOGD ("ConsumerService_getJavaMessageType - OUT");
    return messageType;
}

jobject getJavaMessage(JNIEnv *env, OIC::Service::NSMessage message)
{
    NS_LOGD ("ConsumerService_getJavaMessage - IN");

    NS_LOGD ("id : %llu\n", message.getMessageId());
    NS_LOGD ("title : %s\n", message.getTitle().c_str());
    NS_LOGD ("content : %s\n", message.getContentText().c_str());
    NS_LOGD ("source : %s\n", message.getSourceName().c_str());

    jlong jMessageId = (jlong) message.getMessageId();
    jstring jProviderId = env->NewStringUTF(message.getProviderId().c_str());
    jstring jTitle = env->NewStringUTF(message.getTitle().c_str());
    jstring jContentText = env->NewStringUTF(message.getContentText().c_str());
    jstring jSourceName = env->NewStringUTF(message.getSourceName().c_str());
    jstring jTopic = env->NewStringUTF(message.getTopic().c_str());

    jstring jTime = env->NewStringUTF(message.getTime().c_str());
    jlong jTTL = (jlong) message.getTTL();

    jclass cls_message = (jclass) (env->NewLocalRef(g_cls_Message));
    if (!cls_message)
    {
        NS_LOGE ("Failed to Get ObjectClass for Message");
        return NULL ;
    }
    jmethodID mid_message = env->GetMethodID(
                                cls_message, "<init>",
                                "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid_message)
    {
        NS_LOGE ("Failed to Get MethodID for Message<init>");
        return NULL;
    }
    jobject obj_message = env->NewObject(cls_message, mid_message,
                                         jTitle, jContentText, jSourceName);
    if (!obj_message)
    {
        NS_LOGE ("Failed to Get Java Object for Message");
        return NULL;
    }

    jfieldID fid_messageId = env->GetFieldID(cls_message, "mMessageId", "J");
    if (!fid_messageId)
    {
        NS_LOGE ("Failed to get field MessageID for Message");
        return NULL;
    }
    env->SetLongField(obj_message, fid_messageId, jMessageId);

    jfieldID fid_providerId = env->GetFieldID(cls_message, "mProviderId", "Ljava/lang/String;");
    if (!fid_providerId)
    {
        NS_LOGE ("Failed to get field ProviderID for Message");
        return NULL;
    }
    env->SetObjectField(obj_message, fid_providerId, jProviderId);

    jfieldID fid_time = env->GetFieldID(cls_message, "mTime", "Ljava/lang/String;");
    if (!fid_time)
    {
        NS_LOGE ("Failed to get field Time for Message");
        return NULL;
    }
    env->SetObjectField(obj_message, fid_time, jTime);

    jfieldID fid_ttl = env->GetFieldID(cls_message, "mTTL", "J");
    if (!fid_ttl)
    {
        NS_LOGE ("Failed to get field TTL for Message");
        return NULL;
    }
    env->SetLongField(obj_message, fid_ttl, jTTL);

    jfieldID fid_topic = env->GetFieldID(cls_message, "mTopic", "Ljava/lang/String;");
    if (!fid_topic)
    {
        NS_LOGE ("Failed to get mTopic for Message");
        return NULL;
    }
    env->SetObjectField(obj_message, fid_topic, jTopic);

    OIC::Service::NSMediaContents *mediaCont = message.getMediaContents();
    if (mediaCont != nullptr)
    {
        jstring jIconImage = env->NewStringUTF(mediaCont->getIconImage().c_str());
        jclass cls_mediaContents = (jclass) (env->NewLocalRef(g_cls_MediaContents));
        if (!cls_mediaContents)
        {
            NS_LOGE ("Failed to Get ObjectClass for MediaContents");
            return NULL;
        }
        jmethodID mid_mediaContents = env->GetMethodID(
                                          cls_mediaContents, "<init>", "(Ljava/lang/String;)V");
        if (!mid_mediaContents)
        {
            NS_LOGE ("Failed to Get MethodID for MediaContents<init>");
            return NULL;
        }
        jobject obj_mediaContents = env->NewObject(cls_mediaContents, mid_mediaContents,
                                    jIconImage);

        jfieldID fid_mediaContents = env->GetFieldID(cls_message, "mMediaContents",
                                     "Lorg/iotivity/service/ns/common/MediaContents;");
        if (!fid_mediaContents)
        {
            NS_LOGE ("Failed to get field mediaContents for Message");
            return NULL;
        }
        env->SetObjectField(obj_message, fid_mediaContents, obj_mediaContents);

    }

    jobject jType = getJavaMessageType(env, message.getType());
    if (jType)
    {
        jfieldID fid_type = env->GetFieldID(cls_message, "mType",
                                            "Lorg/iotivity/service/ns/common/Message$MessageType;");
        if (!fid_type)
        {
            NS_LOGE ("Failed to get field Type for Message");
            return NULL;
        }
        env->SetObjectField(obj_message, fid_type, jType);
    }

    NS_LOGD ("Reading OCRepresentation Object from Native");

    OC::OCRepresentation *ocRepresentation = new OC::OCRepresentation(message.getExtraInfo());
    jlong handle = reinterpret_cast<jlong>(ocRepresentation);
    jobject jRepresentation = env->NewObject(g_cls_OcRepresentation, g_mid_OcRepresentation_N_ctor_bool,
                              handle, true);
    if (!jRepresentation)
    {
        NS_LOGE ("Failed to create OcRepresentation");
        delete ocRepresentation;
    }
    else
    {
        NS_LOGD ("Created OCRepresentation Object from Native");
    }
    jfieldID fid_extraInfo = env->GetFieldID(cls_message, "mExtraInfo",
                             "Lorg/iotivity/base/OcRepresentation;");
    if (!fid_extraInfo)
    {
        NS_LOGE ("Failed to get mExtraInfo for Message");
        delete ocRepresentation;
        return NULL;
    }
    NS_LOGD ("setting extraInfo field");
    env->SetObjectField(obj_message, fid_extraInfo, jRepresentation);

    env->DeleteLocalRef(cls_message);
    NS_LOGD ("ConsumerService_getJavaMessage - OUT");
    return obj_message;
}

void onDiscoverProvider(std::shared_ptr<OIC::Service::NSProvider> provider)
{
    NS_LOGD ("ConsumerService_onDiscoverProvider - IN");

    jint envRet = 0;;
    JNIEnv *env = GetJNIEnv(&envRet);
    if (NULL == env)
    {
        return ;
    }

    jobject jDiscoverListener = (jobject) env->NewLocalRef(g_obj_discoverListener);
    if (!jDiscoverListener)
    {
        NS_LOGE ("Failed to Get jDiscoverListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    jobject obj_provider = getJavaProvider(env, provider);
    if (!obj_provider)
    {
        NS_LOGE ("Failed to Get Provider Object");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    jclass cls = env->GetObjectClass(jDiscoverListener);
    if (!cls)
    {
        NS_LOGE ("Failed to Get ObjectClass for jDiscoverListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jmethodID mid = env->GetMethodID(
                        cls,
                        "onProviderDiscovered",
                        "(Lorg/iotivity/service/ns/consumer/Provider;)V");
    if (!mid)
    {
        NS_LOGE ("Failed to Get MethodID for onProviderDiscovered");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    env->CallVoidMethod(jDiscoverListener, mid, obj_provider);

    env->DeleteLocalRef(jDiscoverListener);
    if (JNI_EDETACHED == envRet)
    {
        g_jvm_consumer->DetachCurrentThread();
    }
    NS_LOGD ("ConsumerService_onDiscoverProvider - OUT");
    return ;
}

void onProviderState( OIC::Service::NSProviderState state)
{
    NS_LOGD ("ConsumerService_onProviderState -IN");

    jint envRet = 0;;
    JNIEnv *env = GetJNIEnv(&envRet);
    if (NULL == env)
    {
        return ;
    }

    jobject jAcceptListener = (jobject) env->NewLocalRef(g_obj_acceptListener);
    if (!jAcceptListener)
    {
        NS_LOGE ("Failed to Get jAcceptListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jobject obj_state = getJavaProviderState(env, state);
    if (!obj_state)
    {
        NS_LOGE ("Failed to Get ProviderState Object");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    jclass cls = env->GetObjectClass(jAcceptListener);
    if (!cls)
    {
        NS_LOGE ("Failed to Get ObjectClass for jAcceptListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jmethodID mid = env->GetMethodID(
                        cls,
                        "onProviderStateReceived",
                        "(Lorg/iotivity/service/ns/consumer/Provider$ProviderState;)V");
    if (!mid)
    {
        NS_LOGE ("Failed to Get MethodID for onProviderState");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    env->CallVoidMethod(jAcceptListener, mid, obj_state);

    env->DeleteLocalRef(jAcceptListener);
    if (JNI_EDETACHED == envRet)
    {
        g_jvm_consumer->DetachCurrentThread();
    }
    NS_LOGD ("ConsumerService_onProviderState -OUT");
    return ;

}

void onMessagePosted(OIC::Service::NSMessage message)
{
    NS_LOGD ("ConsumerService_onMessagePosted -IN");

    jint envRet = 0;;
    JNIEnv *env = GetJNIEnv(&envRet);
    if (NULL == env)
    {
        return ;
    }

    jobject jPostListener = (jobject) env->NewLocalRef(g_obj_postListener);
    if (!jPostListener)
    {
        NS_LOGE ("Failed to Get jPostListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    jobject obj_message = getJavaMessage( env, message);
    if (!obj_message)
    {
        NS_LOGE ("Failed to Get Message Object");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jclass cls = env->GetObjectClass(jPostListener);
    if (!cls)
    {
        NS_LOGE ("Failed to Get ObjectClass for jPostListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jmethodID mid = env->GetMethodID(
                        cls,
                        "onMessageReceived",
                        "(Lorg/iotivity/service/ns/common/Message;)V");
    if (!mid)
    {
        NS_LOGE ("Failed to Get MethodID for onMessageReceived");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    env->CallVoidMethod(jPostListener, mid, obj_message);

    env->DeleteLocalRef(jPostListener);
    if (JNI_EDETACHED == envRet)
    {
        g_jvm_consumer->DetachCurrentThread();
    }
    NS_LOGD ("ConsumerService_onMessagePosted -OUT");
    return ;
}

void onSyncInfoReceived(OIC::Service::NSSyncInfo sync)
{
    NS_LOGD ("ConsumerService_onSyncInfoReceived - IN");

    jint envRet = 0;
    JNIEnv *env = GetJNIEnv(&envRet);
    if (NULL == env)
    {
        return ;
    }

    jobject jSyncListener = (jobject) env->NewLocalRef(g_obj_syncListener);
    if (!jSyncListener)
    {
        NS_LOGE ("Failed to Get jSyncListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    NS_LOGD ("Sync ID : %llu\n", sync.getMessageId());
    NS_LOGD ("Sync ProviderId : %s\n", sync.getProviderId().c_str());
    NS_LOGD ("Sync STATE : %d\n", (int) sync.getState());

    jlong jMessageId = (jlong) sync.getMessageId();
    jstring jProviderId = env->NewStringUTF(sync.getProviderId().c_str());
    jobject syncType = getJavaSyncType(env, sync.getState());
    if (!syncType)
    {
        NS_LOGE ("Failed to Get syncType for SyncInfo");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jclass cls_SyncInfo = (jclass) (env->NewLocalRef(g_cls_SyncInfo));
    if (!cls_SyncInfo)
    {
        NS_LOGE ("Failed to Get ObjectClass for SyncInfo");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jmethodID mid_syncInfo = env->GetMethodID(
                                 cls_SyncInfo,
                                 "<init>",
                                 "(JLjava/lang/String;Lorg/iotivity/service/ns/common/SyncInfo$SyncType;)V");
    if (!mid_syncInfo)
    {
        NS_LOGE ("Failed to Get MethodID for SyncInfo");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    jobject obj_syncInfo = env->NewObject( cls_SyncInfo, mid_syncInfo,
                                           jMessageId, jProviderId, syncType);
    if (!obj_syncInfo)
    {
        NS_LOGE ("Failed to Get Object for SyncInfo");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    jclass cls = env->GetObjectClass(jSyncListener);
    if (!cls)
    {
        NS_LOGE ("Failed to Get ObjectClass for jSyncListener");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }
    jmethodID mid = env->GetMethodID(
                        cls,
                        "onSyncInfoReceived",
                        "(Lorg/iotivity/service/ns/common/SyncInfo;)V");
    if (!mid)
    {
        NS_LOGE ("Failed to Get MethodID for onSyncInfoReceived");
        if (JNI_EDETACHED == envRet)
        {
            g_jvm_consumer->DetachCurrentThread();
        }
        return ;
    }

    env->CallVoidMethod(jSyncListener, mid, obj_syncInfo);

    env->DeleteLocalRef(jSyncListener);
    env->DeleteLocalRef(cls_SyncInfo);
    if (JNI_EDETACHED == envRet)
    {
        g_jvm_consumer->DetachCurrentThread();
    }
    NS_LOGD ("ConsumerService_onSyncInfoReceived - OUT");
    return ;
}

JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_ConsumerService_nativeStart
(JNIEnv *env, jobject jObj, jobject jDiscoverListener)
{
    NS_LOGD ("ConsumerService_StartConsumer - IN");
    if (!jDiscoverListener)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Listener cannot be null");
        return;
    }
    if (g_obj_discoverListener != NULL)
    {
        env->DeleteGlobalRef(g_obj_discoverListener);
    }
    g_obj_discoverListener = (jobject) env->NewGlobalRef(jDiscoverListener);

    OIC::Service::NSResult result  = OIC::Service::NSConsumerService::getInstance()->start(
                                         onDiscoverProvider);

    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to start ConsumerService");
        return;
    }
    NS_LOGD ("ConsumerService_StartConsumer - OUT");
    return;

}

JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_ConsumerService_nativeStop
(JNIEnv *env, jobject jObj)
{
    NS_LOGD ("ConsumerService_StopConsumer - IN");

    env->DeleteGlobalRef(g_obj_postListener);
    env->DeleteGlobalRef(g_obj_syncListener);
    env->DeleteGlobalRef(g_obj_discoverListener);
    env->DeleteGlobalRef(g_obj_acceptListener);
    g_obj_postListener = NULL;
    g_obj_syncListener = NULL;
    g_obj_discoverListener = NULL;
    g_obj_acceptListener = NULL;
    OIC::Service::NSResult result  = OIC::Service::NSConsumerService::getInstance()->stop();

    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to Stop ConsumerService");
        return;
    }
    NS_LOGD ("ConsumerService_StopConsumer - OUT");
    return;
}

JNIEXPORT void JNICALL
Java_org_iotivity_service_ns_consumer_ConsumerService_nativeEnableRemoteService
(JNIEnv *env, jobject jObj, jstring jServerAddress)
{
    NS_LOGD ("ConsumerService_EnableRemoteService - IN");
    if (!jServerAddress)
    {
        ThrowNSException(JNI_INVALID_VALUE, "EnableRemoteService server address NULL");
        return;
    }
    const char *serverAddress = env->GetStringUTFChars(jServerAddress, 0);
    OIC::Service::NSResult result =
        OIC::Service::NSConsumerService::getInstance()->enableRemoteService(std::string(serverAddress));

    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to EnableRemoteService");
        return;
    }
    env->ReleaseStringUTFChars(jServerAddress, serverAddress);
    NS_LOGD ("ConsumerService_EnableRemoteService - OUT");
    return;
}

JNIEXPORT void JNICALL
Java_org_iotivity_service_ns_consumer_ConsumerService_nativeSubscribeMQService
(JNIEnv *env, jobject jObj, jstring jserverAddress, jstring jTopicName)
{
    NS_LOGD ("ConsumerService: nativeSubscribeMQService - IN");
    if (!jserverAddress)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Server Address Can't be NULL");
        return;
    }
    if (!jTopicName)
    {
        ThrowNSException(JNI_INVALID_VALUE, "TopicName Can't be NULL");
        return;
    }

    const char *address = env->GetStringUTFChars( jserverAddress, NULL);
    std::string servAddress(address);
    const char *topic = env->GetStringUTFChars( jTopicName, NULL);
    std::string topicName(topic);

    OIC::Service::NSResult result  =
        OIC::Service::NSConsumerService::getInstance()->subscribeMQService(
            servAddress, topicName);
    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to Subscribe to MQ Service");
        return;
    }
    env->ReleaseStringUTFChars(jserverAddress, address);
    env->ReleaseStringUTFChars(jTopicName, topic);
    NS_LOGD ("ConsumerService: nativeSubscribeMQService - OUT");
    return;
}

JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_ConsumerService_nativeRescanProvider
(JNIEnv *env, jobject jObj)
{
    NS_LOGD ("ConsumerService_RescanProvider - IN");
    OIC::Service::NSResult result  = OIC::Service::NSConsumerService::getInstance()->rescanProvider();
    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to RescanProvider");
        return;
    }
    NS_LOGD ("ConsumerService_RescanProvider - OUT");
    return;

}

JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeSubscribe
(JNIEnv *env, jobject jObj)
{
    NS_LOGD ("Provider_Subscribe -IN");
    OIC::Service::NSResult result  = OIC::Service::NSResult::ERROR;
    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return ;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return ;
    }
    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    if (jProvider)
    {
        NS_LOGD ("calling subscribe on mNativeHandle");
        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        try
        {
            result  = objectHolder->get()->subscribe();
        }
        catch (OIC::Service::NSException ex)
        {
            ThrowNSException(NATIVE_EXCEPTION, ex.what());
            return;
        }
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to Subscribe Provider");
        return;
    }
    NS_LOGD ("Provider_Subscribe -OUT");
    return;
}

JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeUnsubscribe
(JNIEnv *env, jobject jObj)
{
    NS_LOGD ("Provider_UnSubscribe -IN");
    OIC::Service::NSResult result  = OIC::Service::NSResult::ERROR;
    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return ;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return ;
    }
    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    if (jProvider)
    {
        NS_LOGD ("calling subscribe on mNativeHandle");
        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        try
        {
            result  = objectHolder->get()->unsubscribe();
        }
        catch (OIC::Service::NSException ex)
        {
            ThrowNSException(NATIVE_EXCEPTION, ex.what());
            return;
        }
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to UnSubscribe Provider");
        return;
    }
    NS_LOGD ("Provider_UnSubscribe -OUT");
    return;
}

JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeSendSyncInfo
(JNIEnv *env, jobject jObj, jlong jMessageId, jint jSyncType)
{
    NS_LOGD ("Provider_SendSyncInfo - IN");
    OIC::Service::NSResult result  = OIC::Service::NSResult::ERROR;
    if (!jMessageId)
    {
        ThrowNSException(JNI_INVALID_VALUE, "MessageId cannot be 0");
        return ;
    }

    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return ;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return ;
    }
    uint64_t messageId = (uint64_t) jMessageId;

    NS_LOGD ("!!!!!!jMessageId: %lld", jMessageId);
    NS_LOGD ("!!!!!!messageId: %lld", messageId);

    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    if (jProvider)
    {
        NS_LOGD ("calling SendSyncInfo on mNativeHandle");
        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        try
        {
            result  = objectHolder->get()->sendSyncInfo(messageId,
                      (OIC::Service::NSSyncInfo::NSSyncType)jSyncType);
        }
        catch (OIC::Service::NSException ex)
        {
            ThrowNSException(NATIVE_EXCEPTION, ex.what());
            return;
        }
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to Send sync info");
        return;
    }
    NS_LOGD ("Provider_SendSyncInfo - OUT");
    return;
}


JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeSetListener
(JNIEnv *env, jobject jObj, jobject jAcceptListener, jobject jPostListener, jobject jSyncListener)
{
    NS_LOGD ("Provider_SetListener - IN");
    if (!jPostListener || !jSyncListener || !jAcceptListener)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Listener cannot be null");
        return ;
    }

    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return ;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return ;
    }
    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    if (jProvider)
    {
        NS_LOGD ("calling SetListener on mNativeHandle");
        if (g_obj_acceptListener != NULL)
        {
            env->DeleteGlobalRef(g_obj_acceptListener);
        }
        if (g_obj_postListener != NULL)
        {
            env->DeleteGlobalRef(g_obj_postListener);
        }
        if (g_obj_syncListener != NULL)
        {
            env->DeleteGlobalRef(g_obj_syncListener);
        }
        g_obj_acceptListener = (jobject) env->NewGlobalRef(jAcceptListener);
        g_obj_postListener = (jobject) env->NewGlobalRef(jPostListener);
        g_obj_syncListener = (jobject) env->NewGlobalRef(jSyncListener);

        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        objectHolder->get()->setListener(onProviderState, onMessagePosted, onSyncInfoReceived);
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
    NS_LOGD ("Provider_SetListener - OUT");
    return;
}

JNIEXPORT jobject JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeGetTopicList
(JNIEnv *env, jobject jObj)
{
    NS_LOGD ("Provider_nativeGetTopicList - IN");
    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return NULL;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return NULL;
    }
    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    std::shared_ptr<OIC::Service::NSTopicsList> topicList = nullptr;
    if (jProvider)
    {
        NS_LOGD ("calling subscribe on mNativeHandle");
        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        try
        {
            topicList = objectHolder->get()->getTopicList();
        }
        catch (OIC::Service::NSException ex)
        {
            ThrowNSException(NATIVE_EXCEPTION, ex.what());
            return NULL;
        }
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
    if (topicList == nullptr)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Topic List doesn't exist");
        return NULL;
    }

    jobject obj_topicList = getJavaTopicsList(env, topicList);

    NS_LOGD ("Provider_nativeGetTopicList - OUT");
    return obj_topicList;
}

JNIEXPORT void JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeUpdateTopicList
(JNIEnv *env, jobject jObj, jobject jTopicsList)
{
    NS_LOGD ("Provider_nativeUpdateTopicList -IN");
    if (!jTopicsList)
    {
        ThrowNSException(JNI_INVALID_VALUE, "TopicList cannot be null");
        return;
    }
    std::shared_ptr<OIC::Service::NSTopicsList> nsTopicsList = getNativeTopicsList(env, jTopicsList);
    if (nsTopicsList == nullptr)
    {
        ThrowNSException(JNI_INVALID_VALUE, "NSTopicList cannot be created ");
        return;
    }

    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return;
    }
    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    OIC::Service::NSResult result = OIC::Service::NSResult::ERROR;
    if (jProvider)
    {
        NS_LOGD ("calling subscribe on mNativeHandle");
        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        try
        {
            result = objectHolder->get()->updateTopicList(nsTopicsList);
        }
        catch (OIC::Service::NSException ex)
        {
            ThrowNSException(NATIVE_EXCEPTION, ex.what());
            return;
        }
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
    if (result !=  OIC::Service::NSResult::OK)
    {
        ThrowNSException((int) result, "Fail to Update Interest Topics");
        return;
    }
    NS_LOGD ("Provider_nativeUpdateTopicList -OUT");
    return;
}

JNIEXPORT jobject JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeGetProviderState
(JNIEnv *env, jobject jObj)
{
    NS_LOGD ("Provider_nativeGetProviderState - IN");
    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return NULL;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return NULL;
    }
    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    OIC::Service::NSProviderState state = OIC::Service::NSProviderState::DENY;
    if (jProvider)
    {
        NS_LOGD ("calling getProviderState on mNativeHandle");
        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        try
        {
            state = objectHolder->get()->getProviderState();
        }
        catch (OIC::Service::NSException ex)
        {
            ThrowNSException(NATIVE_EXCEPTION, ex.what());
            return NULL;
        }
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
    jobject obj_state = getJavaProviderState(env, state);

    NS_LOGD ("Provider_nativeGetProviderState - OUT");
    return obj_state;
}

JNIEXPORT jboolean JNICALL Java_org_iotivity_service_ns_consumer_Provider_nativeIsSubscribed
(JNIEnv *env, jobject jObj)
{
    NS_LOGD ("nativeIsSubscribed - IN");
    jclass providerClass = env->GetObjectClass(jObj);
    if (!providerClass)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to Get ObjectClass for Provider");
        return (jboolean)false;
    }

    jfieldID nativeHandle = env->GetFieldID(providerClass, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        ThrowNSException(JNI_INVALID_VALUE, "Failed to get nativeHandle for Provider");
        return (jboolean)false;
    }
    jlong jProvider = env->GetLongField(jObj, nativeHandle);
    if (jProvider)
    {
        NS_LOGD ("calling isSubscribe on mNativeHandle");
        JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
            reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(jProvider);
        try
        {
            return (jboolean) objectHolder->get()->isSubscribed();
        }
        catch (OIC::Service::NSException ex)
        {
            ThrowNSException(NATIVE_EXCEPTION, ex.what());
            return (jboolean)false;
        }
    }
    else
    {
        NS_LOGE ("Couldn't find Provider");
        ThrowNSException(JNI_NO_NATIVE_POINTER, "Fail to find native Provider");
    }
}

JNIEXPORT void JNICALL
Java_org_iotivity_service_ns_consumer_Provider_nativeDispose
(JNIEnv *env, jobject object)
{
    jclass cls_provider = (jclass) (env->NewLocalRef(g_cls_Provider));
    if (!cls_provider)
    {
        NS_LOGE ("Failed to Get ObjectClass for Provider");
        return;
    }
    jfieldID nativeHandle = env->GetFieldID(cls_provider, "mNativeHandle", "J");
    if (!nativeHandle)
    {
        NS_LOGE ("Failed to get nativeHandle for Provider");
        return;
    }
    jlong handle = env->GetLongField(object, nativeHandle);
    JniSharedObjectHolder<OIC::Service::NSProvider> *objectHolder =
        reinterpret_cast<JniSharedObjectHolder<OIC::Service::NSProvider> *>(handle);
    delete objectHolder;
    return;
}

// JNI OnLoad
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    NS_LOGD ("ConsumerService_JNI_OnLoad");
    g_jvm_consumer = jvm;

    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **)&env, JNI_CURRENT_VERSION) != JNI_OK)
    {
        NS_LOGE ("Failed to get the environment using GetEnv()");
        return JNI_ERR;
    }

    jclass localMessage = env->FindClass(
                              "org/iotivity/service/ns/common/Message");
    if (!localMessage)
    {
        NS_LOGE ("Failed to get local Message class");
        return JNI_ERR;
    }
    g_cls_Message = (jclass) (env->NewGlobalRef(localMessage));
    if (!g_cls_Message)
    {
        NS_LOGE ("Failed to set Global Message reference");
        return JNI_ERR;
    }

    jclass localProvider = env->FindClass(
                               "org/iotivity/service/ns/consumer/Provider");
    if (!localProvider)
    {
        NS_LOGE ("Failed to get local Provider class");
        return JNI_ERR;
    }
    g_cls_Provider = (jclass) (env->NewGlobalRef(localProvider));
    if (!g_cls_Provider)
    {
        NS_LOGE ("Failed to set Global Provider reference");
        return JNI_ERR;
    }

    jclass localSyncInfo = env->FindClass(
                               "org/iotivity/service/ns/common/SyncInfo");
    if (!localSyncInfo)
    {
        NS_LOGE ("Failed to get local SyncInfo class");
        return JNI_ERR;
    }
    g_cls_SyncInfo = (jclass) (env->NewGlobalRef(localSyncInfo));
    if (!g_cls_SyncInfo)
    {
        NS_LOGE ("Failed to set Global NSSyncInfo reference");
        return JNI_ERR;
    }

    jclass localSyncType = env->FindClass(
                               "org/iotivity/service/ns/common/SyncInfo$SyncType");
    if (!localSyncType)
    {
        NS_LOGE ("Failed to get local SyncType enum");
        return JNI_ERR;
    }
    g_cls_SyncType = (jclass) (env->NewGlobalRef(localSyncType));
    if (!g_cls_SyncType)
    {
        NS_LOGE ("Failed to set Global SyncType reference");
        return JNI_ERR;
    }

    jclass localMediaContents = env->FindClass(
                                    "org/iotivity/service/ns/common/MediaContents");
    if (!localMediaContents)
    {
        NS_LOGE ("Failed to get local MediaContents class");
        return JNI_ERR;
    }
    g_cls_MediaContents = (jclass) (env->NewGlobalRef(localMediaContents));
    if (!g_cls_MediaContents)
    {
        NS_LOGE ("Failed to set Global MediaContents reference");
        return JNI_ERR;
    }
    jclass localTopicState = env->FindClass(
                                 "org/iotivity/service/ns/common/Topic$TopicState");
    if (!localTopicState)
    {
        NS_LOGE ("Failed to get local TopicState enum");
        return JNI_ERR;
    }
    g_cls_TopicState = (jclass) (env->NewGlobalRef(localTopicState));
    if (!g_cls_TopicState)
    {
        NS_LOGE ("Failed to set Global TopicState reference");
        return JNI_ERR;
    }

    jclass localTopic = env->FindClass(
                            "org/iotivity/service/ns/common/Topic");
    if (!localTopic)
    {
        NS_LOGE ("Failed to get local TopicState enum");
        return JNI_ERR;
    }
    g_cls_Topic = (jclass) (env->NewGlobalRef(localTopic));
    if (!g_cls_Topic)
    {
        NS_LOGE ("Failed to set Global Topic reference");
        return JNI_ERR;
    }

    jclass localMessageType = env->FindClass(
                                  "org/iotivity/service/ns/common/Message$MessageType");
    if (!localMessageType)
    {
        NS_LOGE ("Failed to get local Message Type class");
        return JNI_ERR;
    }
    g_cls_Message_Type = (jclass) (env->NewGlobalRef(localMessageType));
    if (!g_cls_Message_Type)
    {
        NS_LOGE ("Failed to set Global Message Type reference");
        return JNI_ERR;
    }

    jclass localProviderState = env->FindClass(
                                    "org/iotivity/service/ns/consumer/Provider$ProviderState");
    if (!localProviderState)
    {
        NS_LOGE ("Failed to get localProviderState  Type class");
        return JNI_ERR;
    }
    g_cls_ProviderState = (jclass) (env->NewGlobalRef(localProviderState));
    if (!g_cls_ProviderState)
    {
        NS_LOGE ("Failed to set Global ProviderState Type reference");
        return JNI_ERR;
    }

    jclass localTopicsList = env->FindClass(
                                 "org/iotivity/service/ns/common/TopicsList");
    if (!localTopicsList)
    {
        NS_LOGE ("Failed to get local Topic class");
        return JNI_ERR;
    }
    g_cls_TopicsList = (jclass) (env->NewGlobalRef(localTopicsList));
    if (!g_cls_TopicsList)
    {
        NS_LOGE ("Failed to set Global TopicsList reference");
        return JNI_ERR;
    }
    //OcRepresentation
    jclass localOcRepresentation = env->FindClass("org/iotivity/base/OcRepresentation");
    if (!localOcRepresentation)
    {
        NS_LOGE ("Failed to get local OcRepresentation class");
        return JNI_ERR;
    }
    g_cls_OcRepresentation = (jclass) env->NewGlobalRef(localOcRepresentation);
    if (!g_cls_OcRepresentation)
    {
        NS_LOGE ("Failed to set Global OcRepresentation reference");
        return JNI_ERR;
    }

    g_mid_OcRepresentation_N_ctor_bool = env->GetMethodID(g_cls_OcRepresentation, "<init>", "(JZ)V");
    if (!g_mid_OcRepresentation_N_ctor_bool)
    {
        NS_LOGE ("Failed to get Global OcRepresentation Constructor reference");
        return JNI_ERR;
    }

    env->DeleteLocalRef(localMessage);
    env->DeleteLocalRef(localProvider);
    env->DeleteLocalRef(localSyncInfo);
    env->DeleteLocalRef(localSyncType);
    env->DeleteLocalRef(localMediaContents);
    env->DeleteLocalRef(localTopicState);
    env->DeleteLocalRef(localMessageType);
    env->DeleteLocalRef(localProviderState);
    env->DeleteLocalRef(localTopic);
    env->DeleteLocalRef(localTopicsList);
    env->DeleteLocalRef(localOcRepresentation);

    return NSExceptionInit(env);
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *jvm, void *reserved)
{
    NS_LOGI ("ConsumerService_JNI_OnUnload");
    JNIEnv *env = NULL;

    if (jvm->GetEnv((void **)&env, JNI_CURRENT_VERSION) != JNI_OK)
    {
        NS_LOGE ("Failed to get the environment using GetEnv()");
        return ;
    }

    env->DeleteGlobalRef(g_cls_Message);
    env->DeleteGlobalRef(g_cls_Provider);
    env->DeleteGlobalRef(g_cls_SyncInfo);
    env->DeleteGlobalRef(g_cls_SyncType);
    env->DeleteGlobalRef(g_cls_MediaContents);
    env->DeleteGlobalRef(g_cls_TopicState);
    env->DeleteGlobalRef(g_cls_Message_Type);
    env->DeleteGlobalRef(g_cls_ProviderState);
    env->DeleteGlobalRef(g_cls_Topic);
    env->DeleteGlobalRef(g_cls_TopicsList);
    env->DeleteGlobalRef(g_cls_OcRepresentation);
}
