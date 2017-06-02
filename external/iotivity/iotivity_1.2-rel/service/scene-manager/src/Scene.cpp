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

#include "Scene.h"

#include "SceneCollectionResource.h"

#include <algorithm>

namespace OIC
{
    namespace Service
    {
        Scene::Scene(const std::string& sceneName,
                SceneCollectionResource::Ptr sceneCollectionResource) :
                m_name(sceneName), m_sceneCollectionResource(sceneCollectionResource) {}

        SceneAction::Ptr Scene::addNewSceneAction(
               const RCSRemoteResourceObject::Ptr& pRCSRemoteResourceObject,
               std::string key, RCSResourceAttributes::Value value)
        {
            RCSResourceAttributes resAttr;
            resAttr[key] = value;
            return addNewSceneAction(pRCSRemoteResourceObject, std::move(resAttr));
        }

        SceneAction::Ptr Scene::addNewSceneAction(
                const RCSRemoteResourceObject::Ptr& pRCSRemoteResourceObject,
                RCSResourceAttributes attr)
        {
            if(pRCSRemoteResourceObject == nullptr)
            {
                throw RCSInvalidParameterException("pRCSRemoteResourceObject is empty!");
            }

            SceneMemberResource::Ptr sceneMemberResObj;
            sceneMemberResObj = SceneMemberResource::createSceneMemberResource(
                    pRCSRemoteResourceObject);
            try
            {
                m_sceneCollectionResource->addSceneMember(sceneMemberResObj);
            }
            catch(RCSBadRequestException& e)
            {
                auto sceneMemberRes = m_sceneCollectionResource->findSceneMembers(m_name);

                auto it = std::find_if(sceneMemberRes.begin(), sceneMemberRes.end(),
                        [&pRCSRemoteResourceObject](const SceneMemberResource::Ptr& member)
                        {
                            return member->getRemoteResourceObject() == pRCSRemoteResourceObject;
                        }
                );

                if(it != sceneMemberRes.end())
                {
                    throw RCSBadRequestException(
                            "It is already registered member. Please set Execution Parameter!");
                }

                auto sceneMembers = m_sceneCollectionResource->getSceneMembers();
                auto memberRes = std::find_if(sceneMembers.begin(), sceneMembers.end(),
                        [&pRCSRemoteResourceObject](const SceneMemberResource::Ptr& member)
                        {
                            return member->getRemoteResourceObject() == pRCSRemoteResourceObject;
                        }
                );
                return SceneAction::Ptr(new SceneAction((*memberRes), m_name, attr));
            }

            return SceneAction::Ptr(new SceneAction(sceneMemberResObj, m_name, attr));
        }

        SceneAction::Ptr Scene::getSceneAction(
                const RCSRemoteResourceObject::Ptr& pRCSRemoteResourceObject) const
        {
            auto sceneMemberRes = m_sceneCollectionResource->findSceneMembers(m_name);

            auto it = std::find_if(sceneMemberRes.begin(), sceneMemberRes.end(),
                    [&pRCSRemoteResourceObject](const SceneMemberResource::Ptr& member)
                    {
                        return member->getRemoteResourceObject() == pRCSRemoteResourceObject;
                    }
            );

            if(it == sceneMemberRes.end())
            {
                throw RCSInvalidParameterException("Unknown Remote Resource!");
            }

            RCSResourceAttributes actionParam;
            for(const auto &info : (*it)->findMappingInfos(m_name))
            {
                actionParam[info.key] = info.value;
            }

            return SceneAction::Ptr(new SceneAction((*it), m_name, actionParam));
        }

        std::vector<SceneAction::Ptr> Scene::getSceneActions() const
        {
            std::vector<SceneAction::Ptr> actions;
            auto sceneMemberRes = m_sceneCollectionResource->findSceneMembers(m_name);

            std::for_each(sceneMemberRes.begin(), sceneMemberRes.end(),
                [&](const SceneMemberResource::Ptr& member)
                {
                    RCSResourceAttributes actionParam;

                    for(const auto &it : member->findMappingInfos(m_name))
                    {
                        actionParam[it.key] = it.value;
                    }
                    actions.push_back(SceneAction::Ptr(
                            new SceneAction(member, m_name, actionParam)));
                }
            );

            return actions;
         }

        std::string Scene::getName() const
        {
            return m_name;
        }

        void Scene::execute(ExecuteCallback cb)
        {
            if(cb == nullptr)
            {
                throw RCSInvalidParameterException("Callback is empty!");
            }

            m_sceneCollectionResource->execute(m_name, std::move(cb));
        }
    } /* namespace Service */
} /* namespace OIC */
