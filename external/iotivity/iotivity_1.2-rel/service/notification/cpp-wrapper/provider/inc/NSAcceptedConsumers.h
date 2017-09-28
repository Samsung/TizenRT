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
 * This file provides C++ Wrapper APIs of Notification Service for accepted consumers.
 */

#ifndef _NS_ACCEPTED_CONSUMERS_H_
#define _NS_ACCEPTED_CONSUMERS_H_

#include <map>
#include <mutex>
#include <algorithm>
#include <memory>
#include "NSConsumer.h"

namespace OIC
{
    namespace Service
    {
        /**
              * @class   NSAcceptedConsumers
              * @brief   This class provides a set of C++APIs for managing accepted Consumers.
              */
        class NSAcceptedConsumers
        {
            public :
                /**
                     * Constructor of NSAcceptedConsumers.
                     *
                     */
                NSAcceptedConsumers()
                {
                    removeConsumers();
                }

                /**
                      * Destructor of NSAcceptedConsumers.
                      */
                ~NSAcceptedConsumers()
                {
                    removeConsumers();
                }
                /**
                      *  request to get NSConsumer pointer
                      * @param id -id as string
                      *
                      * @return pointer to NSConsumer
                      */
                std::shared_ptr<NSConsumer> getConsumer(const std::string &id);

                /**
                      *  request to add NSConsumer pointer
                      * @param pointer to NSConsumer
                      *
                      */
                void addConsumer(std::shared_ptr<NSConsumer> consumer);

                /**
                      *  request to remove NSConsumer
                      * @param id -id as string
                      *
                      */
                void removeConsumer(const std::string &id);

                /**
                      *  request to check if NSConsumer is accepted
                      * @param id -id as string
                      *
                      * @return true if accepted else false
                      */
                bool isAccepted(const std::string &id);

                /**
                      *  get size of Consumers accepted.
                      * @return m_consumers size
                      */
                int size();

                /**
                      *  request to remove all NSConsumer
                      */
                void removeConsumers();

                /**
                      *  get the map of Consumers accepted.
                      * @return m_consumers  -map of accepted Consumers
                      */
                std::map<std::string, std::shared_ptr<NSConsumer> > getConsumers();

            private :
                std::map<std::string, std::shared_ptr<NSConsumer> > m_consumers;
                std::mutex m_mutex;
        };
    }
}
#endif /* _NS_ACCEPTED_CONSUMERS_H_ */
