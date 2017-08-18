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

#include "JniNotificationCommon.h"

#ifndef JNI_NOTIFICATION_PROVIDER_H
#define JNI_NOTIFICATION_PROVIDER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeStart
 * Signature: (Lorg/iotivity/service/ns/provider/ProviderService/OnConsumerSubscribedListener;Lorg/iotivity/service/ns/provider/ProviderService/OnMessageSynchronizedListener;ZLjava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_ProviderService_nativeStart
(JNIEnv *, jobject, jobject, jobject, jboolean, jstring, jboolean);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeStop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_ProviderService_nativeStop
(JNIEnv *, jobject);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeSendMessage
 * Signature: (Lorg/iotivity/service/ns/common/Message;)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_ProviderService_nativeSendMessage
(JNIEnv *, jobject, jobject);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeSendSyncInfo
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_ProviderService_nativeSendSyncInfo
(JNIEnv *, jobject, jlong, jint);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeCreateMessage
 * Signature: ()Lorg/iotivity/service/ns/common/Message;
 */
JNIEXPORT jobject JNICALL Java_org_iotivity_service_ns_provider_ProviderService_nativeCreateMessage
(JNIEnv *, jobject);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeEnableRemoteService
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_ns_provider_ProviderService_nativeEnableRemoteService
(JNIEnv *, jobject, jstring);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeDisableRemoteService
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_ns_provider_ProviderService_nativeDisableRemoteService
(JNIEnv *, jobject, jstring);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeSubscribeMQService
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_iotivity_service_ns_provider_ProviderService_nativeSubscribeMQService
(JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeRegisterTopic
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_ProviderService_nativeRegisterTopic
(JNIEnv *, jobject, jstring);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeUnregisterTopic
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_ProviderService_nativeUnregisterTopic
(JNIEnv *, jobject, jstring);

/*
 * Class:     org_iotivity_service_ns_provider_ProviderService
 * Method:    nativeGetRegisteredTopicList
 * Signature: ()Lorg/iotivity/service/ns/common/TopicsList;
 */
JNIEXPORT jobject JNICALL
Java_org_iotivity_service_ns_provider_ProviderService_nativeGetRegisteredTopicList
(JNIEnv *, jobject);

/*
 * Class:     org_iotivity_service_ns_provider_Consumer
 * Method:    nativeAcceptSubscription
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_Consumer_nativeAcceptSubscription
(JNIEnv *, jobject, jstring, jboolean);

/*
 * Class:     org_iotivity_service_ns_provider_Consumer
 * Method:    nativeSetConsumerTopic
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_Consumer_nativeSetConsumerTopic
(JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_iotivity_service_ns_provider_Consumer
 * Method:    nativeUnsetConsumerTopic
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_ns_provider_Consumer_nativeUnsetConsumerTopic
(JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_iotivity_service_ns_provider_Consumer
 * Method:    nativeGetConsumerTopicList
 * Signature: (Ljava/lang/String;)Lorg/iotivity/service/ns/common/TopicsList;
 */
JNIEXPORT jobject JNICALL
Java_org_iotivity_service_ns_provider_Consumer_nativeGetConsumerTopicList
(JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif


