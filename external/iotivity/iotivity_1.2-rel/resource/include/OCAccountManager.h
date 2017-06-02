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

#ifndef OC_ACCOUNT_MANAGER_H_
#define OC_ACCOUNT_MANAGER_H_

#include <map>

#include <OCApi.h>
#include <IClientWrapper.h>
#include <InProcClientWrapper.h>

namespace OC
{
    class OCAccountManager
    {
        friend class OCPlatform_impl;

    public:
        typedef std::shared_ptr<OCAccountManager> Ptr;

        OCAccountManager(OCAccountManager&&) = default;
        OCAccountManager(const OCAccountManager&) = delete;
        OCAccountManager& operator=(OCAccountManager&&) = delete;
        OCAccountManager& operator=(const OCAccountManager&) = delete;

        virtual ~OCAccountManager(void);

        /**
        * Function to get the host address of account server.
        *
        * @return std::string host address
        */
        std::string host() const;

        /**
        * Function to get the connectivity type for account server.
        *
        * @return enum connectivity type (flags and adapter)
        */
        OCConnectivityType connectivityType() const;

        /**
         * Function for account registration to account server.
         *
         * @param authProvider Provider name used for authentication.
         * @param authCode The authorization code obtained by using an authorization server
         *                 as an intermediary between the client and resource owner.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult signUp(const std::string& authProvider,
                             const std::string& authCode,
                             PostCallback cloudConnectHandler);

        /**
         * Overload
         *
         * @param authProvider Provider name used for authentication.
         * @param authCode The authorization code obtained by using an authorization server
         *                 as an intermediary between the client and resource owner.
         * @param options The option values depends on auth provider.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult signUp(const std::string& authProvider,
                             const std::string& authCode,
                             const QueryParamsMap& options,
                             PostCallback cloudConnectHandler);

        /**
         * Function for sign-in to account server.
         *
         * @param userUuid Identifier of the user obtained by account registration.
         * @param accessToken Identifier of the resource obtained by account registration.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult signIn(const std::string& userUuid,
                             const std::string& accessToken,
                             PostCallback cloudConnectHandler);

        /**
         * Function for sign-out to account server.
         *
         * @param accessToken Identifier of the resource obtained by account registration.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult signOut(const std::string& accessToken,
                              PostCallback cloudConnectHandler);

        /**
         * Function for refresh access token to account server.
         *
         * @param userUuid Identifier of the user obtained by account registration.
         * @param refreshToken Refresh token used for access token refresh.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult refreshAccessToken(const std::string& userUuid,
                                         const std::string& refreshToken,
                                         PostCallback cloudConnectHandler);

        /**
         * Function to get information of the user to account server.
         *
         * @param queryParams Map that has a query key and value for specific users.
         *                    Account server can response information of more than one user.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult searchUser(const QueryParamsMap& queryParams,
                                 GetCallback cloudConnectHandler);

        /**
         * Function to delete the device registered on the account signed-in.
         *
         * @param accessToken Identifier of the resource obtained by account registration.
         * @param deviceId Device ID to delete.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult deleteDevice(const std::string& accessToken,
                                   const std::string& deviceId,
                                   DeleteCallback cloudConnectHandler);

        /**
         * Function to create a group on account server.
         *
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult createGroup(PostCallback cloudConnectHandler);

        /**
         * Overload
         *
         * @param queryParams Map that has optional properties and values to create a group.
         *                    Defined properties on the OCF spec are [gname, parent] so far.
         *                    (2016/10/19)
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult createGroup(const QueryParamsMap& queryParams,
                                  PostCallback cloudConnectHandler);

        /**
         * Function to delete the group from account server.
         *
         * @param groupId Group ID to delete.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult deleteGroup(const std::string& groupId,
                                  DeleteCallback cloudConnectHandler);

        /**
         * Function to get infomation of all your group from account server.
         *
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */

        OCStackResult getGroupInfoAll(GetCallback cloudConnectHandler);

        /**
         * Function to get information of the specific group from account server.
         *
         * @param groupId Group ID to get information.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult getGroupInfo(const std::string& groupId,
                                   GetCallback cloudConnectHandler);

        /**
         * Function to add values for properties to the group on account server.
         *
         * @param groupId Group ID to add property values.
         * @param propertyValue OCRepresentation info that has pairs of property and value.
         *                      Defined properties on the OCF spec are [members, masters, devices,
         *                      resources, links] so far. (2016/10/19)
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult addPropertyValueToGroup(const std::string& groupId,
                                              const OCRepresentation propertyValue,
                                              PostCallback cloudConnectHandler);

        /**
         * Function to delete values for properties from the group on account server.
         *
         * @param groupId Group ID to delete information.
         * @param propertyValue OCRepresentation info that has pairs of property and value.
         *                      Defined properties on the OCF spec are [members, masters, devices,
         *                      resources, links] so far. (2016/10/19)
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult deletePropertyValueFromGroup(const std::string& groupId,
                                                   const OCRepresentation propertyValue,
                                                   PostCallback cloudConnectHandler);

        /**
         * Function to update values for properties on the group on account server.
         * It completely replaces existing values for specific properties.
         *
         * @param groupId Group ID to add devices.
         * @param propertyValue OCRepresentation info that has pairs of property and value.
         *                      Defined properties on the OCF spec are [members, gname, owner,
         *                      masters, devices, resources, latitude, longitude, radius,
         *                      backgroundImage] so far. (2016/10/19)
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult updatePropertyValueOnGroup(const std::string& groupId,
                                                 const OCRepresentation propertyValue,
                                                 PostCallback cloudConnectHandler);

        /**
         * Function to register observe to group resource on account server.
         * You can receive a notify when any value of property get changed in the group you joined.
         *
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult observeGroup(ObserveCallback cloudConnectHandler);

        /**
         * Function to cancel observe to group resource on account server.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult cancelObserveGroup();

        /**
         * Function to register observe to invitation resource on account server.
         * You can receive a notify when you send or receive a invitation.
         * Sending a invitation will be notified as 'invite' and Receiving will be as 'invited'.
         * If you receive a invitation from other user, you should call 'replyToInvitation' to
         * delete the invitation on account server, otherwise it will remain on the server.
         *
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult observeInvitation(ObserveCallback cloudConnectHandler);

        /**
         * Function to cancel observe to invitation resource on account server.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult cancelObserveInvitation();

        /**
         * Function to send a invitation to invite a user into a group.
         *
         * @param groupId Group ID for inviting.
         * @param userUuid Identifier of the user to invite.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult sendInvitation(const std::string& groupId,
                                     const std::string& userUuid,
                                     PostCallback cloudConnectHandler);

        /**
         * Function to cancel a invitation you has sent on account server before the invited user
         * replies.
         *
         * @param groupId Group ID to cancel a invitation.
         * @param userUuid Identifier of the user to cancel a invitation.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult cancelInvitation(const std::string& groupId,
                                       const std::string& userUuid,
                                       DeleteCallback cloudConnectHandler);

        /**
         * Function to reply to the invitation that you has received.
         * If you set accept as true, you will join the group as a member and the invitation
         * will be deleted on account server.
         * If false, only the invitation will be deleted.
         *
         * @param groupId Group ID to delete a invitation.
         * @param accept boolean whether to join the group or not.
         * @param cloudConnectHandler Callback function that will get the result of the operation.
         *
         * @return Returns ::OC_STACK_OK if success
         */
        OCStackResult replyToInvitation(const std::string& groupId,
                                        const bool accept,
                                        DeleteCallback cloudConnectHandler);

    private:
        std::weak_ptr<IClientWrapper> m_clientWrapper;
        std::string m_deviceID;
        std::string m_host;
        std::string m_userUuid;
        OCDoHandle m_invitationObserveHandle;
        OCDoHandle m_groupObserveHandle;
        OCConnectivityType m_connType;
        QualityOfService m_defaultQos;

    private:
        OCAccountManager(std::weak_ptr<IClientWrapper> clientWrapper,
                         const std::string& host,
                         OCConnectivityType connectivityType);

        OCStackResult signInOut(const std::string& userUuid,
                                const std::string& accessToken,
                                bool isSignIn,
                                PostCallback cloudConnectHandler);
    };
} // namespace OC

#endif // OC_ACCOUNT_MANAGER_H_

