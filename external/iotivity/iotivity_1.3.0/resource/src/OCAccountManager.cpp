/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include "OCAccountManager.h"
#include "OCUtilities.h"

#include "ocstack.h"

#define VERIFY_VALID_STRING(arg, log_message, ret) \
    if (std::string::npos == (arg).find_first_not_of(" \f\n\r\t\v")) \
    { \
        oclog() << log_message << std::flush; \
        return result_guard(ret); \
    } \

namespace OC {

using OC::result_guard;
using OC::checked_guard;

OCAccountManager::OCAccountManager(std::weak_ptr<IClientWrapper> clientWrapper,
                                   const std::string& host,
                                   OCConnectivityType connectivityType)
 : m_clientWrapper(clientWrapper), m_host(host), m_userUuid(""),
   m_invitationObserveHandle(nullptr), m_groupObserveHandle(nullptr), m_connType(connectivityType)
{
    if (std::string::npos == m_host.find_first_not_of(" \f\n\r\t\v") || m_clientWrapper.expired())
    {
        throw OCException(OC::Exception::INVALID_PARAM);
    }

    const char* di = OCGetServerInstanceIDString();
    if (!di)
    {
        oclog() << "The mode should be Server or Both to generate UUID" << std::flush;
        throw OCException(OC::Exception::INVALID_PARAM);
    }

    m_deviceID.append(di);
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, m_defaultQos);
}

OCAccountManager::~OCAccountManager()
{
}

std::string OCAccountManager::host() const
{
    return m_host;
}

OCConnectivityType OCAccountManager::connectivityType() const
{
    return m_connType;
}

OCStackResult OCAccountManager::signUp(const std::string& authProvider,
                                       const std::string& authCode,
                                       PostCallback cloudConnectHandler)
{
    return result_guard(signUp(authProvider, authCode, QueryParamsMap(), cloudConnectHandler));
}

OCStackResult OCAccountManager::signUp(const std::string& authProvider,
                                       const std::string& authCode,
                                       const QueryParamsMap& options,
                                       PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(authProvider, "authProvider cannot be empty.", OC_STACK_INVALID_PARAM);
    VERIFY_VALID_STRING(authCode, "authCode cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACCOUNT_URI;

    OCRepresentation rep;
    rep.setValue<std::string>(OC_RSRVD_DEVICE_ID, m_deviceID);
    rep.setValue<std::string>(OC_RSRVD_AUTHPROVIDER, authProvider);
    rep.setValue<std::string>(OC_RSRVD_AUTHCODE, authCode);

    if (!options.empty())
    {
        for (auto iter : options)
        {
            rep.setValue(iter.first, iter.second);
        }
    }

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, rep, QueryParamsMap(), HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::signIn(const std::string& userUuid,
                                       const std::string& accessToken,
                                       PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(userUuid, "userUuid cannot be empty.", OC_STACK_INVALID_PARAM);
    VERIFY_VALID_STRING(accessToken, "accessToken cannot be empty.", OC_STACK_INVALID_PARAM);

    OCStackResult result = result_guard(signInOut(userUuid, accessToken, true,
                                                  cloudConnectHandler));
    if (OC_STACK_OK == result)
    {
        m_userUuid = userUuid;
    }

    return result;
}

OCStackResult OCAccountManager::signOut(const std::string& accessToken,
                                        PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);
    VERIFY_VALID_STRING(accessToken, "accessToken cannot be empty.", OC_STACK_INVALID_PARAM);

    OCStackResult result = result_guard(signInOut(m_userUuid, accessToken, false,
                                                  cloudConnectHandler));
    if (OC_STACK_OK == result)
    {
        m_userUuid = "";
    }

    return result;
}

OCStackResult OCAccountManager::signInOut(const std::string& userUuid,
                                          const std::string& accessToken,
                                          bool isSignIn,
                                          PostCallback cloudConnectHandler)
{
    std::string uri = m_host + OC_RSRVD_ACCOUNT_SESSION_URI;

    OCRepresentation rep;

    if (isSignIn)
    {
        rep.setValue<std::string>(OC_RSRVD_USER_UUID, userUuid);
    }
    rep.setValue<std::string>(OC_RSRVD_DEVICE_ID, m_deviceID);
    rep.setValue<std::string>(OC_RSRVD_ACCESS_TOKEN, accessToken);
    rep.setValue<bool>(OC_RSRVD_LOGIN, isSignIn);

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, rep, QueryParamsMap(), HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::refreshAccessToken(const std::string& userUuid,
                                                   const std::string& refreshToken,
                                                   PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(userUuid, "userUuid cannot be empty.", OC_STACK_INVALID_PARAM);
    VERIFY_VALID_STRING(refreshToken, "refreshToken cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACCOUNT_TOKEN_REFRESH_URI;

    OCRepresentation rep;
    rep.setValue<std::string>(OC_RSRVD_USER_UUID, userUuid);
    rep.setValue<std::string>(OC_RSRVD_DEVICE_ID, m_deviceID);
    rep.setValue<std::string>(OC_RSRVD_GRANT_TYPE, std::string(OC_RSRVD_GRANT_TYPE_REFRESH_TOKEN));
    rep.setValue<std::string>(OC_RSRVD_REFRESH_TOKEN, refreshToken);

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, rep, QueryParamsMap(), HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::searchUser(const QueryParamsMap& queryParams,
                                           GetCallback cloudConnectHandler)
{
    if (queryParams.empty())
    {
        oclog() << "queryParams cannot be empty." << std::flush;
        return OC_STACK_INVALID_PARAM;
    }

    std::string uri = m_host + OC_RSRVD_ACCOUNT_SEARCH_URI;

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetResourceRepresentation,
                         OCDevAddr(), uri, queryParams, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::deleteDevice(const std::string& accessToken,
                                             const std::string& deviceId,
                                             DeleteCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(accessToken, "accessToken cannot be empty.", OC_STACK_INVALID_PARAM);
    VERIFY_VALID_STRING(deviceId, "deviceId cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACCOUNT_URI
                      + "?" + OC_RSRVD_ACCESS_TOKEN + "=" + accessToken
                      + ";" + OC_RSRVD_DEVICE_ID + "=" + deviceId;

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::DeleteResource,
                         OCDevAddr(), uri, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::createGroup(PostCallback cloudConnectHandler)
{
    return result_guard(createGroup(QueryParamsMap(), cloudConnectHandler));
}

OCStackResult OCAccountManager::createGroup(const QueryParamsMap& queryParams,
                                            PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI;

    std::vector<std::string> members{m_userUuid};

    OCRepresentation rep;
    rep.setValue<std::string>(OC_RSRVD_OWNER, m_userUuid);
    rep.setValue<std::vector<std::string>>(OC_RSRVD_MEMBERS, members);

    if (!queryParams.empty())
    {
        for (auto iter : queryParams)
        {
            rep.setValue(iter.first, iter.second);
        }
    }

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, rep, QueryParamsMap(), HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::deleteGroup(const std::string& groupId,
                                            DeleteCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI + "/" + groupId
                      + "?" + OC_RSRVD_OWNER + "=" + m_userUuid;

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::DeleteResource,
                         OCDevAddr(), uri, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::getGroupInfoAll(GetCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI;

    QueryParamsMap query = {};
    query.insert(std::make_pair(OC_RSRVD_MEMBERS, m_userUuid));

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetResourceRepresentation,
                         OCDevAddr(), uri, query, HeaderOptions(), m_connType,
                         cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::getGroupInfo(const std::string& groupId,
                                             GetCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI + "/" + groupId;

    QueryParamsMap query = {};
    query.insert(std::make_pair(OC_RSRVD_MEMBERS, m_userUuid));

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetResourceRepresentation,
                         OCDevAddr(), uri, query, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::addPropertyValueToGroup(const std::string& groupId,
                                                        const OCRepresentation propertyValue,
                                                        PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI + "/" + groupId;

    QueryParamsMap query = {};
    query.insert(std::make_pair(OC_RSRVD_OPERATION, OC_RSRVD_ADD));

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, propertyValue, query, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::deletePropertyValueFromGroup(const std::string& groupId,
                                                             const OCRepresentation propertyValue,
                                                             PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI + "/" + groupId;

    QueryParamsMap query = {};
    query.insert(std::make_pair(OC_RSRVD_OPERATION, OC_RSRVD_DELETE));

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, propertyValue, query, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::updatePropertyValueOnGroup(const std::string& groupId,
                                                           const OCRepresentation propertyValue,
                                                           PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI + "/" + groupId;

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, propertyValue, QueryParamsMap(), HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::observeGroup(ObserveCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI;

    QueryParamsMap query = {};
    query.insert(std::make_pair(OC_RSRVD_MEMBERS, m_userUuid));

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::ObserveResource,
                         ObserveType::Observe, &m_groupObserveHandle, OCDevAddr(), uri,
                         query, HeaderOptions(), cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::cancelObserveGroup()
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);

    if (nullptr == m_groupObserveHandle)
    {
        oclog() << "observeGroup() has not been done." << std::flush;
        return result_guard(OC_STACK_INVALID_PARAM);
    }

    std::string uri = m_host + OC_RSRVD_ACL_GROUP_URI
                      + "?" + OC_RSRVD_MEMBERS + "=" + m_userUuid;

    OCStackResult result = checked_guard(m_clientWrapper.lock(),
                                         &IClientWrapper::CancelObserveResource,
                                         m_groupObserveHandle, (const char*)"", uri,
                                         HeaderOptions(), m_defaultQos);
    if (OC_STACK_OK == result)
    {
        m_groupObserveHandle = nullptr;
    }

    return result;
}

OCStackResult OCAccountManager::observeInvitation(ObserveCallback cloudConnectHandler)
{
    std::string uri = m_host + OC_RSRVD_ACL_INVITE_URI;

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::ObserveResource,
                         ObserveType::Observe, &m_invitationObserveHandle, OCDevAddr(), uri,
                         QueryParamsMap(), HeaderOptions(), cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::cancelObserveInvitation()
{
    if (nullptr == m_invitationObserveHandle)
    {
        oclog() << "observeInvitation() has not been done." << std::flush;
        return result_guard(OC_STACK_INVALID_PARAM);
    }

    std::string uri = m_host + OC_RSRVD_ACL_INVITE_URI;

    OCStackResult result = checked_guard(m_clientWrapper.lock(),
                                         &IClientWrapper::CancelObserveResource,
                                         m_invitationObserveHandle,
                                         (const char*)"", uri, HeaderOptions(), m_defaultQos);
    if (OC_STACK_OK == result)
    {
        m_invitationObserveHandle = nullptr;
    }

    return result;
}

OCStackResult OCAccountManager::sendInvitation(const std::string& groupId,
                                               const std::string& userUuid,
                                               PostCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(m_userUuid, "Need to sign-in first.", OC_STACK_ERROR);
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);
    VERIFY_VALID_STRING(userUuid, "userUuid cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACL_INVITE_URI;

    OCRepresentation invitation;
    invitation.setValue<std::string>(OC_RSRVD_GROUP_ID, groupId);
    invitation.setValue<std::string>(OC_RSRVD_MEMBER_ID, userUuid);

    std::vector<OCRepresentation> invite{invitation};

    OCRepresentation rep;
    rep.setValue<std::string>(OC_RSRVD_USER_UUID, m_userUuid);
    rep.setValue<std::vector<OCRepresentation>>(OC_RSRVD_INVITE, invite);

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         OCDevAddr(), uri, rep, QueryParamsMap(), HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::cancelInvitation(const std::string& groupId,
                                                 const std::string& userUuid,
                                                 DeleteCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);
    VERIFY_VALID_STRING(userUuid, "userUuid cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string uri = m_host + OC_RSRVD_ACL_INVITE_URI + "?" + OC_RSRVD_GROUP_ID + "=" + groupId
                      + ";" + OC_RSRVD_MEMBER_ID + "=" + userUuid;

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::DeleteResource,
                         OCDevAddr(), uri, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

OCStackResult OCAccountManager::replyToInvitation(const std::string& groupId,
                                                  const bool accept,
                                                  DeleteCallback cloudConnectHandler)
{
    VERIFY_VALID_STRING(groupId, "groupId cannot be empty.", OC_STACK_INVALID_PARAM);

    std::string isAccept = accept ? "1" : "0";

    std::string uri = m_host + OC_RSRVD_ACL_INVITE_URI + "?" + OC_RSRVD_GROUP_ID + "=" + groupId
                      + ";" + OC_RSRVD_ACCEPT + "=" + isAccept;

    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::DeleteResource,
                         OCDevAddr(), uri, HeaderOptions(),
                         m_connType, cloudConnectHandler, m_defaultQos);
}

} // namespace OC
