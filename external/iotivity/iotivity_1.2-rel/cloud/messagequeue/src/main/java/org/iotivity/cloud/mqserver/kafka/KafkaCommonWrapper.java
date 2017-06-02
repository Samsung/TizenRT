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

import kafka.admin.AdminUtils;
import kafka.utils.ZKStringSerializer$;
import kafka.utils.ZkUtils;

import org.I0Itec.zkclient.ZkClient;
import org.I0Itec.zkclient.ZkConnection;
import org.iotivity.cloud.mqserver.Constants;
import org.iotivity.cloud.util.Log;

/**
 *
 * This class provides a set of APIs to use Kafka APIs for basic operation.
 *
 */
public class KafkaCommonWrapper {

    private ZkClient mZkClient = null;
    private ZkUtils  mZkUtils  = null;

    public KafkaCommonWrapper(String zookeeperAddress, String brokerAddress) {

        mZkClient = new ZkClient(zookeeperAddress,
                Constants.KAFKA_SESSION_TIMEOUT,
                Constants.KAFKA_CONNECT_TIMEOUT, ZKStringSerializer$.MODULE$);

        mZkUtils = new ZkUtils(mZkClient, new ZkConnection(zookeeperAddress),
                false);
    }

    /**
     * API to create topic using Kafka utilities
     * 
     * @param topic
     *            name of topic to create
     * 
     * @return returns true if the topic is successfully created, otherwise
     *         false
     */
    public boolean createTopic(String topic) {

        Log.d("kafka createTopic - " + topic);

        topic = topic.replace('/', '.');

        try {
            // TODO check options - partitions, replicationFactor
            AdminUtils.createTopic(mZkUtils, topic, 1, 1, new Properties());
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    /**
     * API to delete topic using Kafka utilities
     * 
     * @param topic
     *            name of topic to delete
     * 
     * @return returns true if the topic is successfully deleted, otherwise
     *         false
     */
    public boolean deleteTopic(String topic) {

        Log.d("kafka deleteTopic - " + topic);

        topic = topic.replace('/', '.');

        try {
            AdminUtils.deleteTopic(mZkUtils, topic);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

}