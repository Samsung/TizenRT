//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "OCRepresentation.h"
#include "OCApi.h"
#include "octypes.h"
#include "rd_client.h"

typedef std::function<void(const OC::OCRepresentation&, const int)> PublishResourceCallback;
typedef std::function<void(const int)> DeleteResourceCallback;

using namespace OC;

namespace ServerCallbackContext
{
    struct PublishContext
    {
        PublishResourceCallback callback;
        PublishContext(PublishResourceCallback cb) : callback(cb){}
    };

    struct DeleteContext
    {
        DeleteResourceCallback callback;
        DeleteContext(DeleteResourceCallback cb) : callback(cb){}
    };
}

class RDClient
{
private:
    OCQualityOfService m_qos;
    std::shared_ptr<std::recursive_mutex> _csdkLock;
    std::weak_ptr<std::recursive_mutex> m_csdkLock;

public:
    RDClient(OCQualityOfService qos = OC_NA_QOS)
        : m_qos(qos),
        _csdkLock{ std::make_shared<std::recursive_mutex>() }
    {
        m_csdkLock = _csdkLock;
    }

    static RDClient& Instance()
    {
        static RDClient client;
        return client;
    }

    /**
     * API for Virtual Resource("/oic/d" and "/oic/p") Publish to Resource Directory.
     * @note This API applies to resource server side only.
     *
     * @param host             Host IP Address of a service to direct resource publish query.
     * @param connectivityType ::OCConnectivityType type of connectivity.
     * @param resourceHandles  This is the resource handle which we need to register to RD.
     * @param callback         Handles callbacks, success states and failure states.
     *
     * @return Returns ::OC_STACK_OK if success.
     */
    OCStackResult publishResourceToRD(const std::string& host,
                                      OCConnectivityType connectivityType,
                                      OC::ResourceHandles& resourceHandles,
                                      uint32_t ttl,
                                      PublishResourceCallback callback);

    OCStackResult publishResourceToRD(const std::string& host,
                                     OCConnectivityType connectivityType,
                                     uint32_t ttl,
                                     PublishResourceCallback callback,
                                     QualityOfService qos);

    OCStackResult publishResourceToRD(const std::string& host,
                                      OCConnectivityType connectivityType,
                                      OC::ResourceHandles& resourceHandles,
                                      uint32_t ttl,
                                      PublishResourceCallback callback,
                                      QualityOfService qos);

    OCStackResult publishResourceToRD(const std::string& host,
                                      OCConnectivityType connectivityType,
                                      OC::ResourceHandles& resourceHandles,
                                      PublishResourceCallback callback)
    {
        return publishResourceToRD(host, connectivityType, resourceHandles, OIC_RD_PUBLISH_TTL, callback);
    }

    OCStackResult publishResourceToRD(const std::string& host,
                                     OCConnectivityType connectivityType,
                                     PublishResourceCallback callback,
                                     QualityOfService qos)
    {
        return publishResourceToRD(host, connectivityType, OIC_RD_PUBLISH_TTL, callback, qos);
    }

    OCStackResult publishResourceToRD(const std::string& host,
                                      OCConnectivityType connectivityType,
                                      OC::ResourceHandles& resourceHandles,
                                      PublishResourceCallback callback,
                                      QualityOfService qos)
    {
        return publishResourceToRD(host, connectivityType, resourceHandles, OIC_RD_PUBLISH_TTL, callback, qos);
    }

    /**
     * API for published resource delete from Resource Directory.
     * @note This API applies to resource server side only.
     *
     * @param host             Host IP Address of a service to direct resource delete query.
     * @param connectivityType ::OCConnectivityType type of connectivity.
     * @param resourceHandles  This is the resource handle which we need to register to RD.
     * @param callback         Handles callbacks, success states and failure states.
     *
     * @return Returns ::OC_STACK_OK if success.
     */
    OCStackResult deleteResourceFromRD(const std::string& host,
                                        OCConnectivityType connectivityType,
                                        OC::ResourceHandles& resourceHandles,
                                        DeleteResourceCallback callback);

    OCStackResult deleteResourceFromRD(const std::string& host,
                                       OCConnectivityType connectivityType,
                                       DeleteResourceCallback callback,
                                       QualityOfService qos);

    OCStackResult deleteResourceFromRD(const std::string& host,
                                      OCConnectivityType connectivityType,
                                      OC::ResourceHandles &resourceHandles,
                                      DeleteResourceCallback callback,
                                      QualityOfService qos);

};
