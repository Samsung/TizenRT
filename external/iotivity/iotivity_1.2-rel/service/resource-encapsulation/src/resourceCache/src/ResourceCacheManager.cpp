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

#include "ResourceCacheManager.h"
#include "RCSException.h"
#include "ocrandom.h"

namespace OIC
{
    namespace Service
    {
        ResourceCacheManager *ResourceCacheManager::s_instance = NULL;
        std::mutex ResourceCacheManager::s_mutexForCreation;
        std::mutex ResourceCacheManager::s_mutex;
        std::unique_ptr<std::list<DataCachePtr>> ResourceCacheManager::s_cacheDataList(nullptr);

        ResourceCacheManager::~ResourceCacheManager()
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            if (s_cacheDataList != nullptr)
            {
                s_cacheDataList->clear();
            }
        }

        ResourceCacheManager *ResourceCacheManager::getInstance()
        {
            if (s_instance == nullptr)
            {
                s_mutexForCreation.lock();
                if (s_instance == nullptr)
                {
                    s_instance = new ResourceCacheManager();
                    s_instance->initializeResourceCacheManager();
                }
                s_mutexForCreation.unlock();
            }
            return s_instance;
        }

        CacheID ResourceCacheManager::requestResourceCache(
            PrimitiveResourcePtr pResource, CacheCB func, CACHE_METHOD cm,
            REPORT_FREQUENCY rf, long reportTime)
        {
            if (pResource == nullptr)
            {
                throw RCSInvalidParameterException {"[requestResourceCache] Primitive Resource is invaild"};
            }

            CacheID retID = 0;

            if (cm == CACHE_METHOD::OBSERVE_ONLY)
            {
                if (func == NULL || func == nullptr)
                {
                    throw RCSInvalidParameterException {"[requestResourceCache] CacheCB is invaild"};
                }

                std::lock_guard<std::mutex> lock(s_mutex);
                retID = OCGetRandom();
                while(observeCacheIDmap.find(retID) != observeCacheIDmap.end())
                {
                    retID = OCGetRandom();
                }

                auto newHandler = std::make_shared<ObserveCache>(pResource);
                newHandler->startCache(std::move(func));
                m_observeCacheList.push_back(newHandler);

                observeCacheIDmap.insert(std::make_pair(retID, newHandler));
                return retID;
            }

            if (rf != REPORT_FREQUENCY::NONE)
            {
                if (func == NULL || func == nullptr)
                {
                    throw RCSInvalidParameterException {"[requestResourceCache] CacheCB is invaild"};
                }
                if (!reportTime)
                {
                    // default setting
                    reportTime = CACHE_DEFAULT_REPORT_MILLITIME;
                }
            }

            DataCachePtr newHandler = findDataCache(pResource);
            if (newHandler == nullptr)
            {
                std::lock_guard<std::mutex> lock(s_mutex);
                newHandler.reset(new DataCache());
                newHandler->initializeDataCache(pResource);
                s_cacheDataList->push_back(newHandler);
            }
            retID = newHandler->addSubscriber(func, rf, reportTime);

            cacheIDmap.insert(std::make_pair(retID, newHandler));

            return retID;
        }

        void ResourceCacheManager::cancelResourceCache(CacheID id)
        {
            auto observeIns = observeCacheIDmap.find(id);
            auto dataCacheIns = cacheIDmap.find(id);
            if ((dataCacheIns == cacheIDmap.end() && observeIns == observeCacheIDmap.end())
                || id == 0)
            {
                throw RCSInvalidParameterException {"[cancelResourceCache] CacheID is invaild"};
            }

            if (observeIns != observeCacheIDmap.end())
            {
                (observeIns->second)->stopCache();
                (observeIns->second).reset();
                observeCacheIDmap.erase(observeIns);
                return;
            }

            DataCachePtr foundCacheHandler = findDataCache(id);
            if (foundCacheHandler != nullptr)
            {
                CacheID retID = foundCacheHandler->deleteSubscriber(id);
                if (retID == id)
                {
                    cacheIDmap.erase(id);
                }
                std::lock_guard<std::mutex> lock(s_mutex);
                if (foundCacheHandler->isEmptySubscriber())
                {
                    s_cacheDataList->remove(foundCacheHandler);
                }
            }
        }

        void ResourceCacheManager::updateResourceCache(CacheID updateId) const
        {
            if (updateId == 0)
            {
                throw RCSInvalidParameterException {"[getCachedData] CacheID is NULL"};
            }

            DataCachePtr foundCache = findDataCache(updateId);
            if (foundCache == nullptr)
            {
                throw RCSInvalidParameterException {"[getCachedData] CacheID is invaild"};
            }
            foundCache->requestGet();
        }

        const RCSResourceAttributes ResourceCacheManager::getCachedData(CacheID id) const
        {
            if (id == 0)
            {
                throw RCSInvalidParameterException {"[getCachedData] CacheID is NULL"};
            }

            auto observePtr = observeCacheIDmap.find(id);
            if (observePtr != observeCacheIDmap.end())
            {
                return (observePtr->second)->getCachedData();
            }

            DataCachePtr handler = findDataCache(id);
            if (handler == nullptr)
            {
                throw RCSInvalidParameterException {"[getCachedData] CacheID is invaild"};
            }

            if (handler->isCachedData() == false)
            {
                throw HasNoCachedDataException {"[getCachedData] Cached Data is not stored"};
            }

            return handler->getCachedData();
        }

        CACHE_STATE ResourceCacheManager::getResourceCacheState(CacheID id) const
        {
            if (id == 0)
            {
                throw RCSInvalidParameterException {"[getResourceCacheState] CacheID is NULL"};
            }

            auto observePtr = observeCacheIDmap.find(id);
            if (observePtr != observeCacheIDmap.end())
            {
                return (observePtr->second)->getCacheState();
            }

            DataCachePtr handler = findDataCache(id);
            if (handler == nullptr)
            {
                return CACHE_STATE::NONE;
            }
            return handler->getCacheState();
        }

        bool ResourceCacheManager::isCachedData(CacheID id) const
        {
            if (id == 0)
            {
                throw RCSInvalidParameterException {"[isCachedData] CacheID is NULL"};
            }

            auto observePtr = observeCacheIDmap.find(id);
            if (observePtr != observeCacheIDmap.end())
            {
                return (observePtr->second)->isCachedData();
            }

            DataCachePtr handler = findDataCache(id);
            if (handler == nullptr)
            {
                throw RCSInvalidParameterException {"[isCachedData] CacheID is invaild"};
            }
            return handler->isCachedData();
        }

        void ResourceCacheManager::initializeResourceCacheManager()
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            if (s_cacheDataList == nullptr)
            {
                s_cacheDataList
                    = std::unique_ptr<std::list<DataCachePtr>>(new std::list<DataCachePtr>);
            }
        }

        DataCachePtr ResourceCacheManager::findDataCache(PrimitiveResourcePtr pResource) const
        {
            DataCachePtr retHandler = nullptr;
            std::lock_guard<std::mutex> lock(s_mutex);
            for (auto &i : * s_cacheDataList)
            {
                if (i->getPrimitiveResource()->getUri() == pResource->getUri() &&
                    i->getPrimitiveResource()->getHost() == pResource->getHost())
                {
                    retHandler = i;
                    break;
                }
            }
            return retHandler;
        }

        DataCachePtr ResourceCacheManager::findDataCache(CacheID id) const
        {
            DataCachePtr retHandler = nullptr;
            for (auto it : cacheIDmap)
            {
                if (it.first == id)
                {
                    retHandler = it.second;
                    break;
                }
            }

            return retHandler;
        }
    } // namespace Service
} // namespace OIC
