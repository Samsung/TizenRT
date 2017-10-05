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
/* hashkey.c -- definition of hash key type and helper functions
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include <protocols/libcoap/hashkey.h>

/* Caution: When changing this, update COAP_DEFAULT_WKC_HASHKEY
 * accordingly (see int coap_hash_path());
 */
void coap_hash_impl(const unsigned char *s, unsigned int len, coap_key_t h)
{
	size_t j;

	while (len--) {
		j = sizeof(coap_key_t) - 1;

		while (j) {
			h[j] = ((h[j] << 7) | (h[j - 1] >> 1)) + h[j];
			--j;
		}

		h[0] = (h[0] << 7) + h[0] + *s++;
	}
}
