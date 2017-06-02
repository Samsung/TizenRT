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

package org.iotivity.service.ns.sample.provider;

import java.util.Iterator;
import java.util.Vector;
import java.util.concurrent.CountDownLatch;

import android.app.Application;
import android.content.Context;
import android.test.ApplicationTestCase;
import android.util.Log;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;
import org.iotivity.service.ns.common.NSException;
import org.iotivity.service.ns.common.NSErrorCode;
import org.iotivity.service.ns.common.SyncInfo;
import org.iotivity.service.ns.common.Topic;

import android.support.test.runner.AndroidJUnit4;

import org.iotivity.service.ns.consumer.ConsumerService;
import org.iotivity.service.ns.provider.Consumer;
import org.iotivity.service.ns.provider.ProviderService;
import org.junit.After;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.Before;
import org.junit.runner.RunWith;

import java.util.concurrent.TimeUnit;

/**
 * To work on unit tests, switch the Test Artifact in the Build Variants view.
 */
@RunWith(AndroidJUnit4.class)
public class ExampleUnitTest extends ApplicationTestCase<Application> {
    public ExampleUnitTest() {
        super(Application.class);
    }

    private static Context                      mContext = null;
    private static SubscriptionCallbackListener subCb;
    private static SyncCallbackListener         syncCb;
    private CountDownLatch                      lockObject;
    private Response                            response;
    private static ProviderService              gProviderRes;
    private static ConsumerService              gConsumerRes;
    private static ConsumerSimulator            gConsumerSim;
    private static String                       TAG      = "UnitTest ProviderService";

    public void start(boolean subControllability) {
        try {
            gProviderRes.start(subCb, syncCb, subControllability, "ok", false);
            gConsumerRes.start(gConsumerSim);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public org.iotivity.service.ns.common.Message createMessage() {
        org.iotivity.service.ns.common.Message msg = null;
        try {
            msg = gProviderRes.createMessage();
            msg.setTitle("Title");
            msg.setSourceName("Source");
            msg.setContentText("ContentText");
        } catch (Exception e) {
            e.printStackTrace();
        }
        return msg;
    }

    public org.iotivity.service.ns.common.Message sendMessage(
            boolean accepted) {
        lockObject = new CountDownLatch(1);
        response = new Response();
        org.iotivity.service.ns.common.Message msg = null;
        gConsumerSim.set(lockObject, response, "msg");
        try {
            subCb.get().acceptSubscription(accepted);
            CountDownLatch waitLock = new CountDownLatch(1);
            waitLock.await(400, TimeUnit.MILLISECONDS);
            msg = createMessage();
            gProviderRes.sendMessage(msg);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return msg;
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
        subCb = new SubscriptionCallbackListener();
        syncCb = new SyncCallbackListener();
        gConsumerRes = ConsumerService.getInstance();
        gProviderRes = ProviderService.getInstance();
        gConsumerSim = new ConsumerSimulator();
        lockObject = new CountDownLatch(1);
        response = new Response();
        subCb.set(lockObject, response);
        Log.i(TAG, "SetUp - OUT");
    }

    @After
    public void TearDown() {
        Log.i(TAG, "TearDown - IN");
        try {
            gProviderRes.stop();
            gConsumerRes.stop();
            lockObject = new CountDownLatch(1);
            lockObject.await(2000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        Log.i(TAG, "TearDown - OUT");
    }

    @Test
    public void StartProviderPositiveWithPolicyTrue() {
        try {
            gProviderRes.start(subCb, syncCb, true, "ok", false);
            gProviderRes.stop();
        } catch (NSException e) {
            e.printStackTrace();
            assertEquals(NSErrorCode.OK, e.getErrorCode());
        }
    }

    @Test
    public void StartProviderPositiveWithPolicyFalse() {
        try {
            gProviderRes.start(subCb, syncCb, false, "ok", false);
            gProviderRes.stop();
        } catch (NSException e) {
            e.printStackTrace();
            assertEquals(NSErrorCode.OK, e.getErrorCode());
        }
    }

    @Test
    public void StopProviderPositive() {
        try {
            gProviderRes.start(subCb, syncCb, true, "ok", false);
            gProviderRes.stop();
        } catch (NSException e) {
            e.printStackTrace();
            assertEquals(NSErrorCode.OK, e.getErrorCode());
        }
    }

    @Test
    public void ExpectCallbackWhenReceiveSubscribeRequestWithAccepterProvider() {
        start(true);
        assertEquals(true, response.get());
    }

    @Test
    public void NeverCallNotifyOnConsumerByAcceptIsFalse() {
        start(true);
        assertEquals(true, response.get());
        sendMessage(false);
        assertEquals(false, response.get());
    }

    @Test
    public void ExpectCallNotifyOnConsumerByAcceptIsTrue() {

        start(true);
        assertEquals(true, response.get());
        sendMessage(true);
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectCallbackSyncOnReadToConsumer() {
        start(true);
        assertEquals(true, response.get());
        org.iotivity.service.ns.common.Message msg = sendMessage(true);
        assertEquals(true, response.get());
        try {
            lockObject = new CountDownLatch(1);
            response = new Response();
            gConsumerSim.set(lockObject, response, "sync");
            gProviderRes.sendSyncInfo(msg.getMessageId(),
                    SyncInfo.SyncType.READ);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals(true, response.get());
    }

    @Test
    public void ExpectCallbackSyncOnReadFromConsumer() {

        start(true);
        assertEquals(true, response.get());
        org.iotivity.service.ns.common.Message msg = sendMessage(true);
        assertEquals(true, response.get());
        try {
            lockObject = new CountDownLatch(1);
            response = new Response();
            syncCb.set(lockObject, response);
            gConsumerSim.sendSyncInfo(msg.getMessageId(),
                    SyncInfo.SyncType.READ);
            lockObject.await(4000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void ExpectEqualAddedTopicsAndRegisteredTopics() {
        boolean isSame = true;
        String str[] = { "TOPIC1", "TOPIC2" };
        try {
            gProviderRes.start(subCb, syncCb, true, "ok", false);
            gProviderRes.registerTopic(str[0]);
            gProviderRes.registerTopic(str[1]);
            Vector<Topic> list = gProviderRes.getRegisteredTopicList()
                    .getTopicsList();
            Iterator<Topic> it = list.iterator();
            if (list.size() != 2) {
                isSame = false;
                Log.i(TAG, "Size is " + list.size());
            }
            int count = 0;
            while (it.hasNext()) {
                Topic element = it.next();
                Log.i(TAG, "element is " + element.getTopicName());
                if (!str[count].equals(element.getTopicName())) {
                    isSame = false;
                }
                count++;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals(true, isSame);
    }

    @Test
    public void ExpectEqualUnregisteredTopicsAndRegisteredTopics() {
        boolean isSame = true;
        String str[] = { "TOPIC1", "TOPIC2" };
        try {
            gProviderRes.start(subCb, syncCb, true, "ok", false);
            gProviderRes.registerTopic(str[0]);
            gProviderRes.registerTopic(str[1]);
            gProviderRes.unregisterTopic(str[0]);
            Vector<Topic> list = gProviderRes.getRegisteredTopicList()
                    .getTopicsList();
            Iterator<Topic> it = list.iterator();
            if (list.size() != 1) {
                isSame = false;
                Log.i(TAG, "Size is " + list.size());
            }
            int count = 1;
            while (it.hasNext()) {
                Topic element = it.next();
                Log.i(TAG, "element is " + element.getTopicName());
                if (!str[count].equals(element.getTopicName())) {
                    isSame = false;
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals(true, isSame);
    }

    @Test
    public void ExpectEqualSetConsumerTopicsAndGetConsumerTopics() {
        boolean isSame = false;
        start(true);
        assertEquals(true, response.get());
        String str[] = { "TOPIC1", "TOPIC2" };
        try {
            gProviderRes.registerTopic(str[0]);
            gProviderRes.registerTopic(str[1]);
            subCb.get().setTopic(str[0]);

            Vector<Topic> list = subCb.get().getConsumerTopicList()
                    .getTopicsList();
            Iterator<Topic> it = list.iterator();
            int count = 0;
            String[] compStr = new String[10];
            Topic.TopicState[] state = new Topic.TopicState[10];
            while (it.hasNext()) {
                Topic element = it.next();
                Log.i(TAG, "element is " + element.getTopicName());
                compStr[count] = element.getTopicName();
                state[count++] = element.getState();
            }
            if (compStr[0].compareTo(str[0]) == 0
                    && compStr[1].compareTo(str[1]) == 0
                    && state[0] == Topic.TopicState.SUBSCRIBED
                    && state[1] == Topic.TopicState.UNSUBSCRIBED) {
                isSame = true;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals(true, isSame);
    }

    @Test
    public void ExpectEqualUnSetConsumerTopicsAndGetConsumerTopics() {
        boolean isSame = false;
        start(true);
        assertEquals(true, response.get());
        String str[] = { "TOPIC1", "TOPIC2" };
        try {
            gProviderRes.registerTopic(str[0]);
            gProviderRes.registerTopic(str[1]);
            subCb.get().setTopic(str[0]);
            subCb.get().setTopic(str[1]);
            subCb.get().unsetTopic(str[0]);

            Vector<Topic> list = subCb.get().getConsumerTopicList()
                    .getTopicsList();
            Iterator<Topic> it = list.iterator();
            int count = 0;
            String[] compStr = new String[10];
            Topic.TopicState[] state = new Topic.TopicState[10];
            while (it.hasNext()) {
                Topic element = it.next();
                Log.i(TAG, "element is " + element.getTopicName());
                compStr[count] = element.getTopicName();
                state[count++] = element.getState();
            }
            if (compStr[0].compareTo(str[0]) == 0
                    && compStr[1].compareTo(str[1]) == 0
                    && state[0] == Topic.TopicState.UNSUBSCRIBED
                    && state[1] == Topic.TopicState.SUBSCRIBED) {
                isSame = true;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals(true, isSame);
    }

}

class SubscriptionCallbackListener
        implements ProviderService.OnConsumerSubscribedListener {
    private CountDownLatch mLockObject;
    private Response       mResponse;
    private Consumer       mConsumer;
    private static String  TAG = "UnitTest SubscriptionCallbackListener";

    @Override
    public void onConsumerSubscribed(Consumer consumer) {
        Log.i(TAG, "onConsumerSubscribed");
        mConsumer = consumer;
        if (mResponse != null) {
            mResponse.set(true);
            mLockObject.countDown();
        }
    }

    public void set(CountDownLatch lockObject, Response response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    public Consumer get() {
        return mConsumer;
    }
}

class SyncCallbackListener
        implements ProviderService.OnMessageSynchronizedListener {
    private CountDownLatch mLockObject;
    private Response       mResponse;
    private static String  TAG = "UnitTest SyncCallbackListener";

    @Override
    public void onMessageSynchronized(SyncInfo syncInfo) {
        Log.i(TAG, "onMessageSynchronized");
        if (mResponse != null) {
            mResponse.set(true);
            mLockObject.countDown();
        }
    }

    public void set(CountDownLatch lockObject, Response response) {
        mLockObject = lockObject;
        mResponse = response;
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
