/*******************************************************************************
 *
 * Copyright (c) 2015 Intel Corporation and others.
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
 *    David Graeff - Make this a test suite
 *    
 *******************************************************************************/

#include "internals.h"
#include "tests.h"
#include "CUnit/Basic.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

const char * tests[]={"1", "-114" , "2", "0", "-2", "919293949596979899", "-98979969594939291", "999999999999999999999999999999", "1.2" , "0.134" , "432f.43" , "0.01", "1.00000000000002", NULL};
int64_t tests_expected_int[]={1,-114,2,0,-2,919293949596979899,-98979969594939291,-1,-1,-1,-1,-1,-1};
double tests_expected_float[]={1,-114,2,0,-2,9.1929394959698e+17,-9.897996959493928e+16,1e+30,1.2,0.134,-1,0.01,1.00000000000002};

int64_t ints[]={12, -114 , 1 , 134 , 43243 , 0, -215025};
const char* ints_expected[] = {"12","-114","1", "134", "43243","0","-215025"};
double floats[]={12, -114 , -30 , 1.02 , 134.000235 , 0.43243 , 0, -21.5025, -0.0925, 0.98765};
const char* floats_expected[] = {"12","-114","-30", "1.02", "134.000235","0.43243","0","-21.5025","-0.0925","0.98765"};

static void test_lwm2m_PlainTextToInt64(void)
{
    int i;

    for (i = 0 ; tests[i] != NULL ; i++)
    {
        int64_t res;

        if (utils_plainTextToInt64((unsigned char*)tests[i], strlen(tests[i]), &res) != 1)
            res = -1;

        CU_ASSERT_EQUAL(res, tests_expected_int[i]);
        //printf ("%i \"%s\" -> fail (%li)\n", i , tests[i], res );
    }
}

static void test_lwm2m_PlainTextToFloat64(void)
{
    int i;

    for (i = 0 ; tests[i] != NULL ; i++)
    {
        double res;

        if (utils_plainTextToFloat64((unsigned char*)tests[i], strlen(tests[i]), &res) != 1)
            res = -1;

        CU_ASSERT_DOUBLE_EQUAL(res, tests_expected_float[i], 0.0001);
        //printf ("%i \"%s\" -> fail (%f)\n", i , tests[i], res );
    }
}

static void test_lwm2m_int64ToPlainText(void)
{
    unsigned int i;

    for (i = 0 ; i < sizeof(ints)/sizeof(int64_t); i++)
    {
        char * res = (char*)"";
        int len;

        len = utils_int64ToPlainText(ints[i], (uint8_t**)&res);

        CU_ASSERT_FATAL(len);
        CU_ASSERT_NSTRING_EQUAL(res, ints_expected[i],len);
        //printf ("%i \"%i\" -> fail (%s)\n", i , ints[i], res );

        if (len>0)
            lwm2m_free(res);
    }
}

static void test_lwm2m_float64ToPlainText(void)
{
    unsigned int i;

    for (i = 0 ; i < sizeof(floats)/sizeof(floats[0]); i++)
    {
        char * res;
        int len;

        len = utils_float64ToPlainText(floats[i], (uint8_t**)&res);

        CU_ASSERT_FATAL(len);
        CU_ASSERT_NSTRING_EQUAL(res, floats_expected[i],len);
        //printf ("%i \"%.16g\" -> fail (%s)\n", i , floats[i], res );

        if (len>0)
            lwm2m_free(res);
    }
}

static struct TestTable table[] = {
        { "test of utils_plainTextToInt64()", test_lwm2m_PlainTextToInt64 },
        { "test of utils_plainTextToFloat64()", test_lwm2m_PlainTextToFloat64 },
        { "test of utils_int64ToPlainText()", test_lwm2m_int64ToPlainText },
        { "test of utils_float64ToPlainText()", test_lwm2m_float64ToPlainText },
        { NULL, NULL },
};

CU_ErrorCode create_convert_numbers_suit()
{
   CU_pSuite pSuite = NULL;

   pSuite = CU_add_suite("Suite_ConvertNumbers", NULL, NULL);
   if (NULL == pSuite) {
      return CU_get_error();
   }

   return add_tests(pSuite, table);
}


