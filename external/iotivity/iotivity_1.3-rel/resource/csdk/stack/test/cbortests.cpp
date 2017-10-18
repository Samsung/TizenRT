//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "iotivity_config.h"

extern "C"
{
    #include "ocstack.h"
    #include "ocpayload.h"
    #include "ocpayloadcbor.h"
    #include "logger.h"
    #include "oic_malloc.h"
}

#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <stdint.h>

#include "gtest_helper.h"

//-----------------------------------------------------------------------------
// CBOR_BIN_STRING_DEBUG may be defined when compiling to save the output of
// the OC payload conversion to CBOR to a file for further examination with any
// CBOR related tools.
//-----------------------------------------------------------------------------
//#define CBOR_BIN_STRING_DEBUG

class CborByteStringTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            // Create Payload
            payload_in = OCRepPayloadCreate();
            ASSERT_TRUE(payload_in != NULL);
        }

        virtual void TearDown() {
            OCPayloadDestroy((OCPayload*)payload_in);
        }

        OCRepPayload* payload_in;
};

TEST_F(CborByteStringTest, ByteStringSetGetTest)
{
    OCRepPayloadSetUri(payload_in, "/a/quake_sensor");
    OCRepPayloadSetPropInt(payload_in, "scale", 4);

    uint8_t binval[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0xA, 0xB, 0xC,
                        0xD, 0xE, 0xF};
    OCByteString quakedata_in = { binval, sizeof(binval)};

    EXPECT_EQ(true, OCRepPayloadSetPropByteString(payload_in, "quakedata", quakedata_in));

    OCByteString quakedata_out = { NULL, 0};
    ASSERT_EQ(true, OCRepPayloadGetPropByteString(payload_in, "quakedata", &quakedata_out));
    ASSERT_NE((uint8_t*)NULL, quakedata_out.bytes);
    EXPECT_EQ(quakedata_in.len, quakedata_out.len);
    EXPECT_EQ(0, memcmp(quakedata_in.bytes, quakedata_out.bytes, quakedata_in.len));

    // Cleanup
    OICFree(quakedata_out.bytes);
}

TEST_F(CborByteStringTest, ByteStringConvertParseTest)
{
    OCRepPayloadSetUri(payload_in, "/a/quake_sensor");
    OCRepPayloadSetPropInt(payload_in, "scale", 4);

    uint8_t binval[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0xA, 0xB, 0xC,
                        0xD, 0xE, 0xF};
    OCByteString quakedata_in = { binval, sizeof(binval)};

    // Set ByteString in Payload
    EXPECT_EQ(true, OCRepPayloadSetPropByteString(payload_in, "quakedata", quakedata_in));

    // Convert OCPayload to CBOR
    uint8_t *payload_cbor = NULL;
    size_t payload_cbor_size = 0;
    EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*) payload_in, OC_FORMAT_CBOR,
            &payload_cbor, &payload_cbor_size));

#ifdef CBOR_BIN_STRING_DEBUG
    FILE *fp = fopen("binstring.cbor", "wb+");
    if (fp)
    {
        fwrite(payload_cbor, 1, payload_cbor_size, fp);
        fclose(fp);
    }
#endif //CBOR_BIN_STRING_DEBUG

    // Parse CBOR back to OCPayload
    OCPayload* payload_out = NULL;
    EXPECT_EQ(OC_STACK_OK, OCParsePayload(&payload_out, OC_FORMAT_CBOR, PAYLOAD_TYPE_REPRESENTATION,
                 payload_cbor, payload_cbor_size));

    OCByteString quakedata_out = {NULL, 0};
    ASSERT_EQ(true, OCRepPayloadGetPropByteString((OCRepPayload*)payload_out, "quakedata", &quakedata_out));

    // Compare input and output data
    ASSERT_NE((uint8_t*)NULL, quakedata_out.bytes);
    EXPECT_EQ(quakedata_in.len, quakedata_out.len);
    EXPECT_EQ(0, memcmp(quakedata_in.bytes, quakedata_out.bytes, quakedata_in.len));

    // Cleanup
    OICFree(payload_cbor);
    OICFree(quakedata_out.bytes);
    OCPayloadDestroy((OCPayload*)payload_out);
}

TEST_F(CborByteStringTest, ByteStringArraySetGetTest )
{
    OCRepPayloadSetUri(payload_in, "/a/quake_sensor");
    OCRepPayloadSetPropInt(payload_in, "scale", 4);

    size_t dimensions_in[MAX_REP_ARRAY_DEPTH] = { 3, 0, 0};
    uint8_t binval1[] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19};
    uint8_t binval2[] = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29};
    uint8_t binval3[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

    OCByteString quakedata_in[3] = {{binval1, sizeof(binval1)},
                                    {binval2, sizeof(binval2)},
                                    {binval3, sizeof(binval3)}};

    EXPECT_EQ(true, OCRepPayloadSetByteStringArray(payload_in, "quakedata",
                quakedata_in, dimensions_in));

    OCByteString* quakedata_out = NULL;
    size_t dimensions_out[MAX_REP_ARRAY_DEPTH] = {0};
    ASSERT_EQ(true, OCRepPayloadGetByteStringArray(payload_in, "quakedata",
                &quakedata_out, dimensions_out));

    for(size_t i = 0; i < dimensions_in[0]; i++)
    {
        EXPECT_EQ(quakedata_in[i].len, quakedata_out[i].len);
        EXPECT_EQ(0, memcmp(quakedata_in[i].bytes, quakedata_out[i].bytes, quakedata_in[i].len));
    }

    // Cleanup
    for(size_t i = 0; i < dimensions_out[0]; i++)
    {
        OICFree(quakedata_out[i].bytes);
    }
    OICFree(quakedata_out);
}

TEST_F(CborByteStringTest, ByteStringArrayConvertParseTest)
{
    OCRepPayloadSetUri(payload_in, "/a/quake_sensor");
    OCRepPayloadSetPropInt(payload_in, "scale", 4);

    size_t dimensions_in[MAX_REP_ARRAY_DEPTH] = { 3, 0, 0};
    uint8_t binval1[] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19};
    uint8_t binval2[] = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29};
    uint8_t binval3[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

    OCByteString quakedata_in[3] = {{binval1, sizeof(binval1)},
                                    {binval2, sizeof(binval2)},
                                    {binval3, sizeof(binval3)}};

    EXPECT_EQ(true, OCRepPayloadSetByteStringArray(payload_in, "quakedata",
                quakedata_in, dimensions_in));

    // Convert OCPayload to CBOR
    uint8_t *payload_cbor = NULL;
    size_t payload_cbor_size = 0;
    EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*) payload_in, OC_FORMAT_CBOR,
            &payload_cbor, &payload_cbor_size));
#ifdef CBOR_BIN_STRING_DEBUG
    FILE *fp = fopen("binstringarr.cbor", "wb+");
    if (fp)
    {
        fwrite(payload_cbor, 1, payload_cbor_size, fp);
        fclose(fp);
    }
#endif //CBOR_BIN_STRING_DEBUG

    // Parse CBOR back to OCPayload
    OCPayload* payload_out = NULL;
    EXPECT_EQ(OC_STACK_OK, OCParsePayload(&payload_out, OC_FORMAT_CBOR, PAYLOAD_TYPE_REPRESENTATION,
                payload_cbor, payload_cbor_size));

    OCByteString* quakedata_out = NULL;
    size_t dimensions_out[MAX_REP_ARRAY_DEPTH] = {0};
    ASSERT_EQ(true, OCRepPayloadGetByteStringArray((OCRepPayload*)payload_out, "quakedata",
                &quakedata_out, dimensions_out));

    for(size_t i = 0; i < dimensions_in[0]; i++)
    {
        EXPECT_EQ(quakedata_in[i].len, quakedata_out[i].len);
        EXPECT_EQ(0, memcmp(quakedata_in[i].bytes, quakedata_out[i].bytes, quakedata_in[i].len));
    }

    // Cleanup
    OICFree(payload_cbor);
    for(size_t i = 0; i < dimensions_out[0]; i++)
    {
        OICFree(quakedata_out[i].bytes);
    }
    OICFree(quakedata_out);

    OCPayloadDestroy((OCPayload*)payload_out);
}

TEST_F(CborByteStringTest, EmptyByteStringConvertParseTest)
{
    OCByteString bytestring_in = {NULL, 0};

    // Set ByteString in Payload
    EXPECT_EQ(true, OCRepPayloadSetPropByteString(payload_in, "bytestring", bytestring_in));

    // Convert OCPayload to CBOR
    uint8_t *payload_cbor = NULL;
    size_t payload_cbor_size = 0;
    EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*) payload_in, OC_FORMAT_CBOR,
            &payload_cbor, &payload_cbor_size));

#ifdef CBOR_BIN_STRING_DEBUG
    FILE *fp = fopen("emptybinstring.cbor", "wb+");
    if (fp)
    {
        fwrite(payload_cbor, 1, payload_cbor_size, fp);
        fclose(fp);
    }
#endif //CBOR_BIN_STRING_DEBUG

    // Parse CBOR back to OCPayload
    OCPayload* payload_out = NULL;
    EXPECT_EQ(OC_STACK_OK, OCParsePayload(&payload_out, OC_FORMAT_CBOR, PAYLOAD_TYPE_REPRESENTATION,
                 payload_cbor, payload_cbor_size));

    OCByteString bytestring_out = {NULL, 0};
    ASSERT_EQ(true, OCRepPayloadGetPropByteString((OCRepPayload*)payload_out, "bytestring", &bytestring_out));

    // Compare input and output data
    ASSERT_EQ((uint8_t*)NULL, bytestring_out.bytes);
    EXPECT_EQ(bytestring_in.len, bytestring_out.len);

    // Cleanup
    OICFree(payload_cbor);
    OICFree(bytestring_out.bytes);
    OCPayloadDestroy((OCPayload*)payload_out);
}

TEST_F(CborByteStringTest, EmptyByteStringArrayConvertParseTest)
{
    size_t dimensions_in[MAX_REP_ARRAY_DEPTH] = { 3, 0, 0};

    OCByteString bytestring_in[3] = {{NULL, 0},
                                    {NULL, 0},
                                    {NULL, 0}};

    EXPECT_EQ(true, OCRepPayloadSetByteStringArray(payload_in, "bytestring",
                bytestring_in, dimensions_in));

    // Convert OCPayload to CBOR
    uint8_t *payload_cbor = NULL;
    size_t payload_cbor_size = 0;
    EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*) payload_in, OC_FORMAT_CBOR,
            &payload_cbor, &payload_cbor_size));
#ifdef CBOR_BIN_STRING_DEBUG
    FILE *fp = fopen("emptybinstringarr.cbor", "wb+");
    if (fp)
    {
        fwrite(payload_cbor, 1, payload_cbor_size, fp);
        fclose(fp);
    }
#endif //CBOR_BIN_STRING_DEBUG

    // Parse CBOR back to OCPayload
    OCPayload* payload_out = NULL;
    EXPECT_EQ(OC_STACK_OK, OCParsePayload(&payload_out, OC_FORMAT_CBOR, PAYLOAD_TYPE_REPRESENTATION,
                payload_cbor, payload_cbor_size));

    OCByteString* bytestring_out = NULL;
    size_t dimensions_out[MAX_REP_ARRAY_DEPTH] = {0};
    ASSERT_EQ(true, OCRepPayloadGetByteStringArray((OCRepPayload*)payload_out, "bytestring",
                &bytestring_out, dimensions_out));

    for(size_t i = 0; i < dimensions_in[0]; i++)
    {
        EXPECT_EQ(bytestring_in[i].len, bytestring_out[i].len);
        EXPECT_EQ(0, memcmp(bytestring_in[i].bytes, bytestring_out[i].bytes, bytestring_in[i].len));
    }

    // Cleanup
    OICFree(payload_cbor);
    for(size_t i = 0; i < dimensions_out[0]; i++)
    {
        OICFree(bytestring_out[i].bytes);
    }
    OICFree(bytestring_out);

    OCPayloadDestroy((OCPayload*)payload_out);
}

TEST(CborHeterogeneousArrayTest, ConvertParseTest)
{
    OCRepPayload *arr = OCRepPayloadCreate();
    ASSERT_TRUE(arr != NULL);
    EXPECT_TRUE(OCRepPayloadSetPropString(arr, "0", "string"));
    EXPECT_TRUE(OCRepPayloadSetPropDouble(arr, "1", 1.0));
    OCRepPayload *obj = OCRepPayloadCreate();
    ASSERT_TRUE(obj != NULL);
    EXPECT_TRUE(OCRepPayloadSetPropString(obj, "member", "value"));
    EXPECT_TRUE(OCRepPayloadSetPropObjectAsOwner(arr, "2", obj));
    const char *strArray[] = { "string" };
    size_t dim[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };
    EXPECT_TRUE(OCRepPayloadSetStringArray(arr, "3", strArray, dim));

    OCRepPayload* payload_in = OCRepPayloadCreate();
    ASSERT_TRUE(payload_in != NULL);
    EXPECT_TRUE(OCRepPayloadSetPropObjectAsOwner(payload_in, "property", arr));

    // Convert OCPayload to CBOR
    uint8_t *payload_cbor = NULL;
    size_t payload_cbor_size = 0;
    EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*) payload_in, OC_FORMAT_CBOR,
            &payload_cbor, &payload_cbor_size));
#ifdef CBOR_BIN_STRING_DEBUG
    FILE *fp = fopen("binstringhetarr.cbor", "wb+");
    if (fp)
    {
        fwrite(payload_cbor, 1, payload_cbor_size, fp);
        fclose(fp);
    }
#endif //CBOR_BIN_STRING_DEBUG
    OCRepPayloadDestroy(payload_in);

    // Compare that array encoding was used
    uint8_t binstring[] = {
        0xbf, 0x68, 0x70, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x84, 0x66, 0x73, 0x74, 0x72, 0x69,
        0x6e, 0x67, 0xfb, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x66, 0x6d, 0x65, 0x6d,
        0x62, 0x65, 0x72, 0x65, 0x76, 0x61, 0x6c, 0x75, 0x65, 0xff, 0x81, 0x66, 0x73, 0x74, 0x72, 0x69,
        0x6e, 0x67, 0xff
    };
    ASSERT_EQ(sizeof(binstring), payload_cbor_size);
    EXPECT_EQ(0, memcmp(binstring, payload_cbor, payload_cbor_size));

    // Parse CBOR back to OCPayload
    OCPayload* payload_out = NULL;
    EXPECT_EQ(OC_STACK_OK, OCParsePayload(&payload_out, OC_FORMAT_CBOR, PAYLOAD_TYPE_REPRESENTATION,
                                          payload_cbor, payload_cbor_size));

    // Compare values
    EXPECT_TRUE(OCRepPayloadGetPropObject((OCRepPayload*) payload_out, "property", &arr));
    char *str;
    EXPECT_TRUE(OCRepPayloadGetPropString(arr, "0", &str));
    EXPECT_STREQ("string", str);
    OICFree(str);
    double d;
    EXPECT_TRUE(OCRepPayloadGetPropDouble(arr, "1", &d));
    EXPECT_EQ(1.0, d);
    EXPECT_TRUE(OCRepPayloadGetPropObject(arr, "2", &obj));
    OCRepPayloadDestroy(obj);
    char **strArr;
    EXPECT_TRUE(OCRepPayloadGetStringArray(arr, "3", &strArr, dim));
    EXPECT_EQ(1u, dim[0]);
    EXPECT_EQ(0u, dim[1]);
    EXPECT_EQ(0u, dim[2]);
    EXPECT_STREQ("string", strArr[0]);
    for (size_t i = 0; i < dim[0]; ++i)
    {
        OICFree(strArr[i]);
    }
    OICFree(strArr);
    OCRepPayloadDestroy(arr);

    // Cleanup
    OICFree(payload_cbor);
    OCPayloadDestroy(payload_out);
}

TEST(CborEmptyArrayTest, EmptyArrayArraySetGetTest)
{
    OCRepPayload* payload_in = OCRepPayloadCreate();
    ASSERT_TRUE(payload_in != NULL);

    OCRepPayloadSetUri(payload_in, "/a/quake_sensor");
    OCRepPayloadSetPropInt(payload_in, "scale", 4);

    size_t dimensions_in[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};

    EXPECT_EQ(true, OCRepPayloadSetIntArray(payload_in, "quakedata",
                NULL, dimensions_in));

    int64_t* quakedata_out = NULL;
    size_t dimensions_out[MAX_REP_ARRAY_DEPTH] = {0};

    ///TODO: Change Assert to true after fixing OCRepPayloadGetIntArray behaviour
    ASSERT_FALSE(OCRepPayloadGetIntArray(payload_in, "quakedata", &quakedata_out, dimensions_out));

    ASSERT_TRUE(quakedata_out == NULL);

    for(size_t i = 0; i < MAX_REP_ARRAY_DEPTH; i++)
    {
        EXPECT_EQ(0u, dimensions_out[i]);
    }

    // Cleanup
    OCRepPayloadDestroy(payload_in);
}

