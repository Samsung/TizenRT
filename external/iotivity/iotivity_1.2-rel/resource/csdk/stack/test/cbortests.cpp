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


extern "C"
{
    #include "ocstack.h"
    #include "ocpayload.h"
    #include "ocpayloadcbor.h"
    #include "logger.h"
    #include "oic_malloc.h"
}

#include "gtest/gtest.h"
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
    EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*) payload_in, &payload_cbor, &payload_cbor_size));

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
    EXPECT_EQ(OC_STACK_OK, OCParsePayload(&payload_out, PAYLOAD_TYPE_REPRESENTATION,
                 payload_cbor, payload_cbor_size));

    OCByteString quakedata_out = {NULL, 0};
    ASSERT_EQ(true, OCRepPayloadGetPropByteString((OCRepPayload*)payload_out, "quakedata", &quakedata_out));

    // Compare input and output data
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


TEST_F(CborByteStringTest, ByteStringArrayConvertParseTest )
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
    EXPECT_EQ(OC_STACK_OK, OCConvertPayload((OCPayload*) payload_in, &payload_cbor, &payload_cbor_size));
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
    EXPECT_EQ(OC_STACK_OK, OCParsePayload(&payload_out, PAYLOAD_TYPE_REPRESENTATION,
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
