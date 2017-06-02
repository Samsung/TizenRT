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
package org.iotivity.service.ns.provider;

import org.iotivity.service.ns.common.*;
import java.util.Vector;

/**
 *
 * This class provides a set of Java APIs for Notification ProviderService.
 *
 */
public class ProviderService {

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("oc_logger");
        System.loadLibrary("connectivity_abstraction");
        System.loadLibrary("ca-interface");
        System.loadLibrary("octbstack");
        System.loadLibrary("oc");
        System.loadLibrary("ocstack-jni");
        System.loadLibrary("notification_provider");
        System.loadLibrary("notification_provider_wrapper");
        System.loadLibrary("notification_provider_jni");
    }

    private static ProviderService instance;

    static {
        instance = new ProviderService();
    }

    /**
     * API for getting instance of ProviderService
     *
     * @return ProviderService singleton instance created
     */
    public static ProviderService getInstance() {
        return instance;
    }

    /**
     * Start ProviderService
     *
     * @param subscribedListener
     *            OnConsumerSubscribedListener Callback
     * @param messageSynchronized
     *            OnMessageSynchronizedListener Callback
     * @param subControllability
     *            Set the policy for notification servcie which checks whether
     *            provider is capable of denying the subscription of
     *            notification message from consumer and getting controllabliity
     *            to set consumer topic list. If true, provider is able to
     *            control subscription request and consumer topic list.
     *            Otherwise(policy is false), consumer can do the same.
     * @param userInfo
     *            User defined information such as device friendly name
     * @param resourceSecurity
     *            Set on/off for secure resource channel setting
     *
     * @throws NSException
     *             if any callback parameter passed is null
     */
    public void start(OnConsumerSubscribedListener subscribedListener,
            OnMessageSynchronizedListener messageSynchronized,
            boolean subControllability, String userInfo,
            boolean resourceSecurity) throws NSException {
        nativeStart(subscribedListener, messageSynchronized,
                subControllability, userInfo, resourceSecurity);
    }

    /**
     * Stop ProviderService
     *
     * @throws NSException failed to stop ProviderService
     */
    public void stop() throws NSException {
        nativeStop();
    }

    /**
     * Send notification message to all subscribers
     *
     * @param message
     *            Notification message including id, title, contentText
     *
     * @throws NSException failed to send notification message
     */
    public void sendMessage(Message message) throws NSException {
        nativeSendMessage(message);
    }

    /**
     * Send read-check to provider in order to synchronize notification status
     * with other consumers
     *
     * @param messageId
     *            unique Id of Notification message to synchronize the status
     * @param syncType
     *            SyncType of the SyncInfo message
     */
    public void sendSyncInfo(long messageId, SyncInfo.SyncType syncType)
            throws NSException {
        nativeSendSyncInfo(messageId, syncType.ordinal());
    }

    /**
     * Initialize Message class, Mandatory fields which are messge id and
     * provider(device) id are filled with
     *
     * @return Message
     */
    public Message createMessage() throws NSException {
        return nativeCreateMessage();
    }

    /**
     * Request to publish resource to cloud server
     *
     * @param servAdd
     *            servAdd combined with IP address and port number using
     *            delimiter
     *
     * @throws NSException failed to publish resource
     */
    public void enableRemoteService(String servAdd) throws NSException {
        nativeEnableRemoteService(servAdd);
    }

    /**
     * Request to cancel remote service using cloud server
     *
     * @param servAdd
     *            servAdd combined with IP address and port number using
     *            delimiter
     *
     * @throws NSException failed to publish resource
     */
    public void disableRemoteService(String servAdd) throws NSException {
        nativeDisableRemoteService(servAdd);
    }

    /**
     * Request to subscribe to MQ server
     *
     * @param servAdd
     *            servAdd combined with IP address and port number and MQ broker
     *            uri using delimiter
     * @param topicName
     *            the interest Topic name for subscription
     *
     * @throws NSException failed to subscribe to MQ server
     */
    public void subscribeMQService(String servAdd, String topicName)
            throws NSException {
        nativeSubscribeMQService(servAdd, topicName);
    }

    /**
     * Add topic to topic list
     *
     * @param topicName
     *            Topic name to add
     *
     * @throws NSException failed to add topic
     */
    public void registerTopic(String topicName) throws NSException {
        nativeRegisterTopic(topicName);
    }

    /**
     * Delete topic from topic list
     *
     * @param topicName
     *            Topic name to add
     *
     * @throws NSException failed to delete topic
     */
    public void unregisterTopic(String topicName) throws NSException {
        nativeUnregisterTopic(topicName);
    }

    /**
     * Request topics list already registered by provider user
     *
     * @throws NSException failed to get topics list
     */
    public TopicsList getRegisteredTopicList() throws NSException {
        return nativeGetRegisteredTopicList();
    }

    /**
     * Interface to implement callback function to receive subscription request
     * of consumer
     */
    public interface OnConsumerSubscribedListener {

        /**
         * Callback function to receive subscription request of consumer
         *
         * @param consumer
         *            Consumer who subscribes the notification message resource
         */
        public void onConsumerSubscribed(Consumer consumer);
    }

    /**
     * Interface to implement callback function to receive the status of the
     * message synchronization
     */
    public interface OnMessageSynchronizedListener {

        /**
         * Callback function to receive the status of the message
         * synchronization
         *
         * @param syncInfo
         *            Synchronization information of the notification message
         */
        public void onMessageSynchronized(SyncInfo syncInfo);
    }

    private native void nativeStart(
            OnConsumerSubscribedListener subscribedListener,
            OnMessageSynchronizedListener messageSynchronized,
            boolean subControllability, String userInfo,
            boolean resourceSecurity) throws NSException;

    private native void nativeStop() throws NSException;

    private native void nativeSendMessage(Message message) throws NSException;

    private native void nativeSendSyncInfo(long messageId, int type)
            throws NSException;

    private native Message nativeCreateMessage() throws NSException;

    private native void nativeEnableRemoteService(String servAdd)
            throws NSException;

    private native void nativeDisableRemoteService(String servAdd)
            throws NSException;

    private native void nativeSubscribeMQService(String servAdd, String topicName)
            throws NSException;

    private native void nativeRegisterTopic(String topicName) throws NSException;

    private native void nativeUnregisterTopic(String topicName)
            throws NSException;

    private native TopicsList nativeGetRegisteredTopicList() throws NSException;
}
