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
package org.iotivity.cloud.mqserver.kafka;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.I0Itec.zkclient.ZkClient;
import org.I0Itec.zkclient.ZkConnection;
import org.iotivity.cloud.mqserver.Constants;
import org.iotivity.cloud.mqserver.topic.Topic;
import org.iotivity.cloud.util.Log;

import kafka.admin.AdminUtils;
import kafka.api.FetchRequest;
import kafka.api.FetchRequestBuilder;
import kafka.api.PartitionOffsetRequestInfo;
import kafka.common.TopicAndPartition;
import kafka.consumer.Consumer;
import kafka.consumer.ConsumerConfig;
import kafka.consumer.KafkaStream;
import kafka.javaapi.FetchResponse;
import kafka.javaapi.OffsetResponse;
import kafka.javaapi.consumer.ConsumerConnector;
import kafka.javaapi.consumer.SimpleConsumer;
import kafka.javaapi.message.ByteBufferMessageSet;
import kafka.message.MessageAndMetadata;
import kafka.message.MessageAndOffset;
import kafka.utils.ZKStringSerializer$;
import kafka.utils.ZkUtils;

/**
 *
 * This class provides a set of APIs to use Kafka consumer APIs for receiving
 * messages.
 *
 */
public class KafkaConsumerWrapper {

    private String            mTopicName         = null;

    private String            mZookeeper         = null;
    private String            mBroker            = null;

    private ZkClient          mZkClient          = null;
    private ZkUtils           mZkUtils           = null;

    private ConsumerConnector mConsumerConnector = null;
    private ExecutorService   mConsumerExecutor  = null;

    private Topic             mInternalConsumer  = null;

    private boolean           mConsumerStarted   = false;

    public KafkaConsumerWrapper(String zookeeperAddress, String brokerAddress,
            Topic consumer) {

        mTopicName = consumer.getName().replace("/", ".");

        mZookeeper = zookeeperAddress;
        mBroker = brokerAddress;
        mInternalConsumer = consumer;

        mZkClient = new ZkClient(zookeeperAddress,
                Constants.KAFKA_SESSION_TIMEOUT,
                Constants.KAFKA_CONNECT_TIMEOUT, ZKStringSerializer$.MODULE$);

        mZkUtils = new ZkUtils(mZkClient, new ZkConnection(zookeeperAddress),
                false);
    }

    /**
     * API to check if Kafka consumer is started
     * 
     * @return returns true if Kafka consumer started, otherwise false
     */
    public boolean consumerStarted() {
        return mConsumerStarted;
    }

    /**
     * API to subscribe Kafka topic to receive messages
     * 
     * @return returns true if the topic is successfully subscribed, otherwise
     *         false
     */
    public boolean subscribeTopic() {

        Log.d("kafka subscribeTopic - " + mTopicName);

        if (mConsumerStarted == true) {
            return true;
        }

        // remove consumer group info if already exist
        List<String> subscribers = mZkClient
                .getChildren(ZkUtils.ConsumersPath());

        if (subscribers.contains(mTopicName)) {
            AdminUtils.deleteConsumerGroupInZK(mZkUtils, mTopicName);
        }

        ConsumerConfig consumerConfig = new ConsumerConfig(
                buildPropertiesForSubscribe());

        mConsumerConnector = Consumer
                .createJavaConsumerConnector(consumerConfig);

        Map<String, Integer> topicCountMap = new HashMap<>();
        topicCountMap.put(mTopicName, Constants.KAFKA_CONSUMMER_THREADS);

        Map<String, List<KafkaStream<byte[], byte[]>>> consumerMap = mConsumerConnector
                .createMessageStreams(topicCountMap);

        List<KafkaStream<byte[], byte[]>> streams = consumerMap.get(mTopicName);

        mConsumerExecutor = Executors
                .newFixedThreadPool(Constants.KAFKA_CONSUMMER_THREADS);

        for (final KafkaStream<byte[], byte[]> stream : streams) {

            Log.d("kafka subscribe complete");

            mConsumerExecutor.execute(new Runnable() {

                public void run() {

                    for (final MessageAndMetadata<byte[], byte[]> messageAndMetadata : stream) {

                        mInternalConsumer.onMessagePublished(
                                messageAndMetadata.message());
                    }
                }
            });
        }

        mConsumerStarted = true;

        return true;
    }

    /**
     * API to unsubscribe Kafka topic to stop receiving messages
     * 
     * @return returns true if the topic is successfully unsubscribed, otherwise
     *         false
     */
    public boolean unsubscribeTopic() {

        Log.d("kafka unsubscribeTopic - " + mTopicName);

        // remove consumer group info in zookeeper
        List<String> subscribers = mZkClient
                .getChildren(ZkUtils.ConsumersPath());

        if (subscribers.contains(mTopicName)) {
            AdminUtils.deleteConsumerGroupInZK(mZkUtils, mTopicName);
        }

        mConsumerExecutor.shutdown();
        mConsumerConnector.shutdown();

        mConsumerStarted = false;

        return true;
    }

    /**
     * API to close Kafka consumer connection
     */
    public void closeConnection() {

        if (mConsumerStarted == true) {
            unsubscribeTopic();
        }

        mZkUtils.close();
        mZkClient.close();
    }

    /**
     * API to get all messages from Kafka topic
     * 
     * @return returns the list of messages published to the topic
     */
    public ArrayList<byte[]> getMessages() {

        Log.d("kafka get all messages - " + mTopicName);

        String brokerHost = mBroker.substring(0, mBroker.indexOf(':'));
        int brokerPort = Integer
                .parseInt(mBroker.substring(mBroker.indexOf(':') + 1));

        Log.d("host " + brokerHost + ", port " + brokerPort);

        // TODO check options - Timeout: Int, bufferSize: Int
        SimpleConsumer simpleConsumer = new SimpleConsumer(brokerHost,
                brokerPort, 100000, 64 * 1024, mTopicName);

        long lastOffset = getLastOffset(simpleConsumer, mTopicName, 0,
                kafka.api.OffsetRequest.EarliestTime(), mTopicName);

        // TODO check option - fetch size
        FetchRequest req = new FetchRequestBuilder().clientId(mTopicName)
                .addFetch(mTopicName, 0, lastOffset, 100000).build();

        FetchResponse fetchResponse = simpleConsumer.fetch(req);

        if (fetchResponse == null || fetchResponse.hasError()) {

            Log.e("Error fetching data from the Broker");
            return null;
        }

        ArrayList<byte[]> initialData = new ArrayList<>();

        ByteBufferMessageSet messageSet = fetchResponse.messageSet(mTopicName,
                0);

        if (messageSet != null) {
            for (MessageAndOffset messageAndOffset : messageSet) {

                long currentOffset = messageAndOffset.offset();
                if (currentOffset < lastOffset) {
                    Log.e("Found an old offset: " + currentOffset
                            + " Expecting: " + lastOffset);
                    continue;
                }

                lastOffset = messageAndOffset.nextOffset();
                ByteBuffer payload = messageAndOffset.message().payload();

                if (payload != null) {
                    byte[] bytes = new byte[payload.limit()];
                    payload.get(bytes);

                    initialData.add(bytes);
                }
            }
        }

        simpleConsumer.close();

        Log.d("kafka get all messages complete");

        return initialData;
    }

    private Properties buildPropertiesForSubscribe() {

        // TODO check property settings
        Properties props = new Properties();

        props.put("group.id", mTopicName);
        props.put("zookeeper.connect", mZookeeper);
        props.put("enable.auto.commit", "true");
        props.put("auto.commit.interval.ms", Constants.KAFKA_COMMIT_INTERVAL);

        return props;
    }

    private long getLastOffset(SimpleConsumer consumer, String topic,
            int partition, long whichTime, String clientName) {

        TopicAndPartition topicAndPartition = new TopicAndPartition(topic,
                partition);

        Map<TopicAndPartition, PartitionOffsetRequestInfo> requestInfo = new HashMap<>();
        requestInfo.put(topicAndPartition,
                new PartitionOffsetRequestInfo(whichTime, 1));

        kafka.javaapi.OffsetRequest request = new kafka.javaapi.OffsetRequest(
                requestInfo, kafka.api.OffsetRequest.CurrentVersion(),
                clientName);

        OffsetResponse response = consumer.getOffsetsBefore(request);

        if (response == null || response.hasError()) {
            Log.e("Error fetching data Offset Data the Broker");
            return 0;
        }

        long[] offsets = response.offsets(topic, partition);
        return offsets[0];
    }

}