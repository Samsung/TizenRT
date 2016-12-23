/*******************************************************************************
 *
 * Copyright (c) 2013, 2014, 2015 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    David Gräff - Convert to test case
 *    
 *******************************************************************************/

#include "liblwm2m.h"
#include "internals.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h> // isspace

#include "commandline.h"

#include "tests.h"
#include "CUnit/Basic.h"

static void test_data(const char * uriStr,
                        lwm2m_media_type_t format,
                        lwm2m_data_t * tlvP,
                        int size,
                        const char * id)
{
    lwm2m_uri_t uri;
    uint8_t * buffer;
    int length;
    
    if (uriStr != NULL)
    {
        lwm2m_stringToUri(uriStr, strlen(uriStr), &uri);
    }

    length = lwm2m_data_serialize((uriStr != NULL) ? &uri : NULL, size, tlvP, &format, &buffer);
    if (length <= 0)
    {
        printf("Serialize lwm2m_data_t %s to %s failed.\n", id, format==LWM2M_CONTENT_JSON?"JSON":"TLV");
        //dump_data_t(stdout, size, tlvP, 0);
        CU_TEST_FATAL(CU_FALSE);
    }
    else
    {
        //printf("\n\nSerialize lwm2m_data_t %s:\n", id);
        //output_buffer(stdout, buffer, length, 0);
        lwm2m_free(buffer);
    }
}

static void test_data_and_compare(const char * uriStr,
                        lwm2m_media_type_t format,
                        lwm2m_data_t * tlvP,
                        int size,
                        const char * id,
                        const uint8_t* original_buffer,
                        size_t original_length)
{
    lwm2m_uri_t uri;
    uint8_t * buffer;
    size_t length;
    
    if (uriStr != NULL)
    {
        lwm2m_stringToUri(uriStr, strlen(uriStr), &uri);
    }

    length = lwm2m_data_serialize((uriStr != NULL) ? &uri : NULL, size, tlvP, &format, &buffer);
    if (length <= 0)
    {
        printf("Serialize lwm2m_data_t %s to TLV failed.\n", id);
        //dump_data_t(stdout, size, tlvP, 0);
        CU_TEST_FATAL(CU_FALSE);
        return;
    }

    char* original_compact;
    if (format == LWM2M_CONTENT_JSON) {
        // Remove white spaces from original_buffer if not in a "" context
        // so that the original input string can be compared to the serialized data output,
        // which does not contain additional white spaces.
        original_compact= malloc(original_length);
        char* s = (char*)original_buffer; char* d = original_compact;
        uint8_t in_string_context = 0;
        do
        {
            *d = *s;
            // Toggle "in_string_context" flag if " has been detected and if " is not escaped
            if (*d == '"' && *(d-1) != '\\')
                in_string_context = !in_string_context;
            if(in_string_context || !isspace(*d))
                d++;

        } while(*s++ != 0);
        original_length = strlen(original_compact);
    } else {
        // No JSON format? Just use the original buffer
        original_compact = (char*)original_buffer;
    }

    CU_ASSERT_EQUAL(original_length, length);

    if ((original_length != length) ||
            (memcmp(original_compact, buffer, length) != 0))
    {
        printf("Comparing buffer after parse/serialize failed for %s:\n", id);
        output_buffer(stdout, buffer, length, 0);
        printf("\ninstead of:\n");
        output_buffer(stdout, (uint8_t*)original_compact, original_length, 0);
        CU_TEST_FATAL(CU_FALSE);
    }

    // Free the compact representation of the original buffer
    if (original_compact != (char*)original_buffer)
        free(original_compact);

    lwm2m_free(buffer);
}

// Hack: Due to the TLV format does not store data type information,
// all dataTypes are undefined after parsing. The json serializer does not work with undefined
// data types, therefore hardcode all objects to be strings.
static void make_all_objects_string_types(lwm2m_data_t * tlvP, int size)
{
    int i;

    for (i=0 ; i<size ; ++i)
    {
        if (tlvP[i].type == LWM2M_TYPE_OPAQUE)
        {
            tlvP[i].type = LWM2M_TYPE_STRING;
        }
        else if (tlvP[i].type == LWM2M_TYPE_MULTIPLE_RESOURCE)
        {
            unsigned int j;

            for (j = 0; j < tlvP[i].value.asChildren.count; ++j)
            {
                tlvP[i].value.asChildren.array[j].type = LWM2M_TYPE_STRING;
            }
        }
    }
}

/**
 * @brief Parses the testBuf to an array of lwm2m_data_t objects and serializes the result
 *        to TLV and JSON and if applicable compares it to the original testBuf.
 * @param testBuf The input buffer.
 * @param testLen The length of the input buffer.
 * @param format The format of the testBuf. Maybe LWM2M_CONTENT_TLV or LWM2M_CONTENT_JSON at the moment.
 * @param id The test object id for debug out.
 */
static void test_raw(const char * uriStr,
                     const char * testBuf,
                     size_t testLen,
                     lwm2m_media_type_t format,
                     const char * id)
{
    lwm2m_data_t * tlvP;
    lwm2m_uri_t uri;
    int size;
    
    if (uriStr != NULL)
    {
        lwm2m_stringToUri(uriStr, strlen(uriStr), &uri);
    }

    size = lwm2m_data_parse((uriStr != NULL) ? &uri : NULL, (uint8_t *)testBuf, testLen, format, &tlvP);
    CU_ASSERT_TRUE_FATAL(size>0);

    // Serialize to the same format and compare to the input buffer
    test_data_and_compare(uriStr, format, tlvP, size, id, (uint8_t*)testBuf, testLen);

    // Serialize to the TLV format
    // the reverse is not possible as TLV format loses the data type information
    if (format == LWM2M_CONTENT_JSON)
    {
        test_data(uriStr, LWM2M_CONTENT_TLV, tlvP, size, id);
    }
}

static void test_raw_expected(const char * uriStr,
                              const char * testBuf,
                              size_t testLen,
                              const char * expectBuf,
                              size_t expectLen,
                              lwm2m_media_type_t format,
                              const char * id)
{
    lwm2m_data_t * tlvP;
    lwm2m_uri_t uri;
    int size;
    
    if (uriStr != NULL)
    {
        lwm2m_stringToUri(uriStr, strlen(uriStr), &uri);
    }

    size = lwm2m_data_parse((uriStr != NULL) ? &uri : NULL, (uint8_t *)testBuf, testLen, format, &tlvP);
    CU_ASSERT_TRUE_FATAL(size>0);

    // Serialize to the same format and compare to the input buffer
    test_data_and_compare(uriStr, format, tlvP, size, id, (uint8_t*)expectBuf, expectLen);

    // Serialize to the other format respectivly.
    if (format == LWM2M_CONTENT_TLV)
        test_data(uriStr, LWM2M_CONTENT_JSON, tlvP, size, id);
    else if (format == LWM2M_CONTENT_JSON)
        test_data(uriStr, LWM2M_CONTENT_TLV, tlvP, size, id);
}

static void test_1(void)
{
    const char buffer[] = {0x03, 0x0A, 0xC1, 0x01, 0x14, 0x03, 0x0B, 0xC1, 0x01, 0x15, 0x03, 0x0C, 0xC1, 0x01, 0x16};
    int testLen = sizeof(buffer);
    lwm2m_media_type_t format = LWM2M_CONTENT_TLV;
    lwm2m_data_t * tlvP;
    int size = lwm2m_data_parse(NULL, (uint8_t *)buffer, testLen, format, &tlvP);
    CU_ASSERT_TRUE_FATAL(size>0);
    // Serialize to the same format and compare to the input buffer
    test_data_and_compare(NULL, format, tlvP, size, "1", (uint8_t*)buffer, testLen);

}

static void test_2(void)
{
    const char buffer[] = {
        0xC8, 0x00, 0x14, 0x4F, 0x70, 0x65, 0x6E, 0x20, 0x4D, 0x6F, 0x62, 0x69, 0x6C, 0x65, 0x20,
        0x41, 0x6C, 0x6C, 0x69, 0x61, 0x6E, 0x63, 0x65, 0xC8, 0x01, 0x16, 0x4C, 0x69, 0x67, 0x68,
        0x74, 0x77, 0x65, 0x69, 0x67, 0x68, 0x74, 0x20, 0x4D, 0x32, 0x4D, 0x20, 0x43, 0x6C, 0x69,
        0x65, 0x6E, 0x74, 0xC8, 0x02, 0x09, 0x33, 0x34, 0x35, 0x30, 0x30, 0x30, 0x31, 0x32, 0x33,
        0xC3, 0x03, 0x31, 0x2E, 0x30, 0x86, 0x06, 0x41, 0x00, 0x01, 0x41, 0x01, 0x05, 0x88, 0x07,
        0x08, 0x42, 0x00, 0x0E, 0xD8, 0x42, 0x01, 0x13, 0x88, 0x87, 0x08, 0x41, 0x00, 0x7D, 0x42,
        0x01, 0x03, 0x84, 0xC1, 0x09, 0x64, 0xC1, 0x0A, 0x0F, 0x83, 0x0B, 0x41, 0x00, 0x00, 0xC4,
        0x0D, 0x51, 0x82, 0x42, 0x8F, 0xC6, 0x0E, 0x2B, 0x30, 0x32, 0x3A, 0x30, 0x30, 0xC1, 0x0F,
        0x55};
    test_raw(NULL, buffer, sizeof(buffer), LWM2M_CONTENT_TLV, "2");
}

static void test_3(void)
{
    const char * buffer = "{\"e\":[                                              \
                         {\"n\":\"0\",\"sv\":\"Open Mobile Alliance\"},      \
                         {\"n\":\"1\",\"sv\":\"Lightweight M2M Client\"},    \
                         {\"n\":\"2\",\"sv\":\"345000123\"},                 \
                         {\"n\":\"3\",\"sv\":\"1.0\"},                       \
                         {\"n\":\"6/0\",\"v\":1},                            \
                         {\"n\":\"6/1\",\"v\":5},                            \
                         {\"n\":\"7/0\",\"v\":3800},                         \
                         {\"n\":\"7/1\",\"v\":5000},                         \
                         {\"n\":\"8/0\",\"v\":125},                          \
                         {\"n\":\"8/1\",\"v\":900},                          \
                         {\"n\":\"9\",\"v\":100},                            \
                         {\"n\":\"10\",\"v\":15},                            \
                         {\"n\":\"11/0\",\"v\":0},                           \
                         {\"n\":\"13\",\"v\":1367491215},                    \
                         {\"n\":\"14\",\"sv\":\"+02:00\"},                   \
                         {\"n\":\"15\",\"sv\":\"U\"}]                        \
                      }";
    test_raw("/3/0", buffer, strlen(buffer), LWM2M_CONTENT_JSON, "3");
}

static void test_4(void)
{
    const char * buffer = "{\"e\":[                                      \
                         {\"n\":\"0\",\"sv\":\"a \\\"test\\\"\"},   \
                         {\"n\":\"1\",\"v\":2015},                  \
                         {\"n\":\"2/0\",\"bv\":true},               \
                         {\"n\":\"2/1\",\"bv\":false}]              \
                      }";
    test_raw("/11/3", buffer, strlen(buffer), LWM2M_CONTENT_JSON, "4");
}

static void test_5(void)
{
    const char * buffer = "{\"bn\" : \"/65/543/\",                \
                       \"e\":[                              \
                         {    \"n\" :   \"0\",              \
                            \"sv\":\"LWM2M\"},              \
                         {\"v\":2015, \"n\":\"1\"},         \
                         {\"n\":\"2/0\",    \"bv\":true},   \
                         {\"bv\": false, \"n\":\"2/1\"}]    \
                      }";
    // We do a string comparison, therefore first n and then v in this order have to appear,
    // because that is the order that serialize will output.
    const char * expect = "{\"bn\" : \"/65/543/\",\"e\":[                              \
                         {  \"n\" :   \"0\", \"sv\":\"LWM2M\"},  \
                         {\"n\":\"1\", \"v\":2015},         \
                         { \"n\":\"2/0\", \"bv\":true},   \
                         {\"n\":\"2/1\", \"bv\": false}]    \
                       }";
    test_raw_expected("/65", buffer, strlen(buffer), expect, strlen(expect), LWM2M_CONTENT_JSON, "5");
}

static void test_6(void)
{
    const char * buffer = "{\"e\":[                              \
                         {\"n\":\"/0\",\"v\":1234}],   \
                       \"bn\" : \"/2/3\"                   \
                      }";

    test_raw(NULL, buffer, strlen(buffer), LWM2M_CONTENT_JSON, "6");
}

static void test_7(void)
{
    const char * buffer = "{\"e\":[                              \
                         {\"n\":\"1\",\"v\":1234},          \
                         {\"n\":\"3\",\"v\":56.789},        \
                         {\"n\":\"2/0\",\"v\":0.23},        \
                         {\"n\":\"2/1\",\"v\":-52.0006}],   \
                       \"bn\" : \"/12/0/\",                  \
                       \"bt\" : 1234567                     \
                      }";

    // We do a string comparison. Because parsing+serialization changes double value
    // precision, we expect a slighty different output than input.
    const char * expect = "{\"e\":[                              \
                            {\"n\":\"0\",\"v\":1234},          \
                            {\"n\":\"1\",\"v\":56.78900146484375},        \
                            {\"n\":\"2/0\",\"v\":0.2300000041723251},        \
                            {\"n\":\"2/1\",\"v\":-52.0005989074707}],    \
                            \"bn\" : \"/12/0/\",                  \
                            \"bt\" : 1234567 \
                          }";
    test_raw_expected(NULL, buffer, strlen(buffer), expect, strlen(expect), LWM2M_CONTENT_JSON, "7a");
    test_raw_expected("/12", buffer, strlen(buffer), expect, strlen(expect), LWM2M_CONTENT_JSON, "7b");
    test_raw_expected("/12/0", buffer, strlen(buffer), expect, strlen(expect), LWM2M_CONTENT_JSON, "7c");
    test_raw_expected("/12/0/3", buffer, strlen(buffer), expect, strlen(expect), LWM2M_CONTENT_JSON, "7d");
}

static void test_8(void)
{
    const char * buffer = "{ \"bn\":\"/\",                                       \
                        \"e\":[                                             \
                          {\"n\":\"34/0/1\",\"sv\":\"8613800755500\"},      \
                          {\"n\":\"34/0/2\",\"v\":1},                       \
                          {\"n\":\"66/0/0\",\"sv\":\"myService1\"},         \
                          {\"n\":\"66/0/1\",\"sv\":\"Internet.15.234\"},    \
                          {\"n\":\"66/1/0\",\"sv\":\"myService2\"},         \
                          {\"n\":\"66/1/1\",\"sv\":\"Internet.15.235\"},    \
                          {\"n\":\"31/0/0\",\"sv\":\"85.76.76.84\"},        \
                          {\"n\":\"31/0/1\",\"sv\":\"85.76.255.255\"}]      \
                      }";

    test_raw(NULL, buffer, strlen(buffer), LWM2M_CONTENT_JSON, "8");
}

static void test_9(void)
{
    const char * buffer = "{ \"bn\":\"/\",                                       \
                        \"e\":[                                             \
                          {\"n\":\"34/0/0/0\",\"ov\":\"66:0\"},             \
                          {\"n\":\"34/0/0/1\",\"ov\":\"66:1\"},             \
                          {\"n\":\"34/0/1\",\"sv\":\"8613800755500\"},      \
                          {\"n\":\"34/0/2\",\"v\":1},                       \
                          {\"n\":\"66/0/0\",\"sv\":\"myService1\"},         \
                          {\"n\":\"66/0/1\",\"sv\":\"Internet.15.234\"},    \
                          {\"n\":\"66/0/2\",\"ov\":\"31:0\"},               \
                          {\"n\":\"66/1/0\",\"sv\":\"myService2\"},         \
                          {\"n\":\"66/1/1\",\"sv\":\"Internet.15.235\"},    \
                          {\"n\":\"66/1/2\",\"ov\":\"FFFF:FFFF\"},          \
                          {\"n\":\"31/0/0\",\"sv\":\"85.76.76.84\"},        \
                          {\"n\":\"31/0/1\",\"sv\":\"85.76.255.255\"}]      \
                      }";

    test_raw(NULL, buffer, strlen(buffer), LWM2M_CONTENT_JSON, "9");
}
static void test_10(void)
{
    lwm2m_data_t * data1 = lwm2m_data_new(17);
    int i;

    for (i = 0; i < 17; i++)
    {
        data1[i].id = i;
    }

    lwm2m_data_encode_int(12, data1);
    lwm2m_data_encode_int(-12, data1 + 1);
    lwm2m_data_encode_int(255, data1 + 2);
    lwm2m_data_encode_int(1000, data1 + 3);
    lwm2m_data_encode_int(-1000, data1 + 4);
    lwm2m_data_encode_int(65535, data1 + 5);
    lwm2m_data_encode_int(3000000, data1 + 6);
    lwm2m_data_encode_int(-3000000, data1 + 7);
    lwm2m_data_encode_int(4294967295, data1 + 8);
    lwm2m_data_encode_int(7000000000, data1 + 9);
    lwm2m_data_encode_int(-7000000000, data1 + 10);
    lwm2m_data_encode_float(3.14, data1 + 11);
    lwm2m_data_encode_float(-3.14, data1 + 12);
    lwm2m_data_encode_float(4E+38, data1 + 13);
    lwm2m_data_encode_float(-4E+38, data1 + 14);
    lwm2m_data_encode_bool(true, data1 + 15);
    lwm2m_data_encode_bool(false, data1 + 16);

    test_data(NULL, LWM2M_CONTENT_TLV, data1, sizeof(data1)/sizeof(lwm2m_data_t), "1");
    test_data(NULL, LWM2M_CONTENT_JSON, data1, sizeof(data1)/sizeof(lwm2m_data_t), "1");
}

static struct TestTable table[] = {
        { "test of test_1()", test_1 },
        { "test of test_2()", test_2 },
        { "test of test_3()", test_3 },
        { "test of test_4()", test_4 },
        { "test of test_5()", test_5 },
        { "test of test_6()", test_6 },
        { "test of test_7()", test_7 },
        { "test of test_8()", test_8 },
        { "test of test_9()", test_9 },
        { "test of test_10()", test_10 },
        { NULL, NULL },
};

CU_ErrorCode create_tlv_json_suit()
{
   CU_pSuite pSuite = NULL;

   pSuite = CU_add_suite("Suite_TLV_JSON", NULL, NULL);
   if (NULL == pSuite) {
      return CU_get_error();
   }

   return add_tests(pSuite, table);
}


