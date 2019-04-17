/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __SECURITY_HAL_TEST_H__
#define __SECURITY_HAL_TEST_H__

#include <pthread.h>

/*
 * Definitions
 */
#define HAL_TEST_STACK_SIZE 16384

#define HAL_TEST_TRIAL 10
#define HAL_TEST_LIMIT 1000000
#define HAL_TEST_MEM_SIZE 4096

#define HAL_TEST_KEY_LEN 32
#define HAL_TEST_KEY_SLOT 1
#define HAL_TEST_AES_DATA_LEN 16
#define HAL_TEST_RSA_DATA_LEN 128

#define HAL_TEST_MAX_SLOT_INDEX 8
#define HAL_TEST_MAX_DATA 64

/*  Configuration */
#define HAL_AUTH_TEST_TRIAL HAL_TEST_TRIAL
#define HAL_AUTH_TEST_LIMIT_TIME HAL_TEST_LIMIT
#define HAL_AUTH_TEST_MEM_SIZE HAL_TEST_MEM_SIZE

#define HAL_CRYPTO_TEST_TRIAL HAL_TEST_TRIAL
#define HAL_CRYPTO_TEST_LIMIT_TIME HAL_TEST_LIMIT
#define HAL_CRYPTO_TEST_MEM_SIZE HAL_TEST_MEM_SIZE

#define HAL_KEYMGR_TEST_TRIAL HAL_TEST_TRIAL
#define HAL_KEYMGR_TEST_LIMIT_TIME HAL_TEST_LIMIT
#define HAL_KEYMGR_TEST_MEM_SIZE HAL_TEST_MEM_SIZE

#define HAL_SS_TEST_TRIAL HAL_TEST_TRIAL
#define HAL_SS_TEST_LIMIT_TIME HAL_TEST_LIMIT
#define HAL_SS_TEST_MEM_SIZE HAL_TEST_MEM_SIZE


/*
 * Functions
 */

pthread_addr_t hal_auth_test(void);
pthread_addr_t hal_keymgr_test(void);
pthread_addr_t hal_ss_test(void);
pthread_addr_t hal_crypto_test(void);

#endif /* __SECURITY_HAL_TEST_H__ */
