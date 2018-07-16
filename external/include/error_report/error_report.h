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
#ifndef ERROR_REPORT_H
#define ERROR_REPORT_H

#include <tinyara/config.h>
#include <time.h>
#include <stdint.h>

/**
 * @defgroup error_report error_report
 * @ingroup error_report
 * @brief Provides APIs for Error Reporting Modules
 * @{
 */
#define ERR_FUNCNAME_LEN_MAX	32
#define ERR_TASKNAME_LEN_MAX CONFIG_TASK_NAME_SIZE + 1
#define ERR_TIMESTAMP_LEN	26
#define ERR_BUFLEN	1024
enum error_module_id_e {
	ERRMOD_WIFI_MANAGER,
	ERRMOD_WPA_SUPPLICANT,
	ERRMOD_LWIP_CORE,
	ERRMOD_LWIP_SOCKET,
};

#ifdef CONFIG_WIFIMGR_ERROR_REPORT
enum error_code_wifi_manager_e {
	ERR_WIFIMGR_CONNECT_FAIL = -100,
	ERR_WIFIMGR_CONNECT_DHCPC_FAIL,
	ERR_WIFIMGR_DISCONNECT_FAIL,
	ERR_WIFIMGR_INIT_FAIL,
	ERR_WIFIMGR_DEINIT_FAIL,
	ERR_WIFIMGR_INVALID_STATE,
	ERR_WIFIMGR_INVALID_EVENT,
	ERR_WIFIMGR_SOFTAP_FAIL,
	ERR_WIFIMGR_SOFTAP_DHCPD_FAIL,
	ERR_WIFIMGR_STA_FAIL,
	ERR_WIFIMGR_SCAN_FAIL,
	ERR_WIFIMGR_INVALID_ARGUMENTS,
	ERR_WIFIMGR_UTILS_FAIL,
	ERR_WIFIMGR_INTERNAL_FAIL,
	ERR_WIFIMGR_API_FAIL,
	ERR_WIFIMGR_MSGQ_FAIL,
};
typedef enum error_code_wifi_manager_e error_code_wifi_manager_t;
#endif

typedef struct {
	struct timeval timestamp;
	uint32_t pc_value;
	uint32_t task_addr;
	int16_t error_code;
	uint16_t module_id;
} error_data_t;

typedef enum {
	ERR_FAIL = -1,
	ERR_SUCCESS,
} err_status_t;

/**
 * @file error_report/error_report.h
 * @brief Provides APIs for Error Reporting Module
 */
err_status_t error_report_init(void);
err_status_t error_report_deinit(void);
error_data_t *error_report_data_create(int module_id, int error_code, uint32_t pc_value);
int error_report_data_read(char *readbuf);
int error_report_send(const char *ep, char *readbuf, int readbuf_len);

#define ERR_DATA_CREATE(module_id, reason_code)	    \
do {                                                                   \
	uint32_t pc_value;                                                 \
	__asm volatile ("mov %[result], r15":[result] "=r" (pc_value));    \
	if (error_report_data_create(module_id, reason_code, pc_value) == NULL) {\
		printf("Failed to create error log\n");                        \
	}                                                                  \
} while(0)

#endif
/**
 *@}
 */
