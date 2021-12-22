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
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include <netdb.h>
#include "connect_test.h"
#include "connect_test_log.h"

#define TAG "[CT_HOSTNAMES]"
#define HOSTNAME_BUFSIZE 128

static int _wm_store_file(char *buf, unsigned int buf_size, char *file_name)
{
	FILE *fp;
	fp = fopen(file_name, "a+");
	if (!fp) {
		CT_LOGE(TAG, "file open error(%d)", errno);
		return -1;
	}

	int ret = fputs(buf, fp);
	if (ret < 0) {
		CT_LOGE(TAG, "file write error(%d)", errno);
		fclose(fp);
		return -1;
	}

	fclose(fp);
	return 0;
}

static int _wm_save_hostname(struct ct_options *hostname_info)
{
	char buf[HOSTNAME_BUFSIZE];
	int ret = -1;
	char *file_name = NULL;
	if (hostname_info == NULL) {
		CT_LOGE(TAG, "AP Info is NULL");
		return -1;
	}

	snprintf(buf, HOSTNAME_BUFSIZE, "%s\n", hostname_info->hostname);
	file_name = hostname_info->path;

	ret = _wm_store_file(buf, HOSTNAME_BUFSIZE, file_name);
	if (ret < 0) {
		CT_LOGE(TAG, "store file failed");
		return -2;
	}
	return 0;
}

void connect_test_add_hostname(void *arg)
{
	struct ct_options *hostname_info = (struct ct_options *)arg;
	int ret = -1;
	CT_LOG(TAG, "Hostname[%s]", hostname_info->hostname);
	ret = _wm_save_hostname(hostname_info);
	if (ret < 0) {
		CT_LOGE(TAG, "Save AP configuration failed");
		return;
	}
}

static void hostname_to_ip4(char *hostname)
{
        if (hostname == NULL) {
                CT_LOGE(TAG, "Hostname not present");
		return;
        }

        struct hostent *shost;
        shost = (struct hostent*)gethostbyname((const char *)hostname);
        if (shost == NULL) {
                CT_LOGE(TAG, "gethostbyname fail %d", shost);
                return;
        }
        CT_LOG(TAG, "Hostname: %s IPAddress: %s", hostname,
		inet_ntoa(*((struct in_addr *)shost->h_addr_list[0])));
}

static void default_dns_service(void) {
	CT_LOG(TAG, "Running default DNS service");
	char hostname[30]; 
	char hostnames[3][20] = {"www.google.com", "www.samsung.com", "www.facebook.com"};
	for (int hname = 0; hname < 3; hname++) {
		strcpy(hostname, hostnames[hname]);
		hostname_to_ip4(hostname);
	}
}

void dns_service(char *hostname_file) {
	if (!hostname_file) {
		default_dns_service();
	} else {
		char *file_name = hostname_file;
		FILE* file = fopen(file_name, "r"); 
		if(!file) {
			CT_LOG(TAG, "Unable to open : %s", file_name);
			default_dns_service();
		} else {
			char hostname[30]; 
			while (fgets(hostname, sizeof(hostname), file)) {
				hostname[strlen(hostname) - 1] = '\0';
				hostname_to_ip4(hostname); 
			}
			fclose(file);
		}
	}
	return 0;
}
