/* *****************************************************************
 *
 * Copyright 2017 Microsoft
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

#include "OCPlatform_impl.h"
#include <OCPlatform.h>
#include <OCResource.h>
#include <string>
#include "ipcatestdata.h"

namespace OC
{
    // This is mocked because OCPlatform_impl instantiates the client and the server wrappers.
    // There is no implementation in this file since the mock directly links the client and server
    // apps.
    InProcClientWrapper::InProcClientWrapper(
                            std::weak_ptr<std::recursive_mutex> csdkLock,
                            PlatformConfig cfg) :
        m_threadRun(false),
        m_csdkLock(csdkLock),
        m_cfg { cfg }
    {
    }

    InProcClientWrapper::~InProcClientWrapper()
    {
    }

    OCStackResult InProcClientWrapper::start()
    {
       return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::stop()
    {
       return OC_STACK_OK;
    }

    void InProcClientWrapper::listeningFunc()
    {
    }

    OCStackResult InProcClientWrapper::ListenForResource(
                                                const std::string& serviceUrl,
                                                const std::string& resourceType,
                                                OCConnectivityType connectivityType,
                                                FindCallback& callback,
                                                QualityOfService QoS)
    {

        OC_UNUSED(serviceUrl);
        OC_UNUSED(resourceType);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::ListenErrorForResource(
                                                const std::string& serviceUrl,
                                                const std::string& resourceType,
                                                OCConnectivityType connectivityType,
                                                FindCallback& callback,
                                                FindErrorCallback& errorCallback,
                                                QualityOfService QoS)
    {
        OC_UNUSED(serviceUrl);
        OC_UNUSED(resourceType);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);
        OC_UNUSED(errorCallback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }


    OCStackResult InProcClientWrapper::ListenForResourceList(
                                                const std::string& serviceUrl,
                                                const std::string& resourceType,
                                                OCConnectivityType connectivityType,
                                                FindResListCallback& callback,
                                                QualityOfService QoS)
    {
        OC_UNUSED(serviceUrl);
        OC_UNUSED(resourceType);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::ListenForResourceListWithError(
                                                const std::string& serviceUrl,
                                                const std::string& resourceType,
                                                OCConnectivityType connectivityType,
                                                FindResListCallback& callback,
                                                FindErrorCallback& errorCallback,
                                                QualityOfService QoS)
    {
        OC_UNUSED(serviceUrl);
        OC_UNUSED(resourceType);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);
        OC_UNUSED(errorCallback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::ListenForDevice(
                                                const std::string& serviceUrl,
                                                const std::string& deviceURI,
                                                OCConnectivityType connectivityType,
                                                FindDeviceCallback& callback,
                                                QualityOfService QoS)
    {
        OC_UNUSED(serviceUrl);
        OC_UNUSED(deviceURI);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::GetResourceRepresentation(
                                                const OCDevAddr& devAddr,
                                                const std::string& resourceUri,
                                                const QueryParamsMap& queryParams,
                                                const HeaderOptions& headerOptions,
                                                OCConnectivityType connectivityType,
                                                GetCallback& callback,
                                                QualityOfService QoS)
    {
        OC_UNUSED(devAddr);
        OC_UNUSED(resourceUri);
        OC_UNUSED(queryParams);
        OC_UNUSED(headerOptions);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::PutResourceRepresentation(
                                                const OCDevAddr& devAddr,
                                                const std::string& uri,
                                                const OCRepresentation& rep,
                                                const QueryParamsMap& queryParams,
                                                const HeaderOptions& headerOptions,
                                                PutCallback& callback,
                                                QualityOfService QoS)
    {
        OC_UNUSED(devAddr);
        OC_UNUSED(uri);
        OC_UNUSED(rep);
        OC_UNUSED(queryParams);
        OC_UNUSED(headerOptions);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::PostResourceRepresentation(
                                                const OCDevAddr& devAddr,
                                                const std::string& uri,
                                                const OCRepresentation& rep,
                                                const QueryParamsMap& queryParams,
                                                const HeaderOptions& headerOptions,
                                                OCConnectivityType connectivityType,
                                                PostCallback& callback,
                                                QualityOfService QoS)
    {
        OC_UNUSED(devAddr);
        OC_UNUSED(uri);
        OC_UNUSED(rep);
        OC_UNUSED(queryParams);
        OC_UNUSED(headerOptions);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }


    OCStackResult InProcClientWrapper::DeleteResource(
                                                const OCDevAddr& devAddr,
                                                const std::string& uri,
                                                const HeaderOptions& headerOptions,
                                                OCConnectivityType connectivityType,
                                                DeleteCallback& callback,
                                                QualityOfService /*QoS*/)
    {
        OC_UNUSED(devAddr);
        OC_UNUSED(uri);
        OC_UNUSED(headerOptions);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);

        return OC_STACK_OK;
    }


    OCStackResult InProcClientWrapper::ObserveResource(
                                                ObserveType observeType,
                                                OCDoHandle* handle,
                                                const OCDevAddr& devAddr,
                                                const std::string& uri,
                                                const QueryParamsMap& queryParams,
                                                const HeaderOptions& headerOptions,
                                                ObserveCallback& callback,
                                                QualityOfService QoS)

    {
        OC_UNUSED(observeType);
        OC_UNUSED(handle);
        OC_UNUSED(devAddr);
        OC_UNUSED(uri);
        OC_UNUSED(queryParams);
        OC_UNUSED(headerOptions);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::CancelObserveResource(
            OCDoHandle handle,
            const std::string& /*host*/,
            const std::string& /*uri*/,
            const HeaderOptions& headerOptions,
            QualityOfService QoS)

    {
        OC_UNUSED(handle);
        OC_UNUSED(headerOptions);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::SubscribePresence(
                                                OCDoHandle* handle,
                                                const std::string& host,
                                                const std::string& resourceType,
                                                OCConnectivityType connectivityType,
                                                SubscribeCallback& presenceHandler)
    {
        OC_UNUSED(handle);
        OC_UNUSED(host);
        OC_UNUSED(resourceType);
        OC_UNUSED(connectivityType);
        OC_UNUSED(presenceHandler);

        return OC_STACK_OK;
    }


    OCStackResult InProcClientWrapper::UnsubscribePresence(OCDoHandle handle)
    {
        OC_UNUSED(handle);
        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::GetDefaultQos(QualityOfService& qos)
    {
        OC_UNUSED(qos);
        return OC_STACK_OK;
    }

#ifdef WITH_CLOUD
    OCStackResult InProcClientWrapper::SubscribeDevicePresence(
                                                OCDoHandle* handle,
                                                const std::string& host,
                                                const std::vector<std::string>& di,
                                                OCConnectivityType connectivityType,
                                                ObserveCallback& callback)
    {
        OC_UNUSED(handle);
        OC_UNUSED(host);
        OC_UNUSED(di);
        OC_UNUSED(connectivityType);
        OC_UNUSED(callback);

       return OC_STACK_OK;
    }
#endif

#ifdef WITH_MQ
    OCStackResult InProcClientWrapper::ListenForMQTopic(
                                                        const OCDevAddr& devAddr,
                                                        const std::string& resourceUri,
                                                        const QueryParamsMap& queryParams,
                                                        const HeaderOptions& headerOptions,
                                                        MQTopicCallback& callback,
                                                        QualityOfService QoS)
    {
        OC_UNUSED(devAddr);
        OC_UNUSED(resourceUri);
        OC_UNUSED(queryParams);
        OC_UNUSED(headerOptions);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
    }

    OCStackResult InProcClientWrapper::PutMQTopicRepresentation(
                                                        const OCDevAddr& devAddr,
                                                        const std::string& uri,
                                                        const OCRepresentation& rep,
                                                        const QueryParamsMap& queryParams,
                                                        const HeaderOptions& headerOptions,
                                                        MQTopicCallback& callback,
                                                        QualityOfService QoS)
   {
        OC_UNUSED(devAddr);
        OC_UNUSED(uri);
        OC_UNUSED(rep);
        OC_UNUSED(queryParams);
        OC_UNUSED(headerOptions);
        OC_UNUSED(callback);
        OC_UNUSED(QoS);

        return OC_STACK_OK;
   }
#endif
}
