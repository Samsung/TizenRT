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

#include "NSConsumer.h"
#include <cstring>
#include "NSProviderService.h"
#include "NSAcceptedConsumers.h"
#include "NSProviderInterface.h"
#include "NSException.h"
#include "NSConstants.h"
#include "oic_string.h"
#include "oic_malloc.h"

namespace OIC
{
    namespace Service
    {
        ::NSConsumer *NSConsumer::getNSConsumer()
        {
            ::NSConsumer *nsCon = new ::NSConsumer;
            OICStrcpy(nsCon->consumerId, NS_UTILS_UUID_STRING_SIZE, m_consumerId.c_str());
            return nsCon;
        }

        NSConsumer::NSConsumer(::NSConsumer *consumer)
        {
            if (consumer != nullptr)
            {
                m_consumerId.assign(consumer->consumerId, NS_UTILS_UUID_STRING_SIZE - 1);
            }
        }

        std::string NSConsumer::getConsumerId() const
        {
            return m_consumerId;
        }

        NSResult NSConsumer::acceptSubscription(bool accepted)
        {
            NS_LOG(DEBUG, "acceptSubscription - IN");
            if (!isValid())
            {
                throw NSException("Invalid Operation with stale reference of Consumer");
            }
            NSResult result = (NSResult) NSAcceptSubscription(getConsumerId().c_str(), accepted);
            NS_LOG(DEBUG, "acceptSubscription - OUT");
            return result;
        }

        NSResult NSConsumer::setTopic(const std::string &topicName)
        {
            NS_LOG(DEBUG, "setTopic - IN");
            if (!isValid())
            {
                throw NSException("Invalid Operation with stale reference of Consumer");
            }
            NSResult result = (NSResult) NSProviderSetConsumerTopic(getConsumerId().c_str(),
                              topicName.c_str());
            NS_LOG(DEBUG, "setTopic - OUT");
            return result;
        }

        NSResult NSConsumer::unsetTopic(const std::string &topicName)
        {
            NS_LOG(DEBUG, "unsetTopic - IN");
            if (!isValid())
            {
                throw NSException("Invalid Operation with stale reference of Consumer");
            }
            NSResult result = (NSResult) NSProviderUnsetConsumerTopic(getConsumerId().c_str(),
                              topicName.c_str());
            NS_LOG(DEBUG, "unsetTopic - OUT");
            return result;
        }

        std::shared_ptr<NSTopicsList> NSConsumer::getConsumerTopicList()
        {
            NS_LOG(DEBUG, "getConsumerTopicList - IN");
            if (!isValid())
            {
                throw NSException("Invalid Operation with stale reference of Consumer");
            }
            ::NSTopicLL *topics = NSProviderGetConsumerTopics(getConsumerId().c_str());

            std::shared_ptr<NSTopicsList> nsTopics = std::make_shared<NSTopicsList>(topics, false);
            NS_LOG(DEBUG, "getConsumerTopicList - OUT");
            return nsTopics;
        }

        bool NSConsumer::isValid() const
        {
            if (!NSProviderService::getInstance()->getAcceptedConsumers()->isAccepted(getConsumerId()))
            {
                NS_LOG(DEBUG, "Invalid Operation with stale reference of Consumer. Consumer ID doesnot exist");
                return false;
            }
            return true;
        }
    }
}
