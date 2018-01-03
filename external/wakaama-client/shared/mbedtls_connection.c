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
#include <tls/see_api.h>
#include <tls/base64.h>
#include <tls/pem.h>

#include "connection.h"

/* Needed for Mac OS X */
#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif

typedef struct {
    int transport;
    int auth_mode;
    int fd;
    int *ciphersuites;
    unsigned char *psk;
    int psk_len;
    unsigned char *psk_identity;
    int psk_identity_len;
    char *root_ca;
    unsigned char *device_cert;
    int device_cert_len;
    unsigned char *device_private_key;
    int device_private_key_len;
    bool use_se;
} lwm2m_config_ssl_t;

// from commandline.c
void output_buffer(FILE * stream, uint8_t * buffer, int length, int indent);
static char *security_get_public_id(lwm2m_object_t * obj, int instanceId, int * length);
static char *security_get_secret_key(lwm2m_object_t * obj, int instanceId, int * length);

static int security_get_security_mode(lwm2m_object_t * obj, int instanceId)
{
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 2; // security mode

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (dataP != NULL &&
            dataP->type == LWM2M_TYPE_INTEGER)
    {
        int val = dataP->value.asInteger;
        lwm2m_free(dataP);
        return val;
    }
    else
    {
        lwm2m_free(dataP);
        return -1;
    }
}

static void context_ssl_free(context_ssl_t *ssl)
{
    mbedtls_ssl_config_free(&ssl->config);
    mbedtls_entropy_free(&ssl->entropy);
    mbedtls_ctr_drbg_free(&ssl->ctr_drbg);
    mbedtls_ssl_free(&ssl->session);
    mbedtls_net_free(&ssl->net);

    if (ssl->clicert) {
        mbedtls_x509_crt_free(ssl->clicert);
        free(ssl->clicert);
        ssl->clicert = NULL;
    }

    if (ssl->device_cert) {
        mbedtls_x509_crt_free(ssl->device_cert);
        free(ssl->device_cert);
        ssl->device_cert = NULL;
    }

    if (ssl->pkey) {
        mbedtls_pk_free(ssl->pkey);
        free(ssl->pkey);
        ssl->pkey = NULL;
    }

    if (ssl->timer) {
        free(ssl->timer);
        ssl->timer = NULL;
    }
}

static char *security_get_server_public(lwm2m_object_t * obj, int instanceId, int * length)
{
    int size = 1;
    lwm2m_data_t * dataP = lwm2m_data_new(size);
    dataP->id = 4; // server public key or id

    obj->readFunc(instanceId, &size, &dataP, obj);
    if (dataP != NULL &&
            dataP->type == LWM2M_TYPE_OPAQUE)
    {
        char *val = (char*)dataP->value.asBuffer.buffer;
        *length = dataP->value.asBuffer.length;
        lwm2m_free(dataP);
        return val;
    }
    else
    {
        lwm2m_free(dataP);
        return NULL;
    }
}

static void lwm2m_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
#ifdef WITH_LOGS
    fprintf(stderr, "%04: |%d| %s", line, level, str);
#endif
}

static bool ssl_mbedtls_init(context_ssl_t *ssl, lwm2m_config_ssl_t *lwm2m_ssl_config)
{
    mbedtls_ssl_config_init(&ssl->config);
    mbedtls_entropy_init(&ssl->entropy);
    mbedtls_ctr_drbg_init(&ssl->ctr_drbg);
    mbedtls_ssl_init(&ssl->session);

    if (lwm2m_ssl_config->root_ca) {
        ssl->clicert = lwm2m_malloc(sizeof(mbedtls_x509_crt));
        if (!ssl->clicert) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to allocate clicert\r\n");
#endif
            goto error;
        }
        mbedtls_x509_crt_init(ssl->clicert);
    }

    if ((lwm2m_ssl_config->device_private_key || lwm2m_ssl_config->use_se)
        && lwm2m_ssl_config->device_cert) {
        ssl->device_cert = lwm2m_malloc(sizeof(mbedtls_x509_crt));
        if (!ssl->device_cert) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to allocate device_cert");
#endif
            goto error;
        }
        mbedtls_x509_crt_init(ssl->device_cert);
        ssl->pkey = lwm2m_malloc(sizeof(mbedtls_pk_context));
        if (!ssl->pkey) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to allocate pkey");
#endif
        }
        mbedtls_pk_init(ssl->pkey);
    }

    if (mbedtls_ssl_config_defaults(&ssl->config,
                                    MBEDTLS_SSL_IS_CLIENT,
                                    lwm2m_ssl_config->transport,
                                    MBEDTLS_SSL_PRESET_DEFAULT) != 0) {
#ifdef WITH_LOGS
        fprintf(stderr, "mbedtls_ssl_config_defaults failed");
#endif
        goto error;
    }

    mbedtls_ssl_conf_authmode(&ssl->config, lwm2m_ssl_config->auth_mode);

    if (lwm2m_ssl_config->ciphersuites)
        mbedtls_ssl_conf_ciphersuites(&ssl->config, lwm2m_ssl_config->ciphersuites);

    if (lwm2m_ssl_config->psk && lwm2m_ssl_config->psk_identity) {
        if (mbedtls_ssl_conf_psk(&ssl->config, lwm2m_ssl_config->psk, lwm2m_ssl_config->psk_len,
                                 lwm2m_ssl_config->psk_identity, lwm2m_ssl_config->psk_identity_len) != 0)
        {
#ifdef WITH_LOGS
            fprintf(stderr, "mbedtls_ssl_conf_psk failed.\r\n");
#endif
            goto error;
        }
    } else if (lwm2m_ssl_config->device_cert && lwm2m_ssl_config->device_private_key) {
        if (mbedtls_x509_crt_parse(ssl->device_cert,
                                   lwm2m_ssl_config->device_cert,
                                   lwm2m_ssl_config->device_cert_len) != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to parse device certificate.\r\n");
#endif
            goto error;
        }

        int ret = mbedtls_pk_parse_key(ssl->pkey,
                                       lwm2m_ssl_config->device_private_key,
                                       lwm2m_ssl_config->device_private_key_len,
                                       NULL, 0);
        if (ret != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to parse device private key (err %d).\r\n", ret);
#endif
            goto error;
        }

        if (mbedtls_ssl_conf_own_cert(&ssl->config, ssl->device_cert, ssl->pkey) != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to configure device cert/key.\r\n");
#endif
            goto error;
        }
    } else if (lwm2m_ssl_config->device_cert && lwm2m_ssl_config->use_se) {
        if (mbedtls_x509_crt_parse(ssl->device_cert,
                                   lwm2m_ssl_config->device_cert,
                                   lwm2m_ssl_config->device_cert_len) != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to parse device certificate.\r\n");
#endif
            goto error;
        }

        if (mbedtls_pk_setup(ssl->pkey, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY)) != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to setup private key info.\r\n");
#endif
            goto error;
        }

        ((mbedtls_ecdsa_context *)(ssl->pkey->pk_ctx))->grp.id =
            MBEDTLS_ECP_DP_SECP256R1;
        ((mbedtls_ecdsa_context *)(ssl->pkey->pk_ctx))->key_index =
            FACTORYKEY_ARTIK_DEVICE;

        if (mbedtls_ssl_conf_own_cert(&ssl->config, ssl->device_cert, ssl->pkey) != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to configure device cert/key.\r\n");
#endif
            goto error;
        }
    }

    if (lwm2m_ssl_config->root_ca) {
        int ret = mbedtls_x509_crt_parse(ssl->clicert,
                                   (unsigned char*)lwm2m_ssl_config->root_ca,
                                   strlen(lwm2m_ssl_config->root_ca)+1);
        if (ret != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to parse Root CA (%d) (%s)\n", ret, lwm2m_ssl_config->root_ca);
#endif
            goto error;
        }

        mbedtls_ssl_conf_ca_chain(&ssl->config, ssl->clicert,NULL);
    }

    if (mbedtls_ctr_drbg_seed(&ssl->ctr_drbg, mbedtls_entropy_func, &ssl->entropy, NULL, 0) != 0) {
#ifdef WITH_LOGS
        fprintf(stderr, "mbedtls_ctr_drbg_seed failed");
#endif
        goto error;
    }

    mbedtls_ssl_conf_rng(&ssl->config, mbedtls_ctr_drbg_random, &ssl->ctr_drbg);
    mbedtls_ssl_conf_dbg(&ssl->config, lwm2m_tls_debug, stderr);
    ssl->net.fd = lwm2m_ssl_config->fd;

    if (mbedtls_ssl_setup(&ssl->session, &ssl->config) != 0)
    {
#ifdef WITH_LOGS
        fprintf(stderr, "mbedtls_ssl_setup failed");
#endif
        goto error;
    }

    if (lwm2m_ssl_config->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM) {
        ssl->timer = malloc(sizeof(mbedtls_timing_delay_context));
        mbedtls_ssl_set_timer_cb(&ssl->session, ssl->timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);
        mbedtls_ssl_set_bio(&ssl->session, &ssl->net, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);
    } else {
        mbedtls_ssl_set_bio(&ssl->session, &ssl->net, mbedtls_net_send, mbedtls_net_recv, NULL);
    }

#ifdef MBEDTLS_DEBUG_C
    mbedtls_debug_set_threshold(0);
#endif

    return true;

error:
    context_ssl_free(ssl);
    return false;
}

static bool ssl_configure_certificate_mode(connection_t *conn)
{
    static lwm2m_config_ssl_t lwm2m_ssl_config;

    conn->ssl = lwm2m_malloc(sizeof(context_ssl_t));
    if (!conn->ssl) {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to allocate memory for ssl context\r\n");
#endif
        return false;
    }

    memset(&lwm2m_ssl_config, 0, sizeof(lwm2m_config_ssl_t));
    memset(conn->ssl, 0, sizeof(context_ssl_t));

    if (conn->protocol == COAP_UDP_DTLS && conn->sec_obj) {
        lwm2m_ssl_config.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
        lwm2m_ssl_config.ciphersuites = NULL;
        lwm2m_ssl_config.auth_mode = MBEDTLS_SSL_VERIFY_NONE;
    } else {
        lwm2m_ssl_config.transport = MBEDTLS_SSL_TRANSPORT_STREAM;
        lwm2m_ssl_config.ciphersuites = NULL;
        lwm2m_ssl_config.auth_mode = conn->verify_cert ? MBEDTLS_SSL_VERIFY_REQUIRED : MBEDTLS_SSL_VERIFY_NONE;

        if (conn->root_ca) {
            lwm2m_ssl_config.root_ca = strdup(conn->root_ca);
        }
    }

    lwm2m_ssl_config.device_cert =
        (unsigned char *)security_get_public_id(conn->sec_obj, conn->sec_inst, &lwm2m_ssl_config.device_cert_len);
    if (!lwm2m_ssl_config.device_cert) {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to get Public ID\r\n");
#endif
        goto error;
    }

    if (!conn->use_se) {
        int device_private_key_der_len = 0;
        unsigned char *device_private_key_der = 
            (unsigned char *)security_get_secret_key(conn->sec_obj, conn->sec_inst,
                                                     &device_private_key_der_len);
        if (!device_private_key_der) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to get private key\r\n");
#endif
            goto error;
        }

        if (mbedtls_pem_write_buffer("-----BEGIN EC PRIVATE KEY-----\n",
                                     "-----END EC PRIVATE KEY-----\n",
                                     device_private_key_der,
                                     device_private_key_der_len,
                                     NULL,
                                     0,
                                     &lwm2m_ssl_config.device_private_key_len)
			!= MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to parse private key\r\n");
#endif
            goto error;
        }

        lwm2m_ssl_config.device_private_key = lwm2m_malloc(lwm2m_ssl_config.device_private_key_len);
        if (!lwm2m_ssl_config.device_private_key) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to allocate private key\r\n");
#endif
            goto error;
        }

        if (mbedtls_pem_write_buffer("-----BEGIN EC PRIVATE KEY-----\n",
                                     "-----END EC PRIVATE KEY-----\n",
                                     device_private_key_der,
                                     device_private_key_der_len,
                                     lwm2m_ssl_config.device_private_key,
                                     lwm2m_ssl_config.device_private_key_len,
                                     &lwm2m_ssl_config.device_private_key_len)
            != 0) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to parse DER private key\r\n");
#endif
            goto error;
        }
    }

    lwm2m_ssl_config.use_se = conn->use_se;

    lwm2m_ssl_config.fd = conn->sock;
    if (!ssl_mbedtls_init(conn->ssl, &lwm2m_ssl_config)) {
        goto error;
    }

    if (lwm2m_ssl_config.device_private_key)
        lwm2m_free(lwm2m_ssl_config.device_private_key);

    return true;

error:
    if (lwm2m_ssl_config.device_private_key)
        lwm2m_free(lwm2m_ssl_config.device_private_key);

    lwm2m_free(conn->ssl);

    return false;
}

static bool ssl_configure_pre_shared_key(connection_t *conn)
{
    lwm2m_config_ssl_t lwm2m_ssl_config;
    int ciphersuites[2];
    ciphersuites[0] = MBEDTLS_TLS_PSK_WITH_AES_128_CCM_8;
    ciphersuites[1] = 0;

    conn->ssl = malloc(sizeof(context_ssl_t));
    if (!conn->ssl) {
#ifdef WITH_LOGS
        fprintf(stderr, "Failed to allocate memory for ssl context\r\n");
#endif
        return false;
    }

    memset(&lwm2m_ssl_config, 0, sizeof(lwm2m_config_ssl_t));
    memset(conn->ssl, 0, sizeof(context_ssl_t));

    if (conn->protocol == COAP_UDP_DTLS && conn->sec_obj) {
        lwm2m_ssl_config.psk_identity =
            (unsigned char*)security_get_public_id(conn->sec_obj, conn->sec_inst, &lwm2m_ssl_config.psk_identity_len);
        if (!lwm2m_ssl_config.psk_identity) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to get Public ID\r\n");
#endif
            goto error;
        }

        if (lwm2m_ssl_config.psk_identity_len > MAX_DTLS_INFO_LEN) {
#ifdef WITH_LOGS
            fprintf(stderr, "Public ID is too long %d\n", lwm2m_ssl_config.psk_identity_len);
#endif
            goto error;
        }

        lwm2m_ssl_config.psk =
            (unsigned char *)security_get_secret_key(conn->sec_obj, conn->sec_inst, &lwm2m_ssl_config.psk_len);
        if (!lwm2m_ssl_config.psk) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to get PSK\r\n");
#endif
            goto error;
        }

        if (lwm2m_ssl_config.psk_len > MAX_DTLS_INFO_LEN) {
#ifdef WITH_LOGS
            fprintf(stderr, "PSK is too long\r\n");
#endif
            goto error;
        }

        lwm2m_ssl_config.transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
        lwm2m_ssl_config.ciphersuites = ciphersuites;
        lwm2m_ssl_config.auth_mode = MBEDTLS_SSL_VERIFY_NONE;
    } else {
        lwm2m_ssl_config.transport = MBEDTLS_SSL_TRANSPORT_STREAM;
        lwm2m_ssl_config.ciphersuites = NULL;
        lwm2m_ssl_config.auth_mode = conn->verify_cert ? MBEDTLS_SSL_VERIFY_REQUIRED : MBEDTLS_SSL_VERIFY_NONE;
        lwm2m_ssl_config.root_ca = conn->root_ca;
    }

    lwm2m_ssl_config.fd = conn->sock;
    if (!ssl_mbedtls_init(conn->ssl, &lwm2m_ssl_config)) {
        goto error;
    }

    return true;

error:
    free(conn->ssl);

    return false;
}

static bool ssl_init(connection_t * conn)
{
    char *cert = NULL;
    uint8_t securityMode = security_get_security_mode(conn->sec_obj, conn->sec_inst);
    if (securityMode == LWM2M_SECURITY_MODE_PRE_SHARED_KEY) {
        if (!ssl_configure_pre_shared_key(conn)) {
            return false;
        }
    } else if (securityMode == LWM2M_SECURITY_MODE_CERTIFICATE) {
        if (!ssl_configure_certificate_mode(conn)) {
            return false;
        }
    } else {
        return false;
    }

    int ret;
    while ((ret = mbedtls_ssl_handshake(&conn->ssl->session)) != 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
#ifdef WITH_LOGS
        fprintf(stderr, "Handshake failed  (err %04X) !!\n", -ret);
#endif
            goto error;
        }
    }

    if (securityMode == LWM2M_SECURITY_MODE_CERTIFICATE && conn->protocol == COAP_UDP_DTLS) {
        const mbedtls_x509_crt *peer_cert = mbedtls_ssl_get_peer_cert(&conn->ssl->session);
        int len = 0;
        cert = security_get_server_public(conn->sec_obj, conn->sec_inst, &len);

        if (!cert || (len < 0)) {
#ifdef WITH_LOGS
            fprintf(stderr, "Failed to get server certificate\n");
#endif
            goto error;
        }

        if (!((len == peer_cert->raw.len) && (memcmp(peer_cert->raw.p, cert, len) == 0))) {
#ifdef WITH_LOGS
            fprintf(stderr, "server.serverCertificate does not match peer certificate.\n");
#endif
            goto error;
        }
    }

    return true;

error:
    context_ssl_free(conn->ssl);
    free(conn->ssl);
    if (cert)
        free(cert);
    return false;
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
        char *val = (char*)dataP->value.asBuffer.buffer;
        *length = dataP->value.asBuffer.length;
        lwm2m_free(dataP);
        return val;
    }
    else
    {
        lwm2m_free(dataP);
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
        char *val = (char*)dataP->value.asBuffer.buffer;
        *length = dataP->value.asBuffer.length;
        lwm2m_free(dataP);
        return val;
    }
    else
    {
        lwm2m_free(dataP);
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

    if (conn->ssl) {
        context_ssl_free(conn->ssl);
        free(conn->ssl);
        conn->ssl = NULL;
    }

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
                                conn->root_ca,
                                conn->verify_cert,
                                conn->use_se,
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
                                 char *root_ca,
                                 bool verify_cert,
                                 bool use_se,
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

        if (root_ca)
            connP->root_ca = strdup(root_ca);

        memcpy(&(connP->addr), sa, sl);
        connP->host = strndup(host, strlen(host));
        connP->addrLen = sl;
        strncpy(connP->local_port, remote_port, 16);
        strncpy(connP->remote_port, remote_port, 16);
        connP->address_family = addressFamily;
        connP->sec_obj = sec_obj;
        connP->sec_inst = sec_inst;
        connP->use_se = use_se;

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

    if (servinfo)
        freeaddrinfo(servinfo);

    connP->connected = true;

    return connP;

error:
    if (servinfo)
        freeaddrinfo(servinfo);

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
        connection_t * nextP = connList->next;
        if (connList->host)
            free(connList->host);

        if (connList->ssl) {
            context_ssl_free(connList->ssl);
            free(connList->ssl);
        }

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
            nbSent = mbedtls_ssl_write(&connP->ssl->session, buffer + offset, length - offset);
            if (nbSent < 1) {
#ifdef WITH_LOGS
                fprintf(stderr, "SSL Send error: %d\n", nbSent);
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

int connection_read(connection_t *connP, uint8_t * buffer, size_t size) {
    int numBytes;

    switch(connP->protocol)
    {
        case COAP_UDP:
            numBytes = recvfrom(connP->sock, buffer, size, 0, NULL, NULL);
            if (numBytes < 0)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
#endif
                return 0;
            }
            break;
        case COAP_TCP:
            numBytes = recv(connP->sock, buffer, size, 0);
            if (numBytes < 0)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "Error in recv(): %d %s\r\n", errno, strerror(errno));
#endif
                return 0;
            }
            break;
        case COAP_UDP_DTLS:
        case COAP_TCP_TLS:
            numBytes = mbedtls_ssl_read(&connP->ssl->session, buffer, size);
            if (numBytes < 1)
            {
                return 0;
            }
            break;
        default:
#ifdef WITH_LOGS
            fprintf(stderr, "Error protocol = %d is not supported.\r\n", connP->protocol);
#endif
            return 0;
    }

    return numBytes;
}
