/*******************************************************************************
 *
 * Copyright (c) 2015 Bosch Software Innovations GmbH, Germany.
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
 *    Bosch Software Innovations GmbH - Please refer to git log
 *
 *******************************************************************************/

#include "tests.h"
#include "CUnit/Basic.h"
#include "internals.h"
#include "liblwm2m.h"
#include "memtest.h"


static void test_tlv_new(void)
{
   MEMORY_TRACE_BEFORE;
   lwm2m_data_t *dataP =  lwm2m_data_new(10);
   CU_ASSERT_PTR_NOT_NULL(dataP);
   MEMORY_TRACE_AFTER(<);
}

static void test_tlv_free(void)
{
   MEMORY_TRACE_BEFORE;
   lwm2m_data_t *dataP =  lwm2m_data_new(10);
   CU_ASSERT_PTR_NOT_NULL_FATAL(dataP);
   lwm2m_data_free(10, dataP);
   MEMORY_TRACE_AFTER_EQ;
}

static void test_decodeTLV()
{
    MEMORY_TRACE_BEFORE;
    uint8_t data1[] = {0xC3, 55, 1, 2, 3};
    uint8_t data2[] = {0x28, 2, 3, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    uint8_t data3[0x194] = {0x90, 33, 1, 0x90 };
    lwm2m_data_type_t type;
    uint16_t id = 0;
    size_t   index = 0;
    size_t   length = 0;
    int result;


    result = lwm2m_decode_TLV(data1, sizeof(data1) - 1, &type, &id, &index, &length);
    CU_ASSERT_EQUAL(result, 0);

    result = lwm2m_decode_TLV(data1, sizeof(data1), &type, &id, &index, &length);
    CU_ASSERT_EQUAL(result, 5);
    CU_ASSERT_EQUAL(type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(id, 55);
    CU_ASSERT_EQUAL(index, 2);
    CU_ASSERT_EQUAL(length, 3);

    result = lwm2m_decode_TLV(data2, sizeof(data2) - 1, &type, &id, &index, &length);
    CU_ASSERT_EQUAL(result, 0);

    result = lwm2m_decode_TLV(data2, sizeof(data2), &type, &id, &index, &length);
    CU_ASSERT_EQUAL(result, 13);
    CU_ASSERT_EQUAL(type, LWM2M_TYPE_OBJECT_INSTANCE);
    CU_ASSERT_EQUAL(id, 0x0203);
    CU_ASSERT_EQUAL(index, 4);
    CU_ASSERT_EQUAL(length, 9);

    result = lwm2m_decode_TLV(data3, sizeof(data3) - 1, &type, &id, &index, &length);
    CU_ASSERT_EQUAL(result, 0);

    result = lwm2m_decode_TLV(data3, sizeof(data3), &type, &id, &index, &length);
    CU_ASSERT_EQUAL(result, 0x194);
    CU_ASSERT_EQUAL(type, LWM2M_TYPE_MULTIPLE_RESOURCE);
    CU_ASSERT_EQUAL(id, 33);
    CU_ASSERT_EQUAL(index, 4);
    CU_ASSERT_EQUAL(length, 0x190);

    MEMORY_TRACE_AFTER_EQ;
}

static void test_opaqueToInt()
{
    MEMORY_TRACE_BEFORE;
    // Data represented in big endian, two'2 complement
    uint8_t data1[] = {1 };
    uint8_t data2[] = {1, 2 };
    uint8_t data3[] = {0xfe, 0xfd, 0xfc, 0xfc};
    uint8_t data4[] = {0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf8};
    // Result in platform endianess
    int64_t value = 0;
    int length;


    length = utils_opaqueToInt(data1, sizeof(data1), &value);
    CU_ASSERT_EQUAL(length, 1);
    CU_ASSERT_EQUAL(value, 1);

    length = utils_opaqueToInt(data2, sizeof(data2), &value);
    CU_ASSERT_EQUAL(length, 2);
    CU_ASSERT_EQUAL(value, 0x102);

    length = utils_opaqueToInt(data3, sizeof(data3), &value);
    CU_ASSERT_EQUAL(length, 4);
    CU_ASSERT_EQUAL(value, -0x1020304);

    length = utils_opaqueToInt(data4, sizeof(data4), &value);
    CU_ASSERT_EQUAL(length, 8);
    CU_ASSERT_EQUAL(value, -0x102030405060708);

    MEMORY_TRACE_AFTER_EQ;
}

static void test_tlv_parse()
{
    MEMORY_TRACE_BEFORE;
    // Resource 55 {1, 2, 3}
    uint8_t data1[] = {0xC3, 55, 1, 2, 3};
    // Instance 0x203 {Resource 55 {1, 2, 3}, Resource 66 {4, 5, 6, 7, 8, 9, 10, 11, 12 } }
    uint8_t data2[] = {0x28, 2, 3, 17, 0xC3, 55, 1, 2, 3, 0xC8, 66, 9, 4, 5, 6, 7, 8, 9, 10, 11, 12, };
    // Instance 11 {MultiResource 11 {ResourceInstance 0 {1, 2, 3}, ResourceInstance 1 {4, 5, 6, 7, 8, 9, ... } }
    uint8_t data3[174] = {0x08, 11, 171, 0x88, 77, 168, 0x43, 0, 1, 2, 3, 0x48, 1, 160, 4, 5, 6, 7, 8, 9};
    int result;
    lwm2m_data_t *dataP;
    lwm2m_data_t *tlvSubP;

    result = lwm2m_data_parse(NULL, data1, sizeof(data1), LWM2M_CONTENT_TLV, &dataP);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dataP);
    CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(dataP->id, 55);
    CU_ASSERT_EQUAL(dataP->value.asBuffer.length, 3);
    CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, &data1[2], 3));
    lwm2m_data_free(result, dataP);

    result = lwm2m_data_parse(NULL, data2, sizeof(data2), LWM2M_CONTENT_TLV, &dataP);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dataP);
    CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OBJECT_INSTANCE);
    CU_ASSERT_EQUAL(dataP->id, 0x203);
    CU_ASSERT_EQUAL(dataP->value.asChildren.count, 2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dataP->value.asChildren.array);
    tlvSubP = dataP->value.asChildren.array;

    CU_ASSERT_EQUAL(tlvSubP[0].type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(tlvSubP[0].id, 55);
    CU_ASSERT_EQUAL(tlvSubP[0].value.asBuffer.length, 3);
    CU_ASSERT(0 == memcmp(tlvSubP[0].value.asBuffer.buffer, &data2[6], 3));

    CU_ASSERT_EQUAL(tlvSubP[1].type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(tlvSubP[1].id, 66);
    CU_ASSERT_EQUAL(tlvSubP[1].value.asBuffer.length, 9);
    CU_ASSERT(0 == memcmp(tlvSubP[1].value.asBuffer.buffer, &data2[12], 9));
    lwm2m_data_free(result, dataP);

    result = lwm2m_data_parse(NULL, data3, sizeof(data3), LWM2M_CONTENT_TLV, &dataP);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dataP);
    CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OBJECT_INSTANCE);
    CU_ASSERT_EQUAL(dataP->id, 11);
    CU_ASSERT_EQUAL(dataP->value.asChildren.count, 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dataP->value.asChildren.array);
    tlvSubP = dataP->value.asChildren.array;

    CU_ASSERT_EQUAL(tlvSubP[0].type, LWM2M_TYPE_MULTIPLE_RESOURCE);
    CU_ASSERT_EQUAL(tlvSubP[0].id, 77);
    CU_ASSERT_EQUAL(tlvSubP[0].value.asChildren.count, 2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(tlvSubP[0].value.asChildren.array);
    tlvSubP = tlvSubP[0].value.asChildren.array;

    CU_ASSERT_EQUAL(tlvSubP[0].type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(tlvSubP[0].id, 0);
    CU_ASSERT_EQUAL(tlvSubP[0].value.asBuffer.length, 3);
    CU_ASSERT(0 == memcmp(tlvSubP[0].value.asBuffer.buffer, &data3[8], 3));

    CU_ASSERT_EQUAL(tlvSubP[1].type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(tlvSubP[1].id, 1);
    CU_ASSERT_EQUAL(tlvSubP[1].value.asBuffer.length, 160);
    CU_ASSERT(0 == memcmp(tlvSubP[1].value.asBuffer.buffer, &data3[14], 160));
    lwm2m_data_free(result, dataP);

    MEMORY_TRACE_AFTER_EQ;
}

static void test_tlv_serialize()
{
    MEMORY_TRACE_BEFORE;

    int result;
    lwm2m_data_t *dataP;
    lwm2m_data_t *tlvSubP;
    lwm2m_data_t *tlvRscInstP;
    uint8_t data1[] = {1, 2, 3, 4};
    uint8_t data2[170] = {5, 6, 7, 8};
    uint8_t* buffer;



    tlvSubP = lwm2m_data_new(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(tlvSubP);

    tlvSubP[0].id = 66;
    lwm2m_data_encode_opaque(data1, sizeof(data1), tlvSubP);
    CU_ASSERT_EQUAL(tlvSubP[0].type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(tlvSubP[0].value.asBuffer.length, sizeof(data1));
    CU_ASSERT_PTR_NOT_NULL_FATAL(tlvSubP[0].value.asBuffer.buffer);

    tlvSubP[1].type = LWM2M_TYPE_MULTIPLE_RESOURCE;
    tlvSubP[1].id = 77;
    tlvSubP[1].value.asChildren.count = 1;
    tlvSubP[1].value.asChildren.array = lwm2m_data_new(1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(tlvSubP[1].value.asChildren.array);
    tlvRscInstP = tlvSubP[1].value.asChildren.array;

    tlvRscInstP[0].id = 0;
    lwm2m_data_encode_opaque(data2, sizeof(data2), tlvRscInstP);
    CU_ASSERT_EQUAL(tlvRscInstP[0].type, LWM2M_TYPE_OPAQUE);
    CU_ASSERT_EQUAL(tlvRscInstP[0].value.asBuffer.length, sizeof(data2));
    CU_ASSERT_PTR_NOT_NULL_FATAL(tlvRscInstP[0].value.asBuffer.buffer);

    dataP = lwm2m_data_new(1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dataP);
    dataP->id = 3;
    lwm2m_data_include(tlvSubP, 2, dataP);
    CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OBJECT_INSTANCE);
    CU_ASSERT_EQUAL(dataP->value.asChildren.count, 2);
    CU_ASSERT_EQUAL(dataP->value.asChildren.array, tlvSubP);

    lwm2m_media_type_t media_type = LWM2M_CONTENT_TLV;
    result = lwm2m_data_serialize(NULL, 1, dataP, &media_type, &buffer);
    CU_ASSERT_EQUAL(result, sizeof(data2) + sizeof(data1) + 11);

    CU_ASSERT_EQUAL(buffer[0], 0x08);
    CU_ASSERT_EQUAL(buffer[1], 3);
    CU_ASSERT_EQUAL(buffer[2], sizeof(data2) + sizeof(data1) + 8);

    CU_ASSERT_EQUAL(buffer[3], 0xC0 + sizeof(data1));
    CU_ASSERT_EQUAL(buffer[4], 66);
    CU_ASSERT(0 == memcmp(data1, &buffer[5], sizeof(data1)));

    CU_ASSERT_EQUAL(buffer[5 + sizeof(data1)], 0x88);
    CU_ASSERT_EQUAL(buffer[6 + sizeof(data1)], 77);
    CU_ASSERT_EQUAL(buffer[7 + sizeof(data1)], sizeof(data2) + 3);

    CU_ASSERT_EQUAL(buffer[8 + sizeof(data1)], 0x48);
    CU_ASSERT_EQUAL(buffer[9 + sizeof(data1)], 0);
    CU_ASSERT_EQUAL(buffer[10 + sizeof(data1)], sizeof(data2));
    CU_ASSERT(0 == memcmp(data2, &buffer[11 + sizeof(data1)], sizeof(data2)));

    lwm2m_data_free(1, dataP);
    lwm2m_free(buffer);

    MEMORY_TRACE_AFTER_EQ;
}

static void test_tlv_int(void)
{
   MEMORY_TRACE_BEFORE;
   int64_t value;
   int result;
   lwm2m_data_t *dataP =  lwm2m_data_new(1);
   CU_ASSERT_PTR_NOT_NULL(dataP);

   result = lwm2m_data_decode_int(dataP, &value);
   CU_ASSERT_EQUAL(result, 0);

   lwm2m_data_encode_int(0x12, dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_INTEGER);
   CU_ASSERT_EQUAL(dataP->value.asInteger, 0x12);
   result = lwm2m_data_decode_int(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, 0x12);

   lwm2m_data_encode_string("18", dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_STRING);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, 2);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, "18", 2));
   result = lwm2m_data_decode_int(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, 18);

   lwm2m_data_encode_string("-14678", dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_STRING);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, 6);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, "-14678", 6));
   result = lwm2m_data_decode_int(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, -14678);

   uint8_t data1[] = { 0xed, 0xcc };
   lwm2m_data_encode_opaque(data1, sizeof(data1), dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OPAQUE);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, sizeof(data1));
   CU_ASSERT_PTR_NOT_NULL_FATAL(dataP->value.asBuffer.buffer);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, data1, sizeof(data1)));
   result = lwm2m_data_decode_int(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, -0x1234);
   lwm2m_free(dataP->value.asBuffer.buffer);

   uint8_t data2[] = { 0x7f, 0x34, 0x56, 0x78, 0x91, 0x22, 0x33, 0x44 };
   lwm2m_data_encode_opaque(data2, sizeof(data2), dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OPAQUE);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, sizeof(data2));
   CU_ASSERT_PTR_NOT_NULL_FATAL(dataP->value.asBuffer.buffer);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, data2, sizeof(data2)));
   result = lwm2m_data_decode_int(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, 0x7f34567891223344);

   lwm2m_data_free(1, dataP);
   MEMORY_TRACE_AFTER_EQ;
}

static void test_tlv_bool(void)
{
   MEMORY_TRACE_BEFORE;
   bool value;
   int result;
   lwm2m_data_t *dataP =  lwm2m_data_new(1);
   CU_ASSERT_PTR_NOT_NULL(dataP);

   result = lwm2m_data_decode_bool(dataP, &value);
   CU_ASSERT_EQUAL(result, 0);

   lwm2m_data_encode_bool(true, dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_BOOLEAN);
   CU_ASSERT_EQUAL(dataP->value.asBoolean, true);
   result = lwm2m_data_decode_bool(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, true);

   lwm2m_data_encode_bool(false, dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_BOOLEAN);
   CU_ASSERT_EQUAL(dataP->value.asBoolean, false);
   result = lwm2m_data_decode_bool(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, false);

   lwm2m_data_encode_string("1", dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_STRING);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, 1);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, "1", 1));
   result = lwm2m_data_decode_bool(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, true);

   lwm2m_data_encode_string("0", dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_STRING);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, 1);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, "0", 1));
   result = lwm2m_data_decode_bool(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, false);

   uint8_t data1[] = { 0x00 };
   lwm2m_data_encode_opaque(data1, sizeof(data1), dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OPAQUE);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, sizeof(data1));
   CU_ASSERT_PTR_NOT_NULL_FATAL(dataP->value.asBuffer.buffer);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, data1, sizeof(data1)));
   result = lwm2m_data_decode_bool(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, false);
   lwm2m_free(dataP->value.asBuffer.buffer);

   uint8_t data2[] = { 0x01 };
   lwm2m_data_encode_opaque(data2, sizeof(data2), dataP);
   CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_OPAQUE);
   CU_ASSERT_EQUAL(dataP->value.asBuffer.length, sizeof(data2));
   CU_ASSERT_PTR_NOT_NULL_FATAL(dataP->value.asBuffer.buffer);
   CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, data2, sizeof(data2)));
   result = lwm2m_data_decode_bool(dataP, &value);
   CU_ASSERT_EQUAL(result, 1);
   CU_ASSERT_EQUAL(value, true);

   lwm2m_data_free(1, dataP);
   MEMORY_TRACE_AFTER_EQ;
}

static void test_tlv_float(void)
{
    MEMORY_TRACE_BEFORE;
    double value;
    int result;
    lwm2m_data_t *dataP = lwm2m_data_new(1);
    CU_ASSERT_PTR_NOT_NULL(dataP);

    result = lwm2m_data_decode_float(dataP, &value);
    CU_ASSERT_EQUAL(result, 0);

    lwm2m_data_encode_float(1234.56, dataP);
    CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_FLOAT);
    CU_ASSERT_EQUAL(dataP->value.asFloat, 1234.56);
    result = lwm2m_data_decode_float(dataP, &value);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_EQUAL(value, 1234.56);

    lwm2m_data_encode_string("1234.56", dataP);
    CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_STRING);
    CU_ASSERT_EQUAL(dataP->value.asBuffer.length, 7);
    CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, "1234.56", 7));
    result = lwm2m_data_decode_float(dataP, &value);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_EQUAL(value, 1234.56);

    lwm2m_data_encode_string("-123456789.987", dataP);
    CU_ASSERT_EQUAL(dataP->type, LWM2M_TYPE_STRING);
    CU_ASSERT_EQUAL(dataP->value.asBuffer.length, 14);
    CU_ASSERT(0 == memcmp(dataP->value.asBuffer.buffer, "-123456789.987", 14));
    result = lwm2m_data_decode_float(dataP, &value);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_EQUAL(value, -123456789.987);

    lwm2m_data_free(1, dataP);
    MEMORY_TRACE_AFTER_EQ;
}

static struct TestTable table[] = {
        { "test of lwm2m_data_new()", test_tlv_new },
        { "test of lwm2m_data_free()", test_tlv_free },
        { "test of lwm2m_decodeTLV()", test_decodeTLV },
        { "test of lwm2m_opaqueToInt()", test_opaqueToInt },
        { "test of lwm2m_data_parse()", test_tlv_parse },
        { "test of lwm2m_data_serialize()", test_tlv_serialize },
        { "test of lwm2m_data_encode_int() and lwm2m_data_decode_int()", test_tlv_int },
        { "test of lwm2m_data_encode_bool()and lwm2m_data_decode_bool()", test_tlv_bool },
        { "test of lwm2m_data_encode_float() and lwm2m_data_decode_float()", test_tlv_float },
        { NULL, NULL },
};

CU_ErrorCode create_tlv_suit()
{
   CU_pSuite pSuite = NULL;

   pSuite = CU_add_suite("Suite_TLV", NULL, NULL);
   if (NULL == pSuite) {
      return CU_get_error();
   }

   return add_tests(pSuite, table);
}


