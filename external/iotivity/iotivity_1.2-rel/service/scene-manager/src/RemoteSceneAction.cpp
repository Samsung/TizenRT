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

#include "RemoteSceneAction.h"

#include <cassert>

#include "SceneCommons.h"
#include "SceneMemberResourceRequestor.h"

namespace OIC
{
    namespace Service
    {

        RemoteSceneAction::RemoteSceneAction(
            SceneMemberResourceRequestor::Ptr requestor,
            const std::string &sceneName, const RCSResourceAttributes &attrs)
                : m_sceneName{ sceneName }, m_attributes{ attrs }, m_requestor{ requestor }
        {
            assert(requestor);
        }

        RemoteSceneAction::RemoteSceneAction(
            SceneMemberResourceRequestor::Ptr requestor, const std::string &sceneName,
            const std::string &key, const RCSResourceAttributes::Value &value)
                : m_sceneName{ sceneName }, m_requestor{ requestor }
        {
            assert(requestor);
            m_attributes[key] = value;
        }

        void RemoteSceneAction::resetExecutionParameter(const std::string &key,
                                       const RCSResourceAttributes::Value &value,
                                       ResetExecutionParameterCallback clientCB)
        {
            RCSResourceAttributes attr;
            attr[key] = RCSResourceAttributes::Value(value);

            resetExecutionParameter(attr, std::move(clientCB));
        }

        void RemoteSceneAction::resetExecutionParameter(const RCSResourceAttributes &attr,
            ResetExecutionParameterCallback clientCB)
        {
            if (!clientCB)
            {
                throw RCSInvalidParameterException{ "resetExecutionParameter : Callback is NULL" };
            }

            SceneMemberResourceRequestor::InternalAddSceneActionCallback internalCB
                = std::bind(&RemoteSceneAction::onExecutionParameterSet, this,
                std::placeholders::_1, attr, std::move(clientCB));

            m_requestor->requestSceneActionCreation(
                m_sceneName, attr, internalCB);
        }

        RCSResourceAttributes RemoteSceneAction::getExecutionParameter() const
        {
            std::lock_guard< std::mutex > lock(m_attributeLock);
            return m_attributes;
        }

        RCSRemoteResourceObject::Ptr RemoteSceneAction::getRemoteResourceObject() const
        {
            return m_requestor->getRemoteResourceObject();
        }

        void RemoteSceneAction::onExecutionParameterSet(int eCode, const RCSResourceAttributes &attr,
            const ResetExecutionParameterCallback &clientCB)
        {
            int result = SCENE_CLIENT_BADREQUEST;
            if (eCode == SCENE_RESPONSE_SUCCESS)
            {
                std::lock_guard< std::mutex > lock(m_attributeLock);
                m_attributes = attr;
                result = SCENE_RESPONSE_SUCCESS;
            }

            clientCB(result);
        }
    }
}
