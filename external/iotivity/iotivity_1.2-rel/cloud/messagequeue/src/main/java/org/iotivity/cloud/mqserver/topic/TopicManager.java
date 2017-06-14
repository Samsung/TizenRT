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
package org.iotivity.cloud.mqserver.topic;

import java.util.ArrayList;

import org.iotivity.cloud.mqserver.kafka.KafkaCommonWrapper;

/**
 *
 * This class provides a set of APIs to manage topics in MessageQueue Broker
 *
 */
public class TopicManager {

    private ArrayList<Topic>   mTopics              = new ArrayList<>();

    // for Kafka
    String                     mKafkaZookeeper      = null;
    String                     mKafkaBroker         = null;

    private KafkaCommonWrapper mKafkaCommonOperator = null;

    /**
     * API to create topic
     * 
     * @param topic
     *            topic to create
     * 
     * @return returns true if the topic successfully created, otherwise false
     */
    public boolean createTopic(Topic topic) {

        if (mKafkaCommonOperator.createTopic(topic.getName()) == false) {
            return false;
        }

        synchronized (mTopics) {
            mTopics.add(topic);
        }

        return true;
    }

    /**
     * API to remove topic
     * 
     * @param topic
     *            topic to remove
     * 
     * @return returns true if the topic successfully removed, otherwise false
     */
    public boolean removeTopic(Topic topic) {

        return removeTopics(topic.getName());
    }

    /**
     * API to get list of topics
     * 
     * @return returns list of topic uris
     */
    public ArrayList<String> getTopicList() {

        ArrayList<String> topicList = new ArrayList<>();

        synchronized (mTopics) {
            for (Topic topic : mTopics) {
                topicList.add(topic.getName());
            }
        }

        return topicList;
    }

    /**
     * API to get list of topics with specific topic type
     * 
     * @param type
     *            topic type
     * 
     * @return returns list of topic uris searched with the topic type
     */
    public ArrayList<String> getTopicListByType(String type) {

        ArrayList<String> topicList = new ArrayList<>();

        synchronized (mTopics) {

            for (Topic topic : mTopics) {
                if (type.equals(topic.getType())) {
                    topicList.add(topic.getName());
                }
            }
        }

        return topicList;
    }

    /**
     * API to get topic with topic name
     * 
     * @param topicName
     *            topic name to search
     * 
     * @return topic searched with the topic name
     */
    public Topic getTopic(String topicName) {

        Topic foundTopic = null;

        synchronized (mTopics) {

            for (Topic topic : mTopics) {
                if (topicName.equals(topic.getName())) {
                    foundTopic = topic;
                    break;
                }
            }
        }

        return foundTopic;
    }

    /**
     * API to set Kafka zookeeper and broker information
     * 
     * @param zookeeper
     *            address and port number of the zookeeper
     * @param broker
     *            address and port number of the Kafka broker
     */
    public void setKafkaInformation(String zookeeper, String broker) {
        mKafkaZookeeper = zookeeper;
        mKafkaBroker = broker;

        mKafkaCommonOperator = new KafkaCommonWrapper(zookeeper, broker);
    }

    /**
     * API to get zookeeper information
     * 
     * @return address and port number of the zookeeper
     */
    public String getKafkaZookeeper() {
        return mKafkaZookeeper;
    }

    /**
     * API to get Kafka broker information
     * 
     * @return address and port number of the Kafka broker
     */
    public String getKafkaBroker() {
        return mKafkaBroker;
    }

    private boolean removeTopics(String topicName) {

        synchronized (mTopics) {

            for (Topic topic : new ArrayList<>(mTopics)) {

                if (topic.getName().startsWith(topicName)) {

                    if (mKafkaCommonOperator.deleteTopic(topic.getName()) == false) {
                        return false;
                    }

                    mTopics.remove(topic);
                }
            }
        }

        return true;
    }

}