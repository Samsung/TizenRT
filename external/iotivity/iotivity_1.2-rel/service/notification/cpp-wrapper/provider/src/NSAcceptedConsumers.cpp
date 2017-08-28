//******************************************************************
//
// Copyright 2017 Samsung Electronics All Rights Reserved.
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


#include "NSProviderService.h"
#include <cstring>
#include "NSCommon.h"
#include "NSConstants.h"
#include "NSAcceptedConsumers.h"

namespace OIC
{
    namespace Service
    {
        std::shared_ptr<NSConsumer> NSAcceptedConsumers::getConsumer(const std::string &id)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(DEBUG, "AcceptedConsumers::getConsumer size  : %d", (int) m_consumers.size());
            auto it = m_consumers.find(id);
            if (it != m_consumers.end() )
            {
                NS_LOG(DEBUG, "getConsumer : Found Consumer with given ID");
                return it->second;
            }
            NS_LOG(DEBUG, "getConsumer : Not Found Consumer with given ID");
            return NULL;
        }
        void NSAcceptedConsumers::addConsumer(std::shared_ptr<NSConsumer> consumer)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(INFO_PRIVATE, "AcceptedConsumers::addConsumer  Id : %s", consumer->getConsumerId().c_str());
            m_consumers[consumer->getConsumerId()] = consumer;
            return;
        }

        void NSAcceptedConsumers::removeConsumer(const std::string &id)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(INFO_PRIVATE, "AcceptedConsumers::removeConsumer  Id : %s", id.c_str());
            m_consumers.erase(id);
            return;
        }

        bool NSAcceptedConsumers::isAccepted(const std::string &id)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(INFO_PRIVATE, "AcceptedConsumers::isAccepted  Id : %s", id.c_str());
            if ( m_consumers.find(id) == m_consumers.end() )
            {
                NS_LOG(DEBUG, "isAccepted : Not Found Consumer with given ID");
                return false;
            }
            else
            {
                NS_LOG(DEBUG, "isAccepted : Found Consumer with given ID");
                return true;
            }
        }

        int NSAcceptedConsumers::size()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(DEBUG, "AcceptedConsumers::size  : %d", (int) m_consumers.size());
            return m_consumers.size();
        }

        void NSAcceptedConsumers::removeConsumers()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG(DEBUG, "AcceptedConsumers::removeConsumers ");
            m_consumers.clear();
            return;
        }

        std::map<std::string, std::shared_ptr<NSConsumer> > NSAcceptedConsumers::getConsumers()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_consumers;
        }
    }
}
