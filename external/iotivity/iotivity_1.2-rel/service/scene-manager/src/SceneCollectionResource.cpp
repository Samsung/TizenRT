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

#include "SceneCollectionResource.h"

#include <atomic>
#include "OCApi.h"
#include "RCSRequest.h"
#include "RCSSeparateResponse.h"

namespace OIC
{
    namespace Service
    {
        namespace
        {
            std::atomic_int g_numOfSceneCollection(0);
        }

        SceneCollectionResource::SceneCollectionResource()
        : m_uri(PREFIX_SCENE_COLLECTION_URI + "/" + std::to_string(g_numOfSceneCollection++)),
          m_address(), m_sceneCollectionResourceObject(), m_requestHandler()
        {
            m_sceneCollectionResourceObject = createResourceObject();
        }

        SceneCollectionResource::Ptr SceneCollectionResource::create()
        {
            SceneCollectionResource::Ptr sceneCollectionResource(new SceneCollectionResource());

            sceneCollectionResource->setDefaultAttributes();

            sceneCollectionResource->initSetRequestHandler();

            sceneCollectionResource->m_address = SceneUtils::getNetAddress();

            return sceneCollectionResource;
        }

        SceneCollectionResource::Ptr SceneCollectionResource::create(
                const RCSResourceAttributes & inputAttr)
        {
            auto sceneCollectionResource = SceneCollectionResource::create();
            if (inputAttr.contains(SCENE_KEY_NAME))
            {
                sceneCollectionResource->setName(inputAttr.at(SCENE_KEY_NAME).get<std::string>());
            }

            if (inputAttr.contains(SCENE_KEY_SCENEVALUES))
            {
                auto sceneValues = inputAttr.at(SCENE_KEY_SCENEVALUES).
                        get<std::vector<std::string>>();
                sceneCollectionResource->getRCSResourceObject()->setAttribute(
                        SCENE_KEY_SCENEVALUES, sceneValues);
            }

            if (inputAttr.contains(SCENE_KEY_LAST_SCENE))
            {
                auto sceneValues = inputAttr.at(SCENE_KEY_LAST_SCENE).get<std::string>();
                sceneCollectionResource->getRCSResourceObject()->setAttribute(
                        SCENE_KEY_LAST_SCENE, sceneValues);
            }

            return sceneCollectionResource;
        }

        RCSResourceObject::Ptr SceneCollectionResource::createResourceObject()
        {
            return RCSResourceObject::Builder(
                        m_uri, SCENE_COLLECTION_RT, BASELINE_IF).
                        addInterface(OC::BATCH_INTERFACE).
                        addInterface(LINK_BATCH).
                        setDiscoverable(true).setObservable(false).build();
        }

        void SceneCollectionResource::setDefaultAttributes()
        {
            m_sceneCollectionResourceObject->setAttribute(SCENE_KEY_LAST_SCENE, std::string());
            m_sceneCollectionResourceObject->setAttribute(SCENE_KEY_NAME, std::string());
            m_sceneCollectionResourceObject->setAttribute(
                    SCENE_KEY_ID, SceneUtils::OICGenerateUUIDStr());
            m_sceneCollectionResourceObject->setAttribute(SCENE_KEY_RTS, SCENE_MEMBER_RT);
            m_sceneCollectionResourceObject->setAttribute(
                    SCENE_KEY_SCENEVALUES, std::vector<std::string>());
            m_sceneCollectionResourceObject->setAttribute(SCENE_KEY_URI, m_uri);
        }

        void SceneCollectionResource::initSetRequestHandler()
        {
            m_requestHandler.m_owner
                = std::weak_ptr<SceneCollectionResource>(shared_from_this());

            m_sceneCollectionResourceObject->setSetRequestHandler(std::bind(
                    &SceneCollectionResource::SceneCollectionRequestHandler::onSetRequest,
                    m_requestHandler, std::placeholders::_1, std::placeholders::_2));
        }

        void SceneCollectionResource::addScene(const std::string & newScene)
        {
            addScene(std::string(newScene));
        }

        void SceneCollectionResource::addScene(std::string && newScene)
        {
            auto sceneValues = m_sceneCollectionResourceObject->getAttributeValue(
                    SCENE_KEY_SCENEVALUES).get< std::vector< std::string > >();

            auto foundScene
                = std::find(sceneValues.begin(), sceneValues.end(), newScene);
            if (foundScene == sceneValues.end())
            {
                sceneValues.push_back(std::move(newScene));

                m_sceneCollectionResourceObject->setAttribute(SCENE_KEY_SCENEVALUES, sceneValues);
            }
            else
            {
                throw RCSInvalidParameterException("Scene name is duplicate!");
            }
        }

        void SceneCollectionResource::addSceneMember(
                SceneMemberResource::Ptr newMember)
        {
            std::lock_guard<std::mutex> memberlock(m_sceneMemberLock);

            auto foundmember = std::find_if(m_sceneMembers.begin(), m_sceneMembers.end(),
                    [& newMember](const SceneMemberResource::Ptr & ptr) -> bool
                    {
                        return ptr->getTargetUri() == newMember->getTargetUri();
                    });

            if (foundmember != m_sceneMembers.end())
            {
                throw RCSBadRequestException("It is already registered member.");
            }

            m_sceneMembers.push_back(newMember);
            m_sceneCollectionResourceObject->bindResource(newMember->getRCSResourceObject());
        }

        void SceneCollectionResource::execute(std::string && sceneName)
        {
            execute(std::move(sceneName), nullptr);
        }

        void SceneCollectionResource::execute(const std::string & sceneName)
        {
            execute(std::string(sceneName));
        }

        void SceneCollectionResource::execute(
                const std::string & sceneName, SceneExecuteCallback executeCB)
        {
            execute(std::string(sceneName), std::move(executeCB));
        }

        void SceneCollectionResource::execute(
                std::string && sceneName, SceneExecuteCallback executeCB)
        {
            auto sceneValues = m_sceneCollectionResourceObject->getAttributeValue(
                    SCENE_KEY_SCENEVALUES).get< std::vector< std::string > >();

            auto foundSceneValue
                = std::find(sceneValues.begin(), sceneValues.end(), sceneName);
            if (foundSceneValue == sceneValues.end() && executeCB && !m_sceneMembers.size())
            {
                std::thread(std::move(executeCB), SCENE_CLIENT_BADREQUEST).detach();
                return;
            }

            m_sceneCollectionResourceObject->setAttribute(
                    SCENE_KEY_LAST_SCENE, sceneName);

            {
                std::lock_guard<std::mutex> memberlock(m_sceneMemberLock);
                auto executeHandler
                    = SceneExecuteResponseHandler::createExecuteHandler(
                            shared_from_this(), std::move(executeCB));
                for (auto & it : m_sceneMembers)
                {
                    it->execute(std::move(sceneName), std::bind(
                            &SceneExecuteResponseHandler::onResponse, executeHandler,
                            std::placeholders::_1, std::placeholders::_2));
                }
            }
        }

        std::string SceneCollectionResource::getId() const
        {
            return m_sceneCollectionResourceObject->getAttributeValue(
                    SCENE_KEY_ID).get<std::string>();
        }

        std::string SceneCollectionResource::getUri() const
        {
            return m_uri;
        }

        std::string SceneCollectionResource::getAddress() const
        {
            return m_address;
        }

        std::vector<std::string> SceneCollectionResource::getSceneValues() const
        {
            return m_sceneCollectionResourceObject->getAttributeValue(
                    SCENE_KEY_SCENEVALUES).get<std::vector<std::string>>();
        }

        std::vector<SceneMemberResource::Ptr> SceneCollectionResource::getSceneMembers() const
        {
            std::lock_guard<std::mutex> memberlock(m_sceneMemberLock);
            return m_sceneMembers;
        }

        std::vector<SceneMemberResource::Ptr> SceneCollectionResource::findSceneMembers(
                const std::string & sceneName) const
        {
            std::lock_guard<std::mutex> memberlock(m_sceneMemberLock);
            std::vector<SceneMemberResource::Ptr> retMembers;
            std::for_each(m_sceneMembers.begin(), m_sceneMembers.end(),
                    [& retMembers, & sceneName](SceneMemberResource::Ptr pMember)
                    {
                        if(pMember->hasSceneValue(sceneName))
                        {
                            retMembers.push_back(pMember);
                        }
                    });
            return retMembers;
        }

        RCSResourceObject::Ptr SceneCollectionResource::getRCSResourceObject() const
        {
            return m_sceneCollectionResourceObject;
        }

        void SceneCollectionResource::setName(std::string && sceneCollectionName)
        {
            m_sceneCollectionResourceObject->setAttribute(
                    SCENE_KEY_NAME, std::move(sceneCollectionName));
        }

        void SceneCollectionResource::setName(const std::string & sceneCollectionName)
        {
            setName(std::string(sceneCollectionName));
        }

        std::string SceneCollectionResource::getName() const
        {
            return m_sceneCollectionResourceObject->getAttributeValue(
                    SCENE_KEY_NAME).get<std::string>();
        }

        RCSSetResponse SceneCollectionResource::SceneCollectionRequestHandler::
        onSetRequest(const RCSRequest & request, RCSResourceAttributes & attributes)
        {
            if (request.getInterface() == LINK_BATCH)
            {
                return createSceneMemberRequest(request, attributes);
            }

            if (attributes.contains(SCENE_KEY_SCENEVALUES))
            {
                return addSceneRequest(request, attributes);
            }

            if (attributes.contains(SCENE_KEY_LAST_SCENE))
            {
                return executeSceneRequest(request, attributes);
            }

            if (attributes.contains(SCENE_KEY_NAME))
            {
                return setSceneCollectionName(request, attributes);
            }

            return RCSSetResponse::create(attributes, (int)SCENE_CLIENT_BADREQUEST).
                    setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
        }

        RCSSetResponse SceneCollectionResource::SceneCollectionRequestHandler::
        addSceneRequest(const RCSRequest & /*request*/, RCSResourceAttributes & attributes)
        {
            SceneCollectionResource::Ptr ptr = m_owner.lock();
            if (ptr == nullptr)
            {
                return RCSSetResponse::create(attributes, SCENE_CLIENT_BADREQUEST).
                        setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
            }

            auto values = attributes.at(SCENE_KEY_SCENEVALUES).get<std::vector<std::string>>();

            auto sizeofValues = values.size();
            unsigned int sameSize = 0;
            std::for_each(values.begin(), values.end(),
                    [& ptr, & sameSize](const std::string & value)
                    {
                        try
                        {
                            ptr->addScene(value);
                        } catch (...)
                        {
                            sameSize++;
                        }
                    });

            int eCode = SCENE_RESPONSE_SUCCESS;
            if (sameSize == sizeofValues)
            {
                eCode = SCENE_CLIENT_BADREQUEST;
            }

            return RCSSetResponse::create(attributes, eCode).
                    setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
        }

        RCSSetResponse SceneCollectionResource::SceneCollectionRequestHandler::
        executeSceneRequest(const RCSRequest & request, RCSResourceAttributes & attributes)
        {
            SceneCollectionResource::Ptr ptr = m_owner.lock();
            if (ptr == nullptr)
            {
                return RCSSetResponse::create(attributes, SCENE_CLIENT_BADREQUEST).
                        setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
            }

            auto requestKey = attributes.at(SCENE_KEY_LAST_SCENE).get<std::string>();

            RCSRequest req(request.getResourceObject().lock(), request.getOCRequest());

            ptr->execute(std::string(requestKey),
                    [req](int /*eCode*/)
                    {
                        // TODO need to set error code.
                        // and need to set specific attr' but this attr not to be apply to RCSResourceObject.
                        RCSSeparateResponse(req).set();
                    });

            return RCSSetResponse::separate();
        }

        RCSSetResponse SceneCollectionResource::SceneCollectionRequestHandler::
        createSceneMemberRequest(const RCSRequest & /*request*/, RCSResourceAttributes & attributes)
        {
            int eCode = SCENE_CLIENT_BADREQUEST;
            SceneCollectionResource::Ptr ptr = m_owner.lock();
            if (!ptr)
            {
                return RCSSetResponse::create(attributes, eCode).
                        setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
            }

            RCSResourceAttributes responseAtt(attributes);
            if (attributes.contains(SCENE_KEY_PAYLOAD_LINK))
            {
                auto linkAtt = attributes.at(SCENE_KEY_PAYLOAD_LINK).get<RCSResourceAttributes>();
                if (linkAtt.contains(SCENE_KEY_HREF) &&
                        linkAtt.contains(SCENE_KEY_RT) && linkAtt.contains(SCENE_KEY_IF))
                {
                    auto memberObj = SceneMemberResource::createSceneMemberResource(linkAtt);
                    try
                    {
                        ptr->addSceneMember(memberObj);
                    }
                    catch (...)
                    {
                        return RCSSetResponse::create(responseAtt, eCode).
                                setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
                    }
                    eCode = SCENE_RESPONSE_SUCCESS;

                    if (attributes.contains(SCENE_KEY_SCENEMAPPINGS))
                    {
                        addMemberInfoFromRemote(memberObj, attributes.at(
                                SCENE_KEY_SCENEMAPPINGS).get<std::vector<RCSResourceAttributes>>());
                    }
                    responseAtt[SCENE_KEY_ID] = RCSResourceAttributes::Value(memberObj->getId());
                    responseAtt[SCENE_KEY_CREATEDLINK]
                                = RCSResourceAttributes::Value(memberObj->getFullUri());
                }
            }

            return RCSSetResponse::create(responseAtt, eCode).
                    setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
        }

        RCSSetResponse
        SceneCollectionResource::SceneCollectionRequestHandler::setSceneCollectionName(
                const RCSRequest & /*request*/, RCSResourceAttributes & attr)
        {
            int eCode = SCENE_CLIENT_BADREQUEST;
            SceneCollectionResource::Ptr ptr = m_owner.lock();
            if (ptr != nullptr)
            {
                eCode = SCENE_RESPONSE_SUCCESS;
                ptr->setName(attr.at(SCENE_KEY_NAME).get<std::string>());
            }

            return RCSSetResponse::create(attr, eCode).
                    setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
        }

        void SceneCollectionResource::SceneCollectionRequestHandler::addMemberInfoFromRemote(
                SceneMemberResource::Ptr memberObj, std::vector<RCSResourceAttributes> mInfo)
        {
            std::for_each(mInfo.begin(), mInfo.end(),
                    [& memberObj](const RCSResourceAttributes & att)
                    {
                        memberObj->addMappingInfo(SceneMemberResource::MappingInfo::create(att));
                    });
        }

        void SceneCollectionResource::SceneExecuteResponseHandler::
        onResponse(const RCSResourceAttributes & /*attributes*/, int errorCode)
        {
            m_responseMembers++;
            if (errorCode != SCENE_RESPONSE_SUCCESS && m_errorCode != errorCode)
            {
                m_errorCode = errorCode;
            }
            if (m_responseMembers == m_numOfMembers)
            {
                m_cb(m_errorCode);
            }
        }

        SceneCollectionResource::SceneExecuteResponseHandler::Ptr
        SceneCollectionResource::SceneExecuteResponseHandler::createExecuteHandler(
                const SceneCollectionResource::Ptr ptr, SceneExecuteCallback executeCB)
        {
            auto executeHandler = std::make_shared<SceneExecuteResponseHandler>();

            executeHandler->m_numOfMembers = ptr->m_sceneMembers.size();
            executeHandler->m_responseMembers = 0;

            executeHandler->m_cb =
                    [executeCB](int eCode)
                    {
                        std::thread(std::move(executeCB), eCode).detach();
                    };

            executeHandler->m_owner
                = std::weak_ptr<SceneCollectionResource>(ptr);
            executeHandler->m_errorCode  = SCENE_RESPONSE_SUCCESS;

            return executeHandler;
        }

    }
}
