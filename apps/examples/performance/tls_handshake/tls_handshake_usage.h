/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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

#define TLS_HANDSHAKE_SERVER_USAGE \
	"\ntls_handshake -s\n"             \
	"example: tls_handshake -s\n"

#define TLS_HANDSHAKE_CLIENT_USAGE    \
	"\ntls_handshake -c <target_address>\n" \
	"example: tls_handshake -c 127.0.0.1\n"

#define TLS_HANDSHAKE_USAGE        \
	"usage: tls_handshake <mode>\n" \
	"mode : client, server\n" \
TLS_HANDSHAKE_CLIENT_USAGE \
TLS_HANDSHAKE_SERVER_USAGE
