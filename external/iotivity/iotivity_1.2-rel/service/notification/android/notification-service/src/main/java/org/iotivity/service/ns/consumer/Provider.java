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

package org.iotivity.service.ns.consumer;

import android.util.Log;
import org.iotivity.service.ns.common.*;
import java.util.Vector;

/**
 *
 * This class provides implementation of Notification Provider object.
 *
 */
public class Provider {

    private static final String LOG_TAG = "ConsumerService_Provider";

    /**
     * Enum for defining different state of provider object
     */
    public enum ProviderState {
        ALLOW(1),
        DENY(2),
        TOPIC(3),
        STOPPED(12);

        private int state;

        private ProviderState(int state) {
            this.state = state;
        }

        public int getProviderState() {
            return this.state;
        }
    };

    private String mProviderId   = null;
    private long  mNativeHandle = 0;

    /**
     * Constructor of Provider.
     *
     * @param providerId
     *            unique Id of Provider
     */
    public Provider(String providerId) {
        Log.i(LOG_TAG, "Provider()");

        mProviderId = providerId;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            nativeDispose();
        } catch (Throwable t) {
            throw t;
        } finally {
            super.finalize();
        }
    }
    /**
     * API for getting providerId
     *
     * @return ConsumerId as string
     */
    public String getProviderId() {
        return mProviderId;
    }

    /**
     * API for getting for getting Topic List
     *
     * @return TopicsList
     */
    public TopicsList getTopicList() throws NSException {
        return nativeGetTopicList();
    }

    /**
     * API for getting for getting ProviderState
     *
     * @return ProviderState
     */
    public ProviderState getProviderState() throws NSException {
        return nativeGetProviderState();
    }

    /**
     * API for requesting subscription of Notification service
     * This API should be called with a valid Provider object obtained from Discovery callback.
     * The API should not be called when the Provider is in STOPPED state.
     *
     * Discovery APIs to discover Providers are as below.
     * Start/rescanProvider for D2D,
     * enableRemoteService for D2S,
     *
     * @throws NSException failure to subscribe
     */
    public void subscribe() throws NSException {
        nativeSubscribe();
    }

    /**
     * API for requesting unsubscription of Notification service
     *
     * This API should be called with a valid Provider object obtained from Discovery callback.
     * The API should not be called when the Provider is in STOPPED state.
     *
     * @throws NSException failure to subscribe
     */
    public void unsubscribe() throws NSException {
        nativeUnsubscribe();
    }

    /**
     * API for requesting subscription status from Provider of Notification
     * service
     */
    public boolean isSubscribed() throws NSException {
        return nativeIsSubscribed();
    }

    /**
     * This method is for Sending SyncInfo of Notification service.
     *
     * @param messageId
     *            unique Id of message
     * @param syncType
     *            SyncType of Notification service
     */
    public void sendSyncInfo(long messageId, SyncInfo.SyncType syncType)
            throws NSException {
        nativeSendSyncInfo(messageId, syncType.ordinal());
    }

    /**
     * This method is for registering for listeners of Notification .
     *
     * @param onProviderStateListener
     *            OnProviderStateListener callback Interface
     * @param onMessageReceivedListner
     *            OnMessageReceivedListner callback Interface
     * @param onSyncInfoReceivedListner
     *            OnSyncInfoReceivedListner callback Interface
     */
    public void setListener(OnProviderStateListener onProviderStateListener,
            OnMessageReceivedListener onMessageReceivedListener,
            OnSyncInfoReceivedListener onSyncInfoReceivedListener)
            throws NSException {
        nativeSetListener(onProviderStateListener, onMessageReceivedListener,
                onSyncInfoReceivedListener);
    }

    /**
     * Update Topic list that is wanted to be subscribed from provider
     *
     * @param topicsList
     *            TopicsList of interested Topics
     *
     * @throws NSException failure to update topic list. 
     */
    public void updateTopicList(TopicsList topicsList) throws NSException {
        nativeUpdateTopicList(topicsList);
    }

    /**
     * Interface to implement callback function to receive provider state
     * information
     */
    public interface OnProviderStateListener {

        /**
         * Callback function to receive provider state information
         *
         * @param state
         *            ProviderState
         */
        public void onProviderStateReceived(ProviderState state);
    }

    /**
     * Interface to implement callback function to receive Notification Message
     */
    public interface OnMessageReceivedListener {

        /**
         * Callback function to receive Notification Message.
         *
         * @param message
         *            Notification Message
         */
        public void onMessageReceived(Message message);
    }

    /**
     * Interface to implement callback function to receive message read
     * synchronization
     */
    public interface OnSyncInfoReceivedListener {

        /**
         * Callback function to receive message read synchronization
         *
         * @param sync
         *            SyncInfo
         */
        public void onSyncInfoReceived(SyncInfo sync);
    }

    private native void nativeSubscribe() throws NSException;
    private native void nativeUnsubscribe() throws NSException;

    private native void nativeSendSyncInfo(long messageId, int syncType)
            throws NSException;

    private native void nativeSetListener(
            OnProviderStateListener onProviderStateListener,
            OnMessageReceivedListener onMessageReceivedListener,
            OnSyncInfoReceivedListener onSyncInfoReceivedListener)
            throws NSException;

    private native TopicsList nativeGetTopicList() throws NSException;

    private native void nativeUpdateTopicList(TopicsList topicsList)
            throws NSException;

    private native ProviderState nativeGetProviderState() throws NSException;

    private native boolean nativeIsSubscribed() throws NSException;

    private native void nativeDispose();

}
