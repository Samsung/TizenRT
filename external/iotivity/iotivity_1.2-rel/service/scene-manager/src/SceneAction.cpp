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

#include "SceneAction.h"

#include "SceneMemberResource.h"

namespace OIC
{
    namespace Service
    {
        SceneAction::SceneAction(const SceneMemberResource::Ptr SceneMemberResource,
                const std::string& sceneName, const RCSResourceAttributes& attr) :
                m_pRemoteResourceObject(SceneMemberResource->getRemoteResourceObject()),
                m_sceneName(sceneName),
                m_sceneMemberResource(SceneMemberResource)
        {
            for (const auto& it : attr)
            {
                m_sceneMemberResource->addMappingInfo(
                        SceneMemberResource::MappingInfo(m_sceneName, it.key(), it.value()));
            }
        }

        SceneAction::SceneAction(const SceneMemberResource::Ptr SceneMemberResource,
                const std::string& sceneName, const std::string& key,
                const RCSResourceAttributes::Value& value) :
                m_pRemoteResourceObject(SceneMemberResource->getRemoteResourceObject()),
                m_sceneName(sceneName), m_sceneMemberResource(SceneMemberResource)
        {
            m_sceneMemberResource->addMappingInfo(
                                SceneMemberResource::MappingInfo(m_sceneName, key, value));
        }

        void SceneAction::resetExecutionParameter(const std::string& key,
                RCSResourceAttributes::Value value)
        {
            RCSResourceAttributes attr;
            attr[key] = value;
            resetExecutionParameter(attr);
        }

        void SceneAction::resetExecutionParameter(const RCSResourceAttributes& attr)
        {
            for(const auto& it : attr)
            {
                m_sceneMemberResource->addMappingInfo(
                        SceneMemberResource::MappingInfo(m_sceneName, it.key(), it.value()));
            }
        }

        RCSResourceAttributes SceneAction::getExecutionParameter() const
        {
            RCSResourceAttributes attr;
            for(const auto& it : m_sceneMemberResource->getMappingInfos())
            {
                if(it.sceneName == m_sceneName)
                {
                    attr[it.key] = RCSResourceAttributes::Value(it.value);
                }
            }
            return attr;
        }

        RCSRemoteResourceObject::Ptr SceneAction::getRemoteResourceObject() const
        {
            return m_pRemoteResourceObject;
        }

    } /* namespace Service */
} /* namespace OIC */

