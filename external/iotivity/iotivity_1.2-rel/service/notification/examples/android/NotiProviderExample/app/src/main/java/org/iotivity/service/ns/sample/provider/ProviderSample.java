/*
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
 */

package org.iotivity.service.ns.sample.provider;

import android.content.Context;
import android.os.*;
import android.util.Log;
import android.widget.Toast;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.OcResourceHandle;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;
import org.iotivity.service.ns.common.Message;
import org.iotivity.service.ns.provider.*;
import org.iotivity.service.ns.common.*;

import java.util.HashMap;

public class ProviderSample
        implements ProviderService.OnConsumerSubscribedListener,
        ProviderService.OnMessageSynchronizedListener {

    private static final String      TAG                 = "NS_PROVIDER_SAMPLE";

    private Context                  mContext            = null;
    private OcResourceHandle         mResourceHandle;
    private ProviderService          ioTNotification     = null;
    private HashMap<String, Integer> msgMap;

    private Handler                  mHandler            = null;

    private static final int         CONSUMER_SUBSCRIBED = 1;
    private static final int         MESSAGE_SYNC        = 2;

    private static final int         SYNC_READ           = 0;
    private static final int         SYNC_DISMISS        = 1;
    private static final int         SYNC_UNREAD         = 2;
    private boolean                  gAcceptor;
    private Consumer                 gConsumer;

    public ProviderSample(Context context) {
        Log.i(TAG, "Create providerSample Instance");

        this.msgMap = new HashMap<>();
        this.mContext = context;
        ioTNotification = ProviderService.getInstance();
    }

    public void setHandler(Handler handler) {
        this.mHandler = handler;
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

    public void start(boolean policy) {
        Log.i(TAG, "Start ProviderService -IN");
        configurePlatform();
        gAcceptor = policy;
        try {
            ioTNotification.start(this, this, policy, "Info", false);
        } catch (Exception e) {
            Log.e(TAG, "Exception: start : " + e);
        }
        Log.i(TAG, "Start ProviderService - OUT");
    }

    public void registerTopic() {
        Log.i(TAG, "Register Topics -IN");
        try {
            ioTNotification.registerTopic("OCF_TOPIC1");
            ioTNotification.registerTopic("OCF_TOPIC2");
            ioTNotification.registerTopic("OCF_TOPIC3");
            ioTNotification.registerTopic("OCF_TOPIC4");
        } catch (Exception e) {
            Log.e(TAG, "Exception: registerTopic  : "+ e);
        }

        Log.i(TAG, "Start ProviderService - OUT");
    }

    public void setTopic() {
        Log.i(TAG, "Set Topic -IN");
        if (gConsumer == null) {
            return;
        }
        try {
            gConsumer.setTopic("OCF_TOPIC1");
            gConsumer.setTopic("OCF_TOPIC2");
            gConsumer.setTopic("OCF_TOPIC3");
            gConsumer.setTopic("OCF_TOPIC4");
        } catch (Exception e) {
            Log.e(TAG, "Exception: setTopic : " + e);
        }

        Log.i(TAG, "setTopic ProviderService - OUT");
        return;
    }

    public void stop() {
        Log.i(TAG, "Stop ProviderService - IN");
        try {
            OcPlatform.stopPresence();
        } catch (Exception e) {
            Log.e(TAG,"Exception: stopping presence when terminating NS server: "+ e);
        }
        try {
            ioTNotification.stop();
        } catch (Exception e) {
            Log.e(TAG, "Exception: stop : " + e);
        }

        Log.i(TAG, "Stop ProviderService - OUT");
    }

    public Message createMessage() {
        Log.i(TAG, "createMessage ProviderService - IN");
        Message message = null;
        try {
            message = ioTNotification.createMessage();
        } catch (Exception e) {
            Log.e(TAG, "Exception: createMessage : " + e);
        }
        Log.i(TAG, "createMessage ProviderService - OUT");
        return message;
    }

    public void sendMessage(Message notiMessage) {
        Log.i(TAG, "SendMessage ProviderService - IN");

        try {
            ioTNotification.sendMessage(notiMessage);
        } catch (Exception e) {
            Log.e(TAG, "Exception: sendMessage : " + e);
        }

        Log.i(TAG, "SendMessage ProviderService - OUT");
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(mContext, "Notification sent",
                        Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void sendSyncInfo(long messageId, SyncInfo.SyncType syncType) {
        Log.i(TAG, "SendSyncInfo ProviderService - IN");
        if (msgMap.containsKey(messageId)) {
            if (msgMap.get(messageId) == SYNC_UNREAD) {
                try {
                    ioTNotification.sendSyncInfo(messageId, syncType);
                    Log.i(TAG, "Notification Sync ");
                } catch (Exception e) {
                    Log.e(TAG, "Exception: sendSyncInfo : " + e);
                }
                Log.i(TAG, "SendSyncInfo ProviderService - OUT");
                msgMap.put("ID: " + messageId, SYNC_READ);
            }
        }
    }

    public void enableRemoteService(String servAdd) {
        Log.i(TAG, "EnableRemoteService ProviderService - IN");
        try {
            ioTNotification.enableRemoteService(servAdd);
        } catch (Exception e) {
            Log.e(TAG, "Exception: enableRemoteService : " + e);
        }
        Log.i(TAG, "EnableRemoteService ProviderService - OUT");
        return;
    }

    public void disableRemoteService(String servAdd) {
        Log.i(TAG, "DisableRemoteService ProviderService - IN");
        try {
            ioTNotification.disableRemoteService(servAdd);
        } catch (Exception e) {
            Log.e(TAG, "Exception: disableRemoteService : " + e);
        }
        Log.i(TAG, "DisableRemoteService ProviderService - OUT");
        return;
    }

    public void subscribeMQService(String servAdd, String topicName) {
        Log.i(TAG, "SubscribeMQService ProviderService - IN");
        try {
            ioTNotification.subscribeMQService(servAdd, topicName);
        } catch (Exception e) {
            Log.e(TAG, "Exception: subscribeMQService : " + e);
        }
        Log.i(TAG, "SubscribeMQService ProviderService - OUT");
        return;
    }

    public void acceptSubscription(Consumer consumer, boolean accepted) {
        Log.i(TAG, "AcceptSubscription ProviderService - IN");
        try {
            consumer.acceptSubscription(accepted);
        } catch (Exception e) {
            Log.e(TAG, "Exception: acceptSubscription : " + e);
        }
        Log.i(TAG, "AcceptSubscription ProviderService - OUT");
    }

    @Override
    public void onConsumerSubscribed(Consumer consumer) {
        Log.i(TAG, "onConsumerSubscribed - IN");
        gConsumer = consumer;
        acceptSubscription(consumer, true);
        android.os.Message msg = mHandler.obtainMessage(CONSUMER_SUBSCRIBED,
                "Consumer Id: " + consumer.getConsumerId());
        mHandler.sendMessage(msg);
        Log.i(TAG, "onConsumerSubscribed - OUT");
    }

    @Override
    public void onMessageSynchronized(SyncInfo syncInfo) {
        Log.i(TAG,
                "Received SyncInfo with messageID: " + syncInfo.getMessageId());
        android.os.Message msg = mHandler.obtainMessage(MESSAGE_SYNC,
                "Message Id: " + syncInfo.getMessageId());
        mHandler.sendMessage(msg);
    }

    public HashMap<String, Integer> getMsgMap() {
        return msgMap;
    }
}
