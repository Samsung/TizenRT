//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "ObserveCache.h"
#include "RCSException.h"

namespace OIC
{
    namespace Service
    {
        ObserveCache::ObserveCache(std::weak_ptr<PrimitiveResource> pResource)
        : m_wpResource(pResource), m_attributes(), m_state(CACHE_STATE::NONE),
          m_reportCB(), m_isStart(false), m_id(0)
        {
        }

        void ObserveCache::startCache(DataCacheCB func)
        {
            if (m_isStart)
            {
                throw RCSBadRequestException{ "Caching already started." };
            }

            m_reportCB = std::move(func);

            auto resource = m_wpResource.lock();
            if (resource == nullptr)
            {
                m_reportCB = nullptr;
                throw RCSBadRequestException{ "Resource was not initialized." };
            }

            if (resource->isObservable())
            {
                resource->requestObserve(
                        std::bind(&ObserveCache::verifyObserveCB,
                                  std::placeholders::_1, std::placeholders::_2,
                                  std::placeholders::_3, std::placeholders::_4,
                                  shared_from_this()));
            }
            else
            {
                throw RCSBadRequestException{ "Can't observe, Never updated data." };
            }

            m_isStart = true;
            m_state = CACHE_STATE::READY_YET;
        }

        void ObserveCache::stopCache()
        {
            auto resource = m_wpResource.lock();
            if (resource != nullptr)
            {
                resource->cancelObserve();
            }

            m_reportCB = nullptr;
            m_state = CACHE_STATE::NONE;

            m_isStart = false;

        }

        CACHE_STATE ObserveCache::getCacheState() const
        {
            return m_state;
        }

        RCSResourceAttributes ObserveCache::getCachedData() const
        {
            return m_attributes;
        }

        bool ObserveCache::isCachedData() const
        {
            return !m_attributes.empty();
        }

        bool ObserveCache::isStartCache() const
        {
            return m_isStart;
        }

        void ObserveCache::onObserve(const HeaderOptions &,
                       const ResponseStatement & rep, int _result, unsigned int)
        {
            if (!convertOCResultToSuccess((OCStackResult)_result))
            {
                return;
            }

            m_state = CACHE_STATE::READY;

            if (m_attributes == rep.getAttributes())
            {
                return ;
            }

            m_attributes = rep.getAttributes();
            if (m_reportCB)
            {
                m_reportCB(m_wpResource.lock(), m_attributes);
            }
        }

        void ObserveCache::verifyObserveCB(const HeaderOptions &_hos,
                                    const ResponseStatement &_rep, int _result,
                                    unsigned int _seq, weakDataCache wPtr)
        {
            auto ptr = wPtr.lock();
            if (ptr)
            {
                ptr->onObserve(_hos, _rep, _result, _seq);
            }
        }

        bool ObserveCache::convertOCResultToSuccess(OCStackResult ret)
        {
            switch (ret)
            {
                case OC_STACK_OK:
                case OC_STACK_RESOURCE_CREATED:
                case OC_STACK_RESOURCE_DELETED:
                case OC_STACK_PRESENCE_STOPPED:
                case OC_STACK_CONTINUE:
                case OC_STACK_RESOURCE_CHANGED:
                    return true;
                default:
                    return false;
            }
        }
    }
}
