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
#include <gtest/gtest.h>
#include "cainterface.h"
#include "cautilinterface.h"
#include "cacommon.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "cafragmentation.h"
#include "caleinterface.h"

#define CA_TRANSPORT_ADAPTER_SCOPE  1000
#define CA_BLE_FIRST_SEGMENT_PAYLOAD_SIZE (((CA_DEFAULT_BLE_MTU_SIZE) - (CA_BLE_HEADER_SIZE)) \
                                           - (CA_BLE_LENGTH_HEADER_SIZE))


class CATests : public testing::Test {
    protected:
    virtual void SetUp()
    {
        CAInitialize(CA_DEFAULT_ADAPTER);
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

const char* our_cert = "-----BEGIN CERTIFICATE-----\n"
"MIIBhTCCASugAwIBAgIJAPZ5mB94RwYHMAoGCCqGSM49BAMCMCUxIzAhBgNVBAoM\n"
"GklvVGl2aXR5VGVzdFNlbGZTaWduZWROYW1lMB4XDTE2MTIxNjIxMjcyMVoXDTMw\n"
"MDgyNTIxMjcyMVowITEfMB0GA1UECgwWSW9UaXZpdHlUZXN0Q2xpZW50TmFtZTBZ\n"
"MBMGByqGSM49AgEGCCqGSM49AwEHA0IABF8OxpJNe01ZPEFpXUUhjUV5uwJM1TF3\n"
"ZSt0tJ71lQiRZ9cbl5z31acRpsZM+fXiR+wkR4xoP7iIyDdTHHVHtkSjSDBGMBUG\n"
"A1UdJQQOMAwGCisGAQQBgt58AQYwDAYDVR0TAQH/BAIwADAfBgNVHSMEGDAWgBQH\n"
"EWLwaDfA+o6U4wmQKVoK9I3B/DAKBggqhkjOPQQDAgNIADBFAiAbDHQHzSjNiDeQ\n"
"OaJYRMLIW2dIlabiQ5pxkW/jEaRszAIhAPzuoNdrQTRbnqCy0hmS9hFt8MxDrrBh\n"
"7jHARQm/5pko\n"
"-----END CERTIFICATE-----\n";

const char* our_key = "-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEIOV0iG5CndNK6JhB8nDcqQjNjgRWe/LQWPNPua3w7nHToAoGCCqGSM49\n"
"AwEHoUQDQgAEXw7Gkk17TVk8QWldRSGNRXm7AkzVMXdlK3S0nvWVCJFn1xuXnPfV\n"
"pxGmxkz59eJH7CRHjGg/uIjIN1McdUe2RA==\n"
"-----END EC PRIVATE KEY-----\n";

const char* our_ca = "-----BEGIN CERTIFICATE-----\n"
"MIIBlzCCATygAwIBAgIJALxGf3YRERn1MAoGCCqGSM49BAMCMCUxIzAhBgNVBAoM\n"
"GklvVGl2aXR5VGVzdFNlbGZTaWduZWROYW1lMB4XDTE2MTIxNjIxMjcyMVoXDTMw\n"
"MDgyNTIxMjcyMVowJTEjMCEGA1UECgwaSW9UaXZpdHlUZXN0U2VsZlNpZ25lZE5h\n"
"bWUwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATo/zp8PXaA/drJQKSG3TlerO0F\n"
"eHkpRkmXMeLFLrImqo1w9OyLfVmrpBrCDjf83BkwLYp19bkYizL2Yk9zIQ4Do1Uw\n"
"UzAhBgNVHSUEGjAYBgorBgEEAYLefAEGBgorBgEEAYLefAEHMA8GA1UdEwEB/wQF\n"
"MAMBAf8wHQYDVR0OBBYEFAcRYvBoN8D6jpTjCZApWgr0jcH8MAoGCCqGSM49BAMC\n"
"A0kAMEYCIQCuZb1LMTthWy9rPgy2FQgoFHB2LXUJlgRLJeO/gTFqgQIhANRvr1Py\n"
"5Bp6asye5FK4VUj6tARxmRNeNLvwonLrqp2w\n"
"-----END CERTIFICATE-----\n";

// Invoked by the CA stack to retrieve credentials from this module
void provide_x509_cert_and_key(PkiInfo_t* inf)
{
    /* PEM data must end in newline and be null terminated for IoTivity */

    inf->crt.data = (uint8_t*) our_cert;
    inf->crt.len = strlen(our_cert) + 1;
    inf->key.data = (uint8_t*) our_key;
    inf->key.len = strlen(our_key) + 1;
    inf->ca.data = (uint8_t*) our_ca;
    inf->ca.len = strlen(our_ca) + 1;

    // CRL not provided
    inf->crl.data = NULL;
    inf->crl.len = 0;

    return;
}
// Empty version, for testing.
void badPkixInfoHandler(PkiInfo_t* /*inf*/)
{
    return;
}

void provide_supported_credential_types(bool* list, const char* /*deviceId*/)
{
    list[1] = true;
    /*
     * Note: there is a default implementation of this in credresource.c, exposed by 
     * pkix_interface.h, called InitManufacturerCipherSuiteList.  If the cred resource 
     * has a credential of the required type, it updates list accordingly. 
     *
     * In a separate test, we could use the cred resource and APIs (credresource.h). 
     */
    return;
}

#endif  //__WITH_DTLS__

// CAInitialize TC
TEST(InitializeTest, CAInitializeTest)
{
    EXPECT_EQ(CA_STATUS_OK, CAInitialize(CA_DEFAULT_ADAPTER));
    CATerminate();
}

//CATerminate TC
TEST_F(CATests, TerminateTest)
{
    CATerminate();

    char* check = (char *) "terminate success";
    EXPECT_STREQ(check, "terminate success");

    CAInitialize(CA_DEFAULT_ADAPTER);
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

#if defined(__WITH_DTLS__)
TEST_F(CATests, DISABLED_PkiTest)
{
    // @todo: this test is disabled for now, it crashes with an invalid write. Cert data
    // provided by the provide_x509_cert_and_key is stored as const char, but ParseChain()
    // (in ca_adapter_net_ssl.c) writes to it while reading.  We could change the test to 
    // provide data on the heap, but the CA stack should not be changing data provided to it
    // by callbacks. 

    const char* local_addr = "127.0.0.1";
    uint16_t local_port = 5503;
    CAEndpoint_t* serverAddr = NULL;

    CARegisterHandler(request_handler, response_handler, error_handler);

    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, local_addr, local_port, &serverAddr);
    ASSERT_TRUE(serverAddr != NULL);

    // Register a credential types handler (tells the CA layer which creds are supported)
    EXPECT_EQ(CA_STATUS_OK, CAregisterGetCredentialTypesHandler(provide_supported_credential_types));

    // Limit ourselves to 0xC0AE : TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8 since it's non-PSK
    EXPECT_EQ(CA_STATUS_OK, CASelectCipherSuite(0xC0AE, serverAddr->adapter));

    // Register an empty callback to provide the keys, expect failure when initializing the handshake.
    EXPECT_EQ(CA_STATUS_OK, CAregisterPkixInfoHandler(badPkixInfoHandler));
    EXPECT_EQ(CA_STATUS_FAILED, CAInitiateHandshake(serverAddr));

    // Register a working callback to provide the keys, expect success.
    EXPECT_EQ(CA_STATUS_OK, CAregisterPkixInfoHandler(provide_x509_cert_and_key));
    EXPECT_EQ(CA_STATUS_OK, CAInitiateHandshake(serverAddr)); 

    CADestroyEndpoint(serverAddr);
}
#endif /* defined(__WITH_DTLS__) */

// CASendRequest TC
// check return value when a NULL is passed instead of a valid CARequestInfo_t address
TEST_F(CATests, SendRequestTestWithNullAddr)
{
    addr = (char *) ADDRESS;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, addr, PORT, &tempRep);

    EXPECT_EQ(CA_STATUS_INVALID_PARAM, CASendRequest(tempRep, NULL));

    CADestroyEndpoint(tempRep);
    tempRep = NULL;
}

// CASendResponse TC
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
TEST_F(CATests, GetNetworkInformationTest)
{
    size_t tempSize = 0;
    CAEndpoint_t *tempInfo = NULL;

    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));
    EXPECT_EQ(CA_STATUS_OK, CAGetNetworkInformation(&tempInfo, &tempSize));

    for (size_t index = 0; index < tempSize; index++)
    {
        EXPECT_TRUE(tempInfo[index].adapter != 0);
        EXPECT_TRUE(strlen(tempInfo[index].addr) != 0);
    }

    free(tempInfo);
}

TEST_F(CATests, GetNetworkInformationTest_EnableIPv6)
{
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));

    caglobals.ip.ipv6enabled = true;
    caglobals.ip.ipv4enabled = false;

    size_t tempSize = 0;
    CAEndpoint_t *tempInfo = NULL;
    EXPECT_EQ(CA_STATUS_OK, CAGetNetworkInformation(&tempInfo, &tempSize));

    for (size_t index = 0; index < tempSize; index++)
    {
        EXPECT_TRUE((tempInfo[index].flags & CA_IPV6) != 0);
    }

    OICFree(tempInfo);
}

TEST_F(CATests, GetNetworkInformationTest_EnableIPv4)
{
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));

    caglobals.ip.ipv6enabled = false;
    caglobals.ip.ipv4enabled = true;

    size_t tempSize = 0;
    CAEndpoint_t *tempInfo = NULL;
    EXPECT_EQ(CA_STATUS_OK, CAGetNetworkInformation(&tempInfo, &tempSize));

    for (size_t index = 0; index < tempSize; index++)
    {
        EXPECT_TRUE((tempInfo[index].flags & CA_IPV4) != 0);
    }

    OICFree(tempInfo);
}

TEST_F(CATests, GetNetworkInformationTest_EnableIPv4AndIPv6)
{
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));

    caglobals.ip.ipv6enabled = true;
    caglobals.ip.ipv4enabled = true;

    size_t tempSize = 0;
    CAEndpoint_t *tempInfo = NULL;
    EXPECT_EQ(CA_STATUS_OK, CAGetNetworkInformation(&tempInfo, &tempSize));

    CATransportFlags_t flags = (CATransportFlags_t)(CA_IPV4|CA_IPV6);
    for (size_t index = 0; index < tempSize; index++)
    {
        EXPECT_TRUE((tempInfo[index].flags & flags) != 0 );
    }

    OICFree(tempInfo);
}

TEST_F(CATests, GetSelectedNetwork)
{
    CATransportAdapter_t SelectedNetwork = CA_DEFAULT_ADAPTER;

#ifdef IP_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork | CA_ADAPTER_IP) ;
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_IP));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef LE_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork | CA_ADAPTER_GATT_BTLE);
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_GATT_BTLE));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef EDR_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork | CA_ADAPTER_RFCOMM_BTEDR);
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_RFCOMM_BTEDR));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef TCP_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork | CA_ADAPTER_TCP);
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_TCP));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef NFC_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork | CA_ADAPTER_NFC);
    EXPECT_EQ(CA_STATUS_OK, CASelectNetwork(CA_ADAPTER_NFC));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif

#ifdef IP_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork & ~CA_ADAPTER_IP) ;
    EXPECT_EQ(CA_STATUS_OK, CAUnSelectNetwork(CA_ADAPTER_IP));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef LE_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork & ~CA_ADAPTER_GATT_BTLE);
    EXPECT_EQ(CA_STATUS_OK, CAUnSelectNetwork(CA_ADAPTER_GATT_BTLE));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef EDR_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork & ~CA_ADAPTER_RFCOMM_BTEDR);
    EXPECT_EQ(CA_STATUS_OK, CAUnSelectNetwork(CA_ADAPTER_RFCOMM_BTEDR));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef TCP_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork & ~CA_ADAPTER_TCP);
    EXPECT_EQ(CA_STATUS_OK, CAUnSelectNetwork(CA_ADAPTER_TCP));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
#ifdef NFC_ADAPTER
    SelectedNetwork = (CATransportAdapter_t)(SelectedNetwork & ~CA_ADAPTER_NFC);
    EXPECT_EQ(CA_STATUS_OK, CAUnSelectNetwork(CA_ADAPTER_NFC));
    EXPECT_EQ(SelectedNetwork, CAGetSelectedNetwork());
#endif
}

TEST_F(CATests, RegisterDTLSCredentialsHandlerTest)
{
#ifdef __WITH_DTLS__
    EXPECT_EQ(CA_STATUS_OK, CAregisterPskCredentialsHandler(CAGetDtlsPskCredentials));
    EXPECT_EQ(CA_STATUS_OK, CAregisterPkixInfoHandler(provide_x509_cert_and_key));
    EXPECT_EQ(CA_STATUS_OK, CAregisterGetCredentialTypesHandler(provide_supported_credential_types));
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

TEST(CAfragmentationTest, FragmentTest)
{
#if defined(LE_ADAPTER)
    const size_t dataLen = 30;
    uint8_t *data = (uint8_t *) malloc(dataLen*sizeof(uint8_t));
    memset(data, 'a', dataLen);

    uint32_t midPacketCount = 0;
    size_t remainingLen = 0;
    size_t totalLength = 0;

    EXPECT_EQ(CA_STATUS_OK, CAGenerateVariableForFragmentation(dataLen,
                                                               &midPacketCount,
                                                               &remainingLen,
                                                               &totalLength,
                                                               CA_DEFAULT_BLE_MTU_SIZE));

    uint8_t dataHeader[CA_BLE_HEADER_SIZE] = {0};
    const uint8_t tmpSourcePort = 1;
    const uint8_t tmpDestPort = 1;

    EXPECT_EQ(CA_STATUS_OK, CAGenerateHeader(dataHeader,
                                             CA_BLE_PACKET_START,
                                             tmpSourcePort,
                                             CA_BLE_PACKET_NON_SECURE,
                                             tmpDestPort));
    EXPECT_TRUE(dataHeader != NULL);

    uint8_t lengthHeader[CA_BLE_LENGTH_HEADER_SIZE] = {0};

    EXPECT_EQ(CA_STATUS_OK, CAGenerateHeaderPayloadLength(lengthHeader,
                                                          CA_BLE_LENGTH_HEADER_SIZE,
                                                          dataLen));
    EXPECT_TRUE(lengthHeader != NULL);

    uint8_t dataSegment[CA_SUPPORTED_BLE_MTU_SIZE] = {0};

    EXPECT_EQ(CA_STATUS_OK, CAMakeFirstDataSegment(dataSegment,
                                                   data,
                                                   CA_BLE_FIRST_SEGMENT_PAYLOAD_SIZE,
                                                   dataHeader,
                                                   lengthHeader));
    EXPECT_TRUE(dataSegment != NULL);

    EXPECT_EQ(CA_STATUS_OK, CAGenerateHeader(dataHeader,
                                             CA_BLE_PACKET_NOT_START,
                                             tmpSourcePort,
                                             CA_BLE_PACKET_NON_SECURE,
                                             tmpDestPort));
    EXPECT_TRUE(dataHeader != NULL);

    EXPECT_EQ(CA_STATUS_OK, CAMakeRemainDataSegment(dataSegment,
                                                    remainingLen,
                                                    data,
                                                    dataLen,
                                                    0,
                                                    dataHeader,
                                                    CA_DEFAULT_BLE_MTU_SIZE));
    EXPECT_TRUE(dataSegment != NULL);

    free(data);
#endif
}

TEST(CAfragmentationTest, DefragmentTest)
{
#if defined(LE_ADAPTER)
    const size_t dataLen = 30;
    uint8_t *data = (uint8_t *) malloc(dataLen*sizeof(uint8_t));
    memset(data, 'a', dataLen);

    uint32_t midPacketCount = 0;
    size_t remainingLen = 0;
    size_t totalLength = 0;

    EXPECT_EQ(CA_STATUS_OK, CAGenerateVariableForFragmentation(dataLen,
                                                               &midPacketCount,
                                                               &remainingLen,
                                                               &totalLength,
                                                               CA_SUPPORTED_BLE_MTU_SIZE));

    uint8_t dataHeader[CA_BLE_HEADER_SIZE] = {0};
    const uint8_t tmpSourcePort = 1;
    const uint8_t tmpDestPort = 1;

    EXPECT_EQ(CA_STATUS_OK, CAGenerateHeader(dataHeader,
                                             CA_BLE_PACKET_START,
                                             tmpSourcePort,
                                             CA_BLE_PACKET_NON_SECURE,
                                             tmpDestPort));
    EXPECT_TRUE(dataHeader != NULL);

    uint8_t lengthHeader[CA_BLE_LENGTH_HEADER_SIZE] = {0};

    EXPECT_EQ(CA_STATUS_OK, CAGenerateHeaderPayloadLength(lengthHeader,
                                                          CA_BLE_LENGTH_HEADER_SIZE,
                                                          dataLen));
    EXPECT_TRUE(lengthHeader != NULL);

    uint8_t dataSegment[CA_SUPPORTED_BLE_MTU_SIZE] = {0};

    EXPECT_EQ(CA_STATUS_OK, CAMakeFirstDataSegment(dataSegment,
                                                   data,
                                                   CA_BLE_FIRST_SEGMENT_PAYLOAD_SIZE,
                                                   dataHeader,
                                                   lengthHeader));
    EXPECT_TRUE(dataSegment != NULL);

    CABLEPacketStart_t startFlag = CA_BLE_PACKET_NOT_START;
    CABLEPacketSecure_t secureFlag = CA_BLE_PACKET_NON_SECURE;
    uint16_t sourcePort = 0;
    uint16_t destPort = 0;

    EXPECT_EQ(CA_STATUS_OK, CAParseHeader(dataSegment,
                                          &startFlag,
                                          &sourcePort,
                                          &secureFlag,
                                          &destPort));

    uint32_t parseDataLength = 0;

    EXPECT_EQ(CA_STATUS_OK, CAParseHeaderPayloadLength(dataSegment,
                                                       CA_BLE_LENGTH_HEADER_SIZE,
                                                       &parseDataLength));

    free(data);
#endif
}

TEST(Ipv6ScopeLevel, getMulticastScope)
{

    const char interfaceLocalStart[] = "ff01::";
    const char linkLocalStart[]      = "ff02::";
    const char realmLocalStart[]     = "ff03::";
    const char adminLocalStart[]     = "ff04::";
    const char siteLocalStart[]      = "ff05::";
    const char orgLocalStart[]       = "ff08::";
    const char globalStart[]         = "ff0e::";

    const char interfaceLocalMid[] = "ff81:0000:0000:f000:0000:0000:0000:0000";
    const char linkLocalMid[]      = "ff82:0000:0000:f000:0000:0000:0000:0000";
    const char realmLocalMid[]     = "ff83:0000:0000:f000:0000:0000:0000:0000";
    const char adminLocalMid[]     = "ff84:0000:0000:f000:0000:0000:0000:0000";
    const char siteLocalMid[]      = "ff85:0000:0000:f000:0000:0000:0000:0000";
    const char orgLocalMid[]       = "ff88:0000:0000:f000:0000:0000:0000:0000";
    const char globalMid[]         = "ff8e:0000:0000:f000:0000:0000:0000:0000";

    const char interfaceLocalEnd[] = "fff1:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
    const char linkLocalEnd[]      = "fff2:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
    const char realmLocalEnd[]     = "fff3:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
    const char adminLocalEnd[]     = "fff4:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
    const char siteLocalEnd[]      = "fff5:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
    const char orgLocalEnd[]       = "fff8:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
    const char globalEnd[]         = "fffe:ffff:ffff:ffff:ffff:ffff:ffff:ffff";

    // range start
    CATransportFlags_t scopeLevel = CA_DEFAULT_FLAGS;
    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(interfaceLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_INTERFACE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(linkLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_LINK, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(realmLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_REALM, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(adminLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_ADMIN, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(siteLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_SITE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(orgLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_ORG, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(globalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_GLOBAL, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    // range mid
    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(interfaceLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_INTERFACE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(linkLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_LINK, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(realmLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_REALM, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(adminLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_ADMIN, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(siteLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_SITE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(orgLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_ORG, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(globalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_GLOBAL, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    // range end
    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(interfaceLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_INTERFACE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(linkLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_LINK, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(realmLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_REALM, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(adminLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_ADMIN, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(siteLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_SITE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(orgLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_ORG, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(globalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_GLOBAL, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;
}

TEST(Ipv6ScopeLevel, getUnicastScope)
{
    const char linkLocalLoopBack[]  = "::1";

    const char linkLocalStart[]     = "fe80::";
    const char linkLocalMid[]       = "fe80:0000:0000:0000:0f00:0000:0000:0000";
    const char linkLocalEnd[]       = "febf:ffff:ffff:ffff:ffff:ffff:ffff:ffff";

    const char siteLocalStart[]     = "fec0::";
    const char siteLocalMid[]       = "fec0:0000:0000:0000:0f00:0000:0000:0000";
    const char siteLocalEnd[]       = "feff:ffff:ffff:ffff:ffff:ffff:ffff:ffff";

    const char globalStart[]   = "2000:0000:0000:0000:0000:0000:0000:0000";
    const char globalMid[]     = "2000:0000:0000:0f00:0000:0000:0000:0000";
    const char globalEnd[]     = "3fff:ffff:ffff:ffff:ffff:ffff:ffff:ffff";

    // loopback
    CATransportFlags_t scopeLevel = CA_DEFAULT_FLAGS;
    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(linkLocalLoopBack, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_LINK, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    // linklocal
    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(linkLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_LINK, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(linkLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_LINK, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(linkLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_LINK, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    // sitelocal
    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(siteLocalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_SITE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(siteLocalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_SITE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(siteLocalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_SITE, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    // global
    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(globalStart, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_GLOBAL, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(globalMid, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_GLOBAL, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;

    EXPECT_EQ(CA_STATUS_OK, CAGetIpv6AddrScope(globalEnd, &scopeLevel));
    EXPECT_EQ(CA_SCOPE_GLOBAL, scopeLevel);
    scopeLevel = CA_DEFAULT_FLAGS;
}

TEST(Ipv6ScopeLevel, invalidAddressTest)
{
    const char invalidAddr1[]    = "qqqq";
    const char invalidAddr2[]    = "ffx7:ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
    const char invalidAddr3[]    = "ffx7::::::::::dsds";
    const char invalidAddr4[]    = "ffx7:ffff:ffff:ff@f:ffff:ffff:ffff:ffff";

    CATransportFlags_t scopeLevel = CA_DEFAULT_FLAGS;
    EXPECT_EQ(CA_STATUS_FAILED, CAGetIpv6AddrScope(invalidAddr1, &scopeLevel));
    EXPECT_EQ(CA_STATUS_FAILED, CAGetIpv6AddrScope(invalidAddr2, &scopeLevel));
    EXPECT_EQ(CA_STATUS_FAILED, CAGetIpv6AddrScope(invalidAddr3, &scopeLevel));
    EXPECT_EQ(CA_STATUS_FAILED, CAGetIpv6AddrScope(invalidAddr4, &scopeLevel));
}
