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

#ifndef SM_REMOTE_SCENELIST_H_
#define SM_REMOTE_SCENELIST_H_

#include <memory>
#include <vector>
#include <functional>
#include <mutex>

#include "RemoteSceneCollection.h"
#include "RCSRemoteResourceObject.h"

namespace OIC
{
    namespace Service
    {
        class SceneListResourceRequestor;

        /**
        * @class RemoteSceneList
        *
        * @brief RemoteSceneList class is an interface class to send a request to
        * SceneList resource on remote side. This class provides APIs for adding
        * new SceneCollection resource to the SceneList resource and
        * creating a RemoteSceneCollection instance corresponding to the
        * created SceneCollection resource. This class also supports retrieving the existing
        * instances as well as setting/getting a name attribute of the SceneList resource.
        */
        class RemoteSceneList
        {
            public:
                typedef std::unique_ptr< RemoteSceneList > Ptr;

                /**
                * Callback definition to be invoked when a response of createInstance is
                * received.
                *
                * @param list Created RemoteSceneList instance pointer
                * @param eCode The error code received from the SceneList on remote side
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see createInstance
                */
                typedef std::function< void(RemoteSceneList::Ptr list, int eCode) >
                    CreateInstanceCallback;

                /**
                * Callback definition to be invoked when a response of addNewSceneCollection is
                * received.
                *
                * @param collection Created RemoteSceneCollection instance pointer
                * @param eCode The error code received from the SceneList on remote
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see addNewSceneCollection
                */
                typedef std::function< void(RemoteSceneCollection::Ptr collection, int eCode) >
                    AddNewSceneCollectionCallback;

                /**
                * Callback definition to be invoked when a response of setName is
                * received.
                *
                * @param eCode the error code received from the SceneList on remote
                *
                * @note Error code '200' stands for success, '400' for bad request,
                * and '500' for internal error.
                *
                * @see setName
                */
                typedef std::function< void(int eCode) > SetNameCallback;

            public:
                ~RemoteSceneList() = default;

                /**
                * Creates RemoteSceneList instance with provided RCSRemoteResourceObject of
                * discovered SceneList resource on remote side.
                *
                * To create RemoteSceneList instance, discovery of SceneList resource
                * which has 'oic.wk.scenelist' as resource type is required,
                * and the found resource should be provided as parameter.
                * After that, one can acceess existing SceneCollections, Scenes, and SceneActions
                * instances that are already produced at the SceneList resource.
                * Created RemoteSceneList will be delivered to CreateInstanceCallback.
                *
                * @param sceneListResource RCSRemoteResourceObject pointer of SceneList
                * @param cb A callback to receive the response
                *
                * @throws RCSInvalidParameterException If parameter is invalid.
                * @throws PlatformException If the platform operation failed
                *
                * @see RCSRemoteResourceObject
                */
                static void createInstance(
                    RCSRemoteResourceObject::Ptr sceneListResource, CreateInstanceCallback cb);

                /**
                * Requests to add new SceneCollection resource to the SceneList resource on remote
                * side and creates RemoteSceneCollection instance corresponding to the created
                * SceneCollection resource.
                *
                * @param cb A callback to receive created RemoteSceneCollection instance
                *
                * @throws RCSInvalidParameterException If callback is null.
                * @throws PlatformException If the platform operation failed
                *
                * @note RemoteSceneCollection instance is only produced by RemoteSceneList class.
                */
                void addNewSceneCollection(AddNewSceneCollectionCallback cb);

                /**
                * Gets all RemoteSceneCollection instances stored in the RemoteSceneList instance.
                *
                * @return A vector of shared pointers of RemoteSceneCollection instances
                */
                std::vector< RemoteSceneCollection::Ptr > getRemoteSceneCollections() const;

                /**
                * Request to set a name attribute of the SceneList resource on remote side.
                *
                * @param name A name of the SceneList
                * @param cb A callback to receive the response
                *
                * @throws RCSInvalidParameterException If callback is null.
                * @throws PlatformException If the platform operation failed
                */
                void setName(const std::string &name, SetNameCallback cb);

                /**
                * Gets a name attribute of the SceneList resource.
                *
                * @return A name of the SceneList resource
                */
                std::string getName() const;

            private:
                RemoteSceneList(std::shared_ptr< SceneListResourceRequestor >);

                static void onInstanceCreated(const RCSRepresentation &, int, const std::string &,
                    std::shared_ptr< SceneListResourceRequestor >, const CreateInstanceCallback &);

                static RemoteSceneList::Ptr buildSceneList(
                    std::shared_ptr< SceneListResourceRequestor >, const RCSResourceAttributes &);

                RemoteSceneCollection::Ptr createRemoteSceneCollection(
                    const std::string &link, const std::string &id, const std::string &name);

                std::shared_ptr< SceneListResourceRequestor > getListResourceRequestor() const;

                std::vector<std::pair<RCSResourceAttributes, std::vector<RCSResourceAttributes>>>
                    parseSceneListFromAttributes(const RCSResourceAttributes &);

                std::vector<RCSResourceAttributes> getChildrenAttributes(
                    const RCSResourceAttributes &) const;

                void onSceneCollectionCreated(
                    const std::string &link, const std::string &id, const std::string &name,
                    int, const AddNewSceneCollectionCallback &);

                void onNameSet(int, const std::string &, const SetNameCallback &);

            private:
                std::string m_name;
                std::vector< RemoteSceneCollection::Ptr > m_remoteSceneCollections;
                mutable std::mutex m_nameLock;
                mutable std::mutex m_collectionLock;
                std::shared_ptr< SceneListResourceRequestor > m_requestor;
        };

    }
}

#endif /* SM_REMOTE_SCENELIST_H_ */