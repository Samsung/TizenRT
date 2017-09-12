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

// Warning disabled globally but VS2013 ignores the /wd4200 option in C++ files.
#if defined(_MSC_VER) && _MSC_VER < 1900
#pragma warning(disable : 4200)
#endif

#include <gtest/gtest.h>
#include "cainterface.h"
#include "cautilinterface.h"
#include "cacommon.h"
#include "cablockwisetransfer.h"

#define LARGE_PAYLOAD_LENGTH    1024

class CABlockTransferTests : public testing::Test {
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

TEST_F(CABlockTransferTests, CACreateNewDataSetTest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.type = CA_MSG_NONCONFIRM;

    requestData.payload = (CAPayload_t) calloc(LARGE_PAYLOAD_LENGTH, sizeof(char));
    if (!requestData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    memset(requestData.payload, '1', sizeof(requestData.payload) - 1);
    requestData.payloadSize = sizeof(requestData.payload);
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        CAData_t *getData = CAGetDataSetFromBlockDataList(currData->blockDataId);
        EXPECT_TRUE(getData != NULL);

        EXPECT_EQ(CA_STATUS_OK, CARemoveBlockDataFromList(currData->blockDataId));
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

TEST_F(CABlockTransferTests, CARemoveBlockDataFromListWithSeed)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.type = CA_MSG_NONCONFIRM;

    requestData.payload = (CAPayload_t) calloc(LARGE_PAYLOAD_LENGTH, sizeof(char));
    if (!requestData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    memset(requestData.payload, '1', sizeof(requestData.payload) - 1);
    requestData.payloadSize = sizeof(requestData.payload);
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        EXPECT_EQ(CA_STATUS_OK, CARemoveBlockDataFromListWithSeed(tempToken,
                                                                  CA_MAX_TOKEN_LEN,
                                                                  tempRep->addr,
                                                                  tempRep->port));
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

TEST_F(CABlockTransferTests, CAGetBlockDataFromBlockDataListTest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        CABlockData_t *data = CAGetBlockDataFromBlockDataList(currData->blockDataId);
        EXPECT_TRUE(data != NULL);

        EXPECT_EQ(CA_STATUS_OK, CARemoveBlockDataFromList(currData->blockDataId));
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

TEST_F(CABlockTransferTests, CAGetPayloadFromBlockDataListTest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        size_t fullPayload = 0;
        CAPayload_t payload = CAGetPayloadFromBlockDataList(currData->blockDataId,
                                                            &fullPayload);

        size_t payloadLen = (payload != NULL) ? strlen((const char*) payload) : 0;
        EXPECT_TRUE(fullPayload == payloadLen);

        CARemoveBlockDataFromList(currData->blockDataId);
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

// request and block option1
TEST_F(CABlockTransferTests, CAAddBlockOptionTest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;

    requestData.payload = (CAPayload_t) calloc(LARGE_PAYLOAD_LENGTH, sizeof(char));
    if(!requestData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    memset(requestData.payload, '1', sizeof(requestData.payload) - 1);
    requestData.payloadSize = sizeof(requestData.payload);
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    EXPECT_EQ(CA_STATUS_OK, CAAddBlockOption(&pdu, &requestData, tempRep, &options));

    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

// request and block option1
TEST_F(CABlockTransferTests, CAAddBlockOption1InRequest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;

    requestData.payload = (CAPayload_t) calloc(LARGE_PAYLOAD_LENGTH, sizeof(char));
    if(!requestData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    memset(requestData.payload, '1', sizeof(requestData.payload) - 1);
    requestData.payloadSize = sizeof(requestData.payload);
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        EXPECT_EQ(CA_STATUS_OK, CAUpdateBlockOptionType(currData->blockDataId,
                                                        COAP_OPTION_BLOCK1));

        EXPECT_EQ(CA_STATUS_OK, CAAddBlockOption1(&pdu, &requestData,
                                                  requestData.payloadSize,
                                                  currData->blockDataId, &options));
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

// response and block option1
TEST_F(CABlockTransferTests, CAAddBlockOption1InResponse)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t responseData;
    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.token = tempToken;
    responseData.tokenLength = CA_MAX_TOKEN_LEN;
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;

    responseData.payload = (CAPayload_t) calloc(LARGE_PAYLOAD_LENGTH, sizeof(char));
    if(!responseData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    memset(responseData.payload, '1', sizeof(responseData.payload) - 1);
    responseData.payloadSize = sizeof(responseData.payload);

    pdu = CAGeneratePDU(CA_CREATED, &responseData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        EXPECT_EQ(CA_STATUS_OK, CAUpdateBlockOptionType(currData->blockDataId,
                                                        COAP_OPTION_BLOCK1));

        EXPECT_EQ(CA_STATUS_OK, CAAddBlockOption1(&pdu, &responseData,
                                                  responseData.payloadSize,
                                                  currData->blockDataId, &options));
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(responseData.payload);
}

// response and block option2
TEST_F(CABlockTransferTests, CAAddBlockOption2InResponse)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t responseData;
    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.token = tempToken;
    responseData.tokenLength = CA_MAX_TOKEN_LEN;
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;

    responseData.payload = (CAPayload_t) calloc(LARGE_PAYLOAD_LENGTH, sizeof(char));
    if(!responseData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    memset(responseData.payload, '1', sizeof(responseData.payload) - 1);
    responseData.payloadSize = sizeof(responseData.payload);

    pdu = CAGeneratePDU(CA_CREATED, &responseData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        EXPECT_EQ(CA_STATUS_OK, CAUpdateBlockOptionType(currData->blockDataId,
                                                        COAP_OPTION_BLOCK2));

        EXPECT_EQ(CA_STATUS_OK, CAAddBlockOption2(&pdu, &responseData,
                                                  responseData.payloadSize,
                                                  currData->blockDataId, &options));
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(responseData.payload);
}

// request and block option1
TEST_F(CABlockTransferTests, CAAddBlockOption2InRequest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;

    requestData.payload = (CAPayload_t) calloc(LARGE_PAYLOAD_LENGTH, sizeof(char));
    if(!requestData.payload)
    {
        CADestroyToken(tempToken);
        FAIL() << "requestData.payload allocation failed";
    }
    memset(requestData.payload, '1', sizeof(requestData.payload) - 1);
    requestData.payloadSize = sizeof(requestData.payload);
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    if (currData)
    {
        EXPECT_EQ(CA_STATUS_OK, CAUpdateBlockOptionType(currData->blockDataId,
                                                        COAP_OPTION_BLOCK2));

        EXPECT_EQ(CA_STATUS_OK, CAAddBlockOption2(&pdu, &requestData,
                                                  requestData.payloadSize,
                                                  currData->blockDataId, &options));
    }

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

TEST_F(CABlockTransferTests, CAGetBlockSizeOptionFromPduTest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    size_t totalPayloadLen = 0;
    EXPECT_FALSE(CAIsPayloadLengthInPduWithBlockSizeOption(pdu, COAP_OPTION_SIZE1,
                                                           &totalPayloadLen));

    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

TEST_F(CABlockTransferTests, CASetNextBlockOption1WithRequest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    coap_block_t block = {0, 0, 0};
    EXPECT_EQ(CA_STATUS_OK, CASetNextBlockOption1(pdu, tempRep, cadata, block, pdu->length));

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

TEST_F(CABlockTransferTests, CASetNextBlockOption1WithResponse)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t responseData;
    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.token = tempToken;
    responseData.tokenLength = CA_MAX_TOKEN_LEN;
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;

    pdu = CAGeneratePDU(CA_CREATED, &responseData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    coap_block_t block = {0, 0, 0};
    EXPECT_EQ(CA_STATUS_OK, CASetNextBlockOption1(pdu, tempRep, cadata, block, pdu->length));

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(responseData.payload);
}

TEST_F(CABlockTransferTests, CASetNextBlockOption2WithRequest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    memset(&requestData, 0, sizeof(CAInfo_t));
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.type = CA_MSG_NONCONFIRM;

    pdu = CAGeneratePDU(CA_GET, &requestData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    coap_block_t block = {0, 0, 0};
    EXPECT_EQ(CA_STATUS_OK, CASetNextBlockOption2(pdu, tempRep, cadata, block, pdu->length));

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(requestData.payload);
}

TEST_F(CABlockTransferTests, CASetNextBlockOption2WithResponse)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    coap_pdu_t *pdu = NULL;
    coap_list_t *options = NULL;
    coap_transport_t transport = COAP_UDP;

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t responseData;
    memset(&responseData, 0, sizeof(CAInfo_t));
    responseData.token = tempToken;
    responseData.tokenLength = CA_MAX_TOKEN_LEN;
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.messageId = 1;

    pdu = CAGeneratePDU(CA_CREATED, &responseData, tempRep, &options, &transport);

    CAData_t *cadata = CACreateNewDataSet(pdu, tempRep);
    EXPECT_TRUE(cadata != NULL);

    CABlockData_t *currData = CACreateNewBlockData(cadata);
    EXPECT_TRUE(currData != NULL);

    coap_block_t block = {0, 0, 0};
    EXPECT_EQ(CA_STATUS_OK, CASetNextBlockOption2(pdu, tempRep, cadata, block, pdu->length));

    CADestroyDataSet(cadata);
    coap_delete_list(options);
    coap_delete_pdu(pdu);

    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
    free(responseData.payload);
}

TEST_F(CABlockTransferTests, CAUpdatePayloadToCADataWithRequest)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t requestData;
    requestData.type = CA_MSG_NONCONFIRM;
    requestData.token = tempToken;
    requestData.tokenLength = CA_MAX_TOKEN_LEN;
    requestData.options = NULL;
    requestData.payload = NULL;
    requestData.payloadSize = 0;

    CARequestInfo_t requestInfo;
    requestInfo.method = CA_GET;
    requestInfo.info = requestData;
    requestInfo.isMulticast = false;

    CAData_t cadata;
    cadata.type = SEND_TYPE_UNICAST;
    cadata.remoteEndpoint = tempRep;
    cadata.requestInfo = &requestInfo;
    cadata.dataType = CA_REQUEST_DATA;

    CAPayload_t payload = (CAPayload_t) "requestPayload";
    size_t payloadLen = strlen((const char*) payload) + 1;

    EXPECT_EQ(CA_STATUS_OK, CAUpdatePayloadToCAData(&cadata, payload, payloadLen));

    EXPECT_STREQ((const char*) payload, (const char*) cadata.requestInfo->info.payload);

    free(cadata.requestInfo->info.payload);
    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
}

TEST_F(CABlockTransferTests, CAUpdatePayloadToCADataWithResponse)
{
    CAEndpoint_t* tempRep = NULL;
    CACreateEndpoint(CA_DEFAULT_FLAGS, CA_ADAPTER_IP, "127.0.0.1", 5683, &tempRep);

    CAToken_t tempToken = NULL;
    CAGenerateToken(&tempToken, CA_MAX_TOKEN_LEN);

    CAInfo_t responseData;
    responseData.type = CA_MSG_NONCONFIRM;
    responseData.token = tempToken;
    responseData.tokenLength = CA_MAX_TOKEN_LEN;
    responseData.options = NULL;
    responseData.payload = NULL;
    responseData.payloadSize = 0;

    CAResponseInfo_t responseInfo;
    responseInfo.result = CA_VALID;
    responseInfo.info = responseData;

    CAData_t cadata;
    cadata.type = SEND_TYPE_UNICAST;
    cadata.remoteEndpoint = tempRep;
    cadata.responseInfo = &responseInfo;
    cadata.dataType = CA_RESPONSE_DATA;

    CAPayload_t payload = (CAPayload_t) "responsePayload";
    size_t payloadLen = strlen((const char*) payload) + 1;

    EXPECT_EQ(CA_STATUS_OK, CAUpdatePayloadToCAData(&cadata, payload, payloadLen));

    EXPECT_STREQ((const char*) payload, (const char*) cadata.responseInfo->info.payload);

    free(cadata.responseInfo->info.payload);
    CADestroyToken(tempToken);
    CADestroyEndpoint(tempRep);
}
