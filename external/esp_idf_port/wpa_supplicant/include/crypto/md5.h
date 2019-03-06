/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/*
 * MD5 hash implementation and interface functions
 * Copyright (c) 2003-2009, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef MD5_H
#define MD5_H

#define MD5_MAC_LEN 16

int hmac_md5_vector(const uint8_t *key, size_t key_len, size_t num_elem,
		    const uint8_t *addr[], const size_t *len, uint8_t *mac);
int hmac_md5(const uint8_t *key, size_t key_len, const uint8_t *data, size_t data_len,
	     uint8_t *mac);
#ifdef CONFIG_FIPS
int hmac_md5_vector_non_fips_allow(const uint8_t *key, size_t key_len,
				   size_t num_elem, const uint8_t *addr[],
				   const size_t *len, uint8_t *mac);
int hmac_md5_non_fips_allow(const uint8_t *key, size_t key_len, const uint8_t *data,
			    size_t data_len, uint8_t *mac);
#else /* CONFIG_FIPS */
#define hmac_md5_vector_non_fips_allow hmac_md5_vector
#define hmac_md5_non_fips_allow hmac_md5
#endif /* CONFIG_FIPS */

#endif /* MD5_H */
