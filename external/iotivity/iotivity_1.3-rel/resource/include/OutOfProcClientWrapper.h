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

#ifndef OC_OUT_OF_PROC_CLIENT_WRAPPER_H_
#define OC_OUT_OF_PROC_CLIENT_WRAPPER_H_

#include <OCApi.h>

namespace OC
{
    class OutOfProcClientWrapper : public IClientWrapper
    {
    public:
        OutOfProcClientWrapper(std::weak_ptr<std::recursive_mutex> /*csdkLock*/,
                               PlatformConfig /*cfg*/)
        {}

        virtual OCStackResult stop()
        {
            return OC_STACK_NOTIMPL;
        }

        virtual OCStackResult start()
        {
            return OC_STACK_NOTIMPL;
        }

        virtual OCStackResult ListenForResource(const std::string& /*servUrl*/,
                                                const std::string& /*rsrcType*/,
                                                OCConnectivityType /*connType*/,
                                                FindCallback& /*callback*/,
                                                QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult ListenForResourceList(const std::string& /*servUrl*/,
                                                const std::string& /*rsrcType*/,
                                                OCConnectivityType /*connType*/,
                                                FindResListCallback& /*callback*/,
                                                QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult ListenForResourceListWithError(const std::string& /*servUrl*/,
                                                const std::string&  /*rsrcType*/,
                                                OCConnectivityType /*connType*/,
                                                FindResListCallback& /*callback*/,
                                                FindErrorCallback&  /*errorCallback*/,
                                                QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult ListenErrorForResource(const std::string& /*servUrl*/,
                                                     const std::string& /*rsrcType*/,
                                                     OCConnectivityType /*connType*/,
                                                     FindCallback& /*callback*/,
                                                     FindErrorCallback& /*errorCallback*/,
                                                     QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult ListenForDevice(const std::string& /*serviceUrl*/,
                                              const std::string& /*deviceURI*/,
                                              OCConnectivityType /*connType*/,
                                              FindDeviceCallback& /*callback*/,
                                              QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult ListenForDevice(const std::string& /*serviceUrl*/,
                                              const std::string& /*deviceURI*/,
                                              FindDeviceCallback& /*callback*/,
                                              QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult GetResourceRepresentation(
            const OCDevAddr& /*devAddr*/,
            const std::string& /*uri*/,
            const QueryParamsMap& /*queryParams*/,
            const HeaderOptions& /*headerOptions*/,
            OCConnectivityType /*connectivityType*/,
            GetCallback& /*callback*/, QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult PutResourceRepresentation(
            const OCDevAddr& /*devAddr*/,
            const std::string& /*uri*/,
            const OCRepresentation& /*attributes*/,
            const QueryParamsMap& /*queryParams*/,
            const HeaderOptions& /*headerOptions*/,
            PutCallback& /*callback*/,
            QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult PostResourceRepresentation(
            const OCDevAddr& /*devAddr*/,
            const std::string& /*uri*/,
            const OCRepresentation& /*attributes*/,
            const QueryParamsMap& /*queryParams*/,
            const HeaderOptions& /*headerOptions*/,
            OCConnectivityType /*connectivityType*/,
            PostCallback& /*callback*/, QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult DeleteResource(
            const OCDevAddr& /*devAddr*/,
            const std::string& /*uri*/,
            const HeaderOptions& /*headerOptions*/,
            OCConnectivityType /*connectivityType*/,
            DeleteCallback& /*callback*/, QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult ObserveResource(
            ObserveType /*observeType*/, OCDoHandle* /*handle*/,
            const OCDevAddr& /*devAddr*/,
            const std::string& /*uri*/,
            const QueryParamsMap& /*queryParams*/,
            const HeaderOptions& /*headerOptions*/,
            ObserveCallback& /*callback*/, QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult CancelObserveResource(
            OCDoHandle /*handle*/,
            const std::string& /*host*/,
            const std::string& /*uri*/,
            const HeaderOptions& /*headerOptions*/, QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult SubscribePresence(
            OCDoHandle* /*handle*/,
            const std::string& /*host*/,
            const std::string& /*resourceType*/,
            OCConnectivityType /*connectivityType*/,
            SubscribeCallback& /*presenceHandler*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult UnsubscribePresence(OCDoHandle /*handle*/)
            {return OC_STACK_NOTIMPL;}
#ifdef WITH_CLOUD
        virtual OCStackResult SubscribeDevicePresence(
            OCDoHandle* /*handle*/,
            const std::string& /*host*/,
            const std::vector<std::string>& /*di*/,
            OCConnectivityType /*connectivityType*/,
            ObserveCallback& /*callback*/)
            {return OC_STACK_NOTIMPL;}
#endif

        virtual OCStackResult GetDefaultQos(QualityOfService& /*QoS*/)
            {return OC_STACK_NOTIMPL;}

#ifdef WITH_MQ
        virtual OCStackResult ListenForMQTopic(const OCDevAddr& /*devAddr*/,
                                               const std::string& /*resourceUri*/,
                                               const QueryParamsMap& /*queryParams*/,
                                               const HeaderOptions& /*headerOptions*/,
                                               MQTopicCallback& /*callback*/,
                                               QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}

        virtual OCStackResult PutMQTopicRepresentation(const OCDevAddr& /*devAddr*/,
                                                       const std::string& /*uri*/,
                                                       const OCRepresentation& /*rep*/,
                                                       const QueryParamsMap& /*queryParams*/,
                                                       const HeaderOptions& /*headerOptions*/,
                                                       MQTopicCallback& /*callback*/,
                                                       QualityOfService /*QoS*/)
            {return OC_STACK_NOTIMPL;}
#endif
    };
}

#endif
