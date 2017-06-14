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

#ifndef SM_SCENECOLLECTION_RESOURCE_REQUESTOR_H_
#define SM_SCENECOLLECTION_RESOURCE_REQUESTOR_H_

#include <map>
#include <mutex>

#include "SceneCommons.h"
#include "RCSRemoteResourceObject.h"
#include "RCSRepresentation.h"
#include "SceneMemberResourceRequestor.h"

namespace OIC
{
    namespace Service
    {

        class SceneCollectionResourceRequestor
            : public std::enable_shared_from_this< SceneCollectionResourceRequestor >
        {
            public:
                typedef std::shared_ptr< SceneCollectionResourceRequestor > Ptr;
                typedef std::weak_ptr< SceneCollectionResourceRequestor > wPtr;

                enum REQUEST_TYPE
                {
                    ADD_SCENE, REMOVE_SCENE, EXECUTE_SCENE
                };

                typedef std::function< 
                    void(REQUEST_TYPE, const std::string &name, int eCode) >
                        InternalSceneRequestCallback;

                typedef std::function < void(int eCode) > InternalAddMemberCallback;

                typedef std::function < void(int eCode) > InternalSetNameCallback;

            public:
                SceneCollectionResourceRequestor(RCSRemoteResourceObject::Ptr collectionResource);
                ~SceneCollectionResourceRequestor() = default;

                void requestSceneCreation(const std::string &name, InternalSceneRequestCallback);
                void requestSceneRemoval(const std::string &name, InternalSceneRequestCallback);

                void requestSceneExecution(const std::string &name, InternalSceneRequestCallback);

                void requestAddSceneMember(RCSRemoteResourceObject::Ptr targetResource,
                                           const std::string &sceneName,
                                           const RCSResourceAttributes &attr,
                                           InternalAddMemberCallback);

                void requestSetName(const std::string &, InternalSetNameCallback);

                void requestGet(const std::string &, RCSRemoteResourceObject::GetCallback);

                RCSRemoteResourceObject::Ptr getRemoteResourceObject() const;

                SceneMemberResourceRequestor::Ptr createSceneMemberResourceRequestor(
                    const std::string &memHref, const std::string &id,
                    RCSRemoteResourceObject::Ptr target);

                SceneMemberResourceRequestor::Ptr getSceneMemberResourceRequestor(
                    const std::string &targetHref) const;

            private:
                static void onSetResponseForScene(
                    const RCSResourceAttributes &attrs, int eCode,
                    const std::string &name, const InternalSceneRequestCallback &,
                    REQUEST_TYPE, SceneCollectionResourceRequestor::wPtr);

                void onSetResponseForScene_impl(
                    const RCSResourceAttributes &attrs, int eCode,
                    const std::string &name, const InternalSceneRequestCallback &,
                    REQUEST_TYPE);

                static void onSceneMemberAdded(
                    const RCSRepresentation &, int eCode,
                    RCSRemoteResourceObject::Ptr, const InternalAddMemberCallback &,
                    SceneCollectionResourceRequestor::wPtr);

                void onSceneMemberAdded_impl(
                    const RCSRepresentation &, int eCode,
                    RCSRemoteResourceObject::Ptr, const InternalAddMemberCallback &);

                static void onNameSet(const RCSRepresentation &, int eCode, const std::string &,
                    const InternalSetNameCallback &, SceneCollectionResourceRequestor::wPtr);

                void onNameSet_impl(const RCSRepresentation &, int eCode, const std::string &,
                    const InternalSetNameCallback &);

            private:
                RCSRemoteResourceObject::Ptr m_sceneCollectionResource;
                mutable std::mutex m_memberRequestorLock;
                std::map< std::string, SceneMemberResourceRequestor::Ptr > m_memberRequestors;
        };
        
    }
}

#endif /* SM_SCENECOLLECTION_RESOURCE_REQUESTOR_H_ */

