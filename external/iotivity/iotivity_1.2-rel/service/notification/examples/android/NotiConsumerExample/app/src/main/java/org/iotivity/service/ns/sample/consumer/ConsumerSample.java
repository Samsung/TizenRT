/******************************************************************
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 * <p>
 * <p>
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************/

package org.iotivity.service.ns.sample.consumer;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;
import org.iotivity.service.ns.common.NSException;
import org.iotivity.service.ns.common.SyncInfo;
import org.iotivity.service.ns.common.TopicsList;
import org.iotivity.service.ns.common.Topic;
import org.iotivity.service.ns.consumer.ConsumerService;
import org.iotivity.service.ns.consumer.Provider;

public class ConsumerSample
        implements ConsumerService.OnProviderDiscoveredListener,
        Provider.OnProviderStateListener, Provider.OnMessageReceivedListener,
        Provider.OnSyncInfoReceivedListener {
    private static final String TAG                 = "NS_CONSUMER_SAMPLE";

    private Context             mContext            = null;
    private ConsumerService     consumerService     = null;
    private boolean             mAcceptor           = true;
    private Provider            mProvider           = null;

    private Handler             mHandler            = null;

    private static final int    PROVIDER_DISCOVERED = 1;
    private static final int    STATE_CHANGED       = 2;
    private static final int    MESSAGE_RECEIVED    = 3;
    private static final int    SYNCINFO_RECEIVED   = 4;
    private static final int    TOPICS_RECEIVED     = 5;

    public ConsumerSample(Context context) {
        Log.i(TAG, "Create consumer sample Instance");

        this.mContext = context;
        consumerService = new ConsumerService();
    }

    public void setHandler(Handler handler) {
        this.mHandler = handler;
    }

    public boolean getAcceptor() {
        return mAcceptor;
    }

    private void configurePlatform() {

        PlatformConfig platformConfig = new PlatformConfig(mContext,
                ServiceType.IN_PROC, ModeType.CLIENT_SERVER, "0.0.0.0",
                0, // Uses randomly available port
                QualityOfService.LOW);

        Log.i(TAG, "Configuring platform.");
        OcPlatform.Configure(platformConfig);
        try {
            OcPlatform.stopPresence(); // Initialize OcPlatform
        } catch (Exception e) {
            Log.e(TAG, "Exception: stopping presence when configuration step: "
                    + e);
        }
        Log.i(TAG, "Configuration done Successfully");
    }

    public void startNotificationConsumer() {
        configurePlatform();
        try {
            consumerService.start(this);
        } catch (Exception e) {
            Log.e(TAG, "Exception: startNotificationConsumer : " + e);
        }
    }

    public void stopNotificationConsumer() {
        try {
            consumerService.stop();
            mProvider = null;
        } catch (Exception e) {
            Log.e(TAG, "Exception: stopNotificationConsumer : " + e);
        }
    }

    public void enableRemoteService(String serverAddress) {
        try {
            consumerService.enableRemoteService(serverAddress);
        } catch (NSException e) {
            Log.e(TAG, "NSException: enableRemoteService : " + e);
        }
    }

    public void subscribeMQService(String servAdd, String topicName) {
        Log.i(TAG, "SubscribeMQService  - IN");
        try {
            consumerService.subscribeMQService(servAdd, topicName);
        } catch (Exception e) {
            Log.e(TAG, "Exception: subscribeMQService : " + e);
        }
        Log.i(TAG, "SubscribeMQService  - OUT");
        return;
    }

    public void rescanProvider() {
        try {
            consumerService.rescanProvider();
        } catch (Exception e) {
            Log.e(TAG, "Exception: rescanProvider : " + e);
        }
    }

    public void subscribe() {
        try {
            mProvider.subscribe();
            Log.i(TAG, "Notification consumer subscribe: " );
        } catch (Exception e) {
            Log.e(TAG, "Exception: subscribe : " + e);
        }
    }

    public void unsubscribe() {
        try {
            mProvider.unsubscribe();
            Log.i(TAG, "Notification consumer unsubscribe: ");
        } catch (Exception e) {
            Log.e(TAG, "Exception: unsubscribe : " + e);
        }
    }
    public void getTopicsList() {
        if (mProvider != null) {
            try {
                TopicsList topicsList = mProvider.getTopicList();
                StringBuilder stringBuilder = new StringBuilder();
                stringBuilder.append("TopicList Received :");
                for (Topic topic : topicsList.getTopicsList()) {
                    Log.i(TAG, "Topic Name : " + topic.getTopicName());
                    Log.i(TAG, "Topic State : " + topic.getState());
                    stringBuilder
                            .append("\nTopic Name : " + topic.getTopicName());
                    stringBuilder.append("\nTopic State : " + topic.getState());
                }
                Message msg = mHandler.obtainMessage(TOPICS_RECEIVED,
                        stringBuilder.toString());
                mHandler.sendMessage(msg);
            } catch (Exception e) {
                Log.e(TAG, "Exception: getTopicsList : " + e);
            }
        } else {
            Log.e(TAG, "getTopicsList Provider NULL");
        }
    }

    public void updateTopicList(TopicsList topicsList) {
        if (mProvider != null) {
            try {
                mProvider.updateTopicList(topicsList);
            } catch (Exception e) {
                Log.e(TAG, "Exception: updateTopicList : " + e);
            }
        } else {
            Log.e(TAG, "updateTopicList Provider NULL");
        }
    }

    @Override
    public void onProviderDiscovered(Provider provider) {
        Log.i(TAG, "onProviderDiscovered");
        if (provider == null) {
            Log.e(TAG, "providerID is Null  ");
            return;
        }
        mProvider = provider;
        Log.i(TAG, "Provider ID: " + provider.getProviderId());
        Message msg = mHandler.obtainMessage(PROVIDER_DISCOVERED,
                "Provider Discovered Id: " + provider.getProviderId());
        mHandler.sendMessage(msg);
        try {
            Log.i(TAG, "setListeners to Discovered Provider");
            provider.setListener(this, this, this);
            Log.i(TAG, "setListeners done");
            if (!provider.isSubscribed()) {
                Log.i(TAG, "Provider not subscribed. Acceptor is Consumer");
                mAcceptor = false;
                provider.subscribe();
            } else {
                Log.i(TAG,
                        "Provider is already subscribed. Acceptor is Provider");
                mAcceptor = true;
            }
        } catch (Exception e) {
            Log.e(TAG, "Exception : " + e);
        }
    }

    @Override
    public void onProviderStateReceived(Provider.ProviderState state) {
        Log.i(TAG, "onProviderStateReceived");

        Log.i(TAG, "ProviderState Received : " + state);
        Message msg = mHandler.obtainMessage(STATE_CHANGED,
                "ProviderState Received : " + state);
        mHandler.sendMessage(msg);
    }

    @Override
    public void onMessageReceived(
            org.iotivity.service.ns.common.Message message) {
        Log.i(TAG, "onMessageReceived");

        Log.i(TAG, "Message Id: " + message.getMessageId());
        Log.i(TAG, "Message title: " + message.getTitle());
        Log.i(TAG, "Message Content: " + message.getContentText());
        Log.i(TAG, "Message Topic: " + message.getTopic());
        Log.i(TAG, "Message Source: " + message.getSourceName());

        Message msg = mHandler.obtainMessage(MESSAGE_RECEIVED,
                "Message Id: " + message.getMessageId() + "\n"
                        + "Message title: " + message.getTitle() + "\n"
                        + "Message Content: " + message.getContentText() + "\n"
                        + "Message Topic: " + message.getTopic() + "\n"
                        + "Message Source: " + message.getSourceName());
        mHandler.sendMessage(msg);
        try {
            Log.i(TAG, "send READ syncInfo");
            mProvider.sendSyncInfo(message.getMessageId(),
                    SyncInfo.SyncType.READ);
        } catch (Exception e) {
            Log.e(TAG, "Exception : " + e);
        }
    }

    @Override
    public void onSyncInfoReceived(SyncInfo sync) {
        Log.i(TAG, "onSyncInfoReceived");

        Log.i(TAG, "Sync Id: " + sync.getMessageId());
        Log.i(TAG, "Sync STATE: " + sync.getState());
        Message msg = mHandler.obtainMessage(SYNCINFO_RECEIVED,
                "Sync Id: " + sync.getMessageId() + "\n" + "Sync STATE: "
                        + sync.getState());
        mHandler.sendMessage(msg);
    }
}
