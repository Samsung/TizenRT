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
 * This file contains Notification service Message representation.
 */

#ifndef _NS_MESSAGE_H_
#define _NS_MESSAGE_H_

#include <string>
#include "NSMediaContents.h"
#include "OCRepresentation.h"

namespace OIC
{
    namespace Service
    {
        /**
         * @class   NSMessage
         * @brief   This class provides a set of APIs for Notification service Message .
         */
        class NSMessage
        {
            public:
                /** NSMessageType - enumeration for Notification service MessageType*/
                enum class NSMessageType
                {
                    NS_MESSAGE_ALERT = 1,
                    NS_MESSAGE_NOTICE = 2,
                    NS_MESSAGE_EVENT = 3,
                    NS_MESSAGE_INFO = 4,
                    NS_MESSAGE_WARNING = 5,
                };

                /**
                        * Constructor of NSMessage.
                        */
                NSMessage(): m_messageId(0), m_type(NSMessageType::NS_MESSAGE_ALERT), m_ttl(0),
                    m_mediaContents(new NSMediaContents), m_extraInfo(OC::OCRepresentation())
                {
                }

                /**
                        * Constructor of NSMessage.
                        *
                        * @param msg - pointer to NSMessage struct to initialize.
                        */
                NSMessage(::NSMessage *msg);

                /**
                     * Copy Constructor of NSMessage.
                     *
                     * @param msg - NSMessage to initialize.
                     */
                NSMessage(const NSMessage &msg);

                /**
                     * Copy assignment operator of NSMessage.
                     *
                     * @param msg -  NSMessage to initialize.
                     * @return NSMessage object reference
                     */
                NSMessage &operator=(const NSMessage &msg);

                /**
                        * Destructor of NSMessage.
                        */
                ~NSMessage();

                /**
                     * This method is for getting Message Id from the Notification service Message.
                     *
                     * @return Id as uint64_t.
                     */
                uint64_t getMessageId() const;

                /**
                     * This method is for getting Provider Id from the Notification service Message.
                     *
                     * @return Id as string.
                     */
                std::string getProviderId() const;

                /**
                     * This method is for getting type from the Notification service Message.
                     *
                     * @return type as NSMessageType.
                     */
                NSMessageType getType() const;

                /**
                     * This method is for setting type from the Notification service Message.
                     *
                     * @param type as NSMessageType.
                     */
                void setType(const NSMessageType &type);

                /**
                     * This method is for getting time from the Notification service Message.
                     *
                     * @return time as string.
                     */
                std::string getTime() const;

                /**
                     * This method is for setting time from the Notification service Message.
                     *
                     * @param time as string.
                     */
                void setTime(const std::string &time);

                /**
                     * This method is for getting time to live from the Notification service Message.
                     *
                     * @return ttl as uint64_t.
                     */
                uint64_t getTTL() const;

                /**
                     * This method is for setting time to live from the Notification service Message.
                     *
                     * @param ttl as uint64_t.
                     */
                void setTTL(const uint64_t &ttl);

                /**
                     * This method is for getting Title from the Notification service Message.
                     *
                     * @return Title as string.
                     */
                std::string getTitle() const;

                /**
                     * This method is for setting Title from the Notification service Message.
                     *
                     * @param title as string.
                     */
                void setTitle(const std::string &title);

                /**
                     * This method is for getting contentText from the Notification service Message.
                     *
                     * @return contentText as string.
                     */
                std::string getContentText() const;

                /**
                     * This method is for setting contentText from the Notification service Message.
                     *
                     * @param contextText as string.
                     */
                void setContentText(const std::string &contextText);

                /**
                     * This method is for getting sourceName from the Notification service Message.
                     *
                     * @return sourceName as string.
                     */
                std::string getSourceName() const;

                /**
                     * This method is for setting sourceName from the Notification service Message.
                     *
                     * @param sourceName as string.
                     */
                void setSourceName(const std::string &sourceName);

                /**
                     * This method is for getting mediaContents from the Notification service Message.
                     *
                     * @return mediaContents as NSMediaContents pointer.
                     */
                NSMediaContents *getMediaContents() const;

                /**
                     * This method is for setting mediaContents from the Notification service Message.
                     *
                     * @param mediaContents as NSMediaContents pointer.
                     */
                void setMediaContents(NSMediaContents *mediaContents);

                /**
                     * This method is for getting Topic from the Notification service Message.
                     *
                     * @return Topic as string.
                     */
                std::string getTopic() const;

                /**
                     * This method is for setting Topic for the Notification service Message.
                     *
                     * @return Topic as string.
                     */
                void setTopic(const std::string &topic);

                /**
                     * This method is for getting extraInfo from the Notification service Message.
                     *
                     * @return extraInfo as OCRepresentation.
                     */
                OC::OCRepresentation getExtraInfo() const;

                /**
                     * This method is for setting extraInfo for the Notification service Message.
                     *
                     * @return extraInfo as OCRepresentation.
                     */
                void setExtraInfo(const OC::OCRepresentation &extraInfo);

            private:
                uint64_t m_messageId;
                std::string m_providerId;

                NSMessageType m_type;
                std::string m_time;
                uint64_t m_ttl;
                std::string m_title;
                std::string m_contentText;
                std::string m_sourceName;
                NSMediaContents *m_mediaContents;
                std::string m_topic;
                OC::OCRepresentation m_extraInfo;

        };
    }
}
#endif /* _NS_MESSAGE_H_ */
