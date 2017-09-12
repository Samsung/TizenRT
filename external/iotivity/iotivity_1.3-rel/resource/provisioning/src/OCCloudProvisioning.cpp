/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include "ocstack.h"
#include "OCCloudProvisioning.hpp"

namespace OC
{
    void OCCloudProvisioning::callbackWrapper(void *ctx,
            OCClientResponse *response,
            void *data)
    {
        CloudProvisionContext* context = static_cast<CloudProvisionContext*>(ctx);

        std::thread exec(context->callback, response->result, data);
        exec.detach();

        delete context;
    }

    void OCCloudProvisioning::aclIdResponseWrapper(void *ctx,
            OCClientResponse *response,
            void *data)
    {
        std::string aclId = "";
        AclIdContext* context = static_cast<AclIdContext*>(ctx);

        if ((OC_STACK_OK == response->result) && data)
        {
            aclId = (char *)data;
        }
        std::thread exec(context->callback, response->result, aclId);
        exec.detach();

        delete context;
    }

    OCCloudProvisioning::OCCloudProvisioning(std::string& ipAddr, uint16_t port)
    {
        memset(&m_devAddr, 0, sizeof(m_devAddr));
        memcpy(m_devAddr.addr, ipAddr.c_str(), MAX_ADDR_STR_SIZE);
        m_devAddr.port = port;
    }

    OCCloudProvisioning::~OCCloudProvisioning(void)
    {
    }

    OCStackResult OCCloudProvisioning::requestCertificate(ResponseCallBack callback)
    {
        if (!callback)
        {
            oclog() <<"Result callback can't be null";
            return OC_STACK_INVALID_CALLBACK;
        }

        OCStackResult result;
        auto cLock = OCPlatform_impl::Instance().csdkLock().lock();

        if (cLock)
        {
            CloudProvisionContext *context = new CloudProvisionContext(callback);

            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCloudCertificateIssueRequest(static_cast<void*>(context), &m_devAddr,
                    &OCCloudProvisioning::callbackWrapper);
        }
        else
        {
            oclog() <<"Mutex not found";
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult OCCloudProvisioning::getIndividualAclInfo(const std::string& aclId,
                        ResponseCallBack callback)
    {
        if (!callback)
        {
            oclog() <<"Result callback can't be null";
            return OC_STACK_INVALID_CALLBACK;
        }

        OCStackResult result;
        auto cLock = OCPlatform_impl::Instance().csdkLock().lock();

        if (cLock)
        {
            CloudProvisionContext *context = new CloudProvisionContext(callback);

            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCloudAclIndividualGetInfo(static_cast<void*>(context), aclId.c_str(),
                    &m_devAddr,
                    &OCCloudProvisioning::callbackWrapper);
        }
        else
        {
            oclog() <<"Mutex not found";
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult OCCloudProvisioning::getAclIdByDevice(const std::string& deviceId,
                                                        AclIdResponseCallBack callback)
    {
        if (!callback)
        {
            oclog() <<"Result callback can't be null";
            return OC_STACK_INVALID_CALLBACK;
        }

        OCStackResult result;
        auto cLock = OCPlatform_impl::Instance().csdkLock().lock();

        if (cLock)
        {
            AclIdContext *context = new AclIdContext(callback);

            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCloudGetAclIdByDevice(static_cast<void*>(context), deviceId.c_str(),
                    &m_devAddr,
                    &OCCloudProvisioning::aclIdResponseWrapper);
        }
        else
        {
            oclog() <<"Mutex not found";
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult OCCloudProvisioning::createAclId(const std::string& ownerId, const std::string& deviceId,
                              AclIdResponseCallBack callback)
    {
        if (!callback)
        {
            oclog() <<"Result callback can't be null";
            return OC_STACK_INVALID_CALLBACK;
        }

        OCStackResult result;
        auto cLock = OCPlatform_impl::Instance().csdkLock().lock();

        if (cLock)
        {
            AclIdContext *context = new AclIdContext(callback);

            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCloudAclIdCreate(static_cast<void*>(context), ownerId.c_str(),
                                        deviceId.c_str(), &m_devAddr,
                                        &OCCloudProvisioning::aclIdResponseWrapper);
        }
        else
        {
            oclog() <<"Mutex not found";
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult OCCloudProvisioning::getCRL(ResponseCallBack callback)
    {
        if (!callback)
        {
            oclog() <<"Result callback can't be null";
            return OC_STACK_INVALID_CALLBACK;
        }

        OCStackResult result;
        auto cLock = OCPlatform_impl::Instance().csdkLock().lock();

        if (cLock)
        {
            CloudProvisionContext *context = new CloudProvisionContext(callback);

            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCloudGetCRL(static_cast<void*>(context), &m_devAddr,
                    &OCCloudProvisioning::callbackWrapper);
        }
        else
        {
            oclog() <<"Mutex not found";
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult OCCloudProvisioning::updateIndividualACL(const cloudAce_t *aces, std::string& aclId, ResponseCallBack callback)
    {
        if (!callback)
        {
            oclog() <<"Result callback can't be null";
            return OC_STACK_INVALID_CALLBACK;
        }
        if (!aces)
        {
            oclog() <<"Aces List can not be empty";
            return OC_STACK_INVALID_CALLBACK;
        }
        OCStackResult result;
        auto cLock = OCPlatform_impl::Instance().csdkLock().lock();

        if (cLock)
        {
            CloudProvisionContext *context = new CloudProvisionContext(callback);

            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCloudAclIndividualAclUpdate(static_cast<void*>(context), aclId.c_str(),
                    aces, &m_devAddr, &OCCloudProvisioning::callbackWrapper);
        }
        else
        {
            oclog() <<"Mutex not found";
            result = OC_STACK_ERROR;
        }
        return result;
    }

    OCStackResult OCCloudProvisioning::postCRL(const std::string& thisUpdate,
                                              const std::string& nextUpdate,
                                              const OCByteString *crl,
                                              const stringArray_t *serialNumbers,
                                              ResponseCallBack callback)
    {
        if (!callback)
        {
            oclog() <<"Result callback can't be null";
            return OC_STACK_INVALID_CALLBACK;
        }

        OCStackResult result;
        auto cLock = OCPlatform_impl::Instance().csdkLock().lock();

        if (cLock)
        {
            CloudProvisionContext *context = new CloudProvisionContext(callback);

            std::lock_guard<std::recursive_mutex> lock(*cLock);
            result = OCCloudPostCRL(static_cast<void*>(context), thisUpdate.c_str(),
                    nextUpdate.c_str(), crl, serialNumbers, &m_devAddr,
                    &OCCloudProvisioning::callbackWrapper);
        }
        else
        {
            oclog() <<"Mutex not found";
            result = OC_STACK_ERROR;
        }
        return result;
    }

}
