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
 * This file contains the declaration of classes and its members related to SceneListResrouceObject
 */

#ifndef SCENE_LIST_RESOURCE_OBJECT_H
#define SCENE_LIST_RESOURCE_OBJECT_H

#include <string>

#include "RCSResourceObject.h"
#include "SceneCollectionResource.h"
#include "SceneCommons.h"

namespace OIC
{
    namespace Service
    {
        class SceneListResource
        {
        public:
            /**
             * Returns Scene List Resource object as single instance.
             */
            static SceneListResource * getInstance();

            /**
             * Add Scene Collection resource object to Scene List Resource.
             *
             * @param collectionObj created Scene Collection Resource Object by constructor of SceneCollectionResourceObject class
             */
            void addSceneCollectionResource(SceneCollectionResource::Ptr collectionObj);

            /**
             * Returns Scene List name.
             */
            std::string getName() const;

            /**
             * Sets Scene List name.
             *
             * @param name name to set
             */
            void setName(std::string && name);

            /**
             * @overload
             */
            void setName(const std::string &);

            /**
             * Returns all of Scene Collection Resource object.
             */
            std::vector<SceneCollectionResource::Ptr> getSceneCollections() const;

            std::vector<RCSResourceAttributes> getChildrenAttributes() const;

            RCSResourceObject::Ptr getResourceObject() const;

        private:
            class SceneListRequestHandler
            {
            public:
                SceneListRequestHandler() = default;
                ~SceneListRequestHandler() = default;

                static RCSSetResponse onSetRequest(const RCSRequest &, RCSResourceAttributes &);
                static RCSGetResponse onGetRequest(const RCSRequest &, RCSResourceAttributes &);
            };

            RCSResourceObject::Ptr m_sceneListObj;
            mutable std::mutex m_sceneCollectionLock;
            std::vector<SceneCollectionResource::Ptr> m_sceneCollections;

            SceneListResource();
            ~SceneListResource() = default;

            SceneListResource(const SceneListResource &) = delete;
            SceneListResource & operator = (const SceneListResource &) = delete;

            SceneListResource(SceneListResource &&) = delete;
            SceneListResource & operator = (SceneListResource &&) = delete;

            RCSResourceObject::Ptr createResourceObject();
        };
    }
}

#endif // SCENE_LIST_RESOURCE_OBJECT_H
