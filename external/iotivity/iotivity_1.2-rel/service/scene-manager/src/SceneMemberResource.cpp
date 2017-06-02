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

#include "SceneMemberResource.h"

#include <atomic>
#include "OCPlatform.h"

namespace OIC
{
    namespace Service
    {
        namespace
        {
            std::atomic_int g_numOfSceneMember(0);
        }

        SceneMemberResource::Ptr
        SceneMemberResource::createSceneMemberResource(
                RCSRemoteResourceObject::Ptr remoteObject)
        {
            SceneMemberResource::Ptr sceneMemberResource(new SceneMemberResource());

            sceneMemberResource->m_uri = PREFIX_SCENE_MEMBER_URI + "/" +
                std::to_string(g_numOfSceneMember++);

            sceneMemberResource->m_remoteMemberObj = remoteObject;

            sceneMemberResource->createResourceObject();
            sceneMemberResource->initSetRequestHandler();
            sceneMemberResource->setDefaultAttributes();

            return sceneMemberResource;
        }

        SceneMemberResource::Ptr
        SceneMemberResource::createSceneMemberResource(const RCSResourceAttributes & link)
        {
            return createSceneMemberResource(RCSResourceAttributes(link));
        }

        SceneMemberResource::Ptr
        SceneMemberResource::createSceneMemberResource(RCSResourceAttributes && link)
        {
            auto href = link.at(SCENE_KEY_HREF).get<std::string>();

            std::string address;
            std::string uri;

            SceneUtils::getHostUriString(href, &address, &uri);

            auto ocResourcePtr
                = OC::OCPlatform::constructResourceObject(
                    address, uri, OCConnectivityType::CT_ADAPTER_IP, false,
                    link.at(SCENE_KEY_RT).get<std::vector<std::string>>(),
                    link.at(SCENE_KEY_IF).get<std::vector<std::string>>());

            return createSceneMemberResource(RCSRemoteResourceObject::fromOCResource(ocResourcePtr));
        }

        void SceneMemberResource::createResourceObject()
        {
            m_sceneMemberResourceObj
                = RCSResourceObject::Builder(
                        m_uri, SCENE_MEMBER_RT, OC_RSRVD_INTERFACE_DEFAULT).
                        setDiscoverable(true).setObservable(false).build();
        }

        void SceneMemberResource::setDefaultAttributes()
        {
            m_sceneMemberResourceObj->setAttribute(SCENE_KEY_ID, SceneUtils::OICGenerateUUIDStr());
            m_sceneMemberResourceObj->setAttribute(SCENE_KEY_NAME, std::string());

            RCSResourceAttributes subAtt;
            subAtt[SCENE_KEY_HREF]
                    = RCSResourceAttributes::Value(
                            m_remoteMemberObj->getAddress() + m_remoteMemberObj->getUri());
            subAtt[SCENE_KEY_IF] = RCSResourceAttributes::Value(m_remoteMemberObj->getInterfaces());
            subAtt[SCENE_KEY_RT] = RCSResourceAttributes::Value(m_remoteMemberObj->getTypes());
            m_sceneMemberResourceObj->setAttribute(SCENE_KEY_PAYLOAD_LINK, subAtt);

            m_sceneMemberResourceObj->setAttribute(
                    SCENE_KEY_SCENEMAPPINGS, std::vector<RCSResourceAttributes>());
            m_sceneMemberResourceObj->setAttribute(SCENE_KEY_URI, m_uri);
        }

        void SceneMemberResource::initSetRequestHandler()
        {
            m_requestHandler.m_owner = std::weak_ptr<SceneMemberResource>(shared_from_this());
            m_sceneMemberResourceObj->setSetRequestHandler(std::bind(
                    &SceneMemberResource::SceneMemberRequestHandler::onSetRequest,
                    m_requestHandler, std::placeholders::_1, std::placeholders::_2));
        }

        void SceneMemberResource::addMappingInfo(MappingInfo && mInfo)
        {
            RCSResourceAttributes newAtt;
            {
                RCSResourceObject::LockGuard guard(m_sceneMemberResourceObj);
                newAtt = m_sceneMemberResourceObj->getAttributes();
            }

            auto mappingInfo = newAtt.at(SCENE_KEY_SCENEMAPPINGS).
                    get<std::vector<RCSResourceAttributes>>();

            auto foundMInfo = std::find_if(mappingInfo.begin(), mappingInfo.end(),
                    [& mInfo](const RCSResourceAttributes & att) -> bool
                    {
                        return (att.at(SCENE_KEY_SCENE).get<std::string>() == mInfo.sceneName) &&
                                (att.at(SCENE_KEY_MEMBERPROPERTY).get<std::string>() == mInfo.key);
                    });

            if (foundMInfo != mappingInfo.end())
            {
                mappingInfo.erase(foundMInfo);
            }
            RCSResourceAttributes newMapInfo;
            newMapInfo[SCENE_KEY_SCENE] = RCSResourceAttributes::Value(std::move(mInfo.sceneName));
            newMapInfo[SCENE_KEY_MEMBERPROPERTY] = RCSResourceAttributes::Value(std::move(mInfo.key));
            newMapInfo[SCENE_KEY_MEMBERVALUE] = std::move(mInfo.value);
            mappingInfo.push_back(newMapInfo);

            m_sceneMemberResourceObj->setAttribute(SCENE_KEY_SCENEMAPPINGS, mappingInfo);
        }

        void SceneMemberResource::addMappingInfo(const MappingInfo & mInfo)
        {
            addMappingInfo(MappingInfo(mInfo));
        }

        std::vector<SceneMemberResource::MappingInfo> SceneMemberResource::getMappingInfos() const
        {
            std::vector<MappingInfo> retMInfo;

            auto mInfo = m_sceneMemberResourceObj->getAttributeValue(SCENE_KEY_SCENEMAPPINGS).
                    get<std::vector<RCSResourceAttributes>>();
            std::for_each(mInfo.begin(), mInfo.end(),
                    [& retMInfo](const RCSResourceAttributes & att)
                    {
                        retMInfo.push_back(MappingInfo::create(att));
                    });

            return retMInfo;
        }

        std::string SceneMemberResource::getId() const
        {
            return m_sceneMemberResourceObj->getAttributeValue(SCENE_KEY_ID).get<std::string>();
        }

        std::string SceneMemberResource::getFullUri() const
        {
            return std::string(COAP_TAG + SceneUtils::getNetAddress() + m_uri);
        }

        std::string SceneMemberResource::getTargetUri() const
        {
            return std::string(m_remoteMemberObj->getAddress() + m_remoteMemberObj->getUri());
        }

        RCSRemoteResourceObject::Ptr SceneMemberResource::getRemoteResourceObject() const
        {
            return m_remoteMemberObj;
        }

        RCSResourceObject::Ptr SceneMemberResource::getRCSResourceObject() const
        {
            return m_sceneMemberResourceObj;
        }

        void SceneMemberResource::execute(std::string && sceneName)
        {
            execute(std::move(sceneName), nullptr);
        }

        void SceneMemberResource::execute(const std::string & sceneName)
        {
            execute(std::string(sceneName));
        }

        void SceneMemberResource::execute(std::string && sceneName, MemberexecuteCallback executeCB)
        {
            RCSResourceAttributes setAtt;

            auto mInfo = getMappingInfos();
            std::for_each(mInfo.begin(), mInfo.end(),
                    [& setAtt, & sceneName](const MappingInfo & info)
                    {
                        if(info.sceneName == sceneName)
                        {
                            setAtt[info.key] = info.value;
                        }
                    });

            if (setAtt.empty() && executeCB != nullptr)
            {
                executeCB(RCSResourceAttributes(), SCENE_RESPONSE_SUCCESS);
            }

            m_remoteMemberObj->setRemoteAttributes(setAtt, executeCB);
        }

        void SceneMemberResource::execute(
                const std::string & sceneName, MemberexecuteCallback executeCB)
        {
            execute(std::string(sceneName), std::move(executeCB));
        }

        void SceneMemberResource::setName(const std::string & name)
        {
            setName(std::string(name));
        }

        void SceneMemberResource::setName(std::string && name)
        {
            m_sceneMemberResourceObj->setAttribute(SCENE_KEY_NAME, std::move(name));
        }

        std::string SceneMemberResource::getName() const
        {
            return m_sceneMemberResourceObj->getAttributeValue(SCENE_KEY_NAME).toString();
        }

        std::vector<SceneMemberResource::MappingInfo> SceneMemberResource::findMappingInfos(
                const std::string & sceneValue) const
        {
            auto mInfo = getMappingInfos();
            std::vector<MappingInfo> retMInfo;

            std::for_each(mInfo.begin(), mInfo.end(),
                    [& retMInfo, & sceneValue](const MappingInfo & info)
                    {
                        if(info.sceneName == sceneValue)
                        {
                            retMInfo.push_back(MappingInfo(info));
                        }
                    });
            return retMInfo;
        }

        bool SceneMemberResource::hasSceneValue(const std::string & sceneValue) const
        {
            auto mInfo = getMappingInfos();
            if (std::find_if(mInfo.begin(), mInfo.end(),
                    [& sceneValue](const MappingInfo & info) -> bool
                    {
                        return info.sceneName == sceneValue;
                    }) != mInfo.end())
            {
                return true;
            }
            return false;
        }

        SceneMemberResource::MappingInfo
        SceneMemberResource::MappingInfo::create(const RCSResourceAttributes & att)
        {
            return MappingInfo(att.at(SCENE_KEY_SCENE).get<std::string>(),
                    att.at(SCENE_KEY_MEMBERPROPERTY).get<std::string>(),
                    att.at(SCENE_KEY_MEMBERVALUE));
        }

        RCSSetResponse SceneMemberResource::SceneMemberRequestHandler::
        onSetRequest(const RCSRequest & request, RCSResourceAttributes & attributes)
        {
            if (attributes.contains(SCENE_KEY_SCENEMAPPINGS))
            {
                addMappingInfos(request, attributes);
            }

            if (attributes.contains(SCENE_KEY_NAME))
            {
                setSceneMemberName(request, attributes);
            }

            return RCSSetResponse::create(attributes, SCENE_CLIENT_BADREQUEST).
                    setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
        }

        RCSSetResponse
        SceneMemberResource::SceneMemberRequestHandler::addMappingInfos(
                const RCSRequest & /*request*/, RCSResourceAttributes & attributes)
        {
            int eCode = SCENE_RESPONSE_SUCCESS;
            auto ptr = m_owner.lock();
            if (!ptr)
            {
                eCode = SCENE_CLIENT_BADREQUEST;
            }
            else
            {
                auto mInfo = attributes.at(SCENE_KEY_SCENEMAPPINGS).
                        get<std::vector<RCSResourceAttributes>>();
                std::for_each(mInfo.begin(), mInfo.end(),
                        [& ptr](const RCSResourceAttributes & att)
                        {
                            ptr->addMappingInfo(SceneMemberResource::MappingInfo::create(att));
                        });
            }

            return RCSSetResponse::create(attributes, eCode).
                    setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
        }

        RCSSetResponse
        SceneMemberResource::SceneMemberRequestHandler::setSceneMemberName(
                const RCSRequest & /*request*/, RCSResourceAttributes & attributes)
        {
            int eCode = SCENE_RESPONSE_SUCCESS;
            auto ptr = m_owner.lock();
            if (!ptr)
            {
                eCode = SCENE_CLIENT_BADREQUEST;
            }
            else
            {
                ptr->setName(attributes.at(SCENE_KEY_NAME).get<std::string>());
            }

            return RCSSetResponse::create(attributes, eCode).
                    setAcceptanceMethod(RCSSetResponse::AcceptanceMethod::IGNORE);
        }
    }
}
