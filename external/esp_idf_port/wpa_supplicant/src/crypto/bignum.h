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
 * Big number math
 * Copyright (c) 2006, Jouni Malinen <j@w1.fi>
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

#ifndef BIGNUM_H
#define BIGNUM_H

struct bignum;

struct bignum * bignum_init(void);
void bignum_deinit(struct bignum *n);
size_t bignum_get_unsigned_bin_len(struct bignum *n);
int bignum_get_unsigned_bin(const struct bignum *n, u8 *buf, size_t *len);
int bignum_set_unsigned_bin(struct bignum *n, const u8 *buf, size_t len);
int bignum_cmp(const struct bignum *a, const struct bignum *b);
int bignum_cmp_d(const struct bignum *a, unsigned long b);
int bignum_add(const struct bignum *a, const struct bignum *b,
	       struct bignum *c);
int bignum_sub(const struct bignum *a, const struct bignum *b,
	       struct bignum *c);
int bignum_mul(const struct bignum *a, const struct bignum *b,
	       struct bignum *c);
int bignum_mulmod(const struct bignum *a, const struct bignum *b,
		  const struct bignum *c, struct bignum *d);
int bignum_exptmod(const struct bignum *a, const struct bignum *b,
		   const struct bignum *c, struct bignum *d);

#endif /* BIGNUM_H */
