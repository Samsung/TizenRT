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

#ifndef SCENE_COLLECTION_RESOURCE_OBJECT_H
#define SCENE_COLLECTION_RESOURCE_OBJECT_H

#include <list>

#include "RCSResourceObject.h"
#include "SceneCommons.h"
#include "SceneMemberResource.h"

namespace OIC
{
    namespace Service
    {
        class SceneCollectionResource
                : public std::enable_shared_from_this<SceneCollectionResource>
        {
        public:
            typedef std::shared_ptr< SceneCollectionResource > Ptr;
            typedef std::function< void(int) > SceneExecuteCallback;

            ~SceneCollectionResource() = default;

            static SceneCollectionResource::Ptr create();
            static SceneCollectionResource::Ptr create(const RCSResourceAttributes &);

            void addScene(std::string &&);
            void addScene(const std::string &);

            void addSceneMember(SceneMemberResource::Ptr);

            void execute(std::string &&);
            void execute(const std::string &);
            void execute(std::string &&, SceneExecuteCallback);
            void execute(const std::string &, SceneExecuteCallback);

            void setName(std::string &&);
            void setName(const std::string &);

            std::vector<std::string> getSceneValues() const;

            std::string getName() const;

            std::string getId() const;
            std::string getUri() const;
            std::string getAddress() const;

            std::vector<SceneMemberResource::Ptr> getSceneMembers() const;

            std::vector<SceneMemberResource::Ptr> findSceneMembers(
                    const std::string & sceneName) const;

            RCSResourceObject::Ptr getRCSResourceObject() const;

        private:
            class SceneExecuteResponseHandler
            {
            public:
                typedef std::shared_ptr<SceneExecuteResponseHandler> Ptr;

                SceneExecuteResponseHandler()
                : m_numOfMembers(0), m_responseMembers(0), m_errorCode(0) { }
                ~SceneExecuteResponseHandler() = default;

                int m_numOfMembers;
                int m_responseMembers;
                int m_errorCode;
                std::weak_ptr<SceneCollectionResource> m_owner;
                SceneExecuteCallback m_cb;

                static SceneExecuteResponseHandler::Ptr createExecuteHandler(
                        const SceneCollectionResource::Ptr, SceneExecuteCallback);
                void onResponse(const RCSResourceAttributes &, int);
            };

            class SceneCollectionRequestHandler
            {
            public:
                SceneCollectionRequestHandler() = default;
                ~SceneCollectionRequestHandler() = default;

                std::weak_ptr<SceneCollectionResource> m_owner;

                RCSSetResponse onSetRequest(
                        const RCSRequest &, RCSResourceAttributes &);

            private:
                RCSSetResponse addSceneRequest(
                        const RCSRequest &, RCSResourceAttributes &);
                RCSSetResponse executeSceneRequest(
                        const RCSRequest &, RCSResourceAttributes &);
                RCSSetResponse createSceneMemberRequest(
                        const RCSRequest &, RCSResourceAttributes &);
                RCSSetResponse setSceneCollectionName(
                        const RCSRequest &, RCSResourceAttributes &);

                void addMemberInfoFromRemote(SceneMemberResource::Ptr,
                        std::vector<RCSResourceAttributes>);
            };

            std::string m_uri;
            std::string m_address;

            RCSResourceObject::Ptr m_sceneCollectionResourceObject;
            mutable std::mutex m_sceneMemberLock;
            std::vector<SceneMemberResource::Ptr> m_sceneMembers;

            SceneCollectionRequestHandler m_requestHandler;

            SceneCollectionResource();

            SceneCollectionResource(const SceneCollectionResource &) = delete;
            SceneCollectionResource & operator = (
                    const SceneCollectionResource &) = delete;

            SceneCollectionResource(SceneCollectionResource &&) = delete;
            SceneCollectionResource & operator = (
                    SceneCollectionResource &&) = delete;

            RCSResourceObject::Ptr createResourceObject();
            void setDefaultAttributes();
            void initSetRequestHandler();
        };
    }
}

#endif // SCENE_COLLECTION_RESOURCE_OBJECT_H

