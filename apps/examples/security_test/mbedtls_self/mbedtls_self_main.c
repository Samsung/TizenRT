/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <protocols/ntpclient.h>

#include "wifi_manager/wifi_manager.h"

#include "mbedtls/entropy.h"
#include "mbedtls/entropy_poll.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/dhm.h"
#include "mbedtls/gcm.h"
#include "mbedtls/ccm.h"
#include "mbedtls/cmac.h"
#include "mbedtls/md2.h"
#include "mbedtls/md4.h"
#include "mbedtls/md5.h"
#include "mbedtls/ripemd160.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"
#include "mbedtls/arc4.h"
#include "mbedtls/des.h"
#include "mbedtls/aes.h"
#include "mbedtls/camellia.h"
#include "mbedtls/base64.h"
#include "mbedtls/bignum.h"
#include "mbedtls/rsa.h"
#include "mbedtls/x509.h"
#include "mbedtls/xtea.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ecjpake.h"
#include "mbedtls/timing.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#define MBEDTLS_PRINTF     printf
#define MBEDTLS_SNPRINTF   snprintf
#define MBEDTLS_EXIT       exit
#define MBEDTLS_EXIT_SUCCESS 0
#define MBEDTLS_EXIT_FAILURE -1
#endif

/*
 * wi-fi global
 */
#define MBEDTLS_WIFI_SSID "Gorani"
#define MBEDTLS_WIFI_PASSWORD "jonbeo1@"
static pthread_mutex_t wifi_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wifi_cond;

#define WIFI_WAIT                                   \
	do {                                            \
		pthread_mutex_lock(&wifi_mutex);            \
		printf(" t%d wait signal\n", getpid());     \
		pthread_cond_wait(&wifi_cond, &wifi_mutex); \
		pthread_mutex_unlock(&wifi_mutex);          \
	} while (0)

#define WIFI_SIGNAL                                 \
	do {                                            \
		pthread_mutex_lock(&wifi_mutex);            \
		printf("t%d send signal\n", getpid());      \
		pthread_cond_signal(&wifi_cond);            \
		pthread_mutex_unlock(&wifi_mutex);          \
	} while (0)

/*
 * wi-fi callbacks
 */
void sta_connected(wifi_manager_result_e res)
{
	printf(" t%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	WIFI_SIGNAL;
}

void sta_disconnected(wifi_manager_disconnect_e disconn)
{
	sleep(1);
	printf(" t%d --> %s\n", getpid(), __FUNCTION__);
	WIFI_SIGNAL;
}

static wifi_manager_cb_s wifi_callbacks = {
	sta_connected,
	sta_disconnected,
	NULL,
	NULL,
	NULL,
};

/*
 *  * signal
 *   */
int wifi_signal_init(void)
{
	int res = pthread_mutex_init(&wifi_mutex, NULL);
	if (res != 0) {
		printf(" pthread mutex init fail(%d)\n", res);
		return -1;
	}

	res = pthread_cond_init(&wifi_cond, NULL);
	if (res != 0) {
		printf(" conditional mutex init fail(%d)\n", res);
		return -1;
	}

	return 0;
}

void wifi_signal_deinit(void)
{
	pthread_mutex_destroy(&wifi_mutex);
	pthread_cond_destroy(&wifi_cond);
}

/* ntp client */
#ifdef CONFIG_NETUTILS_NTPCLIENT
static void ntp_link_error(void)
{
	printf("ntp_link_error() callback is called\n");
}

void *connect_ntp(void *data)
{
	if (ntpc_get_status() != NTP_RUNNING) {
		struct timespec pre_tp;
		clock_gettime(CLOCK_REALTIME, &pre_tp);

		struct timespec init_tp;
		init_tp.tv_sec = 0;
		init_tp.tv_nsec = 0;
		clock_settime(CLOCK_REALTIME, &init_tp);

		if (ntpc_start(NULL, 0, 0, ntp_link_error) < 0) {
			printf("ntpc_start() failed\n");
		} else {
			printf("ntpc_start() ok\n");
			int time_sync = 0;

			while (1) {
				struct timespec sync_tp;
				clock_gettime(CLOCK_REALTIME, &sync_tp);
				if ((init_tp.tv_sec + 1000) < sync_tp.tv_sec) {
					time_sync = 1;
					break;
				} else if ((init_tp.tv_sec + * ((int *)data)) < sync_tp.tv_sec) {
					break;
				}
				usleep(100000);
			}
			if (time_sync) {
				printf("ntpc_time sync done\n");
			} else {
				printf("ntpc_time sync fail\n");
				clock_settime(CLOCK_REALTIME, &pre_tp);
			}
		}
	} else {
		printf("ntpc already runnig\n");
	}

	if (ntpc_stop() < 0) {
		printf("ntcp_stop() failed.\n");
	}
	printf("ntpc_stop() ok.\n");

	return NULL;
}
#endif

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#include "mbedtls/memory_buffer_alloc.h"
#endif

static int test_snprintf(size_t n, const char ref_buf[10], int ref_ret)
{
	int ret;
	char buf[10] = "xxxxxxxxx";
	const char ref[10] = "xxxxxxxxx";

	ret = mbedtls_snprintf(buf, n, "%s", "123");
	if ((ret < 0) || ((size_t) ret >= n)) {
		ret = -1;
	}

	if ((strncmp(ref_buf, buf, sizeof(buf)) != 0) || (ref_ret != ret) || (memcmp(buf + n, ref + n, sizeof(buf) - n) != 0)) {
		return 1;
	}

	return 0;
}

static int run_test_snprintf(void)
{
	return test_snprintf(0, "xxxxxxxxx", -1) != 0 || test_snprintf(1, "", -1) != 0 || test_snprintf(2, "1", -1) != 0 || test_snprintf(3, "12", -1) != 0 || test_snprintf(4, "123", 3) != 0 || test_snprintf(5, "123", 3) != 0;
}

/*
 * check if a seed file is present, and if not create one for the entropy
 * self-test. if this fails, we attempt the test anyway, so no error is passed
 * back.
 */
#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_ENTROPY_C)
#if defined(MBEDTLS_ENTROPY_NV_SEED) && !defined(MBEDTLS_NO_PLATFORM_ENTROPY)
static void create_entropy_seed_file(void)
{
	int result;
	size_t output_len = 0;
	unsigned char seed_value[MBEDTLS_ENTROPY_BLOCK_SIZE];

	/* attempt to read the entropy seed file. if this fails - attempt to write
	 * to the file to ensure one is present. */
	result = mbedtls_platform_std_nv_seed_read(seed_value, MBEDTLS_ENTROPY_BLOCK_SIZE);
	if (0 == result) {
		return;
	}

	result = mbedtls_platform_entropy_poll(NULL, seed_value, MBEDTLS_ENTROPY_BLOCK_SIZE, &output_len);
	if (0 != result) {
		return;
	}

	if (MBEDTLS_ENTROPY_BLOCK_SIZE != output_len) {
		return;
	}
	mbedtls_platform_std_nv_seed_write(seed_value, MBEDTLS_ENTROPY_BLOCK_SIZE);
}
#endif

int mbedtls_entropy_self_test_wrapper(int verbose)
{
#if defined(MBEDTLS_ENTROPY_NV_SEED) && !defined(MBEDTLS_NO_PLATFORM_ENTROPY)
	create_entropy_seed_file();
#endif
	return mbedtls_entropy_self_test(verbose);
}
#endif

#if defined(MBEDTLS_SELF_TEST)
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
int mbedtls_memory_buffer_alloc_free_and_self_test(int verbose)
{
	if (verbose != 0) {
#if defined(MBEDTLS_MEMORY_DEBUG)
		mbedtls_memory_buffer_alloc_status();
#endif
	}
	mbedtls_memory_buffer_alloc_free();
	return mbedtls_memory_buffer_alloc_self_test(verbose);
}
#endif

struct selftest {
	const char *name;
	int (*function)(int);
};
typedef struct selftest selftest_t;

const selftest_t selftests[] = {
#if defined(MBEDTLS_MD2_C)
	{"md2", mbedtls_md2_self_test},
#endif
#if defined(MBEDTLS_MD4_C)
	{"md4", mbedtls_md4_self_test},
#endif
#if defined(MBEDTLS_MD5_C)
	{"md5", mbedtls_md5_self_test},
#endif
#if defined(MBEDTLS_RIPEMD160_C)
	{"ripemd160", mbedtls_ripemd160_self_test},
#endif
#if defined(MBEDTLS_SHA1_C)
	{"sha1", mbedtls_sha1_self_test},
#endif
#if defined(MBEDTLS_SHA256_C)
	{"sha256", mbedtls_sha256_self_test},
#endif
#if defined(MBEDTLS_SHA512_C)
	{"sha512", mbedtls_sha512_self_test},
#endif
#if defined(MBEDTLS_ARC4_C)
	{"arc4", mbedtls_arc4_self_test},
#endif
#if defined(MBEDTLS_DES_C)
	{"des", mbedtls_des_self_test},
#endif
#if defined(MBEDTLS_AES_C)
	{"aes", mbedtls_aes_self_test},
#endif
#if defined(MBEDTLS_GCM_C) && defined(MBEDTLS_AES_C)
	{"gcm", mbedtls_gcm_self_test},
#endif
#if defined(MBEDTLS_CCM_C) && defined(MBEDTLS_AES_C)
	{"ccm", mbedtls_ccm_self_test},
#endif
#if defined(MBEDTLS_CMAC_C)
	{"cmac", mbedtls_cmac_self_test},
#endif
#if defined(MBEDTLS_BASE64_C)
	{"base64", mbedtls_base64_self_test},
#endif
#if defined(MBEDTLS_BIGNUM_C)
	{"mpi", mbedtls_mpi_self_test},
#endif
#if defined(MBEDTLS_RSA_C)
	{"rsa", mbedtls_rsa_self_test},
#endif
#if defined(MBEDTLS_X509_USE_C)
	{"x509", mbedtls_x509_self_test},
#endif
#if defined(MBEDTLS_XTEA_C)
	{"xtea", mbedtls_xtea_self_test},
#endif
#if defined(MBEDTLS_CAMELLIA_C)
	{"camellia", mbedtls_camellia_self_test},
#endif
#if defined(MBEDTLS_CTR_DRBG_C)
	{"ctr_drbg", mbedtls_ctr_drbg_self_test},
#endif
#if defined(MBEDTLS_HMAC_DRBG_C)
	{"hmac_drbg", mbedtls_hmac_drbg_self_test},
#endif
#if defined(MBEDTLS_ECP_C)
	{"ecp", mbedtls_ecp_self_test},
#endif
#if defined(MBEDTLS_ECJPAKE_C)
	{"ecjpake", mbedtls_ecjpake_self_test},
#endif
#if defined(MBEDTLS_DHM_C)
	{"dhm", mbedtls_dhm_self_test},
#endif
#if defined(MBEDTLS_ENTROPY_C)
	{"entropy", mbedtls_entropy_self_test_wrapper},
#endif
#if defined(MBEDTLS_PKCS5_C)
	{"pkcs5", mbedtls_pkcs5_self_test},
#endif
	/* slower test after the faster ones */
#if defined(MBEDTLS_TIMING_C)
	{"timing", mbedtls_timing_self_test},
#endif
	/* heap test comes last */
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	{"memory_buffer_alloc", mbedtls_memory_buffer_alloc_free_and_self_test},
#endif
	{NULL, NULL}
};
#endif							/* mbedtls_self_test */

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, far char *argv[])
#else
int mbedtls_self_main(int argc, char *argv[])
#endif
{
#if defined(MBEDTLS_SELF_TEST)
	const selftest_t *test;
#endif							/* mbedtls_self_test */
	char **argp;
	int v = 1;					/* v=1 for verbose mode */
	int exclude_mode = 0;
	int suites_tested = 0;
	int suites_failed = 0;
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C) && defined(MBEDTLS_SELF_TEST)
	unsigned char buf[1000000];
#endif
	void *pointer;

	/* wi-fi connection and ntp */
	if (wifi_signal_init() < 0) {
		printf("wifi_signal_init fail\n");
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}

	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("wifi_manager_init fail\n");
		wifi_manager_deinit();
		wifi_signal_deinit();
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}

	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, MBEDTLS_WIFI_SSID, WIFIMGR_SSID_LEN);
	apconfig.ssid_length = strlen(MBEDTLS_WIFI_SSID);
	apconfig.ssid[WIFIMGR_SSID_LEN] = '\0';
	strncpy(apconfig.passphrase, MBEDTLS_WIFI_PASSWORD, WIFIMGR_PASSPHRASE_LEN);
	apconfig.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
	apconfig.passphrase_length = strlen(MBEDTLS_WIFI_PASSWORD);
	apconfig.ap_auth_type = WIFI_MANAGER_AUTH_WPA2_PSK;
	apconfig.ap_crypto_type = WIFI_MANAGER_CRYPTO_AES;

	printf("wi-fi connection: %s\n", MBEDTLS_WIFI_SSID);
	wifi_manager_result_e wifi_res = wifi_manager_connect_ap(&apconfig);
	if (wifi_res != WIFI_MANAGER_SUCCESS) {
		printf("ap connect failed\n");
		wifi_manager_deinit();
		wifi_signal_deinit();
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}
	/* wait for dhcp connection */
	printf("wait dhcp\n");
	WIFI_WAIT;

#ifdef CONFIG_NETUTILS_NTPCLIENT
	pthread_t p_ntp;
	int thr_id;
	int ntp_res;
	int guarantee_time = 10;
	thr_id = pthread_create(&p_ntp, NULL, connect_ntp, (void *)&guarantee_time);
	if (thr_id < 0) {
		printf("ntpclient thread open is failed\n");
		wifi_manager_deinit();
		wifi_signal_deinit();
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}
	pthread_join(p_ntp, (void *)&ntp_res);
	if (ntp_res < 0) {
		printf("ntpclient fails to make a synchronization\n");
		wifi_manager_deinit();
		wifi_signal_deinit();
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}
#endif
	/*
	 * the c standard doesn't guarantee that all-bits-0 is the representation
	 * of a NULL pointer. we do however use that in our code for initializing
	 * structures, which should work on every modern platform. let's be sure.
	 */
	memset(&pointer, 0, sizeof(void *));
	if (pointer != NULL) {
		mbedtls_printf("all-bits-zero is not a NULL pointer\n");
		wifi_manager_deinit();
		wifi_signal_deinit();
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}

	for (argp = argv + (argc >= 1 ? 1 : argc); *argp != NULL; ++argp) {
		if ((strcmp(*argp, "--quiet") == 0) || (strcmp(*argp, "-q") == 0)) {
			v = 0;
		} else if ((strcmp(*argp, "--exclude") == 0) || (strcmp(*argp, "-x") == 0)) {
			exclude_mode = 1;
		} else {
			break;
		}
	}

	if (v != 0) {
		mbedtls_printf("\n");
	}
#if defined(MBEDTLS_SELF_TEST)
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_init(buf, sizeof(buf));
#endif

	if (*argp != NULL && exclude_mode == 0) {
		/* run the specified tests */
		for (; *argp != NULL; argp++) {
			for (test = selftests; test->name != NULL; test++) {
				if (!strcmp(*argp, test->name)) {
					if (test->function(v) != 0) {
						suites_failed++;
					}
					suites_tested++;
					break;
				}
			}
			if (test->name == NULL) {
				mbedtls_printf("test suite %s not available -> failed\n\n", *argp);
				suites_failed++;
			}
		}
	} else {
		/* run all the tests except excluded ones */
		for (test = selftests; test->name != NULL; test++) {
			if (exclude_mode) {
				char **excluded;
				for (excluded = argp; *excluded != NULL; ++excluded) {
					if (!strcmp(*excluded, test->name)) {
						break;
					}
				}
				if (*excluded) {
					if (v) {
						mbedtls_printf("skip: %s\n", test->name);
					}
					continue;
				}
			}
			if (test->function(v) != 0) {
				suites_failed++;
			}
			suites_tested++;
		}
	}

#else
	(void)exclude_mode;
	mbedtls_printf("mbedtls_self_test not defined.\n");
#endif

	if (v != 0) {
		mbedtls_printf("executed %d test suites\n\n", suites_tested);
		if (suites_failed > 0) {
			mbedtls_printf("[%d tests fail]\n\n", suites_failed);
		} else {
			mbedtls_printf("[all tests pass]\n\n");
		}
	}

	if (suites_failed > 0) {
		wifi_manager_deinit();
		wifi_signal_deinit();
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}

	wifi_res = wifi_manager_deinit();
	if (wifi_res != WIFI_MANAGER_SUCCESS) {
		printf("wifi_manager_deinit fail\n");
		wifi_signal_deinit();
		mbedtls_exit(MBEDTLS_EXIT_FAILURE);
	}
	wifi_signal_deinit();
	/* return() is here to prevent compiler warnings */
	return MBEDTLS_EXIT_SUCCESS;
}
