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

#ifndef SM_SCENECOLLECTION_H_
#define SM_SCENECOLLECTION_H_

#include "Scene.h"

namespace OIC
{
    namespace Service
    {

        class SceneCollectionResource;

        /**
         * @class SceneCollection
         *
         * @brief SceneCollection class is an interface class to manage SceneCollection resource.
         * This class provides APIs to create a new Scene instance and retrieve all Scene instances
         * created before. Besides, it provide APIs for retrieving and updating attribute values
         * like name attribute
         *
         */
        class SceneCollection
        {
        public:
            typedef std::shared_ptr< SceneCollection > Ptr;

        private:
            SceneCollection(const std::shared_ptr< SceneCollectionResource >&);
            friend class SceneList;

        public:

            /**
             * Adds new Scene instance to SceneCollection resource
             *
             * @param sceneName              A scene's name
             *
             * @return A shared pointer of Scene instance
             *
             * @throw RCSInvalidParameterException if scene name is empty
             * @throw RCSInvalidParameterException if scene name is dupltcated
             *
             * @note Scene instance is only produced by SceneCollection class
             * @note Scene's name must unique in one SceneCollection resource
             */
            Scene::Ptr addNewScene(const std::string& sceneName);

            /**
             * Gets all Scene instances from SceneCollection resource
             *
             * @return A unordered_map of shared pointers of Scene instances with a Scene's name
             */
            std::unordered_map< std::string, Scene::Ptr > getScenes() const;

            /**
             * Gets a Scene instance with a specific Scene's name.
             *
             * @param sceneName             A Scene's name
             *
             * @return A shared pointer of Scene instance
             *
             * @throws RCSInvalidParameterException
             * if Scene's name does not exist in SceneCollection resource
             */
            Scene::Ptr getScene(const std::string& sceneName) const;

            /**
             * Sets a name attribute of SceneCollection resource
             *
             * @param name               A SceneCollection resource's name
             */
            void setName(const std::string& name);

            /**
             * Gets a name attribute from SceneCollection resource.
             *
             * @return A SceneCollection resource's name
             */
            std::string getName() const;

            /**
             * Gets a Id attribute of SceneCollection resource.
             *
             * @return A SceneCollection resource's Id
             *
             */
            std::string getId() const;

        private:
            std::shared_ptr< SceneCollectionResource > m_sceneCollectionResource;

        };
    } /* namespace Service */
} /* namespace OIC */

#endif /* SM_SCENECOLLECTION_H_ */

