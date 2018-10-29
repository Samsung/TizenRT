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
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sched.h>
#include <tinyara/sched.h>
#include <error_report/error_report.h>

#define ERR_IP_ADRR_LEN 16

error_data_t g_error_report[CONFIG_ERROR_REPORT_NENTRIES];
typedef enum {
	ERRSTATE_UNINITIALIZED = -1,
	ERRSTATE_INITIALIZED,
	ERRSTATE_MEMUNDERFLOW,
	ERRSTATE_BUFFER_EXCEEDED,
} err_fsm_t;

typedef struct {
	uint16_t rear;
	uint16_t front;
	int8_t q_pending;
	pthread_mutex_t err_mutex;
	err_fsm_t fsm;
} g_error_info_t;

static g_error_info_t g_err_info;

static unsigned long prv_fetch_taskaddr(int pid)
{
	struct tcb_s *tcbptr = sched_gettcb(pid);
	if (tcbptr != NULL) {
		entry_t e = tcbptr->entry;
		if ((tcbptr->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD) {
			return (unsigned long)e.pthread;
		} else {
			return (unsigned long)e.main;
		}
	}
	return 0;
}

err_status_t error_report_init(void)
{
	pthread_mutex_init(&g_err_info.err_mutex, NULL);
	g_err_info.fsm = ERRSTATE_INITIALIZED;
	return ERR_SUCCESS;
}

err_status_t error_report_deinit(void)
{
	pthread_mutex_destroy(&g_err_info.err_mutex);

	return ERR_SUCCESS;
}

error_data_t *error_report_data_create(int module_id, int error_code, uint32_t pc_value)
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	error_data_t *ret = NULL;

	pthread_mutex_lock(&g_err_info.err_mutex);

	ret = (error_data_t *) g_error_report + g_err_info.rear;
	ret->timestamp.tv_sec = ts.tv_sec;
	ret->timestamp.tv_usec = ts.tv_usec;
	ret->module_id = module_id;
	ret->error_code = error_code;
	ret->pc_value = pc_value;
	nwerr_vdbg("pc_value: %08x\n", pc_value);
	ret->task_addr = prv_fetch_taskaddr(getpid());
	nwerr_vdbg("task_addr: %08x\n", ret->task_addr);
	if (g_err_info.fsm == ERRSTATE_MEMUNDERFLOW) {
		g_err_info.fsm = ERRSTATE_INITIALIZED;
	}
	g_err_info.rear = (g_err_info.rear + 1) % CONFIG_ERROR_REPORT_NENTRIES;
	if (g_err_info.q_pending < CONFIG_ERROR_REPORT_NENTRIES) {
		g_err_info.q_pending++;
	}
	pthread_mutex_unlock(&g_err_info.err_mutex);
	return ret;
}

int error_report_data_read(char *readbuf)
{
	int nbytes = 0;
	int i;
	int nentries = g_err_info.q_pending;
	pthread_mutex_lock(&g_err_info.err_mutex);
	if (g_err_info.q_pending < CONFIG_ERROR_REPORT_NENTRIES) {
		g_err_info.front = 0;
	} else {
		g_err_info.front = g_err_info.rear;
	}
	if (readbuf != NULL) {
		if (!nentries) {
			g_err_info.fsm = ERRSTATE_MEMUNDERFLOW;
		} else {
			while (nentries) {
				if (nbytes + sizeof(error_data_t) >= ERR_BUFLEN) {
					g_err_info.fsm = ERRSTATE_BUFFER_EXCEEDED;
					goto err_read_done;
				}
				memcpy(readbuf + nbytes, (char *)(g_error_report + g_err_info.front), sizeof(error_data_t));
				nbytes += sizeof(error_data_t);
				g_err_info.front = (g_err_info.front + 1) % CONFIG_ERROR_REPORT_NENTRIES;
				nentries--;
			}
			*(readbuf + nbytes) = '\0';
		}
	}

err_read_done:
	printf("Report in Hex: ");
	for (i = 0; i < nbytes; i++) {
		printf("%02x ", readbuf[i]);
	}
	printf("\n");

	pthread_mutex_unlock(&g_err_info.err_mutex);
	return nbytes;
}

int error_report_send(const char *ep, char *readbuf, int readbuf_len)
{
	int nbytes;
	int socket_fd;
	struct sockaddr_in endpoint;
	char ip_addr[ERR_IP_ADRR_LEN];
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < CONFIG_NSOCKET_DESCRIPTORS) {
		return ERR_FAIL;
	}
	memset((char *)&endpoint, 0x00, sizeof(endpoint));
	endpoint.sin_family = AF_INET;
	if (!ep) {
		endpoint.sin_port = htons(atoi(CONFIG_EPORT_ERROR_REPORT));
		strncpy(ip_addr, CONFIG_EIPADDR_ERROR_REPORT, strlen(CONFIG_EIPADDR_ERROR_REPORT));
		ip_addr[strlen(CONFIG_EIPADDR_ERROR_REPORT)] = 0;
	} else {
		int ipaddr_len;
		char *pos = strchr(ep, ':');
		if (pos == NULL) {
			nwerrdbg("endpoint parsing issue\n");
			return ERR_FAIL;
		}
		ipaddr_len = (int)(pos - ep);
		strncpy(ip_addr, ep, ipaddr_len);
		ip_addr[ipaddr_len] = '\0';
		if (strlen(ep) <= ipaddr_len) {
			nwerrdbg("Port not specified\n");
			return ERR_FAIL;
		} else {
			endpoint.sin_port = htons(atoi((pos + 1)));
		}
	}
	nwerr_vdbg("ipaddr = %s, len = %d, port = %d\n", ip_addr, strlen(ip_addr), endpoint.sin_port);
	if (inet_pton(AF_INET, ip_addr, &(endpoint.sin_addr)) == -1) {
		nwerrdbg("inet_pton() failed\n");
		return ERR_FAIL;
	}

	if (connect(socket_fd, (struct sockaddr *)&endpoint, sizeof(struct sockaddr_in)) < 0) {
		nwerrdbg("connect() failed: %d\n", errno);
		return ERR_FAIL;
	}

	nbytes = sendto(socket_fd, readbuf, readbuf_len, 0, (struct sockaddr *)&endpoint, sizeof(struct sockaddr_in));
	if (nbytes > 0) {
		nwerr_vdbg("%d bytes sent\n", nbytes);
		nwerr_vdbg("Data: %s\n", readbuf);
	}
	close(socket_fd);
	return nbytes;
}
