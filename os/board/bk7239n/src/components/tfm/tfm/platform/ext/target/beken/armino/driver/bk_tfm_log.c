// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdarg.h>
#include "uart_stdout.h"
#include "components/log.h"
#include "driver/efuse.h"
#include "bk_tfm_log.h"
#include "sdkconfig.h"

int printf(const char *fmt, ...)
{
	char string[CONFIG_STDIO_PRINTF_BUF_SIZE] = {0};
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsnprintf(string, sizeof(string) - 1, fmt, ap);
	va_end(ap);

	string[CONFIG_STDIO_PRINTF_BUF_SIZE - 1] = 0;

	stdio_output_string((const unsigned char *)string, len);
	return len;
}

int printf_dummy(const char *fmt, ...)
{
	return 0;
}

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')

void printf_word_buf_hex(const uint8_t *ptr, size_t buflen)
{
	unsigned char *buf = (unsigned char*)ptr;
	uint32_t *word_buf;
	int i, j, unalign_cnt = ((uint32_t)buf & 0x3);

	if (!efuse_is_info_log_enabled()) {
		return;
	}

	for(i = 0; i < unalign_cnt; i ++)
	{
		printf("%08X: %02X", i, buf[i]);
	}

	word_buf = (uint32_t *)&buf[unalign_cnt];
	buflen = (buflen - unalign_cnt) >> 2;

	for (i = 0; i < buflen; i += 8)
	{
		printf("%08X: ", i);

		for (j=0; j<8; j++)
			if (i+j < buflen)
				printf("%08X ", word_buf[i+j]);
			else
				printf("   ");
		printf(" ");
		printf("\n");
	}
}

void printf_buf_hex(const uint8_t *ptr, size_t buflen)
{
	unsigned char *buf = (unsigned char*)ptr;
	int i, j;

	for (i=0; i<buflen; i+=16)
	{
		printf("%08X: ", i);

		for (j=0; j<16; j++)
			if (i+j < buflen)
				printf("%02X ", buf[i+j]);
			else
				printf("   ");
		printf(" ");

		for (j=0; j<16; j++)
			if (i+j < buflen)
				printf("%c", __is_print(buf[i+j]) ? buf[i+j] : '.');
		printf("\n");
	}
}

void bk_tfm_dump_buf(const char *str, const uint8_t *buf, uint32_t len)
{
	if (!efuse_is_info_log_enabled()) {
		return;
	}

	printf("%s\r\n", str);
	for (int i = 0; i < len; i++) {
		if (i && (i % 32) == 0) {
			printf("\r\n");
		}
		printf("%02x ", buf[i]);
	}
	printf("\r\n");
}
