//****************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef OC_CLOUD_PROVISIONING_CXX_H_
#define OC_CLOUD_PROVISIONING_CXX_H_

#include <thread>

#include "occloudprovisioning.h"
#include "OCApi.h"
#include "OCPlatform_impl.h"
#include "CAManager.h"

namespace OC
{
    typedef std::function<void(OCStackResult result, void *data)> ResponseCallBack;
    typedef std::function<void(OCStackResult result, std::string aclId)> AclIdResponseCallBack;

    /**
     * Context to be passed to the underlying stack. This is passed back as argument
     * to the callback function
     */
    struct CloudProvisionContext
    {
        ResponseCallBack callback;
        CloudProvisionContext(ResponseCallBack cb) : callback(cb){}
    };

    struct AclIdContext
    {
        AclIdResponseCallBack callback;
        AclIdContext(AclIdResponseCallBack cb) : callback(cb){}
    };

    class OCCloudProvisioning
    {

        private:
            OCDevAddr  m_devAddr;
        public:

            /**
             * API to construct the CloudProvisioning
             * @param ipAddr address of the cloud server
             * @param port port of the cloud server
             */
            OCCloudProvisioning(std::string& ipAddr, uint16_t port);
            ~OCCloudProvisioning();

            void setIpAddr(std::string& ipAddr)
            {
                memcpy(m_devAddr.addr, ipAddr.c_str(), MAX_ADDR_STR_SIZE);
            }

            void setPort(uint16_t port)
            {
                m_devAddr.port = port;
            }

            /**
             * API to Request a certificate from the cloud
             * @param callback function called by the stack on completion of request
             * @return ::OC_STACK_OK on Success and other values otherwise
             */
            OCStackResult requestCertificate(ResponseCallBack callback);

            /**
             * API to get ACL ID for the device
             * @param deviceId device ID for which the Acl ID is requested
             * @param callback function called by the stack on completion of request
             * @return ::OC_STACK_OK on Success and other values otherwise
             */
            OCStackResult getAclIdByDevice(const std::string& deviceId,
                                           AclIdResponseCallBack callback);

            /**
             * API to create ACL ID for the device
             * @param ownerId owner ID for which the Acl ID is to be created
             * @param deviceId device ID for which the Acl ID is requested
             * @param endPoint cloud host and port
             * @return ::OC_STACK_OK on Success and other values otherwise
             */
            OCStackResult createAclId(const std::string& ownerId, const std::string& deviceId,
                                      AclIdResponseCallBack callback);

            /**
             * API to get ACL information about the given Acl ID
             * @param aclId ACL ID for which the Acl information is requested
             * @param callback function called by the stack on completion of request
             * @return ::OC_STACK_OK on Success and other values otherwise
             */
            OCStackResult getIndividualAclInfo(const std::string& aclId, ResponseCallBack callback);

            /**
             * API to get certificate revocation list
             * @param callback function called by the stack on completion of request
             * @return ::OC_STACK_OK on Success and other values otherwise
             */
            OCStackResult getCRL(ResponseCallBack callback);

            /**
             * API for updation of Individual Acl
             * @param aces is the list of Access control entities for updation.
             * @param callback function called by the stack on completion of request
             * @return ::OC_STACK_OK on Success and other values otherwise
             */
            OCStackResult updateIndividualACL(const cloudAce_t *aces, std::string &aclId,
                    ResponseCallBack callback);

            /**
             * API to post the  certificate revocation list to cloud
             * @param thisUpdate thisUpdate [mandatory param]
             * @param nextUpdate nextUpdate [mandatory param]
             * @param crl revocation list [optional]
             * @param serialNumbers [optional]
             * @param callback function called by the stack on completion of request
             * @return ::OC_STACK_OK on Success and other values otherwise
             */
            OCStackResult postCRL(const std::string& thisUpdate,
                    const std::string& nextUpdate,
                    const OCByteString *crl,
                    const stringArray_t *serialNumbers,
                    ResponseCallBack callback);

            /**
             * Common callback wrapper for all the callback functions.
             * @param ctx user context passed to the request API
             * @param result result of the request performed
             * @param data response data
             */
            static void callbackWrapper(void* ctx, OCClientResponse *response, void* data);

            /**
             * Callback wrapper for Acl ID get request
             * @param ctx user context passed to the request API
             * @param result result of the request performed
             * @param data AclID for the device
             */
            static void aclIdResponseWrapper(void* ctx, OCClientResponse *response, void* data);
    };
}
#endif //OC_CLOUD_PROVISIONING_CXX_H_
