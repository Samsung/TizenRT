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
/* str.h -- strings to be used in the CoAP library
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#ifndef _COAP_STR_H_
#define _COAP_STR_H_

#include <string.h>

typedef struct {
	size_t length;				/* length of string */
	unsigned char *s;			/* string data */
} str;

#define COAP_SET_STR(st,l,v) { (st)->length = (l), (st)->s = (v); }

/**
 * Returns a new string object with at least size bytes storage
 * allocated.  The string must be released using coap_delete_string();
 */
str *coap_new_string(size_t size);

/** Deletes the given string and releases any memory allocated. */
void coap_delete_string(str *);

#endif							/* _COAP_STR_H_ */
