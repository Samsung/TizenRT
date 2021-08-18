/*
 *	SSL client demonstration program
 *
 *	Copyright The Mbed TLS Contributors
 *	SPDX-License-Identifier: Apache-2.0
 *
 *	Licensed under the Apache License, Version 2.0 (the "License"); you may
 *	not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *	WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>

extern int ssl_client(void);
extern int ssl_server(void);

int hello_main(int argc, char *argv[])
{
	if (argc == 2) {
		int cmd = atoi(argv[1]);
		if (cmd == 1) {
			printf("run ssl client\n");
			ssl_client();
		} else if (cmd == 2) {
			printf("run ssl server\n");
			ssl_server();
		}
	}
	printf("Hello World1 !!\n");
	return 0;
}
