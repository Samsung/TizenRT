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

#ifndef SM_SCENEACTION_H_
#define SM_SCENEACTION_H_

#include "RCSRemoteResourceObject.h"
#include "RCSResourceAttributes.h"

namespace OIC
{
    namespace Service
    {

        class SceneMemberResource;

        /**
         * @class SceneAction
         *
         * @brief SceneAction class indicates a unit of actions when a scene is executed.
         * SceneAction instance is initialized with 3 essential parameters: a target resource,
         * target attribute key, and its target value. And this class also provide APIs to update
         * a target attribute information if one wants
         *
         */
        class SceneAction
        {
        public:
            typedef std::shared_ptr< SceneAction > Ptr;

        private:
            SceneAction(const std::shared_ptr< SceneMemberResource >,
                    const std::string&, const RCSResourceAttributes&);
            SceneAction(const std::shared_ptr< SceneMemberResource >,
                    const std::string&, const std::string&,
                    const RCSResourceAttributes::Value&);
            friend class Scene;

        public:
            /**
             * Sets the SceneAction parameters like a target attribute key and its value
             * It replaces existing execution parameter
             *
             * @param key                   A key of attributes
             * @param value                 A value to be mapped against the key
             *
             * @see RCSResourceAttributes
             */
            void resetExecutionParameter(const std::string& key, RCSResourceAttributes::Value value);

            /**
             * Sets the SceneAction parameters like a target attribute key and its value
             * It replaces existing execution parameter
             *
             * @param attr                  Attributes to set
             *
             * @see RCSResourceAttributes
             */
            void resetExecutionParameter(const RCSResourceAttributes& attr);

            /**
             * Gets execution parameter of the SceneAction instance
             *
             * @return attributes of SceneMember resource
             *
             * @see RCSResourceAttributes
             */
            RCSResourceAttributes getExecutionParameter() const;

            /**
             * Gets remote resource object
             *
             * @return RCSRemoteResourceObject
             */
            RCSRemoteResourceObject::Ptr getRemoteResourceObject() const;

        private:
            RCSRemoteResourceObject::Ptr m_pRemoteResourceObject;
            std::string m_sceneName;
            std::shared_ptr< SceneMemberResource > m_sceneMemberResource;
        };
    } /* namespace Service */
} /* namespace OIC */

#endif /* SM_SCENEACTION_H_ */
