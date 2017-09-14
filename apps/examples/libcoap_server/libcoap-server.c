/* coap -- simple implementation of the Constrained Application Protocol (CoAP)
 *         as defined in draft-ietf-core-coap
 *
 * Copyright (C) 2010--2013 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>

#include <protocols/libcoap/coap.h>

#define COAP_RESOURCE_CHECK_TIME 2

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif

#if defined(__TINYARA__)
#define COAP_SERVER_SCHED_PRI    100
#define COAP_SERVER_SCHED_POLICY SCHED_RR
#define COAP_SERVER_STACK_SIZE   (1024 * 16)

/* used for sending input arguments to pthread */
struct coap_server_input {
	int argc;
	char **argv;
};

#ifndef FD_SETSIZE
#define FD_SETSIZE	(CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS)
#endif
#endif /* __TINYARA__ */

#define COAP_STANDARD_PORT "5683"
#define COAP_SECURITY_PORT "5684"

enum server_status {
	SERVER_STOPPED = 0,
	SERVER_RUNNING,
	SERVER_UNKNOWN
};
static enum server_status g_operating_status;

/* temporary storage for dynamic resource representations */
static int quit = 0;

/* changeable clock base (see handle_put_time()) */
static time_t my_clock_base = 0;

struct coap_resource_t *time_resource = NULL;

#ifndef WITHOUT_ASYNC
/* This variable is used to mimic long-running tasks that require
 * asynchronous responses. */
static coap_async_state_t *async = NULL;
#endif							/* WITHOUT_ASYNC */

#if !defined(__TINYARA__)
/* SIGINT handler: set quit to 1 for graceful termination */
static void handle_sigint(int signum)
{
	quit = 1;
}
#endif

#define INDEX \
	"This is a test server made with libcoap (see http://libcoap.sf.net)\n" \
	"Copyright (C) 2010--2013 Olaf Bergmann <bergmann@tzi.org>\n\n"

void hnd_get_index(coap_context_t *ctx, struct coap_resource_t *resource, coap_address_t *peer, coap_pdu_t *request, str *token, coap_pdu_t *response)
{
	unsigned char buf[3];

	response->transport_hdr->udp.code = COAP_RESPONSE_CODE(205);

	coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);

	coap_add_option(response, COAP_OPTION_MAXAGE, coap_encode_var_bytes(buf, 0x2ffff), buf);

	coap_add_data(response, strlen(INDEX), (unsigned char *)INDEX);
}

void hnd_get_time(coap_context_t *ctx, struct coap_resource_t *resource, coap_address_t *peer, coap_pdu_t *request, str *token, coap_pdu_t *response)
{
	coap_opt_iterator_t opt_iter;
	coap_opt_t *option;
	unsigned char buf[40];
	size_t len;
	time_t now;
	coap_tick_t t;
	coap_subscription_t *subscription;

	coap_transport_t transport = COAP_UDP;

	/* FIXME: return time, e.g. in human-readable by default and ticks
	 * when query ?ticks is given. */

	/* if my_clock_base was deleted, we pretend to have no such resource */

	switch (ctx->protocol) {
	case COAP_PROTO_UDP:
	case COAP_PROTO_DTLS:
		transport = COAP_UDP;
		break;
	case COAP_PROTO_TCP:
	case COAP_PROTO_TLS:
		if (request) {
			transport = coap_get_tcp_header_type_from_initbyte(((unsigned char *)request->transport_hdr)[0] >> 4);
		}
		break;
	default:
		/* Should not enter here */
		break;
	}

	response->transport_hdr->udp.code = my_clock_base ? COAP_RESPONSE_CODE(205) : COAP_RESPONSE_CODE(404);

	if (request != NULL && coap_check_option2(request, COAP_OPTION_OBSERVE, &opt_iter, transport)) {
		if (token && token->length > 0) {
			printf("hnd_get_time : add observer with token %s, token len %d\n", token->s, token->length);
		}

		subscription = coap_add_observer(resource, peer, token);
		if (subscription) {
			if (ctx->protocol == COAP_PROTO_UDP || ctx->protocol == COAP_PROTO_DTLS) {
				subscription->non = request->transport_hdr->udp.type == COAP_MESSAGE_NON;
			} else if (ctx->protocol == COAP_PROTO_TCP || ctx->protocol == COAP_PROTO_TLS) {
				/* TCP / TLS doesn't have type field on header */
				printf("hnd_get_time : succeed to add observer\n");
				subscription->non = 1;
			} else {
				/* Should not enter here */
			}
			coap_add_option(response, COAP_OPTION_OBSERVE, 0, NULL);
		}
	}
	if (resource->dirty == 1) {
		coap_add_option(response, COAP_OPTION_OBSERVE, coap_encode_var_bytes(buf, ctx->observe), buf);
	}

	if (my_clock_base) {
		coap_add_option(response, COAP_OPTION_CONTENT_FORMAT, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	}

	coap_add_option(response, COAP_OPTION_MAXAGE, coap_encode_var_bytes(buf, 0x01), buf);

	if (my_clock_base) {

		/* calculate current time */
		coap_ticks(&t);
		now = my_clock_base + (t / COAP_TICKS_PER_SECOND);

		if (request != NULL && (option = coap_check_option2(request, COAP_OPTION_URI_QUERY, &opt_iter, transport))
			&& memcmp(COAP_OPT_VALUE(option), "ticks", min(5, COAP_OPT_LENGTH(option))) == 0) {
			/* output ticks */
			len = snprintf((char *)buf, min(sizeof(buf), response->max_size - response->length), "%u", (unsigned int)now);
			coap_add_data(response, len, buf);

		} else {				/* output human-readable time */
			struct tm *tmp;
			tmp = gmtime(&now);
			len = strftime((char *)buf, min(sizeof(buf), response->max_size - response->length), "%b %d %H:%M:%S", tmp);
			coap_add_data(response, len, buf);
		}
	}
}

void hnd_put_time(coap_context_t *ctx, struct coap_resource_t *resource, coap_address_t *peer, coap_pdu_t *request, str *token, coap_pdu_t *response)
{
	coap_tick_t t;
	size_t size;
	unsigned char *data;

	/* FIXME: re-set my_clock_base to clock_offset if my_clock_base == 0
	 * and request is empty. When not empty, set to value in request payload
	 * (insist on query ?ticks). Return Created or Ok.
	 */

	debug("hnd_put_time : enter\n");

	/* if my_clock_base was deleted, we pretend to have no such resource */
	response->transport_hdr->udp.code = my_clock_base ? COAP_RESPONSE_CODE(204) : COAP_RESPONSE_CODE(201);

	resource->dirty = 1;

	coap_get_data(request, &size, &data);

	if (size == 0) {			/* re-init */
		my_clock_base = clock_offset;
	} else {
		my_clock_base = 0;
		coap_ticks(&t);
		while (size--) {
			my_clock_base = my_clock_base * 10 + *data++;
		}
		my_clock_base -= t / COAP_TICKS_PER_SECOND;
	}
	printf("coap_server : hnd_put_time, my_clock_base changed %ld\n", my_clock_base);
}

void hnd_delete_time(coap_context_t *ctx, struct coap_resource_t *resource, coap_address_t *peer, coap_pdu_t *request, str *token, coap_pdu_t *response)
{
	my_clock_base = 0;			/* mark clock as "deleted" */

	/* type = request->transport_hdr->udp.type == COAP_MESSAGE_CON  */
	/*   ? COAP_MESSAGE_ACK : COAP_MESSAGE_NON; */
}

#ifndef WITHOUT_ASYNC
void hnd_get_async(coap_context_t *ctx, struct coap_resource_t *resource, coap_address_t *peer, coap_pdu_t *request, str *token, coap_pdu_t *response)
{
	coap_opt_iterator_t opt_iter;
	coap_opt_t *option;
	unsigned long delay = 5;
	size_t size;

	coap_opt_filter_t f;
	coap_transport_t transport = COAP_UDP;

	switch (ctx->protocol) {
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

	if (async) {
		coap_option_filter_clear(f);
		if (ctx->protocol == COAP_PROTO_UDP || ctx->protocol == COAP_PROTO_DTLS) {
			if (async->id != request->transport_hdr->udp.id) {
				response->transport_hdr->udp.code = COAP_RESPONSE_CODE(503);
			}
		} else if (ctx->protocol == COAP_PROTO_TCP || ctx->protocol == COAP_PROTO_TLS) {
			//response->transport_hdr->udp.code = COAP_RESPONSE_CODE(503);
		} else {
			/* Should not enter here */
		}
		return;
	}

	option = coap_check_option2(request, COAP_OPTION_URI_QUERY, &opt_iter, transport);
	if (option) {
		unsigned char *p = COAP_OPT_VALUE(option);

		delay = 0;
		for (size = COAP_OPT_LENGTH(option); size; --size, ++p) {
			delay = delay * 10 + (*p - '0');
		}
	}

	async = coap_register_async(ctx, peer, request, COAP_ASYNC_SEPARATE | COAP_ASYNC_CONFIRM, (void *)(COAP_TICKS_PER_SECOND * delay));
}

void check_async(coap_context_t *ctx, coap_tick_t now)
{
	coap_pdu_t *response;
	coap_async_state_t *tmp;

	size_t size = sizeof(coap_hdr_t) + 8;

	if (!async || now < async->created + (unsigned long)async->appdata) {
		return;
	}

	response = coap_pdu_init(async->flags & COAP_ASYNC_CONFIRM ? COAP_MESSAGE_CON : COAP_MESSAGE_NON, COAP_RESPONSE_CODE(205), 0, size);
	if (!response) {
		debug("check_async : insufficient memory, we'll try later\n");
		async->appdata = (void *)((unsigned long)async->appdata + 15 * COAP_TICKS_PER_SECOND);
		return;
	}

	response->transport_hdr->udp.id = coap_new_message_id(ctx);

	if (async->tokenlen) {
		coap_add_token(response, async->tokenlen, async->token);
	}

	coap_add_data(response, 4, (unsigned char *)"done");

	if (ctx->protocol == COAP_PROTO_UDP || ctx->protocol == COAP_PROTO_DTLS) {
		if (coap_send(ctx, &async->peer, response) == COAP_INVALID_TID) {
			debug("check_async : cannot send response for message %d\n", response->transport_hdr->udp.id);
		}
		coap_delete_pdu(response);
	} else if (ctx->protocol == COAP_PROTO_TCP || ctx->protocol == COAP_PROTO_TLS) {
		coap_pdu_t *tcp_resp = NULL;
		tcp_resp = coap_convert_to_tcp_pdu(response);
		if (tcp_resp != NULL) {
			warn("check_async : failed to create tcp response\n");
		} else {
			if (coap_send(ctx, &async->peer, tcp_resp) == COAP_INVALID_TID) {
				debug("check_async : cannot send TCP response for message %d\n", response->transport_hdr->udp.id);
			}
			coap_delete_pdu(tcp_resp);
		}
	} else {
		/* Should not enter here */
	}

	coap_remove_async(ctx, async->id, &tmp);
	coap_free_async(async);
	async = NULL;
}
#endif							/* WITHOUT_ASYNC */

void init_resources(coap_context_t *ctx)
{
	coap_resource_t *r;

	r = coap_resource_init(NULL, 0, 0);
	coap_register_handler(r, COAP_REQUEST_GET, hnd_get_index);

	coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
	coap_add_attr(r, (unsigned char *)"title", 5, (unsigned char *)"\"General Info\"", 14, 0);
	coap_add_resource(ctx, r);

	/* store clock base to use in /time */
	my_clock_base = clock_offset;

	r = coap_resource_init((unsigned char *)"time", 4, 0);
	coap_register_handler(r, COAP_REQUEST_GET, hnd_get_time);
	coap_register_handler(r, COAP_REQUEST_PUT, hnd_put_time);
	//coap_register_handler(r, COAP_REQUEST_DELETE, hnd_delete_time);

	coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
	coap_add_attr(r, (unsigned char *)"title", 5, (unsigned char *)"\"Internal Clock\"", 16, 0);
	coap_add_attr(r, (unsigned char *)"rt", 2, (unsigned char *)"\"Ticks\"", 7, 0);
	r->observable = 1;
	coap_add_attr(r, (unsigned char *)"if", 2, (unsigned char *)"\"clock\"", 7, 0);

	coap_add_resource(ctx, r);
	time_resource = r;

#ifndef WITHOUT_ASYNC
	r = coap_resource_init((unsigned char *)"async", 5, 0);
	coap_register_handler(r, COAP_REQUEST_GET, hnd_get_async);

	coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
	coap_add_resource(ctx, r);
#endif							/* WITHOUT_ASYNC */
}

static void usage(const char *program, const char *version)
{
	const char *p;

	p = strrchr(program, '/');
	if (p) {
		program = ++p;
	}

	fprintf(stderr, "%s v%s -- a small CoAP implementation\n"
					"(c) 2010,2011 Olaf Bergmann <bergmann@tzi.org>\n\n"
					"usage: %s [-A address] [-p port] [-v level] [-P protocol]\n\n"
					"\t-A address\tinterface address to bind to\n"
					"\t-p port\t\tlisten on specified port\n"
					"\t-v num\t\tverbosity level (default: 3)\n"
#ifdef WITH_MBEDTLS
					"\t-P protocol\t\t type of transport protocol\n"
					"\t\t\t\t - 0 : UDP, 1 : DTLS , 2 : TCP, 3 : TLS (default : 0)\n"
					"\t-i identity\tPre-Shared Key identity used to security session\n"
					"\t-s pre-shared key\tPre-Shared Key. Input length MUST be even (e.g, 11, 1111.)\n"
#else
					"\t-P protocol\t\t type of transport protocol\n"
					"\t\t\t\t - 0 : UDP, 2 : TCP (default : 0)\n"
#endif /* WITH_MBEDTLS */
					"\t-Q exit server program\n"
					, program, version, program);
}

#if defined(__TINYARA__)
int coap_server_test_run(void *arg)
#else
int main(int argc, char **argv)
#endif
{
	coap_context_t *ctx = NULL;
	fd_set readfds;
	struct timeval tv, *timeout;
	int result;
	coap_tick_t now;
	coap_queue_t *nextpdu;
	char *addr_str = NULL;
	char port_str[NI_MAXSERV] = {0,};
	int opt;
	int invalid_opt = 0;
	int portChanged = 0;

	coap_log_t log_level = LOG_WARNING;
	coap_protocol_t protocol = COAP_PROTO_UDP;

#if defined(__TINYARA__)
	int argc;
	char **argv;

	argc = ((struct coap_server_input *)arg)->argc;
	argv = ((struct coap_server_input *)arg)->argv;
#endif

#ifdef WITH_MBEDTLS
	char *pskId = "coaptest";
	char *pskBuffer = "12345678";
	unsigned char psk[MBEDTLS_PSK_MAX_LEN];

	tls_opt tls_option;
	tls_ctx *tls_context = NULL;

	memset(&tls_option, 0, sizeof(tls_opt));

	tls_option.server = MBEDTLS_SSL_IS_SERVER;
	tls_option.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
	tls_option.auth_mode = 2;
	tls_option.debug_mode = 3;
#endif

	while ((opt = getopt(argc, argv, "A:p:v:P:i:s:Q")) != -1) {
		switch (opt) {
		case 'A':
			addr_str = (char *)coap_malloc(sizeof(char) * NI_MAXHOST);
			if (addr_str == NULL) {
				coap_log(LOG_CRIT, "memory allocation failure\n");
				return -1;
			}
			strncpy(addr_str, optarg, NI_MAXHOST-1);
			addr_str[NI_MAXHOST - 1] = '\0';
			printf("coap-server : address %s\n", addr_str);
			break;
		case 'p':
			strncpy(port_str, optarg, NI_MAXSERV-1);
			port_str[NI_MAXSERV - 1] = '\0';
			portChanged = 1;
			printf("coap-server : port %s\n", port_str);
			break;
		case 'v':
			log_level = strtol(optarg, NULL, 10);
			printf("coap-server : log_level %d\n", log_level);
			break;
		case 'Q':
			quit = 1;
			break;
		case 'P':
			protocol = strtol(optarg, NULL, COAP_PROTO_MAX);
			printf("coap-server : protocol type %d\n", protocol);
			break;
#ifdef WITH_MBEDTLS
		case 's':
			pskBuffer = optarg;
			printf("coap-server : Pre-Shared Key %s\n", pskBuffer);
			break;
		case 'i':
			pskId = optarg;
			printf("coap-server : Pre-Shared Key ID %s\n", pskId);
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
			printf("coap-server : not-supported protocol %d\n", protocol);
			return -1;
		}
	}

	/* Exit program when invalid argument is passed from command line*/
	if (invalid_opt)
		return 0;

	if (g_operating_status == SERVER_RUNNING) {
		if (quit) {
			printf("coap-server : set quit flag %d\n", quit);
			while (g_operating_status != SERVER_STOPPED) {
				usleep(100000);
			}
			printf("coap-server : server process has been stopped\n");
			if (addr_str) {
				free(addr_str);
			}
		} else {
			printf("coap-server : another libcoap-server is running\n");
		}

		return 0;
	}

	coap_set_log_level(log_level);

	ctx = coap_create_context(protocol);
	if (!ctx) {
		printf("coap-server : failed to create context\n");
		return -1;
	}

#ifdef WITH_MBEDTLS
	/* Initialize TLS context */
	if (protocol == COAP_PROTO_TLS || protocol == COAP_PROTO_DTLS) {
		tls_option.transport = (protocol == COAP_PROTO_TLS) ?
			(MBEDTLS_SSL_TRANSPORT_STREAM) : (MBEDTLS_SSL_TRANSPORT_DATAGRAM);
		tls_cred cred;
		memset(&cred, 0, sizeof(tls_cred));

		if (pskBuffer) {
			if (coap_unhexify(psk, pskBuffer, &cred.psk_len) == 0) {
				if (pskId) {
					cred.psk_identity = pskId;
					cred.psk = psk;
				}
			}
			if (cred.psk_identity == NULL && cred.psk == NULL) {
				printf("coap-server : failed to set psk info\n");
				goto exit;
			}
		} else {
			printf("coap-server : need to set psk and psk ID\n");
			goto exit;
		}

		tls_context = TLSCtx(&cred);
		if (tls_context == NULL) {
			printf("coap-server : failed to initialize TLS context\n");
			goto exit;
		}
	}
#else
	if (protocol == COAP_PROTO_TLS || protocol == COAP_PROTO_DTLS) {
		printf("coap-server : not supported protocol\n");
		goto exit;
	}
#endif /* WITH_MBEDTLS */

#ifdef WITH_MBEDTLS
	if (coap_net_bind(ctx, NULL, port_str, (void *)tls_context, (void *)&tls_option) < 0)
#else
	if (coap_net_bind(ctx, NULL, port_str, NULL, NULL) < 0)
#endif
	{
		printf("coap-server : failed to get session from client\n");
		goto exit;
	}

	init_resources(ctx);

#if !defined(__TINYARA__)
	signal(SIGINT, handle_sigint);
#endif

	g_operating_status = SERVER_RUNNING;
	printf("coap-server : coap_server is started, sockfd %d\n", ctx->sockfd);

	while (!quit) {
		FD_ZERO(&readfds);
		FD_SET(ctx->sockfd, &readfds);

		nextpdu = coap_peek_next(ctx);

		coap_ticks(&now);
		while (nextpdu && nextpdu->t <= now - ctx->sendqueue_basetime) {
			coap_retransmit(ctx, coap_pop_next(ctx));
			nextpdu = coap_peek_next(ctx);
		}

		if (nextpdu && nextpdu->t <= COAP_RESOURCE_CHECK_TIME) {
			/* set timeout if there is a pdu to send before our automatic timeout occurs */
			tv.tv_usec = ((nextpdu->t) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
			tv.tv_sec = (nextpdu->t) / COAP_TICKS_PER_SECOND;
			timeout = &tv;
		} else {
			tv.tv_usec = 0;
			tv.tv_sec = COAP_RESOURCE_CHECK_TIME;
			timeout = &tv;
		}
		result = select(FD_SETSIZE, &readfds, 0, 0, timeout);

		if (result < 0) {		/* error */
			printf("ERROR : failed on select, errno %d\n", errno);
			break;
		} else if (result > 0) {	/* read from socket */
			if (FD_ISSET(ctx->sockfd, &readfds)) {
				coap_read(ctx);	/* read received data */
				coap_dispatch(ctx);	/* and dispatch PDUs from receivequeue */
			}
		} else {				/* timeout */
			if (time_resource) {
				time_resource->dirty = 1;
			}
		}

#ifndef WITHOUT_ASYNC
		/* check if we have to send asynchronous responses */
		check_async(ctx, now);
#endif							/* WITHOUT_ASYNC */

#ifndef WITHOUT_OBSERVE
		/* check if we have to send observe notifications */
		coap_check_notify(ctx);
#endif							/* WITHOUT_OBSERVE */
	}

exit:
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
#ifdef WITH_TCP
	if (ctx->listen_sockfd > 0) {
		close(ctx->listen_sockfd);
	}
#endif

	coap_free_context(ctx);
	if (addr_str) {
		coap_free(addr_str);
	}

	/* initialize global variables */
	quit = 0;
	g_operating_status = SERVER_STOPPED;

	printf("coap-server : good bye\n");

	return 0;
}

#if defined(__TINYARA__)
int coap_server_test_main(int argc, char **argv)
{
	int status;

	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	struct coap_server_input arg;

	status = pthread_attr_init(&attr);

	if (status != 0) {
		printf("coap_server_test_main : failed to start coap-client\n");
		return -1;
	}

	sparam.sched_priority = COAP_SERVER_SCHED_PRI;
	if ((status = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		printf("coap_server_test_main : failed pthread_attr_setschedparam, errno %d\n", errno);
		return -1;
	}

	if ((status = pthread_attr_setschedpolicy(&attr, COAP_SERVER_SCHED_POLICY)) != 0) {
		printf("coap_server_test_main : failed pthread_attr_setschedpolicy, errno %d\n", errno);
		return -1;
	}

	if ((status = pthread_attr_setstacksize(&attr, COAP_SERVER_STACK_SIZE)) != 0) {
		printf("coap_server_test_main : failed pthread_attr_setstacksize, errno %d\n", errno);
		return -1;
	}

	arg.argc = argc;
	arg.argv = argv;

	if ((status = pthread_create(&tid, &attr, (pthread_startroutine_t)coap_server_test_run, &arg)) < 0) {
		printf("coap_server_test_main : failed to run coap-client, errno %d\n", errno);
		return -1;
	}

	pthread_setname_np(tid, "coap-server-test");
	pthread_join(tid, NULL);

	return 0;
}
#endif /* __TINYARA__ */
