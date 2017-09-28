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

#include "NSTopicsList.h"
#include "NSException.h"
#include "oic_malloc.h"

namespace OIC
{
    namespace Service
    {
        NSTopicsList::NSTopicsList(::NSTopicLL *topics, bool modify)
        {
            ::NSTopicLL *topicsNode = nullptr;
            topicsNode = topics;
            m_modifiable = modify;

            while (topicsNode != nullptr)
            {
                m_topicsList.push_back(new NSTopic(
                    topicsNode->topicName, (NSTopic::NSTopicState)topicsNode->state));
                topicsNode = topicsNode->next;
            }

        }
        NSTopicsList::NSTopicsList(const NSTopicsList &topicsList)
        {
            for (auto it : topicsList.getTopicsList())
            {
                m_topicsList.push_back(new NSTopic(it.getTopicName(), it.getState()));
            }
            m_modifiable = false;
        }

        NSTopicsList &NSTopicsList::operator=(const NSTopicsList &topicsList)
        {
            for (auto it : topicsList.getTopicsList())
            {
                this->m_topicsList.push_back(new NSTopic(it.getTopicName(), it.getState()));
            }
            return *this;
            m_modifiable = false;
        }

        NSTopicsList::~NSTopicsList()
        {
            for (auto it : m_topicsList)
            {
                delete it;
            }
            m_topicsList.clear();
        }

        void NSTopicsList::addTopic(const std::string &topicName, NSTopic::NSTopicState state)
        {
            if(m_modifiable)
            {
                m_topicsList.push_back(new NSTopic(topicName, state));
            }
            else
            {
                throw NSException("Invalid Operation. Method not supported as the object state is invalid");
            }
        }

        void NSTopicsList::removeTopic(const std::string &topicName)
        {
            if(m_modifiable)
            {
                for (auto it : m_topicsList)
                {
                    if (it->getTopicName().compare(topicName) == 0)
                    {
                        m_topicsList.remove(it);
                    }
                }
            }
            else
            {
                throw NSException("Invalid Operation. Method not supported as the object state is invalid");
            }
        }

        std::list<NSTopic> NSTopicsList::getTopicsList() const
        {
            std::list<NSTopic> topicList;
            for (auto it : m_topicsList)
            {
                NSTopic topic(it->getTopicName(), it->getState());
                topicList.push_back(topic);
            }
            return topicList;
        }
       
        //Below method restricts the application from illegally modifying Topics when
        //Provider is in Invalid state. By calling the API, the service prevents and protects
        //the integrity of TopicsList updation when the associated object is Invalid
        //The default value of the variable is 'false' in the provider side. Also, the state is irreversible.
        void NSTopicsList::unsetModifiability()
        {
            m_modifiable = false;
        }
    }
}
