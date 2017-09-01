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
/* encode.c -- encoding and decoding of CoAP data types
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#ifndef NDEBUG
#include <stdio.h>
#endif

#include <protocols/libcoap/config.h>
#include <protocols/libcoap/encode.h>

/* Carsten suggested this when fls() is not available: */
int coap_fls(unsigned int i)
{
	int n;
	for (n = 0; i; n++) {
		i >>= 1;
	}
	return n;
}

unsigned int coap_decode_var_bytes(unsigned char *buf, unsigned int len)
{
	unsigned int i, n = 0;
	for (i = 0; i < len; ++i) {
		n = (n << 8) + buf[i];
	}

	return n;
}

unsigned int coap_encode_var_bytes(unsigned char *buf, unsigned int val)
{
	unsigned int n, i;

	for (n = 0, i = val; i && n < sizeof(val); ++n) {
		i >>= 8;
	}

	i = n;
	while (i--) {
		buf[i] = val & 0xff;
		val >>= 8;
	}

	return n;
}

#ifdef WITH_MBEDTLS
#if !defined (MBEDTLS_PSK_MAX_LEN)
#define MBEDTLS_PSK_MAX_LEN    16 /* 128-bits */
#endif

#define HEX2NUM(c)	\
do {	\
	if (c >= '0' && c <= '9')	\
		c -= '0';	\
	else if (c >= 'a' && c <= 'f')	\
		c -= 'a' - 10;	\
	else if (c >= 'A' && c <= 'F')	\
		c -= 'A' - 10;	\
	else	\
		return(-1);	\
} while (0)

int coap_unhexify(unsigned char *output, const char *input, size_t *olen)
{
	unsigned char c;
	size_t j;

	*olen = strlen(input);
	if (*olen % 2 != 0 || *olen / 2 > MBEDTLS_PSK_MAX_LEN) {
		return (-1);
	}
	*olen /= 2;

	for (j = 0; j < *olen * 2; j += 2) {
		c = input[j];
		HEX2NUM(c);
		output[j / 2] = c << 4;

		c = input[j + 1];
		HEX2NUM(c);
		output[j / 2] |= c;
	}

	return 0;
}
#endif /* WITH_MBEDTLS */
