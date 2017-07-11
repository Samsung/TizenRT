/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Gregory Lemercier, Samsung Semiconductor - support for TCP/TLS
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <net/lwip/tcp.h>

#include "connection.h"

/* Needed for Mac OS X */
#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif

static lwm2m_dtls_info_t *dtlsinfo_list = NULL;

// from commandline.c
void output_buffer(FILE * stream, uint8_t * buffer, int length, int indent);
static char *security_get_public_id(lwm2m_object_t * obj, int instanceId, int * length);
static char *security_get_secret_key(lwm2m_object_t * obj, int instanceId, int * length);

#ifndef WITH_MBEDTLS
static unsigned int psk_client_cb(SSL *ssl, const char *hint, char *identity,
                                  unsigned int max_identity_len,
                                  unsigned char *psk,
                                  unsigned int max_psk_len)
{
    int keyLen = 0;
    lwm2m_dtls_info_t *dtls = dtlsinfo_list;
    char *id = NULL, *key = NULL;

    // Look up DTLS info based on SSL pointer
    while (dtls != NULL)
    {
        if (ssl == dtls->connection->ssl)
        {
            id = dtls->identity;
            key = dtls->key;
            keyLen = dtls->key_length;
            break;
        }

        dtls = dtls->next;
    }

    if(!id || !key)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "Could not find DTLS credentials\n");
#endif
        return 0;
    }

    if (strlen(id) > max_identity_len)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "PSK identity is too long\n");
#endif
        return 0;
    }

    strncpy(identity, id, max_identity_len);

    if (keyLen > max_psk_len)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "PSK key is too long\n");
#endif
        return 0;
    }

    memcpy(psk, key, keyLen);

#ifdef WITH_LOGS
    {
        int i = 0;
        fprintf(stdout, "id: %s\n", identity);
        fprintf(stdout, "Key:");
        for (i=0; i<keyLen; i++)
            fprintf(stdout, "%02x", psk[i]);
        fprintf(stdout, "\n");
    }
#endif

    return keyLen;
}
#endif

static bool ssl_init(connection_t * conn)
{
#ifndef WITH_MBEDTLS
    BIO *sbio = NULL;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    int flags = 0;
    int ret = 0;

    OpenSSL_add_all_algorithms();
    ERR_clear_error();
    ERR_load_BIO_strings();
    SSL_load_error_strings();

    if(SSL_library_init() < 0)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to initialize OpenSSL\n");
#endif
        goto error;
    }

    if (conn->protocol == COAP_UDP_DTLS)
    {
        ctx = SSL_CTX_new(DTLS_client_method());
        SSL_CTX_set_psk_client_callback(ctx, psk_client_cb);
        SSL_CTX_set_cipher_list(ctx, "PSK-AES128-CCM8:PSK-AES128-CBC-SHA");
    }
    else
    {
        ctx = SSL_CTX_new(TLS_client_method());
        SSL_CTX_set_cipher_list(ctx, "ALL");
        SSL_CTX_set_verify(ctx, conn->verify_cert ? SSL_VERIFY_PEER : SSL_VERIFY_NONE, NULL);
        SSL_CTX_set_default_verify_dir(ctx);

        /* Ignore SIGPIPE to avoid the program from exiting on closed socket */
        signal(SIGPIPE, SIG_IGN);
    }

    if (!ctx)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to create SSL context\n");
#endif
        goto error;
    }

    ssl = SSL_new(ctx);
    if (!ssl)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to allocate SSL connection\n");
#endif
        goto error;
    }

    if (conn->protocol == COAP_UDP_DTLS)
    {
        struct sockaddr peer;
        int peerlen = sizeof (struct sockaddr);
        struct timeval timeout;
        int ret;
        int handshake_timeout = 50;
        int oldflags = fcntl (conn->sock, F_GETFL, 0);

        oldflags |= O_NONBLOCK;
        fcntl(conn->sock, F_SETFL, oldflags);

        sbio = BIO_new_dgram (conn->sock, BIO_NOCLOSE);
        if (getsockname (conn->sock, &peer, (socklen_t *)&peerlen) < 0)
        {
#ifdef WITH_LOGS
            fprintf(stderr, "getsockname failed (%s)\n", strerror (errno));
#endif
        }

        BIO_ctrl_set_connected (sbio, &peer);
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        BIO_ctrl(sbio, BIO_CTRL_DGRAM_SET_SEND_TIMEOUT, 0, &timeout);
        BIO_ctrl(sbio, BIO_CTRL_DGRAM_MTU_DISCOVER, 0, NULL);
        SSL_set_bio (ssl, sbio, sbio);
        SSL_set_connect_state (ssl);

        conn->ssl = ssl;
        conn->ssl_ctx = ctx;

        do {
            ret = SSL_do_handshake(ssl);
            if (ret < 1)
            {
                switch (SSL_get_error(ssl, ret))
                {
                case SSL_ERROR_WANT_READ:
                case SSL_ERROR_WANT_WRITE:
                    break;
                default:
#ifdef WITH_LOGS
                    fprintf(stderr, "%s: SSL error: %s\n", __func__,
                            ERR_error_string(SSL_get_error(ssl, ret), NULL));
#endif
                    goto error;
                }

                usleep(100*1000);
                if (handshake_timeout-- <= 0) {
#ifdef WITH_LOGS
                    fprintf(stderr, "%s: SSL handshake timed out\n", __func__);
#endif
                    goto error;
                }
            }
        } while(ret != 1);

        oldflags &= O_NONBLOCK;
        fcntl(conn->sock, F_SETFL, oldflags);
    }
    else
    {
        sbio = BIO_new_socket(conn->sock, BIO_NOCLOSE);
        if (!sbio)
        {
#ifdef WITH_LOGS
            fprintf(stderr, "%s: failed to create socket BIO\n", __func__);
#endif
            goto error;
        }

        SSL_set_bio (ssl, sbio, sbio);
        ret = SSL_connect(ssl);
        if (ret < 1)
        {
#ifdef WITH_LOGS
            fprintf(stderr, "%s: SSL handshake failed\n", __func__);
#endif
            ERR_print_errors_fp(stderr);
            goto error;
        }

        conn->ssl = ssl;
        conn->ssl_ctx = ctx;
    }

    return true;

error:
    if (ssl)
        SSL_free(ssl);
    if (ctx)
        SSL_CTX_free(ctx);
    return false;
#else
	/**
	 * FIXME:
	 *   "wakaama-client" Linux version use SSL_VERIFY_NONE option.
	 *   So, set tls_opt.auth_mode to 0
	 */
	tls_opt option = {
		MBEDTLS_SSL_IS_CLIENT,
		MBEDTLS_SSL_TRANSPORT_DATAGRAM,
		0,
		5,
		NULL
	};
	option.force_ciphersuites[0] = 0;
	option.force_ciphersuites[1] = 0;

	if (conn->protocol == COAP_TCP_TLS) {
		option.transport = MBEDTLS_SSL_TRANSPORT_STREAM;
	}
	else if (conn->protocol == COAP_UDP_DTLS) {
		option.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
	}
	conn->tls_opt = &option;

	tls_cred cred;
	memset(&cred, 0, sizeof(tls_cred));

	/**
	 * FIXME:
	 *   The tls_set_cred() API returns fail if ca_cert / dev_cert / psk
	 *   option are all empty.
	 *   So, fill the useless value to psk and psk_identify to avoid fail
	 *   return. (it will always success because tls_opt.auth_mode is 0)
	 */
	cred.psk_identity = "no verify";
	cred.psk = (unsigned char *)"no verify";
	cred.psk_len = strlen((char *)cred.psk);

	conn->tls_context = TLSCtx(&cred);
	if (conn->tls_context == NULL) {
		fprintf(stdout, "TLS context initialize failed\r\n");
		return false;
	}

	conn->session = TLSSession(conn->sock, conn->tls_context, conn->tls_opt);
	if (conn->session == NULL) {
		fprintf(stderr, "Failed to create secure session. \r\n");
		return false;
	}
#ifdef WITH_LOGS
	fprintf(stderr, "successfully create secure session. \r\n");
#endif
#endif

	return true;
}

static char *security_get_public_id(lwm2m_object_t * obj, int instanceId, int * length)
{
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 3; // public key or id

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (dataP != NULL &&
            dataP->type == LWM2M_TYPE_OPAQUE)
    {
        *length = dataP->value.asBuffer.length;
        return (char*)dataP->value.asBuffer.buffer;
    }
    else
    {
        return NULL;
    }
}

static char *security_get_secret_key(lwm2m_object_t * obj, int instanceId, int * length)
{
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 5; // secret key

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (dataP != NULL &&
            dataP->type == LWM2M_TYPE_OPAQUE)
    {
        *length = dataP->value.asBuffer.length;
        return (char*)dataP->value.asBuffer.buffer;
    }
    else
    {
        return NULL;
    }
}

int connection_restart(connection_t *conn)
{
    int sock;
    connection_t *newConn = NULL;

    conn->connected = false;

    /* Close previous connection */
    close(conn->sock);

#ifndef WITH_MBEDTLS
    if (conn->ssl) {
        SSL_free(conn->ssl);
        conn->ssl = NULL;
    }
    if (conn->ssl_ctx) {
        SSL_CTX_free(conn->ssl_ctx);
        conn->ssl_ctx = NULL;
    }
#else
	if (conn->session) {
		TLSSession_free(conn->session);
	}
#endif

    /* Increase port in case of TCP connections to avoid TIME_WAIT issue */
    if ((conn->protocol == COAP_TCP) || (conn->protocol == COAP_TCP_TLS))
    {
        char portStr[16];
        snprintf(portStr, 16, "%d", atoi(conn->local_port) + 1);
        strncpy(conn->local_port, portStr, 16);
    }

    sock = create_socket(conn->protocol, conn->local_port, conn->address_family);
    if (sock <= 0)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to create new socket\n");
#endif
        return -1;
    }

    newConn = connection_create(conn->protocol,
                                conn->verify_cert,
                                sock,
                                conn->host,
                                conn->local_port,
                                conn->remote_port,
                                conn->address_family,
                                conn->sec_obj,
                                conn->sec_inst);

    if (!newConn)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to create new connection\n");
#endif
        close(sock);
        return -1;
    }

    if (conn->protocol == COAP_UDP_DTLS)
    {
        lwm2m_dtls_info_t *dtls = dtlsinfo_list;

        /* Delete old connection's DTLS info */
        while (dtls != NULL)
        {
            if (conn == dtls->connection)
            {
                lwm2m_dtls_info_t *node;
                dtlsinfo_list = (lwm2m_dtls_info_t *)LWM2M_LIST_RM(dtlsinfo_list, dtls->id, &node);
                free(node);
                break;
            }
            dtls = dtls->next;
        }

        /* Replace connection pointer in new DTLS info */
        dtls = dtlsinfo_list;
        while (dtls != NULL)
        {
            if (newConn == dtls->connection)
            {
                dtls->connection = newConn;
                break;
            }
            dtls = dtls->next;
        }
    }

    /*
     * Copy new connection on top of the old one to keep same pointer,
     * then dispose of the newly allocated memory
     */
    free(conn->host);
    memcpy(conn, newConn, sizeof(connection_t));
    free(newConn);

    return 0;
}

int create_socket(coap_protocol_t protocol, const char * portStr, int addressFamily)
{
    int s = -1;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = addressFamily;
    switch(protocol)
    {
    case COAP_TCP:
    case COAP_TCP_TLS:
        hints.ai_socktype = SOCK_STREAM;
        break;
    case COAP_UDP:
    case COAP_UDP_DTLS:
        hints.ai_socktype = SOCK_DGRAM;
        break;
    default:
        break;
    }

    hints.ai_flags = AI_PASSIVE;

    if (0 != getaddrinfo(NULL, portStr, &hints, &res))
    {
        return -1;
    }

    for(p = res ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            if (-1 == bind(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }
        }
    }

    freeaddrinfo(res);

    return s;
}

connection_t * connection_find(connection_t * connList,
                               struct sockaddr_storage * addr,
                               size_t addrLen)
{
    connection_t * connP;

    connP = connList;
    while (connP != NULL)
    {
        if ((connP->addrLen == addrLen)
         && (memcmp(&(connP->addr), addr, addrLen) == 0))
        {
            return connP;
        }
        connP = connP->next;
    }

    return connP;
}

connection_t * connection_create(coap_protocol_t protocol,
                                 bool verify_cert,
                                 int sock,
                                 char *host,
                                 char *local_port,
                                 char *remote_port,
                                 int addressFamily,
                                 lwm2m_object_t * sec_obj,
                                 int sec_inst)
{
    struct addrinfo hints;
    struct addrinfo *servinfo = NULL;
    struct addrinfo *p;
    int s;
    struct sockaddr *sa = NULL;
    socklen_t sl = 0;
    connection_t * connP = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = addressFamily;

    switch(protocol)
    {
    case COAP_TCP:
    case COAP_TCP_TLS:
        hints.ai_socktype = SOCK_STREAM;
        break;
    case COAP_UDP:
    case COAP_UDP_DTLS:
        hints.ai_socktype = SOCK_DGRAM;
        break;
    default:
        break;
    }

    if (0 != getaddrinfo(host, remote_port, &hints, &servinfo) || servinfo == NULL)
    {
        return NULL;
    }

    // we test the various addresses
    s = -1;
    for(p = servinfo ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            sa = p->ai_addr;
            sl = p->ai_addrlen;

            if (-1 == connect(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }
        }
    }

    if (s >= 0)
    {
        if (protocol != COAP_UDP)
        {
            if (connect(sock, sa, sl) < 0)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "Failed to connect to socket: %s\n", strerror(errno));
#endif
                close(sock);
                return NULL;
            }
        }

        /* Allocate and fill up connection structure */
        connP = (connection_t *)malloc(sizeof(connection_t));
        if (connP == NULL)
        {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to allocate memory for connection\n");
#endif
            return NULL;
        }

        memset(connP, 0, sizeof(connection_t));
        connP->sock = sock;
        connP->protocol = protocol;
        connP->verify_cert = verify_cert;
        memcpy(&(connP->addr), sa, sl);
        connP->host = strndup(host, strlen(host));
        connP->addrLen = sl;
        strncpy(connP->local_port, remote_port, 16);
        strncpy(connP->remote_port, remote_port, 16);
        connP->address_family = addressFamily;
        connP->sec_obj = sec_obj;
        connP->sec_inst = sec_inst;

        if ((protocol == COAP_UDP_DTLS) && sec_obj)
        {
            char *id = NULL, *psk = NULL;
            int len = 0;
            lwm2m_dtls_info_t *dtls = NULL;

            // Retrieve ID/PSK from security object for DTLS handshake
            dtls = malloc(sizeof(lwm2m_dtls_info_t));

            if (!sec_obj)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "No security object provided\n");
#endif
                goto error;
            }

            if (!dtls)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "Failed to allocate memory for DTLS security info\n");
#endif
                goto error;
            }

            memset(dtls, 0, sizeof(lwm2m_dtls_info_t));

            id = security_get_public_id(sec_obj, sec_inst, &len);
            if (len > MAX_DTLS_INFO_LEN)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "Public ID is too long\n");
#endif
                goto error;
            }

            memcpy(dtls->identity, id, len);

            psk = security_get_secret_key(sec_obj, sec_inst, &len);
            if (len > MAX_DTLS_INFO_LEN)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "Secret key is too long\n");
#endif
                goto error;
            }

            memcpy(dtls->key, psk, len);
            dtls->key_length = len;
            dtls->connection = connP;
            dtls->id = lwm2m_list_newId((lwm2m_list_t*)dtlsinfo_list);
            dtlsinfo_list = (lwm2m_dtls_info_t*)LWM2M_LIST_ADD((lwm2m_list_t*)dtlsinfo_list, (lwm2m_list_t*)dtls);
        }

        if ((protocol == COAP_TCP_TLS) ||
            (protocol == COAP_UDP_DTLS))
        {
            if (!ssl_init(connP))
            {
#ifdef WITH_LOGS
                fprintf(stderr, "Failed to initialize SSL session\n");
#endif
                goto error;
            }
        }
        close(s);
    }
    else
    {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to find responsive server\n");
#endif
        goto error;
    }

    if (NULL != servinfo)
        free(servinfo);

    connP->connected = true;

    return connP;

error:
    if (NULL != servinfo)
        free(servinfo);

    if (connP)
    {
        free(connP->host);
        free(connP);
        connP = NULL;
    }

    return NULL;
}

void connection_free(connection_t * connList)
{
    while (connList != NULL)
    {
        connection_t * nextP;
        lwm2m_dtls_info_t *dtls = dtlsinfo_list;

        // Free DTLS info if any
        while (dtls != NULL)
        {
            if (connList == dtls->connection)
            {
                lwm2m_dtls_info_t *node;
                dtlsinfo_list = (lwm2m_dtls_info_t *)LWM2M_LIST_RM(dtlsinfo_list, dtls->id, &node);
                free(node);
                break;
            }

            dtls = dtls->next;
        }

        nextP = connList->next;
        if (connList->host)
            free(connList->host);
        free(connList);

        connList = nextP;
    }
}

int connection_send(connection_t *connP,
                    uint8_t * buffer,
                    size_t length)
{
    int nbSent = 0;
    size_t offset;

    if (!connP->connected)
        return -1;

#ifdef WITH_LOGS
    char s[INET6_ADDRSTRLEN];
    in_port_t port = 0;

    s[0] = 0;

    if (AF_INET == connP->addr.sin6_family)
    {
        struct sockaddr_in *saddr = (struct sockaddr_in *)&connP->addr;
        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin_port;
    }
    else if (AF_INET6 == connP->addr.sin6_family)
    {
        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&connP->addr;
        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin6_port;
    }

    fprintf(stdout, "Sending %lu bytes to [%s]:%hu\r\n", length, s, ntohs(port));

    output_buffer(stderr, buffer, length, 0);
#endif

    offset = 0;
    while (offset != length)
    {
        switch(connP->protocol)
        {
        case COAP_UDP_DTLS:
        case COAP_TCP_TLS:
#ifndef WITH_MBEDTLS
            nbSent = SSL_write(connP->ssl, buffer + offset, length - offset);
#else
            nbSent = mbedtls_ssl_write(connP->session->ssl, buffer + offset, length - offset);
#endif
            if (nbSent < 1) {
#ifdef WITH_LOGS
#ifndef WITH_MBEDTLS
                fprintf(stderr, "SSL Send error: %s\n", ERR_error_string(SSL_get_error(connP->ssl, nbSent), NULL));
#else
                fprintf(stderr, "SSL Send error: %d\n", nbSent);
#endif
#endif
                return -1;
            }
            break;
        case COAP_TCP:
            nbSent = send(connP->sock, buffer + offset, length - offset, 0);
            if (nbSent == -1) {
#ifdef WITH_LOGS
                fprintf(stderr, "Send error: %s\n", strerror(errno));
#endif
                return -1;
            }
            break;
        case COAP_UDP:
            nbSent = sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
            if (nbSent == -1) {
#ifdef WITH_LOGS
                fprintf(stderr, "Send error: %s\n", strerror(errno));
#endif
                return -1;
            }
            break;
        default:
            break;
        }

        offset += nbSent;
    }
    return 0;
}

uint8_t lwm2m_buffer_send(void * sessionH,
                          uint8_t * buffer,
                          size_t length,
                          void * userdata)
{
    connection_t * connP = (connection_t*) sessionH;

    if (connP == NULL)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "#> failed sending %lu bytes, missing connection\r\n", length);
#endif
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (-1 == connection_send(connP, buffer, length))
    {
#ifdef WITH_LOGS
        fprintf(stderr, "#> failed sending %lu bytes, try reconnecting\r\n", length);
#endif
        connP->connected = false;
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    return COAP_NO_ERROR;
}

bool lwm2m_session_is_equal(void * session1,
                            void * session2,
                            void * userData)
{
    return (session1 == session2);
}
