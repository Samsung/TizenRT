/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/// @file getfile_main.c
#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <arch/irq.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
/****************************************************************************
 * Definitions
 ****************************************************************************/
#define HANDSHAKE_STRING                "GETFILE"
#define HANDSHAKE_STR_LEN_MAX			(7)
#define FILEPATH_STR_LEN_MAX			(30)
/****************************************************************************
 * Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int getfile_main(int argc, char *argv[])
#endif
{
	int i;
	FILE *fp;
	int f_length = 0;
	int ch;
	uint8_t *ptr;
	char *fbuf;
	uint32_t size;
	int psize = 0;
	char *target_str = HANDSHAKE_STRING;
	char host_buf[HANDSHAKE_STR_LEN_MAX] = "";
	char file_path[FILEPATH_STR_LEN_MAX] = "";
	irqstate_t flags;
#if !defined(CONFIG_ARCH_LOWPUTC)
	/* If lowlevel serial is not available, getfile is not possible */
	return -1;
#endif
	/* Inform the terminal user */
	up_puts("****************************************************\n");
	up_puts("Disconnect this serial terminal and Run getfile Tool\n");
	up_puts("****************************************************\n");
	flags = irqsave();
	/* Receive hanshake string from HOST */
	do {
		host_buf[0] = up_getc();
	} while (host_buf[0] != target_str[0]);

	for (i = 1; i < strlen(target_str);) {
		if ((ch = up_getc()) != -1) {
			host_buf[i] = ch;
			i++;
		}
	}

	if (strncmp(host_buf, target_str, strlen(target_str)) != 0) {
		/* Send NAK */
		up_lowputc('N');
	}

	/* Send ACK */
	up_lowputc('A');

	irqrestore(flags);
	while (1) {

		flags = irqsave();
		for (i = 0; i < 1;) {
			if ((ch = up_getc()) == 'S') {
				irqrestore(flags);
				return OK;
			} else
				psize = ch;
			i++;
		}

		memset(file_path, 0, sizeof(file_path));
		do {
			file_path[0] = up_getc();
		} while (file_path[0] != '/');

		for (i = 1; i < psize;) {
			if ((ch = up_getc()) != -1) {
				file_path[i] = ch;
				i++;
			}
		}

		/* alloc memory for store file data */
		if ((fp = fopen(file_path, "r")) == NULL) {
			irqrestore(flags);
			return -1;
		}

		fseek(fp, 0, SEEK_END);

		f_length = ftell(fp);

		if ((fbuf = malloc(f_length)) == NULL) {
			irqrestore(flags);
			fclose(fp);
			return -1;
		}

		/* Send RAM address */
		for (i = 0; i < 4; i++) {
			up_lowputc(((char *)&fbuf)[i]);
		}

		size = f_length;
		/* Send RAM size */
		ptr = (uint8_t *)&size;
		for (i = 0; i < sizeof(size); i++) {
			up_lowputc((uint8_t) * ptr);
			ptr++;
		}

		rewind(fp);
		if (fread(fbuf, 1, f_length, fp) != f_length) {
			irqrestore(flags);
			fclose(fp);
			free(fbuf);
			return -1;
		}

		/* Send file data to HOST */
		ptr = (uint8_t *) fbuf;
		while (size) {
			up_lowputc((uint8_t) * ptr);
			ptr++;
			size--;
		}
		fclose(fp);
		free(fbuf);
		irqrestore(flags);
	}
	return OK;
}
