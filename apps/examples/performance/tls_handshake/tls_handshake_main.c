/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "tls_handshake_usage.h"

extern int tls_handshake_server(void);
extern int tls_handshake_client(char *ipaddr);

int tls_handshake_main(int argc, char **argv)
{
	if (argc == 2 && !strncmp("-s", argv[1], 3)) {
		tls_handshake_server();
		return 0;
	} else if (argc == 3 && !strncmp("-c", argv[1], 3)) {
		tls_handshake_client(argv[2]);
		return 0;
	}

	printf("%s\n", TLS_HANDSHAKE_USAGE);
	return -1;
}
