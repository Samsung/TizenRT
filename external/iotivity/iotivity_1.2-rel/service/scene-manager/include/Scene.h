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

#ifndef SM_SCENE_H_
#define SM_SCENE_H_

#include "SceneAction.h"

#include <vector>

namespace OIC
{
    namespace Service
    {

        class SceneCollectionResource;

        /**
         * @class Scene
         *
         * @brief Scene class is an interface class to manage scenes provided by SceneCollection
         * resource. This class provide APIs for creating a new SceneAction instance, retrieving all
         * SceneAction instances created before. And it provides an API to execute a scene.
         *
         */
        class Scene
        {
        public:
            typedef std::shared_ptr< Scene > Ptr;

            /**
             * Typedef for callback of execute APIs
             *
             * @see execute
             */
            typedef std::function< void(int) >  ExecuteCallback;

        private:
            Scene(const Scene&) = default;
            Scene(const std::string&, std::shared_ptr<SceneCollectionResource>);
            friend class SceneCollection;

        public:

            /**
             * Adds new SceneAction instance to the Scene instance
             *
             * @param pRCSRemoteResourceObject        A pointer of discovered resource
             * @param key                               A key of attributes
             * @param value                             A value to be mapped against the key
             *
             * @return A shared pointer of SceneAction instance
             *
             * @throws RCSInvalidParameterException if pRCSRemoteResourceObject is nullptr
             * @throws RCSBadRequestException if scene member resource is already registered
             *
             * @note SceneAction instance is only produced by Scene class
             *
             * @see RCSResourceAttributes
             */
            SceneAction::Ptr addNewSceneAction(
                    const RCSRemoteResourceObject::Ptr& pRCSRemoteResourceObject,
                    std::string key, RCSResourceAttributes::Value value);

            /**
             * Adds new SceneAction instance to the Scene instance
             *
             * @param pRCSRemoteResourceObject        A pointer of discovered resource
             * @param attr                              A attribute set of key and value
             *
             * @return A shared pointer of SceneAction instance
             *
             * @throws RCSInvalidParameterException if pRCSRemoteResourceObject is nullptr
             * @throws RCSBadRequestException if SceneMember is already registered
             *
             * @note SceneAction instance is only produced by Scene class
             *
             * @see RCSResourceAttributes
             */
            SceneAction::Ptr addNewSceneAction(
                    const RCSRemoteResourceObject::Ptr& pRCSRemoteResourceObject,
                    RCSResourceAttributes attr);

            /**
             * Gets SceneAction using discovered resource
             *
             * @param pRCSRemoteResourceObject        A pointer of discovered resource
             *
             * @return A shared pointer of SceneAction
             *
             * @throws RCSInvalidParameterException
             * if pRCSRemoteResourceObject is unknown resource
             */
            SceneAction::Ptr getSceneAction(
                    const RCSRemoteResourceObject::Ptr& pRCSRemoteResourceObject) const;

            /**
             * Gets all SceneActions include current Scene
             *
             * @return A vector of shared pointer of SceneAction instance
             *
             */
            std::vector<SceneAction::Ptr> getSceneActions() const ;

            /**
             * Gets Scene's name provided SceneCollection resource
             *
             * @return Scene's name
             */
            std::string getName() const;

            /**
             * Requests executing Scene to SceneCollection resource
             *
             * @param cb                        A callback to execute Scene
             */
            void execute(ExecuteCallback cb);

        private:
            std::string m_name;
            std::shared_ptr< SceneCollectionResource > m_sceneCollectionResource;

        };
    } /* namespace Service */
} /* namespace OIC */

#endif /* SM_SCENE_H_ */

