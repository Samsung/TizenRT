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

#ifndef _NS_PROVIDER_SERVICE_SIMULATOR_H_
#define _NS_PROVIDER_SERVICE_SIMULATOR_H_

#include <iostream>
#include <memory>

#include "OCPlatform.h"
#include "OCApi.h"
#include "OCResourceResponse.h"

namespace
{
    enum class requestType
    {
        NS_NOTIFICATION,
        NS_MESSAGE,
        NS_SYNC,
        NS_TOPIC,
    };
    enum class messageType
    {
        NS_ALLOW = 1,
        NS_DENY,
        NS_TOPIC,
    };
}

class NSProviderSimulator
{
    public:
        enum class TopicAllowState
        {
            DENY,
            ALLOW
        };
        typedef std::pair<std::string, TopicAllowState> NS_TopicState;
        typedef std::map<std::string, TopicAllowState> NS_TopicStateList;

        typedef std::list<std::string> NS_TopicList;
    private:
        OCResourceHandle m_notificationHandle;
        OCResourceHandle m_messageHandle;
        OCResourceHandle m_syncHandle;
        OCResourceHandle m_topicHandle;
        OC::OCRepresentation m_syncRep;
        OC::OCRepresentation m_messageRep;
        int m_accepter;

        std::string m_notificationUri;
        std::string m_messageUri;
        std::string m_syncUri;
        std::string m_topicUri;
        NS_TopicList m_topicList;
        NS_TopicStateList m_allowedTopicList;

        OC::ObservationIds m_syncObservers;

    public:
        NSProviderSimulator()
            : m_notificationHandle(), m_messageHandle(), m_syncHandle(), m_topicHandle(),
              m_syncRep(), m_messageRep(), m_accepter(0),
              m_notificationUri(std::string("/notification")),
              m_messageUri(std::string("/message")),
              m_syncUri(std::string("/sync")),
              m_topicUri(std::string("/topic")),
              m_topicList(),
              m_allowedTopicList(),
              m_syncObservers()
        {

        };

        ~NSProviderSimulator() = default;

        NSProviderSimulator(const NSProviderSimulator &) = delete;
        NSProviderSimulator &operator = (const NSProviderSimulator &) = delete;

        NSProviderSimulator(NSProviderSimulator &&) = delete;
        NSProviderSimulator &operator = (NSProviderSimulator &&) = delete;

    private:
        std::shared_ptr<OC::OCResourceResponse> getResponse(
            std::shared_ptr< OC::OCResourceRequest > requests, requestType type)
        {
            auto response = std::make_shared<OC::OCResourceResponse>();
            response->setRequestHandle(requests->getRequestHandle());
            response->setResourceHandle(requests->getResourceHandle());

            int requestFlag = requests->getRequestHandlerFlag();
            if (requestFlag == OC::RequestHandlerFlag::RequestFlag)
            {
                std::string request = requests->getRequestType();

            response->setErrorCode(200);
                response->setResponseResult(OC_EH_OK);

                if (request == "GET")
                {
                    OC::OCRepresentation rep;

                    if (type == requestType::NS_NOTIFICATION)
                    {
                        std::string msgUri = m_notificationUri + m_messageUri;
                        std::string syncUri = m_notificationUri + m_syncUri;
                        std::string topicUri = m_notificationUri + m_topicUri;
                        std::string providerId = "123456789012345678901234567890123456";
                        rep.setValue("x.org.iotivity.ns.subcontrollability", m_accepter);
                        rep.setValue("x.org.iotivity.ns.messageuri", msgUri);
                        rep.setValue("x.org.iotivity.ns.syncuri", syncUri);
                        rep.setValue("x.org.iotivity.ns.topicuri", topicUri);
                        rep.setValue("x.org.iotivity.ns.providerid", providerId);
                    }
                    else if (type == requestType::NS_SYNC)
                    {
                        rep = m_syncRep;
                    }
                    else if (type == requestType::NS_MESSAGE)
                    {
                        rep = m_messageRep;
                    }
                    else if (type == requestType::NS_TOPIC)
                    {
                        if (m_allowedTopicList.empty())
                        {
                            std::for_each (m_topicList.begin(), m_topicList.end(),
                                           [this](const std::string & topic)
                            {
                                m_allowedTopicList.insert(
                                    std::make_pair<std::string, TopicAllowState>(
                                        std::string(topic), TopicAllowState::DENY));
                            }
                                          );
                        }

                        std::vector<OC::OCRepresentation> topicArr;

                        std::for_each (m_allowedTopicList.begin(), m_allowedTopicList.end(),
                                       [& topicArr](const NS_TopicState & topicState)
                        {
                            OC::OCRepresentation topic;
                            topic.setValue("x.org.iotivity.ns.topicname", topicState.first);
                            topic.setValue("x.org.iotivity.ns.topicstate", (int) topicState.second);
                            topicArr.push_back(topic);
                        }
                                      );

                        rep.setValue<std::vector<OC::OCRepresentation>>
                                ("x.org.iotivity.ns.topiclist", topicArr);
                    }
                    else
                    {
                        return NULL;
                    }

                    response->setResourceRepresentation(rep);
                    return response;
                }

                else if (request == "POST")
                {
                    if (type == requestType::NS_SYNC)
                    {
                        m_syncRep = requests->getResourceRepresentation();

                        std::cout << "Receive POST for Sync" << std::endl;
                        std::cout << "provider Id : " << m_syncRep.getValueToString("x.org.iotivity.ns.providerid") << std::endl;
                        std::cout << "Sync State : " << m_syncRep.getValueToString("x.org.iotivity.ns.state") << std::endl;

                        response->setResourceRepresentation(m_syncRep);

                        OC::OCPlatform::notifyAllObservers(m_syncHandle);

                        return response;
                    }
                    else if (type == requestType::NS_TOPIC)
                    {
                        auto receivePayload =
                            requests->getResourceRepresentation()
                            .getValue<std::vector<OC::OCRepresentation>>("x.org.iotivity.ns.topiclist");

                        std::for_each (receivePayload.begin(), receivePayload.end(),
                                       [this](const OC::OCRepresentation & rep)
                        {
                            auto tmp = m_allowedTopicList.find(rep.getValueToString("x.org.iotivity.ns.topicname"));
                            if (tmp != m_allowedTopicList.end())
                            {
                                tmp->second = (TopicAllowState) rep.getValue<int>("x.org.iotivity.ns.topicstate");
                            }
                        }
                                      );
                    }
                }
            }

            return NULL;
        }

        void setObserver(std::shared_ptr< OC::OCResourceRequest > requests, requestType type)
        {
            if (type == requestType::NS_SYNC)
            {
                OC::ObservationInfo observationInfo = requests->getObservationInfo();
                if (OC::ObserveAction::ObserveRegister == observationInfo.action)
                {
                    m_syncObservers.push_back(observationInfo.obsId);
                }
                else if (OC::ObserveAction::ObserveUnregister == observationInfo.action)
                {
                    m_syncObservers.erase(std::remove(
                                              m_syncObservers.begin(), m_syncObservers.end(),
                                              observationInfo.obsId), m_syncObservers.end());
                }
            }
            else if (type == requestType::NS_MESSAGE)
            {
                OC::OCRepresentation rep;
                std::string providerId = "123456789012345678901234567890123456";
                rep.setValue<int>("x.org.iotivity.ns.messageid", (int)messageType::NS_ALLOW);
                rep.setValue("x.org.iotivity.ns.providerid", providerId);

                auto response = std::make_shared<OC::OCResourceResponse>();
                response->setRequestHandle(requests->getRequestHandle());
                response->setResourceHandle(requests->getResourceHandle());
            response->setErrorCode(200);
                response->setResponseResult(OC_EH_OK);
                response->setResourceRepresentation(rep);

                OC::ObservationIds ids;
                ids.push_back(requests->getObservationInfo().obsId);

                OC::OCPlatform::notifyListOfObservers(m_messageHandle, ids, response);
            }
        }

        OCEntityHandlerResult entityHandler(
            std::shared_ptr< OC::OCResourceRequest > request, requestType type)
        {
            if (!request)
            {
                return OC_EH_ERROR;
            }

            std::cout << "Provider : Income request : " << request->getRequestHandlerFlag() << std::endl;
            if ((request->getRequestHandlerFlag() & OC::RequestHandlerFlag::ObserverFlag))
            {
                std::cout << "Provider : Income Observe : " << std::endl;
                setObserver(request, type);
                return OC_EH_OK;
            }

            auto pResponse = getResponse(request, type);
            if (pResponse == nullptr)
            {
                return OC_EH_ERROR;
            }

            try
            {
                OC::OCPlatform::sendResponse(pResponse);
            }
            catch (std::exception &e)
            {
                return OC_EH_ERROR;
            }

            return OC_EH_OK;
        }

    public:

        void setAccepter(int accepter)
        {
            m_accepter = accepter;
        }

        void notifyMessage()
        {
            std::cout << "Provider : notify~" << std::endl;
            OC::OCPlatform::notifyAllObservers(m_messageHandle);
        }

        void notifyMessage(const uint64_t &id, const std::string &title, const std::string &content)
        {
            setMessage(id, title, content);
            notifyMessage();
        }

        void sendRead(const uint64_t &id)
        {
            std::string providerId = "123456789012345678901234567890123456";
            m_syncRep.setValue<int>("x.org.iotivity.ns.messageid", id);
            m_syncRep.setValue("x.org.iotivity.ns.state", (int)1);
            m_syncRep.setValue("x.org.iotivity.ns.providerid", providerId);
            OC::OCPlatform::notifyAllObservers(m_syncHandle);
        }
        void sendDismiss(const uint64_t &id)
        {
            std::string providerId = "123456789012345678901234567890123456";
            m_syncRep.setValue<int>("x.org.iotivity.ns.messageid", id);
            m_syncRep.setValue("x.org.iotivity.ns.state", (int)2);
            m_syncRep.setValue("x.org.iotivity.ns.providerid", providerId);
            OC::OCPlatform::notifyAllObservers(m_syncHandle);
        }

        void setMessage(const uint64_t &id, const std::string &title, const std::string &content)
        {
            std::string providerId = "123456789012345678901234567890123456";
            m_messageRep.setValue<int>("x.org.iotivity.ns.messageid", id);
            m_messageRep.setValue("x.org.iotivity.ns.title", title);
            m_messageRep.setValue("x.org.iotivity.ns.contenttext", content);
            m_messageRep.setValue("x.org.iotivity.ns.providerid", providerId);
        }

        void setTopics(const NS_TopicList &topics)
        {
            bool isChanged = false;
            std::for_each (topics.begin(), topics.end(),
                           [this, & isChanged](const std::string & topic)
            {
                auto found = std::find(
                                 this->m_topicList.begin(), this->m_topicList.end(), topic);
                if (found == this->m_topicList.end())
                {
                    this->m_topicList.push_back(topic);
                    isChanged = true;
                }
            });

            if (isChanged)
            {
                this->notifyMessage((uint64_t)messageType::NS_TOPIC, "", "");
            }
        }

        NS_TopicList getTopics() const
        {
            return m_topicList;
        }

        void updateTopicState(const NS_TopicStateList &allowedTopics)
        {
            std::for_each (allowedTopics.begin(), allowedTopics.end(),
                           [this](const NS_TopicState & allowedTopic)
            {
                auto found = this->m_allowedTopicList.find(allowedTopic.first);
                if (found != this->m_allowedTopicList.end())
                {
                    found->second = allowedTopic.second;
                }
            });
        }

        NS_TopicStateList getTopicAllowState() const
        {
            return m_allowedTopicList;
        }

        void deleteNotificationResource()
        {
            OC::OCPlatform::unregisterResource(m_notificationHandle);
            OC::OCPlatform::unregisterResource(m_messageHandle);
            OC::OCPlatform::unregisterResource(m_syncHandle);
            OC::OCPlatform::unregisterResource(m_topicHandle);
            m_allowedTopicList.clear();
            m_topicList.clear();
        }

        void createNotificationResource()
        {
            createNotificationResource(m_notificationUri);
        }

        void createNotificationResource(const std::string &uri)
        {
            if (m_notificationUri != uri)
            {
                m_notificationUri = uri;
            }

            OC::OCPlatform::startPresence(30);

            std::string notificationUri = m_notificationUri;
            std::string resourceTypeName = "x.org.iotivity.notification.topic";
            std::string resourceInterface = OC::DEFAULT_INTERFACE;

            uint8_t resourceProperty = OC_OBSERVABLE;
            std::string childUri = uri + m_topicUri;
            try
            {
                OC::OCPlatform::registerResource(
                    m_topicHandle, childUri,
                    resourceTypeName, resourceInterface,
                    std::bind(& NSProviderSimulator::entityHandler, this,
                              std::placeholders::_1, requestType::NS_TOPIC),
                    resourceProperty);
            }
            catch (std::exception &e)
            {
                std::cout << e.what() << std::endl;
            }

            //resourceProperty |= OC_OBSERVABLE;
            resourceTypeName = "x.org.iotivity.notification.message";
            childUri = uri + m_messageUri;
            try
            {
                OC::OCPlatform::registerResource(
                    m_messageHandle, childUri,
                    resourceTypeName, resourceInterface,
                    std::bind(& NSProviderSimulator::entityHandler, this,
                              std::placeholders::_1, requestType::NS_MESSAGE),
                    resourceProperty);
            }
            catch (std::exception &e)
            {
                std::cout << e.what() << std::endl;
            }

            resourceTypeName = "x.org.iotivity.notification.sync";
            childUri = uri + m_syncUri;
            try
            {
                OC::OCPlatform::registerResource(
                    m_syncHandle, childUri,
                    resourceTypeName, resourceInterface,
                    std::bind(& NSProviderSimulator::entityHandler, this,
                              std::placeholders::_1, requestType::NS_SYNC),
                    resourceProperty);
            }
            catch (std::exception &e)
            {
                std::cout << e.what() << std::endl;
            }

            resourceProperty |= OC_DISCOVERABLE;
            resourceTypeName = "x.org.iotivity.notification";
            try
            {
                OC::OCPlatform::registerResource(
                    m_notificationHandle, notificationUri,
                    resourceTypeName, resourceInterface,
                    std::bind(& NSProviderSimulator::entityHandler, this,
                              std::placeholders::_1, requestType::NS_NOTIFICATION),
                    resourceProperty);
            }
            catch (std::exception &e)
            {
                std::cout << e.what() << std::endl;
            }
        }
};

#endif /* _NS_PROVIDER_SERVICE_SIMULATOR_H_ */
