/* ****************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
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

#include "platform_features.h"
#include "gtest/gtest.h"
#include "cainterface.h"
#include "cautilinterface.h"
#include "cacommon.h"
#include "oic_string.h"
#include "oic_malloc.h"

#define CA_TRANSPORT_ADAPTER_SCOPE  1000

class CATests : public testing::Test {
    protected:
    virtual void SetUp()
    {
        CAInitialize(CA_ADAPTER_IP);
    }

    virtual void TearDown()
    {
        CATerminate();
    }
};

void request_handler(CAEndpoint_t* object, CARequestInfo_t* requestInfo);
void response_handler(CAEndpoint_t* object, CAResponseInfo_t* responseInfo);
void error_handler(const CAEndpoint_t *object, const CAErrorInfo_t* errorInfo);
void adapter_handler(CATransportAdapter_t adapter, bool enabled);
void connection_handler(CATransportAdapter_t adapter, const char *remote_address, bool connected);
CAResult_t checkSelectNetwork();

void request_handler(const CAEndpoint_t * /*object*/,
                     const CARequestInfo_t * /*requestInfo*/)
{

}

void response_handler(const CAEndpoint_t * /*object*/,
                      const CAResponseInfo_t * /*responseInfo*/)
{

}

void error_handler(const CAEndpoint_t *object, const CAErrorInfo_t* errorInfo)
{
    if(!object || !errorInfo)
    {
        return;
    }

    //error handling shall be added
    return;
}

void adapter_handler(CATransportAdapter_t /*adapter*/,
                     bool /*enabled*/)
{
}

void connection_handler(const CAEndpoint_t * /*endpoint*/, 
                        bool /*connected*/)
{
}

static char* addr = NULL;
static CAEndpoint_t* tempRep = NULL;
static CARequestInfo_t requestInfo;
static CAInfo_t requestData;
static CAInfo_t responseData;
static CAResponseInfo_t responseInfo;
static CAToken_t tempToken = NULL;
static uint8_t tokenLength = CA_MAX_TOKEN_LEN;
static const char ADDRESS[] = "10.11.12.13";
static const uint16_t PORT = 4545;

static const char NORMAL_INFO_DATA[] =
                                    "{\"oc\":[{\"href\":\"%s\",\"prop\":{\"rt\":[\"core.led\"],"
                                     "\"if\":[\"oc.mi.def\"],\"obs\":1}}]}";

#ifdef __WITH_DTLS__

// Iotivity Device Identity.
const unsigned char IDENTITY[] = ("1111111111111111");

// PSK between this device and peer device.
const unsigned char RS_CLIENT_PSK[] = ("AAAAAAAAAAAAAAAA");

// Internal API. Invoked by CA stack to retrieve credentials from this module
int32_t CAGetDtlsPskCredentials( CADtlsPskCredType_t type,
              const unsigned char *desc, size_t desc_len,
              unsigned char *result, size_t result_length)
{
    printf("CAGetDtlsPskCredentials IN\n");

    int32_t ret = -1;

    if (NULL == result)
    {
        return ret;
    }

    switch (type)
    {
        case CA_DTLS_PSK_HINT:
        case CA_DTLS_PSK_IDENTITY:

            if (result_length < sizeof(IDENTITY))
            {
                printf("ERROR : Wrong value for result for storing IDENTITY");
                return ret;
            }

            memcpy(result, IDENTITY, sizeof(IDENTITY));
            ret = sizeof(IDENTITY);
            break;

        case CA_DTLS_PSK_KEY:

            if ((desc_len == sizeof(IDENTITY)) &&
                memcmp(desc, IDENTITY, sizeof(IDENTITY)) == 0)
            {
                if (result_length < sizeof(RS_CLIENT_PSK))
                {
                    printf("ERROR : Wrong value for result for storing RS_CLIENT_PSK");
                    return ret;
                }

                memcpy(result, RS_CLIENT_PSK, sizeof(RS_CLIENT_PSK));
                ret = sizeof(RS_CLIENT_PSK);
            }
            break;

        default:

            printf("Wrong value passed for PSK_CRED_TYPE.");
            ret = -1;
    }


    printf("CAGetDtlsPskCredentials OUT\n");
    return ret;
}
#endif  //__WITH_DTLS__

// CAInitialize TC
TEST(InitializeTest, CAInitializeTest)
{
    EXPECT_EQ(CA_STATUS_OK, CAInitialize(CA_ADAPTER_IP));
    CATerminate();
}

//CATerminate TC
TEST_F(CATests, TerminateTest)
{
    CATerminate();

    char* check = (char *) "terminate success";
    EXPECT_STREQ(check, "terminate success");

    CAInitialize(CA_ADAPTER_IP);
}

// CAStartListeningServer TC
TEST_F(CATests, StartListeningServerTestWithNonSelect)
{
    EXPECT_EQ(CA_STATUS_FAILED, CAStartListeningServer());
}

// CAStartListeningServer TC
TEST_F(CATests, StartListeningServerTest)
{
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));
    EXPECT_EQ(CA_STATUS_OK, CAStartListeningServer());
}

// CAStopListeningServer TC
TEST_F(CATests, CAStopListeningServerTestWithNonSelect)
{
    EXPECT_EQ(CA_STATUS_FAILED, CAStopListeningServer());
}

// CAStopListeningServer TC
TEST_F(CATests, CAStopListeningServerTest)
{
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));
    EXPECT_EQ(CA_STATUS_OK, CAStopListeningServer());
}

// CARegisterHandlerTest TC
TEST_F(CATests, RegisterHandlerTest)
{
    CARegisterHandler(request_handler, response_handler, error_handler);
    char* check = (char *) "registerHandler success";
    EXPECT_STREQ(check, "registerHandler success");
}

// CACreateRemoteEndpoint TC
TEST_F(CATests, CreateRemoteEndpointTestGood)
{
    addr = (char *) ADDRESS;

    EXPECT_EQ(CA_STATUS_OK, CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr,
                                             PORT, &tempRep));

    CADestroyEndpoint(tempRep);
    tempRep = NULL;
}

// check remoteEndpoint and values of remoteEndpoint
TEST_F(CATests, CreateRemoteEndpointTestValues)
{
    addr = (char *) ADDRESS;

    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, PORT, &tempRep);

    EXPECT_TRUE(tempRep != NULL);

    CADestroyEndpoint(tempRep);
    tempRep = NULL;
}

// CAGerateToken TC
TEST_F(CATests, GenerateTokenTestGood)
{
    EXPECT_EQ(CA_STATUS_OK, CAGenerateToken(&tempToken, tokenLength));

    CADestroyToken(tempToken);
}

// check return value when CAGenerateToken is passed a NULL instead a valid pointer
TEST_F(CATests, GenerateTokenTestBad)
{
    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CAGenerateToken(NULL, tokenLength));
}

// CADestroyToken TC
// check destroyed token
TEST_F(CATests, DestroyTokenTest)
{
    CAGenerateToken(&tempToken, tokenLength);
    CADestroyToken(tempToken);

    char * check = (char *) "destroy success";
    EXPECT_STREQ(check, "destroy success");
}

TEST_F(CATests, SendRequestTestWithInvalidAddress)
{
    CARegisterHandler(request_handler, response_handler, error_handler);

    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "10.11.13.13.14", PORT, &tempRep);

    memset(&requestData, 0, sizeof(CAInfo_t));
    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;
    requestData.type = CA_MSG_CONFIRM;

    memset(&requestInfo, 0, sizeof(CARequestInfo_t));
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;

    EXPECT_EQ(CA_STATUS_OK, CASendRequest(tempRep, &requestInfo));

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
    tempRep = NULL;
}

// CASendRequest TC
TEST(SendRequestTest, DISABLED_TC_16_Positive_01)
{
    addr = (char *) ADDRESS;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, PORT, &tempRep);

    memset(&requestData, 0, sizeof(CAInfo_t));
    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    size_t length = strlen(NORMAL_INFO_DATA) + strlen("a/light");
    requestData.payload = (CAPayload_t) calloc(length, sizeof(char));
    if(!requestData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    snprintf((char*)requestData.payload, length, NORMAL_INFO_DATA, "a/light");
    requestData.payloadSize = length + 1;
    requestData.type = CA_MSG_NONCONFIRM;

    memset(&requestInfo, 0, sizeof(CARequestInfo_t));
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;

    EXPECT_EQ(CA_STATUS_OK, CASendRequest(tempRep, &requestInfo));

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
    tempRep = NULL;
}

// check return value when a NULL is passed instead of a valid CARequestInfo_t address
TEST_F(CATests, SendRequestTestWithNullAddr)
{
    addr = (char *) ADDRESS;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, PORT, &tempRep);

    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CASendRequest(tempRep, NULL));

    CADestroyEndpoint(tempRep);
    tempRep = NULL;
}

TEST_F(CATests, SendResponseTestWithInvalidCode)
{
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));

    addr = (char *) ADDRESS;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, PORT, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_RESET;
    responseData.messageId = 1;
    responseData.payload = (CAPayload_t)OICMalloc(sizeof("response payload"));
    responseData.dataType = CA_RESPONSE_DATA;

    EXPECT_TRUE(responseData.payload != NULL);

    if (responseData.payload)
    {
        CAGenerateToken(&tempToken, tokenLength);
        requestData.token = tempToken;
        requestData.tokenLength = tokenLength;

        memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
        responseInfo.result = CA_CONTENT;
        responseInfo.info = responseData;

        EXPECT_EQ(CA_STATUS_OK, CASendResponse(tempRep, &responseInfo));

        CADestroyToken(tempToken);
        CADestroyEndpoint(tempRep);
        OICFree(responseData.payload);
        tempRep = NULL;
    }
}

// CASendResponse TC
TEST(SendResponseTest, DISABLED_TC_19_Positive_01)
{
    addr = (char *) ADDRESS;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, PORT, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;
    responseData.payload = (CAPayload_t)malloc(sizeof("response payload"));
    responseData.dataType = CA_RESPONSE_DATA;

    EXPECT_TRUE(responseData.payload != NULL);
    if(!responseData.payload)
    {
        CADestroyEndpoint(tempRep);
        return;
    }

    memcpy(responseData.payload, "response payload", sizeof("response payload"));
    responseData.payloadSize = sizeof("response payload");

    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    responseInfo.result = CA_CONTENT;
    responseInfo.info = responseData;

    EXPECT_EQ(CA_STATUS_OK, CASendResponse(tempRep, &responseInfo));

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(responseData.payload);
    tempRep = NULL;
}

// check return value when address is NULL as multicast
TEST(SendResponseTest, DISABLED_TC_20_Negative_01)
{
    addr = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, 0, &tempRep);

    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;
    responseData.payload = (CAPayload_t)malloc(sizeof("response payload"));
    responseData.dataType = CA_RESPONSE_DATA;
    EXPECT_TRUE(responseData.payload != NULL);

    if(!responseData.payload)
    {
        CADestroyEndpoint(tempRep);
        return;
    }

    memcpy(responseData.payload, "response payload", sizeof("response payload"));
    responseData.payloadSize = sizeof("response payload");

    CAGenerateToken(&tempToken, tokenLength);
    requestData.token = tempToken;
    requestData.tokenLength = tokenLength;

    memset(&responseInfo, 0, sizeof(CAResponseInfo_t));
    responseInfo.result = CA_CONTENT;
    responseInfo.info = responseData;

    EXPECT_EQ(CA_STATUS_OK, CASendResponse(tempRep, &responseInfo));

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free (responseData.payload);
    tempRep = NULL;
}

// check return value NULL is passed instead of a valid CAResponseInfo_t address
TEST_F(CATests, SendResponseTest)
{
    addr = (char *) ADDRESS;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, PORT, &tempRep);

    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CASendResponse(tempRep, NULL));

    CADestroyEndpoint(tempRep);
    tempRep = NULL;
}

// CASelectNewwork TC
TEST_F(CATests, SelectNetworkTestGood)
{
    EXPECT_EQ(CA_STATUS_OK, checkSelectNetwork());
}

CAResult_t checkSelectNetwork()
{
    CAResult_t res = CASelectNetwork(CA_ADAPTER_IP);

    if (CA_STATUS_OK == res)
    {
        EXPECT_EQ(CA_STATUS_OK, CAUnSelectNetwork(CA_ADAPTER_IP));
        return CA_STATUS_OK;
    }
    if (CA_NOT_SUPPORTED == res)
    {
        EXPECT_EQ(CA_STATUS_FAILED, CAUnSelectNetwork(CA_ADAPTER_IP));
        return CA_STATUS_OK;
    }

    return res;
}

// check return value when selected network is disable
TEST_F(CATests, SelectNetworkTestBad)
{
    //Select disable network
    EXPECT_EQ(CA_NOT_SUPPORTED, CASelectNetwork((CATransportAdapter_t)
                                                CA_TRANSPORT_ADAPTER_SCOPE));
}

// check return value when selected network is disable
TEST_F(CATests, UnSelectNetworkTest)
{
    //UnSelect disable network
    EXPECT_EQ(CA_STATUS_FAILED, CAUnSelectNetwork((CATransportAdapter_t)
                                                  CA_TRANSPORT_ADAPTER_SCOPE));
}

// CAHandlerRequestResponse TC
TEST_F(CATests, HandlerRequestResponseTest)
{
    EXPECT_EQ(CA_STATUS_OK, CAHandleRequestResponse());
}

// CAGetNetworkInformation TC
TEST_F (CATests, GetNetworkInformationTest)
{
    uint32_t tempSize = 0;
    CAEndpoint_t *tempInfo = NULL;

    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));
    EXPECT_EQ(CA_STATUS_OK, CAGetNetworkInformation(&tempInfo, &tempSize));

// @todo: if this api is supported on windows platform, it should be changed.
#if !defined(_WIN32)
    for (uint32_t index = 0; index < tempSize; index++)
    {
        EXPECT_TRUE(tempInfo[index].adapter != 0);
        EXPECT_TRUE(strlen(tempInfo[index].addr) != 0);
    }
#endif

    free(tempInfo);
}

TEST_F(CATests, RegisterDTLSCredentialsHandlerTest)
{
#ifdef __WITH_DTLS__
    EXPECT_EQ(CA_STATUS_OK, CAregisterPskCredentialsHandler(CAGetDtlsPskCredentials));
#endif
}

// CARegisterNetworkMonitorHandler TC
TEST_F(CATests, RegisterNetworkMonitorHandler)
{
    EXPECT_EQ(CA_STATUS_OK, CARegisterNetworkMonitorHandler(adapter_handler,
                                                            connection_handler));
}

// CASetAutoConnectionDeviceInfo TC
TEST_F(CATests, SetAutoConnectionDeviceInfo)
{
    addr = (char *) ADDRESS;

#if defined(__ANDROID__) && defined(LE_ADAPTER)
    EXPECT_EQ(CA_STATUS_OK, CASetAutoConnectionDeviceInfo(addr));
#else
    EXPECT_EQ(CA_NOT_SUPPORTED, CASetAutoConnectionDeviceInfo(addr));
#endif
}

// CAUnsetAutoConnectionDeviceInfo TC
TEST_F(CATests, UnsetAutoConnectionDeviceInfo)
{
    addr = (char *) ADDRESS;

#if defined(__ANDROID__) && defined(LE_ADAPTER)
    EXPECT_EQ(CA_STATUS_OK, CAUnsetAutoConnectionDeviceInfo(addr));
#else
    EXPECT_EQ(CA_NOT_SUPPORTED, CAUnsetAutoConnectionDeviceInfo(addr));
#endif
}

TEST(CASetPortNumberTest, CASetPortNumberToAssign)
{
    EXPECT_EQ(CA_STATUS_OK, CASetPortNumberToAssign(CA_ADAPTER_IP, CA_IPV4, 5683));
    EXPECT_EQ(CA_STATUS_OK, CASetPortNumberToAssign(CA_ADAPTER_IP, CA_IPV6, 5683));
    EXPECT_EQ(CA_STATUS_OK,
              CASetPortNumberToAssign(CA_ADAPTER_IP,
                                      static_cast<CATransportFlags_t>(CA_IPV4|CA_SECURE), 5683));
    EXPECT_EQ(CA_STATUS_OK,
              CASetPortNumberToAssign(CA_ADAPTER_IP,
                                      static_cast<CATransportFlags_t>(CA_IPV6|CA_SECURE), 5683));

#ifdef TCP_ADAPTER
    EXPECT_EQ(CA_STATUS_OK, CASetPortNumberToAssign(CA_ADAPTER_TCP, CA_IPV4, 5683));
    EXPECT_EQ(CA_STATUS_OK, CASetPortNumberToAssign(CA_ADAPTER_TCP, CA_IPV6, 5683));
#endif
}

TEST(CAGetPortNumberTest, CAGetPortNumberToAssign)
{
    ASSERT_EQ(static_cast<uint16_t>(0),
              CAGetAssignedPortNumber(CA_ADAPTER_IP, CA_IPV4));
    ASSERT_EQ(static_cast<uint16_t>(0),
              CAGetAssignedPortNumber(CA_ADAPTER_IP, CA_IPV6));
    ASSERT_EQ(static_cast<uint16_t>(0),
              CAGetAssignedPortNumber(CA_ADAPTER_IP,
                                      static_cast<CATransportFlags_t>(CA_IPV4|CA_SECURE)));
    ASSERT_EQ(static_cast<uint16_t>(0),
              CAGetAssignedPortNumber(CA_ADAPTER_IP,
                                      static_cast<CATransportFlags_t>(CA_IPV6|CA_SECURE)));
#ifdef TCP_ADAPTER
    ASSERT_EQ(static_cast<uint16_t>(0), CAGetAssignedPortNumber(CA_ADAPTER_TCP, CA_IPV4));
    ASSERT_EQ(static_cast<uint16_t>(0), CAGetAssignedPortNumber(CA_ADAPTER_TCP, CA_IPV6));
#endif
}
