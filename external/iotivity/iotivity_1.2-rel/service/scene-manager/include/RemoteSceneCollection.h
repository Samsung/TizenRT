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

#ifndef SM_REMOTE_SCENECOLLECTION_H_
#define SM_REMOTE_SCENECOLLECTION_H_

#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <mutex>

#include "RemoteScene.h"
#include "RCSRemoteResourceObject.h"

namespace OIC
{
    namespace Service
    {

        class SceneCollectionResourceRequestor;

        /**
        * @class RemoteSceneCollection
        *
        * @brief RemoteSceneCollection class is an interface class to send a request to
        * SceneCollection resource on remote side. This class provides APIs for adding new Scene
        * to the SceneCollection resource and creating a new RemoteSceneCollection instance
        * corresponding to the created SceneCollection resource. This class also supports
        * retrieving all Scene instances created before. Besides, it provides APIs for retrieving
        * and updating attribute values of the SceneCollection resource like name attribute.
        */
        class RemoteSceneCollection
        {
            public:
                typedef std::shared_ptr< RemoteSceneCollection > Ptr;

                /**
                * Callback definition to be invoked when a response of addNewScene is
                * received.
                *
                * @param scene created RemoteScene instance pointer
                * @param eCode the error code received from the SceneCollection on remote
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see addNewScene
                */
                typedef std::function< void(RemoteScene::Ptr scene, int eCode) >
                    AddNewSceneCallback;

                /**
                * Callback definition to be invoked when a response of setName is
                * received.
                *
                * @param eCode the error code received from the SceneCollection on remote
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see setName
                */
                typedef std::function< void(int eCode) > SetNameCallback;

            public:
                ~RemoteSceneCollection() = default;

                /**
                * Requests to add new Scene to the SceneCollection resource on remote side
                * and creates RemoteScene instance corresponding to the created Scene.
                *
                * @param name A name of Scene to add
                * @param cb A callback to receive created RemoteScene instance
                *
                * @throws RCSInvalidParameterException If parameter is invalid
                * @throws PlatformException If the platform operation failed
                *
                * @note RemoteScene instance is only produced by RemoteSceneCollection class.
                * @note Name of Scene must be unique in one SceneCollection
                */
                void addNewScene(const std::string &name, AddNewSceneCallback cb);

                /**
                * Gets all RemoteScene instances from RemoteSceneCollection instance.
                *
                * @return A unordered_map of shared pointers of RemoteScene instances
                */
                std::unordered_map< std::string, RemoteScene::Ptr > getRemoteScenes() const;

                /**
                * Gets RemoteScene instance with a specific Scene name.
                *
                * @param sceneName name of the Scene to get
                *
                * @return A shared pointer of RemoteScene instance
                *
                * @throws RCSInvalidParameterException If sceneName is invalid
                */
                RemoteScene::Ptr getRemoteScene(const std::string &sceneName) const;

                /**
                * Request to set a name attribute of the SceneCollection resource on remote side.
                *
                * @param name A name of the SceneCollection
                * @param cb A callback to receive the response
                *
                * @throws RCSInvalidParameterException If callback is null
                * @throws PlatformException If the platform operation failed
                */
                void setName(const std::string &name, SetNameCallback cb);

                /**
                * Gets a name attribute of the SceneCollection resource
                *
                * @return A name of the SceneCollection
                */
                std::string getName() const;

                /**
                * Gets an id attribute of the SceneCollection resource.
                *
                * @return an id of the SceneCollection resource
                */
                std::string getId() const;

            private:
                RemoteSceneCollection(
                    std::shared_ptr< SceneCollectionResourceRequestor >,
                    const std::string &id, const std::string &name);

                void addExistingRemoteScenes(const std::vector< std::string > &);

                void initializeRemoteScenes(const std::vector< RCSResourceAttributes > &,
                                                     const std::string &);

                RemoteScene::Ptr createRemoteScene(const std::string &);

                void onSceneAddedRemoved(int, const std::string &name, int,
                                         const AddNewSceneCallback &);

                void onNameSet(int, const std::string &, const SetNameCallback &);

            private:
                std::string m_id;
                std::string m_name;
                mutable std::mutex m_nameLock;
                mutable std::mutex m_sceneLock;
                std::unordered_map< std::string, RemoteScene::Ptr > m_remoteScenes;
                std::shared_ptr< SceneCollectionResourceRequestor > m_requestor;

                friend class RemoteSceneList;
        };

    }
}

#endif /* SM_REMOTE_SCENECOLLECTION_H_ */