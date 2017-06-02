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

package org.iotivity.service.ns.sample.consumer;

import android.util.Log;

import org.iotivity.service.ns.common.MediaContents;
import org.iotivity.service.ns.common.Message;
import org.iotivity.service.ns.common.SyncInfo;
import org.iotivity.service.ns.provider.Consumer;
import org.iotivity.service.ns.provider.ProviderService;

class ProviderSimulator
        implements ProviderService.OnMessageSynchronizedListener,
        ProviderService.OnConsumerSubscribedListener {
    private String   TAG = "Provider Simulator";
    private Consumer gConsumer;

    @Override
    public void onMessageSynchronized(SyncInfo syncInfo) {
        Log.i(TAG, "Notification onMessageSynchronized: ");
    }

    @Override
    public void onConsumerSubscribed(Consumer consumer) {
        gConsumer = consumer;
        try {
            gConsumer.acceptSubscription(true);
            Log.i(TAG, "Notification AcceptSubscription" );
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Message getMessage() {
        try {
            Message msg = ProviderService.getInstance().createMessage();
            msg.setTitle("Title");
            msg.setSourceName("provider");
            msg.setContentText("notification");
            MediaContents media = new MediaContents("new");
            msg.setMediaContents(media);
            return msg;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

}
