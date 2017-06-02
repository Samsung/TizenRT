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

import java.util.Properties;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.iotivity.cloud.util.Log;

/**
 *
 * This class provides a set of APIs to use Kafka producer APIs for publishing
 * messages.
 *
 */
public class KafkaProducerWrapper {

    private String                   mTopicName = null;
    private String                   mBroker    = null;

    private Producer<byte[], byte[]> mProducer  = null;

    public KafkaProducerWrapper(String brokerAddress, String topic) {

        mTopicName = topic.replace("/", ".");

        mBroker = brokerAddress;

        mProducer = new KafkaProducer<>(buildPropertiesForPublish());
    }

    /**
     * API to publish message to Kafka topic
     * 
     * @param message
     *            message to publish
     * 
     * @return returns true if the message is successfully published, otherwise
     *         false
     */
    public boolean publishMessage(byte[] message) {

        Log.d("kafka publishMessage - " + mTopicName);

        ProducerRecord<byte[], byte[]> record = new ProducerRecord<>(
                mTopicName, message);

        mProducer.send(record);
        mProducer.flush();

        return true;
    }

    /**
     * API to close Kafka producer connection
     */
    public void closeConnection() {

        mProducer.close();
    }

    private Properties buildPropertiesForPublish() {

        // TODO check property settings
        Properties props = new Properties();

        props.put("bootstrap.servers", mBroker);
        props.put("acks", "all");
        props.put("retries", 0);
        props.put("batch.size", 16384);
        props.put("linger.ms", 1);
        props.put("buffer.memory", 33554432);
        props.put("key.serializer",
                "org.apache.kafka.common.serialization.ByteArraySerializer");
        props.put("value.serializer",
                "org.apache.kafka.common.serialization.ByteArraySerializer");

        return props;
    }

}