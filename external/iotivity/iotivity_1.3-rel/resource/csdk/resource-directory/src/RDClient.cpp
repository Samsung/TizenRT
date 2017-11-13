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

#include <thread>
#include <mutex>

#ifdef RD_CLIENT

#include "OCApi.h"
#include "OCRepresentation.h"
#include "OCResourceRequest.h"
#include "OCResourceResponse.h"
#include "OCPlatform.h"
#include "OCException.h"
#include "ocpayload.h"

#include "RDClient.h"

using namespace OC;


OCRepresentation parseRDResponseCallback(OCClientResponse* clientResponse)
{
    if (nullptr == clientResponse || nullptr == clientResponse->payload ||
        PAYLOAD_TYPE_REPRESENTATION != clientResponse->payload->type)
    {
        return OCRepresentation();
    }

    MessageContainer oc;
    oc.setPayload(clientResponse->payload);

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

OCStackApplicationResult publishResourceToRDCallback(void* ctx, OCDoHandle /*handle*/,
                                                     OCClientResponse* clientResponse)
{
    ServerCallbackContext::PublishContext* context =
    static_cast<ServerCallbackContext::PublishContext*>(ctx);

    try
    {
        if (clientResponse)
        {
            OCRepresentation rep = parseRDResponseCallback(clientResponse);
            std::thread exec(context->callback, rep, clientResponse->result);
            exec.detach();
        }
    }
    catch (OC::OCException& e)
    {
        oclog() <<"Exception in publishResourceToRDCallback, ignoring response: "
            <<e.what() <<std::flush;
    }

    return OC_STACK_KEEP_TRANSACTION;
}

OCStackResult RDClient::publishResourceToRD(const std::string& host,
                                            OCConnectivityType connectivityType,
                                            ResourceHandles& resourceHandles,
                                            uint32_t ttl,
                                            PublishResourceCallback callback)
{
    return publishResourceToRD(host, connectivityType, resourceHandles, ttl, callback, static_cast<QualityOfService>(m_qos));
}

OCStackResult RDClient::publishResourceToRD(const std::string& host,
                                            OCConnectivityType connectivityType,
                                            uint32_t ttl,
                                            PublishResourceCallback callback,
                                            QualityOfService qos)
{
    ResourceHandles resourceHandles;
    return publishResourceToRD(host, connectivityType, resourceHandles, ttl, callback, qos);
}

OCStackResult RDClient::publishResourceToRD(const std::string& host,
                                   OCConnectivityType connectivityType,
                                   ResourceHandles& resourceHandles,
                                   uint32_t ttl,
                                   PublishResourceCallback callback,
                                   QualityOfService qos)
{
    ServerCallbackContext::PublishContext* ctx =
        new ServerCallbackContext::PublishContext(callback);
    OCCallbackData cbdata(
            static_cast<void*>(ctx),
            publishResourceToRDCallback,
            [](void* c)
            {delete static_cast<ServerCallbackContext::PublishContext*>(c);}
            );

    auto cLock = m_csdkLock.lock();
    OCStackResult result = OC_STACK_ERROR;
    if (cLock)
    {
        std::lock_guard<std::recursive_mutex> lock(*cLock);
        if (resourceHandles.size() > UINT8_MAX)
        {
            throw OCException(OC::Exception::PUBLISH_RESOURCE_FAILED, result);
        }
        result = OCRDPublish(nullptr, host.c_str(), connectivityType, &resourceHandles[0],
                             (uint8_t)resourceHandles.size(), ttl, &cbdata, static_cast<OCQualityOfService>(qos));
    }

    if (OC_STACK_OK != result)
    {
        throw OCException(OC::Exception::PUBLISH_RESOURCE_FAILED, result);
    }
    return result;
}

OCStackApplicationResult deleteResourceFromRDCallback(void* ctx, OCDoHandle /*handle*/,
                                                      OCClientResponse* clientResponse)
{
    ServerCallbackContext::DeleteContext* context =
    static_cast<ServerCallbackContext::DeleteContext*>(ctx);

    std::thread exec(context->callback, clientResponse->result);
    exec.detach();
    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult RDClient::deleteResourceFromRD(const std::string& host,
                                OCConnectivityType connectivityType,
                                ResourceHandles& resourceHandles,
                                DeleteResourceCallback callback)
{
    return deleteResourceFromRD(host, connectivityType, resourceHandles, callback, static_cast<QualityOfService>(m_qos));
}


OCStackResult RDClient::deleteResourceFromRD(const std::string& host,
                                OCConnectivityType connectivityType,
                                DeleteResourceCallback callback,
                                QualityOfService qos)
{
    ResourceHandles resourceHandles;
    return deleteResourceFromRD(host, connectivityType, resourceHandles, callback, qos);
}

OCStackResult RDClient::deleteResourceFromRD(const std::string& host,
                                OCConnectivityType connectivityType,
                                ResourceHandles& resourceHandles,
                                DeleteResourceCallback callback,
                                QualityOfService qos)
{
    ServerCallbackContext::DeleteContext* ctx =
        new ServerCallbackContext::DeleteContext(callback);
    OCCallbackData cbdata(
            static_cast<void*>(ctx),
            deleteResourceFromRDCallback,
            [](void* c)
            {delete static_cast<ServerCallbackContext::DeleteContext*>(c);}
            );

    auto cLock = m_csdkLock.lock();
    OCStackResult result = OC_STACK_ERROR;
    if (cLock)
    {
        std::lock_guard<std::recursive_mutex> lock(*cLock);
        if (resourceHandles.size() > UINT8_MAX)
        {
            throw OCException(OC::Exception::DELETE_RESOURCE_FAILED, result);
        }
        result = OCRDDelete(nullptr, host.c_str(), connectivityType, &resourceHandles[0],
                            (uint8_t)resourceHandles.size(), &cbdata, static_cast<OCQualityOfService>(qos));
    }

    if (OC_STACK_OK != result)
    {
        throw OCException(OC::Exception::PUBLISH_RESOURCE_FAILED, result);
    }
    return result;
}
#endif
