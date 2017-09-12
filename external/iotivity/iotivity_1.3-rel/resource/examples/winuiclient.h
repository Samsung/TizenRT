/* ****************************************************************
 *
 * Copyright 2016 Intel Corporation All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef WINUICLIENT_H_
#define WINUICLIENT_H_

#include <memory>
#include <mutex>
#include <map>
#include "OCPlatform.h"
#include "OCApi.h"
#include "OCResource.h"

using namespace OC;
namespace WinUIClient{

    class Media
    {
        public:

            bool m_state;
            int m_volume;
            std::string m_name;

            Media() : m_state(false), m_volume(0), m_name("")
            {
            }
    };

    class WinUIClientApp
    {
        public:
            WinUIClientApp(OCPersistentStorage ps);
            ~WinUIClientApp();

            bool Initialize();
            void Run();
            void FindResources();
            void GetMediaRepresentation();
            void PutMediaRepresentation();
            void PostMediaRepresentation();
            void BeginObserving();
            void CancelObserving();

            std::shared_ptr<OCResource> GetResource();
            Media GetMedia(){return mymedia;}
            void SetMedia(bool state, int volume){mymedia.m_state = state; mymedia.m_volume=volume;}
            bool observing;

        private:
            void foundResource(std::shared_ptr<OCResource> resource);
            void onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);
            void onPut(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);
            void onPost(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode);
            void onPost2(const HeaderOptions& /*headerOptions*/,const OCRepresentation& rep, const int eCode);
            void onObserve(const HeaderOptions /*headerOptions*/, const OCRepresentation& rep, const int& eCode, const int& sequenceNumber);
        private:
            int observe_count();

        private:
            typedef std::map<OCResourceIdentifier, std::shared_ptr<OCResource>> DiscoveredResourceMap;

            OCPersistentStorage persistentStorage;
            Media mymedia;
            DiscoveredResourceMap discoveredResources;
            std::shared_ptr<OCResource> curResource;
            ObserveType OBSERVE_TYPE_TO_USE;
            std::mutex curResourceLock;
            bool initialized;
    };

}

#endif

