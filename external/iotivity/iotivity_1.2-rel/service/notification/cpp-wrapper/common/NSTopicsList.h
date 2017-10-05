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

/**
 * @file
 *
 * This file contains Notification service topics linked list representation.
 */

#ifndef _NS_TOPICS_LIST_H_
#define _NS_TOPICS_LIST_H_


#include <string>
#include <list>
#include "NSTopic.h"

namespace OIC
{
    namespace Service
    {
        /**
         * @class   NSTopicsList
         * @brief   This class provides a set of APIs for Notification service Topics Linked list.
         */
        class NSTopicsList
        {
            public:
                /**
                     * Constructor of NSTopicsList.
                     */
                NSTopicsList() : m_modifiable(true) { }

                /**
                     * Constructor of NSTopicsList.
                     *
                     * @param topics - pointer to NSTopicLL struct to initialize.
                     */
                NSTopicsList(::NSTopicLL *topics, bool modify);

                /**
                     * Copy Constructor of NSTopicsList.
                     *
                     * @param topicsList - NSTopicsList to initialize.
                     */
                NSTopicsList(const NSTopicsList &topicsList);

                /**
                     * Copy assignment operator of NSTopicsList.
                     *
                     * @param topicsList -  NSTopicsList to initialize.
                     * @return NSTopicsList object reference
                     */
                NSTopicsList &operator=(const NSTopicsList &topicsList);

                /**
                     * Destructor of NSTopicsList.
                     */
                ~NSTopicsList();

                /**
                     * This method is for adding topic for the Notification service Topics Linked list.
                     *
                     * @param topicName as string.
                     * @param state - as NSTopicState.
                     */
                void addTopic(const std::string &topicName, NSTopic::NSTopicState state);

                /**
                     * This method is for removing topic from the Notification service Topics Linked list.
                     *
                     * @param topicName as string.
                     */
                void removeTopic(const std::string &topicName);

                /**
                     * This method is for getting topic LL from the Notification service Topics Linked list.
                     *
                     * @return topic as NSTopics Linked list.
                     */
                std::list<NSTopic> getTopicsList() const;

                /**
                     * This method is for preventing for add/remove Topic from Topicslist.
                     * Below method restricts the application from illegally modifying Topics when 
                     * Provider is in Invalid state. By calling the API, the service prevents and protects
                     * the integrity of TopicsList updation when the associated object is Invalid.
                     *
                     */
                void unsetModifiability();

            private:
                std::list<NSTopic *> m_topicsList;
                bool m_modifiable;

        };
    }
}
#endif /* _NS_TOPICS_LIST_H_ */
