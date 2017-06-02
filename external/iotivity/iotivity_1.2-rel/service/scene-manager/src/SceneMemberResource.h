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
 * This file contains the declaration of classes and its members related to SceneMemberResrouceObject
 */

#ifndef SCENE_MEMBER_RESOURCE_OBJECT_H
#define SCENE_MEMBER_RESOURCE_OBJECT_H

#include "RCSResourceObject.h"
#include "RCSRemoteResourceObject.h"
#include "SceneCommons.h"

namespace OIC
{
    namespace Service
    {
        class SceneMemberResource
                : public std::enable_shared_from_this<SceneMemberResource>
        {
        public:
            typedef std::shared_ptr< SceneMemberResource > Ptr;

            /**
             * Callback definition to be invoked when the response of setRemoteAttribitues is received.
             *
             * @param attrs the result attributes
             * @param eCode the error code received from the resource
             *
             * @see RCSRemoteResourceObject::setRemoteAttributes
             */
            typedef std::function< void(const RCSResourceAttributes & attrs, int eCode) >
                MemberexecuteCallback;

            /**
             * A Mapping information about each scene values.
             */
            struct MappingInfo
            {
                MappingInfo(
                        const std::string & scene,
                        const std::string & keyName,
                        const RCSResourceAttributes::Value & val)
                :sceneName(scene), key(keyName), value(val) { }

                MappingInfo(MappingInfo &&) = default;
                MappingInfo(const MappingInfo &) = default;

                static MappingInfo create(const RCSResourceAttributes &);

                std::string sceneName;              ///< name of scene value
                std::string key;                    ///< key to set at attributes of remote resource
                RCSResourceAttributes::Value value; ///< val to set at attributes of remote resource
            };

            ~SceneMemberResource() = default;

            /**
             * Register a Scene member resource and return a SceneMemberResourceObject
             * using link information of remote resource.
             *
             * @param attrs information to make scene member resource
             */
            static SceneMemberResource::Ptr
            createSceneMemberResource(RCSResourceAttributes && attrs);

            /**
             * @overload
             */
            static SceneMemberResource::Ptr
            createSceneMemberResource(const RCSResourceAttributes &);

            /**
             * Register a Scene member resource and returns a SceneMemberResourceObject
             * using information of RCSRemoteResourceObject.
             *
             * @param remoteObj information to make scene member resource
             */
            static SceneMemberResource::Ptr
            createSceneMemberResource(RCSRemoteResourceObject::Ptr remoteObj);

            /**
             * Add Scene mapping information at scene member resource.
             *
             * @param mappingInfo
             */
            void addMappingInfo(MappingInfo && mappingInfo);

            /**
             * @overload
             */
            void addMappingInfo(const MappingInfo &);

            /**
             * Returns all Mapping information of a scene member resource.
             */
            std::vector<MappingInfo> getMappingInfos() const;

            std::vector<MappingInfo> findMappingInfos(const std::string & sceneValue) const;

            bool hasSceneValue(const std::string &) const;

            /**
             * Returns ID of a Scene member resource.
             */
            std::string getId() const;

            /**
             * Returns Uri of a Scene member resource. (e.g. coap://192.168.0.2.1:12345/SceneMember)
             */
            std::string getFullUri() const;

            /**
             * Returns Uri of a Target resource of scene member. (e.g. coap://192.168.0.2.1:12345/light)
             */
            std::string getTargetUri() const;

            /**
             * Returns RCSRemoteResourceObject about Scene member resource
             */
            RCSRemoteResourceObject::Ptr getRemoteResourceObject() const;

            /**
             * Returns RCSResourceObject of Scene member resource
             */
            RCSResourceObject::Ptr getRCSResourceObject() const;

            /**
             * Execute of Scene Action (with callback for response).
             *
             * @param sceneValue scene value to execute
             * @param cb callback to response
             */
            void execute(std::string && sceneValue, MemberexecuteCallback cb);

            /**
             * @overload
             */
            void execute(const std::string &, MemberexecuteCallback);

            /**
             * Execute of Scene Action (without callback for response).
             *
             * @param sceneValue scene value to execute
             */
            void execute(std::string && sceneValue);

            /**
             * @overload
             */
            void execute(const std::string &);

            void setName(const std::string &);
            void setName(std::string &&);

            std::string getName() const;

        private:
            class SceneMemberRequestHandler
            {
            public:
                SceneMemberRequestHandler() = default;
                ~SceneMemberRequestHandler() = default;

                std::weak_ptr<SceneMemberResource> m_owner;

                RCSSetResponse onSetRequest(const RCSRequest & , RCSResourceAttributes &);

                RCSSetResponse addMappingInfos(const RCSRequest & , RCSResourceAttributes &);
                RCSSetResponse setSceneMemberName(const RCSRequest & , RCSResourceAttributes &);
            };

            std::string m_uri;
            RCSResourceObject::Ptr m_sceneMemberResourceObj;
            RCSRemoteResourceObject::Ptr m_remoteMemberObj;
            SceneMemberRequestHandler m_requestHandler;

            SceneMemberResource() = default;

            SceneMemberResource(const SceneMemberResource &) = delete;
            SceneMemberResource & operator = (const SceneMemberResource &) = delete;

            SceneMemberResource(SceneMemberResource &&) = delete;
            SceneMemberResource & operator = (SceneMemberResource &&) = delete;

            void createResourceObject();
            void setDefaultAttributes();
            void initSetRequestHandler();
        };
    }
}

#endif // SCENE_MEMBER_RESOURCE_OBJECT_H
