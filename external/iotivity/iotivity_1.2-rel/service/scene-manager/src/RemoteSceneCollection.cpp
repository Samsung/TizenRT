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

#include "RemoteSceneCollection.h"

#include <cassert>

#include "SceneCommons.h"
#include "RemoteSceneUtils.h"
#include "SceneCollectionResourceRequestor.h"
#include "OCPlatform.h"

namespace OIC
{
    namespace Service
    {

        RemoteSceneCollection::RemoteSceneCollection(
            std::shared_ptr< SceneCollectionResourceRequestor > requestor,
            const std::string &id, const std::string &name)
            : m_id{ id }, m_name{ name }, m_requestor{ requestor }
        {
            assert(requestor);
        }

        void RemoteSceneCollection::addNewScene
        (const std::string &name, AddNewSceneCallback clientCB)
        {
            if (name.empty())
            {
                throw RCSInvalidParameterException("Scene name is an empty string");
            }
            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "addNewScene : Callback is NULL" };
            }

            SceneCollectionResourceRequestor::InternalSceneRequestCallback internalCB
                = std::bind(&RemoteSceneCollection::onSceneAddedRemoved, this,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3, std::move(clientCB));

            m_requestor->requestSceneCreation(name, internalCB);
        }

        std::unordered_map< std::string, RemoteScene::Ptr >
            RemoteSceneCollection::getRemoteScenes() const
        {
            std::lock_guard< std::mutex > scenelock(m_sceneLock);
            return m_remoteScenes;
        }

        RemoteScene::Ptr RemoteSceneCollection::getRemoteScene(const std::string &sceneName) const
        {
            std::lock_guard< std::mutex > scenelock(m_sceneLock);
            auto itr = m_remoteScenes.find(sceneName);

            if (itr == m_remoteScenes.end())
            {
                throw RCSInvalidParameterException("Invalid scene name");
            }

            return  itr->second;
        }

        void RemoteSceneCollection::setName(const std::string &name, SetNameCallback clientCB)
        {
            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "setName : Callback is NULL" };
            }

            SceneCollectionResourceRequestor::InternalSetNameCallback internalCB
                = std::bind(&RemoteSceneCollection::onNameSet, this,
                            std::placeholders::_1, name, std::move(clientCB));

            m_requestor->requestSetName(name, internalCB);
        }

        std::string RemoteSceneCollection::getName() const
        {
            std::lock_guard< std::mutex > lock(m_nameLock);
            return m_name;
        }

        std::string RemoteSceneCollection::getId() const
        {
            return m_id;
        }

        void RemoteSceneCollection::addExistingRemoteScenes(const std::vector< std::string > &scenes)
        {
            for (const auto &scenename : scenes)
            {
                createRemoteScene(scenename);
            }
        }

        void RemoteSceneCollection::initializeRemoteScenes(
            const std::vector< RCSResourceAttributes > &MemberReps, const std::string &host)
        {
            try
            {
                for (const auto &attrs : MemberReps)
                {
                    for (const auto &mappingInfo :
                            attrs.at(SCENE_KEY_SCENEMAPPINGS).get
                                <std::vector< RCSResourceAttributes > >())
                    {
                        std::string sceneName
                            = mappingInfo.at(SCENE_KEY_SCENE).get< std::string >();

                        auto remoteScene = m_remoteScenes.find(sceneName);
                        if (remoteScene == m_remoteScenes.end()) continue;

                        RemoteScene::Ptr pRemoteScene = m_remoteScenes.at(sceneName);

                        RCSResourceAttributes targetLinkAttrs
                            = attrs.at(SCENE_KEY_PAYLOAD_LINK).get< RCSResourceAttributes >();

                        RCSRemoteResourceObject::Ptr targetResource
                            = SceneUtils::createRCSResourceObject(
                            targetLinkAttrs.at(SCENE_KEY_HREF).get< std::string >(),
                            SCENE_CONNECTIVITY,
                            targetLinkAttrs.at(SCENE_KEY_RT).get< std::vector< std::string > >(),
                            targetLinkAttrs.at(SCENE_KEY_IF).get< std::vector< std::string > >());

                        std::string mappingInfoKey
                            = mappingInfo.at(SCENE_KEY_MEMBERPROPERTY).get< std::string >();
                        RCSResourceAttributes::Value mappingInfoValue
                            = mappingInfo.at(SCENE_KEY_MEMBERVALUE);

                        pRemoteScene->addExistingRemoteSceneAction(
                            host + attrs.at(SCENE_KEY_URI).get< std::string >(),
                            attrs.at(SCENE_KEY_ID).get< std::string >(), targetResource,
                            mappingInfoKey, mappingInfoValue);
                    }
                }
            }
            catch (const std::exception &e)
            {
                SCENE_CLIENT_PRINT_LOG(e.what());
            }
        }

        RemoteScene::Ptr RemoteSceneCollection::createRemoteScene(const std::string &name)
        {
            std::lock_guard< std::mutex > scenelock(m_sceneLock);
            RemoteScene::Ptr pNewRemoteScene(new RemoteScene(name, m_requestor));

            m_remoteScenes[name] = pNewRemoteScene;

            return pNewRemoteScene;
        }

        void RemoteSceneCollection::onSceneAddedRemoved(int reqType,
            const std::string &name, int eCode,
            const AddNewSceneCallback &addCB)
        {
            switch (reqType)
            {
                case SceneCollectionResourceRequestor::REQUEST_TYPE::ADD_SCENE:
                {
                    if (eCode == SCENE_RESPONSE_SUCCESS)
                    {
                        addCB(createRemoteScene(name), SCENE_RESPONSE_SUCCESS);
                    }
                    else
                    {
                        addCB(nullptr, SCENE_CLIENT_BADREQUEST);
                    }
                }
                    break;

                case SceneCollectionResourceRequestor::REQUEST_TYPE::REMOVE_SCENE:
                    break;
            }
        }

        void RemoteSceneCollection::onNameSet(int eCode, const std::string &name,
                                              const SetNameCallback &clientCB)
        {
            int result = SCENE_CLIENT_BADREQUEST;
            if (eCode == SCENE_RESPONSE_SUCCESS)
            {
                std::lock_guard< std::mutex > lock(m_nameLock);
                m_name = name;
                result = SCENE_RESPONSE_SUCCESS;
            }

            clientCB(result);
        }

    }
}
