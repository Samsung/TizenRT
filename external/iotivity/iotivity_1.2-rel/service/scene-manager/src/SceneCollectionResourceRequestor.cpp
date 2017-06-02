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

#include "SceneCollectionResourceRequestor.h"
#include "RemoteSceneUtils.h"
#include "OCPlatform.h"

namespace OIC
{
    namespace Service
    {

        SceneCollectionResourceRequestor::SceneCollectionResourceRequestor(
            RCSRemoteResourceObject::Ptr collectionResource)
                : m_sceneCollectionResource{ collectionResource }
        {
            SCENE_CLIENT_ASSERT_NOT_NULL(collectionResource);
        }

        void SceneCollectionResourceRequestor::requestSceneCreation(
            const std::string &name, InternalSceneRequestCallback createSceneCB)
        {
            RCSResourceAttributes attributesToSet;
            std::vector< std::string > scenenames{ name };

            attributesToSet[SCENE_KEY_SCENEVALUES] = scenenames;

            RCSRemoteResourceObject::RemoteAttributesSetCallback setRequestCB
                = std::bind(&SceneCollectionResourceRequestor::onSetResponseForScene,
                            std::placeholders::_1, std::placeholders::_2,
                            name, std::move(createSceneCB), ADD_SCENE,
                            SceneCollectionResourceRequestor::wPtr(shared_from_this()));

            m_sceneCollectionResource->setRemoteAttributes(
                std::move(attributesToSet), std::move(setRequestCB));
        }

        void SceneCollectionResourceRequestor::requestSceneRemoval
        (const std::string &/* name */, InternalSceneRequestCallback)
        {

        }

        void SceneCollectionResourceRequestor::requestSceneExecution
        (const std::string &name, InternalSceneRequestCallback executeSceneCB)
        {
            RCSResourceAttributes attributesToSet;
            attributesToSet[SCENE_KEY_LAST_SCENE] = name;

            RCSRemoteResourceObject::RemoteAttributesSetCallback setRequestCB
                = std::bind(&SceneCollectionResourceRequestor::onSetResponseForScene,
                            std::placeholders::_1, std::placeholders::_2,
                            name, std::move(executeSceneCB), EXECUTE_SCENE,
                            SceneCollectionResourceRequestor::wPtr(shared_from_this()));

            m_sceneCollectionResource->setRemoteAttributes(
                std::move(attributesToSet), std::move(setRequestCB));
        }

        void SceneCollectionResourceRequestor::requestAddSceneMember(
            RCSRemoteResourceObject::Ptr targetResource, const std::string &sceneName,
            const RCSResourceAttributes &attr, InternalAddMemberCallback addMemberCB)
        {
            SCENE_CLIENT_ASSERT_NOT_NULL(targetResource);

            RCSResourceAttributes attributesToSet, linkAttrs;

            linkAttrs[SCENE_KEY_HREF] = targetResource->getAddress() + targetResource->getUri();
            linkAttrs[SCENE_KEY_IF] = targetResource->getInterfaces();
            linkAttrs[SCENE_KEY_RT] = targetResource->getTypes();

            attributesToSet[SCENE_KEY_PAYLOAD_LINK] = linkAttrs;

            if (!attr.empty())
            {
                std::vector< RCSResourceAttributes > vecSceneMappings;
                for (const auto &itr : attr)
                {
                    RCSResourceAttributes sceneMappingAttrs;
                    sceneMappingAttrs[SCENE_KEY_SCENE] = sceneName;
                    sceneMappingAttrs[SCENE_KEY_MEMBERPROPERTY] = itr.key();
                    sceneMappingAttrs[SCENE_KEY_MEMBERVALUE] = itr.value();

                    vecSceneMappings.push_back(sceneMappingAttrs);
                }

                attributesToSet[SCENE_KEY_SCENEMAPPINGS] = vecSceneMappings;
            }

            RCSRemoteResourceObject::SetCallback setRequestCB
                = std::bind(&SceneCollectionResourceRequestor::onSceneMemberAdded,
                            std::placeholders::_2, std::placeholders::_3,
                            targetResource,
                            std::move(addMemberCB),
                            SceneCollectionResourceRequestor::wPtr(shared_from_this()));

            RCSQueryParams queryParams;
            queryParams.setResourceInterface(LINK_BATCH);

            m_sceneCollectionResource->set(queryParams, std::move(attributesToSet),
                                              std::move(setRequestCB));
        }

        void SceneCollectionResourceRequestor::requestSetName
        (const std::string &name, InternalSetNameCallback internalCB)
        {
            RCSResourceAttributes attrs;
            attrs[SCENE_KEY_NAME] = name;

            RCSRemoteResourceObject::SetCallback setRequestCB
                = std::bind(&SceneCollectionResourceRequestor::onNameSet,
                std::placeholders::_2, std::placeholders::_3, name, std::move(internalCB),
                SceneCollectionResourceRequestor::wPtr(shared_from_this()));

            RCSQueryParams queryParams;
            queryParams.setResourceInterface(SCENE_CLIENT_REQ_IF);

            m_sceneCollectionResource->set(queryParams, std::move(attrs), std::move(setRequestCB));
        }

        void SceneCollectionResourceRequestor::requestGet(
            const std::string &ifType, RCSRemoteResourceObject::GetCallback cb)
        {
            RCSQueryParams params;
            params.setResourceInterface(ifType);

            m_sceneCollectionResource->get(params, cb);
        }

        RCSRemoteResourceObject::Ptr
            SceneCollectionResourceRequestor::getRemoteResourceObject() const
        {
            return m_sceneCollectionResource;
        }

        SceneMemberResourceRequestor::Ptr
        SceneCollectionResourceRequestor::createSceneMemberResourceRequestor(
            const std::string &memHref, const std::string &id, RCSRemoteResourceObject::Ptr target)
        {
            try
            {
                std::vector< std::string > vecRT{ SCENE_MEMBER_RT };
                std::vector< std::string > vecIF{ SCENE_CLIENT_REQ_IF };

                RCSRemoteResourceObject::Ptr pResource
                    = SceneUtils::createRCSResourceObject(
                        memHref, SCENE_CONNECTIVITY, vecRT, vecIF);

                SceneMemberResourceRequestor::Ptr pMemRequestor =
                    std::make_shared< SceneMemberResourceRequestor >(pResource, id);

                pMemRequestor->setRemoteResourceObject(target);

                {
                    std::lock_guard< std::mutex > memberlock(m_memberRequestorLock);
                    m_memberRequestors[target->getAddress() + target->getUri()] = pMemRequestor;
                }

                return pMemRequestor;
            }
            catch (const std::exception &e)
            {
                SCENE_CLIENT_PRINT_LOG(e.what());
                return nullptr;
            }
        }

        SceneMemberResourceRequestor::Ptr
        SceneCollectionResourceRequestor::getSceneMemberResourceRequestor(
            const std::string &targetHref) const
        {
            std::lock_guard< std::mutex > memberlock(m_memberRequestorLock);

            return m_memberRequestors.find(targetHref) != m_memberRequestors.end() ?
                m_memberRequestors.at(targetHref) : nullptr;
        }

        void SceneCollectionResourceRequestor::onSetResponseForScene(
            const RCSResourceAttributes &attrs, int eCode,
            const std::string &name, const InternalSceneRequestCallback &cb,
            REQUEST_TYPE reqType, SceneCollectionResourceRequestor::wPtr ptr)
        {
            SceneCollectionResourceRequestor::Ptr collectionPtr = ptr.lock();

            if (collectionPtr)
            {
                collectionPtr->onSetResponseForScene_impl(
                    std::move(attrs), eCode, name, std::move(cb), reqType);
            }
        }

        void SceneCollectionResourceRequestor::onSetResponseForScene_impl(
            const RCSResourceAttributes &attrs, int eCode, const std::string &name,
            const InternalSceneRequestCallback &internalCB, REQUEST_TYPE reqType)
        {
            // TODO error code
            int resultCode = SCENE_CLIENT_BADREQUEST;
            if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CHANGED)
            {
                try
                {
                    switch (reqType)
                    {
                        case ADD_SCENE:
                            {
                                auto scenes
                                    = attrs.at(SCENE_KEY_SCENEVALUES).
                                        get< std::vector< std::string > >();

                                if ((std::find(scenes.begin(), scenes.end(), name))
                                    != scenes.end())
                                {
                                    resultCode = SCENE_RESPONSE_SUCCESS;
                                }
                            }
                            break;

                        case REMOVE_SCENE:
                            break;

                        case EXECUTE_SCENE:
                            {
                                auto lastScene
                                    = attrs.at(SCENE_KEY_LAST_SCENE).get< std::string >();

                                if (lastScene.compare(name) == 0)
                                {
                                    resultCode = SCENE_RESPONSE_SUCCESS;
                                }
                            }
                            break;
                    }
                }
                catch (const std::exception &e)
                {
                    SCENE_CLIENT_PRINT_LOG(e.what());
                    resultCode = SCENE_SERVER_INTERNALSERVERERROR;
                }
            }

            internalCB(reqType, name, resultCode);
        }

        void SceneCollectionResourceRequestor::onSceneMemberAdded(
            const RCSRepresentation &rep, int eCode,
            RCSRemoteResourceObject::Ptr target, const InternalAddMemberCallback &internalCB,
            SceneCollectionResourceRequestor::wPtr ptr)
        {
            SceneCollectionResourceRequestor::Ptr collection = ptr.lock();

            if (collection)
            {
                collection->onSceneMemberAdded_impl(
                    std::move(rep), eCode, target, std::move(internalCB));
            }
        }

        void SceneCollectionResourceRequestor::onSceneMemberAdded_impl(
            const RCSRepresentation &rep, int eCode,
            RCSRemoteResourceObject::Ptr target, const InternalAddMemberCallback &internalCB)
        {
            // TODO error code
            int result = SCENE_CLIENT_BADREQUEST;
            SceneMemberResourceRequestor::Ptr memRequestor = nullptr;

            if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CHANGED)
            {
                try
                {
                    RCSResourceAttributes receivedAttrs = rep.getAttributes();

                    memRequestor
                        = createSceneMemberResourceRequestor(
                              receivedAttrs.at(SCENE_KEY_CREATEDLINK).get< std::string >(),
                              receivedAttrs.at(SCENE_KEY_ID).get< std::string >(), target);

                    if (memRequestor)
                    {
                        memRequestor->setRemoteResourceObject(target);
                        result = SCENE_RESPONSE_SUCCESS;
                    }
                }
                catch (const std::exception &e)
                {
                    SCENE_CLIENT_PRINT_LOG(e.what());
                    result = SCENE_SERVER_INTERNALSERVERERROR;
                }
            }

            internalCB(result);
        }

        void SceneCollectionResourceRequestor::onNameSet(const RCSRepresentation &rep, int eCode,
            const std::string &name, const InternalSetNameCallback &internalCB,
            SceneCollectionResourceRequestor::wPtr ptr)
        {
            SceneCollectionResourceRequestor::Ptr collectionPtr = ptr.lock();

            if (collectionPtr)
            {
                collectionPtr->onNameSet_impl(std::move(rep), eCode, name, std::move(internalCB));
            }
        }

        void SceneCollectionResourceRequestor::onNameSet_impl(
            const RCSRepresentation &rep, int eCode, const std::string &name,
            const InternalSetNameCallback &internalCB)
        {
            int result = SCENE_CLIENT_BADREQUEST;
            if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CHANGED)
            {
                if (rep.getAttributes().at(SCENE_KEY_NAME).get< std::string >() == name)
                {
                    result = SCENE_RESPONSE_SUCCESS;
                }

            }

            internalCB(result);
        }

    }
}
