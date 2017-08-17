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
#include "connection.h"

// from commandline.c
void output_buffer(FILE * stream, uint8_t * buffer, int length, int indent);

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

connection_t * connection_new_incoming(connection_t * connList,
                                       int sock,
                                       struct sockaddr * addr,
                                       size_t addrLen)
{
    connection_t * connP;

    connP = (connection_t *)malloc(sizeof(connection_t));
    if (connP != NULL)
    {
        connP->sock = sock;
        memcpy(&(connP->addr), addr, addrLen);
        connP->addrLen = addrLen;
        connP->next = connList;
    }

    return connP;
}

connection_t * connection_create(coap_protocol_t protocol,
                                 connection_t * connList,
                                 int sock,
                                 char * host,
                                 char * port,
                                 int addressFamily)
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

    switch(protocol) {
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

    if (0 != getaddrinfo(host, port, &hints, &servinfo) || servinfo == NULL) return NULL;

    // we test the various addresses
    s = -1;
    for(p = servinfo ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            sa = p->ai_addr;
#ifdef CONFIG_NET_LWIP
            sa->sa_len = p->ai_addrlen;
#endif
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
                fprintf(stderr, "Failed to connect to socket: %s\n", strerror(errno));
                close(sock);
                return NULL;
            }
        }
        connP = connection_new_incoming(connList, sock, sa, sl);
        if ((protocol == COAP_TCP_TLS) ||
            (protocol == COAP_UDP_DTLS))
        {
            /* ToDo : add TLS init routine : if (!ssl_init(connP)) */
            {
                fprintf(stderr, "Failed to initialize TLS session\n");
                goto error;
            }
        }
        close(s);
    }
    if (NULL != servinfo) {
#ifdef CONFIG_NET_LWIP
        freeaddrinfo(servinfo);
#else
        if (servinfo->ai_addr)
            free(servinfo->ai_addr);
        if (servinfo->ai_canonname)
            free(servinfo->ai_canonname);
        free(servinfo);
#endif
    }

    return connP;
error:
    if (NULL != servinfo)
#ifdef CONFIG_NET_LWIP
        freeaddrinfo(servinfo);
#else
        free(servinfo);
#endif

    if (connP)
    {
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

        nextP = connList->next;
        free(connList);

        connList = nextP;
    }
}

int connection_send(connection_t *connP,
                    uint8_t * buffer,
                    size_t length,
                    coap_protocol_t proto)
{
    int nbSent;
    size_t offset;

#ifdef WITH_LOGS
    char s[INET6_ADDRSTRLEN];
    in_port_t port;

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

    fprintf(stderr, "Sending %d bytes to [%s]:%hu\r\n", length, s, ntohs(port));

    output_buffer(stderr, buffer, length, 0);
#endif

    offset = 0;
    while (offset != length)
    {
        nbSent = 0;
        switch(proto)
        {
        case COAP_UDP_DTLS:
        case COAP_TCP_TLS:
            break;
        case COAP_TCP:
            nbSent = send(connP->sock, buffer + offset, length - offset, 0);
            if (nbSent == -1) {
                fprintf(stderr, "Send error: %s\n", strerror(errno));
                return -1;
            }
            break;
        case COAP_UDP:
            nbSent = sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
            if (nbSent == -1) {
                fprintf(stderr, "Sendto error: %s\n", strerror(errno));
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
                          void * userdata,
                          coap_protocol_t proto)
{
    connection_t * connP = (connection_t*) sessionH;

    if (connP == NULL)
    {
        fprintf(stderr, "#> failed sending %lu bytes, missing connection\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (-1 == connection_send(connP, buffer, length, proto))
    {
        fprintf(stderr, "#> failed sending %lu bytes\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    return COAP_NO_ERROR;
}

bool lwm2m_session_is_equal(void * session1,
                            void * session2,
                            void * userData)
{
    return (session1 == session2);
}

int connection_read(coap_protocol_t protocol,
                    int sock,
                    uint8_t *buffer,
                    size_t len,
                    struct sockaddr_storage *from,
                    socklen_t *fromLen)
{
    int numBytes = -1;

    switch(protocol) {
        case COAP_UDP:
            numBytes = recvfrom(sock, buffer, len, 0, (struct sockaddr *)from, fromLen);
            break;
        case COAP_TCP:
            numBytes = recv(sock, buffer, len, 0);
            break;
        case COAP_TCP_TLS:
        case COAP_UDP_DTLS:
            break;
        default:
            fprintf(stderr, "connection_read : unsupported protocol type : %d\n", protocol);
            break;
    }
    return numBytes;
}
