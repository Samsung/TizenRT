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
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sched.h>
#include <tinyara/sched.h>
#include <tinyara/error_report_internal.h>
#include <tinyara/fs/ioctl.h>
#include <semaphore.h>
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
#ifdef CONFIG_ERROR_REPORT_INFINITE_WAIT
	sem_t infinity_loop_exit;
	pthread_t infinity_thread;
	uint8_t ntasks;
#endif
	err_fsm_t fsm;
} g_error_info_t;

static g_error_info_t g_err_info;
static int g_err_report_fd;

#ifdef CONFIG_ERROR_REPORT_INFINITE_WAIT
static void prv_create_infwait_err_rec(int pid, int state, unsigned long task_addr)
{
	infinty_waitdata_t param;
	int ret;
	param.pid = pid;
	ret = ioctl(g_err_report_fd, ERIOC_GET_BACKTRACE, &param);
	if (ret < 0) {
		nwerrdbg("ioctl failed\n");
		return;
	}
	error_infwait_data_t send_data;
	struct timeval ts;
	char sendbuf[ERR_BUFLEN];
	int i;
	gettimeofday(&ts, NULL);
	send_data.error_type = ERRTYPE_HANGING;
	send_data.ncalls = param.ncalls;
	send_data.task_state = param.task_state;
	send_data.task_addr = param.entry;
	for (i = 0; i < CONFIG_ERROR_REPORT_BACKTRACE_MAX_DEPTH; i++) {
		send_data.backtrace[i] = param.backtrace[i];
	}
	send_data.timestamp.tv_sec = ts.tv_sec;
	send_data.timestamp.tv_usec = ts.tv_usec;
	if (sizeof(sendbuf) < sizeof(error_infwait_data_t)) {
		nwerrdbg("Buffer space inadequate\n");
		return;
	}
	memcpy(sendbuf, (void *)&send_data, sizeof(error_infwait_data_t));
	sendbuf[sizeof(error_infwait_data_t)] = 0;
	/* Send error record to default endpoint */
	error_report_send(0, sendbuf, sizeof(error_infwait_data_t) + 1);
}
#endif


static int prv_check_infinite_wait(void *args)
{
	struct timespec ts;
	struct timeval t1;
	int ret;

	while (1) {
		ret = ioctl(g_err_report_fd, ERIOC_CHECK_INFWAIT, NULL);
		if (ret < 0) {
			nwerr_vdbg("ioctl failed\n");
			goto done;
		}
		gettimeofday(&t1, NULL);
		/* Logic to check every thread state whether it is in WAITSEM or WAITSIG mode */
		nwerr_vdbg("%lu: sem_timedwait for %lu seconds\n", t1.tv_sec, ts.tv_sec);
		ts.tv_sec = t1.tv_sec + CONFIG_ERROR_REPORT_INFINITE_CHECK_TIMER;
		ts.tv_nsec = t1.tv_usec * 1000;
		ret = sem_timedwait(&g_err_info.infinity_loop_exit, &ts);
		gettimeofday(&t1, NULL);
		nwerr_vdbg("%lu: sem_timedwait returned with value %d\n", t1.tv_sec, ret);
		if (!ret) {
			break;
		} else if ((ret < 0) && (get_errno() != ETIMEDOUT)) {
			printf("exiting with errno %d", get_errno());
			goto done;
		}
	}
	return 0;
done:
	sem_destroy(&g_err_info.infinity_loop_exit);
	return ret;
}

err_status_t error_report_init(void)
{
	if (g_err_info.fsm == ERRSTATE_INITIALIZED) {
		return ERR_SUCCESS;
	}
	pthread_mutex_init(&g_err_info.err_mutex, NULL);
	g_err_info.fsm = ERRSTATE_INITIALIZED;
	g_err_report_fd = open(ERROR_REPORT_DRVPATH, O_RDWR);
	if (g_err_report_fd < 0) {
		nwerrdbg("Failed to open error report driver file: %d\n", get_errno());
		goto err_case;
	}
#ifdef CONFIG_ERROR_REPORT_INFINITE_WAIT
	int retval;
	sem_init(&g_err_info.infinity_loop_exit, 0, 0);
	retval = ioctl(g_err_report_fd, ERIOC_SET_CALLBACK, prv_create_infwait_err_rec);
	if (retval < 0) {
		nwerr_vdbg("Ioctl failed\n");
		sem_destroy(&g_err_info.infinity_loop_exit);
		goto err_case;
	}
#endif
	return ERR_SUCCESS;
err_case:
	pthread_mutex_destroy(&g_err_info.err_mutex);
	return ERR_FAIL;
}

err_status_t error_report_start_infinitywait(void)
{
	int r;
	pthread_attr_t attr;
	struct sched_param sparam;
	sparam.sched_priority = 100;
	if ((r = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		nwerrdbg("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
		return ERR_FAIL;
	}

	if ((r = pthread_attr_setschedpolicy(&attr, SCHED_RR)) != 0) {
		nwerrdbg("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
		return ERR_FAIL;
	}

	if ((r = pthread_attr_setstacksize(&attr, 4096)) != 0) {
		nwerrdbg("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
		return ERR_FAIL;
	}
	if ((r = pthread_create(&g_err_info.infinity_thread, &attr, (pthread_startroutine_t)prv_check_infinite_wait, NULL)) != 0) {
		nwerrdbg("%s: pthread_create failed, status=%d\n", __func__, r);
		return ERR_FAIL;
	}
	nwerr_vdbg("Created infinity thread\n");
	return ERR_SUCCESS;
}

err_status_t error_report_deinit(void)
{
#ifdef CONFIG_ERROR_REPORT_INFINITE_WAIT
	sem_post(&g_err_info.infinity_loop_exit);
	pthread_join(g_err_info.infinity_thread, NULL);
#endif
	pthread_mutex_lock(&g_err_info.err_mutex);
	g_err_info.front = 0;
	g_err_info.rear = 0;
	g_err_info.q_pending = 0;
	g_err_info.fsm = ERRSTATE_UNINITIALIZED;
	pthread_mutex_unlock(&g_err_info.err_mutex);
	pthread_mutex_destroy(&g_err_info.err_mutex);
	return ERR_SUCCESS;
}

error_data_t *error_report_data_create(int err_type, int module_id, int error_code, uint32_t pc_value, uint32_t task_addr)
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	error_data_t *ret = NULL;

	pthread_mutex_lock(&g_err_info.err_mutex);

	ret = (error_data_t *) g_error_report + g_err_info.rear;
	ret->error_type = ERRTYPE_SERVICE;
	ret->timestamp.tv_sec = ts.tv_sec;
	ret->timestamp.tv_usec = ts.tv_usec;
	ret->module_id = module_id;
	ret->error_code = error_code;
	ret->pc_value = pc_value;
	nwerr_vdbg("pc_value: %08x\n", pc_value);
	if (module_id == ERRMOD_INFINITE_WAIT) {
		ret->task_addr = task_addr;
	} else {
		int retval;
		thread_entry_t thd;
		thd.pid = getpid();
		retval = ioctl(g_err_report_fd, ERIOC_GET_ENTRY, &thd);
		if (retval < 0) {
			nwerr_vdbg("Ioctl failed\n");
			ret = NULL;
			goto done;
		}
		ret->task_addr = thd.entry;
	}
	nwerr_vdbg("task_addr: %08x\n", ret->task_addr);
	if (g_err_info.fsm == ERRSTATE_MEMUNDERFLOW) {
		g_err_info.fsm = ERRSTATE_INITIALIZED;
	}
	g_err_info.rear = (g_err_info.rear + 1) % CONFIG_ERROR_REPORT_NENTRIES;
	if (g_err_info.q_pending < CONFIG_ERROR_REPORT_NENTRIES) {
		g_err_info.q_pending++;
	}
done:
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
			nwerr_vdbg("No entries\n");
			g_err_info.fsm = ERRSTATE_MEMUNDERFLOW;
		} else {
			while (nentries) {
				if (nbytes + sizeof(error_data_t) >= ERR_BUFLEN) {
					g_err_info.fsm = ERRSTATE_BUFFER_EXCEEDED;
					nwerr_vdbg("Buffer exceeded\n");
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
	printf("Report (%d bytes) in Hex: ", nbytes);
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
	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
