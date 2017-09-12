/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

package org.iotivity.cloud.mqserver.resources;

import static com.jayway.awaitility.Awaitility.await;
import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;

import org.I0Itec.zkclient.ZkClient;
import org.I0Itec.zkclient.ZkConnection;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.exception.ServerException.ForbiddenException;
import org.iotivity.cloud.base.exception.ServerException.NotFoundException;
import org.iotivity.cloud.base.exception.ServerException.PreconditionFailedException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.Observe;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.mqserver.Constants;
import org.iotivity.cloud.util.Cbor;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import kafka.admin.TopicCommand;
import kafka.admin.TopicCommand.TopicCommandOptions;
import kafka.utils.ZKStringSerializer$;
import kafka.utils.ZkUtils;

public class MQBrokerResourceTest {
    private final String     MQ_BROKER_URI     = Constants.MQ_BROKER_FULL_URI;

    private MQBrokerResource mMqBrokerResource = null;
    private String           mTopicPrefix      = null;
    private CoapDevice       mMockDevice       = null;
    IResponse                mResponse         = null;
    CountDownLatch           mLatch            = null;

    // insert user's zookeper and broker addresses
    String                   mZookeeper        = "127.0.0.1:2181";
    String                   mBroker           = "127.0.0.1:9092";

    @Before
    // setup for each test
    public void setUp() throws Exception {
        mMqBrokerResource = new MQBrokerResource();

        mTopicPrefix = "mqtestTopic";
        mMqBrokerResource.setKafkaInformation(mZookeeper, mBroker);
        mLatch = new CountDownLatch(1);
        mResponse = null; // initialize response packet
        mMockDevice = mock(CoapDevice.class);

        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                mResponse = resp;
                mLatch.countDown();
                return resp;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
    }

    @After
    public void tearDown() throws Exception {
        // delete topics in Kafka broker
        ZkClient zkClient = new ZkClient(mZookeeper, 10000, 10000,
                ZKStringSerializer$.MODULE$);
        ZkUtils zkUtils = new ZkUtils(zkClient, new ZkConnection(mZookeeper),
                false);

        String topic = MQ_BROKER_URI + "/*";
        topic = topic.replace('/', '.');

        String[] arr = { "--topic", topic };
        TopicCommandOptions opts = new TopicCommandOptions(arr);
        TopicCommand.deleteTopic(zkUtils, opts);

        zkClient.close();
        zkUtils.close();
    }

    @Test
    // test topic creation
    public void testTopicCreationOnDefaultRequestReceived() throws Exception {
        System.out.println("\t--------------Topic Creation Test------------");
        CreateTopic(mMockDevice, mTopicPrefix);
        // assertion: if the response status is "CREATED"
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.CREATED));
    }

    @Test
    // test subtopic create
    public void testSubTopicCreationOnDefaultRequestReceived()
            throws Exception {
        System.out
                .println("\t--------------SubTopic Creation Test------------");
        String mainTopic = mTopicPrefix + "Main";
        String subTopic = mTopicPrefix + "Sub";
        // create main topic
        CreateTopic(mMockDevice, mainTopic);
        // create sub topic
        CreateSubTopic(mMockDevice, mainTopic, subTopic);
        // assertion: if the response status is "CREATED"
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.CREATED));
    }

    @Test
    // test topic publish
    public void testTopicPublishOnDefaultRequestReceived() throws Exception {
        System.out.println("\t--------------Topic Publish Test------------");
        String topic = mTopicPrefix + "ForPub";
        // topic creation
        CreateTopic(mMockDevice, topic);
        // topic publish
        PublishTopic(mMockDevice, topic);
        // assertion: if the response status is "CHANGED"
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
    }

    @Test
    // test subscribe request
    public void testSubscribeOnDefaultRequestReceived() throws Exception {
        System.out.println("\t--------------Topic Subscribe Test------------");
        CoapDevice mockSubscriber = mock(CoapDevice.class);
        String topic = mTopicPrefix + "SubscribeTest";
        // create topic
        CreateTopic(mMockDevice, topic);
        // publish topic
        PublishTopic(mMockDevice, topic);
        // callback mock for subscriber
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                // assertion: if the response status is "CONTENT"
                // assertion: if the response payload has data
                assertTrue(methodCheck(resp, ResponseStatus.CONTENT));
                assertTrue(payloadCheck(resp));
                return resp;
            }
        }).when(mockSubscriber).sendResponse(Mockito.anyObject());
        // subscribe topic
        SubscribeTopic(mockSubscriber, topic, Observe.SUBSCRIBE);
    }

    @Test
    // test unsubscribe request
    public void testUnSubscribeOnDefaultRequestReceived() throws Exception {
        System.out
                .println("\t--------------Topic Unsubscribe Test------------");
        CountDownLatch latchSubscriber = new CountDownLatch(2);
        CoapDevice mockSubscriber = mock(CoapDevice.class);
        String topic = mTopicPrefix + "UnSubscribeTest";
        // create topic
        CreateTopic(mMockDevice, topic);
        // publish topic
        PublishTopic(mMockDevice, topic);
        // callback mock for subscriber
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                latchSubscriber.countDown();
                if (latchSubscriber.getCount() == 0) {
                    // assertion: if the response payload has data
                    assertTrue(methodCheck(resp, ResponseStatus.CONTENT));
                    assertTrue(payloadCheck(resp));
                }
                return resp;
            }
        }).when(mockSubscriber).sendResponse(Mockito.anyObject());
        // subscribe topic
        SubscribeTopic(mockSubscriber, topic, Observe.SUBSCRIBE);
        // unsubscribe topic
        SubscribeTopic(mockSubscriber, topic, Observe.UNSUBSCRIBE);
    }

    @Test
    // test delete request
    public void testDeleteTopicOnDefaultRequestReceived() throws Exception {
        System.out.println("\t--------------Topic Delete Test------------");
        String topic = mTopicPrefix + "DeleteTest";
        // create topic
        CreateTopic(mMockDevice, topic);
        // delete topic
        DeleteTopic(mMockDevice, topic);
        // assertion: if the response status is "DELETED"
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
        assertTrue(mLatch.await(1L, SECONDS));
    }

    @Test
    // test delete subtopic request
    public void testDeleteSubTopicOnDefaultRequestReceived() throws Exception {
        System.out.println("\t--------------Subtopic Delete Test------------");
        String topic = mTopicPrefix + "DeleteTest";
        String subTopic = mTopicPrefix + "DeleteTestSub";
        // create topic
        CreateTopic(mMockDevice, topic);
        // create subtopic
        CreateSubTopic(mMockDevice, topic, subTopic);
        // delete subtopic
        DeleteSubTopic(mMockDevice, topic, subTopic);
        // assertion: if the response status is "DELETED"
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
    }

    @Test
    // test notify
    public void testTopicSubscribeNofityOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Topic Publish Notify Test------------");
        CoapDevice mockSubscriber = mock(CoapDevice.class);
        CountDownLatch latchSubscriber = new CountDownLatch(2);
        AtomicBoolean countTrue = new AtomicBoolean();
        countTrue.set(false);
        String topic = mTopicPrefix + "NotifyTest";
        // callback mock for subscriber
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                latchSubscriber.countDown();
                // assertion for subscriber
                if (latchSubscriber.getCount() == 0) {
                    assertTrue(methodCheck(resp, ResponseStatus.CONTENT));
                    assertTrue(payloadCheck(resp));

                    DeleteTopic(mMockDevice, topic);
                }
                return resp;
            }
        }).when(mockSubscriber).sendResponse(Mockito.anyObject());
        // create topic
        CreateTopic(mMockDevice, topic);
        // publish topic (publisher)
        PublishTopic(mMockDevice, topic);
        // subscribe topic (subscriber)
        SubscribeTopic(mockSubscriber, topic, Observe.SUBSCRIBE);
        await().atMost(2, SECONDS).untilFalse(countTrue);
        PublishTopic(mMockDevice, topic);
        // verity if subscriber receives two responses
        assertTrue(latchSubscriber.await(2L, SECONDS));
        verify(mockSubscriber, timeout(5000).times(2))
                .sendResponse(Mockito.anyObject());
    }

    @Test
    // test discover request
    public void testTopicDiscoverOnDefaultRequestReceived() throws Exception {
        System.out.println("\t--------------Topic Discover Test------------");
        String topic = mTopicPrefix + "DiscoverTest";
        String subTopic = topic + "sub";
        // create topic
        CreateTopic(mMockDevice, topic);
        // create sub topic
        CreateSubTopic(mMockDevice, topic, subTopic);
        // discover topic
        DiscoverTopic();
        // assertion 1: if the response status is "CONTENT"
        // assertion 2: if the response payload has "topiclist" property
        // and there is the topic created in this unit test in the array
        boolean methodCheck = methodCheck(mResponse, ResponseStatus.CONTENT);
        Cbor<HashMap<String, ArrayList<String>>> mArrayCbor = new Cbor<>();
        HashMap<String, ArrayList<String>> payloadData = mArrayCbor
                .parsePayloadFromCbor(mResponse.getPayload(), HashMap.class);
        ArrayList<String> topicList = payloadData.get("topiclist");
        System.out.println("\ttopicList : " + topicList);
        assertTrue(methodCheck);
        assertTrue(topicList.contains(MQ_BROKER_URI + "/" + topic));
        assertTrue(topicList
                .contains(MQ_BROKER_URI + "/" + topic + "/" + subTopic));
    }

    @Test
    // topic read request
    public void testTopicReadOnDefaultRequestReceived() throws Exception {
        System.out.println("\t--------------Topic Read Test------------");
        String topic = mTopicPrefix + "ReadTest";
        // create topic
        CreateTopic(mMockDevice, topic);
        // publish topic
        PublishTopic(mMockDevice, topic);
        // read topic
        ReadTopic(topic);
        // assertion1 : if the response status is "CONTENT"
        // assertion2 : if the response payload has data
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertTrue(payloadCheck(mResponse));
    }

    @Test(expected = NotFoundException.class)
    public void testNotCreatedTopicDeleteOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Not Created Topic Delete Test------------");
        String topic = mTopicPrefix + "NotCreatedTopicDeleteTest";

        DeleteTopic(mMockDevice, topic);
    }

    @Test(expected = NotFoundException.class)
    public void testNotCreatedSubtopicDeleteOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Not Created Subtopic Delete Test------------");
        String topic = mTopicPrefix + "Maintopic";

        CreateTopic(mMockDevice, topic);

        topic += "/" + "NotCreatedSubtopicTest";

        DeleteTopic(mMockDevice, topic);
    }

    @Test(expected = ForbiddenException.class)
    // duplicate topic creation
    public void testDuplicatedTopicCreateOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Duplicated Topic Creation Test------------");
        String topic = mTopicPrefix + "DuplicateTest";
        // create topic
        CreateTopic(mMockDevice, topic);
        // create topic again
        CreateTopic(mMockDevice, topic);
    }

    @Test(expected = ForbiddenException.class)
    // duplicate subtopic creation
    public void testDuplicatedSubtopicCreateOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Duplicated Subtopic Creation Test------------");

        String topic = mTopicPrefix + "DuplicateTest2";

        // create topic
        CreateTopic(mMockDevice, topic);

        // create subtopic
        topic += "/subtopic";
        CreateTopic(mMockDevice, topic);

        // create subtopic again
        CreateTopic(mMockDevice, topic);
    }

    @Test(expected = NotFoundException.class)
    // publish not created topic
    public void testNotCreatedTopicPublishOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Not Created Topic Publish Test------------");
        String topic = mTopicPrefix + "NotCreatedTopicTest";
        // publish not created topic
        PublishTopic(mMockDevice, topic);
    }

    @Test(expected = NotFoundException.class)
    // subscribe not created topic
    public void testNotCreatedTopicSubscribeOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Not Created Topic Subscribe Test------------");
        String topic = mTopicPrefix + "NotCreatedTopicSubscribeTest";
        SubscribeTopic(mMockDevice, topic, Observe.SUBSCRIBE);
    }

    @Test(expected = NotFoundException.class)
    // unsubscribe not created topic
    public void testNotCreatedTopicUnSubscribeOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Not Created Topic Unsubscribe Test------------");
        String topic = mTopicPrefix + "NotCreatedTopicUnSubscribeTest";
        SubscribeTopic(mMockDevice, topic, Observe.UNSUBSCRIBE);
    }

    @Test(expected = PreconditionFailedException.class)
    public void testTopicPublishWithoutMessage() throws Exception {
        System.out.println(
                "\t--------------Topic Publish Without Message Test------------");
        String topic = mTopicPrefix + "ForPubWithoutMessage";

        // topic creation
        CreateTopic(mMockDevice, topic);

        // topic publish without message
        String requestUri = MQ_BROKER_URI + "/" + topic;
        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                requestUri, null);

        mMqBrokerResource.onDefaultRequestReceived(mMockDevice, request);
    }

    @Test(expected = NotFoundException.class)
    // create subtopic under not created maintopic
    public void testSubTopicCreateUnderNotCreatedTopicOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Create Subtopic under Not Created Maintopic  ------------");
        String mainTopic = mTopicPrefix + "NotCreatedMain";
        String subTopic = mTopicPrefix + "NotCreatedSub";
        // create sub topic
        CreateSubTopic(mMockDevice, mainTopic, subTopic);
    }

    @Test
    // create topic which has 'core.light' rt
    public void testTopicCreationWithRtOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Topic Creation with RT Test------------");
        String topicName = mTopicPrefix + "RtTest";
        String rt = "rt=core.light";
        CreateTopicWithRt(mMockDevice, topicName, rt);
        // assertion: if the response status is "CREATED"
        assertTrue(methodCheck(mResponse, ResponseStatus.CREATED));
    }

    @Test
    // create topic which has 'core.light' rt
    public void testSubtopicCreationWithRtOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Subtopic Creation with RT Test------------");
        String topicName = mTopicPrefix + "RtTest2";
        String rt = "rt=core.light";

        // create main topic
        CreateTopicWithRt(mMockDevice, topicName, rt);

        // create sub topic
        topicName += "/subtopic";
        CreateTopicWithRt(mMockDevice, topicName, rt);

        assertTrue(methodCheck(mResponse, ResponseStatus.CREATED));
    }

    @Test
    // test discover request with rt
    public void testDiscoverTopicWithRtOnDefaultRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------Topic Discover with Rt Test------------");
        String topicName = mTopicPrefix + "DiscoverRtTest";
        String topicNameWithoutRt = mTopicPrefix + "DiscoverRtTestWithoutRt";
        String rt = "rt=core.light";
        // create topic with rt
        CreateTopicWithRt(mMockDevice, topicName, rt);
        // create topic
        CreateTopic(mMockDevice, topicNameWithoutRt);
        // discover topic
        DiscoverTopicWithRt(rt);
        // assertion 1: if the response status is "CONTENT"
        // assertion 2: if topic list contains the topic with rt
        // assertion 3: if topic list contains no topics which does not have the
        // rt
        Cbor<HashMap<String, ArrayList<String>>> mArrayCbor = new Cbor<>();
        HashMap<String, ArrayList<String>> payloadData = mArrayCbor
                .parsePayloadFromCbor(mResponse.getPayload(), HashMap.class);
        ArrayList<String> topicList = payloadData.get("topiclist");
        System.out.println("\ttopicList : " + topicList);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertTrue(topicList.contains(MQ_BROKER_URI + "/" + topicName));
        assertFalse(
                topicList.contains(MQ_BROKER_URI + "/" + topicNameWithoutRt));
    }

    private IRequest PublishTopicRequest(String topicName) {
        IRequest request = null;
        HashMap<String, Object> message = new HashMap<String, Object>();
        message.put("status", "on");
        message.put("brightness", 20);
        Cbor<HashMap<String, Object>> cbor = new Cbor<HashMap<String, Object>>();
        String uri = MQ_BROKER_URI + "/" + topicName;
        request = MessageBuilder.createRequest(RequestMethod.POST, uri, null,
                ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(message));
        return request;
    }

    private IRequest CreateTopicRequest(String topicName) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.PUT,
                MQ_BROKER_URI + "/" + topicName, null);
        return request;
    }

    private IRequest CreateSubTopicRequest(String topicName,
            String subTopicName) {
        IRequest request = null;
        String uri = MQ_BROKER_URI + "/" + topicName + "/" + subTopicName;
        request = MessageBuilder.createRequest(RequestMethod.PUT, uri, null);
        return request;
    }

    private IRequest CreateTopicWithRtRequest(String topicName, String type) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.PUT,
                MQ_BROKER_URI + "/" + topicName, type);
        return request;
    }

    private IRequest DeleteTopicRequest(String topicName) {
        IRequest request = null;
        String uri = MQ_BROKER_URI + "/" + topicName;
        request = MessageBuilder.createRequest(RequestMethod.DELETE, uri, null);
        return request;
    }

    private IRequest SubscribeTopicRequest(String topicName) {
        IRequest request = null;
        String uri = MQ_BROKER_URI + "/" + topicName;
        request = MessageBuilder.createRequest(RequestMethod.GET, uri, null);
        return request;
    }

    private IRequest DiscoverTopicRequest() {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET, MQ_BROKER_URI,
                null);
        return request;
    }

    private IRequest DiscoverTopicWithRtRequest(String rt) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET, MQ_BROKER_URI,
                rt);
        return request;
    }

    private CoapRequest ReadTopicRequest(String topicName) {
        IRequest request = null;
        String uri = MQ_BROKER_URI + "/" + topicName;
        request = MessageBuilder.createRequest(RequestMethod.GET, uri, null);
        CoapRequest mqRequest = (CoapRequest) request;
        return mqRequest;
    }

    private void CreateTopicWithRt(CoapDevice mockDevice, String topicName,
            String type) throws Exception {
        System.out.println("-----CreateTopic || topic : " + topicName);
        IRequest request = null;
        request = CreateTopicWithRtRequest(topicName, type);
        mMqBrokerResource.onDefaultRequestReceived(mockDevice, request);
    }

    private void CreateTopic(CoapDevice mockDevice, String topicName)
            throws Exception {
        System.out.println("-----CreateTopic || topic : " + topicName);
        IRequest request = null;
        request = CreateTopicRequest(topicName);
        mMqBrokerResource.onDefaultRequestReceived(mockDevice, request);
    }

    private void CreateSubTopic(CoapDevice mockDevice, String mainTopicName,
            String subTopicName) throws Exception {
        System.out.println("-----CreateSubTopic || topic : " + mainTopicName
                + " || subtopic : " + subTopicName);
        IRequest subTopicRequest = null;
        subTopicRequest = CreateSubTopicRequest(mainTopicName, subTopicName);
        mMqBrokerResource.onDefaultRequestReceived(mockDevice, subTopicRequest);
    }

    private void PublishTopic(CoapDevice mockDevice, String topicName)
            throws Exception {
        System.out.println("-----PublishTopic : " + topicName);
        IRequest request = null;
        request = PublishTopicRequest(topicName);
        mMqBrokerResource.onDefaultRequestReceived(mockDevice, request);
    }

    private void SubscribeTopic(CoapDevice mockSubscriber, String topicName,
            Observe observe) throws Exception {
        System.out.println("-----SubscribeTopic : " + topicName);
        IRequest requestSubscribe = null;
        requestSubscribe = SubscribeTopicRequest(topicName);
        CoapRequest mqRequest = (CoapRequest) requestSubscribe;
        mqRequest.setObserve(observe);
        mMqBrokerResource.onDefaultRequestReceived(mockSubscriber, mqRequest);
    }

    private void DeleteTopic(CoapDevice mockDevice, String topicName)
            throws Exception {
        System.out.println("-----DeleteTopic : " + topicName);
        IRequest requestToDelete = null;
        requestToDelete = DeleteTopicRequest(topicName);
        mMqBrokerResource.onDefaultRequestReceived(mockDevice, requestToDelete);
    }

    private void DeleteSubTopic(CoapDevice mockDevice, String topicName,
            String subTopicName) throws Exception {
        System.out.println("-----DeleteTopic : " + topicName);
        String deleteUri = topicName + "/" + subTopicName;
        IRequest requestToDelete = null;
        requestToDelete = DeleteTopicRequest(deleteUri);
        mMqBrokerResource.onDefaultRequestReceived(mockDevice, requestToDelete);
    }

    private void DiscoverTopic() throws Exception {
        System.out.println("-----DiscoverTopic : ");
        IRequest requestToDiscover = null;
        requestToDiscover = DiscoverTopicRequest();
        mMqBrokerResource.onDefaultRequestReceived(mMockDevice,
                requestToDiscover);
    }

    private void DiscoverTopicWithRt(String rt) throws Exception {
        System.out.println("-----DiscoverTopicWithRt : ");
        IRequest requestToDiscover = null;
        requestToDiscover = DiscoverTopicWithRtRequest(rt);
        mMqBrokerResource.onDefaultRequestReceived(mMockDevice,
                requestToDiscover);
    }

    private void ReadTopic(String topicName) throws Exception {
        System.out.println("-----ReadTopic : " + topicName);
        CoapRequest readRequest = null;
        readRequest = ReadTopicRequest(topicName);
        mMqBrokerResource.onDefaultRequestReceived(mMockDevice, readRequest);
    }

    private boolean payloadCheck(IResponse response) {
        Cbor<HashMap<String, Object>> mCbor = new Cbor<>();
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData != null && payloadData.containsKey("status")
                && payloadData.containsKey("brightness")) {
            return true;
        } else
            return false;
    }

    private boolean methodCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }
}