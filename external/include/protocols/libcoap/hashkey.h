/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/* hashkey.h -- definition of hash key type and helper functions
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

/**
 * @file protocols/libcoap/hashkey.h
 * @brief definition of hash key type and helper functions
 */

#ifndef _COAP_HASHKEY_H_
#define _COAP_HASHKEY_H_

#include <protocols/libcoap/str.h>

typedef unsigned char coap_key_t[4];

#ifndef coap_hash
/**
 * Calculates a fast hash over the given string @p s of length @p len
 * and stores the result into @p h. Depending on the exact
 * implementation, this function cannot be used as one-way function to
 * check message integrity or simlar.
 *
 * @param s   The string used for hash calculation.
 * @param len The length of @p s.
 * @param h   The result buffer to store the calculated hash key.
 */
void coap_hash_impl(const unsigned char *s, unsigned int len, coap_key_t h);

#define coap_hash(String,Length,Result) \
  coap_hash_impl((String),(Length),(Result))

/* This is used to control the pre-set hash-keys for resources. */
#define __COAP_DEFAULT_HASH
#else
#undef __COAP_DEFAULT_HASH
#endif							/* coap_hash */

/**
 * Calls coap_hash() with given @c str object as parameter.
 *
 * @param Str Must contain a pointer to a coap string object.
 * @param H   A coap_key_t object to store the result.
 *
 * @hideinitializer
 */
#define coap_str_hash(Str,H) {			\
    assert(Str);				\
    memset((H), 0, sizeof(coap_key_t));		\
    coap_hash((H), (Str)->s, (Str)->length);	\
  }

#endif							/* _COAP_HASHKEY_H_ */
