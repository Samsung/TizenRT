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
/// @file   tash_security.c
/// @brief  functions to check secure tash command

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mbedtls/sha256.h>
#include "tash_internal.h"

void tash_check_security(int fd)
{
	char *line_buff;
	int i;
	int nbytes;
	char chr[1];
	unsigned char temp[2];

	unsigned char sha256[64];
	unsigned char result[64];
	unsigned char sha256_hex[64];
	int len = 0;
	const char passwd_prompt[] = "PASSWORD>>";
	int count = 0;

	do {
		nbytes = write(fd, passwd_prompt, sizeof(passwd_prompt));
		if (nbytes <= sizeof(passwd_prompt)) {
			usleep(20);
			continue;
		}
		line_buff = tash_read_input_line(fd);
		len = strlen(line_buff);

		sprintf((char *)sha256, "%s", CONFIG_TASH_PASSWORD_SHA256);

		for (i = 0; i < 64; i++) {
			strncpy(chr, (char *)&sha256[i], 1);
			temp[i % 2] = strtoul(chr, NULL, 16);

			if (i % 2) {
				sha256_hex[i / 2] = (temp[0] << 4) | temp[1];
			}
		}

		mbedtls_sha256((unsigned char *)line_buff, len, result, 0);

		tash_free(line_buff);

		if (!strncmp((char *)sha256_hex, (char *)result, 32)) {
			break;
		} else if (len != 0) {
			count++;
			if (count % 3 == 0) {
				printf("TASH: It will be locked for 30sec\n");
				sleep(30);
			}
		}
	} while (1);
}
