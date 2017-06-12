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
package org.iotivity.service.ns.provider;

import org.iotivity.service.ns.common.*;
import java.util.Vector;

/**
 *
 * This class provides implementation of Notification Consumer object.
 *
 */
public class Consumer {

    private String mConsumerId = null;

    /**
     * Constructor of Consumer
     */
    public Consumer(final String consumerId) {
        mConsumerId = consumerId;
    }

    /**
     * API for getting consumerId
     *
     * @return ConsumerId as string
     */
    public String getConsumerId() {
        return mConsumerId;
    }

    /**
     * API for accepting Subscription request. This function is valid only when
     * subControllability is set true.
     *
     * @param accepted
     *            boolean variable representing Subscription response as
     *            TRUE/FALSE.
     *
     * @throws NSException failure accepting subscription request
     */
    public void acceptSubscription(boolean accepted) throws NSException {
        nativeAcceptSubscription(mConsumerId, accepted);
    }

    /**
     * Select a topic for a consumer
     *
     * @param topicName
     *            Topic name to select
     *
     * @throws NSException failure selecting a topic
     */
    public void setTopic(String topicName) throws NSException {
        nativeSetConsumerTopic(mConsumerId, topicName);
    }

    /**
     * Unselect a topic for a consumer
     *
     * @param topicName
     *            Topic name to Unselect
     *
     * @throws NSException failure unselecting topic
     */
    public void unsetTopic(String topicName) throws NSException {
        nativeUnsetConsumerTopic(mConsumerId, topicName);
    }

    /**
     * Request topic list with selection state for the consumer
     *
     * @return Topic list
     */
    public TopicsList getConsumerTopicList() throws NSException {
        return nativeGetConsumerTopicList(mConsumerId);
    }

    private native void nativeAcceptSubscription(String consumerId,
            boolean accepted) throws NSException;

    private native void nativeSetConsumerTopic(String consumerId,
            String topicName) throws NSException;

    private native void nativeUnsetConsumerTopic(String consumerId,
            String topicName) throws NSException;

    private native TopicsList nativeGetConsumerTopicList(String consumerId)
            throws NSException;
}
