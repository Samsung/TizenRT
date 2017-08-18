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
 * This file contains Notification service topic representation.
 */

#ifndef _NS_TOPIC_H_
#define _NS_TOPIC_H_


#include <string>
#include "NSCommon.h"

namespace OIC
{
    namespace Service
    {
        /**
         * @class   NSTopic
         * @brief   This class provides a set of APIs for Notification service Topic.
         */
        class NSTopic
        {
            public:
                /**
                     *  Notification topic State
                     */
                enum class NSTopicState
                {
                    UNSUBSCRIBED = 0,
                    SUBSCRIBED = 1,
                };

                /**
                     * Constructor of NSTopic.
                     */
                NSTopic(): m_state(NSTopicState::UNSUBSCRIBED)
                {
                }

                /**
                     * Constructor of NSTopic.
                     *
                     * @param topicName - topicName of the Notification service Topic.
                     * @param state - as NSTopicState.
                     */
                NSTopic(const std::string &topicName, const NSTopicState state)
                    : m_topicName(topicName) , m_state(state)
                {
                }


                /**
                     * Destructor of NSTopic.
                     */
                ~NSTopic() = default;

                /**
                      * This method is for getting topicName from the Notification service Topic.
                      *
                      * @return topicName as string.
                      */
                std::string getTopicName() const;

                /**
                      * This method is for setting topicName for the Notification service Topic.
                      *
                      * @param topicName - as string.
                      */
                void setTopicName(const std::string &topicName);

                /**
                      * This method is for getting state from the Notification service Topic.
                      *
                      * @return state as NSTopicState.
                      */
                NSTopicState getState() const;

                /**
                      * This method is for setting state for the Notification service Topic.
                      *
                      * @param state - as NSTopicState.
                      */
                void setState(const NSTopicState state);

            private:
                std::string m_topicName;
                NSTopicState m_state;

        };
    }
}
#endif /* _NS_TOPIC_H_ */
