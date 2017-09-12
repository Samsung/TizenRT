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

#include <OCPlatform.h>
#include <OCApi.h>
#include <gtest/gtest.h>

namespace OCAccountManagerTest
{
    using namespace OC;

    // Callbacks
    void accountHandler(const HeaderOptions& /*headerOptions*/, const OCRepresentation& /*rep*/,
            const int /*eCode*/)
    {
    }

    void deleteHandler(const HeaderOptions& /*headerOptions*/, const int /*eCode*/)
    {
    }

    void onObserve(const HeaderOptions /*headerOptions*/, const OCRepresentation& /*rep*/,
                        const int& /*eCode*/, const int& /*sequenceNumber*/)
    {
    }

    // Helper method
    OCAccountManager::Ptr ConstructAccountManagerObject(std::string host)
    {
        auto ret = OCPlatform::constructAccountManagerObject(host, CT_DEFAULT);

        if (!ret)
        {
            ADD_FAILURE() << "ConstructAccountManagerObject result was null";
            throw std::runtime_error("ConstructAccountManagerObject result was null");
        }

        return ret;
    }

    // Host Test
    TEST(HostTest, Host)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_TRUE(accountManager->host() == host);
    }

    // ConnectivityType Test
    TEST(ConnectivityTypeTest, ConnectivityType)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_TRUE(accountManager->connectivityType() == CT_DEFAULT);
    }

    // SignUp Test
    TEST(SignUpTest, DISABLED_SignUpWithoutOptionForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string authProvider("AnyAuthProvider");
        std::string authCode("AnyAuthCode");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->signUp(authProvider, authCode, &accountHandler));
    }

    TEST(SignUpTest, DISABLED_SignUpForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string authProvider("AnyAuthProvider");
        std::string authCode("AnyAuthCode");
        QueryParamsMap options = {};
        options.insert(std::pair<std::string, std::string>("AnyOptionKey", "AnyOptionValue"));
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->signUp(authProvider, authCode, options,
                                                      &accountHandler));
    }

    TEST(SignUpTest, SignUpWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string authProvider("AnyAuthProvider");
        std::string authCode("AnyAuthCode");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->signUp(authProvider, authCode, nullptr));
    }

    // SignIn Test
    TEST(SignInTest, DISABLED_SignInForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string userId("AnyUserId");
        std::string accessToken("AnyAccessToken");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->signIn(userId, accessToken, &accountHandler));
    }

    TEST(SignInTest, SignInWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string userId("AnyUserId");
        std::string accessToken("AnyAccessToken");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->signIn(userId, accessToken, nullptr));
    }

    // SignOut Test
    TEST(SignOutTest, DISABLED_SignOutForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string accessToken("AnyAccessToken");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->signOut(accessToken, &accountHandler));
    }

    TEST(SignOutTest, SignOutWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string accessToken("AnyAccessToken");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->signOut(accessToken, nullptr));
    }

    // RefreshAccessToken Test
    TEST(RefreshAccessTokenTest, DISABLED_RefreshAccessTokenForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string userId("AnyUserId");
        std::string refreshToken("AnyRefreshToken");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->refreshAccessToken(userId, refreshToken,
                                                                  &accountHandler));
    }

    TEST(RefreshAccessTokenTest, RefreshAccessTokenWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string userId("AnyUserId");
        std::string refreshToken("AnyRefreshToken");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->refreshAccessToken(userId, refreshToken, nullptr));
    }

    // SearchUser Test
    TEST(SearchUserTest, DISABLED_SearchUserForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string key("AnyKey");
        std::string value("AnyValue");
        QueryParamsMap query = {};
        query.insert(std::pair<std::string, std::string>(key, value));
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->searchUser(query, &accountHandler));
    }

    TEST(SearchUserTest, SearchUserWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string key("AnyKey");
        std::string value("AnyValue");
        QueryParamsMap query = {};
        query.insert(std::pair<std::string, std::string>(key, value));
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->searchUser(query, nullptr));
    }

    // DeleteDevice Test
    TEST(DeleteDeviceTest, DISABLED_DeleteDeviceForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string accessToken("AnyAccessToken");
        std::string deviceId("AnyDeviceId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->deleteDevice(accessToken, deviceId,
                                                            &deleteHandler));
    }

    TEST(DeleteDeviceTest, DeleteDeviceWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string accessToken("AnyAccessToken");
        std::string deviceId("AnyDeviceId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->deleteDevice(accessToken, deviceId, nullptr));
    }

    // CreateGroup Test
    TEST(CreateGroupTest, DISABLED_CreateGroupForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->createGroup(&accountHandler));
    }

    TEST(CreateGroupTest, DISABLED_CreateGroupWithOptionForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        std::string property("AnyProperty");
        std::string value("AnyValue");
        QueryParamsMap query = {};
        query.insert(std::pair<std::string, std::string>(property, value));
        EXPECT_EQ(OC_STACK_OK, accountManager->createGroup(query, &accountHandler));
    }

    TEST(CreateGroupTest, CreateGroupWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->createGroup(nullptr));
    }

    // DeleteGroup Test
    TEST(DeleteGroupTest, DISABLED_DeleteGroupForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->deleteGroup(groupId, &deleteHandler));
    }

    TEST(DeleteGroupTest, DeleteGroupWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->deleteGroup(groupId, nullptr));
    }

    // GetGroupInfoAll Test
    TEST(GetGroupInfoAllTest, DISABLED_GetGroupInfoAllForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->getGroupInfoAll(&accountHandler));
    }

    TEST(GetGroupInfoAllTest, GetGroupInfoAllWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->getGroupInfoAll(nullptr));
    }

    // GetGroupInfo Test
    TEST(GetGroupInfoTest, DISABLED_GetGroupInfoForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->getGroupInfo(groupId, &accountHandler));
    }

    TEST(GetGroupInfoTest, GetGroupInfoWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->getGroupInfo(groupId, nullptr));
    }

    // AddPropertyValueToGroup Test
    TEST(AddPropertyValueToGroupTest, DISABLED_AddPropertyValueToGroupForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCRepresentation propertyValue;
        propertyValue.setValue("AnyProperty", "AnyValue");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->addPropertyValueToGroup(groupId, propertyValue,
                                                                       &accountHandler));
    }

    TEST(AddPropertyValueToGroupTest, AddPropertyValueToGroupWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCRepresentation propertyValue;
        propertyValue.setValue("AnyProperty", "AnyValue");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->addPropertyValueToGroup(groupId, propertyValue,
                                                                 nullptr));
    }

    // DeletePropertyValueFromGroup Test
    TEST(DeletePropertyValueFromGroupTest, DISABLED_DeletePropertyValueFromGroupForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCRepresentation propertyValue;
        propertyValue.setValue("AnyProperty", "AnyValue");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->deletePropertyValueFromGroup(groupId, propertyValue,
                                                                            &accountHandler));
    }

    TEST(DeletePropertyValueFromGroupTest, DeletePropertyValueFromGroupWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCRepresentation propertyValue;
        propertyValue.setValue("AnyProperty", "AnyValue");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->deletePropertyValueFromGroup(groupId, propertyValue,
                                                                      nullptr));
    }

    // UpdatePropertyValueOnGroup Test
    TEST(UpdatePropertyValueOnGroupTest, DISABLED_UpdatePropertyValueOnGroupForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCRepresentation propertyValue;
        propertyValue.setValue("AnyProperty", "AnyValue");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->updatePropertyValueOnGroup(groupId, propertyValue,
                                                                       &accountHandler));
    }

    TEST(UpdatePropertyValueOnGroupTest, UpdatePropertyValueOnGroupWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCRepresentation propertyValue;
        propertyValue.setValue("AnyProperty", "AnyValue");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->updatePropertyValueOnGroup(groupId, propertyValue,
                                                                 nullptr));
    }

    // ObserveGroup Test
    TEST(ObserveGroupTest, DISABLED_ObserveGroupForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->observeGroup(&onObserve));
    }

    TEST(ObserveGroupTest, ObserveGroupWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->observeGroup(nullptr));
    }

    // CancelObserveGroup Test
    TEST(CancelObserveGroupTest, DISABLED_CancelObserveGroupForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->observeGroup(&onObserve));
        EXPECT_EQ(OC_STACK_OK, accountManager->cancelObserveGroup());
    }

    TEST(CancelObserveGroupTest, CancelObserveGroupWithoutObserve)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->cancelObserveGroup());
    }

    // ObserveInvitation Test
    TEST(ObserveInvitationTest, DISABLED_ObserveInvitationForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->observeInvitation(&onObserve));
    }

    TEST(ObserveInvitationTest, ObserveInvitationWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->observeInvitation(nullptr));
    }

    // CancelObserveInvitation Test
    TEST(CancelObserveInvitationTest, DISABLED_CancelObserveInvitationForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->observeInvitation(&onObserve));
        EXPECT_EQ(OC_STACK_OK, accountManager->cancelObserveInvitation());
    }

    TEST(CancelObserveInvitationTest, CancelObserveInvitationWithoutObserve)
    {
        std::string host("coap://192.168.1.2:5000");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->cancelObserveInvitation());
    }

    // SendInvitation Test
    TEST(SendInvitationTest, DISABLED_SendInvitationForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        std::string userId("AnyUserId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->sendInvitation(groupId, userId, &accountHandler));
    }

    TEST(SendInvitationTest, SendInvitationWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        std::string userId("AnyUserId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->sendInvitation(groupId, userId, nullptr));
    }

    // CancelInvitation Test
    TEST(CancelInvitationTest, DISABLED_CancelInvitationForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        std::string userId("AnyUserId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->cancelInvitation(groupId, userId, &deleteHandler));
    }

    TEST(CancelInvitationTest, CancelInvitationWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        std::string userId("AnyUserId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->cancelInvitation(groupId, userId, nullptr));
    }

    // ReplyToInvitation Test
    TEST(ReplyToInvitationTest, DISABLED_ReplyToInvitationForValid)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_EQ(OC_STACK_OK, accountManager->replyToInvitation(groupId, true, &deleteHandler));
    }

    TEST(ReplyToInvitationTest, ReplyToInvitationWithNullCallback)
    {
        std::string host("coap://192.168.1.2:5000");
        std::string groupId("AnyGroupId");
        OCAccountManager::Ptr accountManager = ConstructAccountManagerObject(host);
        EXPECT_TRUE(NULL != accountManager);
        EXPECT_ANY_THROW(accountManager->replyToInvitation(groupId, true, nullptr));
    }
}
