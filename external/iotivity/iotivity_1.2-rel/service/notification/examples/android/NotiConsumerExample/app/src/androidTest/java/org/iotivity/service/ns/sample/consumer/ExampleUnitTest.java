/*
* Copyright 2016 Samsung Electronics All Rights Reserved.
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
*/

package org.iotivity.service.ns.sample.consumer;

import android.app.Application;
import android.content.Context;
import android.support.test.runner.AndroidJUnit4;
import android.test.ApplicationTestCase;
import android.util.Log;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;
import org.iotivity.service.ns.common.Message;
import org.iotivity.service.ns.common.NSException;
import org.iotivity.service.ns.common.NSErrorCode;
import org.iotivity.service.ns.common.Topic;
import org.iotivity.service.ns.common.TopicsList;
import org.iotivity.service.ns.common.SyncInfo;
import org.iotivity.service.ns.consumer.ConsumerService;
import org.iotivity.service.ns.provider.ProviderService;
import org.iotivity.service.ns.consumer.Provider;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Iterator;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * To work on unit tests, switch the Test Artifact in the Build Variants view.
 */
@RunWith(AndroidJUnit4.class)
public class ExampleUnitTest extends ApplicationTestCase<Application> {
    public ExampleUnitTest() {
        super(Application.class);
    }

    private static Context                   mContext = null;
    private ProviderService                  gProviderRes;
    private ProviderSimulator                gProviderSimul;
    private static ConsumerService           gConsumerRes;
    private static DiscoveryCallbackListener disCb;
    private static ProviderCallbackListener  provCb;
    CountDownLatch                           lockObject;
    Response                                 response;
    private static String                    TAG      = "ConsumerExample UnitTest";

    public void startBefore(boolean subControllability) {
        Log.i(TAG, "startConsumerBefore - IN");
        try {
            gConsumerRes.start(disCb);
            gProviderRes.start(gProviderSimul, gProviderSimul,
                    subControllability, "nothing", false);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        Log.i(TAG, "startConsumerBefore - OUT");
    }

    public void startAfter(boolean subControllability) {
        Log.i(TAG, "startConsumerAfter - IN");
        try {
            gProviderRes.start(gProviderSimul, gProviderSimul,
                    subControllability, "nothing", false);
            gConsumerRes.start(disCb);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        Log.i(TAG, "startConsumerAfter - OUT");
    }

    public void setListener(Provider mProvider) {
        try {
            mProvider.setListener(provCb, provCb, provCb);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void subscribe(Provider.ProviderState state) {
        lockObject = new CountDownLatch(1);
        response = new Response();
        provCb.set(lockObject, response);
        provCb.setState(state);
        Provider mProvider = disCb.getProvider();
        setListener(mProvider);
        try {
            if (!mProvider.isSubscribed()) {
                Log.i(TAG, "not subscribed");
                mProvider.subscribe();
            }
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public void unsubscribe() {
        lockObject = new CountDownLatch(1);
        response = new Response();
        provCb.set(lockObject, response);
        provCb.setState(Provider.ProviderState.STOPPED);
        Provider mProvider = disCb.getProvider();
        setListener(mProvider);
        try {
            if (mProvider.isSubscribed()) {
                Log.i(TAG, "subscribed");
                mProvider.unsubscribe();
            }
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public long sendMessage() {
        lockObject = new CountDownLatch(1);
        response = new Response();
        provCb.set(lockObject, response);
        org.iotivity.service.ns.common.Message msg = gProviderSimul
                .getMessage();
        long Id = msg.getMessageId();
        provCb.setId(Id);
        try {
            gProviderRes.sendMessage(msg);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return Id;
    }

    public void sendSyncInfo(long id, SyncInfo.SyncType type) {
        try {
            lockObject = new CountDownLatch(1);
            response = new Response();
            provCb.set(lockObject, response);
            provCb.setType(type);
            gProviderRes.sendSyncInfo(id, type);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void providerSendSyncInfo(Provider provider, long id,
            SyncInfo.SyncType type) {
        try {
            lockObject = new CountDownLatch(1);
            response = new Response();
            provCb.set(lockObject, response);
            provCb.setType(type);
            provider.sendSyncInfo(id, type);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void registerTopic(String topic) {
        lockObject = new CountDownLatch(1);
        response = new Response();
        provCb.set(lockObject, response);
        provCb.setState(Provider.ProviderState.TOPIC);
        try {
            gProviderRes.registerTopic(topic);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @BeforeClass
    public static void Initialize() {
        PlatformConfig platformConfig = new PlatformConfig(mContext,
                ServiceType.IN_PROC, ModeType.CLIENT_SERVER, "0.0.0.0",
                0, // Uses randomly available port
                QualityOfService.LOW);

        OcPlatform.Configure(platformConfig);
        try {
            OcPlatform.stopPresence(); // Initialize OcPlatform
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Before
    public void SetUp() {
        Log.i(TAG, "SetUp - IN");
        disCb = new DiscoveryCallbackListener();
        provCb = new ProviderCallbackListener();
        gConsumerRes = ConsumerService.getInstance();
        gProviderRes = ProviderService.getInstance();
        gProviderSimul = new ProviderSimulator();
        lockObject = new CountDownLatch(1);
        response = new Response();
        disCb.set(lockObject, response);
        Log.i(TAG, "SetUp - OUT");
    }

    @After
    public void TearDown() {
        Log.i(TAG, "TearDown - IN");
        try {
            gConsumerRes.stop();
            gProviderRes.stop();
            lockObject = new CountDownLatch(1);
            lockObject.await(2000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        Log.i(TAG, "TearDown - OUT");
    }

    @Test
    public void DiscoverProviderWithNonAccepterWhenStartedConsumerFirst() {
        startBefore(false);
        assertEquals(true, response.get());
    }

    @Test
    public void DiscoverProviderWithNonAccepterWhenStartedConsumerAfter() {
        startAfter(false);
        assertEquals(true, response.get());
    }

    @Test
    public void DiscoverProviderWithNonAccepterWhenRescan() {
        startAfter(false);
        assertEquals(true, response.get());
        try {
            gConsumerRes.rescanProvider();
        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectSubscribeSuccess() {
        startAfter(false);
        assertEquals(true, response.get());

        Provider.ProviderState state = Provider.ProviderState.ALLOW;
        subscribe(state);
        assertEquals(true, response.get());
    }
    @Test
    public void ExpectUnSubscribeSuccess() {
        startAfter(false);
        assertEquals(true, response.get());

        Provider.ProviderState state = Provider.ProviderState.ALLOW;
        subscribe(state);
        assertEquals(true, response.get());
        unsubscribe();
        assertEquals(true, response.get());
    }
    @Test
    public void ExpectReceiveNotification() {
        startAfter(false);
        assertEquals(true, response.get());

        Provider.ProviderState state = Provider.ProviderState.ALLOW;
        subscribe(state);
        assertEquals(true, response.get());

        sendMessage();
        assertEquals(true, response.get());

    }

    @Test
    public void DiscoverProviderWithAccepterisProvider() {
        startAfter(true);
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectReceiveNotificationWithAccepterisProvider() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        sendMessage();
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectCallbackReadCheckWhenProviderNotifySync() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        long id = sendMessage();
        assertEquals(true, response.get());

        sendSyncInfo(id, SyncInfo.SyncType.READ);
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectCallbackDismissCheckWhenProviderNotifySync() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        long id = sendMessage();
        assertEquals(true, response.get());

        sendSyncInfo(id, SyncInfo.SyncType.DELETED);
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectCallbackReadCheckWhenConsumerPostSync() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        long id = sendMessage();
        assertEquals(true, response.get());

        providerSendSyncInfo(mProvider, id, SyncInfo.SyncType.READ);
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectCallbackDismissCheckWhenConsumerPostSync() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        long id = sendMessage();
        assertEquals(true, response.get());

        providerSendSyncInfo(mProvider, id, SyncInfo.SyncType.DELETED);
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectCallbackTopicUpdated() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        registerTopic("OIC_TOPIC1");
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectEQTopicList() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        try {
            registerTopic("OIC_TOPIC1");
            assertEquals(true, response.get());
            registerTopic("OIC_TOPIC2");
            assertEquals(true, response.get());
            registerTopic("OIC_TOPIC3");
            assertEquals(true, response.get());

            response = new Response();
            Iterator<Topic> it = mProvider.getTopicList().getTopicsList()
                    .iterator();
            Iterator<Topic> it2 = gProviderRes.getRegisteredTopicList()
                    .getTopicsList().iterator();
            while (it.hasNext()) {
                Topic element = it.next();
                Topic element2 = it2.next();
                Log.i(TAG, element2.getTopicName());
                if (!element.getTopicName().equals(element2.getTopicName())) {
                    response.set(false);
                    break;
                }
                response.set(true);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectFailUpdateTopicOnConsumer() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        registerTopic("OIC_TOPIC1");
        assertEquals(true, response.get());

        try {
            TopicsList list = mProvider.getTopicList();
            Iterator<Topic> it = list.getTopicsList().iterator();
            while (it.hasNext()) {
                Topic element = it.next();
                element.setState(Topic.TopicState.SUBSCRIBED);
            }
            mProvider.updateTopicList(list);
        } catch (NSException e) {
            e.printStackTrace();
            assertEquals(NSErrorCode.ERROR, e.getErrorCode());
        }
    }

    @Test
    public void ExpectCallbackStoppedProvider() {
        startAfter(true);
        assertEquals(true, response.get());

        Provider mProvider = disCb.getProvider();
        setListener(mProvider);

        lockObject = new CountDownLatch(1);
        response = new Response();
        provCb.set(lockObject, response);
        provCb.setState(Provider.ProviderState.STOPPED);

        try {
            gProviderRes.stop();
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertEquals(true, response.get());

    }
}

class DiscoveryCallbackListener
        implements ConsumerService.OnProviderDiscoveredListener {
    private CountDownLatch mLockObject;
    private Response       mResponse;
    private Provider       mProvider;
    private static String  TAG = "UnitTest DiscoveryCallbackListener";

    public void set(CountDownLatch lockObject, Response response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    public Provider getProvider() {
        return mProvider;
    }

    @Override
    public void onProviderDiscovered(Provider provider) {
        Log.i(TAG, provider.getProviderId());
        mProvider = provider;
        if (mResponse != null) {
            Log.i(TAG, "onProviderDiscovered: Lock released");
            mResponse.set(true);
            mLockObject.countDown();
        }
    }
}

class ProviderCallbackListener implements Provider.OnProviderStateListener,
        Provider.OnMessageReceivedListener, Provider.OnSyncInfoReceivedListener {

    private CountDownLatch         mLockObject;
    private Response               mResponse;
    private Provider.ProviderState mState;
    private SyncInfo.SyncType      mType;
    private long                   mId;
    private static String          TAG = "UnitTest ProviderCallbackListener";

    public void set(CountDownLatch lockObject, Response response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    public void setState(Provider.ProviderState state) {
        mState = state;
    }

    public void setId(long id) {
        mId = id;
    }

    void setType(SyncInfo.SyncType type) {
        mType = type;
    }

    @Override
    public void onProviderStateReceived(Provider.ProviderState state) {
        Log.i(TAG, "onProviderStateReceived: " + state);
        if (mState == state) {
            Log.i(TAG, "onProviderStateReceived: Lock released");
            mResponse.set(true);
            mLockObject.countDown();
        }

    }

    @Override
    public void onMessageReceived(org.iotivity.service.ns.common.Message msg) {
        Log.i(TAG, "onMessageReceived: " + msg.getMessageId());
        if (mId == msg.getMessageId()) {
            Log.i(TAG, "onMessageReceived: Lock released");
            mResponse.set(true);
            mLockObject.countDown();
        }
    }

    @Override
    public void onSyncInfoReceived(SyncInfo syncInfo) {
        Log.i(TAG, "onSyncInfoReceived: " + syncInfo.getState());
        if (mType == syncInfo.getState()) {
            Log.i(TAG, "onSyncInfoReceived: Lock released");
            mResponse.set(true);
            mLockObject.countDown();
        }
    }
}

class Response {
    private boolean state;

    Response() {
        state = false;
    }

    public void set(boolean val) {
        state = val;
    }

    public boolean get() {
        return state;
    }
}
