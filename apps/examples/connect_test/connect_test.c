/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <wifi_manager/wifi_manager.h>
#include "connect_test.h"
#include "connect_test_usage.h"
#include "connect_test_log.h"

#define TAG "[CT]"
#define CT_MIN_ARG 2

#define CT_TEST_FUNC_SIGNAL              \
	do {                                 \
		sem_post(&g_ct_func_sem);        \
		CT_LOG(TAG, "send func signal"); \
	} while (0)

#define CT_TEST_FUNC_WAIT                \
	do {                                 \
		CT_LOG(TAG, "wait func signal"); \
		sem_wait(&g_ct_func_sem);        \
	} while (0)

static sem_t g_wm_sem;
static sem_t g_ct_func_sem;
static int g_mode = 0; // check program is running

typedef int (*parser_func)(struct ct_options *opt, int argc, char *argv[]);
typedef enum {
#ifdef CT_MEMBER_POOL
#undef CT_MEMBER_POOL
#endif
#define CT_MEMBER_POOL(type, func, parser, str) type,
#include "connect_test_table.h"
	CT_TYPE_MAX,
	CT_TYPE_ERR = -1
} ct_type_e;

/* Handler */
#ifdef CT_MEMBER_POOL
#undef CT_MEMBER_POOL
#endif
#define CT_MEMBER_POOL(type, func, parser, str) static void func(void *arg);
#include "connect_test_table.h"

/* Parser */
#ifdef CT_MEMBER_POOL
#undef CT_MEMBER_POOL
#endif
#define CT_MEMBER_POOL(type, func, parser, str) \
static int parser(struct ct_options *opt, int argc, char *argv[]);
#include "connect_test_table.h"

test_func g_function_table[] = {
#ifdef CT_MEMBER_POOL
#undef CT_MEMBER_POOL
#endif
#define CT_MEMBER_POOL(type, func, parser, str) func,
#include "connect_test_table.h"
};

parser_func g_function_parser_table[] = {
#ifdef CT_MEMBER_POOL
#undef CT_MEMBER_POOL
#endif
#define CT_MEMBER_POOL(type, func, parser, str) parser,
#include "connect_test_table.h"
};

char *g_function_name[] = {
#ifdef CT_MEMBER_POOL
#undef CT_MEMBER_POOL
#endif
#define CT_MEMBER_POOL(type, func, parser, str) str,
#include "connect_test_table.h"
};

static int32_t temp_sum; //temp_sum is not used in stress test, it is kept to do proper function call

static int _parse_security_str(struct ct_options *opt, const char *type, char *pwd)
{
	opt->auth_type = ct_get_auth_type(type);
	opt->password = pwd;
	if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN ||
		opt->auth_type == WIFI_MANAGER_AUTH_IBSS_OPEN) {
		opt->crypto_type = WIFI_MANAGER_CRYPTO_NONE;
	} else if (opt->auth_type == WIFI_MANAGER_AUTH_WEP_SHARED) {
		if ((strlen(pwd) == 13) || (strlen(pwd) == 26)) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
		} else if ((strlen(pwd) == 5) || (strlen(pwd) == 10)) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
		} else {
			return -1;
		}
	} else {
		opt->crypto_type = ct_get_crypto_type(type);
		if (opt->crypto_type == WIFI_MANAGER_CRYPTO_UNKNOWN) {
			CT_LOGE(TAG, "crypto type unknown");
			return -1;
		}
	}
	return 0;
}

static int _ct_signal_init(void)
{
	int res = sem_init(&g_wm_sem, 0, 0);
	if (res != 0) {
		CT_LOGE(TAG, "g_wm_sem init failed");
		return -1;
	}

	res = sem_init(&g_ct_func_sem, 0, 0);
	if (res != 0) {
		CT_LOGE(TAG, "g_ct_func_sem init failed");
		return -1;
	}

	if (g_mode != 0) {
		CT_LOGE(TAG, "Program is already running");
		return -1;
	}
	
	g_mode = 1;
	return 0;
}

static void _ct_signal_deinit(void)
{
	sem_destroy(&g_wm_sem);
	sem_destroy(&g_ct_func_sem);	
	g_mode = 0;
}

void _ct_add_hostname(void *arg)
{
	connect_test_add_hostname(arg);
}

void _ct_server(void *arg)
{
	connect_test_server(arg);
}

void _ct_client(void *arg)
{
	connect_test_client(arg);
}

void _ct_udp_server(void *arg)
{
	struct ct_options *server_info = (struct ct_options *)arg;
	_udp_server(server_info->dtls_data_size, &temp_sum);
}

void _ct_udp_client(void *arg)
{
	struct ct_options *client_info = (struct ct_options *)arg;
	_udp_client(client_info->hostname, client_info->dtls_data_size);
}

void _ct_tcp_server(void *arg)
{
	struct ct_options *server_info = (struct ct_options *)arg;
	_tcp_server(server_info->tls_data_size, &temp_sum);
}

void _ct_tcp_client(void *arg)
{
	struct ct_options *client_info = (struct ct_options *)arg;
	_tcp_client(client_info->hostname, client_info->tls_data_size);
}

void _ct_dtls_server(void *arg)
{
	struct ct_options *server_info = (struct ct_options *)arg;
	_dtls_server(server_info->dtls_data_size, &temp_sum);
}

void _ct_dtls_client(void *arg)
{
	struct ct_options *client_info = (struct ct_options *)arg;
	_dtls_client(client_info->hostname, client_info->dtls_data_size);
}

void _ct_tls_server(void *arg)
{
	struct ct_options *server_info = (struct ct_options *)arg;
	_tls_server(server_info->tls_data_size, &temp_sum);
}

void _ct_tls_client(void *arg)
{
	struct ct_options *client_info = (struct ct_options *)arg;
	_tls_client(client_info->hostname, client_info->tls_data_size);
}

void _ct_dns_service(void *arg)
{
	struct ct_options *dns_options = (struct ct_options *)arg;
	dns_service(dns_options->path);
}

ct_type_e _ct_get_opt(int argc, char *argv[])
{
	int idx = 0;
	if (argc < CT_MIN_ARG) {
		return CT_TYPE_ERR;
	}

	for (idx = 0; idx < CT_TYPE_MAX; idx++) {		
		if (0 == strncmp(argv[1], g_function_name[idx], strlen(argv[1])+1)) {
			return (ct_type_e)idx;
		}
	}
	return CT_TYPE_ERR;
}

int _ct_parse_add_hostname(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 4) {
		CT_LOGE(TAG, "arg count not valid");
		return -1;
	}
	opt->path = argv[2];
	opt->hostname = argv[3];
	return 0;
}

int _ct_parse_server(struct ct_options *opt, int argc, char *argv[])
{
	if (argc < 9 || argc > 14) {
		CT_LOGE(TAG, "arg count not in valid range");
		return -1;
	}
	if (_parse_security_str(opt, argv[3], argv[4]) != 0) {
		CT_LOGE(TAG, "security parse failed");
		return -1;
	}
	opt->ssid = argv[2];
	opt->softap_ssid = argv[5];
	opt->softap_password = argv[6];
	opt->softap_channel = atoi(argv[7]);
	opt->repeat = atoi(argv[8]);

	int data_idx = 9;
	if (argc > data_idx && (!strncmp(argv[data_idx], "udp", 4))) {
		opt->is_dtls = 0;
		opt->dtls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else if (argc > data_idx && !strncmp(argv[data_idx], "dtls", 5)) {
		opt->is_dtls = 1;
		opt->dtls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else {
		opt->is_dtls = 0;
		opt->dtls_data_size = 64;
	}

	if (argc > data_idx && !strncmp(argv[data_idx], "tcp", 4)) {
		opt->is_tls = 0;
		opt->tls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else if (argc > data_idx && !strncmp(argv[data_idx], "tls", 4)) {
		opt->is_tls = 1;
		opt->tls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else {
		opt->is_tls = 0;
		opt->tls_data_size = 64;
	}

	int hostname_idx = data_idx;
	if (argc > hostname_idx) {
		opt->path = argv[hostname_idx];
	}
	return 0;
}

int _ct_parse_client(struct ct_options *opt, int argc, char *argv[])
{
	if (argc < 9 || argc > 13) {
		CT_LOGE(TAG, "arg count not in valid range");
		return -1;
	}
	if (_parse_security_str(opt, argv[3], argv[4]) != 0) {
		CT_LOGE(TAG, "security parse failed");
		return -1;
	}
	opt->ssid = argv[2];
	opt->softap_ssid = argv[5];
	opt->softap_password = argv[6];
	opt->softap_channel = atoi(argv[7]);
	opt->repeat = atoi(argv[8]);

	int data_idx = 9;
	if (argc > data_idx && (!strncmp(argv[data_idx], "udp", 4))) {
		opt->is_dtls = 0;
		opt->dtls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else if (argc > data_idx && !strncmp(argv[data_idx], "dtls", 5)) {
		opt->is_dtls = 1;
		opt->dtls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else {
		opt->is_dtls = 0;
		opt->dtls_data_size = 64;
	}

	if (argc > data_idx && !strncmp(argv[data_idx], "tcp", 4)) {
		opt->is_tls = 0;
		opt->tls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else if (argc > data_idx && !strncmp(argv[data_idx], "tls", 4)) {
		opt->is_tls = 1;
		opt->tls_data_size = atoi(argv[++data_idx]);
		data_idx++;
	} else {
		opt->is_tls = 0;
		opt->tls_data_size = 64;
	}
	return 0;
}

int _ct_parse_udp_server(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 3) {
		return -1;
	}
	opt->dtls_data_size = atoi(argv[2]);
	return 0;
}

int _ct_parse_udp_client(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 4) {
		return -1;
	}
	opt->hostname = argv[2];
	opt->dtls_data_size = atoi(argv[3]);
	return 0;
}

int _ct_parse_tcp_server(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 3) {
		return -1;
	}
	opt->tls_data_size = atoi(argv[2]);
	return 0;
}

int _ct_parse_tcp_client(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 4) {
		return -1;
	}
	opt->hostname = argv[2];
	opt->tls_data_size = atoi(argv[3]);
	return 0;
}

int _ct_parse_dtls_server(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 3) {
		return -1;
	}
	opt->dtls_data_size = atoi(argv[2]);
	return 0;
}

int _ct_parse_dtls_client(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 4) {
		return -1;
	}
	opt->hostname = argv[2];
	opt->dtls_data_size = atoi(argv[3]);
	return 0;
}

int _ct_parse_tls_server(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 3) {
		return -1;
	}
	opt->tls_data_size = atoi(argv[2]);
	return 0;
}

int _ct_parse_tls_client(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 4) {
		return -1;
	}
	opt->hostname = argv[2];
	opt->tls_data_size = atoi(argv[3]);
	return 0;
}

int _ct_parse_dns_service(struct ct_options *opt, int argc, char *argv[])
{
	if (argc != 3) {
		return -1;
	}
	opt->path = argv[2];
	return 0;
}

int _ct_parse_commands(struct ct_options *opt, int argc, char *argv[])
{
	int ret = 0;
	ct_type_e options = _ct_get_opt(argc, argv);
	if (options == CT_TYPE_ERR) {
		return -1;
	}

	opt->func = g_function_table[options];
	if (opt->func == NULL) {
		return -1;
	}

	if (g_function_parser_table[options]) {
		ret = (g_function_parser_table[options])(opt, argc, argv);
	}
	return ret;
}

void _ct_process(int argc, char *argv[])
{
	struct ct_options opt;
	memset(&opt, 0, sizeof(struct ct_options));
	opt.repeat = 1;
	opt.softap_channel = 1;

	int res = _ct_parse_commands(&opt, argc - 1, &argv[1]);
	if (res < 0) {
		CT_LOGE(TAG, "%s", CT_USAGE);
		goto exit;
	}

	opt.func((void *)&opt);
exit:
	CT_TEST_FUNC_SIGNAL;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int connect_test_main(int argc, char *argv[])
#endif
{
	CT_LOG(TAG, "connect test!!");
	int res = _ct_signal_init();
	if (res < 0) {
		CT_LOGE(TAG, "sem init failed");
		return -1;
	}

	task_create("connect test sample", 100, 1024 * 12, (main_t)_ct_process, argv);

	CT_TEST_FUNC_WAIT;
	CT_LOG(TAG, "wait func signal received");
	_ct_signal_deinit();
	CT_LOG(TAG, "connect test done");
	return 0;
}
