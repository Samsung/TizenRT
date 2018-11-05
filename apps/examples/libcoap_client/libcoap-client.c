/* coap-client -- simple CoAP client
 *
 * Copyright (C) 2010--2013 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include <tinyara/config.h>

#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <protocols/libcoap/coap.h>

#ifdef WITH_MBEDTLS
#define COAP_MBEDTLS_CIPHERSUIT "TLS-PSK-WITH-AES-128-CBC-SHA"
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif

#define COAP_STANDARD_PORT "5683"
#define COAP_SECURITY_PORT "5684"

#if defined(__TINYARA__)
#define COAP_CLIENT_SCHED_PRI    100
#define COAP_CLIENT_SCHED_POLICY SCHED_RR
#define COAP_CLIENT_STACK_SIZE   (1024 * 16)

/* used for sending input arguments to pthread */
struct coap_client_input {
	int argc;
	char **argv;
};
#endif /* __TINYARA__ */

int flags = 0;

static unsigned char _token_data[8];
str the_token = { 0, _token_data };

#define FLAGS_BLOCK 0x01

static coap_list_t *optlist = NULL;
/* Request URI.
 * TODO: associate the resources with transaction id and make it expireable */
static coap_uri_t uri;
static str proxy = { 0, NULL };

static unsigned short proxy_port = COAP_DEFAULT_PORT;

/* reading is done when this flag is set */
static int ready = 0;

static str output_file = { 0, NULL };	/* output file name */

static FILE *file = NULL;		/* output file stream */

static str payload = { 0, NULL };	/* optional payload to send */

unsigned char msgtype = COAP_MESSAGE_CON;	/* usually, requests are sent confirmable */

typedef unsigned char method_t;
method_t method = 1;			/* the method we are using in our requests */

coap_block_t block = {.num = 0, .m = 0, .szx = 6 };

unsigned int wait_seconds = 10;	/* default timeout in seconds */
coap_tick_t max_wait;			/* global timeout (changed by set_timeout()) */

unsigned int obs_seconds = 30;	/* default observe time */
coap_tick_t obs_wait = 0;		/* timeout for current subscription */

#define min(a, b) ((a) < (b) ? (a) : (b))

static inline void set_timeout(coap_tick_t *timer, const unsigned int seconds)
{
	coap_ticks(timer);
	*timer += seconds * COAP_TICKS_PER_SECOND;
}

int append_to_output(const unsigned char *data, size_t len)
{
	size_t written;

	if (!file) {
		if (!output_file.s || (output_file.length && output_file.s[0] == '-')) {
			file = stdout;
		} else {
			if (!(file = fopen((char *)output_file.s, "w"))) {
				perror("fopen");
				return -1;
			}
		}
	}

	do {
		written = fwrite(data, 1, len, file);
		len -= written;
		data += written;
	} while (written && len);
	fflush(file);

	return 0;
}

#if defined(__TINYARA__)
/*
 * separate close_output function
 * to prevent data abort when file descriptor is either stdout or stderr
 * data abort can be occur when fclose among stdin, stdout and stderr
 */
void close_output(void)
{
	if (file) {
		/* add a newline before closing in case were writing to stdout */
		if (!output_file.s || (output_file.length && output_file.s[0] == '-')) {
			fprintf(file, "\n");
		} else {
			fflush(file);
			fclose(file);
		}
	}
}
#else
void close_output(void)
{
	if (file) {

		/* add a newline before closing in case were writing to stdout */
		if (!output_file.s || (output_file.length && output_file.s[0] == '-')) {
			fwrite("\n", 1, 1, file);
		}

		fflush(file);
		fclose(file);
	}
}
#endif /* __TINYARA__ */

coap_pdu_t *new_ack(coap_context_t *ctx, coap_queue_t *node)
{
	coap_pdu_t *pdu = coap_new_pdu();

	if (pdu) {
		pdu->transport_hdr->udp.type = COAP_MESSAGE_ACK;
		pdu->transport_hdr->udp.code = 0;
		pdu->transport_hdr->udp.id = node->pdu->transport_hdr->udp.id;
	}

	return pdu;
}

coap_pdu_t *new_response(coap_context_t *ctx, coap_queue_t *node, unsigned int code)
{
	coap_pdu_t *pdu = new_ack(ctx, node);

	if (pdu) {
		pdu->transport_hdr->udp.code = code;
	}

	return pdu;
}

coap_pdu_t *coap_new_request(coap_context_t *ctx, method_t m, coap_list_t *options)
{
	coap_pdu_t *pdu;
	coap_list_t *opt;

	if (!(pdu = coap_new_pdu())) {
		return NULL;
	}

	pdu->transport_hdr->udp.type = msgtype;
	pdu->transport_hdr->udp.id = coap_new_message_id(ctx);
	pdu->transport_hdr->udp.code = m;

	pdu->transport_hdr->udp.token_length = the_token.length;
	if (!coap_add_token(pdu, the_token.length, the_token.s)) {
		debug("coap_new_request : cannot add token to request\n");
	}

	coap_show_pdu2(pdu, ctx->protocol);

	for (opt = options; opt; opt = opt->next) {
		coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *) opt->data), COAP_OPTION_LENGTH(*(coap_option *) opt->data), COAP_OPTION_DATA(*(coap_option *) opt->data));
	}

	if (payload.length) {
		if ((flags & FLAGS_BLOCK) == 0) {
			coap_add_data(pdu, payload.length, payload.s);
		} else {
			coap_add_block(pdu, payload.length, payload.s, block.num, block.szx);
		}
	}

	if (ctx->protocol == COAP_PROTO_TCP || ctx->protocol == COAP_PROTO_TLS) {
		coap_pdu_t *tcp_pdu = NULL;
		debug("\ncoap_new_request : tcp pdu created\n");
		tcp_pdu = coap_convert_to_tcp_pdu(pdu);
		return tcp_pdu;
	} else {
		return pdu;
	}
}

coap_tid_t clear_obs(coap_context_t *ctx, const coap_address_t *remote)
{
	coap_list_t *option;
	coap_pdu_t *pdu = NULL;
	coap_pdu_t *tcp_pdu = NULL;
	coap_tid_t tid = COAP_INVALID_TID;

	/* create bare PDU w/o any option  */
	/* pdu = coap_new_request(ctx, COAP_REQUEST_GET, NULL); */
	pdu = coap_pdu_init(msgtype, COAP_REQUEST_GET, coap_new_message_id(ctx), COAP_MAX_PDU_SIZE);

	if (pdu) {
		pdu->transport_hdr->udp.token_length = the_token.length;
		if (!coap_add_token(pdu, the_token.length, the_token.s)) {
			debug("clear_obs : cannot add token to pdu\n");
		}

		/* add URI components from optlist */
		for (option = optlist; option; option = option->next) {
			switch (COAP_OPTION_KEY(*(coap_option *) option->data)) {
			case COAP_OPTION_URI_HOST:
			case COAP_OPTION_URI_PORT:
			case COAP_OPTION_URI_PATH:
			case COAP_OPTION_URI_QUERY:
				coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *) option->data), COAP_OPTION_LENGTH(*(coap_option *) option->data), COAP_OPTION_DATA(*(coap_option *) option->data));
				break;
			default:
				break; /* skip other options */
			}
		}

		/* Add payload data to pdu */
		if (payload.length) {
			if ((flags & FLAGS_BLOCK) == 0) {
				coap_add_data(pdu, payload.length, payload.s);
			} else {
				coap_add_block(pdu, payload.length, payload.s, block.num, block.szx);
			}
		}

		switch (ctx->protocol) {
		case COAP_PROTO_UDP:
		case COAP_PROTO_DTLS:
			if (pdu->transport_hdr->udp.type == COAP_MESSAGE_CON) {
				tid = coap_send_confirmed(ctx, remote, pdu);
			} else {
				tid = coap_send(ctx, remote, pdu);
			}

			if (tid == COAP_INVALID_TID) {
				debug("clear_obs: error sending new request");
				coap_delete_pdu(pdu);
			} else if (pdu->transport_hdr->udp.type != COAP_MESSAGE_CON) {
				coap_delete_pdu(pdu);
			}
			break;
		case COAP_PROTO_TCP:
		case COAP_PROTO_TLS:
			tcp_pdu = coap_convert_to_tcp_pdu(pdu);
			if (tcp_pdu == NULL) {
				warn("clear_obs : error to create TCP pdu\n");
			} else {
				tid = coap_send(ctx, remote, tcp_pdu);
				if (tid == COAP_INVALID_TID) {
					debug("clear_obs : error sending new TCP request\n");
				}
				coap_delete_pdu(tcp_pdu);
			}
			break;
		default:
			break;
		}
	}
	return tid;
}

int resolve_address(const str *server, struct sockaddr *dst, coap_protocol_t protocol)
{
	struct addrinfo *res, *ainfo;
	struct addrinfo hints;
	static char addrstr[256];
	int error, len = -1;

	memset(addrstr, 0, sizeof(addrstr));
	if (server->length) {
		memcpy(addrstr, server->s, server->length);
	} else {
		memcpy(addrstr, "localhost", 9);
	}

	memset((char *)&hints, 0, sizeof(hints));
	switch (protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		hints.ai_socktype = SOCK_DGRAM;
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		hints.ai_socktype = SOCK_STREAM;
		break;
	default:
		printf("resolve_address : not-supported protocol %d\n", protocol);
		break;
	}
	hints.ai_family = AF_UNSPEC;

	error = getaddrinfo(addrstr, NULL, &hints, &res);

	if (error != 0) {
		fprintf(stderr, "getaddrinfo: %d\n", error);
		return error;
	}

	for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
		switch (ainfo->ai_family) {
		case AF_INET6:
		case AF_INET:
			len = ainfo->ai_addrlen;
			memcpy(dst, ainfo->ai_addr, len);
			goto finish;
		default:
			;
		}
	}

finish:
	freeaddrinfo(res);
	return len;
}

static inline coap_opt_t *get_block(coap_pdu_t *pdu, coap_opt_iterator_t *opt_iter, coap_transport_t transport)
{
	coap_opt_filter_t f;

	assert(pdu);

	memset(f, 0, sizeof(coap_opt_filter_t));
	coap_option_setb(f, COAP_OPTION_BLOCK1);
	coap_option_setb(f, COAP_OPTION_BLOCK2);

	coap_option_iterator_init2(pdu, opt_iter, f, transport);
	return coap_option_next(opt_iter);
}

#define HANDLE_BLOCK1(Pdu)											\
	((method == COAP_REQUEST_PUT || method == COAP_REQUEST_POST) &&	\
	 ((flags & FLAGS_BLOCK) == 0) &&								\
	 ((Pdu)->hdr->code == COAP_RESPONSE_CODE(201) ||				\
	  (Pdu)->hdr->code == COAP_RESPONSE_CODE(204)))

inline int check_token(coap_pdu_t *received)
{
	return received->hdr->token_length == the_token.length && memcmp(received->hdr->token, the_token.s, the_token.length) == 0;
}

int check_token_by_value(unsigned char *token, size_t len)
{
	if (len == the_token.length) {
		if (!strncmp((char *)token, (char *)the_token.s, len))
			return 1;
	}
	return 0;
}

void message_handler(struct coap_context_t *ctx, const coap_address_t *remote, coap_pdu_t *sent, coap_pdu_t *received, const coap_tid_t id)
{
	coap_pdu_t *pdu = NULL;
	coap_opt_t *block_opt;
	coap_opt_iterator_t opt_iter;
	unsigned char buf[4];
	coap_list_t *option;
	size_t len;
	unsigned char *databuf;
	coap_tid_t tid;

	coap_transport_t transport = COAP_UDP;

	size_t tokenlen;
	unsigned char *token_ptr = NULL;
	char token[8] = {0,};

	unsigned short code;

	if (ctx->protocol == COAP_PROTO_TCP || ctx->protocol == COAP_PROTO_TLS) {
		transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)received->transport_hdr)[0] >> 4);
		debug("message_handler : received transport %d\n", transport);
#ifdef WITH_DEBUG_PDU_PRINT
		coap_debug_pdu_print(received, transport);
#endif
	} else {
		transport = COAP_UDP;
	}

	coap_get_token2((received->transport_hdr), transport, &token_ptr, &tokenlen);

	if (tokenlen > 0) {
		strncpy(token, (const char *)token_ptr, tokenlen);
		debug("message_handler : token %s, len %d, the_token %s, len %d, token_ptr %s\n",
				token, tokenlen, the_token.s, the_token.length, token_ptr);
	}

	/* check if this is a response to our original request */
	if (!check_token_by_value((unsigned char *)token, tokenlen)) {
		/* drop if this was just some message, or send RST in case of notification */
		if (!sent) {
			if (ctx->protocol == COAP_PROTO_UDP || ctx->protocol == COAP_PROTO_DTLS) {
				if (received->transport_hdr->udp.type == COAP_MESSAGE_CON ||
							received->transport_hdr->udp.type == COAP_MESSAGE_NON) {
					coap_send_rst(ctx, remote, received);
				}
			} else if (ctx->protocol == COAP_PROTO_TCP || ctx->protocol == COAP_PROTO_TLS) {
				/* TODO: do nothing? */
				debug("message_handler : do nothing\n");
			} else {
				/* should not enter here */
			}
		}
		warn("message_handler : unmatched token, ignore it\n");
		return;
	}

	if (ctx->protocol == COAP_PROTO_UDP || ctx->protocol == COAP_PROTO_DTLS) {
		switch (received->hdr->type) {
		case COAP_MESSAGE_CON:
			/* acknowledge received response if confirmable (TODO: check Token) */
			coap_send_ack(ctx, remote, received);
			break;
		case COAP_MESSAGE_RST:
			info("got RST\n");
			return;
		default:
			break;
		}
	}

	code = (unsigned short)coap_get_code(received, transport);

	debug("message_handler : code %d, sent %p\n", code, sent);

	/* output the received data, if any */
	if (code == COAP_RESPONSE_CODE(205)) {

		/* set obs timer if we have successfully subscribed a resource */
		if (sent && coap_check_option2(received, COAP_OPTION_SUBSCRIPTION, &opt_iter, transport)) {
			printf("message_handler : observation relationship established, set timeout to %d\n", obs_seconds);
			set_timeout(&obs_wait, obs_seconds);
		}

		/* Got some data, check if block option is set. Behavior is undefined if
		 * both, Block1 and Block2 are present. */
		block_opt = get_block(received, &opt_iter, transport);
		if (!block_opt) {
			/* There is no block option set, just read the data and we are done. */
			if (coap_get_data(received, &len, &databuf)) {
				append_to_output(databuf, len);
			}
		} else {
			unsigned short blktype = opt_iter.type;

			/* TODO: check if we are looking at the correct block number */
			if (coap_get_data(received, &len, &databuf)) {
				append_to_output(databuf, len);
			}

			if (COAP_OPT_BLOCK_MORE(block_opt)) {
				/* more bit is set */
				debug("found the M bit, block size is %u, block nr. %u\n", COAP_OPT_BLOCK_SZX(block_opt), coap_opt_block_num(block_opt));

				/* create pdu with request for next block */
				pdu = coap_new_request(ctx, method, NULL);	/* first, create bare PDU w/o any option  */
				if (pdu) {
					/* add URI components from optlist */
					for (option = optlist; option; option = option->next) {
						switch (COAP_OPTION_KEY(*(coap_option *) option->data)) {
						case COAP_OPTION_URI_HOST:
						case COAP_OPTION_URI_PORT:
						case COAP_OPTION_URI_PATH:
						case COAP_OPTION_URI_QUERY:
							coap_add_option2(pdu, COAP_OPTION_KEY(*(coap_option *) option->data),
									COAP_OPTION_LENGTH(*(coap_option *) option->data),
									COAP_OPTION_DATA(*(coap_option *) option->data), transport);
							break;
						default:
							break;	/* skip other options */
						}
					}

					/* finally add updated block option from response, clear M bit */
					/* blocknr = (blocknr & 0xfffffff7) + 0x10; */
					debug("query block %d\n", (coap_opt_block_num(block_opt) + 1));
					coap_add_option2(pdu, blktype, coap_encode_var_bytes(buf, ((coap_opt_block_num(block_opt) + 1) << 4) | COAP_OPT_BLOCK_SZX(block_opt)), buf, transport);

					switch (ctx->protocol) {
					case COAP_PROTO_UDP:
					case COAP_PROTO_DTLS:
						if (received->hdr->type == COAP_MESSAGE_CON) {
							tid = coap_send_confirmed(ctx, remote, pdu);
						} else {
							tid = coap_send(ctx, remote, pdu);
						}

						if (tid == COAP_INVALID_TID) {
							debug("message_handler: error sending new request");
							coap_delete_pdu(pdu);
						} else {
							set_timeout(&max_wait, wait_seconds);
							if (received->hdr->type != COAP_MESSAGE_CON) {
								coap_delete_pdu(pdu);
							}
						}
						break;
					case COAP_PROTO_TCP:
					case COAP_PROTO_TLS:
						tid = coap_send(ctx, remote, pdu);
						set_timeout(&max_wait, wait_seconds);
						coap_delete_pdu(pdu);
						break;
					default: /* should not be entered here */
						break;
					}
					return;
				}
			}
		}
	} else {					/* no 2.05 */

		debug("message_handler : response class %d\n", COAP_RESPONSE_CLASS(code));
		/* check if an error was signaled and output payload if so */
		if (COAP_RESPONSE_CLASS(code) >= 4) {
			fprintf(stderr, "%d.%02d", (code >> 5), code & 0x1F);
			if (coap_get_data(received, &len, &databuf)) {
				fprintf(stderr, " ");
				while (len--) {
					fprintf(stderr, "%c", *databuf++);
				}
			}
			fprintf(stderr, "\n");
		}

	}

	/* finally send new request, if needed */
	if (pdu != NULL) {
		if (coap_send(ctx, remote, pdu) == COAP_INVALID_TID) {
			debug("message_handler: error sending response");
		}
	} else {
		debug("message_handler: pdu is NULL\n");
	}
	coap_delete_pdu(pdu);

	/* our job is done, we can exit at any time */
	ready = coap_check_option2(received, COAP_OPTION_SUBSCRIPTION, &opt_iter, transport) == NULL;
}

void usage(const char *program, const char *version)
{
	const char *p;

	p = strrchr(program, '/');
	if (p) {
		program = ++p;
	}

	fprintf(stderr, "%s v%s -- a small CoAP implementation\n"
			"(c) 2010-2013 Olaf Bergmann <bergmann@tzi.org>\n\n"
#if defined(__TINYARA__)
			"usage: %s [-A type...] [-t type] [-B seconds] [-e text]\n"
			"\t\t[-m method] [-N] [-p port] [-T string] [-v num] URI\n\n"
			"\tURI can be an absolute or relative coap URI,\n"
			"\t-A type...\taccepted media types as comma-separated list of\n" "\t\t\tsymbolic or numeric values\n"
			"\t-B seconds\tbreak operation after waiting given seconds\n" "\t\t\t(default is %d)\n"
			"\t-e text\t\tinclude text as payload (use percent-encoding for\n" "\t\t\tnon-ASCII characters)\n"
			"\t-m method\trequest method (get|put|post|delete), default is 'get'\n"
			"\t-N\t\tsend NON-confirmable message\n"
			"\t-p port\t\tlisten on specified port\n" "\t-s duration\tsubscribe for given duration [s]\n"
			"\t-v num\t\tverbosity level (default: 3)\n"
			"\t-T token\tinclude specified token\n" "\n"
#ifdef WITH_MBEDTLS
			"\t-I identity\tPre-Shared Key identity used to security session\n"
			"\t-S pre-shared key\tPre-Shared Key. Input length MUST be even (e.g, 11, 1111.)\n"
#endif /* WITH_MBEDTLS */
#else
			"usage: %s [-A type...] [-t type] [-b [num,]size] [-B seconds] [-e text]\n"
			"\t\t[-g group] [-m method] [-N] [-o file] [-P addr[:port]] [-p port]\n"
			"\t\t[-s duration] [-O num,text] [-T string] [-v num] URI\n\n"
			"\tURI can be an absolute or relative coap URI,\n"
			"\t-A type...\taccepted media types as comma-separated list of\n" "\t\t\tsymbolic or numeric values\n"
			"\t-t type\t\tcontent type for given resource for PUT/POST\n"
			"\t-b [num,]size\tblock size to be used in GET/PUT/POST requests\n"
			"\t       \t\t(value must be a multiple of 16 not larger than 1024)\n"
			"\t       \t\tIf num is present, the request chain will start at\n"
			"\t       \t\tblock num\n"
			"\t-B seconds\tbreak operation after waiting given seconds\n" "\t\t\t(default is %d)\n"
			"\t-e text\t\tinclude text as payload (use percent-encoding for\n" "\t\t\tnon-ASCII characters)\n"
			"\t-g group\tjoin the given multicast group\n"
			"\t-m method\trequest method (get|put|post|delete), default is 'get'\n"
			"\t-N\t\tsend NON-confirmable message\n"
			"\t-p port\t\tlisten on specified port\n" "\t-s duration\tsubscribe for given duration [s]\n"
			"\t-v num\t\tverbosity level (default: 3)\n"
			"\t-O num,text\tadd option num with contents text to request\n"
			"\t-P addr[:port]\tuse proxy (automatically adds Proxy-Uri option to\n"
			"\t\t\trequest)\n"
			"\t-T token\tinclude specified token\n" "\n"
#endif
			"examples:\n" "\tlibcoap-client -m get coap://[::1]/\n"
			"\tlibcoap-client -m get coap://[::1]/.well-known/core\n"
			"\tlibcoap-client -m get -T cafe coap://[::1]/time\n"
			"\tlibcoap-client -m put -e 1000 -T cafe coap://[::1]/time\n"
#ifdef WITH_MBEDTLS
			"examples for secure session:\n"
			"\tlibcoap-client -m get coaps://[::1]/.well-known/core\n"
			"\tlibcoap-client -m get coaps+tcp://[::1]/.well-known/core\n"
#endif
			, program, version, program, wait_seconds);
}

#if !defined(__TINYARA__)
int join(coap_context_t *ctx, char *group_name)
{
	struct ipv6_mreq mreq;
	struct addrinfo *reslocal = NULL, *resmulti = NULL, hints, *ainfo;
	int result = -1;

	/* we have to resolve the link-local interface to get the interface id */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;

	result = getaddrinfo("::", NULL, &hints, &reslocal);
	if (result < 0) {
		fprintf(stderr, "join: cannot resolve link-local interface: %s\n", gai_strerror(result));
		goto finish;
	}

	/* get the first suitable interface identifier */
	for (ainfo = reslocal; ainfo != NULL; ainfo = ainfo->ai_next) {
		if (ainfo->ai_family == AF_INET6) {
			mreq.ipv6mr_interface = ((struct sockaddr_in6 *)ainfo->ai_addr)->sin6_scope_id;
			break;
		}
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;

	/* resolve the multicast group address */
	result = getaddrinfo(group_name, NULL, &hints, &resmulti);

	if (result < 0) {
		fprintf(stderr, "join: cannot resolve multicast address: %s\n", gai_strerror(result));
		goto finish;
	}

	for (ainfo = resmulti; ainfo != NULL; ainfo = ainfo->ai_next) {
		if (ainfo->ai_family == AF_INET6) {
			mreq.ipv6mr_multiaddr = ((struct sockaddr_in6 *)ainfo->ai_addr)->sin6_addr;
			break;
		}
	}

	result = setsockopt(ctx->sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&mreq, sizeof(mreq));
	if (result < 0) {
		perror("join: setsockopt");
	}

finish:
	freeaddrinfo(resmulti);
	freeaddrinfo(reslocal);

	return result;
}
#endif

int order_opts(void *a, void *b)
{
	if (!a || !b) {
		return a < b ? -1 : 1;
	}

	if (COAP_OPTION_KEY(*(coap_option *) a) < COAP_OPTION_KEY(*(coap_option *) b)) {
		return -1;
	}

	return COAP_OPTION_KEY(*(coap_option *) a) == COAP_OPTION_KEY(*(coap_option *) b);
}

coap_list_t *new_option_node(unsigned short key, unsigned int length, unsigned char *data)
{
	coap_option *option;
	coap_list_t *node;

	option = coap_malloc(sizeof(coap_option) + length);
	if (!option) {
		goto error;
	}

	COAP_OPTION_KEY(*option) = key;
	COAP_OPTION_LENGTH(*option) = length;
	memcpy(COAP_OPTION_DATA(*option), data, length);

	/* we can pass NULL here as delete function since option is released automatically  */
	node = coap_new_listnode(option, NULL);

	if (node) {
		return node;
	}

error:
	perror("new_option_node: malloc");
	coap_free(option);
	return NULL;
}

typedef struct {
	unsigned char code;
	char *media_type;
} content_type_t;

void cmdline_content_type(char *arg, unsigned short key)
{
	static content_type_t content_types[] = {
		{0, "plain"},
		{0, "text/plain"},
		{40, "link"},
		{40, "link-format"},
		{40, "application/link-format"},
		{41, "xml"},
		{42, "binary"},
		{42, "octet-stream"},
		{42, "application/octet-stream"},
		{47, "exi"},
		{47, "application/exi"},
		{50, "json"},
		{50, "application/json"},
		{255, NULL}
	};
	coap_list_t *node;
	unsigned char i, value[10];
	int valcnt = 0;
	unsigned char buf[2];
	char *p, *q = arg;

	while (q && *q) {
		p = strchr(q, ',');

		if (isdigit(*q)) {
			if (p) {
				*p = '\0';
			}
			value[valcnt++] = atoi(q);
		} else {
			for (i = 0; content_types[i].media_type && strncmp(q, content_types[i].media_type, p ? p - q : strlen(q)) != 0; ++i) ;

			if (content_types[i].media_type) {
				value[valcnt] = content_types[i].code;
				valcnt++;
			} else {
				warn("W: unknown content-type '%s'\n", arg);
			}
		}

		if (!p || key == COAP_OPTION_CONTENT_TYPE) {
			break;
		}

		q = p + 1;
	}

	for (i = 0; i < valcnt; ++i) {
		node = new_option_node(key, coap_encode_var_bytes(buf, value[i]), buf);
		if (node) {
			coap_insert(&optlist, node, order_opts);
		}
	}
}

void cmdline_uri(char *arg)
{
	unsigned char portbuf[2];
#define BUFSIZE 40
	unsigned char _buf[BUFSIZE];
	unsigned char *buf = _buf;
	size_t buflen;
	int res;

	if (proxy.length) {			/* create Proxy-Uri from argument */
		size_t len = strlen(arg);
		while (len > 270) {
			coap_insert(&optlist, new_option_node(COAP_OPTION_PROXY_URI, 270, (unsigned char *)arg), order_opts);
			len -= 270;
			arg += 270;
		}

		coap_insert(&optlist, new_option_node(COAP_OPTION_PROXY_URI, len, (unsigned char *)arg), order_opts);
	} else {					/* split arg into Uri-* options */
		coap_split_uri((unsigned char *)arg, strlen(arg), &uri);

		if (uri.port != COAP_DEFAULT_PORT) {
			coap_insert(&optlist, new_option_node(COAP_OPTION_URI_PORT, coap_encode_var_bytes(portbuf, uri.port), portbuf), order_opts);
		}

		if (uri.path.length) {
			buflen = BUFSIZE;
			res = coap_split_path(uri.path.s, uri.path.length, buf, &buflen);

			while (res--) {
				coap_insert(&optlist, new_option_node(COAP_OPTION_URI_PATH, COAP_OPT_LENGTH(buf), COAP_OPT_VALUE(buf)), order_opts);
				buf += COAP_OPT_SIZE(buf);
			}
		}

		if (uri.query.length) {
			buflen = BUFSIZE;
			buf = _buf;
			res = coap_split_query(uri.query.s, uri.query.length, buf, &buflen);

			while (res--) {
				coap_insert(&optlist, new_option_node(COAP_OPTION_URI_QUERY, COAP_OPT_LENGTH(buf), COAP_OPT_VALUE(buf)), order_opts);
				buf += COAP_OPT_SIZE(buf);
			}
		}
	}
}

int cmdline_blocksize(char *arg)
{
	unsigned short size;

again:
	size = 0;
	while (*arg && *arg != ',') {
		size = size * 10 + (*arg++ - '0');
	}

	if (*arg == ',') {
		arg++;
		block.num = size;
		goto again;
	}

	if (size) {
		block.szx = (coap_fls(size >> 4) - 1) & 0x07;
	}

	flags |= FLAGS_BLOCK;
	return 1;
}

/* Called after processing the options from the commandline to set
 * Block1 or Block2 depending on method. */
void set_blocksize(void)
{
	static unsigned char buf[4];	/* hack: temporarily take encoded bytes */
	unsigned short opt;

	if (method != COAP_REQUEST_DELETE) {
		opt = method == COAP_REQUEST_GET ? COAP_OPTION_BLOCK2 : COAP_OPTION_BLOCK1;

		coap_insert(&optlist, new_option_node(opt, coap_encode_var_bytes(buf, (block.num << 4 | block.szx)), buf), order_opts);
	}
}

void cmdline_subscribe(char *arg)
{
	obs_seconds = atoi(optarg);
	coap_insert(&optlist, new_option_node(COAP_OPTION_SUBSCRIPTION, 0, NULL), order_opts);
}

int cmdline_proxy(char *arg)
{
	char *proxy_port_str = strrchr((const char *)arg, ':');	/* explicit port ? */
	if (proxy_port_str) {
		char *ipv6_delimiter = strrchr((const char *)arg, ']');
		if (!ipv6_delimiter) {
			if (proxy_port_str == strchr((const char *)arg, ':')) {
				/* host:port format - host not in ipv6 hexadecimal string format */
				*proxy_port_str++ = '\0';	/* split */
				proxy_port = atoi(proxy_port_str);
			}
		} else {
			arg = strchr((const char *)arg, '[');
			if (!arg) {
				return 0;
			}
			arg++;
			*ipv6_delimiter = '\0';	/* split */
			if (ipv6_delimiter + 1 == proxy_port_str++) {
				/* [ipv6 address]:port */
				proxy_port = atoi(proxy_port_str);
			}
		}
	}

	proxy.length = strlen(arg);
	if ((proxy.s = coap_malloc(proxy.length + 1)) == NULL) {
		proxy.length = 0;
		return 0;
	}

	memcpy(proxy.s, arg, proxy.length + 1);
	return 1;
}

inline static void cmdline_token(char *arg)
{
	strncpy((char *)the_token.s, arg, min(sizeof(_token_data), strlen(arg)));
	the_token.length = strlen(arg);
}

void cmdline_option(char *arg)
{
	unsigned int num = 0;

	while (*arg && *arg != ',') {
		num = num * 10 + (*arg - '0');
		++arg;
	}
	if (*arg == ',') {
		++arg;
	}

	coap_insert(&optlist, new_option_node(num, strlen(arg), (unsigned char *)arg), order_opts);
}

extern int check_segment(const unsigned char *s, size_t length);
extern void decode_segment(const unsigned char *seg, size_t length, unsigned char *buf);

int cmdline_input(char *text, str *buf)
{
	int len;
	len = check_segment((unsigned char *)text, strlen(text));

	if (len < 0) {
		return 0;
	}

	buf->s = (unsigned char *)coap_malloc(len);
	if (!buf->s) {
		return 0;
	}

	buf->length = len;
	decode_segment((unsigned char *)text, strlen(text), buf->s);
	return 1;
}

int cmdline_input_from_file(char *filename, str *buf)
{
	FILE *inputfile = NULL;
	ssize_t len;
	int result = 1;
	struct stat statbuf;

	if (!filename || !buf) {
		return 0;
	}

	if (filename[0] == '-' && !filename[1]) {	/* read from stdin */
		buf->length = 20000;
		buf->s = (unsigned char *)coap_malloc(buf->length);
		if (!buf->s) {
			return 0;
		}

		inputfile = stdin;
	} else {
		/* read from specified input file */
		if (stat(filename, &statbuf) < 0) {
			perror("cmdline_input_from_file: stat");
			return 0;
		}

		buf->length = statbuf.st_size;
		buf->s = (unsigned char *)coap_malloc(buf->length);
		if (!buf->s) {
			return 0;
		}

		inputfile = fopen(filename, "r");
		if (!inputfile) {
			perror("cmdline_input_from_file: fopen");
			coap_free(buf->s);
			return 0;
		}
	}

	len = fread(buf->s, 1, buf->length, inputfile);

	if (len < buf->length) {
		if (ferror(inputfile) != 0) {
			perror("cmdline_input_from_file: fread");
			coap_free(buf->s);
			buf->length = 0;
			buf->s = NULL;
			result = 0;
		} else {
			buf->length = len;
		}
	}

	if (inputfile != stdin) {
		fclose(inputfile);
	}

	return result;
}

method_t cmdline_method(char *arg)
{
	static char *methods[] = { 0, "get", "post", "put", "delete", 0 };
	unsigned char i;

	for (i = 1; methods[i] && strcasecmp(arg, methods[i]) != 0; ++i) ;

	return i;					/* note that we do not prevent illegal methods */
}

#if defined(__TINYARA__)
int coap_client_test_run(void *arg)
#else
int main(int argc, char **argv)
#endif
{
	coap_context_t *ctx = NULL;
	coap_address_t dst;
	static char addr[INET6_ADDRSTRLEN];
	void *addrptr = NULL;
	fd_set readfds;
	struct timeval tv;
	int result;
	coap_tick_t now;
	coap_queue_t *nextpdu;
	coap_pdu_t *pdu;
	static str server;
	unsigned short port = COAP_DEFAULT_PORT;
	char port_str[NI_MAXSERV] = {0,};
	char host_str[256] = {0,};
	int opt, res;
	int portChanged = 0;
	int invalid_opt = 0;
	char *group = NULL;

	coap_log_t log_level = LOG_WARNING;
	coap_tid_t tid = COAP_INVALID_TID;

	coap_protocol_t protocol = COAP_PROTO_UDP;

#if defined(__TINYARA__)
	int argc;
	char **argv;

	argc = ((struct coap_client_input *)arg)->argc;
	argv = ((struct coap_client_input *)arg)->argv;
#endif

#ifdef WITH_MBEDTLS
	char *pskId = NULL;
	char *pskBuffer = NULL;
	unsigned char psk[MBEDTLS_PSK_MAX_LEN];

	tls_opt tls_option;
	tls_ctx *tls_context = NULL;
	tls_cred cred;

	memset(&tls_option, 0, sizeof(tls_opt));

	tls_option.server = MBEDTLS_SSL_IS_CLIENT;
	tls_option.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
	tls_option.auth_mode = 0;
	tls_option.debug_mode = 3;

	tls_option.force_ciphersuites[0] = mbedtls_ssl_get_ciphersuite_id(COAP_MBEDTLS_CIPHERSUIT);
	tls_option.force_ciphersuites[1] = 0;
#endif

	while ((opt = getopt(argc, argv, "Nb:e:f:g:m:p:s:t:o:v:A:B:O:P:T:I:S:")) != -1) {
		switch (opt) {
		case 'b':
			cmdline_blocksize(optarg);
			break;
		case 'B':
			wait_seconds = atoi(optarg);
			break;
		case 'e':
			if (!cmdline_input(optarg, &payload)) {
				payload.length = 0;
			}
			break;
		case 'g':
			group = optarg;
			break;
		case 'p':
			strncpy(port_str, optarg, NI_MAXSERV - 1);
			portChanged = 1;
			port_str[NI_MAXSERV - 1] = '\0';
			break;
		case 'm':
			method = cmdline_method(optarg);
			break;
		case 'N':
			msgtype = COAP_MESSAGE_NON;
			break;
		case 's':
			cmdline_subscribe(optarg);
			break;
		case 'A':
			cmdline_content_type(optarg, COAP_OPTION_ACCEPT);
			break;
		case 't':
			cmdline_content_type(optarg, COAP_OPTION_CONTENT_TYPE);
			break;
		case 'O':
			cmdline_option(optarg);
			break;
		case 'P':
			if (!cmdline_proxy(optarg)) {
				fprintf(stderr, "error specifying proxy address\n");
				break;
			}
			break;
		case 'T':
			cmdline_token(optarg);
			break;
		case 'v':
			log_level = strtol(optarg, NULL, 10);
			break;
#ifdef WITH_MBEDTLS
		case 'I':
			pskId = optarg;
			printf("coap-client : Pre-Shared Key ID %s\n", pskId);
			break;
		case 'S':
			pskBuffer = optarg;
			printf("coap-client : Pre-Shared Key %s\n", pskBuffer);
			break;
#endif
		default:
			if (!invalid_opt) {
				invalid_opt = 1;
				usage(argv[0], PACKAGE_VERSION);
			}
			break;
		}
	}

	/* Exit program when invalid argument is passed from command line*/
	if (invalid_opt)
		return 0;

	coap_set_log_level(log_level);

	if (optind < argc) {
		protocol = coap_get_protocol_from_uri(argv[optind]);
#ifdef WITH_MBEDTLS
		if (protocol >= COAP_PROTO_MAX) {
			printf("coap-client : uri prefix might be wrong %s\n", argv[optind]);
			return 0;
		} else {
			printf("coap-client : transport protocol %d\n", protocol);
			/* TODO : DTLS not support CON message type ? */
			if (protocol != COAP_PROTO_UDP)
				msgtype = COAP_MESSAGE_NON;
		}
#else
		if (protocol != COAP_PROTO_UDP && protocol != COAP_PROTO_TCP) {
			printf("coap-client : not supported protocol\n");
			return 0;
		}
#endif /* WITH_MBEDTLS*/
		cmdline_uri(argv[optind]);
	} else {
		usage(argv[0], PACKAGE_VERSION);
		return 0;
	}

	/* Set default port when there are no inserted port number from user */
	if (!portChanged) {
		memset(port_str, 0, NI_MAXSERV-1);
		switch (protocol) {
		case COAP_PROTO_UDP:
		case COAP_PROTO_TCP:
			strncpy(port_str, COAP_STANDARD_PORT, strlen(COAP_STANDARD_PORT));
			break;
		case COAP_PROTO_DTLS:
		case COAP_PROTO_TLS:
			strncpy(port_str, COAP_SECURITY_PORT, strlen(COAP_SECURITY_PORT));
			break;
		default:
			printf("coap-client : not-supported protocol %d\n", protocol);
			return -1;
		}
	}

	if (proxy.length) {
		server = proxy;
		port = proxy_port;
	} else {
		server = uri.host;
		port = uri.port;
	}

	/* resolve destination address where server should be sent */
	res = resolve_address(&server, &dst.addr.sa, protocol);

	if (res < 0) {
		fprintf(stderr, "failed to resolve address\n");
		return -1;
	}

	dst.size = res;
	dst.addr.sin.sin_port = htons(port);

	memcpy(host_str, server.s, server.length);

	ctx = coap_create_context(protocol);

	if (!ctx) {
		coap_log(LOG_EMERG, "cannot create context\n");
		return -1;
	}

#ifdef WITH_MBEDTLS
	if (protocol == COAP_PROTO_TLS || protocol == COAP_PROTO_DTLS) {
		tls_option.transport = (protocol == COAP_PROTO_TLS) ?
			(MBEDTLS_SSL_TRANSPORT_STREAM) : (MBEDTLS_SSL_TRANSPORT_DATAGRAM);

		/* Set credential information */
		memset(&cred, 0, sizeof(tls_cred));

		if (pskBuffer) {
			if (coap_unhexify(psk, pskBuffer, &cred.psk_len) == 0) {
				if (pskId) {
					cred.psk_identity = pskId;
					cred.psk = psk;
				}
			}

			if (!cred.psk_identity && !cred.psk) {
				printf("coap-client : failed to create PSK info\n");
				goto error_exit;
			}
		} else {
			printf("coap-client : MUST set PSK and PSK Identity\n");
			goto error_exit;
		}

		tls_context = TLSCtx(&cred);
		if (tls_context == NULL) {
			printf("coap-client : failed to initialize TLS\n");
			goto error_exit;
		}
	}
#endif

	/* Try to connect to network */
#ifdef WITH_MBEDTLS
	if (coap_net_connect(ctx, host_str, port_str, (void *)tls_context, (void *)&tls_option) < 0)
#else
	if (coap_net_connect(ctx, host_str, port_str, NULL, NULL) < 0)
#endif
	{
		printf("coap-client : failed to connect network\n");
		goto error_exit;
	}

	coap_register_option(ctx, COAP_OPTION_BLOCK2);
	coap_register_response_handler(ctx, message_handler);

	/* join multicast group if requested at command line */
#if 0
	if (group) {
		join(ctx, group);
	}
#endif

	/* construct CoAP message */
	if (!proxy.length && addrptr && (inet_ntop(dst.addr.sa.sa_family, addrptr, addr, sizeof(addr)) != 0)
		&& (strlen(addr) != uri.host.length || memcmp(addr, uri.host.s, uri.host.length) != 0)) {
		/* add Uri-Host */

		coap_insert(&optlist, new_option_node(COAP_OPTION_URI_HOST, uri.host.length, uri.host.s), order_opts);
	}

	/* set block option if requested at commandline */
	if (flags & FLAGS_BLOCK) {
		set_blocksize();
	}

	if (!(pdu = coap_new_request(ctx, method, optlist))) {
		return -1;
	}

#ifndef NDEBUG
	if (LOG_DEBUG <= coap_get_log_level()) {
		debug("sending CoAP request:\n");
		coap_show_pdu2(pdu, protocol);
	}
#endif


	switch (protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		if (pdu->transport_hdr->udp.type == COAP_MESSAGE_CON) {
			tid = coap_send_confirmed(ctx, &dst, pdu);
		} else {
			tid = coap_send(ctx, &dst, pdu);
		}

		if (pdu->transport_hdr->udp.type != COAP_MESSAGE_CON || tid == COAP_INVALID_TID) {
			coap_delete_pdu(pdu);
		}
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		tid = coap_send(ctx, &dst, pdu);
		coap_delete_pdu(pdu);
		break;
	default:
		/* should not enter here */
		break;
	}

	set_timeout(&max_wait, wait_seconds);
	debug("timeout is set to %d seconds\n", wait_seconds);

	while (!(ready && coap_can_exit(ctx))) {
		FD_ZERO(&readfds);
		FD_SET(ctx->sockfd, &readfds);

		nextpdu = coap_peek_next(ctx);

		coap_ticks(&now);
		while (nextpdu && nextpdu->t <= now - ctx->sendqueue_basetime) {
			coap_retransmit(ctx, coap_pop_next(ctx));
			nextpdu = coap_peek_next(ctx);
		}

		if (nextpdu && nextpdu->t < min(obs_wait ? obs_wait : max_wait, max_wait) - now) {
			/* set timeout if there is a pdu to send */
			tv.tv_usec = ((nextpdu->t) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
			tv.tv_sec = (nextpdu->t) / COAP_TICKS_PER_SECOND;
		} else {
			/* check if obs_wait fires before max_wait */
			if (obs_wait && obs_wait < max_wait) {
				tv.tv_usec = ((obs_wait - now) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
				tv.tv_sec = (obs_wait - now) / COAP_TICKS_PER_SECOND;
			} else {
				tv.tv_usec = ((max_wait - now) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
				tv.tv_sec = (max_wait - now) / COAP_TICKS_PER_SECOND;
			}
		}

		/* To prevent abnormal waiting on select */
		if (tv.tv_sec > (time_t)wait_seconds) {
			tv.tv_sec = (time_t)wait_seconds;
			tv.tv_usec = 0;
		}

		debug("coap-client : timeout info, tv %lu sec. %ld usec, now %lu, obs_wait %lu\n",
				(unsigned long)tv.tv_sec, tv.tv_usec, (unsigned long)now, (unsigned long)obs_wait);

		result = select(ctx->sockfd + 1, &readfds, 0, 0, &tv);

		if (result < 0) {		/* error */
			printf("ERROR : failed on select, errno %d\n", errno);
			break;
		} else if (result > 0) {	/* read from socket */
			if (FD_ISSET(ctx->sockfd, &readfds)) {
				coap_read(ctx);	/* read received data */
				coap_dispatch(ctx);	/* and dispatch PDUs from receivequeue */
			}
		} else {				/* timeout */
			coap_ticks(&now);
			if (max_wait <= now) {
				info("timeout\n");
				break;
			}
			if (obs_wait && obs_wait <= now) {
				info("\nclear observation relationship obs_wait %lu, now %lu\n",
						(unsigned long)obs_wait, (unsigned long)now);
				clear_obs(ctx, &dst);	/* FIXME: handle error case COAP_TID_INVALID */
				/* make sure that the obs timer does not fire again */
				obs_wait = 0;
				obs_seconds = 0;
			}
		}
	}
error_exit:
	close_output();
#ifdef WITH_MBEDTLS
	if (ctx->session) {
		TLSSession_free(ctx->session);
	}

	if (tls_context) {
		TLSCtx_free(tls_context);
	}
#endif
	if (ctx->sockfd > 0) {
		if (FD_ISSET(ctx->sockfd, &readfds)) {
			FD_CLR(ctx->sockfd, &readfds);
		}
		close(ctx->sockfd);
	}

	coap_free_context(ctx);

	/*
	 * free payload.s
	 * when cmdline_input function is used (e option), parameter s has allocated memory
	 */
	if (payload.s) {
		free(payload.s);
	}

	payload.s = NULL;
	payload.length = 0;

	/*
	 * deinitialize variables
	 * optlist : without initialize optlist, new option is appened to old option existed in optlist
	 *           it can cause wrong creation of option field
	 * ready : when NON message is continously sent to server, ready field should be deinitialized
	 *         without deinitialization, it cannot enter select loop
	 */

	coap_list_t *next = NULL;

	if (optlist) {
		do {
			next = optlist->next;
			coap_delete(optlist);
			optlist = next;
		} while (optlist != NULL);
	}
	optlist = NULL;
	ready = 0;

	printf("coap-client : good bye\n");

	return 0;
}

#if defined(__TINYARA__)
int coap_client_test_main(int argc, char **argv)
{
	int status;

	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	struct coap_client_input arg;

	status = pthread_attr_init(&attr);

	if (status != 0) {
		printf("coap_client_test_main : failed to start coap-client\n");
		return -1;
	}

	sparam.sched_priority = COAP_CLIENT_SCHED_PRI;
	if ((status = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		printf("coap_client_test_main : failed pthread_attr_setschedparam, errno %d\n", errno);
		return -1;
	}

	if ((status = pthread_attr_setschedpolicy(&attr, COAP_CLIENT_SCHED_POLICY)) != 0) {
		printf("coap_client_test_main : failed pthread_attr_setschedpolicy, errno %d\n", errno);
		return -1;
	}

	if ((status = pthread_attr_setstacksize(&attr, COAP_CLIENT_STACK_SIZE)) != 0) {
		printf("coap_client_test_main : failed pthread_attr_setstacksize, errno %d\n", errno);
		return -1;
	}

	arg.argc = argc;
	arg.argv = argv;

	if ((status = pthread_create(&tid, &attr, (pthread_startroutine_t)coap_client_test_run, &arg)) < 0) {
		printf("coap_client_test_main : failed to run coap-client, errno %d\n", errno);
		return -1;
	}

	pthread_setname_np(tid, "coap-client-test");
	pthread_join(tid, NULL);

	return 0;
}
#endif /* __TINYARA__ */
