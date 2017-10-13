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
 *    David Navarro, Intel Corporation - initial API and implementation
 *
 *******************************************************************************/

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <liblwm2m.h>

#ifndef WITH_MBEDTLS
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#else
#include <tls/easy_tls.h>
#include <tls/certs.h>
#endif

#define LWM2M_STANDARD_PORT_STR "5683"
#define LWM2M_STANDARD_PORT      5683
#define LWM2M_DTLS_PORT_STR     "5684"
#define LWM2M_DTLS_PORT          5684
#define LWM2M_BSSERVER_PORT_STR "5685"
#define LWM2M_BSSERVER_PORT      5685

#ifdef WITH_MBEDTLS
typedef struct
{
    mbedtls_ssl_config config;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_net_context net;
    mbedtls_ssl_context session;
    mbedtls_timing_delay_context *timer;
    mbedtls_x509_crt *clicert;
    mbedtls_x509_crt *device_cert;
    mbedtls_pk_context *pkey;
} context_ssl_t;
#endif

typedef struct _connection_t
{
    struct _connection_t *  next;
    int                     sock;
    struct sockaddr_in6     addr;
    size_t                  addrLen;
    coap_protocol_t         protocol;
#ifndef WITH_MBEDTLS
    SSL_CTX               * ssl_ctx;
    SSL                   * ssl;
#else
    context_ssl_t          * ssl;
#endif
    char                  * host;
    char                  * root_ca;
    bool                    verify_cert;
    char                    local_port[16];
    char                    remote_port[16];
    int                     address_family;
    lwm2m_object_t        * sec_obj;
    int                     sec_inst;
    bool                    connected;
    bool                    use_se;
} connection_t;

#define MAX_DTLS_INFO_LEN    128
typedef struct _lwm2m_dtls_info_
{
    struct _lwm2m_dtls_info_ * next;  // matches lwm2m_list_t::next
    uint16_t                   id;    // matches lwm2m_list_t::id
    connection_t             * connection;
    char                       identity[MAX_DTLS_INFO_LEN];
    char                       key[MAX_DTLS_INFO_LEN];
    int                        key_length;
} lwm2m_dtls_info_t;

int create_socket(coap_protocol_t protocol, const char * portStr, int ai_family);

connection_t *connection_find(connection_t *connList, struct sockaddr_storage *addr,
        size_t addrLen);

connection_t *connection_create(coap_protocol_t protocol, char *root_ca, bool verify_cert,
        bool use_se, int sock, char *host, char *local_port, char *remote_port, int addressFamily,
        lwm2m_object_t * obj, int instanceId);

int connection_restart(connection_t *conn);

void connection_free(connection_t * connList);

int connection_send(connection_t *connP, uint8_t * buffer, size_t length);

int connection_read(connection_t *connP, uint8_t * buffer, size_t length);
#endif
