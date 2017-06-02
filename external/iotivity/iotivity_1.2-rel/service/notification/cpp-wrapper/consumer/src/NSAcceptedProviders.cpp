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

#include "NSAcceptedProviders.h"
#include <cstring>
#include "NSCommon.h"
#include "NSConstants.h"

namespace OIC
{
    namespace Service
    {
        std::shared_ptr<NSProvider> NSAcceptedProviders::getProvider(const std::string &id)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(DEBUG, "AcceptedProviders::getProvider size  : %d", (int) m_providers.size());
            auto it = m_providers.find(id);
            if (it != m_providers.end() )
            {
                NS_LOG(DEBUG, "getProvider : Found Provider with given ID");
                return it->second;
            }
            NS_LOG(DEBUG, "getProvider : Not Found Provider with given ID");
            return NULL;
        }
        void NSAcceptedProviders::addProvider(std::shared_ptr<NSProvider> provider)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(INFO_PRIVATE, "AcceptedProviders::addProvider  Id : %s", provider->getProviderId().c_str());
            m_providers[provider->getProviderId()] = provider;
            return;
        }

        void NSAcceptedProviders::removeProvider(const std::string &id)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(INFO_PRIVATE, "AcceptedProviders::removeProvider  Id : %s", id.c_str());
            m_providers.erase(id);
            return;
        }

        bool NSAcceptedProviders::isAccepted(const std::string &id)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(INFO_PRIVATE, "AcceptedProviders::isAccepted  Id : %s", id.c_str());
            if ( m_providers.find(id) == m_providers.end() )
            {
                NS_LOG(DEBUG, "isAccepted : Not Found Provider with given ID");
                return false;
            }
            else
            {
                NS_LOG(DEBUG, "isAccepted : Found Provider with given ID");
                return true;
            }
        }

        int NSAcceptedProviders::size()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG_V(DEBUG, "AcceptedProviders::size  : %d", (int) m_providers.size());
            return m_providers.size();
        }

        void NSAcceptedProviders::removeProviders()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            NS_LOG(DEBUG, "AcceptedProviders::removeProviders ");
            m_providers.clear();
            return;
        }

        std::map<std::string, std::shared_ptr<NSProvider> > NSAcceptedProviders::getProviders()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_providers;
        }
    }
}
