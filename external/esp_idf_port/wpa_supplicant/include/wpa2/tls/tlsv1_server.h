/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/*
 * TLS v1.0/v1.1/v1.2 server (RFC 2246, RFC 4346, RFC 5246)
 * Copyright (c) 2006-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef TLSV1_SERVER_H
#define TLSV1_SERVER_H

#include "tlsv1_cred.h"

struct tlsv1_server;

int tlsv1_server_global_init(void);
void tlsv1_server_global_deinit(void);
struct tlsv1_server * tlsv1_server_init(struct tlsv1_credentials *cred);
void tlsv1_server_deinit(struct tlsv1_server *conn);
int tlsv1_server_established(struct tlsv1_server *conn);
int tlsv1_server_prf(struct tlsv1_server *conn, const char *label,
		     int server_random_first, u8 *out, size_t out_len);
u8 * tlsv1_server_handshake(struct tlsv1_server *conn,
			    const u8 *in_data, size_t in_len, size_t *out_len);
int tlsv1_server_encrypt(struct tlsv1_server *conn,
			 const u8 *in_data, size_t in_len,
			 u8 *out_data, size_t out_len);
int tlsv1_server_decrypt(struct tlsv1_server *conn,
			 const u8 *in_data, size_t in_len,
			 u8 *out_data, size_t out_len);
int tlsv1_server_get_cipher(struct tlsv1_server *conn, char *buf,
			    size_t buflen);
int tlsv1_server_shutdown(struct tlsv1_server *conn);
int tlsv1_server_resumed(struct tlsv1_server *conn);
int tlsv1_server_get_keys(struct tlsv1_server *conn, struct tls_keys *keys);
int tlsv1_server_get_keyblock_size(struct tlsv1_server *conn);
int tlsv1_server_set_cipher_list(struct tlsv1_server *conn, u8 *ciphers);
int tlsv1_server_set_verify(struct tlsv1_server *conn, int verify_peer);

typedef int (*tlsv1_server_session_ticket_cb)
(void *ctx, const u8 *ticket, size_t len, const u8 *client_random,
 const u8 *server_random, u8 *master_secret);

void tlsv1_server_set_session_ticket_cb(struct tlsv1_server *conn,
					tlsv1_server_session_ticket_cb cb,
					void *ctx);

#endif /* TLSV1_SERVER_H */
