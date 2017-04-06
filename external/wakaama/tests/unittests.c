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


#include <stdio.h>
#include <stdint.h>

#include "CUnit/Basic.h"

#include "tests.h"

// stub function
void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    return (void *)secObjInstID;
}

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    return;
}

CU_ErrorCode add_tests(CU_pSuite pSuite, struct TestTable* testTable)
{
    int index;
    for (index = 0; NULL != testTable && NULL != testTable[index].name; ++index) {
        if (NULL == CU_add_test(pSuite, testTable[index].name, testTable[index].function)) {
            fprintf(stderr, "Failed to add test %s\n", testTable[index].name);
            return CU_get_error();
         }
    }
    return CUE_SUCCESS;
}

int main()
{
   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   if (CUE_SUCCESS != create_tlv_suit()) {
       goto exit;
   }
   if (CUE_SUCCESS != create_uri_suit()) {
       goto exit;
   }
   if (CUE_SUCCESS != create_convert_numbers_suit()) {
       goto exit;
   }
   if (CUE_SUCCESS != create_tlv_json_suit()) {
       goto exit;
   }
   if (CUE_SUCCESS != create_block1_suit()) {
       goto exit;
   }

   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
exit:
   CU_cleanup_registry();
   return CU_get_error();
}
