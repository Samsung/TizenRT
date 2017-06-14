//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#include "InProcClientWrapper.h"
#include "ocstack.h"

#include "OCPlatform.h"
#include "OCResource.h"
#include "ocpayload.h"
#include <OCSerialization.h>
#include "logger.h"
#ifdef TCP_ADAPTER
#include "oickeepalive.h"
#endif

#define TAG "OIC_CLIENT_WRAPPER"

using namespace std;

namespace OC
{
    InProcClientWrapper::InProcClientWrapper(
        std::weak_ptr<std::recursive_mutex> csdkLock, PlatformConfig cfg)
            : m_threadRun(false), m_csdkLock(csdkLock),
              m_cfg { cfg }
    {
        // if the config type is server, we ought to never get called.  If the config type
        // is both, we count on the server to run the thread and do the initialize
        start();
    }

    InProcClientWrapper::~InProcClientWrapper()
    {
        try
        {
            stop();
        }
        catch (InitializeException &e)
        {
            oclog() << "Exception in stop"<< e.what() << std::flush;
        }
    }

    OCStackResult InProcClientWrapper::start()
    {
        OIC_LOG_V(INFO, TAG, "start ocplatform for client : %d", m_cfg.transportType);

        if (m_cfg.mode == ModeType::Client)
        {
            OCTransportFlags serverFlags =
                            static_cast<OCTransportFlags>(m_cfg.serverConnectivity & CT_MASK_FLAGS);
            OCTransportFlags clientFlags =
                            static_cast<OCTransportFlags>(m_cfg.clientConnectivity & CT_MASK_FLAGS);
            OCStackResult result = OCInit2(OC_CLIENT, serverFlags, clientFlags,
                                           m_cfg.transportType);

            if (OC_STACK_OK != result)
            {
                throw InitializeException(OC::InitException::STACK_INIT_ERROR, result);
            }

            if (false == m_threadRun)
            {
                m_threadRun = true;
                m_listeningThread = std::thread(&InProcClientWrapper::listeningFunc, this);
            }
        }
        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::stop()
    {
        OIC_LOG(INFO, TAG, "stop ocplatform");

        if (m_threadRun && m_listeningThread.joinable())
        {
            m_threadRun = false;
            m_listeningThread.join();
        }

        // only stop if we are the ones who actually called 'start'.  We are counting
        // on the server to do the stop.
        if (m_cfg.mode == ModeType::Client)
        {
            OCStackResult result = OCStop();

            if (OC_STACK_OK != result)
            {
               throw InitializeException(OC::InitException::STACK_TERMINATE_ERROR, result);
            }
        }
        return OC_STACK_OK;
    }

    void InProcClientWrapper::listeningFunc()
    {
        while(m_threadRun)
        {
            OCStackResult result;
            auto cLock = m_csdkLock.lock();
            if (cLock)
            {
                std::lock_guard<std::recursive_mutex> lock(*cLock);
                result = OCProcess();
            }
            else
            {
                result = OC_STACK_ERROR;
            }

            if (result != OC_STACK_OK)
            {
                // TODO: do something with result if failed?
            }

            // To minimize CPU utilization we may wish to do this with sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    OCRepresentation parseGetSetCallback(OCClientResponse* clientResponse)
    {
        if (clientResponse->payload == nullptr ||
                (
                    clientResponse->payload->type != PAYLOAD_TYPE_REPRESENTATION
                )
          )
        {
            //OCPayloadDestroy(clientResponse->payload);
            return OCRepresentation();
        }

        MessageContainer oc;
        oc.setPayload(clientResponse->payload);
        //OCPayloadDestroy(clientResponse->payload);

        std::vector<OCRepresentation>::const_iterator it = oc.representations().begin();
        if (it == oc.representations().end())
        {
            return OCRepresentation();
        }

        // first one is considered the root, everything else is considered a child of this one.
        OCRepresentation root = *it;
        root.setDevAddr(clientResponse->devAddr);
        root.setUri(clientResponse->resourceUri);
        ++it;

        std::for_each(it, oc.representations().end(),
                [&root](const OCRepresentation& repItr)
                {root.addChild(repItr);});
        return root;
    }

    OCStackApplicationResult listenCallback(void* ctx, OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        if (!ctx || !clientResponse)
        {
            return OC_STACK_KEEP_TRANSACTION;
        }

        ClientCallbackContext::ListenContext* context =
            static_cast<ClientCallbackContext::ListenContext*>(ctx);

        if (clientResponse->result != OC_STACK_OK)
        {
            oclog() << "listenCallback(): failed to create resource. clientResponse: "
                    << clientResponse->result
                    << std::flush;

            return OC_STACK_KEEP_TRANSACTION;
        }

        if (!clientResponse->payload || clientResponse->payload->type != PAYLOAD_TYPE_DISCOVERY)
        {
            oclog() << "listenCallback(): clientResponse payload was null or the wrong type"
                << std::flush;
            return OC_STACK_KEEP_TRANSACTION;
        }

        auto clientWrapper = context->clientWrapper.lock();

        if (!clientWrapper)
        {
            oclog() << "listenCallback(): failed to get a shared_ptr to the client wrapper"
                    << std::flush;
            return OC_STACK_KEEP_TRANSACTION;
        }

        try
        {
            ListenOCContainer container(clientWrapper, clientResponse->devAddr,
                                    reinterpret_cast<OCDiscoveryPayload*>(clientResponse->payload));
            // loop to ensure valid construction of all resources

            for(auto resource : container.Resources())
            {
                std::thread exec(context->callback, resource);
                exec.detach();
            }
        }
        catch (std::exception &e)
        {
            oclog() << "Exception in listCallback, ignoring response: "
                    << e.what() << std::flush;
        }


        return OC_STACK_KEEP_TRANSACTION;
    }

    OCStackApplicationResult listenErrorCallback(void* ctx, OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        if (!ctx || !clientResponse)
        {
            return OC_STACK_KEEP_TRANSACTION;
        }

        ClientCallbackContext::ListenErrorContext* context =
            static_cast<ClientCallbackContext::ListenErrorContext*>(ctx);

        OCStackResult result = clientResponse->result;
        if (result == OC_STACK_OK)
        {
            if (!clientResponse->payload || clientResponse->payload->type != PAYLOAD_TYPE_DISCOVERY)
            {
                oclog() << "listenCallback(): clientResponse payload was null or the wrong type"
                    << std::flush;
                return OC_STACK_KEEP_TRANSACTION;
            }

            auto clientWrapper = context->clientWrapper.lock();

            if (!clientWrapper)
            {
                oclog() << "listenCallback(): failed to get a shared_ptr to the client wrapper"
                        << std::flush;
                return OC_STACK_KEEP_TRANSACTION;
            }

            ListenOCContainer container(clientWrapper, clientResponse->devAddr,
                                        reinterpret_cast<OCDiscoveryPayload*>(clientResponse->payload));
            // loop to ensure valid construction of all resources
            for (auto resource : container.Resources())
            {
                std::thread exec(context->callback, resource);
                exec.detach();
            }
            return OC_STACK_KEEP_TRANSACTION;
        }

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::string resourceURI = clientResponse->resourceUri;
        std::thread exec(context->errorCallback, resourceURI, result);
        exec.detach();
        return OC_STACK_KEEP_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::ListenForResource(
            const std::string& serviceUrl,
            const std::string& resourceType,
            OCConnectivityType connectivityType,
            FindCallback& callback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result;
        ostringstream resourceUri;
        resourceUri << serviceUrl << resourceType;

        ClientCallbackContext::ListenContext* context =
            new ClientCallbackContext::ListenContext(callback, shared_from_this());
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(context),
        cbdata.cb      = listenCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::ListenContext*)c;};

        auto cLock = m_csdkLock.lock();
        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCDoResource(nullptr, OC_REST_DISCOVER,
                                  resourceUri.str().c_str(),
                                  nullptr, nullptr, connectivityType,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  nullptr, 0);
        }
        else
        {
            delete context;
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult InProcClientWrapper::ListenErrorForResource(
            const std::string& serviceUrl,
            const std::string& resourceType,
            OCConnectivityType connectivityType,
            FindCallback& callback, FindErrorCallback& errorCallback,
            QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }

        ostringstream resourceUri;
        resourceUri << serviceUrl << resourceType;

        ClientCallbackContext::ListenErrorContext* context =
            new ClientCallbackContext::ListenErrorContext(callback, errorCallback,
                                                          shared_from_this());
        if (!context)
        {
            return OC_STACK_ERROR;
        }

        OCCallbackData cbdata(
                static_cast<void*>(context),
                listenErrorCallback,
                [](void* c){delete static_cast<ClientCallbackContext::ListenErrorContext*>(c);}
            );

        OCStackResult result;
        auto cLock = m_csdkLock.lock();
        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCDoResource(nullptr, OC_REST_DISCOVER,
                                  resourceUri.str().c_str(),
                                  nullptr, nullptr, connectivityType,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  nullptr, 0);
        }
        else
        {
            delete context;
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackApplicationResult listenResListCallback(void* ctx, OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        if (!ctx || !clientResponse)
        {
            return OC_STACK_KEEP_TRANSACTION;
        }

        ClientCallbackContext::ListenResListContext* context =
            static_cast<ClientCallbackContext::ListenResListContext*>(ctx);

        if (clientResponse->result != OC_STACK_OK)
        {
            oclog() << "listenResListCallback(): failed to create resource. clientResponse: "
                    << clientResponse->result
                    << std::flush;

            return OC_STACK_KEEP_TRANSACTION;
        }

        if (!clientResponse->payload || clientResponse->payload->type != PAYLOAD_TYPE_DISCOVERY)
        {
            oclog() << "listenResListCallback(): clientResponse payload was null or the wrong type"
                << std::flush;
            return OC_STACK_KEEP_TRANSACTION;
        }

        auto clientWrapper = context->clientWrapper.lock();

        if (!clientWrapper)
        {
            oclog() << "listenResListCallback(): failed to get a shared_ptr to the client wrapper"
                    << std::flush;
            return OC_STACK_KEEP_TRANSACTION;
        }

        try
        {
            ListenOCContainer container(clientWrapper, clientResponse->devAddr,
                                    reinterpret_cast<OCDiscoveryPayload*>(clientResponse->payload));

            OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
            std::thread exec(context->callback, container.Resources());
            exec.detach();
        }
        catch (std::exception &e)
        {
            oclog() << "Exception in listenResListCallback(), ignoring response: "
                    << e.what() << std::flush;
        }

        return OC_STACK_KEEP_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::ListenForResourceList(
            const std::string& serviceUrl,
            const std::string& resourceType,
            OCConnectivityType connectivityType,
            FindResListCallback& callback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result;
        ostringstream resourceUri;
        resourceUri << serviceUrl << resourceType;

        ClientCallbackContext::ListenResListContext* context =
            new ClientCallbackContext::ListenResListContext(callback, shared_from_this());
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(context),
        cbdata.cb      = listenResListCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::ListenResListContext*)c;};

        auto cLock = m_csdkLock.lock();
        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCDoResource(nullptr, OC_REST_DISCOVER,
                                  resourceUri.str().c_str(),
                                  nullptr, nullptr, connectivityType,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  nullptr, 0);
        }
        else
        {
            delete context;
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackApplicationResult listenResListWithErrorCallback(void* ctx, OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        if (!ctx || !clientResponse)
        {
            return OC_STACK_KEEP_TRANSACTION;
        }

        ClientCallbackContext::ListenResListWithErrorContext* context =
                static_cast<ClientCallbackContext::ListenResListWithErrorContext*>(ctx);

        OCStackResult result = clientResponse->result;
        if (result != OC_STACK_OK)
        {
            oclog() << "listenResListWithErrorCallback(): failed to create resource. clientResponse: "
                    << result << std::flush;

             //send the error callback
            std::string uri = clientResponse->resourceUri;
            std::thread exec(context->errorCallback, uri, result);
            exec.detach();
            return OC_STACK_KEEP_TRANSACTION;
        }

        if (!clientResponse->payload || clientResponse->payload->type != PAYLOAD_TYPE_DISCOVERY)
        {
            oclog() << "listenResListWithErrorCallback(): clientResponse payload was null or the wrong type"
                << std::flush;
            return OC_STACK_KEEP_TRANSACTION;
        }

        auto clientWrapper = context->clientWrapper.lock();

        if (!clientWrapper)
        {
            oclog() << "listenResListWithErrorCallback(): failed to get a shared_ptr to the client wrapper"
                    << std::flush;
            return OC_STACK_KEEP_TRANSACTION;
        }

        try
        {
            ListenOCContainer container(clientWrapper, clientResponse->devAddr,
                            reinterpret_cast<OCDiscoveryPayload*>(clientResponse->payload));

            OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
            std::thread exec(context->callback, container.Resources());
            exec.detach();
        }
        catch (std::exception &e)
        {
            oclog() << "Exception in listenResListWithErrorCallback(), ignoring response: "
            << e.what() << std::flush;
        }

        return OC_STACK_KEEP_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::ListenForResourceListWithError(
            const std::string& serviceUrl,
            const std::string& resourceType,
            OCConnectivityType connectivityType,
            FindResListCallback& callback,
            FindErrorCallback& errorCallback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result;
        ostringstream resourceUri;
        resourceUri << serviceUrl << resourceType;

        ClientCallbackContext::ListenResListWithErrorContext* context =
            new ClientCallbackContext::ListenResListWithErrorContext(callback, errorCallback,
                                                          shared_from_this());
        if (!context)
        {
            return OC_STACK_ERROR;
        }

        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(context),
        cbdata.cb      = listenResListWithErrorCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::ListenResListWithErrorContext*)c;};

        auto cLock = m_csdkLock.lock();
        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCDoResource(nullptr, OC_REST_DISCOVER,
                                  resourceUri.str().c_str(),
                                  nullptr, nullptr, connectivityType,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  nullptr, 0);
        }
        else
        {
            delete context;
            result = OC_STACK_ERROR;
        }
        return result;
    }

#ifdef WITH_MQ
    OCStackApplicationResult listenMQCallback(void* ctx, OCDoHandle /*handle*/,
                                              OCClientResponse* clientResponse)
    {
        ClientCallbackContext::MQTopicContext* context =
            static_cast<ClientCallbackContext::MQTopicContext*>(ctx);

        if (!clientResponse || !context)
        {
            return OC_STACK_DELETE_TRANSACTION;
        }

        std::string resourceURI = clientResponse->resourceUri;
        if (clientResponse->result != OC_STACK_OK)
        {
            oclog() << "listenMQCallback(): failed to create resource. clientResponse: "
                    << clientResponse->result
                    << std::flush;

            std::thread exec(context->callback, clientResponse->result,
                             resourceURI, nullptr);
            exec.detach();

            return OC_STACK_DELETE_TRANSACTION;
        }

        auto clientWrapper = context->clientWrapper.lock();
        if (!clientWrapper)
        {
            oclog() << "listenMQCallback(): failed to get a shared_ptr to the client wrapper"
                    << std::flush;
            return OC_STACK_DELETE_TRANSACTION;
        }

        try
        {
            ListenOCContainer container(clientWrapper, clientResponse->devAddr,
                                        (OCRepPayload *) clientResponse->payload);

            // loop to ensure valid construction of all resources
            for (auto resource : container.Resources())
            {
                std::thread exec(context->callback, clientResponse->result,
                                 resourceURI, resource);
                exec.detach();
            }
        }
        catch (std::exception &e)
        {
            oclog() << "Exception in listCallback, ignoring response: "
                    << e.what() << std::flush;
        }

        return OC_STACK_DELETE_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::ListenForMQTopic(const OCDevAddr& devAddr,
                                                        const std::string& resourceUri,
                                                        const QueryParamsMap& queryParams,
                                                        const HeaderOptions& headerOptions,
                                                        MQTopicCallback& callback,
                                                        QualityOfService QoS)
    {
        oclog() << "ListenForMQTopic()" << std::flush;

        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }

        ClientCallbackContext::MQTopicContext* context =
            new ClientCallbackContext::MQTopicContext(callback, shared_from_this());
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(context),
        cbdata.cb      = listenMQCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::MQTopicContext*)c;};

        std::string uri = assembleSetResourceUri(resourceUri, queryParams);

        OCStackResult result = OC_STACK_ERROR;
        auto cLock = m_csdkLock.lock();
        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCHeaderOption options[MAX_HEADER_OPTIONS];
            result = OCDoResource(
                                  nullptr, OC_REST_GET,
                                  uri.c_str(),
                                  &devAddr, nullptr,
                                  CT_DEFAULT,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  assembleHeaderOptions(options, headerOptions),
                                  headerOptions.size());
        }
        else
        {
            delete context;
        }

        return result;
    }
#endif

    OCStackApplicationResult listenDeviceCallback(void* ctx,
                                                  OCDoHandle /*handle*/,
            OCClientResponse* clientResponse)
    {
        ClientCallbackContext::DeviceListenContext* context =
            static_cast<ClientCallbackContext::DeviceListenContext*>(ctx);

        try
        {
            OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
            OCRepresentation rep = parseGetSetCallback(clientResponse);
            std::thread exec(context->callback, rep);
            exec.detach();
        }
        catch(OC::OCException& e)
        {
            oclog() <<"Exception in listenDeviceCallback, ignoring response: "
                <<e.what() <<std::flush;
        }

        return OC_STACK_KEEP_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::ListenForDevice(
            const std::string& serviceUrl,
            const std::string& deviceURI,
            OCConnectivityType connectivityType,
            FindDeviceCallback& callback,
            QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;
        ostringstream deviceUri;
        deviceUri << serviceUrl << deviceURI;

        ClientCallbackContext::DeviceListenContext* context =
            new ClientCallbackContext::DeviceListenContext(callback, shared_from_this());
        OCCallbackData cbdata;

        cbdata.context = static_cast<void*>(context),
        cbdata.cb      = listenDeviceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::DeviceListenContext*)c;};

        auto cLock = m_csdkLock.lock();
        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCDoResource(nullptr, OC_REST_DISCOVER,
                                  deviceUri.str().c_str(),
                                  nullptr, nullptr, connectivityType,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  nullptr, 0);
        }
        else
        {
            delete context;
            result = OC_STACK_ERROR;
        }
        return result;
    }

    void parseServerHeaderOptions(OCClientResponse* clientResponse,
                    HeaderOptions& serverHeaderOptions)
    {
        if (clientResponse)
        {
            // Parse header options from server
            uint16_t optionID;
            std::string optionData;

            for(int i = 0; i < clientResponse->numRcvdVendorSpecificHeaderOptions; i++)
            {
                optionID = clientResponse->rcvdVendorSpecificHeaderOptions[i].optionID;
                optionData = reinterpret_cast<const char*>
                                (clientResponse->rcvdVendorSpecificHeaderOptions[i].optionData);
                HeaderOption::OCHeaderOption headerOption(optionID, optionData);
                serverHeaderOptions.push_back(headerOption);
            }
        }
        else
        {
            // clientResponse is invalid
            // TODO check proper logging
            std::cout << " Invalid response " << std::endl;
        }
    }

#ifdef WITH_MQ
    OCStackApplicationResult createMQTopicCallback(void* ctx, OCDoHandle /*handle*/,
                    OCClientResponse* clientResponse)
    {
        ClientCallbackContext::MQTopicContext* context =
            static_cast<ClientCallbackContext::MQTopicContext*>(ctx);
        HeaderOptions serverHeaderOptions;

        if (!clientResponse || !context)
        {
            return OC_STACK_DELETE_TRANSACTION;
        }

        std::string createdUri;
        bool isLocationOption = false;
        OCStackResult result = clientResponse->result;
        if (OC_STACK_OK               == result ||
            OC_STACK_RESOURCE_CREATED == result)
        {
            parseServerHeaderOptions(clientResponse, serverHeaderOptions);

            for (auto headerOption : serverHeaderOptions)
            {
                if (HeaderOption::LOCATION_PATH_OPTION_ID == headerOption.getOptionID())
                {
                    createdUri += "/";
                    createdUri += headerOption.getOptionData();
                    if (!isLocationOption)
                    {
                        isLocationOption = true;
                    }
                }
            }
        }

        if (!isLocationOption)
        {
            createdUri = std::string(clientResponse->resourceUri);
        }

        auto clientWrapper = context->clientWrapper.lock();

        if (!clientWrapper)
        {
            oclog() << "createMQTopicCallback(): failed to get a shared_ptr to the client wrapper"
                    << std::flush;
            return OC_STACK_DELETE_TRANSACTION;
        }

        try
        {
            if (OC_STACK_OK               == result ||
                OC_STACK_RESOURCE_CREATED == result)
            {
                ListenOCContainer container(clientWrapper, clientResponse->devAddr,
                                            createdUri);
                for (auto resource : container.Resources())
                {
                    std::thread exec(context->callback, result,
                                     createdUri,
                                     resource);
                    exec.detach();
                }
            }
            else
            {
                OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
                std::thread exec(context->callback, result,
                                 createdUri,
                                 nullptr);
                exec.detach();
            }
        }
        catch (std::exception &e)
        {
            oclog() << "Exception in createMQTopicCallback, ignoring response: "
                    << e.what() << std::flush;
        }
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::PutMQTopicRepresentation(
                const OCDevAddr& devAddr,
                const std::string& uri,
                const OCRepresentation& rep,
                const QueryParamsMap& queryParams, const HeaderOptions& headerOptions,
                MQTopicCallback& callback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;
        ClientCallbackContext::MQTopicContext* ctx =
                new ClientCallbackContext::MQTopicContext(callback, shared_from_this());
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = createMQTopicCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::MQTopicContext*)c;};

        std::string url = assembleSetResourceUri(uri, queryParams);

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCHeaderOption options[MAX_HEADER_OPTIONS];

            result = OCDoResource(nullptr, OC_REST_PUT,
                                  url.c_str(), &devAddr,
                                  assembleSetResourcePayload(rep),
                                  CT_DEFAULT,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  assembleHeaderOptions(options, headerOptions),
                                  headerOptions.size());
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }

        return result;
    }
#endif
    OCStackApplicationResult getResourceCallback(void* ctx,
                                                 OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        ClientCallbackContext::GetContext* context =
            static_cast<ClientCallbackContext::GetContext*>(ctx);

        OCRepresentation rep;
        HeaderOptions serverHeaderOptions;
        OCStackResult result = clientResponse->result;

        parseServerHeaderOptions(clientResponse, serverHeaderOptions);
        try
        {
            rep = parseGetSetCallback(clientResponse);
        }
        catch(OC::OCException& e)
        {
            result = e.code();
        }

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::thread exec(context->callback, serverHeaderOptions, rep, result);
        exec.detach();
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::GetResourceRepresentation(
        const OCDevAddr& devAddr,
        const std::string& resourceUri,
        const QueryParamsMap& queryParams, const HeaderOptions& headerOptions,
        OCConnectivityType connectivityType,
        GetCallback& callback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;
        ClientCallbackContext::GetContext* ctx =
            new ClientCallbackContext::GetContext(callback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = getResourceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::GetContext*)c;};


        std::string uri = assembleSetResourceUri(resourceUri, queryParams);

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCHeaderOption options[MAX_HEADER_OPTIONS];

            result = OCDoResource(
                                  nullptr, OC_REST_GET,
                                  uri.c_str(),
                                  &devAddr, nullptr,
                                  connectivityType,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  assembleHeaderOptions(options, headerOptions),
                                  headerOptions.size());
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }
        return result;
    }


    OCStackApplicationResult setResourceCallback(void* ctx,
                                                 OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        ClientCallbackContext::SetContext* context =
            static_cast<ClientCallbackContext::SetContext*>(ctx);
        OCRepresentation attrs;
        HeaderOptions serverHeaderOptions;

        OCStackResult result = clientResponse->result;

        parseServerHeaderOptions(clientResponse, serverHeaderOptions);
        try
        {
            attrs = parseGetSetCallback(clientResponse);
        }
        catch(OC::OCException& e)
        {
            result = e.code();
        }

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::thread exec(context->callback, serverHeaderOptions, attrs, result);
        exec.detach();
        return OC_STACK_DELETE_TRANSACTION;
    }

    std::string InProcClientWrapper::assembleSetResourceUri(std::string uri,
        const QueryParamsMap& queryParams)
    {
        if (!uri.empty())
        {
            if (uri.back() == '/')
            {
                uri.resize(uri.size() - 1);
            }
        }

        ostringstream paramsList;
        if (queryParams.size() > 0)
        {
            paramsList << '?';
        }

        for (auto& param : queryParams)
        {
            paramsList << param.first <<'='<<param.second<<';';
        }

        std::string queryString = paramsList.str();

        if (queryString.empty())
        {
            return uri;
        }

        if (queryString.back() == ';')
        {
            queryString.resize(queryString.size() - 1);
        }

        std::string ret = uri + queryString;
        return ret;
    }

    std::string InProcClientWrapper::assembleSetResourceUri(std::string uri,
        const QueryParamsList& queryParams)
    {
        if (!uri.empty())
        {
            if (uri.back() == '/')
            {
                uri.resize(uri.size() - 1);
            }
        }

        ostringstream paramsList;
        if (queryParams.size() > 0)
        {
            paramsList << '?';
        }

        for (auto& param : queryParams)
        {
            for (auto& paramList : param.second)
            {
                paramsList << param.first << '=' << paramList << ';';
            }
        }

        std::string queryString = paramsList.str();

        if (queryString.empty())
        {
            return uri;
        }

        if (queryString.back() == ';')
        {
            queryString.resize(queryString.size() - 1);
        }

        std::string ret = uri + queryString;
        return ret;
    }

    OCPayload* InProcClientWrapper::assembleSetResourcePayload(const OCRepresentation& rep)
    {
        MessageContainer ocInfo;
        ocInfo.addRepresentation(rep);
        for(const OCRepresentation& r : rep.getChildren())
        {
            ocInfo.addRepresentation(r);
        }

        return reinterpret_cast<OCPayload*>(ocInfo.getPayload());
    }

    OCStackResult InProcClientWrapper::PostResourceRepresentation(
        const OCDevAddr& devAddr,
        const std::string& uri,
        const OCRepresentation& rep,
        const QueryParamsMap& queryParams, const HeaderOptions& headerOptions,
        OCConnectivityType connectivityType,
        PostCallback& callback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;
        ClientCallbackContext::SetContext* ctx = new ClientCallbackContext::SetContext(callback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = setResourceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::SetContext*)c;};


        std::string url = assembleSetResourceUri(uri, queryParams);

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCHeaderOption options[MAX_HEADER_OPTIONS];

            result = OCDoResource(nullptr, OC_REST_POST,
                                  url.c_str(), &devAddr,
                                  assembleSetResourcePayload(rep),
                                  connectivityType,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  assembleHeaderOptions(options, headerOptions),
                                  headerOptions.size());
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }

        return result;
    }

    OCStackResult InProcClientWrapper::PutResourceRepresentation(
        const OCDevAddr& devAddr,
        const std::string& uri,
        const OCRepresentation& rep,
        const QueryParamsMap& queryParams, const HeaderOptions& headerOptions,
        PutCallback& callback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;
        ClientCallbackContext::SetContext* ctx = new ClientCallbackContext::SetContext(callback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = setResourceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::SetContext*)c;};


        std::string url = assembleSetResourceUri(uri, queryParams).c_str();

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCDoHandle handle;
            OCHeaderOption options[MAX_HEADER_OPTIONS];

            result = OCDoResource(&handle, OC_REST_PUT,
                                  url.c_str(), &devAddr,
                                  assembleSetResourcePayload(rep),
                                  CT_DEFAULT,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  assembleHeaderOptions(options, headerOptions),
                                  headerOptions.size());
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }

        return result;
    }

    OCStackApplicationResult deleteResourceCallback(void* ctx,
                                                    OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        ClientCallbackContext::DeleteContext* context =
            static_cast<ClientCallbackContext::DeleteContext*>(ctx);
        HeaderOptions serverHeaderOptions;

        parseServerHeaderOptions(clientResponse, serverHeaderOptions);

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::thread exec(context->callback, serverHeaderOptions, clientResponse->result);
        exec.detach();
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::DeleteResource(
        const OCDevAddr& devAddr,
        const std::string& uri,
        const HeaderOptions& headerOptions,
        OCConnectivityType connectivityType,
        DeleteCallback& callback,
        QualityOfService /*QoS*/)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;
        ClientCallbackContext::DeleteContext* ctx =
            new ClientCallbackContext::DeleteContext(callback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = deleteResourceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::DeleteContext*)c;};


        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            OCHeaderOption options[MAX_HEADER_OPTIONS];

            std::lock_guard<std::recursive_mutex> lock(*cLock);

            result = OCDoResource(nullptr, OC_REST_DELETE,
                                  uri.c_str(), &devAddr,
                                  nullptr,
                                  connectivityType,
                                  static_cast<OCQualityOfService>(m_cfg.QoS),
                                  &cbdata,
                                  assembleHeaderOptions(options, headerOptions),
                                  headerOptions.size());
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }

        return result;
    }

    OCStackApplicationResult observeResourceCallback(void* ctx,
                                                     OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        ClientCallbackContext::ObserveContext* context =
            static_cast<ClientCallbackContext::ObserveContext*>(ctx);
        OCRepresentation attrs;
        HeaderOptions serverHeaderOptions;
        uint32_t sequenceNumber = clientResponse->sequenceNumber;
        OCStackResult result = clientResponse->result;

        parseServerHeaderOptions(clientResponse, serverHeaderOptions);
        try
        {
            attrs = parseGetSetCallback(clientResponse);
        }
        catch(OC::OCException& e)
        {
            result = e.code();
        }

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::thread exec(context->callback, serverHeaderOptions, attrs,
                    result, sequenceNumber);
        exec.detach();
        if (sequenceNumber == MAX_SEQUENCE_NUMBER + 1)
        {
            return OC_STACK_DELETE_TRANSACTION;
        }

        return OC_STACK_KEEP_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::ObserveResource(ObserveType observeType, OCDoHandle* handle,
        const OCDevAddr& devAddr,
        const std::string& uri,
        const QueryParamsMap& queryParams, const HeaderOptions& headerOptions,
        ObserveCallback& callback, QualityOfService QoS)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;

        ClientCallbackContext::ObserveContext* ctx =
            new ClientCallbackContext::ObserveContext(callback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = observeResourceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::ObserveContext*)c;};


        OCMethod method;
        if (observeType == ObserveType::Observe)
        {
            method = OC_REST_OBSERVE;
        }
        else if (observeType == ObserveType::ObserveAll)
        {
            method = OC_REST_OBSERVE_ALL;
        }
        else
        {
            method = OC_REST_OBSERVE_ALL;
        }

        std::string url = assembleSetResourceUri(uri, queryParams).c_str();

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCHeaderOption options[MAX_HEADER_OPTIONS];

            result = OCDoResource(handle, method,
                                  url.c_str(), &devAddr,
                                  nullptr,
                                  CT_DEFAULT,
                                  static_cast<OCQualityOfService>(QoS),
                                  &cbdata,
                                  assembleHeaderOptions(options, headerOptions),
                                  headerOptions.size());
        }
        else
        {
            delete ctx;
            return OC_STACK_ERROR;
        }

        return result;
    }

    OCStackResult InProcClientWrapper::CancelObserveResource(
            OCDoHandle handle,
            const std::string& /*host*/,
            const std::string& /*uri*/,
            const HeaderOptions& headerOptions,
            QualityOfService QoS)
    {
        OCStackResult result;
        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCHeaderOption options[MAX_HEADER_OPTIONS];

            result = OCCancel(handle,
                    static_cast<OCQualityOfService>(QoS),
                    assembleHeaderOptions(options, headerOptions),
                    headerOptions.size());
        }
        else
        {
            result = OC_STACK_ERROR;
        }

        return result;
    }

    OCStackApplicationResult subscribePresenceCallback(void* ctx,
                                                       OCDoHandle /*handle*/,
            OCClientResponse* clientResponse)
    {
        ClientCallbackContext::SubscribePresenceContext* context =
        static_cast<ClientCallbackContext::SubscribePresenceContext*>(ctx);

        /*
         * This a hack while we rethink presence subscription.
         */
        std::string url = clientResponse->devAddr.addr;

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::thread exec(context->callback, clientResponse->result,
                    clientResponse->sequenceNumber, url);

        exec.detach();

        return OC_STACK_KEEP_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::SubscribePresence(OCDoHandle* handle,
        const std::string& host, const std::string& resourceType,
        OCConnectivityType connectivityType, SubscribeCallback& presenceHandler)
    {
        if (!presenceHandler)
        {
            return OC_STACK_INVALID_PARAM;
        }

        ClientCallbackContext::SubscribePresenceContext* ctx =
            new ClientCallbackContext::SubscribePresenceContext(presenceHandler);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = subscribePresenceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::SubscribePresenceContext*)c;};


        auto cLock = m_csdkLock.lock();

        std::ostringstream os;
        os << host << OC_RSRVD_PRESENCE_URI;

        if (!resourceType.empty())
        {
            os << "?rt=" << resourceType;
        }

        if (!cLock)
        {
            delete ctx;
            return OC_STACK_ERROR;
        }

        return OCDoResource(handle, OC_REST_PRESENCE,
                            os.str().c_str(), nullptr,
                            nullptr, connectivityType,
                            OC_LOW_QOS, &cbdata, NULL, 0);
    }

    OCStackResult InProcClientWrapper::UnsubscribePresence(OCDoHandle handle)
    {
        OCStackResult result;
        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCancel(handle, OC_LOW_QOS, NULL, 0);
        }
        else
        {
            result = OC_STACK_ERROR;
        }

        return result;
    }

#ifdef WITH_CLOUD
    OCStackResult InProcClientWrapper::SubscribeDevicePresence(OCDoHandle* handle,
                                                               const std::string& host,
                                                               const std::vector<std::string>& di,
                                                               OCConnectivityType connectivityType,
                                                               ObserveCallback& callback)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }
        OCStackResult result;

        ClientCallbackContext::ObserveContext* ctx =
            new ClientCallbackContext::ObserveContext(callback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = observeResourceCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::ObserveContext*)c;};

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);

            std::ostringstream os;
            os << host << OC_RSRVD_DEVICE_PRESENCE_URI;
            QueryParamsList queryParams({{OC_RSRVD_DEVICE_ID, di}});
            std::string url = assembleSetResourceUri(os.str(), queryParams);

            result = OCDoResource(handle, OC_REST_OBSERVE,
                                  url.c_str(), nullptr,
                                  nullptr, connectivityType,
                                  OC_LOW_QOS, &cbdata,
                                  nullptr, 0);
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }

        return result;
    }
#endif

    OCStackResult InProcClientWrapper::GetDefaultQos(QualityOfService& qos)
    {
        qos = m_cfg.QoS;
        return OC_STACK_OK;
    }

    OCHeaderOption* InProcClientWrapper::assembleHeaderOptions(OCHeaderOption options[],
           const HeaderOptions& headerOptions)
    {
        int i = 0;

        if ( headerOptions.size() == 0)
        {
            return nullptr;
        }

        for (auto it=headerOptions.begin(); it != headerOptions.end(); ++it)
        {
            options[i] = OCHeaderOption();
            options[i].protocolID = OC_COAP_ID;
            options[i].optionID = it->getOptionID();
            options[i].optionLength = it->getOptionData().length() + 1;
            strcpy((char*)options[i].optionData, (it->getOptionData().c_str()));
            i++;
        }

        return options;
    }

    std::shared_ptr<OCDirectPairing> cloneDevice(const OCDPDev_t* dev)
    {
        if (!dev)
        {
            return nullptr;
        }

        OCDPDev_t* result = new OCDPDev_t(*dev);
        result->prm = new OCPrm_t[dev->prmLen];
        memcpy(result->prm, dev->prm, sizeof(OCPrm_t)*dev->prmLen);
        return std::shared_ptr<OCDirectPairing>(new OCDirectPairing(result));
    }

    void InProcClientWrapper::convert(const OCDPDev_t *list, PairedDevices& dpList)
    {
        while(list)
        {
            dpList.push_back(cloneDevice(list));
            list = list->next;
        }
    }

    OCStackResult InProcClientWrapper::FindDirectPairingDevices(unsigned short waittime,
            GetDirectPairedCallback& callback)
    {
        if (!callback || 0 == waittime)
        {
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result = OC_STACK_ERROR;
        const OCDPDev_t *list = nullptr;
        PairedDevices dpDeviceList;

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);

            list = OCDiscoverDirectPairingDevices(waittime);
            if (NULL == list)
            {
                result = OC_STACK_NO_RESOURCE;
                oclog() << "findDirectPairingDevices(): No device found for direct pairing"
                    << std::flush;
            }
            else {
                OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
                convert(list, dpDeviceList);
                std::thread exec(callback, dpDeviceList);
                exec.detach();
                result = OC_STACK_OK;
            }
        }
        else
        {
            result = OC_STACK_ERROR;
        }

        return result;
    }

    OCStackResult InProcClientWrapper::GetDirectPairedDevices(GetDirectPairedCallback& callback)
    {
        if (!callback)
        {
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result = OC_STACK_ERROR;
        const OCDPDev_t *list = nullptr;
        PairedDevices dpDeviceList;

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);

            list = OCGetDirectPairedDevices();
            if (NULL == list)
            {
                result = OC_STACK_NO_RESOURCE;
                oclog() << "findDirectPairingDevices(): No device found for direct pairing"
                    << std::flush;
            }
            else {
                OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
                convert(list, dpDeviceList);
                std::thread exec(callback, dpDeviceList);
                exec.detach();
                result = OC_STACK_OK;
            }
        }
        else
        {
            result = OC_STACK_ERROR;
        }

        return result;
    }

    void directPairingCallback(void *ctx, OCDPDev_t *peer,
            OCStackResult result)
    {

        ClientCallbackContext::DirectPairingContext* context =
            static_cast<ClientCallbackContext::DirectPairingContext*>(ctx);

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::thread exec(context->callback, cloneDevice(peer), result);
        exec.detach();
    }

    OCStackResult InProcClientWrapper::DoDirectPairing(std::shared_ptr<OCDirectPairing> peer,
            const OCPrm_t& pmSel, const std::string& pinNumber, DirectPairingCallback& callback)
    {
        if (!peer || !callback)
        {
            oclog() << "Invalid parameters" << std::flush;
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result = OC_STACK_ERROR;
        ClientCallbackContext::DirectPairingContext* context =
            new ClientCallbackContext::DirectPairingContext(callback);

        auto cLock = m_csdkLock.lock();
        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCDoDirectPairing(static_cast<void*>(context), peer->getDev(),
                    pmSel, const_cast<char*>(pinNumber.c_str()), directPairingCallback);
        }
        else
        {
            delete context;
            result = OC_STACK_ERROR;
        }
        return result;
    }
#ifdef TCP_ADAPTER
    OCStackApplicationResult KeepAliveRespCallback(void* ctx,
                                                 OCDoHandle /*handle*/,
        OCClientResponse* clientResponse)
    {
        ClientCallbackContext::KeepAliveContext* context =
            static_cast<ClientCallbackContext::KeepAliveContext*>(ctx);
        OCRepresentation attrs;
        OCStackResult result = clientResponse->result;

        try
        {
            attrs = parseGetSetCallback(clientResponse);
        }
        catch(OC::OCException& e)
        {
            result = e.code();
        }

        OIC_LOG_V(DEBUG, TAG, "%s: call response callback", __func__);
        std::thread exec(context->callback, result, attrs);
        exec.detach();
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCStackResult InProcClientWrapper::findKeepAliveResource(std::string host,
                                                             KeepAliveCallback resultCallback)
    {
        if (host.empty() || !resultCallback)
        {
            oclog() << "Invalid parameters" << std::flush;
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result = OC_STACK_ERROR;

        ClientCallbackContext::KeepAliveContext* ctx =
                       new ClientCallbackContext::KeepAliveContext(resultCallback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = KeepAliveRespCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::KeepAliveContext*)c;};

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCFindKeepAliveResource(nullptr, host.c_str(), &cbdata);
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult InProcClientWrapper::sendKeepAliveRequest(std::string host,
                                                            const OCRepresentation& rep,
                                                            KeepAliveCallback resultCallback)
    {
        if (!resultCallback)
        {
            oclog() << "Invalid parameters" << std::flush;
            return OC_STACK_INVALID_PARAM;
        }

        OCStackResult result = OC_STACK_ERROR;

        ClientCallbackContext::KeepAliveContext* ctx = new ClientCallbackContext::KeepAliveContext(resultCallback);
        OCCallbackData cbdata;
        cbdata.context = static_cast<void*>(ctx),
        cbdata.cb      = KeepAliveRespCallback;
        cbdata.cd      = [](void* c){delete (ClientCallbackContext::KeepAliveContext*)c;};

        auto cLock = m_csdkLock.lock();

        if (cLock)
        {
            std::lock_guard<std::recursive_mutex> lock(*cLock);
            OCRepPayload *payload = rep.getPayload();
            result = OCSendKeepAliveRequest (nullptr, host.c_str(), (OCPayload*)payload, &cbdata);
            if (result != OC_STACK_OK)
            {
                OCRepPayloadDestroy(payload);
            }
        }
        else
        {
            delete ctx;
            result = OC_STACK_ERROR;
        }
        return result;
    }
#endif
}
