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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NETLOGGER_BUF_SIZE 256

int netlogger_init(netmgr_logger_p *log)
{
	printf("[pkbuild] --> %s\n", __FUNCTION__);
	netmgr_logger *tlog = (netmgr_logger *)malloc(sizeof(netmgr_logger));
	if (!tlog) {
		return -1;
	}
	tlog->idx = 0;
	tlog->buf = NULL;
	tlog->size = 0;

	*log = tlog;
	return 0;
}

void netlogger_deinit(netmgr_logger_p log)
{
	printf("[pkbuild] --> %s\n", __FUNCTION__);
	if (log) {
		if (log->buf) {
			free(log->buf);
		}
		free(log);
	}
}

int netlogger_debug_msg(netmgr_logger_p log, const char *format, ...)
{
	printf("[pkbuild] --> %s\n", __FUNCTION__);
	va_list argp;
	char str[NETLOGGER_BUF_SIZE];
	int ret;

	va_start(argp, format);
	ret = vsnprintf(str, NETLOGGER_BUF_SIZE, format, argp);
	va_end(argp);

	printf("ret %d\n", ret);
	int remain = log->size - log->idx;
	printf("remain %d\n", remain);

	if (ret > remain) {
		log->size =
			((log->idx + ret) / NETLOGGER_BUF_SIZE + 1) * NETLOGGER_BUF_SIZE;
		printf("log->size %d\n", log->size);
		log->buf = realloc(log->buf, log->size);
		if (!log->buf) {
			printf("realloc fail\n");
			return -1;
		}
	}
	memcpy(log->buf + log->idx, str, ret);
	log->idx += ret;
}

/*
 * return: return size of buffer if success, else return -1
 *
 * note: a caller has responsibile to free buf.
 */
int netlogger_serialize(netmgr_logger_p log, char **buf)
{
	printf("[pkbuild] --> %s\n", __FUNCTION__);
	printf("[pkbuild] size %d idx %d\n", log->size, log->idx);
	*buf = (char *)malloc(log->idx + 1); // pad 1bypte for null character
	if (!*buf) {
		return -1;
	}
	memcpy(*buf, log->buf, log->idx);
	(*buf)[log->idx] = 0;

	return log->idx;
}
