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
#ifndef OC_CLOUD_WRAPPER_H
#define OC_CLOUD_WRAPPER_H

#include "occloudprovisioning.h"
#include "OCCloudProvisioning.hpp"

using namespace OC;
/**
 * Certificate-Issue request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperCertificateIssueRequest(OCCloudProvisioning& ptr, ResponseCallBack callback);

/**
 * CRL GET request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
*/
OCStackResult OCWrapperGetCRL(OCCloudProvisioning& ptr, ResponseCallBack callback);

/**
 * CRL POST request function (with Serial Numbers list to revoke)
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperPostCRL(OCCloudProvisioning& ptr, ResponseCallBack callback);

/**
 * ACL id retrieve by device id
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclIdGetByDevice(OCCloudProvisioning& ptr, AclIdResponseCallBack callback);

/**
 * ACL id create
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclIdCreate(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL id delete
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclIdDelete(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL individual get info
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclIndividualGetInfo(OCCloudProvisioning& ptr, ResponseCallBack callback);

/**
 * ACL individual update ACE
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclIndividualUpdateAce(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL individual delete
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclIndividualDelete(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL post group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclCreateGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL get group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclFindMyGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL delete group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclDeleteGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL join to invited group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclJoinToInvitedGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL observe group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclObserveGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL share device into group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclShareDeviceIntoGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL delete device from group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclDeleteDeviceFromGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL get group info request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclGroupGetInfo(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL invite user to group request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclInviteUser(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL get invitation request function
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclGetInvitation(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL delete invitation request function (by receiver)
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclDeleteInvitation(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL cancel invitation request function (by sender)
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclCancelInvitation(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * ACL check that given request can be applied to resource
 *
 * @param[in] endPoint          cloud host and port
 * @param[in] callback          result callback
 * @return  OCStackResult application result
 */
OCStackResult OCWrapperAclPolicyCheck(const OCDevAddr *endPoint, OCCloudResponseCB callback);

/**
 * Read user input (expect string value)
 *
 * @param[out] item           string item to fill
 * @param[in] length          max allowed string length
 * @param[in] description     item description
 * @param[in] example         item example
 */
void readString(char* item, int length, const char* description, const char* example);

/**
 * Read user input (expect integer value)
 *
 * @param[out] item           integer item to fill
 * @param[in] description     item description
 * @param[in] example         item example
 */
void readInteger(int* item, const char* description, const char* example);


#endif //OC_CLOUD_WRAPPER_H
