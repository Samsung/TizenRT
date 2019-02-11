/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <error_report/error_report.h>
#include <wifi_manager/wifi_manager.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ERR_REPORT_USAGE                            \
	"\n usage: error_report [options]\n"            \
	"\n Send error report module:\n"                \
	"    error_report send [endpoint_ipaddr:endpoint_port]\n"\
	"\n Verify error_report_records_main\n"         \
	"	 error_report test [endpoint_ipaddr:endpoint_port]"

#define RT_MSG(msg) printf("%s: %s\n", __BASE_FILE__, msg)
sem_t g_err_sem;
uint8_t g_err_nscenarios;
uint8_t g_err_nsuccess;

#define ERR_REPORT_LOG_START                        \
	do {                                                               \
		printf("-->%s\n", __FUNCTION__);                               \
	} while (0)

#define ERR_REPORT_LOG_END                          \
	do {                                                               \
		printf("<--%s\n", __FUNCTION__);                               \
	} while (0)

#define ERR_REPORT_TC_START                         \
	do {                                                               \
		g_err_nscenarios++;                         \
	} while (0)

#define ERR_REPORT_TC_END_CHECK(exp)                \
	do {                                                               \
		if (exp) {                                                     \
			g_err_nsuccess++;                       \
			printf("ERR_REPORT_TC_END_CHECK [%s]: PASS\n", __FUNCTION__);\
		} else {                                                       \
			printf("ERR_REPORT_TC_END_CHECK [%s]: FAIL\n", __FUNCTION__);\
		}                                                              \
	} while (0)
#define ERR_REPORT_PRINT_STATS                      \
	do {                                                               \
		printf("%d out of %d cases passed.\n", g_err_nsuccess, g_err_nscenarios);\
	} while (0)

#define ERR_REPORT_TEST_SIGNAL                      \
	do {                                                               \
		printf(" [RT] T%d sem_post\n", getpid());                      \
		sem_post(&g_err_sem);                       \
	} while (0)

#define ERR_REPORT_TEST_WAIT                        \
	do {                                                               \
		printf(" [RT] T%d sem_wait\n", getpid());                      \
		sem_wait(&g_err_sem);                       \
	} while (0)

/*
 * WiFi Manager callback prototypes
 */
static void prv_sta_connected(wifi_manager_result_e res);
static void prv_sta_disconnected(wifi_manager_disconnect_e res);
static void prv_softap_sta_join(void);
static void prv_softap_sta_leave(void);
static void prv_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

/*
 * WiFi Manager callback definitions
 */
static void prv_sta_connected(wifi_manager_result_e res)
{
	printf(" [RT] T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	ERR_REPORT_TEST_SIGNAL;
}

static void prv_sta_disconnected(wifi_manager_disconnect_e res)
{
	sleep(2);
	printf(" [RT] T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	ERR_REPORT_TEST_SIGNAL;
}

static void prv_softap_sta_join(void)
{
	printf(" [RT] T%d --> %s\n", getpid(), __FUNCTION__);
	ERR_REPORT_TEST_SIGNAL;
}

static void prv_softap_sta_leave(void)
{
	printf(" [RT] T%d --> %s\n", getpid(), __FUNCTION__);
	ERR_REPORT_TEST_SIGNAL;
}

static void prv_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	printf(" [RT] T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	ERR_REPORT_TEST_SIGNAL;
}

static wifi_manager_cb_s wifi_callbacks = {
	prv_sta_connected,
	prv_sta_disconnected,
	prv_softap_sta_join,
	prv_softap_sta_leave,
	prv_scan_done,
};
/*
 * Error Reporting Functions
 */
static void error_report_records(const char *endpoint)
{
	ERR_REPORT_LOG_START;
	char readbuf[1024];
	int nbytes;

	nbytes = error_report_data_read(readbuf);
	if (nbytes <= 0) {
		printf("No data to send\n");
		return;
	}

	nbytes = error_report_send(endpoint, readbuf, nbytes);
	if (nbytes < 0) {
		printf("Failed to send data\n");
	}
	ERR_REPORT_LOG_END;
}

static void error_report_single(const char *endpoint)
{
	ERR_REPORT_TC_START;

	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	wifi_manager_ap_config_s bad_apconfig = { "Gorani", 6, "jonbeo2#", 8, WIFI_MANAGER_AUTH_WPA2_PSK, WIFI_MANAGER_CRYPTO_AES };
	wifi_manager_ap_config_s good_apconfig = { "Gorani", 6, "jonbeo1@", 8, WIFI_MANAGER_AUTH_WPA2_PSK, WIFI_MANAGER_CRYPTO_AES };
	char readbuf[1024];
	int nbytes_read = 0;
	int nbytes_sent = 0;

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" wifi_manager_init fail\n");
		goto done;
	}
	res = wifi_manager_connect_ap(&bad_apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" AP connect failed\n");
		goto done;
	}
	/* Wait for DHCP connection */
	ERR_REPORT_TEST_WAIT;
	res = wifi_manager_connect_ap(&good_apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" AP connect failed\n");
		goto done;
	}
	/* Wait for DHCP connection */
	ERR_REPORT_TEST_WAIT;
done:
	/* Send error report for the bad AP connection */
	nbytes_read = error_report_data_read(readbuf);
	if (nbytes_read > 0) {
		nbytes_sent = error_report_send(endpoint, readbuf, nbytes_read);
	}
	wifi_manager_disconnect_ap();
	ERR_REPORT_TEST_WAIT;
	wifi_manager_deinit();
	ERR_REPORT_TC_END_CHECK(nbytes_sent > 0);
}

static void error_report_multiple(const char *endpoint)
{
	ERR_REPORT_TC_START;

	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	wifi_manager_ap_config_s apconfig = { "Gorani", 6, "jonbeo1@", 8, WIFI_MANAGER_AUTH_WPA2_PSK, WIFI_MANAGER_CRYPTO_AES };
	char readbuf[1024];
	int nbytes_read;
	int nbytes_sent = 0;
	int i;
	int sock_cnt = 0;
	int sock_fds[CONFIG_NSOCKET_DESCRIPTORS];

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" wifi_manager_init fail\n");
		goto done;
	}
	/* Create dummy sockets, the idea here is to
	 * prevent dhcp from successfully opening a socket,
	 * and then capture the resulting error.
	 */
	for (i = 0; i < CONFIG_NSOCKET_DESCRIPTORS; i++) {
		sock_fds[i] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock_fds[i] < 0) {
			/* We are done here, time to exit */
			break;
		} else {
			sock_cnt++;
		}
	}
	res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" AP connect failed\n");
		goto done;
	}
	/* Wait for DHCP connection */
	ERR_REPORT_TEST_WAIT;
	/* Wait for Disconnected event, we must sleep here,
	 * because WiFi Manager prevents callbacks to application
	 * when disconnecting.
	 */
	sleep(10);

	/* close dummy sockets created above */
	for (i = 0; i < sock_cnt; i++) {
		close(sock_fds[i]);
	}

	res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" AP connect failed\n");
		goto done;
	}
	/* Wait for DHCP connection */
	ERR_REPORT_TEST_WAIT;
done:
	/* Send error report for the bad AP connection */
	nbytes_read = error_report_data_read(readbuf);
	if (nbytes_read > 0) {
		nbytes_sent = error_report_send(endpoint, readbuf, nbytes_read);
	}
	wifi_manager_disconnect_ap();
	ERR_REPORT_TEST_WAIT;
	wifi_manager_deinit();
	ERR_REPORT_TC_END_CHECK(nbytes_sent > 0);
}

static void error_report_queue_underflow(void)
{
	ERR_REPORT_TC_START;

	int nbytes_read;
	char readbuf[1024];
	nbytes_read = error_report_data_read(readbuf);

	ERR_REPORT_TC_END_CHECK(nbytes_read == 0);
}

static void error_report_test(const char *endpoint)
{
	ERR_REPORT_LOG_START;
	/* Verify unsuccessful data read */
	error_report_queue_underflow();

	/* Verify single error case in WiFi Manager */
	error_report_single(endpoint);

	/* Verify multiple errors across WiFi Manager */
	error_report_multiple(endpoint);

	ERR_REPORT_PRINT_STATS;
	ERR_REPORT_LOG_END;
}

void error_report_process(int argc, char *argv[])
{
	if (argc < 3) {
		printf("%s", ERR_REPORT_USAGE);
		return;
	}
	if (strcmp(argv[2], "send") == 0) {
		if (argc < 4) {
			error_report_records(NULL);
		} else {
			error_report_records(argv[3]);
		}
	} else if (strcmp(argv[2], "test") == 0) {
		if (argc < 4) {
			error_report_test(NULL);
		} else {
			error_report_test(argv[3]);
		}
	} else {
		printf("%s", ERR_REPORT_USAGE);
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int error_report_main(int argc, char *argv[])
#endif
{
	printf("Network Error Report test!!\n");
	sem_init(&g_err_sem, 0, 0);
	g_err_nscenarios = 0;
	g_err_nsuccess = 0;

	task_create("Network Error Reporting", 100, 1024 * 10, (main_t) error_report_process, argv);

	return 0;
}
