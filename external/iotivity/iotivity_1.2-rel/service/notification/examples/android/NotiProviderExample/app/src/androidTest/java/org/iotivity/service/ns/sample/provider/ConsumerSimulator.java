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

package org.iotivity.service.ns.sample.provider;

import android.util.Log;

import org.iotivity.service.ns.common.Message;
import org.iotivity.service.ns.common.SyncInfo;
import org.iotivity.service.ns.consumer.ConsumerService;
import org.iotivity.service.ns.consumer.Provider;

import java.util.concurrent.CountDownLatch;

class ConsumerSimulator implements ConsumerService.OnProviderDiscoveredListener,
        Provider.OnProviderStateListener, Provider.OnMessageReceivedListener,
        Provider.OnSyncInfoReceivedListener {
    private String         TAG = "Consumer Simulator";
    private Provider       mProvider;
    private CountDownLatch mLockObject;
    private Response       mResponse;
    private String         mExpectCb;

    public void set(CountDownLatch lockObject, Response response, String name) {
        Log.i(TAG, "Setting lock Simulator: ");
        mLockObject = lockObject;
        mResponse = response;
        mExpectCb = name;
    }

    @Override
    public void onProviderDiscovered(Provider provider) {
        mProvider = provider;
        try {
            provider.setListener(this, this, this);
            if (!provider.isSubscribed()) {
                provider.subscribe();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @Override
    public void onProviderStateReceived(Provider.ProviderState providerState) {
    }

    @Override
    public void onMessageReceived(Message message) {
        if (mExpectCb == "msg") {
            mResponse.set(true);
            mLockObject.countDown();
        }
    }

    @Override
    public void onSyncInfoReceived(SyncInfo syncInfo) {
        if (mExpectCb == "sync") {
            mResponse.set(true);
            mLockObject.countDown();
        }
    }

    public void sendSyncInfo(long id, SyncInfo.SyncType type) {
        try {
            mProvider.sendSyncInfo(id, type);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}