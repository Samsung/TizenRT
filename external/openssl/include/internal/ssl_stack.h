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
#ifndef _SSL_STACK_H_
#define _SSL_STACK_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "ssl_types.h"

#define STACK_OF(type)  struct stack_st_##type

#define SKM_DEFINE_STACK_OF(t1, t2, t3) \
    STACK_OF(t1); \
    static ossl_inline STACK_OF(t1) *sk_##t1##_new_null(void) \
    { \
        return (STACK_OF(t1) *)OPENSSL_sk_new_null(); \
    } \

#define DEFINE_STACK_OF(t) SKM_DEFINE_STACK_OF(t, t, t)

/**
 * @brief create a openssl stack object
 *
 * @param c - stack function
 *
 * @return openssl stack object point
 */
OPENSSL_STACK* OPENSSL_sk_new(OPENSSL_sk_compfunc c);

/**
 * @brief create a NULL function openssl stack object
 *
 * @param none
 *
 * @return openssl stack object point
 */
OPENSSL_STACK *OPENSSL_sk_new_null(void);

/**
 * @brief free openssl stack object
 *
 * @param openssl stack object point
 *
 * @return none
 */
void OPENSSL_sk_free(OPENSSL_STACK *stack);

#ifdef __cplusplus
}
#endif

#endif
