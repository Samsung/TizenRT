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

#ifndef SM_REMOTE_SCENE_H_
#define SM_REMOTE_SCENE_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

#include "RemoteSceneAction.h"
#include "RCSRemoteResourceObject.h"

namespace OIC
{
    namespace Service
    {
        class SceneCollectionResourceRequestor;
        class SceneMemberResourceRequestor;

        /*
        * @class RemoteScene
        *
        * @brief RemoteScene class is an interface class to send a request to a Scene provided by
        * SceneCollection resource on remote side. This class provides APIs for adding new
        * SceneAction to the Scene and creating a new SceneAction instance, retrieving all
        * SceneAction instances created before. And it also provides an API to execute a Scene
        * on remote side.
        */
        class RemoteScene
        {
            public:
                typedef std::shared_ptr< RemoteScene > Ptr;

                /**
                * Callback definition to be invoked a the response of addNewSceneAction is
                * received.
                *
                * @param action created RemoteSceneAction instance pointer
                * @param eCode the error code received
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see addNewSceneAction
                */
                typedef std::function< void(RemoteSceneAction::Ptr action, int eCode) >
                    AddNewSceneActionCallback;

                /**
                * Callback definition to be invoked when a response of execute is
                * received.
                *
                * @param sceneName name of the scene which is executed
                * @param eCode the error code received
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see execute
                */
                typedef std::function< void(const std::string &sceneName, int eCode) >
                    RemoteSceneExecuteCallback;

            public:
                ~RemoteScene() = default;

                /**
                * Requests to add new SceneAction to the Scene on remote side and
                * creates RemoteSceneAction instance corresponding to the created SceneAction.
                *
                * @param targetResource A pointer of discovered resource
                * @param attrs AttributeS to set when the Scene executed
                * @param cb A callback to receive created RemoteSceneAction instance
                *
                * @throws RCSInvalidParameterException If parameter is invalid.
                * @throws PlatformException If the platform operation failed
                *
                * @note RemoteSceneAction instance is only produced by RemoteScene class
                *
                * @see RCSResourceAttributes
                */
                void addNewSceneAction(RCSRemoteResourceObject::Ptr targetResource,
                    const RCSResourceAttributes &attrs, AddNewSceneActionCallback cb);

                /**
                * Requests to add new SceneAction to the Scene on remote side and
                * creates RemoteSceneAction instance corresponding to the created SceneAction.
                *
                * @param targetResource A pointer of discovered resource
                * @param key A key of an attribute
                * @param value A value to be mapped against the key
                * @param cb A callback to receive created RemoteSceneAction instance
                *
                * @throws RCSInvalidParameterException If parameter is invalid.
                * @throws PlatformException If the platform operation failed
                *
                * @note RemoteSceneAction instance is only produced by RemoteScene class
                *
                * @see RCSResourceAttributes::Value
                */
                void addNewSceneAction(RCSRemoteResourceObject::Ptr targetResource,
                                       const std::string &key,
                                       const RCSResourceAttributes::Value &value,
                                       AddNewSceneActionCallback cb);

                /**
                * Gets all RemoteSceneAction instances included in the Scene.
                *
                * @return A vector of shared pointer of RemoteSceneAction instances
                */
                std::vector< RemoteSceneAction::Ptr > getRemoteSceneActions() const;

                /**
                * Gets RemoteSceneAction instance by using a certain discovered resource.
                *
                * @param targetResource A pointer of discovered resource
                *
                * @return A shared pointer of RemoteSceneAction instance
                *
                * @throws RCSInvalidParameterException If targetResource is invalid
                */
                RemoteSceneAction::Ptr getRemoteSceneAction(
                    const RCSRemoteResourceObject::Ptr targetResource) const;

                /**
                * Gets a name attribute of the Scene.
                *
                * @return A name of the Scene
                */
                std::string getName() const;

                /**
                * Requests to execute the Scene on remote side.
                *
                * @param cb A callback to receive result of Scene execution
                *
                * @throws RCSInvalidParameterException If callback is null
                * @throws PlatformException If the platform operation failed
                */
                void execute(RemoteSceneExecuteCallback cb);

            private:
                RemoteScene(
                    const std::string &name, std::shared_ptr< SceneCollectionResourceRequestor >);

                RemoteSceneAction::Ptr createRemoteSceneAction(
                    const std::string &,  const RCSResourceAttributes &);

                void addExistingRemoteSceneAction(const std::string &, const std::string &,
                    RCSRemoteResourceObject::Ptr, const std::string &key,
                    const RCSResourceAttributes::Value &);

                void onSceneActionAdded(
                    int, RCSRemoteResourceObject::Ptr,
                    const RCSResourceAttributes &, const AddNewSceneActionCallback &);

                void onSceneExecuted(const std::string &name, int,
                                    const RemoteSceneExecuteCallback &);

            private:
                std::string m_name;
                mutable std::mutex m_sceneActionLock;
                std::unordered_map < std::string, RemoteSceneAction::Ptr >
                    m_remoteSceneActions;

                std::shared_ptr< SceneCollectionResourceRequestor > m_requestor;

                friend class RemoteSceneCollection;
        };

    }
}

#endif /* SM_REMOTE_SCENE_H_ */