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

#include "SceneCollection.h"

#include "SceneCollectionResource.h"

namespace OIC
{
    namespace Service
    {
        SceneCollection::SceneCollection(
                const SceneCollectionResource::Ptr& sceneCollectionResource) :
                m_sceneCollectionResource(sceneCollectionResource) {}

        Scene::Ptr SceneCollection::addNewScene(const std::string& sceneName)
        {
            if(sceneName.empty())
            {
                throw RCSInvalidParameterException("Scene name is an empty string");
            }

            m_sceneCollectionResource->addScene(sceneName);

            return Scene::Ptr(new Scene(sceneName, m_sceneCollectionResource));
        }

        std::unordered_map< std::string, Scene::Ptr > SceneCollection::getScenes() const
        {
            std::unordered_map< std::string, Scene::Ptr > scenes;

            for(const auto &it : m_sceneCollectionResource->getSceneValues())
            {
                Scene::Ptr scenePtr(new Scene(it, m_sceneCollectionResource));
                scenes.insert(std::pair< std::string, Scene::Ptr >(it, scenePtr));
            }
            return scenes;
        }

        Scene::Ptr SceneCollection::getScene(const std::string& sceneName) const
        {
            auto sceneValues = m_sceneCollectionResource->getSceneValues();
            auto it = std::find(sceneValues.begin(), sceneValues.end(), sceneName);
            if(it == sceneValues.end())
            {
                throw RCSInvalidParameterException("Scene Name is Invalid!");
            }
            return Scene::Ptr(new Scene(sceneName, m_sceneCollectionResource));
        }

        void SceneCollection::setName(const std::string& name)
        {
            m_sceneCollectionResource->setName(name);
        }

        std::string SceneCollection::getName() const
        {
            return m_sceneCollectionResource->getName();
        }

        std::string SceneCollection::getId() const
        {
            return m_sceneCollectionResource->getId();
        }

    } /* namespace Service */
} /* namespace OIC */

