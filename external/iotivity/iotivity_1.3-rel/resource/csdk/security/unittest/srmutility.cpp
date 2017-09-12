// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <gtest/gtest.h>
#include "ocstack.h"
#include "srmutility.h"
#include "oic_string.h"

#define TEST_FILL_CHARACTER ((int)0xfb)

//ParseRestQuery Tests
TEST(ParseRestQueryTest, ParseRestQueryEmpty)
{
    unsigned char query[] = "";
    OicParseQueryIter_t parseIter =  OicParseQueryIter_t();
    ParseQueryIterInit(query, &parseIter);
    EXPECT_EQ(NULL,  GetNextQuery(&parseIter));
}


TEST(ParseRestQueryTest, ParseSingleRestQuery)
{
    char attr[10], val[10];
    unsigned char query[] = "owned=false";

    OicParseQueryIter_t parseIter =  OicParseQueryIter_t();
    ParseQueryIterInit(query, &parseIter);
    EXPECT_NE((OicParseQueryIter_t *)NULL,  GetNextQuery(&parseIter));

    OICStrcpyPartial(attr, sizeof(attr), (char *)parseIter.attrPos, parseIter.attrLen);
    OICStrcpyPartial(val, sizeof(val), (char *)parseIter.valPos, parseIter.valLen);
    printf("\nAttribute: %s  value: %s\n\n", attr, val);
}

TEST(ParseRestQueryTest, ParseRestMultipleQuery)
{
    char attr[10], val[10];
    unsigned char query[] = "oxm=0;owned=true;owner=owner1";

    OicParseQueryIter_t parseIter =  OicParseQueryIter_t();
    ParseQueryIterInit(query, &parseIter);
    printf("\n");
    while(GetNextQuery(&parseIter))
    {
        EXPECT_NE(static_cast<size_t>(0),  parseIter.pi.segment_length);

        OICStrcpyPartial(attr, sizeof(attr), (char *)parseIter.attrPos, parseIter.attrLen);
        OICStrcpyPartial(val, sizeof(val), (char *)parseIter.valPos, parseIter.valLen);
        printf("Attribute: %s  value: %s\n", attr, val);

    }
    printf("\n");
}

TEST(ConvertStrToUuidTest, LowCase)
{
    OicUuid_t uuid;
    memset(&uuid, TEST_FILL_CHARACTER, sizeof(uuid));

    const char *uuidString1 = "ca5bfba0-c0f1-4906-b995-f139d626dfbd";
    EXPECT_EQ(OC_STACK_OK, ConvertStrToUuid(uuidString1, &uuid));

    EXPECT_EQ((uint8_t)0xca, uuid.id[0]);
    EXPECT_EQ((uint8_t)0x5b, uuid.id[1]);
    EXPECT_EQ((uint8_t)0xfb, uuid.id[2]);
    EXPECT_EQ((uint8_t)0xa0, uuid.id[3]);
    EXPECT_EQ((uint8_t)0xc0, uuid.id[4]);
    EXPECT_EQ((uint8_t)0xf1, uuid.id[5]);
    EXPECT_EQ((uint8_t)0x49, uuid.id[6]);
    EXPECT_EQ((uint8_t)0x06, uuid.id[7]);
    EXPECT_EQ((uint8_t)0xb9, uuid.id[8]);
    EXPECT_EQ((uint8_t)0x95, uuid.id[9]);
    EXPECT_EQ((uint8_t)0xf1, uuid.id[10]);
    EXPECT_EQ((uint8_t)0x39, uuid.id[11]);
    EXPECT_EQ((uint8_t)0xd6, uuid.id[12]);
    EXPECT_EQ((uint8_t)0x26, uuid.id[13]);
    EXPECT_EQ((uint8_t)0xdf, uuid.id[14]);
    EXPECT_EQ((uint8_t)0xbd, uuid.id[15]);
}

TEST(ConvertStrToUuidTest, UpCase)
{
    OicUuid_t uuid;
    memset(&uuid, TEST_FILL_CHARACTER, sizeof(uuid));

    const char *uuidString1 = "2320EF4B-D714-4936-93E5-846E1B5DA202";
    EXPECT_EQ(OC_STACK_OK, ConvertStrToUuid(uuidString1, &uuid));

    EXPECT_EQ((uint8_t)0x23, uuid.id[0]);
    EXPECT_EQ((uint8_t)0x20, uuid.id[1]);
    EXPECT_EQ((uint8_t)0xef, uuid.id[2]);
    EXPECT_EQ((uint8_t)0x4b, uuid.id[3]);
    EXPECT_EQ((uint8_t)0xd7, uuid.id[4]);
    EXPECT_EQ((uint8_t)0x14, uuid.id[5]);
    EXPECT_EQ((uint8_t)0x49, uuid.id[6]);
    EXPECT_EQ((uint8_t)0x36, uuid.id[7]);
    EXPECT_EQ((uint8_t)0x93, uuid.id[8]);
    EXPECT_EQ((uint8_t)0xe5, uuid.id[9]);
    EXPECT_EQ((uint8_t)0x84, uuid.id[10]);
    EXPECT_EQ((uint8_t)0x6e, uuid.id[11]);
    EXPECT_EQ((uint8_t)0x1b, uuid.id[12]);
    EXPECT_EQ((uint8_t)0x5d, uuid.id[13]);
    EXPECT_EQ((uint8_t)0xa2, uuid.id[14]);
    EXPECT_EQ((uint8_t)0x02, uuid.id[15]);
}

TEST(ConvertStrToUuidTest, MixedCase)
{
    OicUuid_t uuid;
    memset(&uuid, TEST_FILL_CHARACTER, sizeof(uuid));

    const char *uuidString1 = "6905485c-9693-4CF7-9350-4eB39022af7d";
    EXPECT_EQ(OC_STACK_OK, ConvertStrToUuid(uuidString1, &uuid));

    EXPECT_EQ((uint8_t)0x69, uuid.id[0]);
    EXPECT_EQ((uint8_t)0x05, uuid.id[1]);
    EXPECT_EQ((uint8_t)0x48, uuid.id[2]);
    EXPECT_EQ((uint8_t)0x5c, uuid.id[3]);
    EXPECT_EQ((uint8_t)0x96, uuid.id[4]);
    EXPECT_EQ((uint8_t)0x93, uuid.id[5]);
    EXPECT_EQ((uint8_t)0x4c, uuid.id[6]);
    EXPECT_EQ((uint8_t)0xf7, uuid.id[7]);
    EXPECT_EQ((uint8_t)0x93, uuid.id[8]);
    EXPECT_EQ((uint8_t)0x50, uuid.id[9]);
    EXPECT_EQ((uint8_t)0x4e, uuid.id[10]);
    EXPECT_EQ((uint8_t)0xb3, uuid.id[11]);
    EXPECT_EQ((uint8_t)0x90, uuid.id[12]);
    EXPECT_EQ((uint8_t)0x22, uuid.id[13]);
    EXPECT_EQ((uint8_t)0xaf, uuid.id[14]);
    EXPECT_EQ((uint8_t)0x7d, uuid.id[15]);
}

TEST(ConvertStrToUuidTest, EmptyStringConversion)
{
    OicUuid_t uuid;
    memset(&uuid, TEST_FILL_CHARACTER, sizeof(uuid));

    const char *uuidString1 = "";
    EXPECT_EQ(OC_STACK_OK, ConvertStrToUuid(uuidString1, &uuid));

    for (size_t i = 0; i < sizeof(uuid.id); i++)
    {
        EXPECT_EQ((uint8_t)0, uuid.id[i]);
    }
}

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
TEST(SetUuidSeedTest, NullParam)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SetDeviceIdSeed(NULL, 0));
}

TEST(SetUuidSeedTest, InvalidParam)
{
    uint8_t seed[1024] = {0};

    EXPECT_EQ(OC_STACK_INVALID_PARAM, SetDeviceIdSeed(seed, 0));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, SetDeviceIdSeed(seed, sizeof(seed)));
}

TEST(SetUuidSeedTest, ValidValue)
{
    uint8_t seed[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                        0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    EXPECT_EQ(OC_STACK_OK, SetDeviceIdSeed(seed, sizeof(seed)));
}
#endif
