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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.ForbiddenException;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.base.exception.ServerException.NotFoundException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.mqserver.Constants;
import org.iotivity.cloud.mqserver.MessageQueueUtils;
import org.iotivity.cloud.mqserver.topic.Topic;
import org.iotivity.cloud.mqserver.topic.TopicManager;

/**
 * 
 * This class provides a set of APIs to handle requests to MessageQueue Broker.
 *
 */
public class MQBrokerResource extends Resource {

    private TopicManager mTopicManager = new TopicManager();

    public MQBrokerResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.MQ_BROKER_URI));
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
        mTopicManager.setKafkaInformation(zookeeper, broker);
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {

        IResponse response = null;

        switch (request.getMethod()) {
            case GET:
                response = handleGetRequest(srcDevice, request);
                break;
            case PUT:
                response = handlePutRequest(request);
                break;
            case POST:
                response = handlePostRequest(request);
                break;
            case DELETE:
                response = handleDeleteRequest(request);
                break;
        }

        srcDevice.sendResponse(response);
    }

    // DISCOVER, READ, SUBSCRIBE, UNSUBSCRIBE
    private IResponse handleGetRequest(Device srcDevice, IRequest request) {

        // DISCOVER
        if (request.getUriPathSegments().size() == getUriPathSegments()
                .size()) {
            return discoverTopic(request);
        }

        switch (request.getObserve()) {
            case NOTHING:
                return readMessage(request);

            case SUBSCRIBE:
                return subscribeTopic(srcDevice, request);

            case UNSUBSCRIBE:
                return unsubscribeTopic(request);

            default:
                throw new BadRequestException("observe type not supported");
        }
    }

    // CREATE topic
    private IResponse handlePutRequest(IRequest request) {

        if (request.getUriPathSegments().size() == getUriPathSegments()
                .size()) {

            throw new BadRequestException(
                    "topic name is not included in request uri");
        }

        return createTopic(request);
    }

    // PUBLISH
    private IResponse handlePostRequest(IRequest request) {

        return publishMessage(request);
    }

    // REMOVE topic
    private IResponse handleDeleteRequest(IRequest request) {

        return removeTopic(request);
    }

    private IResponse createTopic(IRequest request) {

        // main topic creation request
        if (request.getUriPathSegments().size() == getUriPathSegments().size()
                + 1) {
            return createMainTopic(request);
        }

        // subtopic creation request
        String uriPath = request.getUriPath();

        if (uriPath == null) {
            throw new BadRequestException("uriPath is not invalid");
        }

        uriPath = uriPath.substring(0, uriPath.lastIndexOf('/'));
        Topic targetTopic = mTopicManager.getTopic(uriPath);

        if (targetTopic == null) {
            throw new NotFoundException("main topic doesn't exist");
        }

        return targetTopic.handleCreateSubtopic(request);
    }

    private IResponse removeTopic(IRequest request) {

        // main topic removal request
        if (request.getUriPathSegments().size() - 1 == getUriPathSegments()
                .size()) {
            return removeMainTopic(request);
        }

        // subtopic removal request
        String uriPath = request.getUriPath();

        if (uriPath == null) {
            throw new BadRequestException("uriPath is not invalid");
        }

        String parentName = uriPath.substring(0, uriPath.lastIndexOf('/'));
        String targetName = request.getUriPathSegments()
                .get(request.getUriPathSegments().size() - 1);

        Topic parentTopic = mTopicManager.getTopic(parentName);

        if (parentTopic == null) {
            throw new NotFoundException("main topic doesn't exist");
        }

        return parentTopic.handleRemoveSubtopic(request, targetName);
    }

    private IResponse subscribeTopic(Device srcDevice, IRequest request) {

        Topic topic = mTopicManager.getTopic(request.getUriPath());

        if (topic == null) {
            throw new NotFoundException("topic doesn't exist");
        }

        return topic.handleSubscribeTopic(srcDevice, request);
    }

    private IResponse unsubscribeTopic(IRequest request) {

        Topic topic = mTopicManager.getTopic(request.getUriPath());

        if (topic == null) {
            throw new NotFoundException("topic doesn't exist");
        }

        return topic.handleUnsubscribeTopic(request);
    }

    private IResponse publishMessage(IRequest request) {

        Topic topic = mTopicManager.getTopic(request.getUriPath());

        if (topic == null) {
            throw new NotFoundException("topic doesn't exist");
        }

        return topic.handlePublishMessage(request);
    }

    private IResponse readMessage(IRequest request) {

        Topic topic = mTopicManager.getTopic(request.getUriPath());

        if (topic == null) {
            throw new NotFoundException("topic doesn't exist");
        }

        return topic.handleReadMessage(request);
    }

    private IResponse discoverTopic(IRequest request) {

        ArrayList<String> topicList = null;

        HashMap<String, List<String>> query = request.getUriQueryMap();

        if (query != null && query.containsKey("rt")) {

            topicList = mTopicManager
                    .getTopicListByType(query.get("rt").get(0));
        } else {

            topicList = mTopicManager.getTopicList();
        }

        return MessageBuilder.createResponse(request, ResponseStatus.CONTENT,
                ContentFormat.APPLICATION_CBOR, MessageQueueUtils
                        .buildPayload(Constants.MQ_TOPICLIST, topicList));
    }

    private IResponse createMainTopic(IRequest request) {

        String topicName = request.getUriPathSegments()
                .get(request.getUriPathSegments().size() - 1);

        String type = new String();

        if (request.getUriQueryMap() != null) {
            type = request.getUriQueryMap().get("rt").get(0);
        }

        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("/");

        for (String uri : getUriPathSegments()) {
            stringBuilder.append(uri);
            stringBuilder.append("/");
        }

        stringBuilder.append(topicName);

        topicName = stringBuilder.toString();

        if (mTopicManager.getTopic(topicName) != null) {
            throw new ForbiddenException("topic already exist");
        }

        Topic newTopic = new Topic(topicName, type, mTopicManager);

        if (mTopicManager.createTopic(newTopic) == false) {
            throw new InternalServerErrorException("create topic falied");
        }

        IResponse response = MessageBuilder.createResponse(request,
                ResponseStatus.CREATED);
        String uriPath = request.getUriPath();
        if (uriPath == null) {
            throw new InternalServerErrorException(
                    "uriPath is null in handleCreateSubtopic");
        }
        response.setLocationPath(uriPath);
        return response;
    }

    private IResponse removeMainTopic(IRequest request) {

        String topicName = request.getUriPath();

        Topic targetTopic = mTopicManager.getTopic(topicName);

        if (targetTopic == null) {
            throw new NotFoundException("topic doesn't exist");
        }

        targetTopic.cleanup();

        if (mTopicManager.removeTopic(targetTopic) == false) {
            throw new InternalServerErrorException("remove topic failed");
        }

        return MessageBuilder.createResponse(request, ResponseStatus.DELETED);
    }
}