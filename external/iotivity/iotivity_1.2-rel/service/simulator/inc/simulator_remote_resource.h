/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/**
 * @file simulator_remote_resource.h
 *
 * @brief This file provides a class for handling discovered resources.
 *
 */

#ifndef SIMULATOR_REMOTE_RESOURCE_H_
#define SIMULATOR_REMOTE_RESOURCE_H_

#include "simulator_client_types.h"
#include "simulator_resource_model.h"
#include "simulator_request_model.h"
#include "simulator_uncopyable.h"
#include "simulator_exceptions.h"

/**
 * @class   SimulatorRemoteResource
 * @brief   This class provides a API for handling discovered resources.
 */
class SimulatorRemoteResource : private UnCopyable
{
    public:

        /**
         * Callback method for receiving response for GET, PUT and POST requests.
         *
         * @param uid - Identifier of remote resource.
         * @param result - Result of the request mapped to one of the enum value
         * in @SimulatorResult.
         * @param resModel - Resource representation model.
         */
        typedef std::function<void (const std::string &uid, SimulatorResult result,
                                    const SimulatorResourceModel &resModel)>
        ResponseCallback;

        typedef ResponseCallback GetResponseCallback;
        typedef ResponseCallback PutResponseCallback;
        typedef ResponseCallback PostResponseCallback;
        typedef ResponseCallback DeleteResponseCallback;

        /**
         * Callback method for receiving representation change notifications from remote resource.
         *
         * @param uid - Identifier of remote resource.
         * @param result - Result of the request mapped to one of the enum value
         * in @SimulatorResult.
         * @param resModel - Resource representation model.
         * @param id - Notificaiton sequence id.
         */
        typedef std::function<void (const std::string &uid, SimulatorResult result,
                                    const SimulatorResourceModel &resModel, int id)>
        ObserveNotificationCallback;

        /**
         * Callback method for receiving auto request generation and verifiction progress state.
         *
         * @param uid - Identifier of remote resource.
         * @param id - Auto request generation id.
         * @param sate - Auto request generation and validation state.
         */
        typedef std::function<void(const std::string &uid, int id, OperationState state)>
        AutoRequestGenerationCallback;

        /**
         * API for getting URI of resource.
         *
         * @return URI of resource.
         */
        virtual std::string getURI() const = 0;

        /**
         * API for getting host address of resource.
         *
         * @return Host address of resource.
         */
        virtual std::string getHost() const = 0;

        /**
         * API for getting unique id of resource.
         *
         * @return ID of resource.
         */
        virtual std::string getID() const = 0;


        /**
         * API for getting connectivity type of resource.
         *
         * @return enum SimulatorConnectivityType value
         */
        virtual SimulatorConnectivityType getConnectivityType() const = 0;

        /**
         * API for getting resource types bound with the resource.
         *
         * @return vector of strings representing resource types.
         */
        virtual std::vector < std::string > getResourceTypes() const = 0;

        /**
         * API for getting interface types bound with the resource.
         *
         * @return vector of strings representing interface types.
         */
        virtual std::vector < std::string > getInterface() const = 0;

        /**
         * API to check whether resource can be observed or not.
         *
         * @return true if resource is observable, otherwise false.
         */
        virtual bool isObservable() const = 0;

        /**
         * API to send observe request to remote resource.
         *
         * @param type - Observe request type.
         * @param callback - callback for receiving notifications from remote resource
         * asynchronously.
         */
        virtual void observe(ObserveType type, ObserveNotificationCallback callback) = 0;

        /**
         * API to send cancel observe request to remote resource.
         *
         */
        virtual void cancelObserve() = 0;

        /**
         * API to send GET request to remote resource.
         *
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void get(const GetResponseCallback &callback) = 0;

        /**
         * API to send GET request to remote resource.
         *
         * @param queryParams - Query parameters string.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void get(const std::map<std::string, std::string> &queryParams,
                         const GetResponseCallback &) = 0;

        /**
         * API to send GET request to remote resource.
         *
         * @param interfaceType - Interace type on which request to be sent.
         * @param queryParams - Query parameters string.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void get(const std::string &interfaceType,
                         const std::map<std::string, std::string> &queryParams,
                         const GetResponseCallback &) = 0;

        /**
         * API to send PUT request to remote resource.
         *
         * @param representation - Resource representation to be sent with request.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void put(const SimulatorResourceModel &representation,
                         const PutResponseCallback &callback) = 0;


        /**
         * API to send PUT request to remote resource.
         *
         * @param queryParams - Query parameters string.
         * @param representation - Resource representation to be sent with request.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void put(const std::map<std::string, std::string> &queryParams,
                         const SimulatorResourceModel &representation,
                         const PutResponseCallback &callback) = 0;

        /**
         * API to send PUT request to remote resource.
         *
         * @param interfaceType - Interace type on which request to be sent.
         * @param queryParams - Query parameters string.
         * @param representation - Resource representation to be sent with request.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void put(const std::string &interfaceType,
                         const std::map<std::string, std::string> &queryParams,
                         const SimulatorResourceModel &representation,
                         const PutResponseCallback &callback) = 0;

        /**
         * API to send POST request to remote resource.
         *
         * @param representation - Resource representation to be sent with request.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void post(const SimulatorResourceModel &representation,
                          const PostResponseCallback &callback) = 0;

        /**
         * API to send POST request to remote resource.
         *
         * @param queryParams - Query parameters string.
         * @param representation - Resource representation to be sent with request.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void post(const std::map<std::string, std::string> &queryParams,
                          const SimulatorResourceModel &representation,
                          const PostResponseCallback &callback) = 0;

        /**
         * API to send POST request to remote resource.
         *
         * @param interfaceType - Interace type on which request to be sent.
         * @param queryParams - Query parameters string.
         * @param representation - Resource representation to be sent with request.
         * @param callback - callback for receiving response from remote resource
         * asynchronously.
         */
        virtual void post(const std::string &interfaceType,
                          const std::map<std::string, std::string> &queryParams,
                          const SimulatorResourceModel &representation,
                          const PostResponseCallback &callback) = 0;

        /**
         * API to configure resource from RAML file.
         *
         * @param path - Path to RAML file.
         *
         * @return Map of request models representing the format of requests.
         */
        virtual std::map<RequestType, SimulatorRequestModel> configure(
            const std::string &path) = 0;

        /**
         * API to start generating requests and send to remote resource.
         *
         * @param type - Request type.
         * @param callback - callback for receiving progress state of auto request
         * generation process.
         *
         * @return Identifier of auto request generating session. This id should be used
         * for stopping the same.
         */
        virtual int startAutoRequesting(RequestType type,
                                        AutoRequestGenerationCallback callback) = 0;

        /**
         * API to stop generating requests and send to remote resource.
         *
         * @param id - Identifier of auto request generating session.
         */
        virtual void stopAutoRequesting(int id) = 0;
};

typedef std::shared_ptr<SimulatorRemoteResource> SimulatorRemoteResourceSP;

#endif

