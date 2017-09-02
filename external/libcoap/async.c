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
/* async.c -- state management for asynchronous messages
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

/**
 * @file async.c
 * @brief state management for asynchronous messages
 */

#ifndef WITHOUT_ASYNC

#include <protocols/libcoap/config.h>

#include <protocols/libcoap/utlist.h>

#include <protocols/libcoap/mem.h>
#include <protocols/libcoap/debug.h>
#include <protocols/libcoap/async.h>

coap_async_state_t *coap_register_async(coap_context_t *context, coap_address_t *peer, coap_pdu_t *request, unsigned char flags, void *data)
{
	coap_async_state_t *s;
	coap_tid_t id;

	coap_transport_t transport = COAP_UDP;

	unsigned char *token;
	unsigned int token_len = 0;

	switch(context->protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		transport = COAP_UDP;
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)request->transport_hdr)[0] >> 4);
		break;
	default:
		break;
	}

	coap_transaction_id2(peer, request, &id, context->protocol);
	LL_SEARCH_SCALAR(context->async_state, s, id, id);

	coap_get_token2(request->transport_hdr, transport, &token, &token_len);

	if (token_len > 8) {
		debug("coap_register_async : invalied length of token\n");
		return NULL;
	}

	if (s != NULL) {
		/* We must return NULL here as the caller must know that he is
		 * responsible for releasing @p data. */
		debug("coap_register_async : asynchronous state for transaction %d already registered\n", id);
		return NULL;
	}

	/* store information for handling the asynchronous task */
	s = (coap_async_state_t *)coap_malloc(sizeof(coap_async_state_t) + token_len);
	if (!s) {
		coap_log(LOG_CRIT, "coap_register_async : insufficient memory\n");
		return NULL;
	}

	memset(s, 0, sizeof(coap_async_state_t) + token_len);

	/* set COAP_ASYNC_CONFIRM according to request's type */
	s->flags = flags & ~COAP_ASYNC_CONFIRM;
	if (context->protocol == COAP_PROTO_UDP || context->protocol == COAP_PROTO_TCP) {
		if (request->transport_hdr->udp.type == COAP_MESSAGE_CON) {
			s->flags |= COAP_ASYNC_CONFIRM;
		}
	}

	s->appdata = data;

	memcpy(&s->peer, peer, sizeof(coap_address_t));

	if (token_len) {
		s->tokenlen = token_len;
		memcpy(s->token, token, token_len);
	}

	memcpy(&s->id, &id, sizeof(coap_tid_t));

	coap_touch_async(s);

	LL_PREPEND(context->async_state, s);

	return s;
}

coap_async_state_t *coap_find_async(coap_context_t *context, coap_tid_t id)
{
	coap_async_state_t *tmp;
	LL_SEARCH_SCALAR(context->async_state, tmp, id, id);
	return tmp;
}

int coap_remove_async(coap_context_t *context, coap_tid_t id, coap_async_state_t **s)
{
	coap_async_state_t *tmp = coap_find_async(context, id);

	if (tmp) {
		LL_DELETE(context->async_state, tmp);
	}

	*s = tmp;
	return tmp != NULL;
}

void coap_free_async(coap_async_state_t *s)
{
	if (s && (s->flags & COAP_ASYNC_RELEASE_DATA) != 0) {
		coap_free(s->appdata);
	}
	coap_free(s);
}

#else
void does_not_exist();			/* make some compilers happy */
#endif							/* WITHOUT_ASYNC */
