/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include "rreport_config.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/


/****************************************************************************
 * Public Data
 ****************************************************************************/
#define RR_USAGE								\
	"rreport [options]\n\n"						\
	RR_STR_OPTIONS

#define RR_STR_OPTIONS							\
	"options:\n"								\
	"\t-i interval(ms)\n"						\
	"\t-f filename\n"							\
	"\t-t target host address\n"				\
	"\t-s AP ssid\n"							\
	"\t-p AP passwod\n"							\
	"\t-l location name\n"						\
	"\t-c the number of rssi data to be sent\n"	\
	"\t-n sync time by NTP\n"					\
	"\t-h help\n\n"


#define OPT_OPTIONS "i:f:t:s:p:l:c:nh"
typedef struct {
	char *url;
} rr_app_msg;

/*
 * External Function
 */
extern char *rr_generate_message(int *data, int size);
extern int rr_send_result(char *str, char *url);
extern int rr_sync_time(void);
extern int rr_connect_ap(char *ssid, char *passwd);

/*
 * Private Function
 */

void rr_monitor_rssi(void)
{
	char *str_interval = NULL;
	int interval = RR_INTERVAL;
	(void)rr_get_interval(&str_interval);
	if (str_interval) {
		interval = atoi(str_interval);
	}

	char *str_count = NULL;
	int count = RR_NUM_OF_RSSI;
	(void)rr_get_counts(&str_count);
	if (str_count) {
		count = atoi(str_count); // it's mendatory
	}

	int *rssi = (int *)malloc(sizeof(int) * count);
	if (!rssi) {
		RR_ERR;
		return;
	}

	char *host = NULL;
	(void)rr_get_host(&host);

	while (1) {
		struct timeval start, end;
		gettimeofday(&start, NULL);
		for (int i = 0; i < count; i++) {
			rssi[i] = rr_get_rssi();
			usleep(interval * 1000);
		}
		gettimeofday(&end, NULL);
		uint32_t duration = (end.tv_sec - start.tv_sec)*1000 +
			(end.tv_usec - start.tv_usec)/1000;
		rr_set_duration(duration);

		char *msg = rr_generate_message(rssi, count);
		if (!msg) {
			RR_ERR;
			continue;
		}
		printf("[json] %s\n", msg);
		/*  if host is set then send to host */
		if (host) {
			rr_send_result(msg, host);
		}

		free(msg);
	}

	return;
}

static void rr_run(void)
{
	char *ssid = NULL;
	char *password = NULL;
	int set = 0;

	(void)rr_get_ssid(&ssid);
	if (ssid) {
		(void)rr_get_password(&password);
		if (rr_connect_ap(ssid, password) < 0) {
			printf("fail to connect AP\n");
			return;
		}
	}

	(void)rr_get_ntp_option(&set);
	if (set) {
		if (rr_sync_time() < 0) {
			printf("fail to sync time\n");
			return;
		}
	}

	rr_monitor_rssi();

	return;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int rreport_main(int argc, char *argv[])
#endif
{
	int opt;
	int run = 0;
	while ((opt = getopt(argc, argv, OPT_OPTIONS)) != -1) {
		run = 1;
		switch (opt) {
		case 'i':
			rr_set_interval(optarg);
			break;
		case 'f':
			rr_set_filename(optarg);
			break;
		case 't':
			rr_set_host(optarg);
			break;
		case 's':
			rr_set_ssid(optarg);
			break;
		case 'p':
			rr_set_password(optarg);
			break;
		case 'l':
			rr_set_location(optarg);
			break;
		case 'n':
			rr_set_ntp_option(1);
			break;
		case 'c':
			rr_set_counts(optarg);
			break;
		case 'h':
			run = 0;
			break;
		case ':':
			printf("option needs a value\n");
			run = 0;
			break;
		case '?':
			printf("unknown option: %c\n", optopt);
			run = 0;
			break;
		default:
			run = 0;
			break;
		}
	}

	if (!run) {
		printf("USAGE: %s\n", RR_USAGE);
		return -1;
	}

	rr_run();

	printf("report is finish\n");
	return 0;
}
