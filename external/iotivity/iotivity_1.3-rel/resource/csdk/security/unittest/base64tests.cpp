/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

#include <gtest/gtest.h>
#include "base64.h"
#include "oic_malloc.h"
#include <stdlib.h>
#include <stdint.h>

// Tests for base64 encode function
TEST(B64Test, ValidInputForEncoding)
{
    char buf[128];
    size_t outputLength = 0;
    size_t i = 0;
    B64Result res = B64_OK;

    const char *input = "IoTivity base64~!@#$%^&*()-=0123456789<>?;:'[]{},.\"\\|";

    /**< expected output is generated from
             "http://www.convertstring.com/EncodeDecode/Base64Encode" */
    const char *expectedOutput[53] =
    {
        "SQ==",
        "SW8=",
        "SW9U",
        "SW9UaQ==",
        "SW9UaXY=",
        "SW9UaXZp",
        "SW9UaXZpdA==",
        "SW9UaXZpdHk=",
        "SW9UaXZpdHkg",
        "SW9UaXZpdHkgYg==",
        "SW9UaXZpdHkgYmE=",
        "SW9UaXZpdHkgYmFz",
        "SW9UaXZpdHkgYmFzZQ==",
        "SW9UaXZpdHkgYmFzZTY=",
        "SW9UaXZpdHkgYmFzZTY0",
        "SW9UaXZpdHkgYmFzZTY0fg==",
        "SW9UaXZpdHkgYmFzZTY0fiE=",
        "SW9UaXZpdHkgYmFzZTY0fiFA",
        "SW9UaXZpdHkgYmFzZTY0fiFAIw==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQ=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQl",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXg==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiY=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYq",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCk=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCkt",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPQ==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTA=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAx",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMg==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NQ==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3OA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pg==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj8=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Og==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Oic=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Oidb",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXQ==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXs=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4i",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4iXA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4iXHw="
    };

    for (i = 0; i < strlen(input); i++)
    {
        memset(buf, 0, sizeof(buf));

        size_t expectedLength = strlen(expectedOutput[i]);

        res = b64Encode((const uint8_t *)input, i + 1, buf, 128, &outputLength);

        EXPECT_EQ(B64_OK, res);
        EXPECT_EQ(expectedLength, outputLength);
        EXPECT_EQ(0, strcmp(expectedOutput[i], buf));
    }
}

// Tests for base64 decode function
TEST(B64Test, ValidInputForDecoding)
{
    uint8_t buf[128];
    size_t outputLength = 0;
    size_t i = 0;
    B64Result res = B64_OK;

    const char *input[53] =
    {
        "SQ==",
        "SW8=",
        "SW9U",
        "SW9UaQ==",
        "SW9UaXY=",
        "SW9UaXZp",
        "SW9UaXZpdA==",
        "SW9UaXZpdHk=",
        "SW9UaXZpdHkg",
        "SW9UaXZpdHkgYg==",
        "SW9UaXZpdHkgYmE=",
        "SW9UaXZpdHkgYmFz",
        "SW9UaXZpdHkgYmFzZQ==",
        "SW9UaXZpdHkgYmFzZTY=",
        "SW9UaXZpdHkgYmFzZTY0",
        "SW9UaXZpdHkgYmFzZTY0fg==",
        "SW9UaXZpdHkgYmFzZTY0fiE=",
        "SW9UaXZpdHkgYmFzZTY0fiFA",
        "SW9UaXZpdHkgYmFzZTY0fiFAIw==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQ=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQl",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXg==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiY=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYq",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCk=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCkt",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPQ==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTA=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAx",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMg==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NQ==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3OA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pg==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj8=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Og==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Oic=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Oidb",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXQ==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXs=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4i",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4iXA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4iXHw="
    };
    const char *expectedOutput = "IoTivity base64~!@#$%^&*()-=0123456789<>?;:'[]{},.\"\\|";

    for (i = 0; i < (sizeof(input) / sizeof(char *)); i++)
    {
        memset(buf, 0, sizeof(buf));

        res = b64Decode(input[i], strlen(input[i]), buf, 128, &outputLength);

        EXPECT_EQ(B64_OK, res);
        EXPECT_EQ(i + 1, outputLength);
        EXPECT_EQ(0, memcmp(expectedOutput, buf, i + 1));
    }
}

// Tests for base64 decode function
TEST(B64Test, InvalidInputForDecoding)
{
    uint8_t buf[128] = {0,};
    size_t outputLength = 0;
    size_t i = 0;

    const char *input[53] =
    {
        "SQ=",
        "Sw8=",
        "SW1U",
        "SW9Uaq==",
        "SW9uaXY=",
        "SW91UaXZp",
        "SW9UaXZpdA.==",
        "Sw9UAXZpdHk=",
        "SW9UAXZpdHkg",
        "SW9UaXZpdhkgYg==",
        "SW9UaXZpd5kgYmE=",
        "SW9UaXZ1dHkgYmFz",
        "SW9UaXZpdHkgymFzZQ==",
        "SW9UaXZpdHkgYmFzZTY==",
        "SW9UaXZpdHkgYmFzZTY0=",
        "SW9UaXZpdHkgYmFzZTY0fg=",
        "SW9UaXZpdHkgYmFzZTY0fiE==",
        "SW8UaXZpdHkgYmFzZTY0fiFA",
        "SW9UaxzPDHkgYmFzZTY0fiFAIw==",
        "SW9UaXZpdHKGYmFzZTY0fiFAIyQ=",
        "SW9UaXZpdHkgYmFZztY0fiFAIyQl=",
        "SW8UaXZpdHkgYmFzZTY0fiFAIyQlXg=",
        "#SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiY=",
        "SSW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYq",
        "SW9UaXZpdHkgYmFzZTY0fiFAiyQlXiYqKA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCk===",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKckt",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKcktpQ==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQiXiYqKCktPTA=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAx=",
        "SW9UaXZpdHkgYmFzZTY0fifAIyQlXiYqKCktPTAxmg=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM#1=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM1",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0Nq==",
        "sw9uaxzpdhkgymfzzty0fifaiyqlxiyqKcktptaxmjm0nty=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0nTY3",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3Ok==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3OA==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pg",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pg=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjm0NTY3ODk8Pj9=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXqKCktPTAxMjM0NTY3ODk8Pj87=",
        "SW9UaXZpdHkgYmFzZTY0fiFaIyiYqKCktPTAxMjM0NTY3ODk8Pj87Og==",
        "W9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Oic1=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCkTAxMjM0NTY3ODk8Pj87Oidb==",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYCktPTAxMjM0NTY3ODk8Pj87Oidbxq==",
        "SW9UaXZpdHkgYmzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87Oidbxxs=",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCkPTAxMjM0NTY3ODk8Pj87OidbXXT9",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9la==",
        "SW9UaXZpdHkgYzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9Lc4=",
        "SW9UaXZpdHkgYmFzZ0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9lC4i",
        "SW9UaXZpdHkgYmFzZTY0fiFAIyQlXiYqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9lc4iXA==",
        "SW9UaXZpdHkgYmFzZTY0fiFqKCktPTAxMjM0NTY3ODk8Pj87OidbXXt9LC4ixHw="
    };
    const char *expectedOutput = "IoTivity base64~!@#$%^&*()-=0123456789<>?;:'[]{},.\"\\|";

    for (i = 0; i < (sizeof(input) / sizeof(char *)); i++)
    {
        memset(buf, 0, sizeof(buf));

        if (B64_OK == b64Decode(input[i], strlen(input[i]), buf, 128, &outputLength))
        {
            EXPECT_NE(0, memcmp(expectedOutput, buf, i + 1));
        }
    }
}

/*
 * Expected input and output comes from section 10 of RFC4648
 */
TEST(B64Test, RFC4648_EncodeTestVectors)
{
    char buf[128];
    size_t outputLength = 0;
    B64Result b64Result;
    const char *input[] =
    {
        "",
        "f",
        "fo",
        "foo",
        "foob",
        "fooba",
        "foobar"
    };

    const char *output[] =
    {
        "",
        "Zg==",
        "Zm8=",
        "Zm9v",
        "Zm9vYg==",
        "Zm9vYmE=",
        "Zm9vYmFy"
    };

    const size_t expectedOutputLenth[] =
    {
        0,
        4,
        4,
        4,
        8,
        8,
        8
    };

    size_t bufSize = (sizeof(buf)/sizeof(buf[0]));
    size_t inputArraySize = (sizeof(input) / sizeof(input[0]));
    size_t outputArraySize = (sizeof(output) / sizeof(output[0]));
    size_t expectedOutputLenthArraySize = (sizeof(expectedOutputLenth) / sizeof(
            expectedOutputLenth[0]));

    ASSERT_EQ(inputArraySize, outputArraySize)
            << "Input test data and output test data missmatch.";
    ASSERT_EQ(inputArraySize, expectedOutputLenthArraySize)
            << "Input test data and output test data missmatch.";
    for (size_t i = 0; i < inputArraySize; ++i)
    {
        b64Result = b64Encode((const uint8_t *)input[i], strlen(input[i]), buf, bufSize, &outputLength);
        EXPECT_EQ(B64_OK, b64Result)
                << "Failed to Base64 encode \"" << input[i] << "\" to \"" << output[i] << "\"";
        EXPECT_EQ(0u, outputLength % 4)
                << "The return size for all b64Encode operations should be a multiple of 4.";
        EXPECT_STREQ(output[i], buf)
                << "Failed to Base64 encode \"" << input[i] << "\" to \"" << output[i] << "\"";
        EXPECT_EQ(expectedOutputLenth[i], outputLength);
    }
}


TEST(B64Test, RFC4648_DecodeTestVectors)
{
    uint8_t buf[128] = {0,};
    size_t outputLength = 0;
    B64Result b64Result;

    const char *input[] =
    {
        "",
        "Zg==",
        "Zm8=",
        "Zm9v",
        "Zm9vYg==",
        "Zm9vYmE=",
        "Zm9vYmFy"
    };

    const char *output[] =
    {
        "",
        "f",
        "fo",
        "foo",
        "foob",
        "fooba",
        "foobar"
    };

    const size_t expectedOutputLenth[] =
    {
        0,
        1,
        2,
        3,
        4,
        5,
        6
    };

    size_t bufSize = (sizeof(buf)/sizeof(buf[0]));
    size_t inputArraySize = (sizeof(input) / sizeof(input[0]));
    size_t outputArraySize = (sizeof(output) / sizeof(output[0]));
    size_t expectedOutputLenthArraySize = (sizeof(expectedOutputLenth) / sizeof(
            expectedOutputLenth[0]));

    ASSERT_EQ(inputArraySize, outputArraySize)
            << "Input test data and output test data missmatch.";
    ASSERT_EQ(inputArraySize, expectedOutputLenthArraySize)
            << "Input test data and output test data missmatch.";

    for (size_t i = 0; i < inputArraySize; ++i)
    {
        b64Result = b64Decode(input[i], strlen(input[i]), buf, bufSize, &outputLength);
        EXPECT_EQ(B64_OK, b64Result)
                << "Failed to Base64 decode \"" << input[i] << "\" to \"" << output[i] << "\"";
        EXPECT_STREQ(output[i], (char *)buf)
                << "Failed to Base64 decode \"" << input[i] << "\" to \"" << output[i] << "\"";
        EXPECT_EQ(expectedOutputLenth[i], outputLength);
    }
}

TEST(B64Test, DecodeInputMissingPadding)
{
    uint8_t buf[128] = {0,};
    size_t outputLength = 0;
    B64Result b64Result;

    const char *input[] =
    {
        "Zg",
        "Zg="
    };

    size_t bufSize = (sizeof(buf)/sizeof(buf[0]));
    size_t inputArraySize = (sizeof(input) / sizeof(input[0]));

    for (size_t i = 0; i < inputArraySize; ++i)
    {
        b64Result = b64Decode(input[i], strlen(input[i]), buf, bufSize, &outputLength);
        EXPECT_EQ(B64_INVALID_PARAM, b64Result)
                << "Base64 decode for \"" << input[i] << "\" did not fail as expected.";
    }
}

TEST(B64Test, DecodeInputInvalidCharacters)
{
    uint8_t buf[128] = {0,};
    size_t outputLength = 0;
    B64Result b64Result;
    // Characters '-' and '_' chosen because the are part of other encoding
    // standards, other characters chosen at random just to increase test
    // coverage
    const char *input[] =
    {
        "-a==",
        "_a==",
        "&a==",
        "<>=="
    };

    size_t bufSize = (sizeof(buf)/sizeof(buf[0]));
    size_t inputArraySize = (sizeof(input) / sizeof(input[0]));

    for (size_t i = 0; i < inputArraySize; ++i)
    {
        b64Result = b64Decode(input[i], strlen(input[i]), buf, bufSize, &outputLength);
        EXPECT_EQ(B64_INVALID_PARAM, b64Result)
                << "Base64 decode for \"" << input[i] << "\" did not fail as expected.";
    }
}

TEST(B64Test, DecodeInputInvalidPadding)
{
    uint8_t buf[128] = {0,};
    size_t outputLength = 0;
    B64Result b64Result;
    const char *input[] =
    {
        "Zg==Zg==", // Invalid padding in middle of encoded string
        "Zm8=Zm8=", // Invalid padding in middle of encoded string
        "Z===", // Invalid padding max padding for Base64 string is two '=='
        "====", // Invalid padding max padding for Base64 string is two '=='
        "Zm=v" // Invalid padding no characters should follow padding
    };

    size_t bufSize = (sizeof(buf)/sizeof(buf[0]));
    size_t inputArraySize = (sizeof(input) / sizeof(input[0]));

    for (size_t i = 0; i < inputArraySize; ++i)
    {
        b64Result = b64Decode(input[i], strlen(input[i]), buf, bufSize, &outputLength);
        EXPECT_EQ(B64_INVALID_PARAM, b64Result)
                << "Base64 decode for \"" << input[i] << "\" did not fail as expected.";
    }
}

// verify round trip encoding
TEST(B64Test, EncodeThenDecode)
{

    const char input[] = "This is a string that will be passed into  the Base64 "
                         "encoder.  After it is encoded the encoded result will "
                         "be passed into the Base64 decoded and the result will "
                         "be checked with the original input to make sure the "
                         "round trip results are as expected.";

    size_t b64Size = 0;
    // encode the null character at the end of the string.
    size_t b64BufSize = B64ENCODE_OUT_SAFESIZE(sizeof(input));
    char *b64Buf = (char *)OICCalloc(1, b64BufSize);
    ASSERT_NE(nullptr, b64Buf) << "memory allocation error.";
    EXPECT_EQ(B64_OK, b64Encode((const uint8_t *)input, sizeof(input), b64Buf, b64BufSize, &b64Size));
    EXPECT_EQ(0u, b64Size % 4) <<
                              "The return size for all b64Encode operations should be a multiple of 4.";

    size_t outSize;
    size_t outBufSize = B64DECODE_OUT_SAFESIZE(b64Size);
    uint8_t *outBuf = (uint8_t *)OICCalloc(1, outBufSize);
    if (nullptr == outBuf)
    {
        OICFree(b64Buf);
    }
    ASSERT_NE(nullptr, outBuf) << "memory allocation error.";
    EXPECT_EQ(B64_OK, b64Decode(b64Buf, b64Size, outBuf, outBufSize, &outSize));
    EXPECT_STREQ(input, (char *)outBuf);
    OICFree(b64Buf);
    OICFree(outBuf);
}

TEST(B64Test, Test_B64ENCODE_OUT_SAFESIZE_macro)
{
    EXPECT_EQ(1, B64ENCODE_OUT_SAFESIZE(0)) << "macro tested B64ENCODE_OUT_SAFESIZE(0)";
    EXPECT_EQ(5, B64ENCODE_OUT_SAFESIZE(1)) << "macro tested B64ENCODE_OUT_SAFESIZE(1)";
    EXPECT_EQ(5, B64ENCODE_OUT_SAFESIZE(2)) << "macro tested B64ENCODE_OUT_SAFESIZE(2)";
    EXPECT_EQ(5, B64ENCODE_OUT_SAFESIZE(3)) << "macro tested B64ENCODE_OUT_SAFESIZE(3)";
    EXPECT_EQ(9, B64ENCODE_OUT_SAFESIZE(4)) << "macro tested B64ENCODE_OUT_SAFESIZE(4)";
    EXPECT_EQ(9, B64ENCODE_OUT_SAFESIZE(5)) << "macro tested B64ENCODE_OUT_SAFESIZE(5)";
    EXPECT_EQ(9, B64ENCODE_OUT_SAFESIZE(6)) << "macro tested B64ENCODE_OUT_SAFESIZE(6)";
    EXPECT_EQ(13, B64ENCODE_OUT_SAFESIZE(7)) << "macro tested B64ENCODE_OUT_SAFESIZE(7)";
    EXPECT_EQ(13, B64ENCODE_OUT_SAFESIZE(8)) << "macro tested B64ENCODE_OUT_SAFESIZE(8)";
    EXPECT_EQ(13, B64ENCODE_OUT_SAFESIZE(9)) << "macro tested B64ENCODE_OUT_SAFESIZE(8)";
    EXPECT_EQ(17, B64ENCODE_OUT_SAFESIZE(10)) << "macro tested B64ENCODE_OUT_SAFESIZE(10)";
}

TEST(B64Test, Test_B64DECODE_OUT_SAFESIZE_macro)
{
    // Valid Base64 encode string is multiples of 4 in size.
    EXPECT_EQ(0, B64DECODE_OUT_SAFESIZE(0)) << "macro tested B64DECODE_OUT_SAFESIZE(0)";
    EXPECT_EQ(3, B64DECODE_OUT_SAFESIZE(4)) << "macro tested B64DECODE_OUT_SAFESIZE(4)";
    EXPECT_EQ(6, B64DECODE_OUT_SAFESIZE(8)) << "macro tested B64DECODE_OUT_SAFESIZE(8)";
    EXPECT_EQ(9, B64DECODE_OUT_SAFESIZE(12)) << "macro tested B64DECODE_OUT_SAFESIZE(12)";
    EXPECT_EQ(12, B64DECODE_OUT_SAFESIZE(16)) << "macro tested B64DECODE_OUT_SAFESIZE(16)";
    EXPECT_EQ(15, B64DECODE_OUT_SAFESIZE(20)) << "macro tested B64DECODE_OUT_SAFESIZE(20)";
}

TEST(B64Test, EncodeBufferTooSmall)
{
    // buffer too small the size must be 5. 4 for the first encoded byte + 1
    // for '\0' at end of string
    const char *input = "A";
    char b64Buf[4];
    size_t b64Size = 0;
    EXPECT_EQ(B64_OUTPUT_BUFFER_TOO_SMALL, b64Encode((const uint8_t *)input, strlen(input),
              b64Buf, sizeof(b64Buf) / sizeof(char *), &b64Size));
}

TEST(B64Test, DecodeBufferTooSmall)
{
    uint8_t buf[2] = {0,};
    size_t outputLength = 0;
    const char *input = "Zm9v"; //Decodes to "foo"
    EXPECT_EQ(B64_OUTPUT_BUFFER_TOO_SMALL, b64Decode(input, strlen(input),
              buf, sizeof(buf) / sizeof(uint8_t), &outputLength));
}
