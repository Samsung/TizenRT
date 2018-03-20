#ifndef HEADER_CURL_SENDF_H
#define HEADER_CURL_SENDF_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2014, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include "curl_setup.h"

CURLcode Curl_sendf(curl_socket_t sockfd, struct connectdata *,
                    const char *fmt, ...);

#ifdef CONFIG_LOG_CURL_FAIL
#define failf CURL_fail
#else
#define failf(...)
#endif

#ifdef CONFIG_LOG_CURL_INFO
#define infof CURL_info
#else
#define infof(...)
#endif

#ifdef CONFIG_LOG_CURL_DEBUG
#define Curl_debug CURL_debug
#else
#define Curl_debug(...)
#endif

#define CLIENTWRITE_BODY   (1<<0)
#define CLIENTWRITE_HEADER (1<<1)
#define CLIENTWRITE_BOTH   (CLIENTWRITE_BODY|CLIENTWRITE_HEADER)

CURLcode Curl_client_chop_write(struct connectdata *conn, int type, char *ptr,
                                size_t len) WARN_UNUSED_RESULT;
CURLcode Curl_client_write(struct connectdata *conn, int type, char *ptr,
                           size_t len) WARN_UNUSED_RESULT;

bool Curl_recv_has_postponed_data(struct connectdata *conn, int sockindex);

/* internal read-function, does plain socket only */
CURLcode Curl_read_plain(curl_socket_t sockfd,
                         char *buf,
                         size_t bytesfromsocket,
                         ssize_t *n);

ssize_t Curl_recv_plain(struct connectdata *conn, int num, char *buf,
                        size_t len, CURLcode *code);
ssize_t Curl_send_plain(struct connectdata *conn, int num,
                        const void *mem, size_t len, CURLcode *code);

/* internal read-function, does plain socket, SSL and krb4 */
CURLcode Curl_read(struct connectdata *conn, curl_socket_t sockfd,
                   char *buf, size_t buffersize,
                   ssize_t *n);
/* internal write-function, does plain socket, SSL, SCP, SFTP and krb4 */
CURLcode Curl_write(struct connectdata *conn,
                    curl_socket_t sockfd,
                    const void *mem, size_t len,
                    ssize_t *written);

/* internal write-function, does plain sockets ONLY */
CURLcode Curl_write_plain(struct connectdata *conn,
                          curl_socket_t sockfd,
                          const void *mem, size_t len,
                          ssize_t *written);

#endif /* HEADER_CURL_SENDF_H */
