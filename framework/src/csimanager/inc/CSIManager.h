/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#pragma once

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "mqueue.h"
#include <pthread.h>
#include "csifw_log.h"
#include <semaphore.h>
#include "csimanager/csifw_api.h"

#define ERROR -1  // for ERROR
#define CSIFW_MAX_NUM_APPS 3 // Max number of Services
#define CSIFW_CSI_HEADER_LEN 43 // CSI packet header size
#define CSIFW_MIN_INTERVAL_MS 30  // Minimum supported CSI interval in milliseconds
#define CSIFW_MAX_RAW_BUFF_LEN 1024 // CSI data buffer xax size

typedef unsigned long long u64;  // for u64
typedef void (*CSIDataListener)(CSIFW_RES res, int csi_buff_len, unsigned char *csi_buff, int csi_data_len);

typedef enum _CSI_FRAMEWORK_STATE {
  CSI_FRAMEWORK_STATE_UNINITIALIZED = -1,                   /* CSI_FRAMEWORK_STATE NOT INITIALIZED  */
  CSI_FRAMEWORK_STATE_STARTED = 0,                          /* CSI_FRAMEWORK_STATE STARTED  */
  CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW = 1,           /* CSI_FRAMEWORK_STATE STARTED But Waiting for Network  */
  CSI_FRAMEWORK_STATE_STOPPED = 2,                          /* CSI_FRAMEWORK_STATE STOPPED  */
  CSI_FRAMEWORK_STATE_INITIALIZED = 3,                      /* CSI_FRAMEWORK_STATE INITIALIZED  */
  CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT = 4, /* CSI_FRAMEWORK_STATE STARTED But Waiting for Network Reconnect */
} CSI_FRAMEWORK_STATE;

typedef enum _SERVICE_STATE {
  CSI_SERVICE_REGISTERED = 1, /* SERVICE REGISTERED */
  CSI_SERVICE_START = 2,      /* SERVICE COLLECTING DATA */
  CSI_SERVICE_STOP = 3        /* SERVICE NOT COLLECTING DATA */
} SERVICE_STATE;

typedef struct CSIFW_SERVICE_INFO_T {
  client_parsed_data_listener parsed_data_cb;
  client_raw_data_listener raw_data_cb;
  SERVICE_STATE svc_state;
  void *service_data;
  int svc_id;
} csifw_service_info_t;

// CSI Framework Context Structure
typedef struct {
  csifw_service_info_t csi_services[CSIFW_MAX_NUM_APPS]; /* Service information */
  CSIDataListener CSI_DataCallback;                      /* CSI Data Callback */
  CSI_FRAMEWORK_STATE csifw_state;                       /* CSIFW Collection State */
  csi_config_type_t csi_config;                          /* CSI configuration */

  unsigned char *get_data_buffptr; /* Buffer to get data from driver */
  unsigned int task_run_success;   /* Track if csifw_task initialization */
  unsigned int task_run_state;     /* Task Stopped Status */
  unsigned int ping_Interval;      /* Ping Inetrval */
  unsigned int csi_interval;       /* Data collection interval (ms) */
  unsigned int ping_count;         /* Ping Count */

  uint8_t parsed_data_cb_started_count; /* Started Parsed data count */
  bool data_receiver_thread_stop;       /* Data Receiver Thread Stop Status */
  uint16_t ParsedDataBufferLen;         /* CSI Parsed data buffer length */
  uint8_t parsed_data_cb_count;         /* Parsed data count */
  bool ping_thread_stop;                /* Ping Thread Stop Status */
  float *parsed_buffptr;                /* CSI Parsed data buffer pointer */
  uint8_t service_count;                /* Registered Service count */
  bool disable_required;                /* CSI Config Disable Required */
  int data_receiver_fd;                 /* Value from OPEN Driver */
  int task_handle;                      /* Task Manager handle */
  int ping_socket;                      /* Ping Socket */

  pthread_mutex_t data_reciever_mutex; /* CSI Data Reciever Mutex */
  pthread_t csi_data_receiver_th;      /* CSI Data Receiver Thread Status */
  struct icmp_echo_hdr *p_iecho;       /* Echo Header */
  struct sockaddr *socketAddr;         /* Sokcet Address */
  pthread_t csi_ping_thread;           /* CSI Ping Thread */
  struct addrinfo *addr_info;          /* Address Information */
  sem_t csifw_task_sema;               /* CSIFW Task Sema */
  mqd_t mq_handle;                     /* MQ Hanlde */

} csifw_context_t;

int start_csi_framework(csifw_context_t *p_con_ctx);
int stop_csi_framework(csifw_context_t *p_con_ctx);
csifw_context_t *get_csifw_context(void);

#define CSIFW_MUTEX_LOCK(mutex)                   \
  do {                                            \
    if (pthread_mutex_lock(mutex) != 0) {         \
      CSIFW_LOGE("Mutex lock failed! %d", errno); \
    }                                             \
  } while (0)

#define CSIFW_MUTEX_UNLOCK(mutex)                   \
  do {                                              \
    if (pthread_mutex_unlock(mutex) != 0) {	    \
      CSIFW_LOGE("Mutex unlock failed! %d", errno); \
    }                                               \
  } while(0)
  
