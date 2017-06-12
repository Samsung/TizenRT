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

#ifndef SM_REMOTE_SCENEACTION_H_
#define SM_REMOTE_SCENEACTION_H_

#include <memory>
#include <string>
#include <mutex>

#include "RCSRemoteResourceObject.h"

namespace OIC
{
    namespace Service
    {

        class SceneMemberResourceRequestor;

        /*
        * @class RemoteSceneAction
        *
        * @brief RemoteSceneAction class indicates a unit of actions when a scene is executed.
        * RemoteSceneAction instance is initialized with 3 essential parameters:
        * a target resource, target attribute key, and its target value.
        * And this class also provides APIs to update a target attribute information if one wants.
        * Note that, adding a new RemoteSceneAction is done by sending a CoAP request to update a
        * SceneMember resource's attribute.
        */
        class RemoteSceneAction
        {
            public:
                typedef std::shared_ptr< RemoteSceneAction > Ptr;

                /**
                * Callback definition to be invoked when a response of resetExecutionParameter is
                * received.
                *
                * @param eCode the error code received on a remote-side scene resource server
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see resetExecutionParameter
                */
                typedef std::function< void(int eCode) > ResetExecutionParameterCallback;

            public:
                ~RemoteSceneAction() = default;

                /**
                * Requests to reset the RemoteSceneAction parameters like
                * a target attribute key and its value.
                *
                * @param key key of attribute
                * @param value value to be mapped against the key
                * @param cb A callback to receive the response
                *
                * @throws RCSInvalidParameterException If parameter is invalid.
                * @throws PlatformException If the platform operation failed
                *
                * @see RCSResourceAttributes::Value
                */
                void resetExecutionParameter(const std::string &key,
                    const RCSResourceAttributes::Value &value, ResetExecutionParameterCallback cb);

                /**
                * Requests to reset the RemoteSceneAction parameters like
                * a target attribute key and its value.
                *
                * @param attr Attributes to set
                * @param cb A callback to receive the response
                *
                * @throws RCSInvalidParameterException If parameter is invalid.
                * @throws PlatformException If the platform operation failed
                *
                * @see RCSResourceAttributes
                */
                void resetExecutionParameter(
                    const RCSResourceAttributes &attr, ResetExecutionParameterCallback cb);

                /**
                * Returns an execution parameter of the SceneAction.
                *
                * @return RCSResourceAttributes
                */
                RCSResourceAttributes getExecutionParameter() const;

                /**
                * Returns a target remote resource object of the RemoteSceneAction instance
                *
                * @return pointer of RCSRemoteResourceObject
                */
                RCSRemoteResourceObject::Ptr getRemoteResourceObject() const;

            private:
                RemoteSceneAction(std::shared_ptr< SceneMemberResourceRequestor >,
                                  const std::string &sceneName, const RCSResourceAttributes &);
                RemoteSceneAction(std::shared_ptr< SceneMemberResourceRequestor >,
                                  const std::string &sceneName,
                                  const std::string &key, const RCSResourceAttributes::Value &);

                void onExecutionParameterSet(int, const RCSResourceAttributes &,
                    const ResetExecutionParameterCallback &);

            private:
                std::string m_sceneName;
                mutable std::mutex m_attributeLock;
                RCSResourceAttributes m_attributes;
                std::shared_ptr< SceneMemberResourceRequestor > m_requestor;

                friend class RemoteScene;
        };

    }
}

#endif /* SM_REMOTE_SCENEACTION_H_ */