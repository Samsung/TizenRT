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

#include "RemoteSceneList.h"

#include <map>
#include <algorithm>

#include "SceneCommons.h"
#include "RemoteSceneUtils.h"
#include "SceneListResourceRequestor.h"
#include "SceneCollectionResourceRequestor.h"
#include "OCPlatform.h"

namespace OIC
{
    namespace Service
    {

        RemoteSceneList::RemoteSceneList(SceneListResourceRequestor::Ptr requestor)
            : m_requestor{ requestor }
        {

        }

        void RemoteSceneList::createInstance(RCSRemoteResourceObject::Ptr sceneListResource,
                                             CreateInstanceCallback clientCB)
        {
            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "createInstance : Callback is NULL" };
            }

            if (sceneListResource == nullptr)
            {
                throw RCSInvalidParameterException("Scene List resource object is null");
            }

            std::vector< std::string > rts = sceneListResource->getTypes();
            auto it = std::find(rts.begin(), rts.end(), SCENE_LIST_RT);
            if (it == rts.end())
            {
                throw RCSInvalidParameterException(
                    "Remote resource object is not a Scene List Resource");
            }

            SceneListResourceRequestor::Ptr pRequestor =
                std::make_shared< SceneListResourceRequestor >(sceneListResource);

            std::string requestIf = SCENE_CLIENT_REQ_IF;
            pRequestor->requestGet(requestIf, std::bind(
                &RemoteSceneList::onInstanceCreated,
                std::placeholders::_2, std::placeholders::_3, requestIf,
                pRequestor, std::move(clientCB)));
        }

        void RemoteSceneList::addNewSceneCollection(AddNewSceneCollectionCallback clientCB)
        {
            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "addNewSceneCollection : Callback is NULL" };
            }

            SceneListResourceRequestor::InternalCreateSceneCollectionCallback internalCB
                = std::bind(&RemoteSceneList::onSceneCollectionCreated, this,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                std::placeholders::_4, std::move(clientCB));

            m_requestor->requestSceneCollectionCreation("", internalCB);
        }

        std::vector< RemoteSceneCollection::Ptr >
            RemoteSceneList::getRemoteSceneCollections() const
        {
            std::lock_guard< std::mutex > collectionlock(m_collectionLock);
            return m_remoteSceneCollections;
        }

        void RemoteSceneList::setName(const std::string &name, SetNameCallback clientCB)
        {
            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "setName : Callback is NULL" };
            }

            SceneListResourceRequestor::InternalSetNameCallback internalCB
                = std::bind(&RemoteSceneList::onNameSet, this,
                std::placeholders::_1, name, std::move(clientCB));

            m_requestor->requestSetName(name, internalCB);
        }

        std::string RemoteSceneList::getName() const
        {
            std::lock_guard< std::mutex > lock(m_nameLock);
            return m_name;
        }

        void RemoteSceneList::onInstanceCreated(
            const RCSRepresentation &rep, int eCode, const std::string &If,
            SceneListResourceRequestor::Ptr requestor, const CreateInstanceCallback &cb)
        {
            if (eCode == OC_STACK_OK)
            {
                if (If == OC::DEFAULT_INTERFACE)
                {
                    auto retPtr = buildSceneList(requestor, rep.getAttributes());
                    cb(std::move(retPtr), SCENE_RESPONSE_SUCCESS);
                }
                else if (If == OC::BATCH_INTERFACE)
                {
                    // TODO build remote scene list instance with batch interface.
                }
                else
                {
                    // TODO error handle.
                }
            }
        }

        RemoteSceneList::Ptr RemoteSceneList::buildSceneList(
            SceneListResourceRequestor::Ptr requestor, const RCSResourceAttributes &attrs)
        {
            RemoteSceneList::Ptr newList(new RemoteSceneList(requestor));
            auto collections = newList->parseSceneListFromAttributes(attrs);

            try
            {
                newList->m_name = attrs.at(SCENE_KEY_NAME).get< std::string >();

                for (const auto &itr : collections)
                {
                    auto collection = itr.first;
                    auto host = newList->getListResourceRequestor()
                                    ->getRemoteResourceObject()->getAddress();

                    RemoteSceneCollection::Ptr newCollection
                        = newList->createRemoteSceneCollection(
                                    host + collection.at(SCENE_KEY_URI).get< std::string >(),
                                    collection.at(SCENE_KEY_ID).get< std::string >(),
                                    collection.at(SCENE_KEY_NAME).get< std::string >());

                    newCollection->addExistingRemoteScenes(
                        collection.at(SCENE_KEY_SCENEVALUES).get< std::vector< std::string > >());

                    newCollection->initializeRemoteScenes(itr.second, host);
                }
            }
            catch (const std::exception &e)
            {
                SCENE_CLIENT_PRINT_LOG(e.what());
            }

            return std::move(newList);
        }

        RemoteSceneCollection::Ptr RemoteSceneList::createRemoteSceneCollection(
            const std::string &link, const std::string &id, const std::string &name)
        {
            try
            {
                std::vector< std::string > vecRT{ SCENE_COLLECTION_RT };
                std::vector< std::string > vecIF{ SCENE_CLIENT_REQ_IF };

                RCSRemoteResourceObject::Ptr pResource
                    = SceneUtils::createRCSResourceObject(link, SCENE_CONNECTIVITY, vecRT, vecIF);

                SceneCollectionResourceRequestor::Ptr pRequestor(
                    new SceneCollectionResourceRequestor(pResource));

                RemoteSceneCollection::Ptr newCollection(
                    new RemoteSceneCollection(pRequestor, id, name));

                {
                    std::lock_guard< std::mutex > collectionlock(m_collectionLock);
                    m_remoteSceneCollections.push_back(newCollection);
                }

                return newCollection;
            }
            catch (const std::exception &e)
            {
                SCENE_CLIENT_PRINT_LOG(e.what());
                return nullptr;
            }
        }

        SceneListResourceRequestor::Ptr RemoteSceneList::getListResourceRequestor() const
        {
            return m_requestor;
        }

        std::vector<std::pair<RCSResourceAttributes, std::vector<RCSResourceAttributes>>>
            RemoteSceneList::parseSceneListFromAttributes(const RCSResourceAttributes & listAttrs)
        {
            std::vector<std::pair<RCSResourceAttributes, std::vector<RCSResourceAttributes>>>
                retParsed;

            auto collectionsResourceAttrs = getChildrenAttributes(listAttrs);

            for (unsigned int i = 0; i < collectionsResourceAttrs.size(); ++i)
            {
                retParsed.push_back(
                    std::make_pair(
                        collectionsResourceAttrs[i],
                        getChildrenAttributes(collectionsResourceAttrs[i])));
            }

            return retParsed;
        }

        std::vector<RCSResourceAttributes> RemoteSceneList::getChildrenAttributes(
            const RCSResourceAttributes & attrs) const
        {
            const std::string SCENE_CHILD = "child";

            std::vector<RCSResourceAttributes> retChildren = {};

            if (attrs.contains(SCENE_CHILD))
            {
                retChildren
                    = attrs.at(SCENE_CHILD).get<std::vector<RCSResourceAttributes>>();
            }

            return retChildren;
        }

        void RemoteSceneList::onSceneCollectionCreated(
            const std::string &link, const std::string &id, const std::string &name, int eCode,
            const AddNewSceneCollectionCallback &clientCB)
        {
            int result = SCENE_CLIENT_BADREQUEST;
            RemoteSceneCollection::Ptr newCollection = nullptr;

            if (eCode == SCENE_RESPONSE_SUCCESS)
            {
                newCollection =
                    createRemoteSceneCollection(link, id, name);

                if (newCollection)
                    result = SCENE_RESPONSE_SUCCESS;
            }

            clientCB(newCollection, result);
        }

        void RemoteSceneList::onNameSet(int eCode, const std::string &name,
            const SetNameCallback &clientCB)
        {
            int result = SCENE_CLIENT_BADREQUEST;
            if (eCode == SCENE_RESPONSE_SUCCESS)
            {
                std::lock_guard< std::mutex > lock(m_nameLock);
                m_name = name;
                result = SCENE_RESPONSE_SUCCESS;
            }

            clientCB(result);
        }

    }
}