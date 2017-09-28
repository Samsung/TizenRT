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


/**
 * @file
 *
 * This file provides C++ APIs for Notification Service for Accepted Providers.
 */

#ifndef _NS_ACCEPTED_PROVIDERS_H_
#define _NS_ACCEPTED_PROVIDERS_H_

#include <map>
#include <mutex>
#include <algorithm>
#include "NSProvider.h"


namespace OIC
{
    namespace Service
    {
        /**
              * @class   NSAcceptedProviders
              * @brief   This class provides a set of C++APIs for managing accepted Providers.
              */
        class NSAcceptedProviders
        {
            public :
                /**
                      * Constructor of NSAcceptedProviders.
                      *
                      */
                NSAcceptedProviders()
                {
                    removeProviders();
                }

                /**
                      * Destructor of NSAcceptedProviders.
                      */
                ~NSAcceptedProviders()
                {
                    removeProviders();
                }

                /**
                     *  request to get NSProvider pointer
                     * @param id -id as string
                     *
                     * @return shared pointer to NSProvider
                     */
                std::shared_ptr<NSProvider> getProvider(const std::string &id);

                /**
                      *  request to add NSProvider pointer
                      * @param shared pointer to NSProvider
                      *
                      */
                void addProvider(std::shared_ptr<NSProvider> provider);

                /**
                      *  request to remove NSProvider
                      * @param id -id as string
                      *
                      */
                void removeProvider(const std::string &id);

                /**
                      *  request to check if NSProvider is accepted
                      * @param id -id as string
                      *
                      * @return true if accepted else false
                      */
                bool isAccepted(const std::string &id);

                /**
                      *  get size of providers accepted.
                      * @return m_providers size
                      */
                int size();

                /**
                      *  request to remove all NSProviders
                      */
                void removeProviders();

                /**
                      *  get the map of providers accepted.
                      * @return m_providers  -map of accepted providers
                      */
                std::map<std::string, std::shared_ptr<NSProvider> > getProviders();

            private :
                std::map<std::string, std::shared_ptr<NSProvider> > m_providers;
                std::mutex m_mutex;
        };
    }
}

#endif //_NS_ACCEPTED_PROVIDERS_H_
