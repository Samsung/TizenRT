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

#ifndef RCM_OBSERVECACHE_H_
#define RCM_OBSERVECACHE_H_

#include <list>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>

#include "CacheTypes.h"
#include "PrimitiveResource.h"
#include "ExpiryTimer.h"

namespace OIC
{
    namespace Service
    {
        class ObserveCache : public std::enable_shared_from_this<ObserveCache>
        {
            public:
                typedef std::function<OCStackResult(std::shared_ptr<PrimitiveResource>,
                                            const RCSResourceAttributes &)> DataCacheCB;
                typedef std::shared_ptr<ObserveCache> Ptr;

            public:
                ObserveCache(std::weak_ptr<PrimitiveResource> pResource);
                ~ObserveCache() = default;

                ObserveCache(const ObserveCache &) = delete;
                ObserveCache(ObserveCache &&) = delete;
                ObserveCache & operator = (const ObserveCache &) = delete;
                ObserveCache & operator = (ObserveCache &&) = delete;

                void startCache(DataCacheCB func);
                void stopCache();

                CACHE_STATE getCacheState() const;

                RCSResourceAttributes getCachedData() const;

                bool isCachedData() const;
                bool isStartCache() const;

            private:
                typedef unsigned int TimerID;
                typedef std::weak_ptr<PrimitiveResource> weakPrimitiveResource;
                typedef std::weak_ptr<ObserveCache> weakDataCache;

                // resource instance
                weakPrimitiveResource m_wpResource;

                // cached data info
                RCSResourceAttributes m_attributes;
                CACHE_STATE m_state;

                DataCacheCB m_reportCB;

                std::atomic<bool> m_isStart;

                CacheID m_id;

            private:
                static void verifyObserveCB(const HeaderOptions &_hos,
                                            const ResponseStatement &_rep, int _result,
                                            unsigned int _seq, weakDataCache ptr);
                void onObserve(const HeaderOptions &_hos,
                               const ResponseStatement &_rep, int _result, unsigned int _seq);
                bool convertOCResultToSuccess(OCStackResult ret);
        };
    } // namespace Service
} // namespace OIC

#endif /* RCM_DATACACHE_H_ */
