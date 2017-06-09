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
/****************************************************************************
 * Copyright (c) 2010--2011, Olaf Bergmann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  o Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  o Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 ****************************************************************************/
/* coap_list.c -- CoAP list structures
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include "config.h"

#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "mem.h"
#include "coap_list.h"

int coap_insert(coap_list_t **queue, coap_list_t *node, int (*order)(void *, void *node))
{
	coap_list_t *p, *q;
	if (!queue || !node) {
		return 0;
	}

	/* set queue head if empty */
	if (!*queue) {
		*queue = node;
		return 1;
	}

	/* replace queue head if new node has to be added before the existing queue head */
	q = *queue;
	if (order(node->data, q->data) < 0) {
		node->next = q;
		*queue = node;
		return 1;
	}

	/* search for right place to insert */
	do {
		p = q;
		q = q->next;
	} while (q && order(node->data, q->data) >= 0);

	/* insert new item */
	node->next = q;
	p->next = node;
	return 1;
}

int coap_delete(coap_list_t *node)
{
	if (!node) {
		return 0;
	}

	if (node->delete_func) {
		node->delete_func(node->data);
	}
	coap_free(node->data);
	coap_free(node);

	return 1;
}

void coap_delete_list(coap_list_t *queue)
{
	if (!queue) {
		return;
	}

	coap_delete_list(queue->next);
	coap_delete(queue);
}

coap_list_t *coap_new_listnode(void *data, void (*delete_func)(void *))
{
	coap_list_t *node = coap_malloc(sizeof(coap_list_t));
	if (!node) {
#ifndef NDEBUG
		coap_log(LOG_CRIT, "coap_new_listnode: malloc\n");
#endif
		return NULL;
	}

	memset(node, 0, sizeof(coap_list_t));
	node->data = data;
	node->delete_func = delete_func;
	return node;
}
