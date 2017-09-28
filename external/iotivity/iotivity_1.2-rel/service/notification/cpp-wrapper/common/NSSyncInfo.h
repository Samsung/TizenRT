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
 * This file contains Notification service SyncInfo representation.
 */

#ifndef _NS_SYNC_INFO_H_
#define _NS_SYNC_INFO_H_

#include <string>
#include "NSCommon.h"

namespace OIC
{
    namespace Service
    {
        /**
         * @class   NSSyncInfo
         * @brief   This class provides a set of APIs for Notification service SyncInfo .
         */
        class NSSyncInfo
        {
            public:
                /** NSSyncType - enumeration for Notification service SyncType*/
                enum class NSSyncType
                {
                    NS_SYNC_UNREAD = 0,
                    NS_SYNC_READ = 1,
                    NS_SYNC_DELETED = 2,
                };

                /**
                        * Constructor of NSSyncInfo.
                        */
                NSSyncInfo(): m_messageId(0), m_state(NSSyncType::NS_SYNC_UNREAD)
                {
                }

                /**
                        * Constructor of NSSyncInfo.
                        *
                        * @param syncInfo - pointer to NSSyncInfo struct to initialize.
                        */
                NSSyncInfo(::NSSyncInfo *syncInfo);


                /**
                        * Constructor of NSSyncInfo.
                        *
                        * @param messageId - messageId of the Notification SyncInfo.
                        * @param providerId - providerId of the Notification SyncInfo.
                        * @param state - state of the Notification SyncInfo.
                        */
                NSSyncInfo(const uint64_t &messageId, const std::string &providerId,
                           const NSSyncType &state)
                    : m_messageId(messageId), m_providerId(providerId), m_state(state)
                {
                }

                /**
                        * Destructor of NSSyncInfo.
                        */
                ~NSSyncInfo() = default;

                /**
                     * This method is for getting Message Id from the Notification service sync info.
                     *
                     * @return Id as uint64_t.
                     */
                uint64_t getMessageId() const;

                /**
                     * This method is for getting Provider Id from the Notification service sync info.
                     *
                     * @return Id as string.
                     */
                std::string getProviderId() const;

                /**
                     * This method is for getting state from the Notification service sync info.
                     *
                     * @return state as NSSyncType.
                     */
                NSSyncType getState() const;

            private:
                uint64_t m_messageId;
                std::string m_providerId;
                NSSyncType m_state;
        };
    }
}
#endif /* _NS_SYNC_INFO_H_ */
