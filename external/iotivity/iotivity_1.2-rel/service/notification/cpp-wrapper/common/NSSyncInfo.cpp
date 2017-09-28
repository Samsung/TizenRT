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

#include "NSSyncInfo.h"
#include "string.h"
#include "NSUtils.h"

namespace OIC
{
    namespace Service
    {
        NSSyncInfo::NSSyncInfo(::NSSyncInfo *syncInfo)
        {
            m_messageId = 0;
            m_state = NSSyncInfo::NSSyncType::NS_SYNC_UNREAD;
            if (syncInfo != nullptr)
            {
                m_messageId = syncInfo->messageId;
                m_providerId.assign(syncInfo->providerId, NS_UTILS_UUID_STRING_SIZE - 1);
                m_state = (NSSyncType) syncInfo->state;
            }
        }

        uint64_t NSSyncInfo::getMessageId() const
        {
            return m_messageId;
        }

        std::string NSSyncInfo::getProviderId() const
        {
            return m_providerId;
        }

        NSSyncInfo::NSSyncType NSSyncInfo::getState() const
        {
            return m_state;
        }
    }
}
