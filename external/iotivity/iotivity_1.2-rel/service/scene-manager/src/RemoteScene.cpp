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

#include "RemoteScene.h"

#include <utility>
#include <cassert>

#include "SceneCommons.h"
#include "SceneCollectionResourceRequestor.h"
#include "SceneMemberResourceRequestor.h"
#include "OCPlatform.h"

namespace OIC
{
    namespace Service
    {

        RemoteScene::RemoteScene(
            const std::string &name, std::shared_ptr< SceneCollectionResourceRequestor > requestor)
            : m_name{ name }, m_requestor{ requestor }
        {
            assert(requestor);
        }

        void RemoteScene::addNewSceneAction(
            RCSRemoteResourceObject::Ptr targetResource, const RCSResourceAttributes &attrs,
            AddNewSceneActionCallback clientCB)
        {
            if (targetResource == nullptr)
            {
                throw RCSInvalidParameterException("RCSRemoteResoureObject value is null");
            }

            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "addNewSceneAction : Callback is NULL" };
            }

            SceneCollectionResourceRequestor::InternalAddMemberCallback internalCB
                = std::bind(&RemoteScene::onSceneActionAdded, this,
                std::placeholders::_1, targetResource, attrs, std::move(clientCB));

            m_requestor->requestAddSceneMember(
                targetResource, m_name, attrs, internalCB);
        }

        void RemoteScene::addNewSceneAction(
            RCSRemoteResourceObject::Ptr targetResource,
            const std::string &key, const RCSResourceAttributes::Value &value,
            AddNewSceneActionCallback clientCB)
        {
            RCSResourceAttributes attrs;
            attrs[key] = RCSResourceAttributes::Value(value);

            addNewSceneAction(targetResource, attrs, clientCB);
        }

        std::vector< RemoteSceneAction::Ptr > RemoteScene::getRemoteSceneActions() const
        {
            std::lock_guard< std::mutex > actionlock(m_sceneActionLock);
            std::vector< RemoteSceneAction::Ptr > sceneActionList;

            for (auto itrMap : m_remoteSceneActions)
            {
                sceneActionList.push_back(itrMap.second);
            }

            return sceneActionList;
        }

        RemoteSceneAction::Ptr RemoteScene::getRemoteSceneAction(
            const RCSRemoteResourceObject::Ptr targetResource) const
        {
            if (targetResource == nullptr)
            {
                throw RCSInvalidParameterException("RCSRemoteResoureObject value is null");
            }

            std::lock_guard< std::mutex > actionlock(m_sceneActionLock);
            auto itr = m_remoteSceneActions.find(
                targetResource->getAddress() + targetResource->getUri());

            if (itr == m_remoteSceneActions.end())
            {
                throw RCSInvalidParameterException("Invalid RCSRemoteResoureObject");
            }

            return itr->second;
        }

        std::string RemoteScene::getName() const
        {
            return m_name;
        }

        void RemoteScene::execute(RemoteSceneExecuteCallback clientCB)
        {
            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "execute : Callback is NULL" };
            }

            SceneCollectionResourceRequestor::InternalSceneRequestCallback internalCB
                = std::bind(&RemoteScene::onSceneExecuted, this, std::placeholders::_2,
                            std::placeholders::_3, std::move(clientCB));

            m_requestor->requestSceneExecution(m_name, internalCB);
        }

        RemoteSceneAction::Ptr RemoteScene::createRemoteSceneAction(
            const std::string &targetHref, const RCSResourceAttributes &attrs)
        {
            SceneMemberResourceRequestor::Ptr memRequestor
                = m_requestor->getSceneMemberResourceRequestor(targetHref);

            if (memRequestor == nullptr)
            {
                return nullptr;
            }

            RemoteSceneAction::Ptr newAction(new RemoteSceneAction(memRequestor, m_name, attrs));

            {
                std::lock_guard< std::mutex > actionlock(m_sceneActionLock);
                m_remoteSceneActions.insert(
                    std::make_pair(targetHref, newAction));
            }

            return newAction;
        }

        void RemoteScene::addExistingRemoteSceneAction(
            const std::string &href, const std::string &id, RCSRemoteResourceObject::Ptr target,
            const std::string &key, const RCSResourceAttributes::Value &value)
        {
            std::string targetHref = target->getAddress() + target->getUri();

            SceneMemberResourceRequestor::Ptr foundMemberRequestor
                = m_requestor->getSceneMemberResourceRequestor(targetHref);

            if (foundMemberRequestor == nullptr)
                m_requestor->createSceneMemberResourceRequestor(href, id, target);

            RCSResourceAttributes attrs;
            attrs[key] = RCSResourceAttributes::Value(value);

            createRemoteSceneAction(targetHref, attrs);
        }

        void RemoteScene::onSceneActionAdded(
            int eCode, RCSRemoteResourceObject::Ptr target, const RCSResourceAttributes &attrs,
            const AddNewSceneActionCallback &clientCB)
        {
            int result = SCENE_CLIENT_BADREQUEST;
            RemoteSceneAction::Ptr newAction = nullptr;

            if (eCode == SCENE_RESPONSE_SUCCESS)
            {
                std::string targetLink = target->getAddress() + target->getUri();

                newAction = createRemoteSceneAction(targetLink, attrs);

                if (newAction)
                    result = SCENE_RESPONSE_SUCCESS;
            }

            clientCB(newAction, result);
        }

        void RemoteScene::onSceneExecuted(const std::string &sceneName, const int eCode,
            const RemoteSceneExecuteCallback &clientCB)
        {
            clientCB(sceneName, eCode);
        }

    }
}