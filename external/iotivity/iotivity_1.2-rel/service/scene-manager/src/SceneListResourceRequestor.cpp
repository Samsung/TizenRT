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

#include "SceneListResourceRequestor.h"
#include "RemoteSceneUtils.h"
#include "OCPlatform.h"

namespace OIC
{
    namespace Service
    {

        SceneListResourceRequestor::SceneListResourceRequestor(
            RCSRemoteResourceObject::Ptr listResource)
                : m_sceneListResource{ listResource }
        {
            SCENE_CLIENT_ASSERT_NOT_NULL(listResource);
        }

        void SceneListResourceRequestor::requestSceneCollectionCreation(
            const std::string &name, InternalCreateSceneCollectionCallback internalCB)
        {
            RCSResourceAttributes attrs;
            attrs[SCENE_KEY_NAME] = name;

            RCSRemoteResourceObject::SetCallback setRequestCB
                = std::bind(&SceneListResourceRequestor::onSceneCollectionCreated,
                            std::placeholders::_2, std::placeholders::_3,
                            name, std::move(internalCB),
                            SceneListResourceRequestor::wPtr(shared_from_this()));

            RCSQueryParams queryParams;
            queryParams.setResourceInterface(LINK_BATCH);

            m_sceneListResource->set(queryParams, std::move(attrs), std::move(setRequestCB));
        }

        void SceneListResourceRequestor::requestSetName
        (const std::string &name, InternalSetNameCallback internalCB)
        {
            RCSResourceAttributes attrs;
            attrs[SCENE_KEY_NAME] = name;

            RCSRemoteResourceObject::SetCallback setRequestCB
                = std::bind(&SceneListResourceRequestor::onNameSet,
                std::placeholders::_2, std::placeholders::_3, name, std::move(internalCB),
                SceneListResourceRequestor::wPtr(shared_from_this()));

            RCSQueryParams queryParams;
            queryParams.setResourceInterface(SCENE_CLIENT_REQ_IF);

            m_sceneListResource->set(queryParams, std::move(attrs), std::move(setRequestCB));
        }

        void SceneListResourceRequestor::requestGet(
            const std::string &ifType, RCSRemoteResourceObject::GetCallback cb)
        {
            RCSQueryParams params;
            params.setResourceInterface(ifType);

            m_sceneListResource->get(params, cb);
        }

        RCSRemoteResourceObject::Ptr SceneListResourceRequestor::getRemoteResourceObject() const
        {
            return m_sceneListResource;
        }

        void SceneListResourceRequestor::onSceneCollectionCreated(
            const RCSRepresentation &rep, int eCode,
            const std::string &name, const InternalCreateSceneCollectionCallback &cb,
            SceneListResourceRequestor::wPtr ptr)
        {
            SceneListResourceRequestor::Ptr listPtr = ptr.lock();

            if (listPtr)
            {
                listPtr->onSceneCollectionCreated_impl(std::move(rep), eCode, name, std::move(cb));
            }
        }

        void SceneListResourceRequestor::onSceneCollectionCreated_impl(
            const RCSRepresentation &rep, int eCode,
            const std::string &name, const InternalCreateSceneCollectionCallback &internalCB)
        {
            int result = SCENE_CLIENT_BADREQUEST;
            std::string link, id;

            if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CHANGED)
            {
                try
                {
                    RCSResourceAttributes attrs = rep.getAttributes();

                    if (attrs.at(SCENE_KEY_NAME).get< std::string >().compare(name) == 0)
                    {
                        link = attrs.at(SCENE_KEY_PAYLOAD_LINK).get< std::string >();
                        id = attrs.at(SCENE_KEY_ID).get< std::string >();
                        result = SCENE_RESPONSE_SUCCESS;
                    }
                }
                catch (const std::exception &e)
                {
                    SCENE_CLIENT_PRINT_LOG(e.what());
                    result = SCENE_SERVER_INTERNALSERVERERROR;
                }
            }

            internalCB(link, id, name, result);
        }

        void SceneListResourceRequestor::onNameSet(const RCSRepresentation &rep, int eCode,
            const std::string &name, const InternalSetNameCallback &internalCB,
            SceneListResourceRequestor::wPtr ptr)
        {
            SceneListResourceRequestor::Ptr listPtr = ptr.lock();

            if (listPtr)
            {
                listPtr->onNameSet_impl(std::move(rep), eCode, name, std::move(internalCB));
            }
        }

        void SceneListResourceRequestor::onNameSet_impl(
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
