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
/* net.c -- CoAP network interface
 *
 * Copyright (C) 2010--2014 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include <protocols/libcoap/config.h>

#include <ctype.h>
#include <stdio.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#elif HAVE_SYS_UNISTD_H
#include <sys/unistd.h>
#endif
#include <sys/types.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef WITH_LWIP
#include <net/lwip/pbuf.h>
#include <net/lwip/udp.h>
#include <net/lwip/timers.h>
#endif

#include <protocols/libcoap/debug.h>
#include <protocols/libcoap/mem.h>
#include <protocols/libcoap/str.h>
#include <protocols/libcoap/async.h>
#include <protocols/libcoap/resource.h>
#include <protocols/libcoap/option.h>
#include <protocols/libcoap/encode.h>
#include <protocols/libcoap/block.h>
#include <protocols/libcoap/net.h>

#if defined(WITH_POSIX)

time_t clock_offset;

static inline coap_queue_t *coap_malloc_node(void)
{
	return (coap_queue_t *) coap_malloc(sizeof(coap_queue_t));
}

static inline void coap_free_node(coap_queue_t *node)
{
	coap_free(node);
}
#endif							/* WITH_POSIX */
#ifdef WITH_LWIP

#include <net/lwip/memp.h>

static void coap_retransmittimer_execute(void *arg);
static void coap_retransmittimer_restart(coap_context_t *ctx);

static inline coap_queue_t *coap_malloc_node()
{
	return (coap_queue_t *) memp_malloc(MEMP_COAP_NODE);
}

static inline void coap_free_node(coap_queue_t *node)
{
	memp_free(MEMP_COAP_NODE, node);
}

#endif							/* WITH_LWIP */
#ifdef WITH_CONTIKI
#ifndef DEBUG
#define DEBUG DEBUG_PRINT
#endif							/* DEBUG */

#include "memb.h"
#include "net/uip-debug.h"

clock_time_t clock_offset;

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[UIP_LLIPH_LEN])

void coap_resources_init();
void coap_pdu_resources_init();

unsigned char initialized = 0;
coap_context_t the_coap_context;

MEMB(node_storage, coap_queue_t, COAP_PDU_MAXCNT);

PROCESS(coap_retransmit_process, "message retransmit process");

static inline coap_queue_t *coap_malloc_node()
{
	return (coap_queue_t *) memb_alloc(&node_storage);
}

static inline void coap_free_node(coap_queue_t *node)
{
	memb_free(&node_storage, node);
}
#endif							/* WITH_CONTIKI */
#ifdef WITH_LWIP

/** Callback to udp_recv when using lwIP. Gets called by lwIP on arriving
 * packages, places a reference in context->pending_package, and calls
 * coap_read to process the package. Thus, coap_read needs not be called in
 * lwIP main loops. (When modifying this for thread-like operation, ie. if you
 * remove the coap_read call from this, make sure that coap_read gets a chance
 * to run before this callback is entered the next time.)
 */
static void received_package(void *arg, struct udp_pcb *upcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
	struct coap_context_t *context = (coap_context_t *) arg;

	LWIP_ASSERT("pending_package was not cleared.", context->pending_package == NULL);

	context->pending_package = p;	/* we don't free it, coap_read has to do that */
	context->pending_address.addr = addr->addr;	/* FIXME: this has to become address-type independent, probably there'll be an lwip function for that */
	context->pending_port = port;

	coap_read(context);
}

#endif							/* WITH_LWIP */

int print_wellknown(coap_context_t *, unsigned char *, size_t *, size_t, coap_opt_t *);

void coap_handle_failed_notify(coap_context_t *, const coap_address_t *, const str *);

unsigned int coap_adjust_basetime(coap_context_t *ctx, coap_tick_t now)
{
	unsigned int result = 0;
	coap_tick_diff_t delta = now - ctx->sendqueue_basetime;

	if (ctx->sendqueue) {
		/* delta < 0 means that the new time stamp is before the old. */
		if (delta <= 0) {
			ctx->sendqueue->t -= delta;
		} else {
			/* This case is more complex: The time must be advanced forward,
			 * thus possibly leading to timed out elements at the queue's
			 * start. For every element that has timed out, its relative
			 * time is set to zero and the result counter is increased. */

			coap_queue_t *q = ctx->sendqueue;
			coap_tick_t t = 0;
			while (q && (t + q->t < (coap_tick_t) delta)) {
				t += q->t;
				q->t = 0;
				result++;
				q = q->next;
			}

			/* finally adjust the first element that has not expired */
			if (q) {
				q->t = (coap_tick_t) delta - t;
			}
		}
	}

	/* adjust basetime */
	ctx->sendqueue_basetime += delta;

	return result;
}

int coap_insert_node(coap_queue_t **queue, coap_queue_t *node)
{
	coap_queue_t *p, *q;
	if (!queue || !node) {
		return 0;
	}

	/* set queue head if empty */
	if (!*queue) {
		*queue = node;
		return 1;
	}

	/* replace queue head if PDU's time is less than head's time */
	q = *queue;
	if (node->t < q->t) {
		node->next = q;
		*queue = node;
		q->t -= node->t;		/* make q->t relative to node->t */
		return 1;
	}

	/* search for right place to insert */
	do {
		node->t -= q->t;		/* make node-> relative to q->t */
		p = q;
		q = q->next;
	} while (q && q->t <= node->t);

	/* insert new item */
	if (q) {
		q->t -= node->t;		/* make q->t relative to node->t */
	}
	node->next = q;
	p->next = node;
	return 1;
}

int coap_delete_node(coap_queue_t *node)
{
	if (!node) {
		return 0;
	}

	coap_delete_pdu(node->pdu);
	coap_free_node(node);

	return 1;
}

void coap_delete_all(coap_queue_t *queue)
{
	if (!queue) {
		return;
	}

	coap_delete_all(queue->next);
	coap_delete_node(queue);
}

coap_queue_t *coap_new_node()
{
	coap_queue_t *node;
	node = coap_malloc_node();

	if (!node) {
#ifndef NDEBUG
		coap_log(LOG_WARNING, "coap_new_node: malloc\n");
#endif
		return NULL;
	}

	memset(node, 0, sizeof * node);
	return node;
}

coap_queue_t *coap_peek_next(coap_context_t *context)
{
	if (!context || !context->sendqueue) {
		return NULL;
	}

	return context->sendqueue;
}

coap_queue_t *coap_pop_next(coap_context_t *context)
{
	coap_queue_t *next;

	if (!context || !context->sendqueue) {
		return NULL;
	}

	next = context->sendqueue;
	context->sendqueue = context->sendqueue->next;
	if (context->sendqueue) {
		context->sendqueue->t += next->t;
	}
	next->next = NULL;
	return next;
}

#ifdef COAP_DEFAULT_WKC_HASHKEY
/** Checks if @p Key is equal to the pre-defined hash key for.well-known/core. */
#define is_wkc(Key)							\
  (memcmp((Key), COAP_DEFAULT_WKC_HASHKEY, sizeof(coap_key_t)) == 0)
#else
/* Implements a singleton to store a hash key for the .wellknown/core
 * resources. */
int is_wkc(coap_key_t k)
{
	static coap_key_t wkc;
	static unsigned char _initialized = 0;
	if (!_initialized) {
		_initialized = coap_hash_path((unsigned char *)COAP_DEFAULT_URI_WELLKNOWN, sizeof(COAP_DEFAULT_URI_WELLKNOWN) - 1, wkc);
	}
	return memcmp(k, wkc, sizeof(coap_key_t)) == 0;
}
#endif

coap_context_t *coap_create_context(coap_protocol_t protocol)
{
#ifdef WITH_POSIX
	coap_context_t *c = coap_malloc(sizeof(coap_context_t));

	if (!c) {
		coap_log(LOG_EMERG, "failed to create context, errno %d\n", errno);
		return NULL;
	}
	coap_clock_init();
	prng_init(rand());

	memset(c, 0, sizeof(coap_context_t));

	/* initialize message id */
	prng((unsigned char *)&c->message_id, sizeof(unsigned short));

	/* register the critical options that we know */
	coap_register_option(c, COAP_OPTION_IF_MATCH);
	coap_register_option(c, COAP_OPTION_URI_HOST);
	coap_register_option(c, COAP_OPTION_IF_NONE_MATCH);
	coap_register_option(c, COAP_OPTION_URI_PORT);
	coap_register_option(c, COAP_OPTION_URI_PATH);
	coap_register_option(c, COAP_OPTION_URI_QUERY);
	coap_register_option(c, COAP_OPTION_ACCEPT);
	coap_register_option(c, COAP_OPTION_PROXY_URI);
	coap_register_option(c, COAP_OPTION_PROXY_SCHEME);
	coap_register_option(c, COAP_OPTION_BLOCK2);
	coap_register_option(c, COAP_OPTION_BLOCK1);

	/* initialize socket file descriptor */
	c->sockfd = -1;
	/* set transport protocol information in context */
	c->protocol = protocol;
#ifdef WITH_MBEDTLS
	/* initialize TLS session pointer in context */
	c->session = NULL;
#endif /* WITH_MBEDTLS */
	return c;
#else
	return NULL;
#endif /* WITH_POSIX */
}

coap_context_t *coap_new_context(const coap_address_t *listen_addr)
{
#ifdef WITH_POSIX
	coap_context_t *c = coap_malloc(sizeof(coap_context_t));
	int reuse = 1;
#endif							/* WITH_POSIX */
#ifdef WITH_LWIP
	coap_context_t *c = memp_malloc(MEMP_COAP_CONTEXT);
#endif							/* WITH_LWIP */
#ifdef WITH_CONTIKI
	coap_context_t *c;

	if (initialized) {
		return NULL;
	}
#endif							/* WITH_CONTIKI */

	if (!listen_addr) {
		coap_log(LOG_EMERG, "no listen address specified\n");
#ifdef WITH_POSIX
		/* To prevent memory leakage */
		if (c) {
			free(c);
		}
#endif
		return NULL;
	}

	coap_clock_init();
#ifdef WITH_LWIP
	prng_init(LWIP_RAND());
#else							/* WITH_LWIP */
	prng_init((unsigned long)listen_addr ^ clock_offset);
#endif							/* WITH_LWIP */

#ifndef WITH_CONTIKI
	if (!c) {
#ifndef NDEBUG
		coap_log(LOG_EMERG, "coap_init: malloc:\n");
#endif
		return NULL;
	}
#endif							/* not WITH_CONTIKI */
#ifdef WITH_CONTIKI
	coap_resources_init();
	coap_pdu_resources_init();

	c = &the_coap_context;
	initialized = 1;
#endif							/* WITH_CONTIKI */

	memset(c, 0, sizeof(coap_context_t));

	/* initialize message id */
	prng((unsigned char *)&c->message_id, sizeof(unsigned short));

	/* register the critical options that we know */
	coap_register_option(c, COAP_OPTION_IF_MATCH);
	coap_register_option(c, COAP_OPTION_URI_HOST);
	coap_register_option(c, COAP_OPTION_IF_NONE_MATCH);
	coap_register_option(c, COAP_OPTION_URI_PORT);
	coap_register_option(c, COAP_OPTION_URI_PATH);
	coap_register_option(c, COAP_OPTION_URI_QUERY);
	coap_register_option(c, COAP_OPTION_ACCEPT);
	coap_register_option(c, COAP_OPTION_PROXY_URI);
	coap_register_option(c, COAP_OPTION_PROXY_SCHEME);
	coap_register_option(c, COAP_OPTION_BLOCK2);
	coap_register_option(c, COAP_OPTION_BLOCK1);

#ifdef WITH_POSIX
	c->sockfd = socket(listen_addr->addr.sa.sa_family, SOCK_DGRAM, 0);

	if (c->sockfd < 0) {
		coap_log(LOG_EMERG, "coap_new_context: failed to create socket\n");
		goto onerror;
	}

	if (setsockopt(c->sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		coap_log(LOG_WARNING, "coap_new_context : failed to setsockopt SO_REUSEADDR\n");
	}

	if (bind(c->sockfd, &listen_addr->addr.sa, listen_addr->size) < 0) {
		coap_log(LOG_EMERG, "coap_new_context : failed to bind socket\n");
		goto onerror;
	}

	return c;

onerror:
	if (c->sockfd >= 0) {
		close(c->sockfd);
	}
	coap_free(c);
	return NULL;

#endif /* WITH_POSIX */
#ifdef WITH_CONTIKI
	c->conn = udp_new(NULL, 0, NULL);
	udp_bind(c->conn, listen_addr->port);

	process_start(&coap_retransmit_process, (char *)c);

	PROCESS_CONTEXT_BEGIN(&coap_retransmit_process);
#ifndef WITHOUT_OBSERVE
	etimer_set(&c->notify_timer, COAP_RESOURCE_CHECK_TIME * COAP_TICKS_PER_SECOND);
#endif							/* WITHOUT_OBSERVE */
	/* the retransmit timer must be initialized to some large value */
	etimer_set(&the_coap_context.retransmit_timer, 0xFFFF);
	PROCESS_CONTEXT_END(&coap_retransmit_process);
	return c;
#endif							/* WITH_CONTIKI */
#ifdef WITH_LWIP
	c->pcb = udp_new();
	/* hard assert: this is not expected to fail dynamically */
	LWIP_ASSERT("Failed to allocate PCB for CoAP", c->pcb != NULL);

	udp_recv(c->pcb, received_package, (void *)c);
	udp_bind(c->pcb, &listen_addr->addr, listen_addr->port);

	c->timer_configured = 0;

	return c;
#endif
}

void coap_free_context(coap_context_t *context)
{
#if defined(WITH_POSIX) || defined(WITH_LWIP)
	coap_resource_t *res;
#ifndef COAP_RESOURCES_NOHASH
	coap_resource_t *rtmp;
#endif
#endif							/* WITH_POSIX || WITH_LWIP */
	if (!context) {
		return;
	}

	coap_delete_all(context->recvqueue);
	coap_delete_all(context->sendqueue);

#ifdef WITH_LWIP
	context->sendqueue = NULL;
	coap_retransmittimer_restart(context);
#endif

#if defined(WITH_POSIX) || defined(WITH_LWIP)
#ifdef COAP_RESOURCES_NOHASH
	LL_FOREACH(context->resources, res) {
#else
	HASH_ITER(hh, context->resources, res, rtmp) {
#endif
/* To prevent memory leakage due to remained subscribers */
#ifndef WITHOUT_OBSERVE
		coap_subscription_t *s = NULL;
		for (s = list_head(res->subscribers); s; s = list_item_next(s)) {
			if (s) {
				list_remove(res->subscribers, s);
				coap_free(s);
			}
		}
#endif
		coap_delete_resource(context, res->key);
	}
#endif /* WITH_POSIX || WITH_LWIP */

#ifdef WITH_POSIX
	/* coap_delete_list(context->subscriptions); */
	close(context->sockfd);
	coap_free(context);
#endif
#ifdef WITH_LWIP
	udp_remove(context->pcb);
	memp_free(MEMP_COAP_CONTEXT, context);
#endif
#ifdef WITH_CONTIKI
	memset(&the_coap_context, 0, sizeof(coap_context_t));
	initialized = 0;
#endif /* WITH_CONTIKI */
}

int coap_option_check_critical2(coap_context_t *ctx, coap_pdu_t *pdu, coap_opt_filter_t unknown, coap_transport_t transport)
{
	coap_opt_iterator_t opt_iter;
	int ok = 1;

	coap_option_iterator_init2(pdu, &opt_iter, COAP_OPT_ALL, transport);

	while (coap_option_next(&opt_iter)) {

		/* The following condition makes use of the fact that
		 * coap_option_getb() returns -1 if type exceeds the bit-vector
		 * filter. As the vector is supposed to be large enough to hold
		 * the largest known option, we know that everything beyond is
		 * bad.
		 */
		if (opt_iter.type & 0x01 && coap_option_getb(ctx->known_options, opt_iter.type) < 1) {
			debug("unknown critical option %d\n", opt_iter.type);

			ok = 0;

			/* When opt_iter.type is beyond our known option range,
			 * coap_option_setb() will return -1 and we are safe to leave
			 * this loop. */
			if (coap_option_setb(unknown, opt_iter.type) == -1) {
				break;
			}
		}
	}

	return ok;
}

int coap_option_check_critical(coap_context_t *ctx, coap_pdu_t *pdu, coap_opt_filter_t unknown)
{
	return coap_option_check_critical2(ctx, pdu, unknown, COAP_UDP);
}

void coap_transaction_id2(const coap_address_t *peer, const coap_pdu_t *pdu, coap_tid_t *id, coap_protocol_t protocol)
{
	coap_key_t h;

	memset(h, 0, sizeof(coap_key_t));

	/* FIXME : CoAP over TCP doesn't need to check mid */
	if (protocol == COAP_PROTO_TCP || protocol == COAP_PROTO_TLS) {
		*id = protocol;
		return;
	}

	/* Compare the complete address structure in case of IPv4. For IPv6,
	 * we need to look at the transport address only. */

#ifdef WITH_POSIX
	switch (peer->addr.sa.sa_family) {
	case AF_INET:
#if defined (__TINYARA__)
		/* Local Fixes : Make hash value using IPv4 addr and port only */
		coap_hash((const unsigned char *)&peer->addr.sin.sin_addr, sizeof(peer->addr.sin.sin_addr), h);
		coap_hash((const unsigned char *)&peer->addr.sin.sin_port, sizeof(peer->addr.sin.sin_port), h);
#else
		coap_hash((const unsigned char *)&peer->addr.sa, peer->size, h);
#endif
		break;
	case AF_INET6:
		coap_hash((const unsigned char *)&peer->addr.sin6.sin6_port, sizeof(peer->addr.sin6.sin6_port), h);
		coap_hash((const unsigned char *)&peer->addr.sin6.sin6_addr, sizeof(peer->addr.sin6.sin6_addr), h);
		break;
	default:
		return;
	}
#endif
#if defined(WITH_LWIP) || defined(WITH_CONTIKI)
	/* FIXME: with lwip, we can do better */
	coap_hash((const unsigned char *)&peer->port, sizeof(peer->port), h);
	coap_hash((const unsigned char *)&peer->addr, sizeof(peer->addr), h);
#endif							/* WITH_LWIP || WITH_CONTIKI */
	coap_hash((const unsigned char *)&pdu->transport_hdr->udp.id, sizeof(unsigned short), h);

	*id = ((h[0] << 8) | h[1]) ^ ((h[2] << 8) | h[3]);
}

void coap_transaction_id(const coap_address_t *peer, const coap_pdu_t *pdu, coap_tid_t *id)
{
	coap_transaction_id2(peer, pdu, id, COAP_PROTO_UDP);
}

coap_tid_t coap_send_ack(coap_context_t *context, const coap_address_t *dst, coap_pdu_t *request)
{
	coap_pdu_t *response = NULL;
	coap_tid_t result = COAP_INVALID_TID;

	coap_transport_t transport = COAP_UDP;

	switch(context->protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		if (request && request->transport_hdr->udp.type == COAP_MESSAGE_CON) {
			response = coap_pdu_init(COAP_MESSAGE_ACK, 0, request->transport_hdr->udp.id, sizeof(coap_pdu_t));
		}
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)request->transport_hdr)[0] >> 4);
		result = COAP_TCP_TID;
		/* Do nothing? */
		break;
	default:
		break;
	}

	if (response) {
		result = coap_send(context, dst, response);
		coap_delete_pdu(response);
	}

	return result;
}

#ifdef WITH_POSIX
/* releases space allocated by PDU if free_pdu is set */
coap_tid_t coap_send_impl(coap_context_t *context, const coap_address_t *dst, coap_pdu_t *pdu)
{
	ssize_t bytes_written = -1;
	coap_tid_t id = COAP_INVALID_TID;

	if (!context || !dst || !pdu) {
		return id;
	}

	switch (context->protocol) {
	case COAP_PROTO_UDP:
		bytes_written = sendto(context->sockfd, pdu->transport_hdr, pdu->length, 0, &dst->addr.sa, dst->size);
		break;
	case COAP_PROTO_TCP:
		debug("coap_send_impl : enter, sockfd %d\n", context->sockfd);
		bytes_written = send(context->sockfd, pdu->transport_hdr, pdu->length, 0);
		break;
#ifdef WITH_MBEDTLS
	case COAP_PROTO_TLS:
	case COAP_PROTO_DTLS:
		bytes_written = mbedtls_ssl_write(context->session->ssl, (unsigned char *)pdu->hdr, pdu->length);
		break;
#endif
	default:
		warn("coap_send_impl : not supported transport protocol %d\n", context->protocol);
		break;
	}

	if (bytes_written >= 0) {
		coap_transaction_id2(dst, pdu, &id, context->protocol);
		debug("coap_send_impl : protocol %d, bytes_written %d, tid %d\n", context->protocol, bytes_written, id);
	} else {
		coap_log(LOG_CRIT, "coap_send: sendto\n");
	}

	return id;
}
#endif /* WITH_POSIX */
#ifdef WITH_CONTIKI
/* releases space allocated by PDU if free_pdu is set */
coap_tid_t coap_send_impl(coap_context_t *context, const coap_address_t *dst, coap_pdu_t *pdu)
{
	coap_tid_t id = COAP_INVALID_TID;

	if (!context || !dst || !pdu) {
		return id;
	}

	/* FIXME: is there a way to check if send was successful? */
	uip_udp_packet_sendto(context->conn, pdu->hdr, pdu->length, &dst->addr, dst->port);

	coap_transaction_id(dst, pdu, &id);

	return id;
}
#endif							/* WITH_CONTIKI */
#ifdef WITH_LWIP
coap_tid_t coap_send_impl(coap_context_t *context, const coap_address_t *dst, coap_pdu_t *pdu)
{
	coap_tid_t id = COAP_INVALID_TID;
	struct pbuf *p;
	uint8_t err;
	char *data_backup;

	if (!context || !dst || !pdu) {
		return id;
	}

	data_backup = pdu->data;

	/* FIXME: we can't check this here with the existing infrastructure, but we
	 * should actually check that the pdu is not held by anyone but us. the
	 * respective pbuf is already exclusively owned by the pdu. */

	p = pdu->pbuf;
	LWIP_ASSERT("The PDU header is not where it is expected", pdu->hdr == p->payload + sizeof(coap_pdu_t));

	err = pbuf_header(p, -sizeof(coap_pdu_t));
	if (err) {
		debug("coap_send_impl: pbuf_header failed\n");
		pbuf_free(p);
		return id;
	}

	coap_transaction_id(dst, pdu, &id);

	pbuf_realloc(p, pdu->length);

	udp_sendto(context->pcb, p, &dst->addr, dst->port);

	pbuf_header(p, -(ptrdiff_t)((uint8_t *) pdu - (uint8_t *) p->payload) - sizeof(coap_pdu_t));	/* FIXME hack around udp_sendto not restoring; see http://lists.gnu.org/archive/html/lwip-users/2013-06/msg00008.html. for udp over ip over ethernet, this was -42; as we're doing ppp too, this has to be calculated generically */

	err = pbuf_header(p, sizeof(coap_pdu_t));
	LWIP_ASSERT("Cannot undo pbuf_header", err == 0);

	/* restore destroyed pdu data */
	LWIP_ASSERT("PDU not restored", p->payload == pdu);
	pdu->max_size = p->tot_len - sizeof(coap_pdu_t);	/* reduced after pbuf_realloc */
	pdu->hdr = p->payload + sizeof(coap_pdu_t);
	pdu->max_delta = 0;			/* won't be used any more */
	pdu->length = pdu->max_size;
	pdu->data = data_backup;
	pdu->pbuf = p;

	return id;
}
#endif /* WITH_LWIP */

coap_tid_t coap_send(coap_context_t *context, const coap_address_t *dst, coap_pdu_t *pdu)
{
	return coap_send_impl(context, dst, pdu);
}

coap_tid_t coap_send_error(coap_context_t *context, coap_pdu_t *request, const coap_address_t *dst, unsigned char code, coap_opt_filter_t opts)
{
	coap_pdu_t *response;
	coap_tid_t result = COAP_INVALID_TID;

	assert(request);
	assert(dst);

	response = coap_new_error_response(request, code, opts);
	if (response) {
		result = coap_send(context, dst, response);
		coap_delete_pdu(response);
	}

	return result;
}

coap_tid_t coap_send_message_type(coap_context_t *context, const coap_address_t *dst, coap_pdu_t *request, unsigned char type)
{
	coap_pdu_t *response;
	coap_tid_t result = COAP_INVALID_TID;

	coap_transport_t transport;

	switch (context->protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		transport = COAP_UDP;
		if (request) {
			response = coap_pdu_init(type, 0, request->transport_hdr->udp.id, sizeof(coap_pdu_t));
			if (response) {
				result = coap_send(context, dst, response);
				coap_delete_pdu(response);
			}
		}
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)request)[0] >> 4);
		result = COAP_TCP_TID;
		break;
	default:
		break;
	}

	return result;
}

coap_tid_t coap_send_confirmed(coap_context_t *context, const coap_address_t *dst, coap_pdu_t *pdu)
{
	coap_queue_t *node;
	coap_tick_t now;
	int r;

	node = coap_new_node();
	if (!node) {
		debug("coap_send_confirmed: insufficient memory\n");
		return COAP_INVALID_TID;
	}

	node->id = coap_send_impl(context, dst, pdu);
	if (COAP_INVALID_TID == node->id) {
		debug("coap_send_confirmed: error sending pdu\n");
		coap_free_node(node);
		return COAP_INVALID_TID;
	}

	prng((unsigned char *)&r, sizeof(r));

	/* add randomized RESPONSE_TIMEOUT to determine retransmission timeout */
	node->timeout = COAP_DEFAULT_RESPONSE_TIMEOUT * COAP_TICKS_PER_SECOND + (COAP_DEFAULT_RESPONSE_TIMEOUT >> 1) * ((COAP_TICKS_PER_SECOND * (r & 0xFF)) >> 8);

	memcpy(&node->remote, dst, sizeof(coap_address_t));
	node->pdu = pdu;

	/* Set timer for pdu retransmission. If this is the first element in
	 * the retransmission queue, the base time is set to the current
	 * time and the retransmission time is node->timeout. If there is
	 * already an entry in the sendqueue, we must check if this node is
	 * to be retransmitted earlier. Therefore, node->timeout is first
	 * normalized to the base time and then inserted into the queue with
	 * an adjusted relative time.
	 */
	coap_ticks(&now);
	if (context->sendqueue == NULL) {
		node->t = node->timeout;
		context->sendqueue_basetime = now;
	} else {
		/* make node->t relative to context->sendqueue_basetime */
		node->t = (now - context->sendqueue_basetime) + node->timeout;
	}

	coap_insert_node(&context->sendqueue, node);

#ifdef WITH_LWIP
	if (node == context->sendqueue) {	/* don't bother with timer stuff if there are earlier retransmits */
		coap_retransmittimer_restart(context);
	}
#endif

#ifdef WITH_CONTIKI
	{							/* (re-)initialize retransmission timer */
		coap_queue_t *nextpdu;

		nextpdu = coap_peek_next(context);
		assert(nextpdu);		/* we have just inserted a node */

		/* must set timer within the context of the retransmit process */
		PROCESS_CONTEXT_BEGIN(&coap_retransmit_process);
		etimer_set(&context->retransmit_timer, nextpdu->t);
		PROCESS_CONTEXT_END(&coap_retransmit_process);
	}
#endif /* WITH_CONTIKI */

	return node->id;
}

coap_tid_t coap_retransmit(coap_context_t *context, coap_queue_t *node)
{
	if (!context || !node) {
		return COAP_INVALID_TID;
	}

	/* re-initialize timeout when maximum number of retransmissions are not reached yet */
	if (node->retransmit_cnt < COAP_DEFAULT_MAX_RETRANSMIT) {
		node->retransmit_cnt++;
		node->t = node->timeout << node->retransmit_cnt;
		coap_insert_node(&context->sendqueue, node);
#ifdef WITH_LWIP
		if (node == context->sendqueue) {	/* don't bother with timer stuff if there are earlier retransmits */
			coap_retransmittimer_restart(context);
		}
#endif

		debug("** retransmission #%d of transaction %d\n", node->retransmit_cnt, ntohs(node->pdu->transport_hdr->udp.id));

		node->id = coap_send_impl(context, &node->remote, node->pdu);
		return node->id;
	}

	/* no more retransmissions, remove node from system */

#ifndef WITH_CONTIKI
	debug("** removed transaction %d\n", ntohs(node->id));
#endif

#ifndef WITHOUT_OBSERVE
	/* Check if subscriptions exist that should be canceled after
	   COAP_MAX_NOTIFY_FAILURES */
	/* TODO : Considering TCP Case */
	if (node->pdu->transport_hdr->udp.code >= 64) {
		str token = { 0, NULL };

		token.length = node->pdu->transport_hdr->udp.token_length;
		token.s = node->pdu->transport_hdr->udp.token;

		coap_handle_failed_notify(context, &node->remote, &token);
	}
#endif /* WITHOUT_OBSERVE */

	/* And finally delete the node */
	coap_delete_node(node);
	return COAP_INVALID_TID;
}

/**
 * Checks if @p opt fits into the message that ends with @p maxpos.
 * This function returns @c 1 on success, or @c 0 if the option @p opt
 * would exceed @p maxpos.
 */
static inline int check_opt_size(coap_opt_t *opt, unsigned char *maxpos)
{
	if (opt && opt < maxpos) {
		if (((*opt & 0x0f) < 0x0f) || (opt + 1 < maxpos)) {
			return opt + COAP_OPT_SIZE(opt) < maxpos;
		}
	}
	return 0;
}

int coap_read(coap_context_t *ctx)
{
#ifdef WITH_POSIX
	static char buf[COAP_MAX_PDU_SIZE];
#endif
#if defined(WITH_LWIP) || defined(WITH_CONTIKI)
	char *buf;
#endif
	coap_hdr_t *pdu;
	ssize_t bytes_read = -1;
	coap_address_t src, dst;
	coap_queue_t *node;

#ifdef WITH_CONTIKI
	buf = uip_appdata;
#endif							/* WITH_CONTIKI */
#ifdef WITH_LWIP
	LWIP_ASSERT("No package pending", ctx->pending_package != NULL);
	LWIP_ASSERT("Can only deal with contiguous PBUFs to read the initial details", ctx->pending_package->tot_len == ctx->pending_package->len);
	buf = ctx->pending_package->payload;
#endif							/* WITH_LWIP */

	pdu = (coap_hdr_t *) buf;

	coap_address_init(&src);

#ifdef WITH_POSIX
	switch (ctx->protocol) {
	case COAP_PROTO_UDP:
		bytes_read = recvfrom(ctx->sockfd, buf, sizeof(buf), 0, &src.addr.sa, &src.size);
		break;
	case COAP_PROTO_TCP:
		bytes_read = recv(ctx->sockfd, buf, sizeof(buf), 0);
		break;
#ifdef WITH_MBEDTLS
	case COAP_PROTO_DTLS:
	case COAP_PROTO_TLS:
		bytes_read = mbedtls_ssl_read(ctx->session->ssl, (unsigned char *)buf, sizeof(buf));
		break;
#endif
	default:
		warn("coap_read : not supported protocol %d\n", ctx->protocol);
		goto error_early;
	}
#endif /* WITH_POSIX */
#ifdef WITH_CONTIKI
	if (uip_newdata()) {
		uip_ipaddr_copy(&src.addr, &UIP_IP_BUF->srcipaddr);
		src.port = UIP_UDP_BUF->srcport;
		uip_ipaddr_copy(&dst.addr, &UIP_IP_BUF->destipaddr);
		dst.port = UIP_UDP_BUF->destport;

		bytes_read = uip_datalen();
		((char *)uip_appdata)[bytes_read] = 0;
		PRINTF("Server received %d bytes from [", (int)bytes_read);
		PRINT6ADDR(&src.addr);
		PRINTF("]:%d\n", uip_ntohs(src.port));
	}
#endif							/* WITH_CONTIKI */
#ifdef WITH_LWIP
	/* FIXME: use lwip address operation functions */
	src.addr.addr = ctx->pending_address.addr;
	src.port = ctx->pending_port;
	bytes_read = ctx->pending_package->tot_len;
#endif							/* WITH_LWIP */

	if (bytes_read < 0) {
		warn("coap_read: failed to read, ret %d, errno %d\n", bytes_read, errno);
		goto error_early;
	}

	switch (ctx->protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		/* the size of CoAP over UDP header is 4 Bytes */
		if ((size_t)bytes_read < COAP_UDP_HEADER) {
			warn("coap_read : discarded invalid UDP frame\n");
			goto error_early;
		}
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		/* the size of CoAP over TCP header is 2 Bytes */
		if ((size_t)bytes_read < COAP_TCP_HEADER_NO_FIELD) {
			//warn("coap_read : discarded invalid TCP frame\n");
			goto error_early;
		}
		break;
	default:
		break;
	}

	/* TCP doesn't have version field in PDU */
	if ((ctx->protocol == COAP_PROTO_UDP || ctx->protocol == COAP_PROTO_DTLS)
					&& pdu->version != COAP_DEFAULT_VERSION) {
		debug("coap_read: unknown protocol version\n");
		goto error_early;
	}

	node = coap_new_node();
	if (!node) {
		goto error_early;
	}
#ifdef WITH_LWIP
	node->pdu = coap_pdu_from_pbuf(ctx->pending_package);
	ctx->pending_package = NULL;
#endif

	coap_transport_t transport;

	switch (ctx->protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		node->pdu = coap_pdu_init(0, 0, 0, bytes_read);

		if (!node->pdu) {
			warn("coap_read : failed to coap_pdu_init2\n");
			goto error;
		}

		coap_ticks(&node->t);
		memcpy(&node->local, &dst, sizeof(coap_address_t));
		memcpy(&node->remote, &src, sizeof(coap_address_t));

		if (!coap_pdu_parse((unsigned char *)buf, bytes_read, node->pdu)) {
			warn("coap_read : discard malformed PDU");
			goto error;
		}
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)buf)[0] >> 4);
		debug("coap_read : received header type, transport %d, len %u, bytes_read %d\n", transport, ((buf[0] >> 4) & 0x0f), bytes_read);

		node->pdu = coap_pdu_init2(0, 0, 0, bytes_read, transport);
		if (!node->pdu) {
			warn("coap_read : failed to coap_pdu_init2, transport %d\n", transport);
			goto error;
		}

		coap_ticks(&node->t);
		memcpy(&node->local, &dst, sizeof(coap_address_t));
		memcpy(&node->remote, &src, sizeof(coap_address_t));

		if (!coap_pdu_parse2((unsigned char*)buf, bytes_read, node->pdu, transport)) {
			/* FIXME : prevent printing log when continuously received wrong PDU */
			//warn("coap_read : discard malformed TCP PDU\n");
			goto error;
		}
		node->transport = transport;
		debug("coap_read : Received %d type pdu\n", transport);
		break;
	default:
		break;
	}

	/* and add new node to receive queue */
	coap_transaction_id2(&node->remote, node->pdu, &node->id, ctx->protocol);
	coap_insert_node(&ctx->recvqueue, node);

#ifndef NDEBUG
	if (LOG_DEBUG <= coap_get_log_level()) {
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 40
#endif
		unsigned char addr[INET6_ADDRSTRLEN + 8];

		if (coap_print_addr(&src, addr, INET6_ADDRSTRLEN + 8)) {
			debug("** received %d bytes from %s:\n", (int)bytes_read, addr);
		}

		coap_show_pdu2(node->pdu, ctx->protocol);
	}
#endif

	return 0;

error:
	/* FIXME: send back RST? */
	coap_delete_node(node);
	return -1;
error_early:
#ifdef WITH_LWIP
	/* even if there was an error, clean up */
	pbuf_free(ctx->pending_package);
	ctx->pending_package = NULL;
#endif
	return -1;
}

int coap_remove_from_queue(coap_queue_t **queue, coap_tid_t id, coap_queue_t **node)
{
	coap_queue_t *p, *q;

	if (!queue || !*queue) {
		return 0;
	}

	/* replace queue head if PDU's time is less than head's time */

	if (id == (*queue)->id) {	/* found transaction */
		*node = *queue;
		*queue = (*queue)->next;
		if (*queue) {			/* adjust relative time of new queue head */
			(*queue)->t += (*node)->t;
		}
		(*node)->next = NULL;
		/* coap_delete_node( q ); */
		debug("*** removed transaction %u\n", id);
		return 1;
	}

	/* search transaction to remove (only first occurence will be removed) */
	q = *queue;
	do {
		p = q;
		q = q->next;
	} while (q && id != q->id);

	if (q) {					/* found transaction */
		p->next = q->next;
		if (p->next) {			/* must update relative time of p->next */
			p->next->t += q->t;
		}
		q->next = NULL;
		*node = q;
		/* coap_delete_node( q ); */
		debug("*** removed transaction %u\n", id);
		return 1;
	}

	return 0;

}

static inline int token_match(const unsigned char *a, size_t alen, const unsigned char *b, size_t blen)
{
	return alen == blen && (alen == 0 || memcmp(a, b, alen) == 0);
}

void coap_cancel_all_messages(coap_context_t *context, const coap_address_t *dst, const unsigned char *token, size_t token_length)
{
	/* cancel all messages in sendqueue that are for dst
	 * and use the specified token */
	coap_queue_t *p, *q;

	debug("cancel_all_messages\n");
	/* TODO : Considering TCP Case */
	while (context->sendqueue && coap_address_equals(dst, &context->sendqueue->remote)
		   && token_match(token, token_length, context->sendqueue->pdu->transport_hdr->udp.token, context->sendqueue->pdu->transport_hdr->udp.token_length)) {
		q = context->sendqueue;
		context->sendqueue = q->next;
		debug("**** removed transaction %d\n", ntohs(q->pdu->transport_hdr->udp.id));
		coap_delete_node(q);
	}

	if (!context->sendqueue) {
		return;
	}

	p = context->sendqueue;
	q = p->next;

	/* when q is not NULL, it does not match (dst, token), so we can skip it */
	while (q) {
		/* TODO : Considering TCP Case */
		if (coap_address_equals(dst, &q->remote)
			&& token_match(token, token_length, q->pdu->transport_hdr->udp.token, q->pdu->transport_hdr->udp.token_length)) {
			p->next = q->next;
			debug("**** removed transaction %d\n", ntohs(q->pdu->transport_hdr->udp.id));
			coap_delete_node(q);
			q = p->next;
		} else {
			p = q;
			q = q->next;
		}
	}
}

coap_queue_t *coap_find_transaction(coap_queue_t *queue, coap_tid_t id)
{
	while (queue && queue->id != id) {
		queue = queue->next;
	}

	return queue;
}
coap_pdu_t *coap_new_error_response2(coap_pdu_t *request, unsigned char code, coap_opt_filter_t opts, coap_protocol_t protocol)
{
	coap_opt_iterator_t opt_iter;
	coap_pdu_t *response = NULL;

	size_t size = 0;
	int type = 0;
	coap_opt_t *option;
	unsigned short opt_type = 0;	/* used for calculating delta-storage */

	coap_transport_t transport = COAP_UDP;
	size_t tokenLen = 0;
	unsigned char tokenStr[8] = {0,};
	unsigned char *token_ptr = NULL;

#if COAP_ERROR_PHRASE_LENGTH > 0
	char *phrase = NULL;
#endif

	assert(request);

	if (protocol == COAP_PROTO_TCP || protocol == COAP_PROTO_TLS) {
		transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)request->transport_hdr)[0] >> 4);
	} else {
		transport = COAP_UDP;
		/* cannot send ACK if original request was not confirmable */
		type = request->transport_hdr->udp.type == COAP_MESSAGE_CON ? COAP_MESSAGE_ACK : COAP_MESSAGE_NON;
	}

	coap_get_token2(request->transport_hdr, transport, &token_ptr, &tokenLen);
	if (tokenLen > 0) {
		memcpy((char *)tokenStr, (char *)token_ptr, tokenLen);
	} else
		tokenLen = 0;

	size = tokenLen;

#if COAP_ERROR_PHRASE_LENGTH > 0
	phrase = coap_response_phrase(code);

	/* Need some more space for the error phrase and payload start marker */
	if (phrase) {
		size += strlen(phrase) + 1;
	}
#endif

	/* Estimate how much space we need for options to copy from
	 * request. We always need the Token, for 4.02 the unknown critical
	 * options must be included as well. */
	coap_option_clrb(opts, COAP_OPTION_CONTENT_TYPE);	/* we do not want this */

	coap_option_iterator_init2(request, &opt_iter, opts, transport);

	/* Add size of each unknown critical option. As known critical
	   options as well as elective options are not copied, the delta
	   value might grow.
	 */
	while ((option = coap_option_next(&opt_iter))) {
		unsigned short delta = opt_iter.type - opt_type;
		/* calculate space required to encode (opt_iter.type - opt_type) */
		if (delta < 13) {
			size++;
		} else if (delta < 269) {
			size += 2;
		} else {
			size += 3;
		}

		/* add coap_opt_length(option) and the number of additional bytes
		 * required to encode the option length */

		size += coap_opt_length(option);
		switch (*option & 0x0f) {
		case 0x0e:
			size++;
			/* fall through */
		case 0x0d:
			size++;
			break;
		default:
			;
		}

		opt_type = opt_iter.type;
	}

	/* Now create the response and fill with options and payload data. */
	if (protocol == COAP_PROTO_UDP || protocol == COAP_PROTO_DTLS) {
		response = coap_pdu_init(type, code, request->transport_hdr->udp.id, COAP_MAX_PDU_SIZE);
	} else if (protocol == COAP_PROTO_TCP || protocol == COAP_PROTO_TLS) {
		response = coap_pdu_init(type, code, 0, COAP_MAX_PDU_SIZE);
	} else {
		/* Should not enter here */
		response = NULL;
		return response;
	}

	if (response) {
		/* copy token */
		if (!coap_add_token(response, tokenLen, tokenStr)) {
			debug("cannot add token to error response\n");
			coap_delete_pdu(response);
			return NULL;
		}

		/* copy all options */
		coap_option_iterator_init2(request, &opt_iter, opts, transport);
		while ((option = coap_option_next(&opt_iter))) {
			coap_add_option(response, opt_iter.type, COAP_OPT_LENGTH(option), COAP_OPT_VALUE(option));
		}

#if COAP_ERROR_PHRASE_LENGTH > 0
		/* note that diagnostic messages do not need a Content-Format option. */
		if (phrase) {
			debug("coap_new_error_response2 : phrase %s\n", phrase);
			coap_add_data(response, strlen(phrase), (unsigned char *)phrase);
		}
#endif
	}

	if (protocol == COAP_PROTO_TCP || protocol == COAP_PROTO_TLS) {
		coap_pdu_t *tcp_response = NULL;
		tcp_response = coap_convert_to_tcp_pdu(response);
		if (tcp_response == NULL) {
			warn("coap_new_error_response2 : failed to convert to TCP pdu\n");
		} else {
			debug("coap_new_error_response2 : succeed to convert to TCP pdu\n");
		}
		return tcp_response;
	} else {
		return response;
	}
}

coap_pdu_t *coap_new_error_response(coap_pdu_t *request, unsigned char code, coap_opt_filter_t opts)
{
	return coap_new_error_response2(request, code, opts, COAP_PROTO_UDP);
}
/**
 * Quick hack to determine the size of the resource description for
 * .well-known/core.
 */
static inline size_t get_wkc_len(coap_context_t *context, coap_opt_t *query_filter)
{
	unsigned char buf[1];
	size_t len = 0;

	if (print_wellknown(context, buf, &len, UINT_MAX, query_filter) & COAP_PRINT_STATUS_ERROR) {
		warn("cannot determine length of /.well-known/core\n");
		return 0;
	}

	debug("get_wkc_len: print_wellknown() returned %zu\n", len);

	return len;
}

#define SZX_TO_BYTES(SZX) ((size_t)(1 << ((SZX) + 4)))

coap_pdu_t *wellknown_response(coap_context_t *context, coap_pdu_t *request)
{
	coap_pdu_t *resp;
	coap_opt_iterator_t opt_iter;
	size_t len, wkc_len;
	unsigned char buf[2];
	int result = 0;
	int need_block2 = 0;		/* set to 1 if Block2 option is required */
	coap_block_t block;
	coap_opt_t *query_filter;
	size_t offset = 0;

	coap_pdu_t *tcp_resp = NULL; /* used for TCP pdu */

	coap_transport_t transport = COAP_UDP;

	unsigned char msgType = COAP_MESSAGE_NON;
	unsigned short msgId = 0;

	size_t tokenlen = 0;
	unsigned char *token_ptr = NULL; /* Token pointer for request */

	if (context->protocol == COAP_PROTO_TCP || context->protocol == COAP_PROTO_TLS) {
		transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)request->transport_hdr)[0] >> 4);
	} else if (context->protocol == COAP_PROTO_UDP || context->protocol == COAP_PROTO_DTLS) {
		transport = COAP_UDP;
		if (request->transport_hdr->udp.type == COAP_MESSAGE_CON) {
			msgType = COAP_MESSAGE_ACK;
		} else {
			msgType = COAP_MESSAGE_NON;
		}
		msgId = request->transport_hdr->udp.id;
	} else {
		/* Should not enter here */
	}

	coap_get_token2(request->transport_hdr, transport, &token_ptr, &tokenlen);

	if (tokenlen > 0) {
		debug("wellknown_response : token %s, tokenlen %d\n", token_ptr, tokenlen);
	}

	resp = coap_pdu_init(msgType, COAP_RESPONSE_CODE(205), msgId, COAP_MAX_PDU_SIZE);
	if (!resp) {
		debug("wellknown_response: cannot create PDU\n");
		return NULL;
	}

	if (!coap_add_token(resp, tokenlen, token_ptr)) {
		debug("wellknown_response: cannot add token\n");
		goto error;
	}

	query_filter = coap_check_option2(request, COAP_OPTION_URI_QUERY, &opt_iter, transport);
	wkc_len = get_wkc_len(context, query_filter);

	/* check whether the request contains the Block2 option */
	if (coap_get_block2(request, COAP_OPTION_BLOCK2, &block, transport)) {
		offset = block.num << (block.szx + 4);
		if (block.szx > 6) {
			/* invalid, MUST lead to 4.00 Bad Request */
			resp->transport_hdr->udp.code = COAP_RESPONSE_CODE(400);
			return resp;
		} else if (block.szx > COAP_MAX_BLOCK_SZX) {
			block.szx = COAP_MAX_BLOCK_SZX;
			block.num = offset >> (block.szx + 4);
		}

		need_block2 = 1;
	}

	/* Check if there is sufficient space to add Content-Format option
	 * and data. We do this before adding the Content-Format option to
	 * avoid sending error responses with that option but no actual
	 * content. */
	if (resp->max_size <= (size_t) resp->length + 3) {
		debug("wellknown_response: insufficient storage space\n");
		goto error;
	}

	/* Add Content-Format. As we have checked for available storage,
	 * nothing should go wrong here. */
	assert(coap_encode_var_bytes(buf, COAP_MEDIATYPE_APPLICATION_LINK_FORMAT) == 1);
	coap_add_option(resp, COAP_OPTION_CONTENT_FORMAT,
		coap_encode_var_bytes(buf, COAP_MEDIATYPE_APPLICATION_LINK_FORMAT), buf);

	/* check if Block2 option is required even if not requested */
	if (!need_block2 && (resp->max_size - (size_t) resp->length < wkc_len)) {
		assert(resp->length <= resp->max_size);
		const size_t payloadlen = resp->max_size - resp->length;
		/* yes, need block-wise transfer */
		block.num = 0;
		block.m = 0;			/* the M bit is set by coap_write_block_opt() */
		block.szx = COAP_MAX_BLOCK_SZX;
		while (payloadlen < SZX_TO_BYTES(block.szx)) {
			if (block.szx == 0) {
				debug("wellknown_response: message to small even for szx == 0\n");
				goto error;
			} else {
				block.szx--;
			}
		}

		need_block2 = 1;
	}

	/* write Block2 option if necessary */
	if (need_block2) {
		if (coap_write_block_opt(&block, COAP_OPTION_BLOCK2, resp, wkc_len) < 0) {
			debug("wellknown_response: cannot add Block2 option\n");
			goto error;
		}
	}

	/* Manually set payload of response to let print_wellknown() write,
	 * into our buffer without copying data. */

	resp->data = (unsigned char *)resp->hdr + resp->length;
	*resp->data = COAP_PAYLOAD_START;
	resp->data++;
	resp->length++;
	len = need_block2 ? SZX_TO_BYTES(block.szx) : resp->max_size - resp->length;

	result = print_wellknown(context, resp->data, &len, offset, query_filter);
	if ((result & COAP_PRINT_STATUS_ERROR) != 0) {
		debug("wellknown_response : print_wellknown failed\n");
		goto error;
	}

	resp->length += COAP_PRINT_OUTPUT_LENGTH(result);

	if (context->protocol == COAP_PROTO_TCP || context->protocol == COAP_PROTO_TLS) {

		tcp_resp = coap_convert_to_tcp_pdu(resp);

		if (tcp_resp == NULL) {
			warn("wellknown_response : failed to convert to TCP pdu\n");
		} else {
			debug("wellknown_response : convert to new TCP pdu\n");
		}

		return tcp_resp;
	} else {

		debug("wellknown_response : create well-known response\n");
		return resp;
	}

error:
	/* set error code 5.03 and remove all options and data from response */
	/* TODO : Considering TCP Case */
	resp->transport_hdr->udp.code = COAP_RESPONSE_CODE(503);
	resp->length = sizeof(coap_hdr_t) + resp->transport_hdr->udp.token_length;

	if (context->protocol == COAP_PROTO_TCP || context->protocol == COAP_PROTO_TLS) {
		tcp_resp = coap_convert_to_tcp_pdu(resp);
		if (tcp_resp == NULL) {
			warn("wellknown_response : failed to convert to TCP pdu\n");
		} else {
			debug("wellknown_response : convert to new TCP pdu\n");
		}

	}
	return resp;
}

/* TODO : Considering TCP Case */
#define WANT_WKC(Pdu,Key)                   \
	(((Pdu)->transport_hdr->udp.code == COAP_REQUEST_GET) && is_wkc(Key))

void handle_request(coap_context_t *context, coap_queue_t *node)
{
	coap_method_handler_t h = NULL;
	coap_pdu_t *response = NULL;
	coap_opt_filter_t opt_filter;
	coap_resource_t *resource;
	coap_key_t key;

	unsigned int code = 0;
	coap_transport_t transport = 0;

	coap_pdu_t *tcp_response = NULL;

	unsigned char tokenStr[8] = {0,};
	unsigned char *token_ptr = NULL;
	size_t tokenLen = 0;

	coap_option_filter_clear(opt_filter);

	/* try to find the resource from the request URI */
	transport = node->transport;
	coap_hash_request_uri2(node->pdu, key, transport);
	resource = coap_get_resource_from_key(context, key);

	code = coap_get_code(node->pdu, transport);

	debug("handle_request : code %d transport %d, resource %p\n", code, transport, resource);

	if (!resource) {
		/* The resource was not found. Check if the request URI happens to
		 * be the well-known URI. In that case, we generate a default
		 * response, otherwise, we return 4.04 */

		switch (code) {

		case COAP_REQUEST_GET:
			if (is_wkc(key)) {
				/* GET request for .well-known/core */
				info("handle_request : create default response for %s\n", COAP_DEFAULT_URI_WELLKNOWN);

				response = wellknown_response(context, node->pdu);

			} else {
				/* GET request for any another resource, return 4.04 */

				debug("handle_request : GET for unknown resource 0x%02x%02x%02x%02x, return 4.04\n", key[0], key[1], key[2], key[3]);
				response = coap_new_error_response2(node->pdu, COAP_RESPONSE_CODE(404), opt_filter, context->protocol);
			}
			break;

		default:				/* any other request type */

			debug("handle_request : unhandled request for unknown resource 0x%02x%02x%02x%02x\r\n", key[0], key[1], key[2], key[3]);
			if (!coap_is_mcast(&node->local)) {
				response = coap_new_error_response2(node->pdu, COAP_RESPONSE_CODE(405), opt_filter, context->protocol);
			}
		}

		if (response && coap_send(context, &node->remote, response) == COAP_INVALID_TID) {
			warn("handle_request : cannot send response for transaction %u\n", node->id);
		}
		coap_delete_pdu(response);

		return;
	}

	/* the resource was found, check if there is a registered handler */
	if ((size_t)(code - 1) < sizeof(resource->handler) / sizeof(coap_method_handler_t)) {
		h = resource->handler[code - 1];
	}

	if (h) {
		debug("handle_request : call custom handler for resource 0x%02x%02x%02x%02x\n", key[0], key[1], key[2], key[3]);
		switch (context->protocol) {
		case COAP_PROTO_UDP:
		case COAP_PROTO_DTLS:
			response = coap_pdu_init(node->pdu->transport_hdr->udp.type == COAP_MESSAGE_CON ? COAP_MESSAGE_ACK : COAP_MESSAGE_NON, 0, node->pdu->transport_hdr->udp.id, COAP_MAX_PDU_SIZE);
			break;
		default:
			response = coap_pdu_init(COAP_MESSAGE_NON, 0, 0, COAP_MAX_PDU_SIZE);
			break;
		}

		coap_get_token2(node->pdu->transport_hdr, transport, &token_ptr, &tokenLen);
		if (tokenLen > 0) {
			memcpy((char *)tokenStr, (char *)token_ptr, tokenLen);
		}

		/* Implementation detail: coap_add_token() immediately returns 0
		   if response == NULL */
		if (coap_add_token(response, tokenLen, tokenStr)) {
			str token = { tokenLen, tokenStr };

			h(context, resource, &node->remote, node->pdu, &token, response);

			switch(context->protocol) {
			case COAP_PROTO_UDP:
			case COAP_PROTO_DTLS:
				if (response->transport_hdr->udp.type != COAP_MESSAGE_NON
					|| (response->transport_hdr->udp.code >= 64 && !coap_is_mcast(&node->local))) {

					if (coap_send(context, &node->remote, response) == COAP_INVALID_TID) {
						debug("handle_request : cannot send response for message %d\n", node->pdu->transport_hdr->udp.id);
					}
				}
				coap_delete_pdu(response);
				break;
			case COAP_PROTO_TCP:
			case COAP_PROTO_TLS:
				tcp_response = coap_convert_to_tcp_pdu(response);
				if (tcp_response == NULL) {
					warn("handle_request : failed to convert TCP pdu\n");
				} else {
					debug("handle_request : succeed to convert TCP pdu\n");
				}

				code = coap_get_code(tcp_response,
						coap_get_tcp_header_type_from_initbyte(((unsigned char *)tcp_response->transport_hdr)[0] >> 4));
				if (code >= 64 && !coap_is_mcast(&node->local)) {
					if (coap_send(context, &node->remote, tcp_response) == COAP_INVALID_TID) {
						debug("handle_request : cannot send TCP reponse\n");
					}
				}
				coap_delete_pdu(tcp_response);
				break;
			default:
				warn("handle_request : error! un-known protocol %d\n", context->protocol);
				break;

			}
		} else {
			warn("handle_request : cannot generate response\r\n");
		}
	} else {
		if (WANT_WKC(node->pdu, key)) {
			debug("handle_request : create default response for %s\n", COAP_DEFAULT_URI_WELLKNOWN);
			response = wellknown_response(context, node->pdu);
		} else {
			response = coap_new_error_response2(node->pdu, COAP_RESPONSE_CODE(405), opt_filter, context->protocol);
		}

		if (!response || (coap_send(context, &node->remote, response) == COAP_INVALID_TID)) {
			debug("handle_request : cannot send response for transaction %u\n", node->id);
		}
		coap_delete_pdu(response);
	}
}

static inline void handle_response(coap_context_t *context, coap_queue_t *sent, coap_queue_t *rcvd)
{

	/* Call application-specific reponse handler when available.  If
	 * not, we must acknowledge confirmable messages. */
	if (context->response_handler) {
		context->response_handler(context, &rcvd->remote, sent ? sent->pdu : NULL, rcvd->pdu, rcvd->id);
	} else {
		/* send ACK if rcvd is confirmable (i.e. a separate response) */
		if (context->protocol == COAP_PROTO_UDP || context->protocol == COAP_PROTO_DTLS) {
			coap_send_ack(context, &rcvd->remote, rcvd->pdu);
		} else {
			debug("handle_response : do nothing\n");
			/* TODO: CoAP over TCP doesn't have ACK message */
		}
	}
}

static inline int
#ifdef __GNUC__
handle_locally(coap_context_t *context __attribute__((unused)), coap_queue_t *node __attribute__((unused)))
{
#else							/* not a GCC */
handle_locally(coap_context_t *context, coap_queue_t *node)
{
#endif							/* GCC */
	/* this function can be used to check if node->pdu is really for us */
	return 1;
}

/**
 * This function handles RST messages received for the message passed
 * in @p sent.
 */
static void coap_handle_rst(coap_context_t *context, const coap_queue_t *sent)
{
#ifndef WITHOUT_OBSERVE
	coap_resource_t *r;
#ifndef COAP_RESOURCES_NOHASH
	coap_resource_t *tmp;
#endif
	str token = { 0, NULL };

	/* remove observer for this resource, if any
	 * get token from sent and try to find a matching resource. Uh!
	 */
	/* TODO : Considering TCP Case */
	COAP_SET_STR(&token, sent->pdu->transport_hdr->udp.token_length, sent->pdu->transport_hdr->udp.token);

#ifndef WITH_CONTIKI
#ifdef COAP_RESOURCES_NOHASH
	LL_FOREACH(context->resources, r) {
#else
	HASH_ITER(hh, context->resources, r, tmp) {
#endif
		coap_delete_observer(r, &sent->remote, &token);
		coap_cancel_all_messages(context, &sent->remote, token.s, token.length);
	}
#else							/* WITH_CONTIKI */
	r = (coap_resource_t *) resource_storage.mem;
	for (i = 0; i < resource_storage.num; ++i, ++r) {
		if (resource_storage.count[i]) {
			coap_delete_observer(r, &sent->remote, &token);
			coap_cancel_all_messages(context, &sent->remote, token.s, token.length);
		}
	}
#endif							/* WITH_CONTIKI */
#endif							/* WITOUT_OBSERVE */
}

void coap_dispatch(coap_context_t *context)
{
	coap_queue_t *rcvd = NULL, *sent = NULL;
	coap_pdu_t *response;
	coap_opt_filter_t opt_filter;

	coap_transport_t transport;
	unsigned int code;

	if (!context) {
		return;
	}

	memset(opt_filter, 0, sizeof(coap_opt_filter_t));

	while (context->recvqueue) {
		rcvd = context->recvqueue;

		/* remove node from recvqueue */
		context->recvqueue = context->recvqueue->next;
		rcvd->next = NULL;

		if (context->protocol == COAP_PROTO_TCP || context->protocol == COAP_PROTO_TLS) {
			/* CoAP over TCP pdu will be handled as NON */
			transport = rcvd->transport;

			/* find transaction in sendqueue to stop retransmission */
			if (coap_option_check_critical2(context, rcvd->pdu, opt_filter, transport) == 0) {
				goto cleanup;
			}

			if (transport == COAP_UDP) {
				warn("coap_dispatch : wrong header (something wrong..)\n");
				goto cleanup;
			}

			code = coap_get_code(rcvd->pdu, transport);

			debug("coap_dispatch : code %d, transport %d\n", code, transport);

			if (code > 0 && code < 32 ) {
				/* REQUEST */
				handle_request(context, rcvd);
			} else if (code >= 64 && code <= 191) {
				/* RESPONSE */
				handle_response(context, sent, rcvd);
			} else {
				warn("coap_dispatch : dropped message with invalied code %d", code);
				goto cleanup;
			}
		} else if (context->protocol == COAP_PROTO_UDP || context->protocol == COAP_PROTO_DTLS) {
			if (rcvd->pdu->transport_hdr->udp.version != COAP_DEFAULT_VERSION) {
				debug("dropped packet with unknown version %u\n",
							rcvd->pdu->transport_hdr->udp.version);
				goto cleanup;
			}

			switch (rcvd->pdu->transport_hdr->udp.type) {
			case COAP_MESSAGE_ACK:
				/* find transaction in sendqueue to stop retransmission */
				coap_remove_from_queue(&context->sendqueue, rcvd->id, &sent);

				if (rcvd->pdu->transport_hdr->udp.code == 0) {
					goto cleanup;
				}

				/* FIXME: if sent code was >= 64 the message might have been a
			 	 * notification. Then, we must flag the observer to be alive
			 	 * by setting obs->fail_cnt = 0. */
				if (sent && COAP_RESPONSE_CLASS(sent->pdu->transport_hdr->udp.code) == 2) {
					 const str token = {sent->pdu->transport_hdr->udp.token_length,
										sent->pdu->transport_hdr->udp.token
									   };

					coap_touch_observer(context, &sent->remote, &token);
				}
				break;

			case COAP_MESSAGE_RST:
				/* We have sent something the receiver disliked, so we remove
			 	 * not only the transaction but also the subscriptions we might
			 	 * have. */

				coap_log(LOG_ALERT, "got RST for message %u\n", ntohs(rcvd->pdu->transport_hdr->udp.id));

				/* find transaction in sendqueue to stop retransmission */
				coap_remove_from_queue(&context->sendqueue, rcvd->id, &sent);

				if (sent) {
					coap_handle_rst(context, sent);
				}
				goto cleanup;

			case COAP_MESSAGE_NON:	/* check for unknown critical options */
				if (coap_option_check_critical(context, rcvd->pdu, opt_filter) == 0) {
					goto cleanup;
				}
				break;

			case COAP_MESSAGE_CON:	/* check for unknown critical options */
				if (coap_option_check_critical(context, rcvd->pdu, opt_filter) == 0) {

					/* FIXME: send response only if we have received a request. Otherwise,
				 	 * send RST. */
					response = coap_new_error_response(rcvd->pdu, COAP_RESPONSE_CODE(402), opt_filter);

					if (!response) {
						warn("coap_dispatch: cannot create error reponse\n");
					} else {
						if (coap_send(context, &rcvd->remote, response) == COAP_INVALID_TID) {
							warn("coap_dispatch: error sending reponse\n");
						}
						coap_delete_pdu(response);
					}

					goto cleanup;
				}
				break;
			}

			/* Pass message to upper layer if a specific handler was
		 	 * registered for a request that should be handled locally. */
			if (handle_locally(context, rcvd)) {
				if (COAP_MESSAGE_IS_REQUEST(rcvd->pdu->transport_hdr->udp)) {
					handle_request(context, rcvd);
				} else if (COAP_MESSAGE_IS_RESPONSE(rcvd->pdu->transport_hdr->udp)) {
					handle_response(context, sent, rcvd);
				} else {
					debug("dropped message with invalid code\n");
					coap_send_message_type(context, &rcvd->remote, rcvd->pdu, COAP_MESSAGE_RST);
				}
			}
		} else {
			warn("coap_dispatch : error! un-known protocol type\n");
			goto cleanup;
		}

cleanup:
		coap_delete_node(sent);
		coap_delete_node(rcvd);
	}
}

int coap_can_exit(coap_context_t *context)
{
	return !context || (context->recvqueue == NULL && context->sendqueue == NULL);
}

#ifdef WITH_CONTIKI

/*---------------------------------------------------------------------------*/
/* CoAP message retransmission */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(coap_retransmit_process, ev, data)
{
	coap_tick_t now;
	coap_queue_t *nextpdu;

	PROCESS_BEGIN();

	debug("Started retransmit process\r\n");

	while (1) {
		PROCESS_YIELD();
		if (ev == PROCESS_EVENT_TIMER) {
			if (etimer_expired(&the_coap_context.retransmit_timer)) {

				nextpdu = coap_peek_next(&the_coap_context);

				coap_ticks(&now);
				while (nextpdu && nextpdu->t <= now) {
					coap_retransmit(&the_coap_context, coap_pop_next(&the_coap_context));
					nextpdu = coap_peek_next(&the_coap_context);
				}

				/* need to set timer to some value even if no nextpdu is available */
				etimer_set(&the_coap_context.retransmit_timer, nextpdu ? nextpdu->t - now : 0xFFFF);
			}
#ifndef WITHOUT_OBSERVE
			if (etimer_expired(&the_coap_context.notify_timer)) {
				coap_check_notify(&the_coap_context);
				etimer_reset(&the_coap_context.notify_timer);
			}
#endif							/* WITHOUT_OBSERVE */
		}
	}

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/

#endif							/* WITH_CONTIKI */

#ifdef WITH_LWIP
/* FIXME: retransmits that are not required any more due to incoming packages
 * do *not* get cleared at the moment, the wakeup when the transmission is due
 * is silently accepted. this is mainly due to the fact that the required
 * checks are similar in two places in the code (when receiving ACK and RST)
 * and that they cause more than one patch chunk, as it must be first checked
 * whether the sendqueue item to be dropped is the next one pending, and later
 * the restart function has to be called. nothing insurmountable, but it can
 * also be implemented when things have stabilized, and the performance
 * penality is minimal
 *
 * also, this completely ignores COAP_RESOURCE_CHECK_TIME.
 * */

static void coap_retransmittimer_execute(void *arg)
{
	coap_context_t *ctx = (coap_context_t *) arg;
	coap_tick_t now;
	coap_tick_t elapsed;
	coap_queue_t *nextinqueue;

	ctx->timer_configured = 0;

	coap_ticks(&now);

	elapsed = now - ctx->sendqueue_basetime;	/* that's positive for sure, and unless we haven't been called for a complete wrapping cycle, did not wrap */

	nextinqueue = coap_peek_next(ctx);
	while (nextinqueue != NULL) {
		if (nextinqueue->t > elapsed) {
			nextinqueue->t -= elapsed;
			break;
		} else {
			elapsed -= nextinqueue->t;
			coap_retransmit(ctx, coap_pop_next(ctx));
			nextinqueue = coap_peek_next(ctx);
		}
	}

	ctx->sendqueue_basetime = now;

	coap_retransmittimer_restart(ctx);
}

static void coap_retransmittimer_restart(coap_context_t *ctx)
{
	coap_tick_t now, elapsed, delay;

	if (ctx->timer_configured) {
		debug("coap_retransmittimer_restart : clearing\n");
		sys_untimeout(coap_retransmittimer_execute, (void *)ctx);
		ctx->timer_configured = 0;
	}
	if (ctx->sendqueue != NULL) {
		coap_ticks(&now);
		elapsed = now - ctx->sendqueue_basetime;
		if (ctx->sendqueue->t >= elapsed) {
			delay = ctx->sendqueue->t - elapsed;
		} else {
			/* a strange situation, but not completely impossible.
			 *
			 * this happens, for example, right after
			 * coap_retransmittimer_execute, when a retransmission
			 * was *just not yet* due, and the clock ticked before
			 * our coap_ticks was called.
			 *
			 * not trying to retransmit anything now, as it might
			 * cause uncontrollable recursion; let's just try again
			 * with the next main loop run.
			 * */
			delay = 0;
		}

		debug("coap_retransmittimer_restart : scheduling for %d ticks\n", delay);
		sys_timeout(delay, coap_retransmittimer_execute, (void *)ctx);
		ctx->timer_configured = 1;
	}
}
#endif


/** Representation of CoAP Prefix of URI.
 *  It can be used to get transport protocol type from URI through
 *  coap_get_protocol_from_uri function.
 */
#define COAP_MAX_URI_PREFIX_LEN 32
static char coap_uri_prefix[COAP_PROTO_MAX][COAP_MAX_URI_PREFIX_LEN] = {
	"coap://",      /**< URI prefix of CoAP over UDP  */
	"coaps://",     /**< URI prefix of CoAP over DTLS */
	"coap+tcp://",  /**< URI prefix of CoAP over TCP  */
	"coaps+tcp://", /**< URI prefix of CoAP over TLS  */
};

coap_protocol_t coap_get_protocol_from_uri(const char *uri)
{
	coap_protocol_t protocol;

	for (protocol = COAP_PROTO_UDP; protocol < COAP_PROTO_MAX; protocol++) {
		if (!strncmp(uri, coap_uri_prefix[protocol], strlen(coap_uri_prefix[protocol]))) {
			return protocol;
		}
	}

	return protocol;
}

int coap_net_bind(coap_context_t *ctx, const char *host, const char *port, void *tls_context, void *tls_option)
{
	int n;
	int newsock = -1;

	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *p;

	struct sockaddr caddr;
	socklen_t caddrlen;

	struct timeval timeout = {10, 0}; /* Timeout for TCP accept */

#ifdef WITH_MBEDTLS
	tls_session *session = NULL;
#endif

	ASSERT(ctx);
#ifdef WITH_TCP
	ctx->listen_sockfd = -1;
#endif

	memset(&hints, 0, sizeof(hints));

	switch (ctx->protocol) {
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		break;
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		break;
	default:
		/* Should not enter here */
		break;
	}

	hints.ai_family = AF_UNSPEC;
	if (host == NULL) {
		hints.ai_flags = AI_PASSIVE;
	}
	hints.ai_next  = NULL;

	if (0 != getaddrinfo(host, port, &hints, &res)) {
		return ctx->sockfd;
	}

	for (p = res; p; p = p->ai_next) {
		ctx->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (ctx->sockfd < 0) {
			continue;
		} else {
			n = 1;
			if (setsockopt(ctx->sockfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n)) < 0) {
				close(ctx->sockfd);
				ctx->sockfd = -1;
				continue;
			}

			if (bind(ctx->sockfd, p->ai_addr, p->ai_addrlen) < 0) {
				close(ctx->sockfd);
				ctx->sockfd = -1;
				continue;
			}

			switch (ctx->protocol) {
			case COAP_PROTO_TCP:
				if (listen(ctx->sockfd, TCP_LISTEN_BACKLOG) < 0) {
					printf("coap_net_bind : failed to listen, errno %d\n", errno);
					close(ctx->sockfd);
					ctx->sockfd = -1;
					break;
				}

				if (setsockopt(ctx->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0) {
					printf("coap_net_bind : failed to set socket option, errno %d\n", errno);
				}

				newsock = accept(ctx->sockfd, &caddr, &caddrlen);

				if (newsock < 0) {
					printf("coap_net_bind : failed to accept, errno %d\n", errno);
					close(ctx->sockfd);
					ctx->sockfd = -1;
				} else {
#ifdef WITH_TCP
					/* Copy to sock to listen sock */
					ctx->listen_sockfd = ctx->sockfd;
#endif
					ctx->sockfd = newsock;
					goto exit;
				}
				break;
#ifdef WITH_MBEDTLS
			case COAP_PROTO_TLS:
				if (listen(ctx->sockfd, TCP_LISTEN_BACKLOG) < 0) {
					printf("coap_net_bind : failed to listen, errno %d\n", errno);
					close(ctx->sockfd);
					ctx->sockfd = -1;
					break;
				}
			case COAP_PROTO_DTLS:
				session = TLSSession(ctx->sockfd, (tls_ctx *)tls_context, (tls_opt *)tls_option);
				if (session == NULL) {
					printf("coap_net_bind : failed to create TLS session\n");
					close(ctx->sockfd);
					ctx->sockfd = -1;
				} else {
#ifdef WITH_TCP
					/* Copy to sock to listen sock */
					ctx->listen_sockfd = ctx->sockfd;
#endif
					ctx->sockfd = session->net.fd;
					ctx->session = session;
					goto exit;
				}
				break;
#endif
			case COAP_PROTO_UDP:
				/* Do nothing */
				goto exit;
			default:
				/* Should not enter here */
				break;
			}
		}
	}

exit:
	freeaddrinfo(res);
	return ctx->sockfd;
}

int coap_net_connect(coap_context_t *ctx, const char *host, const char *port, void *tls_context, void *tls_option)
{
	struct addrinfo hints;
	struct addrinfo *servinfo = NULL;
	struct addrinfo *p = NULL;
	struct sockaddr *sa = NULL;
	socklen_t sl = 0;

	struct timeval tv = {10, 0};

	ASSERT(ctx);

	memset(&hints, 0, sizeof(hints));

	switch (ctx->protocol) {
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		break;
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		break;
	default:
		/* Should not enter here */
		break;
	}

	hints.ai_family = AF_UNSPEC;
	hints.ai_next  = NULL;

	if (0 != getaddrinfo(host, port, &hints, &servinfo)) {
		return ctx->sockfd;
	}

	for (p = servinfo; p; p = p->ai_next) {
		ctx->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (ctx->sockfd < 0) {
			continue;
		}

		sa = p->ai_addr;
#ifdef CONFIG_NET_LWIP
		sa->sa_len = p->ai_addrlen;
#endif
		sl = p->ai_addrlen;

		if (setsockopt(ctx->sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
			printf("coap_net_connect : setsockopt failed, errno %d\n", errno);
		}

		if (connect(ctx->sockfd, sa, sl) < 0) {
			printf("coap_net_connect : failed to connect, errno %d\n", errno);
			close(ctx->sockfd);
			ctx->sockfd = -1;
			continue;
		} else
			break;
	}

#ifdef WITH_MBEDTLS
	/* Create TLS Session : Start TLS Handshake */
	if (ctx->sockfd > 0 && (ctx->protocol == COAP_PROTO_TLS || ctx->protocol == COAP_PROTO_DTLS)) {
		ctx->session = TLSSession(ctx->sockfd, (tls_ctx *)tls_context, (tls_opt *)tls_option);
		if (ctx->session != NULL) {
			ctx->sockfd = ctx->session->net.fd;
		} else {
			printf("coap_net_connect : TLS handshake, failed to create secure session\n");
			close(ctx->sockfd);
			ctx->sockfd = -1;
		}
	}
#endif /* WITH_MBEDTLS */
	freeaddrinfo(servinfo);
	return ctx->sockfd;
}
