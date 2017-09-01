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
/* str.c -- strings to be used in the CoAP library
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include <protocols/libcoap/config.h>

#include <stdio.h>

#include <protocols/libcoap/debug.h>
#include <protocols/libcoap/mem.h>
#include <protocols/libcoap/str.h>

str *coap_new_string(size_t size)
{
	str *s = coap_malloc(sizeof(str) + size + 1);
	if (!s) {
#ifndef NDEBUG
		coap_log(LOG_CRIT, "coap_new_string: malloc\n");
#endif
		return NULL;
	}

	memset(s, 0, sizeof(str));
	s->s = ((unsigned char *)s) + sizeof(str);
	return s;
}

void coap_delete_string(str *s)
{
	coap_free(s);
}
