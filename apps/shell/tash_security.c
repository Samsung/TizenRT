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

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mbedtls/sha256.h>
#ifdef CONFIG_TASH_REBOOT
#include <sys/boardctl.h>
#endif
#include "tash_internal.h"

#define SHA256_HASH_nCHAR  (64) // 32 bytes
#define SHA256_HASH_nBYTE  (32) // 32 bytes
#define CONVERT_nCHAR      (sizeof(unsigned long) * 2) // unsigned long for strtoul

void tash_check_security(int fd)
{
	const char passwd_prompt[] = "PASSWORD>>";
	char *input;
	int nbytes;
	int input_len = 0;
	char pw_sha256_char[CONVERT_nCHAR + 1] = { 0, };
	unsigned char pw_sha256_hex[SHA256_HASH_nBYTE];
	unsigned char input_sha256_hex[SHA256_HASH_nBYTE];
	unsigned long converted_val;
	int sha256_hex_idx = 0;
	int retry_count = 0;
	int offset = 0;

	if (strlen(CONFIG_TASH_PASSWORD_SHA256) != SHA256_HASH_nCHAR) {
		printf("TASH: CONFIG_TASH_PASSWORD_SHA256 has wrong length (%d).\n", strlen(CONFIG_TASH_PASSWORD_SHA256));
		printf("      It should be 32 bytes, 64 characters.\n");
		return;
	}

	/* Convert password character to hex */
	do {
		strncpy(pw_sha256_char, ((char *)CONFIG_TASH_PASSWORD_SHA256 + offset), CONVERT_nCHAR);
		converted_val = strtoul(pw_sha256_char, NULL, 16);
		pw_sha256_hex[sha256_hex_idx++] = (unsigned char)((converted_val >> 24) & 0xFF);
		pw_sha256_hex[sha256_hex_idx++] = (unsigned char)((converted_val >> 16) & 0xFF);
		pw_sha256_hex[sha256_hex_idx++] = (unsigned char)((converted_val >> 8) & 0xFF);
		pw_sha256_hex[sha256_hex_idx++] = (unsigned char)(converted_val & 0xFF);
		offset += CONVERT_nCHAR;
	} while (offset < SHA256_HASH_nCHAR);

	do {
		/* Print secure prompt */
		nbytes = write(fd, passwd_prompt, sizeof(passwd_prompt));
		if (nbytes < sizeof(passwd_prompt)) {
			usleep(20);
			continue;
		}

		/* Read user input */
		input = tash_read_input_line(fd);
		input_len = strlen(input);
		if (input_len == 0) {
			tash_free(input);
			continue;
		}

		/* Allow reboot command regardless of TASH authentication */
		#ifdef CONFIG_TASH_REBOOT
		if (!strncmp((unsigned char *)input, "reboot", 7)) {
			(void)tash_reboot(0, NULL);
			return;
		}
		#endif

		/* Convert user input to SHA256 hash value */
		mbedtls_sha256((unsigned char *)input, input_len, input_sha256_hex, 0);

		tash_free(input);

		/* Confirm the password */
		if (!strncmp((char *)pw_sha256_hex, (char *)input_sha256_hex, SHA256_HASH_nBYTE)) {
			break;
		} else {
			retry_count++;
			if (retry_count % 3 == 0) {
				printf("TASH: It will be locked for 30sec\n");
				sleep(30);
			}
		}
	} while (1);
}
