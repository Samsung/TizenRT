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
/* pdu.c -- CoAP message structure
 *
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include <protocols/libcoap/config.h>

#if defined(HAVE_ASSERT_H) && !defined(assert)
#include <assert.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#include <protocols/libcoap/debug.h>
#include <protocols/libcoap/pdu.h>
#include <protocols/libcoap/option.h>
#include <protocols/libcoap/encode.h>

#ifdef WITH_CONTIKI
#include "memb.h"

#ifndef WITH_TCP
typedef unsigned char _pdu[sizeof(coap_pdu_t) + COAP_MAX_PDU_SIZE];

MEMB(pdu_storage, _pdu, COAP_PDU_MAXCNT);
#endif

void coap_pdu_resources_init()
{
	memb_init(&pdu_storage);
}
#else							/* WITH_CONTIKI */
#include <protocols/libcoap/mem.h>
#endif							/* WITH_CONTIKI */

void coap_pdu_clear(coap_pdu_t *pdu, size_t size)
{
	coap_pdu_clear2(pdu, size, COAP_UDP, 0);
}

void coap_pdu_clear2(coap_pdu_t *pdu, size_t size, coap_transport_t transport, unsigned int length)
{
	assert(pdu);

	memset(pdu, 0, sizeof(coap_pdu_t) + size);
	pdu->max_size = size;
	pdu->hdr = (coap_hdr_t *)((unsigned char *)pdu + sizeof(coap_pdu_t));

	if (COAP_UDP == transport) {
		pdu->transport_hdr->udp.version = COAP_DEFAULT_VERSION;
		/* data is NULL unless explicitly set by coap_add_data() */
		pdu->length = sizeof(pdu->transport_hdr->udp);
	}
#ifdef WITH_TCP
	else {
		/* data is NULL unless explicitly set by coap_add_data() */
		pdu->length = length;
	}
#else
	(void)length;
#endif
}

#ifdef WITH_LWIP
coap_pdu_t *coap_pdu_from_pbuf(struct pbuf *pbuf)
{
	LWIP_ASSERT("Can only deal with contiguous PBUFs", pbuf->tot_len == pbuf->len);
	LWIP_ASSERT("coap_read needs to receive an exclusive copy of the incoming pbuf", pbuf->ref == 1);

	void *data = pbuf->payload;
	coap_pdu_t *result;

	u8_t header_error = pbuf_header(pbuf, sizeof(coap_pdu_t));
	LWIP_ASSERT("CoAP PDU header does not fit in existing header space", header_error == 0);

	result = (coap_pdu_t *) pbuf->payload;

	memset(result, 0, sizeof(coap_pdu_t));

	result->max_size = pbuf->tot_len - sizeof(coap_pdu_t);
	result->length = pbuf->tot_len - sizeof(coap_pdu_t);
	result->hdr = data;
	result->pbuf = pbuf;

	return result;
}
#endif

coap_pdu_t *coap_pdu_init(unsigned char type, unsigned char code, unsigned short id, size_t size)
{
	return coap_pdu_init2(type, code, id, size, COAP_UDP);
}

coap_pdu_t *coap_pdu_init2(unsigned char type, unsigned char code, unsigned short id, size_t size, coap_transport_t transport)
{
	coap_pdu_t *pdu;
#ifdef WITH_LWIP
	struct pbuf *p;
#endif

	unsigned int length = 0;

	switch (transport) {
	case COAP_UDP:
		length = sizeof(pdu->transport_hdr->udp);
		break;
#ifdef WITH_TCP
	case COAP_TCP:
		length = COAP_TCP_HEADER_NO_FIELD;
		break;
	case COAP_TCP_8BIT:
		length = COAP_TCP_HEADER_8_BIT;
		break;
	case COAP_TCP_16BIT:
		length = COAP_TCP_HEADER_16_BIT;
		break;
	case COAP_TCP_32BIT:
		length = COAP_TCP_HEADER_32_BIT;
		break;
#endif
	default:
		debug("it has wrong type\n");
	}

	debug("coap_pdu_init2 : transport %d, length %d, size %d\n", transport, length, size);

#ifndef WITH_TCP
	assert(size <= COAP_MAX_PDU_SIZE);
	/* Size must be large enough to fit the header. */
	if (size < length || size > COAP_MAX_PDU_SIZE) {
		return NULL;
	}
#endif

	/* size must be large enough for hdr */
#ifdef WITH_POSIX
	pdu = (coap_pdu_t *) coap_malloc(sizeof(coap_pdu_t) + size);
#endif
#ifdef WITH_CONTIKI
	pdu = (coap_pdu_t *) memb_alloc(&pdu_storage);
#endif
#ifdef WITH_LWIP
	p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
	if (p != NULL) {
		u8_t header_error = pbuf_header(p, sizeof(coap_pdu_t));
		/* we could catch that case and allocate larger memory in advance, but then
		 * again, we'd run into greater trouble with incoming packages anyway */
		LWIP_ASSERT("CoAP PDU header does not fit in transport header", header_error == 0);
		pdu = p->payload;
	} else {
		pdu = NULL;
	}
#endif
	if (pdu) {
		coap_pdu_clear2(pdu, size, transport, length);

		switch (transport) {
		case COAP_UDP:
			pdu->transport_hdr->udp.id = id;
			pdu->transport_hdr->udp.type = type;
			pdu->transport_hdr->udp.code = code;
			break;
#ifdef WITH_TCP
		case COAP_TCP:
			pdu->transport_hdr->tcp.header_data[0] = 0;
			pdu->transport_hdr->tcp.header_data[1] = code;
			break;
		case COAP_TCP_8BIT:
			pdu->transport_hdr->tcp_8bit.header_data[0] = COAP_TCP_LENGTH_FIELD_NUM_8_BIT << 4;
			pdu->transport_hdr->tcp_8bit.header_data[2] = code;
			break;
		case COAP_TCP_16BIT:
			pdu->transport_hdr->tcp_16bit.header_data[0] = COAP_TCP_LENGTH_FIELD_NUM_16_BIT << 4;
			pdu->transport_hdr->tcp_16bit.header_data[3] = code;
			break;
		case COAP_TCP_32BIT:
			pdu->transport_hdr->tcp_32bit.header_data[0] = COAP_TCP_LENGTH_FIELD_NUM_32_BIT << 4;
			pdu->transport_hdr->tcp_32bit.header_data[5] = code;
			break;
#endif
		default:
			debug("it has wrong type\n");
		}

#ifdef WITH_LWIP
		pdu->pbuf = p;
#endif
	}
	return pdu;
}

coap_pdu_t *coap_new_pdu()
{
	coap_pdu_t *pdu;

#ifndef WITH_CONTIKI
	pdu = coap_pdu_init(0, 0, ntohs(COAP_INVALID_TID), COAP_MAX_PDU_SIZE);
#else /* WITH_CONTIKI */
	pdu = coap_pdu_init(0, 0, uip_ntohs(COAP_INVALID_TID), COAP_MAX_PDU_SIZE);
#endif /* WITH_CONTIKI */

#ifndef NDEBUG
	if (!pdu)
		coap_log(LOG_CRIT, "coap_new_pdu: cannot allocate memory for new PDU\n");
#endif
	return pdu;
}

coap_pdu_t *coap_new_pdu2(coap_transport_t transport, unsigned int size)
{
#ifndef WITH_TCP
	(void)size;
#endif
	coap_pdu_t *pdu;

#ifndef WITH_CONTIKI
	pdu = coap_pdu_init2(0, 0, ntohs(COAP_INVALID_TID),
#ifndef WITH_TCP
						 COAP_MAX_PDU_SIZE,
#else
						 size,
#endif
						 transport);
#else							/* WITH_CONTIKI */
	pdu = coap_pdu_init2(0, 0, uip_ntohs(COAP_INVALID_TID),
#ifndef WITH_TCP
						 COAP_MAX_PDU_SIZE,
#else
						 size,
#endif
						 transport);
#endif							/* WITH_CONTIKI */

#ifndef NDEBUG
	if (!pdu) {
		coap_log(LOG_CRIT, "coap_new_pdu: cannot allocate memory for new PDU\n");
	}
#endif
	return pdu;
}

void coap_delete_pdu(coap_pdu_t *pdu)
{
#ifdef WITH_POSIX
	coap_free(pdu);
#endif
#ifdef WITH_LWIP
	if (pdu != NULL) {		/* accepting double free as the other implementation accept that too */
		pbuf_free(pdu->pbuf);
	}
#endif
#ifdef WITH_CONTIKI
	memb_free(&pdu_storage, pdu);
#endif
}

#ifdef WITH_TCP
size_t coap_get_total_message_length(const unsigned char *data, size_t size)
{
	if (!data || !size) {
		debug("received data length is null\n");
		return 0;
	}

	coap_transport_t transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)data)[0] >> 4);
	size_t optPaylaodLen = coap_get_length_from_header((unsigned char *)data,
						   transport);
	size_t headerLen = coap_get_tcp_header_length((unsigned char *)data);

	return headerLen + optPaylaodLen;
}

coap_transport_t coap_get_tcp_header_type_from_size(unsigned int size)
{
	if (size < COAP_TCP_LENGTH_FIELD_8_BIT) {
		return COAP_TCP;
	} else if (size < COAP_TCP_LENGTH_FIELD_16_BIT) {
		return COAP_TCP_8BIT;
	} else if (size < COAP_TCP_LENGTH_FIELD_32_BIT) {
		return COAP_TCP_16BIT;
	} else {
		return COAP_TCP_32BIT;
	}
}

coap_transport_t coap_get_tcp_header_type_from_initbyte(unsigned int length)
{
	coap_transport_t type;
	switch (length) {
	case COAP_TCP_LENGTH_FIELD_NUM_8_BIT:
		type = COAP_TCP_8BIT;
		break;
	case COAP_TCP_LENGTH_FIELD_NUM_16_BIT:
		type = COAP_TCP_16BIT;
		break;
	case COAP_TCP_LENGTH_FIELD_NUM_32_BIT:
		type = COAP_TCP_32BIT;
		break;
	default:
		type = COAP_TCP;
	}
	return type;
}

void coap_add_length(const coap_pdu_t *pdu, coap_transport_t transport, unsigned int length)
{
	assert(pdu);

	switch (transport) {
	case COAP_TCP:
		pdu->transport_hdr->tcp.header_data[0] = length << 4;
		break;
	case COAP_TCP_8BIT:
		if (length > COAP_TCP_LENGTH_FIELD_8_BIT) {
			pdu->transport_hdr->tcp_8bit.header_data[1] = length - COAP_TCP_LENGTH_FIELD_8_BIT;
		}
		break;
	case COAP_TCP_16BIT:
		if (length > COAP_TCP_LENGTH_FIELD_16_BIT) {
			unsigned int total_length = length - COAP_TCP_LENGTH_FIELD_16_BIT;
			pdu->transport_hdr->tcp_16bit.header_data[1] = (total_length >> 8) & 0x0000ff;
			pdu->transport_hdr->tcp_16bit.header_data[2] = total_length & 0x000000ff;
		}
		break;
	case COAP_TCP_32BIT:
		if (length > COAP_TCP_LENGTH_FIELD_32_BIT) {
			unsigned int total_length = length - COAP_TCP_LENGTH_FIELD_32_BIT;
			pdu->transport_hdr->tcp_32bit.header_data[1] = total_length >> 24;
			pdu->transport_hdr->tcp_32bit.header_data[2] = (total_length >> 16) & 0x00ff;
			pdu->transport_hdr->tcp_32bit.header_data[3] = (total_length >> 8) & 0x0000ff;
			pdu->transport_hdr->tcp_32bit.header_data[4] = total_length & 0x000000ff;
		}
		break;
	default:
		debug("it has wrong type\n");
	}
}

unsigned int coap_get_length_from_header(const unsigned char *header, coap_transport_t transport)
{
	assert(header);

	unsigned int length = 0;
	unsigned int length_field_data = 0;
	switch (transport) {
	case COAP_TCP:
		length = header[0] >> 4;
		break;
	case COAP_TCP_8BIT:
		length = header[1] + COAP_TCP_LENGTH_FIELD_8_BIT;
		break;
	case COAP_TCP_16BIT:
		length_field_data = (header[1] << 8 | header[2]);
		length = length_field_data + COAP_TCP_LENGTH_FIELD_16_BIT;
		break;
	case COAP_TCP_32BIT:
		length_field_data = header[1] << 24 | header[2] << 16 | header[3] << 8 | header[4];
		length = length_field_data + COAP_TCP_LENGTH_FIELD_32_BIT;
		break;
	default:
		debug("it has wrong type\n");
	}

	return length;
}

unsigned int coap_get_length(const coap_pdu_t *pdu, coap_transport_t transport)
{
	assert(pdu);

	unsigned int length = 0;
	unsigned int length_field_data = 0;
	switch (transport) {
	case COAP_TCP:
		length = pdu->transport_hdr->tcp.header_data[0] >> 4;
		break;
	case COAP_TCP_8BIT:
		length = pdu->transport_hdr->tcp_8bit.header_data[1] + COAP_TCP_LENGTH_FIELD_8_BIT;
		break;
	case COAP_TCP_16BIT:
		length_field_data = pdu->transport_hdr->tcp_16bit.header_data[1] << 8 | pdu->transport_hdr->tcp_16bit.header_data[2];
		length = length_field_data + COAP_TCP_LENGTH_FIELD_16_BIT;
		break;
	case COAP_TCP_32BIT:
		length_field_data = pdu->transport_hdr->tcp_32bit.header_data[1] << 24 | pdu->transport_hdr->tcp_32bit.header_data[2] << 16 | pdu->transport_hdr->tcp_32bit.header_data[3] << 8 | pdu->transport_hdr->tcp_32bit.header_data[4];
		length = length_field_data + COAP_TCP_LENGTH_FIELD_32_BIT;
		break;
	default:
		debug("it has wrong type\n");
	}

	return length;
}

unsigned int coap_get_tcp_header_length(unsigned char *data)
{
	assert(data);

	unsigned int tokenLength = data[0] & 0x0f;
	coap_transport_t transport = coap_get_tcp_header_type_from_initbyte(data[0] >> 4);
	unsigned int length = 0;

	length = coap_get_tcp_header_length_for_transport(transport) + tokenLength;
	return length;
}

unsigned int coap_get_tcp_header_length_for_transport(coap_transport_t transport)
{
	unsigned int length = 0;
	switch (transport) {
	case COAP_TCP:
		length = COAP_TCP_HEADER_NO_FIELD;
		break;
	case COAP_TCP_8BIT:		/* len(4bit) + TKL(4bit) + Len+bytes(1byte) + Code(1byte) */
		length = COAP_TCP_HEADER_8_BIT;
		break;
	case COAP_TCP_16BIT:		/* len(4bit) + TKL(4bit) + Len+bytes(2byte) + Code(1byte) */
		length = COAP_TCP_HEADER_16_BIT;
		break;
	case COAP_TCP_32BIT:		/* len(4bit) + TKL(4bit) + Len+bytes(4byte) + Code(1byte) */
		length = COAP_TCP_HEADER_32_BIT;
		break;
	default:
		debug("it has wrong type\n");
	}

	return length;
}

size_t coap_get_opt_header_length(unsigned short key, size_t length)
{
	size_t headerLength = 0;

	unsigned short optDeltaLength = 0;
	if (COAP_OPTION_FIELD_8_BIT >= key) {
		optDeltaLength = 0;
	} else if (COAP_OPTION_FIELD_8_BIT < key && COAP_OPTION_FIELD_16_BIT >= key) {
		optDeltaLength = 1;
	} else {
		optDeltaLength = 2;
	}

	size_t optLength = 0;
	if (COAP_OPTION_FIELD_8_BIT >= length) {
		optLength = 0;
	} else if (COAP_OPTION_FIELD_8_BIT < length && COAP_OPTION_FIELD_16_BIT >= length) {
		optLength = 1;
	} else if (COAP_OPTION_FIELD_16_BIT < length && COAP_OPTION_FIELD_32_BIT >= length) {
		optLength = 2;
	} else {
		printf("Error : Reserved for the Payload marker for length");
		return 0;
	}

	headerLength = length + optDeltaLength + optLength + 1;

	return headerLength;
}

#endif

void coap_add_code(const coap_pdu_t *pdu, coap_transport_t transport, unsigned int code)
{
	assert(pdu);

	switch (transport) {
	case COAP_UDP:
		pdu->transport_hdr->udp.code = COAP_RESPONSE_CODE(code);
		break;
#ifdef WITH_TCP
	case COAP_TCP:
		pdu->transport_hdr->tcp.header_data[1] = COAP_RESPONSE_CODE(code);
		break;
	case COAP_TCP_8BIT:
		pdu->transport_hdr->tcp_8bit.header_data[2] = COAP_RESPONSE_CODE(code);
		break;
	case COAP_TCP_16BIT:
		pdu->transport_hdr->tcp_16bit.header_data[3] = COAP_RESPONSE_CODE(code);
		break;
	case COAP_TCP_32BIT:
		pdu->transport_hdr->tcp_32bit.header_data[5] = COAP_RESPONSE_CODE(code);
		break;
#endif
	default:
		debug("it has wrong type\n");
	}
}

unsigned int coap_get_code(const coap_pdu_t *pdu, coap_transport_t transport)
{
	assert(pdu);

	unsigned int code = 0;
	switch (transport) {
	case COAP_UDP:
		code = pdu->transport_hdr->udp.code;
		break;
#ifdef WITH_TCP
	case COAP_TCP:
		code = pdu->transport_hdr->tcp.header_data[1];
		break;
	case COAP_TCP_8BIT:
		code = pdu->transport_hdr->tcp_8bit.header_data[2];
		break;
	case COAP_TCP_16BIT:
		code = pdu->transport_hdr->tcp_16bit.header_data[3];
		break;
	case COAP_TCP_32BIT:
		code = pdu->transport_hdr->tcp_32bit.header_data[5];
		break;
#endif
	default:
		debug("it has wrong type\n");
	}
	return code;
}

int coap_add_token(coap_pdu_t *pdu, size_t len, const unsigned char *data)
{
	return coap_add_token2(pdu, len, data, COAP_UDP);
}

int coap_add_token2(coap_pdu_t *pdu, size_t len, const unsigned char *data, coap_transport_t transport)
{
	const size_t HEADERLENGTH = len + 4;
	/* must allow for pdu == NULL as callers may rely on this */
	if (!pdu || len > 8 || pdu->max_size < HEADERLENGTH) {
		return 0;
	}

	unsigned char *token = NULL;
	switch (transport) {
	case COAP_UDP:
		pdu->transport_hdr->udp.token_length = len;
		token = pdu->transport_hdr->udp.token;
		pdu->length = HEADERLENGTH;
		break;
#ifdef WITH_TCP
	case COAP_TCP:
		pdu->transport_hdr->tcp.header_data[0] = pdu->transport_hdr->tcp.header_data[0] | len;
		token = pdu->transport_hdr->tcp.token;
		pdu->length = len + COAP_TCP_HEADER_NO_FIELD;
		break;
	case COAP_TCP_8BIT:
		pdu->transport_hdr->tcp_8bit.header_data[0] = pdu->transport_hdr->tcp_8bit.header_data[0] | len;
		token = pdu->transport_hdr->tcp_8bit.token;
		pdu->length = len + COAP_TCP_HEADER_8_BIT;
		break;
	case COAP_TCP_16BIT:
		pdu->transport_hdr->tcp_16bit.header_data[0] = pdu->transport_hdr->tcp_16bit.header_data[0] | len;
		token = pdu->transport_hdr->tcp_16bit.token;
		pdu->length = len + COAP_TCP_HEADER_16_BIT;
		break;
	case COAP_TCP_32BIT:
		pdu->transport_hdr->tcp_32bit.header_data[0] = pdu->transport_hdr->tcp_32bit.header_data[0] | len;
		token = pdu->transport_hdr->tcp_32bit.token;
		pdu->length = len + COAP_TCP_HEADER_32_BIT;
		break;
#endif
	default:
		debug("it has wrong type\n");
	}

	if (len) {
		memcpy(token, data, len);
	}

	pdu->max_delta = 0;
	pdu->data = NULL;

	return 1;
}

void coap_get_token(const coap_hdr_t *pdu_hdr, unsigned char **token, unsigned int *token_length)
{
	coap_get_token2((const coap_hdr_transport_t *)pdu_hdr, COAP_UDP, token, token_length);
}

void coap_get_token2(const coap_hdr_transport_t *pdu_hdr, coap_transport_t transport, unsigned char **token, unsigned int *token_length)
{
	assert(pdu_hdr);
	assert(token);
	assert(token_length);

	switch (transport) {
	case COAP_UDP:
		*token_length = pdu_hdr->udp.token_length;
		*token = (unsigned char *)pdu_hdr->udp.token;
		break;
#ifdef WITH_TCP
	case COAP_TCP:
		*token_length = (pdu_hdr->tcp.header_data[0]) & 0x0f;
		*token = (unsigned char *)pdu_hdr->tcp.token;
		break;
	case COAP_TCP_8BIT:
		*token_length = (pdu_hdr->tcp_8bit.header_data[0]) & 0x0f;
		*token = (unsigned char *)pdu_hdr->tcp_8bit.token;
		break;
	case COAP_TCP_16BIT:
		*token_length = (pdu_hdr->tcp_16bit.header_data[0]) & 0x0f;
		*token = (unsigned char *)pdu_hdr->tcp_16bit.token;
		break;
	case COAP_TCP_32BIT:
		*token_length = (pdu_hdr->tcp_32bit.header_data[0]) & 0x0f;
		*token = (unsigned char *)pdu_hdr->tcp_32bit.token;
		break;
#endif
	default:
		debug("it has wrong type\n");
	}
}

size_t coap_add_option(coap_pdu_t *pdu, unsigned short type, unsigned int len, const unsigned char *data)
{
	return coap_add_option2(pdu, type, len, data, COAP_UDP);
}

/** @FIXME de-duplicate code with coap_add_option_later */
size_t coap_add_option2(coap_pdu_t *pdu, unsigned short type, unsigned int len, const unsigned char *data, coap_transport_t transport)
{
	size_t optsize;
	coap_opt_t *opt;

	assert(pdu);
	pdu->data = NULL;

	if (type < pdu->max_delta) {
		warn("coap_add_option2: options are not in correct order\n");
		return 0;
	}

	switch (transport) {
	case COAP_UDP:
		opt = (unsigned char *)&(pdu->transport_hdr->udp) + pdu->length;
		break;
#ifdef WITH_TCP
	case COAP_TCP:
		opt = (unsigned char *)&(pdu->transport_hdr->tcp) + pdu->length;
		break;
	case COAP_TCP_8BIT:
		opt = (unsigned char *)&(pdu->transport_hdr->tcp_8bit) + pdu->length;
		break;
	case COAP_TCP_16BIT:
		opt = (unsigned char *)&(pdu->transport_hdr->tcp_16bit) + pdu->length;
		break;
	case COAP_TCP_32BIT:
		opt = (unsigned char *)&(pdu->transport_hdr->tcp_32bit) + pdu->length;
		break;
#endif
	default:
		opt = (unsigned char *)&(pdu->transport_hdr->udp) + pdu->length;
		break;
	}

	/* encode option and check length */
	optsize = coap_opt_encode(opt, pdu->max_size - pdu->length, type - pdu->max_delta, data, len);

	if (!optsize) {
		warn("coap_add_option2: cannot add option\n");
		/* error */
		return 0;
	} else {
		pdu->max_delta = type;
		pdu->length += optsize;
	}

	return optsize;
}

/** @FIXME de-duplicate code with coap_add_option2 */
unsigned char *coap_add_option_later(coap_pdu_t *pdu, unsigned short type, unsigned int len)
{
	size_t optsize;
	coap_opt_t *opt;

	assert(pdu);
	pdu->data = NULL;

	if (type < pdu->max_delta) {
		warn("coap_add_option: options are not in correct order\n");
		return NULL;
	}

	opt = (unsigned char *)pdu->hdr + pdu->length;

	/* encode option and check length */
	optsize = coap_opt_encode(opt, pdu->max_size - pdu->length, type - pdu->max_delta, NULL, len);

	if (!optsize) {
		warn("coap_add_option: cannot add option\n");
		/* error */
		return NULL;
	} else {
		pdu->max_delta = type;
		pdu->length += optsize;
	}

	return ((unsigned char *)opt) + optsize - len;
}

int coap_add_data(coap_pdu_t *pdu, unsigned int len, const unsigned char *data)
{
	assert(pdu);
	assert(pdu->data == NULL);

	if (len == 0) {
		return 1;
	}

	if (pdu->length + len + 1 > pdu->max_size) {
		warn("coap_add_data: cannot add: data too large for PDU\n");
		assert(pdu->data == NULL);
		return 0;
	}

	pdu->data = (unsigned char *)pdu->transport_hdr + pdu->length;
	*pdu->data = COAP_PAYLOAD_START;
	pdu->data++;

	memcpy(pdu->data, data, len);
	pdu->length += len + 1;
	return 1;
}

int coap_get_data(coap_pdu_t *pdu, size_t *len, unsigned char **data)
{
	assert(pdu);
	assert(len);
	assert(data);

	if (pdu->data) {
		*len = (unsigned char *)pdu->transport_hdr + pdu->length - pdu->data;
		*data = pdu->data;
	} else {					/* no data, clear everything */
		*len = 0;
		*data = NULL;
	}

	return *data != NULL;
}

#ifndef SHORT_ERROR_RESPONSE
typedef struct {
	unsigned char code;
	char *phrase;
} error_desc_t;

/* if you change anything here, make sure, that the longest string does not
 * exceed COAP_ERROR_PHRASE_LENGTH. */
error_desc_t coap_error[] = {
	{COAP_RESPONSE_CODE(65), "2.01 Created"},
	{COAP_RESPONSE_CODE(66), "2.02 Deleted"},
	{COAP_RESPONSE_CODE(67), "2.03 Valid"},
	{COAP_RESPONSE_CODE(68), "2.04 Changed"},
	{COAP_RESPONSE_CODE(69), "2.05 Content"},
	{COAP_RESPONSE_CODE(400), "Bad Request"},
	{COAP_RESPONSE_CODE(401), "Unauthorized"},
	{COAP_RESPONSE_CODE(402), "Bad Option"},
	{COAP_RESPONSE_CODE(403), "Forbidden"},
	{COAP_RESPONSE_CODE(404), "Not Found"},
	{COAP_RESPONSE_CODE(405), "Method Not Allowed"},
	{COAP_RESPONSE_CODE(408), "Request Entity Incomplete"},
	{COAP_RESPONSE_CODE(413), "Request Entity Too Large"},
	{COAP_RESPONSE_CODE(415), "Unsupported Media Type"},
	{COAP_RESPONSE_CODE(500), "Internal Server Error"},
	{COAP_RESPONSE_CODE(501), "Not Implemented"},
	{COAP_RESPONSE_CODE(502), "Bad Gateway"},
	{COAP_RESPONSE_CODE(503), "Service Unavailable"},
	{COAP_RESPONSE_CODE(504), "Gateway Timeout"},
	{COAP_RESPONSE_CODE(505), "Proxying Not Supported"},
	{0, NULL}					/* end marker */
};

char *coap_response_phrase(unsigned char code)
{
	int i;
	for (i = 0; coap_error[i].code; ++i) {
		if (coap_error[i].code == code) {
			return coap_error[i].phrase;
		}
	}
	return NULL;
}
#endif

/**
 * Advances *optp to next option if still in PDU. This function
 * returns the number of bytes opt has been advanced or @c 0
 * on error.
 */
static size_t next_option_safe(coap_opt_t **optp, size_t *length, coap_option_t *option)
{
	//coap_option_t option;
	size_t optsize;

	assert(optp);
	assert(*optp);
	assert(length);
	optsize = coap_opt_parse(*optp, *length, option);
	if (optsize) {
		assert(optsize <= *length);

		*optp += optsize;
		*length -= optsize;
	}

	return optsize;
}

int coap_pdu_parse(unsigned char *data, size_t length, coap_pdu_t *pdu)
{
	return coap_pdu_parse2(data, length, pdu, COAP_UDP);
}

int coap_pdu_parse2(unsigned char *data, size_t length, coap_pdu_t *pdu, coap_transport_t transport)
{
	assert(data);
	assert(pdu);

	if (pdu->max_size < length) {
		printf("coap_pdu_parse2 : insufficient space to store parsed PDU\n");
		return -1;
	}

	unsigned int headerSize = 0;

	if (COAP_UDP == transport) {
		headerSize = sizeof(pdu->transport_hdr->udp);
	}
#ifdef WITH_TCP
	else {
		headerSize = coap_get_tcp_header_length_for_transport(transport);
	}
#endif

	if (length < headerSize) {
		debug("discarded invalid PDU\n");
	}

	coap_opt_t *opt = NULL;
	unsigned int tokenLength = 0;
#ifdef WITH_TCP
	size_t i;

	switch (transport) {
	case COAP_UDP:
		break;
	case COAP_TCP:
		for (i = 0; i < headerSize; i++) {
			pdu->transport_hdr->tcp.header_data[i] = data[i];
		}

		tokenLength = data[0] & 0x0f;
		opt = (unsigned char *)(&(pdu->transport_hdr->tcp) + 1) + tokenLength;
		break;
	case COAP_TCP_8BIT:
		for (i = 0; i < headerSize; i++) {
			pdu->transport_hdr->tcp_8bit.header_data[i] = data[i];
		}

		tokenLength = data[0] & 0x0f;
		opt = (unsigned char *)(&(pdu->transport_hdr->tcp_8bit))
			  + tokenLength + COAP_TCP_HEADER_8_BIT;
		break;
	case COAP_TCP_16BIT:
		for (i = 0; i < headerSize; i++) {
			pdu->transport_hdr->tcp_16bit.header_data[i] = data[i];
		}

		tokenLength = data[0] & 0x0f;
		opt = (unsigned char *)(&(pdu->transport_hdr->tcp_16bit) + 1) + tokenLength;
		break;
	case COAP_TCP_32BIT:
		for (i = 0; i < headerSize; i++) {
			pdu->transport_hdr->tcp_32bit.header_data[i] = data[i];
		}

		tokenLength = data[0] & 0x0f;
		opt = ((unsigned char *)&(pdu->transport_hdr->tcp_32bit)) + headerSize + tokenLength;
		break;
	default:
		printf("coap_pdu_parse2 : it has wrong type\n");
	}
#endif
	debug("coap_pdu_parse2 : transport %d, headerSize %d, tokenlen %d\n", transport, headerSize, tokenLength);

	pdu->length = length;

	if (COAP_UDP == transport) {
		pdu->transport_hdr->udp.version = data[0] >> 6;
		pdu->transport_hdr->udp.type = (data[0] >> 4) & 0x03;
		pdu->transport_hdr->udp.token_length = data[0] & 0x0f;
		pdu->transport_hdr->udp.code = data[1];
		pdu->data = NULL;

		tokenLength = pdu->transport_hdr->udp.token_length;

		/* sanity checks */
		if (pdu->transport_hdr->udp.code == 0) {
			if (length != headerSize || tokenLength) {
				debug("coap_pdu_parse2: empty message is not empty\n");
				goto discard;
			}
		}

		if (length < headerSize + tokenLength || tokenLength > 8) {
			debug("coap_pdu_parse2: invalid Token\n");
			goto discard;
		}

		memcpy(&pdu->transport_hdr->udp.id, data + 2, 2);

		/* Finally calculate beginning of data block and thereby check integrity
		 * of the PDU structure. */

		/* append data (including the Token) to pdu structure */
		memcpy(&(pdu->transport_hdr->udp) + 1, data + headerSize, length - headerSize);

		/* skip header + token */
		length -= (tokenLength + headerSize);
		opt = (unsigned char *)(&(pdu->transport_hdr->udp) + 1) + tokenLength;
	}
#ifdef WITH_TCP
	else {					// common for tcp header setting
		pdu->data = NULL;

		if (length < headerSize + tokenLength || tokenLength > 8) {
			debug("coap_pdu_parse2: invalid Token\n");
			goto discard;
		}
		/* Finally calculate beginning of data block and thereby check integrity
		 * of the PDU structure. */

		/* append data (including the Token) to pdu structure */
		memcpy(((unsigned char *)pdu->transport_hdr) + headerSize, data + headerSize, length - headerSize);

		/* skip header + token */
		length -= (tokenLength + headerSize);
	}
#endif

	while (length && *opt != COAP_PAYLOAD_START) {
		coap_option_t option;
		memset(&option, 0, sizeof(coap_option_t));
		if (!next_option_safe(&opt, (size_t *)&length, &option)) {
			debug("coap_pdu_parse2: drop\n");
			goto discard;
		}
	}

	/* end of packet or start marker */
	if (length) {
		assert(*opt == COAP_PAYLOAD_START);
		opt++;
		length--;

		if (!length) {
			debug("coap_pdu_parse2: message ending in payload start marker\n");
			goto discard;
		}

		debug("set data to %p (pdu ends at %p)\n", (unsigned char *)opt, (unsigned char *)pdu->hdr + pdu->length);
		pdu->data = (unsigned char *)opt;
		//printf("[COAP] pdu - data : %s\n", pdu->data);
	}
#ifdef WITH_DEBUG_PDU_PRINT
	printf("\ncoap_parse2 : parsed PDU\n");
	coap_debug_pdu_print(pdu, transport);
#endif
	return 1;

discard:
	return 0;
}

coap_pdu_t *coap_convert_to_tcp_pdu(coap_pdu_t *pdu)
{
	coap_transport_t transport = COAP_TCP;
	coap_pdu_t *tcp_pdu = NULL;
	coap_opt_t *tcp_opt = NULL, *udp_opt = NULL;

	size_t tokenlen = 0;
	size_t newsize = 0; /* size of UDP pdu without UDP Header */
	int length = 0; /* size of UDP pdu length including UDP header */

	if (pdu == NULL) {
		warn("coap_convert_to_tcp_pdu : do not allow empty pdu\n");
		return pdu;
	}

	/* Copy Token length and Token field to CoAP over TCP Header */
	if (pdu->transport_hdr->udp.token_length > 0) {
		tokenlen = pdu->transport_hdr->udp.token_length;
	}

	if ((int)(pdu->length - COAP_UDP_HEADER - tokenlen) < 0) {
		warn("coap_convert_to_tcp_pdu : invalid length of PDU\n");
		coap_delete_pdu(pdu);
		pdu = NULL;
		return pdu;
	} else {
		/* newsize : optionLen + payloadLen + PAYLOAD_MARKER */
		newsize = (pdu->length - COAP_UDP_HEADER - tokenlen);
		transport = coap_get_tcp_header_type_from_size(newsize);
	}

	debug("coap_convert_to_tcp_pdu : transport %d, newsize %d\n", transport, newsize);

	/* FIXME: Can we make more small packet ?*/
	tcp_pdu = coap_pdu_init2(0, pdu->transport_hdr->udp.code, 0, COAP_MAX_PDU_SIZE, transport);

	if (!tcp_pdu) {
		warn("coap_convert_to_tcp_pdu : failed to create pdu for TCP\n");
		goto convert_error_discard;
	} else {
		debug("coap_convert_to_tcp_pdu : new tcp pdu is created, size %d\n", pdu->length);

		/* Fill extended length field on CoAP over TCP header */
		coap_add_length(tcp_pdu, transport, newsize);

		switch(transport) {
		case COAP_TCP:
			tcp_pdu->transport_hdr->tcp.header_data[0] |= tokenlen;
			if (tokenlen > 0)
				memcpy(tcp_pdu->transport_hdr->tcp.token, pdu->transport_hdr->udp.token, tokenlen);
			/* Move TCP pdu pointer to start of option */
			tcp_opt = (unsigned char *)(&(tcp_pdu->transport_hdr->tcp) + 1) + tokenlen;
			break;
		case COAP_TCP_8BIT:
			tcp_pdu->transport_hdr->tcp_8bit.header_data[0] |= tokenlen;
			if (tokenlen > 0)
				memcpy(tcp_pdu->transport_hdr->tcp_8bit.token, pdu->transport_hdr->udp.token, tokenlen);
			/* Move TCP pdu pointer to start of option */
			tcp_opt = (unsigned char *)(&(tcp_pdu->transport_hdr->tcp_8bit) + 1) + tokenlen;
			break;
		case COAP_TCP_16BIT:
			tcp_pdu->transport_hdr->tcp_16bit.header_data[0] |= tokenlen;
			if (tokenlen > 0)
				memcpy(tcp_pdu->transport_hdr->tcp_16bit.token, pdu->transport_hdr->udp.token, tokenlen);
			/* Move TCP pdu pointer to start of option */
			tcp_opt = (unsigned char *)(&(tcp_pdu->transport_hdr->tcp_16bit) + 1) + tokenlen;
			break;
		case COAP_TCP_32BIT:
			tcp_pdu->transport_hdr->tcp_32bit.header_data[0] |= tokenlen;
			if (tokenlen > 0)
				memcpy(tcp_pdu->transport_hdr->tcp_32bit.token, pdu->transport_hdr->udp.token, tokenlen);
			/* Move TCP pdu pointer to start of option */
			tcp_opt = (unsigned char *)(&(tcp_pdu->transport_hdr->tcp_32bit) + 1) + tokenlen;
			break;
		default:
			break;
		}

		/* Fill extra TCP pdu Info */
		tcp_pdu->max_size = pdu->max_size;
		tcp_pdu->max_delta = pdu->max_delta;
		tcp_pdu->length = newsize + tokenlen + coap_get_tcp_header_length_for_transport(transport);
		length = (int)tcp_pdu->length;

		/* Move UDP pdu pointer to start of option */
		udp_opt = (unsigned char *)(&(pdu->transport_hdr->udp) +1) + tokenlen;

		/* copy udp pdu to tcp pdu starting from token */
		memcpy(tcp_opt, udp_opt, (newsize + 1));

		length -= (tokenlen + coap_get_tcp_header_length_for_transport(transport));

		if (length < 0) {
				/* Reset to length to 0 when there are no options and no payload */
				length = 0;
		}

		while (length && *tcp_opt != COAP_PAYLOAD_START) {
			coap_option_t option;
			memset(&option, 0, sizeof(coap_option_t));
			if (!next_option_safe(&tcp_opt, (size_t *)&length, &option)) {
				printf("coap_convert_to_tcp_pdu : drop\n");
				goto convert_error_discard;
			}
		}

		if (length) {
			if (*tcp_opt != COAP_PAYLOAD_START) {
				/* somethings wrong ..*/
				printf("coap_convert_to_tcp_pdu : cannot find COAP_PAYLOAD_START\n");
				goto convert_error_discard;
			} else {
				tcp_opt++;
				length--;
				if (length > 0)
					tcp_pdu->data = (unsigned char *)tcp_opt;
			}
		}

		/* If you want to see the PDU in details, enable below functions */
#ifdef WITH_DEBUG_PDU_PRINT
		coap_debug_pdu_print(pdu, COAP_UDP);
		coap_debug_pdu_print(tcp_pdu, transport);
#endif

		/* Use TCP PDU, so free UDP pdu */
		coap_delete_pdu(pdu);
		pdu = NULL;
	}

	return tcp_pdu;

convert_error_discard:
	coap_delete_pdu(tcp_pdu);
	coap_delete_pdu(pdu);

	pdu = NULL;
	tcp_pdu = NULL;

	return tcp_pdu;
}

void coap_debug_pdu_print(coap_pdu_t *pdu, coap_transport_t transport)
{
	size_t tokenLen = 0;
	char buffer[COAP_MAX_PDU_SIZE] = {0,};

	if (!pdu) {
		printf("coap_debug_pdu_print : pdu is empty\n");
		return;
	}

	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");

	switch(transport) {
	case COAP_UDP: /* 4 Bytes Header */
		printf("UDP header\n");

		tokenLen = pdu->transport_hdr->udp.token_length;

		printf("ver %d | type %d | tkl %d | code %d | mid %d\n",
					pdu->transport_hdr->udp.version,
					pdu->transport_hdr->udp.type,
					pdu->transport_hdr->udp.token_length,
					pdu->transport_hdr->udp.code,
					pdu->transport_hdr->udp.id);

		if (tokenLen > 0) {
			strncpy(buffer, (const char *)pdu->transport_hdr->udp.token, tokenLen);
			printf("token %s\n", buffer);
		}

		break;
	case COAP_TCP: /* 2 Bytes Header */
		printf("TCP header\n");

		tokenLen = (pdu->transport_hdr->tcp.header_data[0] & 0x0f);

		printf("len %d | tkl %d | code %d\n",
					((pdu->transport_hdr->tcp.header_data[0] >> 4) & 0x0f),
					(pdu->transport_hdr->tcp.header_data[0] & 0x0f),
					pdu->transport_hdr->tcp.header_data[1]);

		if (tokenLen > 0) {
			strncpy(buffer, (const char *)pdu->transport_hdr->tcp.token, tokenLen);
			printf("token %s\n", buffer);
		}

		break;
	case COAP_TCP_8BIT: /* 3 Bytes Header */
		printf("TCP-8bit header\n");

		tokenLen = (pdu->transport_hdr->tcp_8bit.header_data[0] & 0x0f);

		printf("len %d | tkl %d | extended len %x | code %d\n",
					((pdu->transport_hdr->tcp_8bit.header_data[0] >> 4) & 0x0f),
					(pdu->transport_hdr->tcp_8bit.header_data[0] & 0x0f),
					pdu->transport_hdr->tcp_8bit.header_data[1],
					pdu->transport_hdr->tcp_8bit.header_data[2]);

		if (tokenLen > 0) {
			strncpy(buffer, (const char *)pdu->transport_hdr->tcp_8bit.token, tokenLen);
			printf("token %s\n", buffer);
		}

		break;
	case COAP_TCP_16BIT: /* 4 Bytes Header */
		printf("TCP-16bit header\n");

		tokenLen = (pdu->transport_hdr->tcp_16bit.header_data[0] & 0x0f);

		printf("len %d | tkl %d | extended len %x%x | code %d\n",
					((pdu->transport_hdr->tcp_16bit.header_data[0] >> 4) & 0x0f),
					(pdu->transport_hdr->tcp_16bit.header_data[0] & 0x0f),
					pdu->transport_hdr->tcp_16bit.header_data[1],
					pdu->transport_hdr->tcp_16bit.header_data[2],
					pdu->transport_hdr->tcp_16bit.header_data[3]);

		if (tokenLen > 0) {
			strncpy(buffer, (const char *)pdu->transport_hdr->tcp_16bit.token, tokenLen);
			printf("token %s\n", buffer);
		}

		break;
	case COAP_TCP_32BIT: /* 6 Bytes Header */
		printf("TCP-32bit header\n");

		tokenLen = (pdu->transport_hdr->tcp_32bit.header_data[0] & 0x0f);

		printf("len %d | tkl %d | extended len %x%x%x%x | code %d\n",
					((pdu->transport_hdr->tcp_32bit.header_data[0] >> 4) & 0x0f),
					(pdu->transport_hdr->tcp_32bit.header_data[0] & 0x0f),
					pdu->transport_hdr->tcp_32bit.header_data[1],
					pdu->transport_hdr->tcp_32bit.header_data[2],
					pdu->transport_hdr->tcp_32bit.header_data[3],
					pdu->transport_hdr->tcp_32bit.header_data[4],
					pdu->transport_hdr->tcp_32bit.header_data[5]);

		if (tokenLen > 0) {
			strncpy(buffer, (const char *)pdu->transport_hdr->tcp_32bit.token, tokenLen);
			printf("token %s\n", buffer);
		}

		break;
	default:
		break;
	}

	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
	coap_opt_iterator_t opt_iterator;

	if (!coap_option_iterator_init2(pdu, &opt_iterator, NULL, transport)) {
		printf("No options\n");
	} else {
		coap_opt_t *option = NULL;
		size_t opt_len = 0;

		while ((option = coap_option_next(&opt_iterator))) {
			/* TODO : should consider option delta (extended) */
			memset(buffer, 0x0, COAP_MAX_PDU_SIZE);
			opt_len = (size_t)coap_opt_length(option);
			if (opt_len > 0) {
				strncpy(buffer, (const char *)coap_opt_value(option), opt_len);
			}

			printf("option delta %u len %d value %s\n",
					coap_opt_delta(option), opt_len, buffer);
		}
	}

	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
	if (!pdu->data)
		printf("No payload\n");
	else
		printf("payload %s\n", pdu->data);

	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
	printf("additional info : pdu_len %d max delta %d max size %d\n",
				pdu->length, pdu->max_delta, pdu->max_size);
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}
