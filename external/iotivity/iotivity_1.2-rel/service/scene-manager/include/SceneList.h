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

#ifndef SM_SCENELIST_H_
#define SM_SCENELIST_H_

#include "SceneCollection.h"

#include <string>

namespace OIC
{
    namespace Service
    {

        /**
         * @class SceneList
         *
         * @brief SceneList class is an interface class to manage SceneList resource. This class
         * provides APIs for creating a new SceneCollection instance and retrieving the existing
         * instances as well as for setting/getting a name attribute of SceneList resource.
         *
         */
        class SceneList
        {
        private:
            SceneList() = default;
            ~SceneList() = default;

        public:
            /**
             * Gets static instance of SceneList
             *
             * @return SceneList instance
             *
             */
            static SceneList* getInstance();

            /**
             * Adds new SceneCollection instance
             *
             * @return A shared pointer of SceneCollection instance
             *
             * @note SceneCollection instance is only produced by SceneList class
             */
            SceneCollection::Ptr addNewSceneCollection();

            /**
             * Gets all SceneCollection instances stored in SceneList resource
             *
             * @return A vector of shared pointers of SceneCollection instances
             *
             * @note SceneCollection instance that addNewSceneCollection returns is not same
             * instance that getSceneCollections returns
             */
            std::vector<SceneCollection::Ptr> getSceneCollections() const;

            /**
             * Sets a name attribute of SceneList resource
             *
             * @param sceneListName               A SceneList resource's name
             */
            void setName(const std::string& sceneListName);

            /**
             * Gets a name attribute of SceneList resource
             *
             * @return A SceneList resource's name
             */
            std::string getName() const;
        };
    } /* namespace Service */
} /* namespace OIC */

#endif /* SM_SCENELIST_H_ */
